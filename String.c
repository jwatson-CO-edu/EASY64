////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "EASY64.h"



////////// STRINGS /////////////////////////////////////////////////////////////////////////////////
// Implements a string as a dynamic character array

String* make_String( void ){
    // Create a `String` on the heap
    String* rtnStr  = (String*) malloc( sizeof( String ) );
    void*   dataArr = malloc( _STR_CHUNK_LEN * sizeof( char ) );
    rtnStr->q   = make_Queue();
    rtnStr->len = 0;
    push_back_Q( rtnStr->q, dataArr );
    return rtnStr;
}


void append_char_String( String* str, const char c ){
    // Store one char, Expand if needed
    ulong nxtDex  = (str->len) % _STR_CHUNK_LEN;
    void* dataArr = NULL;
    // If the previous buffer has filled, then create new
    if( (nxtDex == 0) && ((str->len) != 0) ){
        dataArr = malloc( _STR_CHUNK_LEN * sizeof( char ) );
        push_back_Q( str->q, dataArr );
    }
    ((char*) str->q->back->data)[nxtDex] = c;
    (str->len)++;
}


void append_char_array_String( String* str, const char* chunk ){
    // Read in the char array and store
    // NOTE: This function assumes that `chunk` has a '\0' terminator
    ulong i = 0;
    char  c = chunk[0];
    while( c != '\0' ){
        append_char_String( str, c );
        c = chunk[ ++i ];
    }
}


char* get_String_as_char_array( String* str ){
    // Convert `str` contents to a char array
    ulong N /*-*/ = str->len;
    char* rtnArr  = (char*) malloc( (N+1) * sizeof( char ) );
    ulong i /*-*/ = 0;
    char* segment = NULL;
    ulong nxtDex;
    while( i < N ){
        nxtDex = i % _STR_CHUNK_LEN;
        // FIXME, START HERE: CONVERTING TO AN ARRAY SHOULD *NOT* BE **DESTRUCTIVE**!!
        if( nxtDex == 0 ){  segment = (char*) pop_front_Q( str->q );  }
        rtnArr[ ++i ] = segment[ nxtDex ];
    }
    rtnArr[i] = '\0';
    return rtnArr;
}

// FIXME: DELETE STRING STRUCTURE