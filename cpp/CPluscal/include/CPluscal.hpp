#ifndef CPLUSCAL_HPP
#define CPLUSCAL_HPP

////////// INIT ////////////////////////////////////////////////////////////////////////////////////

///// Imports /////////////////////////////////////////////////////////////
#include <string>
using std::string;
#include <array>
using std::array;
#include <vector>
using std::vector;
#include <deque>
using std::deque;
#include <list>
using std::list;
#include <map>
using std::map;
#include <fstream>
using std::ifstream;
#include <sstream>
using std::stringstream, std::getline;
#include <iostream>
using std::cout, std::endl, std::flush, std::cerr, std::ostream;
#include <stdexcept>
using std::runtime_error;
#include <memory>
using std::shared_ptr;
#include <filesystem>
using std::filesystem::exists;
#include <variant>
using std::variant, std::get, std::holds_alternative;

///// Aliases /////////////////////////////////////////////////////////////
typedef size_t /*-------*/ ullong;
typedef long long /*----*/ llong;
typedef unsigned char /**/ ubyte;
typedef vector<string>     vstr;
typedef vector<vstr> /*-*/ vvstr; 



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

const array<string,1> BUILTINS = { /// Included Functions ///
    "writeln"
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



////////// PRIMITIVE TYPES /////////////////////////////////////////////////////////////////////////
typedef variant<double,llong,char,bool> P_Val; // Primitive Values // WARNING: ASSUMPTION 



////////// CONTAINERS //////////////////////////////////////////////////////////////////////////////

template<typename T>
size_t vec_find_index( const vector<T>& vec, const T q ){
    // Return the index of an element, if it can be found
    size_t i = 0;
    for( const T& elem : vec ){  if( elem == q ){  return i;  }  ++i;  }
    return SIZE_MAX;
}


template<typename T>
vector<T> get_sub_vec( const vector<T>& vec, size_t bgn, size_t end ){
    // Return the specified subvector
    vector<T> rtnVec;
    if( (bgn < end) && (end < vec.size()) || (bgn < vec.size()) ){  
        rtnVec = vector<T>{ vec.begin()+bgn, vec.begin()+end };
    }
    return rtnVec;
}


template<typename T>
ostream& operator<<( ostream& os , const deque<T>& deq ) { 
    // ostream '<<' operator for vectors
    // NOTE: This function assumes that the ostream '<<' operator for T has already been defined
    os << "(front)[ ";
    for (size_t i = 0; i < deq.size(); i++) {
        os << (T) deq[i];
        if (i + 1 < deq.size()) { os << ", "; }
    }
    os << " ](back)";
    return os; // You must return a reference to the stream!
} 


template<typename T>
ostream& operator<<( ostream& os , const vector<T>& vec ) { 
    // ostream '<<' operator for vectors
    // NOTE: This function assumes that the ostream '<<' operator for T has already been defined
    os << "[ ";
    for (size_t i = 0; i < vec.size(); i++) {
        os << (T) vec[i];
        if (i + 1 < vec.size()) { os << ", "; }
    }
    os << " ]";
    return os; // You must return a reference to the stream!
}



////////////////////////////////////////////////////////////////////////////////////////////////////
////////// lexer.cpp ///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

////////// STRING PROCESSING ///////////////////////////////////////////////////////////////////////
size_t vstr_find_index( const vstr& vec, const string& q );
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
// Return the first existing file path, Otherwise return an empty string if none exist
string first_existing_file_path( const vstr& possiblePaths );
    


////////// LEXER ///////////////////////////////////////////////////////////////////////////////////
class LexMachine{ public:
    // The most basic lexing imaginable
    string progPath;
    vstr   lines;
    vvstr  lineTokens;

    LexMachine();
    LexMachine( string fPath );
};



////////////////////////////////////////////////////////////////////////////////////////////////////
////////// interpreter.cpp /////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

////////// SOURCE TREE /////////////////////////////////////////////////////////////////////////////

enum NodeType{
    INVALID, 
    PROGRAM, 
    VAR_SCTN, 
    CON_SCTN, 
    TYP_SCTN, 
    VAR_DECL, 
    ASSIGNMENT, 
    BINARY_OP, 
    NUMBER, 
    IDENTIFIER,
    COMPOUND, 
    IF_STMT, 
    WHILE_STMT, 
    COMPARISON,
    FUNCTION,
    ARGUMENTS,
    MATH_EXPR,
    TYPENAME,
};

class ProgNode;
typedef shared_ptr<ProgNode> NodePtr;

class ProgNode{ public:
    NodeType /**/ type = INVALID;
    vstr /*----*/ tokens;
    list<NodePtr> edges;

    ProgNode( NodeType typ, vstr tkns );
};


////////// INTERPRETER /////////////////////////////////////////////////////////////////////////////

class Context;
typedef shared_ptr<Context> CntxPtr;

class Context{ public:
    CntxPtr /*-*/ parent = nullptr;
    list<NodePtr> types;
    list<NodePtr> constants;
    list<NodePtr> variables;
};


class CPC_Interpreter{ public:
    LexMachine    lexer;
    NodePtr /*-*/ header;    
    CntxPtr /*-*/ context;
    list<NodePtr> code;

    CPC_Interpreter(); // Default Constructor

    bool load_program_file( string fPath ); // ------------------------ Invoke the lexer
    NodePtr build_source_tree( CntxPtr cntx, const vvstr& lineTokens ); // Build a cheap Abstract Source Tree to be executed later
};



#endif