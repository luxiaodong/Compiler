#include "ggeneratecode.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>

const char *Reg8[6] = {"%dil", "%sil", "%dl", "%cl", "%r8b", "%r9b"};
const char *Reg16[6] = {"%di", "%si", "%dx", "%cx", "%r8w", "%r9w"};
const char *Reg32[6] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};
const char *Reg64[6] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};

GGenerateCode::GGenerateCode() : m_deep(0) , m_conditionIndex(0)
{
}

GGenerateCode::~GGenerateCode()
{}

void GGenerateCode::printCode()
{
    qDebug()<<"=========assembly code=============";
    qDebug().noquote()<<m_assemblyCode;
    qDebug()<<"deep is "<<m_deep;

    QString filePath = QDir::currentPath() + QString("/../Compiler/test/test.s");
    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream ts(&file);
        ts<<m_assemblyCode;
    }
}

void GGenerateCode::programNode(GProgramNode* node)
{
    foreach(GSyntaxNode* func, node->m_functionList)
    {
        func->generateCode(this);
    }
}

void GGenerateCode::functionNode(GFunctionNode* node)
{
//    m_assemblyCode  = "";
    m_currentFuncName = node->m_funcName;
    m_assemblyCode += "\t.text\n";
    m_assemblyCode += QString("\t.globl _%1\n").arg(m_currentFuncName);
    m_assemblyCode += QString("_%1:\n").arg(m_currentFuncName);

    //给符号表设置地址
    int offset = 0;
    foreach(GVariable* var, node->m_locals)
    {
        offset += var->m_pType->m_size;
        offset = this->alignTo(offset, var->m_pType->m_align);
        var->m_address = -offset;
    }
    offset = this->alignTo(offset, 16);

    m_assemblyCode += "\tpush %rbp\n";
    m_assemblyCode += "\tmov %rsp, %rbp\n";
    m_assemblyCode += QString("\tsub $%1, %rsp\n").arg(offset);

    //更多的参数要压入栈, 暂不支持
    for(int i = 0; i < node->m_args.size(); ++i)
    {
        GVariable* var = node->m_args.at(i);
        if(var->m_pType->m_size == 1)
        {
            m_assemblyCode += QString("\tmov %1, %2(%rbp)\n").arg(Reg8[i]).arg(var->m_address);
        }
        else if(var->m_pType->m_size == 2)
        {
            m_assemblyCode += QString("\tmov %1, %2(%rbp)\n").arg(Reg16[i]).arg(var->m_address);
        }
        else if(var->m_pType->m_size == 4)
        {
            m_assemblyCode += QString("\tmov %1, %2(%rbp)\n").arg(Reg32[i]).arg(var->m_address);
        }
        else if(var->m_pType->m_size == 8)
        {
            m_assemblyCode += QString("\tmov %1, %2(%rbp)\n").arg(Reg64[i]).arg(var->m_address);
        }
        else
        {
            Q_ASSERT(false);
        }
    }

    foreach(GSyntaxNode* sentence, node->m_sentenceList)
    {
        sentence->generateCode(this);
    }

    m_assemblyCode += QString(".LReturn_%1:\n").arg(m_currentFuncName);
    m_assemblyCode += "\tmov %rbp, %rsp\n";
    m_assemblyCode += "\tpop %rbp\n";
    m_assemblyCode += "\tret\n";
}

void GGenerateCode::functionCallNode(GFunctionCallNode* node)
{
    foreach(GSyntaxNode* arg, node->m_argsList)
    {
        arg->generateCode(this);
        this->push();
    }

    for(int i = node->m_argsList.size() -1; i>=0; i--)
    {
        this->pop(Reg64[i]);
    }

    m_assemblyCode += "\tmov $0, %rax\n";
    m_assemblyCode += QString("\tcall _%1\n").arg(node->m_funcName);
}

void GGenerateCode::returnNode(GReturnNode* node)
{
    node->m_pNode->generateCode(this);
    m_assemblyCode += QString("\tjmp .LReturn_%1\n").arg(m_currentFuncName);
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
    if(node->m_pNode)
    {
        node->m_pNode->generateCode(this);
    }
}

void GGenerateCode::expressionSentenceNode(GExpressionSentenceNode* node)
{
    foreach(GSyntaxNode* sentence, node->m_sentenceList)
    {
        sentence->generateCode(this);
    }
}

