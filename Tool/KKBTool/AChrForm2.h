#pragma once


// AChrForm2 대화 상자

//static AChrForm* instance;
//
//private:
//
//	static AChrForm* GetInstance()
//	{
//		if (instance == NULL)
//		{
//			instance = new AChrForm();
//			instance->m_hWnd = g_hWnd; //->GetSafeHwnd();
//		}
//		return instance;
//	}
//
//	AChrForm();
//	// 표준 생성자입니다.
//	//AChrForm(const AChrForm& other);
//public:
//	static AChrForm* CreateOne(CWnd* pParent);
//	virtual ~AChrForm();



class AChrForm2 : public CFormView
{
	DECLARE_DYNAMIC(AChrForm2)

	int a = 10;

	static AChrForm2* instance;

public:

	static AChrForm2* GetInstance()
	{
		return instance;
	}
private:
	AChrForm2();
	//AChrForm2(const AChrForm& other);
public:
	static AChrForm2* CreateOne(CWnd* pParent);
	virtual ~AChrForm2();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif
	void RecursiveFunction(HTREEITEM hItem);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_TreeList;
};



//#define TVI_ROOT                ((HTREEITEM)(ULONG_PTR)-0x10000)
//#define TVI_FIRST               ((HTREEITEM)(ULONG_PTR)-0x0FFFF)
//#define TVI_LAST                ((HTREEITEM)(ULONG_PTR)-0x0FFFE)
//#define TVI_SORT                ((HTREEITEM)(ULONG_PTR)-0x0FFFD)