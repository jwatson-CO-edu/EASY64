#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>

// Forward declarations
class ASTNode;
class ExpressionAST;
class NumberExprAST;
class VariableExprAST;
class BinaryExprAST;
class CallExprAST;
class IfExprAST;
class ForExprAST;
class PrototypeAST;
class FunctionAST;
class Lexer;
class Parser;
class CodeGenerator;

using namespace llvm;

// Token types
enum Token {
    tok_eof = -1,
    
    // commands
    tok_def = -2,
    tok_extern = -3,
    
    // control
    tok_if = -4,
    tok_then = -5,
    tok_else = -6,
    tok_for = -7,
    tok_in = -8,
    
    // primary
    tok_identifier = -9,
    tok_number = -10,
    
    // operators
    tok_plus = -11,
    tok_minus = -12,
    tok_mul = -13,
    tok_div = -14,
    tok_lt = -15,
    tok_gt = -16,
    tok_eq = -17,
};

//===----------------------------------------------------------------------===//
// Lexer
//===----------------------------------------------------------------------===//

class Lexer {
private:
    std::string IdentifierStr;
    double NumVal;
    int LastChar;
    std::istringstream InputStream;

public:
    Lexer(const std::string& Input) : InputStream(Input) {
        LastChar = InputStream.get();
    }

    Token getToken() {
        // Skip whitespace
        while (isspace(LastChar))
            LastChar = InputStream.get();

        // Identifier: [a-zA-Z][a-zA-Z0-9]*
        if (isalpha(LastChar)) {
            IdentifierStr = LastChar;
            while (isalnum((LastChar = InputStream.get())))
                IdentifierStr += LastChar;

            if (IdentifierStr == "def") return tok_def;
            if (IdentifierStr == "extern") return tok_extern;
            if (IdentifierStr == "if") return tok_if;
            if (IdentifierStr == "then") return tok_then;
            if (IdentifierStr == "else") return tok_else;
            if (IdentifierStr == "for") return tok_for;
            if (IdentifierStr == "in") return tok_in;
            return tok_identifier;
        }

        // Number: [0-9.]+
        if (isdigit(LastChar) || LastChar == '.') {
            std::string NumStr;
            do {
                NumStr += LastChar;
                LastChar = InputStream.get();
            } while (isdigit(LastChar) || LastChar == '.');

            NumVal = strtod(NumStr.c_str(), nullptr);
            return tok_number;
        }

        // Comment handling
        if (LastChar == '#') {
            do {
                LastChar = InputStream.get();
            } while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
            
            if (LastChar != EOF)
                return getToken();
        }

        // Check for end of file
        if (LastChar == EOF)
            return tok_eof;

        // Operators
        if (LastChar == '+') { LastChar = InputStream.get(); return tok_plus; }
        if (LastChar == '-') { LastChar = InputStream.get(); return tok_minus; }
        if (LastChar == '*') { LastChar = InputStream.get(); return tok_mul; }
        if (LastChar == '/') { LastChar = InputStream.get(); return tok_div; }
        if (LastChar == '<') { LastChar = InputStream.get(); return tok_lt; }
        if (LastChar == '>') { LastChar = InputStream.get(); return tok_gt; }
        if (LastChar == '=') { LastChar = InputStream.get(); return tok_eq; }

        // Return this character as its ASCII value
        int ThisChar = LastChar;
        LastChar = InputStream.get();
        return static_cast<Token>(ThisChar);
    }

    double getNumVal() const { return NumVal; }
    std::string getIdentifier() const { return IdentifierStr; }
};

//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (AST)
//===----------------------------------------------------------------------===//

// Base AST class
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual Value* codegen(CodeGenerator& CG) = 0;
};

// Expression class
class ExpressionAST : public ASTNode {
public:
    virtual ~ExpressionAST() = default;
};

// Number literal expression
class NumberExprAST : public ExpressionAST {
    double Val;

public:
    NumberExprAST(double Val) : Val(Val) {}
    double getVal() const { return Val; }
    
    Value* codegen(CodeGenerator& CG) override;
};

// Variable reference expression
class VariableExprAST : public ExpressionAST {
    std::string Name;

public:
    VariableExprAST(const std::string& Name) : Name(Name) {}
    const std::string& getName() const { return Name; }
    
    Value* codegen(CodeGenerator& CG) override;
};

