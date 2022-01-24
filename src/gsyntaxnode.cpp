#include "gsyntaxnode.h"
#include "src/ggeneratecode.h"
#include <QDebug>

void GSyntaxNode::traverse(GSyntaxTreeTraverse* pAst)
{
    (void)pAst;
}

void GProgramNode::traverse(GSyntaxTreeTraverse*  pAst)
{
    pAst->programNode(this);
}

void GFunctionNode::traverse(GSyntaxTreeTraverse*  pAst)
{
    pAst->functionNode(this);
}

void GFunctionCallNode::traverse(GSyntaxTreeTraverse*  pAst)
{
    pAst->functionCallNode(this);
}

void GReturnNode::traverse(GSyntaxTreeTraverse*  pAst)
{
    pAst->returnNode(this);
}

void GSentenceNode::traverse(GSyntaxTreeTraverse*  pAst)
{
    pAst->sentenceNode(this);
}

void GExpressionNode::traverse(GSyntaxTreeTraverse*  pAst)
{
    pAst->expressionNode(this);
}

void GExpressionSentenceNode::traverse(GSyntaxTreeTraverse*  pAst)
{
    pAst->expressionSentenceNode(this);
}

void GBraceNode::traverse(GSyntaxTreeTraverse*  pAst)
{
    pAst->braceNode(this);
}

void GWhileNode::traverse(GSyntaxTreeTraverse*  pAst)
{
    pAst->whileNode(this);
}

void GDoWhileNode::traverse(GSyntaxTreeTraverse*  pAst)
{
    pAst->dowhileNode(this);
}

void GForLoopNode::traverse(GSyntaxTreeTraverse*  pAst)
{
    pAst->forloopNode(this);
}

void GConditionNode::traverse(GSyntaxTreeTraverse*  pAst)
{
    pAst->conditionNode(this);
}

void GAssignNode::traverse(GSyntaxTreeTraverse*  pAst)
{
    pAst->assignNode(this);
}

void GDeclarationNode::traverse(GSyntaxTreeTraverse*  pAst)
{
    pAst->declarationNode(this);
}

void GUnaryNode::traverse(GSyntaxTreeTraverse*  pAst)
{
    pAst->unaryNode(this);
}

void GBinaryNode::traverse(GSyntaxTreeTraverse*  pAst)
{
    pAst->binaryNode(this);
}

void GSizeofNode::traverse(GSyntaxTreeTraverse*  pAst)
{
    pAst->sizeofNode(this);
}

void GConstantNode::traverse(GSyntaxTreeTraverse*  pAst)
{
    pAst->constantNode(this);
}

void GVariableNode::traverse(GSyntaxTreeTraverse*  pAst)
{
    pAst->variableNode(this);
}
