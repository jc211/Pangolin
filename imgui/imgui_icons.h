#pragma once
#include <array>

struct textual_icon
{
    explicit constexpr textual_icon(const char(&unicode_icon)[4]) :
        _icon{ unicode_icon[0], unicode_icon[1], unicode_icon[2], unicode_icon[3] }
    {
    }
    operator const char* () const
    {
        return _icon.data();
    }
private:
    std::array<char, 5> _icon;
};

namespace textual_icons
{
    // A note to a maintainer - preserve order when adding values to avoid duplicates
    static const textual_icon file_movie{ u8"\uf008" };
    static const textual_icon times{ u8"\uf00d" };
    static const textual_icon download{ u8"\uf019" };
    static const textual_icon refresh{ u8"\uf021" };
    static const textual_icon lock{ u8"\uf023" };
    static const textual_icon camera{ u8"\uf030" };
    static const textual_icon video_camera{ u8"\uf03d" };
    static const textual_icon edit{ u8"\uf044" };
    static const textual_icon step_backward{ u8"\uf048" };
    static const textual_icon play{ u8"\uf04b" };
    static const textual_icon pause{ u8"\uf04c" };
    static const textual_icon stop{ u8"\uf04d" };
    static const textual_icon step_forward{ u8"\uf051" };
    static const textual_icon plus_circle{ u8"\uf055" };
    static const textual_icon question_mark{ u8"\uf059" };
    static const textual_icon info_circle{ u8"\uf05a" };
    static const textual_icon fix_up{ u8"\uf062" };
    static const textual_icon minus{ u8"\uf068" };
    static const textual_icon exclamation_triangle{ u8"\uf071" };
    static const textual_icon shopping_cart{ u8"\uf07a" };
    static const textual_icon bar_chart{ u8"\uf080" };
    static const textual_icon upload{ u8"\uf093" };
    static const textual_icon square_o{ u8"\uf096" };
    static const textual_icon unlock{ u8"\uf09c" };
    static const textual_icon floppy{ u8"\uf0c7" };
    static const textual_icon square{ u8"\uf0c8" };
    static const textual_icon bars{ u8"\uf0c9" };
    static const textual_icon caret_down{ u8"\uf0d7" };
    static const textual_icon repeat{ u8"\uf0e2" };
    static const textual_icon circle{ u8"\uf111" };
    static const textual_icon check_square_o{ u8"\uf14a" };
    static const textual_icon cubes{ u8"\uf1b3" };
    static const textual_icon toggle_off{ u8"\uf204" };
    static const textual_icon toggle_on{ u8"\uf205" };
    static const textual_icon connectdevelop{ u8"\uf20e" };
    static const textual_icon usb_type{ u8"\uf287" };
    static const textual_icon braille{ u8"\uf2a1" };
    static const textual_icon window_maximize{ u8"\uf2d0" };
    static const textual_icon window_restore{ u8"\uf2d2" };
    static const textual_icon grid{ u8"\uf1cb" };
    static const textual_icon exit{ u8"\uf011" };
    static const textual_icon see_less{ u8"\uf070" };
    static const textual_icon dotdotdot{ u8"\uf141" };
    static const textual_icon link{ u8"\uf08e" };
    static const textual_icon throphy{ u8"\uF091" };
    static const textual_icon metadata{ u8"\uF0AE" };
    static const textual_icon check{ u8"\uF00C" };
    static const textual_icon mail{ u8"\uF01C" };
    static const textual_icon cube{ u8"\uf1b2" };
    static const textual_icon measure{ u8"\uf545" };
}