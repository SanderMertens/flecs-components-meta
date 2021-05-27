
/* A friendly warning from bake.test
 * ----------------------------------------------------------------------------
 * This file is generated. To add/remove testcases modify the 'project.json' of
 * the test project. ANY CHANGE TO THIS FILE IS LOST AFTER (RE)BUILDING!
 * ----------------------------------------------------------------------------
 */

#include <test.h>

// Testsuite 'Primitive'
void Primitive_bool(void);
void Primitive_byte(void);
void Primitive_char(void);
void Primitive_i8(void);
void Primitive_i16(void);
void Primitive_i32(void);
void Primitive_i64(void);
void Primitive_iptr(void);
void Primitive_u8(void);
void Primitive_u16(void);
void Primitive_u32(void);
void Primitive_u64(void);
void Primitive_uptr(void);
void Primitive_float(void);
void Primitive_double(void);
void Primitive_string(void);
void Primitive_entity(void);

// Testsuite 'Struct'
void Struct_struct(void);
void Struct_nested_struct(void);
void Struct_struct_bool_i32(void);
void Struct_struct_i32_bool(void);
void Struct_array_member(void);

// Testsuite 'Enum'
void Enum_enum(void);
void Enum_enum_explicit_values(void);
void Enum_enum_invalid_value(void);

// Testsuite 'Bitmask'
void Bitmask_bitmask_1(void);
void Bitmask_bitmask_2(void);
void Bitmask_bitmask_3(void);
void Bitmask_bitmask_0_value(void);

// Testsuite 'Array'
void Array_array_bool(void);
void Array_array_int(void);
void Array_array_string(void);
void Array_array_entity(void);
void Array_struct_array_int(void);
void Array_array_struct(void);
void Array_array_nested_struct(void);
void Array_array_array_int(void);
void Array_array_array_string(void);
void Array_array_array_struct(void);
void Array_array_array_nested_struct(void);

// Testsuite 'Vector'
void Vector_vector_bool(void);
void Vector_vector_int(void);
void Vector_vector_string(void);
void Vector_vector_entity(void);
void Vector_vector_struct(void);
void Vector_vector_nested_struct(void);
void Vector_vector_vector_int(void);
void Vector_vector_vector_struct(void);
void Vector_vector_vector_nested_struct(void);
void Vector_vector_empty(void);
void Vector_vector_vector_empty(void);
void Vector_vector_null(void);
void Vector_vector_vector_null(void);

// Testsuite 'Map'
void Map_map_bool_bool(void);
void Map_map_int_bool(void);
void Map_map_string_bool(void);
void Map_map_enum_bool(void);
void Map_map_bitmask_bool(void);
void Map_map_int_int(void);
void Map_map_int_string(void);
void Map_map_int_struct(void);
void Map_map_int_nested_struct(void);
void Map_map_int_array_int(void);
void Map_map_int_vector_int(void);
void Map_map_int_map_int_bool(void);

bake_test_case Primitive_testcases[] = {
    {
        "bool",
        Primitive_bool
    },
    {
        "byte",
        Primitive_byte
    },
    {
        "char",
        Primitive_char
    },
    {
        "i8",
        Primitive_i8
    },
    {
        "i16",
        Primitive_i16
    },
    {
        "i32",
        Primitive_i32
    },
    {
        "i64",
        Primitive_i64
    },
    {
        "iptr",
        Primitive_iptr
    },
    {
        "u8",
        Primitive_u8
    },
    {
        "u16",
        Primitive_u16
    },
    {
        "u32",
        Primitive_u32
    },
    {
        "u64",
        Primitive_u64
    },
    {
        "uptr",
        Primitive_uptr
    },
    {
        "float",
        Primitive_float
    },
    {
        "double",
        Primitive_double
    },
    {
        "string",
        Primitive_string
    },
    {
        "entity",
        Primitive_entity
    }
};

bake_test_case Struct_testcases[] = {
    {
        "struct",
        Struct_struct
    },
    {
        "nested_struct",
        Struct_nested_struct
    },
    {
        "struct_bool_i32",
        Struct_struct_bool_i32
    },
    {
        "struct_i32_bool",
        Struct_struct_i32_bool
    },
    {
        "struct_array_member",
        Struct_array_member
    }
};

