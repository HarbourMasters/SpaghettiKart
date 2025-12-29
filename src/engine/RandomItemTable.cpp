#include "RandomItemTable.h"
#include "defines.h"
#include "port/Game.h"

extern "C" {
#include "math_util.h"
}

RandomItemTable::RandomItemTable() {
    mDistanceEnabled = false;
}

bool RandomItemTable::Add(const std::string& resourceName, uint32_t rank, uint32_t weight, float distance) {
    const ItemInfo* info = gItemRegistry.GetInfo(resourceName);
    if (!info) {
        printf("[ItemTables] Failed to add %s to table,\n  item name passed into resourceName does not exist!\n", resourceName.c_str());
        return false;
    }

    if (rank >= mTable.size()) {
        if (rank >= NUM_PLAYERS) {
            printf("[ItemTables] Failed to add item table, rank value too high!\n");
            return false;
        }
        mTable.resize(rank + 1);
    }

    // Check if already exists
    for (const ItemEntry& entry : mTable[rank]) {
        if (entry.Id == info->Id) {
            printf("[ItemTables] %s already exists for this rank, skipping...\n", resourceName.c_str());
            return false;
        }
    }

    mTable[rank].push_back(ItemEntry{info->Id, weight, distance});

    return true;
}

uint8_t RandomItemTable::Roll(uint32_t rank) const {
    if (rank >= mTable.size()) {
        printf("[ItemTables] [Roll] Invalid rank input %d. Giving player a none item\n", rank);
        return ITEM_NONE;
    }

    // Items that could be selected for this rank
    const std::vector<ItemEntry>& itemList = mTable.at(rank);
    if (itemList.empty()) {
        printf("[ItemTables] [Roll] itemList empty. Giving player a none item\n");
        return ITEM_NONE;
    }

    // Calculate total weight
    uint32_t totalWeight = 0;
    for (const ItemEntry& entry : itemList) {
        if (this->IsBlacklisted(entry.Id)) {
            continue;
        }
        totalWeight += entry.Weight;
    }

    // Avoid division by zero
    if (totalWeight == 0) {
        printf("[ItemTables] [Roll] No items to give out for this rank. Giving player a none item\n");
        return ITEM_NONE;
    }

    // Pick a random number from zero to (totalWeight - 1)
    uint32_t rand = random_int(totalWeight - 1);

    // Cumulative sum over weights
    uint32_t accumulated = 0;
    for (const ItemEntry& entry : itemList) {
        if (this->IsBlacklisted(entry.Id)) {
            continue;
        }
        accumulated += entry.Weight;
        if (rand < accumulated) {
            return static_cast<uint8_t>(entry.Id);
        }
    }

    // Fallback (should not happen)
    printf("[ItemTables] [Roll] Reached unreachable code\n");
    [[unlikely]] return ITEM_NONE;
}

void RandomItemTable::Blacklist(const std::string& resourceName) {
    const ItemInfo* info = gItemRegistry.GetInfo(resourceName);
    if (!info) {
        return;
    }

    mBlacklist.insert(info->Id);
}

bool RandomItemTable::IsBlacklisted(uint32_t itemId) const {
    //return mBlacklist.contains(itemId);
    return mBlacklist.find(itemId) != mBlacklist.end();
}

void RandomItemTable::ClearBlacklist() {
    mBlacklist.clear();
}
