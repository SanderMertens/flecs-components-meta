#ifndef FLECS_COMPONENTS_META_H
#define FLECS_COMPONENTS_META_H

/* This generated file contains includes for project dependencies */
#include "flecs-components-meta/bake_config.h"

////////////////////////////////////////////////////////////////////////////////
//// Utility macro's (do not use in code!)
////////////////////////////////////////////////////////////////////////////////

#define ECS_UNUSED __attribute__((unused))

#ifdef __cplusplus
#define ECS_ALIGNOF(T) alignof(T)
#else
#define ECS_ALIGNOF(T) ((size_t)&((struct { char c; T d; } *)0)->d)
#endif

#define ECS_ALIGN(size, alignment) (((((size) - 1) / (alignment)) + 1) * (alignment))

#define ECS_ENUM_BOOTSTRAP(name, ...)\
typedef enum name __VA_ARGS__ name;\
ECS_UNUSED \
static const char * __##name##__ = #__VA_ARGS__;

#define ECS_STRUCT_IMPL(name, descriptor, ...)\
typedef struct name __VA_ARGS__ name;\
ECS_UNUSED \
static EcsType __##name##__ = {EcsStructType, sizeof(name), ECS_ALIGNOF(name), descriptor};\

#define ECS_ENUM_IMPL(name, descriptor, ...)\
typedef enum name __VA_ARGS__ name;\
ECS_UNUSED \
static EcsType __##name##__ = {EcsEnumType, sizeof(name), ECS_ALIGNOF(name), descriptor};

#define ECS_BITMASK_IMPL(name, descriptor, ...)\
typedef enum name __VA_ARGS__ name;\
ECS_UNUSED \
static EcsType __##name##__ = {EcsBitmaskType, sizeof(name), ECS_ALIGNOF(name), descriptor};

#define ECS_STRUCT_C(T, ...) ECS_STRUCT_IMPL(T, #__VA_ARGS__, __VA_ARGS__)
#define ECS_ENUM_C(T, ...) ECS_ENUM_IMPL(T, #__VA_ARGS__, __VA_ARGS__)
#define ECS_BITMASK_C(T, ...) ECS_BITMASK_IMPL(T, #__VA_ARGS__, __VA_ARGS__)

// Unspecialized class (see below)
namespace flecs {
template <typename T>
class __meta__ { };    
}

// Specialized C++ class that stores name and descriptor of type
#define ECS_META_CPP(T, kind, descr);\
namespace flecs {\
template<>\
class __meta__ <T> {\
public:\
    static const char* name() {\
        return #T;\
    }\
    static EcsType descriptor() {\
        return (EcsType){kind, sizeof(T), ECS_ALIGNOF(T), descr};\
    }\
};\
}


////////////////////////////////////////////////////////////////////////////////
//// Use these macro's to define types.
////   The API will automatically select the right macro's for C or C++
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus

// C++ 

