#pragma once

#include <Server/Entities/UObject.h>
#include <memory>
#include <unordered_map>

class ObjectManager {
public:
    // Создание нового объекта и возврат его ID
    uint64_t createObject(std::shared_ptr<UObject> object) {
        uint64_t id = next_id_++;
        objects_[id] = object;
        return id;
    }

    // Получение объекта по ID
    std::shared_ptr<UObject> getObjectById(uint64_t id) const {
        auto it = objects_.find(id);
        if (it != objects_.end()) {
            return it->second;
        }
        throw std::runtime_error("Object with ID " + std::to_string(id) + " not found");
    }

    // Удаление объекта по ID
    void removeObject(uint64_t id) {
        auto it = objects_.find(id);
        if (it != objects_.end()) {
            objects_.erase(it);
        } else {
            throw std::runtime_error("Object with ID " + std::to_string(id) + " not found");
        }
    }

    // Проверка существования объекта
    bool hasObject(uint64_t id) const {
        return objects_.find(id) != objects_.end();
    }

private:
    std::unordered_map<uint64_t, std::shared_ptr<UObject>> objects_; // Хранилище объектов
    uint64_t next_id_ = 1; // Счётчик для генерации уникальных ID
};
