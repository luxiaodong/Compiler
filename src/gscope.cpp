#include "gscope.h"

GScope::GScope()
{
}

void GScope::enterScope()
{
    m_stack.push( new GScopeItem());
}

void GScope::leaveScope()
{
    GScopeItem* item = m_stack.pop();
    delete item;
}

void GScope::pushVar(const QString& name, GVariable* var)
{
    GScopeItem* top = m_stack.top();
    top->m_vars.insert(name, var);
}

GVariable* GScope::findVar(const QString& name)
{
    for(int j = m_stack.size() - 1; j >=0; j--)
    {
        GScopeItem* item = m_stack.at(j);
        GVariable* var = item->m_vars.value(name, NULL);
        if(var) return var;
    }

    return NULL;
}

GVariable* GScope::findVarInCurrentScope(const QString& name)
{
    GScopeItem* top = m_stack.top();
    GVariable* var = top->m_vars.value(name, NULL);
    return var;
}

void GScope::pushTag(const QString& name, GType* tag)
{
    GScopeItem* top = m_stack.top();
    top->m_tags.insert(name, tag);
}

GType* GScope::findTag(const QString& name)
{
    for(int j = m_stack.size() - 1; j >=0; j--)
    {
        GScopeItem* item = m_stack.at(j);
        GType* tag = item->m_tags.value(name, NULL);
        if(tag) return tag;
    }

    return NULL;
}

GType* GScope::findTagInCurrentScope(const QString& name)
{
    GScopeItem* top = m_stack.top();
    GType* tag = top->m_tags.value(name, NULL);
    return tag;
}

bool GScope::isEmpty()
{
    return m_stack.size() == 0;
}

