#ifndef GAMESCREEN_H_
#define GAMESCREEN_H_

#include "Adim.h"
#include "HGauge.h"
#include "VGauge.h"
#include "Player.h"
#include "TypingClient.h"

#include <map>
#include <algorithm>
#include <QWidget>

class QPaintEvent;
class QKeyEvent;
class QPushButton;
class QPainter;

class GameScreen : public QWidget
{
    Q_OBJECT
public:
	GameScreen(QWidget *parent=0);
protected:
    void paintEvent(QPaintEvent *event);
	void keyPressEvent(QKeyEvent *event);
private:
	VGauge* vGauge_;
	HGauge* hGauge_;
	QPainter* mainPainter;
	NetworkPlayer* player_;

	enum{ MAIN_PLAYER_POSX = 20, MAIN_PLAYER_POSY = 20 };
	enum{ SUB_PLAYER_INTERBAL_Y = 200 };
//	std::map<int,Player*> playerList_;
};

#endif
