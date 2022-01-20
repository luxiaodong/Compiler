#include "gcompiler.h"
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QDir>

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

//    GParser parser(lexer);
//    if(parser.analysis() == false)
//    {
//        qDebug()<<"parser error.";
//        return ;
//    }

//    GGenerateCode genCode;
//    parser.m_pTree->m_pRoot->generateCode(&genCode);
//    genCode.printCode();
}

void GCompiler::test()
{
//    this->readText("2*3");
//    this->readText("5+1-3*4/2");
//    this->readText("5+(1-3)*4/2");
//    this->readText("a=1;2+a;");
//    this->readText("a=3;5+a*4/2-a;");
//    this->readText("a=3;b=4;a*b*2;");
//    this->readText("a=b=3;a*b;");
//    this->readText("a=3;a==3;a!=3;a>3;a>=3;a<3;a<=3;");
//    this->readText("a=3;a<3;");
//    this->readText("a=3; if(a!=2) {b=1; if (b==1) b=2; else b=3; b*a;}");
//    this->readText(";");
//    this->readText("s=0;i=0;while(i<10){s=s+i;i=i+1;} s;");
//    this->readText("s=0;i=0;do{s=s+i;i=i+1;}while(i<11); s;");
//    this->readText("s=0;for(i=0;i<10;i=i+1) s=s+i; s;");

    QString filePath = QDir::currentPath() + QString("/../Compiler/calculate/prog.c");
    this->readFile(filePath);

//    GContextFreeGrammar parser;
//    parser.test();
}

