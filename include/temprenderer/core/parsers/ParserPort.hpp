#pragma once
#include "core/logging/LoggerManager.hpp"
#include "nlohmann.json/json.hpp"
#include "toml++/toml.hpp"

#include <string>

namespace temprenderer::core::parsers {
class ParserPort {
public:
  virtual ~ParserPort() = default;
  virtual bool parserFile(const std::string &path) = 0;
  virtual float getFloat(const std::string &name) = 0;
};

class TOMLPlusPlusParserAdapter final : public ParserPort {
public:
  bool parserFile(const std::string &path) override {
    try {
      LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading config from " + path);
      this->table_ = toml::parse_file(path);
      return true;
    } catch (const toml::parse_error &err) {
      LC_LOG(logging::LogLevel::ERROR, "Failed to parse config file: " + path);
      throw std::runtime_error(std::string("Erro no arquivo de config: ") +
                               err.description().data());
    }
  }
  float getFloat(const std::string &name) override {
    return this->table_.at_path(name).as_floating_point()->value_or(0.0F);
  };
  ;

private:
  toml::table table_;
};

class NlohmannJSONParserAdapter final : public ParserPort {
public:
  bool parserFile(const std::string &path) override {
    try {
      LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading config from " + path);
      std::ifstream f(path);
      this->data_ = nlohmann::json::parse(f);
      return true;
    } catch (const toml::parse_error &err) {
      LC_LOG(logging::LogLevel::ERROR, "Failed to parse config file: " + path);
      throw std::runtime_error(std::string("Erro no arquivo de config: ") +
                               err.description().data());
    }
  };

private:
  nlohmann::json data_;
};
} // namespace temprenderer::core::parsers
