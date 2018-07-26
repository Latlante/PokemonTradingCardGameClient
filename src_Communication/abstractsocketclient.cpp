#include "abstractsocketclient.h"

AbstractSocketClient::AbstractSocketClient(QObject *parent) :
    QObject(parent),
    m_timeOut(10000)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int AbstractSocketClient::timeOut()
{
    return m_timeOut;
}

void AbstractSocketClient::setTimeOut(int timer)
{
    m_timeOut = timer;
}