bake_test_case Enum_testcases[] = {
    {
        "enum",
        Enum_enum
    },
    {
        "enum_explicit_values",
        Enum_enum_explicit_values
    },
    {
        "enum_invalid_value",
        Enum_enum_invalid_value
    }
};

bake_test_case Bitmask_testcases[] = {
    {
        "bitmask_1",
        Bitmask_bitmask_1
    },
    {
        "bitmask_2",
        Bitmask_bitmask_2
    },
    {
        "bitmask_3",
        Bitmask_bitmask_3
    },
    {
        "bitmask_0_value",
        Bitmask_bitmask_0_value
    }
};

bake_test_case Array_testcases[] = {
    {
        "array_bool",
        Array_array_bool
    },
    {
        "array_int",
        Array_array_int
    },
    {
        "array_string",
        Array_array_string
    },
    {
        "array_entity",
        Array_array_entity
    },
    {
        "struct_array_int",
        Array_struct_array_int
    },
    {
        "array_struct",
        Array_array_struct
    },
    {
        "array_nested_struct",
        Array_array_nested_struct
    },
    {
        "array_array_int",
        Array_array_array_int
    },
    {
        "array_array_string",
        Array_array_array_string
    },
    {
        "array_array_struct",
        Array_array_array_struct
    },
    {
        "array_array_nested_struct",
        Array_array_array_nested_struct
    }
};

bake_test_case Vector_testcases[] = {
    {
        "vector_bool",
        Vector_vector_bool
    },
    {
        "vector_int",
        Vector_vector_int
    },
    {
        "vector_string",
        Vector_vector_string
    },
    {
        "vector_entity",
        Vector_vector_entity
    },
    {
        "vector_struct",
        Vector_vector_struct
    },
    {
        "vector_nested_struct",
        Vector_vector_nested_struct
    },
    {
        "vector_vector_int",
        Vector_vector_vector_int
    },
    {
        "vector_vector_struct",
        Vector_vector_vector_struct
    },
    {
        "vector_vector_nested_struct",
        Vector_vector_vector_nested_struct
    },
    {
        "vector_empty",
        Vector_vector_empty
    },
    {
        "vector_vector_empty",
        Vector_vector_vector_empty
    },
    {
        "vector_null",
        Vector_vector_null
    },
    {
        "vector_vector_null",
        Vector_vector_vector_null
    }
};

bake_test_case Map_testcases[] = {
    {
        "map_bool_bool",
        Map_map_bool_bool
    },
    {
        "map_int_bool",
        Map_map_int_bool
    },
    {
        "map_string_bool",
        Map_map_string_bool
    },
    {
        "map_enum_bool",
        Map_map_enum_bool
    },
    {
        "map_bitmask_bool",
        Map_map_bitmask_bool
    },
    {
        "map_int_int",
        Map_map_int_int
    },
    {
        "map_int_string",
        Map_map_int_string
    },
    {
        "map_int_struct",
        Map_map_int_struct
    },
    {
        "map_int_nested_struct",
        Map_map_int_nested_struct
    },
    {
        "map_int_array_int",
        Map_map_int_array_int
    },
    {
        "map_int_vector_int",
        Map_map_int_vector_int
    },
    {
        "map_int_map_int_bool",
        Map_map_int_map_int_bool
    }
};

static bake_test_suite suites[] = {
    {
        "Primitive",
        NULL,
        NULL,
        17,
        Primitive_testcases
    },
    {
        "Struct",
        NULL,
        NULL,
        5,
        Struct_testcases
    },
    {
        "Enum",
        NULL,
        NULL,
        3,
        Enum_testcases
    },
    {
        "Bitmask",
        NULL,
        NULL,
        4,
        Bitmask_testcases
    },
    {
        "Array",
        NULL,
        NULL,
        11,
        Array_testcases
    },
    {
        "Vector",
        NULL,
        NULL,
        13,
        Vector_testcases
    },
    {
        "Map",
        NULL,
        NULL,
        12,
        Map_testcases
    }
};

int main(int argc, char *argv[]) {
    ut_init(argv[0]);
    return bake_test_run("test", argc, argv, suites, 7);
}
