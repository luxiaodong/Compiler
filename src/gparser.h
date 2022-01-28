#ifndef GPARSER_H
#define GPARSER_H

#include <QMap>
#include "src/glexer.h"
#include "src/gsyntaxnode.h"
#include "src/gsyntaxtree.h"
#include "src/gtype.h"

//https://cs.wmich.edu/~gupta/teaching/cs4850/sumII06/The%20syntax%20of%20C%20in%20Backus-Naur%20form.htm

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
    GSyntaxNode* parseStructAccessNode(GSyntaxNode* left);   //MakeMemberAccessNode

private:
    GType* parseDeclarationSpec();
    GType* parseDeclarator(GType* baseType, GToken* &pToken);
    GType* parseTypeSuffix(GType* pType);
    GType* ParseStructDeclaration();
    GType* ParseUnionDeclaration();
    GStructType* ParseStructOrUnionDeclaration(bool isStruct);

private:
    bool isValidType(TokenType type);
    void getNextToken();
    GToken* lookNextToken();
    GVariable* createVariable(const QString& name, GType* pType);
    int alignTo(int size, int align);

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
