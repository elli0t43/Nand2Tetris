#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <regex>
#include <unordered_map>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <bitset>
#include <cctype>
#include "colors.h"


struct symbolTableRecord{
    std::string symbolName;
    int address;
    std::string addressBinary;

};

struct instructionTableRecord{
    std::string instruction;
    int addressBinaryInt;
    std::string addressBinary;
};

void appendToFile(std::string content, std::string fileName, bool append){
    std::ofstream outFile;
    if(append){
        outFile.open(fileName, std::ios_base::app);
    }else{
        outFile.open(fileName);
    }
    outFile << content;
    if(append){
        outFile << "\n";
    }else{
    }
    outFile.close();
}

std::string labelStrip(std::string inputString){
    std::string strippedSymbol{""}; 
    int inputLength = inputString.length();
    for (int i{1}; i<inputLength-1; ++i){
        strippedSymbol +=inputString[i];
    }
    return strippedSymbol;
}

std::string aInstructionStrip(std::string inputString){
    std::string strippedSymbol{""}; 
    int inputLength = inputString.length();
    for (int i{1}; i<inputLength; ++i){
        strippedSymbol +=inputString[i];
    }
    return strippedSymbol;
}

// TODO: need to check these are bug free or not
std::string getFirstPart(std::string inputString, std::string key){
    std::string output{""};
    int inputLength = inputString.length();
    int keyLocation = inputString.find(key);
    for (int i{0}; i<keyLocation; i++){
        output += inputString[i];
    }
    return output;
}

// TODO: need to check if these are bug free or not
std::string getSecondPart(std::string inputString, std::string key){
    std::string output{""};
    int inputLength = inputString.length();
    int keyLocation = inputString.find(key);
    for (int i{keyLocation+1}; i<inputLength; i++){
        output += inputString[i];
    }
    return output;
}


bool isComment(std::string inputString){
    if (inputString.front() == '/' && inputString[1] == '/' || inputString.front() == '/' && inputString[1] == '*' || inputString.front() == '*' && inputString.back() == '/'){
        return true;
    }else{
        return false;
    }
}

bool isAInstruction(std::string inputString){
    if (inputString.front() == '@'){
        return true;
    }else{
        return false;
    }
}

bool isLabel(std::string inputString){
    std::regex labelPattern("\\([a-zA-Z\\_].*");
    //std::regex labelPattern("/[a-zA-Z_].*/gm");
    if (inputString.front() == '(' && inputString.back() == ')'){
        if (std::regex_match(inputString, labelPattern)){
            return true; 
        }else{
            return false;
        }
    }else{
        return false;
    }
}

void remove_whitespace(std::string inputString){
    // inputString.erase(remove(inputString.begin(), inputString.end(), " "), inputString.end());
    inputString.erase(std::remove_if(inputString.begin(), inputString.end(), isspace), inputString.end());
}

// fancy name for string concatinate LMAO
std::string construct_machine_code(std::string string1, std::string string2, bool aFlag, bool jumpFlag){
    std::string output;
    output.append("111");
    if (aFlag == true){
        if(jumpFlag){
            output.append("1");
            output.append(string1);
            output.append("000");
            output.append(string2);
        }else{
            output.append("1");
            output.append(string1);
            output.append(string2);
            output.append("000");
        }
    }else{
        if(jumpFlag){
            output.append("0");
            output.append(string1);
            output.append("000");
            output.append(string2);
        }else{
            output.append("0");
            output.append(string1);
            output.append(string2);
            output.append("000");
        }
    }
    return output;
}


