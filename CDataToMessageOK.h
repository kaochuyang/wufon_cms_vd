#ifndef CDataToMessageOkH
#define CDataToMessageOkH

#include "var.h"
//--------------------------------------------------------------------------
class CDataToMessageOK
{
    public:

    //For SmartSensor------------------------------------------------------------
    /*  HowToUse _MESSAGEOUT = oDataToMessageOK.SendSSProtocalSetTimeInterval(30); */
//        MESSAGEOK SendSSProtocalSetTimeInterval(int);

    /*  HowToUse _MESSAGEOUT = oDataToMessageOK.SendSSProtocalSetClassificationLengths(20, 30); */
    /* class0: 0~20, 21~30, 31~1000  */
//        MESSAGEOK SendSSProtocalSetClassificationLengths(int, int);
//        MESSAGEOK SendSSProtocalSetTime(void);

//        MESSAGEOK QuerySSProtocalClassificationLengths(void);
//        MESSAGEOK QuerySSBasicInfo_S0(void);

    //For 92VD------------------------------------------------------------
    /* HowToUse _MessageOfMake = vPackageINFOTo92Protocol(Info,  Length); */
        MESSAGEOK vPackageINFOTo92Protocol(unsigned char *, int ,bool);                     /*把Info打包成 AABBooxxINFOAADDchk */

    //For Screen------------------------------------------------------------
//        MESSAGEOK vSet92VDTimeGap_6F14ToMessageOK(int);
    //6F30?
//        MESSAGEOK vSet92VDCarLength_6F31ToMessageOK(int, int);
//        MESSAGEOK vSet92VDTransmitCycle_6F3FToMessageOK(short int,short int);

    //For Ver94RedCount
        MESSAGEOK vPackageINFOToVer94RedCount(BYTE,
                                              unsigned short int,
                                              unsigned short int,
                                              unsigned short int,
                                              unsigned short int,
                                              unsigned short int  );

        MESSAGEOK vPackageINFOToVerHKRedCount(int, int, int, int );
        MESSAGEOK vPackageINFOToVerHKRedCountComm();

        MESSAGEOK vPackageINFOToVer94v2RedCount(BYTE,
                                              unsigned short int,
                                              unsigned short int,
                                              unsigned short int,
                                              unsigned short int,
                                              unsigned short int,
                                              unsigned short int,
                                              unsigned short int,
                                              unsigned short int );

        MESSAGEOK vPackageINFOTo92ProtocolSetADDR(unsigned char *, int, unsigned char, unsigned short int);

    private:

};
//--------------------------------------------------------------------------
extern CDataToMessageOK oDataToMessageOK;
#endif
