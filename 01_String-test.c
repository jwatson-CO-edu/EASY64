////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "EASY64.h"



////////// MAIN ////////////////////////////////////////////////////////////////////////////////////
int main(){
    String* str = make_String();
    char*   out = NULL;

    for( ubyte i = 0; i < _STR_CHUNK_LEN; ++i ){
        append_char_array_String( str, "I " );
        append_char_array_String( str, "have " );
        append_char_array_String( str, "made " );
        append_char_array_String( str, "a " );
        append_char_array_String( str, "String!  " );
    }

    printf( "Length of My String: %lu\n", str->len );
    out = get_String_as_char_array( str );
    printf( "Presenting, My String:\n%s", out );

    return 0;
}