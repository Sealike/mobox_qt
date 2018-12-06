#include "HttpTestDialog.h"
#include "ui_HttpTestDialog.h"
#include "Http.h"
#include "nwthread.h"
#include "bdthread.h"
#include "network_job.h"
//#include "<"network_job.h>
HttpTestDialog::HttpTestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HttpTestDialog)
{
    ui->setupUi(this);
    connect(ui->requestBtn,SIGNAL(clicked()),this,SLOT(slot_requestBtnClicked()));//http
    connect(ui->pushButton_login,SIGNAL(clicked()),this,SLOT(slot_loginBtnClicked()));//login
}

HttpTestDialog::~HttpTestDialog()
{
    delete ui;
}

//login
void HttpTestDialog::slot_loginBtnClicked()
{
    qDebug() << "slot_loginBtnClicked";
    NWThread *nwthread = new NWThread(this,ui->lineEdit_username->text(),ui->lineEdit_password->text());
    connect(nwthread,SIGNAL(signal_login_result(bool,QString)),
            this,SLOT(slot_loginResult(bool,QString)));
    nwthread->start();
    //nwthread->sendStr()
}

//loginresult
void HttpTestDialog::slot_loginResult(bool bSuccess,QString strResult)
{
    if(bSuccess)
    {
        ui->resultEdit->setPlainText("login success"); //纯文本显示
    }
    else
    {
        ui->resultEdit->setPlainText("login error:"+strResult);
    }

}

//请求按钮点击
void HttpTestDialog::slot_requestBtnClicked()
{
    qDebug() << "slot_requestBtnClicked";
    Http* pHttpFun = new Http();
    QString strUrl = ui->addressEdit->text().trimmed();
    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
            this,SLOT(slot_requestFinished(bool,const QString&)));
    pHttpFun->get(strUrl);
}

//请求结束
void HttpTestDialog::slot_requestFinished(bool bSuccess, const QString &strResult)
{
    if(bSuccess)
    {
        ui->resultEdit->setPlainText(strResult); //纯文本显示
    }
    else
    {
        ui->resultEdit->setPlainText(tr("请求失败！"));
    }

}
void HttpTestDialog::slot_bd_Data(QString msg)
{

    qDebug() << "recv msg:"<<msg;
    ui->resultEdit->append(msg); //纯文本显示
    BDThread::bd_unlock_cmd();
    Network_Job::warn_cmd_send("mydsitence");
    Network_Job::reqcode_cmd_send();//4G开锁

}
