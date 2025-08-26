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
    vstr /*------*/ expr8{ "2", "-", "3" }; // -1
    vstr /*------*/ expr9{ "2", "-", "3", "*", "4" }; // -10
    vstr /*------*/ exprA{ "2", "*", "(", "3", "+", "4", ")", "/", "5" }; // 2.8
    vstr /*------*/ exprB{ "2.1", "-", "3" }; // -0.9

    CntxPtr /*---*/ cntx{ new Context{} };
    CPC_Interpreter cpc{};
    cout << cpc.calculate( as_vobj( expr0 ), cntx ) << endl; //   5
    cout << cpc.calculate( as_vobj( expr1 ), cntx ) << endl; //  14
    cout << cpc.calculate( as_vobj( expr2 ), cntx ) << endl; //  10
    cout << cpc.calculate( as_vobj( expr3 ), cntx ) << endl; //  26
    cout << cpc.calculate( as_vobj( expr4 ), cntx ) << endl; //  70
    cout << cpc.calculate( as_vobj( expr5 ), cntx ) << endl; // 370
    cout << cpc.calculate( as_vobj( expr6 ), cntx ) << endl; // 300
    cout << cpc.calculate( as_vobj( expr7 ), cntx ) << endl; // 170
    cout << cpc.calculate( as_vobj( expr8 ), cntx ) << endl; //  -1
    cout << cpc.calculate( as_vobj( expr9 ), cntx ) << endl; // -10
    cout << cpc.calculate( as_vobj( exprA ), cntx ) << endl; //   2.8
    cout << cpc.calculate( as_vobj( exprB ), cntx ) << endl; //  -0.9
}