#include "CPluscal.hpp"

string _SRC_PATH = "PAS_Test/00_sections.pas";

int main(){

    vstr /*-------------*/ fLines = read_file_to_lines( _SRC_PATH );
    TextPortions<vstr>     fSeg   = segment_source_file( fLines );
    TextPortions<AST_Node> fAST   = parse_sections( fSeg );

    


    return 0;
}