#include "gsyntaxnode.h"
#include "src/ggeneratecode.h"
#include <QDebug>

void GSyntaxNode::traversal()
{
    if(m_pRightNode) m_pRightNode->traversal();
    if(m_pLeftNode) m_pLeftNode->traversal();
    if(m_pToken) m_pToken->print();
}

void GSyntaxNode::generateCode(GGenerateCode*  genCode)
{
    (void)genCode;
}

void GProgramNode::generateCode(GGenerateCode*  genCode)
{
    genCode->programNode(this);
}

void GBinaryNode::generateCode(GGenerateCode*  genCode)
{
    genCode->binaryNode(this);
}

void GNumberNode::generateCode(GGenerateCode*  genCode)
{
    genCode->numberNode(this);
}
