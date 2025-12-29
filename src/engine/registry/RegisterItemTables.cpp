#include "RegisterContent.h"
#include <utility>
#include "engine/RandomItemTable.h"

/**
 * DO NOT RE-ORDER ANYTHING IN THESE FILES
 * 
 * It will result in differing Ids
 */

enum RANK {
    RANK_ONE,
    RANK_TWO,
    RANK_THREE,
    RANK_FOUR,
    RANK_FIVE,
    RANK_SIX,
    RANK_SEVEN,
    RANK_EIGHT
};

void RegisterHumanItemTable(DataRegistry<RandomItemTable>& r);
void RegisterCPUItemTable(DataRegistry<RandomItemTable>& r);
void RegisterHardCPUItemTable(DataRegistry<RandomItemTable>& r);
void RegisterVersus2PItemTable(DataRegistry<RandomItemTable>& r);
void RegisterVersus3PItemTable(DataRegistry<RandomItemTable>& r);
void RegisterVersus4PItemTable(DataRegistry<RandomItemTable>& r);
void RegisterBattleItemTable(DataRegistry<RandomItemTable>& r);

void RegisterItemTables(DataRegistry<RandomItemTable>& r) {
    RegisterHumanItemTable(r);
    RegisterCPUItemTable(r);
    RegisterHardCPUItemTable(r);
    RegisterVersus2PItemTable(r);
    RegisterVersus3PItemTable(r);
    RegisterVersus4PItemTable(r);
    RegisterBattleItemTable(r);
}

