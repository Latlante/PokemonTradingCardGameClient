#ifndef FIGHTAREA_H
#define FIGHTAREA_H

#include "abstractpacketstatic.h"

class CardPokemon;

class FightArea : public AbstractPacketStatic
{
    Q_OBJECT
    Q_PROPERTY(CardPokemon* pokemonFighter READ pokemonFighter NOTIFY pokemonFighterChanged)

public:
    FightArea(const QString& namePacket, QList<AbstractCard*> listCards = QList<AbstractCard*>());
    virtual ~FightArea();

    static void declareQML();
	
    int maxCards() const override;

    CardPokemon* pokemonFighter();
    Q_INVOKABLE CardPokemon* pokemonFighting(int index);

signals:
    void pokemonFighterChanged();

private slots:
    void onCountChanged();
};

#endif // FIGHTAREA_H
