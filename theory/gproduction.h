#ifndef GPRODUCTION_H
#define GPRODUCTION_H

#include <QtDebug>
#include <QString>
#include <QStringList>

// 终结符号 用小些字母表示
// 非终结符号 用大些字母表示
// 产生式 非终结符号 --> {终结符,非终结符}序列
// 空字符串用@表示, 结束符号用#表示

// 推导，从生成语言的角度
// 规约，从识别语言的角度

enum GrammarType
{
    Unrestricted,       //图灵机
    ContextSensitive,   //有界非确定图灵机 alpha -> beta, |alpha| < |beta|
    ContextFree,        //非确定下推自动机 A -> beta
    Regular,            //有限状态自动机 A -> w*beta 或 A -> beta*w
};

// 0型文法
class GProduction
{
public:
    GProduction();
    static QString emptySymbol(){return "@";}
    static QString terminalSymbol(){return "#";}

private:
    QStringList m_leftList;
    QStringList m_rightList;
};

// 2型文法
class GProductionII
{
public:
    GProductionII(const QString& head, const QStringList& stringList);
    static bool isTerminal(QString str);

    const QString& head(){return m_head;}
    const QString& first(){return m_rightList.first();}
    const QString& last(){return m_rightList.last();}
    const QString& index(int i){return m_rightList.at(i);}

    int size(){return m_rightList.size();}
    bool isEmpty(){return m_isEmpty;}
    void print();

public:
    QStringList m_selectList;

private:
    const QString m_head;
    const QStringList m_rightList;
    bool m_isEmpty;
};

// 3型文法
class GProductionIII
{
public:
    GProductionIII(const QString& head, const QString& first, const QString& last, bool isLeftLinear);
    const QString& head(){return m_head;}
    const QString& first(){return m_first;}
    const QString& last(){return m_last;}
    const QString& terminal();
    const QString& nonterminal();

private:
    const QString m_head;
    const QString m_first;
    const QString m_last;
    bool m_isLeftLinear; //左线性 A->Bw, 右线性 A->wB
};

#endif // GPRODUCTION_H
