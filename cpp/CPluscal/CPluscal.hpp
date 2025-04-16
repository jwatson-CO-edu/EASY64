#ifndef CPLUSCAL_HPP
#define CPLUSCAL_HPP

////////// INIT ////////////////////////////////////////////////////////////////////////////////////

#include <array>
using std::array;
#include <vector>
using std::vector;
#include <deque>
using std::deque;
#include <set>
using std::set;
#include <stack>
using std::stack;
#include <string>
using std::string;
#include <map>
using std::map, std::pair;
#include <variant>
using std::variant, std::get, std::holds_alternative, std::is_same_v;
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
using std::nan, std::isnan, std::pow;

/// Aliases ///
typedef unsigned long  ulong;
typedef unsigned char  ubyte;
typedef vector<string> vstr;
typedef vector<vstr>   vvstr;

/// Defines ///
#define INVALID   SIZE_MAX;
#define NOT_FOUND SIZE_MAX;



////////// TEMPLATED HELPER FUNCTIONS //////////////////////////////////////////////////////////////

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
    return os; // You must return a reference to the stream!operator/
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

const array<string,23> SYMBOLS = { /// Special Symbols ///
    "+", "-", "**", "*", "/", "=", "<", ">", "<>", "<=", ">=", ":=", "..", "<=", 
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

const array<string,4> SECTION_NAMES = { "type", "var", "const", "begin" };


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
    
bool p_primitive_string( const string& q );
P_Val str_2_primitive( const string& q );

vvstr segment_statements( const vstr& tokens_ );
vvstr text_block_to_tokenized_statements( const string& textBlock );
string concat( const vstr& parts, char sepChar = 0 );

vstr get_parenthetical_tokens( const vstr& tokens );
vstr get_bracketed_tokens( const vstr& tokens );

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


vstr read_file_to_lines( string path );
TextPortions segment_source_file( const vstr& totLines );



////////////////////////////////////////////////////////////////////////////////////////////////////
////////// interpreter.cpp /////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

////////// TYPES && SCOPE //////////////////////////////////////////////////////////////////////////
class ValStore{ public:
    // Lookup for values by name
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
    string resolve_prim_alias( const string& name );
    TypeName where_name( const string& name ); // Identify what kind of thing the identifier identifies
};



class Context{ public:
    // Resolves names
    ValStore types;
    ValStore constants;
    ValStore vars;

    P_Val resolve_primitive_name( const string& name );
};

////////// TYPE DEFINITION PART ////////////////////////////////////////////////////////////////////
void define_types( Context& context, const string& defText );

////////// CONSTANT DEFINITION PART ////////////////////////////////////////////////////////////////
void define_constants( Context& context, string defText );

////////// VARIABLE DEFINITION PART ////////////////////////////////////////////////////////////////
void define_variables( Context& context, string defText );



////////// END /////////////////////////////////////////////////////////////////////////////////////
#endif