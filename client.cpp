#include <iostream>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <string>
#include <windows.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable: 4996)  // подавление предупреждения 4996

using namespace std;

#define SRV_HOST "localhost"
#define SRV_PORT 1234
#define CLNT_PORT 1235
#define BUF_SIZE 64

char TXT_ANSW[] = "I am your student\n";

int main() {
    char buff[1024];

    // Инициализация библиотеки Winsock
    if (WSAStartup(MAKEWORD(2, 2), (WSADATA*)&buff[0])) {
        cout << "Error WSAStartup \n" << WSAGetLastError();  // Ошибка!
        return -1;
    }

    SOCKET s;
    int from_len;
    char buf[BUF_SIZE] = { 0 };
    hostent* hp;
    sockaddr_in clnt_sin, srv_sin;

    // Создание сокета
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        cout << "Socket creation failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }

    // Настройка клиентского адреса
    clnt_sin.sin_family = AF_INET;
    clnt_sin.sin_addr.s_addr = INADDR_ANY; // Принять любые входящие адреса
    clnt_sin.sin_port = htons(CLNT_PORT);

    // Привязка сокета к клиентскому адресу
    if (bind(s, (sockaddr*)&clnt_sin, sizeof(clnt_sin)) == SOCKET_ERROR) {
        cout << "Bind failed: " << WSAGetLastError() << endl;
        closesocket(s);
        WSACleanup();
        return -1;
    }

    // Получение адреса сервера
    hp = gethostbyname(SRV_HOST);
    if (hp == nullptr) {
        cout << "gethostbyname failed: " << WSAGetLastError() << endl;
        closesocket(s);
        WSACleanup();
        return -1;
    }

    srv_sin.sin_port = htons(SRV_PORT);
    srv_sin.sin_family = AF_INET;
    srv_sin.sin_addr.s_addr = *(reinterpret_cast<unsigned long*>(hp->h_addr_list[0]));

    // Подключение к серверу
    if (connect(s, (sockaddr*)&srv_sin, sizeof(srv_sin)) == SOCKET_ERROR) {
        cout << "Connect failed: " << WSAGetLastError() << endl;
        closesocket(s);
        WSACleanup();
        return -1;
    }

    string mst;
    do {
        
        from_len = recv(s, buf, BUF_SIZE, 0);
        if (from_len < 0) {
            cout << "Receive failed: " << WSAGetLastError() << endl;
            break; // выйти из цикла при ошибке
        }
        buf[from_len] = '\0';
        cout << buf << endl;

        getline(cin, mst);
        if (!mst.empty()) {
            send(s, mst.c_str(), mst.size(), 0);;
            cout << "Sended" << endl;
        }

    } while (mst != "Bye");

    cout << "exit to infinity" << endl;
    cin.get();
    closesocket(s);
    WSACleanup();
    return 0;
}
