#ifndef GTYPE_H
#define GTYPE_H

#include <QString>
#include "src/gtoken.h"

enum TypeKind
{
    Kind_BuildIn,
    Kind_Pointer,
    Kind_Function,
};

enum BuildInKind
{
    Kind_Int,
};

class GType
{
public:
    GType(TypeKind type, int size, int align):m_type(type),m_size(size),m_align(align){}
    virtual ~GType(){}

public:
    TypeKind m_type;
    int m_size;
    int m_align;
};

class GBuildInType : public GType
{
public:
    static GBuildInType* m_intType;
public:
    GBuildInType(BuildInKind bKind, int size, int align):GType(TypeKind::Kind_BuildIn, size, align),m_buildInKind(bKind){}
    BuildInKind getBuildInKind() const {return m_buildInKind;}
private:
    BuildInKind m_buildInKind;
};

class GPointerType : public GType
{
public:
    GPointerType(GType* baseType) : GType(TypeKind::Kind_Pointer, 8, 8),m_baseType(baseType){}
private:
    GType* m_baseType;
};

class GFunctionParam
{
public:
    GType* m_pType;
    GToken* m_pToken;
};

class GFunctionType : public GType
{
public:
    GFunctionType(GType* returnType) : GType(TypeKind::Kind_Function, 8, 8), m_pReturnType(returnType){}
public:
    QList<GFunctionParam*> m_paramList;
private:
    GType* m_pReturnType; //返回值类型
};

#endif // GTYPE_H
