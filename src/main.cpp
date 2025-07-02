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
        LOG_ERR("���� �ʱ�ȭ ����");
        return;
    }

    std::thread serverThread([&server]() {
        server.Run();
        });

    LOG_INFO("'d'�� �Է��ϸ� ������ �����մϴ�.");
    char cmd;
    while (std::cin >> cmd) {
        if (cmd == 'd') {
            LOG_INFO("���� ���� ��û ������");
            server.Stop();
            break;
        }
    }

    serverThread.join();
}

void RunClientMode() {
    TcpClient client("127.0.0.1", 5000);
    if (!client.Connect()) {
        LOG_ERR("Ŭ���̾�Ʈ ���� ����");
        return;
    }

    std::thread clientThread([&client]() {
        client.Run();
        });

    LOG_INFO("'d'�� �Է��ϸ� Ŭ���̾�Ʈ�� �����մϴ�.");
    char cmd;
    while (std::cin >> cmd) {
        if (cmd == 'd') {
            LOG_INFO("Ŭ���̾�Ʈ ���� ��û ������");
            client.Disconnect();
            break;
        }
    }

    clientThread.join();
}

int main() {
    Logger::GetInstance().SetLogLevel(LogLevel::TRACE);

    std::cout << "[main] ����(s) �Ǵ� Ŭ���̾�Ʈ(c) ��带 �����ϼ���: ";
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
        LOG_WARN("�߸��� �Է��Դϴ�. ���α׷��� �����մϴ�.");
        break;
    }

    return 0;
}
