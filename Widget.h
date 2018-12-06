#ifndef WIDGET_H
#define WIDGET_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QStackedLayout>
#include <QPainter>
#include <QMessageBox>
#include <QTimer>
#include <QStringList>
#include <bd_type.h>
#include <nwthread.h>
#include "keyBoard.h"
#include "network_job.h"
#include <QProcess>
#include "gnssthread.h"
#include "suo.h"
#include <JsonConfig.h>
#include <QMouseEvent>



extern suoParam_tt suoParam;
extern PrccessParam_tt PrccessParam;
extern bool  connection_tcp ;
extern bool  connection_bd ;

extern bool  isReadCard;

#define     LABEL_WIDTH     160
#define     LABEL_HEIGHT    60
#define     EDIT_WIDTH      260
#define     EDIT_HEIGHT     60
#define     BTNSMALL_WIDTH  260
#define     BTNSMALL_HEIGHT 60
#define     BTNBIG_WIDTH    120
#define     BTNBIG_HEIGHT   120

//最大告警条数
#define     MAX_WARNING_NUMBER  200
//告警闪烁间隔
#define     WARNING_BLINK_TIME  200

//断点续传时间
#define     POS_RESUME_TIME     60*1000
//最大断点续传条数
#define     MAX_POS_RESUME      100


//北斗上报位置频率（120s）
#define     BD_POS_TIME         120*1000

//状态上报频率
#define     STATUS_REPORT_TIME  60*1000

enum task_status_t
{
    TASK_NULL,
    TASK_INIT,
    TASK_RUNNING,
    TASK_PAUSE,
    TASK_FINISH
};


typedef struct _UiParam_st
{
    int     reportfreq;         // pos report frequecy
    QString task_name;
    QString from;
    QString to;
    QString task_acquire_time;
    QString task_plan_finish_time;
    QString task_allow_offset_time;
    QString executor_code;      //

    QString phoneNumber;        // 电话号码
    QString dyncode;            // 动态码

    QString sess_name;
    QString sess_pwd;

    QString state;

    QStringList executeor_list;

    QString destory;
    QString bt_disconnected;
    QString lowpower;


}UiParam_st, *UiParam_t;

typedef struct _SystemStatus_st
{
    bool            isB2Indentity;
    bool            isInfoReport;
    bool            isGetTask;
    task_status_t   taskStatus;
    bool            isFirstLogin;
    bool            isFingerPrintInput;
    QString         userName;
    QString         userPwd;
}SystemStatus_st, *SystemStatus_t;

typedef struct _B2_Status
{
    QString check_success;
    QString check_status;
    QString check_msg;

    QString login_sucess;
    QString login_msg;
}B2_Status_st, *B2_Status_t;


class Widget : public QWidget
{
    Q_OBJECT
//    keyBoard* key;
    keyBoard key_Board;

    //system variate
    int     bsi_val[10];
    int     reception_4G;
    int     reception_BD;
    bool    connection_BT;
    int     battery_remain;

    B2_Status_t       B2_Status;
    UiParam_t         m_ui_param;
    task_status_t     task_status;
    QString           system_time;
    int               current_sec;

    int               warning_blink_times;
    QStringList       warning_stringList;
    QStringList       pos_resume_list;


    Sys_Param_t  sys_param;
    JsonConfig   J_Config;


    // widget index
    int widget_resume_index;

    int widget_B2Indentity_index;
    int widget_InfoReport_index;
    int widget_TaskGet_index;
    int widget_Login_index;
    int widget_FingerPrint_index;
    int widget_main_index;
    int widget_work_index;
    int widget_tipDialog_index;
    int widget_BsiInfo_index;
    int widget_unlockScreen_index;
    int widget_blackScreen_index;
    int widget_test_index;
    int widget_taskPause_index;
    int widget_phoneNumber_index;
    int widget_inputDyncode_index;
    int widget_warning_show_index;
    int widget_addition_index;

    // style sheet
    const char*     label_style;
    const char*     edit_style;
    const char*     btn_style;
    const char*     test_edit_style;
    QPalette        msb_Palette;
    QPalette        bg_Palette;

    NWThread*       nwthread;

private:
    int         m_double_click;
    int         pos_offset;

    //flag
    bool        isInfoReport;

    bool        fp1_success;
    bool        fp2_success;
    bool        fp3_success;
    bool        isFirstLogin;
    bool        isFirstFingerPrint;
    bool        flag_BsiInfo;
    bool        time_2min_ok;
    bool        fingerPrint_clear;
    bool        isLockScreen;
    bool        isLockScreenTimerStart;
    bool        fingerPrint_success;
    bool        isPosResume;
    bool        isSendUnlockLog;
    bool        isLowpowerRpt;

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();


protected slots:

/**************************************
*       slots of btn clicked
***************************************/

