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
    BINARY_OP,
    var,
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
    return rtnNode;
}


AST_Node make_ast_number_literal( const string& literalStr ){
    // Number Literal Node, to be evaluated later
    AST_Node rtnNode{};
    rtnNode.type   = NUM_LITERAL;
    rtnNode.tokens = {literalStr,};
    rtnNode.value  = str_2_number( literalStr );
    return rtnNode;
}


AST_Node make_ast_assignment( AST_Node left, AST_Node right ){
    // Number Assignment Node, to be evaluated later
    AST_Node rtnNode{};
    rtnNode.type  = ASSIGNMENT;
    rtnNode.edges = {left, right,};
    return rtnNode;
}


AST_Node make_ast_binary_op( const string& op, AST_Node left, AST_Node right ){
    // Number Assignment Node, to be evaluated later
    AST_Node rtnNode{};
    rtnNode.type   = BINARY_OP;
    rtnNode.tokens = {op,};
    rtnNode.edges  = {left, right,};
    return rtnNode;
}



class AST_Parser{ public:
    vstr   tokens;
    size_t currDex;
    size_t N;

    AST_Parser( const vstr& tokens_ ){
        tokens  = tokens_;
        currDex = 0;
        N /*-*/ = tokens_.size();
    }

    string current_token() const{  if ( currDex < N ){  return tokens[ currDex ];  }else{  return "";  }  }
    void   advance(){  if( currDex < N ){  ++currDex;  }  }

    bool match( const string& expected ){
        if (current_token() == expected) {
            advance();
            return true;
        }
        return false;
    }

    void expect( const string& expected ){
        if( !match( expected ) ){
            throw runtime_error("Expected '" + expected + "', got '" + current_token() + "'");
        }
    }

    AST_Node parse_term(){
        AST_Node value;
        if( current_token().empty() ){  throw runtime_error( "Unexpected end of input" );  }
        
        // Handle numbers
        if( p_number_string( current_token() ) ){
            value = make_ast_number_literal( current_token() );
            advance();
            return value;
        }
        
        // Handle identifiers
        if( p_identifier( current_token() ) ){
            value = make_ast_identifier( current_token() );
            advance();
            return value;
        }
        
        // Handle parenthesized expressions
        if( match("(") ){
            value = parse_expression();
            expect(")");
            return value;
        }
        
        throw runtime_error( "Unexpected token: " + current_token() );
    }

    AST_Node parse_expression(){
        AST_Node left = parse_term();
        AST_Node right;
        string   op;
        stack<AST_Node> operatorStack;
        deque<AST_Node> output;
        
        if( match("=") ){
            right = parse_expression();
            return make_ast_assignment( left, right );
        }
        
        while( match("+") || current_token() == "-" || 
               current_token() == "*" || current_token() == "/" ){
            op    = tokens[ currDex-1 ];
            right = parse_term();
            left  = make_ast_binary_op( op, left, right );
        }
        
        if( match(";") ){  cout << endl << "<Statement End>" << endl;  }
        
        return left;
    }

    
};


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


