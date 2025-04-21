////////// INIT ////////////////////////////////////////////////////////////////////////////////////

/// Imports ///
#include <stdlib.h> // exit, EXIT_FAILURE 
using std::exit;
#include <iostream>
using std::cout, std::endl, std::flush;
using std::ostream;
#include <vector>
using std::vector;
#include <string>
using std::string, std::stod, std::stol, std::stoul;
#include <memory>
using std::shared_ptr;
#include <stack>
using std::stack;
#include <cmath>
#include <variant>
using std::variant, std::get;

/// Aliases ///
typedef unsigned long /*--------*/ ulong;
typedef vector<string> /*-------*/ vstr;
typedef variant<double,long,ulong> N64;

/// Forward Declarations ///
struct MathNode;  typedef shared_ptr<MathNode> mathPtr;

////////// NUMERIC ATOMS ///////////////////////////////////////////////////////////////////////////

enum E_Type_N{
    // Primitive Data Types
    FLOAT, // `double`
    INTGR, // `long`
    U_INT, // `unsigned long`
    NaNum, // Not a Number
    ERROR, // Error
};


enum E_Type_E{
    UNDEF_TYPE, // This type is NOT recognized
};


struct Data64{
    // A container for data
    E_Type_N type;
    N64 /**/ data;

    void set_float( double dbbl ){  
        data = dbbl; 
        type = FLOAT;
    }

    void set_int( long lng ){ 
        data = lng;  
        type = INTGR;
    }

    void set_uint( ulong ulng ){     
        data = ulng; 
        type = U_INT;
    }

    void set_NaN(){
        data = nan("");
        type = NaNum;
    }

    void set_error( ulong code ){
        data = code;
        type = ERROR;
    }

    double get_float(){  return get<double>( data );  }
    double get_int(){    return get<long>( data );    }
    double get_uint(){   return get<ulong>( data );   }
    double get_NaN(){    return nan("");              }
    double get_code(){   return get<ulong>( data );   }


    bool p_NaN(){  return (type == NaNum);  }
};
typedef shared_ptr<Data64> dataPtr;



////////// MATH OPERATIONS /////////////////////////////////////////////////////////////////////////

// FIXME: THERE IS A LOT OF REPEAT CODE HERE: std::variant: A Type-Safe Union, https://dev.to/pauljlucas/unions-stdvariant-in-c-2op1
/*
std::variant<int, std::string> v = 42;
std::cout << "Index: " << v.index() << std::endl; // Output: 0 (int is the first type)
*/


Data64 operator+( const Data64& left, const Data64& rght ){
    // Handle type escalation and return result of addition
    Data64 rtnObj;
    switch( left.type ){
        case FLOAT: 
            switch( rght.type ){
                case FLOAT:  rtnObj.set_float( get<double>(left.data) + get<double>(rght.data)); /**/ break;
                case INTGR:  rtnObj.set_float( get<double>(left.data) + get<long>(rght.data));  break;
                case U_INT:  rtnObj.set_float( get<double>(left.data) + get<ulong>(rght.data) );  break;
                default:     rtnObj.set_error( UNDEF_TYPE );
            }
            break;
        case INTGR: 
            switch( rght.type ){
                case FLOAT:  rtnObj.set_float( get<long>(left.data) + get<double>(rght.data) );   break;
                case INTGR:  rtnObj.set_int(   get<long>(left.data) + get<long>(rght.data) ); /*---*/ break;
                case U_INT:  rtnObj.set_int(   get<long>(left.data) + get<ulong>(rght.data) );  break;
                default:     rtnObj.set_error( UNDEF_TYPE );
            }
            break;
        case U_INT: 
            switch( rght.type ){
                case FLOAT:  rtnObj.set_float( get<ulong>(left.data) + get<double>(rght.data) );   break;
                case INTGR:  rtnObj.set_int(   get<ulong>(left.data) + get<long>(rght.data) );  break;
                case U_INT:  rtnObj.set_uint(  get<ulong>(left.data) + get<ulong>(rght.data) ); /*--*/ break;
                default:     rtnObj.set_error( UNDEF_TYPE );
            }
            break;
        default:  rtnObj.set_error( UNDEF_TYPE );
    }
    return rtnObj;
}

