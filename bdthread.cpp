#include "bdthread.h"
#include <usart_init.h>

bool isReadCard = false;
SysParam_tt SysParam;
PrccessParam_tt PrccessParam;
const char *userid="8765432222345678";
int fd;
//SysParam_tt SysParam;
//PrccessParam_tt PrccessParam;
//const char *userid="8765432222345678";
const char * read_card = "$CCICA,0,0*4B\n";
const char * xmit = "$CCRMO,BSI,2,2*24\n"; /* Default send data */
int nread;			/* Read the counts of data */
char buff[512];		/* Recvice data buffer */
bool rd_datahead = false;
QString rd_framedata;

#define DEV_NAME  "/dev/ttymxc0"

static QString BDrecvdata;
static int data_state;
#define BEGIN 1
#define DATA  2

void BDThread::readCARD(void)
{
    write(fd, read_card,strlen(read_card));
}
void BDThread::readBSI(void)
{
    write(fd, xmit,strlen(xmit));
}



void BDThread::run(){

    int data_top,data_bottom;
    QString framehead= "$";
    QString frametail= "\n\n";


    qDebug() << "BDThread::run";

    fd=OpenDev(DEV_NAME,115200,8,1,'N');//init usart
    if(fd>0)
    {
         qDebug("USART OPEN SUSSCE");
    }else
    {
        qDebug("USART OPEN FAILL");
    }
    write(fd, xmit,strlen(xmit));
    this->device_init();//system init
    CreatPassword();//unlock password



    data_state=BEGIN;

    while (1)
    {
        nread = read(fd, buff, sizeof(buff));
        if(nread>0){
            try{
                for(int i=0;i<nread;i++){
                    char data = buff[i];
                    if(data=='$'){
                        rd_datahead = true;
                        rd_framedata.clear();
                        rd_framedata.append(data);
                    }else{
                        if(rd_datahead){
                            rd_framedata.append(data);
                            if(data=='*'){
                                rd_framedata.append(buff[i+1]);
                                rd_framedata.append(buff[i+2]);
                                this->rd_rn_data_process(rd_framedata);
                                rd_datahead = false;
                                rd_framedata.clear();
                            }
                        }
                    }

                }
            }catch(...){
                rd_datahead = false;
                rd_framedata.clear();
            }
        }



//        while(PrccessParam.bd_id_local == "")
//        {
//            qDebug() << "read card";
//            readCARD();
//            sleep(1);
//        }
//        nread = read(fd, buff, sizeof(buff));
//        try{
//        if (nread > 0)
//        {
//             buff[nread] = '\0';
//             QString dataframe(buff);
////             qDebug()<<"recve data:"<<dataframe;
//             data_top=dataframe.indexOf(framehead,0);
//             data_bottom=dataframe.indexOf(frametail,0);

//             switch (data_state) {
//             case BEGIN:
//                 if((data_top==0)&&(data_bottom==-1))
//                 {
//                     BDrecvdata+=dataframe;
//                     data_state=DATA;
//                 }else if((data_top==0)&&(data_bottom!=-1))
//                 {
//                     BDrecvdata+=dataframe;
////                     qDebug()<<"recve BD data:"<<BDrecvdata;
//                     this->rd_rn_data_process(BDrecvdata);
//                     BDrecvdata.clear();
//                 }
//                  break;
//             case DATA:
//                 if(data_bottom==-1)
//                 {
//                     BDrecvdata+=dataframe;

//                 }else
//                 {

//                     BDrecvdata+=dataframe;
////                     qDebug()<<"recve BD data:"<<BDrecvdata;
//                     this->rd_rn_data_process(BDrecvdata);
//                     BDrecvdata.clear();
//                     data_state=BEGIN;
//                 }
//                  break;
//             default:
//                 break;
//             }
//        }
//        }catch(...)
//        {
//                sleep(1);
//        }
    }
}


