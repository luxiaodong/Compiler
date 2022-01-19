#ifndef GGENERATECODE_H
#define GGENERATECODE_H

#include <QString>
#include <QStringList>
#include "src/gsyntaxnode.h"

class GGenerateCode
{
public:
    GGenerateCode();
    void printCode();
    void programNode(GProgramNode* node);
    void sentenceNode(GSentenceNode* node);
    void braceNode(GBraceNode* node);
    void expressionNode(GExpressionNode* node);
    void whileNode(GWhileNode* node);
    void dowhileNode(GDoWhileNode* node);
    void forloopNode(GForLoopNode* node);
    void conditionNode(GConditionNode* node);
    void assignNode(GAssignNode* node);
    void binaryNode(GBinaryNode* node);
    void constantNode(GConstantNode* node);
    void variableNode(GVariableNode* node);

private:
    void push();
    void pop(QString reg);

private:
    QString m_assemblyCode;
    int m_deep;
    int m_conditionIndex;
};

#endif // GGENERATECODE_H
