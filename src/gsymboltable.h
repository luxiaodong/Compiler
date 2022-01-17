#ifndef GSYMBOLTABLE_H
#define GSYMBOLTABLE_H

#include <QString>
#include <QList>
#include <QMap>
#include "src/gvariable.h"

class GSymbolTable
{
public:
    static void addVariable(QString name);
    static int getAddress(QString name);
    static QList<GVariable*> m_variables;
};

#endif // GSYMBOLTABLE_H
