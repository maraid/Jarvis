#ifndef LOGGER_H
#define LOGGER_H

#include <cstdarg>

namespace jlog
{
    enum class Level
    {
        VERBOSE,
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    class GenericLoggerIF
    {
    public:
        virtual void logV(const char* format, va_list args) const = 0;  // verbose
        virtual void logD(const char* format, va_list args) const = 0;  // debug
        virtual void logI(const char* format, va_list args) const = 0;  // info
        virtual void logW(const char* format, va_list args) const = 0;  // warning
        virtual void logE(const char* format, va_list args) const = 0;  // error
        virtual void log(Level level, const char* format, va_list args) const = 0;
    };

    extern GenericLoggerIF* logger;

    void verbose(const char* format, ...);
    void debug(const char* format, ...);
    void info(const char* format, ...);
    void warning(const char* format, ...);
    void error(const char* format, ...);
    void log(Level level, const char* format, ...);
}

#endif // LOGGER_H