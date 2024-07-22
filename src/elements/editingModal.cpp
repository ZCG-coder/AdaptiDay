#include "imgui.h"
#include "output.hpp"
#include "store/mainStore.hpp"

#include <SDL.h>
#include <SDL_opengl.h>
#include <algorithm>
#include <memory>
#include <string>

using namespace adaptiday;
using namespace std::literals;

void runEditingModal(const std::shared_ptr<MainStore>& store)
{
    ImGui::Begin("Editing",
                 &store->isEditing,
                 ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking |
                     ImGuiWindowFlags_NoResize);
    ImGui::Text("Editing \"%s\"", store->items.at(store->editingIdx).c_str());

    ImGui::Text("Title: ");
    ImGui::SameLine();
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
