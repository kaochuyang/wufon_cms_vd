//---------------------------------------------------------------------------
#ifndef PARSETRAFFICH
#define PARSETRAFFICH

#include "variable.h"
\

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class PARSETRAFFIC
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

        bool MoveLastData(int *,int *,MESSAGEOK *);                             //�h���Ѿl�����
        BCDSW bcdSwitchHi,bcdSwitchLo;



    private:


};
//---------------------------------------------------------------------------
#endif