// Define a struct
#define ECS_STRUCT(T, ...)\
    ECS_STRUCT_IMPL(T, #__VA_ARGS__, __VA_ARGS__);\
    ECS_META_CPP(T, EcsStructType, #__VA_ARGS__);

// Define an enumeration
#define ECS_ENUM(T, ...)\
    ECS_ENUM_IMPL(T, #__VA_ARGS__, __VA_ARGS__);\
    ECS_META_CPP(T, EcsEnumType, #__VA_ARGS__);

// Define a bitmask
#define ECS_BITMASK(T, ...)\
    ECS_BITMASK_IMPL(T, #__VA_ARGS__, __VA_ARGS__);\
    ECS_META_CPP(T, EcsBitmaskType, #__VA_ARGS__);

#else

// C

// Define a struct
#define ECS_STRUCT(name, ...)
    ECS_STRUCT_IMPL(name, #__VA_ARGS__, __VA_ARGS__)

// Define an enumeration
#define ECS_ENUM(name, ...)
    ECS_ENUM_IMPL(name, #__VA_ARGS__, __VA_ARGS__)

// Define a bitmask
#define ECS_BITMASK(name, ...)
    ECS_BITMASK_IMPL(name, #__VA_ARGS__, __VA_ARGS__)

#endif


////////////////////////////////////////////////////////////////////////////////
//// Use these macro's inside types
////////////////////////////////////////////////////////////////////////////////

// Define a vector
#define ecs_vector(T) ecs_vector_t*

// Define a map
#define ecs_map(K, T) ecs_map_t*

// Indicate that members after this should not be serialized
#define ECS_NON_SERIALIZABLE


////////////////////////////////////////////////////////////////////////////////
//// Meta description types
////////////////////////////////////////////////////////////////////////////////

/* Explicit string type */
typedef char* ecs_string_t;

ECS_ENUM_BOOTSTRAP( ecs_type_kind_t, {
    EcsPrimitiveType,
    EcsBitmaskType,
    EcsEnumType,
    EcsStructType,
    EcsArrayType,
    EcsVectorType,
    EcsMapType
});

ECS_STRUCT( EcsType, {
    ecs_type_kind_t kind;
    size_t size;
    int8_t alignment;
    const char *descriptor;
});

ECS_ENUM( ecs_primitive_kind_t, {
    EcsBool,
    EcsChar,
    EcsByte,
    EcsU8,
    EcsU16,
    EcsU32,
    EcsU64,
    EcsI8,
    EcsI16,
    EcsI32,
    EcsI64,
    EcsF32,
    EcsF64,
    EcsUPtr,
    EcsIPtr,
    EcsString,
    EcsEntity
});

ECS_STRUCT( EcsPrimitive, {
    ecs_primitive_kind_t kind;
});

ECS_STRUCT( EcsBitmask, {
    ecs_map(int32_t, ecs_string_t) constants;
});

ECS_STRUCT( EcsEnum, {
    ecs_map(int32_t, ecs_string_t) constants;
});

ECS_STRUCT( EcsMember, {
    const char *name;
    ecs_entity_t type;
});

ECS_STRUCT( EcsStruct, {
    ecs_vector(EcsMember) members;
});

ECS_STRUCT( EcsArray, {
    ecs_entity_t element_type;
    int32_t count;
});

ECS_STRUCT( EcsVector, {
    ecs_entity_t element_type;
});

ECS_STRUCT( EcsMap, {
    ecs_entity_t key_type;
    ecs_entity_t element_type;
});


////////////////////////////////////////////////////////////////////////////////
//// Type serializer
////////////////////////////////////////////////////////////////////////////////

ECS_ENUM_C( ecs_type_op_kind_t, {
    EcsOpHeader,
    EcsOpPrimitive,
    EcsOpEnum,
    EcsOpBitmask,
    EcsOpPush,
    EcsOpPop,
    EcsOpArray,
    EcsOpVector,
    EcsOpMap
});

typedef ecs_vector_t ecs_type_op_vector_t;
typedef ecs_vector_t ecs_constant_vector_t;

ECS_STRUCT_C( ecs_type_op_t, {
    ecs_type_op_kind_t kind;
    uint16_t size;
    uint16_t count;
    const char *name;

    uint32_t offset;
    uint8_t alignment;

ECS_NON_SERIALIZABLE

    union {
        ecs_vector(ecs_type_op_t) collection;
        ecs_primitive_kind_t primitive;
        ecs_map(int32_t, ecs_string_t) constant;
        struct {
            struct ecs_type_op_t *key_op;
            ecs_vector(ecs_type_op_t) element_ops;
        } map;
    } is;
});

ECS_STRUCT_C( EcsTypeSerializer, {
    ecs_vector(ecs_type_op_t) ops;
});


////////////////////////////////////////////////////////////////////////////////
//// Module implementation
////////////////////////////////////////////////////////////////////////////////

typedef struct FlecsComponentsMeta {
    ECS_DECLARE_COMPONENT(EcsPrimitive);
    ECS_DECLARE_COMPONENT(EcsEnum);
    ECS_DECLARE_COMPONENT(EcsBitmask);
    ECS_DECLARE_COMPONENT(EcsStruct);
    ECS_DECLARE_COMPONENT(EcsArray);
    ECS_DECLARE_COMPONENT(EcsVector);
    ECS_DECLARE_COMPONENT(EcsMap);
    ECS_DECLARE_COMPONENT(EcsType);
    ECS_DECLARE_COMPONENT(EcsTypeSerializer);
} FlecsComponentsMeta;

#ifdef __cplusplus
extern "C" {
#endif

FLECS_COMPONENTS_META_EXPORT
void FlecsComponentsMetaImport(
    ecs_world_t *world,
    int flags);

#ifdef __cplusplus
}
#endif

#define FlecsComponentsMetaImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, EcsPrimitive);\
    ECS_IMPORT_COMPONENT(handles, EcsEnum);\
    ECS_IMPORT_COMPONENT(handles, EcsBitmask);\
    ECS_IMPORT_COMPONENT(handles, EcsStruct);\
    ECS_IMPORT_COMPONENT(handles, EcsArray);\
    ECS_IMPORT_COMPONENT(handles, EcsVector);\
    ECS_IMPORT_COMPONENT(handles, EcsMap);\
    ECS_IMPORT_COMPONENT(handles, EcsType);\
    ECS_IMPORT_COMPONENT(handles, EcsTypeSerializer);


////////////////////////////////////////////////////////////////////////////////
//// C++ Module implementation
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
#ifndef FLECS_NO_CPP

namespace flecs {
namespace components {

class meta {
public:
    using Primitive = EcsPrimitive;
    using Enum = EcsEnum;
    using Bitmask = EcsBitmask;
    using Struct = EcsStruct;
    using Array = EcsArray;
    using Vector = EcsVector;
    using Type = EcsType;
    using TypeSerializer = EcsTypeSerializer;

    meta(flecs::world& world, int flags) {
        FlecsComponentsMetaImport(world.c_ptr(), flags);

        flecs::module<flecs::components::meta>(world, "flecs::components::meta");

        flecs::component<Primitive>(world, "EcsPrimitive");
        flecs::component<Enum>(world, "EcsEnum");
        flecs::component<Bitmask>(world, "EcsBitmask");
        flecs::component<Struct>(world, "EcsStruct");
        flecs::component<Array>(world, "EcsArray");
        flecs::component<Vector>(world, "EcsVector");
        flecs::component<Type>(world, "EcsType");
        flecs::component<TypeSerializer>(world, "EcsTypeSerializer");
    }
};
}

#endif
#endif


////////////////////////////////////////////////////////////////////////////////
//// Macro for inserting metadata in C application
////////////////////////////////////////////////////////////////////////////////

#define ECS_META(world, T)\
    ecs_entity_t __R##T##__ = ecs_lookup(world, #T);\
    __R##T##__ = ecs_set(world,__R##T##__, EcsTypeSerializer, {__##T##__});


////////////////////////////////////////////////////////////////////////////////
//// Functions for inserting metadata in C++ applications
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
#ifndef FLECS_NO_CPP

// Template that injects metadata into ECS
template <typename T>
void meta(flecs::world& world) {
    flecs::entity e(world, flecs::__meta__<T>::name());
    e.set<EcsType>({ flecs::__meta__<T>::descriptor() });
}

template <typename T>
void meta_component(flecs::world& world) {
    flecs::entity e = flecs::component<T>(world, flecs::__meta__<T>::name());
    e.set<EcsType>({ flecs::__meta__<T>::descriptor() });
}
}


#endif
#endif

#endif
