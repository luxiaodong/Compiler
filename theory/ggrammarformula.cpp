#include "ggrammarformula.h"
#include <QChar>

GGrammarFormula::GGrammarFormula(const QString& head, const QStringList& stringList):m_head(head), m_stringList(stringList)
{
    m_isEmpty = false;
    if(m_stringList.size() == 1)
    {
        if(m_stringList.first() == emptySymbol())
        {
            m_isEmpty = true;
        }
    }
}

//首字母是小写的 就是 终结符.
bool GGrammarFormula::isTerminal(QString str)
{
    QChar c = str.at(0);
    if(c.isUpper()) return false;
    return true;
}

void GGrammarFormula::print()
{
    qDebug()<<m_head<<" -> "<<m_stringList;
}
