#include "gproduction.h"

GProduction::GProduction()
{

}

GProductionII::GProductionII(const QString& head, const QStringList& stringList):m_head(head), m_rightList(stringList)
{
    m_isEmpty = false;
    if(m_rightList.size() == 1)
    {
        if(m_rightList.first() == GProduction::emptySymbol())
        {
            m_isEmpty = true;
        }
    }
}

//首字母是小写的 就是 终结符.
bool GProductionII::isTerminal(QString str)
{
    QChar c = str.at(0);
    if(c.isUpper()) return false;
    return true;
}


QString GProductionII::production() const
{
    QString str = m_head+QString("->");
    foreach(QString single, m_rightList)
    {
        str += single;
    }
    return str;
}

void GProductionII::print()
{
    qDebug()<<m_head<<"->"<<m_rightList<<"  =>  "<<m_selectList;
}


GProductionIII::GProductionIII(const QString& head, const QString& first, const QString& last, bool isLeftLinear)
    : m_head(head), m_first(first), m_last(last), m_isLeftLinear(isLeftLinear)
{}

const QString& GProductionIII::terminal()
{
    if(m_isLeftLinear) return m_last;
    return m_first;

    //左线性 A->Bw, 右线性 A->wB
}

const QString& GProductionIII::nonterminal()
{
    if(m_isLeftLinear) return m_first;
    return m_last;
}
