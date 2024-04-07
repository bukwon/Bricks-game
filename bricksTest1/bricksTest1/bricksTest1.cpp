#include <iostream>
#include <winsock2.h>
#include <Windows.h>
#include <time.h>
#include <process.h>
#include <stdlib.h>
#include <conio.h>

#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)

using namespace std;

#define PORT 5500
#define BUFFERSIZE 100
#define NAMESIZE 30

unsigned sendThread;

// 게임 상태를 담는 구조체
struct GameState {
    int map[12][6];
    int ballx;
    int bally;
    int brick_num;
};

class Brick1
{
private:
    int map[12][6];
};

class Brick2 :public Brick1
{
private:
    int map[12][6];
    int ballx = 2;
    int bally = 10;
    int brick_edge = 0;
    int brick_num = 18;
public:
    Brick2()
    {
        srand(time(NULL));
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                if (i == 0 || i == 1 || i == 2)
                    map[i][j] = rand() % 3;
                else
                    map[i][j] = 3; //나머진 비공간으로 채우기
                map[bally][ballx] = 4;//공
            }
        }
    }
    ~Brick2()
    {
        system("cls");
        printf("\n게임을 종료합니다. \n");
    }
    void show_map();
    void game_start();
    void start_game();

    void Move_up();
    void Up_straight();
    void Up_right();
    void Up_left();

    void Move_down();
    void Down_right();
    void Down_left();
    void Down_straight();
};

void Brick2::start_game()
{
    system("cls");
    printf("게임을 시작합니다.\n");
    game_start();
}

void Brick2::game_start()
{
    while (1)
    {
        int input = _getch();
        if (input == ' ')
        {
            Move_up();
        }
        else
        {
            return;
        }
    }
}

void Brick2::Move_down()
{
    if (brick_num == 0)
        return;
    int random = rand() % 3;
    if (random == 0)
        Down_straight();
    else if (random == 1)
        Down_left();
    else if (random == 2)
        Down_right();
}

void Brick2::Down_straight()
{
    int m; //바로 이전의 ballx
    int n; //바로 이전의 bally
    while (1)
    {
        Sleep(100);
        system("cls");
        m = ballx;
        n = bally;
        map[n][m] = 3; //이전에 있던 공위치 빈공간으로 초기화
        bally += 1;
        map[bally][ballx] = 4; //다음위치 설정시 공 표시
        show_map();
        if (bally == 10)
            break;
    }
    if (ballx == 0 && bally == 10)
    {
        int random_edge = rand() % 2;
        if (random_edge == 0)
            Up_straight();
        else if (random_edge == 1)
            Up_right();
    }
    else if (ballx == 5 && bally == 10)
    {
        int random_edge = rand() % 2;
        if (random_edge == 0)
            Up_straight();
        else if (random_edge == 1)
            Up_left();
    }
    Move_up();
}

void Brick2::Down_left()
{
    int n, m;//직전에 있던 공 초기화
    while (1)
    {
        Sleep(100);
        system("cls");
        m = ballx;
        n = bally;
        map[n][m] = 3;
        ballx -= 1;
        bally += 1;
        map[bally][ballx] = 4;
        show_map();
        if (ballx == 0)
            break;
        if (bally == 10)
            break;
        if (map[bally][ballx - 1] < 3)
            break;
        if (map[bally + 1][ballx - 1] < 3)
            break;
    }
    if (map[bally][ballx + 1] < 3 && map[bally][ballx - 1] < 3)
        Down_straight();
    else if (map[bally][ballx - 1] < 3)
        Down_right();
    if (ballx == 0 && bally == 10)
    {
        int random_edge = rand() % 2;
        if (random_edge == 0)
            Up_straight();
        else if (random_edge == 1)
            Up_right();
    }
    else if (ballx == 5 && bally == 10)
    {
        int random_edge = rand() % 2;
        if (random_edge == 0)
            Up_straight();
        else if (random_edge == 1)
            Up_left();
    }
    if (ballx == 0)
        Down_right();
    Move_up();
}

