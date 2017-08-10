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

        bool ParseBlock(int,BYTE *,MESSAGEOK *,int *,int *);                    //解析block
        bool vClearMsg(MESSAGEOK *, unsigned short int);
        bool CheckSum(int *,MESSAGEOK *);                                       //檢查checksum
        bool DecideProtocol(int *,MESSAGEOK *,int);                             //給予此封包協定代碼
        bool vJudgeProtocolAndCheckLength(int *,MESSAGEOK *);
        bool CheckReasonable(int *,MESSAGEOK *);                                //檢查封包合理性
        bool AssignLcn(int *,MESSAGEOK *);                                      //根據封包計算LCN

        bool MoveLastData(int *,int *,MESSAGEOK *);                             //搬移剩餘的資料
        BCDSW bcdSwitchHi,bcdSwitchLo;



    private:


};
//---------------------------------------------------------------------------
#endif
