#ifndef GVARIABLE_H
#define GVARIABLE_H

#include <QString>
#include "src/gtype.h"

class GVariable
{
public:
    GVariable();

public:
    GType* m_pType;
    QString m_name;
    int m_address;
};

#endif // GVARIABLE_H
