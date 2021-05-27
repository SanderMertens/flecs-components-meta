#include <test.h>

ECS_STRUCT(Point, {
    int32_t x;
    int32_t y;
});

ECS_STRUCT(Line, {
    Point start;
    Point stop;
});

ECS_STRUCT(bool_i32, {
    bool b;
    int32_t i;
});

ECS_STRUCT(i32_bool, {
    int32_t i;
    bool b;
});

ECS_ARRAY(Color, float, 4);

ECS_STRUCT(StructWithArray,
{
    Line line;
    Color color;
});

void Struct_struct() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Point);

    {
    Point value = {10, 20};
    char *str = ecs_ptr_to_str(world, ecs_entity(Point), &value);
    test_str(str, "{x = 10, y = 20}");
    ecs_os_free(str);
    }

    ecs_fini(world);
}

void Struct_nested_struct() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Point);
    ECS_META(world, Line);

    {
    Line value = {{10, 20}, {30, 40}};
    char *str = ecs_ptr_to_str(world, ecs_entity(Line), &value);
    test_str(str, "{start = {x = 10, y = 20}, stop = {x = 30, y = 40}}");
    ecs_os_free(str);
    }

    ecs_fini(world);
}

void Struct_struct_bool_i32() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, bool_i32);

    {
    bool_i32 value = {false, 10};
    char *str = ecs_ptr_to_str(world, ecs_entity(bool_i32), &value);
    test_str(str, "{b = false, i = 10}");
    ecs_os_free(str);
    }

    ecs_fini(world);
}

void Struct_struct_i32_bool() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, i32_bool);

    {
    i32_bool value = {10, false};
    char *str = ecs_ptr_to_str(world, ecs_entity(i32_bool), &value);
    test_str(str, "{i = 10, b = false}");
    ecs_os_free(str);
    }

    ecs_fini(world);
}

void Struct_array_member() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, StructWithArray);

    {
    StructWithArray value = { 0 };
    char *str = ecs_ptr_to_str(world, ecs_entity(StructWithArray), &value);
    test_str(str, "{x = 10, y = 20}");
    ecs_os_free(str);
    }

    ecs_fini(world);
}