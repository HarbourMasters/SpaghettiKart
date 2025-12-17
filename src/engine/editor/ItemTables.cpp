#include "ItemTables.h"
#include "port/Game.h"

extern "C" {
#include "math_util.h"
}

ItemTable::ItemTable(std::string name) : mName(name) {
    mDistanceEnabled = false;
}

bool ItemTable::Add(const std::string& resourceName, uint32_t rank, uint32_t weight, float distance) {
    const ItemInfo* info = gItemRegistry.GetInfo(resourceName);
    if (!info) {
        return false;
    }

    if (rank >= mTable.size()) {
        if (rank >= NUM_PLAYERS) {
            return false;
        }
        mTable.resize(rank + 1);
    }

    // Check if already exists
    for (const auto& [id, chance] : mTable[rank]) {
        if (id == info->Id) {
            printf("[ItemTables] %s already exists for this rank, skipping...\n", resourceName.c_str());
            return false;
        }
    }

    mTable[rank].push_back(std::make_pair(info->Id, ItemChance{weight, distance}));

    return true;
}

uint8_t ItemTable::Roll(uint32_t rank) const {
    if (rank >= mTable.size()) {
        return ITEM_INVALID;
    }

    // Items that could be selected
    const auto& itemList = mTable[rank];
    if (itemList.empty()) {
        return ITEM_INVALID;
    }

    // Calculate total weight
    uint32_t totalWeight = 0;
    for (const auto& [itemId, chance] : itemList) {
        if (IsBlacklisted(itemId)) {
            continue;
        }
        totalWeight += chance.Weight;
    }

    // Avoid division by zero
    if (totalWeight == 0) {
        return ITEM_INVALID;
    }

    // Pick a random number from zero to (totalWeight - 1)
    uint32_t rand = random_int(totalWeight - 1);

    // Cumulative sum over weights
    uint32_t accumulated = 0;
    for (const auto& [itemId, chance] : itemList) {
        if (IsBlacklisted(itemId)) {
            continue;
        }
        accumulated += chance.Weight;
        if (rand < accumulated) {
            return static_cast<uint8_t>(itemId);
        }
    }

    // Fallback (should not happen)
    [[unlikely]] return ITEM_INVALID;
}

void ItemTable::Blacklist(const std::string& resourceName) {
    const ItemInfo* info = gItemRegistry.GetInfo(resourceName);
    if (!info) {
        return;
    }

    // Find query
    for (const auto& ranks : mTable) {
        for (const auto& [id, chance] : ranks) {
            if (id == info->Id) {
                // Only add once
                if (std::find(mBlacklist.begin(), mBlacklist.end(), info->Id) == mBlacklist.end()) {
                    mBlacklist.push_back(info->Id);
                }
            }
        }
    }
}

bool ItemTable::IsBlacklisted(uint32_t itemId) const {
    return std::find(mBlacklist.begin(), mBlacklist.end(), itemId) != mBlacklist.end();
}

void ItemTable::ClearBlacklist() {
    mBlacklist.clear();
}
