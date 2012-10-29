#ifndef SCORE_H_
#define SCORE_H_

#include <ctime>

class Score
{
public:
	Score() : masterScore_(0), errorCount_(0), comboCount_(0),
	maxComboCount_(0), typeCount_(0), sumTime_(0), tmpStartTime_(0), tmpEndTime_(0) {}

	void startCalcScore();
	void calcScore(bool correct);
	void calcScore(bool corrent, std::time_t startTime);
	
	void setStartTime(std::time_t startTime) { tmpStartTime_ = startTime; }
	std::time_t getStartTime() const { return tmpStartTime_; }
	void setScore(int score) { masterScore_ = score; }

	int getScore() { return masterScore_; }
	int getErrorCount() { return errorCount_; }
	int getComboCount() { return comboCount_; }
	int getMaxComboCount() { return maxComboCount_; }
	int getSumTime() { return sumTime_; }
	double getTypeSpeed() { return (double)(typeCount_) / (double)sumTime_; }
	double getCorrectPercent() { return (double)( 1.0 - (double)errorCount_ / (double)typeCount_ ); }
	
private:
	int masterScore_;    
	int errorCount_;		
	int comboCount_;
	int maxComboCount_;
	int typeCount_;
	int sumTime_;

	std::time_t tmpStartTime_;
	std::time_t tmpEndTime_;
};

#endif