void RegisterHumanItemTable(DataRegistry<RandomItemTable>& r) {
    RandomItemTable table;

    table.Add("mk:banana",         RANK_ONE, 30, 0.0f);
    table.Add("mk:banana_bunch",   RANK_ONE, 5, 0.0f);
    table.Add("mk:green_shell",     RANK_ONE, 30, 0.0f);
    table.Add("mk:triple_green_shell",   RANK_ONE, 5, 0.0f);
    table.Add("mk:red_shell",       RANK_ONE, 5, 0.0f);
    table.Add("mk:fake_item_box",  RANK_ONE, 10, 0.0f);
    table.Add("mk:boo",            RANK_ONE, 5, 0.0f);
    table.Add("mk:mushroom",       RANK_ONE, 10, 0.0f);

    table.Add("mk:banana_bunch",   RANK_TWO, 5, 0.0f);
    table.Add("mk:green_shell",     RANK_TWO, 5, 0.0f);
    table.Add("mk:triple_green_shell",   RANK_TWO, 10, 0.0f);
    table.Add("mk:red_shell",       RANK_TWO, 15, 0.0f);
    table.Add("mk:triple_red_shell",     RANK_TWO, 20, 0.0f);
    table.Add("mk:thunderbolt",    RANK_TWO, 5, 0.0f);
    table.Add("mk:fake_item_box",  RANK_TWO, 5, 0.0f);
    table.Add("mk:star",           RANK_TWO, 5, 0.0f);
    table.Add("mk:boo",            RANK_TWO, 5, 0.0f);
    table.Add("mk:mushroom",       RANK_TWO, 5, 0.0f);
    table.Add("mk:triple_mushroom",RANK_TWO, 15, 0.0f);
    table.Add("mk:super_mushroom", RANK_TWO, 5, 0.0f);

    table.Add("mk:triple_green_shell",   RANK_THREE, 10, 0.0f);
    table.Add("mk:red_shell",       RANK_THREE, 20, 0.0f);
    table.Add("mk:triple_red_shell",     RANK_THREE, 20, 0.0f);
    table.Add("mk:thunderbolt",    RANK_THREE, 5, 0.0f);
    table.Add("mk:star",           RANK_THREE, 10, 0.0f);
    table.Add("mk:mushroom",       RANK_THREE, 5, 0.0f);
    table.Add("mk:triple_mushroom",RANK_THREE, 20, 0.0f);
    table.Add("mk:super_mushroom", RANK_THREE, 10, 0.0f);

    table.Add("mk:red_shell",       RANK_FOUR, 15, 0.0f);
    table.Add("mk:triple_red_shell",     RANK_FOUR, 20, 0.0f);
    table.Add("mk:blue_spiny_shell",     RANK_FOUR, 5, 0.0f);
    table.Add("mk:thunderbolt",    RANK_FOUR, 10, 0.0f);
    table.Add("mk:star",           RANK_FOUR, 15, 0.0f);
    table.Add("mk:mushroom",       RANK_FOUR, 5, 0.0f);
    table.Add("mk:triple_mushroom",RANK_FOUR, 20, 0.0f);
    table.Add("mk:super_mushroom", RANK_FOUR, 10, 0.0f);

    table.Add("mk:red_shell",       RANK_FIVE, 10, 0.0f);
    table.Add("mk:triple_red_shell",     RANK_FIVE, 20, 0.0f);
    table.Add("mk:blue_spiny_shell",     RANK_FIVE, 5, 0.0f);
    table.Add("mk:thunderbolt",    RANK_FIVE, 10, 0.0f);
    table.Add("mk:star",           RANK_FIVE, 15, 0.0f);
    table.Add("mk:mushroom",       RANK_FIVE, 5, 0.0f);
    table.Add("mk:triple_mushroom",RANK_FIVE, 25, 0.0f);
    table.Add("mk:super_mushroom", RANK_FIVE, 10, 0.0f);

    table.Add("mk:triple_red_shell",     RANK_SIX, 20, 0.0f);
    table.Add("mk:blue_spiny_shell",     RANK_SIX, 10, 0.0f);
    table.Add("mk:thunderbolt",    RANK_SIX, 15, 0.0f);
    table.Add("mk:star",           RANK_SIX, 20, 0.0f);
    table.Add("mk:triple_mushroom",RANK_SIX, 25, 0.0f);
    table.Add("mk:super_mushroom", RANK_SIX, 10, 0.0f);

    table.Add("mk:triple_red_shell",     RANK_SEVEN, 20, 0.0f);
    table.Add("mk:blue_spiny_shell",     RANK_SEVEN, 10, 0.0f);
    table.Add("mk:thunderbolt",    RANK_SEVEN, 20, 0.0f);
    table.Add("mk:star",           RANK_SEVEN, 30, 0.0f);
    table.Add("mk:triple_mushroom",RANK_SEVEN, 10, 0.0f);
    table.Add("mk:super_mushroom", RANK_SEVEN, 10, 0.0f);

    table.Add("mk:triple_red_shell",     RANK_EIGHT, 20, 0.0f);
    table.Add("mk:blue_spiny_shell",     RANK_EIGHT, 15, 0.0f);
    table.Add("mk:thunderbolt",    RANK_EIGHT, 20, 0.0f);
    table.Add("mk:star",           RANK_EIGHT, 30, 0.0f);
    table.Add("mk:triple_mushroom",RANK_EIGHT, 5, 0.0f);
    table.Add("mk:super_mushroom", RANK_EIGHT, 10, 0.0f);

    r.Add("mk:human_grand_prix", std::move(table));
}

