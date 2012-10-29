#include "Player.h"
#include <QPoint>
#include <QtGui>
#include <sstream>
#include <iostream>

Player::Player(QWidget* parent, const string& playerName, int drawXPoint, int drawYPoint) 
	: playerName_(playerName), drawPoint_(QPoint(drawXPoint, drawYPoint))
	  , isPlayGame_(false) , QWidget(parent)
{		
}

void Player::adimSetting(const string& keycodeFilename, 
			const string& workbookFilename,
		 	const string& candidateNumberFilename, 
			size_t numberOfQuestion,
			bool randomQuestion,
			size_t firstQuestionNumber,
			int characterCode)
{
	int code;
	switch( characterCode ) {
		case CODE_SHIFT_JIS:
			code = 2;
			break;
		case CODE_UTF_8:
			code = 3;
			break;
		case CODE_UNKNOWN:
			code = 2;
			break;
	}
	player_.initAdim(keycodeFilename,workbookFilename,candidateNumberFilename,
					numberOfQuestion,randomQuestion,firstQuestionNumber,code);
}

void Player::gameStart()
{
	isPlayGame_ = true;
	score_.startCalcScore();
}

void Player::inputCharacter(char code)
{
	if( isPlayGame_ == false )
		return;

	int result = player_.InputCharacter(code);
	enum{ COMPLETE = 2, SUCCESS = 1, FAILURE = 0 };
	
	switch( result ) {
	case SUCCESS:
		score_.calcScore( true );
		break;
	case COMPLETE:
		score_.calcScore( true );
		if( player_.isFinishTyping() ) {
			isPlayGame_ = false;
		}
		break;
	case FAILURE:
		score_.calcScore( false );
		break;
	}
}

void Player::draw(QPainter* painter)
{
	if( isPlayGame_ ) {
		painter->drawText(drawPoint_.x(),drawPoint_.y(),
				QString(tr( player_.GetNowSentenceOfQuestion().c_str() ) ) );
		painter->drawText(drawPoint_.x(),drawPoint_.y() + 20,
				QString(tr( player_.GetNowSentenceOfAgateQuestion().c_str()) ) );
		painter->drawText(drawPoint_.x(),drawPoint_.y() + 40,
				QString(tr( player_.GetNowSentenceOfConvertQuestion().c_str()) ) );
                                                  
		painter->setPen(QPen(Qt::red)); //Ô      
		painter->drawText(drawPoint_.x(),drawPoint_.y() + 20,
				QString(tr( player_.GetNowSentenceOfInputtedAgateQuestionRed().c_str()) ) );
		painter->drawText(drawPoint_.x(),drawPoint_.y() + 40,
				QString(tr( player_.GetNowSentenceOfInputtedConvertQuestionRed().c_str()) ) );
                                                  
		painter->setPen(QPen(Qt::blue)); //Â     
		painter->drawText(drawPoint_.x(),drawPoint_.y() + 20,
				QString(tr( player_.GetNowSentenceOfInputtedAgateQuestionBlue().c_str()) ) );
		painter->drawText(drawPoint_.x(),drawPoint_.y() + 40,
				QString(tr( player_.GetNowSentenceOfInputtedConvertQuestionBlue().c_str()) ) );

	}else{
		painter->drawText(drawPoint_.x(),drawPoint_.y() + 20,QString(tr("‚¨‚Â‚©‚ê‚³‚Ü‚Å‚µ‚½")) );
	}
	
	painter->setPen(QPen(Qt::black));      
	std::ostringstream tmpStream;
	std::ostringstream tmpStream2;
	tmpStream << "Score :" << score_.getScore() << " ErrorCount :" << score_.getErrorCount()
	 << "Combo :" << score_.getComboCount() << " getMaxComboCount :" << score_.getMaxComboCount();
	tmpStream2 << "sumtime:" << score_.getSumTime() << " getTypeSpeed :" << score_.getTypeSpeed()
	 << " corrent:" << score_.getCorrectPercent();
	string tmpStr = tmpStream.str();
	string tmpStr2 = tmpStream2.str();

	painter->drawText(drawPoint_.x(), drawPoint_.y() + 80,
			QString( tr( tmpStr.c_str()) ) );
	painter->drawText(drawPoint_.x(), drawPoint_.y() + 110,
			QString( tr( tmpStr2.c_str()) ) );
}

