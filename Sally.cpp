// File: Sally.cpp
//
// CMSC 341 Fall 2018 Project 2
//
//
// Implementation of member functions of Sally Forth interpreter
//
#include <iostream>
#include <string>
#include <list>
#include <stack>
#include <stdexcept>
#include <cstdlib>
using namespace std;

#include "Sally.h"


// Basic Token constructor. Just assigns values.
//
Token::Token(TokenKind kind, int val, string txt) {
	m_kind = kind;
	m_value = val;
	m_text = txt;
}


// Basic SymTabEntry constructor. Just assigns values.
//
SymTabEntry::SymTabEntry(TokenKind kind, int val, operation_t fptr) {
	m_kind = kind;
	m_value = val;
	m_dothis = fptr;
}


// Constructor for Sally Forth interpreter.
// Adds built-in functions to the symbol table.
//
Sally::Sally(istream& input_stream) :
	istrm(input_stream)  // use member initializer to bind reference
{
	skip = 0;
	marker = 0;
	action = 0;
	symtab["DUMP"] = SymTabEntry(KEYWORD, 0, &doDUMP);

	symtab["+"] = SymTabEntry(KEYWORD, 0, &doPlus);
	symtab["-"] = SymTabEntry(KEYWORD, 0, &doMinus);
	symtab["*"] = SymTabEntry(KEYWORD, 0, &doTimes);
	symtab["/"] = SymTabEntry(KEYWORD, 0, &doDivide);
	symtab["%"] = SymTabEntry(KEYWORD, 0, &doMod);
	symtab["NEG"] = SymTabEntry(KEYWORD, 0, &doNEG);

	symtab["."] = SymTabEntry(KEYWORD, 0, &doDOT);
	symtab["SP"] = SymTabEntry(KEYWORD, 0, &doSP);
	symtab["CR"] = SymTabEntry(KEYWORD, 0, &doCR);

	symtab["SET"] = SymTabEntry(KEYWORD, 0, &doSET);
	symtab["DROP"] = SymTabEntry(KEYWORD, 0, &doDROP);
	symtab["DUP"] = SymTabEntry(KEYWORD, 0, &doDUP);
	symtab["SWAP"] = SymTabEntry(KEYWORD, 0, &doSWAP);
	symtab["ROT"] = SymTabEntry(KEYWORD, 0, &doROT);
	symtab["@"] = SymTabEntry(KEYWORD, 0, &doAMPERSAND);
	symtab["AND"] = SymTabEntry(KEYWORD, 0, &doAND);
	symtab["OR"] = SymTabEntry(KEYWORD, 0, &doOR);
	symtab["NOT"] = SymTabEntry(KEYWORD, 0, &doNOT);
	symtab["!"] = SymTabEntry(KEYWORD, 0, &doEXCLAMATION);
	symtab["<"] = SymTabEntry(KEYWORD, 0, &doLESSTHAN);
	symtab["<="] = SymTabEntry(KEYWORD, 0, &doLESSTHANEQUAL);
	symtab["=="] = SymTabEntry(KEYWORD, 0, &doEQUIVALENCY);
	symtab["!="] = SymTabEntry(KEYWORD, 0, &doNOTEQUALS);
	symtab[">="] = SymTabEntry(KEYWORD, 0, &doGREATERTHANEQUAL);
	symtab[">"] = SymTabEntry(KEYWORD, 0, &doGREATERTHAN);

	symtab["IFTHEN"] = SymTabEntry(KEYWORD, 0, &doIFTHEN);
	symtab["ELSE"] = SymTabEntry(KEYWORD, 0, &doELSE);
	symtab["ENDIF"] = SymTabEntry(KEYWORD, 0, &doENDIF);
	symtab["DO"] = SymTabEntry(KEYWORD, 0, &doDO);
}


