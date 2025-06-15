#pragma once
#include <string>
#include <winsock2.h>

class TcpClient {
public:
    TcpClient(const std::string& ip, int port);
    ~TcpClient();

    bool Connect();
    void Run();
    void Disconnect();

private:
    std::string m_serverIp;
    int m_serverPort;
    SOCKET m_socket;
    bool m_running = true;
};
