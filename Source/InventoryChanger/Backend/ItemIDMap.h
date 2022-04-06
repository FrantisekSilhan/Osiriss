#pragma once

#include <functional>
#include <list>
#include <optional>
#include <unordered_map>

#include <InventoryChanger/Inventory/Item.h> 

namespace inventory_changer::backend
{

class ItemIDMap {
public:
    [[nodiscard]] std::optional<std::list<inventory::Item_v2>::const_iterator> get(std::uint64_t itemID) const
    {
        const auto it = itemIDsIterators.find(itemID);
        return it != itemIDsIterators.end() ? std::make_optional(it->second) : std::nullopt;
    }

    [[nodiscard]] std::optional<std::uint64_t> getItemID(std::list<inventory::Item_v2>::const_iterator iterator) const
    {
        const auto it = iteratorsItemIDs.find(iterator);
        return it != iteratorsItemIDs.end() ? std::make_optional(it->second) : std::nullopt;
    }

    void add(std::uint64_t itemID, std::list<inventory::Item_v2>::const_iterator iterator)
    {
        if (const bool inserted = iteratorsItemIDs.try_emplace(iterator, itemID).second)
            itemIDsIterators.try_emplace(itemID, iterator);
    }

    std::optional<std::uint64_t> remove(std::list<inventory::Item_v2>::const_iterator iterator)
    {
        if (const auto it = iteratorsItemIDs.find(iterator); it != iteratorsItemIDs.end()) {
            const auto itemID = it->second;
            iteratorsItemIDs.erase(it);
            itemIDsIterators.erase(itemID);
            return itemID;
        }
        return {};
    }

private:
    struct ItemIteratorHasher {
        [[nodiscard]] std::size_t operator()(std::list<inventory::Item_v2>::const_iterator iterator) const noexcept
        {
            return std::hash<const inventory::Item_v2*>{}(std::to_address(iterator));
        }
    };

    std::unordered_map<std::uint64_t, std::list<inventory::Item_v2>::const_iterator> itemIDsIterators;
    std::unordered_map<std::list<inventory::Item_v2>::const_iterator, std::uint64_t, ItemIteratorHasher> iteratorsItemIDs;
};

}