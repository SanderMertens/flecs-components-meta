# flecs.meta
![CI](https://github.com/flecs-hub/flecs-meta/workflows/CI/badge.svg)

This is a reflection library for C/C++ that uses Flecs to store the meta definitions. The library uses a combination of macro and template wizardry to directly parse the C/C++ type defintions. No code generation or APIs to describe the types are needed. 

The library supports:
 - Lots of primitive types
 - External types
 - Enumerations
 - Bitmasks
 - Structs
 - Aliases
 - Arrays
 - Vectors
 - Maps
 
Also check the JSON serializer written with flecs.meta: https://github.com/flecs-hub/flecs-json
 
A simple example in C++:
```c++
ECS_STRUCT(Position, {
    float x;
    float y;
});

int main(int argc, char *argv[]) {
    // The world that will store the reflection data
    flecs::world world;

    // Import the reflection module
    flecs::import<flecs::components::meta>(world);

    // Inject reflection data for position
    flecs::meta<Position>(world);

    // Enjoy!
    Position p{10, 20};
    std::cout << flecs::pretty_print(world, p) << std::endl;
}
```

## Building
Add `flecs_meta.c` and `flecs_meta.h` to your project, in addition to `flecs.c` and `flecs.h` from https://github.com/SanderMertens/flecs.

## FAQ
**Does the library work for any C/C++ type?**

No. If you are using the library with C++, you can only use it for trivial types (for more information, see: http://www.cplusplus.com/reference/type_traits/is_trivial/)

**Can I use the library if I am not using Flecs?**

Yes, the library stores the type inforation in a Flecs world, but your application does not need to be written for Flecs.

**I am using types from another library that doesn't use flecs.meta, does this work?**

Yes, but you will have to explicitly register the types with flecs.meta first. An example of how to do this can be found at the bottom of the `main.c` file which registers external types from the Flecs core.

## Examples

### Primitive types
Only fixed-size primitive types are currently supported. For example, use 
`int32_t` instead of `int`, and `uint32_t` instead of `unsigned int`.

```c++
ECS_STRUCT(Primitives, {
    const char *name;
    int32_t value;
    bool is_active;
});
```

Use it like this:

```c++
flecs::meta<Primitives>(world);

Primitives = {"Foobar", 10, true};

std::cout << flecs::pretty_print(world, p) << std::endl;
```

Output:

```
{name = "Foobar", value = 10, is_active = true}
```

### External types

External types can be defined like any other type

```c
#include <library.h>

ECS_STRUCT_EXTERN(vec2,
{
    float x;
    float y;
});

ECS_STRUCT(Point
{
    vec2 Position;
    bool draw;
});
```

### Enumerations
Enumerations can be used in a way that is similar to structs: 

```c++
ECS_ENUM(Color, {
    Red,
    Green,
    Blue
});

ECS_STRUCT(Point, {
    float x;
    float y;
    Color color;
});
```

Use it like this:

```c++
// Make sure to define Color before Point, as Point uses Color
flecs::meta<Color>(world);
flecs::meta<Point>(world);

Point p = {10, 20, Green};

std::cout << flecs::pretty_print(world, p) << std::endl;
```

Output:

```
{x = 10.0000, y = 20.0000, color = Green}
```

### Bitmasks
Bitmasks are just like enumerations, except that a serializer can serialize multiple constants, depending on which flags are set.

```c++
ECS_BITMASK(Toppings, {
    Bacon,
    Lettuce,
    Tomato,
    Egg,
    Ham
});

ECS_STRUCT(Sandwich, {
    const char *name;
    Toppings toppings;
});
```

Use it like this:

```c++
flecs::meta<Toppings>(world);
flecs::meta<Sandwich>(world);

Sandwich s = {"BLT", Bacon | Lettuce | Tomato};

std::cout << flecs::pretty_print(world, p) << std::endl;
```

Output:

```
{name = "BLT", toppings = Bacon | Lettuce | Tomato}
```

### Aliases

Aliases are simple typedef's of a metatype

```c
ECS_STRUCT(Vector3, {
    float x;
    float y;
    float z;
});

ECS_ALIAS(Vector3, EcsVelocity3);
ECS_ALIAS(Vector3, EcsPosition3);
```

### Arrays
The standard C array notation is correctly recognized:

```c++
ECS_STRUCT(Vec3D, {
    float coord[2];
});
```

Use it like this:

```c++
flecs::meta<Vec2D>(world);

Vec2D v = {10, 20};

std::cout << flecs::pretty_print(world, v) << std::endl;
```

Output:

```
{coord = [10.0000, 20.0000]}
```

### Vectors
Vectors are supported through the built-in Flecs vector type.

```c++
ECS_STRUCT(Vert2D, {
    float x;
    float y;
});

ECS_STRUCT(Mesh, {
    flecs::vector<Vert2D> vertices;
});
```

Use it like this:

```c++
flecs::meta<Vert2D>(world);
flecs::meta<Mesh>(world);

Mesh m = {
    {
        {10, 20},
        {30, 40}
    }
};

std::cout << flecs::pretty_print(world, m) << std::endl;
```

Output:

```
{vertices = [{x = 10.0000, y = 20.0000}, {x = 30.0000, y = 40.0000}]}
```

### Maps
Maps use the built-in Flecs hashmap implementation.

```c++
ECS_STRUCT(Menu, {
    flecs::map<flecs::string, int32_t> items;
});
```

Use it like this:

```c++
flecs::meta<Menu>(world);

/* Create an instance of the Menu type */
Menu m = { {
    {"BLT", 3},
    {"Bacon and cheese", 2}
} };

/* Pretty print the value */
std::cout << flecs::pretty_print(world, m) << std::endl;
```

Output:

```
{items = {"BLT" = 3, "Bacon and cheese" = 2}}
```