void GGenerateCode::whileNode(GWhileNode* node)
{
    int n = m_conditionIndex++;
    m_assemblyCode += QString(".L.begin_%1:\n").arg(n);
    node->m_checkNode->generateCode(this);
    m_assemblyCode += "\tcmp $0, %rax\n";
    m_assemblyCode += QString("\tje .L.end_%1\n").arg(n);
    node->m_braceNode->generateCode(this);
    m_assemblyCode += QString("\tjmp .L.begin_%1\n").arg(n);
    m_assemblyCode += QString(".L.end_%1:\n").arg(n);
}

void GGenerateCode::dowhileNode(GDoWhileNode* node)
{
    int n = m_conditionIndex++;
    m_assemblyCode += QString(".L.begin_%1:\n").arg(n);
    node->m_braceNode->generateCode(this);
    node->m_checkNode->generateCode(this);
    m_assemblyCode += QString("\tcmp $0, %rax\n");
    m_assemblyCode += QString("\tje .L.end_%1\n").arg(n);
    m_assemblyCode += QString("\tjmp .L.begin_%1\n").arg(n);
    m_assemblyCode += QString(".L.end_%1:\n").arg(n);
}

void GGenerateCode::forloopNode(GForLoopNode* node)
{
    int n = m_conditionIndex++;
    if(node->m_initNode)
    {
        node->m_initNode->generateCode(this);
    }
    m_assemblyCode += QString(".L.begin_%1:\n").arg(n);
    if(node->m_checkNode)
    {
        node->m_checkNode->generateCode(this);
        m_assemblyCode += QString("\tcmp $0, %rax\n");
        m_assemblyCode += QString("\tje .L.end_%1\n").arg(n);
    }
    node->m_braceNode->generateCode(this);
    if(node->m_incNode)
    {
        node->m_incNode->generateCode(this);
    }
    m_assemblyCode += QString("\tjmp .L.begin_%1\n").arg(n);
    m_assemblyCode += QString(".L.end_%1:\n").arg(n);
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
    this->genAddress(node->m_pLeftNode);
    this->push();
    node->m_pRightNode->generateCode(this);
    this->store(node->m_pType);
}

void GGenerateCode::declarationNode(GDeclarationNode* node)
{
    foreach(GSyntaxNode* assign, node->m_assignList)
    {
        assign->generateCode(this);
    }
}

void GGenerateCode::unaryNode(GUnaryNode* node)
{
    if(node->m_uOp == UnaryOperator::OP_Plus)
    {
        node->m_pNode->generateCode(this);
    }
    else if(node->m_uOp == UnaryOperator::OP_Minus)
    {
        node->m_pNode->generateCode(this);
        m_assemblyCode += "\tneg %rax\n";
    }
    else if(node->m_uOp == UnaryOperator::OP_Star)
    {
        this->genAddress(node);
        this->load(node->m_pType);
    }
    else if(node->m_uOp == UnaryOperator::OP_Amp)
    {
        this->genAddress(node->m_pNode);
    }
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
    case BinaryOperator::OP_PtrAdd:
        if(node->m_pLeftNode->m_pType->m_type == Kind_Array)
        {
            GArrayType *pType = dynamic_cast<GArrayType*>(node->m_pLeftNode->m_pType);
            m_assemblyCode += QString("\timul $%1, %rdi\n").arg(pType->m_elementType->m_size);
        }
        else if(node->m_pLeftNode->m_pType->m_type == Kind_Pointer)
        {
            GPointerType* pType = dynamic_cast<GPointerType*>(node->m_pLeftNode->m_pType);
            m_assemblyCode += QString("\timul $%1, %rdi\n").arg(pType->m_baseType->m_size);
        }
        m_assemblyCode += "\tadd %rdi, %rax\n";
        break;
    case BinaryOperator::OP_PtrSub:
        if(node->m_pLeftNode->m_pType->m_type == Kind_Array)
        {
            GArrayType *pType = dynamic_cast<GArrayType*>(node->m_pLeftNode->m_pType);
            m_assemblyCode += QString("\timul $%1, %rdi\n").arg(pType->m_elementType->m_size);
        }
        else if(node->m_pLeftNode->m_pType->m_type == Kind_Pointer)
        {
            GPointerType* pType = dynamic_cast<GPointerType*>(node->m_pLeftNode->m_pType);
            m_assemblyCode += QString("\timul $%1, %rdi\n").arg(pType->m_baseType->m_size);
        }
        m_assemblyCode += "\tsub %rdi, %rax\n";
        break;
    case BinaryOperator::OP_PtrDiff:
        m_assemblyCode += "\tsub %rdi, %rax\n";
        if(node->m_pLeftNode->m_pType->m_type == Kind_Array)
        {
            GArrayType *pType = dynamic_cast<GArrayType*>(node->m_pLeftNode->m_pType);
            m_assemblyCode += QString("\tmov $%1, %rdi\n").arg(pType->m_elementType->m_size);
        }
        else if(node->m_pLeftNode->m_pType->m_type == Kind_Pointer)
        {
            GPointerType* pType = dynamic_cast<GPointerType*>(node->m_pLeftNode->m_pType);
            m_assemblyCode += QString("\tmov $%1, %rdi\n").arg(pType->m_baseType->m_size);
        }
        m_assemblyCode += "\tcqo\n";
        m_assemblyCode += "\tidiv %rdi\n";
        break;

    default:
        Q_ASSERT(false);
        break;
    }
}

