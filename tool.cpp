#include <QCoreApplication>

#include <QDebug>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>
#include <getopt.h>
#include <string.h>

#include <QString>

#include <QJsonObject>
#include <QJsonDocument>

#include <tool.h>
#include <bd_type.h>
#include <mmmd.h>

#define printf qDebug
extern SysParam_tt SysParam;
extern PrccessParam_tt PrccessParam;

int checkSum( QString & str)
{
    //"$BDBSI,02,03,2,4,3*62"  the str has delate \r\n
     QByteArray data = str.toUtf8();
     char mask = data[1];
     int datalen=data.length()-3;
     for (int i = 2; i <datalen; i++) {
         mask ^= data[i];
     }
     QString maskstr = QString::number(((mask & 0xff) >> 4),16);
     maskstr += QString::number((mask & 0x0f),16);
     maskstr = maskstr.toUpper();

     QString ret = str.right(2);

     if(maskstr == ret)
     {
         return 1;
     }else {
         qDebug() << "checksum Error!";
         return 0;
     }

}
QString make_checkSum(QString & str)
{
//    char high_byte,low_byte,check_resust;
    unsigned char checkSum_data[2];
    QString mask_str;

//    qDebug("Do string checksum.");
//    qDebug()<<str;
   //"$BDBSI,02,03,2,4,3*62"  the str has delate \r\n
    QByteArray data = str.toUtf8();
    unsigned char mask = data[1];//the data[0]=$
    int datalen=data.length();
    for (int i = 2; i <datalen; i++) {
        mask ^= data[i];
    }
    LongHToAscii(&mask,checkSum_data,1);
    mask_str.clear();
    mask_str+=checkSum_data[0];
    mask_str+=checkSum_data[1];
    mask_str="*"+mask_str+"\r\n";
    return mask_str;
}

void CreatPassword(void)
{
        SysParam.DigstCurldata= Curl_rand();//生产随机数列
        LongToAscii( SysParam.DigstCurldata,SysParam.Chuan,16);//将随机数列转换成ASCII串存在DigstCurldata中

        qDebug() <<"DigstCurldata is:";
        Char2Qstring((SysParam.Chuan+2),14,PrccessParam.Chuan);

        memcpy(SysParam.str_temp,(SysParam.Chuan+2),14);
        memcpy((SysParam.str_temp+14),SysParam.UserID,16);
        MDDDn(SysParam.str_temp,30,SysParam.str_temp2);//Calculation MD5_A
        LongHToAscii(SysParam.str_temp2,SysParam.MD5_A,32);

        qDebug() << "MD5_A:";
        Char2Qstring(SysParam.MD5_A,32,PrccessParam.MD5_A);

        memcpy(SysParam.str_temp,(SysParam.Chuan+2),14);//add chuan
        memcpy((SysParam.str_temp+14),SysParam.UserID,16);//add+id
        memcpy((SysParam.str_temp+30),SysParam.UserID,16);//add+id+id
        MDDDn(SysParam.str_temp,46,SysParam.str_temp2);
        LongHToAscii(SysParam.str_temp2,SysParam.MD5_B,32);

        qDebug() << "MD5_B:";
        Char2Qstring((SysParam.MD5_B+26),6,PrccessParam.MD5_B);

}
void Char2Qstring(unsigned char *buf,int data_length,QString & str)
{
//    QString str;
    str.clear();
    for(int i=0;i<data_length;i++){
        str+=buf[i];
    }
    qDebug() << str;
}

int CheckPassword(QString str)//不可重入函数
{
    qDebug() << "CheckPassword str:"<<str<<"MD5_B:"<<PrccessParam.MD5_B;
    if(str==PrccessParam.MD5_B)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
//将整型数转化成字符
unsigned char LongToAscii(unsigned int SrcData,unsigned char * DataBuff,unsigned int Len)
{
    unsigned int		i=0, n=0;
    unsigned int		CNumber = 1;
    unsigned int		CNumber1 = 1;
    unsigned int		Temp;

    if (Len > 0)
    {
        for (i=0; i<Len; i++)
        {
            CNumber = CNumber * 10;
            Temp = SrcData % CNumber;
            DataBuff[Len-i-1] = Temp/CNumber1 + 48;
            SrcData = SrcData - Temp;
            CNumber1 = CNumber1 * 10;
        }
        return	Len;
    }
    else
    {
        while (1)
        {
            if (SrcData >= CNumber)
            {
                n++;
                CNumber = CNumber * 10;
                if (n > 10)
                {
                    return 0;
                }
            }
            else
            {
                CNumber = 1;
                break;
            }
        }
        if (SrcData == 0)
        {
            n = 1;
        }
        for (i=0; i<n; i++)
        {
            CNumber = CNumber * 10;
            Temp = SrcData % CNumber;
            DataBuff[n-i-1] = Temp/CNumber1 + 48;
            SrcData = SrcData - (SrcData % CNumber);
            CNumber1 = CNumber1 * 10;
        }
        return n;
    }
}
static unsigned int randseed;
unsigned int Curl_rand(void)
{
  unsigned int r;
 randseed = rand()+1;
  r = randseed = randseed * 1103515245 + 12345;
  return (r << 16) | ((r >> 16) & 0xFFFF);
}

unsigned char LongHToAscii(unsigned char* SrcBuff,unsigned char* DataBuff,unsigned int Len)
{
   unsigned int		i;

   for (i=0; i<Len; i++)
   {
       if (((SrcBuff[i]>>4)>=0) && ((SrcBuff[i]>>4)<=9))
       {
           DataBuff[i*2] = (SrcBuff[i]>>4) + 48;
       }
       else if (((SrcBuff[i]>>4)>=10) && ((SrcBuff[i]>>4)<=15))
       {
           DataBuff[i*2] = (SrcBuff[i]>>4) + 55;
       }
       else
       {
           return 0;
       }
       if (((SrcBuff[i]&0xF)>=0) && ((SrcBuff[i]&0xF)<=9))
       {
           DataBuff[i*2+1] = (SrcBuff[i]&0xF) + 48;
       }
       else if (((SrcBuff[i]&0xF)>=10) && ((SrcBuff[i]&0xF)<=15))
       {
           DataBuff[i*2+1] = (SrcBuff[i]&0xF) + 55;
       }
       else
       {
           return 0;
       }
   }
   return 1;
}

