#include <QCoreApplication>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <QString>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <network_data_process.h>
#include <tool.h>

/*
void Network_Process:: network_process(QString & str)//命令分类
{
    int data_top,data_bottom;
    QString framehead= "$";
    QString frametail= "\r\n";


    data_top=str.indexOf(framehead,0);
    data_bottom=str.indexOf(frametail,0);
    qDebug() << "data_top:"<<data_top<<"data_bottom:"<<data_bottom;

//检查字符串的校验核是否正确
     //   checkSum(bd_data);//check string XOR result
     //   qDebug() << bd_data;

    QString net_data=str.mid((data_top),(data_bottom-3));//去掉"*AA\r\n"字段
    qDebug() << net_data;
    qDebug() <<"Data length is:"<< net_data.length();
    printf("----------------------");
//    QStringList datalist = str.split(",");
//    //datalist.
//    QString starstr = datalist[0];
    if(net_data.startsWith("$initack")){//平台返回的登录应答协议

        initack_cmd_process(net_data);
    }else
    if (net_data.startsWith("$auth")) {//登录应答协议
        auth_cmd_process(net_data);
    }else
        if (net_data.startsWith("$warn")) {//告警协议

            warn_cmd_process(net_data);
        }else
            if (net_data.startsWith("$dyncode")) {//
                dyncode_cmd_process(net_data);
            }else
                if (net_data.startsWith("$taskpau")) {//任务暂停应答
                    taskpau_cmd_process(net_data);
                }else
                    if (net_data.startsWith("$taskcon")) {//任务恢复应答
                        taskcon_cmd_process(net_data);
                    }else
                        if (net_data.startsWith("$pong")) {//


                        }else{
        qDebug() << "input network data error";
    }
}
*/
void Network_Process:: auth_cmd_process(QString str)//登录应答协议
{
    QString buf=str.right(str.length()-sizeof("$auth,from,to"));
    QByteArray data = buf.toUtf8();

        QJsonParseError jsonError;//Qt5新类
        QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);//Qt5新类
        if (jsonError.error == QJsonParseError::NoError)
        {
            if (json.isObject())
            {
                QJsonObject rootObj = json.object();
                //是否含有key  rslt
                if (rootObj.contains("rslt"))
                {
                    QJsonValue value = rootObj.value("rslt");
                    //判断是否是string类型
                    if (value.isString())
                    {
                        QString data1 = value.toString();
                        if(data1=="ok")
                        {
                            qDebug() << "auth cmd ack rslt :" << data1;
                        }else{
                            qDebug() << "auth cmd ack rslt :" << data1;
                        }

                    }
                }
//                if (rootObj.contains("error"))
//                {
//                    QJsonValue value = rootObj.value("error");
//                    if (value.isString())
//                    {
//                        QString data2 = value.toString();
//                        qDebug() << "error : " << data2;
//                    }
//                }
            }
        }
}
void Network_Process:: initack_cmd_process(QString str)//建链应答协议
{


    QString buf=str.right(str.length()-sizeof("$initack,from,to"));
    QByteArray data = buf.toUtf8();

        QJsonParseError jsonError;//Qt5新类
        QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);//Qt5新类
        if (jsonError.error == QJsonParseError::NoError)
        {
            if (json.isObject())
            {
                QJsonObject rootObj = json.object();
                //是否含有key  rslt
                if (rootObj.contains("rslt"))
                {
                    QJsonValue value = rootObj.value("rslt");
                    //判断是否是string类型
                    if (value.isString())
                    {
                        QString data1 = value.toString();
                        if(data1=="ok")
                        {
                           qDebug() << "initack cmd ack rslt :" << data1;
                        }else{
                        qDebug() << "initack cmd ack rslt :" << data1;

                        }

                    }
                }

            }
        }
}
void Network_Process:: warn_cmd_process(QString str)//告警协议
{
    QString data1;
    QString buf = Split_Pure_Json(str);

//    QString buf=str.right(str.length()-sizeof("$warn,from,to"));
    QByteArray data = buf.toUtf8();

        QJsonParseError jsonError;//Qt5新类
        QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);//Qt5新类
        if (jsonError.error == QJsonParseError::NoError)
        {
            if (json.isObject())
            {
                QJsonObject rootObj = json.object();
                //是否含有key  rslt
                if (rootObj.contains("data"))//是否包含指定的key
                {
                    QJsonValue value = rootObj.value("data");//得到键值data所对应的数值
                    if (value.isObject())
                    {  // Page 的 value 是对象
                        QJsonObject obj = value.toObject();
                         if (obj.contains("type"))
                         {
                        QJsonValue a = obj.value("type");
                        //判断是否是string类型
                        if (a.isString())
                        {
                            data1 = a.toString();
                            if (data1=="distance"){//超距离告警
                                qDebug() << "warn cmd :"<<data1;

                            }else if (data1=="destory"){//破坏告警


                            }else if (data1=="lowpower"){//低电量告警


                            }else if (data1=="fence"){//电子围栏告警

                            }else if (data1=="dateout"){//任务执行超期告警

                            }else if (data1=="disappear"){//消失告警

                            }

                    }
                    }else
                         {
                             qDebug() << "warn cmderror";
                         }
                }

            }
        }
}
}

