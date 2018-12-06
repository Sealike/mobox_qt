#include "Widget.h"
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

void Widget::slot_B2CheckStatus(bool bSuccess, QString& strResult)
{
    bSuccess = bSuccess;

    QString buf = strResult;
    QByteArray data = buf.toUtf8();

    QJsonParseError jsonError;//Qt5新类
    QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);//Qt5新类

    if (jsonError.error == QJsonParseError::NoError)
    {
        if (json.isObject())
        {
            QJsonObject rootObj = json.object();

            //是否含有key  success
            if(rootObj.contains("success"))
            {
                QJsonValue value = rootObj.value("success");
                //判断是否是string类型
                if (value.isString())
                {
                    B2_Status->check_success = value.toString();
                }
            }

            //是否含有key  status
            if(rootObj.contains("status"))
            {
                QJsonValue value = rootObj.value("status");
                //判断是否是string类型
                if (value.isString())
                {
                    B2_Status->check_status = value.toString();
                }
            }

            //是否含有key  msg
            if(rootObj.contains("msg"))
            {
                QJsonValue value = rootObj.value("msg");
                //判断是否是string类型
                if (value.isString())
                {
                    B2_Status->check_msg = value.toString();
                }
            }

        }
    }

    if(B2_Status->check_success == "1")
    {
        if(B2_Status->check_status == "1")
        {
            //设备无资源，进入设置模式，请注入资源!
            generate_WaringMsb("设备无资源，请注入资源! ");
        }
        else if (B2_Status->check_status == "2")
        {
            //尚未进行开机口令验证，需要进入第二步来验证开机口令。
            QString ret = edit_B2Indentity->text();
            Http* pHttpFun = new Http();
            QString strUrl = "http://192.168.1.2/cgi/user.cgi?req=1&user_type=3&user_pwd=" + ret;
            connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
                    this,SLOT(slot_B2LoginStatus(bool,const QString&)));
            pHttpFun->get(strUrl);

        }
        else if(B2_Status->check_status == "3")
        {
            //表示当前状态是已验证开机口令。
            B2_Login_Success();
        }

    }else
    {
        //查询失败：错误原因见msg字段
        B2_Login_failed("查询失败! "+ B2_Status->check_msg);
    }
}


void Widget::slot_B2LoginStatus(bool bSuccess, QString &strResult)
{
    bSuccess = bSuccess;

    QString buf = strResult;
    QByteArray data = buf.toUtf8();

    QJsonParseError jsonError;//Qt5新类
    QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);//Qt5新类

    if (jsonError.error == QJsonParseError::NoError)
    {
        if (json.isObject())
        {
            QJsonObject rootObj = json.object();

            //是否含有key  success
            if(rootObj.contains("success"))
            {
                QJsonValue value = rootObj.value("success");
                //判断是否是string类型
                if (value.isString())
                {
                    B2_Status->login_sucess = value.toString();
                }
            }

            //是否含有key  msg
            if(rootObj.contains("msg"))
            {
                QJsonValue value = rootObj.value("msg");
                //判断是否是string类型
                if (value.isString())
                {
                    B2_Status->login_msg = value.toString();
                }
            }

        }
    }

    if(B2_Status->login_sucess == "1")
    {
        //开机口令验证成功
        B2_Login_Success();
    }
    else
    {
        //开机口令验证失败
        B2_Login_failed("开机口令验证失败! "+ B2_Status->check_msg);
    }
}

void Widget::slot_InfoReportResult(bool bSuccess, const QString strResult)
{
    qDebug() << "slot_InfoReportResult";

    if(bSuccess)
    {
//        QString ret = httpInfoRptParseStore(strResult);
        qDebug()<< "InfoReport receive"<<strResult;
//        if(ret == "ok")
//        {
            isInfoReport = true;
            g_label->show();
            g_label->clear();
            g_label->setText("信息上报成功！");
            m_timer_infoRpt->start(1000);

            //状态存盘
            sys_param.status.inforpt = "yes";
            sys_param.bdid = edit_bdID->text();
            sys_param.devid = edit_devID->text();
            J_Config.config_save(sys_param);
            QProcess::startDetached("sync");
//        }
//        else
//        {
//            isInfoReport = false;
//            g_label->show();
//            g_label->clear();
//            g_label->setText("信息上报失败！ 请重试...");
//            m_timer_infoRpt->start(1000);
//        }
    }
    else
    {
        qDebug()<< "InfoReport receive"<<strResult;
        isInfoReport = false;
        g_label->show();
        g_label->clear();
        g_label->setText("网络连接失败! 请重试...");
        m_timer_infoRpt->start(1000);
    }
}


