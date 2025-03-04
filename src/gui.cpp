#include "gui.hpp"

#include "imgui.h"
#include "output.hpp"

#ifdef MACOSX
    #include <CoreFoundation/CoreFoundation.h>
#elif defined(WINDOWS)
    #include <windows.h>
#endif

namespace adaptiday::__internals
{
    bool isDarkModeEnabled()
    {
#if defined(MACOSX)
        bool isDarkMode = false;
        CFPreferencesAppSynchronize(CFSTR("AppleInterfaceStyle"));
        CFPropertyListRef value = CFPreferencesCopyAppValue(CFSTR("AppleInterfaceStyle"), kCFPreferencesAnyApplication);
        if (value != nullptr)
        {
            const auto* interfaceStyle = static_cast<CFStringRef>(value);
            if (CFStringCompare(interfaceStyle, CFSTR("Dark"), 0) == kCFCompareEqualTo)
                isDarkMode = true;
            CFRelease(value);
        }
        return isDarkMode;
#elif defined(LINUX)
        return std::filesystem::exists("/usr/share/themes/Adwaita-dark/gtk-3.0");
#elif defined(WINDOWS)
        HKEY key;
        if (RegOpenKeyExA(HKEY_CURRENT_USER,
                          "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                          0,
                          KEY_QUERY_VALUE,
                          &key) == ERROR_SUCCESS)
        {
            DWORD value = 0;
            DWORD size = sizeof(DWORD);
            if (RegQueryValueExA(key, "AppsUseLightTheme", nullptr, nullptr, reinterpret_cast<LPBYTE>(&value), &size) ==
                ERROR_SUCCESS)
                return value == 0;
        }
        return false;
#else
        return false;
#endif
    }

    void addFontIfExistent(const ImGuiIO* io,
                           const std::string& path,
                           const ImFontConfig* config,
                           const ImWchar* ranges) noexcept
    {
        if (io->Fonts->Fonts.empty() and config->MergeMode)
            config = nullptr;
        if (std::filesystem::exists(path))
        {
#ifdef DEBUG
            output::info("addIfExistent"s, "Added font {0}"s, { path });
#endif
            io->Fonts->AddFontFromFileTTF(static_cast<const char*>(path.c_str()), 25.0F, config, ranges);
        }
    }

