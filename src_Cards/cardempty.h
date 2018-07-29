#ifndef CARDEMPTY_H
#define CARDEMPTY_H

#include "abstractcard.h"

class CardEmpty : public AbstractCard
{
    Q_OBJECT
public:
    explicit CardEmpty(QObject *parent = nullptr);
    CardEmpty(const CardEmpty& card);
    ~CardEmpty();

    static void declareQML();

    Q_INVOKABLE AbstractCard::Enum_typeOfCard type() override;
    Q_INVOKABLE QUrl image() override;
    AbstractCard* clone() override;

    CardEmpty& operator =(const CardEmpty& source);

signals:

public slots:
};

#endif // CARDEMPTY_H
