#ifndef GUNRESTRICTEDGRAMMAR_H
#define GUNRESTRICTEDGRAMMAR_H

#include <QString>
#include <QStringList>
#include <QDebug>
#include "theory/gproduction.h"

class GUnrestrictedGrammar
{
public:
    GUnrestrictedGrammar();

public:
    QStringList m_nonterminalSymbols;
    QStringList m_terminalSymbols;
    QString m_startSymbol;
};

#endif // GUNRESTRICTEDGRAMMAR_H
