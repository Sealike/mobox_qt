#ifndef TOOL_H
#define TOOL_H

QString make_checkSum(QString & str);
int checkSum( QString & str);
int CheckPassword(QString str);
void CreatPassword(void);
unsigned int Curl_rand(void);
unsigned char LongToAscii(unsigned int SrcData,unsigned char * DataBuff,unsigned int Len);
unsigned char LongHToAscii(unsigned char* SrcBuff,unsigned char* DataBuff,unsigned int Len);
void Char2Qstring(unsigned char *buf,int data_length,QString & str);
#endif // TOOL_H
