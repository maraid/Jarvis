#include "logger.h"



namespace jlog
{
    GenericLoggerIF* logger{nullptr};

    void verbose(const char* format, ...)
    {
        if (logger != nullptr)
        {
            va_list list;
            va_start(list, format);
            logger->logV(format, list);
            va_end(list);
        }
    }

    void debug(const char* format, ...)
    {
        if (logger != nullptr)
        {
            va_list list;
            va_start(list, format);
            logger->logD(format, list);
            va_end(list);
        }
    }

    void info(const char* format, ...)
    {
        if (logger != nullptr)
        {
            va_list list;
            va_start(list, format);
            logger->logI(format, list);
            va_end(list);
        }
    }

    void warning(const char* format, ...)
    {
        if (logger != nullptr)
        {
            va_list list;
            va_start(list, format);
            logger->logW(format, list);
            va_end(list);
        }
    }

    void error(const char* format, ...)
    {
        if (logger != nullptr)
        {
            va_list list;
            va_start(list, format);
            logger->logE(format, list);
            va_end(list);
        }
    }

    void log(Level level, const char* format, ...)
    {
        if (logger != nullptr)
        {
            va_list list;
            va_start(list, format);
            logger->log(level, format, list);
            va_end(list);
        }
    }

} // namespace jlog