void Brick2::Down_right()
{
    int n, m;//직전에 있던 공 초기화
    while (1)
    {
        Sleep(100);
        system("cls");
        m = ballx;
        n = bally;
        map[n][m] = 3;
        ballx += 1;
        bally += 1;
        map[bally][ballx] = 4;
        show_map();
        if (ballx == 5)
            break;
        if (bally == 10)
            break;
        if (map[bally][ballx + 1] < 3)
            break;
        if (map[bally + 1][ballx + 1] < 3)
            break;
    }
    if (map[bally][ballx + 1] < 3 && map[bally][ballx - 1] < 3)
        Down_straight();
    else if (map[bally][ballx + 1] < 3)
        Down_left();
    if (ballx == 5 && bally == 10)
    {
        int random_edge = rand() % 2;
        if (random_edge == 0)
            Up_straight();
        else if (random_edge == 1)
            Up_left();
    }
    else if (ballx == 0 && bally == 10)
    {
        int random_edge = rand() % 2;
        if (random_edge == 0)
            Up_straight();
        else if (random_edge == 1)
            Up_right();
    }
    else if (ballx == 5)
        Down_left();
    Move_up();
}

void Brick2::Up_straight()
{
    int m; //바로 이전의 ballx
    int n; //바로 이전의 bally
    while (1)
    {
        Sleep(100);
        system("cls");
        m = ballx;
        n = bally;
        map[n][m] = 3; //이전에 있던 공위치 빈공간으로 초기화
        bally -= 1;
        if (bally < 0)
            bally += 1;
        map[bally][ballx] = 4; //다음위치 설정시 공 표시
        show_map();
        if (bally == 0)
            break;
        if (map[bally - 1][ballx] < 3)
        {
            map[bally - 1][ballx] = 3;
            brick_num -= 1;
            break;
        }
    }
    if (brick_num == 0)
        return;
    if (map[bally][ballx + 1] < 3 && map[bally][ballx - 1] < 3)
        Down_straight();
    if (ballx == 0 && bally == 0)
    {
        int random_edge = rand() % 2;
        if (random_edge == 0)
            Down_straight();
        else if (random_edge == 1)
            Down_right();
    }
    else if (ballx == 5 && bally == 0)
    {
        int random_edge = rand() % 2;
        if (random_edge == 0)
            Down_straight();
        else if (random_edge == 1)
            Down_left();
    }
    if (ballx == 5)
    {
        int random_edge = rand() % 2;
        if (random_edge == 0)
            Down_straight();
        else if (random_edge == 1)
            Down_left();
    }
    if (ballx == 0)
    {
        int random_edge = rand() % 2;
        if (random_edge == 0)
            Down_straight();
        else if (random_edge == 1)
            Down_right();
    }
    Move_down();
}

void Brick2::Up_right()
{
    int n, m;//직전에 있던 공 초기화
    while (1)
    {
        Sleep(100);
        system("cls");
        m = ballx;
        n = bally;
        map[n][m] = 3;
        ballx += 1;
        bally -= 1;
        map[bally][ballx] = 4;
        show_map();
        if (ballx == 5)
            break;
        if (ballx == 5 && map[bally][ballx - 1] < 3)
            break;
        if (map[bally][ballx + 1] < 3)
        {
            if (map[bally - 1][ballx] < 3)
            {
                map[bally - 1][ballx] = 3;
                brick_num -= 1;
            }
            break;
        }
        if (map[bally - 1][ballx] < 3 && map[bally][ballx - 1] == 3)
        {
            map[bally - 1][ballx] = 3;
            brick_num -= 1;
            break;
        }
        if (map[bally - 1][ballx + 1] < 3 || map[bally][ballx + 1] < 3)
            break;
    }
    if (map[ballx + 1][bally + 1] < 3)
    {
        int random = rand() % 2;
        if (random == 0)
            Down_straight();
        else if (random == 1)
            Down_left();
    }
    if (ballx == 5 && map[bally - 1][ballx] < 3)
    {
        map[bally - 1][ballx] = 3;
        brick_num -= 1;
        Down_straight();
    }
    if (ballx == 5 && bally == 0)
    {
        int random_edge = rand() % 2;
        if (random_edge == 0)
            Down_straight();
        else if (random_edge == 1)
            Down_left();
    }
    if (ballx == 5)
        Up_left();
    else if (ballx == 0 && bally == 0)
    {
        int random_edge = rand() % 2;
        if (random_edge == 0)
            Down_straight();
        else if (random_edge == 1)
            Down_right();
    }
    Move_down();
}

