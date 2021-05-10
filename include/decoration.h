#ifndef GUARD_DECORATION_H
#define GUARD_DECORATION_H

enum DecorationPermission
{
    /* The nomenclature here describes collision and placement permissions, in that order. */
    DECORPERM_SOLID_FLOOR,
};

enum DecorationShape
{
    DECORSHAPE_1x1,
};

enum DecorationCategory
{
    DECORCAT_DESK,
};

struct Decoration
{
    u8 id;
    u8 name[16];
    u8 permission;
    u8 shape;
    u8 category;
    u16 price;
    const u8 *description;
    const u16 *tiles;
};

#endif //GUARD_DECORATION_H
