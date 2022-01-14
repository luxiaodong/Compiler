#ifndef GGRAMMARFORMULA_H
#define GGRAMMARFORMULA_H

#include <QtDebug>
#include <QString>
#include <QStringList>

// 终结符号 用小些字母表示
// 非终结符号 用大些字母表示
// 产生式 非终结符号 --> {终结符,非终结符}序列
// 空字符串用@表示, 结束符号用#表示

class GGrammarFormula
{
public:
    GGrammarFormula(const QString& head, const QStringList& stringList);
    static bool isTerminal(QString str);
    static QString emptySymbol(){return "@";}

    const QString& head(){return m_head;}
    const QString& first(){return m_stringList.first();}
    const QString& last(){return m_stringList.last();}
    const QString& index(int i){return m_stringList.at(i);}

    int size(){return m_stringList.size();}
    bool isEmpty(){return m_isEmpty;}
    void print();

private:
    const QString m_head;
    const QStringList m_stringList;
    bool m_isEmpty;
};

#endif // GGRAMMARFORMULA_H
