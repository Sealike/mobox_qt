#include <QCoreApplication>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <QString>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <network_job.h>
#include <bd_type.h>
#include <tool.h>
//#include <qtimer.h>

extern PrccessParam_tt PrccessParam;
extern SysParam_tt SysParam;
//QString initsess_cmd_send(void)//动态密码应答协议
//{
//        QJsonObject obj;

//        obj.insert("role", PrccessParam.role );
//        obj.insert("seqno", PrccessParam.seqno);

//        QJsonDocument document;
//        document.setObject(obj);
//        QString json(document.toJson(QJsonDocument::Compact));
//        json="$initsess,wy,frontsvr,"+json;

//        QString check_data = make_checkSum(json);//计算校验核

//        json=json+check_data;
//        QByteArray json_out = json.toUtf8();

//        qDebug() << json_out;
//        return json_out;

//}
//void auth_cmd_send(void)//登录请求协议
//{
//    QJsonObject obj;

//    obj.insert("pwd", PrccessParam.pwd );
//    obj.insert("name", PrccessParam.name);
//    obj.insert("taskid", PrccessParam.taskid);
//    obj.insert("seqno", PrccessParam.seqno);

//    QJsonObject loc;
//    loc.insert("lon", PrccessParam.lon);
//    loc.insert("lat", PrccessParam.lat);
//    loc.insert("hgt", PrccessParam.hgt);
//    obj.insert("loc", loc);

////qDebug() << obj;
//    //json to qstring lei xing
//    QJsonDocument document;
//    document.setObject(obj);
//    QString json(document.toJson(QJsonDocument::Compact));
//    json="$auth,wy,frontsvr,"+json;

//    QString check_data = make_checkSum(json);//计算校验核
//    json=json+check_data;
//    QByteArray json_out = json.toUtf8();

//    qDebug() << json_out;
//}
QString Network_Job::pos_cmd_send(void)//send position portocol
{
    QJsonObject obj;

    obj.insert("executor", PrccessParam.executor);
    obj.insert("taskid", PrccessParam.taskid);
    obj.insert("net", "inet");

    QJsonObject data;

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss");

    data.insert("time", current_date);
    data.insert("lon", PrccessParam.lon);
    data.insert("lat", PrccessParam.lat);
    data.insert("hgt", PrccessParam.hgt);
    obj.insert("data", data);

//qDebug() << obj;
    //json to qstring lei xing
    QJsonDocument document;
    document.setObject(obj);
    QString json(document.toJson(QJsonDocument::Compact));
    json="$pos,wy,frontsvr,"+json;

//    QString check_data = make_checkSum(json);//计算校验核
//    json=json+check_data;
//    QByteArray json_out = json.toUtf8();

//    qDebug() << json_out;
    return json;
}
QString  Network_Job:: warn_cmd_send(QString str)//携行箱发送告警协议
{
    QJsonObject obj;

    obj.insert("executor", PrccessParam.executor);
    obj.insert("taskid", PrccessParam.taskid);
    obj.insert("net", "inet");

    QJsonObject data;

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss");

    data.insert("time", current_date);
    data.insert("type", str);

    obj.insert("data", data);

    QJsonObject loc;
    loc.insert("lon", PrccessParam.lon);
    loc.insert("lat", PrccessParam.lat);
    loc.insert("hgt", PrccessParam.hgt);
    obj.insert("loc", loc);

//qDebug() << obj;
    //json to qstring lei xing
    QJsonDocument document;
    document.setObject(obj);
    QString json(document.toJson(QJsonDocument::Compact));
    json="$warn,wy,frontsvr,"+json;

//    QString check_data = make_checkSum(json);//计算校验核
//    json=json+check_data;
//    QByteArray json_out = json.toUtf8();

//    qDebug() << json_out;
    return json;
}



