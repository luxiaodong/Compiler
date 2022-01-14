#ifndef GGRAMMARPARSER_H
#define GGRAMMARPARSER_H

#include <QStringList>
#include <QList>
#include <QMap>
#include "theory/ggrammarformula.h"

class GGrammarParser
{
public:
    GGrammarParser();
    void test();

private:
    void create(const QStringList& stringList);
    void print();
    void calculateFirstSet();
    bool calculateFirstSet(const QString& head); //返回m_firstSet是否改动过
    bool isContainEmpty(const QString& head) const;

    void calculateFollowSet();
    void calculateSelectSet();

private:
    QList<GGrammarFormula*> m_formulas;
    QStringList m_heads;
    QMap<QString, QStringList> m_firstSet;
    QMap<QString, QStringList> m_followSet;
};

#endif // GGRAMMARPARSER_H
