#pragma once 
#include <vector>
#include <string>
#include "ast.h"

enum class InstrType {
    MOV, ADD, SUB, MUL, DIV
};

struct Instruction {
    InstrType type;
    std::string dest;
    std::string source1;
    std::string source2;
};

class InstGenerate {
private:
    std::vector<Instruction> commands;
    int regcount = 0;

    std::string newreg();
    std::string generate(Node* node);

public:
    std::vector<Instruction> createIR(Node* root);
};
