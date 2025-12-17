#pragma once

#include <libultraship/libultraship.h>
#include <utility>

/**
 * Uses weighted random picking
 * 
 * This allows stock game to work identically
 * while allowing mods to add items which equalizes
 * the stock items without requiring addition chance editing
 */

class RandomItemTable {
public:
    RandomItemTable();
    bool Add(const std::string& resourceName, uint32_t rank, uint32_t weight, float distance);
    uint8_t Roll(uint32_t rank) const;
    void Blacklist(const std::string& resourceName);
    bool IsBlacklisted(uint32_t id) const;
    void ClearBlacklist();
protected:
    bool mDistanceEnabled;
    struct ItemEntry {
        uint32_t Id;
        uint32_t Weight;
        float Distance;
    };

    // Rank         index        item_id,   chance
    std::vector<std::vector<ItemEntry>> mTable;
    std::unordered_set<uint32_t> mBlacklist;
};