void RegisterCPUItemTable(DataRegistry<RandomItemTable>& r) {
    RandomItemTable table;

    table.Add("mk:banana",         RANK_ONE, 60, 0.0f);
    table.Add("mk:green_shell",     RANK_ONE, 25, 0.0f);
    table.Add("mk:fake_item_box",  RANK_ONE, 10, 0.0f);
    table.Add("mk:boo",            RANK_ONE, 5, 0.0f);

    table.Add("mk:banana",         RANK_TWO, 50, 0.0f);
    table.Add("mk:green_shell",     RANK_TWO, 25, 0.0f);
    table.Add("mk:triple_green_shell",   RANK_TWO, 5, 0.0f);
    table.Add("mk:fake_item_box",  RANK_TWO, 10, 0.0f);
    table.Add("mk:boo",            RANK_TWO, 5, 0.0f);
    table.Add("mk:mushroom",       RANK_TWO, 5, 0.0f);

    table.Add("mk:banana",         RANK_THREE, 40, 0.0f);
    table.Add("mk:green_shell",     RANK_THREE, 25, 0.0f);
    table.Add("mk:triple_green_shell",   RANK_THREE, 10, 0.0f);
    table.Add("mk:fake_item_box",  RANK_THREE, 10, 0.0f);
    table.Add("mk:boo",            RANK_THREE, 5, 0.0f);
    table.Add("mk:mushroom",       RANK_THREE, 10, 0.0f);

    table.Add("mk:banana",         RANK_FOUR, 35, 0.0f);
    table.Add("mk:green_shell",     RANK_FOUR, 25, 0.0f);
    table.Add("mk:triple_green_shell",   RANK_FOUR, 15, 0.0f);
    table.Add("mk:fake_item_box",  RANK_FOUR, 10, 0.0f);
    table.Add("mk:boo",            RANK_FOUR, 5, 0.0f);
    table.Add("mk:mushroom",       RANK_FOUR, 10, 0.0f);

    table.Add("mk:banana",         RANK_FIVE, 30, 0.0f);
    table.Add("mk:green_shell",     RANK_FIVE, 20, 0.0f);
    table.Add("mk:triple_green_shell",   RANK_FIVE, 20, 0.0f);
    table.Add("mk:fake_item_box",  RANK_FIVE, 5, 0.0f);
    table.Add("mk:star",           RANK_FIVE, 5, 0.0f);
    table.Add("mk:mushroom",       RANK_FIVE, 20, 0.0f);

    table.Add("mk:banana",         RANK_SIX, 30, 0.0f);
    table.Add("mk:green_shell",     RANK_SIX, 20, 0.0f);
    table.Add("mk:triple_green_shell",   RANK_SIX, 20, 0.0f);
    table.Add("mk:fake_item_box",  RANK_SIX, 5, 0.0f);
    table.Add("mk:star",           RANK_SIX, 5, 0.0f);
    table.Add("mk:mushroom",       RANK_SIX, 20, 0.0f);

    table.Add("mk:banana",         RANK_SEVEN, 30, 0.0f);
    table.Add("mk:green_shell",     RANK_SEVEN, 20, 0.0f);
    table.Add("mk:triple_green_shell",   RANK_SEVEN, 20, 0.0f);
    table.Add("mk:star",           RANK_SEVEN, 10, 0.0f);
    table.Add("mk:mushroom",       RANK_SEVEN, 20, 0.0f);

    table.Add("mk:banana",         RANK_EIGHT, 25, 0.0f);
    table.Add("mk:green_shell",     RANK_EIGHT, 20, 0.0f);
    table.Add("mk:triple_green_shell",   RANK_EIGHT, 20, 0.0f);
    table.Add("mk:thunderbolt",    RANK_EIGHT, 1, 0.0f);
    table.Add("mk:star",           RANK_EIGHT, 10, 0.0f);
    table.Add("mk:mushroom",       RANK_EIGHT, 24, 0.0f);

    r.Add("mk:cpu_grand_prix", std::move(table));
}

