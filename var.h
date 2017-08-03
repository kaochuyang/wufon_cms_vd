#ifndef VAR_H
#define VAR_H

#define BYTE unsigned char
#define BUFFERSIZE 1024
#define MSGINSIZE  64

class var
{
    public:
        var() {}
        virtual ~var() {}
    protected:
    private:
};

typedef struct YMDHMS {
        BYTE Year;
        BYTE Month;
        BYTE Day;
        BYTE Hour;
        BYTE Min;
        BYTE Sec;
} YMDHMS;



typedef struct tsUDPMappingLCN {
        char cLocalIP[15];
        int iListenPort;
        char cRemoteIP[15];
        int iSendPort;
        int iMachineLCN;
}tsUDPMappingLCN;


typedef struct MESSAGEOK {
        int protocol;                                                           //
        BYTE packet[BUFFERSIZE];                                                //
        int packetLcn;                                                          //
        int packetLength;                                                       //
        bool cksStatus;                                                         //
        bool success;                                                           //
        int UDPorRS232;                                                         //
        int ReadorWrite;                                                        //
        int InnerOrOutWard;                                                     //

        bool bGetDLE;
        bool bGetSOH; //the same with STX
        bool bGetACK;
        bool bGetNAK;
        unsigned int uiPacketTEXTLEN;


} MESSAGEOK;

#endif // VAR_H
