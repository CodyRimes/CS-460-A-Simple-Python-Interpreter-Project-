//
// Created by Ali Kooshesh on 4/4/23.
//


#include<iostream>
#include "ExprNode.hpp"
//ExprNode is a base class that we literally never use directly. We only use ExprNode pointers so that we can employ polymorphism to have code which works on all ExprNode subclasses and dynamically behaves according to subclass traits at runtime. We are always using the evaluate method of subclasses because every ExprNode subclass must define their own evaluate method (because it is declared a virtual method in ExprNode). If we aren't evaluating an atomic value (a variable, a literal number, a literal string, etc.) then we are using the InfixExprNode evaluate method.
// STEP 3 RENAME ArithExpr to ExprNode
// ExprNode
ExprNode::ExprNode(Token token): _token{token} {}

Token ExprNode::token() { return _token; }

// InfixExprNode functions
InfixExprNode::InfixExprNode(Token tk) : ExprNode{tk}, _left(nullptr), _right(nullptr) {}

ExprNode *&InfixExprNode::left() { return _left; }

ExprNode *&InfixExprNode::right() { return _right; }

int InfixExprNode::evaluate(SymTab &symTab) {
    // Evaluates an infix expression using a post-order traversal of the expression tree.
    int lValue = left()->evaluate(symTab);
    int rValue = right()->evaluate(symTab);
    if(debug)
        std::cout << "InfixExprNode::evaluate: " << lValue << " " << token().symbol() << " " << rValue << std::endl;
    if (token().isAdditionOperator())
        return lValue + rValue;
    else if (token().isSubtractionOperator())
        return lValue - rValue;
    else if (token().isMultiplicationOperator())
        return lValue * rValue;
    else if (token().isDivisionOperator())
        return lValue / rValue; // division by zero?
    else if (token().isModuloOperator())
        return lValue % rValue;
    //Cody: Looks like Adam completed part of Step 4 here and added this functionality to the expression node class function
    else if (token().isLessThanOperator())
        return lValue < rValue;
    else if (token().isLessThanOrEqualToOperator())
        return lValue <= rValue;
    else if (token().isGreaterThanOperator())
        return lValue > rValue;
    else if (token().isGreaterThanOrEqualToOperator())
        return lValue >= rValue;
    else if (token().isEqualOperator())
        return lValue == rValue;
    else if (token().isNotEqualOperator())
        return lValue != rValue;
    else {
        std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
        token().print();
        std::cout << std::endl;
        exit(2);
    }
}

void InfixExprNode::print() {
    _left->print();
    token().print();
    _right->print();
}

// WHoleNumber
WholeNumber::WholeNumber(Token token): ExprNode{token} {}

void WholeNumber::print() {
    token().print();
}

int WholeNumber::evaluate(SymTab &symTab) {
    if(debug)
        std::cout << "WholeNumber::evaluate: returning " << token().getWholeNumber() << std::endl;
    return token().getWholeNumber();
}

// Variable

Variable::Variable(Token token): ExprNode{token} {}

void Variable::print() {
    token().print();
}

int Variable::evaluate(SymTab &symTab) {
    if( ! symTab.isDefined(token().getName())) {
        std::cout << "Use of undefined variable, " << token().getName() << std::endl;
        exit(1);
    }
    if(debug)
        std::cout << "Variable::evaluate: returning " << symTab.getValueFor(token().getName()) << std::endl;
    return symTab.getValueFor(token().getName());
}

