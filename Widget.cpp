#include "Widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QTabWidget>
#include <QThread>
#include <QDebug>
#include <QRectF>
#include <bd_type.h>
#include <network_job.h>
#include <network_data_process.h>
#include <bdthread.h>
#include <nwthread.h>
#include <Http.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "keyBoard.h"
#include <QDateTime>
#include <JsonConfig.h>


Widget::Widget(QWidget *parent): QWidget(parent)
{
    qDebug("Widget construct begin...");

    key_Board.hide();

    BDThread::readCARD();

    syspara_init();
    layout_init();
    connect_init();
    style_init();
    config_init();

    qDebug("Widget construct success...");
}

Widget::~Widget()
{
    qDebug("Widget desconstruct success...");
}

void Widget::syspara_init()
{
    qDebug() << "syspara_init begin...";
/**************************************************
 *             system parameter init
 **************************************************/

    resize(480,272);
    m_ui_param = new(UiParam_st);
    m_ui_param->reportfreq = 10;

    pos_offset = 30;                    // position offset of widget compent

    task_status = TASK_NULL;            // task status.

    for(int i=0; i<10; i++)             // bd bsi init
    {
        bsi_val[i] = 0;
    }

    current_sec = 0;
/**************************************************
 *             timer init
 **************************************************/
    m_timer_B2 = new QTimer;            // B2Indentity success start.

    m_timer_infoRpt = new QTimer;       // InfoReport success start.

    m_timer_fingerPrint = new QTimer;   // fingerPrint3 clicked start.

    m_timer_refresh = new QTimer;       // refresh the status of 4G/BD/BT/Bat
    m_timer_refresh->start(1000);


    m_timer_posrpt = new QTimer;        // position report...login success start.

    m_timer_doubleClick = new QTimer;   // unlock screen doubleclicked...click the screen start.
    m_double_click = 0;

    m_timer_unlockBD = new QTimer;      // timeout warn...click the BD_unlock start

    m_timer_unlock4G = new QTimer;      // timeout warn...click the 4G_unlock start

    m_timer_taskPause = new QTimer;     // timeout warn...click the taskPause start

    m_timer_taskResume = new QTimer;    // timeout warn...click the taskResume start

    m_timer_taskComplete = new QTimer;  // timeout warn...click the taskComplete start

    m_timer_1s = new QTimer;            // 1s

    m_timer_2min = new  QTimer;         // 2min

    m_autoLockScreen = new QTimer;      // auto Lock Screen

    m_timer_starpt = new QTimer;        // report the status

    m_timer_openLock = new QTimer;      //

    m_timer_taskGet = new QTimer;

    m_timer_login = new QTimer;

    m_timer_awake = new QTimer;

    m_timer_warning_blink = new QTimer;

    m_timer_posResume = new QTimer;

/**************************************************
 *             flag init
 **************************************************/

   fp1_success = false;
   fp2_success = false;
   fp3_success = false;
   isFirstLogin = true;
   isFirstFingerPrint = true;
   fingerPrint_success = false;
   isSendUnlockLog = false;
   isLowpowerRpt = false;


//   isFirstFingerPrint = false;   //test


   flag_BsiInfo = false;
   time_2min_ok = true;

   fingerPrint_clear = false;

   isLockScreen = false;

   isLockScreenTimerStart = false;

   isPosResume = false;

   qDebug() << "syspara_init success...";
}





// connect the singals and slots.
void Widget::connect_init()
{
    qDebug() << "connect_init begin...";

    connect(btn_warning_toggle,SIGNAL(clicked(bool)),this,SLOT(on_btn_warning_toggle_clicked()));
    connect(btn_warning_toggle_B2,SIGNAL(clicked(bool)),this,SLOT(on_btn_warning_toggle_clicked()));
    connect(btn_warning_toggle_InfoRpt,SIGNAL(clicked(bool)),this,SLOT(on_btn_warning_toggle_clicked()));
    connect(btn_warning_toggle_tastGet,SIGNAL(clicked(bool)),this,SLOT(on_btn_warning_toggle_clicked()));
    connect(btn_warning_toggle_longin,SIGNAL(clicked(bool)),this,SLOT(on_btn_warning_toggle_clicked()));
    connect(btn_warning_toggle_fp,SIGNAL(clicked(bool)),this,SLOT(on_btn_warning_toggle_clicked()));
    connect(btn_warning_toggle_main,SIGNAL(clicked(bool)),this,SLOT(on_btn_warning_toggle_clicked()));
    connect(btn_warning_toggle_work,SIGNAL(clicked(bool)),this,SLOT(on_btn_warning_toggle_clicked()));
    connect(btn_warning_toggle_bsiInfo,SIGNAL(clicked(bool)),this,SLOT(on_btn_warning_toggle_clicked()));
    connect(btn_warning_toggle_tipDialog,SIGNAL(clicked(bool)),this,SLOT(on_btn_warning_toggle_clicked()));
    connect(btn_warning_toggle_taskPause,SIGNAL(clicked(bool)),this,SLOT(on_btn_warning_toggle_clicked()));
    connect(btn_warning_toggle_inputDyncode,SIGNAL(clicked(bool)),this,SLOT(on_btn_warning_toggle_clicked()));

    // connect slot for button
    connect(m_btn4,SIGNAL(clicked(bool)),this,SLOT(on_m_btn4_clicked()));
    connect(m_btn5,SIGNAL(clicked(bool)),this,SLOT(on_m_btn5_clicked()));

    connect(btn_B2Indentity,SIGNAL(clicked(bool)),this,SLOT(on_btn_B2Indentity_clicked()));

    connect(btn_readCard,SIGNAL(clicked(bool)),this,SLOT(on_btn_readCard_clicked()));
    connect(btn_InfoReport,SIGNAL(clicked(bool)),this,SLOT(on_btn_InfoReport_clicked()));

    connect(btn_taskGet,SIGNAL(clicked(bool)),this,SLOT(on_btn_taskGet_clicked()));
    connect(btn_perLogin,SIGNAL(clicked(bool)),this,SLOT(on_btn_perLogin_clicked()));

    connect(btn_login,SIGNAL(clicked(bool)),this,SLOT(on_btn_login_clicked()));
    connect(btn_loginCancel,SIGNAL(clicked(bool)),this,SLOT(on_btn_loginCancel_clicked()));

//    connect(btn_fingerPrint1,SIGNAL(clickfingerPrint_successed(bool)),this,SLOT(on_btn_fingerPrint1_clicked()));
//    connect(btn_fingerPrint2,SIGNAL(clicked(bool)),this,SLOT(on_btn_fingerPrint2_clicked()));
//    connect(btn_fingerPrint3,SIGNAL(clicked(bool)),this,SLOT(on_btn_fingerPrint3_clicked()));
    connect(btn_fingerPrint_continue,SIGNAL(clicked(bool)),this,SLOT(on_btn_fingerPrint_continue_clicked()));
    connect(btn_fingerPrint_end,SIGNAL(clicked(bool)),this,SLOT(on_btn_fingerPrint_end_clicked()));

    connect(btn_go_to_work,SIGNAL(clicked(bool)),this,SLOT(on_btn_go_to_work_clicked()));
    connect(btn_lock_screen,SIGNAL(clicked(bool)),this,SLOT(on_btn_lock_screen_clicked()));

    connect(btn_addition,SIGNAL(clicked(bool)),this,SLOT(on_btn_addition_clicked()));
    connect(btn_unlock4G,SIGNAL(clicked(bool)),this,SLOT(on_btn_unlock4G_clicked()));
    connect(btn_unlockBD,SIGNAL(clicked(bool)),this,SLOT(on_btn_unlockBD_clicked()));
    connect(btn_taskPause,SIGNAL(clicked(bool)),this,SLOT(on_btn_taskPause_clicked()));
    connect(btn_taskResume,SIGNAL(clicked(bool)),this,SLOT(on_btn_taskResume_clicked()));
    connect(btn_taskComplete,SIGNAL(clicked(bool)),this,SLOT(on_btn_taskComplete_clicked()));
    connect(btn_go_to_main,SIGNAL(clicked(bool)),this,SLOT(on_btn_go_to_main_clicked()));
    connect(btn_infoBsi,SIGNAL(clicked(bool)),this,SLOT(on_btn_infoBsi_clicked()));
    connect(btn_set,SIGNAL(clicked(bool)),this,SLOT(on_btn_set_clicked()));
    connect(btn_lock_screen2,SIGNAL(clicked(bool)),this,SLOT(on_btn_lock_screen2_clicked()));
    connect(btn_closeLock,SIGNAL(clicked(bool)),this,SLOT(on_btn_closeLock_clicked()));

    connect(btn_infoBsiClose,SIGNAL(clicked(bool)),this,SLOT(on_btn_infoBsiClose_clicked()));

    connect(btn_unlockScreen,SIGNAL(clicked(bool)),this,SLOT(on_btn_unlockScreen_clicked()));
    connect(btn_unlockScreen_cancle,SIGNAL(clicked(bool)),this,SLOT(on_btn_unlockScreen_cancle_clicked()));

    connect(btn_blackScreen,SIGNAL(clicked(bool)),this,SLOT(on_btn_blackScreen_clicked()));

    connect(btn_testReadCard,SIGNAL(clicked(bool)),this,SLOT(on_btn_testReadCard_clicked()));
    connect(btn_testReadBsi,SIGNAL(clicked(bool)),this,SLOT(on_btn_testReadBsi_clicked()));
    connect(btn_testInfoRS,SIGNAL(clicked(bool)),this,SLOT(on_btn_testInfoRS_clicked()));
    connect(btn_testReadLocation,SIGNAL(clicked(bool)),this,SLOT(on_btn_testReadLocation_clicked()));

    connect(btn_taskPause_confirm,SIGNAL(clicked(bool)),this,SLOT(on_btn_taskPause_confirm_clicked()));
    connect(btn_taskPause_cancle,SIGNAL(clicked(bool)),this,SLOT(on_btn_taskPause_cancle_clicked()));

    connect(btn_phoneNumber1,SIGNAL(clicked(bool)),this,SLOT(on_btn_phoneNumber1_clicked()));
    connect(btn_phoneNumber2,SIGNAL(clicked(bool)),this,SLOT(on_btn_phoneNumber2_clicked()));

    connect(btn_dyncode_unlock,SIGNAL(clicked(bool)),this,SLOT(on_btn_dyncode_unlock_clicked()));
    connect(btn_dyncode_cancel,SIGNAL(clicked(bool)),this,SLOT(on_btn_dyncode_cancel_clicked()));

    connect(btn_warning_confirm,SIGNAL(clicked(bool)),this,SLOT(on_btn_warning_confirm_clicked()));

    connect(btn_addFingerPrint,SIGNAL(clicked(bool)),this,SLOT(on_btn_addFingerPrint_clicked()));
    connect(btn_backToWork,SIGNAL(clicked(bool)),this,SLOT(on_btn_backToWork_clicked()));


    //connect slot for timer
    connect(m_timer_B2, SIGNAL(timeout()),this,SLOT(m_timer_B2_out()));
    connect(m_timer_infoRpt,SIGNAL(timeout()),this,SLOT(m_timer_infoRpt_out()));
    connect(m_timer_fingerPrint,SIGNAL(timeout()),this,SLOT(m_timer_fingerPrint_out()));
    connect(m_timer_refresh,SIGNAL(timeout()),this,SLOT(m_timer_refresh_out()));
    connect(m_timer_posrpt,SIGNAL(timeout()),this,SLOT(m_timer_posrpt_out()));
    connect(m_timer_doubleClick,SIGNAL(timeout()),this,SLOT(m_timer_doubleClick_out()));    //
    connect(m_timer_unlockBD,SIGNAL(timeout()),this,SLOT(m_timer_unlockBD_out()));
    connect(m_timer_unlock4G,SIGNAL(timeout()),this,SLOT(m_timer_unlock4G_out()));
    connect(m_timer_taskPause,SIGNAL(timeout()),this,SLOT(m_timer_taskPause_out()));
    connect(m_timer_taskResume,SIGNAL(timeout()),this,SLOT(m_timer_taskResume_out()));
    connect(m_timer_taskComplete,SIGNAL(timeout()),this,SLOT(m_timer_taskComplete_out()));
    connect(m_timer_1s,SIGNAL(timeout()),this,SLOT(m_timer_1s_out()));
    connect(m_timer_2min,SIGNAL(timeout()),this,SLOT(m_timer_2min_out()));
    connect(m_autoLockScreen,SIGNAL(timeout()),this,SLOT(m_autoLockScreen_out()));
    connect(m_timer_starpt,SIGNAL(timeout()),this,SLOT(m_timer_starpt_out()));
    connect(m_timer_taskGet,SIGNAL(timeout()),this,SLOT(m_timer_taskGet_out()));
    connect(m_timer_login,SIGNAL(timeout()),this,SLOT(m_timer_login_out()));
    connect(m_timer_awake,SIGNAL(timeout()),this,SLOT(m_timer_awake_out()));
    connect(m_timer_warning_blink,SIGNAL(timeout()),this,SLOT(m_timer_warning_blink_out()));
    connect(m_timer_posResume,SIGNAL(timeout()),this,SLOT(m_timer_posResume_out()));

    qDebug() << "connect_init success...";
}

