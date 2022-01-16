#ifndef GFINITEAUTOMATA_H
#define GFINITEAUTOMATA_H

// sigma, 输入符号的有限集合
// Q, 状态的有限集合
// q0, 初始状态
// F是终止状态集合, F<=Q
// f是Q与sigma的直和
// f(q,a)是多个状态,还是单个状态

#include <QString>
#include <QStringList>

class GFiniteAutomata
{
public:
    GFiniteAutomata();

public:
    void convertNFAToDFA();

private:
    QStringList m_symbolSet;
    QStringList m_stateSet;
    QString m_initState;
    QStringList m_terminalSet;
};

#endif // GFINITEAUTOMATA_H
