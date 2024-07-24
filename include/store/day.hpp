#pragma once

#include <array>
#include <chrono>
#include <vector>

using namespace std::chrono_literals;

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
            name(name), desc(desc), time(time), initialized(true)
        {
            initTimeVector();
        }

        Event(const std::array<char, 1024>& name,
              const std::array<char, 9126>& desc,
              const std::vector<int>& days,
              const std::chrono::time_point<std::chrono::system_clock>& start,
              const std::chrono::time_point<std::chrono::system_clock>& end) :
            name(name), desc(desc)
        {
            initTimeVector();

            // Get the minutes in start / end
            const auto& midnight = std::chrono::time_point<std::chrono::system_clock>{};
            auto startMinutes = std::chrono::duration_cast<std::chrono::minutes>(start - midnight).count();
            auto minutes = std::chrono::duration_cast<std::chrono::minutes>(end - start);

            // Fill in the sessions
            for (auto day : days)
                for (auto i = startMinutes; i < startMinutes + minutes.count(); i++)
                    time[day][i] = true;
        }

        void initTimeVector() { time.resize(7, std::vector<bool>(1440, false)); }
    };

    class Day
    {
    public:
        std::vector<Event> events;

        std::chrono::year_month_day date;

        explicit Day(const std::chrono::year_month_day& date) : date(date) {}

        void addEvent(const Event& event)
        {
            if (event.initialized)
                events.emplace_back(event);
        }
    };

    class Planner
    {
    public:
        int weekNo = 0;

        std::vector<Day> days;
        std::vector<std::string> dates;
        std::chrono::time_point<std::chrono::system_clock> startTime{ std::chrono::hours(8) };
        std::chrono::time_point<std::chrono::system_clock> endTime{ std::chrono::hours(22) };
        int spacing = 30;

        Planner()
        {
            auto now = std::chrono::system_clock::now();
            auto date = std::chrono::floor<std::chrono::days>(now);
            auto ymd = std::chrono::year_month_day(date);
            auto year = ymd.year();

            // Calculate the number of days since January 1st of the current year
            auto jan1 = std::chrono::year{ year } / 1 / 1;
            auto daysSinceJan1 = (date - std::chrono::floor<std::chrono::days>(std::chrono::sys_days{ jan1 })).count();
            weekNo = ceil(static_cast<double>(daysSinceJan1) / 7);

            days.resize(7, Day((2000y / 1 / 1)));
            dates.resize(7, "1/1");
            fillDaysArray();
        }

        void fillDaysArray()
        {
            // Get previous Sunday
            auto now = std::chrono::system_clock::now();
            auto date = std::chrono::floor<std::chrono::days>(now);
            auto ymw = std::chrono::year_month_weekday(date);
            auto ymd = std::chrono::year_month_day(date);
            auto weekday = ymw.weekday();
            auto daysToSunday = weekday == std::chrono::weekday{ 0 } ? 0 : weekday.c_encoding();

            // Fill in the days
            for (auto i = 0; i < 7; i++)
            {
                auto day = std::chrono::year_month_day{ ymw.year(),
                                                        ymw.month(),
                                                        std::chrono::day{ ymd.day() + std::chrono::days(i) -
                                                                          std::chrono::days(daysToSunday) } };
                days[i] = Day(day);
                dates[i] = std::to_string((unsigned)day.day()) + "/" + std::to_string((unsigned)day.month());
            }
        }
    };
} // namespace adaptiday
