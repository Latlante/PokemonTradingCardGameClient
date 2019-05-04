#include "ctrlanimation.h"

#include <QDebug>
#include <QEventLoop>
#include <QQmlEngine>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>
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

bool CtrlAnimation::startAnimationMovingCard(Player *currentPlayer, LocationAnimation start, LocationAnimation end)
{
    setMovingCardPlayer(currentPlayer);
    setMovingCardLocationStart(start);
    setMovingCardLocationEnd(end);
    setMovingCardStarted(true);

    //Add timer in case where animation bug
    QTimer timerTimeOutAnimation;
    timerTimeOutAnimation.setSingleShot(true);
    timerTimeOutAnimation.start(2000);

    QEventLoop loop;
    connect(&timerTimeOutAnimation, &QTimer::timeout, &loop, &QEventLoop::quit);
    connect(this, &CtrlAnimation::movingCardFinished, &loop, &QEventLoop::quit);
    loop.exec();

    if(timerTimeOutAnimation.isActive() == false)
    {
        timerTimeOutAnimation.stop();
        qCritical() << __PRETTY_FUNCTION__ << "timer reaches timeOut";

        return false;
    }

    return true;
}

void CtrlAnimation::animationMovingCardFinished()
{
    setMovingCardStarted(false);
    emit movingCardFinished();
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

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
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