void Widget::layout_init()
{
    qDebug() << "layout_init begin...";

    this->resize(800,480);

    v_Layout    = new QVBoxLayout();
    h_Layout1   = new QHBoxLayout();
    h_Layout2   = new QHBoxLayout();
    s_Layout    = new QStackedLayout();

    g_label = new QLabel(this);
    g_label->setFont((QFont("Arial", 26)));
    g_label->resize(600,60);
    g_label->move(100,420);
    g_label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    g_label->setAlignment(Qt::AlignCenter);
    g_label->hide();


    // h_Layoyt1 init
    m_label = new QLabel();
    m_label->setText("");
    m_label->resize(400,30);
    m_label->move(0,0);
    h_Layout1->addWidget(m_label);
    m_label->hide();

    //h_Layout2 init
    m_btn4 = new QPushButton();
    m_btn5 = new QPushButton();
    m_btn4->setText("上一步");
    m_btn4->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_btn4->setMinimumSize(120,20);
    m_btn5->setText("下一步");
    m_btn5->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_btn5->setMinimumSize(120,20);
    h_Layout2->addWidget(m_btn4);
    h_Layout2->addWidget(g_label);
    h_Layout2->addWidget(m_btn5);


//    m_btn4->hide();
//    m_btn5->hide();


//    h_Layout2->addWidget(btn_warning_toggle);


    //s_Layout init
    widget_B2Indentity_index = s_Layout->addWidget(getwidget_B2Indentity());      // 0
    widget_InfoReport_index = s_Layout->addWidget(getwidget_InfoReport());        // 1
    widget_TaskGet_index = s_Layout->addWidget(getwidget_TaskGet());              // 2
    widget_Login_index = s_Layout->addWidget(getwidget_Login());                  // 3
    widget_FingerPrint_index = s_Layout->addWidget(getwidget_FingerPrint());      // 4
    widget_main_index = s_Layout->addWidget(getwidget_main());                    // 5
    widget_work_index = s_Layout->addWidget(getwidget_work());                    // 6
    widget_tipDialog_index = s_Layout->addWidget(getwidget_tipDialog());          // 7
    widget_BsiInfo_index = s_Layout->addWidget(getwidget_BsiInfo());              // 8
    widget_unlockScreen_index = s_Layout->addWidget(getwidget_unlockScreen());    // 9
    widget_blackScreen_index = s_Layout->addWidget(getwidget_blackScreen());      // 10
    widget_taskPause_index = s_Layout->addWidget(getwidget_taskPause());          // 11
    widget_phoneNumber_index = s_Layout->addWidget(getwidget_phoneNumber());      // 12
    widget_inputDyncode_index = s_Layout->addWidget(getwidget_inputDyncode());    // 13
    widget_warning_show_index = s_Layout->addWidget(getwidget_warning_show());    // 14
    widget_addition_index = s_Layout->addWidget(getwidget_addition_show());

    widget_test_index = s_Layout->addWidget(getwidget_test());                    //


//    v_Layout->addLayout(h_Layout1);
    v_Layout->addLayout(s_Layout);
    v_Layout->addLayout(h_Layout2);


    setLayout(v_Layout);
    s_Layout->setCurrentIndex(widget_InfoReport_index);


    qDebug() << "layout_init success...";
}



// generate widget_B2Indentity
QWidget* Widget::getwidget_B2Indentity()
{
    qDebug() << "getwidget_B2Indentity begin...";

    widget_B2Indentity = new QWidget();

    label_B2Indentity = new QLabel("认证码：",widget_B2Indentity);
    edit_B2Indentity = new QLineEdit(widget_B2Indentity);
    btn_B2Indentity = new QPushButton("B2模块认证",widget_B2Indentity);

    label_B2Indentity->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    edit_B2Indentity->setEchoMode(QLineEdit::Password);
    btn_B2Indentity->setFocusPolicy(Qt::NoFocus);

    label_B2Indentity->resize(LABEL_WIDTH, LABEL_HEIGHT);
    edit_B2Indentity->resize(EDIT_WIDTH, EDIT_HEIGHT);
    btn_B2Indentity->resize(BTNSMALL_WIDTH, BTNSMALL_HEIGHT);

    label_B2Indentity->move(110,160);
    edit_B2Indentity->move(270,160);
    btn_B2Indentity->move(270,270);


    edit_B2Indentity->setText("123456");
//    edit_B2Indentity->setPlaceholderText("请输入认证码...");

    btn_warning_toggle_B2 = new QPushButton(widget_B2Indentity);
    set_warning_btn(btn_warning_toggle_B2);

    qDebug() << "getwidget_B2Indentity success...";

    return widget_B2Indentity;
}

// generate widget_InfoReport
QWidget* Widget::getwidget_InfoReport()
{
    qDebug() << "getwidget_InfoReport begin...";
    widget_InfoReport = new QWidget();

    label_bdID = new QLabel("北斗ID: ",widget_InfoReport);
    label_devID = new QLabel("设备ID: ",widget_InfoReport);
    edit_bdID = new QLineEdit(widget_InfoReport);
    edit_devID = new QLineEdit(widget_InfoReport);
    btn_InfoReport = new QPushButton("信息上报",widget_InfoReport);
    btn_readCard = new QPushButton("读卡",widget_InfoReport);

    label_bdID->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    label_devID->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    btn_InfoReport->setFocusPolicy(Qt::NoFocus);
    btn_readCard->setFocusPolicy(Qt::NoFocus);
    edit_bdID->setReadOnly(true);
    edit_devID->setReadOnly(true);

    label_bdID->resize(LABEL_WIDTH, LABEL_HEIGHT);
    label_devID->resize(LABEL_WIDTH, LABEL_HEIGHT);
    edit_bdID->resize(EDIT_WIDTH+20, EDIT_HEIGHT);
    edit_devID->resize(EDIT_WIDTH+20, EDIT_HEIGHT);
    btn_InfoReport->resize(BTNSMALL_WIDTH+20, BTNSMALL_HEIGHT);
    btn_readCard->resize(BTNSMALL_WIDTH/3, BTNSMALL_HEIGHT);

    label_bdID->move(110,120);edit_bdID->move(270,120);btn_readCard->move(570,120);
    label_devID->move(110,200);edit_devID->move(270,200);
    btn_InfoReport->move(270,300);

    QString bd_ic_num = PrccessParam.bd_id_local;
    QString Dev_ID = PrccessParam.Dev_ID;

    edit_bdID->setText(bd_ic_num);
    edit_devID->setText(Dev_ID);

    if(isReadCard == false)
    {
        btn_readCard->show();
    }
    else{
        btn_readCard->hide();
    }


    btn_warning_toggle_InfoRpt = new QPushButton(widget_InfoReport);
    set_warning_btn(btn_warning_toggle_InfoRpt);

    qDebug() << "getwidget_InfoReport end...";
    return widget_InfoReport;
}

