#include "glexer.h"
#include <QDebug>

GLexer::GLexer(const QString& code) : m_source(code) , m_line(0), m_lineCursor(0), m_cursor(0)
{
    m_tokenList.clear();
    m_size = m_source.size();
}

bool GLexer::analysis()
{
    this->nextChar();
    while(nextToken());

    if(m_currentChar == '\0')
    {
        return true;
    }

    return false;
}

bool GLexer::nextToken()
{
    bool isSpaceOrNewLine = true;
    while(isSpaceOrNewLine)
    {
        if(m_currentChar == '\n')
        {
            m_line++;
            m_lineCursor = m_cursor;
            this->nextChar();
        }
        else if(m_currentChar.isSpace())
        {
            this->nextChar();
        }
        else
        {
            isSpaceOrNewLine = false;
        }
    }

    GToken *token = new GToken();
    token->m_position = QPoint(m_cursor - 1 - m_lineCursor, m_line);
    token->m_context = QString(m_currentChar);

    if(m_currentChar == '\0')
    {
        token->m_type = TokenType::Eof;
        m_tokenList.append(token);
        return false;
    }
    else if(m_currentChar == '+')
    {
        token->m_type = TokenType::Plus;
        nextChar();
    }
    else if(m_currentChar == '-')
    {
        token->m_type = TokenType::Minus;
        nextChar();
    }
    else if(m_currentChar == '*')
    {
        token->m_type = TokenType::Star;
        nextChar();
    }
    else if(m_currentChar == '/')
    {
        token->m_type = TokenType::Slash;
        nextChar();

        if(m_currentChar == '/') //注释
        {
            delete token;
            while(true)
            {
                nextChar();
                if(m_currentChar == '\n')
                {
                    return true;
                }

                if(m_currentChar == '\0')
                {
                    return false;
                }
            }
        }
        else if(m_currentChar == '*')
        {
            delete token;
            while(true)
            {
                nextChar();
                if(m_currentChar == '*')
                {
                    nextChar();
                    if(m_currentChar == '/')
                    {
                        nextChar();
                        return true;
                    }
                }

                if(m_currentChar == '\n')
                {
                    m_line++;
                    m_lineCursor = m_cursor;
                }

                if(m_currentChar == '\0')
                {
                    return false;
                }
            }
        }
    }
    else if(m_currentChar == '(')
    {
        token->m_type = TokenType::LeftParent;
        nextChar();
    }
    else if(m_currentChar == ')')
    {
        token->m_type = TokenType::RightParent;
        nextChar();
    }
    else if(m_currentChar == '{')
    {
        token->m_type = TokenType::LeftBrace;
        nextChar();
    }
    else if(m_currentChar == '}')
    {
        token->m_type = TokenType::RightBrace;
        nextChar();
    }
    else if(m_currentChar == '=')
    {
        token->m_type = TokenType::Assign;
        nextChar();
        if(m_currentChar == '=')
        {
            token->m_type = TokenType::Equal;
            token->m_context += QString(m_currentChar);
            nextChar();
        }
    }
    else if(m_currentChar == '!')
    {
        nextChar();
        if(m_currentChar == '=')
        {
            token->m_type = TokenType::NotEqual;
            token->m_context += QString(m_currentChar);
            nextChar();
        }
        else
        {
            QString msg = QString("line %1, row %2").arg(token->m_position.y()).arg(token->m_position.x());
            Q_ASSERT_X(false, msg.toUtf8(), "unsupport operator !");
        }
    }
    else if(m_currentChar == '>')
    {
        token->m_type = TokenType::Greater;
        nextChar();
        if(m_currentChar == '=')
        {
            token->m_type = TokenType::GreaterEqual;
            token->m_context += QString(m_currentChar);
            nextChar();
        }
    }
    else if(m_currentChar == '<')
    {
        token->m_type = TokenType::Lesser;
        nextChar();
        if(m_currentChar == '=')
        {
            token->m_type = TokenType::LesserEqual;
            token->m_context += QString(m_currentChar);
            nextChar();
        }
    }
    else if(m_currentChar == ';')
    {
        token->m_type = TokenType::Semicolon;
        nextChar();
    }
    else if(m_currentChar == ',')
    {
        token->m_type = TokenType::Comma;
        nextChar();
    }
    else if(m_currentChar == '&')
    {
        token->m_type = TokenType::Amp;
        nextChar();
    }
    else if(m_currentChar.isDigit())
    {
        token->m_type = TokenType::Num;
        int pos = m_cursor - 1;
        do{
            nextChar();
        }while(m_currentChar.isDigit());
        token->m_context = m_source.mid(pos, m_cursor - pos -1);
    }
    else if(m_currentChar.isLetter())
    {
        token->m_type = TokenType::Identifier;
        int pos = m_cursor - 1;
        do{
            nextChar();
        }while(m_currentChar.isLetterOrNumber());
        token->m_context = m_source.mid(pos, m_cursor - pos - 1);

        if(token->m_context == QString("if"))
        {
            token->m_type = TokenType::If;
        }
        else if(token->m_context == QString("else"))
        {
            token->m_type = TokenType::Else;
        }
        else if(token->m_context == QString("while"))
        {
            token->m_type = TokenType::While;
        }
        else if(token->m_context == QString("do"))
        {
            token->m_type = TokenType::Do;
        }
        else if(token->m_context == QString("for"))
        {
            token->m_type = TokenType::For;
        }
        else if(token->m_context == QString("return"))
        {
            token->m_type = TokenType::Return;
        }
        else if(token->m_context == QString("int"))
        {
            token->m_type = TokenType::Int;
        }
        else if(token->m_context == QString("sizeof"))
        {
            token->m_type = TokenType::SizeOf;
        }
    }
    else
    {
        qDebug()<<"unknow token type at "<<(*token);
        return false;
    }

//    qDebug()<<token->m_context;
    m_tokenList.append(token);
    return true;
}

void GLexer::nextChar()
{
    if(m_cursor == m_size)
    {
        m_currentChar = '\0';
        m_cursor++;
    }
    else
    {
        m_currentChar = m_source.at(m_cursor);
        m_cursor++;
    }
}

void GLexer::printTokens()
{
    qDebug()<<"=========token list================";
    for(int i=0; i<m_tokenList.size(); ++i)
    {
        qDebug()<<(*m_tokenList.at(i));
    }
}

bool GLexer::isKeyword(const QString& str) const
{
    static QStringList keys = (QStringList()<<"auto"<<"double"<<"int"<<"struct"<<"break"<<"else"<<"long"<<"switch"<<"case"<<"enum"<<"register"<<"typedef"<<"char"<<"extern"<<"return"<<"union"<<"const"<<"float"<<"short"<<"unsigned"<<"for"<<"signed"<<"void"<<"default"<<"goto"<<"sizeof"<<"volatile"<<"do"<<"if"<<"static"<<"while"<<"continue");
    if (keys.indexOf(str) == -1) return false;
    return true;
}

