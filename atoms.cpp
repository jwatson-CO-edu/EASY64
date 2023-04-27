// g++ atoms.cpp -std=c++17

////////// INIT ////////////////////////////////////////////////////////////////////////////////////

/// Imports ///
#include <string>
using std::string;
#include <map>
using std::map;
#include <vector>
using std::vector;
#include <stack>
using std::stack;
#include <sstream>
using std::istringstream;
#include <iostream>
using std::noskipws, std::skipws, std::cout, std::endl, std::flush, std::ostream;

/// Aliases ///
typedef unsigned long  ulong;
typedef vector<string> vstr;



////////// HELPER FUNCTIONS ////////////////////////////////////////////////////////////////////////

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
    E_Error err;
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
    // Set aliases for reserved tokens and words
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
    // cout << "There are " << expStr.size() << " characters to examine! ";
    // cout << string(";").size() << endl;
    // 1. For every character in the string
    for( char ch_i : expStr ){
        // 2. Fetch character
        c    = ch_i;
        cStr = string(1,c);
        cout << cStr << endl;
        // 3. Either add char to present token or create a new one
        // A. Case Reserved
        if( p_reserved_token( cStr ) ){  
            // cout << "Reserved Token: " << find_reserved_token( cStr ) << endl;
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
    // Parser Modes
    INPUT, // - Begin parser
    TERM, // -- Halt parser
    // Types of AST Node
    DEFINE, //- Identifier definition
    ASSIGN, //- Identifier assignment
    LITERAL, // Primitive data
    RHV,
    MATH,
};

struct AST_Node{
    // Node of an Abstract Syntax Tree
    E_Expr /*------*/ exp; // Expression type
    E_Type /*------*/ typ; // Datatype it resolves to
    E_Error /*-----*/ err; // Error at this level of parsing
    string /*------*/ msg; // Message to user
    vector<AST_Node*> opr; // Operands
    void* /*-------*/ dat; // Data payload
    ulong /*-------*/ lin; // Line number of source
};

map<string,AST_Node*(*)(const vstr&)> RESERVED_PARSERS;

void init_parsers(){

    /// Integer Declaration ///
    RESERVED_PARSERS["int"] = [](const vstr& args)->AST_Node*{ 
        
        // 1. Set identifier name
        AST_Node* name = new AST_Node{};
        name->exp = LITERAL;
        name->typ = STRNG;
        name->err = OKAY;
        name->dat = new Var64<string>{ args[0], STRNG, OKAY };

        // 2. Set parent
        AST_Node* node = new AST_Node{};
        node->exp = DEFINE;
        node->typ = INTGR;
        node->err = OKAY;
        node->opr.push_back( name );

        // N. Return
        return node;
    };
}

struct EASY_Job{
    ulong  tokenDex; //- Index of `tokenExpr` to parse
    E_Expr currMode; //- Current mode of the parser
};

class EASY_Parser{ public:
    // Parsing Finite State Machine for EASY64: As if parsing were ever easy!

    /// Members ///
    vstr /*-------*/ tokenExpr; // Expression we are attempting to parse
    stack<EASY_Job*> jobs; // ---- Stack of jobs for parslets
    
    void flush_jobs(){
        // Erase all jobs
        while( !jobs.empty() ){
            if( jobs.top() )  delete jobs.top();
            jobs.pop();
        }
    }

    void init(){
        // Flush jobs and push the initial job
        EASY_Job* initJob = new EASY_Job{ 0, INPUT };
        flush_jobs();
        jobs.push( initJob );
    }

    EASY_Job* pop_job(){
        // Pop and return the top job on the stack, If no jobs, then return `nullptr`
        EASY_Job* rtnJob = nullptr;
        if( !jobs.empty() ){
            rtnJob = jobs.top();
            jobs.pop();
        }
        return rtnJob;
    }

    bool p_jobs_exist(){  return (!jobs.empty());  } // Are there jobs to process?

    AST_Node* parse_tokens( const vstr& tokens ){
        // Turn the sequence of tokens into an Abstract Syntax Tree
        AST_Node* ASTexpr = nullptr; // Fully parsed expression
        EASY_Job* currJob = nullptr; // Current parselet job
        
        // 0. Copy the token expression
        tokenExpr = tokens; 
        
        // 1. While jobs exist, process jobs
        while( p_jobs_exist() ){
            // Fetch job
            currJob = pop_job();
            // Handle job
            // Delete job
        }

        return ASTexpr;
    }
};




////////// TESTS ///////////////////////////////////////////////////////////////////////////////////

int main(){
    init_reserved();
    cout << tokenize_ws(  "int i;"  )  << endl;
    return 0;
}
