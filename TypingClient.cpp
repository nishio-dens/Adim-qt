#include <QtGui>
#include <QtNetwork>
#include <iostream>
#include <sstream>
#include "TypingClient.h"

TypingClient::TypingClient(QWidget* parent) : QWidget(parent), 
	serverName_("127.0.0.1"), port_(5000), clientId_(0)
{
	//connect(sendButton, SIGNAL(clicked()), this, SLOT(debugFunction()) );
	//connect(connectButton, SIGNAL(clicked()), this, SLOT( connectToServer()) );
	//connect(buttonBox, SIGNAL(accepted()), this, SLOT(closeConnection()) );
	//connect(buttonBox, SIGNAL(rejected()), this, SLOT(closeConnection()) );
	connect(&tcpSocket_, SIGNAL(readyRead()), this, SLOT( receiveRequest()) );
	connect(&tcpSocket_, SIGNAL(disconnected()), this, SLOT( connectionCloseByServer()) );
	connect(&tcpSocket_, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(error()) );
}

void TypingClient::sendDebug(const QString& keycodeFilename,
			const QString& wordFilename, int numberOfQuestion)
{
	/*std::cerr << "send debug" << "\n";
	emit signalInitQuestionArray();
	emit signalAdimSetting( keycodeFilename,
		wordFilename, numberOfQuestion );*/
}

void TypingClient::debugFunction()
{
	//temporary
	//QString tmpQStr;
	//tmpQStr = cMessageEdit->text();
	//std::string tmpStr = tmpQStr.toStdString();

	//sendMessage("Hello server.");
	//sendAddPlayer(tmpStr);
	//sendGameStatus(true);
	//sendScore(380000,3838389,'a');
}

void TypingClient::connectToServer()
{
	tcpSocket_.connectToHost(serverName_, port_ );
	nextBlockSize_ = 0;
}

void TypingClient::sendMessage(const QString& msg)
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_1);
	
	out << quint16(0) << quint16(0x01) << msg;
	out.device()->seek(0);
	out << quint16(block.size() - sizeof(quint16) );
	tcpSocket_.write(block);
}

void TypingClient::sendGameStatus(bool isActive)
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_1);
	
	out << quint16(0) << quint16(0x02) << quint16(clientId_) << isActive;
	out.device()->seek(0);
	out << quint16(block.size() - sizeof(quint16) );
	tcpSocket_.write(block);
}

void TypingClient::sendScore(int score, time_t time, char ch)
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_1);
	
	out << quint16(0) << quint16(0x03) << quint16(clientId_) << quint32(score)
		<< quint32(time) << quint8(ch);
	out.device()->seek(0);
	out << quint16(block.size() - sizeof(quint16) );
	tcpSocket_.write(block);
}

void TypingClient::sendAddPlayer(const QString& name)
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_1);
	
	out << quint16(0) << quint16(0x04) << quint16(clientId_)
		<< name;
	out.device()->seek(0);
	out << quint16(block.size() - sizeof(quint16) );
	tcpSocket_.write(block);
}

void TypingClient::sendRemovePlayer(const QString& name)
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_1);
	
	out << quint16(0) << quint16(0x05) << quint16(clientId_)
		<< name;
	out.device()->seek(0);
	out << quint16(block.size() - sizeof(quint16) );
	tcpSocket_.write(block);
}

void TypingClient::sendAdimSetting(const QString& keycodeFilename,
		const QString& wordFilename, int numberOfQuestion )
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_1);
	
	out << quint16(0) << quint16(0x06) << keycodeFilename
		<< wordFilename << quint16(numberOfQuestion);
	out.device()->seek(0);
	out << quint16(block.size() - sizeof(quint16) );
	tcpSocket_.write(block);
}

void TypingClient::sendInitQuestionArray()
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_1);
	
	out << quint16(0) << quint16(0x07);
	out.device()->seek(0);
	out << quint16(block.size() - sizeof(quint16) );
	tcpSocket_.write(block);
}

void TypingClient::sendAddQuestion(int questionNumber)
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_1);
	
	out << quint16(0) << quint16(0x08) << quint16(questionNumber);
	out.device()->seek(0);
	out << quint16(block.size() - sizeof(quint16) );
	tcpSocket_.write(block);
}

void TypingClient::receiveRequest()
{
	QDataStream in(&tcpSocket_);
	in.setVersion(QDataStream::Qt_4_1);

	forever {
		if( nextBlockSize_ == 0 ) {
			if( tcpSocket_.bytesAvailable() < sizeof(quint16) )
				break;
			in >> nextBlockSize_;
		}
		if( nextBlockSize_ == 0xFFFF ) {
			closeConnection();
			break;
		}
		if( tcpSocket_.bytesAvailable() < nextBlockSize_ )
			break;
		
		quint16 callCode;
		in >> callCode;
		
		QString message, message2;
		quint16 id = 0;
		quint32 score = 0,time = 0;
		quint8 ch = 0;
		bool status;
		
		switch( callCode ) {
		case 0x00:
			in >> id;
			std::cerr << "receive hello message id is " << int(id) << "\n";
			setClientId( int(id) );
			break;
		case 0x01: //receive message
		    in >> message;
			emit signalMessage(message);
		    break;
		case 0x02:
		    in >> id >> status;
		    std::cerr << "receive status id is " << int(id) << 
		    	" stat is " << status << "\n";
			emit signalGameStatus(id,status);
		    break;
		case 0x03:
		    in >> id >> score >> time >> ch;
		    emit signalScore(id,score,time,ch);
			break;
		case 0x04:
		    in >> id >> message;
		   std::cerr << "add id is " << int(id) << 
		    	" name is " << message.toStdString() << "\n";
			emit signalAddPlayer(id,message);
		    break;
		case 0x05:
		    in >> id >> message;
		    std::cerr << "remove id is " << int(id) << 
		    	" name is " << message.toStdString() << "\n";
			emit signalRemovePlayer(id,message);
		    break;
		case 0x06:
			in >> message >> message2 >> id;
			emit signalAdimSetting(message,message2,id);
			break;
		case 0x07:
			std::cerr << "call init question.\n";
			emit signalInitQuestionArray();
			break;
		case 0x08:
			in >> id;
			emit signalAddQuestion(id);
			break;
		default:
		    nextBlockSize_ = 0;
		}
		nextBlockSize_ = 0;
	}
}

void TypingClient::connectionCloseByServer()
{
	if( nextBlockSize_ != 0xFFFF )
		std::cerr << "ERROR. Connection was closed by server.\n";
	closeConnection();
}

void TypingClient::closeConnection()
{
	tcpSocket_.close();
}

void TypingClient::error()
{
	std::cerr << "network error.\n";
	closeConnection();
}

