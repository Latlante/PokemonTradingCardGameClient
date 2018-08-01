#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include "player.h"

#include "common/constantesqml.h"
#include "src_Cards/abstractcard.h"
#include "src_Cards/cardpokemon.h"

class CtrlPopups;

class GameManager : public QObject
{
	Q_OBJECT
	
public:
    GameManager(QObject *parent = nullptr);
	~GameManager();

    bool isReady();

    //Accesseurs
    unsigned int uidGame();
    void setUidGame(unsigned int uid);
    Player* playerYou();
    void setPlayerYou(unsigned int uid, const QString &name);
    Player* playerOpponent();
    void setPlayerOpponent(unsigned int uid, const QString &name);
    Player* playerByName(const QString& name);


private:
    unsigned int m_uidGame;
    Player* m_playerYou;
    Player* m_playerOpponent;
    ConstantesQML::StepGame m_gameStatus;

};

#endif // GAMEMANAGER_H
