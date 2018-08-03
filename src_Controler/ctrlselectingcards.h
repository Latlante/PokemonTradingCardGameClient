#ifndef CTRLSELECTINGCARDS_H
#define CTRLSELECTINGCARDS_H

#include <QObject>

#include "src_Models/modelselectingcards.h"
#include "src_Models/proxymodelselectingcards.h"

class ModelListFiltersSelectingCards;
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

    QString name();
    void setName(const QString& name);

    Q_INVOKABLE void newSelection(const QString& name);
    Q_INVOKABLE void setFilterProxy(int filter);

    QList<InfoCard> listCards();
    Q_INVOKABLE ModelSelectingCards* model();
    Q_INVOKABLE ProxyModelSelectingCards* proxy();
    Q_INVOKABLE ModelListFiltersSelectingCards *modelFilters();


    //Q_INVOKABLE void addANewCard(int id);
    //Q_INVOKABLE void removeACard(int id);
    //Q_INVOKABLE void changeQuantityCard(int id, int quantity);
    Q_INVOKABLE void fillARandomList();

    Q_INVOKABLE void savePacket(const QString& name);
    Q_INVOKABLE void loadPacket(const QString& name);

private:
    ModelSelectingCards* m_modelSelectingCards;
    ProxyModelSelectingCards* m_proxySelectingCards;
    ModelListFiltersSelectingCards *m_modelListFilters;
};

#endif // CTRLSELECTINGCARDS_H
