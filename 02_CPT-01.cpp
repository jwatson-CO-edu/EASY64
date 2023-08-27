#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;
#include <string>
using std::string;
#include <stack>
using std::stack;


enum ParseStatus{
    INITIAL,
    RUNNING,
    SUCCESS,
    FAILURE,
};

struct ParseToken{
    string token;
    uint   line;
};

class SourceNode{ public:
    shared_ptr<SourceNode> /*---*/ parent;
    vector<shared_ptr<SourceNode>> children;
};

class ParseNode{ public:
    string /*-----------------*/ name;
    ParseStatus /*-------------*/ status;
    shared_ptr<ParseNode> /*---*/ parent;
    vector<shared_ptr<ParseNode>> children;
};

class Parse_Machine{
    stack<string>   thing1;
    stack<ParseNode> thing2;
    shared_ptr<SourceNode> source;
    shared_ptr<ParseNode> parseTree;
};