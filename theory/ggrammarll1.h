#ifndef GCONTEXTFREEGRAMMAR_H
#define GCONTEXTFREEGRAMMAR_H

#include <QMap>
#include <QString>
#include <QStringList>
#include <QDebug>
#include "theory/gproduction.h"

// LL(1) 文法.
//

class GGrammarLL1
{
public:
    GGrammarLL1();
    void test();

private:
    void create(const QStringList& stringList);
    void print();
    void calculateFirstSet();
    bool calculateFirstSet(const QString& head); //返回m_firstSet是否改动过
    bool isContainEmpty(const QString& head) const;
    void calculateFollowSet();
    bool calculateFollowSet(const QString& head); //返回m_followSet是否改动过
    void calculateSelectSet();

private:
    QList<GProductionII*> m_formulas;
    QStringList m_heads;
    QMap<QString, QStringList> m_firstSet;
    QMap<QString, QStringList> m_followSet;
    QString m_startSymbol;
};

#endif // GCONTEXTFREEGRAMMAR_H
