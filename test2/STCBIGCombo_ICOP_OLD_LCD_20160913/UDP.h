//---------------------------------------------------------------------------
#ifndef UDPH
#define UDPH

#include "var.h"
#include "IODEVICE.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

//---------------------------------------------------------------------------
class UDP:public IODEVICE
{
    public:

        UDP(void);
        ~UDP(void);

        BYTE block[BUFFERSIZE];                                                 //udp socket buffer

        MESSAGEOK messageIn[MSGINSIZE];                                         //�����ѪR���\��buffer

        int maxMessageIndex;                                                    //�����B�z��messageIn�ʥ]�Ӽ�
        int lastPacketIndex;                                                    //�W�@�����̫�@��messageIn.packet[Index]

        int Getfd(void);                                                        //��ofile description
        bool GetPortAlreadyOpen(void);                                          //�P�_����O�_�w�}��
        int GetConnDevice(void);                                                //��o����ҳs�����]��
        bool SetConnDevice(int);                                                //�]�w����ҳs�����]��
        int OpenUdpSocket(char *,int,char *,int);                               //�}�q�T��(localhost,listen port,remotehost,remote port)
        bool CloseUdpSocket(void);                                              //���q�T��
        int  UdpRead(void);                                                     //Ū�q�T��(read buffer)
        bool UdpSend(BYTE *,int,char *);                                        //�g�q�T��(write array,write length,device name)

    private:

        pthread_mutex_t mutexUdp;                                               //�O�@�O����

        int udpfd;                                                              //file descript
        bool alreadyOpen;                                                       //��port�O�_�w�}��
        int connDevice;                                                         //����s�����]��
        int connSubDevice;                                                      //����s�����]�Ʃҳs�����l�]��

        struct sockaddr_in listen_addr;                                         //�n��ť��port
        struct sockaddr_in send_addr;                                           //�n�e�X��port(�s���o�e�������P�e�X���𤣦P�~���|�S���^��ۤv�e��)

};
//---------------------------------------------------------------------------
#endif
