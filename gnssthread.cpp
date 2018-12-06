#include "gnssthread.h"
#include <usart_init.h>


extern PrccessParam_tt PrccessParam;

int gnss_fd;
int gn_nread;			/* Read the counts of data */
char gnss_buff[512];		/* Recvice data buffer */
#define GNSS_DEV_NAME  "/dev/ttymxc1"

static QString GNSSrecvdata;
static int GNdata_state;
#define GNBEGIN 1
#define GNDATA  2
const char * xmit1 = "$CCRMO,,3,*4F\n";
const char * xmit2 = "$CCRMO,GGA,2,1.0*20\n";
bool datahead = false;
QString framedata;
void GNSSThread::run(){

    int data_top,data_bottom;
    QString framehead= "$";
    QString frametail= "\n\n";

    gnss_fd=OpenDev(GNSS_DEV_NAME,9600,8,1,'N');//init usart
    if(gnss_fd>0)
    {
         qDebug("USART OPEN SUSSCE");
    }else
    {
        qDebug("USART OPEN FAILL");
    }

    GNdata_state=GNBEGIN;
    write(gnss_fd, xmit1,strlen(xmit1));
    write(gnss_fd, xmit2,strlen(xmit2));
    while (1)
    {
        gn_nread = read(gnss_fd, gnss_buff, sizeof(gnss_buff));
        if(gn_nread>0){
            try{
                for(int i=0;i<gn_nread;i++){
                    char data = gnss_buff[i];
                    if(data=='$'){
                        datahead = true;
                        framedata.clear();
                        framedata.append(data);
                    }else{
                        if(datahead){
                            framedata.append(data);
                            if((data=='\n')&&(gnss_buff[i-1]=='\n')){
                                this->gnss_data_process(framedata);
                                datahead = false;
                                framedata.clear();
                            }
                        }
                    }

                }
            }catch(...){
                qDebug() << "";
                datahead = false;
                framedata.clear();
            }
        }
    }
}


void GNSSThread::gnss_data_process(QString& str)
{
    int data_top,data_bottom;
    QString framehead= "$";
    QString frametail= "\n\n";



    data_top=str.indexOf(framehead,0);
    data_bottom=str.indexOf(frametail,0);
//qDebug() << "data_top:"<<data_top<<"data_bottom:"<<data_bottom;
    QString bd_data=str.mid((data_top),(data_bottom));//去掉"*AA\r\n"字段
    //检查字符串的校验核是否正确

   if( checkSum(bd_data) == 0 )//check string XOR result
   {
        qDebug()<<"recve error GNSSdata:"<<str;
        return;
   }
    qDebug()<<"recve GNSSdata:"<<bd_data;

    QStringList datalist = bd_data.split(",");
//    qDebug() << "datalist.length() = "<<datalist.length();

    if( datalist.length() > 9)
    {
        QString starstr = datalist[0];

        if(starstr.startsWith("$GNGGA")){

        //        PrccessParam.lon=datalist[2];
        //        PrccessParam.lat=datalist[4];
            PrccessParam.lat_ori = datalist[2];
            PrccessParam.lon_ori = datalist[4];
            PrccessParam.lat=datalist[2];
            PrccessParam.lon=datalist[4];
            PrccessParam.hgt=datalist[9];


            //lat 3803.3969
            // 1. QString -> float
            double lat = PrccessParam.lat.toDouble();
            // 2. float lat -> 38.033969
            lat = lat/100.0;

            // 3. zheng = 38  xiaoshu = 0.033969

            int integer = lat;   // 38

            double decimals = lat - integer; // 0.033969


            // 4. du = zhengshu ;   fen = xiaoshu * 100 = 3.3969
            lat = integer + decimals*100.0/60.0;

            QString slat = QString::number(lat, 10, 8);


            PrccessParam.lat = slat;


            // 1. QString -> float
            double lon = PrccessParam.lon.toFloat();
            // 2. float lat -> 38.033969
            lon = lon/100.0;
            // 3. zheng = 38  xiaoshu = 0.033969
            integer = lon;   // 38
            decimals = lon - integer; // 0.033969
            // 4. du = zhengshu ;   fen = xiaoshu * 100 = 3.3969
            lon = integer + decimals*100.0/60.0;

            QString slon = QString::number(lon, 10, 8);

            PrccessParam.lon = slon;
        }
    }
    else
    {
        qDebug() << "datalist.length() <9";
        return;
    }


}

//void GNSSThread::gnss_pos_report(void)
//{
////    QString msg;

////    msg = "$CCWBA,";
////    msg += PrccessParam.bd_ic_num;
////    msg += "L,0,0060*14\n\n";

//    const char *msg = "$CCWBA,0241638,L,0,0060*14\n\n";
//    write(gnss_fd, msg,strlen(msg));

//}




