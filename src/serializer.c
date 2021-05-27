#include <flecs_meta.h>
#include "parser.h"
#include "serializer.h"

static
ecs_vector_t* serialize_type(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_vector_t *ops,
    int32_t offset);

static
ecs_size_t ecs_get_primitive_size(
    ecs_primitive_kind_t kind)
{
    switch(kind) {
    case EcsBool: return sizeof(bool);
    case EcsChar: return sizeof(char);
    case EcsByte: return sizeof(char);
    case EcsU8: return sizeof(uint8_t);
    case EcsU16: return sizeof(uint16_t);
    case EcsU32: return sizeof(uint32_t);
    case EcsU64: return sizeof(uint64_t);
    case EcsI8: return sizeof(int8_t);
    case EcsI16: return sizeof(int16_t);
    case EcsI32: return sizeof(int32_t);
    case EcsI64: return sizeof(int64_t);
    case EcsF32: return sizeof(float);
    case EcsF64: return sizeof(double);
    case EcsIPtr: return sizeof(intptr_t);
    case EcsUPtr: return sizeof(uintptr_t);
    case EcsString: return sizeof(char*);
    case EcsEntity: return sizeof(ecs_entity_t);
    default:
        ecs_abort(ECS_INTERNAL_ERROR, NULL);
    }
}

static
int16_t ecs_get_primitive_alignment(
    ecs_primitive_kind_t kind)
{
    switch(kind) {
    case EcsBool: return ECS_ALIGNOF(bool);
    case EcsChar: return ECS_ALIGNOF(char);
    case EcsByte: return ECS_ALIGNOF(char);
    case EcsU8: return ECS_ALIGNOF(uint8_t);
    case EcsU16: return ECS_ALIGNOF(uint16_t);
    case EcsU32: return ECS_ALIGNOF(uint32_t);
    case EcsU64: return ECS_ALIGNOF(uint64_t);
    case EcsI8: return ECS_ALIGNOF(int8_t);
    case EcsI16: return ECS_ALIGNOF(int16_t);
    case EcsI32: return ECS_ALIGNOF(int32_t);
    case EcsI64: return ECS_ALIGNOF(int64_t);
    case EcsF32: return ECS_ALIGNOF(float);
    case EcsF64: return ECS_ALIGNOF(double);
    case EcsIPtr: return ECS_ALIGNOF(intptr_t);
    case EcsUPtr: return ECS_ALIGNOF(uintptr_t);
    case EcsString: return ECS_ALIGNOF(char*);
    case EcsEntity: return ECS_ALIGNOF(ecs_entity_t);
    default:
        ecs_abort(ECS_INTERNAL_ERROR, NULL);
    }
}

static
ecs_vector_t* serialize_primitive(
    ecs_world_t *world,
    ecs_entity_t entity,
    const EcsPrimitive *type,
    ecs_vector_t *ops)
{
    (void)world;
    (void)entity;

    ecs_type_op_t *op;
    if (!ops) {
        op = ecs_vector_add(&ops, ecs_type_op_t);
        *op = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = ecs_get_primitive_size(type->kind),
            .alignment = ecs_get_primitive_alignment(type->kind)
        };
    }

    op = ecs_vector_add(&ops, ecs_type_op_t);

    *op = (ecs_type_op_t) {
        .kind = EcsOpPrimitive,
        .size = ecs_get_primitive_size(type->kind),
        .alignment = ecs_get_primitive_alignment(type->kind),
        .count = 1,
        .is.primitive = type->kind
    };

    return ops;
}

static
ecs_vector_t* serialize_enum(
    ecs_world_t *world,
    ecs_entity_t entity,
    const EcsEnum *type,
    ecs_vector_t *ops)
{
    (void)type;

    ecs_type_op_t *op;
    if (!ops) {
        op = ecs_vector_add(&ops, ecs_type_op_t);
        *op = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = sizeof(int32_t),
            .alignment = ECS_ALIGNOF(int32_t)
        };
    }

    op = ecs_vector_add(&ops, ecs_type_op_t);

    ecs_ref_t ref = {0};
    ecs_get_ref(world, &ref, entity, EcsEnum);

    *op = (ecs_type_op_t) {
        .kind = EcsOpEnum,
        .size = sizeof(int32_t),
        .alignment = ECS_ALIGNOF(int32_t),
        .count = 1,
        .is.constant = ref
    };

    return ops;
}

