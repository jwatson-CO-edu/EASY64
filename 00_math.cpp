////////// INIT ////////////////////////////////////////////////////////////////////////////////////

/// Imports ///
#include <stdlib.h> // exit, EXIT_FAILURE 
using std::exit;
#include <iostream>
using std::cout, std::endl, std::flush;
using std::ostream;

/// Aliases ///
typedef unsigned long ulong;

////////// ATOMS ///////////////////////////////////////////////////////////////////////////////////

enum E_Type{
    // Primitive Data Types
    FLOAT, // `double`
    INTGR, // `long`
    U_INT, // `unsigned long`
};

union D64{
    double f;
    long   i;
    ulong  u;
};

struct Data64{
    // A container for data
    E_Type type;
    D64    data;

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

int main(){
    Data64 A;  A.set_float( 1.0 );
    Data64 B;  B.set_int( 2 );
    Data64 C;  C.set_uint( 3 );
    Data64 D = A + B;
    Data64 E = A + C;
    Data64 F = B + C;

    cout << A << endl;
    cout << B << endl;
    cout << C << endl;
    cout << D << endl;
    cout << E << endl;
    cout << F << endl;

    cout << endl;

    cout << A.type << endl;
    cout << B.type << endl;
    cout << C.type << endl;
    cout << D.type << endl;
    cout << E.type << endl;
    cout << F.type << endl;
}