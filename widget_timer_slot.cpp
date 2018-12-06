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
#include <QDateTime>



// timeout..btn_B2Indentity_clicked
void Widget::m_timer_B2_out()
{
    s_Layout->setCurrentIndex(widget_InfoReport_index);
    g_label->setText("");
    g_label->hide();
    m_timer_B2->stop();
}


void Widget::m_timer_taskGet_out()
{

    g_label->setText("");
    g_label->hide();
    m_timer_taskGet->stop();
}
// timeout..btn_InfoReport_clicked
void Widget::m_timer_infoRpt_out()
{
    if(isInfoReport == true){
        s_Layout->setCurrentIndex(widget_TaskGet_index);
        g_label->setText("");
        g_label->hide();
        m_timer_infoRpt->stop();
    }
    else{
        g_label->setText("");
        g_label->hide();
        m_timer_infoRpt->stop();
    }
}

//timeout..btn_fingerPrint3_clicked
void Widget::m_timer_fingerPrint_out()
{
//    s_Layout->setCurrentIndex(widget_main_index);
    hide_label();
    m_timer_fingerPrint->stop();
//    btn_fingerPrint_end->show();
    btn_fingerPrint_end->setEnabled(true);
}

void Widget::m_timer_login_out()
{
    hide_label();
    m_timer_login->stop();


    if(isFirstFingerPrint)
    {
        s_Layout->setCurrentIndex(widget_FingerPrint_index);
        // if the fp has not been input before
        // send FINGE OPEN cmd to the lock to start inputting.....
        suoThread::suo_control("$FINGE");
    }
    else
    {
        // if the fp has already been input
        // switch directly to the main widget....
        s_Layout->setCurrentIndex(widget_main_index);
    }
}

void Widget::m_timer_awake_out()
{
    current_sec_clear();
    generate_WaringMsb("登录超时，请重试...");
    m_timer_awake->stop();
    s_Layout->setCurrentIndex(widget_unlockScreen_index);
}



// timeout..btn_unlockBD_clicked
void Widget::m_timer_unlockBD_out()
{
    current_sec_clear();
    generate_WaringMsb("获取开锁码超时，请重试...");
    m_timer_unlockBD->stop();
    s_Layout->setCurrentIndex(widget_work_index);
}

// timeout..btn_unlock4G_clicked
void Widget::m_timer_unlock4G_out()
{
    current_sec_clear();
    generate_WaringMsb("获取开锁码超时，请重试...");
    m_timer_unlock4G->stop();
    s_Layout->setCurrentIndex(widget_work_index);
}

void Widget::m_timer_openLock_out()
{
    current_sec_clear();
    m_timer_openLock->stop();
    generate_WaringMsb("开锁超时，请重试...");
    s_Layout->setCurrentIndex(widget_work_index);
}

// timeout..btn_taskPause_clicked
void Widget::m_timer_taskPause_out()
{
    current_sec_clear();
    generate_WaringMsb("任务暂停超时，请重试...");
    m_timer_taskPause->stop();
    s_Layout->setCurrentIndex(widget_work_index);
}

// timeout..btn_taskResume_clicked
void Widget::m_timer_taskResume_out()
{
    current_sec_clear();
    generate_WaringMsb("任务恢复超时，请重试...");
    m_timer_taskResume->stop();
    s_Layout->setCurrentIndex((widget_work_index));
}

// timeout..btn_taskComplete_clicked
void Widget::m_timer_taskComplete_out()
{
    current_sec_clear();
    generate_WaringMsb("任务完成请求超时，请重试...");
    m_timer_taskComplete->stop();
    s_Layout->setCurrentIndex((widget_work_index));
}

// timeout...widget_tipDialog g_label
void Widget::m_timer_1s_out()
{
    current_sec++;
    QString ret =  QString::number(current_sec, 10);
    g_label->show();
    g_label->setText(ret);
}

// timeout..bd send posrpt 2min flag
void Widget::m_timer_2min_out()
{
    time_2min_ok = true;
    m_timer_2min->stop();
}

// timeout..auto lock screen timer out
void Widget::m_autoLockScreen_out()
{
    autoLockScreen_stop();
    lock_screen();
}


