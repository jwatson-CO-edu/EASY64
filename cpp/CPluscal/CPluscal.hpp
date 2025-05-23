#ifndef CPLUSCAL_HPP
#define CPLUSCAL_HPP

////////// INIT ////////////////////////////////////////////////////////////////////////////////////

#include "utils.hpp"

/// Imports ///
#include <array>
using std::array;
#include <set>
using std::set;
#include <stack>
using std::stack;
#include <variant>
using std::variant, std::get, std::holds_alternative;
#include <type_traits>
using std::visit;
#include <memory>
using std::shared_ptr;
#include <fstream>
using std::ifstream;
#include <stdexcept>
using std::runtime_error;
#include <climits>
#include <cmath>
using std::nan, std::isnan, std::pow;
#include <algorithm>
using std::min, std::max;
using std::is_same_v;


/// Defines ///
#define INVALID    SIZE_MAX
#define NOT_FOUND  SIZE_MAX
#define MIN_CHAR_S 3



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



////////// LANGUAGE ENUMS //////////////////////////////////////////////////////////////////////////

enum TypeName{
    // Data Types
    PRIMITIVE,
    ALIAS,
    RANGE_VALUE,
    RANGE_STRING,
    ENUM,
    ARRAY,
    P_FILE,
    RECORD,
    ERROR,
};



////////////////////////////////////////////////////////////////////////////////////////////////////
////////// types_builtin.cpp ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


////////// PRIMITIVE TYPES /////////////////////////////////////////////////////////////////////////
typedef variant<double,long,char,bool> P_Val; // Primitive Values // WARNING: ASSUMPTION 

P_Val make_nan();
bool p_nan( const P_Val& q );

P_Val operator+( const P_Val& lhs, const P_Val& rhs );
P_Val operator-( const P_Val& lhs, const P_Val& rhs );
P_Val operator*( const P_Val& lhs, const P_Val& rhs );
P_Val operator/( const P_Val& lhs, const P_Val& rhs );
P_Val pow( const P_Val& lhs, const P_Val& rhs );

////////// COMPOUND TYPES //////////////////////////////////////////////////////////////////////////

class Enum{ public:
    vstr items;

    Enum();
    Enum( const vstr& );

    Enum copy(); // Make a copy of the enum
};


class ValRange{ public:
    // Represents a range of numbers that can be iterated

    P_Val valMin;
    P_Val valMax;
    P_Val incr;
    P_Val valCur;
    bool  done;

    ValRange(); // Set up the range for iteration
    ValRange( const P_Val& valMin_, const P_Val& valMax_ ); // Set up the range for iteration

    P_Val yield(); // Yield the next value and notify if done
    ValRange copy();
};


class StrRange{ public:
    // This is basically an Enum and I don't really get it!
    vstr values;

    StrRange();
    StrRange( const vstr& values_ );

    StrRange copy();
};


class Array{ protected:
    // AM I ABLE BOTH TO ASSIGN AND TO RETRIEVE?
    struct elem{
        P_Val val;
        P_Val operator=( const P_Val& val_ ){
            val = val_;
            return val;
        }
    };
    vector<elem> values;

    public: 

    Array();
    Array( size_t N );

    Array copy_empty();
    P_Val& operator[]( const size_t& index );
    size_t size() const;
};


class Record{ public: 
    map<string,P_Val> pVars; // Variables, Primitive Types
    map<string,vstr>  condFields; // Conditional Fields

    Record();

    Record copy();
};


class SetString{ public:
    // Set of string -or- enum
    vstr /*-----------------------*/ allowed;
    set<string> /*----------------*/ values;
    pair<set<string>::iterator,bool> res;

    void restrict( const vstr& allowedStrings );

    bool add( const string& val );
};


class SetValue{ public:
    set<P_Val> /*----------------*/ values; // WARNING, ASSUMPTION: C++ IS ABLE TO DIFFERIENTIATE VALUES
    pair<set<P_Val>::iterator,bool> res;

    bool add( const P_Val& val );
};


enum FileMode{
    INSPECTION,
    GENERATION,
};


