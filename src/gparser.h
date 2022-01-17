#ifndef GPARSER_H
#define GPARSER_H

#include <QMap>
#include "src/glexer.h"
#include "src/gsyntaxnode.h"
#include "src/gsyntaxtree.h"
#include "src/gsymboltable.h"

// program : expression
// expression ： add
// add : add + num
//       add - num
//       add + mul
// mul : mul * num
//       mul / num
// num : digit
//       (expression)

// https://www.bilibili.com/read/cv6122366/

class GParser
{
public:
    GParser(const GLexer &lexer);
    ~GParser();
    bool analysis();

private:
    GSyntaxNode* parseProgram();
    GSyntaxNode* parseSentence();
    GSyntaxNode* parseExpression();
    GSyntaxNode* parseExpressionAssign();
    GSyntaxNode* parseExpressionAdd();
    GSyntaxNode* parseExpressionMul();
    GSyntaxNode* parseConstant();

private:
    void getNextToken();

public:
    GSyntaxTree* m_pTree;

private:
    const GLexer m_lexer;
    GToken* m_pCurrentToken;
    int m_tokenIndex;
};

#endif // GPARSER_H