void RegisterHardCPUItemTable(DataRegistry<RandomItemTable>& r) {
    RandomItemTable table;

    table.Add("mk:none",           RANK_ONE, 4, 0.0f);
    table.Add("mk:banana",         RANK_ONE, 30, 0.0f);
    table.Add("mk:banana_bunch",   RANK_ONE, 5, 0.0f);
    table.Add("mk:green_shell",     RANK_ONE, 30, 0.0f);
    table.Add("mk:triple_green_shell",   RANK_ONE, 5, 0.0f);
    table.Add("mk:red_shell",       RANK_ONE, 5, 0.0f);
    table.Add("mk:fake_item_box",  RANK_ONE, 10, 0.0f);
    table.Add("mk:boo",            RANK_ONE, 5, 0.0f);
    table.Add("mk:mushroom",       RANK_ONE, 10, 0.0f);

    table.Add("mk:banana",         RANK_TWO, 5, 0.0f);
    table.Add("mk:banana_bunch",   RANK_TWO, 5, 0.0f);
    table.Add("mk:green_shell",     RANK_TWO, 5, 0.0f);
    table.Add("mk:triple_green_shell",   RANK_TWO, 10, 0.0f);
    table.Add("mk:red_shell",       RANK_TWO, 15, 0.0f);
    table.Add("mk:triple_red_shell",     RANK_TWO, 20, 0.0f);
    table.Add("mk:fake_item_box",  RANK_TWO, 5, 0.0f);
    table.Add("mk:star",           RANK_TWO, 5, 0.0f);
    table.Add("mk:boo",            RANK_TWO, 5, 0.0f);
    table.Add("mk:mushroom",       RANK_TWO, 5, 0.0f);
    table.Add("mk:triple_mushroom",RANK_TWO, 15, 0.0f);
    table.Add("mk:super_mushroom", RANK_TWO, 5, 0.0f);

    table.Add("mk:banana",         RANK_THREE, 5, 0.0f);
    table.Add("mk:banana_bunch",   RANK_THREE, 3, 0.0f);
    table.Add("mk:triple_green_shell",   RANK_THREE, 10, 0.0f);
    table.Add("mk:red_shell",       RANK_THREE, 20, 0.0f);
    table.Add("mk:triple_red_shell",     RANK_THREE, 19, 0.0f);
    table.Add("mk:star",           RANK_THREE, 10, 0.0f);
    table.Add("mk:mushroom",       RANK_THREE, 5, 0.0f);
    table.Add("mk:triple_mushroom",RANK_THREE, 19, 0.0f);
    table.Add("mk:super_mushroom", RANK_THREE, 9, 0.0f);

    table.Add("mk:banana",         RANK_FOUR, 5, 0.0f);
    table.Add("mk:green_shell",     RANK_FOUR, 5, 0.0f);
    table.Add("mk:triple_green_shell",   RANK_FOUR, 5, 0.0f);
    table.Add("mk:red_shell",       RANK_FOUR, 15, 0.0f);
    table.Add("mk:triple_red_shell",     RANK_FOUR, 10, 0.0f);
    table.Add("mk:fake_item_box",  RANK_FOUR, 5, 0.0f);
    table.Add("mk:star",           RANK_FOUR, 15, 0.0f);
    table.Add("mk:boo",            RANK_FOUR, 5, 0.0f);
    table.Add("mk:mushroom",       RANK_FOUR, 5, 0.0f);
    table.Add("mk:triple_mushroom",RANK_FOUR, 20, 0.0f);
    table.Add("mk:super_mushroom", RANK_FOUR, 10, 0.0f);

    table.Add("mk:red_shell",       RANK_FIVE, 10, 0.0f);
    table.Add("mk:triple_red_shell",     RANK_FIVE, 20, 0.0f);
    table.Add("mk:blue_spiny_shell",      RANK_FIVE, 5, 0.0f);
    table.Add("mk:fake_item_box",  RANK_FIVE, 5, 0.0f);
    table.Add("mk:star",           RANK_FIVE, 15, 0.0f);
    table.Add("mk:mushroom",       RANK_FIVE, 10, 0.0f);
    table.Add("mk:triple_mushroom",RANK_FIVE, 25, 0.0f);
    table.Add("mk:super_mushroom", RANK_FIVE, 10, 0.0f);

    table.Add("mk:red_shell",       RANK_SIX, 5, 0.0f);
    table.Add("mk:triple_red_shell",     RANK_SIX, 25, 0.0f);
    table.Add("mk:blue_spiny_shell",      RANK_SIX, 10, 0.0f);
    table.Add("mk:star",           RANK_SIX, 20, 0.0f);
    table.Add("mk:mushroom",       RANK_SIX, 5, 0.0f);
    table.Add("mk:triple_mushroom",RANK_SIX, 25, 0.0f);
    table.Add("mk:super_mushroom", RANK_SIX, 10, 0.0f);

    table.Add("mk:green_shell",     RANK_SEVEN, 5, 0.0f);
    table.Add("mk:red_shell",       RANK_SEVEN, 5, 0.0f);
    table.Add("mk:triple_red_shell",     RANK_SEVEN, 25, 0.0f);
    table.Add("mk:blue_spiny_shell",      RANK_SEVEN, 10, 0.0f);
    table.Add("mk:star",           RANK_SEVEN, 30, 0.0f);
    table.Add("mk:mushroom",       RANK_SEVEN, 5, 0.0f);
    table.Add("mk:triple_mushroom",RANK_SEVEN, 10, 0.0f);
    table.Add("mk:super_mushroom", RANK_SEVEN, 10, 0.0f);

    table.Add("mk:triple_green_shell",   RANK_EIGHT, 5, 0.0f);
    table.Add("mk:red_shell",       RANK_EIGHT, 5, 0.0f);
    table.Add("mk:triple_red_shell",     RANK_EIGHT, 20, 0.0f);
    table.Add("mk:blue_spiny_shell",      RANK_EIGHT, 15, 0.0f);
    table.Add("mk:thunderbolt",    RANK_EIGHT, 10, 0.0f);
    table.Add("mk:star",           RANK_EIGHT, 30, 0.0f);
    table.Add("mk:triple_mushroom",RANK_EIGHT, 5, 0.0f);
    table.Add("mk:super_mushroom", RANK_EIGHT, 10, 0.0f);

    r.Add("mk:hard_cpu_grand_prix", std::move(table));
}

