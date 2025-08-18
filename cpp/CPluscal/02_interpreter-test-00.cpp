////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "include/CPluscal.hpp"



////////// MAIN ////////////////////////////////////////////////////////////////////////////////////
int main( int argc, char *argv[] ){

    vstr pPaths = {
        "/home/james/za_Other/EASY64/cpp/CPluscal/PAS_Test/temperature.pas",
        "/home/james/EASY64/cpp/CPluscal/PAS_Test/temperature.pas",
    };

    cout << "Lexing file ..." << endl;
    LexMachine /**/ lexer{ first_existing_file_path( pPaths ) };
    cout << "Parsing file ..." << endl;
    CPC_Parser /**/ par{};
    cout << "Building source tree ..." << endl;
    NodePtr /*---*/ prog{ par.build_source_tree( lexer.lineTokens ) };
    CPC_Interpreter cpc{};
    cout << "Running program ..." << endl;
    P_Val res = cpc.interpret( prog );
    cout << "Result: " << res << endl << "DONE!" << endl;
    cpc.context->print_constant_state();
    cpc.context->print_variable_state();
}