// This function should be called when tkBuffer is empty.
// It adds tokens to tkBuffer.
//
// This function returns when an empty line was entered 
// or if the end-of-file has been reached.
//
// This function returns false when the end-of-file was encountered.
// 
// Processing done by fillBuffer()
//   - detects and ignores comments.
//   - detects string literals and combines as 1 token
//   - detetcs base 10 numbers
// 
//
bool Sally::fillBuffer() {
	string line;     // single line of input
	int pos;         // current position in the line
	int len;         // # of char in current token
	long int n;      // int value of token
	char *endPtr;    // used with strtol()


	while (true) {    // keep reading until empty line read or eof

					  // get one line from standard in
					  //
		getline(istrm, line);

		// if "normal" empty line encountered, return to mainLoop
		//
		if (line.empty() && !istrm.eof()) {
			//cout << "Red Flag" << endl;
			return true;
		}
		

		// if eof encountered, return to mainLoop, but say no more
		// input available
		//
		if (istrm.eof()) {
			return false;
		}


		// Process line read

		pos = 0;                      // start from the beginning

									  // skip over initial spaces & tabs
									  //
		while (line[pos] != '\0' && (line[pos] == ' ' || line[pos] == '\t')) {
			pos++;
		}

		// Keep going until end of line
		//
		while (line[pos] != '\0') {

			// is it a comment?? skip rest of line.
			//
			if (line[pos] == '/' && line[pos + 1] == '/') break;

			// is it a string literal? 
			//
			if (line[pos] == '.' && line[pos + 1] == '"') {

				pos += 2;  // skip over the ."
				len = 0;   // track length of literal

						   // look for matching quote or end of line
						   //
				while (line[pos + len] != '\0' && line[pos + len] != '"') {
					len++;
				}

				// make new string with characters from
				// line[pos] to line[pos+len-1]
				string literal(line, pos, len);  // copy from pos for len chars

												 // Add to token list
												 //
				tkBuffer.push_back(Token(STRING, 0, literal));

				// Different update if end reached or " found
				//
				if (line[pos + len] == '\0') {
					pos = pos + len;
				}
				else {
					pos = pos + len + 1;
				}

			}
			else {  // otherwise "normal" token

				len = 0;  // track length of token

						  // line[pos] should be an non-white space character
						  // look for end of line or space or tab
						  //
				while (line[pos + len] != '\0' && line[pos + len] != ' ' && line[pos + len] != '\t') {
					len++;
				}

				string literal(line, pos, len);   // copy form pos for len chars
				pos = pos + len;

				// Try to convert to a number
				//
				n = strtol(literal.c_str(), &endPtr, 10);

				if (*endPtr == '\0') {
					tkBuffer.push_back(Token(INTEGER, n, literal));
				}
				else {
					tkBuffer.push_back(Token(UNKNOWN, 0, literal));
				}
			}

			// skip over trailing spaces & tabs
			//
			while (line[pos] != '\0' && (line[pos] == ' ' || line[pos] == '\t')) {
				pos++;
			}

		}
	}
}



// Return next token from tkBuffer.
// Call fillBuffer() if needed.
// Checks for end-of-file and throws exception 
//
Token Sally::nextToken() {
	Token tk;
	bool more = true;

	while (more && tkBuffer.empty()) {
		more = fillBuffer();
	}

	if (!more && tkBuffer.empty()) {
		throw EOProgram("End of Program");
	}

	tk = tkBuffer.front();
	tkBuffer.pop_front();
	return tk;
}


// The main interpreter loop of the Sally Forth interpreter.
// It gets a token and either push the token onto the parameter
// stack or looks for it in the symbol table.
//
//
void Sally::mainLoop() {
	Token tk;
	map<string, SymTabEntry>::iterator it;

	try {
		while (1) {
			tk = nextToken();
			//cout << "skip: " << skip << " tk.m_text: "<<tk.m_text<<" marker: "<<marker<<endl;
			if (skip % 2 == 0)
			{
				if (tk.m_kind == INTEGER || tk.m_kind == STRING) {

					// if INTEGER or STRING just push onto stack
					params.push(tk);

				}
				else {
					it = symtab.find(tk.m_text);
					if (it == symtab.end()) {   // not in symtab

						params.push(tk);

					}
					
					else if (it->second.m_kind == KEYWORD) {

						// invoke the function for this operation
						//
						it->second.m_dothis(this);
					}
					else if (it->second.m_kind == VARIABLE) {

						// variables are pushed as tokens
						//
						tk.m_kind = VARIABLE;
						params.push(tk);

					}
					else {

						// default action
						//
						params.push(tk);

					}
				}
			}
			else
			{
				//it = symtab.find(tk.m_text);
				if (tk.m_text == "ELSE")
				{
					it = symtab.find(tk.m_text);
					it->second.m_dothis(this);
				}
				if (tk.m_text == "ENDIF")
				{
					it = symtab.find(tk.m_text);
					it->second.m_dothis(this);
				}
				if (tk.m_text == "IFTHEN")
				{
					it = symtab.find(tk.m_text);
					it->second.m_dothis(this);
				}
			}
		}
	}

	catch (EOProgram& e) {

		cerr << "End of Program\n";
		if (params.size() == 0) {
			cerr << "Parameter stack empty.\n";
		}
		else {
			cerr << "Parameter stack has " << params.size() << " token(s).\n";
		}

	}
	catch (out_of_range& e) {

		cerr << "Parameter stack underflow??\n";

	}
	catch (...) {

		cerr << "Unexpected exception caught\n";

	}
}

