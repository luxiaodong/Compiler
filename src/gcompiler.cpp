#include "gcompiler.h"
#include <QTextStream>
#include <QFile>
#include <QDebug>

#include "src/ggeneratecode.h"
#include "theory/gcontextfreegrammar.h"

GCompiler::GCompiler()
{
}

void GCompiler::readFile(const QString& filePath)
{
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString text = stream.readAll();
        this->readText(text);
        file.close();
    }
}

void GCompiler::readText(const QString& text)
{
    qDebug()<<text;

    GLexer lexer(text);
    if(lexer.analysis() == false) return;
    lexer.printTokens();

    GParser parser(lexer);
    if(parser.analysis() == false) return;

    GGenerateCode genCode;
    parser.m_pTree->m_pRoot->generateCode(&genCode);
    genCode.printCode();
}

void GCompiler::test()
{
//    this->readText("2*3");
//    this->readText("5+1-3*4/2");
//    this->readText("5+(1-3)*4/2");
//    this->readText("5+a1+(1-3)*4/2");

    GContextFreeGrammar parser;
    parser.test();
}

