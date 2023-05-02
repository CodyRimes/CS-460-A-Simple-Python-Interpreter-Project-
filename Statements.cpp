//
// Created by Ali Kooshesh on 4/4/23.
//

#include "Statements.hpp"

// Statement
Statement::Statement() {}

// Statements

Statements::Statements() {}
void Statements::addStatement(Statement *statement) { _statements.push_back(statement); }

void Statements::print() {
    for (auto s: _statements)
        s->print();
}

void Statements::evaluate(SymTab &symTab) {
    for (auto s: _statements)
        s->evaluate(symTab);
}

// AREA FOR AssignmentStatement

//Default constructor used to default private member values on initialization of an AssignmentStatement
AssignmentStatement::AssignmentStatement() : _lhsVariable{""}, _rhsExpression{nullptr} {}

//Parameterized constructor used to set private member on initialization of an AssignmentStatement
AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr):
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}

//This will be called in main.cpp and is used to see if the values of the assignment statement have changed through the course of reading in the program we are parsing's statements
void AssignmentStatement::evaluate(SymTab &symTab) {
    int rhs = rhsExpression()->evaluate(symTab);
    //Updating the left hand side of the expression to be the new value that we got
    symTab.setValueFor(lhsVariable(), rhs);
}

//Getter function for left hand side private member variable
std::string &AssignmentStatement::lhsVariable() {
    return _lhsVariable;
}
//Getter function for right hand side private member expression
ExprNode* &AssignmentStatement::rhsExpression() {
    return _rhsExpression;
}
//Print function for this assignment statement
//This will be called main.cpp
void AssignmentStatement::print() {
    std::cout << _lhsVariable << " = ";
    _rhsExpression->print();
    std::cout << " " << std::endl;
}


//AREA FOR PrintStatement
//Default constructor for PrintStatement
PrintStatement::PrintStatement()
{
    //Setting private member variables to bogus/null values when we first initialize an instance of this class
    //_lhsVariable = "";
    _rhsExpression = nullptr;
}

//Parameterized constructor for PrintStatement (we use this to correctly set all private member variables after our parsing function has successfully validated we have a print statement/made sure all the correct parts are in order for us to create this "certified" print statement
PrintStatement::PrintStatement( ExprNode* rhsExprs)
{
    //_lhsVariable = lhsVar;
    _rhsExpression = rhsExprs;
}

/*
std::string &PrintStatement::lhsVariableGetter()
{
    return _lhsVariable;
}
*/

ExprNode* PrintStatement::_rhsExpressionGetter()
{
    return _rhsExpression;
}



/* This will be needed for phase 2
ExprNode* &PrintStatement::rhsExpression()
{
    return _rhsExpressions;
}
*/


void PrintStatement::print()
{

    //use the ExprNode's (which is the type our _rhsExpression private member variable is) print function to print out the value that the right hand side expression contains
    std::cout << "print (";
    _rhsExpression->print();
    std::cout << ")" << std::endl;

    /* THIS CODE MAY BE NEEDED FOR PHASE 2
    std::cout << "print " << _lhsVariable << std::endl;
    for (auto expr : _rhsExpressions)
    {
        expr->print();
    }
     */
}

//We will need the symbol table passed in so we can use it to help us evaluate our print statement
void PrintStatement::evaluate(SymTab &symTab)
{

    //Use the symbol table we've passed to the
    std::cout << _rhsExpression->evaluate(symTab) << std::endl;

    /* THIS CODE MAY BE NEEDED FOR PHASE 2
    for (auto expr : _rhsExpressions)
    {
        TypeDescriptor* desc = expr->evaluate(symTab);
        if (desc->type() == TypeDescriptor::INTEGER)
        {
            std::cout << dynamic_cast<NumberDescriptor*>(desc)->value.intValue;
        }
        else if (desc->type() == TypeDescriptor::STRING)
        {
            std::cout << dynamic_cast<StringDescriptor*>(desc)->str;
        }
    }
    std::cout << std::endl;
     */

}

//AREA FOR ForStatement
//Default constructor
ForStatement::ForStatement()
{
    //Setting private member variables to bogus/null values when we first initialize an instance of this class
    _initialAssignmentStatement = nullptr;
    _conditionalExpressionInForLoop = nullptr;
    _updateToInitialAssignmentExpression = nullptr;
    _bodyOfForLoop = nullptr;
}

//Parameterized constructor
ForStatement::ForStatement(AssignmentStatement* incomingFirstAssignmentStatementOfForLoopHeader, ExprNode* incomingRelationalExpressionUsedInForLoopHeader, AssignmentStatement* incomingReassignmentToFirstAssignmentStatementInForLoopHeader, Statements* RestOfStatementsHeldInForLoopBody)
{
    _initialAssignmentStatement = incomingFirstAssignmentStatementOfForLoopHeader;
    _conditionalExpressionInForLoop = incomingRelationalExpressionUsedInForLoopHeader;
    _updateToInitialAssignmentExpression = incomingReassignmentToFirstAssignmentStatementInForLoopHeader;
    _bodyOfForLoop = RestOfStatementsHeldInForLoopBody;
}

//Our four private member variable getters go here:
AssignmentStatement* ForStatement::getOurInitialStatement()
{
    return _initialAssignmentStatement;
}
ExprNode* ForStatement::getOurConditionalExpressionInForLoop()
{
    return _conditionalExpressionInForLoop;
}
AssignmentStatement* ForStatement::getOurUpdateToInitialAssignmentExpression()
{
    return _updateToInitialAssignmentExpression;
}
Statements* ForStatement::getOurBodyOfForLoop()
{
    return _bodyOfForLoop;
}
//Print the contents of this print statement [class instance] using the AssignmentStatement, ExprNode and Statements built-in print functions
//Remember our private members are of the AssignmentStatement, ExprNode and Statements class types
void ForStatement::print()
{
    std:: cout << "for (";
    //Use AssignmentStatement's print function to print the contents of our private member AssignmentStatement variable
    _initialAssignmentStatement->print();
    //Use a break line for more readability
    //Use ExprNode's print function to print the contents of our private member ExprNode variable
    _conditionalExpressionInForLoop->print();
    std::cout << std::endl;

    //Use AssignmentStatement's print function to print the contents of our private member AssignmentStatement variable
    _updateToInitialAssignmentExpression->print();
    std::cout << ")" << std::endl;
    std::cout << "{" << std::endl;
    //Use Statements class print function to print the contents of our private member Statements variable
    _bodyOfForLoop->print();
    std::cout << "}" << std::endl;

}

//Evaluate the contents of this print statement [class instance] using the AssignmentStatement, ExprNode and Statements built-in print functions
//Remember our private members are of the AssignmentStatement, ExprNode and Statements class types
void ForStatement::evaluate(SymTab &symTab)
{
    //Use AssignmentStatement's evaluate function to evaluate the contents of our private member AssignmentStatement variable (It specifically updates the symbol table with value on the right hand side of the statement and ties it with the left hand side expression node)
    _initialAssignmentStatement->evaluate(symTab);

    //While we have our conditional expression:
    while (_conditionalExpressionInForLoop->evaluate(symTab))
    {


        //Use Statements class evaluate function to evaluate the contents of our private member Statements variable
        _bodyOfForLoop->evaluate(symTab);

        //Use AssignmentStatement's evaluate function to evaluate the contents of our private member AssignmentStatement variable
        _updateToInitialAssignmentExpression->evaluate(symTab);
    }


}

