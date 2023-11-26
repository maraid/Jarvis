#include "standalone_adapter.h"


StandaloneJarvisAdapter::StandaloneJarvisAdapter()
    : m_logger()  // call this first
{
}

void StandaloneJarvisAdapter::setup()
{
    Jarvis::setup();
}

void StandaloneJarvisAdapter::loop()
{
    Jarvis::loop();
}

Logger::Logger()
{
    jlog::logger = this;
}

void Logger::logV(const char* format, va_list args) const 
{
    vlog(jlog::Level::VERBOSE, format, args);
}

void Logger::logD(const char* format, va_list args) const
{
    vlog(jlog::Level::DEBUG, format, args);
}

void Logger::logI(const char* format, va_list args) const
{ 
    vlog(jlog::Level::INFO, format, args);
}

void Logger::logW(const char* format, va_list args) const
{ 
    vlog(jlog::Level::WARNING, format, args);
}

void Logger::logE(const char* format, va_list args) const
{
    vlog(jlog::Level::ERROR, format, args);
}

void Logger::vlog(jlog::Level level, const char* format, va_list args) const
{
    char outbuf[100];
    vsprintf(outbuf, format, args);
    Log.println(outbuf);
}
