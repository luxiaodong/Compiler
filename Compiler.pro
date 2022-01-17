QT += core
QT -= gui

CONFIG += c++11

TARGET = Compiler
CONFIG -= console
CONFIG -= app_bundle
CONFIG += sdk_no_version_check

TEMPLATE = app

SOURCES += main.cpp \
    src/glexer.cpp \
    src/gcompiler.cpp \
    src/gparser.cpp \
    src/gsymboltable.cpp \
    src/gtoken.cpp \
    src/ggeneratecode.cpp \
    src/gsyntaxtree.cpp \
    src/gsyntaxnode.cpp \
    src/gvariable.cpp \
    theory/gunrestrictedgrammar.cpp \
    theory/gcontextsensitivegrammar.cpp \
    theory/gcontextfreegrammar.cpp \
    theory/gregulargraamar.cpp \
    theory/gproduction.cpp \
    theory/gfiniteautomata.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    src/glexer.h \
    src/gcompiler.h \
    src/gparser.h \
    src/gsymboltable.h \
    src/gtoken.h \
    src/ggeneratecode.h \
    src/gsyntaxtree.h \
    src/gsyntaxnode.h \
    src/gvariable.h \
    theory/gunrestrictedgrammar.h \
    theory/gcontextsensitivegrammar.h \
    theory/gcontextfreegrammar.h \
    theory/gregulargraamar.h \
    theory/gproduction.h \
    theory/gfiniteautomata.h
