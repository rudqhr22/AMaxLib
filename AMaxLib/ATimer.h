#pragma once
#include "AStd.h"
//������ ��� �ð����� ���


//�۷ι�Ÿ�̸� ����Ÿ�̸� -> ���߸� �ȵȴ�.
class ATimer : public ASingleton<ATimer>
{
	friend class ASingleton<ATimer>;
public:
	LARGE_INTEGER	m_Frequency;		//�ʴ����ļ�					//LARGE_INTEGER�� 64��Ʈ ���������� �뷫 1844�� ������ ƽ ī���͸� ǥ�� �Ҽ� �ִ�
	LARGE_INTEGER	m_Current;			//����ð�						//�Ϸ� 24�ð��� �ҿ�Ǵ� ƽī���ʹ� 3700���� �ǰ� �뷫 4800�� ���� ƽ ī���͸� �Ҽ� �ִ�.
	LARGE_INTEGER	m_Frame;			//����������üũ				//��ð� �ѵξ�� �ϴ� ���� ���� ����ϰ� �ִ�.
	float			m_fSPF;		//�������� ����ð�

	LARGE_INTEGER	m_FPS;				//FPS üũŸ�̸�
	int				m_iFramePerSecond;	//�ʴ� ������
	int				m_iFPSElapse;		//�ʴ� ����� ������

	LARGE_INTEGER	m_Start;			//�̺�Ʈ Ÿ�̸� ���۽ð�
	LARGE_INTEGER	m_Elapse;			//�̺�Ʈ Ÿ�̸� ����ð�
	bool			m_bStarted;			//�̺�Ʈ Ÿ�̸� �۵�����
	float			m_fEventTime;
	float			m_fAccTime;

	void			Reset();
	void			Start();
	void			Stop();
	bool			IsStarted() const;
	float			GetElapsedTime();

	int				GetFPS();
	float			GetSPF();

public:
	bool			Init();
	bool			Frame();
	bool			Render();
	bool			Release();

public:			//�����ڸ� Ǯ��, �̱����� �̱������� ����, ���� ���� �̺�ƮŸ�̸Ӹ� �߰��� ��� ����
	ATimer();			
public:
	virtual ~ATimer();
};
#define I_TIME ATimer::GetInstance()


//�߰��� ��Ʈ�� �� �� �ְԲ� �� �� �ְ� �ؾ� �Ѵ�.
class AEventTimer : public ATimer
{
public:
	bool Init() ;
	bool Frame() ;
	bool Render() {};
	bool Release() {};

public:
	AEventTimer();
public:
	virtual ~AEventTimer() {};
};
#define I_EVENT_TIME AEventTimer::GetInstance()