#ifndef GPARSER_H
#define GPARSER_H

#include <QMap>
#include "src/glexer.h"
#include "src/gsyntaxnode.h"
#include "src/gsyntaxtree.h"
#include "src/gtype.h"

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
    GSyntaxNode* parseExpressionUnary();
    GSyntaxNode* parseConstant();           //ParsePrimaryExpr
    GSyntaxNode* parseExpressionBracket();  //ParsePostFixExpr

private:
    GType* parseDeclarationSpec();
    GType* parseDeclarator(GType* baseType, GToken* &pToken);
    GType* parseTypeSuffix(GType* pType);

private:
    bool isValidType(TokenType type);
    void getNextToken();
    GToken* lookNextToken();
    GVariable* createVariable(const QString& name, GType* pType);

public:
    GSyntaxTree* m_pTree;

private:
    QList<GVariable*> *m_pLocals; //指向函数的local变量
    QMap<QString, GVariable*> m_localMaps; //局部变量

private:
    const GLexer m_lexer;
    GToken* m_pCurrentToken;
    int m_tokenIndex;
};

#endif // GPARSER_H
