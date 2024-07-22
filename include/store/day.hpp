#pragma once

#include <array>
// #include <chrono>
#include <vector>

namespace adaptiday
{
    class Event
    {
    public:
        std::array<char, 1024> name;

        std::array<char, 9126> desc;

        std::vector<std::vector<bool>> time;

        bool initialized = false;

        Event(const std::array<char, 1024>& name,
              const std::array<char, 9126>& desc,
              const std::vector<std::vector<bool>>& time) :
            name(name), desc(desc), time(time), initialized(true) {};
    };

    class Day
    {
    public:
        std::vector<Event> events;

        Day() = default;
    };

    class Planner
    {
    public:
        int weekNo = 0;

        std::array<Day, 7> days{};

        Planner()
        {
            // auto now = std::chrono::system_clock::now();

            // // Convert the current time to a year-month-day
            // auto date = std::chrono::floor<std::chrono::days>(now);
            // auto ymd = std::chrono::year_month_day(date);

            // // Get the weekday for the current date
            // auto wd = std::chrono::weekday(ymd);

            // // Get the year and week number for the current date
            // auto year = static_cast<int>(ymd.year());
            // weekNo = (date - std::chrono::year_month_day(year, 1, 1)).count() / 7 + 1;
            weekNo = 0;
        }
    };
} // namespace adaptiday