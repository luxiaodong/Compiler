#ifndef GSYNTAXNODE_H
#define GSYNTAXNODE_H

#include <QList>
#include "src/gtoken.h"
#include "src/gvariable.h"
#include "src/gtype.h"

enum UnaryOperator
{
    OP_NONE,
    OP_Plus,
    OP_Minus,
    OP_Star,
    OP_Amp,
};

enum BinaryOperator{
    OP_NULL,
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
    OP_PtrAdd,
    OP_PtrSub,
    OP_PtrDiff,
};

class GSyntaxTreeTraverse;

class GSyntaxNode
{
public:
    GSyntaxNode(): m_pToken(NULL) {}
    virtual ~GSyntaxNode(){}
    virtual void traverse(GSyntaxTreeTraverse* );
    void generateCode(GSyntaxTreeTraverse* pAst){this->traverse(pAst);}
    void calculateType(GSyntaxTreeTraverse* pAst){this->traverse(pAst);}
public:
    GToken* m_pToken;
    GType* m_pType;
};

class GProgramNode : public GSyntaxNode
{
public:
    virtual ~GProgramNode(){}
    virtual void traverse(GSyntaxTreeTraverse* );

public:
    QList<GSyntaxNode*> m_functionList;
};

class GFunctionNode : public GSyntaxNode
{
public:
    virtual ~GFunctionNode(){}
    virtual void traverse(GSyntaxTreeTraverse* );

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
    virtual void traverse(GSyntaxTreeTraverse* );
public:
    QString m_funcName;
    QList<GSyntaxNode*> m_argsList;
};

class GReturnNode : public GSyntaxNode
{
public:
    virtual ~GReturnNode(){}
    virtual void traverse(GSyntaxTreeTraverse* );
public:
    GSyntaxNode* m_pNode;
};

class GSentenceNode : public GSyntaxNode
{
public:
    virtual ~GSentenceNode(){}
    virtual void traverse(GSyntaxTreeTraverse* );
public:
    GSyntaxNode* m_pNode;
};

class GExpressionNode :  public GSyntaxNode
{
public:
    virtual ~GExpressionNode(){}
    virtual void traverse(GSyntaxTreeTraverse* );
public:
    GSyntaxNode* m_pNode;
};

class GExpressionSentenceNode : public GSyntaxNode
{
public:
    virtual ~GExpressionSentenceNode(){}
    virtual void traverse(GSyntaxTreeTraverse* );
public:
    QList<GSyntaxNode*> m_sentenceList;
};

class GBraceNode : public GSyntaxNode
{
public:
    virtual ~GBraceNode(){}
    virtual void traverse(GSyntaxTreeTraverse* );
public:
    QList<GSyntaxNode*> m_sentenceList;
};

class GWhileNode : public GSyntaxNode
{
public:
    virtual ~GWhileNode(){}
    virtual void traverse(GSyntaxTreeTraverse* );
public:
    GSyntaxNode* m_checkNode;
    GSyntaxNode* m_braceNode;
};

class GDoWhileNode : public GSyntaxNode
{
public:
    virtual ~GDoWhileNode(){}
    virtual void traverse(GSyntaxTreeTraverse* );
public:
    GSyntaxNode* m_checkNode;
    GSyntaxNode* m_braceNode;
};

class GForLoopNode : public GSyntaxNode
{
public:
    virtual ~GForLoopNode(){}
    virtual void traverse(GSyntaxTreeTraverse* );
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
    virtual void traverse(GSyntaxTreeTraverse* );
public:
    GSyntaxNode* m_checkNode;
    GSyntaxNode* m_yesNode;
    GSyntaxNode* m_noNode; //可选.
};

class GAssignNode : public GSyntaxNode
{
public:
    virtual ~GAssignNode(){}
    virtual void traverse(GSyntaxTreeTraverse* );
public:
    GSyntaxNode* m_pLeftNode;
    GSyntaxNode* m_pRightNode;
};

class GDeclarationNode : public GSyntaxNode
{
public:
    virtual ~GDeclarationNode(){}
    virtual void traverse(GSyntaxTreeTraverse* );
public:
    QList<GSyntaxNode*> m_assignList;
};

class GUnaryNode : public GSyntaxNode
{
public:
    virtual ~GUnaryNode(){}
    virtual void traverse(GSyntaxTreeTraverse* );

public:
    UnaryOperator m_uOp;
    GSyntaxNode* m_pNode;
};

//二元操作符
class GBinaryNode : public GSyntaxNode
{
public:
    virtual ~GBinaryNode(){}
    virtual void traverse(GSyntaxTreeTraverse* );

public:
    BinaryOperator m_binOp;
    GSyntaxNode* m_pLeftNode;
    GSyntaxNode* m_pRightNode;
};

class GStructNode : public GSyntaxNode
{
public:
    virtual ~GStructNode(){}
    virtual void traverse(GSyntaxTreeTraverse *);

public:
    GSyntaxNode* m_pNode;
    GFiled* m_pFiled;
};

class GSizeofNode : public GSyntaxNode
{
public:
    virtual ~GSizeofNode(){}
    virtual void traverse(GSyntaxTreeTraverse* );

public:
    GSyntaxNode* m_pNode;
};

class GConstantNode : public GSyntaxNode
{
public:
    virtual ~GConstantNode(){}
    virtual void traverse(GSyntaxTreeTraverse* );

public:
    long m_value;
};

class GVariableNode : public GSyntaxNode
{
public:
    virtual ~GVariableNode(){}
    virtual void traverse(GSyntaxTreeTraverse* );
public:
    GVariable* m_pVar;
};

#endif // GSYNTAXNODE_H
