#ifndef GTYPE_H
#define GTYPE_H

#include <QString>
#include "src/gtoken.h"

enum TypeKind
{
    Kind_BuildIn,
    Kind_Function,
    Kind_Pointer,
    Kind_Array,
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
    bool isSameTypeKind(TypeKind kind);

public:
    int m_size;
    int m_align;

//private:
    TypeKind m_type;
};

class GBuildInType : public GType
{
public:
    static GBuildInType* m_intType;
public:
    GBuildInType(BuildInKind bKind, int size, int align):GType(TypeKind::Kind_BuildIn, size, align),m_buildInKind(bKind){}
    BuildInKind getBuildInKind() const {return m_buildInKind;}
    bool isSameBuildInKind(BuildInKind kind);
private:
    BuildInKind m_buildInKind;
};

class GPointerType : public GType
{
public:
    GPointerType(GType* baseType) : GType(TypeKind::Kind_Pointer, 8, 8),m_baseType(baseType){}
public:
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

class GArrayType : public GType
{
public:
    GArrayType(GType* baseType, int length) : GType(TypeKind::Kind_Array, length*baseType->m_size, baseType->m_align), m_elementType(baseType), m_length(length){}
public:
    int m_length;
    GType* m_elementType;
};


#endif // GTYPE_H