// -------------------------------------------------------
void Sally::doIFTHEN(Sally *Sptr)
{
	Token tk;
	tk = Sptr->params.top();
	//Sptr->params.pop();
	int truthValue = tk.m_value;
	if(Sptr->skip%2==1)
	{
		tk = Sptr->nextToken();
		Sptr->skip = Sptr->skip + 1;
	}
	else if (truthValue == 1)
	{
		Sptr->skip = Sptr->skip + 2;
		//cout << "IfThen function skip= " <<Sptr->skip<< endl;
	}
	else
	{
		tk = Sptr->nextToken();
		Sptr->skip = Sptr->skip + 1;
		
		Sptr->marker++;
	}
}

void Sally::doELSE(Sally *Sptr)
{
	Token tk;
	Sptr->skip = Sptr->skip - 1;
}

void Sally::doENDIF(Sally *Sptr)
{
	if (Sptr->marker == 0)
	{
		//Sptr->skip--;
		Sptr->skip = Sptr->skip - 1;
		//cout << "function skip= " << Sptr->skip << endl;
	}
	else
	{
		Sptr->marker=Sptr->marker-1;
	}
}
void Sally::doDO(Sally *Sptr)
{
	//cout << "Do triggered" << endl;
	int continueLoop = 1;
	Token tk;
	Token tk2;
	Sptr->tempBuffer.push_back(tk);
	//Token arr[10];
	cout << "DO LOOP" << endl;
	while(1==continueLoop)
	{
		//cout << "Do triggered" << endl;
		tk = Sptr->nextToken();
		//cout << "Token" << tk.m_text << tk.m_value<<endl;
		//cout << "token: " << tk.m_text << endl;
		Sptr->tempBuffer.push_back(tk);
		if (tk.m_text == "UNTIL")
			{
			//cout << "TESTING" << endl;
				continueLoop = 0;
				//cout << "test" << endl;
			}
	}
	continueLoop = 1;
	//cout << "TESTING" << endl;
	while (continueLoop == 1)
	{
		for (Token tk:Sptr->tempBuffer)
			{
			//cout << "tk.m_text "<<tk.m_text << endl;
			//cout << "do triggered" << endl;
				
				if (tk.m_kind == INTEGER || tk.m_kind == STRING) {
					// if INTEGER or STRING just push onto stack
					Sptr->params.push(tk);
					//cout << "triggered" << endl;
				}
				else {
					//cout << "triggered" << endl;
					map<string, SymTabEntry>::iterator it;
					it = Sptr->symtab.find(tk.m_text);

					if (it == Sptr->symtab.end()) {   // not in symtab
						Sptr->params.push(tk);
					}
					else if (it->second.m_kind == KEYWORD) {

						// invoke the function for this operation
						//
						it->second.m_dothis(Sptr);
					}
					else if (it->second.m_kind == VARIABLE) {
						// variables are pushed as tokens
						//
						tk.m_kind = VARIABLE;
						Sptr->params.push(tk);
					}
				}
				if (tk.m_text == "UNTIL")
				{
					//cout << "UNTIL in Play" << endl;
					Sptr->params.pop();
					tk2 = Sptr->params.top();
					Sptr->params.pop();
					
					if (tk2.m_value== 1)
					{
						continueLoop = 0;
						break;
					}
				}
			}
		}
	}

