#ifndef JSONCONFIG_H
#define JSONCONFIG_H

#include <QString>
#include <QDebug>
#include <QByteArray>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QIODevice>
#include <QTextStream>

#define CONFIG_PATH "./Resource/config.json"

typedef struct
{
    QString executeor_name;
    QString executeor_pwd;
}login_info_t;


typedef struct
{
    QString b2indentity;
    QString inforpt;
    QString istask;
    QString isfingprint;
    QString taskstatus;

} Sys_Status_t;

typedef struct
{
    QString id;
    QString name;
    QString status;
    QString from1;
    QString to1;
    QString timestart;
    QString timeend;
    QString reportfreq;

    QStringList executeor_list;         // 获取任务后存入此链表

//    QList<login_info_t> login_info_list;    // 登录成功后存入此链表

} Sys_task_t;

typedef struct
{
    QString bdid;
    QString devid;
    Sys_Status_t status;
    Sys_task_t  task;
} Sys_Param_t;


class JsonConfig
{

    private:
        Sys_Param_t  sys_param;
        QString make_Json(Sys_Param_t &sys_param);
        QString Json_Parse(QString strResult, Sys_Param_t& sys_param);
    public:
        JsonConfig();
        bool config_init(Sys_Param_t&  sys_param);
        bool config_save(Sys_Param_t&  sys_param);
        bool config_read(Sys_Param_t&  sys_param);
};

#endif // JSONCONFIG_H
