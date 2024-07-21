#pragma once

#include <array>
#include <string>
#include <vector>

struct MainStore
{
    std::array<char, 100> quickAddBuf{};
    std::vector<std::string> items;
    bool isFirstLoop = true;

    // Editing modal
    bool isEditing = false;
    bool isEditingFirstLoop = true;
    std::array<char, 100> editingBuf{};
    size_t editingIdx;
    std::string editingErrMsg;
};
