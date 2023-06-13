
// ClientSocketDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ClientSocket.h"
#include "ClientSocketDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &CAboutDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CClientSocketDlg 대화 상자



CClientSocketDlg::CClientSocketDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENTSOCKET_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientSocketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}

BEGIN_MESSAGE_MAP(CClientSocketDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CClientSocketDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CClientSocketDlg::OnBnClickedButton1)
	//ON_EN_CHANGE(IDC_EDIT1, &CClientSocketDlg::OnEnChangeEdit1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CClientSocketDlg::OnLbnSelchangeList1)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CClientSocketDlg 메시지 처리기

BOOL CClientSocketDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_Client.Create(); // 클라이언트 소켓 생성.
	m_Client.Connect(_T("127.0.0.1"), 9000); // 서버의 IP 주소와 포트 번호를 이용하여 연결시도.

	strcpy_s(m_cName, "사용하실 ID를 입력해주세요");
	m_List.AddString((CString)m_cName);


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CClientSocketDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CClientSocketDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CClientSocketDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClientSocketDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


void CClientSocketDlg::OnBnClickedButton1() // 보내기
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 여기에서 서버로 보낼 데이터 처리를 하면 된다.
	CString msg;
	GetDlgItemText(IDC_EDIT1, msg); // Edit Control에 쓴 내용은 가져온다.
	if (!msg.IsEmpty()) // 가져온 내용이 있는지 확인.
	{
		SetDlgItemText(IDC_EDIT1, _T(""));// Edit Control에 적혀있는 내용을 지운다.
		if (m_bMakeID == false)
		{
			m_List.AddString(msg); // 내용이 있다면 해당 문자를 List Box에서 출력.
			CW2A message(msg.GetString());
			strcpy_s(m_cName, sizeof(m_cName), message.m_szBuffer);
			strcat_s(m_cName, " : ");
			m_bMakeID = true;
		}
		else
		{
			// Cstring으로 받은 문자열을 아래와 같이 버퍼에 넣을 수 있다.
			char buf[sizeof(Packet)];
			CW2A message(msg.GetString());
			strcpy_s(buf, sizeof(Packet), message.m_szBuffer);
			// m_Client.Send() 함수를 이용하여 데이터를 전송할 수 있다.
			m_Client.Send(buf, sizeof(Packet));
		}
	}
}


void CClientSocketDlg::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAboutDlg::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
}

void CClientSocketDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (GetAsyncKeyState(VK_RETURN))
		OnBnClickedButton1();

	//CDialogEx::OnOK();
}
