#include "ATimer.h"
#include <stdio.h>
#include <tchar.h>

float    g_fTimer = 0.0f;
float    g_fSPF = 0.0f;
float	 g_fAccTime = 0.0f;

bool	ATimer::Init()
{
	QueryPerformanceCounter(&m_Frame);			//���ػ� ����ī���Ͱ� ������ ī������ ���ļ��� �����´�. �ý����� �۵��ϴ� ���� ���ļ��� ������� �ʱ⿡ 1�ʰ� ��Ÿ�� ƽī���͸� �˼� �ִ�.

	if (m_Frame.QuadPart == 0)
	{
		return false;
	}


	m_FPS = m_Frame;
	m_iFramePerSecond = 0;	//�ʴ�������
	m_iFPSElapse = 0;		//�ʴ� ����� ������
	m_fSPF = 0.0f;	//�������� ����ð�

	Start();
	return true;
}
bool	ATimer::Frame()
{
	QueryPerformanceCounter(&m_Current);
	g_fSPF = m_fSPF = static_cast<float>(m_Current.QuadPart - m_Frame.QuadPart) //����ð�-�����ð� = ����Ƚð�/�����ð�?
		/ static_cast<float>(m_Frequency.QuadPart);					//�����ð�			
	//g_fSPF = m_fSPF;
	g_fAccTime = m_fAccTime += m_fSPF;

	////1�ʴ� ������üũ////
	if (((m_Current.LowPart - m_FPS.LowPart) / m_Frequency.LowPart) >= 1)
	{											// 1�ʰ� ��� �Ǿ��� �� ���� �Ǵ� �ڵ�ν� 1�ʰ� ����Ǳ� ����		
		m_iFramePerSecond = m_iFPSElapse;		//ī���� �Ǵ� m_iFPSElapse ���� �̋� m_iFramePerSecond�� ����ȴ�
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
	//�ý����� ���ļ� �������� ���´�. �ð��� ǥ���� ��� ���ؼ� ���//
	QueryPerformanceFrequency((LARGE_INTEGER *)&m_Frequency);

}


ATimer::~ATimer()
{
}






bool	AEventTimer::Init()
{
	QueryPerformanceCounter(&m_Frame);			//���ػ� ����ī���Ͱ� ������ ī������ ���ļ��� �����´�. �ý����� �۵��ϴ� ���� ���ļ��� ������� �ʱ⿡ 1�ʰ� ��Ÿ�� ƽī���͸� �˼� �ִ�.

	if (m_Frame.QuadPart == 0)
	{
		return false;
	}


	m_FPS = m_Frame;
	m_iFramePerSecond = 0;	//�ʴ�������
	m_iFPSElapse = 0;		//�ʴ� ����� ������
	m_fSPF = 0.0f;	//�������� ����ð�

	Start();
	return true;
}
bool	AEventTimer::Frame()
{
	QueryPerformanceCounter(&m_Current);
	m_fSPF = static_cast<float>(m_Current.QuadPart - m_Frame.QuadPart) //����ð�-�����ð� = ����Ƚð�/�����ð�?
		/ static_cast<float>(m_Frequency.QuadPart);					//�����ð�			
	g_fSPF = m_fSPF;
	g_fAccTime = m_fAccTime += m_fSPF;

	////1�ʴ� ������üũ////
	if (((m_Current.LowPart - m_FPS.LowPart) / m_Frequency.LowPart) >= 1)
	{											// 1�ʰ� ��� �Ǿ��� �� ���� �Ǵ� �ڵ�ν� 1�ʰ� ����Ǳ� ����		
		m_iFramePerSecond = m_iFPSElapse;		//ī���� �Ǵ� m_iFPSElapse ���� �̋� m_iFramePerSecond�� ����ȴ�
		m_iFPSElapse = 0;
		m_FPS = m_Current;
	}
	m_iFPSElapse++;

	m_Frame = m_Current;
	return true;
}