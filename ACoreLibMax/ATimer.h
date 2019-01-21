#pragma once
#include "AStd.h"
//게임의 모드 시간관리 담당


//글로벌타이머 게임타이머 -> 멈추면 안된다.
class ATimer : public ASingleton<ATimer>
{
	friend class ASingleton<ATimer>;
public:
	LARGE_INTEGER	m_Frequency;		//초당주파수					//LARGE_INTEGER는 64비트 정수형으로 대략 1844경 정도의 틱 카운터를 표현 할수 있다
	LARGE_INTEGER	m_Current;			//현재시간						//하루 24시간에 소요되는 틱카운터는 3700조가 되고 대략 4800년 동안 틱 카운터를 할수 있다.
	LARGE_INTEGER	m_Frame;			//이전프레임체크				//장시간 켜두어야 하는 곳에 많이 사용하고 있다.
	float			m_fSPF;		//프레임의 경과시간

	LARGE_INTEGER	m_FPS;				//FPS 체크타이머
	int				m_iFramePerSecond;	//초당 프레임
	int				m_iFPSElapse;		//초당 경과된 프레임

	LARGE_INTEGER	m_Start;			//이벤트 타이머 시작시간
	LARGE_INTEGER	m_Elapse;			//이벤트 타이머 경과시간
	bool			m_bStarted;			//이벤트 타이머 작동유무
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

public:			//생성자를 풀면, 싱글톤은 싱글톤으로 쓰고, 새로 만들어서 이벤트타이머를 추가로 사용 가능
	ATimer();			
public:
	virtual ~ATimer();
};
#define I_TIME ATimer::GetInstance()


//중간에 컨트롤 할 수 있게끔 할 수 있게 해야 한다.
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