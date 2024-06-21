////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "EASY64.h"



////////// DATA STRUCTURES /////////////////////////////////////////////////////////////////////////
// WARNING: ??? YAGNI ???

///// Basic Elements //////////////////////////////////////////////////////

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