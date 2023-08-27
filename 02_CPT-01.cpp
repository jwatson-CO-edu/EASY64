#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;
#include <string>
using std::string;


enum CPT_Status{
    INITIAL,
    RUNNING,
    SUCCESS,
    FAILURE,
};

struct CPT_Token{
    string token;
    uint   line;
};

class CST_Node{ public:
    shared_ptr<CST_Node> /*---*/ parent;
    vector<shared_ptr<CST_Node>> children;
};

class CPT_Node{ public:
    string /*-----------------*/ name;
    CPT_Status /*-------------*/ status;
    shared_ptr<CPT_Node> /*---*/ parent;
    vector<shared_ptr<CPT_Node>> children;
};