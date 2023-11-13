#ifndef STANDALONE_ADAPTER_H
#define STANDALONE_ADAPTER_H

#include <cstdarg>

#include "jarvis.h"
#include "telnet_logger.h"
#include "logger.h"


class TelnetLoggerAdapter : public jlog::GenericLoggerIF
{
public:
    TelnetLoggerAdapter();

    void logV(const char* format, va_list args) const override;
    void logD(const char* format, va_list args) const override;
    void logI(const char* format, va_list args) const override;
    void logW(const char* format, va_list args) const override;
    void logE(const char* format, va_list args) const override;
    void vlog(jlog::Level level, const char* format, va_list args) const override;
};

struct StandaloneJarvisAdapter : public Jarvis
{
    StandaloneJarvisAdapter();
    
    void setup() override;
    void loop() override;

private:
    TelnetLoggerAdapter m_logger;
};

#endif // STANDALONE_ADAPTER_H
