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
using std::map, std::pair;
#include <stack>
using std::stack;
#include <fstream>
using std::ifstream;
#include <sstream>
using std::stringstream, std::getline;
#include <iostream>
using std::cout, std::endl, std::flush, std::cerr, std::ostream, std::cin;
#include <stdexcept>
using std::runtime_error;
#include <memory>
using std::shared_ptr, std::weak_ptr;
#include <filesystem>
using std::filesystem::exists;
#include <variant>
using std::variant, std::get, std::holds_alternative;
#include <cmath>
using std::nan, std::isnan;
#include <cstdlib>
using std::strtoull;

///// Aliases /////////////////////////////////////////////////////////////
typedef size_t /*-------*/ ullong;
typedef long long int /**/ llong;
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

const array<string,2> BUILTINS = { /// Included Functions ///
    "writeln", "sqrt",
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
////////// primitives.cpp //////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

////////// PRIMITIVE TYPES /////////////////////////////////////////////////////////////////////////
typedef variant<double,llong,char,bool> P_Val; // Primitive Values // WARNING: ASSUMPTION 

ostream& operator<<(ostream& os, const P_Val& v);

P_Val make_nan();
P_Val make_double();
P_Val make_llong();
P_Val make_char();
P_Val make_bool();

bool p_nan( const P_Val& q );

P_Val str_2_primitive( const string& q ); // ------------ Return interpret `q` as a primitive literal and return it
P_Val operator+( const P_Val& lhs, const P_Val& rhs ); // Add two numeric variants
P_Val operator+=( P_Val& lhs, const P_Val& rhs ); // ---- Add two numeric variants and store in `lhs`
P_Val operator-( const P_Val& lhs, const P_Val& rhs ); // Subtract two numeric variants
P_Val operator*( const P_Val& lhs, const P_Val& rhs ); // Multiply two numeric variants
P_Val operator/( const P_Val& lhs, const P_Val& rhs ); // Divide two numeric variants
P_Val pow( const P_Val& lhs, const P_Val& rhs ); // ----- Raise `lhs` to the `rhs` power
double as_double( const P_Val& val );


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
ostream& operator<<( ostream& os , const vector<T>& vec ){ 
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


template<typename T>
ostream& operator<<( ostream& os , const stack<T>& stk ){ 
    // ostream '<<' operator for stacks
    // NOTE: This function assumes that the ostream '<<' operator for T has already been defined
    stack<T> pStk = stk;
    size_t   i    = 0;
    size_t   N    = pStk.size();
    T        elem;
    os << "(top)|| ";
    if( !pStk.size() ){  os << "<empty>";  }
    while( pStk.size() ){
        elem = pStk.top();
        os << elem;
        pStk.pop();
        if((i+1) < N){ os << " | "; }
        ++i;
    }
    os << " ||(btm)";
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
////////// parser.cpp //////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

////////// SOURCE TREE /////////////////////////////////////////////////////////////////////////////

enum NodeType{
    INVALID, 
    PROGRAM, 
    CON_SCTN, 
    VAR_SCTN, 
    TYP_SCTN, 
    CODE_BLC,
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
    FOR_LOOP,
    MATH_EXPR,
    TYPENAME,
};

class ProgNode;
typedef shared_ptr<ProgNode> NodePtr;

class ProgNode{ public:
    NodeType /*--*/ type = INVALID;
    vstr /*------*/ tokens;
    vector<NodePtr> edges;

    ProgNode( NodeType typ, vstr tkns );
};


////////// PARSER //////////////////////////////////////////////////////////////////////////////////
bool p_literal_math_expr( const vstr& tokens ); // Does this expression contain only numbers and infix math operators?
bool p_ident_math_expr( const vstr& tokens ); // Does this expression contain only numbers, infix math operators, and identifiers?
// Does this expression contain only numbers, infix math operators, identifiers, and function calls?
bool p_func_math_expr( const vstr& tokens, bool allowComma = false ); 
bool p_number_string( const string& q ); // Return true if the string can represent a primitive
bool p_math_op( const string& q ); // Is this string an infix math operator?
bool p_vstr_has_str( const vstr& vec, const string& q ); // Return true if the `vstr` contains `q`
vstr get_parenthetical( const vstr& expr, size_t bgn = 0 ); // Get the contents of balanced parentheses starting at `bgn`

enum ParseMode{ 
    // What is the parser working on?
    BEGIN, 
    COMMENT, 
    VAR_SECT,
    CONST_SECT,
    MAIN_SECT,
    FOR_BODY,
    FUNC_CALL,
};


class CPC_Parser{ public:
    NodePtr /*---*/ header;    
    vector<NodePtr> code;

    CPC_Parser(); // Default Constructor

    // Build a cheap Abstract Source Tree to be executed later
    NodePtr build_source_tree( const vvstr& lineTokens, ParseMode bgnMode = BEGIN ); 
};



////////////////////////////////////////////////////////////////////////////////////////////////////
////////// interpreter.cpp /////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
class Context;
typedef shared_ptr<Context> CntxPtr;

ostream& operator<<( ostream& os , const P_Val& v ); // Helper function to stream variant contents

////////// CONTEXT /////////////////////////////////////////////////////////////////////////////////

class Context{ public:
    CntxPtr /*-----*/ parent = nullptr;
    map<string,P_Val> constants; // Values that can't change
    map<string,P_Val> variables; // Values that change

    bool  p_name_defined( const string& ident );
    P_Val get_value_by_name( const string& ident );
    bool  set_value_by_name( const string& ident, const P_Val& val );

    void print_constant_state(); // Print all the constants in this context
    void print_variable_state(); // Print all the variables in this context
};


////////// INTERPRETER /////////////////////////////////////////////////////////////////////////////

class CPC_Interpreter : public std::enable_shared_from_this<CPC_Interpreter>{ public:
    // Implements the CPluscal Computer Language

    /// Members ///
    CntxPtr context = nullptr;
    
    /// Constructors ///
    CPC_Interpreter();

    /// Arithmetic ///
    P_Val calculate( const vstr& expr, CntxPtr cntx ); // Implements a stack-based calculator

    /// Built-In Functions ///
    P_Val writeln( const vstr& args, CntxPtr cntx ); // Basic print followed by a newline
    P_Val sqrt( const vstr& args, CntxPtr cntx ); // Compute the square root of the expression
    P_Val read( const vstr& args, CntxPtr cntx ); // Read input from the user

    /// Code Execution ///
    P_Val interpret( NodePtr sourceTree, CntxPtr cntx = nullptr ); // RUN THE CODE (Source Tree)!
};


#endif