// generate widget_TaskGet
QWidget* Widget::getwidget_TaskGet()
{
    qDebug() << "getwidget_TaskGet begin...";

    widget_TaskGet = new QWidget;

    label_task = new QLabel("当前任务：",widget_TaskGet);
    edit_task = new QLineEdit(widget_TaskGet);
    btn_taskGet = new QPushButton("获取任务",widget_TaskGet);
    btn_perLogin = new QPushButton("登录",widget_TaskGet);

    label_task->setAlignment(Qt::AlignCenter);
    btn_taskGet->setFocusPolicy(Qt::NoFocus);
    btn_perLogin->setFocusPolicy(Qt::NoFocus);

    label_task->resize(LABEL_WIDTH*2,LABEL_HEIGHT);
    edit_task->resize(EDIT_WIDTH,EDIT_HEIGHT);
    btn_taskGet->resize(BTNSMALL_WIDTH,BTNSMALL_HEIGHT);
    btn_perLogin->resize(BTNSMALL_WIDTH,BTNSMALL_HEIGHT);

    label_task->move(270,160);
    edit_task->move(270,160);
    btn_taskGet->move(270,270);
    btn_perLogin->move(270,270);

    label_task->hide();
    edit_task->setText(PrccessParam.taskid);
    edit_task->setReadOnly(true);
    edit_task->hide();
    btn_perLogin->hide();

    btn_warning_toggle_tastGet = new QPushButton(widget_TaskGet);
    set_warning_btn(btn_warning_toggle_tastGet);
    qDebug() << "getwidget_TaskGet success...";

    return widget_TaskGet;
}


//generate widget_Login
QWidget* Widget::getwidget_Login()
{
    qDebug() << "getwidget_Login begin...";

    widget_Login = new QWidget();
    label_userName = new QLabel("用户名： ",widget_Login);
    label_userPwd = new QLabel("密    码： ",widget_Login);
    edit_userName = new QLineEdit(widget_Login);
    edit_userPwd = new QLineEdit(widget_Login);
    btn_login = new QPushButton("登录",widget_Login);
    btn_loginCancel = new QPushButton("取消",widget_Login);

    label_userName->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    label_userPwd->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    edit_userPwd->setEchoMode(QLineEdit::Password);
    btn_login->setFocusPolicy(Qt::NoFocus);
    btn_loginCancel->setFocusPolicy(Qt::NoFocus);

    label_userName->resize(LABEL_WIDTH,LABEL_HEIGHT);
    label_userPwd->resize(LABEL_WIDTH,LABEL_HEIGHT);
    edit_userName->resize(EDIT_WIDTH,EDIT_HEIGHT);
    edit_userPwd->resize(EDIT_WIDTH,EDIT_HEIGHT);
    btn_loginCancel->resize(BTNSMALL_WIDTH,BTNSMALL_HEIGHT);
    btn_login->resize(BTNSMALL_WIDTH,BTNSMALL_HEIGHT);

    label_userName->move(110,120); edit_userName->move(270,120);
    label_userPwd->move(110,200);  edit_userPwd->move(270,200);
                                   btn_login->move(270,300);
    btn_loginCancel->move(270,300);

//    edit_userName->setText("007");
//    edit_userPwd->setText("123456");
//    btn_loginCancel->setFocus();
    btn_loginCancel->hide();

    edit_userName->setFocus();

    btn_warning_toggle_longin = new QPushButton(widget_Login);
    set_warning_btn(btn_warning_toggle_longin);

    qDebug() << "getwidget_Login success...";

    return widget_Login;
}

//generate widget_FingerPrint
QWidget *Widget::getwidget_FingerPrint()
{
    qDebug() << "getwidget_FingerPrint begin...";

    widget_FingerPrint = new QWidget();

    label_fingerPrint = new QLabel("请录入指纹信息...",widget_FingerPrint);
    btn_fingerPrint1 = new QPushButton(widget_FingerPrint);
    btn_fingerPrint2 = new QPushButton(widget_FingerPrint);
    btn_fingerPrint3 = new QPushButton(widget_FingerPrint);
    label_fingerPrint1 = new QLabel(widget_FingerPrint);
    label_fingerPrint2 = new QLabel(widget_FingerPrint);
    label_fingerPrint3 = new QLabel(widget_FingerPrint);
    btn_fingerPrint_end = new QPushButton("结束录入",widget_FingerPrint);
    btn_fingerPrint_continue  = new QPushButton("继续录入",widget_FingerPrint);


    label_fingerPrint->setAlignment(Qt::AlignCenter);
    label_fingerPrint1->setAlignment(Qt::AlignCenter);
    label_fingerPrint2->setAlignment(Qt::AlignCenter);
    label_fingerPrint3->setAlignment(Qt::AlignCenter);

    btn_fingerPrint1->setFocusPolicy(Qt::NoFocus);
    btn_fingerPrint2->setFocusPolicy(Qt::NoFocus);
    btn_fingerPrint3->setFocusPolicy(Qt::NoFocus);
    btn_fingerPrint_continue->setFocusPolicy(Qt::NoFocus);
    btn_fingerPrint_end->setFocusPolicy(Qt::NoFocus);


    label_fingerPrint->resize(LABEL_WIDTH*4,LABEL_HEIGHT);
    label_fingerPrint1->resize(LABEL_WIDTH,LABEL_HEIGHT);
    label_fingerPrint2->resize(LABEL_WIDTH,LABEL_HEIGHT);
    label_fingerPrint3->resize(LABEL_WIDTH,LABEL_HEIGHT);
    btn_fingerPrint1->resize(BTNBIG_WIDTH,BTNBIG_HEIGHT);
    btn_fingerPrint2->resize(BTNBIG_WIDTH+20,BTNBIG_HEIGHT+20);
    btn_fingerPrint3->resize(BTNBIG_WIDTH,BTNBIG_HEIGHT);
    btn_fingerPrint_continue->resize(200,60);
    btn_fingerPrint_end->resize(200,60);


    label_fingerPrint->move(80,120);

    btn_fingerPrint1->move(110,210);    btn_fingerPrint2->move(330,210);   btn_fingerPrint3->move(600,210);
    label_fingerPrint1->move(110,370);  label_fingerPrint2->move(330,370); label_fingerPrint3->move(600,370);
    btn_fingerPrint_continue->move(110,350);                               btn_fingerPrint_end->move(490,350);


    btn_fingerPrint1->hide();
    label_fingerPrint1->hide();
    btn_fingerPrint3->hide();
    btn_fingerPrint3->hide();

    btn_warning_toggle_fp= new QPushButton(widget_FingerPrint);
    set_warning_btn(btn_warning_toggle_fp);
    qDebug() << "getwidget_FingerPrint success...";

    return widget_FingerPrint;
}

//generate widget_main
QWidget *Widget::getwidget_main()
{
    qDebug() << "getwidget_main begin...";
    int space = (800 - BTNBIG_WIDTH*4)/5;

    widget_main = new QWidget;
    label_task_name = new QLabel("任务名称：",widget_main);
    label_task_status = new QLabel("任务状态：",widget_main);
    label_task_from = new QLabel("出  发  地：",widget_main);
    label_task_to = new QLabel("目  的  地：",widget_main);
    label_task_acquire_time = new QLabel("开始时间：",widget_main);
    label_task_plan_finish_time = new QLabel("结束时间：",widget_main);
    edit_task_name = new QLabel(widget_main);
    edit_task_status = new QLabel(widget_main);
    edit_task_from = new QLabel(widget_main);
    edit_task_to = new QLabel(widget_main);
    edit_task_acquire_time = new QLabel(widget_main);
    edit_task_plan_finish_time = new QLabel(widget_main);
    btn_go_to_work = new QPushButton( widget_main);
    btn_lock_screen = new QPushButton(widget_main);
    label_go_to_work = new QLabel("工作",widget_main);
    label_lock_screen = new QLabel("锁屏",widget_main);




    btn_go_to_work->setFocusPolicy(Qt::NoFocus);
    btn_lock_screen->setFocusPolicy(Qt::NoFocus);
    label_go_to_work->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    label_lock_screen->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    label_task_name->resize(LABEL_WIDTH, LABEL_HEIGHT-20);
    label_task_name->resize(LABEL_WIDTH, LABEL_HEIGHT-20);
    label_task_status->resize(LABEL_WIDTH, LABEL_HEIGHT-20);
    label_task_from->resize(LABEL_WIDTH, LABEL_HEIGHT-20);
    label_task_to->resize(LABEL_WIDTH, LABEL_HEIGHT-20);
    label_task_acquire_time->resize(LABEL_WIDTH, LABEL_HEIGHT-20);
    label_task_plan_finish_time->resize(LABEL_WIDTH, LABEL_HEIGHT-20);

    edit_task_name->resize(LABEL_WIDTH*2, LABEL_HEIGHT-20);
    edit_task_status->resize(LABEL_WIDTH*2, LABEL_HEIGHT-20);
    edit_task_from->resize(LABEL_WIDTH*2, LABEL_HEIGHT-20);
    edit_task_to->resize(LABEL_WIDTH*2, LABEL_HEIGHT-20);
    edit_task_acquire_time->resize(LABEL_WIDTH*2, LABEL_HEIGHT-20);
    edit_task_plan_finish_time->resize(LABEL_WIDTH*2, LABEL_HEIGHT-20);
    btn_lock_screen->resize(BTNBIG_WIDTH, BTNBIG_HEIGHT);
    btn_go_to_work->resize(BTNBIG_WIDTH, BTNBIG_HEIGHT);
    label_go_to_work->resize(BTNBIG_WIDTH, LABEL_HEIGHT-20);
    label_lock_screen->resize(BTNBIG_WIDTH, LABEL_HEIGHT-20);



    label_task_name->move(80, 80);
    label_task_status->move(80, 130);
    label_task_from->move(80, 180);
    label_task_to->move(80, 230);
    label_task_acquire_time->move(80, 280);
    label_task_plan_finish_time->move(80, 330);

    edit_task_name->move(240, 80);
    edit_task_status->move(240, 130);
    edit_task_from->move(240, 180);
    edit_task_to->move(240, 230);
    edit_task_acquire_time->move(240, 280);
    edit_task_plan_finish_time->move(240, 330);

    btn_go_to_work->move(space*4+BTNBIG_WIDTH*3,80);
    label_go_to_work->move(space*4+BTNBIG_WIDTH*3,200);

    btn_lock_screen->move(space*4+BTNBIG_WIDTH*3,270);
    label_lock_screen->move(space*4+BTNBIG_WIDTH*3,390);


    task_status = TASK_NULL;

    btn_warning_toggle_main= new QPushButton(widget_main);
    set_warning_btn(btn_warning_toggle_main);
    qDebug() << "getwidget_main success...";

    return widget_main;
}

