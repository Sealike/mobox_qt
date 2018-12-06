#ifndef BD_TYPE_H
#define BD_TYPE_H
#include <QString>


typedef struct
{
    unsigned char UserName[20];
    unsigned char UserPWD[20];
    unsigned char UserID[20];

    unsigned int DigstCurldata;
    unsigned char Chuan[20];
    unsigned char MD5_A[40];
    unsigned char MD5_B[40];
    unsigned char str_temp[50];
    unsigned char str_temp2[20];

} SysParam_tt;
typedef struct
{
    QString role;//角色
    QString name;//角色
    QString executor;//执行人
    QString pwd;//密码
    QString bd_ic_num;

    QString seqno;//消息序列号
    QString taskid;//任务号
    QString time;//时间、随机串

    QString lon;//纬度
    QString lat;//经度
    QString hgt;//高程

    QString lat_ori;
    QString lon_ori;

    QString Chuan;
    QString Dev_ID;
    QString MD5_A;
    QString MD5_B;

    QString bd_id_local;


    int     reception_4G;   //4G信号强度
    int     reception_BD;   //BD信号强度
    bool    connection_BT;  //蓝牙连接状态
    int     battery_remain; //电池电量
    QString system_time;    //系统时间
    QString bdbsi;

    QString sess_name;
    QString sess_pwd;

} PrccessParam_tt, *PrccessParam_pt;




//suo state ------------------------------------------
//Suo_Qiao
#define     QIAO_WARN       "01"
#define     QIAONWARN       "02"
//Suo_State
#define     SOU_OFF         "01"
#define     SUO_ON          "02"
//BLE_State
#define     BLE_CONNECT     "01"
#define     BLE_NCONNECT    "02"

//control cmd------------------------------------------
//Suo_control
#define     SUO_OPEN    "ON"
#define     SUO_CLOSE   "OFF"
//Suo_allSound
#define     S_ON    "A1"
#define     S_OFF   "A2"
//Suo_timeoutSound
#define     S_TIMEOUT_ON    "B1"
#define     S_TIMEOUT_OFF   "B2"
//Suo_timeoutSound
#define     S_BATTERT_ON    "C1"
#define     S_BATTERT_OFF   "C2"
//Suo_timeoutSound
#define     S_POSITION_ON    "D1"
#define     S_POSITION_OFF   "D2"
//Suo_timeoutSound
#define     S_BLE_ON    "E1"
#define     S_BLE_OFF   "E2"

typedef struct
{
    //state
    QString Suo_State;//on or off
    QString BLE_State;//connect or nconnect
    QString Suo_Qiao;//warn or nwarn

    //control cmd
    QString Suo_control;

    QString Suo_allSound;//on or off

    QString Suo_timeoutSound;//on or off
    QString Suo_batterySound;//on or off
    QString Suo_positionSound;//on or off
    QString Suo_bleSound;//on or off

} suoParam_tt;
#endif // BD_TYPE_H
