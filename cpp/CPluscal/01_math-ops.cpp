////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "include/CPluscal.hpp"



////////// MAIN ////////////////////////////////////////////////////////////////////////////////////
int main( int argc, char *argv[] ){
    vstr /*------*/ expr0{ "2", "+", "3" }; // 5
    vstr /*------*/ expr1{ "2", "+", "3", "*", "4" }; // 14
    vstr /*------*/ expr2{ "2", "*", "3", "+", "4" }; // 10
    vstr /*------*/ expr3{ "2", "*", "3", "+", "4", "*", "5" }; // 26
    vstr /*------*/ expr4{ "2", "*", "(", "3", "+", "4", ")", "*", "5" }; // 70
    vstr /*------*/ expr5{ "2", "*", "(", "3", "*", "(", "4", "+", "6", ")", "+", "7", ")", "*", "5" }; // 370
    vstr /*------*/ expr6{ "2", "*", "(", "3", "*", "(", "4", "+", "6", ")", ")", "*", "5" }; // 300
    vstr /*------*/ expr7{ "2", "*", "(", "(", "4", "+", "6", ")", "+", "7", ")", "*", "5" }; // 170
    CntxPtr /*---*/ cntx{ new Context{} };
    CPC_Interpreter cpc{};
    cout << cpc.calculate( expr0, cntx ) << endl; //   5
    cout << cpc.calculate( expr1, cntx ) << endl; //  14
    cout << cpc.calculate( expr2, cntx ) << endl; //  10
    cout << cpc.calculate( expr3, cntx ) << endl; //  26
    cout << cpc.calculate( expr4, cntx ) << endl; //  70
    cout << cpc.calculate( expr5, cntx ) << endl; // 370
    cout << cpc.calculate( expr6, cntx ) << endl; // 300
    cout << cpc.calculate( expr7, cntx ) << endl; // 170
}