//generate widget_work
QWidget *Widget::getwidget_work()
{
    qDebug() << "getwidget_work begin...";

    widget_work = new QWidget();

    btn_unlock4G = new QPushButton(widget_work);
    btn_unlockBD = new QPushButton(widget_work);
    btn_taskPause = new QPushButton(widget_work);
    btn_taskResume = new QPushButton(widget_work);
    btn_taskComplete = new QPushButton(widget_work);
//    btn_infoBsi = new QPushButton(widget_work);
//    btn_infoBT = new QPushButton(widget_work);
    btn_go_to_main = new QPushButton(widget_work);
    btn_set = new QPushButton(widget_work);
    btn_lock_screen2 = new QPushButton(widget_work);
//    btn_infoBT = new QPushButton(widget_work);
    btn_closeLock = new QPushButton(widget_work);
    btn_addition = new QPushButton(widget_work);
    label_addition = new QLabel(widget_work);

    label_unlock4G = new QLabel(widget_work);
    label_unlockBD = new QLabel(widget_work);
    label_taskPause = new QLabel(widget_work);
    label_taskResume = new QLabel(widget_work);
    label_taskComplete = new QLabel(widget_work);
//    label_infoBsi = new QLabel(widget_work);
//    label_infoBT = new QLabel(widget_work);
    label_go_to_main = new QLabel(widget_work);
    label_set = new QLabel(widget_work);
    label_lock_screen2 = new QLabel(widget_work);
    label_closeLock = new QLabel(widget_work);



/*
*       label_addition       btn_unlock4G    btn_unlockBD        btn_go_to_main
*
*       btn_closeLock         btn_taskPause   btn_taskComplete    btn_lock_screen2
*                       (btn_taskResume)
*/

//    label_infoBsi->setText("北斗波束");
    label_addition->setText("其他功能");
    label_set->setText("系统设置");
    label_unlock4G->setText("4G开锁");
    label_taskPause->setText("任务暂停");
    label_taskResume->setText("任务恢复");
    label_unlockBD->setText("北斗开锁");
    label_taskComplete->setText("任务完成");
    label_go_to_main->setText("返回");
    label_lock_screen2->setText("锁屏");
    label_closeLock->setText("手动关锁");

// col 1
    btn_addition->resize(BTNBIG_WIDTH,BTNBIG_HEIGHT);
    btn_addition->move(64,80);
    label_addition->resize(BTNBIG_WIDTH,LABEL_HEIGHT-20);
    label_addition->move(64,200);
    label_addition->setAlignment(Qt::AlignCenter);
    btn_addition->setFocusPolicy(Qt::NoFocus);

    btn_set->resize(BTNBIG_WIDTH,BTNBIG_HEIGHT);
    btn_set->move(64,270);
    label_set->resize(BTNBIG_WIDTH,LABEL_HEIGHT-20);
    label_set->move(64,390);
    label_set->setAlignment(Qt::AlignCenter);
    btn_set->setFocusPolicy(Qt::NoFocus);

    btn_set->hide();
    label_set->hide();

    btn_closeLock->resize(BTNBIG_WIDTH,BTNBIG_HEIGHT);
    btn_closeLock->move(64,270);
    label_closeLock->resize(BTNBIG_WIDTH,LABEL_HEIGHT-20);
    label_closeLock->move(64,390);
    label_closeLock->setAlignment(Qt::AlignCenter);
    btn_closeLock->setFocusPolicy(Qt::NoFocus);



// col 2
    btn_unlock4G->resize(BTNBIG_WIDTH,BTNBIG_HEIGHT);
    btn_unlock4G->move(248,80);
    label_unlock4G->resize(BTNBIG_WIDTH,LABEL_HEIGHT-20);
    label_unlock4G->move(248,200);
    label_unlock4G->setAlignment(Qt::AlignCenter);

    btn_taskPause->resize(BTNBIG_WIDTH,BTNBIG_HEIGHT);
    btn_taskPause->move(248,270);
    label_taskPause->resize(BTNBIG_WIDTH,LABEL_HEIGHT-20);
    label_taskPause->move(248,390);
    label_taskPause->setAlignment(Qt::AlignCenter);
    btn_taskPause->setFocusPolicy(Qt::NoFocus);

    btn_taskResume->resize(BTNBIG_WIDTH,BTNBIG_HEIGHT);;
    btn_taskResume->move(248,270);
    label_taskResume->resize(BTNBIG_WIDTH,LABEL_HEIGHT-20);
    label_taskResume->move(248,390);
    label_taskResume->setAlignment(Qt::AlignCenter);
    btn_taskResume->setFocusPolicy(Qt::NoFocus);

// col 3
    btn_unlockBD->resize(BTNBIG_WIDTH,BTNBIG_HEIGHT);
    btn_unlockBD->move(432,80);
    label_unlockBD->resize(BTNBIG_WIDTH,LABEL_HEIGHT-20);
    label_unlockBD->move(432,200);
    label_unlockBD->setAlignment(Qt::AlignCenter);
    btn_unlockBD->setFocusPolicy(Qt::NoFocus);

    btn_taskComplete->resize(BTNBIG_WIDTH,BTNBIG_HEIGHT);
    btn_taskComplete->move(432,270);
    label_taskComplete->resize(BTNBIG_WIDTH,LABEL_HEIGHT-20);
    label_taskComplete->move(432,390);
    label_taskComplete->setAlignment(Qt::AlignCenter);
    btn_taskComplete->setFocusPolicy(Qt::NoFocus);

// col 4
    btn_go_to_main->resize(BTNBIG_WIDTH,BTNBIG_HEIGHT);
    btn_go_to_main->move(616,80);
    label_go_to_main->resize(BTNBIG_WIDTH,LABEL_HEIGHT-20);
    label_go_to_main->move(616,200);
    label_go_to_main->setAlignment(Qt::AlignCenter);
    btn_go_to_main->setFocusPolicy(Qt::NoFocus);

    btn_lock_screen2->resize(BTNBIG_WIDTH,BTNBIG_HEIGHT);
    btn_lock_screen2->move(616,270);
    label_lock_screen2->resize(BTNBIG_WIDTH,LABEL_HEIGHT-20);
    label_lock_screen2->move(616,390);
    label_lock_screen2->setAlignment(Qt::AlignCenter);
    btn_lock_screen2->setFocusPolicy(Qt::NoFocus);

    btn_taskResume->hide();
    label_taskResume->hide();
//    btn_infoBT->hide();
//    label_infoBT->hide();


    btn_warning_toggle_work= new QPushButton(widget_work);
    set_warning_btn(btn_warning_toggle_work);
    qDebug() << "getwidget_work success...";

    return widget_work;
}

//generate getwidget_tipDialog
QWidget *Widget::getwidget_tipDialog()
{
    qDebug() << "getwidget_tipDialog begin...";

    widget_tipDialog = new QWidget();
    label_tipDialog = new QLabel;
    label_tipDialog->setParent(widget_tipDialog);
    label_tipDialog->setAlignment(Qt::AlignCenter);
    label_tipDialog->setText("");
    label_tipDialog->resize(600,60);
    label_tipDialog->move(100,120);

    btn_warning_toggle_tipDialog= new QPushButton(widget_tipDialog);
    set_warning_btn(btn_warning_toggle_tipDialog);
    qDebug() << "getwidget_tipDialog success...";

    return widget_tipDialog;
}

//generate widget_BsiInfo
QWidget *Widget::getwidget_BsiInfo()
{
    qDebug() << "getwidget_BsiInfo begin...";

    widget_BsiInfo = new QWidget;
    btn_infoBsiClose = new QPushButton();
    btn_infoBsiClose->setText("关闭");
    btn_infoBsiClose->setParent(widget_BsiInfo);
    btn_infoBsiClose->move(270,350);
    btn_infoBsiClose->resize(BTNSMALL_WIDTH,BTNSMALL_HEIGHT);
    btn_infoBsiClose->setFocusPolicy(Qt::NoFocus);

    btn_warning_toggle_bsiInfo= new QPushButton(widget_BsiInfo);
    set_warning_btn(btn_warning_toggle_bsiInfo);
    qDebug() << "getwidget_BsiInfo success...";

    return widget_BsiInfo;
}


