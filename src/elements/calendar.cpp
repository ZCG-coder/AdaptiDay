#include "imgui.h"
#include "platform.hpp"
#include "store/mainStore.hpp"

#include <chrono>
#include <memory>
#include <string>

using namespace std::literals;
using namespace adaptiday::__internals::utils;

int timeZone()
{
    std::time_t now = std::time(nullptr);
    std::tm local_tm = localtime_xp(&now);
    return static_cast<int>(local_tm.tm_gmtoff);
}

std::string formatHhMm(const auto& _time)
{
    // Convert to time_t
    auto time = _time;
    time -= std::chrono::seconds(timeZone());
    time_t t = std::chrono::system_clock::to_time_t(time);
    std::tm tm = localtime_xp(&t);
    // Remove the time zone delta
    std::string result;
    if (tm.tm_hour < 10)
        result += "0";
    result += std::to_string(tm.tm_hour);
    result += ":";
    if (tm.tm_min < 10)
        result += "0";
    result += std::to_string(tm.tm_min);
    return result;
}

void calendar(const std::shared_ptr<MainStore>& store)
{
    auto calendarTitle = "Calendar for Week " + std::to_string(store->planner.weekNo);
    ImGui::Begin(calendarTitle.c_str(), nullptr, store->defaultFlags);

    auto todayDate =
        std::chrono::year_month_weekday(std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now()));
    auto startTime = store->planner.startTime;
    auto endTime = store->planner.endTime;
    auto currentTime = startTime;
    const auto& rows = (endTime - startTime) / std::chrono::minutes(store->planner.spacing) - 1;

    auto todayIdx = todayDate.weekday().c_encoding();

    const int& cols = 8;
    auto header = store->planner.dates;
    header.insert(header.begin(), "Day");

    std::vector subheader = { "Time"s, "Mon"s, "Tue"s, "Wed"s, "Thu"s, "Fri"s, "Sat"s, "Sun"s };
    if (ImGui::BeginTable("cal",
                          cols,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY |
                              ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_ContextMenuInBody))
    {
        for (size_t row = 0; row < rows; row++)
        {
            if (row == 0 or row == 1) [[unlikely]]
                ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
            else
                ImGui::TableNextRow(ImGuiTableRowFlags_None);
            for (size_t col = 0; col < cols; col++)
            {
                ImGui::TableNextColumn();
                // Insert date
                if (row == 0) [[unlikely]]
                {
                    auto string = header.at(col);
                    if (not string.empty() and col != todayIdx)
                        ImGui::Text("%s", string.c_str());
                    else
                        ImGui::TextColored({ 0, 1, 0, 1 }, "%s", string.c_str());
                }
                // Insert weekday
                else if (row == 1) [[unlikely]]
                {
                    auto string = subheader.at(col);
                    if (not string.empty() and col != todayIdx)
                        ImGui::Text("%s", string.c_str());
                    else
                        ImGui::TextColored({ 0, 1, 0, 1 }, "%s", string.c_str());
                }

                // Insert Time
                else [[likely]] if (col == 0) [[unlikely]]
                {
                    ImGui::Text("%s", formatHhMm(currentTime).c_str());
                    currentTime += std::chrono::minutes(store->planner.spacing);
                }
            }
        }
        ImGui::EndTable();
    }

    ImGui::End();
}
