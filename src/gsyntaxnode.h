#ifndef GSYNTAXNODE_H
#define GSYNTAXNODE_H

#include "src/gtoken.h"
#include <QStack>

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
    GSyntaxNode(): m_pLeftNode(NULL), m_pRightNode(NULL), m_pToken(NULL) {}
    virtual ~GSyntaxNode(){}
    void traversal();
    virtual void generateCode(GGenerateCode*  genCode);

public:
    GSyntaxNode* m_pLeftNode;
    GSyntaxNode* m_pRightNode;
    GToken* m_pToken;
};

class GProgramNode : public GSyntaxNode
{
public:
    virtual ~GProgramNode(){}
    virtual void generateCode(GGenerateCode*  genCode);
};

//二元操作符
class GBinaryNode : public GSyntaxNode
{
public:
    virtual ~GBinaryNode(){}
    virtual void generateCode(GGenerateCode*  genCode);

public:
    BinaryOperator m_binOp;
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