//timeout..position report
void Widget::m_timer_posrpt_out()
{  

    bool isPosReport = false;
    m_timer_posrpt->stop();

    qDebug() << "connection_tcp: " << connection_tcp;
    qDebug() << "connection_bd: " << connection_bd;
    qDebug() << "task_status : " << task_status;

    if(task_status == TASK_RUNNING)
    {
        //是否有需要续传的位置
        if( connection_tcp == true && isPosResume == true)
        {
            pos_resume_list_report();
        }

        // send position by 4G
        if (connection_tcp && !isPosReport)
        {
            qDebug() << "==================== Network_Job::pos_cmd_send()============ ";
            QString msg = Network_Job::pos_cmd_send();
            nwthread->nw_send_data(msg);
            isPosReport = true;
        }
        // send position by BD
        if (connection_bd && !isPosReport && time_2min_ok)
        {

            qDebug() << "==================== BDThread::bd_pos_report()============ ";
            time_2min_ok = false;
            BDThread::bd_pos_report();
            m_timer_2min->start(BD_POS_TIME);
            isPosReport = true;
            // start timer 2min
        }
        if (!connection_tcp && !connection_bd)
        {
            // disconnect BD and TCP
            generate_WaringMsb("请连接4G或BD模块！");
            //当4G和BD都断开时，开启断点续传
            m_timer_posResume->start(POS_RESUME_TIME);
        }
    }

    m_timer_posrpt->start(m_ui_param->reportfreq);
}


// push position into pos_resume_list 缓存位置信息
void Widget::m_timer_posResume_out()
{
    QString msg = Network_Job::pos_cmd_send();

    if(pos_resume_list.count() > MAX_POS_RESUME)
    {
        pos_resume_list.pop_front();
    }
    pos_resume_list.append(msg);

    isPosResume = true;
}


// timeout..status report
void Widget::m_timer_starpt_out()
{
    if(task_status == TASK_RUNNING)
    {
        QString msg = Network_Job::sta_cmd_send();
        nwthread->nw_send_data(msg);
    }
}



// timeout..btn_blackScreen_clicked
void Widget::m_timer_doubleClick_out()
{
    m_double_click = 0;
}


// timeout..refresh the status of 4G/BD/BT/Bat/Time
void Widget::m_timer_refresh_out()
{

    // 1. update the reception_4G
    if( connection_tcp == true){
        PrccessParam.reception_4G = 5;
    }
    else{
        PrccessParam.reception_4G = 0;
    }
    reception_4G = PrccessParam.reception_4G;

    // 2. update the reception_BD
    reception_BD = PrccessParam.reception_BD;


    // 3. update the connection_BT
    connection_BT = PrccessParam.connection_BT;
    //发送蓝牙断链告警
    if(connection_BT == false && task_status == TASK_RUNNING)
    {
        if(m_ui_param->bt_disconnected == "yes")
        {
            QString msg = Network_Job::warn_cmd_send("bt_disconnected");
            nwthread->nw_send_data(msg);
        }
    }


    // 4. update the battery_remain
    battery_remain = PrccessParam.battery_remain;
    //发送低电量告警
    if(battery_remain == 1  && isLowpowerRpt == false && task_status == TASK_RUNNING)
    {
        isLowpowerRpt = true;
        if(m_ui_param->lowpower == "yes")
        {
            QString msg = Network_Job::warn_cmd_send("lowpower");
            nwthread->nw_send_data(msg);
        }
    }
    if(battery_remain > 1)
    {
        isLowpowerRpt = false;
    }


    // 5. update the time
    system_time = QDateTime::currentDateTime().toString("hh:mm:ss");

    // 6. update the paint event
    update();

    // 7. refresh the widget_main and widget_work
    main_work_refresh();

    // 8. start auto lockscreen or not
    int cur_index = s_Layout->currentIndex();
    if(false == isLockScreenTimerStart)
    {
        if(cur_index == widget_main_index || cur_index == widget_work_index || cur_index == widget_unlockScreen_index || cur_index == widget_unlockScreen_index || cur_index == widget_phoneNumber_index)
        {
            autoLockScreen_start();
        }
    }
    if(true == isLockScreenTimerStart)
    {
        if(cur_index != widget_main_index && cur_index != widget_work_index && cur_index != widget_unlockScreen_index && cur_index != widget_unlockScreen_index && cur_index!= widget_phoneNumber_index)
        {
             autoLockScreen_stop();
        }
    }

    // test
//    static int i=0;
//    i++;
//    if(i >= 60)
//    {
//        QApplication::exit();
//    }

}

