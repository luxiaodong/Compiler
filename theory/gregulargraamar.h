#ifndef GREGULARGRAAMAR_H
#define GREGULARGRAAMAR_H

#include <QString>
#include <QStringList>
#include "theory/gcontextfreegrammar.h"

// 正则表示
// letter_ -> a-z|A-Z|_
// dight   -> 0-9
// id      -> letter_(dight | letter_)*

//文法表示
// letter_ -> a-z|A-Z|_
// dight   -> 0-9
// id      -> letter_
//         -> id|letter_
//         -> id|dight

class GRegularGraamar : public GContextFreeGrammar
{
public:
    GRegularGraamar();
    void test();

public:
    bool isIdentifier(const QString& str) const;
    bool isKeyword(const QString& str) const;
};

#endif // GREGULARGRAAMAR_H