Data64 operator-( const Data64& left, const Data64& rght ){
    // Handle type escalation and return result of subtraction
    Data64 rtnObj;
    switch( left.type ){
        case FLOAT: 
            switch( rght.type ){
                case FLOAT:  rtnObj.set_float( get<double>(left.data) - get<double>(rght.data)); /**/ break;
                case INTGR:  rtnObj.set_float( get<double>(left.data) - get<long>(rght.data));  break;
                case U_INT:  rtnObj.set_float( get<double>(left.data) - get<ulong>(rght.data) );  break;
                default:     rtnObj.set_error( UNDEF_TYPE );
            }
            break;
        case INTGR: 
            switch( rght.type ){
                case FLOAT:  rtnObj.set_float( get<long>(left.data) - get<double>(rght.data) );   break;
                case INTGR:  rtnObj.set_int(   get<long>(left.data) - get<long>(rght.data) ); /*---*/ break;
                case U_INT:  rtnObj.set_int(   get<long>(left.data) - get<ulong>(rght.data) );  break;
                default:     rtnObj.set_error( UNDEF_TYPE );
            }
            break;
        case U_INT: 
            switch( rght.type ){
                case FLOAT:  rtnObj.set_float( get<ulong>(left.data) - get<double>(rght.data) );   break;
                case INTGR:  rtnObj.set_int(   get<ulong>(left.data) - get<long>(rght.data) );  break;
                case U_INT:  rtnObj.set_uint(  get<ulong>(left.data) - get<ulong>(rght.data) ); /*--*/ break;
                default:     rtnObj.set_error( UNDEF_TYPE );
            }
            break;
        default:  rtnObj.set_error( UNDEF_TYPE );
    }
    return rtnObj;
}

Data64 operator*( const Data64& left, const Data64& rght ){
    // Handle type escalation and return result of multiplication
    Data64 rtnObj;
    switch( left.type ){
        case FLOAT: 
            switch( rght.type ){
                case FLOAT:  rtnObj.set_float( get<double>(left.data) * get<double>(rght.data)); /**/ break;
                case INTGR:  rtnObj.set_float( get<double>(left.data) * get<long>(rght.data));  break;
                case U_INT:  rtnObj.set_float( get<double>(left.data) * get<ulong>(rght.data) );  break;
                default:     rtnObj.set_error( UNDEF_TYPE );
            }
            break;
        case INTGR: 
            switch( rght.type ){
                case FLOAT:  rtnObj.set_float( get<long>(left.data) * get<double>(rght.data) );   break;
                case INTGR:  rtnObj.set_int(   get<long>(left.data) * get<long>(rght.data) ); /*---*/ break;
                case U_INT:  rtnObj.set_int(   get<long>(left.data) * get<ulong>(rght.data) );  break;
                default:     rtnObj.set_error( UNDEF_TYPE );
            }
            break;
        case U_INT: 
            switch( rght.type ){
                case FLOAT:  rtnObj.set_float( get<ulong>(left.data) * get<double>(rght.data) );   break;
                case INTGR:  rtnObj.set_int(   get<ulong>(left.data) * get<long>(rght.data) );  break;
                case U_INT:  rtnObj.set_uint(  get<ulong>(left.data) * get<ulong>(rght.data) ); /*--*/ break;
                default:     rtnObj.set_error( UNDEF_TYPE );
            }
            break;
        default:  rtnObj.set_error( UNDEF_TYPE );
    }
    return rtnObj;
}

Data64 operator/( const Data64& left, const Data64& rght ){
    // Handle type escalation and return result of division
    Data64 rtnObj;
    switch( rght.type ){
        case FLOAT: 
            if( get<double>(left.data) == 0.0 ){  rtnObj.set_NaN();  break;  }
            switch( left.type ){
                case FLOAT:  rtnObj.set_float( get<double>(left.data) / get<double>(rght.data));  break;
                case INTGR:  rtnObj.set_float( get<long>(left.data) / get<double>(rght.data));    break;
                case U_INT:  rtnObj.set_float( get<ulong>(left.data) / get<double>(rght.data) );  break;
                default:     rtnObj.set_error( UNDEF_TYPE );
            }
            break;
        case INTGR: 
            if( get<long>(left.data) == 0 ){  rtnObj.set_NaN();  break;  }
            switch( left.type ){
                case FLOAT:  rtnObj.set_float( get<double>(left.data) / get<long>(rght.data) );  break;
                case INTGR:  rtnObj.set_int(   get<long>(left.data) / get<long>(rght.data) );    break;
                case U_INT:  rtnObj.set_int(   get<ulong>(left.data) / get<long>(rght.data) );   break;
                default:     rtnObj.set_error( UNDEF_TYPE );
            }
            break;
        case U_INT: 
            if( get<ulong>(left.data) == 0 ){  rtnObj.set_NaN();  break;  }
            switch( left.type ){
                case FLOAT:  rtnObj.set_float( get<double>(left.data) / get<ulong>(rght.data) );  break;
                case INTGR:  rtnObj.set_int(   get<long>(left.data) / get<ulong>(rght.data) );    break;
                case U_INT:  rtnObj.set_uint(  get<ulong>(left.data) / get<ulong>(rght.data) );   break;
                default:     rtnObj.set_error( UNDEF_TYPE );
            }
            break;
        default:  rtnObj.set_error( UNDEF_TYPE );
    }
    return rtnObj;
}



////////// PRINT FUNCTIONS /////////////////////////////////////////////////////////////////////////


ostream& operator<<( ostream& os , const Data64& dat ){ 
    // Print data
    switch( dat.type ){
        case FLOAT:  os << get<double>(dat.data);  break;
        case INTGR:  os << get<long>(dat.data);    break;
        case U_INT:  os << get<ulong>(dat.data);   break;
        default:     cout << "CANNOT PRINT: BAD TYPE!" << endl;
    }
    return os; // You must return a reference to the stream!
}

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



