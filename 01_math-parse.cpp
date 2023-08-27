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
using std::string;
#include <memory>
using std::shared_ptr;
#include <stack>
using std::stack;

/// Aliases ///
typedef unsigned long  ulong;
typedef vector<string> vstr;

////////// NUMERIC ATOMS ///////////////////////////////////////////////////////////////////////////

enum E_Type_N{
    // Primitive Data Types
    FLOAT, // `double`
    INTGR, // `long`
    U_INT, // `unsigned long`
};

union N64{
    // Numeric union
    double f;
    long   i;
    ulong  u;
};

struct Data64{
    // A container for data
    E_Type_N type;
    N64 /**/ data;

    void set_float( double dbbl ){  
        data.f = dbbl; 
        type   = FLOAT;
    }

    void set_int( long lng ){ 
        data.i = lng;  
        type   = INTGR;
    }

    void set_uint( ulong ulng ){     
        data.u = ulng; 
        type   = U_INT;
    }
};



////////// MATH OPERATIONS /////////////////////////////////////////////////////////////////////////


Data64 operator+( const Data64& left, const Data64& rght ){
    // Handle type escalation and return result of addition
    Data64 rtnObj;
    switch( left.type ){
        case FLOAT: 
            switch( rght.type ){
                case FLOAT:  rtnObj.set_float( left.data.f + rght.data.f ); /**/ break;
                case INTGR:  rtnObj.set_float( left.data.f + rght.data.i*1.0 );  break;
                case U_INT:  rtnObj.set_float( left.data.f + rght.data.u*1.0 );  break;
                default:     cout << "ERROR: BAD TYPE!" << endl;  exit( 1 );
            }
            break;
        case INTGR: 
            switch( rght.type ){
                case FLOAT:  rtnObj.set_float( left.data.i*1.0 + rght.data.f );   break;
                case INTGR:  rtnObj.set_int( left.data.i + rght.data.i ); /*---*/ break;
                case U_INT:  rtnObj.set_int( left.data.i + (long) rght.data.u );  break;
                default:     cout << "ERROR: BAD TYPE!" << endl;  exit( 1 );
            }
            break;
        case U_INT: 
            switch( rght.type ){
                case FLOAT:  rtnObj.set_float( left.data.u*1.0 + rght.data.f );   break;
                case INTGR:  rtnObj.set_int( (long) left.data.u + rght.data.i );  break;
                case U_INT:  rtnObj.set_uint( left.data.u + rght.data.u ); /*--*/ break;
                default:     cout << "ERROR: BAD TYPE!" << endl;  exit( 1 );
            }
            break;
        default:  cout << "ERROR: BAD TYPE!" << endl;  exit( 1 );
    }
    return rtnObj;
}

Data64 operator-( const Data64& left, const Data64& rght ){
    // Handle type escalation and return result of subtraction
    Data64 rtnObj;
    switch( left.type ){
        case FLOAT: 
            switch( rght.type ){
                case FLOAT:  rtnObj.set_float( left.data.f - rght.data.f ); /**/ break;
                case INTGR:  rtnObj.set_float( left.data.f - rght.data.i*1.0 );  break;
                case U_INT:  rtnObj.set_float( left.data.f - rght.data.u*1.0 );  break;
                default:     cout << "ERROR: BAD TYPE!" << endl;  exit( 1 );
            }
            break;
        case INTGR: 
            switch( rght.type ){
                case FLOAT:  rtnObj.set_float( left.data.i*1.0 - rght.data.f );   break;
                case INTGR:  rtnObj.set_int( left.data.i - rght.data.i ); /*---*/ break;
                case U_INT:  rtnObj.set_int( left.data.i - (long) rght.data.u );  break;
                default:     cout << "ERROR: BAD TYPE!" << endl;  exit( 1 );
            }
            break;
        case U_INT: 
            switch( rght.type ){
                case FLOAT:  rtnObj.set_float( left.data.u*1.0 - rght.data.f );   break;
                case INTGR:  rtnObj.set_int( (long) left.data.u - rght.data.i );  break;
                case U_INT:  rtnObj.set_uint( left.data.u - rght.data.u ); /*--*/ break;
                default:     cout << "ERROR: BAD TYPE!" << endl;  exit( 1 );
            }
            break;
        default:  cout << "ERROR: BAD TYPE!" << endl;  exit( 1 );
    }
    return rtnObj;
}

