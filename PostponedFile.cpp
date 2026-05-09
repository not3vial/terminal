#ifndef POSTPONEDFILE_CPP_INCLUDED
#define POSTPONEDFILE_CPP_INCLUDED

#include <iostream>
#include "Entry.cpp"
#include "Directory.cpp"
#include "SingleLineFile.cpp"
#include "MultiLineFile.cpp"

struct PostponedFile : public Entry {
    PostponedFile(const std::string& name) : Entry(name) {}

    std::string cat() const {
        return "<uninitialized file>";
    }

    void edit(std::istream& in, Directory* parent) {
        std::cout << "Choose file type for '" << name_ << "':\n"
                  << "  s - single line\n"
                  << "  m - multi line\n"
                  << "> ";
        std::string choice;
        std::getline(in, choice);

        if (!parent) {
            std::cerr << "Internal error: no current directory\n";
            return;
        }

        Entry* newFile = nullptr;
        if (choice == "s") {
            newFile = new SingleLineFile(name_);
        } else if (choice == "m") {
            newFile = new MultiLineFile(name_);
        } else {
            std::cerr << "Invalid choice, file remains uninitialized\n";
            return;
        }

        parent->removeChild(this);
        parent->addChild(newFile);

        newFile->edit(in, parent);

        delete this;
    }
};

#endif