#ifndef GSYNTAXTREE_H
#define GSYNTAXTREE_H

#include <QStack>
#include "src/gsyntaxnode.h"

class GSyntaxTree
{
public:
    GSyntaxTree();

public:
    GSyntaxNode* m_pRoot;
};

#endif // GSYNTAXTREE_H
