#ifndef GLEXER_H
#define GLEXER_H

#include <QChar>
#include <QString>
#include <QStringList>
#include <QList>
#include "src/gtoken.h"

class GLexer
{
public:
    GLexer(const QString& code);
    bool analysis();
    void printTokens();

private:
    void nextChar();
    bool nextToken();
    bool isKeyword(const QString& str) const;
    bool isIdentifier(const QChar& str) const;

public:
    QList<GToken*> m_tokenList;

private:
    QString m_source;
    QChar m_currentChar;
    int m_line;
    int m_lineCursor; //行的首字符位置.
    int m_cursor; //整个字符串的位置.
    int m_size;
};

#endif // GLEXER_H
