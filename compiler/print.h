#pragma once
#include <vector>
#include <string>
#include "ir.h"
#include "assemb.h"

void printIR(const std::vector<IRInstruction>& ir);
void printASM(const std::vector<AssemblyInstruction>& asmCode);
void printMC(const std::vector<uint32_t>& mc);
void saveMachineCode(const std::vector<uint32_t>& mc, const std::string& filename);
