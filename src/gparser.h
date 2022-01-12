#ifndef GPARSER_H
#define GPARSER_H

#include "src/glexer.h"

class GParser
{
public:
    GParser();

private:
    GLexer* m_lexer;
};

#endif // GPARSER_H