//generate_widget_unlockScreen
QWidget *Widget::getwidget_unlockScreen()
{
    qDebug() << "getwidget_unlockScreen begin...";

    widget_unlockScreen = new QWidget;
    label_lsUserName = new QLabel;
    label_lsUserPwd = new QLabel;
    edit_lsUserName = new QLineEdit;
    edit_lsUserPwd = new QLineEdit;
    btn_unlockScreen = new QPushButton;
    btn_unlockScreen_cancle = new QPushButton;

    label_lsUserName->setParent(widget_unlockScreen);
    label_lsUserPwd->setParent(widget_unlockScreen);
    edit_lsUserName->setParent(widget_unlockScreen);
    edit_lsUserPwd->setParent(widget_unlockScreen);
    btn_unlockScreen->setParent(widget_unlockScreen);
    btn_unlockScreen_cancle->setParent(widget_unlockScreen);

    label_lsUserName->setText("用户名： ");
    label_lsUserPwd->setText("密    码： ");
    btn_unlockScreen->setText("登录");
    btn_unlockScreen->setFocusPolicy(Qt::NoFocus);
    btn_unlockScreen_cancle->setText("取消");
    btn_unlockScreen_cancle->setFocusPolicy(Qt::NoFocus);
    label_lsUserName->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    label_lsUserPwd->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    label_lsUserName->resize(LABEL_WIDTH,LABEL_HEIGHT);
    label_lsUserPwd->resize(LABEL_WIDTH,LABEL_HEIGHT);
    edit_lsUserName->resize(EDIT_WIDTH,EDIT_HEIGHT);
    edit_lsUserPwd->resize(EDIT_WIDTH,EDIT_HEIGHT);
    btn_unlockScreen->resize(BTNSMALL_WIDTH/2,BTNSMALL_HEIGHT);
    btn_unlockScreen_cancle->resize(BTNSMALL_WIDTH/2,BTNSMALL_HEIGHT);

    label_lsUserName->move(110,120);  edit_lsUserName->move(270,120);
    label_lsUserPwd->move(110,200);   edit_lsUserPwd->move(270,200);
    btn_unlockScreen->move(430,300); btn_unlockScreen_cancle->move(240,300);

    edit_lsUserPwd->setEchoMode(QLineEdit::Password);

    qDebug() << "getwidget_unlockScreen success...";

    return widget_unlockScreen;
}

//generate_widget_blackScreen
QWidget *Widget::getwidget_blackScreen()
{
    qDebug() << "getwidget_blackScreen begin...";

    widget_blackScreen = new QWidget;

    btn_blackScreen = new QPushButton;
    btn_blackScreen->setParent(widget_blackScreen);
    btn_blackScreen->move(0,0);
    btn_blackScreen->resize(900,600);
    btn_blackScreen->setText("");
    btn_blackScreen->setFocusPolicy(Qt::NoFocus);

    //set the background to black
    this->setAutoFillBackground(true);
    this->setPalette(QColor(0,0,0));

    qDebug() << "getwidget_blackScreen success...";

    return widget_blackScreen;
}




//generate_widget_taskPause
QWidget *Widget::getwidget_taskPause()
{
    widget_taskPause = new QWidget();


    label_taskPause_tip1 = new QLabel("你确定要暂停任务吗？",widget_taskPause);
    label_taskPause_tip2 = new QLabel("选择原因后请点击“确定”，或者“取消”。",widget_taskPause);
    label_taskPause_tip3 = new QLabel("暂停原因",widget_taskPause);
    comboBox_taskPause_reson = new QComboBox(widget_taskPause);
    btn_taskPause_confirm = new QPushButton("确定",widget_taskPause);
    btn_taskPause_cancle = new QPushButton("取消",widget_taskPause);

    label_taskPause_tip1->resize(600,60);
    label_taskPause_tip2->resize(600,60);
    label_taskPause_tip3->resize(600,60);
    comboBox_taskPause_reson->resize(400,60);
    btn_taskPause_cancle->resize(200,60);
    btn_taskPause_confirm->resize(200,60);

    btn_taskPause_cancle->setFocusPolicy(Qt::NoFocus);
    btn_taskPause_confirm->setFocusPolicy(Qt::NoFocus);

    label_taskPause_tip1->setAlignment(Qt::AlignCenter);
    label_taskPause_tip2->setAlignment(Qt::AlignCenter);
    label_taskPause_tip3->setAlignment(Qt::AlignCenter);
//    comboBox_taskPause_reson->setAlignment(Qt::AlignCenter);


    label_taskPause_tip1->move(100,80);
    label_taskPause_tip2->move(100,140);
    label_taskPause_tip3->move(100,220);
    comboBox_taskPause_reson->move(200,220);
    btn_taskPause_cancle->move(170,330);
    btn_taskPause_confirm->move(430,330);

    label_taskPause_tip3->hide();

    comboBox_taskPause_reson->addItem("登机");
    comboBox_taskPause_reson->addItem("其他");


    btn_warning_toggle_taskPause= new QPushButton(widget_taskPause);
    set_warning_btn(btn_warning_toggle_taskPause);
    return widget_taskPause;
}

//generate_widget_test
QWidget *Widget::getwidget_test()
{
    widget_test = new QWidget;

    btn_testReadCard = new QPushButton(widget_test);
    btn_testReadBsi = new QPushButton(widget_test);
    btn_testInfoRS = new QPushButton(widget_test);
    btn_testReadLocation = new QPushButton(widget_test);

    btn_testReadCard->setText("unLock");
    btn_testReadBsi->setText("Lock");
    btn_testInfoRS->setText("figerPrint");
    btn_testReadLocation->setText("clear");

    btn_testReadCard->resize(BTNBIG_WIDTH,BTNBIG_HEIGHT);
    btn_testReadBsi->resize(BTNBIG_WIDTH,BTNBIG_HEIGHT);
    btn_testInfoRS->resize(BTNBIG_WIDTH,BTNBIG_HEIGHT);;
    btn_testReadLocation->resize(BTNBIG_WIDTH,BTNBIG_HEIGHT);

    btn_testReadCard->move(248,80);
    btn_testReadBsi->move(248,270);
    btn_testInfoRS->move(432,80);
    btn_testReadLocation->move(432,270);

    return widget_test;
}

//generate_widget_phoneNumber
QWidget *Widget::getwidget_phoneNumber()
{
    widget_phoneNumber = new QWidget;
    label_phoneNumber1 = new QLabel(widget_phoneNumber);
    label_phoneNumber2 = new QLabel(widget_phoneNumber);
    btn_phoneNumber1 = new QPushButton("锁屏",widget_phoneNumber);
    btn_phoneNumber2 = new QPushButton(widget_phoneNumber);

    label_phoneNumber1->resize(600,60);
    label_phoneNumber2->resize(600,60);
    btn_phoneNumber1->resize(200,60);
    btn_phoneNumber2->resize(200,60);

    label_phoneNumber1->setAlignment(Qt::AlignCenter);
    label_phoneNumber2->setAlignment(Qt::AlignCenter);
    btn_phoneNumber1->setFocusPolicy(Qt::NoFocus);
    btn_phoneNumber2->setFocusPolicy(Qt::NoFocus);
    label_phoneNumber2->setFont((QFont("Arial", 100)));

    label_phoneNumber1->move(100,80);
    label_phoneNumber2->move(100,180);
//    btn_phoneNumber1->move(170,330);
//    btn_phoneNumber2->move(430,330);
    btn_phoneNumber1->move(300,330);
    btn_phoneNumber2->move(300,330);
    btn_phoneNumber2->hide();

    label_phoneNumber1->setText("紧急联系电话：");
    label_phoneNumber2->setText(m_ui_param->phoneNumber);

    return widget_phoneNumber;
}

//generate_widget_inputDyncode
QWidget *Widget::getwidget_inputDyncode()
{
    widget_inputDyncode = new QWidget;

    label_dyncode1 = new QLabel("收到动态码： ",widget_inputDyncode);
    label_dyncode2 = new QLabel(widget_inputDyncode);
    label_dyncode3 = new QLabel("请输入动态码： ",widget_inputDyncode);
    edit_dyncode = new QLineEdit(widget_inputDyncode);
    btn_dyncode_unlock = new QPushButton("开锁",widget_inputDyncode);
    btn_dyncode_cancel = new QPushButton("取消",widget_inputDyncode);


    label_dyncode1->resize(300,60);
    label_dyncode2->resize(300,100);
    label_dyncode3->resize(300,60);
    edit_dyncode->resize(200,60);
    btn_dyncode_unlock->resize(200,60);
    btn_dyncode_cancel->resize(200,60);

    label_dyncode1->setAlignment(Qt::AlignRight| Qt::AlignVCenter);
    label_dyncode2->setAlignment(Qt::AlignLeft| Qt::AlignVCenter);
    label_dyncode3->setAlignment(Qt::AlignRight| Qt::AlignVCenter);
    label_dyncode2->setFont((QFont("Arial", 60)));
    edit_dyncode->setFont((QFont("Arial", 60)));
    btn_dyncode_unlock->setFocusPolicy(Qt::NoFocus);
    btn_dyncode_cancel->setFocusPolicy(Qt::NoFocus);
    edit_dyncode->clearFocus();

    label_dyncode1->move(100,100);        label_dyncode2->move(400,80);
    label_dyncode3->move(100,200);        edit_dyncode->move(400,200);
    btn_dyncode_unlock->move(430,330);    btn_dyncode_cancel->move(170,330);


    btn_warning_toggle_inputDyncode= new QPushButton(widget_inputDyncode);
    set_warning_btn(btn_warning_toggle_inputDyncode);

    return widget_inputDyncode;
}

//
QWidget *Widget::getwidget_warning_show()
{
    widget_warning_show = new QWidget;

    test_edit = new QPlainTextEdit(widget_warning_show);
    btn_warning_confirm = new QPushButton("返回",widget_warning_show);

    test_edit->resize(600,280);
    btn_warning_confirm->resize(200,60);
    btn_warning_confirm->setFocusPolicy(Qt::NoFocus);

    test_edit->move(100,80);
    btn_warning_confirm->move(300,380);


    btn_warning_toggle = new QPushButton(widget_warning_show);
    set_warning_btn(btn_warning_toggle);

    return widget_warning_show;
}

