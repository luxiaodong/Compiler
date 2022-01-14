#include "ggrammarparser.h"
#include <QtDebug>

GGrammarParser::GGrammarParser()
{
}

void GGrammarParser::test()
{
    // 没有定义开始符号
    QStringList list;
    list<<QString("E->T,E'");
    list<<QString("E'->+,T,E'|@");
    list<<QString("T->F,T'");
    list<<QString("T'->*,F,T'|@");
    list<<QString("F->(,E,)|id");
    this->create(list);
    this->calculateFirstSet();
    this->calculateFollowSet();
    this->calculateSelectSet();
    this->print();
}

void GGrammarParser::print()
{
    qDebug()<<"================================= first";

    foreach(QString head, m_heads)
    {
        QStringList list = m_firstSet.value(head);
        qDebug()<<head<<" -> "<<list;
    }

    qDebug()<<"================================= follow";

    foreach(QString head, m_heads)
    {
        QStringList list = m_followSet.value(head);
        qDebug()<<head<<" -> "<<list;
    }

    qDebug()<<"================================= select";

    foreach(GGrammarFormula* formula, m_formulas)
    {
        formula->print();
    }
}

void GGrammarParser::create(const QStringList& list)
{
    m_formulas.clear();
    m_heads.clear();
    for(int i=0; i< list.size(); ++i)
    {
        QString single = list.at(i);
        QStringList list1 = single.split("->");
        QString head = list1.first();
        m_heads.append(head);
        if(i==0) m_startSymbol = head;

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

// 非终结符的follow集合计算
void GGrammarParser::calculateFollowSet()
{
    QStringList list = m_followSet.value(m_startSymbol);
    list.append("#");
    m_followSet.insert(m_startSymbol, list);

    while(true){
        bool isChanged = false;
        foreach(QString head, m_heads)
        {
            bool isSymbolAppend = this->calculateFollowSet(head);
            if(isSymbolAppend) isChanged = true;
        }

        if(isChanged == false) return ;
    }
}

bool GGrammarParser::calculateFollowSet(const QString& head)
{
    bool isSymbolAppend = false;
    QStringList follow = m_followSet.value(head);
    foreach(GGrammarFormula* formula, m_formulas)
    {
        for(int i = 0; i < formula->size(); ++i)
        {
            if(formula->index(i) == head) //找到了
            {
                int j = i+1;
                while(j < formula->size())
                {
                    QString symbol = formula->index(j);
                    if(GGrammarFormula::isTerminal(symbol))
                    {
                        if(follow.indexOf(symbol) == -1)
                        {
                            follow.append(symbol);
                            isSymbolAppend = true;
                        }
                        break;
                    }
                    else // 非终结符号
                    {
                        bool isContainEmpty = false;
                        QStringList list = m_firstSet.value(symbol);
                        foreach(QString single, list)
                        {
                            if(single == GGrammarFormula::emptySymbol())
                            {
                                isContainEmpty = true;
                            }
                            else
                            {
                                if(follow.indexOf(single) == -1)
                                {
                                    follow.append(single);
                                    isSymbolAppend = true;
                                }
                            }
                        }

                        if(isContainEmpty == false) break;
                        j++;
                    }
                }

                //follow A中的所有符号都在 follow B中
                if(j == formula->size())
                {
                    QStringList list = m_followSet.value(formula->head());
                    foreach(QString single, list)
                    {
                        if(follow.indexOf(single) == -1)
                        {
                            follow.append(single);
                            isSymbolAppend = true;
                        }
                    }
                }
            }
        }
    }

    if(isSymbolAppend)
    {
        m_followSet.insert(head, follow);
    }

    return isSymbolAppend;
}

void GGrammarParser::calculateSelectSet()
{
    foreach(GGrammarFormula* formula, m_formulas)
    {
        if(formula->isEmpty())
        {
            formula->m_selectList = m_followSet.value( formula->head() );
        }
        else
        {
            QString symbol = formula->first();
            if(GGrammarFormula::isTerminal(symbol))
            {
                formula->m_selectList.append(symbol);
            }
            else
            {
                formula->m_selectList = m_firstSet.value( symbol );
            }
        }
    }
}

bool GGrammarParser::isContainEmpty(const QString& head) const
{
    QStringList follow = m_firstSet.value(head);
    if( follow.indexOf( GGrammarFormula::emptySymbol() ) == -1)
    {
        return false;
    }
    return true;
}
