#ifndef VARIABLE_H
#define VARIABLE_H
#define BYTE unsigned char

#define HI(w) ((BYTE) (((unsigned short int) (w) >> 8) & 0xFF))                 //ï¿½DHI byte
#define LO(w) ((BYTE) (w))                                                      //ï¿½DLO byte
#define BUFFERSIZE 1024
#define MSGINSIZE 64
#define cWRITE                81                                                //ï¿½gï¿½X
#define cREAD                 82                                                //Åªï¿½J
#define cRS232                83                                                //RS232
#define cUDP                  84                                                //UDP
#define PROTOCOL92           203

#define cHEX                  31                                                //16ï¿½iï¿½ï¿½sï¿½X
#define cBCD                  32

#define DEVICECENTER92       103
#define DEVICEUNKNOW          99                                                //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½Fï¿½F
#define DEVICEMUTILPROTOCOL  100                                                //ï¿½iï¿½à¦³ï¿½hï¿½Ø¨ï¿½wï¿½qï¿½oï¿½ï¿½portï¿½iï¿½ï¿½
#define DEVICECENTER77       101                                                //77ï¿½~ï¿½ï¿½ï¿½æ±±ï¿½ï¿½ï¿½ï¿½
#define DEVICECENTER87       102                                                //87ï¿½~ï¿½ï¿½ï¿½æ±±ï¿½ï¿½ï¿½ï¿½
#define DEVICECENTER92       103                                                //92ï¿½~ï¿½ï¿½ï¿½æ±±ï¿½ï¿½ï¿½ï¿½
#define DEVICECENTERNTCIP    104                                                //NTCIPï¿½~ï¿½ï¿½ï¿½æ±±ï¿½ï¿½ï¿½ï¿½
#define DEVICETESTER77       105                                                //77ï¿½~ï¿½ï¿½ï¿½{ï¿½ï¿½â´\uFFFD¿½ï¿½Õ¾ï¿½
#define DEVICETESTER87       106                                                //87ï¿½~ï¿½ï¿½ï¿½{ï¿½ï¿½â´\uFFFD¿½ï¿½Õ¾ï¿½
#define DEVICETESTER92       107                                                //92ï¿½~ï¿½ï¿½ï¿½{ï¿½ï¿½â´\uFFFD¿½ï¿½Õ¾ï¿½
#define DEVICETESTERNTCIP    108                                                //NTCIPï¿½~ï¿½ï¿½ï¿½{ï¿½ï¿½â´\uFFFD¿½ï¿½Õ¾ï¿½
#define DEVICE77TC           109                                                //77ï¿½~ï¿½ï¿½ï¿½ï¿½ï¿½xï¿½ï¿½ï¿½î¾¹
#define DEVICE87TC           110                                                //87ï¿½~ï¿½ï¿½ï¿½ï¿½ï¿½xï¿½ï¿½ï¿½î¾¹
#define DEVICE87CMS          111                                                //87ï¿½~ï¿½ï¿½ï¿½ï¿½Tï¿½iï¿½Ü±ï¿½ï¿½î¾¹
#define DEVICE87VD           112                                                //87ï¿½~ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½î¾¹
#define DEVICE92TC           113                                                //92ï¿½~ï¿½ï¿½ï¿½ï¿½ï¿½xï¿½ï¿½ï¿½î¾¹
#define DEVICE92CMS          114                                                //92ï¿½~ï¿½ï¿½ï¿½ï¿½Tï¿½iï¿½Ü±ï¿½ï¿½î¾¹
#define DEVICE92VD           115                                                //92ï¿½~ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½î¾¹
#define DEVICENTCIPTC        116                                                //NTCIPï¿½ï¿½ï¿½xï¿½ï¿½ï¿½î¾¹
#define DEVICENTCIPCMS       117                                                //NTCIPï¿½ï¿½Tï¿½iï¿½Ü±ï¿½ï¿½î¾¹
#define DEVICENTCIPVD        118                                                //NTCIPï¿½ï¿½ï¿½ï¿½ï¿½î¾¹
#define DEVICEKEYPAD         119                                                //ï¿½Fï¿½Í­ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½ï¿½ï¿½L
#define DEVICERTMS           120                                                //RTMS
#define DEVICESS             121                                                //SMART SENSOR
#define DEVICEOWNWAY         122                                                //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Oï¿½Ë¼ï¿½
#define DEVICEEOI            123                                                //ï¿½ï¿½Å¬ï¿½Oï¿½Ë¼ï¿½
#define DEVICELISCMS         124                                                //ï¿½xï¿½_ï¿½ÃµaCMS
/*OTCombo0713*/
#define DEVICETRAFFICLIGHT   125                                                //ï¿½Øµ{ï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½O
#define DEVICEREDCOUNTVER94  126                                                //ï¿½ï¿½Oï¿½Ë¼Æ¸Ë¸m (ï¿½ï¿½ï¿\uFFFD
#define DEVICEREDCOUNTVERHK  127                                                //ï¿½ï¿½Oï¿½Ë¼Æ¸Ë¸m (ï¿½ï¿½ï¿½ï¿½)
#define DEVICEGPS            128
#define DEVICETAINANPEOPLELIGHT 129

