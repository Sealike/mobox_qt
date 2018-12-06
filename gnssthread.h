#ifndef GNSSTHREAD_H
#define GNSSTHREAD_H

#include <QObject>
#include <QThread>
#include <QDebug>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

#include <QString>
#include <bd_type.h>
#include <tool.h>
class GNSSThread : public QThread
{
        Q_OBJECT
private:

    void gnss_data_process(QString& str);
public:

    static void gnss_device_init(void);
//    static void gnss_pos_report(void);

    GNSSThread(QObject *parent=0):QThread(parent)
    {
        qDebug() << "GNSSThread";
    }
    void run();

//    void Signal_bdData(QString data)
//    {
//        qDebug() << "send BD_Data signal:" << data;
//        emit sig_bd_Data(data);
//    }
//    void Signal_unlock_result(bool success,QString msg)
//    {
//        qDebug() << "unlock Signal:" << success << "msg:" << msg;
//        emit sig_unlock_result(success,msg);
//    }
//signals:
//    //for others
//    void sig_bd_Data(QString msg);
//    void sig_unlock_result(bool success,QString msg);

};
#endif // GNSSTHREAD_H
