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
    qDebug()<<"test string is :"<<text;

    GLexer lexer(text);
    if(lexer.analysis() == false)
    {
        qDebug()<<"lexer error.";
        return ;
    }
    lexer.printTokens();


    GParser parser(lexer);
    if(parser.analysis() == false)
    {
        qDebug()<<"parser error.";
        return ;
    }

    GGenerateCode genCode;
    parser.m_pTree->m_pRoot->generateCode(&genCode);
    genCode.printCode();
}

void GCompiler::test()
{
//    this->readText("2*3");
//    this->readText("5+1-3*4/2");
//    this->readText("5+(1-3)*4/2");
//    this->readText("a=1;2+a;");
//    this->readText("a=3;5+a*4/2-a;");
    this->readText("a=3;b=4;a*b*2;");

    GContextFreeGrammar parser;
    parser.test();
}