////////// MATH LEXER //////////////////////////////////////////////////////////////////////////////

bool p_reserved_char( char c ){
    // Is the character a reserved character? 
    return ( c == '(' ) ||
           ( c == ')' ) ||
           ( c == '+' ) ||
           ( c == '-' ) ||
           ( c == '*' ) ||
           ( c == '/' ) ||
           ( c == '=' ) ||
           ( c == ';' );
}

bool p_reserved_char( string c ){
    // Is the one-character string a reserved character? 
    char ch = c[0];
    if( c.size() > 1 )  return false;
    return p_reserved_char( ch );
}

vstr tokenize( string expStr, char sepChar = ' ' ){
    // Separate an expression string into tokens
    vstr tokens;
    char   c = ' ';
    string cStr;
    string token  = "";

    // Helpers //
    auto stow_token = [&](){  tokens.push_back( token );  token = "";  };
    auto stow_char  = [&](){  tokens.push_back( string(1,c) );  };
    auto cache_char = [&](){  token += c;  };

    // 0. Apply the postfix hack
    expStr += sepChar;
    // 1. For every character in the string
    for( char ch_i : expStr ){
        // 2. Fetch character
        c = ch_i;
        if( p_reserved_char( c ) ){ 
            if(token.size() > 0){  stow_token();  }
            stow_char();
        }else if( c == sepChar){
            if(token.size() > 0){  stow_token();  }
        // D. Case any other char
        }else{  cache_char();  } 
    }
    // N. Return the vector of tokens
    return tokens;
}

enum E_Math_Op{
    // Primitive Data Types
    ADD, // Add
    SUB, // Subtract
    MLT, // Multiply
    DIV, // Divide
    ERR, // ERROR
};

struct MathNode{
    // Nestable math operation
    E_Math_Op type;
    mathPtr   parent;
    mathPtr   opLeft;
    mathPtr   opRght;
    dataPtr   nmLeft;
    dataPtr   nmRght;

    MathNode(){
        // Default math operation node
        type   = ERR;
        parent = nullptr;
        opLeft = nullptr;
        opRght = nullptr;
        nmLeft = nullptr;
        nmRght = nullptr;
    }

    Data64 evaluate(){
        // Evaluate the math expression at this node
    }
};
typedef shared_ptr<MathNode> mathPtr;

dataPtr number_from_string( string token ){
    // Attempt to conver the string to a `Data64` number
    dataPtr datum = dataPtr( new Data64{} );
    datum->set_NaN(); // If all tests fail, fall thru to NaN
    if( token.size() > 0 ){
        // If '.' or 'e' is in the token, attempt float conversion
        if(token.find( 'e' ) || token.find( '.' )){
            try{  datum->set_float( stod( token ) );  }catch(...){  cout << "Float conversion FAILED!: " << token << endl;  }
        }else if(token.find( '-' )){
            try{  datum->set_int( stol( token ) );  }catch(...){  cout << "Int conversion FAILED!: " << token << endl;  }
        }else{
            try{  datum->set_uint( stoul( token ) );  }catch(...){  cout << "Uint conversion FAILED!: " << token << endl;  }
        }
    }
    return datum;
}

struct Calculator{
    // Tree-based calculator
    mathPtr /*--*/ root = nullptr;
    mathPtr /*--*/ curr = nullptr;
    mathPtr /*--*/ mNod = nullptr;
    dataPtr /*--*/ dNod = nullptr;
    stack<dataPtr> dStk;
    stack<mathPtr> mStk;

    void parse( string strExpr ){
        // Turn a vector of string `tokens` into a tree of `MathNode`
        vstr tokens = tokenize( strExpr );
        for( string token : tokens ){

            // Conversion to node

            if( p_reserved_char( token ) ){
                mNod = shared_ptr<MathNode>( new MathNode{} );
                /**/ if( token == "+" ){  mNod->type = ADD;  } 
                else if( token == "-" ){  mNod->type = SUB;  }  
                else if( token == "*" ){  mNod->type = MLT;  } 
                else if( token == "/" ){  mNod->type = DIV;  }  
                else{  
                    cout << "UNKNOWN math operator!: " << token << endl;
                    break;
                }
                mStk.push( mNod );
            }else{
                dNod = number_from_string( token );
                if( dNod->p_NaN() ){
                    cout << "Numeric conversion FAILED!: " << token << endl;
                    break;
                }
                dStk.push( dNod );
            }

            // Handle token
            // FIXME, START HERE: HANDLE TOKEN
        }
    }

    Data64 evaluate(){
        // Evaluate the math expression from the `root`
    }
};

////////// TESTING /////////////////////////////////////////////////////////////////////////////////


int main(){
    string expr1 = "C=A+B;";
    vstr   tkns1 = tokenize( expr1 );
    string expr2 = "AA BB CC;";
    vstr   tkns2 = tokenize( expr2 );
    cout << tkns1 << endl;
    cout << tkns2 << endl;
}