void Widget::m_timer_warning_blink_out()
{
    warning_blink_times++;
    QString icon_warning_small = "icon_warning_small.png";
    QString icon_warning_medium = "icon_warning_medium.png";
    QString icon_warning_big = "icon_warning_big.png";


    if(warning_blink_times%3==1  )
    {
        btn_warning_toggle->show();
        btn_warning_toggle_B2->show();
        btn_warning_toggle_InfoRpt->show();
        btn_warning_toggle_tastGet->show();
        btn_warning_toggle_longin->show();
        btn_warning_toggle_fp->show();
        btn_warning_toggle_main->show();
        btn_warning_toggle_work->show();
        btn_warning_toggle_bsiInfo->show();
        btn_warning_toggle_tipDialog->show();
        btn_warning_toggle_taskPause->show();
        btn_warning_toggle_inputDyncode->show();

        set_btn_icon(btn_warning_toggle,icon_warning_small);
        set_btn_icon(btn_warning_toggle_B2,icon_warning_small);
        set_btn_icon(btn_warning_toggle_InfoRpt,icon_warning_small);
        set_btn_icon(btn_warning_toggle_tastGet,icon_warning_small);
        set_btn_icon(btn_warning_toggle_longin,icon_warning_small);
        set_btn_icon(btn_warning_toggle_fp,icon_warning_small);
        set_btn_icon(btn_warning_toggle_main,icon_warning_small);
        set_btn_icon(btn_warning_toggle_work,icon_warning_small);
        set_btn_icon(btn_warning_toggle_tipDialog,icon_warning_small);
        set_btn_icon(btn_warning_toggle_bsiInfo,icon_warning_small);
        set_btn_icon(btn_warning_toggle_taskPause,icon_warning_small);
        set_btn_icon(btn_warning_toggle_inputDyncode,icon_warning_small);
    }
    else if(warning_blink_times%3==2)
    {
        btn_warning_toggle->show();
        btn_warning_toggle_B2->show();
        btn_warning_toggle_InfoRpt->show();
        btn_warning_toggle_tastGet->show();
        btn_warning_toggle_longin->show();
        btn_warning_toggle_fp->show();
        btn_warning_toggle_main->show();
        btn_warning_toggle_work->show();
        btn_warning_toggle_bsiInfo->show();
        btn_warning_toggle_tipDialog->show();
        btn_warning_toggle_taskPause->show();
        btn_warning_toggle_inputDyncode->show();

        set_btn_icon(btn_warning_toggle,icon_warning_medium);
        set_btn_icon(btn_warning_toggle,icon_warning_medium);
        set_btn_icon(btn_warning_toggle_B2,icon_warning_medium);
        set_btn_icon(btn_warning_toggle_InfoRpt,icon_warning_medium);
        set_btn_icon(btn_warning_toggle_tastGet,icon_warning_medium);
        set_btn_icon(btn_warning_toggle_longin,icon_warning_medium);
        set_btn_icon(btn_warning_toggle_fp,icon_warning_medium);
        set_btn_icon(btn_warning_toggle_main,icon_warning_medium);
        set_btn_icon(btn_warning_toggle_work,icon_warning_medium);
        set_btn_icon(btn_warning_toggle_tipDialog,icon_warning_medium);
        set_btn_icon(btn_warning_toggle_bsiInfo,icon_warning_medium);
        set_btn_icon(btn_warning_toggle_taskPause,icon_warning_medium);
        set_btn_icon(btn_warning_toggle_inputDyncode,icon_warning_medium);
    }
    else
    {
        btn_warning_toggle->show();
        btn_warning_toggle_B2->show();
        btn_warning_toggle_InfoRpt->show();
        btn_warning_toggle_tastGet->show();
        btn_warning_toggle_longin->show();
        btn_warning_toggle_fp->show();
        btn_warning_toggle_main->show();
        btn_warning_toggle_work->show();
        btn_warning_toggle_bsiInfo->show();
        btn_warning_toggle_tipDialog->show();
        btn_warning_toggle_taskPause->show();
        btn_warning_toggle_inputDyncode->show();

        set_btn_icon(btn_warning_toggle,icon_warning_big);
        set_btn_icon(btn_warning_toggle,icon_warning_big);
        set_btn_icon(btn_warning_toggle_B2,icon_warning_big);
        set_btn_icon(btn_warning_toggle_InfoRpt,icon_warning_big);
        set_btn_icon(btn_warning_toggle_tastGet,icon_warning_big);
        set_btn_icon(btn_warning_toggle_longin,icon_warning_big);
        set_btn_icon(btn_warning_toggle_fp,icon_warning_big);
        set_btn_icon(btn_warning_toggle_main,icon_warning_big);
        set_btn_icon(btn_warning_toggle_work,icon_warning_big);
        set_btn_icon(btn_warning_toggle_tipDialog,icon_warning_big);
        set_btn_icon(btn_warning_toggle_bsiInfo,icon_warning_big);
        set_btn_icon(btn_warning_toggle_taskPause,icon_warning_big);
        set_btn_icon(btn_warning_toggle_inputDyncode,icon_warning_big);
    }
}





