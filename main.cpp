#include "elements/table.hpp"
#include "gui.hpp"
#include "imgui.h"
#include "output.hpp"
#include "store/mainStore.hpp"

#include <SDL.h>
#include <SDL_opengl.h>
#include <algorithm>
#include <memory>
#include <string>

// NOTE TO DEVELOPERS: This file is a test file for the GUI module of the Steppable library.
//                     When building the GUI part for the first time, this can come in handy.
//                     Run the executable to see if the GUI window works.

using namespace std::literals;
using namespace adaptiday;
using namespace adaptiday::__internals;

std::vector<std::string> onTableEdit(const std::vector<std::string>& _elements,
                                     const size_t pos,
                                     const std::shared_ptr<MainStore>& store)
{
    store->isEditing = true;
    store->isEditingFirstLoop = true;
    return _elements;
}

void runEditingModal(const std::shared_ptr<MainStore>& store)
{
    ImGui::Begin("Editing",
                 &store->isEditing,
                 ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking |
                     ImGuiWindowFlags_NoResize);
    ImGui::Text("Editing \"%s\"", store->items.at(store->editingIdx).c_str());
    bool enterPressed = ImGui::InputTextWithHint(
        " ", "New Title", store->editingBuf.data(), store->editingBuf.size(), ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::TextColored(ImVec4(1.0F, 0, 0, 1.00F), "%s", store->editingErrMsg.c_str());

    if (store->isEditingFirstLoop)
    {
        std::ranges::copy(store->items.at(store->editingIdx), store->editingBuf.data());
        store->isEditingFirstLoop = false;
    }
    bool editingBufEmpty = std::string(store->editingBuf.data()).empty();

    // Show errors
    if (editingBufEmpty)
        store->editingErrMsg = "ERROR: Empty string is not allowed";
    else
        store->editingErrMsg = "";

    if ((ImGui::Button("Done") or enterPressed) and not editingBufEmpty)
    {
        const auto orig = store->items.at(store->editingIdx);
        store->items.at(store->editingIdx) = store->editingBuf.data();
        const auto& after = store->items.at(store->editingIdx);
        store->editingBuf.fill(0);
        store->isEditing = false;

        output::info("runEditingModal"s, "Changed {0} -> {1}"s, { orig, after });
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel") or ImGui::IsKeyReleased(ImGuiKey_Escape))
    {
        store->editingBuf.fill(0);
        store->isEditing = false;
    }
    ImGui::End();
}

void setUpContents(const std::shared_ptr<MainStore>& store)
{
    ImGui::Begin("Plans", nullptr, ImGuiWindowFlags_NoCollapse);
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
        auto res = onTableEdit(elements, pos, store);
        store->items = res;
        store->editingIdx = pos;
    });
    ImGui::EndDisabled();

    // Editing Modal
    if (store->isEditing)
        runEditingModal(store);
    ImGui::End();
}

int main()
{
    const std::shared_ptr<MainStore>& store = std::make_shared<MainStore>();
    runWindow("Hello", [&]() { setUpContents(store); });
}
