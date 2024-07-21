#pragma once

#include <functional>
#include <imgui.h>
#include <string>
#include <vector>

namespace adaptiday
{
    void listTable(const std::string& name,
                   const std::vector<std::string>& _elements,
                   size_t _targetRows,
                   size_t _targetCols,
                   const std::function<void(const std::vector<std::string>& elements, const size_t pos)>& callback);
} // namespace adaptiday
