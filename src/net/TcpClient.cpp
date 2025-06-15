#include "Common.h"
#include "TcpClient.h"

#include <ws2tcpip.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

TcpClient::TcpClient(const std::string& ip, int port)
    : m_serverIp(ip), m_serverPort(port), m_socket(INVALID_SOCKET) {
}

TcpClient::~TcpClient() {
    if (m_socket != INVALID_SOCKET)
        closesocket(m_socket);
    WSACleanup();
}

bool TcpClient::Connect() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        LOG_ERR("WSAStartup failed");
        return false;
    }

    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == INVALID_SOCKET) {
        LOG_ERR("socket() failed");
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, m_serverIp.c_str(), &serverAddr.sin_addr);
    serverAddr.sin_port = htons(m_serverPort);

    if (connect(m_socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        LOG_ERR("connect() failed");
        return false;
    }

    LOG_INFO("Connected to server {}:{}", m_serverIp, m_serverPort);
    return true;
}

void TcpClient::Run() {
    std::string input;
    char buffer[1024];

    while (m_running) {
        std::cout << ">> ";
        std::getline(std::cin, input);

        if (input == "exit")
            break;

        send(m_socket, input.c_str(), static_cast<int>(input.size()), 0);

        int received = recv(m_socket, buffer, sizeof(buffer) - 1, 0);
        if (received <= 0) {
            LOG_INFO("서버 연결 종료 감지");
            break;
        }

        buffer[received] = '\0';
        LOG_INFO("서버로부터 수신: {}", buffer);
    }
}

void TcpClient::Disconnect()
{
    m_running = false;
    shutdown(m_socket, SD_BOTH);  // 강제로 recv() 빠져나오게
}