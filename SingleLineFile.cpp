#ifndef SINGLELINEFILE_CPP_INCLUDED
#define SINGLELINEFILE_CPP_INCLUDED

#include <iostream>
#include <string>
#include "Entry.cpp"

struct SingleLineFile : public Entry {
    std::string content_;

    SingleLineFile(const std::string& name) : Entry(name) {}

    std::string cat() const {
        return content_.empty() ? "<empty>" : content_;
    }

    void edit(std::istream& in, Directory*) {
        std::cout << "Enter new single line:\n";
        std::string line;
        if (std::getline(in, line))
            content_ = line;
    }
};

#endif