#include "Widget.h"

#include <QMessageBox>
#include <QThread>
#include <QDebug>
#include <bd_type.h>
#include <network_job.h>
#include <network_data_process.h>
#include <bdthread.h>
#include <nwthread.h>
#include <Http.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

void Widget::config_init()
{

//    J_Config.config_init(sys_param);
//    J_Config.config_save(sys_param);

    J_Config.config_read(sys_param);

    QString bdid = sys_param.bdid;
    QString devid = sys_param.devid;
    edit_bdID->setText(PrccessParam.bd_id_local);
    edit_devID->setText(devid);

    qDebug() << bdid;
    qDebug() << PrccessParam.bd_id_local;
    qDebug() << "sys_param.status.b2indentity: " << sys_param.status.b2indentity ;
    qDebug() << "sys_param.status.inforpt: " << sys_param.status.inforpt;
    qDebug() << "sys_param.status.istask: " << sys_param.status.istask;

    if(sys_param.status.isfingprint == "yes") {
        isFirstFingerPrint = false;
    }
    else{
        isFirstFingerPrint = true;
    }


    if(sys_param.status.taskstatus == "running"){
        task_status = TASK_RUNNING;
    }
    else if(sys_param.status.taskstatus == "pause"){
        task_status = TASK_PAUSE;
    }
    else if(sys_param.status.taskstatus == "init"){
        task_status = TASK_INIT;
    }
    else if(sys_param.status.taskstatus == "null"){
        task_status = TASK_NULL;
    }
    else if(sys_param.status.taskstatus == "finish"){
        task_status = TASK_FINISH;
    }


/*
    if(sys_param.status.b2indentity == "yes")
    {
        if(sys_param.status.inforpt == "yes")
        {
            if(bdid == PrccessParam.bd_id_local)
            {
                if(sys_param.status.istask == "yes")
                {
                    // 获取任务信息
                    on_btn_taskGet_clicked();
                }
                else{
                    s_Layout->setCurrentIndex(widget_TaskGet_index);
                }
            }
            else{
                s_Layout->setCurrentIndex(widget_InfoReport_index);
            }
        }
        else{
            s_Layout->setCurrentIndex(widget_InfoReport_index);
        }
    }
    else{
        s_Layout->setCurrentIndex(widget_B2Indentity_index);
    }

*/

    int stage;
    bool flag_b2 = false;
    bool flag_infoRpt = false;
    bool flag_bdid_equal = false;
    bool flag_isTask = false;

    if(sys_param.status.b2indentity == "yes")
    {
        flag_b2 = true;
    }
    if(sys_param.status.inforpt == "yes")
    {
        flag_infoRpt = true;
    }
    if(bdid == PrccessParam.bd_id_local)
    {
        flag_bdid_equal = true;
    }
    if(sys_param.status.istask == "yes")
    {
        flag_isTask = true;
    }

    if( !flag_b2)	stage = 1;
    if( flag_b2 && !flag_infoRpt ) stage = 2;
    if( flag_b2 && flag_infoRpt && !flag_bdid_equal ) stage = 2;
    if( flag_b2 && flag_infoRpt && flag_bdid_equal && !flag_isTask)  stage = 3;
    if( flag_b2 && flag_infoRpt && flag_bdid_equal && flag_isTask)   stage = 4;



    switch (stage){
    case 1:
        s_Layout->setCurrentIndex(widget_B2Indentity_index);
        break;
    case 2:
        s_Layout->setCurrentIndex(widget_InfoReport_index);
        break;
    case 3:
        s_Layout->setCurrentIndex(widget_TaskGet_index);
        break;
    case 4:
        s_Layout->setCurrentIndex(widget_TaskGet_index);
        on_btn_taskGet_clicked();
        break;
    default:
        break;
    }
}