Data64 operator*( const Data64& left, const Data64& rght ){
    // Handle type escalation and return result of multiplication
    Data64 rtnObj;
    switch( left.type ){
        case FLOAT: 
            switch( rght.type ){
                case FLOAT:  rtnObj.set_float( left.data.f * rght.data.f ); /**/ break;
                case INTGR:  rtnObj.set_float( left.data.f * rght.data.i*1.0 );  break;
                case U_INT:  rtnObj.set_float( left.data.f * rght.data.u*1.0 );  break;
                default:     cout << "ERROR: BAD TYPE!" << endl;  exit( 1 );
            }
            break;
        case INTGR: 
            switch( rght.type ){
                case FLOAT:  rtnObj.set_float( left.data.i*1.0 * rght.data.f );   break;
                case INTGR:  rtnObj.set_int( left.data.i * rght.data.i ); /*---*/ break;
                case U_INT:  rtnObj.set_int( left.data.i * (long) rght.data.u );  break;
                default:     cout << "ERROR: BAD TYPE!" << endl;  exit( 1 );
            }
            break;
        case U_INT: 
            switch( rght.type ){
                case FLOAT:  rtnObj.set_float( left.data.u*1.0 * rght.data.f );   break;
                case INTGR:  rtnObj.set_int( (long) left.data.u * rght.data.i );  break;
                case U_INT:  rtnObj.set_uint( left.data.u * rght.data.u ); /*--*/ break;
                default:     cout << "ERROR: BAD TYPE!" << endl;  exit( 1 );
            }
            break;
        default:  cout << "ERROR: BAD TYPE!" << endl;  exit( 1 );
    }
    return rtnObj;
}

Data64 operator/( const Data64& left, const Data64& rght ){
    // Handle type escalation and return result of division
    Data64 rtnObj;
    if( (rght.data.f == 0.0) || (rght.data.i == 0) || (rght.data.u == 0) ){
        cout << "ERROR: DIVIDE BY ZERO!" << endl;  
        exit( 1 );
    }
    switch( left.type ){
        case FLOAT: 
            switch( rght.type ){
                case FLOAT:  rtnObj.set_float( left.data.f / rght.data.f ); /**/ break;
                case INTGR:  rtnObj.set_float( left.data.f / rght.data.i*1.0 );  break;
                case U_INT:  rtnObj.set_float( left.data.f / rght.data.u*1.0 );  break;
                default:     cout << "ERROR: BAD TYPE!" << endl;  exit( 1 );
            }
            break;
        case INTGR: 
            switch( rght.type ){
                case FLOAT:  rtnObj.set_float( left.data.i*1.0 / rght.data.f );   break;
                case INTGR:  rtnObj.set_int( left.data.i / rght.data.i ); /*---*/ break;
                case U_INT:  rtnObj.set_int( left.data.i / (long) rght.data.u );  break;
                default:     cout << "ERROR: BAD TYPE!" << endl;  exit( 1 );
            }
            break;
        case U_INT: 
            switch( rght.type ){
                case FLOAT:  rtnObj.set_float( left.data.u*1.0 / rght.data.f );   break;
                case INTGR:  rtnObj.set_int( (long) left.data.u / rght.data.i );  break;
                case U_INT:  rtnObj.set_uint( left.data.u / rght.data.u ); /*--*/ break;
                default:     cout << "ERROR: BAD TYPE!" << endl;  exit( 1 );
            }
            break;
        default:  cout << "ERROR: BAD TYPE!" << endl;  exit( 1 );
    }
    return rtnObj;
}



////////// PRINT FUNCTIONS /////////////////////////////////////////////////////////////////////////


ostream& operator<<( ostream& os , const Data64& dat ){ 
    // Print data
    switch( dat.type ){
        case FLOAT:  os << dat.data.f;  break;
        case INTGR:  os << dat.data.i;  break;
        case U_INT:  os << dat.data.u;  break;
        default:     cout << "ERROR: BAD TYPE!" << endl;  exit( 1 );
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
};

struct MathNode{
    // Nestable math operation
    E_Math_Op /*------*/ type;
    shared_ptr<MathNode> parent;
    shared_ptr<MathNode> leftOp;
    shared_ptr<MathNode> rghtOp;

    Data64 evaluate(){
        // Evaluate the math expression at this node
    }
};

struct Calculator{
    shared_ptr<MathNode> root;
    shared_ptr<MathNode> curr;
    stack<Data64> /*--*/ stck;

    void parse( vstr tokens ){
        // Turn a vector of string `tokens` into a tree of `MathNode`
        for( string token : tokens ){
            // FIXME, START HERE: https://stackoverflow.com/a/16575025
            // Test float, has a decimal or is numeric with terminal lowercase f
            // Test unsigned, numeric non-negative
            // Test int, numeric negative
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