#ifndef GSYNTAXNODE_H
#define GSYNTAXNODE_H

#include "src/gtoken.h"
#include "src/gvariable.h"
#include <QList>

enum BinaryOperator{
    OP_Add,
    OP_Sub,
    OP_Mul,
    OP_Div,
    OP_Equal,
    OP_NotEqual,
    OP_Greater,
    OP_GreaterEqual,
    OP_Lesser,
    OP_LesserEqual,
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
    QList<GSyntaxNode*> m_functionList;
};

class GFunctionNode : public GSyntaxNode
{
public:
    virtual ~GFunctionNode(){}
    virtual void generateCode(GGenerateCode *genCode);

public:
    QString m_funcName;
    QList<GVariable*> m_args;
    QList<GVariable*> m_locals; //局部变量
//    GSyntaxNode* m_braceNode;
    QList<GSyntaxNode*> m_sentenceList;
};

class GFunctionCallNode : public GSyntaxNode
{
public:
    virtual ~GFunctionCallNode(){}
    virtual void generateCode(GGenerateCode *genCode);
public:
    QString m_funcName;
    QList<GSyntaxNode*> m_argsList;
};

class GReturnNode : public GSyntaxNode
{
public:
    virtual ~GReturnNode(){}
    virtual void generateCode(GGenerateCode *genCode);
public:
    GSyntaxNode* m_pNode;
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

class GBraceNode : public GSyntaxNode
{
public:
    virtual ~GBraceNode(){}
    virtual void generateCode(GGenerateCode *genCode);
public:
    QList<GSyntaxNode*> m_sentenceList;
};

class GWhileNode : public GSyntaxNode
{
public:
    virtual ~GWhileNode(){}
    virtual void generateCode(GGenerateCode *genCode);
public:
    GSyntaxNode* m_checkNode;
    GSyntaxNode* m_braceNode;
};

class GDoWhileNode : public GSyntaxNode
{
public:
    virtual ~GDoWhileNode(){}
    virtual void generateCode(GGenerateCode *genCode) override;
public:
    GSyntaxNode* m_checkNode;
    GSyntaxNode* m_braceNode;
};

class GForLoopNode : public GSyntaxNode
{
public:
    virtual ~GForLoopNode(){}
    virtual void generateCode(GGenerateCode *genCode) override;
public:
    GSyntaxNode* m_initNode;
    GSyntaxNode* m_checkNode;
    GSyntaxNode* m_incNode;
    GSyntaxNode* m_braceNode;
};

class GConditionNode : public GSyntaxNode
{
public:
    virtual ~GConditionNode(){}
    virtual void generateCode(GGenerateCode *genCode);
public:
    GSyntaxNode* m_checkNode;
    GSyntaxNode* m_yesNode;
    GSyntaxNode* m_noNode; //可选.
};

class GAssignNode : public GSyntaxNode
{
public:
    virtual ~GAssignNode(){}
    virtual void generateCode(GGenerateCode *genCode);
public:
    GSyntaxNode* m_pLeftNode;
    GSyntaxNode* m_pRightNode;
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

class GConstantNode : public GSyntaxNode
{
public:
    virtual ~GConstantNode(){}
    virtual void generateCode(GGenerateCode*  genCode);

public:
    int m_value;
};

class GVariableNode : public GSyntaxNode
{
public:
    virtual ~GVariableNode(){}
    virtual void generateCode(GGenerateCode *genCode);
public:
    QString m_name;
};

#endif // GSYNTAXNODE_H
