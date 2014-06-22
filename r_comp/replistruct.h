#ifndef REPLISTRUCT_H
#define REPLISTRUCT_H

#include <list>
#include <unordered_map>
#include <vector>

namespace r_comp {

class RepliMacro;
class RepliCondition;
class RepliStruct {
public:
    static std::unordered_map<std::string, RepliMacro *> RepliMacros;
    static std::unordered_map<std::string, int64_t> Counters;
    static std::list<RepliCondition *> Conditions;
    static uint64_t GlobalLine;
    static std::string GlobalFilename;

    enum Type {Root, Structure, Set, Atom, Directive, Condition, Development};
    Type type;
    std::string cmd;
    std::string tail;
    std::string label;
    std::string error;
    std::string fileName;
    uint64_t line;
    std::vector<RepliStruct *> args;
    RepliStruct *parent;

    RepliStruct(r_comp::RepliStruct::Type type);
    ~RepliStruct();

    void reset(); // remove rags that are objects.

    uint64_t getIndent(std::istream *stream);
    int64_t parse(std::istream *stream, uint64_t &curIndent, uint64_t &prevIndent, int64_t paramExpect = 0);
    bool parseDirective(std::istream *stream, uint64_t &curIndent, uint64_t &prevIndent);
    int64_t process();

    RepliStruct *findAtom(const std::string &name);
    RepliStruct *loadReplicodeFile(const std::string &filename);

    RepliStruct *clone() const;
    std::string print() const;
    std::string printError() const;

    friend std::ostream& operator<<(std::ostream &os, const RepliStruct &structure);
    friend std::ostream& operator<<(std::ostream &os, RepliStruct *structure);
private:
    RepliStruct(const RepliStruct &);
    RepliStruct();
};

class RepliMacro {
public:
    std::string name;
    RepliStruct *src;
    RepliStruct *dest;
    std::string error;

    RepliMacro(const std::string &name, RepliStruct *src, RepliStruct *dest);
    ~RepliMacro();

    uint64_t argCount();
    RepliStruct *expandMacro(RepliStruct *oldStruct);
};

class RepliCondition {
public:
    std::string name;
    bool reversed;

    RepliCondition(const std::string &name, bool reversed);
    ~RepliCondition();
    bool reverse();
    bool isActive(std::unordered_map<std::string, RepliMacro*> &RepliMacros, std::unordered_map<std::string, int64_t> &Counters);
};


}

#endif//REPLISTRUCT_H
