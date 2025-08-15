////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "include/CPluscal.hpp"



////////// INTERPRETER /////////////////////////////////////////////////////////////////////////////

CPC_Interpreter::CPC_Interpreter(){  context = CntxPtr{ new Context{} };  }


ubyte PEMDAS( const string& q ){
    vvstr pemdas = {
        {"(",")"}, // Parens
        {"**"}, // -- Exponent
        {"*","/"}, // Mult / Div
        {"+","-"}, // Add / Sub
    };
    ubyte i = 0;
    for( const vstr& lvl : pemdas ){  if( p_vstr_has_str( lvl, q ) ){  return i;  }else{  ++i;  }  }
    return 255;
}


P_Val infix_op( const P_Val& v1, const string& op, const P_Val& v2 ){
    if( op == "**" ){
        return pow( v1, v2 );
    }else if( op == "*" ){
        return v1 * v2;
    }else if( op == "/" ){
        return v1 / v2;
    }else if( op == "+" ){
        return v1 + v2;
    }else if( op == "-" ){
        return v1 - v2;
    }else{  return make_nan();  }
}


vstr get_parenthetical( const vstr& expr, size_t bgn = 0 ){
    size_t depth = 1;
    size_t i     = bgn+1;
    size_t N     = expr.size();
    vstr   rtnVec;
    string elem;
    while( i < N ){
        elem = expr[i];
        if( elem == "(" ){  ++depth;  }
        if( elem == ")" ){  --depth;  }
        if( depth > 0 ){  rtnVec.push_back( elem );  }else{  break;  }
        ++i;
    }
    return rtnVec;
}


P_Val CPC_Interpreter::calculate( const vstr& expr, CntxPtr cntx ){
    // Implements a stack-based calculator
    P_Val /*---*/ result = make_nan(); 
    string /*--*/ lastOp = "";
    size_t /*--*/ N /**/ = expr.size();
    size_t /*--*/ i /**/ = 0;
    size_t /*--*/ skip   = 0;
    vstr /*----*/ subExp;
    stack<string> oprs;
    stack<P_Val>  vals;
    P_Val /*---*/ lastVal;
    P_Val /*---*/ prevVal;
    P_Val /*---*/ currVal;
    
    if( p_literal_math_expr( expr ) ){
        cout << endl << "BGN!-----------------" << endl << endl;
        for( const string& tkn : expr ){

            if( skip > 0 ){
                --skip;
                continue;
            }

            if( p_number_string( tkn ) || (tkn == "(") ){
                if( tkn == "(" ){
                    subExp  = get_parenthetical( expr, i );
                    cout << endl << "PARENTHETICAL: " << subExp << endl << endl;
                    skip    = subExp.size()+1;
                    lastVal = calculate( subExp, cntx );
                }else{
                    lastVal = str_2_primitive( tkn );
                }
                cout << "Got Value: " << lastVal << endl;
                // if( lastOp.size() ){
                if( oprs.size() && (PEMDAS( lastOp ) <= PEMDAS( oprs.top() )) ){
                    cout << lastOp << " <= " << oprs.top() << endl;
                    prevVal = vals.top();
                    vals.pop();
                    // cout << prevVal << " " << lastOp << " " << lastVal << endl;
                    // cout << "result: " << infix_op( prevVal, lastOp, lastVal ) << endl;
                    vals.push( infix_op( prevVal, lastOp, lastVal ) );
                    lastOp = "";
                    lastVal = make_nan();
                }else if( oprs.size() && (PEMDAS( lastOp ) > PEMDAS( oprs.top() )) ){
                    cout << lastOp << " > " << oprs.top() << endl;
                    currVal = vals.top();
                    vals.pop();
                    prevVal = vals.top();
                    vals.pop();
                    // cout << prevVal << " " << oprs.top() << " " << currVal << endl;
                    // cout << "result: " << infix_op( prevVal, oprs.top(), currVal ) << endl;
                    vals.push( infix_op( prevVal, oprs.top(), currVal ) );
                    oprs.pop();
                }
                if( lastOp.size() ){
                    oprs.push( lastOp );
                    // cout << oprs.size() << " on the OP stack!" << endl;
                }
                // }
                if( i >= (N-1) ){  
                    if( !p_nan( lastVal ) ){
                        vals.push( lastVal );
                        // cout << vals.size() << " on the VALUE stack!" << endl;
                    }
                }
                
            }else if( p_math_op( tkn ) ){
                lastOp = tkn;
                cout << "Got Op: " << lastOp << endl;
                if( !p_nan( lastVal ) ){
                    vals.push( lastVal );
                    // cout << vals.size() << " on the VALUE stack!" << endl;
                }else{  return lastVal;  }
            }

            cout << "\n/// Stack State ///" << endl  
                 << "Values:" << vals << endl
                 << "Ops: _ " << oprs << endl << endl;

            ++i;
        }
        while( oprs.size() ){
        // while( 0 ){
            lastOp  = oprs.top(); oprs.pop();
            lastVal = vals.top(); vals.pop();
            prevVal = vals.top(); vals.pop();
            cout << prevVal << " " << lastOp << " " << lastVal << endl;
            vals.push( infix_op( prevVal, lastOp, lastVal ) );
        }
    }
    cout << endl << "------------END!---> " << vals.top() << endl << endl;
    return vals.top();
}


P_Val CPC_Interpreter::interpret( NodePtr sourceTree, CntxPtr cntx ){
    NodePtr root = sourceTree;
    switch ( root->type ){
        case PROGRAM:
            cout << "Run program!" << endl;
            for( const NodePtr node : root->edges ){  interpret( node, cntx );  }
            break;
        
        default:
            break;
    }
    return make_nan();
}
