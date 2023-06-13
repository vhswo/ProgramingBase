#pragma once
#include <afxsock.h>
class CClient :
    public CSocket
{
private:
public:
    virtual void OnClose(int nErrorCode);
    virtual void OnReceive(int nErrorCode);
};

