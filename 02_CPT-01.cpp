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

struct ParseMessage{
    // Bidirectional parslet message - Down: Smallest parsable unit, Up: Parslet result
    ParseStatus status;
    string /**/ token;
    uint /*--*/ line;
    string /**/ msg;
};

class SourceNode{ public:
    // Node of a Concrete Source Tree
    shared_ptr<SourceNode> /*---*/ parent;
    vector<shared_ptr<SourceNode>> children;
    uint /*---------------------*/ line;
};

class ParseNode{ public:
    // Superclass of nestable parslets, inspired by Behavior Trees
    string /*------------------*/ name;
    ParseStatus /*-------------*/ status;
    shared_ptr<ParseNode> /*---*/ parent;
    vector<shared_ptr<ParseNode>> children;
};

class Parse_Machine{
    // Parser: Mediates parsing by the parslet tree, resulting in CST
    stack<string>   thing1;
    stack<ParseNode> thing2;
    shared_ptr<SourceNode> source;
    shared_ptr<ParseNode> parseTree;
};