    // slot..pushBotton_clicked
    void on_btn_warning_toggle_clicked();

    void on_m_btn4_clicked();
    void on_m_btn5_clicked();

    void on_btn_B2Indentity_clicked();

    void on_btn_readCard_clicked();
    void on_btn_InfoReport_clicked();

    void on_btn_taskGet_clicked();
    void on_btn_perLogin_clicked();

    void on_btn_login_clicked();
    void on_btn_loginCancel_clicked();

    void on_btn_fingerPrint1_clicked();
    void on_btn_fingerPrint2_clicked();
    void on_btn_fingerPrint3_clicked();
    void on_btn_fingerPrint_continue_clicked();
    void on_btn_fingerPrint_end_clicked();

    void on_btn_go_to_work_clicked();
    void on_btn_lock_screen_clicked();

    void on_btn_addition_clicked();
    void on_btn_unlock4G_clicked();
    void on_btn_unlockBD_clicked();
    void on_btn_taskPause_clicked();
    void on_btn_taskResume_clicked();
    void on_btn_taskComplete_clicked();
    void on_btn_go_to_main_clicked();
//    void on_btn_infoBsi_clicked();
    void on_btn_infoBT_clicked();
    void on_btn_lock_screen2_clicked();
    void on_btn_set_clicked();
    void on_btn_closeLock_clicked();

    void on_btn_infoBsiClose_clicked();

    void on_btn_unlockScreen_clicked();
    void on_btn_unlockScreen_cancle_clicked();

    void on_btn_blackScreen_clicked();

    void on_btn_testReadCard_clicked();
    void on_btn_testReadBsi_clicked();
    void on_btn_testInfoRS_clicked();
    void on_btn_testReadLocation_clicked();

    void on_btn_taskPause_confirm_clicked();
    void on_btn_taskPause_cancle_clicked();

    void on_btn_phoneNumber1_clicked();
    void on_btn_phoneNumber2_clicked();

    void on_btn_dyncode_unlock_clicked();
    void on_btn_dyncode_cancel_clicked();

    void on_btn_warning_confirm_clicked();


    void on_btn_addFingerPrint_clicked();
    void on_btn_infoBsi_clicked();
    void on_btn_backToWork_clicked();


    // slot..4G/BD_data_proess
    void slot_cardId_data(QString msg);
    void slot_bd_bsi(QString msg);//WY
    void slot_bd_unlock(bool success, QString msg, QString code);
    void slot_4G_Data(QString str);
    void slot_loginResult(bool bSuccess,QString strResult);
    void slot_taskGetResult(bool bSuccess, QString strResult);
    void slot_InfoReportResult(bool bSuccess, const QString strResult);
    void slot_reloginResult(bool bSuccess,QString strResult);
    void slot_suo_ack(QString msg);

    void slot_B2CheckStatus(bool bSuccess, QString& strResult);
    void slot_B2LoginStatus(bool bSuccess, QString& strResult);


    //slot..timer_timeout
    void m_timer_B2_out();
    void m_timer_infoRpt_out();
    void m_timer_fingerPrint_out();
    void m_timer_refresh_out();
    void m_timer_posrpt_out();
    void m_timer_doubleClick_out();
    void m_timer_unlockBD_out();
    void m_timer_unlock4G_out();
    void m_timer_taskPause_out();
    void m_timer_taskResume_out();
    void m_timer_taskComplete_out();
    void m_timer_1s_out();
    void m_timer_2min_out();
    void m_autoLockScreen_out();
    void m_timer_starpt_out();
    void m_timer_openLock_out();
    void m_timer_taskGet_out();
    void m_timer_login_out();
    void m_timer_awake_out();
    void m_timer_warning_blink_out();
    void m_timer_posResume_out();



private:

    //Timer
    QTimer*     m_timer_doubleClick;
    QTimer*     m_timer_B2;
    QTimer*     m_timer_infoRpt;
    QTimer*     m_timer_fingerPrint;
    QTimer*     m_timer_refresh;
    QTimer*     m_timer_posrpt;
    QTimer*     m_timer_unlockBD;
    QTimer*     m_timer_unlock4G;
    QTimer*     m_timer_taskPause;
    QTimer*     m_timer_taskResume;
    QTimer*     m_timer_taskComplete;
    QTimer*     m_timer_1s;
    QTimer*     m_timer_2min;
    QTimer*     m_autoLockScreen;
    QTimer*     m_timer_starpt;
    QTimer*     m_timer_openLock;
    QTimer*     m_timer_taskGet;
    QTimer*     m_timer_login;
    QTimer*     m_timer_awake;
    QTimer*     m_timer_warning_blink;
    QTimer*     m_timer_posResume;


