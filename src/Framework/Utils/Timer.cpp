#include "Common.h"
#include "Timer.h"

using namespace JustRenderIt;

#ifdef WIN32

IMPL_SINGLETON(WindowsTimer);

void WindowsTimer::internalCreate()
{
  QueryPerformanceFrequency(&m_freq);
  QueryPerformanceCounter(&m_initTime);
  m_crtTime = m_lastTime = m_initTime;
  m_isOK = true;
}

void WindowsTimer::internalDestroy()
{

}

double WindowsTimer::Update()
{
  m_lastTime = m_crtTime;
  QueryPerformanceCounter(&m_crtTime);

  unsigned long long elapsedTime = m_crtTime.QuadPart - m_lastTime.QuadPart;
  m_timeSinceLastUpdate = (float)(elapsedTime) / m_freq.QuadPart;

  unsigned long long initTime = m_crtTime.QuadPart - m_initTime.QuadPart;
  m_timeSinceInit = (float)(initTime) / m_freq.QuadPart;

  return m_timeSinceLastUpdate;
}

#endif // eof ifdef WIN32

#ifdef LINUX
IMPL_SINGLETON(LinuxTimer);

#include <sys/time.h>

void LinuxTimer::internalCreate()
{
  clock_gettime(CLOCK_REALTIME, &m_initTime);
}

void LinuxTimer::internalDestroy()
{

}

timespec LinuxTimer::diff(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

#define BILLION 1000000000L

double LinuxTimer::Update()
{
  m_lastTime = m_crtTime;
  clock_gettime(CLOCK_REALTIME, &m_crtTime);

  timespec dt = diff(m_crtTime, m_lastTime);
  //timespec it = diff(m_crtTime, m_initTime);
  m_timeSinceLastUpdate = dt.tv_sec + dt.tv_nsec / BILLION;
  m_timeSinceInit = dt.tv_sec + dt.tv_nsec / BILLION;

  return m_timeSinceLastUpdate;
}

#endif // eof ifdef LINUX

IMPL_SINGLETON(FrameTimer);
IMPL_SINGLETON(PerfTimer);
