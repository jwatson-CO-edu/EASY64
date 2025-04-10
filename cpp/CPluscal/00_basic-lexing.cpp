////////// INIT ////////////////////////////////////////////////////////////////////////////////////

#include <array>
using std::array;
#include <vector>
using std::vector;
#include <deque>
using std::deque;
#include <set>
using std::set;
#include <string>
using std::string;
#include <map>
using std::map;
#include <variant>
using std::variant, std::get, std::holds_alternative;
#include <type_traits>
using std::visit;
#include <memory>
using std::shared_ptr;
#include <fstream>
using std::ifstream;
#include <iostream>
using std::cout, std::endl, std::flush, std::cerr, std::ostream;
#include <stdexcept>
using std::runtime_error;
#include <sstream>
using std::stringstream, std::getline;
#include <climits>
#include <cmath>
using std::nan, std::isnan;

/// Aliases ///
typedef unsigned long  ulong;
typedef unsigned char  ubyte;
typedef vector<string> vstr;
typedef vector<vstr>   vvstr;



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


template<typename T>
ostream& operator<<( ostream& os , const vector<vector<T>>& vec ){ 
    // ostream '<<' operator for vectors
    // NOTE: This function assumes that the ostream '<<' operator for T has already been defined
    os << "[ " << endl;
    for (size_t i = 0; i < vec.size(); i++) {
        os << '\t' << vec[i];
        if (i + 1 < vec.size()) { os << ", "; }
        os << endl;
    }
    os << " ]";
    return os; // You must return a reference to the stream!
}


template<typename T>
bool p_vec_has( const vector<T>& vec, T q ){
    // Return True if the value `q` can be found in the vector
    for( const T& item : vec ){  if( item == q ){  return true;  }  }
    return false;
}


template<typename T>
vector<T> vec_ltrim( const vector<T>& vec, size_t bgnDex ){
    // Get the portion of `vec` from `bgnDex` on
    vector<T> rtnVec;
    if( bgnDex < vec.size() ){  for( size_t i = bgnDex; i < vec.size(); ++i ){  rtnVec.push_back( vec[i] );  }  }
    return rtnVec;
}


////////// LANGUAGE CONSTANTS //////////////////////////////////////////////////////////////////////

const array<char,52> LETTERS = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
    't', 'u', 'v', 'w', 'x', 'y', 'z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
    'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
};

const array<char,10> DIGITS = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

const array<string,22> SYMBOLS = { /// Special Symbols ///
    "+", "-", "*", "/", "=", "<", ">", "<>", "<=", ">=", ":=", "..", "<=", 
    "[", "]", ".", ",", ":", ";", "^", "(", ")"
};

const array<string,35> RESERVED = { /// Word Symbols ///
    "and", "array", "begin", "case", "const", "div", "do", "downto", "else", "end", "file", "for",
    "function", "goto", "if", "in", "label", "mod", "nil", "not", "of", "or", "packed", "procedure",
    "program", "record", "repeat", "set", "then", "to", "type", "until", "var", "while", "with"
};

const array<string,1> PRIM_TYPE_NAMES = { /// Primitive Type Names ///
    "integer"
};

const ubyte MIN_RES_SYMBOL_LEN = 1;
const ubyte MAX_RES_SYMBOL_LEN = 9;

const array<string,2> COMMENT_OPEN = { "{", "(*" };
const array<string,2> COMMENT_CLOS = { "}", "*)" };

const array<string,3> SECTION_NAMES = { "type", "var", "const" };



////////// BASIC LEXING ////////////////////////////////////////////////////////////////////////////

bool p_special( const string& q ){
    // Return True if `q` matches a symbol, otherwise return False
    for( const string& sym : SYMBOLS ){  if( q == sym ){  return true;  }  }
    return false;
}


bool p_reserved( const string& q ){
    // Return True if `q` matches a symbol, otherwise return False
    for( const string& sym : RESERVED ){  if( q == sym ){  return true;  }  }
    return false;
}


bool p_prim_type( const string& q ){
    // Return True if `q` matches a primtive type name, otherwise return False
    for( const string& typ : PRIM_TYPE_NAMES ){  if( q == typ ){  return true;  }  }
    return false;
}