    //layout component
    QVBoxLayout*     v_Layout;
    QHBoxLayout*     h_Layout1;
    QHBoxLayout*     h_Layout2;
    QStackedLayout*  s_Layout;

    QPushButton*    btn_warning_toggle;
    QPushButton*    btn_warning_toggle_B2;
    QPushButton*    btn_warning_toggle_InfoRpt;
    QPushButton*    btn_warning_toggle_tastGet;
    QPushButton*    btn_warning_toggle_longin;
    QPushButton*    btn_warning_toggle_fp;
    QPushButton*    btn_warning_toggle_main;
    QPushButton*    btn_warning_toggle_work;
    QPushButton*    btn_warning_toggle_bsiInfo;
    QPushButton*    btn_warning_toggle_tipDialog;
    QPushButton*    btn_warning_toggle_taskPause;
    QPushButton*    btn_warning_toggle_inputDyncode;


    // h_Layoyt1 component
    QLabel*         m_label;

    // h_Layoyt2 component
    QPushButton*    m_btn4;
    QPushButton*    m_btn5;
    QLabel*         g_label;

    // s_Layoyt..widget_B2Indentity component
    QLabel*         label_B2Indentity;
    QLineEdit*      edit_B2Indentity;
    QPushButton*    btn_B2Indentity;
    QWidget*        widget_B2Indentity;
    QWidget*        getwidget_B2Indentity();

    // s_Layoyt..widget_InfoReport component
    QLabel*         label_bdID;
    QLabel*         label_devID;
    QLineEdit*      edit_bdID;
    QLineEdit*      edit_devID;
    QPushButton*    btn_readCard;
    QPushButton*    btn_InfoReport;
    QWidget*        widget_InfoReport;
    QWidget*        getwidget_InfoReport();


    //s_Layout..widget_TaskGet component
    QLabel*         label_task;
    QLineEdit*      edit_task;
    QPushButton*    btn_taskGet;
    QPushButton*    btn_perLogin;
    QWidget*        widget_TaskGet;
    QWidget*        getwidget_TaskGet();


    //s_Layout.. widget_Login compent
    QLabel*         label_userName;
    QLabel*         label_userPwd;
    QLineEdit*      edit_userName;
    QLineEdit*      edit_userPwd;
    QPushButton*    btn_login;
    QPushButton*    btn_loginCancel;
    QWidget*        widget_Login;
    QWidget*        getwidget_Login();


    //s_Layout..widget_FingerPrint
    QLabel*         label_fingerPrint;
    QPushButton*    btn_fingerPrint1;
    QPushButton*    btn_fingerPrint2;
    QPushButton*    btn_fingerPrint3;
    QPushButton*    btn_fingerPrint_end;
    QPushButton*    btn_fingerPrint_continue;
    QLabel*         label_fingerPrint1;
    QLabel*         label_fingerPrint2;
    QLabel*         label_fingerPrint3;
    QWidget*        widget_FingerPrint;
    QWidget*        getwidget_FingerPrint();


    //s_Layout..widget_main
    QLabel*         label_task_name;
    QLabel*         label_task_status;
    QLabel*         label_task_from;
    QLabel*         label_task_to;
    QLabel*         label_task_acquire_time;
    QLabel*         label_task_plan_finish_time;
    QLabel*         label_go_to_work;
    QLabel*         label_lock_screen;
    QLabel*      edit_task_name;
    QLabel*      edit_task_status;
    QLabel*      edit_task_from;
    QLabel*      edit_task_to;
    QLabel*      edit_task_acquire_time;
    QLabel*      edit_task_plan_finish_time;
    QPushButton*    btn_go_to_work;
    QPushButton*    btn_lock_screen;
    QWidget*        widget_main;
    QWidget*        widget_formLayout;
    QWidget*        getwidget_main();


    //s_Layout..widget_work
    QPushButton*    btn_unlock4G;
    QPushButton*    btn_unlockBD;
    QPushButton*    btn_taskPause;
    QPushButton*    btn_taskResume;
    QPushButton*    btn_taskComplete;
//    QPushButton*    btn_infoBsi;
//    QPushButton*    btn_infoBT;
    QPushButton*    btn_go_to_main;
    QPushButton*    btn_set;
    QPushButton*    btn_lock_screen2;
    QPushButton*    btn_closeLock;
    QPushButton*    btn_addition;
    QLabel*         label_addition;

    QLabel*         label_unlock4G;
    QLabel*         label_unlockBD;
    QLabel*         label_taskPause;
    QLabel*         label_taskResume;
    QLabel*         label_taskComplete;
//    QLabel*         label_infoBsi;
//    QLabel*         label_infoBT;
    QLabel*         label_go_to_main;
    QLabel*         label_set;
    QLabel*         label_lock_screen2;
    QLabel*         label_closeLock;
    QWidget*        widget_work;
    QWidget*        getwidget_work();