void BDThread::rdTXRCmdProcesss(QString& str)
{
//    qDebug() << " progress bd txr data";
    if(str.startsWith("A4BF10"))//开锁
    {
        if(CheckPassword(str.right(6)))
        {
            this->Signal_unlock_result(true,"unlock",str.right(6));

        }else
        {
            this->Signal_unlock_result(false,"code wrong","err");
        }
    }else if(str.startsWith("A4BF11"))//禁止开锁
    {

        this->Signal_unlock_result(false,"forbid unlock","err");
    }else
    {
        qDebug() << "input rdTXR data error";
    }

}
void BDThread::rd_rn_data_process(QString& str)
{
    int data_top,data_bottom;
    QString framehead= "$";
    QString frametail= "\n\n";

//    qDebug() << "data_top:"<<data_top<<"data_bottom:"<<data_bottom;

//检查字符串的校验核是否正确
    if( !checkSum(str) )//check string XOR result
    {
         qDebug()<<"recve error RDSSdata:"<<str;
         return;
    }

    data_top=str.indexOf(framehead,0);
    data_bottom=str.indexOf(frametail,0);

    QString bd_data=str.mid((data_top),(data_bottom-3));//去掉"*AA\r\n"字段
    QStringList datalist = bd_data.split(",");
    QString starstr = datalist[0];

    if(starstr.startsWith("$BDBSI")){//recv BD BSI
        qDebug() <<"the bd BSI is :"<< str;// send TO screen

        this->Signal_bdData(str);

    }else if(starstr.startsWith("$BDICI"))//$BDICI,0241636,24000000,1933442,6,60,3,N,0*30            //recv BD ID
    {
        QString bd_id_str=datalist[1];//save bd ic card num
        PrccessParam.bd_id_local = bd_id_str;
        isReadCard = true;
        emit sig_cardId_Data(bd_id_str);
        qDebug() << "the bd id is :"<< bd_id_str;
    }else if (starstr.startsWith("$BDTXR"))
    {//revc BD Messgae

        qDebug() << " the bd txr data"<<str;
        rdTXRCmdProcesss(datalist[5]);

    }
//    else if(starstr.startsWith("$DEBUG"))
//    {
//        qDebug() <<"recv $DEBUG :"<< "$BDBSI,00,00,0,0,0\n";// send TO screen

//        this->Signal_bdData(QString("$BDBSI,00,00,0,0,0\n"));

//    }
    else
    {
        qDebug() << "input bdrn data error";
    }


}
void BDThread::device_init(void)
{
    memcpy(SysParam.UserID,userid,16);//系统初始化，配置设备ID
    Char2Qstring(SysParam.UserID,16,PrccessParam.Dev_ID);

    PrccessParam.role="app";
    PrccessParam.executor="007";
    PrccessParam.name="abc";
    PrccessParam.pwd="123";
    PrccessParam.bd_ic_num="0241638";
    PrccessParam.seqno="10";
    PrccessParam.taskid="0";
    PrccessParam.lon="0.0";
    PrccessParam.lat="0.0";
    PrccessParam.hgt="0.0";
    PrccessParam.reception_4G = 0;
    PrccessParam.reception_BD = 0;
    PrccessParam.connection_BT = false;
    PrccessParam.battery_remain = 3;
    PrccessParam.system_time = "20:00";
    PrccessParam.bd_id_local = "";

    //test
    PrccessParam.lon="114.645892";
    PrccessParam.lat="38.208924";
    PrccessParam.hgt="67";

}


void BDThread::bd_unlock_cmd(void)
{
    QString msg;
    CreatPassword();//unlock password
//    $CCTXA,0123456,1,2,,A4BF1000200155512738F99088CBE27A916CB179732B33295183*35

    msg = "$CCTXA,";
    msg += PrccessParam.bd_ic_num;
    msg += ",1,2,A4BF10";
    msg += PrccessParam.Chuan;
    msg += PrccessParam.MD5_A;

    QByteArray data = msg.toUtf8();
    char mask = data[1];
    for (int i = 2; i < data.length(); i++)
    {
        mask ^= data[i];
    }
    QString maskstr = QString::number(((mask & 0xff) >> 4),16);
    maskstr += QString::number((mask & 0x0f),16);
    //qDebug() << "mask:" << maskstr;
    data.append('*');
    data.append(maskstr);
    data.append("\n");

    qDebug() << "send bd data:" << data;
    qDebug() <<"_________________________";
    write(fd, data,strlen(data));
}

// bd position report
void BDThread::bd_pos_report()
{

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("hhmmss.ss");

//    const char* msg = "$CCWAA,1,0,0241638,12000000,12.0000,N,13.0000,E,14.0000,M\n\n";
    QString  msg;


    msg = "$CCWAA,1,0,";
    msg += PrccessParam.bd_ic_num;
    msg += ",";
    msg += current_date;
    msg += ",";
    msg += PrccessParam.lat_ori;
    msg += ",N,";
    msg += PrccessParam.lon_ori;
    msg += ",E,";
    msg += PrccessParam.hgt;
    msg += ",M";



    QByteArray data = msg.toUtf8();
    char mask = data[1];
    for (int i = 2; i < data.length(); i++)
    {
        mask ^= data[i];
    }
    QString maskstr = QString::number(((mask & 0xff) >> 4),16);
    maskstr += QString::number((mask & 0x0f),16);
    //qDebug() << "mask:" << maskstr;
    data.append('*');
    data.append(maskstr);
    data.append("\n");
    qDebug() << "send bd data:" << data;
    qDebug() <<"_________________________";
    write(fd, data,strlen(data));






//    char*  ch;

//    QString check_data = make_checkSum(msg);//计算校验核
//    msg=msg+check_data;
//    //msg += "\n\n";

////            QByteArray json_out = json.toUtf8();


//    qDebug() << "bd_pos_report_msg : " << msg;

//    QByteArray ba = msg.toLatin1(); // must
//    ch=ba.data();

//    qDebug() << "bd_pos_report_ch : " << ch;


//    int len = strlen(ch);

//    qDebug() << "strlen(ch) : " << len;

//    int wr = write(fd, ch, len);

//    qDebug() << "write return : " << wr;


//    char ret[20];
//    std::string temp = msg.toStdString();
//    ret = temp.c_str();
//    const char *msg = "$CCWBA,0241638,L,0,0060*14\n\n";
//    write(fd, msg, strlen(msg));

}
