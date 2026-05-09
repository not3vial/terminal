#ifndef TERMINAL_CPP_INCLUDED
#define TERMINAL_CPP_INCLUDED

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "Directory.cpp"
#include "PostponedFile.cpp"

struct Terminal {
    Directory* root_;
    std::vector<Directory*> cwd_path_;

    Terminal() {
        root_ = new Directory("root", nullptr);
        cwd_path_.push_back(root_);
    }

    ~Terminal() {
        delete root_;
    }

    Directory* getCurrentDirectory() const {
        return cwd_path_.back();
    }

    std::string prompt() const {
        std::string pathStr = "/";
        for (size_t i = 1; i < cwd_path_.size(); ++i) {
            pathStr += cwd_path_[i]->getName() + "/";
        }
        return pathStr + "> ";
    }

    void run() {
        std::string line;
        std::cout << "Virtual Terminal. Type 'help' for commands.\n";
        while (true) {
            std::cout << prompt();
            if (!std::getline(std::cin, line))
                break;

            std::istringstream iss(line);
            std::string cmd;
            iss >> cmd;
            std::string args;
            std::getline(iss, args);
            size_t start = args.find_first_not_of(" \t");
            if (start != std::string::npos)
                args = args.substr(start);
            else
                args.clear();

            if (cmd == "exit") break;
            else if (cmd == "help") printHelp();
            else if (cmd == "ls") ls();
            else if (cmd == "cd") cd(args);
            else if (cmd == "cat") cat(args);
            else if (cmd == "touch") touch(args);
            else if (cmd == "mkdir") mkdir(args);
            else if (cmd == "rm") rm(args);
            else if (cmd == "restore") restore(args);
            else if (cmd == "mv") {
                std::istringstream argStream(args);
                std::string oldName, newName;
                argStream >> oldName >> newName;
                mv(oldName, newName);
            }
            else if (cmd == "edit") edit(args);
            else std::cerr << "Unknown command: " << cmd << "\n";
        }
    }

    void printHelp() const {
        std::cout <<
            "help          - this message\n"
            "exit          - quit\n"
            "ls            - list contents\n"
            "cd <dir>      - change directory\n"
            "cat <file>    - display file\n"
            "touch <name>  - create file (type set on first edit)\n"
            "mkdir <name>  - create directory\n"
            "rm <name>     - hide entity\n"
            "restore <name>- unhide entity\n"
            "mv <old> <new>- rename\n"
            "edit <file>   - edit file\n";
    }

    void ls() const {
        cwd_path_.back()->listAll(std::cout);
    }

    void cd(const std::string& path) {
        if (path.empty()) return;
        std::string p = path;
        Directory* current = cwd_path_.back();

        if (p[0] == '/') {
            cwd_path_.clear();
            cwd_path_.push_back(root_);
            current = root_;
            p = p.substr(1);
        }

        std::istringstream iss(p);
        std::string token;
        while (std::getline(iss, token, '/')) {
            if (token.empty() || token == ".") continue;
            if (token == "..") {
                if (cwd_path_.size() > 1) {
                    cwd_path_.pop_back();
                    current = cwd_path_.back();
                }
                continue;
            }

            Entry* child = current->find(token, false);
            if (!child || !child->isDirectory()) {
                std::cerr << "cd: " << token << ": No such directory\n";
                while (cwd_path_.back() != current)
                    cwd_path_.pop_back();
                return;
            }
            Directory* dir = static_cast<Directory*>(child);
            cwd_path_.push_back(dir);
            current = dir;
        }
    }

    void cat(const std::string& name) const {
        Entry* ent = cwd_path_.back()->find(name, false);
        if (!ent) {
            std::cerr << "cat: " << name << ": No such file\n";
            return;
        }
        std::cout << ent->cat() << std::endl;
    }

    void touch(const std::string& name) {
        Directory* cwd = cwd_path_.back();
        if (cwd->find(name, true)) {
            std::cerr << "touch: " << name << ": already exists\n";
            return;
        }
        cwd->addChild(new PostponedFile(name));
    }

    void mkdir(const std::string& name) {
        Directory* cwd = cwd_path_.back();
        if (cwd->find(name, true)) {
            std::cerr << "mkdir: " << name << ": already exists\n";
            return;
        }
        cwd->addChild(new Directory(name, cwd));
    }

    void rm(const std::string& name) {
        Entry* ent = cwd_path_.back()->find(name, false);
        if (!ent) {
            std::cerr << "rm: " << name << ": not found\n";
            return;
        }
        ent->hide();
    }

    void restore(const std::string& name) {
        Entry* ent = cwd_path_.back()->find(name, true);
        if (!ent) {
            std::cerr << "restore: " << name << ": not found\n";
            return;
        }
        if (!ent->isHidden()) {
            std::cerr << "restore: " << name << ": is not hidden\n";
            return;
        }
        ent->restore();
    }

    void mv(const std::string& oldName, const std::string& newName) {
        if (oldName.empty() || newName.empty()) {
            std::cerr << "mv: usage: mv <old> <new>\n";
            return;
        }
        Directory* cwd = cwd_path_.back();
        Entry* ent = cwd->find(oldName, false);
        if (!ent) {
            std::cerr << "mv: " << oldName << ": not found\n";
            return;
        }
        if (cwd->find(newName, true)) {
            std::cerr << "mv: " << newName << ": already exists\n";
            return;
        }
        ent->setName(newName);
    }

    void edit(const std::string& name) {
        Entry* ent = cwd_path_.back()->find(name, false);
        if (!ent) {
            std::cerr << "edit: " << name << ": not found\n";
            return;
        }
        if (ent->isDirectory()) {
            std::cerr << "edit: " << name << ": is a directory\n";
            return;
        }
        ent->edit(std::cin, cwd_path_.back());
    }
};

#endif