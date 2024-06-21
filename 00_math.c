////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "EASY64.h"


////////// ENUMS & STRUCTS /////////////////////////////////////////////////////////////////////////

///// Literals ////////////////////////////////////////////////////////////

enum E_Type{
    // Primitive Data Types
    FLOAT, // `double`
    INTGR, // `long`
    U_INT, // `unsigned long`
    STRNG, // string
    POINT, // pointer
};
typedef enum E_Type E_Type;


union D64{
    // All acceptable 64 bit types
    double f; // `double`
    long   i; // `long`
    ulong  u; // `unsigned long`
    char*  s; // string
    void*  p; // pointer
};
typedef union D64 D64;


typedef struct{
    // A container for data
    E_Type type; // Primitive Type
    D64    data; // Payload
    ulong  size; // Number of chars if `STRNG`, otherwise 1
}Data64;


///// Constructors ///////////////////////////////

Data64 make_float( double dbbl ){  
    // Return a floating point primitive
    Data64 rtn;
    rtn.type   = FLOAT;
    rtn.data.f = dbbl;
    rtn.size   = 1;
    return rtn;
}

Data64 make_int( long lng ){ 
    // Return an integer primitive
    Data64 rtn;
    rtn.type   = INTGR;
    rtn.data.i = lng;  
    rtn.size   = 1;
    return rtn;
}

Data64 make_uint( ulong ulng ){     
    // Return an unsigned integer primitive
    Data64 rtn;
    rtn.type   = U_INT;
    rtn.data.u = ulng; 
    rtn.size   = 1;
    return rtn;
}

Data64 make_string( char* str, ulong Nchrs ){     
    Data64 rtn;
    rtn.type   = STRNG;
    rtn.data.s = str; 
    rtn.size   = Nchrs;
    return rtn;
}


///// Status & Errors /////////////////////////////////////////////////////
// WARNING: ??? YAGNI ???

enum E_Stat{
    // Primitive Data Types
    OKAY, // ----- No error
    SYNTAX_ERR, // Syntax error
};
typedef enum E_Stat E_Stat;


typedef struct{
    // A container for status and errors
    E_Stat status;
    Data64 msg;
}Status;


///// Abstract Math Tree //////////////////////////////////////////////////
// WARNING: ??? YAGNI ???

enum E_Op{
    // Primitive Data Types
    ADD, // Add
    SUB, // Subtract
    MUL, // Multiply
    DIV, // Multiply
};
typedef enum E_Op E_Op;

typedef struct{
    // A container for status and errors
    void*  left;
    void*  right;
    Data64 data;
    E_Op   oprtr;
}AMT_Node;



////////// PARSING /////////////////////////////////////////////////////////////////////////////////

enum TokenMode{
    // Tokenizer flags
    NO_CHAR, // Invalid
    GOT_DGT, // Incoming digit      character
    GOT_MNS, // Incoming minus      character
    GOT_DEC, // Incoming decimal    character
    GOT_SPC, // Incoming whitespace character
    GOT_RES, // Incoming reserved   character
    GOT_CHR, // All other characters
    TKN_BGN,
    TKN_UIN,
};
typedef enum TokenMode TokenMode;

ubyte reserved_test( char c ){
    // Is `c` a reserved character?
    // WARNING: SLOW
    if( c == '+' )  return 1;
    if( c == '-' )  return 1;
    return 0;
}

TokenMode character_test( char c ){
    // Determine the type of character that was received
    // WARNING: SLOW
    if( isdigit( c ) /*-*/ )  return GOT_DGT;
    if( c == '.' /*-----*/ )  return GOT_DEC;
    if( c == '-' /*-----*/ )  return GOT_MNS;
    if( isspace( c ) /*-*/ )  return GOT_SPC;
    if( reserved_test( c ) )  return GOT_RES;
    return GOT_CHR;
}

// Queue* tokenize_math_expr( const char* exprStr, ulong Nchrs ){
//     // Return a queue of tokens relating to a math expression
//     Queue*    rtnQ  = make_Queue();
//     Queue*    token = make_Queue();
//     TokenMode mode = TKN_BGN;
//     TokenMode cTyp = NO_CHAR;
//     char /**/ c_i;
//     for( ulong i = 0; i < Nchrs; ++i ){
//         c_i  = exprStr[i];
//         cTyp = character_test( c_i );
//         switch( mode ){

//             /// State: Token Begin ///////////////
//             case TKN_BGN:
//                 switch (cTyp){
//                     case GOT_DGT:
//                         push_back_Q( token )
//                         break;
                    
//                     default:
//                         break;
//                 }
//                 break;
            


//             default:
//                 break;
//         }
//     }
// }

// FIXME: EVALUTATE A MATH EXPRESSION