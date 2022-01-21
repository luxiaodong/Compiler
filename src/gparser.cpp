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
    node->m_functionList.clear();
    while(m_pCurrentToken->m_type != TokenType::Eof)
    {
        node->m_functionList.append(this->parseFunction());
    }
    Q_ASSERT(m_pCurrentToken->m_type == TokenType::Eof);
    return node;
}

GSyntaxNode* GParser::parseFunction()
{
    GFunctionNode* node = new GFunctionNode();
    GType* baseType = this->parseDeclarationSpec();
    GToken*  pToken;
    node->m_pType = this->parseDeclarator(baseType, pToken);
    node->m_funcName = pToken->m_context; //函数名
    GFunctionType* funType = dynamic_cast<GFunctionType*>(node->m_pType);

    for(int i = 0; i < funType->m_paramList.size(); i++)
    {
        QString name = funType->m_paramList.at(i)->m_pToken->m_context;
        GType* pType = funType->m_paramList.at(i)->m_pType;
        GSymbolTable::addVariable(name, pType);
        GVariable* var = GSymbolTable::getVariable(name);
        node->m_args.append(var);
    }
    // node->m_pType = funType;
    Q_ASSERT(m_pCurrentToken->m_type == TokenType::LeftBrace);
    // node->m_braceNode = this->parseSentence();

    this->getNextToken();
    node->m_sentenceList.clear();
    while(m_pCurrentToken->m_type != TokenType::RightBrace)
    {
//            qDebug()<<m_pCurrentToken->m_context;
        node->m_sentenceList.append(this->parseSentence());
    }
    Q_ASSERT(m_pCurrentToken->m_type == TokenType::RightBrace);
    this->getNextToken();
    return node;
}

GSyntaxNode* GParser::parseFunctionCall()
{
    GFunctionCallNode* node = new GFunctionCallNode();
    node->m_funcName = m_pCurrentToken->m_context;
    Q_ASSERT(m_pCurrentToken->m_type == TokenType::Identifier);
    this->getNextToken();
    Q_ASSERT(m_pCurrentToken->m_type == TokenType::LeftParent);
    this->getNextToken();
    if(m_pCurrentToken->m_type != TokenType::RightParent) //有参数
    {
        node->m_argsList.append( this->parseExpressionAssign() );
        while( m_pCurrentToken->m_type == TokenType::Comma )
        {
            this->getNextToken();
            node->m_argsList.append( this->parseExpressionAssign() );
        }
    }
    Q_ASSERT(m_pCurrentToken->m_type == TokenType::RightParent);
    this->getNextToken();
//    Q_ASSERT(m_pCurrentToken->m_type == TokenType::Semicolon); // 函数后面不一定是; 也可能是+
    return node;
}

