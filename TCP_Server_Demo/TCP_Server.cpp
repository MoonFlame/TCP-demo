/***************************
 * TCP_Server.cpp
 *
 * TCP demo Application
 *
 * 2019.03.14
 ***************************/

#include <stdio.h>
#include <winsock2.h>

#pragma comment (lib, "ws2_32.lib")

#define PAUSE           system("pause")
/** IP_SERVER: IP address to connect
 * htonl(INADDR_ANY) represents any address
 * inet_addr("127.0.0.1") represents a specific IP address */
// #define IP_SERVER       htonl(INADDR_ANY)
#define IP_SERVER       inet_addr("127.0.2.1")
#define PORT_SERVER     5555            // Port
#define NUM_CONNECTION  SOMAXCONN       // The maximum length of the queue of pending connections.

int main()
{
    printf("***** 服务器 TCP Demo 程序 (Server)*****\r\n\r\n");

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

    // 检查库函数版本是否为 2.2
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        // 当前使用库函数版本非 2.2
        printf("DLL 不支持 winsock %u.%u 版本。\r\n", LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));
        WSACleanup();
        PAUSE;
        return -1;
    }
    else
    {
        printf("DLL 支持 winsock 版本 %u.%u。\r\n", LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));
        printf("DLL 最高支持版本  %u.%u。\r\n", LOBYTE(wsaData.wHighVersion), HIBYTE(wsaData.wHighVersion));
    }

    // 建立套接字 socket()
    SOCKET serverSocket;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
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

    // 关联地址 bind()
    SOCKADDR_IN infoServer;
    infoServer.sin_family = AF_INET;
    infoServer.sin_addr.s_addr = IP_SERVER;
    infoServer.sin_port = htons(PORT_SERVER);

    if (bind(serverSocket, (SOCKADDR *)&infoServer, sizeof(infoServer)) == SOCKET_ERROR)
    {
        // 关联地址 bind() 出错
        printf("bind() 关联地址失败 failed，错误码 %d\r\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        PAUSE;
        return 1;
    }
    else
    {
        printf("bind() 运行成功 success。\r\n");
        printf("已建立服务器，服务器地址：%d.%d.%d.%d\r\n",\
        infoServer.sin_addr.s_net,infoServer.sin_addr.s_host,\
        infoServer.sin_addr.s_lh,infoServer.sin_addr.s_impno);
    }

    // 网络监听 listen()
    if (listen(serverSocket, NUM_CONNECTION) != 0)
    {
        // 建立监听 listen() 出错
        printf("bind() 关联地址失败 failed，错误码 %d\r\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        PAUSE;
        return 0;
    }
    else
    {
        printf("listen() 运行成功 success。\r\n");
    }


    char sendbuf[256] = "服务器向你发来了一条信息！Hello by the server!";
    char recvbuf[256] = "";

    // 循环等待连接，并发送、接收数据
    while(true)
    {
        printf("\r\n\r\n");
        printf("等待客户端连接... Waiting for connection...\r\n");

        SOCKET sock;
        do
        {
            sock = accept(serverSocket, NULL, NULL);
        }
        while (sock == SOCKET_ERROR);

        printf("客户端已连接。 Client connected\r\n");

        /************* recv() 接收 *************/
        int bytesRecv = recv(sock, recvbuf, sizeof(recvbuf), 0);     //一次接收长度 256
        // int bytesRecv = recv(sock, recvbuf, 16, 0);        //一次接收长度 16
        if (bytesRecv == 0)
        {
            printf("客户端已正常关闭发送。 The client has closed connection\r\n");
        }
        else if (bytesRecv == SOCKET_ERROR)
        {
            printf("recv() 服务器接收数据失败 failed，错误码 %d\r\n", WSAGetLastError());
            closesocket(serverSocket);
            WSACleanup();
            PAUSE;
            return 1;
        }
        else
        {
            printf("Bytes received：%d\r\n", bytesRecv);
            printf("%s\r\n\r",recvbuf);
        }
        /************* end of recv() *************/

        /************* send() 发送 *************/
        int bytesSent = send(sock, sendbuf, strlen(sendbuf), 0);
        if (bytesSent == SOCKET_ERROR)
        {
            printf("send() 服务器发送数据失败 failed，错误码 %d\r\n", WSAGetLastError());
            closesocket(serverSocket);
            WSACleanup();
            PAUSE;
            return 1;
        }
        printf("Bytes sent：%d\r\n",bytesSent);
        printf("%s\r\n\r",sendbuf);
        /************* end of send() *************/

        /************* shutdown() 关闭发送 *************/
        int errShutdown = shutdown(sock, SD_SEND);
        if (errShutdown == SOCKET_ERROR)
        {
            printf("shutdown() 关闭发送失败 failed，错误码 %d\r\n", WSAGetLastError());
            closesocket(serverSocket);
            WSACleanup();
            PAUSE;
            return 1;
        }
        /************* end of shutdown() *************/

    }

    PAUSE;
    return 0;
}