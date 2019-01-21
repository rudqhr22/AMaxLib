#include "ATimer.h"
#include <stdio.h>
#include <tchar.h>

float    g_fTimer = 0.0f;
float    g_fSPF = 0.0f;
float	 g_fAccTime = 0.0f;

bool	ATimer::Init()
{
	QueryPerformanceCounter(&m_Frame);			//고해상도 서능카운터가 있으면 카운터의 주파수를 가져온다. 시스템이 작동하는 동안 주파수를 변경되지 않기에 1초가 나타는 틱카운터를 알수 있다.

	if (m_Frame.QuadPart == 0)
	{
		return false;
	}


	m_FPS = m_Frame;
	m_iFramePerSecond = 0;	//초당프레임
	m_iFPSElapse = 0;		//초당 경과된 프레임
	m_fSPF = 0.0f;	//프레임의 경과시간

	Start();
	return true;
}
bool	ATimer::Frame()
{
	QueryPerformanceCounter(&m_Current);
	g_fSPF = m_fSPF = static_cast<float>(m_Current.QuadPart - m_Frame.QuadPart) //현재시간-이전시간 = 경과된시간/이전시간?
		/ static_cast<float>(m_Frequency.QuadPart);					//이전시간			
	//g_fSPF = m_fSPF;
	g_fAccTime = m_fAccTime += m_fSPF;

	////1초당 프레임체크////
	if (((m_Current.LowPart - m_FPS.LowPart) / m_Frequency.LowPart) >= 1)
	{											// 1초가 경과 되었을 떄 참이 되는 코드로써 1초가 경과되기 전에		
		m_iFramePerSecond = m_iFPSElapse;		//카운팅 되는 m_iFPSElapse 값이 이떄 m_iFramePerSecond에 저장된다
		m_iFPSElapse = 0;
		m_FPS = m_Current;
	}
	m_iFPSElapse++;

	m_Frame = m_Current;
	return true;
}
bool	ATimer::Render() { return true; }
bool	ATimer::Release() { return true; }


void	ATimer::Reset()
{
	m_bStarted = false;
	m_fEventTime = 0.0f;
	memset(&m_Start, 0, sizeof(LARGE_INTEGER));
	memset(&m_Elapse, 0, sizeof(LARGE_INTEGER));
}
void	ATimer::Start()
{
	m_bStarted = true;
	m_fEventTime = 0.0f;
	QueryPerformanceCounter((LARGE_INTEGER *)&m_Start);
}
void	ATimer::Stop()
{
	GetElapsedTime();
	m_bStarted = false;
}

int		ATimer::GetFPS()
{
	return m_iFramePerSecond;
}

float	ATimer::GetSPF()
{
	return m_fSPF;
}


bool	ATimer::IsStarted() const
{
	return m_bStarted;
}
float	ATimer::GetElapsedTime()
{
	if (m_bStarted)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&m_Elapse);
		m_fEventTime = static_cast<float>(m_Elapse.LowPart - m_Start.LowPart) / static_cast<float>(m_Frequency.LowPart);
	}
	return m_fEventTime;
}

ATimer::ATimer()
{

	m_bStarted = false;
	m_iFramePerSecond = 0;
	m_iFPSElapse = 0;
	m_fSPF = 0.0f;
	m_fEventTime = 0.0f;
	m_fAccTime = 0.0f;
	//시스템의 주파수 변동폭을 얻어온다. 시간의 표준을 잡기 위해서 사용//
	QueryPerformanceFrequency((LARGE_INTEGER *)&m_Frequency);

}


ATimer::~ATimer()
{
}






bool	AEventTimer::Init()
{
	QueryPerformanceCounter(&m_Frame);			//고해상도 서능카운터가 있으면 카운터의 주파수를 가져온다. 시스템이 작동하는 동안 주파수를 변경되지 않기에 1초가 나타는 틱카운터를 알수 있다.

	if (m_Frame.QuadPart == 0)
	{
		return false;
	}


	m_FPS = m_Frame;
	m_iFramePerSecond = 0;	//초당프레임
	m_iFPSElapse = 0;		//초당 경과된 프레임
	m_fSPF = 0.0f;	//프레임의 경과시간

	Start();
	return true;
}
bool	AEventTimer::Frame()
{
	QueryPerformanceCounter(&m_Current);
	m_fSPF = static_cast<float>(m_Current.QuadPart - m_Frame.QuadPart) //현재시간-이전시간 = 경과된시간/이전시간?
		/ static_cast<float>(m_Frequency.QuadPart);					//이전시간			
	g_fSPF = m_fSPF;
	g_fAccTime = m_fAccTime += m_fSPF;

	////1초당 프레임체크////
	if (((m_Current.LowPart - m_FPS.LowPart) / m_Frequency.LowPart) >= 1)
	{											// 1초가 경과 되었을 떄 참이 되는 코드로써 1초가 경과되기 전에		
		m_iFramePerSecond = m_iFPSElapse;		//카운팅 되는 m_iFPSElapse 값이 이떄 m_iFramePerSecond에 저장된다
		m_iFPSElapse = 0;
		m_FPS = m_Current;
	}
	m_iFPSElapse++;

	m_Frame = m_Current;
	return true;
}