bool p_symbol( const string& q ){  return p_special(q) || p_reserved(q);  }


bool p_letter( const char& q ){
    // Return True if `q` matches a letter, otherwise return False
    for( const char& ltr : LETTERS ){  if( q == ltr ){  return true;  }  }
    return false;
}


bool p_digit( const char& q ){
    // Return True if `q` matches a digit, otherwise return False
    for( const char& dgt : DIGITS ){  if( q == dgt ){  return true;  }  }
    return false;
}


bool p_identifier( const string& q ){
    // Return True for strings that are: 1. Not symbols, 2. Begin with a letter, 3. Are composed of alphanumeric chars
    if( p_special(q) ){  return false;  }
    if( p_reserved(q) ){  return false;  }
    if( !p_letter( q[0] ) ){  return false;  }
    for( const char& ch : q ){
        if( (!p_letter( ch ))&&(!p_digit( ch )) ){  return false;  }
    }
    return true;
}


bool p_section_header( const string& q ){ // WARNING, ASSUMPTION: SECTION HEADINGS OCCUPY THEIR OWN LINE
    // Return True if `q` matches a section name, otherwise return False
    for( const string& nam : SECTION_NAMES ){  if( q == nam ){  return true;  }  }
    return false;
}


string strip( const string& inputStr ){
    // Remove whitespace from the beginning and end of a string
    // Author: Kemin Zhou, https://stackoverflow.com/a/36169979
    if( inputStr.empty() ){  return inputStr;  }
    string::size_type i,j;
    i = 0;
    while( i < inputStr.size() && isspace( inputStr[i]) ){  ++i;  }
    if( i == inputStr.size() ){  return string();  } // empty string
    j = inputStr.size() - 1;
    while( isspace( inputStr[j] ) ){  --j;  }
    return inputStr.substr( i, j-i+1 );
}


size_t q_line_begins_comment( const string& q ){
    // Does the stripped line begin a comment?
    string stripped = strip(q);
    size_t rtnIndex = string::npos;
    rtnIndex = stripped.find( COMMENT_OPEN[0], 0 );
    if( rtnIndex != string::npos ){  return rtnIndex;  }
    rtnIndex = stripped.find( COMMENT_OPEN[1], 0 );
    if( rtnIndex != string::npos ){  return rtnIndex;  }
    return string::npos;
}


size_t q_line_ends_comment( const string& q ){
    // Does the stripped line end a comment?
    string stripped = strip(q);
    size_t rtnIndex = string::npos;
    rtnIndex = stripped.rfind( COMMENT_CLOS[0], 0 );
    if( rtnIndex != string::npos ){  return rtnIndex + COMMENT_CLOS[0].size();  }
    rtnIndex = stripped.rfind( COMMENT_CLOS[1], 0 );
    if( rtnIndex != string::npos ){  return rtnIndex + COMMENT_CLOS[1].size();  }
    return string::npos;
}


array<size_t,2> q_str_has_symbol( const string& q ){
    // Find a symbol in the string, if it exists
    array<size_t,2> rtnIndex = {string::npos, 0};
    for( const string& sym : SYMBOLS ){
        rtnIndex[0] = q.find( sym );
        rtnIndex[1] = sym.size();
        if( rtnIndex[0] != string::npos ){  return rtnIndex;  }
    }
    return {string::npos, 0};
}


vstr attempt_reserved_symbol_merge( const vstr& tokens ){
    // Handle the case when reserved symbols are substrings of each other (2 tokens only!)
    vstr /*----*/ rtnTokens;
    deque<string> accumQue;
    string /*--*/ accumStr = "";
    string /*--*/ validStr = "";

    for( const string& token : tokens ){
        while( accumQue.size() > 1 ){  accumQue.pop_front();  }
        accumQue.push_back( token );
        if( accumQue.size() >= 2 ){  
            accumStr = accumQue[0] + accumQue[1];  
            if( p_special( accumStr ) ){
                rtnTokens.push_back( accumStr );
                accumQue.clear();
            }else{
                rtnTokens.push_back( accumQue[0] );
                accumQue.pop_front();
            }
        }
    }
    while( accumQue.size() ){  
        rtnTokens.push_back( accumQue[0] );
        accumQue.pop_front();  
    }
    return rtnTokens;
}


