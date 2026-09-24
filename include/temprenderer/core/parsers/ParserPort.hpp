#pragma once

#include "core/logging/LoggerManager.hpp"
#include "nlohmann.json/json.hpp"
#include "toml++/toml.hpp"
#include <string>
#include <variant>

namespace temprenderer::core::parsers {
/**
 * @brief TODO
 */
template <typename T>
concept ConfigInteger =
    std::same_as<T, std::uint8_t> || std::same_as<T, std::uint16_t> ||
    std::same_as<T, std::uint32_t> || std::same_as<T, std::uint64_t> ||
    std::same_as<T, std::int8_t> || std::same_as<T, std::int16_t> ||
    std::same_as<T, std::int32_t> || std::same_as<T, std::int64_t>;
/**
 * @brief TODO
 */
class ConfigValue {
public:
  using Array = std::vector<ConfigValue>;
  using Table = std::map<std::string, ConfigValue>;
  using Storage = std::variant<std::monostate, bool, std::uint8_t,
                               std::uint16_t, std::uint32_t, std::uint64_t,
                               std::int8_t, std::int16_t, std::int32_t,
                               std::int64_t, float, std::string, Array, Table>;

  ConfigValue() = default;
  explicit ConfigValue(Storage value) : value_(std::move(value)) {};

  /**
   * @brief TODO
   *
   * @param defaultValue
   * @return
   */
  std::string asString(const std::string &defaultValue = "") const {
    if (auto *p = std::get_if<std::string>(&this->value_)) {
      return *p;
    }
    return defaultValue;
  }

  /**
   * @brief TODO
   *
   * @tparam T
   * @param defaultValue
   * @return
   */
  template <ConfigInteger T> T asInt(const T defaultValue = 0) const {
    if (auto *p = std::get_if<int64_t>(&value_)) {
      const int64_t v = *p;
      if constexpr (std::is_unsigned_v<T>) {
        if (v < 0) {
          return defaultValue;
        }
        if (static_cast<uint64_t>(v) >
            static_cast<uint64_t>(std::numeric_limits<T>::max())) {
          return defaultValue;
        }
      } else {
        if (v < static_cast<int64_t>(std::numeric_limits<T>::min()) ||
            v > static_cast<int64_t>(std::numeric_limits<T>::max())) {
          return defaultValue;
        }
      }
      return static_cast<T>(v);
    }
    return defaultValue;
  }

  /**
   * @brief TODO
   *
   * @param defaultValue
   * @return
   */
  float asFloat(const float defaultValue = 0.0F) const {
    if (auto *v = std::get_if<float>(&this->value_)) {
      return *v;
    }
    if (auto *v = std::get_if<int64_t>(&this->value_)) {
      return static_cast<float>(*v);
    }
    return defaultValue;
  }

  /**
   * @brief TODO
   *
   * @param defaultValue
   * @return
   */
  bool asBool(const bool defaultValue = false) const {
    if (auto *v = std::get_if<bool>(&this->value_)) {
      return *v;
    }
    return defaultValue;
  }

  /**
   * @brief TODO
   *
   * @param key
   * @return
   */
  const ConfigValue *get(const std::string &key) const {
    if (auto *t = std::get_if<Table>(&this->value_)) {
      auto it = t->find(key);
      if (it != t->end()) {
        return &it->second;
      }
    }
    LC_LOG_VERBOSE(logging::LogLevel::WARNING,
                   "Key " + key + " does not exist");
    return nullptr;
  }

  const Array &asArray() const {
    static constexpr Array empty{};
    if (auto *a = std::get_if<Array>(&value_))
      return *a;
    return empty;
  }

  /**
   * @brief TODO
   *
   * @return
   */
  bool isTable() const { return std::holds_alternative<Table>(this->value_); }
  /**
   * @brief TODO
   *
   * @return
   */
  bool isArray() const { return std::holds_alternative<Array>(this->value_); }

private:
  Storage value_;
};

/**
 * @brief TODO
 */
class ParserPort {
public:
  virtual ~ParserPort() = default;
  /**
   * @brief TODO
   *
   * @param path
   * @return
   */
  virtual ConfigValue parserFile(const std::string &path) = 0;
};

/**
 * @brief TODO
 */
class TOMLPlusPlusParserAdapter final : public ParserPort {
public:
  ConfigValue parserFile(const std::string &path) override {
    try {
      LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading config from " + path);
      const toml::table root = toml::parse_file(path);
      return convert(root);
    } catch (const toml::parse_error &err) {
      LC_LOG(logging::LogLevel::ERROR, "Failed to parse config file: " + path);
      throw std::runtime_error(std::string("Erro no arquivo de config: ") +
                               err.description().data());
    }
  }

private:
  /**
   * @brief TODO
   *
   * @param node
   * @return
   */
  static ConfigValue convert(const toml::node &node) {
    if (const auto *t = node.as_table()) {
      ConfigValue::Table table;
      for (auto &&[key, val] : *t) {
        table[std::string(key.str())] = convert(val);
      }
      return ConfigValue{table};
    }
    if (const auto *a = node.as_array()) {
      ConfigValue::Array arr;
      for (auto &&el : *a) {
        arr.push_back(convert(el));
      }
      return ConfigValue{arr};
    }
    if (auto v = node.value<std::string>()) {
      return ConfigValue{*v};
    }
    if (auto v = node.value<int64_t>()) {
      return ConfigValue{*v};
    }
    if (auto v = node.value<float>()) {
      return ConfigValue{*v};
    }
    if (auto v = node.value<bool>()) {
      return ConfigValue{*v};
    }
    return ConfigValue{};
  }
};

/**
 * @brief TODO
 */
class NlohmannJSONParserAdapter final : public ParserPort {
public:
  ConfigValue parserFile(const std::string &path) override {
    try {
      LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading config from " + path);
      std::ifstream in(path);
      nlohmann::json root;
      in >> root;
      return this->convert(root);
    } catch (const toml::parse_error &err) {
      LC_LOG(logging::LogLevel::ERROR, "Failed to parse config file: " + path);
      throw std::runtime_error(std::string("Erro no arquivo de config: ") +
                               err.description().data());
    }
  };

private:
  /**
   * @brief TODO
   *
   * @param node
   * @return
   */
  static ConfigValue convert(const nlohmann::json &node) {
    if (node.is_object()) {
      ConfigValue::Table table;
      for (const auto &[key, val] : node.items()) {
        table[key] = convert(val);
      }
      return ConfigValue{table};
    }
    if (node.is_array()) {
      ConfigValue::Array arr;
      for (const auto &el : node) {
        arr.push_back(convert(el));
      }
      return ConfigValue{arr};
    }
    if (node.is_string()) {
      return ConfigValue{node.get<std::string>()};
    }
    if (node.is_number_integer()) {
      return ConfigValue{node.get<int64_t>()};
    }
    if (node.is_number_float()) {
      return ConfigValue{node.get<float>()};
    }
    if (node.is_boolean()) {
      return ConfigValue{node.get<bool>()};
    }
    return ConfigValue{};
  }
};
} // namespace temprenderer::core::parsers
