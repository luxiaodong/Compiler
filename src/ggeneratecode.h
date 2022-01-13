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
    void binaryNode(GBinaryNode* node);
    void numberNode(GNumberNode* node);

private:
    void push();
    void pop(QString reg);

private:
    QString m_assemblyCode;
    int m_deep;
};

#endif // GGENERATECODE_H
