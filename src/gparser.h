#ifndef GPARSER_H
#define GPARSER_H

#include <QMap>
#include "src/glexer.h"
#include "src/gsyntaxnode.h"
#include "src/gsyntaxtree.h"
#include "src/gsymboltable.h"

class GParser
{
public:
    GParser(const GLexer &lexer);
    ~GParser();
    bool analysis();

private:
    GSyntaxNode* parseProgram();
    GSyntaxNode* parseFunction();
    GSyntaxNode* parseFunctionCall();
    GSyntaxNode* parseSentence();
    GSyntaxNode* parseExpression();
    GSyntaxNode* parseExpressionEqual();
    GSyntaxNode* parseExpressionRelation();
    GSyntaxNode* parseExpressionAssign();
    GSyntaxNode* parseExpressionAdd();
    GSyntaxNode* parseExpressionMul();
    GSyntaxNode* parseConstant();

private:
    void getNextToken();
    GToken* lookNextToken();

public:
    GSyntaxTree* m_pTree;

private:
    const GLexer m_lexer;
    GToken* m_pCurrentToken;
    int m_tokenIndex;
};

#endif // GPARSER_H
