#ifndef GSYNTAXNODE_H
#define GSYNTAXNODE_H

#include "src/gtoken.h"
#include <QList>

enum BinaryOperator{
    OP_Add,
    OP_Sub,
    OP_Mul,
    OP_Div,
};

class GGenerateCode;

class GSyntaxNode
{
public:
    GSyntaxNode(): m_pToken(NULL) {}
    virtual ~GSyntaxNode(){}
    virtual void generateCode(GGenerateCode*  genCode);

public:
    GToken* m_pToken;
};

class GProgramNode : public GSyntaxNode
{
public:
    virtual ~GProgramNode(){}
    virtual void generateCode(GGenerateCode*  genCode);

public:
    QList<GSyntaxNode*> m_sentenceList;
};

class GSentenceNode : public GSyntaxNode
{
public:
    virtual ~GSentenceNode(){}
    virtual void generateCode(GGenerateCode *genCode);

public:
    GSyntaxNode* m_pNode;
};

class GExpressionNode :  public GSyntaxNode
{
public:
    virtual ~GExpressionNode(){}
    virtual void generateCode(GGenerateCode *genCode);

public:
    GSyntaxNode* m_pNode;
};

//二元操作符
class GBinaryNode : public GSyntaxNode
{
public:
    virtual ~GBinaryNode(){}
    virtual void generateCode(GGenerateCode*  genCode);

public:
    BinaryOperator m_binOp;
    GSyntaxNode* m_pLeftNode;
    GSyntaxNode* m_pRightNode;
};

class GNumberNode : public GSyntaxNode
{
public:
    virtual ~GNumberNode(){}
    virtual void generateCode(GGenerateCode*  genCode);

public:
    int m_value;
};

#endif // GSYNTAXNODE_H