void Brick2::Up_left()
{
    int n, m;//직전에 있던 공 초기화
    while (1)
    {
        Sleep(100);
        system("cls");
        m = ballx;
        n = bally;
        map[n][m] = 3;
        ballx -= 1;
        bally -= 1;
        map[bally][ballx] = 4;
        show_map();
        if (ballx == 0)
        {
            break;
        }
        if (bally == 0)
            break;
        if (ballx == 0 && map[bally - 1][ballx] < 3)
        {
            map[bally - 1][ballx] = 3;
            break;
        }
        if (map[bally - 1][ballx] < 3 && map[bally][ballx + 1] == 3)
        {
            map[bally - 1][ballx] = 3;
            brick_num -= 1;
            break;
        }
        if (map[bally][ballx - 1] < 3 && map[bally - 1][ballx - 1] < 3)
        {
            break;
        }
        if (map[bally - 1][ballx - 1] < 3)
            break;
    }
    if (map[bally][ballx - 1] < 3 || map[bally - 1][ballx - 1] < 3)
    {
        int random = rand() % 2;
        if (random == 0)
            Down_straight();
        else if (random == 1)
            Down_right();
    }
    if (ballx == 0 && map[bally - 1][ballx] < 3)
    {
        map[bally - 1][ballx] = 3;
        brick_num -= 1;
        Down_straight();
    }
    if (ballx == 0 && bally == 0)
    {
        if (map[bally][ballx + 1] < 3 || map[bally + 1][ballx + 1] < 3)
            Down_straight();
        Down_right();
    }
    if (ballx == 0)
        Up_right();
    else if (map[bally][ballx - 1] < 3 && map[bally][ballx + 1] < 3)
        Down_straight();
    Move_down();
}

void Brick2::Move_up()
{
    if (brick_num == 0)
        return;
    int random = rand() % 3;
    if (random == 0)
        Up_straight();
    else if (random == 1)
        Up_left();
    else if (random == 2)
        Up_right();
}

void Brick2::show_map()
{
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (map[i][j] == 0)
                printf("■");
            else if (map[i][j] == 1)
                printf("★");
            else if (map[i][j] == 2)
                printf("▲");
            else if (map[i][j] == 3)
                printf("  ");
            else if (map[i][j] == 4)
                printf("●");
        }
        printf("\n");
    }
}

class THD {
public:
    int Soc_Cli();
    static unsigned __stdcall sendMessage(void* arg);
};

int THD::Soc_Cli()
{
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);

    //클라이언트 소켓 생성
    int mySocket = socket(PF_INET, SOCK_STREAM, 0);
    if (mySocket == -1) {
        cout << "클라이언트 소켓을 생성하는데 실패 했습니다" << endl;
        return 0;
    }

    //서버 접속
    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    if (connect(mySocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        cout << "서버와 연결하는데 실패했습니다" << endl;
        return 0;
    }

    //환영 메시지
    char greetMessage[BUFFERSIZE];
    recv(mySocket, greetMessage, sizeof(greetMessage), 0);
    cout << greetMessage << endl;

    unsigned long hSendThread = _beginthreadex(NULL, 0, sendMessage, (void*)mySocket, 0, &sendThread); // 키보드 입력
    WaitForSingleObject((HANDLE)hSendThread, INFINITE);
}

unsigned __stdcall THD::sendMessage(void* arg)
{
    int mySocket = (int)arg;
    char fromClientInput[BUFFERSIZE];
    char myName[NAMESIZE];
    char toServer[NAMESIZE + BUFFERSIZE];
    cout << "사용자명을 입력하세요: ";
    cin.getline(myName, NAMESIZE);
    sprintf(toServer, "%s 님이 입장하셨습니다.", myName);
    send(mySocket, toServer, sizeof(toServer), 0);

    // 게임 시작 메시지 전송
    sprintf(toServer, "%s 님의 게임을 시작합니다.", myName);
    send(mySocket, toServer, sizeof(toServer), 0);

    // 게임 동기화
    Brick2 b1;
    b1.start_game();

    return 0;
}

int main(void)
{
    srand((unsigned int)time(NULL));
    THD cli;
    cli.Soc_Cli();
    system("pause");
    return 0;
}