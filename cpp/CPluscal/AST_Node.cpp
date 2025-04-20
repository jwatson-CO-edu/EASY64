////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "CPluscal.hpp"



////////// NODE ////////////////////////////////////////////////////////////////////////////////////

enum NodeType{
    // Types of syntax nodes
    FUNCTION, // Also for main
    FOR,
    ASSIGNMENT,
    LITERAL,
};

map<NodeType,ubyte> NodePriority = {
    {ASSIGNMENT,1},
};

// NOTE: DELAY INTERPRETATION OF STRINGS UNTIL IT IS TIME TO RUN CODE

struct AST_Node{ 
    // Holds code as a tree
    NodeType /*---*/ type;
    vstr /*-------*/ tokens;
    vector<AST_Node> block;
};


AST_Node tokens_2_nodes( const vstr& tokens, NodeType type_ = FUNCTION ){
    // Recursively distribute tokenized text to nodes
    AST_Node rtnNode;
    size_t  idx;
    size_t  i = 0;
    size_t  N = tokens.size();
    vstr    statement;
    string  token;
    bool    accum = false;
    rtnNode.type = type_;
    while( i < N ){
        token = tokens[i];
        
        ++i;
    }
    return rtnNode;
}


