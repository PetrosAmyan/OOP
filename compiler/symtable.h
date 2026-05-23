#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

struct Symbol {
    std::string name;
    std::string type;        
    int stackOffset;         
    bool isParameter;
    int paramIndex;          
    std::string reg;        
    bool inRegister;

    Symbol() : stackOffset(0), isParameter(false),
        paramIndex(-1), inRegister(false) {
    }
};

class SymbolTable {
private:
    std::vector<std::unordered_map<std::string, Symbol>> scopes;
    int currentStackOffset;   

public:
    SymbolTable();

    void enterScope();
    void exitScope();

    bool existsInCurrentScope(const std::string& name);

    bool exists(const std::string& name);

    Symbol* lookup(const std::string& name);

    
    void declare(const std::string& name, const std::string& type);
    void declareParameter(const std::string& name, int index);

    int allocateStackSlot();
    int getCurrentStackOffset() const { return currentStackOffset; }

    void setRegister(const std::string& name, const std::string& reg);
    void clearRegister(const std::string& name);
    std::string getRegister(const std::string& name);
};
