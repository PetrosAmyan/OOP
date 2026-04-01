#include "ir.h"
#include <stdexcept>

std::string InstGenerate::newreg() {
    return "R" + std::to_string(regcount++);
}

std::string InstGenerate::generate(Node* node) {
		if (node->type == NodeType::ASSIGN) {
    std::string rhs = generate(node->right);

    //std::string varName = node->left->var;
		std::string varName(1, node->left->var);
    commands.push_back({
        InstrType::MOV,
        varName,  
        rhs,
        ""
    });

    return varName;
}



    if (node->type == NodeType::NUMBER) {
        std::string reg = newreg();
        commands.push_back({InstrType::MOV, reg, std::to_string(node->value), ""});
        return reg;
    }

    if (node->type == NodeType::VARIABLE) {
    std::string reg = newreg();
    commands.push_back({
        InstrType::MOV,
        reg,
        std::string(1, node->var),
        ""
    });
    return reg;
}

    std::string left = generate(node->left);
    std::string right = generate(node->right);

    std::string reg = newreg();

    InstrType op;
    switch (node->op) {
        case OpType::ADD: op = InstrType::ADD; break;
        case OpType::SUB: op = InstrType::SUB; break;
        case OpType::MUL: op = InstrType::MUL; break;
        case OpType::DIV: op = InstrType::DIV; break;
        default:
            throw std::runtime_error("Invalid operator");
    }

    commands.push_back({op, reg, left, right});
    return reg;
}

std::vector<Instruction> InstGenerate::createIR(Node* root) {
    commands.clear();
    regcount = 0;
    generate(root);
    return commands;
}
