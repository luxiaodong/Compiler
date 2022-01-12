#ifndef GSYNTAXNODE_H
#define GSYNTAXNODE_H

#include "src/gtoken.h"

enum BinaryOperator{
    OP_Add,
    OP_Sub,
    OP_Mul,
    OP_Div,
};

class GSyntaxNode
{
public:
    GSyntaxNode(): m_pLeftNode(NULL), m_pRightNode(NULL), m_pToken(NULL) {}
    virtual ~GSyntaxNode(){}
    void traversal();

public:
    GSyntaxNode* m_pLeftNode;
    GSyntaxNode* m_pRightNode;
    GToken* m_pToken;
};

class GProgramNode : public GSyntaxNode
{
public:
    //virtual ~GProgramNode();
//    virtual void print();
};

//二元操作符
class GBinaryNode : public GSyntaxNode
{
public:
    //virtual ~GBinaryNode();
//    virtual void print();

public:
    BinaryOperator m_binOp;
};

class GNumberNode : public GSyntaxNode
{
public:
//    virtual ~GNumberNode();
//    virtual void print();
};

#endif // GSYNTAXNODE_H
