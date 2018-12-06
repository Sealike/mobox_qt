#include "nwthread.h"
#include <QTcpSocket>
#include <QDateTime>

QString NWThread::serverIP = "27.128.164.177";
int NWThread::serverPort = 7120;
qint64 NWThread::pingdelay = 5000;

void NWThread::run()
{
    qDebug() << "NWThread::run() " << QThread::currentThreadId();
    this->socket = new QTcpSocket();
    qDebug() << "QTcpSocket()";
    while(!this->threadfinished)
    {
        if(!socket->isOpen())
        {
            qDebug("this->socketState=NW_SOCKET_STATE_DISCONNECTED;");
            this->socketState=NW_SOCKET_STATE_DISCONNECTED;
        }
        switch (this->socketState)
        {
        case NW_SOCKET_STATE_DISCONNECTED:
            connection_tcp = false;
            if(this->isfirstlogin)
            {
                QString errmsg;
                if(this->connectAndLogin(errmsg))
                {
                    this->isfirstlogin = false;
                    this->fireLoginResultSignal(true,"");
                }else
                {
                    this->fireLoginResultSignal(false,errmsg);
                    this->endthread();
                }
            }else
            {
                QString msg;
                this->connectAndLogin(msg);
            }
            break;
        case NW_SOCKET_STATE_AUTHED:
            {
                connection_tcp = true;
                QString recvstr;
                if(!NWThread::readLine(recvstr,0)){
                    qDebug("readLine error");
                    this->disconnectSocket();
                    break;
                }
                if(!recvstr.isEmpty() && !recvstr.startsWith("$pong")){
                    //qDebug() << recvstr;
                    this->fireNWDataAvaliableSignal(recvstr);

                }else{
                    //qDebug("nodata");
                }
                //senddata
                {
                    QMutexLocker locker(&sendQueueMutex);
                    if(this->sendQueue.size()>0){
                        qDebug() << "sendQueue:" << sendQueue.size();
                        QString str = this->sendQueue.dequeue();
                        this->sendStr(str);
                    }
                }
                //timeout
                if(QDateTime::currentDateTime().toMSecsSinceEpoch()-this->lastrecvdatatime>=pingdelay*3){
                    qDebug("recv data timeout");
                    this->disconnectSocket();
                }
                //send ping
                if(QDateTime::currentDateTime().toMSecsSinceEpoch()-this->lastpingtime>=pingdelay){
                    QString sendstr = "$ping,{\"from\":\"app\",\"to\":\"frontsvr\"}";
                    if(!this->sendStr(sendstr)){
                        qDebug("ping send error");
                        this->disconnectSocket();
                        break;
                    }
                    this->lastpingtime = QDateTime::currentDateTime().toMSecsSinceEpoch();
                }
                msleep(10);
            }
            break;
        default:
            break;
        }
    }
    //clear the socket and the thread itself
    this->socket->deleteLater();
    this->deleteLater();
    connection_tcp = false;
}

