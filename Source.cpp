/* Filename: LexicalAnalyzer.cpp
CSC 340 - Programming Languages
Andrew Ferguson
A lexical analyzer system for simple arithmetic expressions.

Operator/keyword: +, -, * /, (, )
Identifer: (a+b+...+z+A+B+...Z)(a+b+...+z+A+B+...Z+0+1+2+...+9)*
Integer: (0+1+2+...+9)(0+1+2+...+9)*

*/

#include <iostream>
#include <fstream>
#include <cctype>


/* Global declarations */
/* Variables */
using namespace std;
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
ifstream in_fp("syntax.txt");

/* Function declarations */
void getChar();
void addChar();
void getNonBlank();
void factor();
void expr();
void term();
void assign();
void boolFactor();
void boolTerm();
void boolExpr();
void ifstmt();
int lex();      // to get the nextToken


/* Character classes */
#define LETTER 0
#define DIGIT 1
#define TRUE 2
#define FALSE 4
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define SEMICOLON 12

#define LEFT_CURL 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define MOD_OP 27
#define ASSIGN_OP 28
#define RIGHT_CURL 29


#define NOT 31
#define AND 32
#define OR 33
#define IF 34
#define ELSE 35
/******************************************************/
/* main driver */

void main()
{
	/* Open the input data file and process its contents */

	if (in_fp.fail())
	{
		cout << "File could not be opened\n";
		cin.get();
		exit(1);
	}
	else {
		getChar();
		do {
			lex();  // Getting the nextToken
			ifstmt(); // start the analyzer
		} while (nextToken != EOF);
	}
	in_fp.close();

	system("PAUSE");
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses
and return the token */
int lookup(char ch)
{
	switch (ch) {
	case '(':
		addChar();
		nextToken = LEFT_PAREN;
		break;
	case ')':
		addChar();
		nextToken = RIGHT_PAREN;
		break;
	case '+':
		addChar();
		nextToken = ADD_OP;
		break;
	case '-':
		addChar();
		nextToken = SUB_OP;
		break;
	case '*':
		addChar();
		nextToken = MULT_OP;
		break;
	case '/':
		addChar();
		nextToken = DIV_OP;
		break;
	case '%':
		addChar();
		nextToken = MOD_OP;
		break;
	case ';':
		addChar();
		nextToken = SEMICOLON;
		break;
	case '=':
		addChar();
		nextToken = ASSIGN_OP;
		break;
	case '{':
		addChar();
		nextToken = LEFT_CURL;
		break;
	case '}':
		addChar();
		nextToken = RIGHT_CURL;
		break;
	case '!':
		addChar();
		nextToken = NOT;
		break;
	default:
		addChar();
		nextToken = EOF;
		break;
	}
	return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar()
{
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = 0;
	}
	else
		cout << " Error - lexeme is too long \n";
}

/*****************************************************/
/* getChar - a function to get the next character of
input and determine its character class */
void getChar()
{
	in_fp.get(nextChar);
	if (in_fp.eof())   // if no more character in the file
		nextChar = EOF;

	if (nextChar != EOF) {		
		if (isalpha(nextChar))
			charClass = LETTER;
		else if (isdigit(nextChar))
			charClass = DIGIT;
		else charClass = UNKNOWN;
	}
	else
		charClass = EOF;
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it
returns a non-whitespace character */
void getNonBlank()
{
	while (isspace(nextChar))
		getChar();
}

/***************************************************** /
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex()
{
	lexLen = 0;
	getNonBlank();
	switch (charClass) {
		/* Parse identifiers */
	case LETTER:
		addChar();
		getChar();
		while (charClass == LETTER || charClass == DIGIT) {
			addChar();
			getChar();
		}
		nextToken = IDENT;
		if (lexeme[0] == 'e' && lexeme[1] == 'l' && lexeme[2] == 's' && lexeme[3] == 'e' && lexeme[4] == 0)
		{
			nextToken = ELSE;
		}
		else if (lexeme[0] == 'T' && lexeme[1] == 'R' && lexeme[2] == 'U' && lexeme[3] == 'E' && lexeme[4] == 0)
		{
			nextToken = TRUE;
		}
		else if (lexeme[0] == 'F' && lexeme[1] == 'A' && lexeme[2] == 'L' && lexeme[3] == 'S' && lexeme[4] == 'E' && lexeme[5] == 0)
		{
			nextToken = FALSE;
		}
		else if (lexeme[0] == 'i' && lexeme[1] == 'f' && lexeme[3] == 0)
		{
			nextToken = IF;
		}
		break;
		/* Parse integer literals */
	case DIGIT:
		addChar();
		getChar();
		while (charClass == DIGIT) {
			addChar();
			getChar();
		}
		nextToken = INT_LIT;
		break;
		/* Parentheses and operators */
	case UNKNOWN:
		lookup(nextChar);
		getChar();
		if (nextToken == EOF)
		{
			addChar();
			getChar();
			if (lexeme[0] == '&' && lexeme[1] == '&' && lexeme[2] == 0)
			{
				nextToken = AND;
				
			}
			else if (lexeme[0] == '|' && lexeme[1] == '|' && lexeme[3] == 0)
			{
				nextToken = OR;
				
			}
			
		}
		break;
		/* EOF */
	case EOF:

		nextToken = EOF;
		lexeme[0] = 'E';
		lexeme[1] = 'O';
		lexeme[2] = 'F';
		lexeme[3] = 0;
		break;

	}

	
	 /* End of switch */
	cout << "Next token is: " << nextToken
		<< "       Next lexeme is " << lexeme << "\n";
	return nextToken;
} /* End of function lex */

