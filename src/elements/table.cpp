#include "elements/table.hpp"

#include <functional>
#include <imgui.h>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std::literals;

namespace adaptiday
{
    void listTable(const std::string& name,
                   const std::vector<std::string>& _elements,
                   const size_t _targetRows,
                   const size_t _targetCols,
                   const std::function<void(const std::vector<std::string>& elements, const size_t pos)>& callback)
    {
        auto elements = _elements;
        if (elements.empty())
            return;

        if ((_targetCols != 0) and (_targetRows != 0))
            throw std::invalid_argument("targetRows and targetCols cannot be set together.");

        size_t targetCols = 0;
        if (_targetCols != 0)
            targetCols = _targetCols;
        else if (elements.size() % _targetRows == 0)
            targetCols = elements.size() / _targetRows;
        else
        {
            // +1 to make sure all elements are displayed
            targetCols = elements.size() / _targetRows + 1;

            // Insert empty strings to make sure all elements are displayed
            for (size_t i = 0; i < elements.size() % _targetRows + 1; i++)
                elements.emplace_back("");
        }

        size_t targetRows = _targetRows;
        if (targetCols == 0)
        {
            if (_targetRows == 0 and elements.size() % targetCols == 0)
                targetRows = elements.size() / targetCols;
            else
                targetRows = elements.size() / targetCols + 1;
        }

        if (ImGui::BeginTable(name.c_str(),
                              static_cast<int>(targetCols),
                              ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                                  ImGuiTableFlags_Reorderable | ImGuiTableFlags_Sortable | ImGuiTableFlags_ScrollX |
                                  ImGuiTableFlags_ScrollY | ImGuiTableFlags_HighlightHoveredColumn |
                                  ImGuiTableFlags_ContextMenuInBody))
        {
            for (size_t row = 0; row < targetRows; row++)
            {
                ImGui::TableNextRow();
                for (size_t col = 0; col < targetCols; col++)
                {
                    auto idx = row * targetCols + col;
                    ImGui::TableSetColumnIndex(static_cast<int>(col));
                    auto string = elements.at(idx);
                    if (not string.empty())
                        if (ImGui::Button(string.c_str()))
                            callback(elements, idx);
                }
            }
            ImGui::EndTable();
        }
    }

} // namespace adaptiday
