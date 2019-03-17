/***************************
 * TCP_Client.cpp
 *
 * TCP demo Application
 *
 * 2019.03.15
 ***************************/

#include <stdio.h>
#include <winsock2.h>

#pragma comment (lib, "ws2_32.lib")

#define PAUSE                   system("pause")
/** IP_SERVER_TO_CONNECT: IP address to connect
 * htonl(INADDR_ANY) represents any address
 * inet_addr("127.0.0.1") represents a specific IP address */
#define IP_SERVER_TO_CONNECT    inet_addr("127.0.2.1")
// #define IP_SERVER_TO_CONNECT    htonl(INADDR_ANY)
#define PORT_SERVER             5555            // Port

int main()
{
    printf("***** 客户端 TCP Demo 程序 (Client)*****\r\n\r\n");

    WSADATA wsaData;

    // 初始化库函数
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        // 打开库失败
        printf("打开库失败。Open library failed\r\n");
        PAUSE;
        return -1;
    }
    else
    {
        printf("打开库成功。Open library success\r\n");
        printf("状态：%s。\r\n", wsaData.szSystemStatus);
    }

    // 建立套接字 socket()
    SOCKET clientSocket;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET)
    {
        // 建立套接字失败
        printf("sockect() 建立套接字失败 failed，错误码 %d\r\n", WSAGetLastError());
        WSACleanup();
        PAUSE;
        return 1;
    }
    else
    {
        printf("socket() 运行成功 success。\r\n");
    }

    // 连接客户端 connect()
    SOCKADDR_IN infoServertoconnect;
    infoServertoconnect.sin_family = AF_INET;
    infoServertoconnect.sin_addr.s_addr = IP_SERVER_TO_CONNECT;
    infoServertoconnect.sin_port = htons(PORT_SERVER);

    if (connect(clientSocket, (SOCKADDR *)&infoServertoconnect, sizeof(infoServertoconnect)) == SOCKET_ERROR)
    {
        // 接客户端 connect() 出错
        printf("connect() 连接服务器 failed，错误码 %d\r\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        PAUSE;
        return 1;
    }
    else
    {
        printf("connect() 运行成功 success。\r\n");
        printf("\r\n已连接服务器，服务器地址：%d.%d.%d.%d\r\n\r\n\n",\
        infoServertoconnect.sin_addr.s_net,infoServertoconnect.sin_addr.s_host,\
        infoServertoconnect.sin_addr.s_lh,infoServertoconnect.sin_addr.s_impno);
    }

    char sendbuf[256] = "客户端发来消息！Hi by the client!";
    char recvbuf[256] = "";

    /************* send() 发送 *************/
    int bytesSent = send(clientSocket, sendbuf, strlen(sendbuf), 0);
    if (bytesSent == SOCKET_ERROR)
    {
        printf("send() 客户端发送数据失败 failed，错误码 %d\r\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        PAUSE;
        return 1;
    }
    printf("Bytes sent：%d\r\n",bytesSent);
    printf("%s\r\n",sendbuf);
    /************* end of send() *************/

    /************* shutdown() 关闭发送 *************/
    int errShutdown = shutdown(clientSocket, SD_SEND);
    if (errShutdown == SOCKET_ERROR)
    {
        printf("shutdown() 关闭发送失败 failed，错误码 %d\r\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        PAUSE;
        return 1;
    }
    /************* end of shutdown() *************/

    /************* recv() 接收 *************/
    int bytesRecv = 0;
    while(true)
    {
        bytesRecv = recv(clientSocket, recvbuf, sizeof(recvbuf), 0);     // 一次接收长度 256
        // bytesRecv = recv(clientSocket, recvbuf, 16, 0);        //一次接收长度 16
        if (bytesRecv == 0)
        {
            printf("服务器已正常关闭发送 The server has closed connection，结束接收。\r\n");
            break;
        }
        else if (bytesRecv == SOCKET_ERROR)
        {
            printf("recv() 服务器接收数据失败 failed，错误码 %d\r\n", WSAGetLastError());
            closesocket(clientSocket);
            WSACleanup();
            PAUSE;
            return 1;
        }
        else
        {
            printf("Bytes received：%d\r\n", bytesRecv);
            printf("%s\r\n",recvbuf);
            memset(recvbuf, 0, sizeof(recvbuf));
        }

    }
    /************* end of recv() *************/

    PAUSE;
    return 0;
}