QString Network_Job:: log_cmd_send(void)//携行箱发送操作日志
{
    QJsonObject obj;

    obj.insert("executor", PrccessParam.executor);
    obj.insert("taskid", PrccessParam.taskid);
    obj.insert("net", "inet");

    QJsonObject data;

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss");

    data.insert("time", current_date);
    data.insert("type", "unlock");
    obj.insert("data", data);

    QJsonObject loc;
    loc.insert("lon", PrccessParam.lon);
    loc.insert("lat", PrccessParam.lat);
    loc.insert("hgt", PrccessParam.hgt);
    obj.insert("loc", loc);

    qDebug() << obj;
    //json to qstring lei xing
    QJsonDocument document;
    document.setObject(obj);
    QString json(document.toJson(QJsonDocument::Compact));
    json="$log,wy,frontsvr,"+json;

//    QString check_data = make_checkSum(json);//计算校验核
//    json=json+check_data;
//    QByteArray json_out = json.toUtf8();

//    qDebug() << json_out;

    return json;
}



QString Network_Job::reqcode_cmd_send(void)//携行箱发送动态密码申请//4G开锁
{
     QJsonObject obj;

    obj.insert("executor", PrccessParam.executor);
    obj.insert("taskid", PrccessParam.taskid);
    obj.insert("seqno", PrccessParam.seqno);
    obj.insert("net", "inet");

    CreatPassword();
    QJsonObject data;
    data.insert("time", PrccessParam.Chuan);//随机串
    data.insert("hid", PrccessParam.Dev_ID);//Dev_ID
    data.insert("digest", PrccessParam.MD5_A);//MD5_A
    obj.insert("data", data);

    QJsonObject loc;
    loc.insert("lon", PrccessParam.lon);
    loc.insert("lat", PrccessParam.lat);
    loc.insert("hgt", PrccessParam.hgt);
    obj.insert("loc", loc);

//qDebug() << obj;
    //json to qstring lei xing
    QJsonDocument document;
    document.setObject(obj);
    QString json(document.toJson(QJsonDocument::Compact));
    json="$reqcode,wy,frontsvr,"+json;

//    QString check_data = make_checkSum(json);//计算校验核
//    json=json+check_data;
//    QByteArray json_out = json.toUtf8();

//    qDebug() << json;

    return json;
}
QString Network_Job::taskpau_cmd_send(QString reason)//携行箱发送任务暂停
{
    QJsonObject obj;
    if(reason == "登机")
        reason = "aboard";
    if(reason == "其他")
        reason = "other";

    obj.insert("executor", PrccessParam.executor);
    obj.insert("taskid", PrccessParam.taskid);
    obj.insert("seqno", PrccessParam.seqno);
    obj.insert("net", "inet");

//    CreatPassword();
    QJsonObject data;

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss");

    data.insert("time", current_date);
    data.insert("desc", reason);
    obj.insert("data", data);

    QJsonObject loc;
    loc.insert("lon", PrccessParam.lon);
    loc.insert("lat", PrccessParam.lat);
    loc.insert("hgt", PrccessParam.hgt);
    obj.insert("loc", loc);

//qDebug() << obj;
    //json to qstring lei xing
    QJsonDocument document;
    document.setObject(obj);
    QString json(document.toJson(QJsonDocument::Compact));
    json="$taskpau,wy,frontsvr,"+json;

//    QString check_data = make_checkSum(json);//计算校验核
//    json=json+check_data;
//    QByteArray json_out = json.toUtf8();
//    qDebug() << json_out;

    qDebug() << "++++++++++++++send task pause++++++++++++++";
    return json;

}


QString Network_Job::taskcon_cmd_send(void)//携行箱发送任务恢复请求
{
    QJsonObject obj;

    obj.insert("executor", PrccessParam.executor);
    obj.insert("taskid", PrccessParam.taskid);
    obj.insert("seqno", PrccessParam.seqno);
    obj.insert("net", "inet");

//    CreatPassword();
    QJsonObject data;

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss");

    data.insert("time", current_date);
    data.insert("desc", "reason of taskcontinue");
    obj.insert("data", data);

    QJsonObject loc;
    loc.insert("lon", PrccessParam.lon);
    loc.insert("lat", PrccessParam.lat);
    loc.insert("hgt", PrccessParam.hgt);
    obj.insert("loc", loc);

//    qDebug() << obj;
    //json to qstring lei xing
    QJsonDocument document;
    document.setObject(obj);
    QString json(document.toJson(QJsonDocument::Compact));
    json="$taskcon,wy,frontsvr,"+json;

//    QString check_data = make_checkSum(json);//计算校验核
//    json=json+check_data;
//    QByteArray json_out = json.toUtf8();

//    qDebug() << json_out;
    return json;
}

