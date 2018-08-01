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

    //Accesseurs
    Player* playerYou();
    void setPlayerYou(unsigned int uid, const QString &name);
    Player* playerOpponent();
    void setPlayerOpponent(unsigned int uid, const QString &name);
    Player* playerByName(const QString& name);


private:
	QList<Player*> m_listPlayers;
    Player* m_playerYou;
    Player* m_playerOpponent;
    ConstantesQML::StepGame m_gameStatus;
	
	bool m_gameIsReady;
    //QList<AbstractCard*> m_listSelectionCards;
    QVariant m_elementFromDisplays;


};

#endif // GAMEMANAGER_H
