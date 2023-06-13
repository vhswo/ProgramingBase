#include "pch.h"
#include "CClient.h"
#include "ClientSocketDlg.h"
#include "string"


void CClient::OnClose(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	ShutDown();
	Close();
	CSocket::OnClose(nErrorCode);

	AfxMessageBox(_T("ERROR : Disconnected From Server!!"));

}


void CClient::OnReceive(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	// �������� ���� ������ ó���� ���⼭ �ϸ� �ȴ�.
	char buf[sizeof(Packet)];
	ZeroMemory(buf, sizeof(Packet));
	if (Receive(buf, sizeof(Packet)) > 0)
	{
		// Dlg Ŭ������ ���� �ص� �ɹ� ���� ���� �����ͼ� �� �� �ִ�.
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