//slot..taskGetResult
void Widget::slot_taskGetResult(bool bSuccess, QString strResult)
{
    qDebug() << "slot_taskGetResult";

    s_Layout->setCurrentIndex(widget_resume_index);

    if(bSuccess)
    {
        QString ret = HttpResultParseStore(strResult);
        qDebug() << "strResult" << strResult;

       if ("ok" ==  ret)
       {
//           generate_InfoMsb("任务获m_timer_infoRpt取成功！");
           g_label->clear();
           g_label->show();
           g_label->setText("任务获取成功！");
           label_task->show();
           label_task->setText("任务代号：" + PrccessParam.taskid);
           btn_perLogin->show();
           btn_taskGet->hide();
//           task_status = TASK_RUNNING;
           main_work_refresh();
           m_timer_taskGet->start(1000);

           // 状态存盘
           sys_param.status.istask = "yes";
           J_Config.config_save(sys_param);

       }
       else
       {
//           generate_WaringMsb("任务返回结果格式错误！请重试..." +ret);
           g_label->clear();
           g_label->show();
           g_label->setText("当前无任务，请稍后再试...");
           m_timer_taskGet->start(1000);

       }
    }
    else
    {
//        generate_WaringMsb("任务获取失败！请重试..." +strResult);
        g_label->clear();
        g_label->show();
        g_label->setText("任务获取失败！请重试...");
        m_timer_taskGet->start(1000);
    }
}

//slot..login_result
void Widget::slot_loginResult(bool bSuccess,QString strResult)
{
    strResult = strResult;
    qDebug() << "slot_loginResult";
    if(bSuccess)
    {
//        suoThread::suo_control("$CLEAR");

        isFirstLogin = false;
//        generate_InfoMsb("登录成功！");


        show_label("登录成功！");
        m_timer_login->start(1000);

        QString ret1 = "已制订";
        QString ret2 = "在执行";
        QString ret3 = "已暂停";
        QString ret4 = "已完成";


        if(m_ui_param->state == ret1)
        {
            task_status = TASK_RUNNING;
            // 设置任务状态为“在执行”
            Http* pHttpFun = new Http();
            QString strUrl = "http://27.128.164.177:9090/xxxgw/boxid/updatestatus?opuserid=devid&taskid="+PrccessParam.taskid+ "&status=%e5%9c%a8%e6%89%a7%e8%a1%8c";
            pHttpFun->get(strUrl);

            //状态存盘
            sys_param.status.taskstatus = "running";
            J_Config.config_save(sys_param);
        }
        if(m_ui_param->state == ret2)
        {
             task_status = TASK_RUNNING;
             //状态存盘
             sys_param.status.taskstatus = "running";
             J_Config.config_save(sys_param);
        }
        if(m_ui_param->state == ret3)
        {
             task_status = TASK_PAUSE;
             //状态存盘
             sys_param.status.taskstatus = "pause";
             J_Config.config_save(sys_param);
        }
        if(m_ui_param->state == ret4)
        {
             task_status = TASK_FINISH;
             //状态存盘
             sys_param.status.taskstatus = "finish";
             J_Config.config_save(sys_param);
        }


        PrccessParam.name =  edit_userName->text();
        PrccessParam.executor = edit_userName->text();
        PrccessParam.pwd = edit_userPwd->text();
        PrccessParam.sess_name = edit_userName->text();
        PrccessParam.sess_pwd = edit_userPwd->text();


        edit_userName->clear();
        edit_userPwd->clear();
        // open position report timer
        m_timer_posrpt->start(m_ui_param->reportfreq);
        m_timer_starpt->start(STATUS_REPORT_TIME);

    }
    else
    {
        generate_WaringMsb("登录失败！请重试...");
        s_Layout->setCurrentIndex(widget_Login_index);
    }
}


