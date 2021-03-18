#ifndef FLECS_META_H
#define FLECS_META_H

/* This generated file contains includes for project dependencies */
#include "flecs-meta/bake_config.h"

#ifndef FLECS_LEGACY
////////////////////////////////////////////////////////////////////////////////
//// Utility macro's (do not use in code!)
////////////////////////////////////////////////////////////////////////////////

#define ECS_ENUM_BOOTSTRAP(name, ...)\
typedef enum name __VA_ARGS__ name;\
ECS_UNUSED \
static const char * __##name##__ = #__VA_ARGS__

#define ECS_STRUCT_IMPL(name, descriptor, ...)\
typedef struct name __VA_ARGS__ name;\
ECS_UNUSED \
static EcsMetaType __##name##__ = {EcsStructType, sizeof(name), ECS_ALIGNOF(name), descriptor, NULL}\

#define ECS_STRUCT_EXTERN_IMPL(name, descriptor)\
ECS_UNUSED \
static EcsMetaType __##name##__ = {EcsStructType, sizeof(name), ECS_ALIGNOF(name), descriptor, NULL}

#define ECS_ENUM_IMPL(name, descriptor, ...)\
typedef enum name __VA_ARGS__ name;\
ECS_UNUSED \
static EcsMetaType __##name##__ = {EcsEnumType, sizeof(name), ECS_ALIGNOF(name), descriptor, NULL}

#define ECS_BITMASK_IMPL(name, descriptor, ...)\
typedef enum name __VA_ARGS__ name;\
ECS_UNUSED \
static EcsMetaType __##name##__ = {EcsBitmaskType, sizeof(name), ECS_ALIGNOF(name), descriptor, NULL}

#define ECS_STRUCT_C(T, ...) ECS_STRUCT_IMPL(T, #__VA_ARGS__, __VA_ARGS__)
#define ECS_ENUM_C(T, ...) ECS_ENUM_IMPL(T, #__VA_ARGS__, __VA_ARGS__)
#define ECS_BITMASK_C(T, ...) ECS_BITMASK_IMPL(T, #__VA_ARGS__, __VA_ARGS__)

#define ECS_ARRAY(name, T, length)\
typedef T name[length];\
ECS_UNUSED \
static EcsMetaType __##name##__ = {EcsArrayType, sizeof(T) * length, ECS_ALIGNOF(T), "(" #T "," #length ")", NULL}

#define ECS_ARRAY_EXTERN(name, T, length)\
ECS_UNUSED \
static EcsMetaType __##name##__ = {EcsArrayType, sizeof(T) * length, ECS_ALIGNOF(name), "(" #T "," #length ")", NULL}

#define ECS_VECTOR(name, T)\
typedef ecs_vector_t *name;\
ECS_UNUSED \
static EcsMetaType __##name##__ = {EcsVectorType, sizeof(ecs_vector_t*), ECS_ALIGNOF(ecs_vector_t*), "(" #T ")", NULL}

#define ECS_MAP(name, K, T)\
typedef ecs_map_t *name;\
ECS_UNUSED \
static EcsMetaType __##name##__ = {EcsMapType, sizeof(ecs_map_t*), ECS_ALIGNOF(ecs_map_t*), "(" #K "," #T ")", NULL}

#ifdef __cplusplus

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
    static EcsMetaType descriptor() {\
        return {kind, sizeof(T), ECS_ALIGNOF(T), descr, NULL};\
    }\
};\
}

#endif


////////////////////////////////////////////////////////////////////////////////
//// Use these macro's to define types.
////   The API will automatically select the right macro's for C or C++
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus

// C++

