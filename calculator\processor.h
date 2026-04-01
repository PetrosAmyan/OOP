#include <iostream>
#include "ast.h"
#include "ir.h"
#include "processor.h"

int main() {
    std::string input;
    std::cout << "please enter expression: ";
    std::getline(std::cin, input);

    Lexer lexer(input);
    std::vector<Token> tokens = lexer.tokenize();
    
    Parser parser;
    Node* root = parser.parse(tokens);

    
    std::set<char> vars;
    parser.collectVariables(root, vars);


    SymbolTable symTable;

    for (char v : vars) {
        int val;
        std::cout << "Enter value for " << v << " variable  ";
        std::cin >> val;
        symTable.set(v, val);
    }

    
    InstGenerate ir;
    std::vector<Instruction> commands = ir.createIR(root);

    Processor proc;

    for (char v : vars) {
        proc.setVariable(std::string(1, v), symTable.get(v));
    }

    proc.execute(commands);
    proc.printRegisters();

    return 0;
}
