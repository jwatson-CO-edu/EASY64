////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "include/CPluscal.hpp"



////////// MAIN ////////////////////////////////////////////////////////////////////////////////////
int main( int argc, char *argv[] ){
    vstr pPaths = {
        "/home/james/za_Other/EASY64/cpp/CPluscal/PAS_Test/temperature.pas",
        "/home/james/EASY64/cpp/CPluscal/PAS_Test/temperature.pas",
    };

    CPC_Parser cpc{};
    LexMachine lexer{ first_existing_file_path( pPaths ) };
    
    cpc.build_source_tree( lexer.lineTokens );
    return 0;
}