#ifndef MLAT_JSON_READER_H
#define MLAT_JSON_READER_H

#include <fstream>
#include <json.hpp>
#include <interface_reader.h>

class JSONReader final : public IReader
{
public:
    explicit JSONReader(const std::string& file_name) : _config_file(file_name) { _config_file >> _config; }
    std::string getContent(const std::string& str) final { return _config[str]; }
private:
    std::ifstream _config_file;
    nlohmann::json _config;
};

#endif //MLAT_JSON_READER_H
