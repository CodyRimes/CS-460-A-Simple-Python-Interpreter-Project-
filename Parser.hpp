//
// Created by Ali Kooshesh on 4/4/23.
//

#ifndef APYTHONINTERPRETER_PARSER_HPP
#define APYTHONINTERPRETER_PARSER_HPP

#include "Token.hpp"
#include "Tokenizer.hpp"
#include "SymTab.hpp"
#include "ExprNode.hpp"
#include "Statements.hpp"

#include<vector>
#include<iostream>
#include<map>


class Parser {
public:
    Parser(Tokenizer &tokenizer) : tokenizer{tokenizer} {};

    Statements *statements();

    AssignmentStatement *assignStatement();

    ForStatement *forStatement();

    PrintStatement *printStatement();

    ExprNode *expr();
    ExprNode *term();
    ExprNode *primary();
    //Cody: Adam added relationalExpr()
    //ExprNode *relationalExpr();
    //STEP 4 HERE APRIL 26th 2023: Cody added code for relational expressions and relational terms in accordance with the grammar rules
    ExprNode *relationalExpression();
    ExprNode *relationalTerm();

    std::string id();

private:
    Tokenizer &tokenizer;

    void die(std::string where, std::string message, Token &token);

};


#endif //APYTHONINTERPRETER_PARSER_HPP
