#include "Common.h"
#include <net/TcpClient.h>
#include <net/TcpServer.h>
#include <utils/logger/Logger.h>
#include <iostream>
#include <string>
#include <thread>

void RunServerMode() {
    TcpServer server(5000);
    if (!server.Init()) {
        LOG_ERR("서버 초기화 실패");
        return;
    }

    std::thread serverThread([&server]() {
        server.Run();
        });

    LOG_INFO("'d'를 입력하면 서버를 종료합니다.");
    char cmd;
    while (std::cin >> cmd) {
        if (cmd == 'd') {
            LOG_INFO("서버 종료 요청 감지됨");
            server.Stop();
            break;
        }
    }

    serverThread.join();
}

void RunClientMode() {
    TcpClient client("127.0.0.1", 5000);
    if (!client.Connect()) {
        LOG_ERR("클라이언트 연결 실패");
        return;
    }

    std::thread clientThread([&client]() {
        client.Run();
        });

    LOG_INFO("'d'를 입력하면 클라이언트를 종료합니다.");
    char cmd;
    while (std::cin >> cmd) {
        if (cmd == 'd') {
            LOG_INFO("클라이언트 종료 요청 감지됨");
            client.Disconnect();
            break;
        }
    }

    clientThread.join();
}

int main() {
    Logger::GetInstance().SetLogLevel(LogLevel::TRACE);

    std::cout << "[main] 서버(s) 또는 클라이언트(c) 모드를 선택하세요: ";
    char mode;
    std::cin >> mode;
    std::cin.ignore();

    switch (mode) {
    case 's':
        RunServerMode();
        break;
    case 'c':
        RunClientMode();
        break;
    default:
        LOG_WARN("잘못된 입력입니다. 프로그램을 종료합니다.");
        break;
    }

    return 0;
}
