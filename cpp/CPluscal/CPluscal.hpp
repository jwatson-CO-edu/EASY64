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



////////////////////////////////////////////////////////////////////////////////////////////////////
////////// primitives.cpp //////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


////////// PRIMITIVE TYPES /////////////////////////////////////////////////////////////////////////
enum P_Type{
    // Primitive Data Types
    INT, // `long`
    REAL, // `double`
    BOOL, 
    CHAR, 
};


typedef variant<double,long,char,bool> P_Val; // Primitive Values // WARNING: ASSUMPTION



////////// COMPOUND TYPES //////////////////////////////////////////////////////////////////////////

class Enum{ public:
    vstr items;

    Enum();
    Enum( const vstr& );

    Enum copy(); // Make a copy of the enum
};



////////// END /////////////////////////////////////////////////////////////////////////////////////
#endif