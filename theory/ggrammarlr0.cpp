#include "ggrammarlr0.h"

GGrammarLR0::GGrammarLR0()
{
}

void GGrammarLR0::test()
{
    QStringList list;
    list<<QString("S'->S");
    list<<QString("S->B,B");
    list<<QString("B->a,B");
    list<<QString("B->b");
    this->create(list);
    this->print();
}

void GGrammarLR0::create(const QStringList& list)
{
    m_formulas.clear();
    m_heads.clear();
    m_formulaItems.clear();
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

            QList<GProductionII*> formulaList;
            for(int i=0; i < list3.size()+1; ++i)
            {
                QStringList list4 = list3;
                list4.insert(i, GProduction::partitionSymbol());
                GProductionII* subFormula = new GProductionII(head, list4);
                formulaList.append(subFormula);
            }

            m_formulaItems.insert(formula, formulaList);

            for(int i=0; i < list3.size(); ++i)
            {
                QString str = list3.at(i);
                if(str.at(0).isLower())
                {
                    if(m_terminals.indexOf(str) == -1)
                    {
                        m_terminals.append(str);
                    }
                }
            }
        }
    }
}

void GGrammarLR0::print()
{
    qDebug()<<"================================= production";
    foreach(GProductionII* formula, m_formulas)
    {
        QString str = formula->production();
        qDebug()<<str;
    }

    qDebug()<<"================================= sub production";
    foreach(GProductionII* formula1, m_formulas)
    {
        QList<GProductionII*> list = m_formulaItems.value(formula1);
        foreach(GProductionII* formula2, list)
        {
            QString str = formula2->production();
            qDebug()<<str;
        }
    }

    qDebug()<<"================================= state machine";
    this->createStateMachine();
    int i = 0;
    foreach(QList<GProductionII*> list, m_states)
    {
        qDebug()<<"-------state is "<<i++<<"--------";
        foreach(GProductionII* formula, list)
        {
            QString str = formula->production();
            qDebug()<<str;
        }
    }
}

void GGrammarLR0::createStateMachine()
{
    QStringList allSymbols;
    foreach(QString str, m_heads)
    {
        if(str != m_startSymbol && allSymbols.indexOf(str) == -1)
        {
            allSymbols.append(str);
        }
    }

    foreach(QString str, m_terminals)
    {
        if(str != m_startSymbol && allSymbols.indexOf(str) == -1)
        {
            allSymbols.append(str);
        }
    }

    qDebug()<<"================================= allSymbols";
    foreach(QString str, allSymbols)
    {
        qDebug()<<str;
    }

    m_states.clear();
    GProductionII* formula = this->findProduction(m_startSymbol);
    GProductionII* item = m_formulaItems.value(formula).at(0);
    QList<GProductionII*> state = this->closure(item);
    m_states.append(state);

    int i = 0;
    while(i < m_states.size())
    {
        state = m_states.at(i);
        foreach(QString symbol, allSymbols)
        {
            QList<GProductionII*> nextState = this->goTo(state, symbol);
            if(nextState.isEmpty() == false)
            {
                if(this->exist(nextState) == false)
                {
                    m_states.append(nextState);
                }
            }
        }
        i++;
    }

//    qDebug()<<"====================================================";
//    QList<GProductionII*> nextState = this->goTo(m_states.at(2), "B");
//    m_states.append(nextState);
}

QList<GProductionII*> GGrammarLR0::goTo(const QList<GProductionII*>&state, QString symbol)
{
    QList<GProductionII*> outList;
    foreach(GProductionII* item, state)
    {
//qDebug()<<"item is :"<<item->production();
        int partitionIndex = item->indexOf(GProduction::partitionSymbol());
        int index = item->indexOf(symbol, partitionIndex);
        if( index > 0)
        {
//qDebug()<<"index is :"<<index;
            if( item->index(index-1) == GProduction::partitionSymbol() )
            {
                GProductionII* nextItem = this->findNextItem(item);
//qDebug()<<"next item is :"<<nextItem->production();
                if(nextItem)
                {
                    if(outList.indexOf(nextItem) == -1)
                    {
                        outList.append(nextItem);
//qDebug()<<"---->"<<nextItem->production()<<","<<index<<","<<nextItem->index(index+1);

                        if(nextItem->last() != GProduction::partitionSymbol())
                        {
                            if (GProductionII::isTerminal(nextItem->index(index+1)) == false)
                            {
                                QString head = nextItem->index(index+1);
                                foreach(GProductionII* item2, state)
                                {
                                    if(item2->head() == head)
                                    {
                                        if(outList.indexOf(item2) == -1)
                                        {
                                            outList.append(item2);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return outList;
}

QList<GProductionII*> GGrammarLR0::closure(GProductionII* item)
{
    m_formulaItemList.clear();
    QList<QList<GProductionII*>> values = m_formulaItems.values();
    for(int i =0; i < values.size(); ++i)
    {
        m_formulaItemList.append(values.at(i));
    }

    QList<GProductionII*> outList;
    if(this->exist(item))
    {
        outList.append(item);
    }

    QString symbol = GProduction::partitionSymbol();

    int size = 0;
    while(size != outList.size())
    {
        size = outList.size();
//若A→α·Bβ属于CLOSURE(I)，则每一形如B→·γ的项目也属于CLOSURE(I)。
        for(int i = 0; i < outList.size(); ++i)
        {
//qDebug()<<"i ="<<i;
            GProductionII* item = outList.at(i);
            int index = item->indexOf(symbol);
            if(index >= 0 && index < item->size() - 1) //找到了并且不在最后
            {
                //在剩余的集合里找
                QString head = item->index(index+1); //就是B
//qDebug()<<"head = "<<head;
                foreach(GProductionII* formula, m_formulaItemList)
                {
                    if(formula->head() == head && formula->first() == symbol) //这项要检查
                    {
                        if( this->exist(formula) && outList.indexOf(formula) == -1)
                        {
//qDebug()<<"production = "<<formula->production();
                            outList.append(formula);
                        }
                    }
                }
            }
        }
    }
    return outList;
}

bool GGrammarLR0::exist(GProductionII* item)
{
    int index = m_formulaItemList.indexOf(item);
    Q_ASSERT(index >= 0);
//    m_formulaItemList.removeAt(index);
    return true;
}

bool GGrammarLR0::exist(QList<GProductionII*>& checkState)
{
    foreach(QList<GProductionII*> state, m_states)
    {
        if(state.size() == checkState.size())
        {
            //每个都能找到.
            bool isSame = true;
            foreach(GProductionII* p, state)
            {
                if(checkState.indexOf(p) == -1)
                {
                    isSame = false;
                    break;
                }
            }

            if(isSame) return true;
        }
    }
    return false;
}

GProductionII* GGrammarLR0::findProduction(const QString& head)
{
    foreach(GProductionII* formula, m_formulas)
    {
        if(formula->head() == head)
        {
            return formula;
        }
    }

    return NULL;
}

GProductionII* GGrammarLR0::findNextItem(GProductionII* currentItem)
{
    QList<QList<GProductionII*>> values = m_formulaItems.values();
    for(int i=0; i < values.size(); ++i)
    {
        QList<GProductionII*> list = values.at(i);
        for(int j=0; j < list.size(); ++j)
        {
            GProductionII* item = list.at(j);
            if(item == currentItem)
            {
                if(j + 1 < list.size())
                {
                    return list.at(j+1);
                }
            }
        }
    }

    return NULL;
}
