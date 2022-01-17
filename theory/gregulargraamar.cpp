#include "gregulargraamar.h"

GRegularGraamar::GRegularGraamar()
{

}

void GRegularGraamar::test()
{
    QStringList list;
    list<<QString("letter_->a|b|c|d|e|f|g|h");
    list<<QString("letter_(dight | letter_)*");
    // 需要根据正则表达式, 构造NFA.
//    this->print();
}

// 正则表示
// letter_ -> a-z|A-Z|_
// dight   -> 0-9
// id      -> letter_(dight | letter_)*

bool GRegularGraamar::isIdentifier(const QString& ) const
{
    return false;
}

bool GRegularGraamar::isKeyword(const QString& str) const
{
    static QStringList keys = (QStringList()<<"auto"<<"double"<<"int"<<"struct"<<"break"<<"else"<<"long"<<"switch"<<"case"<<"enum"<<"register"<<"typedef"<<"char"<<"extern"<<"return"<<"union"<<"const"<<"float"<<"short"<<"unsigned"<<"for"<<"signed"<<"void"<<"default"<<"goto"<<"sizeof"<<"volatile"<<"do"<<"if"<<"static"<<"while"<<"continue");
    if (keys.indexOf(str) == -1) return false;
    return true;
}
