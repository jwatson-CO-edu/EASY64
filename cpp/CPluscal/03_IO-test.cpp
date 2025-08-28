////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "include/CPluscal.hpp"



////////// MAIN ////////////////////////////////////////////////////////////////////////////////////
int main( int argc, char *argv[] ){

    vstr pPaths = {
        "/home/james/za_Other/EASY64/cpp/CPluscal/PAS_Test/input.pas",
        "/home/james/EASY64/cpp/CPluscal/PAS_Test/input.pas",
    };

    cout << "Lexing file ..." << endl;
    LexMachine /**/ lexer{ first_existing_file_path( pPaths ) };
    cout << "Parsing file ..." << endl;
    CPC_Parser /**/ par{};
    cout << "Building source tree ..." << endl;
    NodePtr /*---*/ prog{ par.build_source_tree( lexer.lineTokens ) };
    CPC_Interpreter cpc{};
    cout << "Running program ..." << endl << endl;
    P_Obj res = cpc.interpret( prog );
}