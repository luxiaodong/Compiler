#ifndef GGRAMMARLR0_H
#define GGRAMMARLR0_H

#include <QMap>
#include <QString>
#include <QStringList>
#include <QDebug>
#include "theory/gproduction.h"

class GGrammarLR0
{
public:
    GGrammarLR0();
    void test();
    void print();

private:
    void create(const QStringList& stringList);
    QList<GProductionII*> closure(GProductionII* item); //返回闭包
    QList<GProductionII*> goTo(const QList<GProductionII*>&, QString symbol); //生成下一个状态的闭包
    void createStateMachine();

    bool exist(GProductionII* item);
    bool exist(QList<GProductionII*>& list);

    GProductionII* findProduction(const QString& name); //找产生式
    GProductionII* findNextItem(GProductionII* currentItem); //找产生式的下一个结点

private:
    QList<GProductionII*> m_formulas;
    QMap<GProductionII*, QList<GProductionII*>> m_formulaItems;
    QList<GProductionII*> m_formulaItemList; //用于递归减少筛选
    QStringList m_heads; //非终结符号
    QString m_startSymbol;
    QStringList m_terminals; //终结符号
    QList<QList<GProductionII*> > m_states; //生成的状态结点
    // QMap<int, GProductionII*> m_states;
};

#endif // GGRAMMARLR0_H