int NWThread::getseqno(){
    if(this->seqno>=65535)
    {
        this->seqno = 1;
    }else
    {
        this->seqno++;
    }
    return this->seqno;
}
bool NWThread::connectAndLogin(QString& msg)
{
    QString resulterr;
    bool successFlag = false;
    do{
        qDebug("Connecting to server...");
        //sleep(1);
        socket->connectToHost(NWThread::serverIP,NWThread::serverPort);
        if(!socket->waitForConnected()) {
            resulterr="connect server failed";
            sleep(3);
            break;
        }
        qDebug("Connect success");
        {
            QString sendstr = "$initsess,app,frontsvr,{\"role\":\"app\",\"seqno\":\"";
            sendstr+=QString::number(this->getseqno());
            sendstr+="\"}";
            if(!this->sendStr(sendstr))
            {
                resulterr="send initsess error";
                break;
            }
            QString recvstr;
            if(!NWThread::readLine(recvstr,5000))
            {
                resulterr="recv initack error";
                break;
            }
            if(!recvstr.startsWith("$initack"))
            {
                resulterr="initack data error";
                break;
            }
        }
        qDebug("initsess success");
        {
            QString uname = this->username;
            QString upwd = this->password;
            QString sendstr = "$auth,app,frontsvr,{\"pwd\":\"";
            sendstr+=QString(upwd);
            sendstr+="""\",\"name\":\"";
            sendstr+=QString(uname);
//            sendstr+="\",\"taskid\":\"1234\",\"seqno\":\"";

            sendstr+= "\",\"taskid\":\"";
            sendstr+= PrccessParam.taskid;
            sendstr+= "\",\"seqno\":\"";

            sendstr+=QString::number(this->getseqno());
//            sendstr+="\",\"loc\":{\"lon\":\"114.634564\",\"lat\":\"38.7364552\",\"hgt\":\"67\"}}";

            sendstr+="\",\"loc\":{\"lon\":\"";
            sendstr+=PrccessParam.lon;
            sendstr+="\",\"lat\":\"";
            sendstr+=PrccessParam.lat;
            sendstr+="\",\"hgt\":\"";
            sendstr+=PrccessParam.hgt;
            sendstr+="\"}}";

            if(!this->sendStr(sendstr))
            {
                resulterr="send auth error";
                break;
            }
            QString recvstr;
            if(!NWThread::readLine(recvstr,5000)){
                resulterr="recv auth error";
                break;
            }
            if(!recvstr.startsWith("$auth")){
                resulterr="auth data error";
                break;
            }
            if(recvstr.indexOf("ok")==-1){
                resulterr="auth error";
                break;
            }
            if(recvstr.indexOf("time") == -1)
            {
                resulterr="auth time error";
                break;
            }

            int len = recvstr.indexOf("{");
            QString buf= recvstr.mid(len,recvstr.length()-len-5);

//            buf = recvstr.right(recvstr.length()-len);

            // set system time
            {
                QJsonParseError jsonError;//Qt5新类
                QJsonDocument json = QJsonDocument::fromJson(buf.toUtf8(), &jsonError);//Qt5新类
                if (json.isObject())
                 {
                    QJsonObject rootObj = json.object();
                    //是否含有key  rslt
                    if(rootObj.contains("time"))
                    {
                        QJsonValue value = rootObj.value("time");
                        //判断是否是string类型
                        if (value.isString())
                        {
                            QString systime = value.toString();
                            qDebug() << "systime" << systime;

                            QString m = "date -s \"" + systime + "\"";
                            QProcess::startDetached(m);
                            QProcess::startDetached("hwclock -w");
                            QProcess::startDetached("sync");
                        }
                    }
                }
            }

        }
        qDebug("auth success");
        this->socketState=NW_SOCKET_STATE_AUTHED;
        successFlag = true;
    }while(0);
    if(!successFlag){
      qDebug() <<  resulterr;
      msg=resulterr;
    }
    return successFlag;
}
void NWThread::disconnectSocket()
{
    if(this->socket!=NULL){
        qDebug("disconnect start");
        this->socket->disconnect();
        this->socket->waitForDisconnected(1000*3);
        this->socket->abort();
        this->socketState=NW_SOCKET_STATE_DISCONNECTED;
        qDebug("disconnect end");
    }
}

bool NWThread::update_name_pwd(QString userName, QString userPwd)
{
    this->username=userName;
    this->password=userPwd;
}
bool NWThread::readLine(QString& str,int timeout){
    try {
            if(!socket->isOpen())
            {
                return false;
            }
            do{
                this->socket->waitForReadyRead(0);
                if(this->socket->canReadLine()){
                    QByteArray bas = this->socket->readLine();
                    if(!bas.isEmpty()){
                        int di = bas.indexOf('$');
                        if(di>=0){
                            str = bas.right(bas.size()-di);
                            qDebug() << "readLine:" << str;
                            this->lastrecvdatatime = QDateTime::currentDateTime().toMSecsSinceEpoch();
                            return true;
                        }
                    }
                }
                timeout-=100;
                msleep(100);
            }while(timeout>0);
            return true;
        }catch(...){
        qDebug() << "catch error in readLine ";
        return false;
    }
}

bool NWThread::sendStr(QString& msg){
    try {

        if(!socket->isOpen() || this->socket->state()!=QTcpSocket::ConnectedState){
            return false;
        }
        qDebug() << "sendStr:" << msg;
        QByteArray data = msg.toUtf8();
        char mask = data[1];
        for (int i = 1; i < data.length(); i++) {
            mask ^= data[i];
        }
        QString maskstr = QString::number(((mask & 0xff) >> 4),16);
        maskstr += QString::number((mask & 0x0f),16);
        //qDebug() << "mask:" << maskstr;
        data.append('*');
        data.append(maskstr);
        data.append("\r\n");
        socket->write(data,data.length());
        this->socket->waitForBytesWritten();
        socket->flush();
        return true;
    }catch(...){
        qDebug() << "catch error in sendStr ";
        return false;
    }
}


