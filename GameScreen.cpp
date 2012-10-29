#include "GameScreen.h"
#include <QtGui>
#include <QFont>

GameScreen::GameScreen(QWidget *parent) : QWidget(parent)
{
	player_ = new NetworkPlayer(0,"Nishio");
	player_->adimSetting("keycode.txt","word.txt",10);
	player_->setDrawPoint(MAIN_PLAYER_POSX,MAIN_PLAYER_POSY);
	player_->connectToServer("192.168.1.10",5000);
	connect(player_, SIGNAL(updateData()), this, SLOT(update()) );
	//temp
	//player_->gameStart();
	//player_->masterSetting();

	setPalette(QPalette(QColor(250,250,200))); //”wŒi‚Ì•ÏX
	setAutoFillBackground(true);
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale()); //“ú–{Œê•\Ž¦‰Â”\‚É‚·‚é
	setFocusPolicy(Qt::StrongFocus);
	mainPainter = new QPainter;
	update();
}

void GameScreen::keyPressEvent(QKeyEvent *event)
{
	if( player_->isAllPlayerReady() ){ 
		player_->inputCharacter((char)(event->text().toStdString())[0] );
	}else{
		if( (char)(event->text().toStdString())[0] == 'a' ) {
			player_->gameStart();
		}
		if( (char)(event->text().toStdString())[0] == 'b' ) {
			player_->joinToGame();
		}
		if( (char)(event->text().toStdString())[0] == 'c' ) {
			player_->seedQuestion();
			player_->masterSetting();
		}
		
	}
	//player2_->inputCharacter((char)(event->text().toStdString())[0] );
	//vGauge_->setCurrentValue( vGauge_->getCurrentValue() + 1 );
	//hGauge_->setCurrentValue( hGauge_->getCurrentValue() + 1 );
	update();
}

void GameScreen::paintEvent(QPaintEvent * /* event */)
{
	mainPainter->begin(this);
	mainPainter->setPen(QPen(Qt::black));
	mainPainter->setFont(QFont("Times",12) );
	player_->draw(mainPainter);
	mainPainter->end();
}


