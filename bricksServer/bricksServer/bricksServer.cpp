#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <process.h>
#include <stdlib.h>
#include <time.h>

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable:4996)

using namespace std;

#define PORT 5500
#define BUFFERSIZE 100
#define NAMESIZE 30

HANDLE mutex;
int clientNumber = 0;
unsigned threadID;
int allClientSocket[100];

class THD {
public:
    int Soc_Ser();
    static unsigned __stdcall broadcastAllClient(void* arg);
};

int THD::Soc_Ser() {
    mutex = CreateMutex(NULL, FALSE, NULL);
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    cout << "서버 소켓을 생성 했습니다." << endl;
    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    if (bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        cout << "서버 소켓을 바인드 하는데 실패했습니다." << endl;
        return 0;
    }
    if (listen(serverSocket, 5) == -1) {
        cout << "서버 소켓을 listen 모드로 설정하는데 실패했습니다" << endl;
        return 0;
    }
    cout << "채팅 서버가 실행 되었습니다." << endl;
    cout << "**********전체 유저 List***********" << endl;
    while (1) {
        struct sockaddr_in clientAddress;
        int clientAddressSize = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
        WaitForSingleObject(mutex, INFINITE);
        clientNumber++;
        allClientSocket[clientNumber - 1] = clientSocket;
        ReleaseMutex(mutex);
        char greetMessage[BUFFERSIZE];
        sprintf(greetMessage, "[서버]환영합니다. 사용자명을 입력해 주세요\n");
        send(clientSocket, greetMessage, sizeof(greetMessage), 0);
        unsigned long thread;
        thread = _beginthreadex(NULL, 0, broadcastAllClient, (void*)clientSocket, 0, &threadID);
    }
    cout << "게임을 종료 합니다" << endl;
    return 0;
}

unsigned __stdcall THD::broadcastAllClient(void* arg) {
    char fromClient[NAMESIZE + BUFFERSIZE];
    int myClientSocket = (int)arg;
    int i = 0;
    while (1) {
        int readlen = recv(myClientSocket, fromClient, sizeof(fromClient), 0);
        if (readlen <= 0) break;
        cout << fromClient << endl;
        WaitForSingleObject(mutex, INFINITE);
        for (i = 0; i < clientNumber; i++) {
            if (allClientSocket[i] != myClientSocket) {
                send(allClientSocket[i], fromClient, sizeof(fromClient), 0);
            }
        }
        ReleaseMutex(mutex);
    }
    // 클라이언트 접속 종료 시 처리
    WaitForSingleObject(mutex, INFINITE);
    for (i = 0; i < clientNumber; i++) {
        if (allClientSocket[i] == myClientSocket) {
            for (; i < clientNumber - 1; i++) {
                allClientSocket[i] = allClientSocket[i + 1];
            }
            break;
        }
    }
    clientNumber--;
    ReleaseMutex(mutex);
    closesocket(myClientSocket);
    return 0;
}

int main() {
    THD soc;
    soc.Soc_Ser();
}
