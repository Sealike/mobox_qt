#include <QCoreApplication>
#include <QApplication>
#include <stdio.h>
#include <QThread>
#include <nwthread.h>
#include <qfontdatabase.h>
#include <bdthread.h>
#include <Widget.h>
#include <QSplashScreen>
#include <QPixmap>
#include <QMovie>
#include <QDateTime>
#include "suo.h"
#include "keyBoard.h"
#include "gnssthread.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


bool    connection_tcp = false;
bool    connection_bd = false;

bool checkOnly()
{
    const char filename[]  = "/home/root/Resource/lockfile";
    int fd = open (filename, O_WRONLY | O_CREAT , 0644);
    int flock = lockf(fd, F_TLOCK, 0 );
    if (fd == -1) {
            perror("open lockfile/n");
            return false;
    }
    //给文件加锁
    if (flock == -1) {
            perror("lock file error/n");
            return false;
    }
    //程序退出后，文件自动解锁
    return true;
}



int main(int argc, char *argv[])
{
    qDebug("start");

    QApplication a(argc, argv);

    if(checkOnly()==false)
        return 0;

    QFontDatabase database;
    int ret = database.addApplicationFont("./Resource/SourceHanSerifCN-Regular.otf"); //Resource
    if (ret < 0){qDebug("Font not added!");};

    QFont font;
    font.setPixelSize(30);
    a.setFont(font);


/*
    qDebug() << "===============startup===============";
    QMovie *movie = new QMovie("./Resource/login.gif");
    QLabel *label = new QLabel ("", 0, Qt::FramelessWindowHint);
//    label->setGeometry( 300, 300, 500, 500 );
    label->resize(800,480);
    label->setMovie(movie);
    label->setScaledContents(true);
    movie->start();
    label->show();

    QDateTime n=QDateTime::currentDateTime();
    QDateTime now;
    do{
        now=QDateTime::currentDateTime();
        a.processEvents();
    } while (n.secsTo(now)<=3);//3为需要延时的秒数
    label->close();
    qDebug() << "===============startup===============";
*/

    QPixmap pixmap("./Resource/start.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage("正在初始化设备，请稍候...", Qt::AlignBottom|Qt::AlignHCenter, Qt::black);
    a.processEvents();


    BDThread bd;
    bd.start();

    GNSSThread gnss;
    gnss.start();

    suoThread suo;
    suo.start();

    Widget w;
    w.show();

    BDThread::connect(&bd,SIGNAL(sig_cardId_Data(QString)),&w,SLOT(slot_cardId_data(QString)));
    BDThread::connect(&bd,SIGNAL(sig_bd_Data(QString)),&w,SLOT(slot_bd_bsi(QString))); // BDBSI info
    BDThread::connect(&bd,SIGNAL(sig_unlock_result(bool,QString,QString)),&w,SLOT(slot_bd_unlock(bool,QString,QString)));// BD unlock result
    suoThread::connect(&suo,SIGNAL(sig_suo_ack(QString)),&w,SLOT(slot_suo_ack(QString)));  // suo acks

    splash.finish(&w);

    return a.exec();
}


