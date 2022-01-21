#ifndef GCONTEXTFREEGRAMMAR_H
#define GCONTEXTFREEGRAMMAR_H

#include <QMap>
#include "theory/gcontextsensitivegrammar.h"

// LL(1)
// SLR    SDD SDT

class GContextFreeGrammar : public GContextSensitiveGrammar
{
public:
    GContextFreeGrammar();
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
};

#endif // GCONTEXTFREEGRAMMAR_H
