#ifndef EASY64_H
#define EASY64_H

////////// INIT ////////////////////////////////////////////////////////////////////////////////////

/// Imports ///
#include <stddef.h> // NULL, etc.
#include <ctype.h> //- Character tests, etc.
#include <stdio.h> //- `printf`
#include <stdlib.h> // `malloc`

/// Aliases ///
typedef unsigned long ulong;
typedef unsigned char ubyte; // Use this instead of bool



////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////// Structs.c //////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


////////// DATA STRUCTURES /////////////////////////////////////////////////////////////////////////
// WARNING: ??? YAGNI ???

///// Basic Elements //////////////////////////////////////////////////////

typedef struct{
    // Element of a stack, Holds generic data
    void* data;
    void* prev;
    void* next;
}Elem;

Elem* make_Elem( void* data_ ); // Return an empty `Elem`
void del_Elem( Elem* node ); // -- Erase `node`


///// Stack ///////////////////////////////////////////////////////////////

typedef struct{
    // Generic stack
    Elem* top;
    ulong len;
}Stack;

Stack* make_Stack( void ); // ---------------------- Return an empty `Stack`
void   push_Stack( Stack* stck, void* elemData ); // Push data onto stack
void*  pop_Stack( Stack* stck ); // ---------------- Pop data from stack
void   del_Stack( Stack* stck ); // ---------------- Erase `stck` and all nodes contained therein


///// Queue ///////////////////////////////////////////////////////////////

typedef struct{
    // Generic stack
    Elem* front;
    Elem* back;
    ulong len;
}Queue;

Queue* make_Queue( void ); // -------------------- Return an empty `Queue`
void   push_back_Q( Queue* q, void* elemData ); // Push data onto `q` back
void*  pop_front_Q( Queue* q ); // --------------- Pop data from `q` front
void   del_Queue( Queue* q ); // ----------------- Erase `q` and all nodes contained therein



////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////// String.c ///////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

///// Queue as String /////////////////////////////////////////////////////
#define _STR_CHUNK_LEN 128

typedef struct{
    // Wrapper for a `Queue` of character chunks
    Queue* q;
    ulong  len;
}String;

String* make_String( void ); // -------------------------------------- Create a `String` on the heap
void    clear_String( String* str ); // ------------------------------ Erase current contents and prep for insertion
void    append_char_String( String* str, const char c ); // ---------- Store one char, Expand if needed
void    append_char_array_String( String* str, const char* chunk ); // Read in the char array and store
char*   get_String_as_char_array( String* str ); // ------------------ Convert `str` contents to a char array
void    del_String( String* str ); // -------------------------------- Remove the string from the heap
    

#endif