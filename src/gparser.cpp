#include "gparser.h"

GParser::GParser(const GLexer &lexer):m_lexer(lexer),m_tokenIndex(0)
{
    m_pTree = new GSyntaxTree();
}

GParser::~GParser()
{
    delete m_pTree;
}

bool GParser::analysis()
{
    this->getNextToken();
    m_pTree->m_pRoot = this->parseProgram();
    return true;
}

GSyntaxNode* GParser::parseProgram()
{
    GProgramNode* node = new GProgramNode();
    node->m_sentenceList.clear();
    while(m_pCurrentToken->m_type != TokenType::Eof)
    {
        node->m_sentenceList.append(this->parseSentence());
    }

    return node;
}

GSyntaxNode* GParser::parseSentence()
{
    GSentenceNode* node = new GSentenceNode();
    node->m_pNode = this->parseExpression();
    Q_ASSERT(m_pCurrentToken->m_type == TokenType::Semicolon);
    this->getNextToken();
    return node;
}

GSyntaxNode* GParser::parseExpression()
{
    GExpressionNode* node = new GExpressionNode();
    node->m_pNode = this->parseExpressionAssign();
    return node;
}

GSyntaxNode* GParser::parseExpressionAssign()
{
    GSyntaxNode* left = this->parseExpressionAdd();
    if(m_pCurrentToken->m_type == TokenType::Assign)
    {
        this->getNextToken();
        GAssignNode* node = new GAssignNode();
        node->m_pLeftNode = left;
        node->m_pRightNode = this->parseExpressionAssign();
        return node;
    }
    return left;
}

GSyntaxNode* GParser::parseExpressionAdd()
{
    GSyntaxNode* left = this->parseExpressionMul();
    while(m_pCurrentToken)
    {
        if(m_pCurrentToken->m_type == TokenType::Add || m_pCurrentToken->m_type == TokenType::Sub)
        {
            BinaryOperator binOp = BinaryOperator::OP_Sub;
            if(m_pCurrentToken->m_type == TokenType::Add)
            {
                binOp = BinaryOperator::OP_Add;
            }

            GBinaryNode* node = new GBinaryNode();
            node->m_pToken = m_pCurrentToken;
            this->getNextToken();
            node->m_binOp = binOp;
            node->m_pLeftNode = left;
            node->m_pRightNode = this->parseExpressionMul();
            left = node;
        }
        else
        {
            break;
        }
    }
    return left;
}

GSyntaxNode* GParser::parseExpressionMul()
{
    GSyntaxNode* left = this->parseConstant();
    while(m_pCurrentToken)
    {
        if(m_pCurrentToken->m_type == TokenType::Mul || m_pCurrentToken->m_type == TokenType::Div)
        {
            BinaryOperator binOp = BinaryOperator::OP_Div;
            if(m_pCurrentToken->m_type == TokenType::Mul)
            {
                binOp = BinaryOperator::OP_Mul;
            }

            GBinaryNode* node = new GBinaryNode();
            node->m_pToken = m_pCurrentToken;
            this->getNextToken();
            node->m_binOp = binOp;
            node->m_pLeftNode = left;
            node->m_pRightNode = this->parseConstant();
            left = node;
        }
        else
        {
            break;
        }
    }
    return left;
}

GSyntaxNode* GParser::parseConstant()
{
    // todo check point.
    if(m_pCurrentToken && m_pCurrentToken->m_type == TokenType::LeftParent)
    {
        this->getNextToken();
        GSyntaxNode* node = this->parseExpression();
        this->getNextToken();
        return node;
    }
    else if(m_pCurrentToken->m_type == TokenType::Identifier)
    {
        GVariableNode* node = new GVariableNode();
        node->m_pToken = m_pCurrentToken;
        node->m_name = m_pCurrentToken->m_context;
        GSymbolTable::addVariable(node->m_name); //添加到符号表
        this->getNextToken();
        return node;
    }

    GConstantNode* node = new GConstantNode();
    node->m_pToken = m_pCurrentToken;
    node->m_value = m_pCurrentToken->m_context.toInt();
    this->getNextToken();
    return node;
}

void GParser::getNextToken()
{
    if(m_tokenIndex < m_lexer.m_tokenList.size())
    {
        m_pCurrentToken = m_lexer.m_tokenList.at(m_tokenIndex);
        m_tokenIndex++;
    }
    else
    {
        m_pCurrentToken = NULL;
    }
}
