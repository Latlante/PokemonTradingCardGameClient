#ifndef CTRLANIMATION_H
#define CTRLANIMATION_H

#include <QObject>
#include "Share/constantesshared.h"

class QQmlEngine;
class QQmlApplicationEngine;
class Player;

class CtrlAnimation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool movingCardStarted READ movingCardStarted WRITE setMovingCardStarted NOTIFY movingCardStartedChanged)
    Q_PROPERTY(LocationAnimation movingCardLocationStart READ movingCardLocationStart WRITE setMovingCardLocationStart NOTIFY movingCardLocationStartChanged)
    Q_PROPERTY(LocationAnimation movingCardLocationEnd READ movingCardLocationEnd WRITE setMovingCardLocationEnd NOTIFY movingCardLocationEndChanged)
    Q_PROPERTY(Player* movingCardPlayer READ movingCardPlayer WRITE setMovingCardPlayer NOTIFY movingCardPlayerChanged)

    Q_PROPERTY(bool stepInProgress READ stepInProgress WRITE setStepInProgress NOTIFY stepInProgressChanged)

public:
    enum LocationAnimation
    {
        Location_Bench = ConstantesShared::PACKET_Bench,
        Location_Deck = ConstantesShared::PACKET_Deck,
        Location_Hand = ConstantesShared::PACKET_Hand,
        Location_Fight = ConstantesShared::PACKET_Fight,
        Location_Reward = ConstantesShared::PACKET_Rewards,
        Location_Trash = ConstantesShared::PACKET_Trash
    };
    Q_ENUMS(LocationAnimation)

    explicit CtrlAnimation(QObject *parent = nullptr);

    static void declareQML();
    bool install(QQmlApplicationEngine *pEngine);


    //ANIMATION MOVING CARD
    bool startAnimationMovingCard(LocationAnimation start, LocationAnimation end);
    Q_INVOKABLE void animationMovingCardFinished();

    //ANIMATION P_LOADER
    bool stepInProgress();
    void setStepInProgress(bool inProgress);

signals:
    void movingCardStartedChanged();
    void movingCardPlayerChanged();
    void movingCardLocationStartChanged();
    void movingCardLocationEndChanged();
    void movingCardFinished();

    void stepInProgressChanged();

private:
    //ANIMATION MOVING CARD
    bool m_movingCardStarted;
    Player* m_movingCardPlayer;
    LocationAnimation m_movingCardLocationStart;
    LocationAnimation m_movingCardLocationEnd;

    //ANIMATION P_LOADER
    bool m_stepInProgress;

    //ANIMATION MOVING CARD
    bool movingCardStarted();
    void setMovingCardStarted(bool start);
    Player* movingCardPlayer();
    void setMovingCardPlayer(Player* play);
    CtrlAnimation::LocationAnimation movingCardLocationStart();
    void setMovingCardLocationStart(CtrlAnimation::LocationAnimation location);
    CtrlAnimation::LocationAnimation movingCardLocationEnd();
    void setMovingCardLocationEnd(CtrlAnimation::LocationAnimation location);

};

#endif // CTRLANIMATION_H
