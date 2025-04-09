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
using std::variant, std::get;
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

/// Aliases ///
typedef unsigned long  ulong;
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
ostream& operator<<( ostream& os , const vector<vector<T>>& vec ) { 
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
    return tokens;
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
typedef variant<long,double,bool,char> P_Val; // Primitive Values // WARNING: ASSUMPTION


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

typedef vector<string> Enum;

class ValRange{
    P_Val valMin;
    P_Val valMax;
    P_Val incr;
    P_Val valCur;

    ValRange( P_Val valMin_, P_Val valMax_ ){
        valMin = valMin_;
        valMax = valMax_;
        incr   = 1;
        valCur = valMin_;
    }

    P_Val yield(){
        valCur += incr; // FIXME, START HERE: DEFINE ADDITION AND PLUS-EQUALS FOR THE NUMERIC VARIANT
    }
};

class StrRange{
    vstr values;
};

typedef vector<P_Val> Array;

class Struct{ public: // WARNING: ASSUMPTION
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
};



////////// TYPES ///////////////////////////////////////////////////////////////////////////////////
void define_types( ValStore& types, string defText ){
    vvstr typStatements = text_block_to_tokenized_statements( defText );
    cout << "Types:" << endl << typStatements << endl;
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