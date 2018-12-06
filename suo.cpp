#include "suo.h"
#include <usart_init.h>


int suo_fd;
int suo_nread;			/* Read the counts of data */
char suo_buff[512];		/* Recvice data buffer */
#define SUO_DEV_NAME   "/dev/ttymxc2"

static QString SUOrecvdata;
static int SUOdata_state;

suoParam_tt suoParam;
#define SUOBEGIN 1
#define SUODATA  2

const char * STATE_ACK  = "$STATE,ACK*32\n";
const char * OLOCK_ON   = "$OLOCK,ON*32\n";
const char * OLOCK_OFF  = "$OLOCK,OFF*27\n";
const char * FINGE_OPEN = "$FINGE,OPEN*E2\n";
const char * INPUT_OK   = "$INPUT,OK*7E\n";
const char * INPUT_ERR  = "$INPUT,ERR*3F\n";
const char * CLEAR      = "$CLEAR,ON*74\n";
const char * JUDGE_ACK  = "$JUDGE,ACK*3C\n";

void suoThread::run(){

    int data_top,data_bottom;
    QString framehead= "$";
    QString frametail= "\n\n";

    suo_fd=OpenDev(SUO_DEV_NAME,115200,8,1,'N');//init usart
    if(suo_fd>0)
    {
         qDebug("USART suo OPEN SUSSCE");
    }else
    {
        qDebug("USART suo OPEN FAILL");
    }
//init dev suo
    SUOdata_state=SUOBEGIN;

    suoParam.Suo_Qiao=QIAONWARN;
    suoParam.Suo_State=SUO_ON;
    suoParam.BLE_State=BLE_NCONNECT;

    suoParam.Suo_allSound=S_ON;
    suoParam.Suo_timeoutSound=S_TIMEOUT_ON;
    suoParam.Suo_batterySound=S_BATTERT_ON;
    suoParam.Suo_bleSound=S_BLE_ON;
    suoParam.Suo_positionSound=S_POSITION_ON;


    while (1)
    {
        suo_nread = read(suo_fd, suo_buff, sizeof(suo_buff));
        try{
        if (suo_nread > 0)
        {
             suo_buff[suo_nread] = '\0';
             QString dataframe(suo_buff);
             qDebug()<<"SUO recve data:"<<dataframe;
             data_top=dataframe.indexOf(framehead,0);
             data_bottom=dataframe.indexOf(frametail,0);
                qDebug()<<"framehead:"<<data_top<<"frametail:"<<data_bottom;
             switch (SUOdata_state) {
             case SUOBEGIN:
                 if((data_top==0)&&(data_bottom==-1))
                 {
                     SUOrecvdata+=dataframe;
                     SUOdata_state=SUODATA;
                 }else if((data_top==0)&&(data_bottom!=-1))
                 {
                     SUOrecvdata+=dataframe;
                     qDebug()<<"recve data:"<<SUOrecvdata;
                     this->suo_recvdata_process(SUOrecvdata);
                     SUOrecvdata.clear();
                 }
                  break;
             case SUODATA:
                 if(data_bottom==-1)
                 {
                     SUOrecvdata+=dataframe;

                 }else
                 {
                     SUOrecvdata+=dataframe;
                     qDebug()<<"recve data:"<<SUOrecvdata;
                     this->suo_recvdata_process(SUOrecvdata);
                     SUOrecvdata.clear();
                     SUOdata_state=SUOBEGIN;
                 }
                  break;
             default:
                 break;
             }
        }
    }catch(...){
                sleep(1);
    }
    }
}


