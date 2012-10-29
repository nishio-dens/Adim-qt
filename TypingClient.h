#ifndef CLIENT_H_
#define CLIENT_H_

#include <QTcpSocket>
#include <string>
#include <ctime>
#include <QWidget>

class TypingClient : public QWidget 
{
	Q_OBJECT	
public:
	TypingClient(QWidget *parent = 0);
	void settingServer(const QString& serverName, int port) 
	{ serverName_ = serverName; port_ = port; }
	int getClientId() const { return clientId_; }
	
	void sendDebug(const QString& keycodeFilename,
			const QString& wordFilename, int numberOfQuestion);
public slots:
	void connectToServer();
	void debugFunction();
	void sendMessage(const QString& msg);
	void sendGameStatus(bool isActive);
	void sendScore(int score, time_t time, char ch);
	void sendAddPlayer(const QString& name);
	void sendRemovePlayer(const QString& name);
	void sendAdimSetting(const QString& keycodeFilename,
			const QString& wordFilename, int numberOfQuestion);
	void sendInitQuestionArray();
	void sendAddQuestion(int questionNumber);
	void receiveRequest();
	void connectionCloseByServer();
	void error();
	void closeConnection();
signals:
	void signalMessage(const QString& msg);
	void signalGameStatus(int clientId, bool isActive);
	void signalScore(int clientId, int score, time_t time, char ch);
	void signalAddPlayer(int clientId, const QString& name);
	void signalRemovePlayer(int clientId, const QString& name);
	void signalAdimSetting(const QString& keycodeFilename,
		    const QString& wordFilename, int numberOfQuestion);
	void signalInitQuestionArray();
	void signalAddQuestion(int questionNumber);
private:
	void setClientId(int clientId) { clientId_ = clientId; }
private:
	QTcpSocket tcpSocket_;
	quint16 nextBlockSize_;
	QString serverName_;
	int port_;
	int clientId_;
};

#endif