static
ecs_vector_t* serialize_bitmask(
    ecs_world_t *world,
    ecs_entity_t entity,
    const EcsBitmask *type,
    ecs_vector_t *ops)
{
    (void)type;

    ecs_type_op_t *op;
    if (!ops) {
        op = ecs_vector_add(&ops, ecs_type_op_t);
        *op = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = sizeof(int32_t),
            .alignment = ECS_ALIGNOF(int32_t)
        };
    }

    op = ecs_vector_add(&ops, ecs_type_op_t);

    ecs_ref_t ref = {0};
    ecs_get_ref(world, &ref, entity, EcsBitmask);

    *op = (ecs_type_op_t) {
        .kind = EcsOpBitmask,
        .size = sizeof(int32_t),
        .alignment = ECS_ALIGNOF(int32_t),
        .count = 1,
        .is.constant = ref
    };

    return ops;
}

static
ecs_vector_t* serialize_struct(
    ecs_world_t *world,
    ecs_entity_t entity,
    const EcsStruct *type,
    ecs_vector_t *ops,
    int32_t offset)
{
    ecs_type_op_t *op_header = NULL;
    if (!ops) {
        op_header = ecs_vector_add(&ops, ecs_type_op_t);
    }

    int32_t push_op = ecs_vector_count(ops);

    ecs_type_op_t *op = ecs_vector_add(&ops, ecs_type_op_t);
    *op = (ecs_type_op_t) {
        .kind = EcsOpPush
    };

    ecs_size_t size = 0;
    int16_t alignment = 0;

    EcsMember *members = ecs_vector_first(type->members, EcsMember);
    int32_t i, count = ecs_vector_count(type->members);

    for (i = 0; i < count; i ++) {
        /* Add type operations of member to struct ops */
        int32_t prev_count = ecs_vector_count(ops);
        ops = serialize_type(world, members[i].type, ops, offset + size);

#ifndef NDEBUG
        int32_t op_count = ecs_vector_count(ops);

        /* At least one op should be added */
        ecs_assert(prev_count != op_count, ECS_INTERNAL_ERROR, NULL);
        ecs_assert(ops != NULL, ECS_INTERNAL_ERROR, NULL);
#endif

        op = ecs_vector_get(ops, ecs_type_op_t, prev_count);
        op->name = members[i].name;

        const EcsMetaType *meta_type = ecs_get(world, members[i].type, EcsMetaType);
        int16_t member_alignment = meta_type->alignment;
        ecs_size_t member_size;

        if (meta_type->kind == EcsStructType) {
            member_size = meta_type->size;
        }
        else if (meta_type->kind == EcsArrayType) {
            member_size = op->size * op->count;
        }
        else {
            member_size = meta_type->size * op->count;
        }

        ecs_assert(member_size != 0, ECS_INTERNAL_ERROR, op->name);
        ecs_assert(member_alignment != 0, ECS_INTERNAL_ERROR, op->name);

        size = ECS_ALIGN(size, member_alignment);
        op->offset = offset + size;

        size += member_size;

        if (member_alignment > alignment) {
            alignment = member_alignment;
        }
    }

    /* Align struct size to struct alignment */
    size = ECS_ALIGN(size, alignment);

    /* Size and alignment are ordinarily determined by ECS_STRUCT and should be
     * the same as the values computed here. However, there are two exceptions.
     * The first exception is when an application defines a type by populating
     * the EcsStruct component directly and does not provide size and alignment
     * values for EcsMetaType. The second scenario is when the type definition
     * contains an ECS_PRIVATE, in which case the type may contain
     * members that are not described.
     *
     * In the first case the computed values should be set in EcsMetaType. In the
     * second case the values from EcsMetaType should be assigned to the type
     * operation. */
    bool is_added;
    EcsMetaType *base_type = ecs_get_mut(world, entity, EcsMetaType, &is_added);
    ecs_assert(base_type != NULL, ECS_INTERNAL_ERROR, NULL);
    if (!is_added) {
        if (!type->is_partial) {
            /* EcsMetaType existed already, and this is not a partial type. This
             * means that computed size and alignment should match exactly. */
            if (base_type->size) {
                ecs_assert(base_type->size == size, ECS_INTERNAL_ERROR, NULL);
            }

            if (base_type->alignment) {
                ecs_assert(
                    base_type->alignment == alignment, ECS_INTERNAL_ERROR, NULL);
            }
        } else {
            /* EcsMetaType exists, and this is a partial type. In this case the
             * computed values only apply to the members described in EcsStruct
             * but not to the type as a whole. Use the values from EcsMetaType. Note
             * that it is not allowed to have a partial type for which no size
             * and alignment are specified in EcsMetaType. */
            ecs_assert(base_type->size != 0, ECS_INVALID_PARAMETER, NULL);
            ecs_assert(base_type->alignment != 0, ECS_INVALID_PARAMETER, NULL);

            size = base_type->size;
            alignment = base_type->alignment;
        }
    } else {
        /* If EcsMetaType was not set yet, initialize descriptor, alias to NULL
         * since it won't be used here */
        base_type->descriptor = NULL;
        base_type->alias = NULL;
    }

    base_type->kind = EcsStructType;
    base_type->size = size;
    base_type->alignment = alignment;

    op = ecs_vector_add(&ops, ecs_type_op_t);
    *op = (ecs_type_op_t) {
        .kind = EcsOpPop,
    };

    if (op_header) {
        op_header = ecs_vector_first(ops, ecs_type_op_t);
        *op_header = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = size,
            .alignment = alignment
        };
    }

    ecs_type_op_t *op_push = ecs_vector_get(ops, ecs_type_op_t, push_op);
    ecs_assert(op_push->kind == EcsOpPush, ECS_INTERNAL_ERROR, NULL);
    op_push->size = size;
    op_push->alignment = alignment;
    op_push->count = count;

    return ops;
}

