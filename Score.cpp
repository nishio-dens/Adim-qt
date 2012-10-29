#include "Score.h"
using namespace std;

void Score::startCalcScore()
{
	tmpStartTime_ = time(0);
}

//
//[ˆø”] -- true -> ³‰ð‚µ‚½ false -> ŠÔˆá‚¦‚½
//

void Score::calcScore(bool correct)
{
	if( correct ) {
		++comboCount_;
		++typeCount_;
		if( maxComboCount_ < comboCount_ )
			maxComboCount_ = comboCount_;
	}else{
		comboCount_ = 0;
		++errorCount_;
	}

	tmpEndTime_ = time(0);
	int typeTime = tmpEndTime_ - tmpStartTime_;
	tmpStartTime_ = tmpEndTime_;
	sumTime_ += typeTime;

	//TODO:ƒXƒRƒAŒvŽZ Œã‚ÅC³‚¹‚æ
	masterScore_ += typeTime * 10 + comboCount_ * 5;
}

void Score::calcScore(bool corrent, time_t startTime)
{
	tmpStartTime_ = startTime;
	calcScore(corrent);
}
