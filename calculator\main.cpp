#include <iostream>
#include "ast.h"
#include "ir.h"
#include "processor.h"

int main() {
    std::string input;
    std::cout << "please enter expression: ";
    std::getline(std::cin, input);

    try {
        
        Calculator_AST parser;
        Node* root = parser.parse(input);

        InstGenerate ir;
        std::vector<Instruction> commands = ir.createIR(root);

        Processor cpu;

        cpu.setVariable("a", 5);
        cpu.setVariable("b", 10);

        cpu.execute(commands);

        cpu.printRegisters();

        free_tree_recursive(root);

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}
