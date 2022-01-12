#include "gcompiler.h"
#include <QTextStream>
#include <QFile>
#include <QDebug>

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
}

void GCompiler::test()
{
    this->readText("5+1-3*4/2");
}
