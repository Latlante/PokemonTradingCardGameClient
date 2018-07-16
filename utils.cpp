#include "utils.h"

#include <QDateTime>
#include "common/database.h"

Utils::Utils(QObject *parent) : QObject(parent)
{

}

void Utils::initRandomValues()
{
    qsrand(QDateTime::currentDateTime().toTime_t());
}

unsigned short Utils::headOrTail()
{
    return static_cast<short>(randomValue(0, 1));
}

unsigned short Utils::selectFirstPlayer(int count)
{
    return static_cast<short>(randomValue(0, count));
}

unsigned short Utils::selectRandomCardsPokemon()
{
    return randomValue(0, 11);
}

unsigned short Utils::selectRandomCardsEnergy()
{
    return randomValue(INDEX_START_ENERGIES, INDEX_START_ENERGIES+13);
}

int Utils::randomValue(int min, int max)
{
    return (qrand() %(max - min + 1)) + min;
}
