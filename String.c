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
    if( (nxtDex == 0) && ((str->len) > 0) ){
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
        // printf( "(%c,%lu):%lu, ", c, i, str->len );
        c = chunk[ ++i ];
        
    }
}


char* get_String_as_char_array( String* str ){
    // Convert `str` contents to a char array
    ulong N /*-*/ = str->len;
    char* rtnArr  = (char*) malloc( (N+1) * sizeof( char ) );
    ulong i /*-*/ = 0;
    Elem* segment = str->q->front;
    ulong nxtDex;
    while( i < N ){
        nxtDex = i % _STR_CHUNK_LEN;
        if( (nxtDex == 0) && (i > 0) ){  segment = segment->next;  }
        // printf( "(%p,%lu):", segment->data, i );
        rtnArr[ i++ ] = ((char*) segment->data)[ nxtDex ];
        // printf( "%c, ", rtnArr[ i-1 ] );
    }
    rtnArr[i] = '\0';
    return rtnArr;
}


void del_String( String* str ){
    // Remove the string from the heap
    del_Queue( str->q );
    free( str );
}