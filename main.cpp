#include "elements/editingModal.hpp"
#include "elements/table.hpp"
#include "gui.hpp"
#include "imgui.h"
#include "output.hpp"
#include "store/mainStore.hpp"

#include <SDL.h>
#include <SDL_opengl.h>
#include <memory>
#include <string>

// NOTE TO DEVELOPERS: This file is a test file for the GUI module of the Steppable library.
//                     When building the GUI part for the first time, this can come in handy.
//                     Run the executable to see if the GUI window works.

using namespace std::literals;
using namespace adaptiday;
using namespace adaptiday::__internals;

void setUpContents(const std::shared_ptr<MainStore>& store)
{
    auto size = ImGui::GetIO().DisplaySize;
    ImGui::SetNextWindowSize({ size.x * 45 / 100, size.y });
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Plans", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::BeginDisabled(store->isEditing);

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
    ImGui::EndDisabled();

    // Editing Modal
    if (store->isEditing)
        runEditingModal(store);
    ImGui::End();
}

int main(int argc, char** argv)
{
    const std::shared_ptr<MainStore>& store = std::make_shared<MainStore>();
    runWindow("Hello", [&]() { setUpContents(store); });

    return 0;
}
