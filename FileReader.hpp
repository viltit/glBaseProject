/**
 * Very simple line by line reading of a text file
 * TODO: Thread safe? Async?
 * TODO: Add file reading modes (binary, text (ascii, utf8??))
*/

#include <string>
#include <fstream>
#include <exception>

class FileReadError : public std::runtime_error {
    public: FileReadError(const std::string& msg) : std::runtime_error(msg.c_str()) { }
};

class FileReader {
public:
    FileReader(/* read mode */);
    ~FileReader();

    std::string toString(const std::string& filePath);

private:
    std::string _filePath;
    std::ifstream _instream;
};