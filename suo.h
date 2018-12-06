#ifndef SUO_H
#define SUO_H

#include <QObject>
#include <QThread>
#include <QDebug>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

#include <QString>
#include <bd_type.h>
#include <tool.h>
class suoThread : public QThread
{
        Q_OBJECT
private:

    void suo_recvdata_process(QString& str);
public:

    static bool suo_control(QString str);
    suoThread(QObject *parent=0):QThread(parent)
    {
        qDebug() << "GNSSThread";
    }
    void run();

    void Signal_suo_ack(QString data)
    {
        qDebug() << "send sig_suo_ack:" << data;
        emit sig_suo_ack(data);
    }
//    void Signal_unlock_result(bool success,QString msg)
//    {
//        qDebug() << "unlock Signal:" << success << "msg:" << msg;
//        emit sig_unlock_result(success,msg);
//    }
signals:
    //for others
    void sig_suo_ack(QString msg);
//    void sig_unlock_result(bool success,QString msg);

};
#endif // SUO_H
