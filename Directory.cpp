#ifndef DIRECTORY_CPP_INCLUDED
#define DIRECTORY_CPP_INCLUDED

#include <vector>
#include <algorithm>
#include <iostream>
#include "Entry.cpp"

struct Directory : public Entry {
    Directory* parent_;
    std::vector<Entry*> children_;

    Directory(const std::string& name, Directory* parent = nullptr)
        : Entry(name), parent_(parent) {}

    std::string displayName() const {
        return name_ + "/";
    }

    bool isDirectory() const { return true; }
    Directory* asDirectory() { return this; }

    void listAll(std::ostream& os) const {
        for (const auto* child : children_) {
            if (!child->isHidden())
                os << child->displayName() << "\n";
        }
    }

    std::string cat() const {
        return "cat: " + name_ + ": Is a directory";
    }

    void edit(std::istream&, Directory*) {
        std::cerr << "edit: cannot edit a directory\n";
    }

    void hide() {
        Entry::hide();
        for (auto* child : children_)
            child->hide();
    }

    void restore() {
        Entry::restore();
        for (auto* child : children_)
            child->restore();
    }

    void addChild(Entry* child) {
        children_.push_back(child);
    }

    void removeChild(Entry* child) {
        children_.erase(
            std::remove(children_.begin(), children_.end(), child),
            children_.end()
        );
    }

    Entry* find(const std::string& name, bool includeHidden = false) const {
        for (auto* child : children_) {
            if (!includeHidden && !child->isHidden()) {
                if (child->getName() == name)
                    return child;
            }

            else if (includeHidden) {
                if (child->getName() == name)
                    return child;
                if (!child->getName().empty() && child->getName()[0] == '.' && child->getName().substr(1) == name)
                    return child;
            }
        }
        return nullptr;
    }

    ~Directory() {
        for (auto* child : children_)
            delete child;
    }
};

#endif
