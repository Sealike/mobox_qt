#include "JsonConfig.h"

JsonConfig::JsonConfig()
{

}

bool JsonConfig::config_init(Sys_Param_t &sys_param)
{
    sys_param.bdid = "0139311";
    sys_param.devid = "8765432112345678";
    return true;
}

bool JsonConfig::config_save(Sys_Param_t &sys_param)
{
    QString json = make_Json(sys_param);
    QFile *file = new QFile(CONFIG_PATH);
    if(!file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        delete file;
        return false;
    }

    QTextStream out(file);
    out << json;
    file->close();
    delete file;
    return true;
}

bool JsonConfig::config_read(Sys_Param_t &sys_param)
{
     QFile *file = new QFile(CONFIG_PATH);
     if(!file->open(QIODevice::ReadOnly | QIODevice::Text))
     {
         delete file;
         return false;
     }

     QString json = file->readAll();
     QString ret = Json_Parse(json, sys_param);
     if(ret == "ok")
     {
         return true;
     }
     else
     {
         return false;
     }
}


QString JsonConfig::make_Json(Sys_Param_t &sys_param)
{
    QJsonObject obj;

    obj.insert("bdid", sys_param.bdid);
    obj.insert("devid", sys_param.devid);


    QJsonObject status;

    status.insert("b2indentity", sys_param.status.b2indentity);
    status.insert("inforpt", sys_param.status.inforpt);
    status.insert("istask", sys_param.status.istask);
    status.insert("isfingprint", sys_param.status.isfingprint);
    status.insert("taskstatus", sys_param.status.taskstatus);
    obj.insert("status", status);


    QJsonObject task;

    task.insert("name", sys_param.task.name);
    task.insert("state", sys_param.task.status);
    task.insert("id", sys_param.task.id);
    task.insert("from1", sys_param.task.from1);
    task.insert("to1", sys_param.task.to1);
    task.insert("timestart", sys_param.task.timestart);
    task.insert("timeend", sys_param.task.timeend);
    task.insert("reportfreq", sys_param.task.reportfreq);

        QJsonArray executeor;
        for(int i=0; i<sys_param.task.executeor_list.count(); i++)
        {
            QJsonObject obj;
            obj.insert("id",i);
            obj.insert("code",sys_param.task.executeor_list[i]);
            executeor.push_back(obj);
        }

    task.insert("executeor",executeor);


//    QJsonArray logininfo;
//    for(int i=0; i<sys_param.task.login_info_list.count(); i++)
//    {
//        QJsonObject obj;
//        obj.insert("name",sys_param.task.login_info_list[i].executeor_name);
//        obj.insert("pwd",sys_param.task.login_info_list[i].executeor_pwd);
//        executeor.push_back(obj);
//    }
//    task.insert("logininfo",logininfo);

    obj.insert("task", task);

    QJsonDocument document;
    document.setObject(obj);
    QString json(document.toJson(QJsonDocument::Compact));

    qDebug() << json;
    return json;
}




