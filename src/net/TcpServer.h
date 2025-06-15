#pragma once

#include <thread>
#include <atomic>
#include <winsock2.h>

class TcpServer
{
public:
    TcpServer(int port);
    ~TcpServer();

    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;
    TcpServer(TcpServer&&) = delete;
    TcpServer& operator=(TcpServer&&) = delete;

    bool Init();
    void Run();
    void Stop();

private:
    void AcceptLoop();

private:
    int m_port;
    std::atomic<bool> m_running;
    std::thread m_acceptThread;
    SOCKET m_listenSocket;
};
