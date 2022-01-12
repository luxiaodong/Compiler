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
//    m_pTree->m_pRoot->traversal();
    return true;
}

GSyntaxNode* GParser::parseProgram()
{
    GProgramNode* node = new GProgramNode();
    node->m_pLeftNode = this->parseExpression();
    return node;
}

GSyntaxNode* GParser::parseExpression()
{
    return this->parseExpressionAdd();
}

GSyntaxNode* GParser::parseExpressionAdd()
{
    GSyntaxNode* left = this->parseExpressionMul();
    while(true)
    {
        if(m_pCurrentToken->m_type == TokenType::Add || m_pCurrentToken->m_type == TokenType::Sub)
        {
            BinaryOperator binOp = BinaryOperator::OP_Sub;
            if(m_pCurrentToken->m_type == TokenType::Add)
            {
                binOp = BinaryOperator::OP_Add;
            }
            this->getNextToken();
            GBinaryNode* node = new GBinaryNode();
            node->m_binOp = binOp;
            node->m_pLeftNode = left;
            node->m_pRightNode = this->parseExpressionMul();
            node->m_pToken = m_pCurrentToken;
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
    GSyntaxNode* left = this->parseNumber();
    while(true)
    {
        if(m_pCurrentToken->m_type == TokenType::Mul || m_pCurrentToken->m_type == TokenType::Div)
        {
            BinaryOperator binOp = BinaryOperator::OP_Div;
            if(m_pCurrentToken->m_type == TokenType::Mul)
            {
                binOp = BinaryOperator::OP_Mul;
            }
            this->getNextToken();
            GBinaryNode* node = new GBinaryNode();
            node->m_binOp = binOp;
            node->m_pLeftNode = left;
            node->m_pRightNode = this->parseNumber();
            node->m_pToken = m_pCurrentToken;
            left = node;
        }
        else
        {
            break;
        }
    }
    return left;
}

GSyntaxNode* GParser::parseNumber()
{
    GNumberNode* node = new GNumberNode();
    node->m_pToken = m_pCurrentToken;
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
