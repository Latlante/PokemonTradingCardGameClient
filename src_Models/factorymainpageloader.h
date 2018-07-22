#ifndef FACTORYMAINPAGELOADER_H
#define FACTORYMAINPAGELOADER_H

#include <QObject>

class FactoryMainPageLoader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString link READ link NOTIFY linkChanged)

public:
    explicit FactoryMainPageLoader(QObject *parent = nullptr);

    static void declareQML();

    QString link();

    void displayLogin();
    void displayAllPlayers();
    void displayAllGamesAvailable();
    void displayCreateChooseGame();
    void displaySelectPlayers();
    void displaySelectCards();
    void displayBoard();

signals:
    void linkChanged();

private:
    QString m_link;

    void setLink(const QString& link);
};

#endif // FACTORYMAINPAGELOADER_H