class P_File{ public:
    // WARNING: NEED TO READ ABOUT PASCAL FILE MODEL
    FileMode mode;
    string   path;
    string   contents;
};






////////////////////////////////////////////////////////////////////////////////////////////////////
////////// lexing_basic.cpp ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
bool p_special( const string& q ); // Return True if `q` matches a symbol, otherwise return False
bool p_reserved( const string& q ); // Return True if `q` matches a symbol, otherwise return False
bool p_prim_type( const string& q ); // Return True if `q` matches a primtive type name, otherwise return False
bool p_infix_op( const string& q );
// Return True for strings that are: 1. Not symbols, 2. Begin with a letter, 3. Are composed of alphanumeric chars
bool p_identifier( const string& q );
    
    
bool  p_number_string( const string& q );
P_Val str_2_number( const string& q );

vvstr  segment_statements( const vstr& tokens_ );
vvstr  text_block_to_tokenized_statements( const string& textBlock );
string concat( const vstr& parts, char sepChar = 0 );

bool p_has_balanced_parenthetical( const vstr& tokens );
bool p_has_balanced_bracketed( const vstr& tokens );
vstr get_balanced_parenthetical_tokens( const vstr& tokens );
vstr get_balanced_bracketed_tokens( const vstr& tokens );

string strip( const string& inputStr );
// Return a vector of tokenized strings that a separated by whitespace within `expStr`
vstr   tokenize( const string& totStr, char sepChr = ' ' ); 
size_t q_line_begins_comment( const string& q ); // Does the stripped line begin a comment? 
bool   p_str_has( const string& str, const string& q );
size_t q_line_ends_comment( const string& q ); // Does the stripped line end a comment?



////////////////////////////////////////////////////////////////////////////////////////////////////
////////// lexing_file.cpp /////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

////////// FILE LEXING /////////////////////////////////////////////////////////////////////////////

template <typename T>
struct TextPortions{ 
    // Separates text of a file into portions so that we can treat each differently
    /// Sections ///
    T header;
    T type;
    T cnst;
    T var;
    /// Modules ///
    T func;
    T proc;
    /// Main ///
    T prog;
};


vstr read_file_to_lines( string path );
TextPortions<vstr> segment_source_file( const vstr& totLines );



////////////////////////////////////////////////////////////////////////////////////////////////////
////////// AST_Node.cpp ////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

////////// NODE ////////////////////////////////////////////////////////////////////////////////////
// NOTE: DELAY INTERPRETATION OF STRINGS UNTIL IT IS TIME TO RUN CODE

enum NodeType{
    // Types of syntax nodes
    EMPTY,
    PROGRAM, 
    TYPE_DEF, 
    CONSTANT_DEF, 
    VARIABLE_DEF, 
    TYPE_NAME,
    ASSIGNMENT,
    IDENTIFIER,
    NUM_LITERAL,
    BINARY_OP,
    FUNCTION, 
    FOR,
};


struct AST_Node{ 
    // Holds code as a tree
    NodeType /*---*/ type;
    vstr /*-------*/ tokens;
    vector<AST_Node> edges;
    P_Val /*------*/ value;
    size_t /*-----*/ depth;
    bool /*-------*/ visited;

    AST_Node(){
        type    = EMPTY;
        value   = make_nan();
        depth   = 0;
        visited = false;
    }
};


AST_Node make_ast_program( const string& name ); // Identifier Node, to be evaluated later
AST_Node make_ast_type_def(); // Identifier Node, to be evaluated later
AST_Node make_ast_const_def(); // Identifier Node, to be evaluated later
AST_Node make_ast_var_def(); // Identifier Node, to be evaluated later
AST_Node make_ast_type_name( const string& name ); // Identifier Node, to be evaluated later
AST_Node make_ast_identifier( const string& name ); // Identifier Node, to be evaluated later
AST_Node make_ast_number_literal( const string& literalStr ); // Number Literal Node, to be evaluated later
AST_Node make_ast_assignment( AST_Node left, AST_Node right ); // Number Assignment Node, to be evaluated later
AST_Node make_ast_binary_op( const string& op, AST_Node left, AST_Node right ); // Number Assignment Node, to be evaluated later


