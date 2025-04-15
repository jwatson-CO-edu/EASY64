#include "CPluscal.hpp"

string _SRC_PATH = "PAS_Test/00_sections.pas";

int main(){

    vstr /*---*/ fLines = read_file_to_lines( _SRC_PATH );
    TextPortions fSeg = segment_source_file( fLines );
    Context /**/ cntxt;

    define_types(     cntxt, fSeg.type );  cout << endl;
    define_constants( cntxt, fSeg.cnst );  cout << endl;
    define_variables( cntxt, fSeg.var  );  cout << endl;


    return 0;
}