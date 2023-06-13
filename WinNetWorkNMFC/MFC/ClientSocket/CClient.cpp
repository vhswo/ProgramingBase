#include "pch.h"
#include "CClient.h"
#include "ClientSocketDlg.h"
#include "string"


void CClient::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	ShutDown();
	Close();
	CSocket::OnClose(nErrorCode);

	AfxMessageBox(_T("ERROR : Disconnected From Server!!"));

}


void CClient::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// 서버에서 받은 데이터 처리를 여기서 하면 된다.
	char buf[sizeof(Packet)];
	ZeroMemory(buf, sizeof(Packet));
	if (Receive(buf, sizeof(Packet)) > 0)
	{
		// Dlg 클래스에 선언 해둔 맴버 변수 등을 가져와서 쓸 수 있다.
		CClientSocketDlg* pMain = (CClientSocketDlg*)AfxGetMainWnd();

		if (pMain->m_bMakeID)
		{
			char cTmp[64];
			strcpy_s(cTmp, pMain->m_cName);
			strcat_s(cTmp, buf);
			strcpy_s(buf, sizeof(Packet),cTmp);
		}

		pMain->m_List.AddString((CString)buf);
	}
	

	CSocket::OnReceive(nErrorCode);
}
