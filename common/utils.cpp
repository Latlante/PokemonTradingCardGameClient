#include "utils.h"

#include <QDateTime>
#include <QFile>
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

QString Utils::ipAddress()
{
    QString ipAddress;
    QFile fileAccess("Files/access.txt");

    if(fileAccess.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString content = fileAccess.readAll();
        fileAccess.close();

        ipAddress = content.section("\n", 0, 0);
    }

    return ipAddress;
}

int Utils::port()
{
    int port = 0;
    QFile fileAccess("Files/access.txt");

    if(fileAccess.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString content = fileAccess.readAll();
        fileAccess.close();

        port = content.section("\n", 1, 1).toInt();
    }

    return port;
}

int Utils::randomValue(int min, int max)
{
    return (qrand() %(max - min + 1)) + min;
}
