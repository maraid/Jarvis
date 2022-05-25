#include "Timer.h"

Timer::Timer()
    : mState(TimerStateType::NotStarted)
{
}

Timer::Timer(uint32_t duration)
    : mDuration(duration),
    mState(TimerStateType::NotStarted)
{
}

bool Timer::start()
{
    if (mState == TimerStateType::NotStarted)
    {
        mStart = millis();
        mFinish = millis() + mDuration;
        mState = TimerStateType::Running;
        return true;
    }
    return false;
}

bool Timer::start(uint32_t duration)
{
    setDuration(duration);
    return start();
}

void Timer::restart()
{
    reset();
    start();
}

void Timer::reset()
{
    mState = TimerStateType::NotStarted;
}

void Timer::tick()
{
    if (mState == TimerStateType::Running && millis() >= mFinish)
    {
        mState = TimerStateType::Finished;
    }
}

bool Timer::isFinished()
{
    tick();
    return mState == TimerStateType::Finished;
}

bool Timer::setDuration(uint32_t duration)
{
    if (mState == TimerStateType::NotStarted)
    {
        mDuration = duration;
        return true;
    }
    return false;
}