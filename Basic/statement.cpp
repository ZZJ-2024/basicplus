/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"


/* Implementation of the Statement class */

int stringToInt(std::string str);

Statement::Statement() = default;

Statement::~Statement() = default;
int Statement::getType() {return  -1;
}

LetStatement:: LetStatement(const std::string & name,Expression *exp) {
 varname = name;
 expr = exp;
}
LetStatement::~LetStatement(){
 delete expr;
}
void LetStatement::execute(EvalState &state, Program &program)  {
 int value = expr->eval(state);
 state.setValue(varname,value);
}
RemStatement::RemStatement() {
}
RemStatement::~RemStatement(){};
PrintStatement::PrintStatement(Expression*exp) {
 expr = exp;
}
void RemStatement::execute(EvalState &state, Program &program) {

}

void PrintStatement::execute(EvalState &state, Program &program)  {
 int value = expr->eval(state);
 std::cout<<value<<std::endl;
}
PrintStatement::~PrintStatement() {
 delete expr;
}
InputStatement::InputStatement(const std::string &name) {
 varname = name;
}
void InputStatement:: execute(EvalState &state, Program &program) {
 while(true) {
  std::cout<<" ? ";
  std::string input;
  std::getline(std::cin,input);
  TokenScanner scanner_tmp;
  scanner_tmp.ignoreWhitespace();
  scanner_tmp.scanNumbers();
  scanner_tmp.scanStrings();
  scanner_tmp.setInput(input);

  try{
   bool negative = false;
   if(input.front() == '-') negative = true;
   if(negative == false){
  std::string tmp1 = scanner_tmp.nextToken();
   if(scanner_tmp.hasMoreTokens()) {
    error("expr");
   }
   else {
    for (char ch : tmp1) {
     if (!isdigit(ch)&& ch != '-') {
      error("expr");
     }
    }
    state.setValue(varname,stringToInteger(tmp1));
    return;
     }
   }
   if(negative == true) {
    state.setValue(varname,stringToInteger(input));
    return;
   }
  }catch (const std::exception& e) {
   std::cout<<"INVALID NUMBER"<<std::endl;
  }
 }
}
InputStatement::~InputStatement(){};
EndStatement::EndStatement(){};
void EndStatement:: execute(EvalState &state, Program &program)  {
 program.continue_program = false;
}
GotoStatement::GotoStatement(int lineNumber) {
 targetLine= lineNumber;
}
void GotoStatement::execute(EvalState &state, Program &program) {
 if(program.getSourceLine(targetLine) != "") {
  program.setCurrentline(targetLine);
 }
 else {
  std::cout<<"LINE NUMBER ERROR"<<std::endl;
  replace = false;
 }
}
Statement* parselet(TokenScanner&scanner) {
 scanner.nextToken();
 std::string varname = scanner.nextToken();
 scanner.nextToken();
 Expression*expr = parseExp(scanner);
 return  new LetStatement(varname,expr);
}
Statement* parsePrintStatement(TokenScanner& scanner) {
 scanner.nextToken();
 Expression *expr = parseExp(scanner);
 return new PrintStatement(expr);
}
Statement* parseInputStatement(TokenScanner& scanner) {
 scanner.nextToken();
 std::string name = scanner.nextToken();
 return new InputStatement(name);
}
Statement* parseEndStatement(TokenScanner& scanner) {
 return  new EndStatement;
}
Statement* parseRemStatement(TokenScanner& scanner) {
 scanner.nextToken();
 std::string comment ;
 while(scanner.hasMoreTokens()) {
  comment += scanner.nextToken()+" ";
 }
 return  new RemStatement();
}
int LetStatement:: getType() {
 return 1;
}
int RemStatement:: getType() {
 return 2;
}
int PrintStatement:: getType() {
 return 3;
}
int InputStatement:: getType() {
 return 4;
}
int  EndStatement:: getType() {
 return 5;
}
int GotoStatement:: getType() {
 return 6;
}
int IfStatement:: getType() {
 return 7;
}
IfStatement::~IfStatement() {
}
IfStatement::IfStatement(std::string line) {
 line_to_check = line;

}
void IfStatement::execute(EvalState &state, Program &program) {
 TokenScanner tmp_scanner;
 tmp_scanner.ignoreWhitespace();
 tmp_scanner.scanNumbers();
 tmp_scanner.scanStrings();
 tmp_scanner.setInput(line_to_check);
 bool ck;
 int check_ = 0;
 std::string tmp1 = "";
 std::string add1 = "";
 while(true) {
  add1 = tmp_scanner.nextToken();
  if(add1 != ">" && add1 != "<" && add1 != "=") {
   tmp1+= add1;
   tmp1 += " ";
  }
  else {
   if(add1 == ">"){check_ = 1;}
   if(add1 == "="){check_ = 2;}
   if(add1 == "<"){check_ = 3;}
   tmp1.erase(tmp1.size() -1);
   break;
  }
 }
 TokenScanner scanner_tmp1;
 scanner_tmp1.ignoreWhitespace();
 scanner_tmp1.scanNumbers();
 scanner_tmp1.scanStrings();
 scanner_tmp1.setInput(tmp1);
 Expression* condition1 = parseExp(scanner_tmp1);
 int tmp_1 = condition1->eval(state);
 std::string tmp2 = "";
 std::string add2 = "";
 while(true) {
  add2 = tmp_scanner.nextToken();
  if(add2 != "THEN") {
   tmp2+= add2;
   tmp2 += " ";
  }
  else {
   break;
  }
 }
 TokenScanner scanner_tmp2;
 scanner_tmp2.ignoreWhitespace();
 scanner_tmp2.scanNumbers();
 scanner_tmp2.scanStrings();
 scanner_tmp2.setInput(tmp2);
 Expression* condition2 = parseExp(scanner_tmp2);
 int tmp_2 = condition2->eval(state);
 delete condition1;
 delete condition2;
 if(check_ == 1) {
  ck = (tmp_1 > tmp_2);
 }
 if(check_ == 2) {
  ck = (tmp_1 == tmp_2);
 }
 if(check_ == 3) {
  ck = (tmp_1 < tmp_2);
 }
 targetline = stringToInteger(tmp_scanner.nextToken());
 if(program.getSourceLine(targetline) != "") {
  if(ck){
   program.setCurrentline(targetline);
   replace = true;
  }
  else replace = false;
 }
 else {
  std::cout<<"LINE NUMBER ERROR"<<std::endl;
  replace = false;
 }

}
/*Statement* parseIfStatement(TokenScanner& scanner) {
 scanner.nextToken();
 Expression* condition = parseExp(scanner);
 scanner.nextToken();
 int targetLine = stringToInteger(scanner.nextToken());
 return new IfStatement(condition, targetLine);
}*/
//todo
