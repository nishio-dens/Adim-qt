#ifndef PLAYER_H_
#define PLAYER_H_

#include "Adim.h"
#include "Score.h"
#include "TypingClient.h"
#include <string>
#include <QPoint>
#include <QWidget>
#include <vector>
#include <map>
#include <iostream>

using std::string;
using std::vector;

const std::string defaultCandidateNumberFilename_ = "Candidate_Number.txt";

class QPainter;

class Player : QWidget
{
	Q_OBJECT
public :
	enum{ CODE_SHIFT_JIS = 1, CODE_UTF_8 = 2 , CODE_UNKNOWN = 3 }; 
	
	Player(QWidget* parent = 0, const string& playerName = "No Name", 
		int drawXPoint = 0, int drawYPoint = 0);

	void adimSetting(const string& keycodeFilename, 
			const string& workbookFilename,
		 	const string& candidateNumberFilename, 
			size_t numberOfQuestion = 2,
			bool randomQuestion = true,
			size_t firstQuestionNumber = 0,
			int characterCode = CODE_SHIFT_JIS );
	virtual void seedQuestion() { player_.seedQuestion(); }
	void setPlayerName(const string& playerName) { playerName_ = playerName; }
	string getPlayerName() { return playerName_; }

	void setNumberOfQuestionArray(const vector<size_t>& num)
	{ player_.setNumberOfQuestionArray( num ); }
	void addNumberOfQuestionArray(size_t num)
	{ player_.addNumberOfQuestionArray(num); }
	vector<size_t> getNumberOfQuestionArray() const
	{ return player_.getNumberOfQuestionArray(); }
	void clearNumberOfQuestionArray() { player_.clearQuestionArray(); }

	void setDrawPoint(int x, int y) { drawPoint_ = QPoint(x,y); }
	QPoint getDrawPoint() const { return drawPoint_; }
	void setScore(int score) { score_.setScore(score); }
	int getScore() { return score_.getScore(); }
	void setStartTime(time_t time) { score_.setStartTime(time); }
	time_t getStartTime() const { return score_.getStartTime(); }
	void setIsPlayGame(bool is) { isPlayGame_ = is; }
	bool getIsPlayGame() { return isPlayGame_; }
	std::string getKeycodeFilename() const { return player_.GetKeycodeFilename();}
	std::string getWorkbookFilename() {return player_.GetWorkbookFilename();}
	int getNumberOfQuestion() const { return player_.getNumberOfQuestion(); }
	void gameStart();
	void inputCharacter(char code);
	void draw(QPainter* painter);
	
	//debug show question array
	void debugShowQuestion()
	{
		vector<size_t> tmpNum = player_.getNumberOfQuestionArray();
		int size = tmpNum.size();
		for(int i=0; i < size ; i++ ){ 
			std::cerr << tmpNum[i] << " ";
		}
		std::cerr << "\n";
	}
private:
	Adim player_;
	QPoint drawPoint_;
	string playerName_;
	Score score_;
	bool isPlayGame_;
};

class NetworkPlayer : public QWidget
{
	Q_OBJECT
public:
	NetworkPlayer(QWidget* parent = 0, const string& playerName = "No Name",
			int drawXPoint = 0, int drawYPoint = 0);
public slots:
	void connectToServer(const QString& serverName, int port);
	void addPlayer(int clientId, const QString& name);
	void removePlayer(int clientId, const QString& name);
	void setGameStatus(int clientId, bool isActive);
	void setScore(int clientId, int score, time_t time, char key);	
	void adimSetting(const QString& keycodeFilename,
		    const QString& wordFilename, int numberOfQuestion);
	void initQuestionArray();
	void addQuestion(int questionNumber);
public:
	void joinToGame(); //ゲームに参加
	void masterSetting(); //すべてのプレイヤに自分のセッティングを送る
	void seedQuestion() { player_->seedQuestion(); }
	
	//player class とほぼ同じ内容
	void setPlayerName(const string& playerName) { player_->setPlayerName(playerName); }
	string getPlayerName() { return player_->getPlayerName(); }
	void setNumberOfQuestionArray(const vector<size_t>& num) 
		{ player_->setNumberOfQuestionArray( num ); }
	void addNumberOfQuestionArray(size_t num)
		{ player_->addNumberOfQuestionArray(num); }
	vector<size_t> getNumberOfQuestionArray() const
		{ return player_->getNumberOfQuestionArray(); }
	void setDrawPoint(int x, int y) { player_->setDrawPoint(x,y); }
	QPoint getDrawPoint() const { return player_->getDrawPoint(); }
	void setScore(int score) { player_->setScore(score); }
	int getScore() { return player_->getScore(); }
	void setStartTime(time_t time) { player_->setStartTime(time); }
	time_t getStartTime() const { return player_->getStartTime(); }
	void setIsPlayGame(bool is) { player_->setIsPlayGame(is); }
	bool getIsPlayGame() { return player_->getIsPlayGame(); }
	std::string getKeycodeFilename() const { return player_->getKeycodeFilename();}
	std::string getWorkbookFilename() {return player_->getWorkbookFilename();}
	int getNumberOfQuestion() const { return player_->getNumberOfQuestion(); }
	
	//変更アリ
	void gameStart();
	void inputCharacter(char code);
	void draw(QPainter* painter);

	bool isAllPlayerReady() const { return isAllPlayerReady_; }
signals:
	void updateData();
private:
	enum { PLAYER_INTERVAL_Y = 200 };
	Player* player_;
	TypingClient* client_;
	map<int,Player*> playerList_;
	bool isAllPlayerReady_;
};

#endif

