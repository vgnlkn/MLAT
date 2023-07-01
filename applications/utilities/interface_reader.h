#ifndef MLAT_INTERFACE_READER_H
#define MLAT_INTERFACE_READER_H

#include <string>

/**
 * \class IReader
 * \brief The IReader class represents the interface for classes that reads content.
 **/
class IReader
{
public:
    //! Constructor
    IReader() = default;
    //! It is responsible for retrieving and returning the content from the specified source.
    virtual std::string getContent(const std::string& str) = 0;
};

#endif //MLAT_INTERFACE_READER_H
