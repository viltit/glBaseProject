#include "FileReader.hpp"

FileReader::FileReader() :
    _filePath   { },
    _instream   { }
{ }

FileReader::~FileReader() {
    _instream.close();
}

std::string FileReader::toString(const std::string& filePath) {

    if (_instream.is_open()) {
        _instream.close();
    }

    _filePath = filePath;
    _instream.open(filePath);

  	if (!_instream) {
        if (!_instream.is_open()) {
            throw FileReadError("Failed to open file" + _filePath);
        }
        else {
            throw FileReadError("Corrupted stream while reading file " + _filePath);
        }
    }

    std::string content;
	std::string line;

	while (std::getline(_instream, line)) {
		content += line + "\n";
	}
    _instream.close();
    return content;
}