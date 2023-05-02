//
// Created by Ali Kooshesh on 4/4/23.
//

//Cody: Note to self/logic explanation: A lot of the bool functions compare our private attribute _symbol against a char literal, and if they match it will return true in these bool functions below

#ifndef APYTHONINTERPRETER_TOKEN_HPP
#define APYTHONINTERPRETER_TOKEN_HPP

#include<string>

class Token {

public:
    Token();
    bool &eof()  { return _eof; }
    bool &eol()  { return _eol; }

    bool eof() const { return _eof; }
    bool eol() const  { return _eol; }

    bool isOpenParen() const  { return _symbol == '('; }
    bool isCloseParen() const { return _symbol == ')'; }

    bool isOpeningCurlyBrace() const { return _symbol == '{' ;}
    bool isClosingCurlyBrace() const { return _symbol == '}' ;}

    void symbol(char c) { _symbol = c; }
    char symbol() { return _symbol; }

    bool isSemiColon() const { return _symbol == ';'; }
    bool isAssignmentOperator() const              { return _symbol == '='; }
    bool isMultiplicationOperator() const { return _symbol == '*'; }
    bool isAdditionOperator() const       { return _symbol == '+'; }
    bool isSubtractionOperator() const    { return _symbol == '-'; }
    bool isModuloOperator() const         { return _symbol == '%'; }
    bool isDivisionOperator() const       { return _symbol == '/'; }
    bool isNotEqualOperator() const       { return _stringforrationaloperator == "!=";}
    bool isGreaterThanOperator() const    { return _symbol == '>';}
    bool isLessThanOperator() const       { return _symbol == '<'; }
    //Cody: Adding tokens that identify when we see greater than or equal ">=" to or less than or equal to "<=" signs. NOTE: I DO NOT THINK THIS WORKS SINCE _symbol IS A CHAR DATATYPE
    bool isGreaterThanOrEqualToOperator() const   { return _stringforrationaloperator == ">=";}
    bool isLessThanOrEqualToOperator() const      { return _stringforrationaloperator == "<=";}
    bool isEqualOperator() const               { return _stringforrationaloperator == "==";}

    bool isArithmeticOperator() const {
        return isMultiplicationOperator() ||
               isAdditionOperator() ||
               isSubtractionOperator() ||
               isModuloOperator() ||
               isDivisionOperator();
    }
    bool isName() const                   { return _name.length() > 0; }
    std::string getName() const                  { return _name; }
    void setName(std::string n) { _name = n; }

    std::string getStringForRelationalOperator() const { return _stringforrationaloperator; }
    void setStringForRelationalOperator(std::string incomingString) { _stringforrationaloperator = incomingString;}

    bool &isWholeNumber() { return _isWholeNumber; }
    bool isWholeNumber() const { return _isWholeNumber; }
    int getWholeNumber() const { return _wholeNumber; }
    void setWholeNumber(int n) {
        _wholeNumber = n;
        isWholeNumber() = true;
    }
    // STEP 2
    //Cody: Returns true if one of the bools within the function return true
    bool isRelationalOperator() const {
        return isLessThanOperator() ||
        isLessThanOrEqualToOperator() ||
        isGreaterThanOperator() ||
        isGreaterThanOrEqualToOperator() ||
        isEqualOperator() ||
        isNotEqualOperator();
    }


    /* Cody: I think this is Adam's code I'm commenting out?
    bool isNotEqualOperator() const {
        return _name == "!=";
    }
    bool isGreaterThanOperator() const {
        return _name == ">";
    }
    bool isGreaterThanOrEqualOperator() const {
        return _name == ">=";
    }
    bool isLessThanOperator() const {
        return _name == "<";
    }
    bool isLessThanOrEqualOperator() const {
        return _name == "<=";
    }
     */

    // STEP 5 HERE, Cody: Adam added isKeyword
    //Cody: is the variable _name equal to "print" or "for"? If so it will return true
    bool isKeyword() const {
        return _name == "print" || _name == "for";
    }


    void print() const;

private:
    std::string _name;
    std::string _stringforrationaloperator;
    bool _eof, _eol;
    bool _isWholeNumber;
    char _symbol;
    int _wholeNumber;
};


#endif //APYTHONINTERPRETER_TOKEN_HPP
