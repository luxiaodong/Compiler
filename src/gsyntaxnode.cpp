#include "gsyntaxnode.h"
#include "src/ggeneratecode.h"
#include <QDebug>

void GSyntaxNode::generateCode(GGenerateCode*  genCode)
{
    (void)genCode;
}

void GProgramNode::generateCode(GGenerateCode*  genCode)
{
    genCode->programNode(this);
}

void GSentenceNode::generateCode(GGenerateCode *genCode)
{
    genCode->sentenceNode(this);
}

void GExpressionNode::generateCode(GGenerateCode *genCode)
{
    genCode->expressionNode(this);
}

void GBraceNode::generateCode(GGenerateCode *genCode)
{
    genCode->braceNode(this);
}

void GWhileNode::generateCode(GGenerateCode *genCode)
{
    genCode->whileNode(this);
}

void GDoWhileNode::generateCode(GGenerateCode *genCode)
{
    genCode->dowhileNode(this);
}

void GForLoopNode::generateCode(GGenerateCode *genCode)
{
    genCode->forloopNode(this);
}

void GConditionNode::generateCode(GGenerateCode *genCode)
{
    genCode->conditionNode(this);
}

void GAssignNode::generateCode(GGenerateCode *genCode)
{
    genCode->assignNode(this);
}

void GBinaryNode::generateCode(GGenerateCode*  genCode)
{
    genCode->binaryNode(this);
}

void GConstantNode::generateCode(GGenerateCode*  genCode)
{
    genCode->constantNode(this);
}

void GVariableNode::generateCode(GGenerateCode *genCode)
{
    genCode->variableNode(this);
}