// Define a struct
#define ECS_STRUCT(T, ...)\
    ECS_STRUCT_IMPL(T, #__VA_ARGS__, __VA_ARGS__);\
    ECS_META_CPP(T, EcsStructType, #__VA_ARGS__)

// Define an enumeration
#define ECS_ENUM(T, ...)\
    ECS_ENUM_IMPL(T, #__VA_ARGS__, __VA_ARGS__);\
    ECS_META_CPP(T, EcsEnumType, #__VA_ARGS__);

// Define a bitmask
#define ECS_BITMASK(T, ...)\
    ECS_BITMASK_IMPL(T, #__VA_ARGS__, __VA_ARGS__);\
    ECS_META_CPP(T, EcsBitmaskType, #__VA_ARGS__)

#else

// C

// Define a struct
#define ECS_STRUCT(name, ...)\
    ECS_STRUCT_IMPL(name, #__VA_ARGS__, __VA_ARGS__)

// Define an external struct
#define ECS_STRUCT_EXTERN(name, ...)\
    ECS_STRUCT_EXTERN_IMPL(name, #__VA_ARGS__)

// Define an enumeration
#define ECS_ENUM(name, ...)\
    ECS_ENUM_IMPL(name, #__VA_ARGS__, __VA_ARGS__)

// Define a bitmask
#define ECS_BITMASK(name, ...)\
    ECS_BITMASK_IMPL(name, #__VA_ARGS__, __VA_ARGS__)

// Define a type alias
#define ECS_ALIAS(type, name)\
    typedef type name;\
    ECS_UNUSED \
    static EcsMetaType __##name##__ = {0, sizeof(name), ECS_ALIGNOF(name), NULL, &__##type##__};\

#endif


////////////////////////////////////////////////////////////////////////////////
//// Use these macro's inside types
////////////////////////////////////////////////////////////////////////////////

// Define a vector
#define ecs_vector(T) ecs_vector_t*

// Define a map
#define ecs_map(K, T) ecs_map_t*

// Indicate that members after this should not be serialized
#define ECS_PRIVATE


////////////////////////////////////////////////////////////////////////////////
//// Meta description types
////////////////////////////////////////////////////////////////////////////////

/* Explicit string type */
typedef const char* ecs_string_t;

/* Explicit byte type */
typedef uint8_t ecs_byte_t;

#ifdef __cplusplus

namespace flecs {
    using string = ecs_string_t;
    using byte = ecs_byte_t;

    // Define a bitmask
    // In C++ trying to assign multiple flags to a variable of an enum type will
    // result in a compiler error. Use this template so that the serializer knows
    // this value is a bitmask, while also keeping the compiler happy.
    template<typename T>
    using bitmask = int32_t;
}

#endif

ECS_ENUM_BOOTSTRAP( ecs_type_kind_t, {
    EcsPrimitiveType,
    EcsBitmaskType,
    EcsEnumType,
    EcsStructType,
    EcsArrayType,
    EcsVectorType,
    EcsMapType
});

ECS_STRUCT( EcsMetaType, {
    ecs_type_kind_t kind;
    ecs_size_t size;
    int16_t alignment;
    const char *descriptor;
    void *alias;
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

// Define EcsBitmask for both C and C++. Both representations are equivalent in
// memory, but allow for a nicer type-safe API in C++
#if defined(__cplusplus) && !defined(FLECS_NO_CPP)
ECS_STRUCT( EcsBitmask, {
    flecs::map<int32_t, flecs::string> constants;
});
#else
ECS_STRUCT( EcsBitmask, {
    ecs_map(int32_t, ecs_string_t) constants;
});
#endif

// Define EcsEnum for both C and C++. Both representations are equivalent in
// memory, but allow for a nicer type-safe API in C++
#if defined(__cplusplus) && !defined(FLECS_NO_CPP)
ECS_STRUCT( EcsEnum, {
    flecs::map<int32_t, flecs::string> constants;
});
#else
ECS_STRUCT( EcsEnum, {
    ecs_map(int32_t, ecs_string_t) constants;
});
#endif

ECS_STRUCT( EcsMember, {
    char *name;
    ecs_entity_t type;
});

// Define EcsStruct for both C and C++. Both representations are equivalent in
// memory, but allow for a nicer type-safe API in C++
#if defined(__cplusplus) && !defined(FLECS_NO_CPP)
ECS_STRUCT( EcsStruct, {
    flecs::vector<EcsMember> members;
    bool is_partial;
});
#else
ECS_STRUCT( EcsStruct, {
    ecs_vector(EcsMember) members;
    bool is_partial;
});
#endif

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
    ecs_entity_t type;
    ecs_type_op_kind_t kind;
    ecs_size_t size;      /* Size of value or element type if array or vector */
    int16_t alignment; /* Alignment of value */
    int32_t count;        /* Number of array elements or struct members */
    int32_t offset;       /* Offset of value */
    const char *name;     /* Name of value (only used for struct members) */

ECS_PRIVATE

    /* Instruction-specific data */
    union {
        ecs_primitive_kind_t primitive;
        ecs_ref_t constant;
        ecs_ref_t collection;

        struct {
            ecs_ref_t key;
            ecs_ref_t element;
        } map;
    } is;
});

ECS_STRUCT_C( EcsMetaTypeSerializer, {
    ecs_vector(ecs_type_op_t) ops;
});

#endif

#ifdef __cplusplus
extern "C" {
#endif


////////////////////////////////////////////////////////////////////////////////
//// Pretty printer
////////////////////////////////////////////////////////////////////////////////

/** Convert value to a string. */
FLECS_META_API
char* ecs_ptr_to_str(
    ecs_world_t *world,
    ecs_entity_t type,
    void* ptr);

/** Convert value to a string. */
FLECS_META_API
char* ecs_entity_to_str(
    ecs_world_t *world,
    ecs_entity_t entity);


////////////////////////////////////////////////////////////////////////////////
//// Serialization utilities
////////////////////////////////////////////////////////////////////////////////

#define ECS_MAX_I8 127
#define ECS_MIN_I8 -128

#define ECS_MAX_I16 32767
#define ECS_MIN_I16 -32768

#define ECS_MAX_I32 2147483647
#define ECS_MIN_I32 -2147483648

#define ECS_MAX_I64 9223372036854775807
#define ECS_MIN_I64 (-9223372036854775807 - 1)

#define ECS_MAX_U8 255u
#define ECS_MAX_U16 65535u
#define ECS_MAX_U32 4294967295u
#define ECS_MAX_U64 18446744073709551615u

#define ECS_MAX_I8_STR "127"
#define ECS_MIN_I8_STR "-128"

#define ECS_MAX_I16_STR "32767"
#define ECS_MIN_I16_STR "-32768"

#define ECS_MAX_I32_STR "2147483647"
#define ECS_MIN_I32_STR "-2147483648"

#define ECS_MAX_I64_STR "9223372036854775807"
#define ECS_MIN_I64_STR "-9223372036854775808"

#define ECS_MAX_U8_STR "255"
#define ECS_MAX_U16_STR "65535"
#define ECS_MAX_U32_STR "4294967295"
#define ECS_MAX_U64_STR "18446744073709551615"

/** Escape a character */
FLECS_META_API
char* ecs_chresc(
    char *out,
    char in,
    char delimiter);

/** Parse an escaped character */
FLECS_META_API
const char* ecs_chrparse(
    const char *in,
    char *out);

/** Escape a string */
FLECS_META_API
ecs_size_t ecs_stresc(
    char *out,
    ecs_size_t n,
    char delimiter,
    const char *in);

#ifdef __cplusplus
}
#endif


////////////////////////////////////////////////////////////////////////////////
//// Deserialization API
////////////////////////////////////////////////////////////////////////////////

#define ECS_META_MAX_SCOPE_DEPTH (32) /* >32 levels of nesting is not sane */

typedef struct ecs_meta_scope_t {
    ecs_entity_t type;
    ecs_vector_t *ops;
    int32_t start;
    int32_t cur_op;
    int32_t cur_elem;
    int32_t count;
    void *base;
    ecs_vector_t *vector;
    bool is_collection;
} ecs_meta_scope_t;

typedef struct ecs_meta_cursor_t {
    const ecs_world_t *world;
    ecs_meta_scope_t scope[ECS_META_MAX_SCOPE_DEPTH];
    int32_t depth;
} ecs_meta_cursor_t;

FLECS_META_API
ecs_meta_cursor_t ecs_meta_cursor(
    const ecs_world_t *world,
    ecs_entity_t type,
    void *base);

FLECS_META_API
void* ecs_meta_get_ptr(
    ecs_meta_cursor_t *cursor);

FLECS_META_API
int ecs_meta_next(
    ecs_meta_cursor_t *cursor);

FLECS_META_API
int ecs_meta_move(
    ecs_meta_cursor_t *cursor,
    int32_t pos);

FLECS_META_API
int ecs_meta_move_name(
    ecs_meta_cursor_t *cursor,
    const char *name);

FLECS_META_API
int ecs_meta_push(
    ecs_meta_cursor_t *cursor);

FLECS_META_API
int ecs_meta_pop(
    ecs_meta_cursor_t *cursor);

FLECS_META_API
int ecs_meta_set_bool(
    ecs_meta_cursor_t *cursor,
    bool value);

FLECS_META_API
int ecs_meta_set_char(
    ecs_meta_cursor_t *cursor,
    char value);

FLECS_META_API
int ecs_meta_set_int(
    ecs_meta_cursor_t *cursor,
    int64_t value);

FLECS_META_API
int ecs_meta_set_uint(
    ecs_meta_cursor_t *cursor,
    uint64_t value);

FLECS_META_API
int ecs_meta_set_float(
    ecs_meta_cursor_t *cursor,
    double value);

FLECS_META_API
int ecs_meta_set_string(
    ecs_meta_cursor_t *cursor,
    const char *value);

FLECS_META_API
int ecs_meta_set_entity(
    ecs_meta_cursor_t *cursor,
    ecs_entity_t value);

FLECS_META_API
int ecs_meta_set_null(
    ecs_meta_cursor_t *cursor);


////////////////////////////////////////////////////////////////////////////////
//// Module implementation
////////////////////////////////////////////////////////////////////////////////

typedef struct FlecsMeta {
    ECS_DECLARE_COMPONENT(EcsPrimitive);
    ECS_DECLARE_COMPONENT(EcsEnum);
    ECS_DECLARE_COMPONENT(EcsBitmask);
    ECS_DECLARE_COMPONENT(EcsStruct);
    ECS_DECLARE_COMPONENT(EcsArray);
    ECS_DECLARE_COMPONENT(EcsVector);
    ECS_DECLARE_COMPONENT(EcsMap);
    ECS_DECLARE_COMPONENT(EcsMetaType);
    ECS_DECLARE_COMPONENT(EcsMetaTypeSerializer);
} FlecsMeta;

#ifdef __cplusplus
extern "C" {
#endif

FLECS_META_API ECS_COMPONENT_EXTERN(EcsPrimitive);
FLECS_META_API ECS_COMPONENT_EXTERN(EcsEnum);
FLECS_META_API ECS_COMPONENT_EXTERN(EcsBitmask);
FLECS_META_API ECS_COMPONENT_EXTERN(EcsMember);
FLECS_META_API ECS_COMPONENT_EXTERN(EcsStruct);
FLECS_META_API ECS_COMPONENT_EXTERN(EcsArray);
FLECS_META_API ECS_COMPONENT_EXTERN(EcsVector);
FLECS_META_API ECS_COMPONENT_EXTERN(EcsMap);
FLECS_META_API ECS_COMPONENT_EXTERN(EcsMetaType);
FLECS_META_API ECS_COMPONENT_EXTERN(ecs_type_op_kind_t);
FLECS_META_API ECS_COMPONENT_EXTERN(ecs_type_op_t);
FLECS_META_API ECS_COMPONENT_EXTERN(EcsMetaTypeSerializer);

FLECS_META_API
void FlecsMetaImport(
    ecs_world_t *world);

#define FlecsMetaImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, EcsPrimitive);\
    ECS_IMPORT_COMPONENT(handles, EcsEnum);\
    ECS_IMPORT_COMPONENT(handles, EcsBitmask);\
    ECS_IMPORT_COMPONENT(handles, EcsStruct);\
    ECS_IMPORT_COMPONENT(handles, EcsArray);\
    ECS_IMPORT_COMPONENT(handles, EcsVector);\
    ECS_IMPORT_COMPONENT(handles, EcsMap);\
    ECS_IMPORT_COMPONENT(handles, EcsMetaType);\
    ECS_IMPORT_COMPONENT(handles, EcsMetaTypeSerializer);

////////////////////////////////////////////////////////////////////////////////
//// Macro for inserting metadata in C application
////////////////////////////////////////////////////////////////////////////////

FLECS_META_API
void ecs_new_meta(
    ecs_world_t *world,
    ecs_entity_t component,
    struct EcsMetaType *meta_type);

#define ECS_META(world, T)\
    ECS_COMPONENT(world, T);\
    ecs_new_meta(world, ecs_entity(T), &__##T##__);

/** Define a meta component, store in variable outside of the current scope.
* Use this macro in a header when defining a component identifier globally.
* Must be used together with ECS_COMPONENT_DECLARE.
*/
#define ECS_META_DEFINE(world, T)\
    ECS_COMPONENT_DEFINE(world, T);\
    ecs_new_meta(world, ecs_entity(T), &__##T##__);

#ifdef __cplusplus
}
#endif


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
    using Type = EcsMetaType;
    using TypeSerializer = EcsMetaTypeSerializer;

    enum TypeKind {
        PrimitiveType = EcsPrimitiveType,
        BitmaskType = EcsBitmaskType,
        EnumType = EcsEnumType,
        StructType = EcsStructType,
        ArrayType = EcsArrayType,
        VectorType = EcsVectorType,
        MapType = EcsMapType
    };

    meta(flecs::world& world) {
        FlecsMetaImport(world.c_ptr());

        flecs::module<flecs::components::meta>(world, "flecs::components::meta");

        flecs::component<Primitive>(world, "EcsPrimitive");
        flecs::component<Enum>(world, "EcsEnum");
        flecs::component<Bitmask>(world, "EcsBitmask");
        flecs::component<Struct>(world, "EcsStruct");
        flecs::component<Array>(world, "EcsArray");
        flecs::component<Vector>(world, "EcsVector");
        flecs::component<Type>(world, "EcsMetaType");
        flecs::component<TypeSerializer>(world, "EcsMetaTypeSerializer");
    }
};
}

////////////////////////////////////////////////////////////////////////////////
//// Functions for inserting metadata in C++ applications
////////////////////////////////////////////////////////////////////////////////

// Template that injects metadata into ECS
template <typename T>
flecs::entity meta(flecs::world& world) {
    flecs::entity e = flecs::component<T>(world, flecs::__meta__<T>::name());
    e.set<EcsMetaType>({ flecs::__meta__<T>::descriptor() });
    return e;
}


////////////////////////////////////////////////////////////////////////////////
//// Serialize values to string
////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::string pretty_print(flecs::world& world, flecs::entity_t type, T& data) {
    char *str = ecs_ptr_to_str(world.c_ptr(), type, &data);
    std::string result = std::string(str);
    free(str);
    return result;
}

template <typename T>
std::string pretty_print(flecs::world& world, flecs::entity type, T& data) {
    return pretty_print(world, type.id(), data);
}

template <typename T>
std::string pretty_print(flecs::world& world, T& data) {
    entity_t type = _::component_info<T>::id();
    return flecs::pretty_print(world, type, data);
}

template <>
inline std::string pretty_print<flecs::entity>(flecs::world& world, flecs::entity& entity) {
    char *str = ecs_entity_to_str(world.c_ptr(), entity.id());
    std::string result = std::string(str);
    free(str);
    return result;
}

}

#endif // FLECS_NO_CPP
#endif // __cplusplus

#endif // FLECS_META_H
