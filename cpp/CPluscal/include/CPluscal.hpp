#ifndef CPLUSCAL_HPP
#define CPLUSCAL_HPP

////////// INIT ////////////////////////////////////////////////////////////////////////////////////

/// Imports ///
#include <string>
using std::string;
#include <array>
using std::array;
#include <vector>
using std::vector;
#include <deque>
using std::deque;
#include <fstream>
using std::ifstream;
#include <sstream>
using std::stringstream, std::getline;
#include <iostream>
using std::cout, std::endl, std::flush, std::cerr, std::ostream;
#include <stdexcept>
using std::runtime_error;


/// Aliases ///
typedef unsigned long  ulong;
typedef long long /**/ llong;
typedef unsigned char  ubyte;
typedef vector<string> vstr;
typedef vector<vstr>   vvstr; 



////////// LANGUAGE CONSTANTS //////////////////////////////////////////////////////////////////////

const array<char,52> LETTERS = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
    't', 'u', 'v', 'w', 'x', 'y', 'z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
    'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
};

const array<char,10> DIGITS = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

const array<string,24> SYMBOLS = { /// Special Symbols ///
    "**", "+", "-", "*", "/", // Infix Operators
    "=", "<", ">", "<>", "<=", ">=", ":=", "..", "<=", 
    "[", "]", ".", ",", ":", ";", "^", "(", ")",
    "'"
};
const size_t NUM_INFIX = 5;

const array<string,35> RESERVED = { /// Word Symbols ///
    "and", "array", "begin", "case", "const", "div", "do", "downto", "else", "end", "file", "for",
    "function", "goto", "if", "in", "label", "mod", "nil", "not", "of", "or", "packed", "procedure",
    "program", "record", "repeat", "set", "then", "to", "type", "until", "var", "while", "with"
};

const array<string,3> PRIM_TYPE_NAMES = { /// Primitive Type Names ///
    "integer", "real", "Boolean",
};

const ubyte MIN_RES_SYMBOL_LEN = 1;
const ubyte MAX_RES_SYMBOL_LEN = 9;

const array<string,2> COMMENT_OPEN = { "{", "(*" };
const array<string,2> COMMENT_CLOS = { "}", "*)" };

const array<string,3> SECTION_NAMES = { "type", "var", "const" };

const array<string,2> NL_INDICATORS = { "\r\n", "\n" };


///// Internal Tokens /////////////////////////////////////////////////////
const string TKN_NEWLINE = "<nl>";


////////////////////////////////////////////////////////////////////////////////////////////////////
////////// lexer.cpp ///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

////////// STRING PROCESSING ///////////////////////////////////////////////////////////////////////
std::ostream& operator<<( std::ostream& os , vvstr arr ); // Print string vector vector
bool p_special( const string& q ); // ---------------------- Return True if `q` matches a symbol, otherwise return False
bool p_infix_op( const string& q ); // --------------------- Return True if `q` matches an infix arithmetic operator, otherwise return False
bool p_reserved( const string& q ); // --------------------- Return True if `q` matches a symbol, otherwise return False
bool p_prim_type( const string& q ); // -------------------- Return True if `q` matches a primtive type name, otherwise return False
bool p_symbol( const string& q ); // ----------------------- Return True if either Special or Reserved
bool p_letter( const char& q ); // ------------------------- Return True if `q` matches a letter, otherwise return False
bool p_digit( const char& q ); // -------------------------- Return True if `q` matches a digit, otherwise return False
// Return True for strings that are: 1. Not symbols, 2. Begin with a letter, 3. Are composed of alphanumeric chars
bool p_identifier( const string& q ); 
// Handle the case when reserved symbols are substrings of each other (2 tokens only!)
vstr attempt_reserved_symbol_merge( const vstr& tokens ); 
// Return a vector of tokenized strings that a separated by whitespace within `expStr`
vstr tokenize( const string& totStr, char sepChr = ' ' );


////////// FILE OPERATIONS /////////////////////////////////////////////////////////////////////////
vstr read_file_to_lines( string path ); // Read plain text to a vector of strings, separated by newlines


////////// LEXER ///////////////////////////////////////////////////////////////////////////////////
class LexMachine{ public:
    // The most basic lexing imaginable
    string progPath;
    vstr   lines;
    vvstr  lineTokens;

    LexMachine( string fPath );
};

#endif