QString Network_Process:: Split_Pure_Json(QString str)
{
    qDebug() << "Split_Pure_Json...";

    int len = str.indexOf("{");
    //int len2 = str.indexOf("*");
    QString buf=str.right(str.length()-len);
    //buf = buf.left(buf.length()-1);

    return buf;
}

bool Network_Process:: taskpau_cmd_process(QString str)//任务暂停应答
{
    qDebug() << "taskpau_cmd_process" + str;

    bool status = false;
    QString data1;

    QString buf = Split_Pure_Json(str);

    QByteArray data = buf.toUtf8();

        QJsonParseError jsonError;//Qt5新类
        QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);//Qt5新类
        if (jsonError.error == QJsonParseError::NoError)
        {
            qDebug() << "taskpau_cmd_process 1";
            if (json.isObject())
            {
                qDebug() << "taskpau_cmd_process 2";
                QJsonObject rootObj = json.object();
                //是否含有key  rslt
                if (rootObj.contains("data"))//是否包含指定的key
                {

                    qDebug() << "taskpau_cmd_process 3";
                    QJsonValue value = rootObj.value("data");//得到键值data所对应的数值
                    if (value.isObject())
                    {  // Page 的 value 是对象
                        qDebug() << "taskpau_cmd_process 4";
                        QJsonObject obj = value.toObject();
                         if (obj.contains("rslt"))
                         {
                             qDebug() << "taskpau_cmd_process 5";
                        QJsonValue a = obj.value("rslt");
                        //判断是否是string类型
                        if (a.isString())
                        {
                            qDebug() << "taskpau_cmd_process 6";
                            data1 = a.toString();
                            if (data1=="ok"){
                                qDebug() << "taskpau task ack"<<data1;
                                status = true;
                            }else{
                                qDebug() << "taskpau_cmd_process 7";

                            }
                        }
                    }else
                         {
                             qDebug() << "taskpau cmd error";
                         }
                }

            }
        }
    }
