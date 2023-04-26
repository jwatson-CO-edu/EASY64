////////// INIT ////////////////////////////////////////////////////////////////////////////////////

/// Imports ///
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

/// Aliases ///
typedef unsigned long  ulong;
typedef vector<string> vstr;



////////// ATOMS ///////////////////////////////////////////////////////////////////////////////////

enum E_Type{
    // Primitive Data Types
    FLOAT, // `double`
    INTGR, // `long`
    U_INT, // `unsigned long`
    STRNG, // `string`
    ERROR, // `string`
};

enum E_Error{
    // Basic Error Types
    OKAY,
};

template <typename T>
struct Var64{
    // A container for a variable
    T /*-*/ data;
    E_Type  type;
    E_Error err
    // ulong  refCount; // 2023-04-20: Not used?
};



////////// CONTEXT /////////////////////////////////////////////////////////////////////////////////

struct Context{
    // Container for data in this context
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
    RESERVED_TOKENS[";"] = "semicolon"; // Semicolon
    RESERVED_TOKENS["("] = "open_parn"; // Open  paren
    RESERVED_TOKENS[")"] = "clos_parn"; // Close paren
    RESERVED_TOKENS["{"] = "open_crly"; // Open  curly brace
    RESERVED_TOKENS["}"] = "clos_crly"; // Close curly brace
    // Reserved words CAN stand on their own ONLY when they are NOT part of an identifier
    RESERVED_WORDS["int"   ] = "integerDc"; // Integer declaration
    RESERVED_WORDS["uint"  ] = "unsignedD"; // Unsigned integer declaration
    RESERVED_WORDS["float" ] = "floatDecl"; // Float declaration
    RESERVED_WORDS["string"] = "stringDcl"; // String declaration
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

vstr tokenize_ws( string expStr ){
    // Return a vector of tokenized strings that a separated by whitespace within `expStr`
    vstr   tokens;
    char   c;
    string cStr;
    string token  = "";

    // Helpers //
    auto stow_token = [&]{  tokens.push_back( token );  token = "";  };
    auto stow_char  = [&]{  tokens.push_back( string(1,c) );  };
    auto cache_char = [&]{  token.push_back( c );  };

    // 0. Apply the postfix hack
    expStr.push_back(' ');
    // 1. For every character in the string
    for( char ch_i : expStr ){
        // 2. Fetch character
        c    = ch_i;
        cStr = string(1,c);
        // 3. Either add char to present token or create a new one
        // A. Case Reserved
        if( p_reserved_token( cStr ) ){  
            if( token.size() > 0 ){  stow_token();  }
            stow_char();  
        // B. Case separator
        }else if( isspace(c) ){
            if(token.size() > 0){  stow_token();  }
        // C. Case any other char
        }else{  cache_char();  } 
    }
    // N. Return the vector of tokens
    return tokens;
}



////////// PARSER //////////////////////////////////////////////////////////////////////////////////
// The parser recognizes structures that can be converted to ASTs

///// ABSTRACT SYNTAX TREE ///////////////////////

enum E_Expr{
    // Types of AST Node
    ASSIGN,
    LITERAL,
    RHV,
    DEFINE,
    MATH,
};

struct AST_Node{
    // Node of an Abstract Syntax Tree
    E_Expr /*------*/ exp; // Expression type
    E_Type /*------*/ typ; // Datatype it resolves to
    E_Error /*-----*/ err; // Error at this level of parsing
    vector<AST_Node*> opr; // Operands
    void* /*-------*/ dat; // Data payload
};



////////// TESTS ///////////////////////////////////////////////////////////////////////////////////

int main(){
    return 0;
}