//slot..bd_unlock_result
void Widget::slot_bd_unlock(bool success, QString msg, QString code)
{
//    s_Layout->setCurrentIndex(widget_work_index);
    m_timer_unlockBD->stop();
    current_sec_clear();
    QString dyncode = code;
    m_ui_param->dyncode = dyncode;

    if(success == true)
    {
        if(CheckPassword(dyncode))
        {
           s_Layout->setCurrentIndex(widget_inputDyncode_index);
           label_dyncode2->setText(dyncode);
        }
        else
        {
            s_Layout->setCurrentIndex(widget_work_index);
            generate_WaringMsb("动态码获取失败！请重试...");
        }
    }
    else
    {
        if(msg == "code wrong")
        {
            generate_WaringMsb("解锁码错误，请重试！");
        }
        else if(msg == "forbid unlock")
        {
            generate_WaringMsb("禁止开锁！");
        }
    }

/*
    if(true == success)
    {
        label_tipDialog->clear();
        label_tipDialog->setText("正在开锁，请稍候...");
        suoParam.Suo_State = SUO_OPEN;
        suoThread::suo_control("$OLOCK");
        m_timer_openLock->start(30*1000);
        m_timer_1s->start(1000);
    }
    else
    {
        if(msg == "code wrong")
        {
            generate_WaringMsb("解锁码错误，请重试！");
        }
        else if(msg == "forbid unlock")
        {
            generate_WaringMsb("禁止开锁！");
        }
    }

*/

}


//slot..4G_Data_process
void Widget::slot_4G_Data(QString str)
{
    int data_top,data_bottom;
    QString framehead= "$";
    QString frametail= "\r\n";

    data_top=str.indexOf(framehead,0);
    data_bottom=str.indexOf(frametail,0);

    QString net_data=str.mid((data_top),(data_bottom-3));//去掉"*AA\r\n"字段
    qDebug() << "receive net data: " << net_data;
    printf("----------------------");


    // receive initack
    if(net_data.startsWith("$initack")){//平台返回的登录应答协议
            Network_Process::initack_cmd_process(net_data);

    }else
     // receive auth
    if (net_data.startsWith("$auth")) {//登录应答协议
        Network_Process::auth_cmd_process(net_data);
    }else
    // receive warn
    if (net_data.startsWith("$warn")) {//告警协议
//        Network_Process::warn_cmd_process(net_data);
        warn_process(net_data);
    }else
    // receive dyncode
    if(net_data.startsWith("$dyncode")){//获取动态码
//         s_Layout->setCurrentIndex(widget_work_index);
         m_timer_unlock4G->stop();
         current_sec_clear();
         QString dyncode;

         QString ret = Network_Process::dyncode_cmd_process(net_data,dyncode);
         m_ui_param->dyncode = dyncode;
         if(ret == "ok")
         {
             if(CheckPassword(dyncode))
             {
                isSendUnlockLog = true;
                s_Layout->setCurrentIndex(widget_inputDyncode_index);
                label_dyncode2->setText(dyncode);
             }
             else
             {
                 s_Layout->setCurrentIndex(widget_work_index);
                 generate_WaringMsb("动态码获取失败！请重试...");
             }
         }
         else if(ret == "deny")
         {
             s_Layout->setCurrentIndex(widget_work_index);
             generate_WaringMsb("权限不足，禁止开锁！");
         }
         else
         {
             s_Layout->setCurrentIndex(widget_work_index);
             generate_WaringMsb("动态码获取失败！请重试...");
         }

/*
        if(true == Network_Process::dyncode_cmd_process(net_data,dyncode)){
            //执行开锁指令
            label_tipDialog->clear();
            label_tipDialog->setText("正在开锁，请稍候...");
            suoParam.Suo_State = SUO_OPEN;
            suoThread::suo_control("$OLOCK");
            m_timer_openLock->start(30*1000);
            m_timer_1s->start(1000);
        }
        else{
            generate_WaringMsb("解锁码错误! 请重试...");
        }
*/

    }else
    // receive taskpau
    if(net_data.startsWith("$taskpau")) {//任务暂停应答
        s_Layout->setCurrentIndex(widget_work_index);
        m_timer_taskPause->stop();
        current_sec_clear();
        if(true == Network_Process::taskpau_cmd_process(net_data)){
            post_taskPause();
            generate_InfoMsb("任务暂停成功！");
        }
        else{
            generate_WaringMsb("任务暂停失败！");
        }
    }else
    // receive taskcon
    if (net_data.startsWith("$taskcon")) {//任务恢复应答
        s_Layout->setCurrentIndex(widget_work_index);
        m_timer_taskResume->stop();
        current_sec_clear();
        if(true == Network_Process::taskcon_cmd_process(net_data)){
            post_taskResume();
            generate_InfoMsb("任务恢复成功！");
        }
        else{
            generate_WaringMsb("任务恢复失败！");
        }
    }else
    // receive taskdone
    if (net_data.startsWith("$taskdone")) {//任务完成应答
            s_Layout->setCurrentIndex(widget_work_index);
            m_timer_taskComplete->stop();
            current_sec_clear();
            if(true == Network_Process::taskdone_cmd_process(net_data))
            {
                generate_InfoMsb("任务已完成！");
                task_status = TASK_NULL;
                edit_task_status->setText("当前无任务...");
                information_clear();
                s_Layout->setCurrentIndex(widget_TaskGet_index);

            }
            else
            {
                generate_WaringMsb("任务完成请求失败！");
            }
    }else
    // receive awake
    if (net_data.startsWith("$awakeack")) {//awake应答
        m_timer_awake->stop();
        current_sec_clear();

        bool success = Network_Process::awakeack_cmd_process(net_data);
        if(success)
        {
            generate_InfoMsb("登录成功！");
            s_Layout->setCurrentIndex(widget_main_index);
            // 保存sess_name sess_pwd
            PrccessParam.sess_name = m_ui_param->sess_name;
            PrccessParam.sess_pwd = m_ui_param->sess_pwd;
            PrccessParam.executor = m_ui_param->sess_name;
            nwthread->update_name_pwd(PrccessParam.sess_name, PrccessParam.sess_pwd);
        }
        else
        {
            generate_WaringMsb("登录失败！");
            s_Layout->setCurrentIndex(widget_unlockScreen_index);
        }

    }else
    // receive pong
    if (net_data.startsWith("$pong")) {//

    }else{
         qDebug() << "input network data error";
    }
}





