//---------------------------------------------------------------------------
#ifndef READJOBH
#define READJOBH

#include "var.h"
#include "JOB.h"
//---------------------------------------------------------------------------
class READJOB:public JOB
{
    public:

        READJOB(void);
        ~READJOB(void);

        bool SetInterfaceAndReadFlag(int *,MESSAGEOK *,int);                    //�]�w���ʥ]Read��flag
        bool CheckLcn(int *,MESSAGEOK *);                                       //���ߨӪ��ʥ],���n�ˬd���f�s���O�_�ŦX
        bool vCheckIfBcastingForwardToUDP(int *,MESSAGEOK *);                   //�ˬd�O�_�n��ǡA�n���ܴN�����ǤF
        bool SetCenterComm(int *,MESSAGEOK *);                                  //�]�w���߳q�T
        bool SetKeypadComm(int *,MESSAGEOK *);                                  //�]�w��L�q�T
        bool DoWorkByMESSAGEIN(int *,MESSAGEOK *);                              //�ھګʥ]�@�Ʊ�
        bool DoSendUDP_MESSAGEIN(int *,MESSAGEOK *);

        bool vDoDisplay(int *,MESSAGEOK *);

        //OT Pass
        bool vDoRelayUDP_MESSAGEIN_0F8F(int *, MESSAGEOK *, unsigned char);
        //OT Pass 2
        bool ReadWorkByMESSAGEIN(MESSAGEOK);

    private:
        tsUDPMappingLCN _sUDPMappingLCNinREADJOB[MaxOpenUDPPort];

};
//---------------------------------------------------------------------------
extern READJOB readJob;
#endif