void Sally::doPlus(Sally *Sptr) {
	Token p1, p2;

	if (Sptr->params.size() < 2) {
		throw out_of_range("Need two parameters for +.");
	}
	p1 = Sptr->params.top();
	Sptr->params.pop();
	p2 = Sptr->params.top();
	Sptr->params.pop();
	int answer = p2.m_value + p1.m_value;
	Sptr->params.push(Token(INTEGER, answer, ""));
}
void Sally::doMinus(Sally *Sptr) {
	Token p1, p2;

	if (Sptr->params.size() < 2) {
		throw out_of_range("Need two parameters for -.");
	}
	p1 = Sptr->params.top();
	Sptr->params.pop();
	p2 = Sptr->params.top();
	Sptr->params.pop();
	int answer = p2.m_value - p1.m_value;
	Sptr->params.push(Token(INTEGER, answer, ""));
}

void Sally::doTimes(Sally *Sptr) {
	Token p1, p2;

	if (Sptr->params.size() < 2) {
		throw out_of_range("Need two parameters for *.");
	}
	p1 = Sptr->params.top();
	Sptr->params.pop();
	p2 = Sptr->params.top();
	Sptr->params.pop();
	int answer = p2.m_value * p1.m_value;
	Sptr->params.push(Token(INTEGER, answer, ""));
}

void Sally::doDivide(Sally *Sptr) {
	Token p1, p2;

	if (Sptr->params.size() < 2) {
		throw out_of_range("Need two parameters for /.");
	}
	p1 = Sptr->params.top();
	Sptr->params.pop();
	p2 = Sptr->params.top();
	Sptr->params.pop();
	int answer = p2.m_value / p1.m_value;
	Sptr->params.push(Token(INTEGER, answer, ""));
}

void Sally::doMod(Sally *Sptr) {
	Token p1, p2;

	if (Sptr->params.size() < 2) {
		throw out_of_range("Need two parameters for %.");
	}
	p1 = Sptr->params.top();
	Sptr->params.pop();
	p2 = Sptr->params.top();
	Sptr->params.pop();
	int answer = p2.m_value % p1.m_value;
	Sptr->params.push(Token(INTEGER, answer, ""));
}

void Sally::doNEG(Sally *Sptr) {
	Token p;

	if (Sptr->params.size() < 1) {
		throw out_of_range("Need one parameter for NEG.");
	}
	p = Sptr->params.top();
	Sptr->params.pop();
	Sptr->params.push(Token(INTEGER, -p.m_value, ""));
}

void Sally::doDOT(Sally *Sptr) {

	Token p;
	//cout << "Print Function triggering" << endl;
	if (Sptr->params.size() < 1) {
		throw out_of_range("Need one parameter for .");
	}

	p = Sptr->params.top();
	Sptr->params.pop();

	if (p.m_kind == INTEGER) {
		cout << p.m_value;
	}
	else {
		cout << p.m_text;
	}
}

void Sally::doSP(Sally *Sptr) {
	cout << " ";
}

void Sally::doCR(Sally *Sptr) {
	cout << endl;
}

void Sally::doDUMP(Sally *Sptr) {
	// do whatever for debugging
}

void Sally::doDUP(Sally *Sptr) {
	Token p1;

	if (Sptr->params.size() < 1) {
		throw out_of_range("Need one parameter to DUP.");
	}
	p1 = Sptr->params.top();
	//Sptr->params.pop();

	int duplicate = p1.m_value;
	Sptr->params.push(Token(INTEGER, duplicate, ""));
}