vstr tokenize( const string& totStr, char sepChr = ' ' ){
    // Return a vector of tokenized strings that a separated by whitespace within `expStr`
    // WARNING, ASSUMPTION: ONE SYMBOL IS NOT A SUBSTRING OF ANOTHER SYMBOL, (There is!)
    vstr   tokens;
    char   c;
    string cStr;
    string token  = "";
    bool   sepWs  = (sepChr == ' ');
    string expStr = totStr + sepChr; // Apply the postfix hack

    // Helpers //
    auto stow_token = [&]{  tokens.push_back( token );  token = "";  };
    auto stow_char  = [&]{  tokens.push_back( string(1,c) );  };
    auto cache_char = [&]{  token.push_back( c );  };

    // 1. For every character in the string
    for( char ch_i : expStr ){
        // 2. Fetch character
        c    = ch_i;
        cStr = string(1,c);
        // 3. Either add char to present token or create a new one
        // A. Case Reserved
        if( p_symbol( cStr ) ){  
            // cout << "Reserved Token: " << find_reserved_token( cStr ) << endl;
            if( token.size() > 0 ){  stow_token();  }
            stow_char();  
        // B. Case separator
        }else if( (c == sepChr) || (sepWs && isspace(c)) ){
            if(token.size() > 0){  stow_token();  }
        // C. Case any other char
        }else{  cache_char();  } 
    }
    // N. Return the vector of tokens
    return attempt_reserved_symbol_merge( tokens );
}


vvstr segment_statements( const vstr& tokens_ ){
    // Separate a token vector into a vector of token vectors, each representing a statement
    vector<vstr> statements;
    vstr /*---*/ statement;
    vstr /*---*/ tokens = tokens_;
    tokens.push_back( ";" ); // Terminator Hack
    for( const string& token : tokens ){
        if( token == ";" ){
            if( statement.size() ){
                statements.push_back( statement );
                statement.clear();
            }
        }else{
            statement.push_back( token );
        }
    }
    return statements;
}


vvstr text_block_to_tokenized_statements( const string& textBlock ){
    // Render a large block of text as a series of statements, each made of string tokens
    return segment_statements( tokenize( textBlock ) );
}





////////// FILE LEXING /////////////////////////////////////////////////////////////////////////////
struct TextPortions{ public:
    // Separates text of a file into portions so that we can treat each differently
    /// Sections ///
    string type;
    string cnst;
    string var;
    /// Modules ///
    string func;
    string proc;
    /// Main ///
    string prog;
};


vstr read_file_to_lines( string path ){
    // Read plain text to a vector of strings, separated by newlines
    ifstream     inputFile;
    stringstream buffer;
    string /*-*/ errStr = "Could not open ";
    string /*-*/ line, addLine;
    vstr /*---*/ totLines;
    inputFile.open( path );
    if( !inputFile.is_open() ){
        cerr << "Error opening " << path << "!" << endl;
        errStr += path;
        inputFile.close();
        throw runtime_error( errStr ); // Exit with an error code
    }else{
        buffer << inputFile.rdbuf(); // https://stackoverflow.com/a/2602258
        while( getline( buffer, line, '\n' ) ){ // https://stackoverflow.com/a/13172579
            addLine = line + '\n';
            totLines.push_back( addLine );
        }
        inputFile.close();
    }
    return totLines;
}


