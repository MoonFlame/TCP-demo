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
    printf("***** �ͻ��� TCP Demo ���� (Client)*****\r\n\r\n");

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

    // �����׽��� socket()
    SOCKET clientSocket;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET)
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

    // ���ӿͻ��� connect()
    SOCKADDR_IN infoServertoconnect;
    infoServertoconnect.sin_family = AF_INET;
    infoServertoconnect.sin_addr.s_addr = IP_SERVER_TO_CONNECT;
    infoServertoconnect.sin_port = htons(PORT_SERVER);

    if (connect(clientSocket, (SOCKADDR *)&infoServertoconnect, sizeof(infoServertoconnect)) == SOCKET_ERROR)
    {
        // �ӿͻ��� connect() ����
        printf("connect() ���ӷ����� failed�������� %d\r\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        PAUSE;
        return 1;
    }
    else
    {
        printf("connect() ���гɹ� success��\r\n");
        printf("\r\n�����ӷ���������������ַ��%d.%d.%d.%d\r\n\r\n\n",\
        infoServertoconnect.sin_addr.s_net,infoServertoconnect.sin_addr.s_host,\
        infoServertoconnect.sin_addr.s_lh,infoServertoconnect.sin_addr.s_impno);
    }

    char sendbuf[256] = "�ͻ��˷�����Ϣ��Hi by the client!";
    char recvbuf[256] = "";

    /************* send() ���� *************/
    int bytesSent = send(clientSocket, sendbuf, strlen(sendbuf), 0);
    if (bytesSent == SOCKET_ERROR)
    {
        printf("send() �ͻ��˷�������ʧ�� failed�������� %d\r\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        PAUSE;
        return 1;
    }
    printf("Bytes sent��%d\r\n",bytesSent);
    printf("%s\r\n",sendbuf);
    /************* end of send() *************/

    /************* shutdown() �رշ��� *************/
    int errShutdown = shutdown(clientSocket, SD_SEND);
    if (errShutdown == SOCKET_ERROR)
    {
        printf("shutdown() �رշ���ʧ�� failed�������� %d\r\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        PAUSE;
        return 1;
    }
    /************* end of shutdown() *************/

    /************* recv() ���� *************/
    int bytesRecv = 0;
    while(true)
    {
        bytesRecv = recv(clientSocket, recvbuf, sizeof(recvbuf), 0);     // һ�ν��ճ��� 256
        // bytesRecv = recv(clientSocket, recvbuf, 16, 0);        //һ�ν��ճ��� 16
        if (bytesRecv == 0)
        {
            printf("�������������رշ��� The server has closed connection���������ա�\r\n");
            break;
        }
        else if (bytesRecv == SOCKET_ERROR)
        {
            printf("recv() ��������������ʧ�� failed�������� %d\r\n", WSAGetLastError());
            closesocket(clientSocket);
            WSACleanup();
            PAUSE;
            return 1;
        }
        else
        {
            printf("Bytes received��%d\r\n", bytesRecv);
            printf("%s\r\n",recvbuf);
            memset(recvbuf, 0, sizeof(recvbuf));
        }

    }
    /************* end of recv() *************/

    PAUSE;
    return 0;
}