GSyntaxNode* GParser::parseSentence()
{
    if(m_pCurrentToken->m_type == TokenType::If)
    {
        GConditionNode* node = new GConditionNode();
        this->getNextToken();
        Q_ASSERT(m_pCurrentToken->m_type == TokenType::LeftParent);
        this->getNextToken();
        node->m_checkNode = this->parseExpression();
        Q_ASSERT(m_pCurrentToken->m_type == TokenType::RightParent);
        this->getNextToken();
        node->m_yesNode =  this->parseSentence();
        if(m_pCurrentToken->m_type == TokenType::Else)
        {
            this->getNextToken();
            node->m_noNode = this->parseSentence();
        }
        return node;
    }
    else if(m_pCurrentToken->m_type == TokenType::While)
    {
        GWhileNode* node = new GWhileNode();
        this->getNextToken();
        Q_ASSERT(m_pCurrentToken->m_type == TokenType::LeftParent);
        this->getNextToken();
        node->m_checkNode = this->parseExpression();
        Q_ASSERT(m_pCurrentToken->m_type == TokenType::RightParent);
        this->getNextToken();
        node->m_braceNode = this->parseSentence();
        return node;
    }
    else if(m_pCurrentToken->m_type == TokenType::Do)
    {
        GDoWhileNode* node = new GDoWhileNode();
        this->getNextToken();
        Q_ASSERT(m_pCurrentToken->m_type == TokenType::LeftBrace); //do后面必跟{
        node->m_braceNode = this->parseSentence();
        Q_ASSERT(m_pCurrentToken->m_type == TokenType::While);
        this->getNextToken();
        Q_ASSERT(m_pCurrentToken->m_type == TokenType::LeftParent);
        this->getNextToken();
        node->m_checkNode = this->parseExpression();
        Q_ASSERT(m_pCurrentToken->m_type == TokenType::RightParent);
        this->getNextToken();
        Q_ASSERT(m_pCurrentToken->m_type == TokenType::Semicolon);
        this->getNextToken();
        return node;
    }
    else if(m_pCurrentToken->m_type == TokenType::For)
    {
        GForLoopNode* node = new GForLoopNode();
        this->getNextToken();
        Q_ASSERT(m_pCurrentToken->m_type == TokenType::LeftParent);
        this->getNextToken();
        if(m_pCurrentToken->m_type == TokenType::Semicolon) //可能是直接分号,没有语句
        {
            node->m_initNode = NULL;
        }
        else
        {
            node->m_initNode = this->parseExpression();
        }
        Q_ASSERT(m_pCurrentToken->m_type == TokenType::Semicolon);
        this->getNextToken();
        if(m_pCurrentToken->m_type == TokenType::Semicolon)
        {
            node->m_checkNode = NULL;
        }
        else
        {
            node->m_checkNode = this->parseExpression();
        }
        Q_ASSERT(m_pCurrentToken->m_type == TokenType::Semicolon);
        this->getNextToken();
        if(m_pCurrentToken->m_type == TokenType::RightParent)
        {
            node->m_incNode = NULL;
        }
        else
        {
            node->m_incNode = this->parseExpression();
        }
        Q_ASSERT(m_pCurrentToken->m_type == TokenType::RightParent);
        this->getNextToken();
        node->m_braceNode = this->parseSentence();
        return node;
    }
    else if(m_pCurrentToken->m_type == TokenType::LeftBrace)
    {
        GBraceNode* node = new GBraceNode();
        this->getNextToken();
        node->m_sentenceList.clear();
        while(m_pCurrentToken->m_type != TokenType::RightBrace)
        {
            node->m_sentenceList.append(this->parseSentence());
        }
        Q_ASSERT(m_pCurrentToken->m_type == TokenType::RightBrace);
        this->getNextToken();
        return node;
    }
    else if(m_pCurrentToken->m_type == TokenType::Return)
    {
        GReturnNode* node = new GReturnNode();
        this->getNextToken();
        node->m_pNode = this->parseExpression();
//        qDebug()<<m_pCurrentToken->m_context;
        Q_ASSERT(m_pCurrentToken->m_type == TokenType::Semicolon);
        this->getNextToken();
        return node;
    }

    GSentenceNode* node = new GSentenceNode();
    if(m_pCurrentToken->m_type != TokenType::Semicolon)
    {
        node->m_pNode = this->parseExpression();
    }
    else
    {
        node->m_pNode = NULL;
    }

    qDebug()<<m_pCurrentToken->m_context;
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
    GSyntaxNode* left = this->parseExpressionEqual();
    if(m_pCurrentToken->m_type == TokenType::Assign)
    {
        this->getNextToken();
        GAssignNode* node = new GAssignNode();
        node->m_pLeftNode = left;
        node->m_pRightNode = this->parseExpressionEqual();
        return node;
    }
    return left;
}

GSyntaxNode* GParser::parseExpressionEqual()
{
    GSyntaxNode* left = this->parseExpressionRelation();
    while(m_pCurrentToken->m_type == TokenType::Equal || m_pCurrentToken->m_type == TokenType::NotEqual)
    {
        BinaryOperator binOp = BinaryOperator::OP_Equal;
        if(m_pCurrentToken->m_type == TokenType::NotEqual)
        {
            binOp = BinaryOperator::OP_NotEqual;
        }

        GBinaryNode* node = new GBinaryNode();
        node->m_pToken = m_pCurrentToken;
        this->getNextToken();
        node->m_binOp = binOp;
        node->m_pLeftNode = left;
        node->m_pRightNode = this->parseExpressionRelation();
        left = node;
    }
    return left;
}