// Binary operator expression
class BinaryExprAST : public ExpressionAST {
    Token Op;
    std::unique_ptr<ExpressionAST> LHS, RHS;

public:
    BinaryExprAST(Token Op, std::unique_ptr<ExpressionAST> LHS, std::unique_ptr<ExpressionAST> RHS)
        : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
        
    Token getOp() const { return Op; }
    const ExpressionAST* getLHS() const { return LHS.get(); }
    const ExpressionAST* getRHS() const { return RHS.get(); }
    
    Value* codegen(CodeGenerator& CG) override;
};

// Function call expression
class CallExprAST : public ExpressionAST {
    std::string Callee;
    std::vector<std::unique_ptr<ExpressionAST>> Args;

public:
    CallExprAST(const std::string& Callee, std::vector<std::unique_ptr<ExpressionAST>> Args)
        : Callee(Callee), Args(std::move(Args)) {}
        
    const std::string& getCallee() const { return Callee; }
    const std::vector<std::unique_ptr<ExpressionAST>>& getArgs() const { return Args; }
    
    Value* codegen(CodeGenerator& CG) override;
};

// If expression
class IfExprAST : public ExpressionAST {
    std::unique_ptr<ExpressionAST> Cond, Then, Else;

public:
    IfExprAST(std::unique_ptr<ExpressionAST> Cond,
              std::unique_ptr<ExpressionAST> Then,
              std::unique_ptr<ExpressionAST> Else)
        : Cond(std::move(Cond)), Then(std::move(Then)), Else(std::move(Else)) {}
        
    const ExpressionAST* getCond() const { return Cond.get(); }
    const ExpressionAST* getThen() const { return Then.get(); }
    const ExpressionAST* getElse() const { return Else.get(); }
    
    Value* codegen(CodeGenerator& CG) override;
};

// For loop expression
class ForExprAST : public ExpressionAST {
    std::string VarName;
    std::unique_ptr<ExpressionAST> Start, End, Step, Body;

public:
    ForExprAST(const std::string& VarName,
               std::unique_ptr<ExpressionAST> Start,
               std::unique_ptr<ExpressionAST> End,
               std::unique_ptr<ExpressionAST> Step,
               std::unique_ptr<ExpressionAST> Body)
        : VarName(VarName), Start(std::move(Start)), End(std::move(End)),
          Step(std::move(Step)), Body(std::move(Body)) {}
          
    const std::string& getVarName() const { return VarName; }
    const ExpressionAST* getStart() const { return Start.get(); }
    const ExpressionAST* getEnd() const { return End.get(); }
    const ExpressionAST* getStep() const { return Step.get(); }
    const ExpressionAST* getBody() const { return Body.get(); }
    
    Value* codegen(CodeGenerator& CG) override;
};

// Function prototype
class PrototypeAST : public ASTNode {
    std::string Name;
    std::vector<std::string> Args;

public:
    PrototypeAST(const std::string& Name, std::vector<std::string> Args)
        : Name(Name), Args(std::move(Args)) {}
        
    const std::string& getName() const { return Name; }
    const std::vector<std::string>& getArgs() const { return Args; }
    
    Function* codegen(CodeGenerator& CG) override;
};

// Function definition
class FunctionAST : public ASTNode {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExpressionAST> Body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExpressionAST> Body)
        : Proto(std::move(Proto)), Body(std::move(Body)) {}
        
    const PrototypeAST* getProto() const { return Proto.get(); }
    const ExpressionAST* getBody() const { return Body.get(); }
    
    Function* codegen(CodeGenerator& CG) override;
};

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

class Parser {
private:
    Lexer& Lex;
    Token CurTok;
    std::map<char, int> BinopPrecedence;

    void getNextToken() {
        CurTok = Lex.getToken();
    }

    // Error handling
    std::unique_ptr<ExpressionAST> LogError(const char* Str) {
        std::cerr << "Error: " << Str << std::endl;
        return nullptr;
    }

    std::unique_ptr<PrototypeAST> LogErrorP(const char* Str) {
        LogError(Str);
        return nullptr;
    }

