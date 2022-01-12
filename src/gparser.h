#ifndef GPARSER_H
#define GPARSER_H

#include "src/glexer.h"
#include "src/gsyntaxnode.h"
#include "src/gsyntaxtree.h"

// program : expression
// expression ： add
// add : add + num
//       add - num
//       add + mul
// mul : mul * num
//       mul / num
// num : digit

class GParser
{
public:
    GParser(const GLexer &lexer);
    ~GParser();
    bool analysis();

private:
    GSyntaxNode* parseProgram();
    GSyntaxNode* parseExpression();
    GSyntaxNode* parseExpressionAdd();
    GSyntaxNode* parseExpressionMul();
    GSyntaxNode* parseNumber();

private:
    void getNextToken();

private:
    const GLexer m_lexer;
    GToken* m_pCurrentToken;
    GSyntaxTree* m_pTree;
    int m_tokenIndex;
};

#endif // GPARSER_H
