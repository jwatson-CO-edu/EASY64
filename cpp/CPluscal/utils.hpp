#ifndef UTILS_HPP
#define UTILS_HPP

////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include <vector>
using std::vector;
#include <iostream>
using std::cout, std::endl, std::flush, std::cerr, std::ostream;
#include <deque>
using std::deque;
#include <random>
#include <sstream>
using std::stringstream, std::getline;
#include <string>
using std::string, std::to_string;
#include <map>
using std::map, std::pair;


/// Aliases ///
typedef unsigned long  ulong;
typedef long long /**/ llong;
typedef unsigned char  ubyte;
typedef vector<string> vstr;
typedef vector<vstr>   vvstr; 



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
vector<T> vec_sub( const vector<T>& vec, const size_t& bgn, const size_t& endPlus1 ){
    // Get the portion of `vec` with the last `Nrem` elements removed
    vector<T> rtnVec;
    size_t end = min( endPlus1, vec.size() );
    if( (bgn < vec.size()) ){  for( size_t i = bgn; i < end; ++i ){  rtnVec.push_back( vec[i] );  }  }
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
    // Return a version of `vec` with all instances of `remElem` removed
    vector<T> rtnVec;
    for( const T& item : vec ){  if( item != remElem ){  rtnVec.push_back( item );  }  }
    return rtnVec;
}



////////// HELPER UTILS ////////////////////////////////////////////////////////////////////////////

namespace uuid{
    // Author: happy_sisyphus, https://stackoverflow.com/a/60198074
    static std::random_device              rd;
    static std::mt19937                    gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    std::string generate_uuid_v4() {
        std::stringstream ss;
        int i;
        ss << std::hex;
        for (i = 0; i < 8; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 4; i++) {
            ss << dis(gen);
        }
        ss << "-4";
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        ss << dis2(gen);
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 12; i++) {
            ss << dis(gen);
        };
        return ss.str();
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////
////////// json.cpp ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

class JSON{ public:
    // Generic, nested data structure
    bool /*-------*/ leaf;
    string /*-----*/ value;
    map<string,JSON> dict;
    size_t /*-----*/ indent;

    /// Constructors ///
    JSON(){  indent = 2;  }
    JSON( bool   val ) : JSON() {  set( val );  }
    JSON( double val ) : JSON() {  set( val );  }
    JSON( llong  val ) : JSON() {  set( val );  }
    JSON( size_t val ) : JSON() {  set( val );  }
    JSON( string val ) : JSON() {  set( val );  }

    /// Setters ///
    void set( bool   val );
    void set( double val );
    void set( llong  val );
    void set( size_t val );
    void set( string val );
    
    void set_indent( size_t ndnt );

    void unset();

    void add_child( const string& key, const JSON& val ); // Nest some data

    string as_string( size_t depth = 0 ) const;

    ostream& operator<<( ostream& os );
};


#endif