////////// INIT ////////////////////////////////////////////////////////////////////////////////////

#include <array>
using std::array;
#include <vector>
using std::vector;
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
using std::cout, std::endl, std::flush, std::cerr;
#include <stdexcept>
using std::runtime_error;
#include <sstream>
using std::stringstream, std::getline;

/// Aliases ///
typedef unsigned long  ulong;
typedef vector<string> vstr;


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


bool p_section_header( const string& q ){
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


vstr tokenize( string expStr, char sepChr ){
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
    expStr.push_back( sepChr );

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
        }else if( c == sepChr ){
            if(token.size() > 0){  stow_token();  }
        // C. Case any other char
        }else{  cache_char();  } 
    }
    // N. Return the vector of tokens
    return tokens;
}


vector<vstr> segment_statements( vstr& tokens ){
    // Separate a token vector into a vector of token vectors, each representing a statement
    vector<vstr> statements;
    vstr /*---*/ statement;
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



////////// FILE LEXING /////////////////////////////////////////////////////////////////////////////
struct TextPortions{ public:
    // Separates text of a file into portions so that we can treat each differently
    string type;
    string cnst;
    string var;
    string func;
    string proc;
    string prog;
};


TextPortions segment_source_file( string path ){
    // Load sections of the program into the struct so that we can lex/interpret them
    // FIXME: READ FILE IN A SEPARATE FUNCTION
    enum Section{ TYPE, CONST, VAR, COMMENT, OTHER };

    ifstream     inputFile;
    stringstream buffer;
    vstr /*---*/ totLines;
    string /*-*/ line, trimLine;
    Section /**/ mode   = OTHER;
    string /*-*/ errStr = "Could not open ";
    
    inputFile.open( path );
    if( !inputFile.is_open() ){
        cerr << "Error opening " << path << "!" << endl;
        errStr += path;
        inputFile.close();
        throw runtime_error( errStr ); // Exit with an error code
    }else{
        buffer << inputFile.rdbuf(); // https://stackoverflow.com/a/2602258
        while( getline( buffer, line, '\n' ) ){ // https://stackoverflow.com/a/13172579
            totLines.push_back( line );
        }
        inputFile.close();
    }
    for( const string& line : totLines ){
        trimLine = strip( line );
        if( mode == COMMENT ){} // FIXME: CHECK FOR COMMENT END
        if( p_section_header( trimLine ) ){
            if( trimLine == "type" ){
                mode = TYPE;
            }else if( trimLine == "const" ){
                mode = CONST;
            }else if( trimLine == "var" ){
                mode = VAR;
            }
        }
    }
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
    FILE,
    STRUCT,
    PACKED,
    POINTER, // ???
};

typedef vector<string> Enum;

class ValRange{
    vector<P_Val> values;
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

}


////////// CONSTANTS ///////////////////////////////////////////////////////////////////////////////
void define_constants( ValStore& constants, string defText ){

}


////////// VARIABLES ///////////////////////////////////////////////////////////////////////////////
void define_variables( ValStore& variables, string defText ){

}


////////// INTERPRETER /////////////////////////////////////////////////////////////////////////////

class Interpreter{ public:
    // Runs programs
    ValStore types;
    ValStore constants;
    ValStore vars;
};


// ////////// VARIABLE LEXING /////////////////////////////////////////////////////////////////////////
// class BlockFrame;
// typedef shared_ptr<BlockFrame> BlkPtr;

// class BlockFrame{ public: // WARNING: ASSUMPTION
//     BlkPtr /*------*/ parent; // The frame that contains this one
//     map<string,P_Val> pVars; // Variables, Primitive Types
// };