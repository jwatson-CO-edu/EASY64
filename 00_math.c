////////// INIT ////////////////////////////////////////////////////////////////////////////////////

/// Imports ///
#include <stddef.h> // NULL, etc.

/// Aliases ///
typedef unsigned long ulong;
typedef unsigned char ubyte; // Use this instead of bool



////////// ENUMS & STRUCTS /////////////////////////////////////////////////////////////////////////

///// Literals ////////////////////////////////////////////////////////////

enum E_Type{
    // Primitive Data Types
    FLOAT, // `double`
    INTGR, // `long`
    U_INT, // `unsigned long`
    STRNG, // string
};
typedef enum E_Type E_Type;


union D64{
    // All acceptable 64 bit types
    double f;
    long   i;
    ulong  u;
    char*  s;
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


////////// DATA STRUCTURES /////////////////////////////////////////////////////////////////////////
// WARNING: ??? YAGNI ???

typedef struct{
    // Element of a stack, Holds generic data
    void* data;
    void* next;
}StackNode;

Stack* make_StackNode( void ){
    // Return an empty `StackNode`
    StackNode* rtnNode = (StackNode*) malloc( sizeof( StackNode ) );
    rtnNode->data = NULL;
    rtnNode->next = NULL;
    return rtnNode;
}

void del_StackNode( StackNode* node ){
    // Erase `node`
    free( node->data );
    free( node->next );
    free( node );
}

typedef struct{
    // Generic stack
    StackNode* top;
    ulong /**/ len;
}Stack;

Stack* make_Stack( void ){
    // Return an empty `Stack`
    Stack* rtnStk = (Stack*) malloc( sizeof( Stack ) );
    rtnStk->top = NULL;
    rtnStk->len = 0;
    return rtnStk;
}

void push_Stack( Stack* stck, void* elemData ){
    // Push data onto stack
    StackNode* nuNode = (StackNode*) malloc( sizeof( StackNode ) );
    nuNode->data = elemData;
    nuNode->next = stck->top;
    stck->top   = nuNode;
    (stck->len)++;
}

void* pop_Stack( Stack* stck ){
    // Pop data from stack
    StackNode* stkTop = NULL;
    void* /**/ rtnDat = NULL;
    if( stck->len > 0 ){
        stkTop    = stck->top;
        stck->top = stkTop->next;
        rtnDat    = stkTop->data;
        del_StackNode( stkTop );
    }
    return rtnDat;
}

void del_Stack( Stack* stck ){
    // Erase `stck` and all nodes contained therein
    StackNode* stkTop = NULL;
    while( stck->len ){  pop_Stack( stck );  }
    free( stck );
}


////////// BASIC MATH //////////////////////////////////////////////////////////////////////////////
// FIXME: TOKENIZE  A MATH EXPRESSION
// FIXME: EVALUTATE A MATH EXPRESSION