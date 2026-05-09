#ifndef ENTRY_CPP_INCLUDED
#define ENTRY_CPP_INCLUDED

#include <iostream>
#include <string>

struct Directory;

struct Entry {
    std::string name_;

    Entry(const std::string& name) : name_(name) {}

    virtual std::string displayName() const {
        return name_;
    }

    virtual bool isDirectory() const { return false; }
    virtual Directory* asDirectory() { return nullptr; }

    virtual void listAll(std::ostream& os) const {}
    virtual std::string cat() const { return ""; }
    virtual void edit(std::istream& in, Directory* currentDir) {}

    virtual void hide() {
        if (!name_.empty() && name_[0] != '.')
            name_ = "." + name_;
    }

    virtual void restore() {
        if (!name_.empty() && name_[0] == '.')
            name_ = name_.substr(1);
    }

    virtual bool isHidden() const {
        return !name_.empty() && name_[0] == '.';
    }

    void setName(const std::string& newName) { name_ = newName; }
    std::string getName() const { return name_; }

    virtual ~Entry() {}
};

#endif