void Sally::doDROP(Sally *Sptr)
{
	Token p1;
	if (Sptr->params.size() < 1) {
		throw out_of_range("Need a parameter to DROP.");
	}
	p1 = Sptr->params.top();
	Sptr->params.pop();
}
void Sally::doSWAP(Sally *Sptr)
{
	Token p1;
	Token p2;
	if (Sptr->params.size() < 2)
	{
		throw out_of_range("Need 2 items to SWAP them.");
	}
	p1 = Sptr->params.top();
	Sptr->params.pop();
	p2 = Sptr->params.top();
	Sptr->params.pop();
	int answer1 = p2.m_value;
	int answer2 = p1.m_value;
	Sptr->params.push(Token(INTEGER, answer1, ""));
	Sptr->params.push(Token(INTEGER, answer2, ""));
}
void Sally::doROT(Sally *Sptr)
{
	Token p1;
	Token p2;
	Token p3;
	if (Sptr->params.size() < 3)
	{
		throw out_of_range("Need 3 items to SWAP them.");
	}
	p1 = Sptr->params.top();
	Sptr->params.pop();
	p2 = Sptr->params.top();
	Sptr->params.pop();
	p3 = Sptr->params.top();
	Sptr->params.pop();
	int answer1 = p2.m_value;
	int answer2 = p1.m_value;
	int answer3 = p3.m_value;
	Sptr->params.push(Token(INTEGER, answer1, ""));
	Sptr->params.push(Token(INTEGER, answer2, ""));
	Sptr->params.push(Token(INTEGER, answer3, ""));
}
void Sally::doSET(Sally *Sptr)
{
	Token tk;
	Token tk2;
	tk = Sptr->params.top();
	//cout << "tk.m_value" << tk.m_value << "  " << tk.m_text << endl;
	Sptr->params.pop();
	tk2 = Sptr->params.top();
	//cout << "tk2.m_value" << tk2.m_value <<"  " << tk2.m_text<< endl;
	Sptr->params.pop();
	map<string, SymTabEntry>::iterator it;
	it = Sptr->symtab.find(tk.m_text);
	if (it == Sptr->symtab.end())
	{
		//cout << "tk2.m_value" << tk2.m_value << "  " << tk2.m_text << endl;
		Sptr->symtab[tk.m_text] = SymTabEntry(VARIABLE, tk2.m_value, NULL);
	}
	else if (it->second.m_kind == KEYWORD)
	{
		throw out_of_range("This is a keyword and can't be changed");
	}
	else if (it->second.m_kind == VARIABLE)
	{
		cout << "Variable already set" << endl;
		//throw out_of_range("This variable is already set");
	}
}

void Sally::doAMPERSAND(Sally *Sptr)
{
	Token tk;
	tk = Sptr->params.top();
	Sptr->params.pop();
	//cout << "tk.m_value" << tk.m_value << "  " << tk.m_text << endl;
	//Sptr->params.pop();
	map<string, SymTabEntry>::iterator it;
	it = Sptr->symtab.find(tk.m_text);
	if (it == Sptr->symtab.end())
	{
		cout << "This value does not exist" << endl;
		throw out_of_range("This value does not exist");
	}
	else
	{
		Sptr->params.push(Token(INTEGER, it->second.m_value, ""));
		//cout << "Test" << endl;
		//tk = Sptr->params.top();
		//cout << tk.m_value << endl;
	}
}

