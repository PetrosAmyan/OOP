#include <iostream>
#include <fstream>
#include "lexer.h"
#include "parser.h"
#include "ir.h"
#include "assemb.h"
#include "bachinecode.h"
#include "print.h"
#include "processor.h"

int main()
{
    try
    {
    
        std::ifstream file("file1.txt");
        if (!file.is_open()) {
            std::cout << "Cannot open file1.txt\n";
            return 1;
        }

        std::string source((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());

        
        Lexer lexer(source);
        std::vector<Token> tokens = lexer.tokenize();

        
        Parser parser;
        auto ast = parser.parse(tokens);

        IR irGen(&parser.getSymbolTable());
        auto ir = irGen.generate(ast.get());
        printIR(ir);


        Assembler asmGen;
        auto asmCode = asmGen.generate(ir);
        printASM(asmCode);


        BinaryCode mcGen;
        auto machineCode = mcGen.generate(asmCode);
        printMC(machineCode);

        saveMachineCode(machineCode, "program.bin");
      
        ProcessorSimulator cpu;
        cpu.loadProgram("program.bin");
        cpu.run();

        std::cout << "\n compilation process and cpu is done\n";
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
