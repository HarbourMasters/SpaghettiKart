#include <libultraship/libultraship.h>
#include <utility>

struct ItemProbabilitiyTable {
    std::string ResourceName;
    uint8_t Chance;
};

class ItemTable {
public:
    ItemTable(std::string name);
    bool Add(const std::string& resourceName, uint32_t rank, uint32_t weight, float distance);
    uint8_t Roll(uint32_t rank) const;
    void Blacklist(const std::string& resourceName);
    bool IsBlacklisted(uint32_t id) const;
    void ClearBlacklist();

    std::string GetName() {
        return mName;
    }
protected:
    std::string mName;
    bool mDistanceEnabled;
    struct ItemChance {
        uint32_t Weight;
        float Distance;
    };

    // Rank         index        item_id,   chance
    std::vector<std::vector<std::pair<uint32_t, ItemChance>>> mTable;
    std::vector<uint32_t> mBlacklist;
};