QString JsonConfig::Json_Parse(QString strResult, Sys_Param_t& sys_param)
{
    qDebug() << "JsonParse begin...";
    QString ret = "default";

    QString buf = strResult;
    QByteArray data = buf.toUtf8();

    QJsonParseError jsonError;//Qt5新类
    QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);//Qt5新类

    if (jsonError.error == QJsonParseError::NoError)
    {
        if (json.isObject())
        {
            QJsonObject rootObj = json.object();

            //是否含有key  bdid
            if(rootObj.contains("bdid"))
            {
                QJsonValue value = rootObj.value("bdid");
                //判断是否是string类型
                if (value.isString())
                {
                    sys_param.bdid = value.toString();
                }
            }

            //是否含有key  devid
            if(rootObj.contains("devid"))
            {
                QJsonValue value = rootObj.value("devid");
                //判断是否是string类型
                if (value.isString())
                {
                    sys_param.devid = value.toString();
                }
            }

            //是否含有key  status
            if(rootObj.contains("status"))
            {
                QJsonValue value = rootObj.value("status");
                if(value.isObject())
                {
                    QJsonObject statusObj = value.toObject();

                    if(statusObj.contains("b2indentity"))
                    {
                        QJsonValue value = statusObj.value("b2indentity");
                        if(value.isString())
                        {
                            sys_param.status.b2indentity = value.toString();
                        }
                    }

                    if(statusObj.contains("inforpt"))
                    {
                        QJsonValue value = statusObj.value("inforpt");
                        if(value.isString())
                        {
                            sys_param.status.inforpt = value.toString();
                        }
                    }


                    if(statusObj.contains("isfingprint"))
                    {
                        QJsonValue value = statusObj.value("isfingprint");
                        if(value.isString())
                        {
                            sys_param.status.isfingprint = value.toString();
                        }
                    }


                    if(statusObj.contains("istask"))
                    {
                        QJsonValue value = statusObj.value("istask");
                        if(value.isString())
                        {
                            sys_param.status.istask = value.toString();
                        }
                    }

                    if(statusObj.contains("taskstatus"))
                    {
                        QJsonValue value = statusObj.value("taskstatus");
                        if(value.isString())
                        {
                            sys_param.status.taskstatus = value.toString();
                        }
                    }
                }
            }

            if(rootObj.contains("task"))
            {
                QJsonValue value = rootObj.value("task");
                if(value.isObject())
                {
                    QJsonObject taskObj = value.toObject();

                    if(taskObj.contains("name"))
                    {
                        QJsonValue value = taskObj.value("name");
                        if(value.isString())
                        {
                            sys_param.task.name = value.toString();
                        }
                    }

                    if(taskObj.contains("state"))
                    {
                        QJsonValue value = taskObj.value("state");
                        if(value.isString())
                        {
                            sys_param.task.status = value.toString();
                        }
                    }

                    if(taskObj.contains("from1"))
                    {
                        QJsonValue value = taskObj.value("from1");
                        if(value.isString())
                        {
                            sys_param.task.from1 = value.toString();
                        }
                    }

                    if(taskObj.contains("to1"))
                    {
                        QJsonValue value = taskObj.value("to1");
                        if(value.isString())
                        {
                            sys_param.task.to1 = value.toString();
                        }
                    }

                    if(taskObj.contains("timestart"))
                    {
                        QJsonValue value = taskObj.value("timestart");
                        if(value.isString())
                        {
                            sys_param.task.timestart = value.toString();
                        }
                    }

                    if(taskObj.contains("timeend"))
                    {
                        QJsonValue value = taskObj.value("timeend");
                        if(value.isString())
                        {
                            sys_param.task.timeend = value.toString();
                        }
                    }

                    if(taskObj.contains("id"))
                    {
                        QJsonValue value = taskObj.value("id");
                        if(value.isDouble())
                        {
                            sys_param.task.id = QString::number(value.toVariant().toInt(),10);
                        }
                    }

                    if(taskObj.contains("reportfreq"))
                    {
                        QJsonValue value = taskObj.value("reportfreq");
                        if(value.isDouble())
                        {
                            sys_param.task.reportfreq = QString::number(value.toVariant().toInt(),10);
                        }
                    }



                    if(taskObj.contains("executeor"))
                    {
                        QJsonValue value = taskObj.value("executeor");
                        {
                            if(value.isArray())
                            {
                                sys_param.task.executeor_list.clear();
                                QJsonArray array = value.toArray();
                                int size = array.size();
                                for(int i=0; i<size; i++)
                                {
                                    QJsonValue arrayVal = array.at(i);
                                    if(arrayVal.isNull())
                                    {
                                        ret = "arrayVal is null";
                                    }
                                    QJsonObject executeor_obj = arrayVal.toObject();
                                    if(executeor_obj.contains("code"))
                                    {
                                        QJsonValue codeVal = executeor_obj.value("code");
                                        if(codeVal.isString())
                                        {
                                            sys_param.task.executeor_list.append(codeVal.toString());
                                            ret = "ok";
                                        }
                                    }
                                }
                            }
                        }
                    }



//                    if(taskObj.contains("logininfo"))
//                    {
//                        QJsonValue value = taskObj.value("logininfo");
//                        {
//                            if(value.isArray())
//                            {
//                                sys_param.task.login_info_list.clear();
//                                QJsonArray array = value.toArray();
//                                int size = array.size();
//                                for(int i=0; i<size; i++)
//                                {
//                                    QJsonValue arrayVal = array.at(i);
//                                    if(arrayVal.isNull())
//                                    {
//                                        ret = "arrayVal is null";
//                                    }
//                                    QJsonObject logininfo_obj = arrayVal.toObject();
//                                    if(logininfo_obj.contains("name"))
//                                    {
//                                        login_info_t info;

//                                        QJsonValue nameVal = logininfo_obj.value("name");
//                                        if(nameVal.isString())
//                                        {
//                                            info.executeor_name = nameVal.toString();
//                                        }
//                                        QJsonValue pwdVal = logininfo_obj.value("pwd");
//                                        if(pwdVal.isString())
//                                        {
//                                            info.executeor_pwd = pwdVal.toString();
//                                        }

//                                        sys_param.task.login_info_list.append(info);
//                                        ret = "ok";
//                                    }
//                                }
//                            }
//                        }
//                    }


                }
            }
        }
    }
    qDebug() << "JsonParse end...";
    return ret;
}


