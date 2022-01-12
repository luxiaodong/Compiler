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

private:
    bool isSpace(const QChar& c);

private:
    QString m_source;
    const QChar* m_data;
    QList<GToken*> m_tokenList;
    QChar m_currentChar;
    int m_line;
    int m_lineCursor; //行的首字符位置.
    int m_cursor; //整个字符串的位置.
    int m_size;
};

#endif // GLEXER_H
