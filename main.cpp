#include "elements/calendar.hpp"
#include "elements/editingModal.hpp"
#include "elements/table.hpp"
#include "gui.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "output.hpp"
#include "platform.hpp"
#include "store/mainStore.hpp"

#include <SDL.h>
#include <SDL_opengl.h>
#include <_time.h>
#include <chrono>
#include <memory>
#include <string>
#include <vector>

// NOTE TO DEVELOPERS: This file is a test file for the GUI module of the Steppable library.
//                     When building the GUI part for the first time, this can come in handy.
//                     Run the executable to see if the GUI window works.

using namespace std::literals;
using namespace adaptiday;
using namespace adaptiday::__internals;

void setUpContents(const std::shared_ptr<MainStore>& store)
{
    auto size = ImGui::GetIO().DisplaySize;

    ImGui::SetNextWindowSize({ size.x * 35 / 100, size.y * 20 / 100 });
    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::BeginDisabled(store->isEditing);

    // Clock
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = utils::localtime_xp(&time);

    auto todayWeek =
        std::chrono::year_month_weekday(std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now()));
    auto todayDate =
        std::chrono::year_month_day(std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now()));
    auto todayIdx = todayWeek.weekday().c_encoding();

    auto timeStr = std::to_string(tm.tm_hour) + ":" + std::to_string(tm.tm_min) + ":" + std::to_string(tm.tm_sec);
    auto dateStr =
        std::to_string(tm.tm_mon + 1) + "/" + std::to_string(tm.tm_mday) + "/" + std::to_string(tm.tm_year + 1900);
    ImGui::Begin("Clock", nullptr, store->defaultFlags);
    ImGui::Text("Welcome to AdaptiDay!");
    ImGui::Text("Time: %s", timeStr.c_str());
    ImGui::Text("Date: %s", dateStr.c_str());
    ImGui::End();

    // Plans
    ImGui::SetNextWindowSize({ size.x * 35 / 100, size.y * 80 / 100 });
    ImGui::SetNextWindowPos({ 0, size.y * 20 / 100 });
    ImGui::Begin("Plans", nullptr, store->defaultFlags);

    if (store->isFirstLoop)
    {
        store->items = { "School", "Do something", "Eat" };
        store->isFirstLoop = false;
    }

    if (ImGui::InputTextWithHint(" ",
                                 "Quick Add",
                                 store->quickAddBuf.data(),
                                 store->quickAddBuf.size(),
                                 ImGuiInputTextFlags_EnterReturnsTrue))
    {
        output::info("main"s, "Added {0}"s, { std::string(store->quickAddBuf.data()) });
        store->items.emplace_back(store->quickAddBuf.data());
        store->quickAddBuf.fill(0);
    }

    listTable("_elements", store->items, 2, 0, [&](const std::vector<std::string>& elements, const size_t pos) {
        store->isEditing = true;
        store->isEditingFirstLoop = true;
        store->editingIdx = pos;
    });
    ImGui::End();

    // Calendar
    ImGui::SetNextWindowSize({ size.x * 65 / 100, size.y });
    ImGui::SetNextWindowPos({ size.x * 35 / 100, 0 });
    calendar(store);

    ImGui::EndDisabled();

    // Editing Modal
    if (store->isEditing)
        runEditingModal(store);
}

int main(int argc, char** argv)
{
    const std::shared_ptr<MainStore>& store = std::make_shared<MainStore>();
    runWindow("Hello", [&]() { setUpContents(store); });

    return 0;
}
