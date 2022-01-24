#include "gsymboltable.h"

QList<GVariable*> GSymbolTable::m_variables;

void GSymbolTable::addVariable(QString name, GType* pType)
{
    if(name.isEmpty()) return ;
    foreach(GVariable* var, m_variables)
    {
        if(var->m_name == name) return ;
    }

    GVariable* var = new GVariable();
    var->m_name = name;
    var->m_pType = pType;
    var->m_address = 0;
//    m_variables.append(var);
    m_variables.prepend(var);
}

GVariable* GSymbolTable::getVariable(QString name)
{
    foreach(GVariable* var, m_variables)
    {
        if(var->m_name == name) return var;
    }

    return NULL;
}

int GSymbolTable::getAddress(QString name)
{
    foreach(GVariable* var, m_variables)
    {
        if(var->m_name == name) return var->m_address;
    }

    return 0;
}

void GSymbolTable::printTable()
{
    foreach(GVariable* var, GSymbolTable::m_variables)
    {
        qDebug()<<var->m_name<<"("<<var->m_pType->m_type<<")"<<"\t"<<var->m_address;
    }
}
