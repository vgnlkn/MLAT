#ifndef MLAT_INTERFACE_READER_H
#define MLAT_INTERFACE_READER_H

#include <string>

class IReader
{
public:
    IReader() = default;
    virtual std::string getContent(const std::string& str) = 0;
};

#endif //MLAT_INTERFACE_READER_H
