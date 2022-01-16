#include "gcontextfreegrammar.h"

GContextFreeGrammar::GContextFreeGrammar()
{

}

void GContextFreeGrammar::test()
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

void GContextFreeGrammar::print()
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

    foreach(GProductionII* formula, m_formulas)
    {
        formula->print();
    }
}

void GContextFreeGrammar::create(const QStringList& list)
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
            GProductionII* formula = new GProductionII(head, list3);
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
void GContextFreeGrammar::calculateFirstSet()
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

bool GContextFreeGrammar::calculateFirstSet(const QString& head)
{
    bool isSymbolAppend = false;
    QStringList tempList = m_firstSet.value(head);
    foreach(GProductionII* formula, m_formulas)
    {
        if(formula->head() == head) // 只处理相同的左部
        {
            for(int i = 0; i < formula->size(); ++i)
            {
                QString symbol = formula->index(i);
                if( GProductionII::isTerminal(symbol) ) // 是终结符号
                {
                    if(tempList.indexOf(symbol) == -1) // first集里找不到该符号
                    {
                        tempList.append(symbol);
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
                        if(tempList.indexOf(single) == -1)
                        {
                            tempList.append(single);
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
        m_firstSet.insert(head, tempList);
    }

    return isSymbolAppend;
}

// 非终结符的follow集合计算
void GContextFreeGrammar::calculateFollowSet()
{
    QStringList tempList = m_followSet.value(m_startSymbol);
    tempList.append("#");
    m_followSet.insert(m_startSymbol, tempList);

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

bool GContextFreeGrammar::calculateFollowSet(const QString& head)
{
    bool isSymbolAppend = false;
    QStringList tempList = m_followSet.value(head);
    foreach(GProductionII* formula, m_formulas)
    {
        for(int i = 0; i < formula->size(); ++i)
        {
            if(formula->index(i) == head) //找到了
            {
                int j = i+1;
                while(j < formula->size())
                {
                    QString symbol = formula->index(j);
                    if(GProductionII::isTerminal(symbol))
                    {
                        if(tempList.indexOf(symbol) == -1)
                        {
                            tempList.append(symbol);
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
                            if(single == GProduction::emptySymbol())
                            {
                                isContainEmpty = true;
                            }
                            else
                            {
                                if(tempList.indexOf(single) == -1)
                                {
                                    tempList.append(single);
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
                        if(tempList.indexOf(single) == -1)
                        {
                            tempList.append(single);
                            isSymbolAppend = true;
                        }
                    }
                }
            }
        }
    }

    if(isSymbolAppend)
    {
        m_followSet.insert(head, tempList);
    }

    return isSymbolAppend;
}

void GContextFreeGrammar::calculateSelectSet()
{
    foreach(GProductionII* formula, m_formulas)
    {
        if(formula->isEmpty())
        {
            formula->m_selectList = m_followSet.value( formula->head() );
        }
        else
        {
            QString symbol = formula->first();
            if(GProductionII::isTerminal(symbol))
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

bool GContextFreeGrammar::isContainEmpty(const QString& head) const
{
    QStringList follow = m_firstSet.value(head);
    if( follow.indexOf( GProduction::emptySymbol() ) == -1)
    {
        return false;
    }
    return true;
}
