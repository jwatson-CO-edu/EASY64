#include "CPluscal.hpp"

string _SRC_PATH = "PAS_Test/temperature.pas";

int main(){

    PascalInterpreter pascal{};
    pascal.init_file( _SRC_PATH );

    return 0;
}