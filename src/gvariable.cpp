#include "gvariable.h"

GVariable::GVariable():m_name(""),m_pType(NULL), m_address(0)
{
}

GVariable::GVariable(const QString& name, GType* pType):m_name(name),m_pType(pType),m_address(0)
{

}
