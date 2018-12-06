#ifndef BDTHREAD_H
#define BDTHREAD_H

#include <QObject>
#include <QThread>
#include <QDebug>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

#include <QString>
#include <bd_type.h>
#include <tool.h>
#include <QDateTime>


class BDThread : public QThread
{
        Q_OBJECT
private:

    void rd_rn_data_process(QString& str);
    void rdTXRCmdProcesss(QString& str);
public:
//    int fd;
    static double lat;
    static double lon;
    static void bd_unlock_cmd(void);
    static void device_init(void);
    static void readCARD(void);
    static void readBSI(void);

    static void bd_pos_report(void);

    BDThread(QObject *parent=0):QThread(parent)
    {
        qDebug() << "BDThread";
    }
    void run();

    void Signal_bdData(QString data)
    {
//        qDebug() << "send BD_Data signal:" << data;
        emit sig_bd_Data(data);
    }
    void Signal_unlock_result(bool success,QString msg,QString code)
    {
        qDebug() << "unlock Signal:" << success << "msg:" << msg;
        emit sig_unlock_result(success,msg,code);
    }
signals:
    //for others
    void sig_cardId_Data(QString msg);
    void sig_bd_Data(QString msg);
    void sig_unlock_result(bool success,QString msg,QString code);

};

#endif // BDTHREAD_H
