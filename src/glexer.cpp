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
    while(true)
    {
        if(m_cursor > m_size)
        {
            break;
        }

        if(nextToken() == false)
        {
            return false;
        }
    }

    return true;
}

bool GLexer::nextToken()
{
    bool isSpaceOrNewLine = true;
    while(isSpaceOrNewLine)
    {
        if(m_currentChar.isSpace())
        {
            this->nextChar();
        }
        else if(m_currentChar == "\n")
        {
            m_line++;
            m_lineCursor = m_cursor;
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
    }
    else if(m_currentChar == '+')
    {
        token->m_type = TokenType::Add;
        nextChar();
    }
    else if(m_currentChar == '-')
    {
        token->m_type = TokenType::Sub;
        nextChar();
    }
    else if(m_currentChar == '*')
    {
        token->m_type = TokenType::Mul;
        nextChar();
    }
    else if(m_currentChar == '/')
    {
        token->m_type = TokenType::Div;
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
    else
    {
        qDebug()<<"unknow token type at "<<(*token);
        return false;
    }

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
    for(int i=0; i<m_tokenList.size(); ++i)
    {
        qDebug()<<(*m_tokenList.at(i));
    }
}