NetworkPlayer::NetworkPlayer(QWidget* parent, const string& playerName, int drawXPoint, int drawYPoint) 
{
	player_ = new Player(this, playerName, drawXPoint, drawYPoint);
	client_ = new TypingClient;
	isAllPlayerReady_ = false;

	connect(client_, SIGNAL(signalAddPlayer(int,const QString&) ),
		this, SLOT( addPlayer(int,const QString&)) );
	connect(client_, SIGNAL(signalRemovePlayer(int,const QString&)),
			this, SLOT(removePlayer(int,const QString&)) );
	connect(client_, SIGNAL(signalGameStatus(int,bool)), 
			this, SLOT( setGameStatus(int,bool)) );
	connect(client_, SIGNAL(signalScore(int,int,time_t,char)),
			this, SLOT( setScore(int,int,time_t,char)) );
	connect(client_, SIGNAL(signalAdimSetting(const QString&,const QString&,int)),
			this, SLOT( adimSetting(const QString&,const QString&, int)) );
	connect(client_, SIGNAL(signalInitQuestionArray()),
			this, SLOT( initNumberOfQuestionArray()) );
	connect(client_, SIGNAL(signalAddQuestion(int)), this, SLOT( addQuestion(int)));
	
	connect(client_, SIGNAL(signalAddPlayer(int,const QString&) ),
		this, SIGNAL(updateData()) );
	connect(client_, SIGNAL(signalRemovePlayer(int,const QString&)),
			this, SIGNAL(updateData()) );
	connect(client_, SIGNAL(signalGameStatus(int,bool)), 
			this, SIGNAL(updateData()) );
	connect(client_, SIGNAL(signalScore(int,int,time_t,char)),
			this, SIGNAL(updateData()) );
	connect(client_, SIGNAL(signalAdimSetting(const QString&,const QString&,int)),
			this, SIGNAL(updateData()) );
	connect(client_, SIGNAL(signalInitQuestionArray()),
			this, SIGNAL(updateData()) );
	connect(client_, SIGNAL(signalAddQuestion(int)), this, SIGNAL(updateData()));
}

void NetworkPlayer::connectToServer(const QString& serverName, int port)
{
	client_->settingServer(serverName,port);
	client_->connectToServer();
	client_->sendMessage("Hello server.");
	client_->sendGameStatus(player_->getIsPlayGame());
	//client_->sendAddPlayer(QString( player_->getPlayerName().c_str() ) );
}

void NetworkPlayer::joinToGame()
{
	client_->sendAddPlayer(QString( player_->getPlayerName().c_str() ) );
}

void NetworkPlayer::addPlayer(int clientId, const QString& name)
{
	map<int,Player*>::iterator pos = playerList_.find(clientId);
	if( pos != playerList_.end() )
		return;
	
	int num = playerList_.size() + 1;
	std::cerr << "the number of current player is " << num << "\n";
	playerList_[clientId] = new Player(0,name.toStdString(),
			player_->getDrawPoint().x(), player_->getDrawPoint().y() 
			+ PLAYER_INTERVAL_Y * num );
}

void NetworkPlayer::removePlayer(int clientId, const QString& name)
{
	map<int,Player*>::iterator pos = playerList_.find(clientId);
	if( pos == playerList_.end() )
		return;
	playerList_.erase(pos);
}

void NetworkPlayer::setGameStatus(int clientId, bool isActive)
{
	map<int,Player*>::iterator pos = playerList_.find(clientId);
	if( pos == playerList_.end() ) 
		return;
	
	bool isGameStart = true;
	playerList_[clientId]->setIsPlayGame(isActive);
	for( pos = playerList_.begin(); pos != playerList_.end(); ++pos) {
		isGameStart = pos->second->getIsPlayGame();
		std::cout << pos->first << "status is " << isGameStart << "\n";
		if( !isGameStart ) {
			isAllPlayerReady_ = false;
			break;
		}
	}
	//std::cout << "set game status id is " << client_->getClientId() << "\n";
	//if( client_->getClientId() != clientId ) {
	std::cout << "my game status is " << player_->getIsPlayGame() << "\n";
		
	if( isGameStart && player_->getIsPlayGame() ) {
		player_->gameStart();
		isAllPlayerReady_ = true;
		for( pos = playerList_.begin(); pos != playerList_.end(); ++pos) {
			pos->second->gameStart();
		}
	}
}

