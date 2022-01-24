#include "gcalculatetype.h"
#include "src/gsymboltable.h"

GCalculateType::GCalculateType()
{}

GCalculateType::~GCalculateType()
{}

void GCalculateType::programNode(GProgramNode* node)
{
    foreach(GSyntaxNode* func, node->m_functionList)
    {
        func->calculateType(this);
    }
}

void GCalculateType::functionNode(GFunctionNode* node)
{
    foreach(GSyntaxNode* sent, node->m_sentenceList)
    {
        sent->calculateType(this);
    }
}

void GCalculateType::functionCallNode(GFunctionCallNode* node)
{
    foreach(GSyntaxNode* args, node->m_argsList)
    {
        args->calculateType(this);
    }
    node->m_pType = GBuildInType::m_intType;
}

void GCalculateType::returnNode(GReturnNode* node)
{
    node->m_pNode->calculateType(this);
    node->m_pType = node->m_pNode->m_pType;
}

void GCalculateType::sentenceNode(GSentenceNode* node)
{
    if(node->m_pNode)
    {
        node->m_pNode->calculateType(this);
        node->m_pType = node->m_pNode->m_pType;
    }
}

void GCalculateType::braceNode(GBraceNode* node)
{
    foreach(GSyntaxNode* sentence, node->m_sentenceList)
    {
        sentence->calculateType(this);
    }
}

void GCalculateType::expressionNode(GExpressionNode* node)
{
    if(node->m_pNode)
    {
        node->m_pNode->calculateType(this);
        node->m_pType = node->m_pNode->m_pType;
    }
}

void GCalculateType::expressionSentenceNode(GExpressionSentenceNode* node)
{
    foreach(GSyntaxNode* sentence, node->m_sentenceList)
    {
        sentence->calculateType(this);
    }
}

void GCalculateType::whileNode(GWhileNode* node)
{
    node->m_checkNode->calculateType(this);
    node->m_braceNode->calculateType(this);
}

void GCalculateType::dowhileNode(GDoWhileNode* node)
{
    node->m_checkNode->calculateType(this);
    node->m_braceNode->calculateType(this);
}

void GCalculateType::forloopNode(GForLoopNode* node)
{
    if(node->m_initNode) node->m_initNode->calculateType(this);
    if(node->m_checkNode) node->m_checkNode->calculateType(this);
    if(node->m_incNode) node->m_incNode->calculateType(this);
    if(node->m_braceNode) node->m_braceNode->calculateType(this);
}

void GCalculateType::conditionNode(GConditionNode* node)
{
    node->m_checkNode->calculateType(this);
    node->m_yesNode->calculateType(this);
    if(node->m_noNode)
    {
        node->m_noNode->calculateType(this);
    }
}
void GCalculateType::assignNode(GAssignNode* node)
{
    node->m_pLeftNode->calculateType(this);
    node->m_pRightNode->calculateType(this);
    node->m_pType = node->m_pLeftNode->m_pType;
}

void GCalculateType::declarationNode(GDeclarationNode* node)
{
    foreach(GSyntaxNode* sentence, node->m_assignList)
    {
        sentence->calculateType(this);
    }
}

void GCalculateType::unaryNode(GUnaryNode* node)
{
    node->m_pNode->calculateType(this);
    switch (node->m_uOp)
    {
    case UnaryOperator::OP_Plus:
    case UnaryOperator::OP_Minus:
        node->m_pType = node->m_pNode->m_pType;
        break;
    case UnaryOperator::OP_Star:
        if(node->m_pNode->m_pType->m_type == TypeKind::Kind_Pointer)
        {
            node->m_pType = node->m_pNode->m_pType;
        }
        break;
    case UnaryOperator::OP_Amp:
        node->m_pType = new GPointerType(node->m_pNode->m_pType);
        break;
    case UnaryOperator::OP_NONE:
        Q_ASSERT(false);
        break;
    }
}

void GCalculateType::binaryNode(GBinaryNode* node)
{
    node->m_pLeftNode->calculateType(this);
    node->m_pRightNode->calculateType(this);
    switch (node->m_binOp)
    {
    case BinaryOperator::OP_Add:
    case BinaryOperator::OP_Sub:
    case BinaryOperator::OP_Mul:
    case BinaryOperator::OP_Div:
    case BinaryOperator::OP_PtrAdd:
    case BinaryOperator::OP_PtrSub:
        node->m_pType = node->m_pLeftNode->m_pType;
        break;

    case BinaryOperator::OP_Equal:
    case BinaryOperator::OP_NotEqual:
    case BinaryOperator::OP_Greater:
    case BinaryOperator::OP_GreaterEqual:
    case BinaryOperator::OP_Lesser:
    case BinaryOperator::OP_LesserEqual:
    case BinaryOperator::OP_PtrDiff:
        node->m_pType = GBuildInType::m_intType;
        break;
    default:
        Q_ASSERT(false);
    }
}

void GCalculateType::sizeofNode(GSizeofNode* node)
{
    node->m_pNode->calculateType(this);
    node->m_pType = node->m_pNode->m_pType;
}

void GCalculateType::constantNode(GConstantNode* node)
{
    node->m_pType = GBuildInType::m_intType;
}

void GCalculateType::variableNode(GVariableNode* node)
{
    GVariable* pVar = GSymbolTable::getVariable(node->m_name);
    node->m_pType = pVar->m_pType;
}