//<ifstmt> -> if (<boolexpr>) '{'<assign>'}' | if (<boolexpr>) '{'<assign>'}' else '{'<assign>'}'
void ifstmt(){
	if (nextToken == IF)
	{
		lex();		
		if (nextToken == LEFT_PAREN)
		{
			lex();
			boolExpr();
			if (nextToken == RIGHT_PAREN)
			{
				lex();
				if (nextToken == LEFT_CURL)
				{
					lex();
					assign();
					if (nextToken == RIGHT_CURL)
					{
						lex();
						if (nextToken == ELSE)
						{
							lex();
							{
								if (nextToken == LEFT_CURL)
								{
									lex();
									assign();
									if (nextToken == RIGHT_CURL)
									{
										lex();
									}
									else 
									{
										cout << "expected }\n";
										system("PAUSE");

									}
								}
								else
								{
									cout << "expected {\n";
									system("PAUSE");

								}
							}
						}
					}
					else 
					{
						cout << "expected }\n";
						system("PAUSE");

					}
				}
				else
				{
					cout << "expected {\n";
					system("PAUSE");

				}
			}
			else
			{
				cout << "expected )\n";
				system("PAUSE");

			}
		}
		else
		{
			cout << "expected (\n";
			system("PAUSE");
		}			
	}
}

//<boolterm> -> <boolterm> && <boolfactor> | <boolfactor>
void boolTerm() {
	boolFactor();
	while (nextToken == AND)
	{
		lex();
		boolTerm();
	}

}

//< boolexpr > -> <boolexpr> || <boolterm> | <boolterm>
void boolExpr() {
	boolTerm();
	while (nextToken == OR)
	{
		lex();
		boolTerm();
	}
}

/* Function boolfactor Parses strings in the language generated by the rule:<boolfactor> -> TRUE | FALSE | ! <boolfactor> | (<boolexpr>)*/


void boolFactor() {
	if (nextToken == TRUE || nextToken == FALSE)
	{
		lex();
	}
	else if (nextToken == NOT)
	{
		lex();
		boolFactor();
	}
	else if (nextToken == LEFT_PAREN)
	{
		lex();
		boolExpr();
		if (nextToken == RIGHT_PAREN)
		{
			lex();
		}
		else {
			cout << "ERROR: bad expression\n";
			system("PAUSE");

		}
	}
	else 
	{
		cout<< "expected (\n";
		system("PAUSE");

	}

}

void assign() {
	if (nextToken == IDENT)
	{
		lex();
		if (nextToken == ASSIGN_OP)
		{
			lex();
			expr();
			if (nextToken == SEMICOLON)
			{
				lex();
				assign();				
			}
			else
			{
				cout << "ERROR: expected ;\n";
				system("PAUSE");
			}			
		}		
		else
		{
			cout << "ERROR: missing assignment\n";
			system("PAUSE");
		}
	}
}

/* termParses strings in the language generated by the rule:<term> -> <factor> {(* | /) <factor>}*/
void term() {
	//get the first factor
	factor();
	while (nextToken == MULT_OP || nextToken == DIV_OP || nextToken == MOD_OP)
	{
		lex();
		factor();
	}
}

/* Function expr Parses strings in the language generated by the rule:<expr> -> <term> {(+ | -) <term>}*/
void expr()
{
	// get the first term 
	term();
	while (nextToken == ADD_OP || nextToken == SUB_OP)
	{
		//get next token
		lex();		
		term();
	}
}

/* Function factorParses strings in the language generated by the rule: <factor> -> id | int_constant |( <expr>) */

void factor() {
	//if next token is any of the RHS cases call lex
	//<factor> -> id              int_constant |( <expr>)
	if (nextToken == IDENT || nextToken == INT_LIT)
	{
		lex();
	}
	//There must be <expr> inside parentheses, so call expr
	else if (nextToken == LEFT_PAREN)
	{
		lex();
		expr();
		if (nextToken == RIGHT_PAREN)
		{
			lex();
		}
		else
		{
			cout << "ERROR: Expected )\n";
			system("PAUSE");
		}
		
	}
	else {
		cout << "error\n";
		system("PAUSE");
	}
}