    //s_Layout..widget_tipDialog
    QLabel*         label_tipDialog;
    QWidget*        widget_tipDialog;
    QWidget*        getwidget_tipDialog();


    //s_Layout..widget_BsiInfo
    QPushButton*    btn_infoBsiClose;
    QWidget*        widget_BsiInfo;
    QWidget*        getwidget_BsiInfo();


    //s_Layout..widget_unlockScreen
    QLabel*         label_lsUserName;
    QLabel*         label_lsUserPwd;
    QLineEdit*      edit_lsUserName;
    QLineEdit*      edit_lsUserPwd;
    QPushButton*    btn_unlockScreen;
    QPushButton*    btn_unlockScreen_cancle;
    QWidget*        widget_unlockScreen;
    QWidget*        getwidget_unlockScreen();


    //s_Layout..widget_blackScreen
    QPushButton*    btn_blackScreen;
    QWidget*        widget_blackScreen;
    QWidget*        getwidget_blackScreen();


    // s_Layout..widget_taskPause
    QLabel*         label_taskPause_tip1;
    QLabel*         label_taskPause_tip2;
    QLabel*         label_taskPause_tip3;
    QComboBox*      comboBox_taskPause_reson;
    QPushButton*    btn_taskPause_confirm;
    QPushButton*    btn_taskPause_cancle;
    QWidget*        widget_taskPause;
    QWidget*        getwidget_taskPause();

    // s_Layout..widget_test
    QPushButton*    btn_testReadCard;
    QPushButton*    btn_testReadBsi;
    QPushButton*    btn_testInfoRS;
    QPushButton*    btn_testReadLocation;

    QWidget*        widget_test;
    QWidget*        getwidget_test();

    // s_Layout..widget_phoneNumber
    QLabel*         label_phoneNumber1;
    QLabel*         label_phoneNumber2;
    QPushButton*    btn_phoneNumber1;
    QPushButton*    btn_phoneNumber2;
    QWidget*        widget_phoneNumber;
    QWidget*        getwidget_phoneNumber();

    // s_Layout..widget_inputDyncode
    QLabel*         label_dyncode1;
    QLabel*         label_dyncode2;
    QLabel*         label_dyncode3;
    QLineEdit*      edit_dyncode;
    QPushButton*    btn_dyncode_unlock;
    QPushButton*    btn_dyncode_cancel;
    QWidget*        widget_inputDyncode;
    QWidget*        getwidget_inputDyncode();

    // s_Layout..widget_warning_show
    QPlainTextEdit* test_edit;
    QPushButton*    btn_warning_confirm;
    QWidget*        widget_warning_show;
    QWidget*        getwidget_warning_show();


    // s_Layout..widget_addition_show
    QPushButton*    btn_addFingerPrint;
    QLabel*         label_addFingerPrint;

    QPushButton*    btn_infoBsi;
    QLabel*         label_infoBsi;

    QPushButton*    btn_backToWork;
    QLabel*         label_backToWork;

    QWidget*        widget_addition;
    QWidget*        getwidget_addition_show();


/**********************************************************
                Init function
**********************************************************/
    //layout_init
    void layout_init();
    //style_init
    void style_init();
    void set_background(QWidget *widget, QString str);
    void set_btn_icon(QWidget *widget, QString str);
    //connect_init
    void connect_init();
    //my_init
    void syspara_init();

    void config_init();

    void lock_screen();

    //clear the whole information
    void information_clear();
    void current_sec_clear();

protected:
    void paintEvent(QPaintEvent *evt);
//    bool eventFilter(QObject *watched, QEvent *event);
    void mousePressEvent(QMouseEvent *e);

private:
    // draw func
    void draw_4GSinagl();
    void draw_BDSignal();
    void draw_BTSignal();
    void draw_Battery();
    void draw_Time();
    void draw_Bsi();

    //generate the messageaBox
    void generate_WaringMsb( QString text);
    void generate_InfoMsb(QString text);
    int generate_SelectMsb(QString text);


    // DIY function
    void setSystemTime(QString time);
    void autoLockScreen_start();
    void autoLockScreen_stop();
    void post_taskPause();
    void post_taskResume();
    void main_work_refresh();
    void show_label(QString str);
    void hide_label();

    void set_warning_btn(QPushButton *btn);
    void warning_blink();
    void warning_process(QString msg);
    void warning_update();

    void pos_resume_list_report();
    void screen_open();
    void screen_close();

    void B2_Login_Success();
    void B2_Login_failed(QString msg);

    QString HttpResultParseStore(QString strResult);
    QString httpInfoRptParseStore(QString strResult);
    void warn_process(QString str);
};

#endif // WIDGET_H

