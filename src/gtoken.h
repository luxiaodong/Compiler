#ifndef GTOKEN_H
#define GTOKEN_H

#include <QString>
#include <QPoint>
#include <QDebug>

enum TokenType{
    Eof,
    Add,
    Sub,
    Mul,
    Div,
    Num,
    LeftParent,
    RightParent,
};

class GToken
{
public:
    GToken();
    void print();

public:
    TokenType m_type;
    QString m_context; //包含长度
    QPoint m_position; //启始位置
};

QDebug operator <<(QDebug debug, const GToken& token);

#endif // GTOKEN_H
