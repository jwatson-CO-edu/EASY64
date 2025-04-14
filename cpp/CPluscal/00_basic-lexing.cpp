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
using std::map, std::pair;
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

/// Defines ///
#define INVALID   SIZE_MAX;
#define NOT_FOUND SIZE_MAX;


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
vector<T> vec_ltrim( const vector<T>& vec, const size_t& Nrem ){
    // Get the portion of `vec` with the first `Nrem` elements removed
    vector<T> rtnVec;
    if( Nrem < vec.size() ){  for( size_t i = Nrem; i < vec.size(); ++i ){  rtnVec.push_back( vec[i] );  }  }
    return rtnVec;
}


template<typename T>
vector<T> vec_rtrim( const vector<T>& vec, const size_t& Nrem ){
    // Get the portion of `vec` with the last `Nrem` elements removed
    size_t    Nrtn;
    vector<T> rtnVec;
    if( vec.size() > Nrem ){
        Nrtn = vec.size() - Nrem;
        for( size_t i = 0; i < Nrtn; ++i ){  rtnVec.push_back( vec[i] );  }
    }
    return rtnVec;
}


template<typename T>
void vec_extend( vector<T>& vec, const vector<T>& toAdd ){
    // Extend `vec` with elements of `toAdd`
    for( const string& elem : toAdd ){  vec.push_back( elem );  }
}

template<typename T>
size_t vec_find( const vector<T>& vec, T q ){
    // Return index if the value `q` can be found in the vector
    size_t rtnDex = 0;
    for( const T& item : vec ){  ++rtnDex;  if( item == q ){  return rtnDex;  }  }
    return NOT_FOUND;
}


template<typename T>
vector<vector<T>> vec_split( const vector<T>& vec, T sepElem ){
    // Split the `vec` on the `sepElem`, exclusive
    vector<vector<T>> rtnVecVec;
    vector<T> /*---*/ part;
    for( const T& item : vec ){
        if( (item == sepElem) && part.size() ){
            rtnVecVec.push_back( part );
            part.clear();
        }else{  part.push_back( item );  }
    }
    if( part.size() ){  rtnVecVec.push_back( part );  }
    return rtnVecVec;
}


