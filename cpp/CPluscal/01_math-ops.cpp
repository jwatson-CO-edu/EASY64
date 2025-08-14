////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "include/CPluscal.hpp"



////////// MAIN ////////////////////////////////////////////////////////////////////////////////////
int main( int argc, char *argv[] ){
    vstr /*------*/ expr0{ "2", "+", "3" };
    vstr /*------*/ expr1{ "2", "+", "3", "*", "4" };
    vstr /*------*/ expr2{ "2", "*", "3", "+", "4" };
    vstr /*------*/ expr3{ "2", "*", "3", "+", "4", "*", "5" };
    vstr /*------*/ expr4{ "2", "*", "(", "3", "+", "4", ")", "*", "5" };
    vstr /*------*/ expr5{ "2", "*", "(", "3", "*", "(", "4", "+", "6", ")", "+", "7", ")", "*", "5" };
    CntxPtr /*---*/ cntx{ new Context{} };
    CPC_Interpreter cpc{};
    cout << cpc.calculate( expr0, cntx ) << endl;
    cout << cpc.calculate( expr1, cntx ) << endl;
    cout << cpc.calculate( expr2, cntx ) << endl;
    cout << cpc.calculate( expr3, cntx ) << endl;
    cout << cpc.calculate( expr4, cntx ) << endl;
    cout << cpc.calculate( expr5, cntx ) << endl;
}