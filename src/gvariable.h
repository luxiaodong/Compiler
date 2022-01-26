#ifndef GVARIABLE_H
#define GVARIABLE_H

#include <QString>
#include "src/gtype.h"

class GVariable
{
public:
    GVariable();
    GVariable(const QString& name, GType* pType);

public:
    GType* m_pType;
    const QString m_name;
    int m_address;
};

#endif // GVARIABLE_H
