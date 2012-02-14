#ifndef __TIMER_H__
#define __TIMER_H__

#include "Common.h"
#include "Singleton.h"

namespace JustRenderIt
{
  class DLL_DECLARE Timer
  {
  protected:
    double m_timeSinceLastUpdate;
    double m_timeSinceInit;

    bool m_isOK;
  public:
    Timer()
    {
      m_isOK = false;

      m_timeSinceInit = 0;
      // to prevent getting divided by 0 while trying to get fps
      // without calling update, we assign m_timeSinceLastUpdate with 1
      m_timeSinceLastUpdate = 1;
    }

    ~Timer()
    {
    }

    /// Reset the timer and return time elapsed
    virtual double Update() = 0;

    /// Return time elapsed since last update
    double TimeSinceLastUpdate()
    {
      #if defined _WIN32
      if(!m_isOK)
        throw Exception("Timer is not initialized yet while "
                              "trying to get time.");
      return m_timeSinceLastUpdate;
      #else
      return 0;  // TODO
      #endif
    };

    /// REturn time elapsed since the timer is created;
    double TimeSinceInit()
    {
      #if defined _WIN32
      if(!m_isOK)
        throw Exception("Timer is not initialized yet while "
                              "trying to get time.");
      return m_timeSinceInit;
      #else
      return 0;
      #endif
    }
  };

#ifdef WIN32

#define NATIVE_TIMER WindowsTimer

  /// Windows implementation of timer
  class DLL_DECLARE WindowsTimer: public Timer
  {
  protected:
    LARGE_INTEGER m_freq;
    LARGE_INTEGER m_crtTime, m_lastTime, m_initTime;
  protected:
    virtual void internalCreate();
    virtual void internalDestroy();
  public:
    WindowsTimer(){ internalCreate(); };
    ~WindowsTimer(){ internalDestroy(); };
    double Update();

    DECL_SINGLETON(WindowsTimer);
  };
#endif

#ifdef LINUX
#define NATIVE_TIMER LinuxTimer

  /// Linux implementation of timer
  class DLL_DECLARE LinuxTimer: public Timer
  {
  protected:
    timespec m_timeres;
    timespec m_crtTime, m_lastTime, m_initTime;
  protected:
    timespec diff(timespec start, timespec end);

    virtual void internalCreate();
    virtual void internalDestroy();
  public:
    LinuxTimer(){ internalCreate(); };
    ~LinuxTimer(){ internalDestroy(); };
    double Update();

    DECL_SINGLETON(LinuxTimer);
  };
#endif

  class DLL_DECLARE FrameTimer: public NATIVE_TIMER,
    public Singleton<FrameTimer>
  {
  private:
    int    m_framesAccu;
    double m_timesAccu, m_fps;
  public:
    FrameTimer()
    {
      m_fps = 0;
      m_timesAccu = 0;
      m_framesAccu = 0;
    }
    DECL_SINGLETON(FrameTimer);

    double Update()
    {
      NATIVE_TIMER::Update();

      m_framesAccu ++;
      m_timesAccu += m_timeSinceLastUpdate;

      if(m_timesAccu >= 1.0f)
      {
        m_fps = m_framesAccu / m_timesAccu;
        m_framesAccu = 0;
        m_timesAccu = 0.f;
      }

      return m_timeSinceLastUpdate;
    }

    /// FPS is update every second
    double GetFPS()
    {
      return m_fps;
    };
  };

  class DLL_DECLARE PerfTimer: public NATIVE_TIMER,
    public Singleton<PerfTimer>
  {
  public:
    DECL_SINGLETON(PerfTimer);
  };
}

#endif //__TIMER_H__
