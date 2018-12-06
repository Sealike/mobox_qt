#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include "keyBoard.h"

namespace Ui {
class HttpTestDialog;
}

//主窗口
class HttpTestDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit HttpTestDialog(QWidget *parent = 0);
    ~HttpTestDialog();
private slots:
    void slot_requestBtnClicked();//请求按钮点击
    void slot_requestFinished(bool bSuccess, const QString& strResult); //http请求结束
    void slot_loginBtnClicked();
    void slot_loginResult(bool bSuccess,QString strResult);
    void slot_bd_Data(QString msg);//WY
private:
    Ui::HttpTestDialog *ui;
    keyBoard *newKeyBoard;
};

#endif // MAINDIALOG_H
