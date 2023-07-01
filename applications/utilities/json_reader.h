#ifndef MLAT_JSON_READER_H
#define MLAT_JSON_READER_H

#include <fstream>
#include <json.hpp>
#include <interface_reader.h>


/**
 * \class JSONReader
 * \brief The JSONReader class inherits from the IReader interface,
 * providing an implementation for reading content from a JSON file.
 **/
class JSONReader final : public IReader
{
public:
    //! Constructor
    explicit JSONReader(const std::string& file_name) : _config_file(file_name) { _config_file >> _config; }
    //! Represents the key in the JSON object, and returns the corresponding value as a string from the JSON configuration
    std::string getContent(const std::string& str) final { return _config[str]; }
private:
    //! JSON file
    std::ifstream _config_file;
    //! JSON object
    nlohmann::json _config;
};

#endif //MLAT_JSON_READER_H
