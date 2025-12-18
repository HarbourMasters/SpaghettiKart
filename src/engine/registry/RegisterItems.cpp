#include "RegisterContent.h"
#include "Registry.h"

//! @todo turn items into a class
void RegisterItems(Registry<ItemInfo>& r) {
    ItemInfo info;

    info = { .ResourceName = "mk:none", .Name = "No Item" };
    r.Add(info, {});

    info = { .ResourceName = "mk:banana", .Name = "Banana" };
    r.Add(info, {});

    info = { .ResourceName = "mk:banana_bunch", .Name = "Banana Bunch" };
    r.Add(info, {});

    info = { .ResourceName = "mk:green_shell", .Name = "Green Shell" };
    r.Add(info, {});

    info = { .ResourceName = "mk:triple_green_shell", .Name = "Triple Green Shell" };
    r.Add(info, {});

    info = { .ResourceName = "mk:red_shell", .Name = "Red Shell" };
    r.Add(info, {});

    info = { .ResourceName = "mk:triple_red_shell", .Name = "Triple Red Shell" };
    r.Add(info, {});

    info = { .ResourceName = "mk:blue_spiny_shell", .Name = "Blue Spiny Shell" };
    r.Add(info, {});

    info = { .ResourceName = "mk:thunderbolt", .Name = "Thunderbolt" };
    r.Add(info, {});

    info = { .ResourceName = "mk:fake_item_box", .Name = "Fake Item Box" };
    r.Add(info, {});

    info = { .ResourceName = "mk:star", .Name = "Star" };
    r.Add(info, {});

    info = { .ResourceName = "mk:boo", .Name = "Boo" };
    r.Add(info, {});

    info = { .ResourceName = "mk:mushroom", .Name = "Mushroom" };
    r.Add(info, {});

    info = { .ResourceName = "mk:double_mushroom", .Name = "Double Mushroom" };
    r.Add(info, {});

    info = { .ResourceName = "mk:triple_mushroom", .Name = "Triple Mushroom" };
    r.Add(info, {});

    info = { .ResourceName = "mk:super_mushroom", .Name = "Super Mushroom" };
    r.Add(info, {});
}