#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <vector>
#include "Utils/Singleton.h"

#if _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace JustRenderIt
{
  enum LogLevel
  {
    LogLower,
    LogNormal,
    LogHigher,
  };

  /// Handle the log IO.
  class LogPolicy: public BaseObject
  {
  public:
    virtual void Create(STRING filename) = 0;
    virtual void Destroy() = 0;

    virtual void Write(STRING content, LogLevel level) = 0;
    virtual void WriteBanner(STRING content) = 0;
  };

  class DLL_DECLARE Logger: public Singleton<Logger>,
                            public BaseObject
  {
  protected:
    typedef std::vector<LogPolicy*> VecPolicy;
    typedef VecPolicy::iterator     IterPolicy;

    VecPolicy   m_policies;
    LogLevel    m_logLevel;
  protected:
    virtual void internalCreate();
    virtual void internalDestroy();
  public:
    Logger(){ m_logLevel = LogNormal; };

    void AddPolicy(STRING filename, LogPolicy* policy);
    void Destroy();

    void SetFilter(LogLevel level){ m_logLevel = level; };

    virtual void Write(LogLevel level, const char* format, ...);
    virtual void WriteBanner(const char* content, ...);
    virtual void Warning(const char* content, ...);
    virtual void Error(const char* content, ...);
    virtual void Fatal(const char* content, ...);

    DECL_SINGLETON(Logger);
  };

#define LOG1(lv, c1) JustRenderIt::Logger::InstanceRef().Write(LogLower, c1)
#define LOG2(lv, c1, c2) JustRenderIt::Logger::InstanceRef().Write(LogLower, c1, c2)
#define LOG3(lv, c1, c2, c3) JustRenderIt::Logger::InstanceRef().Write( \
    LogLower, c1, c2, c3)

#define LOG_CONTENT1(c1) JustRenderIt::Logger::InstanceRef().Write(LogNormal, c1)
#define LOG_CONTENT2(c1, c2) JustRenderIt::Logger::InstanceRef().Write( \
    LogNormal, c1, c2)
#define LOG_CONTENT3(c1, c2, c3) JustRenderIt::Logger::InstanceRef().Write( \
    LogNormal, c1, c2, c3)

#define LOG_WARNING1(c1) JustRenderIt::Logger::InstanceRef().Warning(c1)
#define LOG_WARNING2(c1, c2) JustRenderIt::Logger::InstanceRef().Warning(c1, c2)
#define LOG_WARNING3(c1, c2, c3) JustRenderIt::Logger::InstanceRef().Warning( \
    c1, c2, c3)

#define LOG_ERROR1(c1) JustRenderIt::Logger::InstanceRef().Error(c1)
#define LOG_ERROR2(c1, c2) JustRenderIt::Logger::InstanceRef().Error(c1, c2)
#define LOG_ERROR3(c1, c2, c3) JustRenderIt::Logger::InstanceRef().Error( \
    c1, c2, c3)

#define LOG_FATAL1(c1) JustRenderIt::Logger::InstanceRef().Fatal(c1)
#define LOG_FATAL2(c1, c2) JustRenderIt::Logger::InstanceRef().Fatal(c1, c2)
#define LOG_FATAL3(c1, c2, c3) JustRenderIt::Logger::InstanceRef().Fatal( \
    c1, c2, c3)

    class LoggerHtml: public LogPolicy
  {
  protected:
    FILE* m_file;
    STRING m_filename;

    virtual void internalCreate();
    virtual void internalDestroy();
  public:
    enum LoggerColor
    {
      LogRed,
      LogGreen,
      LogBlue,
      LogBlack,
      LogColorCount,
    };

    enum LoggerFormat
    {
      LogTitle,
      LogHeader,
      LogSubheader,
      LogContent,
      LogList,
      LogFormatCount,
    };
  public:
    LoggerHtml();

    virtual void Create(STRING filename = "");
    virtual void Destroy();

    virtual void Write(STRING content, LogLevel level);
    virtual void WriteBanner(STRING content);

    virtual void WriteFormat(STRING content, LogLevel level,
                             LoggerFormat format);
  };

  class LoggerPlainText: public LogPolicy
  {
  protected:
    FILE*   m_file;
    STRING m_filename;

    virtual void internalCreate();
    virtual void internalDestroy();
  public:
    LoggerPlainText();

    virtual void Create(STRING filename);
    virtual void Destroy();

    virtual void Write(STRING content, LogLevel level);
    virtual void WriteBanner(STRING content);
  };

};

#if _MSC_VER
#pragma warning(pop)
#endif

#endif
