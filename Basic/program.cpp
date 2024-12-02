/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"

#include <cmath>


Program::Program() = default;

Program::~Program() = default;

void Program::clear() {
    for (auto& pair : parsedlines) {
        delete pair.second;
    }
    lines.clear();
    commands.clear();
    parsedlines.clear();
    // Replace this stub with your own code
    //todo
}

void Program::addSourceLine(int lineNumber, const std::string &line) {
        TokenScanner scanner;
        std::string token = scanner.nextToken();

        auto it = lines.find(lineNumber);
        if(it != lines.end()) {
            it->second = line;
        }
        else {
            lines.insert(std::make_pair(lineNumber,line));
            commands.insert(lineNumber);
        }
        scanner.setInput(line);
        Statement *stmt = nullptr;
        if(token == "LET") {
            stmt = parselet(scanner);
        }
        else if(token == "PRINT"){
            stmt = parsePrintStatement(scanner);
        }
        else if(token == "INPUT") {
            stmt = parseInputStatement(scanner);
        }
        else if(token == "END") {
            stmt = parseEndStatement(scanner);
        }
        else if(token == "REM") {
            stmt = parseRemStatement(scanner);
        }
        setParsedStatement(lineNumber,stmt);

    // Replace this stub with your own code
    //todo
}

void Program::removeSourceLine(int lineNumber) {
    auto it = lines.find(lineNumber);
    if(it != lines.end()) {
        lines.erase(it);
        commands.erase(lineNumber);
    }
    // Replace this stub with your own code
    //todo
}

std::string Program::getSourceLine(int lineNumber) {
    auto it = lines.find(lineNumber);
    if(it != lines.end()) {
        std::string tmp = it->second;
        return tmp;
    }
    return"";
    // Replace this stub with your own code
    //todo
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    if(lines.find(lineNumber) == lines.end()) {
        error("SYNTAX ERROR");
    }
    if(parsedlines[lineNumber] != nullptr) {
        delete parsedlines[lineNumber];
    }
    parsedlines[lineNumber] = stmt;
    // Replace this stub with your own code
    //todo
}

//void Program::removeSourceLine(int lineNumber) {

Statement *Program::getParsedStatement(int lineNumber) {
    if (parsedlines.find(lineNumber) == parsedlines.end()) {
        return nullptr;
    }
    return parsedlines[lineNumber];
   // Replace this stub with your own code
   //todo
}

int Program::getFirstLineNumber() {
    if(commands.empty())return -1;
    else {
        return *commands.begin() ;
    }
    // Replace this stub with your own code
    //todo
}

int Program::getNextLineNumber(int lineNumber) {
    auto it = commands.find(lineNumber);
    if (it == commands.end()) return -1;
    it++;
    if(it == commands.end()) {
        return -1;
    }
    else {
        return *it;
    }
    // Replace this stub with your own code
    //todo
}
void Program::setCurrentline(int line) {
    currentline = line;
}
int Program::getCurrentline()const {
    return currentline;
}

//more func to add
//todo


