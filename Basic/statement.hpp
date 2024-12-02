/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h


#include <string>
#include <sstream>
#include "evalstate.hpp"
#include "exp.hpp"
#include "Utils/tokenScanner.hpp"
#include "parser.hpp"
#include "Utils/error.hpp"
#include "Utils/strlib.hpp"
#include "program.hpp"

class Program;


/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

    Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

    virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

    virtual void execute(EvalState &state, Program &program) = 0;
    virtual  int getType();
    bool replace = true;
};


class LetStatement:public Statement {
public:
 LetStatement(const std::string & name,Expression *exp) ;
 void execute(EvalState &state, Program &program) override ;
 int getType() override;
 ~LetStatement();
private:
 std::string varname;
 Expression* expr;
};
class RemStatement:public Statement {
public:
 RemStatement() ;
 void execute(EvalState& state, Program& program) override;
 int getType() override;
 ~RemStatement();
private:
 std::string comment;
};
class PrintStatement : public Statement {
public:
 PrintStatement(Expression*exp) ;
 void execute(EvalState &state, Program &program) override ;
 int getType()override;
 ~PrintStatement();
private:
 Expression *expr;
};
class InputStatement : public Statement {
public:
 InputStatement(const std::string &name);
 void execute(EvalState &state, Program &program) override ;
 int getType() override;
 ~InputStatement();
private:
 std::string varname;
};
class EndStatement : public Statement {
public:
 EndStatement();
 void execute(EvalState &state, Program &program) override ;
 int getType()override;
private:
};
class GotoStatement : public Statement {
public:
 GotoStatement(int lineNumber) ;

 void execute(EvalState &state, Program &program) override ;
 int getType()override;
private:
 int targetLine;

};
class IfStatement:public Statement {
public:
 IfStatement(std::string line);
 void execute(EvalState &state, Program &program) override;
 ~IfStatement();
 int getType()override;
private:
std::string line_to_check;
 bool check  = false;
 int targetline;
};
Statement* parselet(TokenScanner&scanner);
Statement* parsePrintStatement(TokenScanner& scanner);
Statement* parseInputStatement(TokenScanner& scanner);
Statement* parseEndStatement(TokenScanner& scanner) ;
Statement* parseRemStatement(TokenScanner& scanner);
/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */

#endif
