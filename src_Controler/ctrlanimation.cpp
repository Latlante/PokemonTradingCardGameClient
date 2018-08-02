#include "ctrlanimation.h"

#include <QDebug>
#include <QQmlEngine>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml/qqml.h>

#include "player.h"

CtrlAnimation::CtrlAnimation(QObject *parent) :
    QObject(parent),
    m_movingCardStarted(false),
    m_movingCardPlayer(nullptr),
    m_movingCardLocationStart(Location_Bench),
    m_movingCardLocationEnd(Location_Bench),
    m_stepInProgress(false)
{

}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void CtrlAnimation::declareQML()
{
    qmlRegisterUncreatableType<CtrlAnimation>("model", 1, 0, "CtrlAnimation", "CtrlAnimation error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
bool CtrlAnimation::install(QQmlApplicationEngine *pEngine)
{
    qDebug() << "installing CtrlAnimation...";

    bool bInstalled = false;

    if (nullptr != pEngine)
    {
        QQmlContext* pContext = pEngine->rootContext();
        if (nullptr != pContext)
        {
            bInstalled = true;
            qDebug() << "CtrlAnimation is installed in QML engine";
            pContext->setContextProperty("ctrlAnimation", this);
        }
        else
        {
            qCritical() << "Qml context is null.";
        }
    }

    return bInstalled;
}

void CtrlAnimation::startAnimationMovingCard(LocationAnimation start, LocationAnimation end)
{
    setMovingCardLocationStart(start);
    setMovingCardLocationEnd(end);
    setMovingCardStarted(true);
}

bool CtrlAnimation::movingCardStarted()
{
    return m_movingCardStarted;
}

void CtrlAnimation::setMovingCardStarted(bool start)
{
    if(m_movingCardStarted != start)
    {
        m_movingCardStarted = start;
        emit movingCardStartedChanged();
    }
}

Player* CtrlAnimation::movingCardPlayer()
{
    return m_movingCardPlayer;
}

void CtrlAnimation::setMovingCardPlayer(Player *play)
{
    if(m_movingCardPlayer != play)
    {
        m_movingCardPlayer = play;
        emit movingCardPlayerChanged();
    }
}

CtrlAnimation::LocationAnimation CtrlAnimation::movingCardLocationStart()
{
    return m_movingCardLocationStart;
}

void CtrlAnimation::setMovingCardLocationStart(CtrlAnimation::LocationAnimation location)
{
    if(m_movingCardLocationStart != location)
    {
        m_movingCardLocationStart = location;
        emit movingCardLocationStartChanged();
    }
}

CtrlAnimation::LocationAnimation CtrlAnimation::movingCardLocationEnd()
{
    return m_movingCardLocationEnd;
}

void CtrlAnimation::setMovingCardLocationEnd(CtrlAnimation::LocationAnimation location)
{
    if(m_movingCardLocationEnd != location)
    {
        m_movingCardLocationEnd = location;
        emit movingCardLocationEndChanged();
    }
}

bool CtrlAnimation::stepInProgress()
{
    return m_stepInProgress;
}

void CtrlAnimation::setStepInProgress(bool inProgress)
{
    if(m_stepInProgress != inProgress)
    {
        m_stepInProgress = inProgress;
        emit stepInProgressChanged();
    }
}
