#ifndef CTRLSELECTINGCARDS_H
#define CTRLSELECTINGCARDS_H

#include <QObject>

#include "src_Models/modelselectingcards.h"

class QQmlEngine;
class QQmlApplicationEngine;

class CtrlSelectingCards : public QObject
{
    Q_OBJECT
public:
    explicit CtrlSelectingCards(QObject *parent = nullptr);
    ~CtrlSelectingCards();

    static void declareQML();
    bool install(QQmlApplicationEngine *pEngine);

    void newSelection(const QString& name, bool lastPlayer = false);

    QMap<QString,QList<AbstractCard*> > listCardsByPlayer();
    Q_INVOKABLE ModelSelectingCards* model();

    Q_INVOKABLE void addANewCard(int id);
    Q_INVOKABLE void removeACard(int id);
    Q_INVOKABLE void changeQuantityCard(int id, int quantity);
    Q_INVOKABLE void fillARandomList();
    Q_INVOKABLE void onClickedListFinished();

    Q_INVOKABLE void savePacket();
    Q_INVOKABLE void loadPacket();

    void selectCards(const QStringList& listOfPlayers);

signals:
    void listsComplete();

private:
    ModelSelectingCards* m_modelSelectingCards;
    QMap<QString,QList<InfoCard> > m_listCardsByPlayer;
};

#endif // CTRLSELECTINGCARDS_H
