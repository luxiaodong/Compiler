#include "gcompiler.h"
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QDir>
#include "src/ggeneratecode.h"
#include "src/gcalculatetype.h"
#include "theory/ggrammarll1.h"
#include "theory/ggrammarlr0.h"

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
    GCalculateType calType;
    parser.m_pTree->m_pRoot->calculateType(&calType);
    parser.m_pTree->m_pRoot->generateCode(&genCode);
    genCode.printCode();
}

void GCompiler::test()
{
    QString filePath = QDir::currentPath() + QString("/../Compiler/test/test.c");
    this->readFile(filePath);

//    GGrammarLL1 parser;
//    GGrammarLR0 parser;
//    parser.test();
}