TextPortions segment_source_file( const vstr& totLines ){
    // Load sections of the program into the struct so that we can lex/interpret them
    enum Section{ TYPE, CONST, VAR, COMMENT, OTHER };
    /**/ string /*--*/ trimLine;
    /**/ string /*--*/ line;
    /**/ Section /*-*/ mode     = OTHER;
    /**/ size_t /*--*/ cmmntBgn = string::npos;
    /**/ size_t /*--*/ cmmntEnd = string::npos;
    /**/ size_t /*--*/ N /*--*/ = totLines.size();
    /**/ size_t /*--*/ i /*--*/ = 0;
    /**/ deque<string> qLines;
    /**/ TextPortions  rtnSctns;

    // 0. Load queue with lines
    for( const string& vLine : totLines ){  qLines.push_back( vLine );  }

    // 1. Process each line
    while( qLines.size() ){

        // 2. Fetch line
        line = qLines.at(0);
        qLines.pop_front();
        trimLine = strip( line );

        // cout << trimLine << endl;
        
        // 3. Handle comments
        cmmntBgn = q_line_begins_comment( trimLine );
        if( cmmntBgn == 0 ){  mode = COMMENT;  }
        else if( cmmntBgn != string::npos ){
            qLines.push_front( trimLine.substr( cmmntBgn ) );
            qLines.push_front( trimLine.substr( 0, cmmntBgn ) );
            continue;
        }
        cmmntEnd = q_line_ends_comment( trimLine );
        if( cmmntEnd != string::npos ){  qLines.push_front( trimLine.substr(cmmntEnd) );  }
        
        // 3. Handle section headers
        if( p_section_header( trimLine ) ){ // WARNING, ASSUMPTION: SECTION HEADINGS OCCUPY THEIR OWN LINE
            if( trimLine == "type" ){
                cout << endl << "segment_source_file: Defining `type`s!" << endl << endl;
                mode = TYPE;
            }else if( trimLine == "const" ){
                cout << endl << "segment_source_file: Defining `const`ants!" << endl << endl;
                mode = CONST;
            }else if( trimLine == "var" ){
                cout << endl << "segment_source_file: Defining `var`iables!" << endl << endl;
                mode = VAR;
            }
            continue;
        }

        // 4. Accrue text to sections
        switch( mode ){
            case TYPE:   rtnSctns.type += trimLine;  break;
            case CONST:  rtnSctns.cnst += trimLine;  break;
            case VAR:    rtnSctns.var  += trimLine;  break;
            case OTHER:  rtnSctns.prog += trimLine;  break;            
            default:
                break;
        }
    }
    return rtnSctns;
}




////////// PRIMITIVE TYPES /////////////////////////////////////////////////////////////////////////
enum P_Type{
    // Primitive Data Types
    INT, // `long`
    REAL, // `double`
    BOOL, 
    CHAR, 
};


typedef variant<double,long,char,bool> P_Val; // Primitive Values // WARNING: ASSUMPTION

P_Val make_nan(){  return P_Val{ nan("") };  }

bool p_nan( const P_Val& q ){
    return (holds_alternative<double>( q ) && isnan( get<double>(q) ));
}

P_Val operator+( const P_Val& lhs, const P_Val& rhs ){
    // Add two numeric variants
    // Using std::visit to handle all possible type combinations
    return visit([](auto&& left, auto&& right) -> P_Val {
        // If both are the same type, return that type
        if constexpr ( std::is_same_v< decltype(left), decltype(right) >) {
            return left + right;
        }
        // FIXME: WANT A MORE GRANULAR TYPE ESCALATION HERE
        // If different types, convert to double for maximum precision
        else {
            return static_cast<double>(left) + static_cast<double>(right);
        }
    }, lhs, rhs);
}


void operator+=( P_Val& lhs, const P_Val& rhs ){
    // Increment a numeric variant
    P_Val nuVal = lhs + rhs;
    lhs = nuVal;
}


void print_variant( const P_Val& v ){
    // Helper function to print variant contents
    visit( [](auto&& arg){  cout << arg;  }, v );
}


ostream& operator<<( ostream& os , const P_Val& v ){
    // Helper function to stream variant contents
    visit( [&](auto&& arg){  os << arg;  }, v );
    return os;
}


