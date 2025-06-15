#include "Common.h"
#include "TcpSession.h"

#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

TcpSession::TcpSession(SOCKET clientSocket)
    : m_clientSocket(clientSocket)
{
}

TcpSession::~TcpSession()
{
    if (m_clientSocket != INVALID_SOCKET)
        closesocket(m_clientSocket);
}

void TcpSession::Handle()
{
    char buffer[1024];

    while (true)
    {
        int n = recv(m_clientSocket, buffer, sizeof(buffer), 0);
        if (n <= 0) {
            LOG_INFO("[TcpSession] Client disconnected.");
            break;
        }

        buffer[n] = '\0';
        LOG_DEBUG("[TcpSession] Received: {}", buffer);

        send(m_clientSocket, buffer, n, 0);  // Echo back
    }
}
