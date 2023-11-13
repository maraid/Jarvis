#include "timer.h"

Timer::Timer()
    : m_state(TimerStateType::NotStarted)
{
}

Timer::Timer(uint32_t duration)
    : m_duration(duration),
    m_state(TimerStateType::NotStarted)
{
}

bool Timer::start()
{
    if (m_state == TimerStateType::NotStarted)
    {
        m_start = millis();
        m_finish = millis() + m_duration;
        m_state = TimerStateType::Running;
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
    m_state = TimerStateType::NotStarted;
}

void Timer::tick()
{
    if (m_state == TimerStateType::Running && millis() >= m_finish)
    {
        m_state = TimerStateType::Finished;
    }
}

bool Timer::isFinished()
{
    tick();
    return m_state == TimerStateType::Finished;
}

bool Timer::setDuration(uint32_t duration)
{
    if (m_state == TimerStateType::NotStarted)
    {
        m_duration = duration;
        return true;
    }
    return false;
}