template<typename T>
vector<T> vec_remove( const vector<T>& vec, T remElem ){
    // Split the `vec` on the `sepElem`, exclusive
    vector<T> rtnVec;
    for( const T& item : vec ){  if( item != remElem ){  rtnVec.push_back( item );  }  }
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

const array<string,3> PRIM_TYPE_NAMES = { /// Primitive Type Names ///
    "integer", "real", "Boolean",
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
        // while( accumQue.size() > 1 ){  accumQue.pop_front();  }
        accumQue.push_back( token );
        // cout << '\t' << accumQue;
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
        // cout << " : " << rtnTokens << endl;
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
    // Separate a token vector into a vector of token vectors, each representing a statement (';' included!)
    vector<vstr> statements;
    vstr /*---*/ statement;
    vstr /*---*/ tokens = tokens_;
    tokens.push_back( ";" ); // Terminator Hack
    for( const string& token : tokens ){
        statement.push_back( token );
        if( token == ";" ){
            if( statement.size() > 1 ){
                statements.push_back( statement );
                statement.clear();
            }
        }
    }
    return statements;
}


vvstr text_block_to_tokenized_statements( const string& textBlock ){
    // Render a large block of text as a series of statements, each made of string tokens
    return segment_statements( tokenize( textBlock ) );
}

string concat( const vstr& parts, char sepChar = 0 ){
    // Concat the elements of a string vector
    string rtnStr = "";
    size_t N = parts.size();
    size_t i = 0;
    for( const string& part : parts ){
        rtnStr += part;
        if( (sepChar > 0)&&(i<(N-1)) ){  rtnStr += sepChar;  }
    }
    return rtnStr;
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
            case TYPE:   rtnSctns.type += line;  break;
            case CONST:  rtnSctns.cnst += line;  break;
            case VAR:    rtnSctns.var  += line;  break;
            case OTHER:  rtnSctns.prog += line;  break;            
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

class Enum{ public:
    vstr items;

    Enum(){}
    Enum( const vstr& values ){  items = values;  }

    Enum copy(){  return Enum{ items };  } // Make a copy of the enum
};


class ValRange{ public:
    // Represents a range of numbers that can be iterated

    P_Val valMin;
    P_Val valMax;
    P_Val incr;
    P_Val valCur;
    bool  done;

    ValRange(){
        // Set up the range for iteration
        valMin = make_nan();
        valMax = make_nan();
        incr   = 1;
        valCur = make_nan();
        done   = true;
    }

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



class StrRange{ public:
    // This is basically an Enum and I don't really get it!
    StrRange(){}
    StrRange( const vstr& values_ ){  values = values_;  }
    StrRange copy(){  return StrRange{ values };  }

    vstr values;
};



class Array{ 
    // AM I ABLE BOTH TO ASSIGN AND TO RETRIEVE?
    protected:
    struct elem{
        P_Val val;
        P_Val operator=( const P_Val& val_ ){
            val = val_;
            return val;
        }
    };
    vector<elem> values;

    public:

    Array(){}
    Array( size_t N ){
        values.reserve(N);
    }

    Array copy_empty(){  return Array{ values.size() };  }

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

    Record(){}

    Record copy(){
        Record rtnRec;
        for( const pair<string,P_Val>& item : pVars ){  rtnRec.pVars[ item.first ] = item.second;  }
        for( const pair<string,vstr>& item  : condFields ){  rtnRec.condFields[ item.first ] = item.second;  }
        return rtnRec;
    }
};


class SetString{
    vstr /*--*/ allowed;
    set<string> values;
    pair<set<string>::iterator,bool> res;

    void restrict( const vstr& allowedStrings ){  allowed = allowedStrings;  }

    bool add( const string& val ){
        if( allowed.size() ){
            if( p_vec_has( allowed, val ) ){
                res = values.insert( val );
                return res.second;
            }else{  return false;  }
        }else{  
            res = values.insert( val );  
            return res.second;
        }
    }
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
        else if( inside ){  contents.push_back( token );  }
    }
    return contents;
}


vstr get_bracketed_tokens( const vstr& tokens ){
    // Get square brackets contents
    // WARNING, ASSUMPTION: ONLY ONE DEPTH
    vstr contents;
    bool inside = false;
    for( const string& token : tokens ){
        if( token == "[" ){  inside = true;  }
        else if( token == "]" ){  inside = false;  }
        else if( inside ){  contents.push_back( token );  }
    }
    return contents;
}


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


class ValStore{ public:
    // Lookup for values by name
    map<string,string>   pAlias; // Aliases for primitive types
    map<string,P_Val>    prim; // -- Variables of primitive types
    map<string,ValRange> valrange; 
    map<string,StrRange> strrange; 
    map<string,Enum>     namedEnum; 
    map<string,Array>    namedArray; 
    map<string,P_File>   file; 
    map<string,Record>   record; 

    void set_builtins(){
        // Set values that Pascal knows about
        prim["maxint"] = P_Val{ LONG_MAX };
    }

    ValStore(){
        // Setup this context
        set_builtins();
    }

    bool p_var_name( const string& name ){
        // Is there a variable stored under this `name`?
        if( prim.find( name ) != prim.end() ){  return true;  }else{  return false;  }
    }

    bool p_alias_name( const string& name ){
        // Is there a variable stored under this `name`?
        if( pAlias.find( name ) != pAlias.end() ){  return true;  }else{  return false;  }
    }

    bool p_arr_name( const string& name ){
        // Is there a variable stored under this `name`?
        if( namedArray.find( name ) != namedArray.end() ){  return true;  }else{  return false;  }
    }

    bool p_num_range_name( const string& name ){
        // Is there a variable stored under this `name`?
        if( valrange.find( name ) != valrange.end() ){  return true;  }else{  return false;  }
    }

    bool p_str_range_name( const string& name ){
        // Is there a variable stored under this `name`?
        if( strrange.find( name ) != strrange.end() ){  return true;  }else{  return false;  }
    }

    bool p_file_name( const string& name ){
        // Is there a variable stored under this `name`?
        if( file.find( name ) != file.end() ){  return true;  }else{  return false;  }
    }

    bool p_record_name( const string& name ){
        // Is there a variable stored under this `name`?
        if( record.find( name ) != record.end() ){  return true;  }else{  return false;  }
    }

    P_Val get_var( const string& name ){
        // Get var by `name` if it exists, otherwise return NaN
        if( p_var_name( name ) ){  return prim[ name ];  }else{  return make_nan();  }
    }

    P_Val get_var_or_literal( const string& token ){
        // If the `token` is a variable or a literal, then return its value, Otherwise return NaN
        if( p_var_name( token ) ){  return prim[ token ];  }
        if( LexPrim::p_primitive_string( token ) ){  return LexPrim::str_2_primitive( token );  }
        return make_nan();
    }

    string resolve_prim_alias( const string& name ){
        // Return the primitive name associated with the alias, otherwise return an empty string
        if( p_prim_type( name ) ){  return name;  }
        if( pAlias.find( name ) != pAlias.end() ){  return pAlias[ name ];  }
        return "";
    }

    TypeName where_name( const string& name ){
        // Identify what kind of thing the identifier identifies
        if( p_var_name( name )       ){  return PRIMITIVE;     }
        if( p_alias_name( name )     ){  return ALIAS;         }
        if( p_num_range_name( name ) ){  return RANGE_VALUE;   }
        if( p_str_range_name( name ) ){  return RANGE_STRING;  }
        if( p_arr_name( name )       ){  return ARRAY;         }
        if( p_file_name( name )      ){  return P_FILE;        }
        if( p_record_name( name )    ){  return RECORD;        }
        return ERROR;
    }
};



class Context{ public:
    // Resolves names
    ValStore types;
    ValStore constants;
    ValStore vars;
};



////////// TYPES ///////////////////////////////////////////////////////////////////////////////////
void define_types( Context& context, const string& defText ){
    // Specify aliases and user-defined types
    vvstr  typStatements = text_block_to_tokenized_statements( defText );
    string name, pName;
    P_Val  bgnRange, endRange;
    vstr   expr;
    size_t span;
    bool   accum = false;
    vvstr  mltExp;
    Record rec;
    cout << "Types:" << endl << typStatements << endl;
    for( const vstr& statement : typStatements ){

        cout << "Processing: " << statement << endl;

        if( accum ){
            vec_extend( expr, statement );
            if( p_vec_has( statement, string{"end"} ) ){  accum = false;  }

            if( !accum ){
                /// Handle Record End ///
                if( p_vec_has( expr, string{"record"} ) ){
                    expr   = vec_ltrim( expr, 1 ); // Remove "record"
                    expr   = vec_rtrim( expr, 2 ); // Remove ["end", ";",]
                    mltExp = segment_statements( expr );
                    cout << mltExp << endl;
                    for( const vstr& sttmnt_j : mltExp ){
                        if( context.types.resolve_prim_alias( sttmnt_j[2] ).size() ){
                            rec = Record{};
                            rec.pVars[ sttmnt_j[0] ] = make_nan();
                            context.types.record[ name ] = rec;
                        }
                    }
                }
            }

        }else if( p_vec_has( statement, string{"="} ) ){ // WARNING, ASSUMPTION: ALL TYPEDEFS CONTAIN AN '='
            name = statement[0];
            expr = vec_ltrim( statement, 2 );

            /// Handle Range ///
            if( p_vec_has( expr, string{".."} ) ){
                bgnRange = context.types.get_var_or_literal( expr[0] );
                endRange = context.types.get_var_or_literal( expr[2] );
                if( (!p_nan( bgnRange ))&&(!p_nan( endRange )) ){
                    context.types.valrange[ name ] = ValRange( bgnRange, endRange );
                }

            /// Handle Alias ///
            }else if( (expr.size() == 2) && p_prim_type( expr[0] ) ){
                context.types.pAlias[ name ] = expr[0];

            /// Handle Enum ///
            }else if( p_vec_has( expr, string{"("} ) && p_vec_has( expr, string{")"} ) ){
                context.types.namedEnum[ name ] = Enum{ get_parenthetical_tokens( expr ) };
            
            /// Handle Array ///
            }else if( p_vec_has( expr, string{"array"} ) ){
                expr = get_bracketed_tokens( expr );
                if( (expr.size() == 1)&&(context.types.p_num_range_name( expr[0] )) ){
                    span = get<long>( context.types.valrange[ expr[0] ].valMax )
                           -
                           get<long>( context.types.valrange[ expr[0] ].valMin );
                    context.types.namedArray[ name ] = Array{ span };
                }else if( expr.size() == 3 ){
                    bgnRange = context.types.get_var_or_literal( expr[0] );
                    endRange = context.types.get_var_or_literal( expr[2] );
                    span     = get<long>( endRange ) - get<long>( bgnRange );
                    context.types.namedArray[ name ] = Array{ span };    
                }else{
                    cout << "Malformed array!: " << statement << endl;
                }

            /// Handle File ///
            }else if( p_vec_has( expr, string{"file"} ) ){
                context.types.file[ name ] = P_File{};
            
            /// Handle Record Begin ///
            }else if( p_vec_has( expr, string{"record"} ) ){
                accum = true;

            /// Handle OTHER? ///
            }else{
                cout << "`define_types`, WARNING: COULD NOT PARSE THE FOLLOWING LINE:\n" << statement << endl;
            }

        }
    }
}



////////// CONSTANTS ///////////////////////////////////////////////////////////////////////////////
void define_constants( Context& context, string defText ){
    // Specify values that should not change
    vvstr  conStatements = text_block_to_tokenized_statements( defText );
    string name, pName;
    P_Val  bgnRange, endRange;
    vstr   expr;
    cout << "Constants:" << endl << conStatements << endl;
    for( const vstr& statement : conStatements ){

        cout << "Processing: " << statement << endl;

        if( p_vec_has( statement, string{"="} ) ){ // WARNING, ASSUMPTION: ALL CONSTANT DEFS CONTAIN AN '='
            name = statement[0];
            expr = vec_ltrim( statement, 2 );

            /// Handle Primtive ///
            if( (expr.size() == 2) && p_prim_type( expr[0] ) ){
                context.types.pAlias[ name ] = expr[0];
            }else{
                cout << "`define_constants`, WARNING: COULD NOT PARSE THE FOLLOWING LINE:\n" << statement << endl;
            }
        }
    }
}



////////// VARIABLES ///////////////////////////////////////////////////////////////////////////////
void define_variables( Context& context, string defText ){
    vvstr  varStatements = text_block_to_tokenized_statements( defText );
    vvstr  parts;
    vstr   names, tExpr;
    string type, errStr;
    P_Val  bgnRange, endRange;
    size_t span;

    cout << "Variables:" << endl << varStatements << endl;
    for( const vstr& statement : varStatements ){

        cout << "Processing: " << statement << endl;

        if( p_vec_has( statement, string{":"} ) ){ // WARNING, ASSUMPTION: ALL VARIABLE DEFS CONTAIN AN ':'
            parts = vec_split( statement, string{":"} );
            names = vec_remove( parts[0], string{","} );
            tExpr  = vec_remove( parts[1], string{";"} );
            cout << names << " : " << tExpr << endl;

            /// Handle Named Types ///
            if( tExpr.size() == 1 ){

                switch( context.types.where_name( tExpr[0] ) ){
                    case PRIMITIVE:
                    case ALIAS:
                        for( const string& name : names ){
                            context.vars.prim[ name ] = make_nan();
                        }
                        break;
                    case RANGE_VALUE:
                        for( const string& name : names ){
                            context.vars.valrange[ name ] = context.types.valrange[ tExpr[0] ].copy();
                        }
                        break;
                    case RANGE_STRING:
                        for( const string& name : names ){
                            context.vars.strrange[ name ] = context.types.strrange[ tExpr[0] ].copy();
                        }
                        break;
                    case ENUM:
                        for( const string& name : names ){
                            context.vars.namedEnum[ name ] = context.types.namedEnum[ tExpr[0] ].copy();
                        }
                        break;
                    case ARRAY:
                        for( const string& name : names ){
                            context.vars.namedArray[ name ] = context.types.namedArray[ tExpr[0] ].copy_empty();
                        }
                        break;
                    case P_FILE:
                        for( const string& name : names ){
                            context.vars.file[ name ] = P_File{};
                        }
                        break;
                    case RECORD:
                        for( const string& name : names ){
                            context.vars.record[ name ] = context.types.record[ tExpr[0] ].copy();
                        }
                        break;
                    case ERROR:
                        cerr << tExpr[0] << " does not name a type!" << endl;
                        break;
                    default:
                        cerr << "THIS SHOULD NOT HAPPEN!\nThere was an ERROR in interpreting " << statement << "!" << endl;
                        errStr = "`define_variables`: Could not process the following!:\t" + concat( statement, ',' ) + "\n\n";
                        throw runtime_error( errStr );

                }

                type = context.types.resolve_prim_alias( tExpr[0] );
                if( type.size() ){  for( const string& name : names ){    }  }
            

            /// Handle Numeric Range Variables ///
            }else if( p_vec_has( tExpr, string{".."} ) ){
                bgnRange = context.types.get_var_or_literal( tExpr[0] );
                endRange = context.types.get_var_or_literal( tExpr[2] );
                if( (!p_nan( bgnRange ))&&(!p_nan( endRange )) ){
                    for( const string& name : names ){
                        context.vars.valrange[ name ] = ValRange( bgnRange, endRange );
                    }
                }

            /// Handle Enum ///
            }if( p_vec_has( tExpr, string{"("} ) && p_vec_has( tExpr, string{")"} ) ){
                tExpr = get_parenthetical_tokens( tExpr );
                for( const string& name : names ){
                    context.vars.namedEnum[ name ] =  Enum{ tExpr };
                }
            
            /// Handle Array ///
            }else if( p_vec_has( tExpr, string{"array"} ) ){
                tExpr = get_bracketed_tokens( tExpr );
                if( (tExpr.size() == 1)&&(context.types.p_num_range_name( tExpr[0] )) ){
                    span = get<long>( context.types.valrange[ tExpr[0] ].valMax )
                           -
                           get<long>( context.types.valrange[ tExpr[0] ].valMin );
                    for( const string& name : names ){
                        context.types.namedArray[ name ] = Array{ span };
                    }
                }else if( tExpr.size() == 3 ){
                    bgnRange = context.types.get_var_or_literal( tExpr[0] );
                    endRange = context.types.get_var_or_literal( tExpr[2] );
                    span     = get<long>( endRange ) - get<long>( bgnRange );
                    for( const string& name : names ){
                        context.types.namedArray[ name ] = Array{ span };    
                    }
                }else{
                    cout << "Malformed array!: " << statement << endl;
                }

            /// Handle File ///
            }else if( p_vec_has( tExpr, string{"file"} ) ){
                for( const string& name : names ){
                    context.types.file[ name ] = P_File{};
                }
            
            /// Handle Set ///
            } // FIXME, START HERE: HANDLE A SET OF TYPE ENUM!
        }
    }
}


////////// INTERPRETER /////////////////////////////////////////////////////////////////////////////

// FIXME: RUN INTERPRETER MACHINERY IN ORDER!


////////// MAIN ////////////////////////////////////////////////////////////////////////////////////

string _SRC_PATH = "PAS_Test/00_sections.pas";

int main(){

    vstr /*---*/ fLines = read_file_to_lines( _SRC_PATH );
    TextPortions fSeg = segment_source_file( fLines );
    Context /**/ cntxt;

    define_types(     cntxt, fSeg.type );  cout << endl;
    define_constants( cntxt, fSeg.cnst );  cout << endl;
    define_variables( cntxt, fSeg.var  );  cout << endl;


    return 0;
}