#ifndef VARIABLE_H
#define VARIABLE_H
#define BYTE unsigned char

#define HI(w) ((BYTE) (((unsigned short int) (w) >> 8) & 0xFF))                 //ï¿½DHI byte
#define LO(w) ((BYTE) (w))                                                      //ï¿½DLO byte
#define BUFFERSIZE 1024
class variable
{
    public:
        variable() {}
        virtual ~variable() {}
    protected:
    private:
};

typedef struct MESSAGEOK {
        int protocol;                                                           //ï¿½Ï¥Îªï¿½ï¿½qï¿½Tï¿½ï¿½wï¿½ï¿½ï¿½ï¿½,ï¿½Ð°Ñ¦ï¿½var.hï¿½wï¿½qï¿½iï¿½àªºï¿½ï¿½w
        BYTE packet[BUFFERSIZE];                                                //ï¿½ï¿½Ú¨ï¿½wï¿½Ê¥]ï¿½ï¿½ï¿½e
        int packetLcn;                                                          //ï¿½ï¿½ï¿½ï¿½wï¿½ï¿½ï¿½ï¿½fï¿½sï¿½ï¿½,ï¿½uï¿½Aï¿½Î©ï¿½PTRAFFIC(ï¿½ï¿½qï¿½ï¿½ï¿½qï¿½Tï¿½ï¿½w)ï¿½Î¥ÃµaCMS...ï¿½ï¿½
        int packetLength;                                                       //ï¿½ï¿½ï¿½ï¿½wï¿½ï¿½ï¿\uFFFD
        bool cksStatus;                                                         //ï¿½Pï¿½Oï¿½ï¿½ï¿½Ê¥]ï¿½ï¿½checkSumï¿½ï¿½ï¿½Tï¿½Pï¿½_,ï¿½iï¿½aï¿½ï¿½
        bool success;                                                           //ï¿½Pï¿½Oï¿½ï¿½ï¿½Ê¥]ï¿½ï¿½ï¿½ï¿½wï¿½ï¿½×µï¿½ï¿½ï¿½Lï¿½Ô­ï¿½ï¿½ï¿½ï¿½Pï¿½O,ï¿½iï¿½aï¿½ï¿½
        int UDPorRS232;                                                         //ï¿½ï¿½ï¿½Ê¥]ï¿½Ó¦Û¦ï¿½B
        int ReadorWrite;                                                        //ï¿½Mï¿½wï¿½ï¿½ï¿½Ê¥]ï¿½OÅªï¿½Jï¿½ï¿½ï¿½Î¼gï¿½Xï¿½ï¿½ï¿½Ê¥] read or write
        int InnerOrOutWard;                                                     //ï¿½ï¿½ï¿½ï¿½ï¿½Ç¿ï¿½ï¿½Ù¬Oï¿½zï¿½LI/O mtyu

        bool bGetDLE;
        bool bGetSOH; //the same with STX
        bool bGetACK;
        bool bGetNAK;
        unsigned int uiPacketTEXTLEN;


} MESSAGEOK;


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

#endif // VARIABLE_H
