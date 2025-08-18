#pragma once

#include <chrono>

class Timer {
public:
    static void startTimer();
    [[nodiscard]] static float stopTimer();

private:
    static std::chrono::time_point<std::chrono::system_clock> s_start, s_end;
    static std::chrono::duration<float> s_duration;
};