//generate_Waring_MessageBox
void Widget::generate_WaringMsb( QString text)
{
    QMessageBox msb;
    msb.setWindowTitle("警告");
    msb.setIcon(QMessageBox::Warning);
    msb.setText(text);
    msb.setDefaultButton(QMessageBox::Ok);
    msb.setButtonText(QMessageBox::Ok, "确定");
    set_background(&msb,"./Resource/msg_background.png");
    QTimer::singleShot(1500, &msb,SLOT(accept()));
    msb.exec();
}

//generate_Infomation_MessageBox
void Widget::generate_InfoMsb( QString text)
{
    QMessageBox msb;
    msb.setWindowTitle("提示");
    msb.setIcon(QMessageBox::Information);
    msb.setText(text);
    msb.setDefaultButton(QMessageBox::Ok);
    msb.setButtonText(QMessageBox::Ok, "确定");
    set_background(&msb,"./Resource/msg_background.png");
    QTimer::singleShot(1500, &msb,SLOT(accept()));
    msb.exec();
}

//generate_SelectMsb
int Widget::generate_SelectMsb(QString text)
{
    QMessageBox msb;
    msb.setWindowTitle("提示");
    msb.setIcon(QMessageBox::Information);
    msb.setText(text);
    msb.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msb.setButtonText(QMessageBox::Ok, "确定");
    msb.setButtonText(QMessageBox::Cancel, "取消");
    set_background(&msb,"./Resource/msg_background.png");
    int ret = msb.exec();
    return ret;
}




// set system time
void Widget::setSystemTime(QString time)
{
    QString m = "date -s \"" + time + "\"";
    QProcess::startDetached(m);
    QProcess::startDetached("hwclock -w");
    QProcess::startDetached("sync");
}

void Widget::autoLockScreen_start()
{
    qDebug() << "autoLockScreen_start";
    m_autoLockScreen->start(60*1000);
    isLockScreenTimerStart = true;
}

void Widget::autoLockScreen_stop()
{
    qDebug() << "autoLockScreen_stop";
    m_autoLockScreen->stop();
    isLockScreenTimerStart = false;
}


// task complete...clear the whole infomation.
void Widget::information_clear()
{
    // 1. clear the flag
    isFirstLogin = true;
    isFirstFingerPrint = true;
    fingerPrint_success = false;
    task_status = TASK_FINISH;

    // 2. clear the syspara
    PrccessParam.name = "";
    PrccessParam.pwd = "";
    PrccessParam.sess_name = "";
    PrccessParam.sess_pwd = "";
    m_ui_param->executeor_list.clear();

    // 3. set the widget to default
    btn_perLogin->hide();
    btn_taskGet->show();
    label_task->hide();

    // 4. disconnect the tcp
    nwthread->endthread();


    set_btn_icon(btn_fingerPrint2,"icon_fingerprint_no");

    // 5. clear the fingerprint
//    for(int i=0; i<10; i++)
    {
        suoThread::suo_control("$CLEAR");
//        if(fingerPrint_clear) break;
    }

    // 状态存盘
    sys_param.status.taskstatus = "null";
    sys_param.status.istask = "no";
    J_Config.config_save(sys_param);

}

// func..clear the widget_tipdialog g_label
void Widget::current_sec_clear()
{
    g_label->setText("");
    g_label->hide();
    m_timer_1s->stop();
    current_sec = 0;
}

// func..taskpause..process
void Widget::post_taskPause()
{
    // 1. set task status
    task_status = TASK_PAUSE;

    //状态存盘
    sys_param.status.taskstatus = "pause";
    J_Config.config_save(sys_param);

    // 2. refresh the screen
    main_work_refresh();

    // 3. disconnect the tcp
    nwthread->endthread();


}

// func..taskresume..process
void Widget::post_taskResume()
{
    // 1. set task status
    task_status = TASK_RUNNING;

    //状态存盘
    sys_param.status.taskstatus = "running";
    J_Config.config_save(sys_param);

    // 2. refresh the screen
    main_work_refresh();
}

