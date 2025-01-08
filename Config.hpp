#include <algorithm>
#include <fstream>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string>

namespace bre {
    class Config {
    public:
        [[nodiscard]] static Config &GetInstance() {
            static std::unique_ptr<Config> instance(new Config());
            return *instance;
        }

        std::optional<std::string> Get(const std::string &key) const {
            auto it = configMap.find(key);
            if (it != configMap.end()) {
                return it->second;
            }
            return std::nullopt;
        }

    private:
        Config() {
            loaConfig();
        }
        Config(const Config &) = delete;
        Config &operator=(const Config &) = delete;

        std::map<std::string, std::string> configMap;

        void loaConfig() {
            std::ifstream configFile("config.txt");

            if (!configFile.is_open()) {
                throw std::runtime_error("Fail to open config file!");
            }

            std::string line;
            while (std::getline(configFile, line)) {
                std::istringstream iss(line);

                std::string key, value;
                if (std::getline(iss, key, ':') && std::getline(iss, value, ':')) {
                    roveAllspace(key);
                    roveAllspace(value);
                    configMap[key] = value;
                }
            }
        }

        void roveAllspace(std::string &str) {
            str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
        }
    };
};