///// Lex Primitives //////////////////////////////////////////////////////
namespace LexPrim{

bool p_primitive_string( const string& q ){
    // Return true if the string can represent a primitive
    /// Handle `bool` ///
    if( q == "true"  ){  return true;  }
    if( q == "false" ){  return true;  }
    /// Handle `long` ///
    try {
        stol(q);
        return true;
    }catch( const std::invalid_argument& e ){
        cerr << "Invalid argument: " << e.what() << endl;
    } catch( const std::out_of_range& e ){
        cerr << "Out of range: " << e.what() << endl;
    }
    /// Handle `char` ///
    if( q.size() == 1 ){  return true;  }
    /// Handle `double` ///
    try {
        stod(q);
        return true;
    }catch( const std::invalid_argument& e ){
        cerr << "Invalid argument: " << e.what() << endl;
    }catch( const std::out_of_range& e ){
        cerr << "Out of range: " << e.what() << endl;
    }
    return false;
}


P_Val str_2_primitive( const string& q ){
    // Return interpret `q` as a primitive literal and return it
    /// Handle `bool` ///
    if( q == "true"  ){  return P_Val{ true  };  }
    if( q == "false" ){  return P_Val{ false };  }
    /// Handle `long` ///
    try {
        return P_Val{ stol(q) };
    }catch( const std::invalid_argument& e ){
        cerr << "Invalid argument: " << e.what() << endl;
    } catch( const std::out_of_range& e ){
        cerr << "Out of range: " << e.what() << endl;
    }
    /// Handle `char` ///
    if( q.size() == 1 ){  return P_Val{ q[0] };  }
    /// Handle `double` ///
    try {
        return P_Val{ stod(q) };
    }catch( const std::invalid_argument& e ){
        cerr << "Invalid argument: " << e.what() << endl;
    }catch( const std::out_of_range& e ){
        cerr << "Out of range: " << e.what() << endl;
    }
    /// Handle NaN ///
    return P_Val{ nan("") };
};

};

////////// COMPOUND TYPES //////////////////////////////////////////////////////////////////////////
enum C_Type{
    // Compound Data Types
    ENUM,
    RANGE,
    ARRAY,
    RECORD,
    SET,
    // FILE,
    STRUCT,
    PACKED,
    POINTER, // ???
};


class Enum{ public:
    vstr items;

    Enum( const vstr& values ){  items = values;  }
};


class ValRange{ public:
    // Represents a range of numbers that can be iterated

    P_Val valMin;
    P_Val valMax;
    P_Val incr;
    P_Val valCur;
    bool  done;

    ValRange( P_Val valMin_, P_Val valMax_ ){
        // Set up the range for iteration
        valMin = valMin_;
        valMax = valMax_;
        incr   = 1;
        valCur = valMin_;
        done   = false;
    }

    P_Val yield(){
        // Yield the next value and notify if done
        valCur += incr; 
        if( valCur > valMax ){  
            done = true;
            return valMax;  
        }
    }

    ValRange copy(){  return ValRange{ valMin, valMax };  }
};

class StrRange{
    vstr values;
};

class Array{ 
    // AM I ABLE BOTH TO ASSIGN AND TO RETRIEVE?
    private:
    struct elem{
        P_Val val;
        P_Val operator=( const P_Val& val_ ){
            val = val_;
            return val;
        }
    };
    vector<elem> values;

    public:

    Array( size_t N ){
        values.reserve(N);
    }

    P_Val& operator[]( const size_t& index ){
        return values[ index ].val;
    }
};



class Struct{ public: // WARNING, ASSUMPTION: WILL THIS COVER A STRUCT?
    bool /*--------*/ packed;
    map<string,P_Val> pVars; // Variables, Primitive Types
};



class Record{ public: 
    map<string,P_Val> pVars; // Variables, Primitive Types
    map<string,vstr>  condFields; // Conditional Fields
};


template<typename T>
class Set{
    set<T> values;
};

class File{ public:

};

class Pointer{ public:
    // Pointer explanation is VERY UNCLEAR
};



////////// COMPONENTS //////////////////////////////////////////////////////////////////////////////

vstr get_parenthetical_tokens( const vstr& tokens ){
    // Get parentheses contents
    // WARNING, ASSUMPTION: ONLY ONE DEPTH
    vstr contents;
    bool inside = false;
    for( const string& token : tokens ){
        if( token == "(" ){  inside = true;  }
        else if( token == ")" ){  inside = false;  }
        else if( inside ){    }
    }
}


