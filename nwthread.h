#ifndef NWTHREAD_H
#define NWTHREAD_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QTcpSocket>
#include <QMutex>
#include <QQueue>
#include <QMutexLocker>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QProcess>
#include <QJsonObject>
#include "bd_type.h"

extern bool  connection_tcp ;
extern PrccessParam_tt PrccessParam;

enum NW_SOCKET_STATE{NW_SOCKET_STATE_DISCONNECTED=1,NW_SOCKET_STATE_AUTHED=3};

class NWThread : public QThread
{
    Q_OBJECT
private:
    bool threadfinished;
    NW_SOCKET_STATE socketState;
    int seqno;
    QTcpSocket* socket;
    qint64 lastrecvdatatime;
    qint64 lastpingtime;
    QString username;
    QString password;
    bool isfirstlogin;
    QMutex sendQueueMutex;
    QQueue<QString> sendQueue;
    int getseqno();

    bool sendStr(QString& str);
    bool readLine(QString& str,int timeout);
    bool connectAndLogin(QString& msg);
    void disconnectSocket();
    bool isconnected(){ return !this->threadfinished && this->socketState==NW_SOCKET_STATE_AUTHED;}


public:
    static QString serverIP;
    static int serverPort;
    static qint64 pingdelay;
    bool update_name_pwd(QString userName, QString userPwd);

    NWThread(QObject *parent=0,const QString& username="",const QString& password=""):QThread(parent)
    {
        qDebug() << "NWThread";
        threadfinished = false;
        socketState = NW_SOCKET_STATE_DISCONNECTED;
        seqno = 0;
        lastrecvdatatime = 0;
        lastpingtime = 0;
        this->username=username;
        this->password=password;
        isfirstlogin = true;
    }
    ~NWThread()
    {
        qDebug() << "~NWThread";
    }
    void run();

    //manual end the thread
    void endthread()
    {
        qDebug("endthread");
        this->threadfinished = true;
    }

    void fireLoginResultSignal(bool success,QString msg)
    {
        qDebug() << "fireLoginResultSignal:" << success << "msg:" << msg;
        emit signal_login_result(success,msg);
    }
    void fireNWDataAvaliableSignal(QString data)
    {
        qDebug() << "fireNWDataAvaliableSignal:" << data;
        emit signal_nw_data_avaliable(data);
    }

    void nw_send_data(QString data){
        if(this->isconnected())
        {
            QMutexLocker locker(&sendQueueMutex);
            this->sendQueue.enqueue(data);
        }
    };

    void nw_isconnected(){
        emit signal_nw_isconnected(this->isconnected());
        connection_tcp = true;
    }

signals:
    //for others
    void signal_login_result(bool success,QString msg);
    void signal_nw_data_avaliable(QString data);
    void signal_nw_isconnected(bool connected);

};



#endif // NWTHREAD_H
