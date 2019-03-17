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
    printf("***** ������ TCP Demo ���� (Server)*****\r\n\r\n");

    WSADATA wsaData;

    // ��ʼ���⺯��
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        // �򿪿�ʧ��
        printf("�򿪿�ʧ�ܡ�Open library failed\r\n");
        PAUSE;
        return -1;
    }
    else
    {
        printf("�򿪿�ɹ���Open library success\r\n");
        printf("״̬��%s��\r\n", wsaData.szSystemStatus);
    }

    // ���⺯���汾�Ƿ�Ϊ 2.2
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        // ��ǰʹ�ÿ⺯���汾�� 2.2
        printf("DLL ��֧�� winsock %u.%u �汾��\r\n", LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));
        WSACleanup();
        PAUSE;
        return -1;
    }
    else
    {
        printf("DLL ֧�� winsock �汾 %u.%u��\r\n", LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));
        printf("DLL ���֧�ְ汾  %u.%u��\r\n", LOBYTE(wsaData.wHighVersion), HIBYTE(wsaData.wHighVersion));
    }

    // �����׽��� socket()
    SOCKET serverSocket;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        // �����׽���ʧ��
        printf("sockect() �����׽���ʧ�� failed�������� %d\r\n", WSAGetLastError());
        WSACleanup();
        PAUSE;
        return 1;
    }
    else
    {
        printf("socket() ���гɹ� success��\r\n");
    }

    // ������ַ bind()
    SOCKADDR_IN infoServer;
    infoServer.sin_family = AF_INET;
    infoServer.sin_addr.s_addr = IP_SERVER;
    infoServer.sin_port = htons(PORT_SERVER);

    if (bind(serverSocket, (SOCKADDR *)&infoServer, sizeof(infoServer)) == SOCKET_ERROR)
    {
        // ������ַ bind() ����
        printf("bind() ������ַʧ�� failed�������� %d\r\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        PAUSE;
        return 1;
    }
    else
    {
        printf("bind() ���гɹ� success��\r\n");
        printf("�ѽ�������������������ַ��%d.%d.%d.%d\r\n",\
        infoServer.sin_addr.s_net,infoServer.sin_addr.s_host,\
        infoServer.sin_addr.s_lh,infoServer.sin_addr.s_impno);
    }

    // ������� listen()
    if (listen(serverSocket, NUM_CONNECTION) != 0)
    {
        // �������� listen() ����
        printf("bind() ������ַʧ�� failed�������� %d\r\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        PAUSE;
        return 0;
    }
    else
    {
        printf("listen() ���гɹ� success��\r\n");
    }


    char sendbuf[256] = "���������㷢����һ����Ϣ��Hello by the server!";
    char recvbuf[256] = "";

    // ѭ���ȴ����ӣ������͡���������
    while(true)
    {
        printf("\r\n\r\n");
        printf("�ȴ��ͻ�������... Waiting for connection...\r\n");

        SOCKET sock;
        do
        {
            sock = accept(serverSocket, NULL, NULL);
        }
        while (sock == SOCKET_ERROR);

        printf("�ͻ��������ӡ� Client connected\r\n");

        /************* recv() ���� *************/
        int bytesRecv = recv(sock, recvbuf, sizeof(recvbuf), 0);     //һ�ν��ճ��� 256
        // int bytesRecv = recv(sock, recvbuf, 16, 0);        //һ�ν��ճ��� 16
        if (bytesRecv == 0)
        {
            printf("�ͻ����������رշ��͡� The client has closed connection\r\n");
        }
        else if (bytesRecv == SOCKET_ERROR)
        {
            printf("recv() ��������������ʧ�� failed�������� %d\r\n", WSAGetLastError());
            closesocket(serverSocket);
            WSACleanup();
            PAUSE;
            return 1;
        }
        else
        {
            printf("Bytes received��%d\r\n", bytesRecv);
            printf("%s\r\n\r",recvbuf);
        }
        /************* end of recv() *************/

        /************* send() ���� *************/
        int bytesSent = send(sock, sendbuf, strlen(sendbuf), 0);
        if (bytesSent == SOCKET_ERROR)
        {
            printf("send() ��������������ʧ�� failed�������� %d\r\n", WSAGetLastError());
            closesocket(serverSocket);
            WSACleanup();
            PAUSE;
            return 1;
        }
        printf("Bytes sent��%d\r\n",bytesSent);
        printf("%s\r\n\r",sendbuf);
        /************* end of send() *************/

        /************* shutdown() �رշ��� *************/
        int errShutdown = shutdown(sock, SD_SEND);
        if (errShutdown == SOCKET_ERROR)
        {
            printf("shutdown() �رշ���ʧ�� failed�������� %d\r\n", WSAGetLastError());
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