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

TelnetLoggerAdapter::TelnetLoggerAdapter()
{
    jlog::logger = this;
}

void TelnetLoggerAdapter::logV(const char* format, va_list args) const 
{
    vlog(jlog::Level::VERBOSE, format, args);
}

void TelnetLoggerAdapter::logD(const char* format, va_list args) const
{
    vlog(jlog::Level::DEBUG, format, args);
}

void TelnetLoggerAdapter::logI(const char* format, va_list args) const
{ 
    vlog(jlog::Level::INFO, format, args);
}

void TelnetLoggerAdapter::logW(const char* format, va_list args) const
{ 
    vlog(jlog::Level::WARNING, format, args);
}

void TelnetLoggerAdapter::logE(const char* format, va_list args) const
{
    vlog(jlog::Level::ERROR, format, args);
}

void TelnetLoggerAdapter::vlog(jlog::Level level, const char* format, va_list args) const
{
    char outbuf[100];
    vsprintf(outbuf, format, args);
    Log.println(outbuf);
}
