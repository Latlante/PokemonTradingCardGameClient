#include "constantesqml.h"
#include <QtQml/qqml.h>

ConstantesQML::ConstantesQML(QObject *parent) : QObject(parent)
{

}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void ConstantesQML::declareQML()
{
    qmlRegisterUncreatableType<ConstantesQML>("model", 1, 0, "ConstantesQML", "ConstantesQML error");
}