void Widget::slot_reloginResult(bool bSuccess,QString strResult)
{
    strResult = strResult;
    if(bSuccess)
    {
        QString msg = Network_Job::taskcon_cmd_send();
        nwthread->nw_send_data(msg);
    }
    else
    {
        generate_WaringMsb("任务恢复失败，请重试...");
        s_Layout->setCurrentIndex(widget_work_index);
        m_timer_taskResume->stop();
        current_sec_clear();
    }
}




void Widget::slot_suo_ack(QString msg)
{
    qDebug() << "slot_suo_ack, receive: " << msg;

    QStringList datalist = msg.split(",");
    QString str = datalist[0];
    if(str.startsWith("$STATE"))
    {

    }else if(str.startsWith("$SOUND"))
    {

    }else if(str.startsWith("$OLOCK"))
    {
//        current_sec_clear();
//        m_timer_openLock->stop();
        hide_label();
        s_Layout->setCurrentIndex(widget_work_index);

        if (datalist[1] == "OK")
        {
            if(isSendUnlockLog == true){
                QString msg = Network_Job::log_cmd_send();
                nwthread->nw_send_data(msg);
            }
            generate_InfoMsb("操作成功！");
        }
        else
        {
            generate_WaringMsb("操作失败！请重试...");
        }

    }else if(str.startsWith("$FINGE"))
    {
        if (datalist[1] == "OK")
        {
            show_label("请录入指纹...");
//            btn_fingerPrint_end->hide();
//            btn_fingerPrint_end->setAttribute(Qt::WA_Disabled,true);
            btn_fingerPrint_end->setEnabled(false);
        }

    }else if(str.startsWith("$INPUT"))
    {
        if (datalist[1] == "01")
        {
            // return ack to the lock , if input ok
            suoThread::suo_control("$INPUT");

            set_btn_icon(btn_fingerPrint2,"icon_fingerprint_yes");
            isFirstFingerPrint = false;
            show_label("指纹录入成功！");
            fingerPrint_success = true;
            m_timer_fingerPrint->start(1000);

            //状态存盘
            sys_param.status.isfingprint = "yes";
            J_Config.config_save(sys_param);

        }
        else
        {
            generate_WaringMsb("指纹录入失败！请重试...");

            // send FINGE OPEN cmd to the lock, to restart input fingerprints....
            suoThread::suo_control("$FINGE");

        }

    }else if(str.startsWith("$CLEAR"))
    {
         if (datalist[1] == "OK")
         {
             fingerPrint_clear = true;
             //状态存盘
             sys_param.status.isfingprint = "no";
             J_Config.config_save(sys_param);
         }
         else
         {
             fingerPrint_clear = false;
             qDebug() << "Clear err and resend $CLEAR";
             suoThread::suo_control("$CLEAR");
         }

    }else if(str.startsWith("$JUDGE"))
    {
        if (datalist[1] == "OK")
        {
//            generate_InfoMsb("fingerPrint judged OK!");
            if( isLockScreen )
            {
                s_Layout->setCurrentIndex(widget_unlockScreen_index);
                set_background(this,"./Resource/background1.png");
                suoThread::suo_control("$JUDGE");
                screen_open();
            }
        }
        else
        {
//            generate_WaringMsb("fingerPring judged ERROR!");
            if( isLockScreen )
            {
                //show phoneNumber
                s_Layout->setCurrentIndex(widget_phoneNumber_index);
                set_background(widget_phoneNumber,"./Resource/background1.png");
            }
        }
    }
}