    // Expression parsing
    std::unique_ptr<ExpressionAST> ParseExpression();
    std::unique_ptr<ExpressionAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExpressionAST> LHS);
    std::unique_ptr<ExpressionAST> ParsePrimary();
    std::unique_ptr<ExpressionAST> ParseNumberExpr();
    std::unique_ptr<ExpressionAST> ParseParenExpr();
    std::unique_ptr<ExpressionAST> ParseIdentifierExpr();
    std::unique_ptr<ExpressionAST> ParseIfExpr();
    std::unique_ptr<ExpressionAST> ParseForExpr();

    // Function parsing
    std::unique_ptr<PrototypeAST> ParsePrototype();
    std::unique_ptr<FunctionAST> ParseDefinition();
    std::unique_ptr<FunctionAST> ParseTopLevelExpr();
    std::unique_ptr<PrototypeAST> ParseExtern();

    int GetTokPrecedence();

public:
    Parser(Lexer& L) : Lex(L) {
        // Install standard binary operators
        BinopPrecedence['+'] = 20;
        BinopPrecedence['-'] = 20;
        BinopPrecedence['*'] = 40;
        BinopPrecedence['/'] = 40;
        BinopPrecedence['<'] = 10;
        BinopPrecedence['>'] = 10;
        BinopPrecedence['='] = 5;

        // Prime the first token
        getNextToken();
    }

    // Parse methods
    std::unique_ptr<FunctionAST> ParseFunction();
    std::vector<std::unique_ptr<ASTNode>> ParseProgram();
};

// Get the precedence of the current token
int Parser::GetTokPrecedence() {
    if (CurTok < 0)
        return -1;

    int TokPrec = BinopPrecedence[CurTok];
    if (TokPrec <= 0)
        return -1;
    return TokPrec;
}

// Parse a number literal
std::unique_ptr<ExpressionAST> Parser::ParseNumberExpr() {
    auto Result = std::make_unique<NumberExprAST>(Lex.getNumVal());
    getNextToken(); // consume the number
    return std::move(Result);
}

// Parse an expression in parentheses
std::unique_ptr<ExpressionAST> Parser::ParseParenExpr() {
    getNextToken(); // eat (.
    auto V = ParseExpression();
    if (!V)
        return nullptr;

    if (CurTok != ')')
        return LogError("expected ')'");
    getNextToken(); // eat ).
    return V;
}