QWidget *Widget::getwidget_addition_show()
{
    widget_addition = new QWidget();

    btn_addFingerPrint = new QPushButton(widget_addition);
    btn_infoBsi = new QPushButton(widget_addition);
    btn_backToWork = new QPushButton(widget_addition);
    label_addFingerPrint = new QLabel("增加指纹",widget_addition);
    label_infoBsi = new QLabel("北斗波束",widget_addition);
    label_backToWork =new QLabel("返回",widget_addition);

    btn_addFingerPrint->resize(BTNBIG_WIDTH,BTNBIG_HEIGHT);
    btn_infoBsi->resize(BTNBIG_WIDTH,BTNBIG_HEIGHT);
    btn_backToWork->resize(BTNBIG_WIDTH,BTNBIG_HEIGHT);
    label_addFingerPrint->resize(BTNBIG_WIDTH,LABEL_HEIGHT-20);
    label_infoBsi->resize(BTNBIG_WIDTH,LABEL_HEIGHT-20);
    label_backToWork->resize(BTNBIG_WIDTH,LABEL_HEIGHT-20);

    btn_addFingerPrint->setFocusPolicy(Qt::NoFocus);
    label_addFingerPrint->setAlignment(Qt::AlignCenter);

    btn_infoBsi->setFocusPolicy(Qt::NoFocus);
    label_infoBsi->setAlignment(Qt::AlignCenter);

    btn_backToWork->setFocusPolicy(Qt::NoFocus);
    label_backToWork->setAlignment(Qt::AlignCenter);


/*
*       btn_infoBsi(64,80)                ********(248,80)      ********(432,80)        btn_backToWork(616,80)
*                  (64,200)                       (248,200)             (432,200)                     (616,200)
*
*       btn_addFingerPrint(64,270)         ********(248,270)     ********(432,270)       *************(616,270)
*                         (64,390)                 (248,390)             (432,390)                    (616,390)
*/

    btn_infoBsi->move(64,80);
    label_infoBsi->move(64,200);

    btn_addFingerPrint->move(64,270);
    label_addFingerPrint->move(64,390);

    btn_backToWork->move(616,80);
    label_backToWork->move(616,200);

    return widget_addition;
}


// paint event
void Widget::paintEvent(QPaintEvent *evt)
{
    evt = evt;
    draw_4GSinagl();
    draw_BDSignal();
    draw_BTSignal();
    draw_Battery();
    draw_Time();
    draw_Bsi();
}



void Widget::mousePressEvent(QMouseEvent *e)
{
   QPoint pos =  e->pos();

   int index = s_Layout->currentIndex();

   if (index == widget_Login_index)
   {
       int x1 = edit_userName->x();
       int y1 = edit_userName->y();
       int x2 = edit_userName->x()+edit_userName->width();
       int y2 = edit_userName->y()+edit_userName->height();

       int x3 = edit_userPwd->x();
       int y3 = edit_userPwd->y();
       int x4 = edit_userPwd->x()+edit_userPwd->width();
       int y4 = edit_userPwd->y()+edit_userPwd->height();

       if(pos.x() >= x1 && pos.x() <= x2 && pos.y() >= y1 && pos.y() <= y2)
       {
           edit_userName->clearFocus();
           edit_userName->setFocus();
           key_Board.show();
       }
       if(pos.x() >= x3 && pos.x() <= x4 && pos.y() >= y3 && pos.y() <= y4)
       {
           edit_userPwd->clearFocus();
           edit_userPwd->setFocus();
           key_Board.show();
       }
       else
       {
           edit_userName->clearFocus();
           edit_userPwd->clearFocus();
           key_Board.hide();
       }
   }
   if( index == widget_unlockScreen_index)
   {

       int x1 = edit_lsUserName->x();
       int y1 = edit_lsUserName->y();
       int x2 = edit_lsUserName->x()+edit_lsUserName->width();
       int y2 = edit_lsUserName->y()+edit_lsUserName->height();

       int x3 = edit_lsUserPwd->x();
       int y3 = edit_lsUserPwd->y();
       int x4 = edit_lsUserPwd->x()+edit_lsUserPwd->width();
       int y4 = edit_lsUserPwd->y()+edit_lsUserPwd->height();

       if(pos.x() >= x1 && pos.x() <= x2 && pos.y() >= y1 && pos.y() <= y2)
       {
           edit_lsUserName->clearFocus();
           edit_lsUserName->setFocus();
           key_Board.show();
       }
       if(pos.x() >= x3 && pos.x() <= x4 && pos.y() >= y3 && pos.y() <= y4)
       {
           edit_lsUserPwd->clearFocus();
           edit_lsUserPwd->setFocus();
           key_Board.show();
       }
       else
       {
           edit_lsUserName->clearFocus();
           edit_lsUserPwd->clearFocus();
           key_Board.hide();
       }
   }
   if(index == widget_inputDyncode_index)
   {
       int x1 = edit_dyncode->x();
       int y1 = edit_dyncode->y();
       int x2 = edit_dyncode->x()+edit_dyncode->width();
       int y2 = edit_dyncode->y()+edit_dyncode->height();

       if(pos.x() >= x1 && pos.x() <= x2 && pos.y() >= y1 && pos.y() <= y2)
       {
           edit_dyncode->clearFocus();
           edit_dyncode->setFocus();
           key_Board.show();
       }
       else
       {
           edit_dyncode->clearFocus();
           key_Board.hide();
       }
   }
}

// draw the 4G singal
void Widget::draw_4GSinagl()
{

    QPainter painter(this);
    painter.save();
    QPoint G4_begin(QPoint(150,30));
    int w=4,s=2,h1=4,h2=8,h3=12,h4=16,h5=20;
    QPen pen_4G;
    pen_4G.setColor(Qt::gray);
    pen_4G.setStyle(Qt::SolidLine);
    pen_4G.setWidthF(1);
    painter.setPen(pen_4G);
    painter.setBrush(Qt::green);
    painter.setFont(QFont("Arial", 24));
    painter.drawText(G4_begin.x(),G4_begin.y()+30,"4G");


    switch (reception_4G) {
    case 0:
        painter.setBrush(Qt::gray);
        painter.drawRect(G4_begin.x()+30+0*(w+s), G4_begin.y()+30-h1, w, h1);
        painter.drawRect(G4_begin.x()+30+1*(w+s), G4_begin.y()+30-h2, w, h2);
        painter.drawRect(G4_begin.x()+30+2*(w+s), G4_begin.y()+30-h3, w, h3);
        painter.drawRect(G4_begin.x()+30+3*(w+s), G4_begin.y()+30-h4, w, h4);
        painter.drawRect(G4_begin.x()+30+4*(w+s), G4_begin.y()+30-h5, w, h5);
        painter.setBrush(Qt::green);
        break;
    case 1:
        painter.setBrush(Qt::green);
        painter.drawRect(G4_begin.x()+30+0*(w+s), G4_begin.y()+30-h1, w, h1);
        painter.setBrush(Qt::gray);
        painter.drawRect(G4_begin.x()+30+1*(w+s), G4_begin.y()+30-h2, w, h2);
        painter.drawRect(G4_begin.x()+30+2*(w+s), G4_begin.y()+30-h3, w, h3);
        painter.drawRect(G4_begin.x()+30+3*(w+s), G4_begin.y()+30-h4, w, h4);
        painter.drawRect(G4_begin.x()+30+4*(w+s), G4_begin.y()+30-h5, w, h5);

        break;
    case 2:
        painter.setBrush(Qt::green);
        painter.drawRect(G4_begin.x()+30+0*(w+s), G4_begin.y()+30-h1, w, h1);
        painter.drawRect(G4_begin.x()+30+1*(w+s), G4_begin.y()+30-h2, w, h2);
        painter.setBrush(Qt::gray);
        painter.drawRect(G4_begin.x()+30+2*(w+s), G4_begin.y()+30-h3, w, h3);
        painter.drawRect(G4_begin.x()+30+3*(w+s), G4_begin.y()+30-h4, w, h4);
        painter.drawRect(G4_begin.x()+30+4*(w+s), G4_begin.y()+30-h5, w, h5);
        break;
    case 3:
        painter.setBrush(Qt::green);
        painter.drawRect(G4_begin.x()+30+0*(w+s), G4_begin.y()+30-h1, w, h1);
        painter.drawRect(G4_begin.x()+30+1*(w+s), G4_begin.y()+30-h2, w, h2);
        painter.drawRect(G4_begin.x()+30+2*(w+s), G4_begin.y()+30-h3, w, h3);
        painter.setBrush(Qt::gray);
        painter.drawRect(G4_begin.x()+30+3*(w+s), G4_begin.y()+30-h4, w, h4);
        painter.drawRect(G4_begin.x()+30+4*(w+s), G4_begin.y()+30-h5, w, h5);
        break;
    case 4:
        painter.setBrush(Qt::green);
        painter.drawRect(G4_begin.x()+30+0*(w+s), G4_begin.y()+30-h1, w, h1);
        painter.drawRect(G4_begin.x()+30+1*(w+s), G4_begin.y()+30-h2, w, h2);
        painter.drawRect(G4_begin.x()+30+2*(w+s), G4_begin.y()+30-h3, w, h3);
        painter.drawRect(G4_begin.x()+30+3*(w+s), G4_begin.y()+30-h4, w, h4);
        painter.setBrush(Qt::gray);
        painter.drawRect(G4_begin.x()+30+4*(w+s), G4_begin.y()+30-h5, w, h5);
        break;
    case 5:
        painter.setBrush(Qt::green);
        painter.drawRect(G4_begin.x()+30+0*(w+s), G4_begin.y()+30-h1, w, h1);
        painter.drawRect(G4_begin.x()+30+1*(w+s), G4_begin.y()+30-h2, w, h2);
        painter.drawRect(G4_begin.x()+30+2*(w+s), G4_begin.y()+30-h3, w, h3);
        painter.drawRect(G4_begin.x()+30+3*(w+s), G4_begin.y()+30-h4, w, h4);
        painter.drawRect(G4_begin.x()+30+4*(w+s), G4_begin.y()+30-h5, w, h5);
        painter.setBrush(Qt::gray);
        break;

    default:
        break;
    }
    painter.restore();
}