/*

QString Widget::httpInfoRptParseStore(QString strResult)
{
    QString ret = "default";
    QString data1;

    QString buf = strResult;
    QByteArray data = buf.toUtf8();

    QJsonParseError jsonError;//Qt5新类
    QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);//Qt5新类
    if (jsonError.error == QJsonParseError::NoError)
    {
        if (json.isObject())
        {
            QJsonObject rootObj = json.object();
            //是否含有key  rslt
            if(rootObj.contains("rslt"))
            {
                QJsonValue value = rootObj.value("rslt");
                //判断是否是string类型
                if (value.isString())
                {
                    data1 = value.toString();
                    if(data1=="ok")
                    {
                        ret = "ok";
                    }
                }
            }
        }
    }
    return ret;
}


// slot...HttpResultParseStore
QString Widget::HttpResultParseStore(QString strResult)
{
    qDebug() << "HttpResultParseStore begin...";

    QString ret = "default";
    QString data1;

//    QString buf=str.right(str.length()-sizeof("$warn,from,to"));
    QString buf = strResult;
    QByteArray data = buf.toUtf8();

        QJsonParseError jsonError;//Qt5新类
        QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);//Qt5新类
        if (jsonError.error == QJsonParseError::NoError)
        {
            if (json.isObject())
            {
                QJsonObject rootObj = json.object();
                //是否含有key  rslt
                if(rootObj.contains("rslt"))
                {
                    QJsonValue value = rootObj.value("rslt");
                    //判断是否是string类型
                    if (value.isString())
                    {
                        data1 = value.toString();
                        if(data1=="ok")
                        {

                            if (rootObj.contains("data"))//是否包含指定的key
                            {
                                QJsonValue value = rootObj.value("data");//得到键值data所对应的数值

                                if(value.isArray())
                                {
                                    QJsonArray array = value.toArray();
                                    //int size = array.size();

                                    QJsonValue arrayVal = array.at(0);
                                    if(arrayVal.isNull())
                                    {
                                        // arrayVal is null
                                        ret = "arrayVal is null";
                                    }
                                    QJsonObject arrayobj = arrayVal.toObject();


                                    // AAAAAAAA    data parse  AAAAAAAA
                                    // arrayobj is our obj, it contains every point we need such as id and name etc.

                                    // data.id  --> taskid
                                    if(arrayobj.contains("id"))
                                    {
                                        QJsonValue idVal = arrayobj.value("id");
                                        if(idVal.isDouble())
                                        {
                                            data1 = QString::number(idVal.toVariant().toInt(),10);

                                            PrccessParam.taskid = data1;
                                            ret = "id ok";
                                        }
                                        else
                                        {
                                            ret = "idVal is string";
                                        }
                                    }
                                    else
                                    {
                                        // id not contained
                                        ret = "id not contained";
                                    }


                                    // data.name:"押运"  -> taskname
                                    if(arrayobj.contains("name"))
                                    {
                                        QJsonValue val = arrayobj.value("name");
                                        if(val.isString())
                                        {
                                            m_ui_param->task_name = val.toString();
                                            ret = "name ok";
                                        }
                                        else
                                        {
                                            ret = "name is not string";
                                        }
                                    }
                                    else
                                    {
                                        ret = "name not contained";
                                    }

                                    // "from1":"海一",
                                    if(arrayobj.contains("from1"))
                                    {
                                        QJsonValue val = arrayobj.value("from1");
                                        if(val.isString())
                                        {
                                            m_ui_param->from = val.toString();
                                            ret = "from1 ok";
                                        }
                                        else
                                        {
                                            ret = "from1 is not string";
                                        }
                                    }
                                    else
                                    {
                                        ret = "from1 not contained";
                                    }



                                    //"to1":"空二",
                                    if(arrayobj.contains("to1"))
                                    {
                                        QJsonValue val = arrayobj.value("to1");
                                        if(val.isString())
                                        {
                                            m_ui_param->to = val.toString();
                                            ret = "to1 ok";
                                        }
                                        else
                                        {
                                            ret = "to1 is not string";
                                        }
                                    }
                                    else
                                    {
                                        ret = "to1 not contained";
                                    }

                                    //"timestart":"2018-06-03 24:11:11",
                                    if(arrayobj.contains("timestart"))
                                    {
                                        QJsonValue val = arrayobj.value("timestart");
                                        if(val.isString())
                                        {
                                            m_ui_param->task_acquire_time = val.toString();
                                            ret = "timestart ok";
                                        }
                                        else
                                        {
                                            ret = "timestart is not string";
                                        }
                                    }
                                    else
                                    {
                                        ret = "timestart not contained";
                                    }

                                    //"timeend":"2018-06-05 24:11:11",
                                    if(arrayobj.contains("timeend"))
                                    {
                                        QJsonValue val = arrayobj.value("timeend");
                                        if(val.isString())
                                        {
                                            m_ui_param->task_plan_finish_time = val.toString();
                                            ret = "timeend ok";
                                        }
                                        else
                                        {
                                            ret = "timeend is not string";
                                        }
                                    }
                                    else
                                    {
                                        ret = "timeend not contained";
                                    }



                                    //"maxmisstime":"24"
                                    if(arrayobj.contains("maxmisstime"))
                                    {
                                        QJsonValue val = arrayobj.value("maxmisstime");
                                        if(val.isString())
                                        {
                                            m_ui_param->task_allow_offset_time = val.toString();
                                            ret = "maxmisstime ok";
                                        }
                                        else
                                        {
                                            ret = "maxmisstime is not string";
                                        }
                                    }
                                    else
                                    {
                                        ret = "maxmisstime not contained";
                                    }


                                    if(arrayobj.contains("executor_code"))
                                    {
                                        QJsonValue val = arrayobj.value("executor_code");
                                        if(val.isString())
                                        {
                                            m_ui_param->executor_code = val.toString();
                                            ret = "executor_code ok";
                                        }
                                        else
                                        {
                                            ret = "executor_code is not string";
                                        }
                                    }
                                    else
                                    {
                                        ret = "executor_code not contained";
                                    }




                                    // data."reportfreq":10
                                    if(arrayobj.contains("reportfreq"))
                                    {
                                        QJsonValue val = arrayobj.value("reportfreq");
                                        if(val.isDouble())
                                        {
                                            // QString::number(val.toVariant().toInt(),10);
                                            int temp = (val.toInt());
                                            if(temp <=0)  temp = 10;
                                            m_ui_param->reportfreq = temp*1000;
                                            ret = "reportfreq ok";
                                            ret = "ok";
                                        }
                                        else
                                        {
                                            ret = "reportfreq is not double";
                                        }
                                    }
                                    else
                                    {
                                        ret = "reportfreq not contained";
                                    }

                                }
                                else
                                {
                                    ret = "data is not array";
                                }
                            }
                            else
                            {
                                ret = "data is not string";
                            }
                        }
                        else
                        {
                            // rslt error
                            ret = "rslt error";
                        }
                    }
                    else
                    {
                        // rslt is not string
                        ret = "rslt is not string";
                    }
                }
                else
                {
                    ret = "rslt is not contained";
                }
            }
            else
            {
                // json is not object
                ret = "json is not object";
            }
        }
        else
        {
            // json format error
            ret = "json format error";
        }

    qDebug() << ret;
    qDebug() << "HttpResultParseStore end...";
    return ret;
}



*/
void Widget::slot_cardId_data(QString msg)
{
    edit_bdID->setText(msg);
}