#define DEVICECCJDYNCTL      130
#define DEVICEREDCOUNTVERCCT97  131
#define DEVICEREVMANUALPAD   132

#define DEVICEREVSYNC   133
#define DEVICEAMEGIDS   134
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------   //ï¿½wï¿½qï¿½iï¿½àªºï¿½ï¿½w
#define PROTOCOLUNKNOW       200                                                //ï¿½ï¿½ï¿½Mï¿½ï¿½ï¿½ï¿½ï¿½ï¿½w
#define PROTOCOL77           201                                                //ï¿½ï¿½qï¿½ï¿½77ï¿½~ï¿½ï¿½ï¿½qï¿½Tï¿½ï¿½w
#define PROTOCOL87           202                                                //ï¿½ï¿½qï¿½ï¿½87ï¿½~ï¿½ï¿½ï¿½qï¿½Tï¿½ï¿½w
#define PROTOCOL92           203                                                //ï¿½ï¿½qï¿½ï¿½92ï¿½~ï¿½ï¿½ï¿½qï¿½Tï¿½ï¿½w
#define PROTOCOLNTCIP        204                                                //ï¿½ï¿½qï¿½ï¿½NTCIPï¿½qï¿½Tï¿½ï¿½w
#define PROTOCOLKEYPAD       205                                                //ï¿½Fï¿½Í­ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Lï¿½qï¿½Tï¿½ï¿½w
#define PROTOCOLRTMS         206                                                //RTMSï¿½qï¿½Tï¿½ï¿½w
#define PROTOCOLSS           207                                                //SMART SENSORï¿½qï¿½Tï¿½ï¿½w
#define PROTOCOLOWNWAY       208                                                //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Oï¿½Ë¼Æ³qï¿½Tï¿½ï¿½w
#define PROTOCOLEOI          209                                                //ï¿½ï¿½Å¬ï¿½Oï¿½Ë¼Æ³qï¿½Tï¿½ï¿½w
#define PROTOCOLLISCMS       210                                                //ï¿½xï¿½_ï¿½ÃµaCMSï¿½qï¿½Tï¿½ï¿½w
#define PROTOCOLTRAFFICLIGHT 211
#define PROTOCOLREDCOUNTVER94 212                                               //94ï¿½~ï¿½ï¿½Oï¿½Ë¼ï¿½
#define PROTOCOLREDCOUNTVERHK 213                                               //93ï¿½~ï¿½ï¿½Oï¿½Ë¼ï¿½ (ï¿½ï¿½ï¿½ï¿½)