// Parse an identifier expression (var reference or function call)
std::unique_ptr<ExpressionAST> Parser::ParseIdentifierExpr() {
    std::string IdName = Lex.getIdentifier();
    getNextToken(); // eat identifier.

    if (CurTok != '(') // Simple variable ref.
        return std::make_unique<VariableExprAST>(IdName);

    // Function call.
    getNextToken(); // eat (
    std::vector<std::unique_ptr<ExpressionAST>> Args;
    if (CurTok != ')') {
        while (true) {
            if (auto Arg = ParseExpression())
                Args.push_back(std::move(Arg));
            else
                return nullptr;

            if (CurTok == ')')
                break;

            if (CurTok != ',')
                return LogError("Expected ')' or ',' in argument list");
            getNextToken();
        }
    }

    getNextToken(); // Eat the ')'.
    return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

// Parse if-then-else
std::unique_ptr<ExpressionAST> Parser::ParseIfExpr() {
    getNextToken(); // eat the if.

    // condition.
    auto Cond = ParseExpression();
    if (!Cond)
        return nullptr;

    if (CurTok != tok_then)
        return LogError("expected then");
    getNextToken(); // eat the then

    auto Then = ParseExpression();
    if (!Then)
        return nullptr;

    if (CurTok != tok_else)
        return LogError("expected else");

    getNextToken();

    auto Else = ParseExpression();
    if (!Else)
        return nullptr;

    return std::make_unique<IfExprAST>(std::move(Cond), std::move(Then),
                                          std::move(Else));
}

// Parse for loop
std::unique_ptr<ExpressionAST> Parser::ParseForExpr() {
    getNextToken(); // eat the for.

    if (CurTok != tok_identifier)
        return LogError("expected identifier after for");

    std::string IdName = Lex.getIdentifier();
    getNextToken(); // eat identifier.

    if (CurTok != '=')
        return LogError("expected '=' after for variable");
    getNextToken(); // eat '='.

    auto Start = ParseExpression();
    if (!Start)
        return nullptr;
    if (CurTok != ',')
        return LogError("expected ',' after for start value");
    getNextToken();

    auto End = ParseExpression();
    if (!End)
        return nullptr;

    // The step value is optional.
    std::unique_ptr<ExpressionAST> Step;
    if (CurTok == ',') {
        getNextToken();
        Step = ParseExpression();
        if (!Step)
            return nullptr;
    } else {
        // If not specified, use 1.0.
        Step = std::make_unique<NumberExprAST>(1.0);
    }

    if (CurTok != tok_in)
        return LogError("expected 'in' after for");
    getNextToken(); // eat 'in'.

    auto Body = ParseExpression();
    if (!Body)
        return nullptr;

    return std::make_unique<ForExprAST>(IdName, std::move(Start), std::move(End),
                                           std::move(Step), std::move(Body));
}

// Parse a primary expression
std::unique_ptr<ExpressionAST> Parser::ParsePrimary() {
    switch (CurTok) {
    case tok_identifier:
        return ParseIdentifierExpr();
    case tok_number:
        return ParseNumberExpr();
    case '(':
        return ParseParenExpr();
    case tok_if:
        return ParseIfExpr();
    case tok_for:
        return ParseForExpr();
    default:
        return LogError("unknown token when expecting an expression");
    }
}

// Parse a binary operation
std::unique_ptr<ExpressionAST> Parser::ParseBinOpRHS(int ExprPrec,
                                                    std::unique_ptr<ExpressionAST> LHS) {
    while (true) {
        int TokPrec = GetTokPrecedence();

        if (TokPrec < ExprPrec)
            return LHS;

        Token BinOp = CurTok;
        getNextToken(); // eat binop

        auto RHS = ParsePrimary();
        if (!RHS)
            return nullptr;

        int NextPrec = GetTokPrecedence();
        if (TokPrec < NextPrec) {
            RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
            if (!RHS)
                return nullptr;
        }

        LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
    }
}

// Parse an expression
std::unique_ptr<ExpressionAST> Parser::ParseExpression() {
    auto LHS = ParsePrimary();
    if (!LHS)
        return nullptr;

    return ParseBinOpRHS(0, std::move(LHS));
}

// Parse a function prototype
std::unique_ptr<PrototypeAST> Parser::ParsePrototype() {
    if (CurTok != tok_identifier)
        return LogErrorP("Expected function name in prototype");

    std::string FnName = Lex.getIdentifier();
    getNextToken();

    if (CurTok != '(')
        return LogErrorP("Expected '(' in prototype");

    std::vector<std::string> ArgNames;
    getNextToken();
    while (CurTok == tok_identifier) {
        ArgNames.push_back(Lex.getIdentifier());
        getNextToken();

        if (CurTok == ')')
            break;

        if (CurTok != ',')
            return LogErrorP("Expected ')' or ',' in prototype");
        getNextToken();
    }

    if (CurTok != ')')
        return LogErrorP("Expected ')' in prototype");

    getNextToken(); // eat ')'.

    return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

// Parse a function definition
std::unique_ptr<FunctionAST> Parser::ParseDefinition() {
    getNextToken(); // eat def.
    auto Proto = ParsePrototype();
    if (!Proto)
        return nullptr;

    if (auto E = ParseExpression())
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    return nullptr;
}

// Parse an extern declaration
std::unique_ptr<PrototypeAST> Parser::ParseExtern() {
    getNextToken(); // eat extern.
    return ParsePrototype();
}

// Parse a top-level expression
std::unique_ptr<FunctionAST> Parser::ParseTopLevelExpr() {
    if (auto E = ParseExpression()) {
        // Make an anonymous proto.
        auto Proto = std::make_unique<PrototypeAST>("__anon_expr",
                                                std::vector<std::string>());
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}

// Parse a function (either definition or top-level expression)
std::unique_ptr<FunctionAST> Parser::ParseFunction() {
    if (CurTok == tok_def)
        return ParseDefinition();
    else
        return ParseTopLevelExpr();
}

// Parse the entire program
std::vector<std::unique_ptr<ASTNode>> Parser::ParseProgram() {
    std::vector<std::unique_ptr<ASTNode>> Program;
    
    while (CurTok != tok_eof) {
        switch (CurTok) {
        case tok_def:
            if (auto FnAST = ParseDefinition()) {
                Program.push_back(std::move(FnAST));
            } else {
                getNextToken(); // Skip token to recover from error
            }
            break;
        case tok_extern:
            if (auto ProtoAST = ParseExtern()) {
                Program.push_back(std::move(ProtoAST));
            } else {
                getNextToken(); // Skip token to recover from error
            }
            break;
        default:
            if (auto FnAST = ParseTopLevelExpr()) {
                Program.push_back(std::move(FnAST));
            } else {
                getNextToken(); // Skip token to recover from error
            }
            break;
        }
    }
    
    return Program;
}

//===----------------------------------------------------------------------===//
// Code Generator
//===----------------------------------------------------------------------===//

class CodeGenerator {
private:
    std::unique_ptr<LLVMContext> TheContext;
    std::unique_ptr<Module> TheModule;
    std::unique_ptr<IRBuilder<>> Builder;
    std::map<std::string, Value*> NamedValues;
    std::map<std::string, Function*> FunctionProtos;

public:
    CodeGenerator() {
        TheContext = std::make_unique<LLVMContext>();
        TheModule = std::make_unique<Module>("my compiler", *TheContext);
        Builder = std::make_unique<IRBuilder<>>(*TheContext);
    }

    LLVMContext& getContext() { return *TheContext; }
    Module& getModule() { return *TheModule; }
    IRBuilder<>& getBuilder() { return *Builder; }
    
    std::map<std::string, Value*>& getNamedValues() { return NamedValues; }
    std::map<std::string, Function*>& getFunctionProtos() { return FunctionProtos; }
    
    Function* getFunction(const std::string& Name) {
        // First, see if the function has already been added to the current module.
        if (auto* F = TheModule->getFunction(Name))
            return F;

        // If not, check whether we can codegen the declaration from a prototype.
        auto FI = FunctionProtos.find(Name);
        if (FI != FunctionProtos.end())
            return FI->second;

        // If not, the function isn't defined.
        return nullptr;
    }

    void dumpCode() {
        TheModule->print(outs(), nullptr);
    }
};

//===----------------------------------------------------------------------===//
// AST Code Generation Implementation
//===----------------------------------------------------------------------===//

Value* NumberExprAST::codegen(CodeGenerator& CG) {
    return ConstantFP::get(CG.getContext(), APFloat(Val));
}

Value* VariableExprAST::codegen(CodeGenerator& CG) {
    // Look this variable up in the function.
    Value* V = CG.getNamedValues()[Name];
    if (!V)
        std::cerr << "Unknown variable name: " << Name << std::endl;
    return V;
}

Value* BinaryExprAST::codegen(CodeGenerator& CG) {
    Value* L = LHS->codegen(CG);
    Value* R = RHS->codegen(CG);
    if (!L || !R)
        return nullptr;
    
    IRBuilder<>& Builder = CG.getBuilder();
    
    switch (Op) {
    case tok_plus:
        return Builder.CreateFAdd(L, R, "addtmp");
    case tok_minus:
        return Builder.CreateFSub(L, R, "subtmp");
    case tok_mul:
        return Builder.CreateFMul(L, R, "multmp");
    case tok_div:
        return Builder.CreateFDiv(L, R, "divtmp");
    case tok_lt:
        L = Builder.CreateFCmpULT(L, R, "cmptmp");
        // Convert bool 0/1 to double 0.0 or 1.0
        return Builder.CreateUIToFP(L, Type::getDoubleTy(CG.getContext()), "booltmp");
    case tok_gt:
        L = Builder.CreateFCmpUGT(L, R, "cmptmp");
        return Builder.CreateUIToFP(L, Type::getDoubleTy(CG.getContext()), "booltmp");
    case tok_eq:
        L = Builder.CreateFCmpUEQ(L, R, "cmptmp");
        return Builder.CreateUIToFP(L, Type::getDoubleTy(CG.getContext()), "booltmp");
    default:
        std::cerr << "Invalid binary operator" << std::endl;
        return nullptr;
    }
}

Value* CallExprAST::codegen(CodeGenerator& CG) {
    // Look up the function in the module table.
    Function* CalleeF = CG.getFunction(Callee);
    if (!CalleeF) {
        std::cerr << "Unknown function referenced: " << Callee << std::endl;
        return nullptr;
    }

    // If argument mismatch error.
    if (CalleeF->arg_size() != Args.size()) {
        std::cerr << "Incorrect # arguments passed" << std::endl;
        return nullptr;
    }

    std::vector<Value*> ArgsV;
    for (unsigned i = 0, e = Args.size(); i != e; ++i) {
        ArgsV.push_back(Args[i]->codegen(CG));
        if (!ArgsV.back())
            return nullptr;
    }

    return CG.getBuilder().CreateCall(CalleeF, ArgsV, "calltmp");
}

Value* IfExprAST::codegen(CodeGenerator& CG) {
    Value* CondV = Cond->codegen(CG);
    if (!CondV)
        return nullptr;

    // Convert condition to a bool by comparing non-equal to 0.0.
    CondV = CG.getBuilder().CreateFCmpONE(
        CondV, ConstantFP::get(CG.getContext(), APFloat(0.0)), "ifcond");

    Function* TheFunction = CG.getBuilder().GetInsertBlock()->getParent();

    // Create blocks for the then and else cases. Insert the 'then' block at the
    // end of the function.
    BasicBlock* ThenBB = BasicBlock::Create(CG.getContext(), "then", TheFunction);
    BasicBlock* ElseBB = BasicBlock::Create(CG.getContext(), "else");
    BasicBlock* MergeBB = BasicBlock::Create(CG.getContext(), "ifcont");

    CG.getBuilder().CreateCondBr(CondV, ThenBB, ElseBB);

    // Emit then value.
    CG.getBuilder().SetInsertPoint(ThenBB);

    Value* ThenV = Then->codegen(CG);
    if (!ThenV)
        return nullptr;

    CG.getBuilder().CreateBr(MergeBB);
    // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
    ThenBB = CG.getBuilder().GetInsertBlock();

    // Emit else block.
    TheFunction->getBasicBlockList().push_back(ElseBB);
    CG.getBuilder().SetInsertPoint(ElseBB);

    Value* ElseV = Else->codegen(CG);
    if (!ElseV)
        return nullptr;

    CG.getBuilder().CreateBr(MergeBB);
    // Codegen of 'Else' can change the current block, update ElseBB for the PHI.
    ElseBB = CG.getBuilder().GetInsertBlock();

    // Emit merge block.
    TheFunction->getBasicBlockList().push_back(MergeBB);
    CG.getBuilder().SetInsertPoint(MergeBB);
    PHINode* PN = CG.getBuilder().CreatePHI(Type::getDoubleTy(CG.getContext()), 2, "iftmp");

    PN->addIncoming(ThenV, ThenBB);
    PN->addIncoming(ElseV, ElseBB);
    return PN;
}

Value* ForExprAST::codegen(CodeGenerator& CG) {
    // Emit the start expression first, without 'variable' in scope.
    Value* StartVal = Start->codegen(CG);
    if (!StartVal)
        return nullptr;

    // Make the new basic block for the loop header, inserting after current block.
    Function* TheFunction = CG.getBuilder().GetInsertBlock()->getParent();
    BasicBlock* PreheaderBB = CG.getBuilder().GetInsertBlock();
    BasicBlock* LoopBB = BasicBlock::Create(CG.getContext(), "loop", TheFunction);

    // Insert an explicit fall through from the current block to the LoopBB.
    CG.getBuilder().CreateBr(LoopBB);

    // Start insertion in LoopBB.
    CG.getBuilder().SetInsertPoint(LoopBB);

    // Create the PHI node with an entry for Start.
    PHINode* Variable = CG.getBuilder().CreatePHI(Type::getDoubleTy(CG.getContext()), 2, VarName);
    Variable->addIncoming(StartVal, PreheaderBB);

    // Within the loop, the variable is defined equal to the PHI node.
    // Save previous value of the variable in the symbol

    // Within the loop, the variable is defined equal to the PHI node.
    // Save previous value of the variable in the symbol table.
    Value* OldVal = CG.getNamedValues()[VarName];
    CG.getNamedValues()[VarName] = Variable;

    // Emit the body of the loop. This, like any other expr, can change the
    // current BB. Note that we ignore the value computed by the body.
    if (!Body->codegen(CG))
        return nullptr;

    // Emit the step value.
    Value* StepVal = nullptr;
    if (Step) {
        StepVal = Step->codegen(CG);
        if (!StepVal)
            return nullptr;
    } else {
        // If not specified, use 1.0.
        StepVal = ConstantFP::get(CG.getContext(), APFloat(1.0));
    }

    // Compute the end condition.
    Value* EndCond = End->codegen(CG);
    if (!EndCond)
        return nullptr;

    // Reload, increment, and restore the alloca. This handles the case where
    // the body of the loop mutates the variable.
    Value* CurVar = Variable;
    Value* NextVar = CG.getBuilder().CreateFAdd(CurVar, StepVal, "nextvar");

    // Convert condition to a bool by comparing non-equal to 0.0.
    EndCond = CG.getBuilder().CreateFCmpONE(
        EndCond, ConstantFP::get(CG.getContext(), APFloat(0.0)), "loopcond");

    // Create the "after loop" block and insert it.
    BasicBlock* LoopEndBB = CG.getBuilder().GetInsertBlock();
    BasicBlock* AfterBB = BasicBlock::Create(CG.getContext(), "afterloop", TheFunction);

    // Insert the conditional branch into the end of LoopEndBB.
    CG.getBuilder().CreateCondBr(EndCond, LoopBB, AfterBB);

    // Any new code will be inserted in AfterBB.
    CG.getBuilder().SetInsertPoint(AfterBB);

    // Add a new entry to the PHI node for the backedge.
    Variable->addIncoming(NextVar, LoopEndBB);

    // Restore the unshadowed variable.
    if (OldVal)
        CG.getNamedValues()[VarName] = OldVal;
    else
        CG.getNamedValues().erase(VarName);

    // for expr always returns 0.0.
    return Constant::getNullValue(Type::getDoubleTy(CG.getContext()));
}

Function* PrototypeAST::codegen(CodeGenerator& CG) {
    // Create a vector of doubles for the function parameters
    std::vector<Type*> Doubles(Args.size(), Type::getDoubleTy(CG.getContext()));
    
    // Create function type: double(double, double, ...) based on args
    FunctionType* FT = FunctionType::get(Type::getDoubleTy(CG.getContext()), Doubles, false);
    
    // Create the function
    Function* F = Function::Create(FT, Function::ExternalLinkage, Name, CG.getModule());
    
    // Set names for all arguments
    unsigned Idx = 0;
    for (auto& Arg : F->args())
        Arg.setName(Args[Idx++]);
    
    return F;
}

Function* FunctionAST::codegen(CodeGenerator& CG) {
    // Check if the function already exists in the current module
    Function* TheFunction = CG.getFunction(Proto->getName());
    
    if (!TheFunction)
        TheFunction = Proto->codegen(CG);
    
    if (!TheFunction)
        return nullptr;
    
    // Create a new basic block to start insertion into
    BasicBlock* BB = BasicBlock::Create(CG.getContext(), "entry", TheFunction);
    CG.getBuilder().SetInsertPoint(BB);
    
    // Record the function arguments in the NamedValues map
    CG.getNamedValues().clear();
    for (auto& Arg : TheFunction->args())
        CG.getNamedValues()[std::string(Arg.getName())] = &Arg;
    
    if (Value* RetVal = Body->codegen(CG)) {
        // Finish off the function
        CG.getBuilder().CreateRet(RetVal);
        
        // Validate the generated code, checking for consistency
        verifyFunction(*TheFunction);
        
        return TheFunction;
    }
    
    // Error reading body, remove function
    TheFunction->eraseFromParent();
    return nullptr;
}

//===----------------------------------------------------------------------===//
// Main Driver
//===----------------------------------------------------------------------===//

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <source-file>" << std::endl;
        return 1;
    }
    
    std::ifstream SourceFile(argv[1]);
    if (!SourceFile) {
        std::cerr << "Error: could not open file " << argv[1] << std::endl;
        return 1;
    }
    
    std::stringstream Buffer;
    Buffer << SourceFile.rdbuf();
    std::string SourceCode = Buffer.str();
    
    // Initialize LLVM
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();

    // Create the lexer
    Lexer Lex(SourceCode);
    
    // Create the parser
    Parser P(Lex);
    
    // Parse the program
    auto Program = P.ParseProgram();
    
    // Generate code
    CodeGenerator CG;
    
    // Codegen the AST
    for (auto& Node : Program) {
        Node->codegen(CG);
    }
    
    // Print the generated code
    CG.dumpCode();
    
    return 0;
}

//===----------------------------------------------------------------------===//
// Sample program
//===----------------------------------------------------------------------===//

/*
# Simple language example:

# Define a function to compute fibonacci numbers
def fib(x)
  if x < 3 then
    1
  else
    fib(x-1) + fib(x-2)

# Call function
fib(10)

# Define a function with a loop
def loop_example(n)
  for i = 1, n, 1 in
    i * i

# Simple arithmetic
def arithmetic(a, b)
  a + b * (a - b) / 2

*/