// slot..bd_bsi
void Widget::slot_bd_bsi(QString msg)
{
    int bsi = 0;
    bool ok;
    qDebug() << "recv msg:"<<msg;
    QString ret[10];
    QStringList datalist = msg.split(",");
    QString bdbsi = "";

    // 1. get the bsi
    for(int i=0; i<10; i++)
    {
        ret[i] = datalist[i+3];
        bsi_val[i] = (ret[i].toInt(&ok,10)) % 5;
        bdbsi += QString::number(bsi_val[i],10);
    }
    PrccessParam.bdbsi = bdbsi;

    // 2. calc the reception_BD
    for(int y=0; y<10; y++){
        bsi += bsi_val[y];
    }
    if( bsi == 0)   PrccessParam.reception_BD = 0;//0
    if( bsi >= 1)   PrccessParam.reception_BD = 1;//1
    if( bsi >= 3)   PrccessParam.reception_BD = 2;//2
    if( bsi >= 6)   PrccessParam.reception_BD = 3;//3
    if( bsi >= 10 ) PrccessParam.reception_BD = 4;//4
    if( bsi >= 12 ) PrccessParam.reception_BD = 5;//5

    // 3. set the connection_bd
    if( reception_BD >= 3){
        connection_bd = true;
    }else{
        connection_bd = false;
    }
}


