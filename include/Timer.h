#ifndef Timer_h
#define Timer_h

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
    uint32_t mStart;
    uint32_t mDuration;
    uint32_t mFinish;
    TimerStateType mState;
};

#endif  // Timer_h
