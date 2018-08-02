#ifndef CTRLANIMATION_H
#define CTRLANIMATION_H

#include <QObject>

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
        Location_Bench = 0,
        Location_Deck,
        Location_Hand,
        Location_Fight,
        Location_Reward,
        Location_Trash
    };
    Q_ENUMS(LocationAnimation)

    explicit CtrlAnimation(QObject *parent = nullptr);

    static void declareQML();
    bool install(QQmlApplicationEngine *pEngine);

    void startAnimationMovingCard(LocationAnimation start, LocationAnimation end);
    bool movingCardStarted();
    void setMovingCardStarted(bool start);
    Player* movingCardPlayer();
    void setMovingCardPlayer(Player* play);
    CtrlAnimation::LocationAnimation movingCardLocationStart();
    void setMovingCardLocationStart(CtrlAnimation::LocationAnimation location);
    CtrlAnimation::LocationAnimation movingCardLocationEnd();
    void setMovingCardLocationEnd(CtrlAnimation::LocationAnimation location);

    bool stepInProgress();
    void setStepInProgress(bool inProgress);

signals:
    void movingCardStartedChanged();
    void movingCardPlayerChanged();
    void movingCardLocationStartChanged();
    void movingCardLocationEndChanged();

    void stepInProgressChanged();

private:
    bool m_movingCardStarted;
    Player* m_movingCardPlayer;
    LocationAnimation m_movingCardLocationStart;
    LocationAnimation m_movingCardLocationEnd;

    bool m_stepInProgress;
};

#endif // CTRLANIMATION_H