void RegisterVersus2PItemTable(DataRegistry<RandomItemTable>& r) {
    RandomItemTable table;

    table.Add("mk:banana",         RANK_ONE, 25, 0.0f);
    table.Add("mk:banana_bunch",   RANK_ONE, 10, 0.0f);
    table.Add("mk:green_shell",     RANK_ONE, 30, 0.0f);
    table.Add("mk:triple_green_shell",   RANK_ONE, 5, 0.0f);
    table.Add("mk:red_shell",       RANK_ONE, 5, 0.0f);
    table.Add("mk:fake_item_box",  RANK_ONE, 10, 0.0f);
    table.Add("mk:boo",            RANK_ONE, 5, 0.0f);
    table.Add("mk:mushroom",       RANK_ONE, 10, 0.0f);

    table.Add("mk:banana_bunch",   RANK_TWO, 5, 0.0f);
    table.Add("mk:triple_green_shell",   RANK_TWO, 5, 0.0f);
    table.Add("mk:red_shell",       RANK_TWO, 5, 0.0f);
    table.Add("mk:triple_red_shell",     RANK_TWO, 15, 0.0f);
    table.Add("mk:blue_spiny_shell",      RANK_TWO, 5, 0.0f);
    table.Add("mk:thunderbolt",    RANK_TWO, 15, 0.0f);
    table.Add("mk:star",           RANK_TWO, 15, 0.0f);
    table.Add("mk:triple_mushroom",RANK_TWO, 15, 0.0f);
    table.Add("mk:super_mushroom", RANK_TWO, 20, 0.0f);

    r.Add("mk:versus_2p", std::move(table));
}

void RegisterVersus3PItemTable(DataRegistry<RandomItemTable>& r) {
    RandomItemTable table;

    table.Add("mk:banana",         RANK_ONE, 35, 0.0f);
    table.Add("mk:banana_bunch",   RANK_ONE, 5, 0.0f);
    table.Add("mk:green_shell",     RANK_ONE, 30, 0.0f);
    table.Add("mk:red_shell",       RANK_ONE, 5, 0.0f);
    table.Add("mk:fake_item_box",  RANK_ONE, 10, 0.0f);
    table.Add("mk:boo",            RANK_ONE, 5, 0.0f);
    table.Add("mk:mushroom",       RANK_ONE, 10, 0.0f);

    table.Add("mk:banana",         RANK_TWO, 5, 0.0f);
    table.Add("mk:banana_bunch",   RANK_TWO, 5, 0.0f);
    table.Add("mk:triple_green_shell",   RANK_TWO, 10, 0.0f);
    table.Add("mk:red_shell",       RANK_TWO, 15, 0.0f);
    table.Add("mk:triple_red_shell",     RANK_TWO, 15, 0.0f);
    table.Add("mk:thunderbolt",    RANK_TWO, 5, 0.0f);
    table.Add("mk:fake_item_box",  RANK_TWO, 5, 0.0f);
    table.Add("mk:star",           RANK_TWO, 5, 0.0f);
    table.Add("mk:boo",            RANK_TWO, 5, 0.0f);
    table.Add("mk:mushroom",       RANK_TWO, 5, 0.0f);
    table.Add("mk:triple_mushroom",RANK_TWO, 20, 0.0f);
    table.Add("mk:super_mushroom", RANK_TWO, 5, 0.0f);

    table.Add("mk:red_shell",       RANK_THREE, 10, 0.0f);
    table.Add("mk:triple_red_shell",     RANK_THREE, 20, 0.0f);
    table.Add("mk:blue_spiny_shell",      RANK_THREE, 10, 0.0f);
    table.Add("mk:thunderbolt",    RANK_THREE, 15, 0.0f);
    table.Add("mk:star",           RANK_THREE, 15, 0.0f);
    table.Add("mk:triple_mushroom",RANK_THREE, 20, 0.0f);
    table.Add("mk:super_mushroom", RANK_THREE, 10, 0.0f);

    r.Add("mk:versus_3p", std::move(table));
}