void Widget::warn_process(QString str)
{
    QString data1;
    QString buf = Network_Process::Split_Pure_Json(str);

//    QString buf=str.right(str.length()-sizeof("$warn,from,to"));
    QByteArray data = buf.toUtf8();

        QJsonParseError jsonError;//Qt5新类
        QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);//Qt5新类
        if (jsonError.error == QJsonParseError::NoError)
        {
            if (json.isObject())
            {
                QJsonObject rootObj = json.object();
                //是否含有key  rslt
                if (rootObj.contains("data"))//是否包含指定的key
                {
                    QJsonValue value = rootObj.value("data");//得到键值data所对应的数值
                    if (value.isObject())
                    {  // Page 的 value 是对象
                        QJsonObject obj = value.toObject();
                         if (obj.contains("type"))
                         {
                        QJsonValue a = obj.value("type");
                        //判断是否是string类型
                        if (a.isString())
                        {
                            data1 = a.toString();
                            if (data1=="distance"){//超距离告警    send


                            }else if (data1=="destory"){//破坏告警    send


                            }else if (data1=="lowpower"){//低电量告警   send


                            }else if (data1=="fence"){//电子围栏告警   receive
//                                 generate_WaringMsb("电子围栏告警!");
                                 warning_process("电子围栏告警!");

                            }else if (data1=="dateout"){//任务执行超期告警    receive
//                                generate_WaringMsb("任务执行超期告警!");
                                warning_process("任务执行超期告警!");

                            }else if (data1=="disappear"){//消失告警    receive
                                generate_WaringMsb("消失告警!");
                                warning_process("消失告警!");

                            }

                    }
                    }else
                         {
                             qDebug() << "warn cmderror";
                         }
                }

            }
        }
    }
}



