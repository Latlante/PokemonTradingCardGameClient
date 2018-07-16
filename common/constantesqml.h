#ifndef CONSTANTESQML_H
#define CONSTANTESQML_H

#include <QObject>



class ConstantesQML : public QObject
{
    Q_OBJECT
public:
    enum StepGame
    {
        StepPreparation = 0,
        StepGameInProgress,
        StepFinished
    };
    Q_ENUMS(StepGame)

    explicit ConstantesQML(QObject *parent = nullptr);

    static void declareQML();

signals:

public slots:
};

#endif // CONSTANTESQML_H
