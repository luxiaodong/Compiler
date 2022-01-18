#include "ggeneratecode.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include "src/gsymboltable.h"

GGenerateCode::GGenerateCode() : m_deep(0) , m_conditionIndex(0)
{
}

void GGenerateCode::printCode()
{
    qDebug()<<"=========assembly code=============";
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

    //给符号表设置地址
    int stackSize = 0;
    foreach(GVariable* var, GSymbolTable::m_variables)
    {
        stackSize += 8;
        var->m_address = stackSize * -1;
    }

    m_assemblyCode += "\tpush %rbp\n";
    m_assemblyCode += "\tmov %rsp, %rbp\n";
    //m_assemblyCode += "\tsub $32, %rsp\n";
    m_assemblyCode += QString("\tsub $%1, %rsp\n").arg(stackSize);

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
    if(node->m_pNode)
    {
        node->m_pNode->generateCode(this);
    }
}

void GGenerateCode::braceNode(GBraceNode* node)
{
    foreach(GSyntaxNode* sentence, node->m_sentenceList)
    {
        sentence->generateCode(this);
    }
}

void GGenerateCode::expressionNode(GExpressionNode* node)
{
    node->m_pNode->generateCode(this);
}

void GGenerateCode::conditionNode(GConditionNode* node)
{
    int n = m_conditionIndex++;
    node->m_checkNode->generateCode(this);
    m_assemblyCode += "\tcmp $0, %rax\n";

    if(node->m_noNode)
    {
        m_assemblyCode += QString("\tje .L.else_%1\n").arg(n);
    }
    else
    {
        m_assemblyCode += QString("\tje .L.end_%1\n").arg(n);
    }

    node->m_yesNode->generateCode(this);
    m_assemblyCode += QString("\tjmp .L.end_%1\n").arg(n);

    if(node->m_noNode)
    {
        m_assemblyCode += QString(".L.else_%1:\n").arg(n);
        node->m_noNode->generateCode(this);
        m_assemblyCode += QString("\tjmp .L.end_%1\n").arg(n);
    }

    m_assemblyCode += QString(".L.end_%1:\n").arg(n);
}

void GGenerateCode::assignNode(GAssignNode* node)
{
    GVariableNode* left = dynamic_cast<GVariableNode*>(node->m_pLeftNode);
    m_assemblyCode += QString("\tlea %1(%rbp), %rax\n").arg(GSymbolTable::getAddress(left->m_name));
    this->push();
    node->m_pRightNode->generateCode(this);
    this->pop("%rdi");
    m_assemblyCode += "\tmov %rax, (%rdi)\n";
}

void GGenerateCode::binaryNode(GBinaryNode* node)
{
    node->m_pRightNode->generateCode(this);
    this->push();
    node->m_pLeftNode->generateCode(this);
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
    case BinaryOperator::OP_Equal:
        m_assemblyCode += "\tcmp %rdi, %rax\n";
        m_assemblyCode += "\tsete %al\n";
        m_assemblyCode += "\tmovzb %al, %rax\n";
        break;
    case BinaryOperator::OP_NotEqual:
        m_assemblyCode += "\tcmp %rdi, %rax\n";
        m_assemblyCode += "\tsetne %al\n";
        m_assemblyCode += "\tmovzb %al, %rax\n";
        break;
    case BinaryOperator::OP_Greater:
        m_assemblyCode += "\tcmp %rdi, %rax\n";
        m_assemblyCode += "\tsetg %al\n";
        m_assemblyCode += "\tmovzb %al, %rax\n";
        break;
    case BinaryOperator::OP_GreaterEqual:
        m_assemblyCode += "\tcmp %rdi, %rax\n";
        m_assemblyCode += "\tsetge %al\n";
        m_assemblyCode += "\tmovzb %al, %rax\n";
        break;
    case BinaryOperator::OP_Lesser:
        m_assemblyCode += "\tcmp %rdi, %rax\n";
        m_assemblyCode += "\tsetl %al\n";
        m_assemblyCode += "\tmovzb %al, %rax\n";
        break;
    case BinaryOperator::OP_LesserEqual:
        m_assemblyCode += "\tcmp %rdi, %rax\n";
        m_assemblyCode += "\tsetle %al\n";
        m_assemblyCode += "\tmovzb %al, %rax\n";
        break;

    default:
        Q_ASSERT(false);
        break;
    }
}

void GGenerateCode::constantNode(GConstantNode* node)
{
    m_assemblyCode += QString("\tmov $%1, %rax\n").arg(node->m_value);
}

void GGenerateCode::variableNode(GVariableNode* node)
{
    m_assemblyCode += QString("\tlea %1(%rbp), %rax\n").arg(GSymbolTable::getAddress(node->m_name));
    m_assemblyCode += QString("\tmov (%rax), %rax\n");
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
