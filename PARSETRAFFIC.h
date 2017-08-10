//---------------------------------------------------------------------------
#ifndef PARSETRAFFICH
#define PARSETRAFFICH

#include "var.h"
#include "PARSE.h"

//---------------------------------------------------------------------------
/* Have move to var.h
typedef struct BCD{
        BYTE b1:4;
        BYTE b2:4;
}BCD;
typedef union BCDSW{
        BCD bcdHiLo;
        BYTE bcdCode;
}BCDSW;
*/
//---------------------------------------------------------------------------
class PARSETRAFFIC:public PARSE
{
    public:

        PARSETRAFFIC(void);
        ~PARSETRAFFIC(void);

        bool ParseBlock(int,BYTE *,MESSAGEOK *,int *,int *);                    //�ѪRblock
        bool vClearMsg(MESSAGEOK *, unsigned short int);
        bool CheckSum(int *,MESSAGEOK *);                                       //�ˬdchecksum
        bool DecideProtocol(int *,MESSAGEOK *,int);                             //�������ʥ]��w�N�X
        bool vJudgeProtocolAndCheckLength(int *,MESSAGEOK *);
        bool CheckReasonable(int *,MESSAGEOK *);                                //�ˬd�ʥ]�X�z��
        bool AssignLcn(int *,MESSAGEOK *);                                      //�ھګʥ]�p��LCN
        bool EchoToGUI(int *,MESSAGEOK *,char *);                               //��ܦܵe��
        bool MoveLastData(int *,int *,MESSAGEOK *);                             //�h���Ѿl�����
        BCDSW bcdSwitchHi,bcdSwitchLo;

        bool ParseBlockEasyVersion(int,BYTE *,MESSAGEOK *,int *,int *);

    private:/*
        bool decide77Or87(MESSAGEOK *);                                          //�M�w���ʥ]�O77�~��87�~
        bool decideProtocol92Normal(MESSAGEOK *);                                //�M�w���ʥ]�O92�~�@�ΰT��
        bool decideProtocol92Tc(MESSAGEOK *);                                    //�M�w���ʥ]�O92�~���x����T��
        bool decideProtocol92Vd(MESSAGEOK *);                                    //�M�w���ʥ]�O92�~�����������T��
        bool decideProtocol92Cms(MESSAGEOK *);                                   //�M�w���ʥ]�O92�~��T�i�ܱ���T��
*/

};
//---------------------------------------------------------------------------
#endif
