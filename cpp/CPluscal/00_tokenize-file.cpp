////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "include/CPluscal.hpp"



////////// MAIN ////////////////////////////////////////////////////////////////////////////////////
int main( int argc, char *argv[] ){
    vstr pPaths = {
        "/home/james/za_Other/EASY64/cpp/CPluscal/PAS_Test/temperature.pas",
        "/home/james/EASY64/cpp/CPluscal/PAS_Test/temperature.pas",
    };
    LexMachine lex{ first_existing_file_path( pPaths ) };
    return 0;
}