void NetworkPlayer::setScore(int clientId, int score, time_t time, char key)
{
	map<int,Player*>::iterator pos = playerList_.find(clientId);
	if( pos == playerList_.end() )
		return;
	playerList_[clientId]->inputCharacter(key);
	playerList_[clientId]->setScore(score);
	playerList_[clientId]->setStartTime(time);
}

void NetworkPlayer::adimSetting(const QString& keycodeFilename,
		    const QString& wordFilename, int numberOfQuestion)
{
	player_->adimSetting( keycodeFilename.toStdString(),
				wordFilename.toStdString() ,
			defaultCandidateNumberFilename_,numberOfQuestion,true,0);

	for( map<int,Player*>::iterator pos = playerList_.begin();
			pos != playerList_.end(); ++pos ) {
		pos->second->adimSetting( keycodeFilename.toStdString(),
				wordFilename.toStdString() ,
			defaultCandidateNumberFilename_,numberOfQuestion,true,0);
	}
}
void NetworkPlayer::initQuestionArray()
{
	player_->clearNumberOfQuestionArray();
	std::cerr << "init question number\n";
	for( map<int,Player*>::iterator pos = playerList_.begin();
			pos != playerList_.end(); ++pos ) {
		pos->second->clearNumberOfQuestionArray();
	}
}

void NetworkPlayer::addQuestion(int questionNumber)
{
	player_->addNumberOfQuestionArray(questionNumber);
	std::cerr << "add question number is " << questionNumber << "\n";
	for( map<int,Player*>::iterator pos = playerList_.begin();
			pos != playerList_.end(); ++pos ) {
		pos->second->addNumberOfQuestionArray(questionNumber);
	}
}

void NetworkPlayer::masterSetting()
{
	vector<size_t> tmpNum = player_->getNumberOfQuestionArray();
	
	client_->sendInitQuestionArray();
	client_->sendAdimSetting( QString( player_->getKeycodeFilename().c_str()),
			QString( player_->getWorkbookFilename().c_str() ),
			player_->getNumberOfQuestion() );
	/*client_->sendDebug( QString( player_->getKeycodeFilename().c_str()),
			QString( player_->getWorkbookFilename().c_str() ),
			player_->getNumberOfQuestion() );*/
	std::cerr << "before call myself init question\n";
	player_->clearNumberOfQuestionArray();
	player_->debugShowQuestion();
	std::cerr << "call myself init question.\n";
	player_->debugShowQuestion();
	
	for(vector<size_t>::iterator pos = tmpNum.begin();
			pos != tmpNum.end(); ++pos ) {
		std::cerr << "send pos :" << *pos << "\n";
		client_->sendAddQuestion( *pos );
	}
}

void NetworkPlayer::gameStart() 
{ 
	player_->gameStart();
	//temp
	player_->debugShowQuestion();
	
	map<int,Player*>::iterator pos;
	bool isGameStart = true;
	for( pos = playerList_.begin(); pos != playerList_.end(); ++pos) {
		isGameStart = pos->second->getIsPlayGame();
		std::cout << pos->first << "status is " << isGameStart << "\n";
		
		if( !isGameStart ) {
			isAllPlayerReady_ = false;
			break;
		}
	}
	if( isGameStart )
		isAllPlayerReady_ = true;
	client_->sendGameStatus(true);
}

void NetworkPlayer::inputCharacter(char code) 
{ 
	player_->inputCharacter(code);
	client_->sendScore(getScore(),getStartTime(),code);
}

void NetworkPlayer::draw(QPainter* painter) 
{ 
	player_->draw(painter);
	map<int,Player*>::iterator last = playerList_.end();
	for( map<int,Player*>::iterator pos = playerList_.begin();
		pos != last; ++pos ) {
			pos->second->draw(painter);
		}
}