void suoThread::suo_recvdata_process(QString& str)
{
    int data_top,data_bottom;
    QString framehead= "$";
    QString frametail= "\n\n";

//    qDebug() << "data_top:"<<data_top<<"data_bottom:"<<data_bottom;

//检查字符串的校验核是否正确
//    checkSum(bd_data);//check string XOR result
//    qDebug() << bd_data;

    data_top=str.indexOf(framehead,0);
    data_bottom=str.indexOf(frametail,0);

    QString bd_data=str.mid((data_top),(data_bottom-3));//去掉"*AA\r\n"字段
    QStringList datalist = bd_data.split(",");
    QString starstr = datalist[0];

    if(starstr.startsWith("$STATE")){
        suoParam.Suo_Qiao=datalist[1];
        suoParam.Suo_State=datalist[2];
        suoParam.BLE_State=datalist[3];
        write(suo_fd, STATE_ACK,strlen(STATE_ACK));
        //signal
        Signal_suo_ack("$STATE,ACK");


    }else if(starstr.startsWith("$SOUND"))
    {
        if(datalist[1]=="OK")
        {
            Signal_suo_ack("$SOUND,OK");

        }else if(datalist[1]=="ERR")
        {
            Signal_suo_ack("$SOUND,ERR");

        }else
        {
            qDebug()<<"input SOUND cmd err";
        }

    }else if (starstr.startsWith("$OLOCK"))
    {
        qDebug() << "starstr" << starstr;
        if(datalist[1]=="OK")
        {
            Signal_suo_ack("$OLOCK,OK");

        }else if(datalist[1]=="ERR")
        {
            Signal_suo_ack("$OLOCK,OK");

        }else
        {
            qDebug()<<"input OLOCK cmd err";
        }

    }else if (starstr.startsWith("$FINGE"))
    {
        if(datalist[1]=="OK")
        {
            Signal_suo_ack("$FINGE,OK");

        }else if(datalist[1]=="ERR")
        {
            Signal_suo_ack("$FINGE,ERR");

        }else
        {
            qDebug()<<"input FINGE cmd err";
        }

    }else if (starstr.startsWith("$INPUT"))
    {
        if(datalist[1]=="01")
        {
            Signal_suo_ack("$INPUT,01");

        }else if(datalist[1]=="02")
        {
            Signal_suo_ack("$INPUT,02");

        }else
        {
            qDebug()<<"INPUT INPUT cmd err";
        }

    }else if (starstr.startsWith("$CLEAR"))
    {
        if(datalist[1]=="OK")
        {
            Signal_suo_ack("$CLEAR,OK");

        }else if(datalist[1]=="ERR")
        {
            Signal_suo_ack("$CLEAR,ERR");

        }else
        {
            qDebug()<<"INPUT CLEAR cmd err";
        }

    }else if (starstr.startsWith("$JUDGE"))
    {
        if(datalist[1]=="01")//指纹判断  符合
        {
            Signal_suo_ack("$JUDGE,OK");

        }else if(datalist[1]=="02")//指纹判断  不符合
        {
            Signal_suo_ack("$JUDGE,ERR");

        }else
        {
            qDebug()<<"INPUT JUDGE cmd err";
        }

    }else
    {
        qDebug()<<"The cmd come from suo is ERR !!!!!!!";

    }

}

//suoParam.Suo_allSound=SUO_OPEN;
//suo_control("$SOUND");

bool suoThread::suo_control(QString str)
{
    QString send_data;
    QByteArray data;
    QString maskstr;
    char mask;
    bool successFlag = false;
    if(str.startsWith("$SOUND"))
    {
        send_data.clear();
        send_data+="$SOUND";
        send_data+=suoParam.Suo_allSound;
        send_data+=suoParam.Suo_timeoutSound;
        send_data+=suoParam.Suo_batterySound;
        send_data+=suoParam.Suo_positionSound;
        send_data+=suoParam.Suo_bleSound;

        data = send_data.toUtf8();
        mask = data[1];
        for (int i = 2; i < data.length(); i++)
        {
            mask ^= data[i];
        }
        maskstr = QString::number(((mask & 0xff) >> 4),16);
        maskstr += QString::number((mask & 0x0f),16);
        //qDebug() << "mask:" << maskstr;
        data.append('*');
        data.append(maskstr);
        data.append("\n");

        qDebug() << "send SOUND cmd data:" << data;
        qDebug() <<"_________________________";
        write(suo_fd, data,strlen(data));
        successFlag = true;


    }else if (str.startsWith("$OLOCK"))
    {
        if(suoParam.Suo_State==SUO_OPEN)
        {
            write(suo_fd, OLOCK_ON,strlen(OLOCK_ON));
        }else if(suoParam.Suo_State==SUO_CLOSE)
        {
            write(suo_fd, OLOCK_OFF,strlen(OLOCK_OFF));
        }else
        {
            qDebug() << "OLOCK CMD ERR" ;
        }
        successFlag = true;

    }else if (str.startsWith("$FINGE"))
    {
        qDebug() << "SEND SUO CMD FINGE";
        write(suo_fd, FINGE_OPEN,strlen(FINGE_OPEN));
        successFlag = true;

    }else if (str.startsWith("$INPUT"))
    {
        write(suo_fd, INPUT_OK,strlen(INPUT_OK));
        successFlag = true;

    }else if (str.startsWith("$CLEAR"))
    {
        qDebug() << "SEND SUO CMD CLEAR";
         write(suo_fd, CLEAR,strlen(CLEAR));
         successFlag = true;

    }else if (str.startsWith("$JUDGE"))
    {
         write(suo_fd, JUDGE_ACK,strlen(JUDGE_ACK));
         successFlag = true;
    }
    else
    {
        qDebug() << "The cmd of LOCK control is ERR" ;
        return successFlag;
    }
    return  successFlag;
}
