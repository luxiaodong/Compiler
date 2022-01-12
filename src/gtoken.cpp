#include "gtoken.h"

GToken::GToken()
{

}

QDebug operator <<(QDebug debug, const GToken& token)
{
    debug.nospace() << token.m_context << " at position " << token.m_position;
    return debug.maybeSpace();
}
