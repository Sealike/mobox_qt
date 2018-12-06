#include "b2auth.h"

QString B2Auth::urlBase = "http://192.168.1.2/cgi/";

B2Auth::B2Auth(QObject *parent) : QObject(parent)
{
    this->authed = false;
}
