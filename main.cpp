#include <QApplication>
#include <QHBoxLayout>
#include "GameScreen.h"

class MainWindow : public QWidget
{
public:
    MainWindow(QWidget *parent = 0);
};

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
	GameScreen* gameScreen = new GameScreen;
	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(gameScreen);
	setLayout(layout);
	setMinimumSize(600,600);
}

int main(int argc, char *argv[])
{
     QApplication app(argc, argv);
     MainWindow* window = new MainWindow;
	 window->show();
     return app.exec();
}
