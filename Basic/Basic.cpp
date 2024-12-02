/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"


/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

/* Main program */

int main() {
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
                continue;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }

    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.scanStrings();
    scanner.setInput(line);
    if(!scanner.hasMoreTokens())return;;
    std::string  token = scanner.nextToken();
    if(token == "RUN") {
        program.continue_program = true;
        while (program.continue_program == true){
            int currentline  = program.getFirstLineNumber();
            program.setCurrentline(currentline);
            while(currentline != -1) {
                currentline  = program.getCurrentline();
                /*std::cout<<currentline;*/
                Statement *stmt = program.getParsedStatement(currentline);
                if(stmt != nullptr) {
                    stmt->execute(state,program);
                }
                if(program.continue_program == false)break;
                currentline  = program.getCurrentline();
                if((stmt->getType() != 6||(stmt->getType() == 6 && stmt->replace ==false))&& (stmt->getType() != 7||(stmt->getType() == 7 && stmt->replace ==false))) {
                    currentline = program.getNextLineNumber(currentline);
                }
                program.setCurrentline(currentline);
            }

            return;
        }
    }
    if(token == "LIST") {
        int currentline  = program.getFirstLineNumber();
        while(currentline != -1) {
            std::cout << program.getSourceLine(currentline)<<std::endl;
            currentline = program.getNextLineNumber(currentline);

        }
        return;
    }
    if(token == "CLEAR") {
        program.clear();
        state.Clear();
        return;
    }
    if(token == "QUIT") {
        program.clear();
        state.Clear();
        exit(0);
    }
    if(token == "PRINT") {
        Expression*expr = nullptr;
        bool fault = false;
        if(scanner.hasMoreTokens()) {
            expr = parseExp(scanner);
        }
        Statement *stmt = new PrintStatement(expr);
        try {
            stmt->execute(state, program);
        }catch (ErrorException &ex) {
            delete stmt;
            fault = true;
            std::cout << ex.getMessage() << std::endl;
        }
        if(!fault){
        delete stmt;}
        return;
    }
    if(token == "LET") {
        std::string varname = scanner.nextToken();
        bool fault = false;
        if(varname =="LET") {
            error("SYNTAX ERROR");
        }
        scanner.nextToken();
        Expression*expr = parseExp(scanner);
        Statement* stmt = new LetStatement(varname,expr);
        try {
            stmt->execute(state,program);
        }catch (ErrorException &ex) {
            delete stmt;
            fault = true;
            std::cout << ex.getMessage() << std::endl;
        }
        if(!fault){
            delete stmt;}
        return;
    }
    if(token == "INPUT") {
        std::string name = scanner.nextToken();
        Statement*stmt =new InputStatement(name);
        stmt->execute(state,program);
        delete stmt;
        return;
    }
    if(isdigit(token[0])) {
        int lineNumber = stringToInteger(token);
        if(scanner.hasMoreTokens()) {
            std::string next = scanner.nextToken();
            std::string sourceline = line;
            program.addSourceLine(lineNumber,sourceline);
            if(next == "PRINT") {
                Expression*expr = nullptr;
                if(scanner.hasMoreTokens()) {
                    expr = parseExp(scanner);
                }
                Statement *stmt = new PrintStatement(expr);
                program.setParsedStatement(lineNumber,stmt);
                return;
            }
            if(next == "LET") {
                std::string varname = scanner.nextToken();
                scanner.nextToken();
                Expression*expr = parseExp(scanner);
                Statement* stmt = new LetStatement(varname,expr);
                program.setParsedStatement(lineNumber,stmt);

                return;
            }
            if(next == "INPUT") {
                std::string name = scanner.nextToken();
                Statement*stmt =new InputStatement(name);
                program.setParsedStatement(lineNumber,stmt);
                return;
            }
            if(next == "END") {
                Statement*stmt = new EndStatement;
                program.setParsedStatement(lineNumber,stmt);
                return;
            }
            if(next == "GOTO") {
                int targetline = stringToInteger(scanner.nextToken());
                Statement*stmt = new GotoStatement(targetline);
                program.setParsedStatement(lineNumber,stmt);
                return;
            }
            if(next == "IF") {
                std::string line_="";
                while(scanner.hasMoreTokens()) {
                    line_ += scanner.nextToken()+" ";
                }

                Statement*stmt = new IfStatement(line_);
                program.setParsedStatement(lineNumber,stmt);
                return;
            }
            if(next == "REM") {
                Statement*stmt = new RemStatement();
                program.setParsedStatement(lineNumber,stmt);
                return;
            }
            /*std::string sourceline = line;
            program.addSourceLine(lineNumber,sourceline);
            Statement* stmt = nullptr;
            program.setParsedStatement(lineNumber,stmt);
            delete stmt;*/
        }
        else {
            program.removeSourceLine(lineNumber);
        }
        return;
    }




    //todo
}