return status;
}
bool Network_Process:: taskcon_cmd_process(QString str)//任务回复应答
{
    bool status = false;
    QString data1;

    QString buf = Split_Pure_Json(str);

    QByteArray data = buf.toUtf8();

        QJsonParseError jsonError;//Qt5新类
        QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);//Qt5新类
        if (jsonError.error == QJsonParseError::NoError)
        {
            if (json.isObject())
            {
                QJsonObject rootObj = json.object();
                //是否含有key  rslt
                if (rootObj.contains("data"))//是否包含指定的key
                {
                    QJsonValue value = rootObj.value("data");//得到键值data所对应的数值
                    if (value.isObject())
                    {  // Page 的 value 是对象
                        QJsonObject obj = value.toObject();
                         if (obj.contains("rslt"))
                         {
                        QJsonValue a = obj.value("rslt");
                        //判断是否是string类型
                        if (a.isString())
                        {
                            data1 = a.toString();
                            if (data1=="ok"){
                                status = true;
                                qDebug() << "taskcon test:"<< data1;
                            }else{

                            }
                        }
                    }else
                         {
                             qDebug() << "taskcon cmd error";
                         }
                }

            }
        }
    }
     return status;
}
bool Network_Process:: taskdone_cmd_process(QString str)//任务回复应答
{
    qDebug() << "taskpau_cmd_process" + str;

    bool status = false;
    QString data1;

    QString buf = Split_Pure_Json(str);

    QByteArray data = buf.toUtf8();

        QJsonParseError jsonError;//Qt5新类
        QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);//Qt5新类
        if (jsonError.error == QJsonParseError::NoError)
        {
            qDebug() << "taskpau_cmd_process 1";
            if (json.isObject())
            {
                qDebug() << "taskpau_cmd_process 2";
                QJsonObject rootObj = json.object();
                //是否含有key  rslt
                if (rootObj.contains("data"))//是否包含指定的key
                {

                    qDebug() << "taskpau_cmd_process 3";
                    QJsonValue value = rootObj.value("data");//得到键值data所对应的数值
                    if (value.isObject())
                    {  // Page 的 value 是对象
                        qDebug() << "taskpau_cmd_process 4";
                        QJsonObject obj = value.toObject();
                         if (obj.contains("rslt"))
                         {
                             qDebug() << "taskpau_cmd_process 5";
                        QJsonValue a = obj.value("rslt");
                        //判断是否是string类型
                        if (a.isString())
                        {
                            qDebug() << "taskpau_cmd_process 6";
                            data1 = a.toString();
                            if (data1=="ok"){
                                qDebug() << "taskpau task ack"<<data1;
                                status = true;
                            }else{
                                qDebug() << "taskpau_cmd_process 7";

                            }
                        }
                    }else
                         {
                             qDebug() << "taskpau cmd error";
                         }
                }

            }
        }
    }
return status;
}
QString Network_Process::dyncode_cmd_process(QString str, QString & dyncode)//动态密码应答协议
{
    QString ret = "default";

    QString data1;

    QString buf = Split_Pure_Json(str);
    QByteArray data = buf.toUtf8();

    QJsonParseError jsonError;//Qt5新类
    QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);//Qt5新类
    if (jsonError.error == QJsonParseError::NoError)
    {
        if (json.isObject())
        {
            QJsonObject rootObj = json.object();
            //是否含有key  rslt
            if (rootObj.contains("data"))//是否包含指定的key
            {
                QJsonValue value = rootObj.value("data");//得到键值data所对应的数值
                if (value.isObject())
                {  // Page 的 value 是对象
                     QJsonObject obj = value.toObject();
                     if (obj.contains("rslt"))
                     {
                         QJsonValue a = obj.value("rslt");
                         //判断是否是string类型
                         if (a.isString())
                         {
                             data1 = a.toString();
                             if (data1=="ok")
                             {
                                qDebug() << "agree lock application";
                                ret = "ok";
                             }
                             else
                             {
                                 qDebug() << "disagree lock application";
                                 ret = "deny";
                             }
                         }
                     if (obj.contains("code"))
                     {
                         QJsonValue a = obj.value("code");
                         //判断是否是string类型
                         if (a.isString())
                         {
                               data1 = a.toString();
                               qDebug() << "unlock password:"<<data1;
                               dyncode = data1;

//                               if(CheckPassword(data1))
//                               {
//                                   return true;
//                               }
//                               else
//                               {
//                                   return false;
//                               }
                           }
                       }
                }else
                {
                    qDebug() << "error";
                }
            }

        }
    }
    }
    return ret;
}

bool Network_Process::awakeack_cmd_process(QString str)
{
    qDebug() << "awakeack_cmd_process" + str;

    bool status = false;
    QString buf = Split_Pure_Json(str);

    QByteArray data = buf.toUtf8();

    QJsonParseError jsonError;//Qt5新类
    QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);//Qt5新类
    if (jsonError.error == QJsonParseError::NoError)
    {
        if (json.isObject())
        {
            QJsonObject rootObj = json.object();
            //是否含有key  rslt
            if (rootObj.contains("rslt"))
            {
                QJsonValue value = rootObj.value("rslt");
                //判断是否是string类型
                if (value.isString())
                {
                    QString data1 = value.toString();
                    if(data1=="ok")
                    {
                        status = true;
                       qDebug() << "awakeack rslt :" << data1;
                    }else{
                    qDebug() << "awakeack rslt :" << data1;

                    }

                }
            }

        }
    }
return status;
}