#define PROTOCOLCCJ        214
#define PROTOCOLREDCOUNTVERCCT97  215
#define PROTOCOLREVMANUALPAD  216

#define PROTOCOLREVSYNC      217
#define PROTOCOLAMEGIDS      218

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------   //ï¿½wï¿½qï¿½iï¿½àªºLCDï¿½eï¿½ï¿½
#define cNONE                300                                                //ï¿½Å¥Õ­ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½Oï¿½@)
#define cLOGO                301                                                //ï¿½ï¿½ï¿½ï¿½LOGO
#define cLOGIN               302                                                //ï¿½nï¿½Jï¿½eï¿½ï¿½
#define cLOGINERR            303                                                //ï¿½nï¿½Jï¿½ï¿½~ï¿½eï¿½ï¿½
#define cMAIN                304                                                //ï¿½Dï¿½ï¿½ï¿\uFFFD
#define cCTLSETUP            305                                                //ï¿½ï¿½ï¿½î¾¹ï¿½]ï¿½wï¿½Dï¿½ï¿½ï¿\uFFFD
#define cCTLNO               306                                                //ï¿½]ï¿½Æ½sï¿½ï¿½
#define cLASTDOWN            307                                                //ï¿½Wï¿½ï¿½ï¿½_ï¿½qï¿½É¶ï¿½
#define cMODIFYDT            308                                                //ï¿½ï¿½nÉ¶ï¿½ï¿½Þ²z
#define cFWVERDT             309                                                //ï¿½bï¿½éª©ï¿½ï¿½ï¿½ï¿½ï¿\uFFFD
#define cHWCYCLE             310                                                //ï¿½wï¿½ï¿½^ï¿½ï¿½gï¿½s]ï¿½w
#define cMODIFYPWD           311                                                //ï¿½ï¿½ï¿½Kï¿½X
#define cDBSETUP             312                                                //ï¿½ï¿½sï¿½tï¿½Î±Kï¿½X
#define cOPERSTAT            313                                                //ï¿½Bï¿½@ï¿½ï¿½ï¿½p
#define cOPERMODE            314                                                //ï¿½Þ§@ï¿½Ò¦ï¿½
#define cCOMMSTAT            315                                                //ï¿½qï¿½Tï¿½ï¿½ï¿½A

#define cCARLENGTH           316                                                //ï¿½ï¿½ï¿½ï¿½]ï¿½w
#define cSENDCYCLE           317                                                //ï¿½Ç¿ï¿½gï¿½s]ï¿½w
#define cRECORDCYCLE         318                                                //ï¿½Oï¿½ï¿½gï¿½s]ï¿½w
#define cUPDATEDB            319                                                //ï¿½ï¿½sï¿½ï¿½Æ®w
#define cLANESET             320                                                //ï¿½ï¿½ï¿½Dï¿½ï¿½Óªï¿\uFFFD
#define cREALSTAT            321                                                //ï¿½ï¿½ï¿½yï¿½Yï¿½É¸ï¿½T
#define cREALEVENT           322                                                //ï¿½Yï¿½É¨Æ¥ï¿½

#define cONLINE              323                                                //ï¿½ï¿½ï¿½ß¦ï¿½ï¿½sï¿½uï¿½ï¿½ï¿½nï¿½Jï¿½eï¿½ï¿½
#define cOFFLINE             324                                                //ï¿½ï¿½ï¿½ß¨Sï¿½ï¿½ï¿½sï¿½uï¿½ï¿½ï¿½nï¿½Jï¿½eï¿½ï¿½
#define cLOCKDB              325                                                //ï¿½ï¿½ï¿½Oï¿½ï¿½Åªï¿½ï¿½ï¿½eï¿½ï¿½
#define cTOTALRUN            326                                                //ï¿½`ï¿½}ï¿½ï¿½É¶ï¿\uFFFD

