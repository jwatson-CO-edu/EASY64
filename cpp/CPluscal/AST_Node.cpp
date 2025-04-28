////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "CPluscal.hpp"



////////// NODE ////////////////////////////////////////////////////////////////////////////////////


AST_Node make_ast_program( const string& name ){
    // Identifier Node, to be evaluated later
    AST_Node rtnNode{};
    rtnNode.type   = PROGRAM;
    rtnNode.tokens = {name,};
    return rtnNode;
}


AST_Node make_ast_type_def(){
    // Identifier Node, to be evaluated later
    AST_Node rtnNode{};
    rtnNode.type = TYPE_DEF;
    return rtnNode;
}


AST_Node make_ast_const_def( const string& name ){
    // Identifier Node, to be evaluated later
    AST_Node rtnNode{};
    rtnNode.type = CONSTANT_DEF;
    return rtnNode;
}


AST_Node make_ast_var_def( const string& name ){
    // Identifier Node, to be evaluated later
    AST_Node rtnNode{};
    rtnNode.type = VARIABLE_DEF;
    return rtnNode;
}


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



////////// PARSER //////////////////////////////////////////////////////////////////////////////////


AST_Parser::AST_Parser( const vstr& tokens_ ){
    // Load tokenized program into the parser
    tokens  = tokens_;
    currDex = 0;
    N /*-*/ = tokens_.size();
}

// Peek current token string
string AST_Parser::current_token() const{  if ( currDex < N ){  return tokens[ currDex ];  }else{  return "";  }  }


// Move to next token string
void AST_Parser::advance(){  if( currDex < N ){  ++currDex;  }  }


bool AST_Parser::match( const string& expected ){
    // Return True if the current token string matches what is `expected`
    if( current_token() == expected ){
        advance();
        return true;
    }
    return false;
}


void AST_Parser::expect( const string& expected ){
    // Throw a `runtime_error` if `match` fails
    if( !match( expected ) ){
        throw runtime_error("Expected '" + expected + "', got '" + current_token() + "'");
    }
}


AST_Node AST_Parser::parse_term(){
    // Parse one expression term as an AST node
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


AST_Node AST_Parser::parse_expression(){
    // Read tokens until an expression subtree is obtained
    AST_Node left = parse_term();
    AST_Node right;
    string   op;
    
    if( match("=") ){
        right = parse_expression();
        return make_ast_assignment( left, right );
    }
    
    while( match("+") || current_token() == "-" || // WARNING: THIS DOES NOT RESPECT OPERATOR PRECEDENCE!
            current_token() == "*" || current_token() == "/" || 
            current_token() == "**" ){
        op    = tokens[ currDex-1 ];
        right = parse_term();
        left  = make_ast_binary_op( op, left, right );
    }
    
    if( match(";") ){  cout << endl << "<Statement End>" << endl;  }
    
    return left;
}


AST_Node AST_Parser::parse(){
    // Convert the collection of tokens to an Abstract Source Tree
    AST_Node program = make_ast_program( "main" );
    while( currDex < N ){  program.edges.push_back(  parse_expression()  );  }
    return program;
}




