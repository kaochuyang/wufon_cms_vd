#ifndef UDP_H
#define UDP_H
#include "var.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

class UDP
{
    public:

        UDP(void);
        ~UDP(void);

        BYTE block[BUFFERSIZE];                                                 //udp socket buffer

        MESSAGEOK messageIn[MSGINSIZE];                                         //buffer

        int maxMessageIndex;                                                    //
        int lastPacketIndex;                                                    //

        int Getfd(void);                                                        //
        bool GetPortAlreadyOpen(void);                                          //
        int GetConnDevice(void);                                                //
        bool SetConnDevice(int);                                                //
        int OpenUdpSocket(char *local_ip,int listen_port,char *send_ip,int send_port);
        bool CloseUdpSocket(void);                                              //
        int  UdpRead(void);                                                     //
        bool UdpSend(BYTE *,int,char *);                                        //

    private:

        pthread_mutex_t mutexUdp;                                               //

        int udpfd;                                                              //file descript
        bool alreadyOpen;                                                       //
        int connDevice;                                                         //
        int connSubDevice;                                                      //

        struct sockaddr_in listen_addr;                                         //
        struct sockaddr_in send_addr;                                           //­

};
#endif // UDP_H
