#include "ggrammarll1.h"

GGrammarLL1::GGrammarLL1()
{
}

void GGrammarLL1::test()
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

void GGrammarLL1::print()
{
    int maxCharCount = 0;
    qDebug()<<"================================= production";
    foreach(GProductionII* formula, m_formulas)
    {
        QString str = formula->production();
        if(str.length() > maxCharCount) maxCharCount = str.length();
        qDebug()<<str;
    }

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

    qDebug()<<"================================= prediction analysis table";
    //预测分析表, 行是输入的终结符号, 列是非终结符号, 中间值是使用的产生式
    this->collectTerminalSymbol();

    maxCharCount += 2;
    QString str = "|" + QString(maxCharCount,' ') + "|";
    foreach(QString single, m_terminals)
    {
        int charCount = (maxCharCount - single.length())/2;
        str += QString(charCount,' ');
        str += single;
        str += QString(maxCharCount - charCount - single.length(),' ');
        str += QString("|");
    }

    QString line = QString( (maxCharCount+1)*(m_terminals.length()+1) + 1,'-');
    qDebug()<<line;
    qDebug()<<str;
    qDebug()<<line;

    foreach(QString head, m_heads)
    {
        QString str = "|";
        int charCount = (maxCharCount - head.length())/2;
        str += QString(charCount,' ');
        str += head;
        str += QString(maxCharCount - charCount - head.length(),' ');
        str += QString("|");

        QMap<QString, QString> map;
        foreach(GProductionII* formula, m_formulas)
        {
            if(formula->head() == head) //找到了一个产生式
            {
                foreach(QString select, formula->m_selectList) //遍历产生式的select集
                {
                    map.insert( select,  formula->production() );
                }
            }
        }

        foreach(QString single, m_terminals)
        {
            QString value = map.value(single);
            if(value.isEmpty())
            {
                str += QString(maxCharCount,' ');
                str += QString("|");
            }
            else
            {
                int charCount = (maxCharCount - value.length())/2;
                str += QString(charCount,' ');
                str += value;
                str += QString(maxCharCount - charCount - value.length(),' ');
                str += QString("|");
            }
        }

        qDebug()<<str;
        qDebug()<<line;
    }
}

void GGrammarLL1::create(const QStringList& list)
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
void GGrammarLL1::calculateFirstSet()
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

bool GGrammarLL1::calculateFirstSet(const QString& head)
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
void GGrammarLL1::calculateFollowSet()
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

bool GGrammarLL1::calculateFollowSet(const QString& head)
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

void GGrammarLL1::calculateSelectSet()
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

void GGrammarLL1::collectTerminalSymbol()
{
    m_terminals.clear();
    foreach(GProductionII* formula, m_formulas)
    {
        foreach(QString single, formula->m_selectList)
        {
            if(m_terminals.indexOf(single) == -1)
            {
                m_terminals.append(single);
            }
        }
    }
}


bool GGrammarLL1::isContainEmpty(const QString& head) const
{
    QStringList follow = m_firstSet.value(head);
    if( follow.indexOf( GProduction::emptySymbol() ) == -1)
    {
        return false;
    }
    return true;
}