#define cABOERROR            327                                                //ï¿½Tï¿½ï¿½ï¿½]ï¿½wï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
#define cCMDSET              328                                                //ï¿½Tï¿½ï¿½ï¿½]ï¿½w
#define cHWRESET             329                                                //ï¿½wï¿½é­«ï¿½m
#define cLCNENCODE           330                                                //ï¿½]ï¿½Æ½sï¿½ï¿½ï¿½sï¿½X

//OTADD
#define cSEGTYPE             331                                                //ï¿½É¬qï¿½ï¿½ï¿½A
#define cSEGTYPEMENU         332
#define cWEEKDAY             333
#define cSPECIALDAY          334
#define cSEGTYPESEL          335
#define cPLANMENU            336
#define cPLANSEL             337
#define cPLAN                338
#define cRUNTABLE            339
#define cSTRATEGY            340
#define cRTMSInfo            341
#define cCURRENTLIGHTSTATUS  342
#define cCURRENTCOMMPACKET   343
#define cPLANEditStep1       344
#define cPLANEditStep2       345
#define cPLANEditStep3       346
#define cPLANEditStep4       347
#define cREDCOUNTMENU        348
#define cREDCOUNTHWCHECKSEL  349
#define cREDCOUNTHWCHECK     350
#define cREDCOUNTHWVERMENU   351
#define cREDCOUNTMAPPING     352

#define cSegTypeEditStep1    360
#define cSegTypeEditStep2    361
#define cSegTypeEditStep3    362

#define cREVERSETIMTMENU     363
#define cREVWEEKDAY          364
#define cREVSPECIALDAY       365
#define cREVERSE             366

#define cPLAN0523            370
#define cPLANF2              371
#define cSEGTYPEEDITF2       372
#define cWEEKDAYEDITF2       373
#define cSPECIALDAYVIEW      374
#define cSPECIALDAYEDITF2    375
#define cSEGTYPEPREVIEW      376
#define cPLANPREVIEW         377
#define cPHASEPREVIEW        378

#define cActuateEdit         380

//OT Pass
#define cPassModeSet         381
//OT Debug Direct
#define cDIRMAPPING          382
#define cLIGHTBOARDMAPPING   383
#define cDIRLIGHTBOARDMAPPINGMENU   384

#define cPHASESEL            385

#define cLAST92TCPLANSEGMENTUPDATE 386

//OT CHAIN
#define cCHAINMENU           387
#define cMOTHERCHAINSET      388
#define cCHILDCHAINSET       389
#define cCHAINSTATUS         390

//Light Edit
#define cLIGHTPREVIEW        391
#define cLIGHTEDIT           392
#define cLIGHTEMULATOR       393

//IP Editor
#define cNETWORKSET          394
#define cNETWORKSETCONFIRM   395

#define cMBSELECT            396
#define cUART2SELECT         397
#define cWARNING             398

#define cTFDSTATUS           399

#define cREDCOUNTHWCHECKDEVID     400
#define cREDCOUNTHWCHECKCYCLENUM  401
#define cREDCOUNTHWCHECKNUM88     402

#define cRAILWAYCHAINSET     403

#define cACTUATEARWENSTATUS 410

#define cNETMASKSET         411

