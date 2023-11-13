#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

enum class TimerStateType: uint8_t
{
    NotStarted,
    Running,
    Finished
};

class Timer
{
public:
    Timer();
    Timer(uint32_t duration);
    bool start();
    bool start(uint32_t duration);
    void restart();
    void reset();
    bool setDuration(uint32_t duration);
    bool isFinished();
    void tick();
    TimerStateType& getState();

private:
    uint32_t m_start;
    uint32_t m_duration;
    uint32_t m_finish;
    TimerStateType m_state;
};

#endif  // TIMER_H
