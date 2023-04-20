////////// INIT ////////////////////////////////////////////////////////////////////////////////////

#include <string>
using std::string;
#include <map>
using std::map;

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
    map<string, Var64<long>>   ntgrStore;
    map<string, Var64<ulong>>  uintStore;
};