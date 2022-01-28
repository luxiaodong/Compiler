#include "gtype.h"

GBuildInType* GBuildInType::m_charType = new GBuildInType(BuildInKind::Kind_Char, 1, 1);
GBuildInType* GBuildInType::m_shortType = new GBuildInType(BuildInKind::Kind_Short, 2, 2);
GBuildInType* GBuildInType::m_intType = new GBuildInType(BuildInKind::Kind_Int, 4, 4);
GBuildInType* GBuildInType::m_longType = new GBuildInType(BuildInKind::Kind_Long, 8, 8);

bool GType::isSameTypeKind(TypeKind kind)
{
    if(m_type == kind) return true;
    return false;
}

bool GBuildInType::isSameBuildInKind(BuildInKind kind)
{
    if(m_buildInKind == kind) return true;
    return false;
}

bool GBuildInType::isIntegerKind()
{
    if(m_buildInKind == Kind_Char || m_buildInKind == Kind_Short || m_buildInKind == Kind_Int || m_buildInKind == Kind_Long)
    {
        return true;
    }

    return false;
}

