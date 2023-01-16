#pragma once

namespace moe {

    struct Color {
        Color(float r, float g, float b)
                : r{r}, g{g}, b{b} {}

        Color()
                : r{0}, g{0}, b{0} {}

        float r;
        float g;
        float b;

        static Color black() { return Color{ 0, 0, 0 }; }
        static Color white() { return Color{ 1, 1, 1 };}
    };
}