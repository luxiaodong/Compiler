#include "gtype.h"

GBuildInType* GBuildInType::m_intType = new GBuildInType(BuildInKind::Kind_Int, 8, 8);

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