QString Network_Job::taskdone_cmd_send()
{
    QJsonObject obj;

    obj.insert("executor", PrccessParam.executor);
    obj.insert("taskid", PrccessParam.taskid);
    obj.insert("seqno", PrccessParam.seqno);
    obj.insert("net", "inet");

//    CreatPassword();
    QJsonObject data;

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss");

    data.insert("time", current_date);
    data.insert("desc", "reason of taskdone");
    obj.insert("data", data);

    QJsonObject loc;
    loc.insert("lon", PrccessParam.lon);
    loc.insert("lat", PrccessParam.lat);
    loc.insert("hgt", PrccessParam.hgt);
    obj.insert("loc", loc);

//qDebug() << obj;
    //json to qstring lei xing
    QJsonDocument document;
    document.setObject(obj);
    QString json(document.toJson(QJsonDocument::Compact));
    json="$taskdone,wy,frontsvr,"+json;

//    QString check_data = make_checkSum(json);//计算校验核
//    json=json+check_data;
//    QByteArray json_out = json.toUtf8();
//    qDebug() << json_out;

    return json;
}
QString Network_Job::sta_cmd_send(void)//携行箱发送设备转态
{
    QString batter = QString::number(PrccessParam.battery_remain,10);
    QString beam = PrccessParam.bdbsi;


    QJsonObject obj;

    obj.insert("executor", PrccessParam.executor);
    obj.insert("taskid", PrccessParam.taskid);
    obj.insert("seqno", PrccessParam.seqno);
    obj.insert("net", "inet");

//    CreatPassword();
    QJsonObject data;
    data.insert("battery", batter);
    data.insert("beam", beam);
    obj.insert("data", data);

    QJsonObject loc;
    loc.insert("lon", PrccessParam.lon);
    loc.insert("lat", PrccessParam.lat);
    loc.insert("hgt", PrccessParam.hgt);
    obj.insert("loc", loc);

//qDebug() << obj;
    //json to qstring lei xing
    QJsonDocument document;
    document.setObject(obj);
    QString json(document.toJson(QJsonDocument::Compact));
    json="$sta,wy,frontsvr,"+json;

//    QString check_data = make_checkSum(json);//计算校验核
//    json=json+check_data;
//    QByteArray json_out = json.toUtf8();

//    qDebug() << "sta_cmd_send" << json;
    return json;
}
//void ping_cmd_send(void)//ping协议
//{
//        QJsonObject obj;

//        obj.insert("from", "liuzhizhi");
//        obj.insert("to", "frontsvr");

//        QJsonDocument document;
//        document.setObject(obj);
//        QString json(document.toJson(QJsonDocument::Compact));
//        json="$ping,"+json;

//        QString check_data = make_checkSum(json);//计算校验核
//        json=json+check_data;
//        QByteArray json_out = json.toUtf8();

//        qDebug() << json_out;
//}

QString Network_Job::awake_cmd_send(QString userName, QString userPwd)
{
    QJsonObject obj;

    obj.insert("pwd", userPwd);
    obj.insert("name", userName);
    obj.insert("taskid", PrccessParam.taskid);
    obj.insert("seqno", PrccessParam.seqno);

    QJsonObject loc;
    loc.insert("lon", PrccessParam.lon);
    loc.insert("lat", PrccessParam.lat);
    loc.insert("hgt", PrccessParam.hgt);
    obj.insert("loc", loc);

    QJsonDocument document;
    document.setObject(obj);
    QString json(document.toJson(QJsonDocument::Compact));
    json="$awake,from,to,"+json;

//    QString check_data = make_checkSum(json);//计算校验核
//    json=json+check_data;
//    QByteArray json_out = json.toUtf8();

//    qDebug() << "sta_cmd_send" << json;
    return json;
}
