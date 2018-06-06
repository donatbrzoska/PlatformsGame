#pragma once
#include <chrono>

class FrameTimer {
public:
    static void initialize(int fps);
    static bool newFrame();
};
