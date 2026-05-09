#ifndef MULTILINEFILE_CPP_INCLUDED
#define MULTILINEFILE_CPP_INCLUDED

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "Entry.cpp"

struct MultiLineFile : public Entry {
    std::vector<std::string> lines_;

    MultiLineFile(const std::string& name) : Entry(name) {}

    std::string cat() const {
        if (lines_.empty())
            return "<empty>";
        std::ostringstream oss;
        for (size_t i = 0; i < lines_.size(); ++i) {
            oss << (i + 1) << ": " << lines_[i];
            if (i != lines_.size() - 1) oss << '\n';
        }
        return oss.str();
    }

    void edit(std::istream& in, Directory*) {
        if (!lines_.empty()) {
            std::cout << "Current content:\n" << cat() << "\n";
        }

        std::cout << "Editing multi-line file. Choose mode:\n"
                  << "  1 - add lines (finish with -1)\n"
                  << "  2 - edit a specific line by number\n"
                  << "  -1 - finish editing\n";

        std::string choice;
        while (std::getline(in, choice)) {
            if (choice == "-1") break;

            if (choice == "1") {
                std::cout << "Enter lines (type -1 to finish adding):\n";
                std::string newLine;
                while (std::getline(in, newLine)) {
                    if (newLine == "-1") break;
                    lines_.push_back(newLine);
                }
                std::cout << "Lines added. Choose mode (1, 2, -1):\n";
            }
            else if (choice == "2") {
                if (lines_.empty()) {
                    std::cout << "File is empty, nothing to edit.\n";
                    std::cout << "Choose mode (1, 2, -1):\n";
                    continue;
                }
                std::cout << "Enter line number (1.." << lines_.size() << "): ";
                std::string numStr;
                if (std::getline(in, numStr)) {
                    // проверяем, что это число
                    int number = 0;
                    bool valid = true;
                    for (char c : numStr) {
                        if (c < '0' || c > '9') { valid = false; break; }
                        number = number * 10 + (c - '0');
                    }
                    if (valid && number >= 1 && number <= (int)lines_.size()) {
                        std::cout << "Enter new text for line " << number << ":\n";
                        std::string newText;
                        if (std::getline(in, newText)) {
                            lines_[number - 1] = newText;
                            std::cout << "Line updated.\n";
                        }
                    } else {
                        std::cout << "Invalid line number.\n";
                    }
                }
                std::cout << "Choose mode (1, 2, -1):\n";
            }
            else {
                std::cout << "Invalid option. Use 1, 2 or -1.\n";
            }
        }
    }
};

#endif