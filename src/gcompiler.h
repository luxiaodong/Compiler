#ifndef GCOMPILER_H
#define GCOMPILER_H

#include <QString>
#include "src/glexer.h"
#include "src/gparser.h"

class GCompiler
{
public:
    GCompiler();

public:
    void readFile(const QString& filePath);
    void readText(const QString& text);
    void test();
};

#endif // GCOMPILER_H
