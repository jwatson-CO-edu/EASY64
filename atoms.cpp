////////// INIT ////////////////////////////////////////////////////////////////////////////////////

#include <string>
using std::string;
#include <map>
using std::map;
#include <vector>
using std::vector;
#include <sstream>
using std::istringstream;
#include <iostream>
using std::noskipws, std::skipws;

typedef unsigned long ulong;

////////// ATOMS ///////////////////////////////////////////////////////////////////////////////////

enum E_Type{
    FLOAT,
    INTGR,
    U_INT,
    ERROR,
};

enum E_Error{
    OKAY,
};

template <typename T>
struct Var64{
    T /*-*/ data;
    E_Type  type;
    E_Error err
    // ulong  refCount; // 2023-04-20: Not used?
};


////////// CONTEXT /////////////////////////////////////////////////////////////////////////////////

struct Context{
    Context* /*-------------*/ parent;
    map<string, Var64<double>> flotStore;
    map<string, Var64<long>>   intrStore;
    map<string, Var64<ulong>>  uintStore;
    map<string, Var64<string>> strnStore;
};

////////// LEXER ///////////////////////////////////////////////////////////////////////////////////
// The lexer recognizes tokens

///// Predicates /////

bool p_float_string( string myString ) {
    // Original Author: Bill the Lizard, https://stackoverflow.com/a/447307
    std::istringstream iss( myString );
    double f;
    iss >> skipws >> f; // skipws considers leading whitespace valid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail(); 
}

bool p_int_string( string myString ) {
    // Original Author: Bill the Lizard, https://stackoverflow.com/a/447307
    std::istringstream iss( myString );
    long f;
    iss >> skipws >> f; // skipws considers leading whitespace valid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail(); 
}

bool p_uint_string( string myString ) {
    // Original Author: Bill the Lizard, https://stackoverflow.com/a/447307
    std::istringstream iss( myString );
    ulong f;
    iss >> skipws >> f; // skipws considers leading whitespace valid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail(); 
}

map<string,string> RESERVED_TOKENS; // Reserved tokens ALWAYS stand on their own and are NEVER part of an identifier
map<string,string> RESERVED_WORDS; //- Reserved words CAN stand on their own ONLY when they are NOT part of an identifier

void init_reserved(){
    // Reserved tokens ALWAYS stand on their own and are NEVER part of an identifier
    RESERVED_TOKENS["("] = "open_parn"; // Open  paren
    RESERVED_TOKENS[")"] = "clos_parn"; // Close paren
    RESERVED_TOKENS[";"] = "semicolon"; // Semicolon
    RESERVED_TOKENS["{"] = "open_crly"; // Open  curly brace
    RESERVED_TOKENS["}"] = "clos_crly"; // Close curly brace
    // Reserved words CAN stand on their own ONLY when they are NOT part of an identifier
    RESERVED_WORDS["int"  ] = "integerDc"; // Integer declaration
    RESERVED_WORDS["uint" ] = "unsignedD"; // Unsigned integer declaration
    RESERVED_WORDS["float"] = "floatDecl"; // Float declaration
}

bool p_reserved_token( string token ){  return (RESERVED_TOKENS.count( token ) > 0);  } // Is this a reserved token?
bool p_reserved_word(  string word  ){  return (RESERVED_TOKENS.count( word  ) > 0);  } // Is this a reserved word?

string find_reserved_token( string token ){
    // Return the name of the reserved token, Otherwise return empty string if DNE
    if( p_reserved_token( token ) )
        return RESERVED_TOKENS[ token ];
    else
        return "";
}

string find_reserved_word( string word ){
    // Return the name of the reserved word, Otherwise return empty string if DNE
    if( p_reserved_word( word ) )
        return RESERVED_WORDS[ word ];
    else
        return "";
}

vector<string> tokenize_ws( string expStr ){
    // Return a vector of tokenized strings that a separated by whitespace within `expStr`
    vector<string> tokens;
    // FIXME, START HERE: LINE 50 of "lexer.d"
}


////////// PARSER //////////////////////////////////////////////////////////////////////////////////
// The parder recognizes structures that can be converted to ASTs

///// ABSTRACT SYNTAX TREE ///////////////////////

enum E_Expr{
    ASSIGN,
    LITERAL,
    RHV,
    DEFINE,
    MATH,
};

struct AST_Expr{
    E_Expr /*------*/ exp; // Expression type
    E_Type /*------*/ typ; // Datatype it resolves to
    E_Error /*-----*/ err; // Error at this level of parsing
    vector<AST_Expr*> opr; // Operands
    void* /*-------*/ dat; // Data payload
};

