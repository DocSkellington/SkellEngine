#include "FileManager.h"

#include <fstream>

#include "Context.h"
#include "FileNotFound.h"

namespace engine {
    FileManager::FileManager(const Context &context) :
        m_context(context) {

    }

    nlohmann::json FileManager::getGameDescription() const {
        std::ifstream file("media/game.json");
        nlohmann::json j;
        file >> j;
        return j;
    }

    nlohmann::json FileManager::getEntityJSON(const char *entityType) const {
        return getEntityJSON(std::string(entityType));
    }

    nlohmann::json FileManager::getEntityJSON(const std::string &entityType) const {
        std::ifstream defaultFile("media/entities/data/" + entityType + ".json");
        std::ifstream level;
        if (!m_context.levelName.empty()) {
            level.open("media/levels/" + m_context.levelName + "/entities/data/" + entityType + ".json");
        }

        if (!defaultFile && !level) {
            throw FileNotFound(entityType + " is an unknown entity type");
        }
        else {
            nlohmann::json def, lev;
            if (defaultFile.is_open()) {
                defaultFile >> def;
            }
            if (level.is_open()) {
                std::cout << "Reading from level\n";
                level >> lev;
            }
            return fusion(def, lev);
        }
    }

    void FileManager::loadEntityDefaultLua(const std::string &entityType) const {
        if (!((*m_context.lua)[entityType].valid()))
            m_context.lua->safe_script_file("media/entities/scripts/" + entityType + ".lua");
    }

    sol::table FileManager::getEntityLevelLua(const std::string &entityType) const {
        if (!m_context.levelName.empty()) {
            if (!((*m_context.lua)[entityType].valid())) {
                // If there isn't any script linked to that entity, we load the level one
                m_context.lua->safe_script_file("media/levels/" + m_context.levelName + "/entities/scripts/" + entityType + ".lua");
            }
            else {
                // If there is already a script, we merge them
                sol::load_result script = m_context.lua->load_file("media/levels/" + m_context.levelName + "/entities/scripts" + entityType + ".lua");

                // TODO: read every value. If table, go in it. Copy/overwrite data in m_context.lua
            }
        }
    }

    nlohmann::json FileManager::fusion(const nlohmann::json &a, const nlohmann::json &b) const {
        nlohmann::json result(a);
        for (auto itr = b.begin() ; itr != b.end() ; ++itr) {
            result[itr.key()] = itr.value();
        }
        return result;
    }
}