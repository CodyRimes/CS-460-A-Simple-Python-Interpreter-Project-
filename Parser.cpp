//
// Created by Ali Kooshesh on 4/4/23.
//


#include<vector>
#include<iostream>
#include "Token.hpp"
#include "Parser.hpp"
#include "Statements.hpp"

// Parser functions

void Parser::die(std::string where, std::string message, Token &token) {
    std::cout << where << " " << message << std::endl;
    token.print();
    std::cout << std::endl;
    std::cout << "\nThe following is a list of tokens that have been identified up to this point.\n";
    tokenizer.printProcessedTokens();
    exit(1);
}

Statements *Parser::statements() {
    // This function is called when we KNOW that we are about to parse
    // a series of assignment statements.

    // This function parses the grammar rules:

    //KOOSHESH:
    // <statement> -> <assignStatement> <statement>
    // <statement> -> Epsilon

    //CODY APRIL 27th 2023: THIS CODE SHOULD NOW PARSE THE GRAMMAR RULES:
    //<statements> - <statement> {<statement>}   <--- note how braces {} denotes 0 or more statements following the first statement

    //A single statement itself can be of 3 different types, a for statement, an assignment statement, or a print statement. The grammar rules are as follows:
    //<statement> -> <for-statement> | <assignment-statement> | <print-statement>


    //Create an instance of statements
    //This will at LEAST hold one statement, if not more as we parse and identify what we have
    Statements *stmts = new Statements();
    //Get a token
    Token tok = tokenizer.getToken();
    //if our token is a name (a variable/identifier), or a keyword (which would be "for" or "print"), or an End Of Line marker enter the while loop
    while (tok.isName() || tok.isKeyword() || tok.eol())
    {
        //un-get the token so we can use this same token later the next time we call .getToken() in our code
        if (!tok.eol())
        {
            tokenizer.ungetToken();
        }
        //If we have a name of a variable we know we will expect an assignment statement.
        if (tok.isName() && !tok.isKeyword())
        {
            //Create an AssignmentStatement type pointer and point it to the returned value of hte assignStatement() function call
            //Lets call it assignStmt
            AssignmentStatement *assignStmt = assignStatement();
            //Add this assignment-statement to our vector of statements held by the Statements class
            stmts->addStatement(assignStmt);

            //HOW DO WE PROCESS STATEMENTS AFTER THIS ONE STATEMENTS? HANDLED IN ASSIGNSTATEMENT() FUNCTION?
        }
        //If we have a keyword, lets process the correct statement each keyword represents
        if (tok.isKeyword())
        {
            //If we have a "for" keyword,
            if (tok.getName() == "for")
            {
                //Lets make an instance of a for statement pointer that captures the returned result of our ForStatement() function
                //Call it forstatement
                ForStatement *forstatement = forStatement();
                //Lets add this for-statement to our vector of statements held by the Statements class
                stmts->addStatement(forstatement);
            }
            //If we have a "print" keyword,
            if (tok.getName() == "print")
            {
                //Lets make an instance of a print statement pointer that captures the returned result of our PrintStatement() function
                //Call it  printstatement
                PrintStatement *printstatement = printStatement();
                //Lets add this print-statement to our vector of statements held by the Statements class
                stmts->addStatement(printstatement);
            }
        }


        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return stmts;
}

AssignmentStatement *Parser::assignStatement() {
    // Parses the following grammar rule
    //
    // <assign-stmtement> -> <id> = <expr>

    // STEP 4 HERE BY CODY APRIL 26th 2023:
    // The grammar we now need to implement should follow the grammar rule:
    // <assign-statement> -> <id> "=" <relational-expression> NOTE: "=" denotes a literal equal sign expected

    //Lets get our variable name if we expect a string in the form of an assignment statement.
    //Thus our very first token in an assignment should be an <id> which should be the variable name
    Token varName = tokenizer.getToken();
    //If the token we get back is not a variable name we need to kill the program and display an error message
    //We can do this with our die() function
    if (!varName.isName())
        die("Parser::assignStatement", "Expected a name token, instead got", varName);

    //The next token we would expect after our <id> token would be the assignment/equal sign operator we expect, see grammar here: <id> "=" <relational-expression>
    Token assignOp = tokenizer.getToken();
    //If we do not  have an assignment operator we should kill the program and tell our user why the program has failed
    if (!assignOp.isAssignmentOperator())
        die("Parser::assignStatement", "Expected an equal sign, instead got", assignOp);

    // STEP 4 HERE BY CODY APRIL 26th 2023: Commenting out this code as assignment statemnet should lead to:
    // <assign-statement> -> <id> "=" <relational-expression> NOTE: "=" denotes a literal equal sign expected
    //ExprNode *rightHandSideExpr = expr();
    //The above code should now be replace with:
    ExprNode *rightHandSideRelationalExpression = relationalExpression();


    /* STEP 1 HERE. We no longer need to process semi-colons (as we are parsing python code) so we have commented this section of code out
    Token tok = tokenizer.getToken();

    if (!tok.isSemiColon())
        die("Parser::assignStatement", "Expected a semicolon, instead got", tok);
    */
    //Return a new AssignmentStatement, but first give this new AssignmentStatement the appropriate parameters using it's parameterized constructor
    //Note how in the Statements.hpp/.cpp files we this assignment statement an evaluation function. This is what we will be using when we eventually return all of our statements back to the main.cpp via our parser::statements() function. See how it recursively goes back up the stack?
    return new AssignmentStatement(varName.getName(), rightHandSideRelationalExpression);
}

//CODY APRIL 29th 2023: STEP 7 HERE.
//Adding a function to the parser that will handle seeing a for-statement
//Also added functionality to the Statements class/.cpp/.hpp file to make a sub-class of statement called ForStatement
ForStatement *Parser::forStatement()
{
    // Parses the following grammar rule
    //
    // <for-statement> -> "for" "(" <assignment-statement> ";" <relational-expression ";" <assignment-statement> ")" {<statements>}
    //NOTE: remember {<statements>} denotes zero or more statements can be expected

    //Our first expected token in a for-statement according to our grammar is a "for" keyword.
    Token forKeyword = tokenizer.getToken();

    //Lets check if that's what we get, if not, lets kill the program
    if (!forKeyword.isKeyword() && !(forKeyword.getName() == "for"))
        die("Parser::forStatement", "Expected an open parentheses \"(\" , instead got", forKeyword);

    //Our second expected token in a for-statement according to our grammar is an opening parentheses
    Token leftParentheses = tokenizer.getToken();
    //Lets check if we indeed got the expected token, a opening parentheses
    if (!leftParentheses.isOpenParen())
        die("Parser::forStatement", "Expected an open parentheses \"(\" , instead got", leftParentheses);

    //Next we would expect an assignment statement in our for statement conditional (think: for (i=0; i<array.length; i++))
    //This would take care of the i=0 for our example for statement above
    AssignmentStatement *firstAssignmentStatement = assignStatement();

    //After our first assignment statement should come our first semi-colon. Lets pick up that token and analyze it
    Token firstSemiColon = tokenizer.getToken();
    //If our token we picked up isn't a semi colon we do not have a properly syntaxed for statement
    if (!firstSemiColon.isSemiColon())
        die("Parser::forStatement", "Expected a semi-colon \";\" , instead got", firstSemiColon);

    //After our first semi-colon should be a relational expression, so lets assign something that will drill down and figure out what that expression is
    //JOE IS THIS AN INFIX EXPRESSION? THIS SHOULD BE ASSIGNED TO RELATIONAL EXPRESSION CORRECT?
    ExprNode* ourRelationalExpression = relationalExpression();

    //After our relational expression should come another semi-colon to terminate it. Lets pick up another token and analyze it
    Token secondSemiColon = tokenizer.getToken();

    //If we do not capture a semi-colon at this point we have an ill-formatted for-statement and we should kill the program
    if (!secondSemiColon.isSemiColon())
        die("Parser::forStatement", "Expected a semi-colon \";\" , instead got", secondSemiColon);

    //After the second semi-colon we expect another assignment statement
    //Lets create an instance of an assignmentStatement pointer to capture what the assignmentStatement() function figures out
    AssignmentStatement *secondAssignmentStatement = assignStatement();

    //After the last assignment statement in our for loop's conditional, we expect a closing parentheses to close out the conditional
    //Lets grab a token and analyze it
    Token rightParentheses = tokenizer.getToken();
    //Lets check if we indeed got the expected token, a closing parentheses
    if (!rightParentheses.isCloseParen())
        die("Parser::forStatement", "Expected a closing parentheses \")\" , instead got", rightParentheses);

    Token openingCurlyBrace = tokenizer.getToken();

    if (!openingCurlyBrace.isOpeningCurlyBrace())
        die("Parser::forStatement", "Expected a closing parentheses \"{\" , instead got", rightParentheses);

    //Next we will need to deal with the for statement's loop body. Here we can assign a Statements pointer to capture the return value of what the statements() function processes
    Statements* forLoopBody = statements();

    Token closingCurlyBrace = tokenizer.getToken();

    if (!closingCurlyBrace.isClosingCurlyBrace())
        die("Parser::forStatement", "Expected a closing parentheses \"}\" , instead got", rightParentheses);

    return new ForStatement(firstAssignmentStatement, ourRelationalExpression, secondAssignmentStatement, forLoopBody);
}

//CODY APRIL 29th 2023: STEP 6 HERE
//Added a function to the parser that will handle seeing a for-statement
//Also added functionality to the Statements class/.cpp/.hpp file to make a sub-class of statement called PrintStatement
PrintStatement *Parser::printStatement()
{
    // Parses the following grammar rule
    //
    // <print-statement> -> print <ID>

    //Note how in our sample input for phase 1 all of our print statements are only printing out variables and their values
    //These values are integers from the symbol table

    //Our first token we get should be a print keyword, lets grab a token and analyze it
    Token ourPrintKeyword = tokenizer.getToken();

    //If the token we got and expected to be our print keyword is not, we need to kill the program and throw an error message to our user
    if (!ourPrintKeyword.isKeyword() && !(ourPrintKeyword.getName() == "print"))
        die("Parser::printStatement()", "Expected a \"print\" keyword here , instead got", ourPrintKeyword);

    //Next we want to declare an expression node that will drill down and identify the term we want to print after the print keyword we've received
    //We will want to start from the top even though in phase 1 we could assume it will be what the primary() function would solve for us, however this could give us some issues:
    //It is better coding practice to let the program be as generic as possible (and thus will be more flexible for other use cases) and let the code solve the problem/do the work for us. Eventually it will drill down to primary and resolve the variable names and corresponding values when it reaches primary() down the call stack
    //In conversation with Joe, there was a good explanation in that, "if you used primary() to drill down, you would be assuming that the user is exclusively passing you a literal number or a variable with no modification. This fails to account for other valid print expressions involving any operator, such as a + 1, 2 < 3, or a + b + c. You don't need to think to much about what you choose to drill down with in the parser: the answer is always to start from the top of your "drilling" methods." My response was, "So better to start from the top and be safer rather than sorry. I think I get it. It doesn't hurt to drill down rather thank skipping steps/checks which come with starting at the top". And he responded, " Otherwise you open yourself to missing text which is valid. Its necessary to check every condition to perform the fundamental function of the program"
    ExprNode *variableWhosValueIsContainedInSymbolTableWeWillWantToPrint = relationalExpression();

    //Return a new printStatement instance who has all the correct parts since we have gotten through all of our parsing for a printStatement
    return new PrintStatement(variableWhosValueIsContainedInSymbolTableWeWillWantToPrint);

}

//STEP 4 HERE BY CODY APRIL 26th 2023: Adding a parser function that will take care of the grammar rule:
//<rel-expr> -> <rel-term> {(==, !=) <rel-term>} in english that is: <relational-expression> -> <relational-term> {(==, !=) <relational-term>}
ExprNode *Parser::relationalExpression()
{
    //Our first token expected for this grammar rule is a relational term, thus
    //I guess you can kind of think of this as a place holder, but in reality it is not
    //This variable holds the return value of a function that drills down and will eventually return the atomic values the grammar rules will eventually parse
    ExprNode *leftSideOfRelationalExpressionIsOurFirstRelationalTerm = relationalTerm();

    //Our second token which is expected for this grammar rule should be an equal to "==" or not equal to "!=" operator, thus
    Token relationalOperator = tokenizer.getToken();

    //If the token we get back from the .getToken() function is not an equal to "==" or not equal to "!=" relational operator, as we would expect in our grammar, kill the program and throw and error message back to our user
    //if (!relationalOperator.isEqualOperator() | !relationalOperator.isNotEqualOperator())
        //die("Parser::relationalExpression", "Expected an assignment operator \"==\" or a not equal to operator \"!=\" , instead got", relationalOperator);

    //While our relational operator is indeed an equal or not equal to operator
    while (relationalOperator.isEqualOperator() || relationalOperator.isNotEqualOperator())
    {
        //We know that by grammar rules we should expect a relational term after them, and in this program
        //we handle those terms via our InfixExprNode sub-class described in ExprNode.cpp
        //Lets make an InfixExprNode that will parse the left and right values of an infix expression i.e. 2+3 (2 being left, 3 being right)
        //LOOK UP LEFT DERIVATION SYNTENTIAL FORM TO VISUALIZE THIS TREE THAT GETS CREATED
        InfixExprNode *p = new InfixExprNode(relationalOperator);
        p->left() = leftSideOfRelationalExpressionIsOurFirstRelationalTerm;
        p->right() = relationalTerm();
        leftSideOfRelationalExpressionIsOurFirstRelationalTerm = p;
        //Get the next token. If it is indeed an equal operator or a not equal to operator, we continue to get relational expressions using this while loop
        relationalOperator = tokenizer.getToken();
    }
    //If the next token is not a equal operator or a not equal operator, we break out of the while loop. There are no more relational expressions to read at this point

    //Set flag for ungetToken to tell .getToken() function that will use last gotten token we previously picked up (i.e. getToken() will return the last gotten token which is stored as a private variable in the tokenizer class)
    tokenizer.ungetToken();
    
    return leftSideOfRelationalExpressionIsOurFirstRelationalTerm;
}

//STEP 4 HERE BY CODY APRIL 26th 2023:  Adding a parser function that will take care of the grammar rule:
//<rel-term> -> <rel-primary> {(>, >=, <, <=) <rel-primary>} in english that is: <relational-term> -> <relational-primary> {(>, >=, <, <=) <relational-primary>}
ExprNode *Parser::relationalTerm()
{
    //CODY APRIL 26th 2023: USE C++ EXPR() FUNCTION TO REPRESENT <REL-PRIMARY> GRAMMAR RULE FOR NOW
    //THIS MAY NEED TO BE FIXED LATER ON FOR A LATER PHASE OF THE PROJECT


    //Our first token expected for this grammar rule is a relational term, thus
    //I guess you can kind of think of this as a place holder, but in reality it is not
    //This variable holds the return value of a function that drills down and will eventually return the atomic values the grammar rules will eventually parse
    ExprNode *leftSideOfRelationalTermIsOurFirstRelationalPrimary = expr();

    //Our second token which is expected for this grammar rule should be a ">", ">=", "<", or "<=" operator, thus
    Token relationalOperator = tokenizer.getToken();

    //If the token we get back from the .getToken() function is not a ">", ">=", "<", or "<=" operator, as we would expect in our grammar, kill the program and throw and error message back to our user
    //if (!relationalOperator.isLessThanOperator() | !relationalOperator.isLessThanOrEqualToOperator() | !relationalOperator.isGreaterThanOperator() | !relationalOperator.isGreaterThanOrEqualToOperator())
        //die("Parser::relationalTerm", "Expected a less than operator \"<\" or a less than or equal to operator \"<=\", or a greater than or equal to operator \">=\", or a greater than operator \">\" instead got", relationalOperator);

    //While our relational operator is indeed a">", ">=", "<", or "<=" operator
    while (relationalOperator.isLessThanOperator() || relationalOperator.isLessThanOrEqualToOperator() || relationalOperator.isGreaterThanOperator() || relationalOperator.isLessThanOrEqualToOperator())
    {
        //We know that by grammar rules we should expect a relational primary after them, and in this program
        //we handle those primary's via our InfixExprNode sub-class described in ExprNode.cpp
        //Lets make an InfixExprNode that will parse the left and right values of an infix expression i.e. 2+3 (2 being left, 3 being right)
        //LOOK UP LEFT DERIVATION SYNTENTIAL FORM TO VISUALIZE THIS TREE THAT GETS CREATED
        InfixExprNode *p = new InfixExprNode(relationalOperator);
        p->left() = leftSideOfRelationalTermIsOurFirstRelationalPrimary;
        p->right() = expr();
        leftSideOfRelationalTermIsOurFirstRelationalPrimary = p;
        //Get the next token. If it is indeed an equal operator or a not equal to operator, we continue to get relational expressions using this while loop
        relationalOperator = tokenizer.getToken();
    }
    //If the next token is not a equal operator or a not equal operator, we break out of the while loop. There are no more relational expressions to read at this point

    //Set flag for ungetToken to tell .getToken() function that will use last gotten token we previously picked up (i.e. getToken() will return the last gotten token which is stored as a private variable in the tokenizer class)
    tokenizer.ungetToken();
    //
    return leftSideOfRelationalTermIsOurFirstRelationalPrimary;
}

ExprNode *Parser::expr() {
    // This function parses the grammar rules:

    // <expr> -> <term> { <add_op> <term> }
    // <add_op> -> + | -

    // However, it makes the <add_op> left associative.
    //ADD ADDITIONAL RELATIONAL OPERATORS HERE???
    ExprNode *left = term();
    Token tok = tokenizer.getToken();
    while (tok.isAdditionOperator() || tok.isSubtractionOperator() || tok.isEqualOperator() || tok.isNotEqualOperator()){
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = term();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}


ExprNode *Parser::term() {
    // This function parses the grammar rules:

    // <term> -> <primary> { <mult_op> <primary> }
    // <mult_op> -> * | / | %

    // However, the implementation makes the <mult-op> left associate.
    ExprNode *left = primary();
    Token tok = tokenizer.getToken();

    while (tok.isMultiplicationOperator() || tok.isDivisionOperator() || tok.isModuloOperator() || tok.isLessThanOrEqualToOperator() || tok.isLessThanOperator() || tok.isGreaterThanOrEqualToOperator() || tok.isGreaterThanOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = primary();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::primary() {
    // This function parses the grammar rules:

    // <primary> -> [0-9]+
    // <primary> -> [_a-zA-Z]+
    // <primary> -> (<expr>)

    Token tok = tokenizer.getToken();

    if (tok.isWholeNumber() )
        return new WholeNumber(tok);
    else if( tok.isName() )
        return new Variable(tok);
    else if (tok.isOpenParen()) {
        ExprNode *p = relationalExpression();
        Token token = tokenizer.getToken();
        if (!token.isCloseParen())
            die("Parser::primary", "Expected close-parenthesis, instead got", token);
        return p;
    }
    die("Parser::primary", "Unexpected token", tok);

    return nullptr;  // Will not reach this statement!
}

//Cody: Looks like Adam added this functionality
/*
ExprNode *Parser::relationalExpr() {
    // This function parses the grammar rules:
    // <relational-expr> -> <expr> <rel_op> <expr>
    // <rel_op> -> < | <= | > | >= | == | !=
    // However, it makes the <rel_op> left associative.

    ExprNode *left = expr();
    Token tok = tokenizer.getToken();
    while (tok.isLessThanOperator() || tok.isLessThanOrEqualToOperator()||
           tok.isGreaterThanOperator() || tok.isGreaterThanOrEqualToOperator() ||
           tok.isEqualOperator() || tok.isNotEqualOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = expr();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return static_cast<ExprNode *>(left);
}
*/