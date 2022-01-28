#ifndef GSCOPE_H
#define GSCOPE_H

#include <QStack>
#include <QList>
#include <QMap>
#include "src/gvariable.h"
#include "src/gtype.h"

class GScopeItem
{
public:
    QMap<QString, GVariable*> m_vars;
    QMap<QString, GType*> m_tags;
};

class GScope
{
public:
    GScope();
    void enterScope();
    void leaveScope();

    void pushVar(const QString& name, GVariable* var);
    GVariable* findVar(const QString& name);
    GVariable* findVarInCurrentScope(const QString& name);

    void pushTag(const QString& name, GType* tag);
    GType* findTag(const QString& name);
    GType* findTagInCurrentScope(const QString& name);

    bool isEmpty();

private:
    QStack<GScopeItem*> m_stack;
};

#endif // GSCOPE_H
