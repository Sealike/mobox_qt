#ifndef NETWORK_DATA_PROCESS_H
#define NETWORK_DATA_PROCESS_H
#include <QString>
class Network_Process
{

private:
    QString m_body;
    QStringList m_headers;

public:

    Network_Process();
    Network_Process(const Network_Process &other);
    ~Network_Process();
//    static void network_process(QString & str);//命令分类
    static void auth_cmd_process(QString str);//登录应答协议
    static void initack_cmd_process(QString str);//建链应答协议
    static void warn_cmd_process(QString str);//告警协议
    static bool taskpau_cmd_process(QString str);//任务暂停应答
    static bool taskcon_cmd_process(QString str);//任务恢复应答
    static bool taskdone_cmd_process(QString str); //任务完成应答
    static QString dyncode_cmd_process(QString str, QString &dyncode);//动态密码应答协议
    static bool awakeack_cmd_process(QString str);
    static QString Split_Pure_Json(QString str);

};

#endif // NETWORK_DATA_PROCESS_H
