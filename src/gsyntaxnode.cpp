#include "gsyntaxnode.h"
#include <QDebug>

void GSyntaxNode::traversal()
{
    if(m_pLeftNode) m_pLeftNode->traversal();
    if(m_pRightNode) m_pRightNode->traversal();
    if(m_pToken) m_pToken->print();
}
