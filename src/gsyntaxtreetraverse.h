#ifndef GSYNTAXTREETRAVERSE_H
#define GSYNTAXTREETRAVERSE_H

#include <QString>
#include <QStringList>
#include "src/gsyntaxnode.h"

class GSyntaxTreeTraverse
{
public:
    GSyntaxTreeTraverse();
    virtual ~GSyntaxTreeTraverse();
    virtual void programNode(GProgramNode* node) = 0;
    virtual void functionNode(GFunctionNode* node) = 0;
    virtual void functionCallNode(GFunctionCallNode* node) = 0;
    virtual void returnNode(GReturnNode* node) = 0;
    virtual void sentenceNode(GSentenceNode* node) = 0;
    virtual void braceNode(GBraceNode* node) = 0;
    virtual void expressionNode(GExpressionNode* node) = 0;
    virtual void expressionSentenceNode(GExpressionSentenceNode* node) = 0;
    virtual void whileNode(GWhileNode* node) = 0;
    virtual void dowhileNode(GDoWhileNode* node) = 0;
    virtual void forloopNode(GForLoopNode* node) = 0;
    virtual void conditionNode(GConditionNode* node) = 0;
    virtual void assignNode(GAssignNode* node) = 0;
    virtual void declarationNode(GDeclarationNode* node) = 0;
    virtual void unaryNode(GUnaryNode* node) = 0;
    virtual void binaryNode(GBinaryNode* node) = 0;
    virtual void sizeofNode(GSizeofNode* node) = 0;
    virtual void constantNode(GConstantNode* node) = 0;
    virtual void variableNode(GVariableNode* node) = 0;
};

#endif // GSYNTAXTREETRAVERSE_H
