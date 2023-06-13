
// ClientSocketDlg.h: 헤더 파일
//

#pragma once
#include"CClient.h"

#pragma pack(1)
typedef struct
{
	char cName[64];
}Packet;
#pragma pack()

// CClientSocketDlg 대화 상자
class CClientSocketDlg : public CDialogEx
{
// 생성입니다.
public:
	CClientSocketDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENTSOCKET_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	char m_cName[32];
	bool m_bMakeID = false;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	CListBox m_List;
	CClient m_Client;
	afx_msg void OnLbnSelchangeList1();
	virtual void OnOK();
};
