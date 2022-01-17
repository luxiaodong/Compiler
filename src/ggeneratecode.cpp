#include "ggeneratecode.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>

GGenerateCode::GGenerateCode() : m_deep(0)
{
}

void GGenerateCode::printCode()
{
    qDebug().noquote()<<m_assemblyCode;
    qDebug()<<"deep is "<<m_deep;

    QString filePath = QDir::currentPath() + QString("/../Compiler/calculate/prog.s");
    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream ts(&file);
        ts<<m_assemblyCode;
    }
}

void GGenerateCode::programNode(GProgramNode* node)
{
    m_assemblyCode  = "";
    m_assemblyCode += "\t.text\n";
    m_assemblyCode += "\t.globl _prog\n";
    m_assemblyCode += "_prog:\n";
    m_assemblyCode += "\tpush %rbp\n";
    m_assemblyCode += "\tmov %rsp, %rbp\n";
    m_assemblyCode += "\tsub $32, %rsp\n";

    foreach(GSyntaxNode* sentence, node->m_sentenceList)
    {
        sentence->generateCode(this);
    }

    m_assemblyCode += "\tmov %rbp, %rsp\n";
    m_assemblyCode += "\tpop %rbp\n";
    m_assemblyCode += "\tret\n";
}

void GGenerateCode::sentenceNode(GSentenceNode* node)
{
    node->m_pNode->generateCode(this);
}

void GGenerateCode::expressionNode(GExpressionNode* node)
{
    node->m_pNode->generateCode(this);
}

void GGenerateCode::binaryNode(GBinaryNode* node)
{
    if(node->m_pRightNode) node->m_pRightNode->generateCode(this);
    this->push();
    if(node->m_pLeftNode) node->m_pLeftNode->generateCode(this);
    this->pop("%rdi");

    switch (node->m_binOp) {
    case BinaryOperator::OP_Add:
        m_assemblyCode += "\tadd %rdi, %rax\n";
        break;
    case BinaryOperator::OP_Sub:
        m_assemblyCode += "\tsub %rdi, %rax\n";
        break;
    case BinaryOperator::OP_Mul:
        m_assemblyCode += "\timul %rdi, %rax\n";
        break;
    case BinaryOperator::OP_Div:
        m_assemblyCode += "\tcqo\n";
        m_assemblyCode += "\tidiv %rdi\n";
        break;
    default:
        Q_ASSERT(false);
        break;
    }
}

void GGenerateCode::numberNode(GNumberNode* node)
{
    m_assemblyCode += QString("\tmov $%1, %rax\n").arg(node->m_value);
}

void GGenerateCode::push()
{
    m_assemblyCode += "\tpush %rax\n";
    m_deep++;
}

void GGenerateCode::pop(QString reg)
{
    m_assemblyCode += QString("\tpop %1\n").arg(reg);
    m_deep--;
}
