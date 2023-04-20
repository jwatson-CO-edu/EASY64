////////// INIT ////////////////////////////////////////////////////////////////////////////////////

#include <string>
using std::string;
#include <map>
using std::map;
#include <vector>
using std::vector;

typedef unsigned long ulong;

////////// ATOMS ///////////////////////////////////////////////////////////////////////////////////

enum E_Type{
    FLOAT,
    INTGR,
    U_INT,
};

template <typename T>
struct Var64{
    T      data;
    // ulong  refCount; // 2023-04-20: Not used?
    E_Type typ;
};


////////// CONTEXT /////////////////////////////////////////////////////////////////////////////////

struct Context{
    Context* /*-------------*/ parent;
    map<string, Var64<double>> flotStore;
    map<string, Var64<long>>   intrStore;
    map<string, Var64<ulong>>  uintStore;
};

////////// ABSTRACT SYNTAX TREE ////////////////////////////////////////////////////////////////////

enum E_Expr{
    ASSIGN,
    LITERAL,
    RHV,
};

struct AST_Expr{
    E_Expr /*------*/ exp; // Expression type
    E_Type /*------*/ typ; // Datatype it resolves to
    vector<AST_Expr*> opr; // Operands
    void* /*-------*/ dat; // Data payload
};