#pragma once

#include "imgui.h"
#include "store/day.hpp"

#include <array>
#include <string>
#include <vector>

struct MainStore
{
    std::array<char, 100> quickAddBuf{};
    std::vector<std::string> items;
    bool isFirstLoop = true;

    adaptiday::Planner planner;

    // Editing modal
    bool isEditing = false;
    bool isEditingFirstLoop = true;
    std::array<char, 100> editingBuf{};
    size_t editingIdx;
    std::string editingErrMsg;

    static const auto defaultFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
};
