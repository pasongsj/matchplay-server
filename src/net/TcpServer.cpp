#include "Common.h"
#include "TcpServer.h"
#include "TcpSession.h"

#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

TcpServer::TcpServer(int port)
    : m_port(port), m_running(false), m_listenSocket(INVALID_SOCKET)
{
}

TcpServer::~TcpServer()
{
    Stop();
}

bool TcpServer::Init()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        LOG_ERR("[TcpServer] WSAStartup failed.");
        return false;
    }

    m_listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_listenSocket == INVALID_SOCKET)
    {
        LOG_ERR("[TcpServer] socket() failed.");
        return false;
    }

    int opt = 1;
    setsockopt(m_listenSocket, SOL_SOCKET, SO_REUSEADDR,
        reinterpret_cast<const char*>(&opt), sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_listenSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR)
    {
        LOG_ERR("[TcpServer] bind() failed.");
        return false;
    }

    if (listen(m_listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        LOG_ERR("[TcpServer] listen() failed.");
        return false;
    }

    LOG_INFO("[TcpServer] Listening on port {}", m_port);
    return true;
}

void TcpServer::Run()
{
    m_running = true;
    m_acceptThread = std::thread(&TcpServer::AcceptLoop, this);
}

void TcpServer::Stop()
{
    m_running = false;

    if (m_listenSocket != INVALID_SOCKET)
    {
        shutdown(m_listenSocket, SD_BOTH);
        closesocket(m_listenSocket);
    }

    if (m_acceptThread.joinable())
    {
        m_acceptThread.join();
    }

    WSACleanup();
}

void TcpServer::AcceptLoop()
{
    while (m_running)
    {
        sockaddr_in clientAddr{};
        int len = sizeof(clientAddr);

        SOCKET clientSocket = accept(m_listenSocket, reinterpret_cast<sockaddr*>(&clientAddr), &len);
        if (clientSocket == INVALID_SOCKET)
        {
            LOG_ERR("[TcpServer] accept() failed.");
            continue;
        }

        LOG_INFO("[TcpServer] Accepted client socket: {}", clientSocket);

        std::thread sessionThread([](SOCKET fd)
            {
                TcpSession session(fd);
                session.Handle();
            }, clientSocket);
        sessionThread.detach();
    }
}