////////// PARSER //////////////////////////////////////////////////////////////////////////////////

class AST_Parser{ public:
    // Machine that turns a string vector into an Abstract Source Tree

    deque<string> input;
    string /*--*/ curTkn;
    size_t /*--*/ N;

    void load_program_tokens( const vstr& tokens_ ); // Load tokenized program into the parser

    AST_Parser(); // Empty constructor
    AST_Parser( const vstr& tokens_ ); // Load tokenized program into the parser

    bool advance(); // Move to next token string
    bool match( const string& expected ); // Return True if the current token string matches what is `expected`
    void expect( const string& expected ); // Throw a `runtime_error` if `match` fails

    AST_Node parse_term(); // Parse one expression term as an AST node
    AST_Node parse_expression(); // Read tokens until an expression subtree is obtained
    AST_Node parse( const string& name ); // Convert the collection of tokens to an Abstract Source Tree 
};


TextPortions<AST_Node> parse_sections( TextPortions<vstr>& tokens ); // Turn each section into trees



////////////////////////////////////////////////////////////////////////////////////////////////////
////////// interpreter.cpp /////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

////////// TYPES && SCOPE //////////////////////////////////////////////////////////////////////////
class ValStore{ public:
    // Lookup for values by name
    AST_Node /*--------*/ expr;
    map<string,string>    pAlias; // Aliases for primitive types
    map<string,P_Val>     prim; // -- Variables of primitive types
    map<string,ValRange>  valrange; 
    map<string,StrRange>  strrange; 
    map<string,Enum> /**/ namedEnum; 
    map<string,Array>     namedArray; 
    map<string,P_File>    file; 
    map<string,Record>    record; 
    map<string,SetString> setstring; 
    map<string,SetValue>  setvalue; 

    void set_builtins(); // Set values that Pascal knows about

    ValStore(); // Setup this context

    bool p_var_name( const string& name ); // Is there a variable stored under this `name`?
    bool p_alias_name( const string& name ); // Is there a variable stored under this `name`?
    bool p_arr_name( const string& name ); // Is there a variable stored under this `name`?
    bool p_num_range_name( const string& name ); // Is there a variable stored under this `name`?
    bool p_str_range_name( const string& name ); // Is there a variable stored under this `name`?
    bool p_file_name( const string& name ); // Is there a variable stored under this `name`?
    bool p_record_name( const string& name ); // Is there a variable stored under this `name`?
    bool p_enum_name( const string& name ); // Is there a variable stored under this `name`?

    P_Val get_var( const string& name ); // Get var by `name` if it exists, otherwise return NaN
    // If the `token` is a variable or a literal, then return its value, Otherwise return NaN
    P_Val get_var_or_literal( const string& token ); 
    // Return the primitive name associated with the alias, otherwise return an empty string
    string   resolve_prim_alias( const string& name );
    TypeName where_name( const string& name ); // Identify what kind of thing the identifier identifies
};



///// Execution Context ///////////////////////////////////////////////////

class Context{ public:
    // Resolves names
    ValStore types;
    ValStore constants;
    ValStore vars;

    P_Val resolve_primitive_name( const string& name );
    bool  p_math_expr( const vstr& expr );
};

// ///// Type Definition Part /////
// void define_types( Context& context, const string& defText );

// ///// Constant Definition Part /////
// void define_constants( Context& context, string defText );

// ///// Variable Definition Part /////
// void define_variables( Context& context, string defText );



////////// THE MACHINE /////////////////////////////////////////////////////////////////////////////

class PascalInterpreter{ public:
    // The actual interpreter
    string  progName;
    bool    enableInput;
    bool    enableOutput;
    Context context;
    

    PascalInterpreter();
    PascalInterpreter( const Context& context_ );

    void set_IO( bool in, bool out );
    void init_file( const string& sourcePath );
};



////////// END /////////////////////////////////////////////////////////////////////////////////////
#endif