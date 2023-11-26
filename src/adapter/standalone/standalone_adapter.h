#ifndef STANDALONE_ADAPTER_H
#define STANDALONE_ADAPTER_H

#include <cstdarg>

#include "jarvis.h"
#include "telnet_logger.h"
#include "logger.h"


class Logger : public jlog::GenericLoggerIF
{
public:
    Logger();

    void logV(const char* format, va_list args) const override;
    void logD(const char* format, va_list args) const override;
    void logI(const char* format, va_list args) const override;
    void logW(const char* format, va_list args) const override;
    void logE(const char* format, va_list args) const override;
    void vlog(jlog::Level level, const char* format, va_list args) const override;
};

extern Logger logger;

struct StandaloneJarvisAdapter : public Jarvis
{
    StandaloneJarvisAdapter();
    
    void setup() override;
    void loop() override;

private:
    Logger m_logger;
};

#endif // STANDALONE_ADAPTER_H
