#ifndef B2AUTH_H
#define B2AUTH_H

#include <QObject>
#include "Http.h"

class B2Auth : public QObject
{
    Q_OBJECT
public:
    explicit B2Auth(QObject *parent = 0);
    static QString urlBase;
private:
    bool authed;
signals:
    void signal_auth_status(bool connected);
public slots:

    //void slot_requestBtnClicked();//请求按钮点击
    //void slot_requestFinished(bool bSuccess, const QString& strResult); //http请求结束
    //void slot_loginBtnClicked();
    //void slot_loginResult(bool bSuccess,QString strResult);

    void slot_nw_send_data(QString data){

    };
    void slot_get_auth_status(){

    };

    void slot_execute_auth(){
        qDebug() << "slot_execute_auth";
        //Http* pHttpFun = new Http();
        //connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
        //        this,SLOT(slot_requestFinished(bool,const QString&)));
        //pHttpFun->get(this->urlBase+"req=1016");
    };
};

#endif // B2AUTH_H