//smem.vSetACK_W2W / smem.vGetACK_W2W
#define cTCToCenter          0
#define cVD01ToCenter        1
#define cVD02ToCenter        2
#define cVD03ToCenter        3
#define cVD04ToCenter        4
#define cVD05ToCenter        5
#define cVD06ToCenter        6
#define cVD07ToCenter        7
#define cVD08ToCenter        8
#define cVD09ToCenter        9
#define cVD10ToCenter        10
#define cVD11ToCenter        11
#define cVD12ToCenter        12
#define cVD13ToCenter        13
#define cVD14ToCenter        14
#define cVD15ToCenter        15
#define cVD16ToCenter        16
#define cVD17ToCenter        17
#define cVD18ToCenter        18
#define cVD19ToCenter        19
#define cVD20ToCenter        20
#define cVD21ToCenter        21
#define cVD22ToCenter        22
#define cVD23ToCenter        23
#define cVD24ToCenter        24
#define cVD25ToCenter        25
#define cVD26ToCenter        26
#define cVD27ToCenter        27
#define cVD28ToCenter        28
#define cVD29ToCenter        29
#define cVD30ToCenter        30
#define cVD31ToCenter        31
#define cVD32ToCenter        32
#define cVD33ToCenter        33
#define cVD34ToCenter        34
#define cVD35ToCenter        35
#define cVD36ToCenter        36
#define cVD37ToCenter        37
#define cVD38ToCenter        38
#define cVD39ToCenter        39
#define cVD40ToCenter        40

#define cCenterToTC          100
#define cCenterToVD01        101
#define cCenterToVD02        102
#define cCenterToVD03        103
#define cCenterToVD04        104
#define cCenterToVD05        105
#define cCenterToVD06        106
#define cCenterToVD07        107
#define cCenterToVD08        108
#define cCenterToVD09        109
#define cCenterToVD10        110
#define cCenterToVD11        111
#define cCenterToVD12        112
#define cCenterToVD13        113
#define cCenterToVD14        114
#define cCenterToVD15        115
#define cCenterToVD16        116
#define cCenterToVD17        117
#define cCenterToVD18        118
#define cCenterToVD19        119
#define cCenterToVD20        120
#define cCenterToVD21        121
#define cCenterToVD22        122
#define cCenterToVD23        123
#define cCenterToVD24        124
#define cCenterToVD25        125
#define cCenterToVD26        126
#define cCenterToVD27        127
#define cCenterToVD28        128
#define cCenterToVD29        129
#define cCenterToVD30        130
#define cCenterToVD31        131
#define cCenterToVD32        132
#define cCenterToVD33        133
#define cCenterToVD34        134
#define cCenterToVD35        135
#define cCenterToVD36        136
#define cCenterToVD37        137
#define cCenterToVD38        138
#define cCenterToVD39        139
#define cCenterToVD40        140
//OT Pass uPassServer
#define cNoPassMode          0
#define cPassServerMode      1
#define cTermPassMode        2

//OT Debug Direct
#define dirN                 0
#define dirNE                1
#define dirE                 2
#define dirES                3
#define dirS                 4
#define dirSW                5
#define dirW                 6
#define dirWN                7
#define lightOut1            0
#define lightOut2            1
#define lightOut3            2
#define lightOut4            3
#define lightOut5            4
#define lightOut6            5
#define lightOut7            6
#define lightOut8            7

//---------------------------------------------------------------------------
#define ViewScreen    0
#define PreviewScreen 1
#define EditScreen    2

//---------------------------------------------------------------------------
#define MB_ARBOR      0
#define MB_ICOP6047   1
#define MB_ICOP6115   2
//---------------------------------------------------------------------------
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
typedef struct SwitchBIT {
        BYTE b1:1;
        BYTE b2:1;
        BYTE b3:1;
        BYTE b4:1;
        BYTE b5:1;
        BYTE b6:1;
        BYTE b7:1;
        BYTE b8:1;
} SwitchBIT;
typedef union  DATA_Bit{
        SwitchBIT switchBit;
        BYTE DBit;
} DATA_Bit;
typedef struct BCD{
        BYTE b1:4;
        BYTE b2:4;
}BCD;
typedef union BCDSW{
        BCD bcdHiLo;
        BYTE bcdCode;
}BCDSW;


typedef struct machine_common_info
{
    int LCN;
    int localIp[4];
    int localIp_port;
    int remoteIP1[4];
    int remoteIP1_port;
    int remoteIP2[4];
    int remoteIP2_port;
    int netmask[4];
    int gateway[4];
}machine_common_info;
#endif // VARIABLE_H