static
ecs_vector_t* serialize_array(
    ecs_world_t *world,
    ecs_entity_t entity,
    const EcsArray *type,
    ecs_vector_t *ops)
{
    (void)entity;

    ecs_type_op_t *op_header = NULL;
    if (!ops) {
        op_header = ecs_vector_add(&ops, ecs_type_op_t);
    }

    const EcsMetaType *element_type = ecs_get(world, type->element_type, EcsMetaType);
    ecs_assert(element_type != NULL, ECS_INTERNAL_ERROR, NULL);

    ecs_ref_t ref = {0};
    ecs_get_ref(world, &ref, type->element_type, EcsMetaTypeSerializer);

    ecs_type_op_t *op = ecs_vector_add(&ops, ecs_type_op_t);
    *op = (ecs_type_op_t){
        .kind = EcsOpArray,
        .count = type->count,
        .size = element_type->size,
        .alignment = element_type->alignment,
        .is.collection = ref
    };

    if (op_header) {
        op_header = ecs_vector_first(ops, ecs_type_op_t);
        *op_header = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = op->size,
            .alignment = op->alignment
        };
    }

    EcsMetaType *meta = ecs_get_mut(world, entity, EcsMetaType, NULL);

    if(!meta->size || !meta->alignment) {
        meta->size = type->count * element_type->size;
        meta->alignment = element_type->alignment;
    }

    return ops;
}

static
ecs_vector_t* serialize_vector(
    ecs_world_t *world,
    ecs_entity_t entity,
    const EcsVector *type,
    ecs_vector_t *ops)
{
    (void)entity;

    ecs_type_op_t *op = NULL;
    if (!ops) {
        op = ecs_vector_add(&ops, ecs_type_op_t);
        *op = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = sizeof(ecs_vector_t*),
            .alignment = ECS_ALIGNOF(ecs_vector_t*)
        };
    }

    const EcsMetaType *element_type = ecs_get(world, type->element_type, EcsMetaType);
    ecs_assert(element_type != NULL, ECS_INTERNAL_ERROR, NULL);

    ecs_ref_t ref = {0};
    ecs_get_ref(world, &ref, type->element_type, EcsMetaTypeSerializer);

    op = ecs_vector_add(&ops, ecs_type_op_t);
    *op = (ecs_type_op_t){
        .kind = EcsOpVector,
        .count = 1,
        .size = element_type->size,
        .alignment = element_type->alignment,
        .is.collection = ref
    };

    return ops;
}