void GGenerateCode::sizeofNode(GSizeofNode* node)
{
    m_assemblyCode += QString("\tmov $%1, %rax\n").arg(node->m_pType->m_size);
}

void GGenerateCode::constantNode(GConstantNode* node)
{
//    m_assemblyCode += QString("\tmov $%1, %rax\n").arg(node->m_value);
    m_assemblyCode += QString("\tmovabs $%1, %rax\n").arg(node->m_value);
}

void GGenerateCode::variableNode(GVariableNode* node)
{
    genAddress(node);
    load(node->m_pType);
}

void GGenerateCode::structNode(GStructNode *node)
{
    genAddress(node);
    load(node->m_pType);
}

void GGenerateCode::genAddress(GSyntaxNode* node)
{
    GVariableNode* varNode = dynamic_cast<GVariableNode*>(node);
    if(varNode)
    {
        m_assemblyCode += QString("\tlea %1(%rbp), %rax\n").arg(varNode->m_pVar->m_address);
    }
    else
    {
        GUnaryNode* unaryNode = dynamic_cast<GUnaryNode*>(node);
        if(unaryNode)
        {
            if(unaryNode->m_uOp == UnaryOperator::OP_Star)
            {
                unaryNode->m_pNode->generateCode(this);
            }
        }
        else
        {
            GStructNode* structNode = dynamic_cast<GStructNode*>(node);
            this->genAddress(structNode->m_pNode);
            m_assemblyCode += QString("\tadd $%1, %rax\n").arg(structNode->m_pFiled->m_offset);
        }
    }
}

void GGenerateCode::load(GType* pType)
{
    if(pType->isSameTypeKind(Kind_Array)) return ;
    if(pType->isSameTypeKind(Kind_StructUnion)) return ;

    if(pType->m_size == 1)
    {
        m_assemblyCode += QString("\tmovsb (%rax), %rax\n");
    }
    else if(pType->m_size == 2)
    {
        m_assemblyCode += QString("\tmovsw (%rax), %rax\n");
    }
    else if(pType->m_size == 4)
    {
        m_assemblyCode += QString("\tmovsl (%rax), %rax\n");
    }
    else if(pType->m_size == 8)
    {
        m_assemblyCode += QString("\tmov (%rax), %rax\n");
    }
    else
    {
        Q_ASSERT(false);
    }
}

void GGenerateCode::store(GType* pType)
{
    this->pop("%rdi");
    if(pType->m_size == 1)
    {
        m_assemblyCode += QString("\tmov %al, (%rdi)\n");
    }
    else if(pType->m_size == 2)
    {
        m_assemblyCode += QString("\tmov %ax, (%rdi)\n");
    }
    else if(pType->m_size == 4)
    {
        m_assemblyCode += QString("\tmov %eax, (%rdi)\n");
    }
    else if(pType->m_size == 8)
    {
        m_assemblyCode += QString("\tmov %rax, (%rdi)\n");
    }
    else
    {
        qDebug()<<pType->m_size;
        Q_ASSERT(false);
    }
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

int GGenerateCode::alignTo(int size, int align)
{
    return (size + align - 1) / align * align;
}

