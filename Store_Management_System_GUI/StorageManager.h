#pragma once
#include "StoredItem.h"
#include <unordered_map>
#include <map>
#include <memory>
#include <vector>

using namespace std;

class StorageManager {
private:
    unordered_map<string, shared_ptr<StoredItem>> itemById;
    map<string, shared_ptr<StoredItem>> itemByDescription;

public:
    void addItem(const shared_ptr<StoredItem>& item); //add item function

    shared_ptr<StoredItem> findById(const string& id) const; //find item

    void removeItem(const string& id); //remove item

    void listItemsByDescription() const; //list all items

    //Return all items, sorted by description
    vector<shared_ptr<StoredItem>> getAllItemsByDescription() const {
        vector<shared_ptr<StoredItem>> result;
        result.reserve(itemByDescription.size());
        for (auto const& kv : itemByDescription) {
            result.push_back(kv.second);
        }
        return result;
    }
};