static
ecs_vector_t* serialize_map(
    ecs_world_t *world,
    ecs_entity_t entity,
    const EcsMap *type,
    ecs_vector_t *ops)
{
    ecs_type_op_t *op = NULL;
    if (!ops) {
        op = ecs_vector_add(&ops, ecs_type_op_t);
        *op = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = sizeof(ecs_map_t*),
            .alignment = ECS_ALIGNOF(ecs_map_t*)
        };
    }

    const EcsMetaTypeSerializer *key_cache = ecs_get(world, type->key_type, EcsMetaTypeSerializer);
    ecs_assert(key_cache != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(key_cache->ops != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(ecs_vector_count(key_cache->ops) != 0, ECS_INTERNAL_ERROR, NULL);

    /* Make sure first op is the header */
    ecs_type_op_t *key_op = ecs_vector_first(key_cache->ops, ecs_type_op_t);
    ecs_assert(key_op->kind == EcsOpHeader, ECS_INTERNAL_ERROR, NULL);

    if (ecs_vector_count(key_cache->ops) != 2) {
        const EcsMetaType *ptr = ecs_get(world, entity, EcsMetaType);
        ecs_assert(ptr != NULL, ECS_INTERNAL_ERROR, NULL);

        ecs_meta_parse_ctx_t ctx = {
            .name = ecs_get_name(world, entity),
            .decl = ptr->descriptor
        };

        ecs_meta_error( &ctx, ctx.decl,
            "invalid key type '%s' for map", ecs_get_name(world, type->key_type));
    }

    key_op = ecs_vector_get(key_cache->ops, ecs_type_op_t, 1);
    ecs_assert(key_op != NULL, ECS_INTERNAL_ERROR, NULL);

    if (key_op->count != 1) {
        const EcsMetaType *ptr = ecs_get(world, entity, EcsMetaType);
        ecs_assert(ptr != NULL, ECS_INTERNAL_ERROR, NULL);

        ecs_meta_parse_ctx_t ctx = {
            .name = ecs_get_name(world, entity),
            .decl = ptr->descriptor
        };
        ecs_meta_error( &ctx, ctx.decl, "array type invalid for key type");
    }

    ecs_ref_t key_ref = {0};
    ecs_get_ref(world, &key_ref, type->key_type, EcsMetaTypeSerializer);

    ecs_ref_t element_ref = {0};
    ecs_get_ref(world, &element_ref, type->element_type, EcsMetaTypeSerializer);

    op = ecs_vector_add(&ops, ecs_type_op_t);
    *op = (ecs_type_op_t){
        .kind = EcsOpMap,
        .count = 1,
        .size = sizeof(ecs_map_t*),
        .alignment = ECS_ALIGNOF(ecs_map_t*),
        .is.map = {
            .key = key_ref,
            .element = element_ref
        }
    };

    return ops;
}

static
ecs_vector_t* serialize_type(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_vector_t *ops,
    int32_t offset)
{
    const EcsMetaType *type = ecs_get(world, entity, EcsMetaType);
    ecs_assert(type != NULL, ECS_INVALID_PARAMETER, NULL);

    switch(type->kind) {
    case EcsPrimitiveType: {
        const EcsPrimitive *t = ecs_get(world, entity, EcsPrimitive);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_primitive(world, entity, t, ops);
    }

    case EcsEnumType: {
        const EcsEnum *t = ecs_get(world, entity, EcsEnum);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_enum(world, entity, t, ops);
    }

    case EcsBitmaskType: {
        const EcsBitmask *t = ecs_get(world, entity, EcsBitmask);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_bitmask(world, entity, t, ops);
    }

    case EcsStructType: {
        const EcsStruct *t = ecs_get(world, entity, EcsStruct);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_struct(world, entity, t, ops, offset);
    }

    case EcsArrayType: {
        const EcsArray *t = ecs_get(world, entity, EcsArray);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_array(world, entity, t, ops);
    }

    case EcsVectorType: {
        const EcsVector *t = ecs_get(world, entity, EcsVector);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_vector(world, entity, t, ops);
    }

    case EcsMapType: {
        const EcsMap *t = ecs_get(world, entity, EcsMap);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_map(world, entity, t, ops);
    }

    default:
        break;
    }

    return NULL;
}

void EcsSetPrimitive(ecs_iter_t *it) {
    EcsPrimitive *type = ecs_column(it, EcsPrimitive, 1);

    ecs_world_t *world = it->world;

    int i;
    for (i = 0; i < it->count; i ++) {
        ecs_entity_t e = it->entities[i];

        /* Size and alignment for primitive types can only be set after we know
         * what kind of primitive type it is. Set values in case they haven't
         * been set already */
        bool is_added;
        EcsMetaType *base_type = ecs_get_mut(world, e, EcsMetaType, &is_added);
        ecs_assert(base_type != NULL, ECS_INTERNAL_ERROR, NULL);

        base_type->size = ecs_get_primitive_size(type[i].kind);
        base_type->alignment = ecs_get_primitive_alignment(type[i].kind);

        ecs_set(world, e, EcsMetaTypeSerializer, {
            serialize_primitive(
                world, e, &type[i], NULL)
        });
    }
}

void EcsSetEnum(ecs_iter_t *it) {
    EcsEnum *type = ecs_column(it, EcsEnum, 1);

    ecs_world_t *world = it->world;

    int i;
    for (i = 0; i < it->count; i ++) {
        ecs_entity_t e = it->entities[i];

        ecs_set(it->world, e, EcsMetaTypeSerializer, {
            serialize_enum(world, e, &type[i], NULL)
        });
    }
}

void EcsSetBitmask(ecs_iter_t *it) {
    EcsBitmask *type = ecs_column(it, EcsBitmask, 1);

    ecs_world_t *world = it->world;

    int i;
    for (i = 0; i < it->count; i ++) {
        ecs_entity_t e = it->entities[i];
        ecs_set(it->world, e, EcsMetaTypeSerializer, {
            serialize_bitmask(world, e, &type[i], NULL)
        });
    }
}

void EcsSetStruct(ecs_iter_t *it) {
    EcsStruct *type = ecs_column(it, EcsStruct, 1);

    ecs_world_t *world = it->world;

    int i;
    for (i = 0; i < it->count; i ++) {
        ecs_entity_t e = it->entities[i];
        ecs_set(it->world, e, EcsMetaTypeSerializer, {
            serialize_struct(world, e, &type[i], NULL, 0)
        });
    }
}

void EcsSetArray(ecs_iter_t *it) {
    EcsArray *type = ecs_column(it, EcsArray, 1);

    ecs_world_t *world = it->world;

    int i;
    for (i = 0; i < it->count; i ++) {
        ecs_entity_t e = it->entities[i];
        ecs_set(it->world, e, EcsMetaTypeSerializer, {
            serialize_array(world, e, &type[i], NULL)
        });
    }
}

void EcsSetVector(ecs_iter_t *it) {
    EcsVector *type = ecs_column(it, EcsVector, 1);

    ecs_world_t *world = it->world;

    int i;
    for (i = 0; i < it->count; i ++) {
        ecs_entity_t e = it->entities[i];
        ecs_set(it->world, e, EcsMetaTypeSerializer, {
            serialize_vector(world, e, &type[i], NULL)
        });
    }
}

void EcsSetMap(ecs_iter_t *it) {
    EcsMap *type = ecs_column(it, EcsMap, 1);

    ecs_world_t *world = it->world;

    int i;
    for (i = 0; i < it->count; i ++) {
        ecs_entity_t e = it->entities[i];
        ecs_set(it->world, e, EcsMetaTypeSerializer, {
            serialize_map(world, e, &type[i], NULL)
        });
    }
}
