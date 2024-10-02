// gcc -o mathTest.out 00_math.c String.c Structs.c -lm

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

Data64* make_float( double dbbl ){  
    // Return a floating point primitive
    Data64* rtn = (Data64*) malloc( sizeof( Data64 ) );
    rtn->type   = FLOAT;
    rtn->data.f = dbbl;
    rtn->size   = 1;
    return rtn;
}

Data64* make_int( long lng ){ 
    // Return an integer primitive
    Data64* rtn = (Data64*) malloc( sizeof( Data64 ) );
    rtn->type   = INTGR;
    rtn->data.i = lng;  
    rtn->size   = 1;
    return rtn;
}

Data64* make_uint( ulong ulng ){     
    // Return an unsigned integer primitive
    Data64* rtn = (Data64*) malloc( sizeof( Data64 ) );
    rtn->type   = U_INT;
    rtn->data.u = ulng; 
    rtn->size   = 1;
    return rtn;
}

Data64* make_string( char* str, ulong Nchrs ){     
    Data64* rtn = (Data64*) malloc( sizeof( Data64 ) );
    rtn->type   = STRNG;
    rtn->data.s = str; 
    rtn->size   = Nchrs;
    return rtn;
}


///// Printing ////////////////////////////////////////////////////////////

void print_Data64( Data64* d64 ){
    // Print the datum
    switch( d64->type ){
        case FLOAT: // `double`
            printf( "<D64:FLOAT=%f>", d64->data.f );
            break;
        case INTGR: // `long`
            printf( "<D64:INTGR=%d>", d64->data.i );
            break;
        case U_INT: // `unsigned long`
            printf( "<D64:U_INT=%lu>", d64->data.u );
            break;
        case STRNG: // string
            printf( "<D64:STRNG=\"%s\">", d64->data.s );
            break;
        case POINT: // pointer
            printf( "<D64:POINT=0x%p>", d64->data.p );
            break;
        default:
            printf( "<D64:ERROR-UNK-TYPE>" );
    }
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
    NUM, // Number Literal
    ADD, // Add
    SUB, // Subtract
    MUL, // Multiply
    DIV, // Divide
};
typedef enum E_Op E_Op;

typedef struct{
    // A container for status and errors
    void*  left; //- Left  Child
    void*  right; // Right Child
    Data64 data; //- Data Payload
    E_Op   oprtr; // Operator represented by this node
    ubyte  prec; //- Precedence of this operation (Higher done first!)
}AMT_Node;



////////// PARSING /////////////////////////////////////////////////////////////////////////////////

enum TokenMode{
    // Tokenizer flags
    NO_CHAR, // Invalid
    ALL_END, // Termination
    GOT_DGT, // Incoming digit      character
    GOT_MNS, // Incoming minus      character
    GOT_DEC, // Incoming decimal    character
    GOT_SPC, // Incoming whitespace character
    GOT_RES, // Incoming reserved   character
    GOT_TRM, // Incoming terminator character
    GOT_CHR, // All other characters
    TKN_BGN,
    TKN_UIN,
    TKN_FLT,
    TKN_INT,
    TKN_STR,
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
    if( c == '\0' /*----*/ )  return GOT_TRM;
    /*---------------------*/ return GOT_CHR;
}

Queue* tokenize_math_expr( const char* exprStr, ulong Nchrs ){
    // Return a queue of tokens relating to a math expression
    
    /// Init ///
    Queue*    rtnQ  = make_Queue(); // Instantiate a linear struct for tokens
    String*   token = make_String(); // Dyn. String representation of a single token
    TokenMode mode  = TKN_BGN; // Init mode
    TokenMode cTyp  = NO_CHAR; // Init char flag
    char /**/ c_i;
    char*     ptr;
    
    /// Tokenizer Loop ///
    for( ulong i = 0; i < Nchrs; ++i ){
        c_i  = exprStr[i];
        cTyp = character_test( c_i );

        if( i >= (Nchrs-1) ){  mode = ALL_END;  }

        switch( mode ){

            /// State: Token Begin ///////////////
            case TKN_BGN:
                switch (cTyp){

                    case GOT_DGT:
                        append_char_String( token, c_i );
                        mode = TKN_UIN;
                        break;

                    case GOT_DEC:
                        append_char_String( token, '0' );
                        append_char_String( token, c_i );
                        mode = TKN_FLT;
                        break;

                    case GOT_SPC:
                        mode = TKN_BGN;
                        break;

                    case GOT_MNS:
                        append_char_String( token, c_i );
                        mode = TKN_INT;
                        break;

                    case GOT_RES:
                        append_char_String( token, c_i );
                        push_back_Q( rtnQ, (void*) make_string( 
                            get_String_as_char_array( token ),
                            1     
                        ) );
                        clear_String( token );
                        mode = TKN_BGN;
                        break;

                    case GOT_TRM:
                        mode = ALL_END;
                        break;

                    case GOT_CHR:
                        append_char_String( token, c_i );
                        mode = TKN_STR;
                        break;

                    default:
                        printf( "WARNING, UNHANDLED CHAR: %c in initial mode!\n", c_i );
                        break;
                }
                break;
            
            /// State: Token Unsigned Int ////////
            case TKN_UIN:
                switch (cTyp){
                    
                    case GOT_DGT:
                        append_char_String( token, c_i );
                        mode = TKN_UIN;
                        break;

                    case GOT_DEC:
                        append_char_String( token, c_i );
                        mode = TKN_FLT;
                        break;

                    case GOT_MNS:
                    case GOT_RES:
                    case GOT_SPC:

                        push_back_Q( rtnQ, (void*) make_uint( strtoul( 
                            get_String_as_char_array( token ), 
                            &ptr, 
                            10
                        ) ) );
                        clear_String( token );

                        if( cTyp != GOT_SPC ){
                            append_char_String( token, c_i );
                            push_back_Q( rtnQ, (void*) make_string( get_String_as_char_array( token ), token->len ) );
                            clear_String( token );
                        }

                        mode = TKN_BGN;
                        break;

                    case GOT_CHR:
                        append_char_String( token, c_i );
                        mode = TKN_STR;
                        break;
                    
                    default:
                        printf( "WARNING, UNHANDLED CHAR: %c in initial mode!\n", c_i );
                        break;
                }
                break;

            /// State: Token Unsigned Int ////////
            case ALL_END:
                print( "TOKENIZER HALTED\n" );
                break;

            /// State: UNKNOWN ///////////////////
            default:
                printf( "WARNING, UNHANDLED TOKENIZER MODE: %i\n", (int) mode );
                break;
        }
    }
}

void print_math_token_Queue( Queue* mq ){
    // Print the math tokens in order
    // FIXME, START HERE: PRINT EACH TOKEN (`Data64`) IN ORDER
}

// https://inspirnathan.com/posts/156-abstract-syntax-trees-with-shunting-yard-algorithm


////////// MAIN ////////////////////////////////////////////////////////////////////////////////////

// FIXME: EVALUTATE A MATH EXPRESSION
int main(){
    Queue* mathExpr = tokenize_math_expr( "2+3", 3 );
}