void Sally::doEXCLAMATION(Sally *Sptr)
{
	Token tk;
	Token tk2;
	tk = Sptr->params.top();
	Sptr->params.pop();
	tk2 = Sptr->params.top();
	Sptr->params.pop();
	map<string, SymTabEntry>::iterator it;
	it = Sptr->symtab.find(tk.m_text);
	if (it == Sptr->symtab.end()) {
		cout << "No such variable exists" << endl;
		throw out_of_range("No such variable exists");
	}
	else
	{
		Sptr->symtab[tk.m_text] = SymTabEntry(VARIABLE, tk2.m_value, NULL);
	}
}
void Sally::doLESSTHAN(Sally *Sptr)
{
	{
		Token p1;
		Token p2;
		if (Sptr->params.size() < 2)
		{
			throw out_of_range("Need 2 items to do comparison.");
		}
		p1 = Sptr->params.top();
		Sptr->params.pop();
		p2 = Sptr->params.top();
		Sptr->params.pop();
		int answer1 = p2.m_value;
		int answer2 = p1.m_value;
		if (answer1 <= answer2)
		{
			Sptr->params.push(Token(INTEGER, 1, ""));
		}
		else
		{
			Sptr->params.push(Token(INTEGER, 0, ""));
		}
	}
}
void Sally::doLESSTHANEQUAL(Sally *Sptr)
{
	Token p1;
	Token p2;
	if (Sptr->params.size() < 2)
	{
		throw out_of_range("Need 2 items to do comparison.");
	}
	p1 = Sptr->params.top();
	Sptr->params.pop();
	p2 = Sptr->params.top();
	Sptr->params.pop();
	int answer1 = p2.m_value;
	int answer2 = p1.m_value;
	if (answer1 <= answer2)
	{
		Sptr->params.push(Token(INTEGER, 1, ""));
	}
	else
	{
		Sptr->params.push(Token(INTEGER, 0, ""));
	}
}
void Sally::doEQUIVALENCY(Sally *Sptr)
{
	{
		Token p1;
		Token p2;
		if (Sptr->params.size() < 2)
		{
			throw out_of_range("Need 2 items to do comparison.");
		}
		p1 = Sptr->params.top();
		Sptr->params.pop();
		p2 = Sptr->params.top();
		Sptr->params.pop();
		int answer1 = p2.m_value;
		int answer2 = p1.m_value;
		if (answer1 <= answer2)
		{
			Sptr->params.push(Token(INTEGER, 1, ""));
		}
		else
		{
			Sptr->params.push(Token(INTEGER, 0, ""));
		}
	}
}
void Sally::doNOTEQUALS(Sally *Sptr)
{
	{
		Token p1;
		Token p2;
		if (Sptr->params.size() < 2)
		{
			throw out_of_range("Need 2 items to do comparison.");
		}
		p1 = Sptr->params.top();
		Sptr->params.pop();
		p2 = Sptr->params.top();
		Sptr->params.pop();
		int answer1 = p2.m_value;
		int answer2 = p1.m_value;
		if (answer1 != answer2)
		{
			Sptr->params.push(Token(INTEGER, 1, ""));
		}
		else
		{
			Sptr->params.push(Token(INTEGER, 0, ""));
		}
	}
}
void Sally::doGREATERTHANEQUAL(Sally *Sptr)
{
	{
		Token p1;
		Token p2;
		if (Sptr->params.size() < 2)
		{
			throw out_of_range("Need 2 items to do comparison.");
		}
		p1 = Sptr->params.top();
		Sptr->params.pop();
		p2 = Sptr->params.top();
		Sptr->params.pop();
		int answer1 = p2.m_value;
		int answer2 = p1.m_value;
		if (answer1 >= answer2)
		{
			Sptr->params.push(Token(INTEGER, 1, ""));
		}
		else
		{
			Sptr->params.push(Token(INTEGER, 0, ""));
		}
	}
}
void Sally::doGREATERTHAN(Sally *Sptr)
{
	{
		Token p1;
		Token p2;
		if (Sptr->params.size() < 2)
		{
			throw out_of_range("Need 2 items to do comparison.");
		}
		p1 = Sptr->params.top();
		Sptr->params.pop();
		p2 = Sptr->params.top();
		Sptr->params.pop();
		int answer1 = p2.m_value;
		int answer2 = p1.m_value;
		if (answer1 > answer2)
		{
			Sptr->params.push(Token(INTEGER, 1, ""));
		}
		else
		{
			Sptr->params.push(Token(INTEGER, 0, ""));
		}
	}
}
void Sally::doAND(Sally *Sptr)
{
	Token p1, p2;

	if (Sptr->params.size() < 2) {
		cout << "Need two parameters for -." << endl;
		throw out_of_range("Need two parameters for -.");
	}
	p1 = Sptr->params.top();
	Sptr->params.pop();
	p2 = Sptr->params.top();
	Sptr->params.pop();
	if (p1.m_value == 0 || p2.m_value == 0)
	{
		Sptr->params.push(Token(INTEGER, 0, ""));
	}
	else
	{
		Sptr->params.push(Token(INTEGER, 1, ""));
	}
}
void Sally::doOR(Sally *Sptr)
{
	Token p1, p2;

	if (Sptr->params.size() < 2) {
		cout << "Need two parameters for -." << endl;
		throw out_of_range("Need two parameters for -.");
	}
	p1 = Sptr->params.top();
	Sptr->params.pop();
	p2 = Sptr->params.top();
	Sptr->params.pop();
	if (p1.m_value == 1 || p2.m_value == 1)
	{
		Sptr->params.push(Token(INTEGER, 1, ""));
	}
	else
	{
		Sptr->params.push(Token(INTEGER, 0, ""));
	}
}
void Sally::doNOT(Sally *Sptr)
{
	Token p1;

	if (Sptr->params.size() < 1) {
		cout << "Need two parameters for -." << endl;
		throw out_of_range("Need one parameters for -.");
	}
	p1 = Sptr->params.top();
	Sptr->params.pop();
	if (p1.m_value == 0)
	{
		Sptr->params.push(Token(INTEGER, 1, ""));
	}
	else
	{
		Sptr->params.push(Token(INTEGER, 0, ""));
	}
}


/*
Finally, these are the operations that you have to add to the Sally FORTH interpreter :

Stack operations : DUP DROP SWAP ROT
Variable operations : SET @ !
Comparison operations : < <= == != >= >
Logic operations : AND OR NOT
If statements : IFTHEN ELSE ENDIF
Loop construct : DO UNTIL
*/