//draw the BD signal
void Widget::draw_BDSignal()
{
    QPainter painter(this);
    painter.save();
    QPoint BD_begin(QPoint(240,30));
    int w=4,s=2,h1=4,h2=8,h3=12,h4=16,h5=20;
    QPen pen_BD;
    pen_BD.setColor(Qt::gray);
    pen_BD.setStyle(Qt::SolidLine);
    pen_BD.setWidthF(1);
    painter.setPen(pen_BD);
    painter.setBrush(Qt::green);
    painter.setFont(QFont("Arial", 24));
    painter.drawText(BD_begin.x(),BD_begin.y()+30,"BD");


    switch (reception_BD) {
    case 0:
        painter.setBrush(Qt::gray);
        painter.drawRect(BD_begin.x()+30+0*(w+s), BD_begin.y()+30-h1, w, h1);
        painter.drawRect(BD_begin.x()+30+1*(w+s), BD_begin.y()+30-h2, w, h2);
        painter.drawRect(BD_begin.x()+30+2*(w+s), BD_begin.y()+30-h3, w, h3);
        painter.drawRect(BD_begin.x()+30+3*(w+s), BD_begin.y()+30-h4, w, h4);
        painter.drawRect(BD_begin.x()+30+4*(w+s), BD_begin.y()+30-h5, w, h5);
        painter.setBrush(Qt::green);
        break;
    case 1:
        painter.setBrush(Qt::green);
        painter.drawRect(BD_begin.x()+30+0*(w+s), BD_begin.y()+30-h1, w, h1);
        painter.setBrush(Qt::gray);
        painter.drawRect(BD_begin.x()+30+1*(w+s), BD_begin.y()+30-h2, w, h2);
        painter.drawRect(BD_begin.x()+30+2*(w+s), BD_begin.y()+30-h3, w, h3);
        painter.drawRect(BD_begin.x()+30+3*(w+s), BD_begin.y()+30-h4, w, h4);
        painter.drawRect(BD_begin.x()+30+4*(w+s), BD_begin.y()+30-h5, w, h5);
        break;
    case 2:
        painter.setBrush(Qt::green);
        painter.drawRect(BD_begin.x()+30+0*(w+s), BD_begin.y()+30-h1, w, h1);
        painter.drawRect(BD_begin.x()+30+1*(w+s), BD_begin.y()+30-h2, w, h2);
        painter.setBrush(Qt::gray);
        painter.drawRect(BD_begin.x()+30+2*(w+s), BD_begin.y()+30-h3, w, h3);
        painter.drawRect(BD_begin.x()+30+3*(w+s), BD_begin.y()+30-h4, w, h4);
        painter.drawRect(BD_begin.x()+30+4*(w+s), BD_begin.y()+30-h5, w, h5);
        break;

    case 3:
        painter.setBrush(Qt::green);
        painter.drawRect(BD_begin.x()+30+0*(w+s), BD_begin.y()+30-h1, w, h1);
        painter.drawRect(BD_begin.x()+30+1*(w+s), BD_begin.y()+30-h2, w, h2);
        painter.drawRect(BD_begin.x()+30+2*(w+s), BD_begin.y()+30-h3, w, h3);
        painter.setBrush(Qt::gray);
        painter.drawRect(BD_begin.x()+30+3*(w+s), BD_begin.y()+30-h4, w, h4);
        painter.drawRect(BD_begin.x()+30+4*(w+s), BD_begin.y()+30-h5, w, h5);
        break;

    case 4:
        painter.setBrush(Qt::green);
        painter.drawRect(BD_begin.x()+30+0*(w+s), BD_begin.y()+30-h1, w, h1);
        painter.drawRect(BD_begin.x()+30+1*(w+s), BD_begin.y()+30-h2, w, h2);
        painter.drawRect(BD_begin.x()+30+2*(w+s), BD_begin.y()+30-h3, w, h3);
        painter.drawRect(BD_begin.x()+30+3*(w+s), BD_begin.y()+30-h4, w, h4);
        painter.setBrush(Qt::gray);
        painter.drawRect(BD_begin.x()+30+4*(w+s), BD_begin.y()+30-h5, w, h5);
        break;

    case 5:
        painter.setBrush(Qt::green);
        painter.drawRect(BD_begin.x()+30+0*(w+s), BD_begin.y()+30-h1, w, h1);
        painter.drawRect(BD_begin.x()+30+1*(w+s), BD_begin.y()+30-h2, w, h2);
        painter.drawRect(BD_begin.x()+30+2*(w+s), BD_begin.y()+30-h3, w, h3);
        painter.drawRect(BD_begin.x()+30+3*(w+s), BD_begin.y()+30-h4, w, h4);
        painter.drawRect(BD_begin.x()+30+4*(w+s), BD_begin.y()+30-h5, w, h5);
        painter.setBrush(Qt::gray);
        break;

    default:
        break;
    }
    painter.restore();
}

// draw the BT signal
void Widget::draw_BTSignal()
{
    QPainter painter(this);
    painter.save();
    QPoint BT_begin(QPoint(380,30));
    QPen pen_BT;
    if(connection_BT)
    {
        pen_BT.setColor(Qt::green);
        pen_BT.setStyle(Qt::SolidLine);
        pen_BT.setWidthF(2);
        painter.setPen(pen_BT);
        painter.drawLine(QPoint(BT_begin.x()+20, BT_begin.y()+10),
                         QPoint(BT_begin.x()+20, BT_begin.y()+30));
        painter.drawLine(QPoint(BT_begin.x()+20-6, BT_begin.y()+30-10-6),
                         QPoint(BT_begin.x()+20+6, BT_begin.y()+30-10+6));
        painter.drawLine(QPoint(BT_begin.x()+20-6, BT_begin.y()+30-10+6),
                         QPoint(BT_begin.x()+20+6, BT_begin.y()+30-10-6));
        painter.drawLine(QPoint(BT_begin.x()+20, BT_begin.y()+30),
                         QPoint(BT_begin.x()+20+6, BT_begin.y()+30-10+6));
        painter.drawLine(QPoint(BT_begin.x()+20, BT_begin.y()+10),
                         QPoint(BT_begin.x()+20+6, BT_begin.y()+30-10-6));
    }
    else
    {
        pen_BT.setColor(Qt::gray);
        pen_BT.setStyle(Qt::SolidLine);
        pen_BT.setWidthF(2);
        painter.setPen(pen_BT);
        painter.drawLine(QPoint(BT_begin.x()+20, BT_begin.y()+10),
                         QPoint(BT_begin.x()+20, BT_begin.y()+30));
        painter.drawLine(QPoint(BT_begin.x()+20-6, BT_begin.y()+30-10-6),
                         QPoint(BT_begin.x()+20+6, BT_begin.y()+30-10+6));
        painter.drawLine(QPoint(BT_begin.x()+20-6, BT_begin.y()+30-10+6),
                         QPoint(BT_begin.x()+20+6, BT_begin.y()+30-10-6));
        painter.drawLine(QPoint(BT_begin.x()+20, BT_begin.y()+30),
                         QPoint(BT_begin.x()+20+6, BT_begin.y()+30-10+6));
        painter.drawLine(QPoint(BT_begin.x()+20, BT_begin.y()+10),
                         QPoint(BT_begin.x()+20+6, BT_begin.y()+30-10-6));
    }
    painter.restore();
}

