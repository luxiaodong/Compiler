#include "gparser.h"
#include "src/gcalculatetype.h"

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
    m_scope.enterScope();
    this->getNextToken();
    m_pTree->m_pRoot = this->parseProgram();
    m_scope.leaveScope();
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
    m_pLocals = &node->m_locals;

    GType* baseType = this->parseDeclarationSpec();
    GToken* pToken;
    node->m_pType = this->parseDeclarator(baseType, pToken);
    node->m_funcName = pToken->m_context; //函数名
    GFunctionType* funType = dynamic_cast<GFunctionType*>(node->m_pType);

    for(int i = 0; i < funType->m_paramList.size(); i++)
    {
        QString name = funType->m_paramList.at(i)->m_pToken->m_context;
        GType* pType = funType->m_paramList.at(i)->m_pType;
        node->m_args.append( this->createVariable(name, pType));
    }
    node->m_pType = funType;
    m_scope.enterScope();
    Q_ASSERT(m_pCurrentToken->m_type == TokenType::LeftBrace);
    this->getNextToken();
    node->m_sentenceList.clear();
    while(m_pCurrentToken->m_type != TokenType::RightBrace)
    {
        node->m_sentenceList.append(this->parseSentence());
    }
    Q_ASSERT(m_pCurrentToken->m_type == TokenType::RightBrace);
    this->getNextToken();
    m_scope.leaveScope();
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
        m_scope.enterScope();
        GBraceNode* node = new GBraceNode();
        this->getNextToken();
        node->m_sentenceList.clear();
        while(m_pCurrentToken->m_type != TokenType::RightBrace)
        {
            node->m_sentenceList.append(this->parseSentence());
        }
        Q_ASSERT(m_pCurrentToken->m_type == TokenType::RightBrace);
        this->getNextToken();
        m_scope.leaveScope();
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
    else if(this->isValidType(m_pCurrentToken->m_type)) //变量声明
    {
        GType* baseType = this->parseDeclarationSpec();
        GDeclarationNode* node = new GDeclarationNode();
        int i = 0;
        while(m_pCurrentToken->m_type != TokenType::Semicolon)
        {
            if(i > 0)
            {
                this->getNextToken();
            }

            GToken* pToken;
            GType* pType = this->parseDeclarator(baseType, pToken);
            GVariable* var = this->createVariable(pToken->m_context, pType);
            i++;
            if(m_pCurrentToken->m_type == TokenType::Assign)
            {
                GVariableNode* varNode = new GVariableNode();
                varNode->m_pVar = var;
                GAssignNode* assignNode = new GAssignNode();
                this->getNextToken();
                assignNode->m_pLeftNode = varNode;
                assignNode->m_pRightNode = this->parseExpression();
                node->m_assignList.append(assignNode);
            }
        }

        Q_ASSERT(m_pCurrentToken->m_type == TokenType::Semicolon);
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

//    qDebug()<<m_pCurrentToken->m_context;
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

GSyntaxNode* GParser::parseExpressionBracket()
{
    GCalculateType calType;
    GSyntaxNode* left = this->parseConstant();
    while(true)
    {
        if(m_pCurrentToken->m_type == TokenType::LeftBracket)
        {
            this->getNextToken();
            left->calculateType(&calType);
            GSyntaxNode* right = this->parseExpression();
            right->calculateType(&calType);

            if(left->m_pType->isSameTypeKind(Kind_BuildIn) && right->m_pType->isSameTypeKind(Kind_Array))
            {
                GBuildInType* buildInType = dynamic_cast<GBuildInType*>(left->m_pType);
                if(buildInType->isIntegerKind())
                {
                    GSyntaxNode* temp = left;
                    left = right;
                    right = temp;
                }
            }

            GBinaryNode* binNode = new GBinaryNode();
            binNode->m_binOp = BinaryOperator::OP_PtrAdd;
            binNode->m_pLeftNode = left;
            binNode->m_pRightNode = right;

            GUnaryNode* starNode = new GUnaryNode();
            starNode->m_uOp = UnaryOperator::OP_Star;
            starNode->m_pNode = binNode;
            Q_ASSERT(m_pCurrentToken->m_type == TokenType::RightBracket);
            this->getNextToken();
            left = starNode;
            continue;
        }
        else if(m_pCurrentToken->m_type == TokenType::Dot)
        {
            this->getNextToken();
            GSyntaxNode* pNode = this->parseStructAccessNode(left);
            left = pNode;
            continue;
        }
        else if(m_pCurrentToken->m_type == TokenType::PointerTo)
        {
            this->getNextToken();
            GUnaryNode* pStarNode = new GUnaryNode();
            pStarNode->m_uOp = UnaryOperator::OP_Star;
            pStarNode->m_pNode = left;
            GSyntaxNode* sNode = this->parseStructAccessNode(pStarNode);
            left = sNode;
            continue;
        }
        else
        {
            break;
        }
    }
    return left;
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
        if(m_pCurrentToken->m_type == TokenType::Plus || m_pCurrentToken->m_type == TokenType::Minus)
        {
            GToken* currentToken = m_pCurrentToken;
            this->getNextToken();
            GSyntaxNode* right = this->parseExpressionMul();

            GCalculateType calculate;
            left->calculateType(&calculate);
            right->calculateType(&calculate);

            BinaryOperator binOp = BinaryOperator::OP_NULL;
            if(currentToken->m_type == TokenType::Plus)
            {
                if((left->m_pType->isSameTypeKind(Kind_Pointer) || left->m_pType->isSameTypeKind(Kind_Array)) && right->m_pType->isSameTypeKind(Kind_BuildIn))
                {
                    GBuildInType* pType = dynamic_cast<GBuildInType*>(right->m_pType);
                    if(pType->isIntegerKind())
                    {
                        binOp = BinaryOperator::OP_PtrAdd;
                    }
                }
                else if(left->m_pType->isSameTypeKind(Kind_BuildIn) && (right->m_pType->isSameTypeKind(Kind_Pointer) || right->m_pType->isSameTypeKind(Kind_Array)))
                {
                    GBuildInType* pType = dynamic_cast<GBuildInType*>(left->m_pType);
                    {
                        if(pType->isIntegerKind())
                        {
                            GSyntaxNode* temp = left;
                            left = right;
                            right = temp;
                            binOp = BinaryOperator::OP_PtrAdd;
                        }
                    }
                }
                else if(left->m_pType->isSameTypeKind(Kind_BuildIn) && right->m_pType->isSameTypeKind(Kind_BuildIn))
                {
                    GBuildInType* lType = dynamic_cast<GBuildInType*>(left->m_pType);
                    GBuildInType* rType = dynamic_cast<GBuildInType*>(right->m_pType);
                    if(lType->isIntegerKind() && rType->isIntegerKind())
                    {
                        binOp = BinaryOperator::OP_Add;
                    }
                }
            }
            else if(currentToken->m_type == TokenType::Minus)
            {
                if((left->m_pType->isSameTypeKind(Kind_Pointer) || left->m_pType->isSameTypeKind(Kind_Array)) && right->m_pType->isSameTypeKind(Kind_BuildIn))
                {
                    GBuildInType* pType = dynamic_cast<GBuildInType*>(right->m_pType);
                    if(pType->isIntegerKind())
                    {
                        binOp = BinaryOperator::OP_PtrSub;
                    }
                }
                else if((left->m_pType->isSameTypeKind(Kind_Pointer) || left->m_pType->isSameTypeKind(Kind_Array)) && (right->m_pType->isSameTypeKind(Kind_Pointer) || right->m_pType->isSameTypeKind(Kind_Array)))
                {
                    binOp = BinaryOperator::OP_PtrDiff;
                }
                else if(left->m_pType->isSameTypeKind(Kind_BuildIn) && right->m_pType->isSameTypeKind(Kind_BuildIn))
                {
                    GBuildInType* lType = dynamic_cast<GBuildInType*>(left->m_pType);
                    GBuildInType* rType = dynamic_cast<GBuildInType*>(right->m_pType);
                    if(lType->isIntegerKind() && rType->isIntegerKind())
                    {
                        binOp = BinaryOperator::OP_Sub;
                    }
                }
            }

            Q_ASSERT(binOp != BinaryOperator::OP_NULL);
            GBinaryNode* node = new GBinaryNode();
            node->m_pToken = currentToken;
            node->m_binOp = binOp;
            node->m_pLeftNode = left;
            node->m_pRightNode = right;
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
    GSyntaxNode* left = this->parseExpressionUnary();
    while(m_pCurrentToken)
    {
        if(m_pCurrentToken->m_type == TokenType::Star || m_pCurrentToken->m_type == TokenType::Slash)
        {
            BinaryOperator binOp = BinaryOperator::OP_Div;
            if(m_pCurrentToken->m_type == TokenType::Star)
            {
                binOp = BinaryOperator::OP_Mul;
            }

            GBinaryNode* node = new GBinaryNode();
            node->m_pToken = m_pCurrentToken;
            this->getNextToken();
            node->m_binOp = binOp;
            node->m_pLeftNode = left;
            node->m_pRightNode = this->parseExpressionUnary();
            left = node;
        }
        else
        {
            break;
        }
    }
    return left;
}

GSyntaxNode* GParser::parseExpressionUnary()
{
    if(m_pCurrentToken->m_type == TokenType::Plus ||
       m_pCurrentToken->m_type == TokenType::Minus ||
       m_pCurrentToken->m_type == TokenType::Star ||
       m_pCurrentToken->m_type == TokenType::Amp)
    {
        GUnaryNode* node = new GUnaryNode();
        node->m_pToken = m_pCurrentToken;
        node->m_uOp = UnaryOperator::OP_Plus;
        if(m_pCurrentToken->m_type  == TokenType::Minus)
        {
            node->m_uOp = UnaryOperator::OP_Minus;
        }
        else if(m_pCurrentToken->m_type == TokenType::Star)
        {
            node->m_uOp = UnaryOperator::OP_Star;
        }
        else if(m_pCurrentToken->m_type == TokenType::Amp)
        {
            node->m_uOp = UnaryOperator::OP_Amp;
        }
        this->getNextToken();
        node->m_pNode = this->parseExpressionUnary();
        return node;
    }

//    return this->parseConstant();
    return this->parseExpressionBracket();
}

GSyntaxNode* GParser::parseStructAccessNode(GSyntaxNode* left)
{
    GCalculateType calType;
    left->calculateType(&calType);
//qDebug()<<left->m_pType->m_type;
    Q_ASSERT(left->m_pType->isSameTypeKind(Kind_StructUnion));

    GStructType* pStructType = dynamic_cast<GStructType*>(left->m_pType);
    GFiled* pFiled = NULL;
    foreach(GFiled* filed, pStructType->m_fileds)
    {
        if(m_pCurrentToken->m_context == filed->m_pToken->m_context)
        {
            pFiled = filed;
            break;
        }
    }

    Q_ASSERT(pFiled != NULL);
    GStructNode* pNode = new GStructNode();
    pNode->m_pNode = left;
    pNode->m_pFiled = pFiled;
    Q_ASSERT(m_pCurrentToken->m_type == TokenType::Identifier);
    this->getNextToken();
    return pNode;
}

GSyntaxNode* GParser::parseConstant()
{
    if(m_pCurrentToken->m_type == TokenType::LeftParent)
    {
        this->getNextToken();
        if(m_pCurrentToken->m_type == TokenType::LeftBrace)
        {
            m_scope.enterScope();
            this->getNextToken();
            GExpressionSentenceNode* node = new GExpressionSentenceNode();
            while(m_pCurrentToken->m_type != TokenType::RightBrace)
            {
                node->m_sentenceList.append(this->parseSentence());
            }
            this->getNextToken();
            Q_ASSERT(m_pCurrentToken->m_type == TokenType::RightParent);
            this->getNextToken();
            m_scope.leaveScope();
            return node;
        }

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

        GVariable* var = m_scope.findVar(m_pCurrentToken->m_context);
        Q_ASSERT(var);
        GVariableNode* node = new GVariableNode();
        node->m_pToken = m_pCurrentToken;
        node->m_pVar = var;
        this->getNextToken();
        return node;
    }
    else if(m_pCurrentToken->m_type == TokenType::SizeOf)
    {
        GSizeofNode* node = new GSizeofNode();
        this->getNextToken();
        node->m_pNode = this->parseExpressionUnary();
        return node;
    }

    GConstantNode* node = new GConstantNode();
    node->m_pToken = m_pCurrentToken;    
//qDebug()<<"---------->"<<m_pCurrentToken->m_context;
    node->m_value = m_pCurrentToken->m_context.toLong();
    this->getNextToken();
    return node;
}

GType* GParser::parseDeclarationSpec()
{
    if(m_pCurrentToken->m_type == TokenType::Char)
    {
        this->getNextToken();
        return GBuildInType::m_charType;
    }
    else if(m_pCurrentToken->m_type == TokenType::Short)
    {
        this->getNextToken();
        return GBuildInType::m_shortType;
    }
    else if(m_pCurrentToken->m_type == TokenType::Int)
    {
        this->getNextToken();
        return GBuildInType::m_intType;
    }
    else if(m_pCurrentToken->m_type == TokenType::Long)
    {
        this->getNextToken();
        return GBuildInType::m_longType;
    }
    else if(m_pCurrentToken->m_type == TokenType::Struct)
    {
        this->getNextToken();
        return this->ParseStructDeclaration();
    }
    else if(m_pCurrentToken->m_type == TokenType::Union)
    {
        this->getNextToken();
        return this->ParseUnionDeclaration();
    }
    return NULL;
}

GType* GParser::parseDeclarator(GType* baseType, GToken* &pToken)
{
    GType* pType = baseType;
    while(m_pCurrentToken->m_type == TokenType::Star)
    {
        pType = new GPointerType(pType);
        this->getNextToken();
    }

//qDebug()<<m_pCurrentToken->m_context;
    Q_ASSERT(m_pCurrentToken->m_type == TokenType::Identifier);
    pToken = m_pCurrentToken; //第一个变量 或者函数名
    this->getNextToken();
    pType = parseTypeSuffix(pType);
    return pType;
}

GType* GParser::parseTypeSuffix(GType* pType)
{
    if(m_pCurrentToken->m_type == TokenType::LeftParent) //是个函数
    {
        m_scope.enterScope();
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
        m_scope.leaveScope();
        return pFunType;
    }
    else if(m_pCurrentToken->m_type == TokenType::LeftBracket)
    {
        this->getNextToken();
        Q_ASSERT(m_pCurrentToken->m_type == TokenType::Num);
        int num = m_pCurrentToken->m_context.toLong();
        this->getNextToken();
        Q_ASSERT(m_pCurrentToken->m_type == TokenType::RightBracket);
        this->getNextToken();
        GType* arrayType = this->parseTypeSuffix(pType);
        return new GArrayType(arrayType, num);
    }

    return pType;
}

GType* GParser::ParseStructDeclaration()
{
    GStructType* pType = this->ParseStructOrUnionDeclaration(true);
    int offset = 0;
    foreach(GFiled* filed, pType->m_fileds)
    {
        offset = this->alignTo(offset, filed->m_pType->m_align);
        filed->m_offset = offset;
        offset += filed->m_pType->m_size;

        if(pType->m_align < filed->m_pType->m_align)
        {
            pType->m_align = filed->m_pType->m_align;
        }
    }
    pType->m_size = this->alignTo(offset, pType->m_align);
    return pType;
}

GType* GParser::ParseUnionDeclaration()
{
    GStructType* pType = this->ParseStructOrUnionDeclaration(false);
    foreach(GFiled* filed, pType->m_fileds)
    {
        if(pType->m_size < filed->m_pType->m_size)
        {
            pType->m_size = filed->m_pType->m_size;
        }

        if(pType->m_align < filed->m_pType->m_align)
        {
            pType->m_align = filed->m_pType->m_align;
        }
    }
    return pType;
}

GStructType* GParser::ParseStructOrUnionDeclaration(bool isStruct)
{
    StructKind sKind = StructKind::Kind_Struct;
    if(isStruct == false) sKind = StructKind::Kind_Union;

    GToken* pToken = NULL;
    if(m_pCurrentToken->m_type == TokenType::Identifier)
    {
        pToken = m_pCurrentToken;
        this->getNextToken();
    }

    if(pToken && m_pCurrentToken->m_type != TokenType::LeftBrace)
    {
        GType* pType = m_scope.findTag(pToken->m_context);
        Q_ASSERT(pType);
        return dynamic_cast<GStructType*>(pType);
    }

    GStructType* pStructType = new GStructType(sKind);
    this->getNextToken();
    while( m_pCurrentToken->m_type != TokenType::RightBrace )
    {
        GType* pBaseType = this->parseDeclarationSpec();
        GToken* pToken;
        GType* pType = this->parseDeclarator(pBaseType,pToken);
        pStructType->m_fileds.append( new GFiled(pType, pToken, 0) );

        while( m_pCurrentToken->m_type == TokenType::Comma)
        {
            this->getNextToken();
            pType = this->parseDeclarator(pBaseType, pToken);
            pStructType->m_fileds.append( new GFiled(pType, pToken, 0) );
        }

        Q_ASSERT(m_pCurrentToken->m_type == TokenType::Semicolon);
        this->getNextToken();
    }
    Q_ASSERT(m_pCurrentToken->m_type == TokenType::RightBrace);
    this->getNextToken();

    if(pToken)
    {
        m_scope.pushTag(pToken->m_context, pStructType);
    }

    return pStructType;
}

bool GParser::isValidType(TokenType type)
{
    if(type == TokenType::Char) return true;
    if(type == TokenType::Short) return true;
    if(type == TokenType::Int) return true;
    if(type == TokenType::Long) return true;
    if(type == TokenType::Struct) return true;
    if(type == TokenType::Union) return true;
    return false;
}

GVariable* GParser::createVariable(const QString& name, GType* pType)
{
    GVariable* var = new GVariable(name, pType);
    m_pLocals->append(var);
//    m_pLocals->prepend(var);
    m_scope.pushVar(name, var);
    return var;
}

void GParser::getNextToken()
{
    if(m_tokenIndex < m_lexer.m_tokenList.size())
    {
        m_pCurrentToken = m_lexer.m_tokenList.at(m_tokenIndex);
        m_tokenIndex++;
//qDebug()<<"============>"<<m_pCurrentToken->m_context;
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

int GParser::alignTo(int size, int align)
{
    return (size + align - 1) / align * align;
}