    void loadFonts(const ImGuiIO* io) noexcept
    {
#ifdef HAS_FREETYPE
        output::info("loadFonts"s, "Loading fonts using FreeType"s);
#endif

        ImFontConfig config;
        config.MergeMode = true;
        config.RasterizerDensity = 2.0F;
        config.PixelSnapH = true;
        config.OversampleH = config.OversampleV = 3;

#ifdef WINDOWS
        // WINDOWS fonts
        // -------------
        // Chinese    -> Microsoft YaHei
        // Cyrillic   -> Segoe UI -----------------+
        // Greek      -> Segoe UI -----------------|
        // Japanese   -> Meiryo                    |
        // Korean     -> Malgun Gothic             +--> Top-priority
        // Thai       -> Leelawadee                |
        // Vietnamese -> Segoe UI -----------------+

        // Load top-priority fonts
        addFontIfExistent(io, "C:/Windows/Fonts/segoeui.ttf", &config, io->Fonts->GetGlyphRangesCyrillic());
        addFontIfExistent(io, "C:/Windows/Fonts/segoeui.ttf", &config, io->Fonts->GetGlyphRangesDefault());
        addFontIfExistent(io, "C:/Windows/Fonts/segoeui.ttf", &config, io->Fonts->GetGlyphRangesGreek());
        addFontIfExistent(io, "C:/Windows/Fonts/segoeui.ttf", &config, io->Fonts->GetGlyphRangesVietnamese());

        // Load Chinese fonts
        addFontIfExistent(io, "C:/Windows/Fonts/msyh.ttc", &config, io->Fonts->GetGlyphRangesChineseFull());

        // Load Japanese fonts
        addFontIfExistent(io, "C:/Windows/Fonts/meiryo.ttc", &config, io->Fonts->GetGlyphRangesJapanese());

        // Load Korean fonts
        addFontIfExistent(io, "C:/Windows/Fonts/malgun.ttf", &config, io->Fonts->GetGlyphRangesKorean());

        // Load Thai fonts
        addFontIfExistent(io, "C:/Windows/Fonts/leelawad.ttf", &config, io->Fonts->GetGlyphRangesThai());
#elif defined(MACOSX)
        // MACOS fonts
        // -------------
        // Chinese    -> PingFang SC (*)
        // Cyrillic   -> SF Pro -----------------+
        // Greek      -> SF Pro -----------------|
        // Japanese   -> Hiragino Sans           |
        // Korean     -> Apple SD Gothic Neo     +--> Top-priority
        // Thai       -> Thonburi                |
        // Vietnamese -> SF Pro -----------------+
        //
        // (*) NOTE: PingFang may not be available on all systems, but STHeiti Medium is a good alternative.

        // Load top-priority fonts
        addFontIfExistent(io, "/Library/Fonts/SF-Pro.ttf", &config, io->Fonts->GetGlyphRangesCyrillic());
        addFontIfExistent(io, "/Library/Fonts/SF-Pro.ttf", &config, io->Fonts->GetGlyphRangesDefault());
        addFontIfExistent(io, "/Library/Fonts/SF-Pro.ttf", &config, io->Fonts->GetGlyphRangesGreek());
        addFontIfExistent(io, "/Library/Fonts/SF-Pro.ttf", &config, io->Fonts->GetGlyphRangesVietnamese());

        // Load Chinese fonts
        addFontIfExistent(io, "/System/Library/Fonts/PingFang.ttc", &config, io->Fonts->GetGlyphRangesChineseFull());
        addFontIfExistent(
            io, "/System/Library/Fonts/STHeiti Medium.ttc", &config, io->Fonts->GetGlyphRangesChineseFull());

        // Load Japanese fonts
        addFontIfExistent(io, "/System/Library/Fonts/Hiragino.ttc", &config, io->Fonts->GetGlyphRangesJapanese());

        // Load Korean fonts
        addFontIfExistent(io, "/System/Library/Fonts/AppleSDGothicNeo.ttc", &config, io->Fonts->GetGlyphRangesKorean());

        // Load Thai fonts
        addFontIfExistent(io, "/System/Library/Fonts/Thonburi.ttf", &config, io->Fonts->GetGlyphRangesThai());
        addFontIfExistent(
            io, "/System/Library/Fonts/Supplemental/Ayuthaya.ttf", &config, io->Fonts->GetGlyphRangesThai());
#elif defined(LINUX)
        // LINUX fonts
        // -------------
        // Chinese    -> WenQuanYi Zen Hei
        // Cyrillic   -> DejaVu Sans -----------------+
        // Greek      -> DejaVu Sans -----------------|
        // Japanese   -> Takao Gothic                 |
        // Korean     -> Nanum Gothic                 +--> Top-priority
        // Thai       -> Garuda                       |
        // Vietnamese -> DejaVu Sans -----------------+

        // Load top-priority fonts
        addIfExistent(io, "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf", &config, io->Fonts->GetGlyphRangesCyrillic());
        addIfExistent(io, "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf", &config, io->Fonts->GetGlyphRangesDefault());
        addIfExistent(io, "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf", &config, io->Fonts->GetGlyphRangesGreek());
        addIfExistent(io, "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf", &config, io->Fonts->GetGlyphRangesVietnamese());

        // Load Chinese fonts
        addIfExistent(io, "/usr/share/fonts/TTF/wqy-zenhei.ttc", &config, io->Fonts->GetGlyphRangesChineseFull());

        // Load Japanese fonts
        addIfExistent(io, "/usr/share/fonts/TTF/takao-mincho.ttf", &config, io->Fonts->GetGlyphRangesJapanese());

        // Load Korean fonts
        addIfExistent(io, "/usr/share/fonts/TTF/NanumGothic.ttf", &config, io->Fonts->GetGlyphRangesKorean());

        // Load Thai fonts
        addIfExistent(io, "/usr/share/fonts/TTF/garuda.ttf", &config, io->Fonts->GetGlyphRangesThai());
#endif
#if defined(WINDOWS) || defined(LINUX) || defined(MACOSX)
        // Add the default font as well.
        io->Fonts->AddFontDefault(&config);
#else
        io->Fonts->AddFontDefault();
#endif
        io->Fonts->Build();
        ImGui::GetIO().FontGlobalScale = 0.6F;
    }
} // namespace adaptiday::__internals
