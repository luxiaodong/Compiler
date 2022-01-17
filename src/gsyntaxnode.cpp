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

void GBinaryNode::generateCode(GGenerateCode*  genCode)
{
    genCode->binaryNode(this);
}

void GNumberNode::generateCode(GGenerateCode*  genCode)
{
    genCode->numberNode(this);
}
