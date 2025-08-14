#include "Timer.hpp"

std::chrono::time_point<std::chrono::system_clock> Timer::s_start, Timer::s_end;
std::chrono::duration<float> Timer::s_duration;

void Timer::startTimer() {
    s_start = std::chrono::high_resolution_clock::now();
}

float Timer::stopTimer() {
    s_end = std::chrono::high_resolution_clock::now();
    s_duration = s_end - s_start;

    float milliseconds = s_duration.count() * 1000.0f;
    return milliseconds;
}