GSyntaxNode* GParser::parseExpressionRelation()
{
    GSyntaxNode* left = this->parseExpressionAdd();
    while(m_pCurrentToken->m_type == TokenType::Greater || m_pCurrentToken->m_type == TokenType::GreaterEqual
        || m_pCurrentToken->m_type == TokenType::Lesser || m_pCurrentToken->m_type == TokenType::LesserEqual)
    {
        BinaryOperator binOp = BinaryOperator::OP_Greater;
        if(m_pCurrentToken->m_type == TokenType::GreaterEqual)
        {
            binOp = BinaryOperator::OP_GreaterEqual;
        }
        else if(m_pCurrentToken->m_type == TokenType::Lesser)
        {
            binOp = BinaryOperator::OP_Lesser;
        }
        else if(m_pCurrentToken->m_type == TokenType::LesserEqual)
        {
            binOp = BinaryOperator::OP_LesserEqual;
        }

        GBinaryNode* node = new GBinaryNode();
        node->m_pToken = m_pCurrentToken;
        this->getNextToken();
        node->m_binOp = binOp;
        node->m_pLeftNode = left;
        node->m_pRightNode = this->parseExpressionAdd();
        left = node;
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
    if(m_pCurrentToken->m_type == TokenType::LeftParent)
    {
        this->getNextToken();
        GSyntaxNode* node = this->parseExpression();
        Q_ASSERT(m_pCurrentToken->m_type == TokenType::RightParent);
        this->getNextToken();
        return node;
    }
    else if(m_pCurrentToken->m_type == TokenType::Identifier)
    {
        GToken* nextToken = this->lookNextToken();
        if(nextToken->m_type == TokenType::LeftParent) //这里假设函数调用
        {
            return this->parseFunctionCall();
        }

        GVariableNode* node = new GVariableNode();
        node->m_pToken = m_pCurrentToken;
        node->m_name = m_pCurrentToken->m_context;
         GSymbolTable::addVariable(node->m_name, GBuildInType::m_intType); //添加到符号表
//        Q_ASSERT(false); //这里不可能走到了, 只有单个变量名, 没有类型
        this->getNextToken();
        return node;
    }

    GConstantNode* node = new GConstantNode();
    node->m_pToken = m_pCurrentToken;
    node->m_value = m_pCurrentToken->m_context.toInt();
    this->getNextToken();
    return node;
}

GType* GParser::parseDeclarationSpec()
{
    if(m_pCurrentToken->m_type == TokenType::Int)
    {
        this->getNextToken();
        return GBuildInType::m_intType;
    }

    return NULL;
}

GType* GParser::parseDeclarator(GType* baseType, GToken* &pToken)
{
    GType* pType = baseType;
    while(m_pCurrentToken->m_type == TokenType::Mul)
    {
        pType = new GPointerType(pType);
        this->getNextToken();
    }

    Q_ASSERT(m_pCurrentToken->m_type == TokenType::Identifier);
    pToken = m_pCurrentToken; //第一个变量.
    this->getNextToken();
    pType = parseTypeSuffix(pType);
    return pType;
}

GType* GParser::parseTypeSuffix(GType* pType)
{
    if(m_pCurrentToken->m_type == TokenType::LeftParent) //是个函数
    {
        GFunctionType* pFunType = new GFunctionType(pType);
        this->getNextToken();

        if(m_pCurrentToken->m_type != TokenType::RightParent)
        {
            while(true)
            {
                GToken* pToken;
                GType* argcType = this->parseDeclarator(this->parseDeclarationSpec(), pToken);
                GFunctionParam* param = new GFunctionParam();
                param->m_pType = argcType;
                param->m_pToken = pToken;
                pFunType->m_paramList.append(param);

                if(m_pCurrentToken->m_type == TokenType::RightParent)
                {
                    break;
                }

                Q_ASSERT(m_pCurrentToken->m_type == TokenType::Comma);
                this->getNextToken();
            }
        }
        Q_ASSERT(m_pCurrentToken->m_type == TokenType::RightParent);
        this->getNextToken();
        return pFunType;
    }

    return pType;
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

GToken* GParser::lookNextToken()
{
    if(m_tokenIndex < m_lexer.m_tokenList.size())
    {
        return m_lexer.m_tokenList.at(m_tokenIndex);
    }

    return NULL;
}
