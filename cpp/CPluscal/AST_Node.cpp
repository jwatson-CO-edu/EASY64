////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "CPluscal.hpp"



////////// NODE ////////////////////////////////////////////////////////////////////////////////////

enum NodeType{
    // Types of syntax nodes
    PROGRAM, 
    FUNCTION, 
    FOR,
    ASSIGNMENT,
    IDENTIFIER,
    NUM_LITERAL,
    EXPONENT,
    MULTIPLY,
    DIVIDE,
    ADD,
    SUBTRACT,
};

map<NodeType,ubyte> NodePriority = {
    {ASSIGNMENT,4},
    {EXPONENT,  3},
    {MULTIPLY,  2},
    {DIVIDE,    2},
    {ADD, /*-*/ 1},
    {SUBTRACT,  1},
};

// NOTE: DELAY INTERPRETATION OF STRINGS UNTIL IT IS TIME TO RUN CODE

struct AST_Node{ 
    // Holds code as a tree
    NodeType /*---*/ type;
    vstr /*-------*/ tokens;
    vector<AST_Node> edges;
    P_Val /*------*/ value;
};

AST_Node make_ast_identifier( const string& name ){
    // Identifier Node, to be evaluated later
    AST_Node rtnNode{};
    rtnNode.type   = IDENTIFIER;
    rtnNode.tokens = {name,};
}


AST_Node token_expr_2_nodes( const vstr& tokens ){
    // Recursively distribute tokenized text to nodes
    AST_Node rtnNode;
    

    stack<AST_Node> operators;
    stack<AST_Node> operands;

    size_t  idx;
    size_t  i = 0;
    size_t  N = tokens.size();
    vstr    expr;
    string  token;
    bool    accum = false;

    /// Top-Level Tests ///
    if( p_vec_has( tokens, string{"="} ) ){
        idx = vec_find( tokens, string{"="} );
        rtnNode.type = ASSIGNMENT;
        rtnNode.edges.push_back( make_ast_identifier( tokens[0] ) );
        expr = vec_ltrim( tokens, 2 );
        rtnNode.edges.push_back( token_expr_2_nodes( expr ) );
    }

    // while( i < N ){
    //     token = tokens[i];
        
    //     ++i;
    // }

    return rtnNode;
}


