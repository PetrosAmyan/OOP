#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "ir.h"

class Processor {
private:
    std::unordered_map<std::string, int> registers;
    std::unordered_map<std::string, int> varmemory;

    int getValue(const std::string& s);

public:
    void setVariable(const std::string& name, int value);
    void execute(const std::vector<Instruction>& commands);
    void printRegisters();
};