void validate_asm(std::string inputFileName, std::string outputFileName){
    std::unordered_map<std::string, symbolTableRecord> symbolTableMap;
    std::unordered_map<std::string, instructionTableRecord> cInstructionTableMap;
    std::unordered_map<std::string, instructionTableRecord> dInstructionTableMap;
    std::unordered_map<std::string, instructionTableRecord> jInstructionTableMap;

    symbolTableMap["R0"] = symbolTableRecord{ "R0", 0, std::bitset<16>(0).to_string() };
    symbolTableMap["R1"] = symbolTableRecord{ "R1", 1, std::bitset<16>(1).to_string() };
    symbolTableMap["R2"] = symbolTableRecord{ "R2", 2, std::bitset<16>(2).to_string() };
    symbolTableMap["R3"] = symbolTableRecord{ "R3", 3, std::bitset<16>(3).to_string() };
    symbolTableMap["R4"] = symbolTableRecord{ "R4", 4, std::bitset<16>(4).to_string() };
    symbolTableMap["R5"] = symbolTableRecord{ "R5", 5, std::bitset<16>(5).to_string() };
    symbolTableMap["R6"] = symbolTableRecord{ "R6", 6, std::bitset<16>(6).to_string() };
    symbolTableMap["R7"] = symbolTableRecord{ "R7", 7, std::bitset<16>(7).to_string() };
    symbolTableMap["R8"] = symbolTableRecord{ "R8", 8, std::bitset<16>(8).to_string() };
    symbolTableMap["R9"] = symbolTableRecord{ "R9", 9, std::bitset<16>(9).to_string() };
    symbolTableMap["R10"] = symbolTableRecord{ "R10", 10, std::bitset<16>(10).to_string() };
    symbolTableMap["R11"] = symbolTableRecord{ "R11", 11, std::bitset<16>(11).to_string() };
    symbolTableMap["R12"] = symbolTableRecord{ "R12", 12, std::bitset<16>(12).to_string() };
    symbolTableMap["R13"] = symbolTableRecord{ "R13", 13, std::bitset<16>(13).to_string() };
    symbolTableMap["R14"] = symbolTableRecord{ "R14", 14, std::bitset<16>(14).to_string() };
    symbolTableMap["R15"] = symbolTableRecord{ "R15", 15, std::bitset<16>(15).to_string() };
    symbolTableMap["SCREEN"] = symbolTableRecord{ "SCREEN", 16384, std::bitset<16>(16384).to_string() };
    symbolTableMap["KBD"] = symbolTableRecord{ "KBD", 24576, std::bitset<16>(24576).to_string() }; 
    symbolTableMap["SP"] = symbolTableRecord{ "SP", 0, std::bitset<16>(0).to_string() };
    symbolTableMap["LCL"] = symbolTableRecord{ "LCL", 1, std::bitset<16>(1).to_string() };
    symbolTableMap["ARG"] = symbolTableRecord{ "SP", 2, std::bitset<16>(2).to_string() };
    symbolTableMap["THIS"] = symbolTableRecord{ "LCL", 3, std::bitset<16>(3).to_string() };
    symbolTableMap["THAT"] = symbolTableRecord{ "SP", 4, std::bitset<16>(4).to_string() };



    cInstructionTableMap["c_flag0"] = instructionTableRecord{ "c_flag0", 0, "0" };
    cInstructionTableMap["c_flag1"] = instructionTableRecord{ "c_flag1", 1, "1" };
    cInstructionTableMap["a_flag0"] = instructionTableRecord{ "a_flag0", 0, "0" };
    cInstructionTableMap["a_flag1"] = instructionTableRecord{ "a_flag1", 1, "1" };
    cInstructionTableMap["non_used"] = instructionTableRecord{ "non_used", 11, "11" };

    cInstructionTableMap["0"] = instructionTableRecord{ "0", 101010 ,"101010" };
    cInstructionTableMap["1"] = instructionTableRecord{ "1", 111111 ,"111111" };
    cInstructionTableMap["-1"] = instructionTableRecord{ "-1", 111010 ,"111010" };
    cInstructionTableMap["D"] =  instructionTableRecord{ "D", 001100 ,"001100" };
    cInstructionTableMap["A"] = instructionTableRecord{ "A", 110000 ,"110000" };
    cInstructionTableMap["M"] = instructionTableRecord{ "M", 110000 ,"110000" };
    cInstructionTableMap["!D"] = instructionTableRecord{ "!D", 001101 ,"001101" };
    cInstructionTableMap["!A"] = instructionTableRecord{ "!A", 110001 ,"110001" };
    cInstructionTableMap["!M"] = instructionTableRecord{ "!M", 110001 ,"110001" };
    cInstructionTableMap["-D"] = instructionTableRecord{ "-D", 001111 ,"001111" };
    cInstructionTableMap["-A"] = instructionTableRecord{ "-A", 110111 ,"110011" };
    cInstructionTableMap["-M"] = instructionTableRecord{ "-M", 110011 ,"110011" };
    cInstructionTableMap["D+1"] = instructionTableRecord{ "D+1", 011111 ,"011111" };
    cInstructionTableMap["A+1"] = instructionTableRecord{ "A+1", 110111 ,"110111" };
    cInstructionTableMap["M+1"] = instructionTableRecord{ "M+1", 110111 ,"110111" };
    cInstructionTableMap["D-1"] = instructionTableRecord{ "D-1", 001110 ,"001110" };
    cInstructionTableMap["A-1"] = instructionTableRecord{ "A-1", 110010 ,"110010" };
    cInstructionTableMap["M-1"] = instructionTableRecord{ "M-1", 110010 ,"110010" };
    cInstructionTableMap["D+A"] = instructionTableRecord{ "D+A", 000010 ,"000010" };
    cInstructionTableMap["D-A"] = instructionTableRecord{ "D-A", 010011 ,"010011" };
    cInstructionTableMap["A-D"] = instructionTableRecord{ "A-D", 000111 ,"000111" };
    cInstructionTableMap["D&A"] = instructionTableRecord{ "D&A", 000000 ,"000000" };
    cInstructionTableMap["D|A"] = instructionTableRecord{ "D|A", 010101 ,"010101" };
    cInstructionTableMap["D+M"] = instructionTableRecord{ "D+M", 000010 ,"000010" };
    cInstructionTableMap["D-M"] = instructionTableRecord{ "D-M", 010010 ,"010011" };
    cInstructionTableMap["M-D"] = instructionTableRecord{ "M-D", 000111 ,"000111" };
    cInstructionTableMap["D&M"] = instructionTableRecord{ "D&M", 000000 ,"000000" };
    cInstructionTableMap["D|M"] = instructionTableRecord{ "D|M", 010101 ,"010101" };


    dInstructionTableMap["M"] = instructionTableRecord{ "M", 001, "001" };
    dInstructionTableMap["D"] = instructionTableRecord{ "D", 010, "010" };
    dInstructionTableMap["MD"] = instructionTableRecord{ "MD", 011, "011" };
    dInstructionTableMap["A"] = instructionTableRecord{ "A", 100, "100" };
    dInstructionTableMap["AM"] = instructionTableRecord{ "AM", 101, "101" };
    dInstructionTableMap["AD"] = instructionTableRecord{ "AD", 110, "110" };
    dInstructionTableMap["AMD"] = instructionTableRecord{ "AMD", 111, "111" };

    jInstructionTableMap["JGT"] = instructionTableRecord{ "JGT", 001, "001" };
    jInstructionTableMap["JEQ"] = instructionTableRecord{ "JEQ", 010, "010" };
    jInstructionTableMap["JGE"] = instructionTableRecord{ "JGE", 011, "011" };
    jInstructionTableMap["JLT"] = instructionTableRecord{ "JLT", 100, "100" };
    jInstructionTableMap["JNE"] = instructionTableRecord{ "JNE", 101, "101" };
    jInstructionTableMap["JLE"] = instructionTableRecord{ "JLE", 110, "110" };
    jInstructionTableMap["JMP"] = instructionTableRecord{ "JMP", 111, "111" };

    std::string pass1InputString{};
    std::string pass2InputString{};
    std::ifstream secondPassInputFile (inputFileName.c_str());
    std::ifstream firstPassInputFile (inputFileName.c_str());
    std::ofstream outputFile (outputFileName.c_str());
    int memoryAllocator { 16 }; // for the variables
    int firstPassLineNumber{0};
    int secondPassLineNumber{0};

    if(firstPassInputFile.good() == false){
        std::cerr << "File couldn't be parsed!\n";
        EXIT_FAILURE;
    }else{
        std::cout << "File parsed\n";
    }
    // TODO: ASSEMBLER IS A SIMPLE ASSEMBLER DON'T TRY TO MAKE ASSEMBLER UNDERSTAND WHAT THE CODE IS DOING.
    std::cout << magenta << "======================================== STARTING FIRST PASS ========================================\n" << reset;
    while(true){
        std::getline(firstPassInputFile, pass1InputString);
        std::cout << red << "[DEBUG] First Pass Line Number: " << cyan << firstPassLineNumber << "\n";
        pass1InputString.erase(std::remove_if(pass1InputString.begin(), pass1InputString.end(), isspace), pass1InputString.end());
        remove_whitespace(pass1InputString);
        if (!pass1InputString.empty()){
            std::cout << pass1InputString << "\n";

            // LABEL
            if(isLabel(pass1InputString)){
                std::cout << green << "[1] Label Declaration Found: " << yellow <<  labelStrip(pass1InputString) << "\n";
                symbolTableMap[labelStrip(pass1InputString)].symbolName = labelStrip(pass1InputString);
                symbolTableMap[labelStrip(pass1InputString)].address = firstPassLineNumber;
                symbolTableMap[labelStrip(pass1InputString)].addressBinary = std::bitset<16>(firstPassLineNumber).to_string();
                std::cout << green << "[1] Address for Label " << yellow << labelStrip(pass1InputString) << green << " : " << yellow << symbolTableMap[labelStrip(pass1InputString)].addressBinary << "\n"; 


            // A INSTRUCTION
            }else if (isAInstruction(pass1InputString)){
                std::cout << green << "[1] A instruction found\n";
                if (symbolTableMap.find(aInstructionStrip(pass1InputString)) != symbolTableMap.end()){
                    std::cout << green << "[1]" << aInstructionStrip(pass1InputString) << " instruction symbol found in symbols table\n";
                }else{
                    std::cout  << green << "[1] "<< yellow << aInstructionStrip(pass1InputString) << " not found in symbols table! Adding to symbols table!\n";
                    if (isalpha(aInstructionStrip(pass1InputString).front()) == 0){
                        std::cout << red << "[DEBUG] Symbol Name: " << aInstructionStrip(pass1InputString) << "\n";
                        std::cout << green << "[1] A instruction has a number value\n";
                        symbolTableMap[aInstructionStrip(pass1InputString)].symbolName = aInstructionStrip(pass1InputString);
                        symbolTableMap[(aInstructionStrip(pass1InputString))].address = atoi(aInstructionStrip(pass1InputString).c_str());
                        symbolTableMap[(aInstructionStrip(pass1InputString))].addressBinary = std::bitset<16>(atoi(aInstructionStrip(pass1InputString).c_str())).to_string();
                        std::cout << red << "[DEBUG] Symbol Address: " << yellow << symbolTableMap[(aInstructionStrip(pass1InputString))].addressBinary << "\n";
                    }else{
                        std::cout << red << "[DEBUG] Symbol Name: " << aInstructionStrip(pass1InputString) << "\n";
                        std::cout << green << "[1] A instruction has a alphabet value, we do nothing in the first pass\n";
                    }
                }
                firstPassLineNumber++;


            // COMMENT
            }else if (isComment(pass1InputString)){
                std::cout << white << "[1] Comment Found\n";



            // C INSTRUCTION
            }else{
                std::cout << green << "[1] C instruction found\n";
                std::cout << green << "[1] We do nothing since first pass\n";
                firstPassLineNumber++;
            }
        }else{
        }
        if(firstPassInputFile.eof()){
            std::cout << magenta << "======================================== END OF FIRST PASS ========================================\n";
            break;
        }
    }


    std::cout << blue << "======================================== START OF SECOND PASS ========================================\n";
    while (true) {
        std::getline(secondPassInputFile, pass2InputString);
        pass2InputString.erase(std::remove_if(pass2InputString.begin(), pass2InputString.end(), isspace), pass2InputString.end());
        remove_whitespace(pass2InputString);
        std::cout << red << "[DEBUG] Second Pass Line number: " << cyan << secondPassLineNumber << "\n";
        if (!pass2InputString.empty()){
            std::cout << pass2InputString << "\n";

            // LABEL 
            if(isLabel(pass2InputString)){
                std::cout << green << "[2] Label value parsed\n";
                std::cout << labelStrip(pass2InputString) << "\n";
                secondPassLineNumber++;
                std::cout << green << "[2] Label address found\n";


            // A INSTRUCTION
            }else if (isAInstruction(pass2InputString)){
                secondPassLineNumber++;
                std::cout << aInstructionStrip(pass2InputString) << "\n";
                    std::cout << green << "[2] A instruction found\n";
                    // is alphabet
                    if (symbolTableMap[(aInstructionStrip(pass2InputString))].addressBinary.empty()){
                        symbolTableMap[(aInstructionStrip(pass2InputString))].address = memoryAllocator;
                        symbolTableMap[(aInstructionStrip(pass2InputString))].addressBinary = std::bitset<16>(memoryAllocator).to_string();
                        memoryAllocator++;
                    }else{

                    }
                    std::cout << red << "[DEBUG] A instruction address: " << yellow << symbolTableMap[(aInstructionStrip(pass2InputString))].addressBinary << "\n";
                    appendToFile(symbolTableMap[(aInstructionStrip(pass2InputString))].addressBinary, outputFileName, true);


            // COMMENT
            }else if (isComment(pass2InputString)){
                std::cout << white << "[2] Comment found\n";


            // C INSTRUCTION
            }else {
                secondPassLineNumber++;
                std::cout << green << "[2] C instruction found\n";
                std::string firstPart1{getFirstPart(pass2InputString, "=")};
                std::string secondPart1{getSecondPart(pass2InputString, "=")};
                std::string firstPart2{getFirstPart(pass2InputString, ";")};
                std::string secondPart2{getSecondPart(pass2InputString, ";")};
                std::string machineCode;

                std::cout << red << "[DEBUG] FirstPart: " << yellow <<  firstPart1 << red << " Second part: " << yellow << secondPart1 << "\n";
                std::cout << red << "[DEBUG] FirstPart2: " << yellow << firstPart2 << red << " Second part2: " << yellow << secondPart2 << "\n";
                switch(pass2InputString.find(";")){
                    case 0xFFFFFFFF:
                        std::cout << green << "[2] C instruction based on \"=\" key \n";
                        if (getSecondPart(pass2InputString, "=").find("M") != 0xFFFFFFFF){
                            std::cout << red << "[DEBUG] M found!\n";
                            machineCode = construct_machine_code(cInstructionTableMap[getSecondPart(pass2InputString, "=")].addressBinary, dInstructionTableMap[getFirstPart(pass2InputString, "=")].addressBinary, true, false);
                            std::cout << red << "[DEBUG] C instruction address: " << yellow << cInstructionTableMap[getSecondPart(pass2InputString, "=")].addressBinary << white << "-"  << white << yellow << dInstructionTableMap[getFirstPart(pass2InputString, "=")].addressBinary << "\n";

                            std::cout << red << "[DEBUG] Maching code: " << yellow <<  machineCode << "\n";
                            appendToFile(machineCode, outputFileName, true);
                            break;
                        }else{
                            machineCode = construct_machine_code(cInstructionTableMap[getSecondPart(pass2InputString, "=")].addressBinary, dInstructionTableMap[getFirstPart(pass2InputString, "=")].addressBinary, false, false);
                            std::cout << red << "[DEBUG] M not found!\n";
                            std::cout << red << "[DEBUG] C instruction address: " << yellow << cInstructionTableMap[getSecondPart(pass2InputString, "=")].addressBinary << white << "-"  << white << yellow << dInstructionTableMap[getFirstPart(pass2InputString, "=")].addressBinary << "\n";
                            std::cout << red << "[DEBUG] Maching code: " << yellow <<  machineCode << "\n";
                            appendToFile(machineCode, outputFileName, true);
                            break;
                        }

                    default:
                        std::cout << green << "[2] C instruction found based on ; key (JUMP) \n";
                            std::cout << red << "[DEBUG] FOUND M\n";
                            machineCode = construct_machine_code(cInstructionTableMap[getFirstPart(pass2InputString, ";")].addressBinary, jInstructionTableMap[getSecondPart(pass2InputString, ";")].addressBinary,false, true);
                            std::cout << red << "[DEBUG] C instruction address: " << yellow << cInstructionTableMap[getSecondPart(pass2InputString, ";")].addressBinary << white << "-"  << white << yellow << jInstructionTableMap[getFirstPart(pass2InputString, ";")].addressBinary << "\n";
                            std::cout << red << "[DEBUG] Maching code: " << yellow <<  machineCode << "\n";
                            appendToFile(machineCode, outputFileName, true);
                        continue;
                }
            }

        }else{
        }
        if(secondPassInputFile.eof()){
            break;
        }
    }


}


int main(){
    std::string inputfileName{"null"};
    std::string outputFileName{"null"};

    std::cout << "Enter the file name: ";
    std::cin >> inputfileName;

    validate_asm(inputfileName, outputFileName);

    return 0;
}

