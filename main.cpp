#include <QCoreApplication>
#include "src/gcompiler.h"

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

//     QCoreApplication a(argc, argv);
     GCompiler compiler;
     compiler.test();
//     return a.exec();
}
