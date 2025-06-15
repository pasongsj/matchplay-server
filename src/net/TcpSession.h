#pragma once

#include <winsock2.h>

class TcpSession
{
public:
    TcpSession(SOCKET clientSocket);
    ~TcpSession();

    TcpSession(const TcpSession&) = delete;
    TcpSession& operator=(const TcpSession&) = delete;
    TcpSession(TcpSession&&) = delete;
    TcpSession& operator=(TcpSession&&) = delete;

    void Handle();

private:
    SOCKET m_clientSocket;
};