void RegisterVersus4PItemTable(DataRegistry<RandomItemTable>& r) {
    RandomItemTable table;

    table.Add("mk:banana",         RANK_ONE, 35, 0.0f);
    table.Add("mk:banana_bunch",   RANK_ONE, 5, 0.0f);
    table.Add("mk:green_shell",     RANK_ONE, 30, 0.0f);
    table.Add("mk:red_shell",       RANK_ONE, 5, 0.0f);
    table.Add("mk:fake_item_box",  RANK_ONE, 10, 0.0f);
    table.Add("mk:boo",            RANK_ONE, 5, 0.0f);
    table.Add("mk:mushroom",       RANK_ONE, 10, 0.0f);

    table.Add("mk:banana",         RANK_TWO, 5, 0.0f);
    table.Add("mk:banana_bunch",   RANK_TWO, 5, 0.0f);
    table.Add("mk:green_shell",     RANK_TWO, 5, 0.0f);
    table.Add("mk:triple_green_shell",   RANK_TWO, 10, 0.0f);
    table.Add("mk:red_shell",       RANK_TWO, 15, 0.0f);
    table.Add("mk:triple_red_shell",     RANK_TWO, 15, 0.0f);
    table.Add("mk:fake_item_box",  RANK_TWO, 5, 0.0f);
    table.Add("mk:star",           RANK_TWO, 5, 0.0f);
    table.Add("mk:boo",            RANK_TWO, 5, 0.0f);
    table.Add("mk:mushroom",       RANK_TWO, 5, 0.0f);
    table.Add("mk:triple_mushroom",RANK_TWO, 25, 0.0f);

    table.Add("mk:banana_bunch",   RANK_THREE, 5, 0.0f);
    table.Add("mk:triple_green_shell",   RANK_THREE, 5, 0.0f);
    table.Add("mk:red_shell",       RANK_THREE, 10, 0.0f);
    table.Add("mk:triple_red_shell",     RANK_THREE, 15, 0.0f);
    table.Add("mk:blue_spiny_shell",      RANK_THREE, 5, 0.0f);
    table.Add("mk:thunderbolt",    RANK_THREE, 10, 0.0f);
    table.Add("mk:fake_item_box",  RANK_THREE, 5, 0.0f);
    table.Add("mk:star",           RANK_THREE, 10, 0.0f);
    table.Add("mk:mushroom",       RANK_THREE, 5, 0.0f);
    table.Add("mk:triple_mushroom",RANK_THREE, 25, 0.0f);
    table.Add("mk:super_mushroom", RANK_THREE, 5, 0.0f);

    table.Add("mk:triple_red_shell",     RANK_FOUR, 20, 0.0f);
    table.Add("mk:blue_spiny_shell",      RANK_FOUR, 10, 0.0f);
    table.Add("mk:thunderbolt",    RANK_FOUR, 15, 0.0f);
    table.Add("mk:star",           RANK_FOUR, 20, 0.0f);
    table.Add("mk:triple_mushroom",RANK_FOUR, 25, 0.0f);
    table.Add("mk:super_mushroom", RANK_FOUR, 10, 0.0f);

    r.Add("mk:versus_4p", std::move(table));
}

void RegisterBattleItemTable(DataRegistry<RandomItemTable>& r) {
    RandomItemTable table;
    for (int rank = RANK_ONE; rank <= RANK_EIGHT; rank++) {
        table.Add("mk:banana",           rank, 10, 0.0f);
        table.Add("mk:banana_bunch",     rank, 5, 0.0f);
        table.Add("mk:green_shell",       rank, 5, 0.0f);
        table.Add("mk:triple_green_shell",     rank, 20, 0.0f);
        table.Add("mk:red_shell",         rank, 20, 0.0f);
        table.Add("mk:fake_item_box",    rank, 15, 0.0f);
        table.Add("mk:star",             rank, 20, 0.0f);
        table.Add("mk:boo",              rank, 5, 0.0f);
    }

    r.Add("mk:battle", std::move(table));
}
