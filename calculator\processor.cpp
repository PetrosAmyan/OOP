#include "processor.h"
#include <iostream>
#include <cctype>

int Processor::getValue(const std::string& s) {
    if (isdigit(s[0]))
        return std::stoi(s);

    if (s[0] == 'R')
        return registers[s];

    return varmemory[s];
}

void Processor::setVariable(const std::string& name, int value) {
    varmemory[name] = value;
}

void Processor::execute(const std::vector<Instruction>& commands) {
    for (const auto& instr : commands) {

        switch (instr.type) {

							case InstrType::MOV:
    									if (instr.dest[0] == 'R') {
        											// destination is register
        											registers[instr.dest] = getValue(instr.source1);
    									} else {
        												// destination is variable
        							varmemory[instr.dest] = getValue(instr.source1);
    									}
    						break;
            case InstrType::ADD:
                registers[instr.dest] =
                    getValue(instr.source1) + getValue(instr.source2);
                break;

            case InstrType::SUB:
                registers[instr.dest] =
                    getValue(instr.source1) - getValue(instr.source2);
                break;

            case InstrType::MUL:
                registers[instr.dest] =
                    getValue(instr.source1) * getValue(instr.source2);
                break;

            case InstrType::DIV:
                registers[instr.dest] =
                    getValue(instr.source1) / getValue(instr.source2);
                break;
        }
    }
}

void Processor::printRegisters() {
    std::cout << "\nFinal Registers:\n";
    for (auto& [k, v] : registers) {
        std::cout << k << " = " << v << "\n";
    }
}