// func..update the widget_main and widget work
void Widget::main_work_refresh()
{
    // 1. refresh the widget_main
    edit_task_name->setText(m_ui_param->task_name);

    if(task_status == TASK_INIT) {
        edit_task_status->setText("任务待执行！");
    }
    else if(task_status == TASK_PAUSE){
        edit_task_status->setText("任务已暂停！");
    }
    else if(task_status == TASK_RUNNING){
        edit_task_status->setText("任务执行中...");
    }
    else if(task_status == TASK_FINISH) {
        edit_task_status->setText("任务已完成！");
    }
    else{
        edit_task_status->setText("无");
    }

    edit_task_from->setText(m_ui_param->from);
    edit_task_to->setText(m_ui_param->to);
    edit_task_acquire_time->setText(m_ui_param->task_acquire_time);
    edit_task_plan_finish_time->setText(m_ui_param->task_plan_finish_time);

    // 2. refresh the widget_work
    if(task_status == TASK_PAUSE)
    {
        btn_taskPause->hide();
        label_taskPause->hide();
        btn_taskResume->show();
        label_taskResume->show();
    }
    else
    {
        btn_taskPause->show();
        label_taskPause->show();
        btn_taskResume->hide();
        label_taskResume->hide();
    }
}

// func..lock screen
void Widget::lock_screen()
{
    this->setAutoFillBackground(true);
    this->setPalette(QColor(0,0,0));
    s_Layout->setCurrentIndex(widget_blackScreen_index);
    isLockScreen = true;
    screen_close();
}

void Widget::show_label(QString str)
{
    g_label->clear();
    g_label->show();
    g_label->setText(str);
}
void Widget::hide_label()
{
    g_label->clear();
    g_label->hide();
}

void Widget::set_warning_btn(QPushButton* btn)
{
//    btn = new QPushButton;
//    btn->setParent(widget);
    btn->resize(60,60);
    btn->move(20,20);
    btn->setFocusPolicy(Qt::NoFocus);
    btn->hide();
}



void Widget::warning_process(QString msg)
{
    QString system_time = QDateTime::currentDateTime().toString("hh:mm:ss");
    if(warning_stringList.count() > MAX_WARNING_NUMBER)
    {
        warning_stringList.pop_front();
    }
    warning_stringList.append(QString("%1 %2\n").arg(system_time).arg(msg));
    warning_blink();
//    warning_update();
}
void Widget::warning_blink()
{
    warning_blink_times = 0;
    m_timer_warning_blink->start(WARNING_BLINK_TIME);
}

//更新告警信息
void Widget::warning_update()
{

    QString ret;
    for(int i=0; i<warning_stringList.count(); i++)
    {
//        qDebug()<<warning_stringList[i];
        ret.append(warning_stringList[i]);
    }
    test_edit->setPlainText(ret);
    test_edit->moveCursor(QTextCursor::End);


//    if(warning_stringList.count() > 0)
//    {
//        QString str = warning_stringList.last();
//        test_edit->appendPlainText(str);
//        test_edit->moveCursor(QTextCursor::End);
//    }
}

//续传位置信息
void Widget::pos_resume_list_report()
{
    m_timer_posResume->stop();
    for(int i=0; i<pos_resume_list.count();i++)
    {
        QString msg = pos_resume_list[i];
        nwthread->nw_send_data(msg);
    }
    pos_resume_list.clear();
    isPosResume = false;
}


void Widget::screen_open()
{
    QString screen_open = "echo \"7\" > /sys/devices/platform/backlight/backlight/backlight/brightness";
    QProcess::startDetached(screen_open);
}

void Widget::screen_close()
{
    QString screen_close = "echo \"0\" > /sys/devices/platform/backlight/backlight/backlight/brightness";
    QProcess::startDetached(screen_close);
}

void Widget::B2_Login_Success()
{
    show_label("认证成功！");
    m_timer_B2->start(1000);

    sys_param.status.b2indentity = "yes";
    J_Config.config_save(sys_param);
}

void Widget::B2_Login_failed(QString msg)
{
    generate_WaringMsb(msg);

    edit_B2Indentity->clear();
    edit_B2Indentity->setFocus();
}
