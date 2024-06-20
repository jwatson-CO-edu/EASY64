////////// INIT ////////////////////////////////////////////////////////////////////////////////////

/// Imports ///
#include <stddef.h> // NULL, etc.
#include <ctype.h> //- Character tests, etc.

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


////////// DATA STRUCTURES /////////////////////////////////////////////////////////////////////////
// WARNING: ??? YAGNI ???

///// Basic Elements //////////////////////////////////////////////////////

typedef struct{
    // Element of a stack, Holds generic data
    void* data;
    void* prev;
    void* next;
}Elem;

Elem* make_Elem( void* data_ ){
    // Return an empty `Elem`
    Elem* rtnNode = (Elem*) malloc( sizeof( Elem ) );
    rtnNode->data = data_;
    rtnNode->prev = NULL;
    rtnNode->next = NULL;
    return rtnNode;
}

void del_Elem( Elem* node ){
    // Erase `node`
    free( node->data );
    free( node );
}

///// Stack ///////////////////////////////////////////////////////////////

typedef struct{
    // Generic stack
    Elem* top;
    ulong len;
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
    Elem* nuNode = make_Elem( elemData );
    nuNode->next = stck->top;
    stck->top   = nuNode;
    (stck->len)++;
}

void* pop_Stack( Stack* stck ){
    // Pop data from stack
    Elem* stkTop = NULL;
    void* /**/ rtnDat = NULL;
    if( stck->len > 0 ){
        stkTop    = stck->top;
        stck->top = stkTop->next;
        rtnDat    = stkTop->data;
        free( stkTop ); // Leaves data intact @ heap
        (stck->len)--;
    }
    return rtnDat;
}

void del_Stack( Stack* stck ){
    // Erase `stck` and all nodes contained therein
    Elem* stkTop = NULL;
    void* data_i;
    while( stck->len ){  
        data_i = pop_Stack( stck );  
        free( data_i );
    }
    free( stck );
}


///// Queue ///////////////////////////////////////////////////////////////

typedef struct{
    // Generic stack
    Elem* front;
    Elem* back;
    ulong len;
}Queue;

Queue* make_Queue( void ){
    // Return an empty `Queue`
    Queue* rtnQ = (Queue*) malloc( sizeof( Queue ) );
    rtnQ->front = NULL;
    rtnQ->back  = NULL;
    rtnQ->len   = 0;
    return rtnQ;
}

void push_back_Q( Queue* q, void* elemData ){
    // Push data onto `q` back
    Elem* nuNode = make_Elem( elemData );
    if( (q->len) == 0 ){  
        q->front = nuNode;    
    }else{
        q->back->next = nuNode;
    }
    q->back = nuNode;
    (q->len)++;
}

void* pop_front_Q( Queue* q ){
    // Pop data from `q` front
    Elem* qFront = NULL;
    void* rtnDat = NULL;
    if( q->len > 0 ){
        qFront    = q->front;
        q->front  = qFront->next;
        rtnDat    = qFront->data;
        free( qFront );
        (q->len)--;
    }
    return rtnDat;
}

void del_Queue( Queue* q ){
    // Erase `q` and all nodes contained therein
    Elem* stkTop = NULL;
    void* data_i;
    while( q->len ){  
        data_i = pop_front_Q( q );  
        free( data_i );
    }
    free( q );
}

///// Queue as String /////////////////////////////////////////////////////
#define _CHUNK_LEN 128

typedef struct{
    // Wrapper for a `Queue` of character chunks
    Queue* q;
    ulong  len;
}String;

String* make_String( void ){
    String* // FIXME, START HERE: ALLOCATE STRING 
}



////////// BASIC MATH //////////////////////////////////////////////////////////////////////////////

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

Queue* tokenize_math_expr( const char* exprStr, ulong Nchrs ){
    // Return a queue of tokens relating to a math expression
    Queue*    rtnQ  = make_Queue();
    Queue*    token = make_Queue();
    TokenMode mode = TKN_BGN;
    TokenMode cTyp = NO_CHAR;
    char /**/ c_i;
    for( ulong i = 0; i < Nchrs; ++i ){
        c_i  = exprStr[i];
        cTyp = character_test( c_i );
        switch( mode ){

            /// State: Token Begin ///////////////
            case TKN_BGN:
                switch (cTyp){
                    case GOT_DGT:
                        push_back_Q( token )
                        break;
                    
                    default:
                        break;
                }
                break;
            


            default:
                break;
        }
    }
}

// FIXME: EVALUTATE A MATH EXPRESSION