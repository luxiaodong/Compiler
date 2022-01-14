#include "ggrammarparser.h"
#include <QtDebug>

GGrammarParser::GGrammarParser()
{
}

void GGrammarParser::test()
{
    QStringList list;
    list<<QString("E->T,E'");
    list<<QString("E'->+,T,E'|@");
    list<<QString("T->F,T'");
    list<<QString("T'->*,F,T'|@");
    list<<QString("F->(,E,)|id");
    this->create(list);
    this->calculateFirstSet();
    this->print();
}

void GGrammarParser::print()
{
    foreach(GGrammarFormula* formula, m_formulas)
    {
        formula->print();
    }

    qDebug()<<"=================================";

    foreach(QString head, m_heads)
    {
        QStringList follow = m_firstSet.value(head);
        qDebug()<<head<<" -> "<<follow;
    }
}

void GGrammarParser::create(const QStringList& list)
{
    m_formulas.clear();
    m_heads.clear();
    foreach(QString single, list)
    {
        QStringList list1 = single.split("->");
        QString head = list1.first();
        m_heads.append(head);

        QStringList list2 = list1.last().split("|");
        foreach(QString str, list2)
        {
            QStringList list3 = str.split(",");
            GGrammarFormula* formula = new GGrammarFormula(head, list3);
            m_formulas.append(formula);
        }
    }

    foreach(QString head, m_heads)
    {
        m_firstSet.insert(head, QStringList());
        m_followSet.insert(head, QStringList());
    }
}

// 非终结符的first集合计算
void GGrammarParser::calculateFirstSet()
{
    m_firstSet.clear();
    while(true)
    {
        bool isChanged = false;
        foreach(QString head, m_heads)
        {
            bool isSymbolAppend = this->calculateFirstSet(head);
            if(isSymbolAppend) isChanged = true;
        }

        if(isChanged == false) return ;
    }
}

bool GGrammarParser::calculateFirstSet(const QString& head)
{
    bool isSymbolAppend = false;
    QStringList follow = m_firstSet.value(head);
    foreach(GGrammarFormula* formula, m_formulas)
    {
        if(formula->head() == head) // 只处理相同的左部
        {
            for(int i = 0; i < formula->size(); ++i)
            {
                QString symbol = formula->index(i);
                if( GGrammarFormula::isTerminal(symbol) ) // 是终结符号
                {
                    if(follow.indexOf(symbol) == -1) // first集里找不到该符号
                    {
                        follow.append(symbol);
                        isSymbolAppend = true;
                    }
                    break;
                }
                else
                {
                    // 把这个非终结符对应的first集加进来.
                    QStringList list = m_firstSet.value(symbol);
                    foreach(QString single, list)
                    {
                        if(follow.indexOf(single) == -1)
                        {
                            follow.append(single);
                            isSymbolAppend = true;
                        }
                    }

                    if(this->isContainEmpty(symbol) == false)
                    {
                        break;
                    }
                }
            }
        }
    }

    if(isSymbolAppend)
    {
        m_firstSet.insert(head, follow);
    }

    return isSymbolAppend;
}

bool GGrammarParser::isContainEmpty(const QString& head) const
{
    QStringList follow = m_followSet.value(head);
    if( follow.indexOf( GGrammarFormula::emptySymbol() ) == -1)
    {
        return false;
    }
    return true;
}

void GGrammarParser::calculateFollowSet()
{}

void GGrammarParser::calculateSelectSet()
{}
