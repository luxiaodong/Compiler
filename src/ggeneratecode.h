#ifndef GGENERATECODE_H
#define GGENERATECODE_H

#include <QString>
#include <QStringList>
#include "src/gsyntaxtreetraverse.h"

class GGenerateCode : public GSyntaxTreeTraverse
{
public:
    GGenerateCode();
    virtual ~GGenerateCode();
    void printCode();
    virtual void programNode(GProgramNode* node);
    virtual void functionNode(GFunctionNode* node);
    virtual void functionCallNode(GFunctionCallNode* node);
    virtual void returnNode(GReturnNode* node);
    virtual void sentenceNode(GSentenceNode* node);
    virtual void braceNode(GBraceNode* node);
    virtual void expressionNode(GExpressionNode* node);
    virtual void expressionSentenceNode(GExpressionSentenceNode* node);
    virtual void whileNode(GWhileNode* node);
    virtual void dowhileNode(GDoWhileNode* node);
    virtual void forloopNode(GForLoopNode* node);
    virtual void conditionNode(GConditionNode* node);
    virtual void assignNode(GAssignNode* node);
    virtual void declarationNode(GDeclarationNode* node);
    virtual void unaryNode(GUnaryNode* node);
    virtual void binaryNode(GBinaryNode* node);
    virtual void sizeofNode(GSizeofNode* node);
    virtual void constantNode(GConstantNode* node);
    virtual void variableNode(GVariableNode* node);
    virtual void structNode(GStructNode *node);

private:
    void genAddress(GSyntaxNode* node);
    void load(GType* pType);
    void store(GType* pType);

    void push();
    void pop(QString reg);
    int alignTo(int size, int align);

private:
    QString m_assemblyCode;
    QString m_currentFuncName;
    int m_deep;
    int m_conditionIndex;
};

#endif // GGENERATECODE_H
