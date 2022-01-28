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
    Kind_StructUnion,
};

enum BuildInKind
{
    Kind_Char,
    Kind_Short,
    Kind_Int,
    Kind_Long,
};

enum StructKind
{
    Kind_Struct,
    Kind_Union,
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
    static GBuildInType* m_charType;
    static GBuildInType* m_shortType;
    static GBuildInType* m_intType;
    static GBuildInType* m_longType;

public:
    GBuildInType(BuildInKind bKind, int size, int align):GType(TypeKind::Kind_BuildIn, size, align),m_buildInKind(bKind){}
    BuildInKind getBuildInKind() const {return m_buildInKind;}
    bool isSameBuildInKind(BuildInKind kind);
    bool isIntegerKind();

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

class GFiled
{
public:
    GFiled(GType* pType, GToken* pToken, int offset) : m_pType(pType), m_pToken(pToken), m_offset(offset){}
    GType* m_pType;
    GToken* m_pToken;
    int m_offset;
};

class GStructType : public GType
{
public:
    GStructType(StructKind sKind) : GType(Kind_StructUnion, 0, 1), m_structKind(sKind){}
    bool isSameKind(StructKind sKind){return m_structKind = sKind;}
    StructKind m_structKind;
    QList<GFiled*> m_fileds;
};

#endif // GTYPE_H