class ValStore{ public:
    // Lookup for values by name
    map<string,string>   pAlias; // Aliases for primitive types
    map<string,P_Val>    var; // -- Variables of primitive types
    map<string,ValRange> valrange; 
    map<string,StrRange> strrange; 
    map<string,Enum>     namedEnum; 
    map<string,Array>    namedArray; 
    map<string,File>     file; 
    map<string,Record>   record; 

    void set_builtins(){
        // Set values that Pascal knows about
        var["maxint"] = P_Val{ LONG_MAX };
    }

    ValStore(){
        // Setup this context
        set_builtins();
    }

    bool p_var_name( const string& name ){
        // Is there a variable stored under this `name`?
        if( var.find( name ) != var.end() ){  return true;  }else{  return false;  }
    }

    P_Val get_var( const string& name ){
        // Get var by `name` if it exists, otherwise return NaN
        if( p_var_name( name ) ){  return var[ name ];  }else{  return make_nan();  }
    }

    P_Val get_var_or_literal( const string& token ){
        // If the `token` is a variable or a literal, then return its value, Otherwise return NaN
        if( p_var_name( token ) ){  return var[ token ];  }
        if( LexPrim::p_primitive_string( token ) ){  return LexPrim::str_2_primitive( token );  }
        return make_nan();
    }
};



////////// TYPES ///////////////////////////////////////////////////////////////////////////////////
void define_types( ValStore& types, string defText ){
    vvstr typStatements = text_block_to_tokenized_statements( defText );
    string name;
    P_Val  bgnRange, endRange;
    vstr   expr;
    cout << "Types:" << endl << typStatements << endl;
    for( const vstr& statement : typStatements ){
        // WARNING, ASSUMPTION: ALL TYPEDEFS CONTAIN AN '='
        if( p_vec_has( statement, string{"="} ) ){
            name = statement[0];
            expr = vec_ltrim( statement, 2 );

            /// Handle Range ///
            if( p_vec_has( expr, string{".."} ) ){
                bgnRange = types.get_var_or_literal( expr[0] );
                endRange = types.get_var_or_literal( expr[2] );
                if( (!p_nan( bgnRange ))&&(!p_nan( endRange )) ){
                    types.valrange[ name ] = ValRange( bgnRange, endRange );
                }

            /// Handle Alias ///
            }else if( (expr.size() == 1) && p_prim_type( expr[0] ) ){
                types.pAlias[ name ] = expr[0];

            /// Handle Enum ///
            }else if( p_vec_has( expr, string{"("} ) && p_vec_has( expr, string{")"} ) ){
                types.namedEnum[ name ] = Enum{ get_parenthetical_tokens( expr ) };
            
            /// Handle Array ///
            }else if( p_vec_has( expr, string{"array"} ) ){
                // FIXME, START HERE: CREATE AN ARRAY!
            }

        }
    }
}



////////// CONSTANTS ///////////////////////////////////////////////////////////////////////////////
void define_constants( ValStore& constants, string defText ){
    vvstr conStatements = text_block_to_tokenized_statements( defText );
    cout << "Constants:" << endl << conStatements << endl;
}



////////// VARIABLES ///////////////////////////////////////////////////////////////////////////////
void define_variables( ValStore& variables, string defText ){
    vvstr varStatements = text_block_to_tokenized_statements( defText );
    cout << "Variables:" << endl << varStatements << endl;
}


////////// INTERPRETER /////////////////////////////////////////////////////////////////////////////

class Interpreter{ public:
    // Runs programs
    ValStore types;
    ValStore constants;
    ValStore vars;
};


////////// MAIN ////////////////////////////////////////////////////////////////////////////////////

string _SRC_PATH = "PAS_Test/00_sections.pas";

int main(){

    vstr /*---*/ fLines = read_file_to_lines( _SRC_PATH );
    TextPortions fSeg = segment_source_file( fLines );
    ValStore     store;

    define_types(     store, fSeg.type );
    define_constants( store, fSeg.cnst );
    define_variables( store, fSeg.var  );


    return 0;
}