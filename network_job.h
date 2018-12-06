#ifndef NETWORK_JOB_H
#define NETWORK_JOB_H
#include <QString>
#include <QDateTime>

class Network_Job
{

private:
    QString m_body;
    QStringList m_headers;
public:
    Network_Job();
    Network_Job(const Network_Job &other);
    ~Network_Job();

    static QString pos_cmd_send(void);//携行箱发送位置协议
    static QString warn_cmd_send(QString str);//携行箱发送告警协议
    static QString log_cmd_send(void);//携行箱发送操作日志监控协议
    //互联网-开锁
    static QString reqcode_cmd_send(void);//携行箱发送动态密码申请//4G开锁
    static QString taskpau_cmd_send(QString reason);//任务暂请求协议
    static QString taskcon_cmd_send(void);//任务恢复求协议
    static QString taskdone_cmd_send(void);//taskdone
    static QString sta_cmd_send(void);//设备状态协议
    static QString awake_cmd_send(QString userName, QString userPwd);//设备唤醒协议

};


#endif // NETWORK_JOB_H