//drew the battery
void Widget::draw_Battery()
{
    QPainter painter(this);
    painter.save();
    QPoint Bat_begin(QPoint(460,30));
    int bat_w=6, bat_s=2, bat_h=12;
    QPen pen_Bat;
    pen_Bat.setColor(Qt::gray);
    pen_Bat.setStyle(Qt::SolidLine);
    pen_Bat.setWidthF(1);
    painter.setPen(pen_Bat);
    painter.setBrush(Qt::gray);
    painter.drawRect(Bat_begin.x()+10, Bat_begin.y()+10, 5*(bat_w+bat_s)+bat_s, 20);
    painter.drawRect(Bat_begin.x()+10+5*(bat_w+bat_s)+bat_s, Bat_begin.y()+30-14, 4,8);

    switch (battery_remain) {
    case 0:
        painter.setBrush(Qt::gray);
        painter.drawRect(Bat_begin.x()+10+0*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.drawRect(Bat_begin.x()+10+1*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.drawRect(Bat_begin.x()+10+2*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.drawRect(Bat_begin.x()+10+3*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.drawRect(Bat_begin.x()+10+4*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        break;
    case 1:
        painter.setBrush(Qt::green);
        painter.drawRect(Bat_begin.x()+10+0*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.setBrush(Qt::gray);
        painter.drawRect(Bat_begin.x()+10+1*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.drawRect(Bat_begin.x()+10+2*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.drawRect(Bat_begin.x()+10+3*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.drawRect(Bat_begin.x()+10+4*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        break;
    case 2:
        painter.setBrush(Qt::green);
        painter.drawRect(Bat_begin.x()+10+0*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.drawRect(Bat_begin.x()+10+1*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.setBrush(Qt::gray);
        painter.drawRect(Bat_begin.x()+10+2*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.drawRect(Bat_begin.x()+10+3*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.drawRect(Bat_begin.x()+10+4*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        break;
    case 3:
        painter.setBrush(Qt::green);
        painter.drawRect(Bat_begin.x()+10+0*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.drawRect(Bat_begin.x()+10+1*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.drawRect(Bat_begin.x()+10+2*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.setBrush(Qt::gray);
        painter.drawRect(Bat_begin.x()+10+3*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.drawRect(Bat_begin.x()+10+4*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        break;
    case 4:
        painter.setBrush(Qt::green);
        painter.drawRect(Bat_begin.x()+10+0*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.drawRect(Bat_begin.x()+10+1*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.drawRect(Bat_begin.x()+10+2*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.drawRect(Bat_begin.x()+10+3*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.setBrush(Qt::gray);
        painter.drawRect(Bat_begin.x()+10+4*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        break;
    case 5:
        painter.setBrush(Qt::green);
        painter.drawRect(Bat_begin.x()+10+0*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.drawRect(Bat_begin.x()+10+1*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.drawRect(Bat_begin.x()+10+2*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.drawRect(Bat_begin.x()+10+3*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.drawRect(Bat_begin.x()+10+4*(bat_s+bat_w)+bat_s, Bat_begin.y()+30-4-bat_h, bat_w,bat_h);
        painter.setBrush(Qt::gray);
        break;
    default:
        break;
    }
    painter.restore();

}

//drew the time
void Widget::draw_Time()
{
    QPainter painter(this);
    painter.save();
    QPoint Time_begin(QPoint(600,30));
    QPen pen_Time;
    pen_Time.setColor(Qt::gray);
    pen_Time.setStyle(Qt::SolidLine);
    pen_Time.setWidthF(1);
    painter.setPen(pen_Time);
    painter.setBrush(Qt::gray);
    painter.setFont(QFont("Arial", 30));
    painter.drawText(Time_begin.x(),Time_begin.y()+30,system_time);
    painter.restore();
}

//drew the bsi
void Widget::draw_Bsi()
{
    if(flag_BsiInfo)
    {
        QPainter painter(this);
        painter.save();
        painter.setViewport(100,100,600,250);
        painter.setWindow(0,4,10,-4);  // (0,4) (10,0)
        painter.drawPixmap(0,0,10,4,QPixmap("./Resource/background1.png")
                                .scaled(QSize(400,130),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

        QPen pen;
        pen.setColor(Qt::green);
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(0.5);
        painter.setPen(pen);
        painter.setBrush(Qt::green);

        QRectF rect1(0, 0, 0.2, bsi_val[0]);
        QRectF rect2(1, 0, 0.2, bsi_val[1]);
        QRectF rect3(2, 0, 0.2, bsi_val[2]);
        QRectF rect4(3, 0, 0.2, bsi_val[3]);
        QRectF rect5(4, 0, 0.2, bsi_val[4]);
        QRectF rect6(5, 0, 0.2, bsi_val[5]);
        QRectF rect7(6, 0, 0.2, bsi_val[6]);
        QRectF rect8(7, 0, 0.2, bsi_val[7]);
        QRectF rect9(8, 0, 0.2, bsi_val[8]);
        QRectF rect10(9, 0, 0.2,bsi_val[9]);

        painter.drawRect(rect1);
        painter.drawRect(rect2);
        painter.drawRect(rect3);
        painter.drawRect(rect4);
        painter.drawRect(rect5);
        painter.drawRect(rect6);
        painter.drawRect(rect7);
        painter.drawRect(rect8);
        painter.drawRect(rect9);
        painter.drawRect(rect10);
        painter.restore();
    }
}

// init_style_sheet.
void Widget::style_init()
{
    btn_style = "QPushButton{background-color:rgb(255,178,0);\
            color: gray;   border-radius: 10px;  border: 2px groove gray; border-style: outset;}"
           "QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
           "QPushButton:pressed{padding-left:5px;padding-top:5px;}";

    edit_style = "QLineEdit{border-width:3px;  border-radius:4px;  font-size:24px;  color:black;  border:2px solid gray;}"
                 "QLineEdit:hover{border-width:3px;  border-radius:4px;  font-size:24px;  color:black;  border:2px solid rgb(70,200,50);}";

    test_edit_style = "QPlainTextEdit{border-image:url(./Resource/background1.png);"
                                     "border-width:3px;  border-radius:4px;  font-size:20px;  color:white;  border:2px solid gray;}"
                      "QPlainTextEdit:hover{border-image:url(./Resource/background1.png);"
                                     "border-width:3px;  border-radius:4px;  font-size:20px;  color:black;  border:2px solid rgb(70,200,50);}";

    // style..label
    label_style = "color:white;";

    //color..messageBox
    msb_Palette = QColor(85,170,0);

    //color..background
    bg_Palette = QColor(85,85,127);

    set_background(this,"./Resource/background1.png");
/*
    btn_warning_toggle->setStyleSheet("QPushButton{background-color:rgb(255,178,0);\
                                      color: gray;   border-radius: 10px;  border: 0px groove gray; border-style: outset;}"
                                     "QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }");
*/
    //set the label style
    this->m_label->setStyleSheet("color: black");
    this->g_label->setStyleSheet(label_style);
    this->label_B2Indentity->setStyleSheet(label_style);
    this->label_B2Indentity->setStyleSheet(label_style);
    this->label_bdID->setStyleSheet(label_style);
    this->label_devID->setStyleSheet(label_style);
    this->label_task->setStyleSheet(label_style);
    this->label_userName->setStyleSheet(label_style);
    this->label_userPwd->setStyleSheet(label_style);
    this->label_fingerPrint->setStyleSheet(label_style);
    this->label_tipDialog->setStyleSheet(label_style);

    //set the lineEdit style.
    this->edit_B2Indentity->setStyleSheet(edit_style);
    this->edit_bdID->setStyleSheet(edit_style);
    this->edit_devID->setStyleSheet(edit_style);
    this->edit_task->setStyleSheet(edit_style);
    this->edit_userName->setStyleSheet(edit_style);
    this->edit_userPwd->setStyleSheet(edit_style);

    //set the pushButton style
    this->m_btn4->setStyleSheet(btn_style);
    this->m_btn5->setStyleSheet(btn_style);
    this->btn_B2Indentity->setStyleSheet(btn_style);
    this->btn_readCard->setStyleSheet(btn_style);
    this->btn_InfoReport->setStyleSheet(btn_style);
    this->btn_taskGet->setStyleSheet(btn_style);
    this->btn_perLogin->setStyleSheet(btn_style);
    this->btn_login->setStyleSheet(btn_style);
    this->btn_loginCancel->setStyleSheet(btn_style);

//    this->btn_infoBT->setStyleSheet(btn_style);
    this->btn_go_to_main->setStyleSheet(btn_style);
    this->btn_infoBsiClose->setStyleSheet(btn_style);


    // widget..mian
    this->label_task_name->setStyleSheet(label_style);
    this->label_task_status->setStyleSheet(label_style);
    this->label_task_from->setStyleSheet(label_style);
    this->label_task_to->setStyleSheet(label_style);
    this->label_task_acquire_time->setStyleSheet(label_style);
    this->label_task_plan_finish_time->setStyleSheet(label_style);
    this->edit_task_name->setStyleSheet(label_style);
    this->edit_task_status->setStyleSheet(label_style);
    this->edit_task_from->setStyleSheet(label_style);
    this->edit_task_to->setStyleSheet(label_style);
    this->edit_task_acquire_time->setStyleSheet(label_style);
    this->edit_task_plan_finish_time->setStyleSheet(label_style);
    this->label_go_to_work->setStyleSheet(label_style);
    this->label_lock_screen->setStyleSheet(label_style);


    // widget..unlock_Screen
    this->label_lsUserName->setStyleSheet(label_style);
    this->label_lsUserPwd->setStyleSheet(label_style);
    this->edit_lsUserName->setStyleSheet(edit_style);
    this->edit_lsUserPwd->setStyleSheet(edit_style);
    this->btn_unlockScreen->setStyleSheet(btn_style);
    this->btn_unlockScreen_cancle->setStyleSheet(btn_style);

    //widget..black_Screen
    this->btn_blackScreen->setPalette(QColor(0,0,0));

    //set icon of widget fingerprint
    set_btn_icon(btn_fingerPrint1,"icon_fingerprint_no.png");
    set_btn_icon(btn_fingerPrint2,"icon_fingerprint_no.png");
    set_btn_icon(btn_fingerPrint3,"icon_fingerprint_no.png");
    btn_fingerPrint_continue->setStyleSheet(btn_style);
    btn_fingerPrint_end->setStyleSheet(btn_style);

    //set icon of widget mian
    set_btn_icon(btn_go_to_work,"icon_gotowork.png");
    set_btn_icon(btn_lock_screen,"icon_lockscreen.png");

    //set icon of widget work
    set_btn_icon(btn_addition,"icon_addition.png");
    set_btn_icon(btn_set,"icon_set.png");
    set_btn_icon(btn_unlock4G,"icon_unlock4g.png");
    set_btn_icon(btn_taskPause,"icon_taskpause.png");
    set_btn_icon(btn_taskResume,"icon_taskresume.png");
    set_btn_icon(btn_unlockBD,"icon_unlockbd.png");
    set_btn_icon(btn_taskComplete,"icon_taskcomplete.png");
    set_btn_icon(btn_go_to_main,"icon_gotomain.png");
    set_btn_icon(btn_lock_screen2,"icon_lockscreen.png");
    set_btn_icon(btn_closeLock,"icon_closelock.png");


    set_btn_icon(btn_addFingerPrint,"icon_addfingerprint.png");
    set_btn_icon(btn_infoBsi,"icon_infobsi.png");
    set_btn_icon(btn_backToWork,"icon_backtowork.png");


    // set icon of widget test
    btn_testInfoRS->setStyleSheet(btn_style);
    btn_testReadBsi->setStyleSheet(btn_style);
    btn_testReadCard->setStyleSheet(btn_style);
    btn_testReadLocation->setStyleSheet(btn_style);

    // set style of widget taskPause
    btn_taskPause_cancle->setStyleSheet(btn_style);
    btn_taskPause_confirm->setStyleSheet(btn_style);
    comboBox_taskPause_reson->setStyleSheet(edit_style);

    //set style of widget_phoneNumber
    label_phoneNumber1->setStyleSheet(label_style);
    label_phoneNumber2->setStyleSheet(label_style);
    btn_phoneNumber1->setStyleSheet(btn_style);
    btn_phoneNumber2->setStyleSheet(btn_style);


    label_dyncode1->setStyleSheet(label_style);
    label_dyncode2->setStyleSheet(label_style);
    label_dyncode3->setStyleSheet(label_style);
    edit_dyncode->setStyleSheet(edit_style);
    btn_dyncode_cancel->setStyleSheet(btn_style);
    btn_dyncode_unlock->setStyleSheet(btn_style);


    test_edit->setStyleSheet(test_edit_style);
    btn_warning_confirm->setStyleSheet(btn_style);
}

 // set the background
void Widget::set_background(QWidget *widget, QString str)
{
    widget->setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    QPalette palette = widget->palette();
    palette.setBrush(QPalette::Window,// 缩放背景图.
                     QBrush(QPixmap(str).scaled(widget->size(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    widget->setPalette(palette);      // 给widget加上背景图
}

// set the btn icon
void Widget::set_btn_icon(QWidget *widget, QString str)
{
     QStringList stringList;
     stringList.append(QString("QPushButton{ border-image:url(./Resource/icon/%1);}").arg(str));
     stringList.append(QString("QPushButton{ color: gray;}"));
     stringList.append(QString("QPushButton{ border-radius: 10px;  border: 2px groove gray; border-style: outset;}"));
     widget->setStyleSheet(stringList.join(""));
}

