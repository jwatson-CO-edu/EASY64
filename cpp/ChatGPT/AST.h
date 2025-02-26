#ifndef AST_H
#define AST_H

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include <memory>
#include <map>
#include <vector>

extern llvm::LLVMContext TheContext;
extern llvm::IRBuilder<> Builder;
extern std::unique_ptr<llvm::Module> TheModule;
extern std::map<std::string, llvm::Value*> NamedValues;

// If-Else Statement AST
class IfExprAST : public ExprAST {
    std::unique_ptr<ExprAST> Cond, Then, Else;
public:
    IfExprAST(std::unique_ptr<ExprAST> Cond, std::unique_ptr<ExprAST> Then, std::unique_ptr<ExprAST> Else)
        : Cond(std::move(Cond)), Then(std::move(Then)), Else(std::move(Else)) {}
    llvm::Value* codegen() override;
};

// While Loop AST
class WhileExprAST : public ExprAST {
    std::unique_ptr<ExprAST> Cond, Body;
public:
    WhileExprAST(std::unique_ptr<ExprAST> Cond, std::unique_ptr<ExprAST> Body)
        : Cond(std::move(Cond)), Body(std::move(Body)) {}
    llvm::Value* codegen() override;
};

// For Loop AST
class ForExprAST : public ExprAST {
    std::string VarName;
    std::unique_ptr<ExprAST> Start, End, Step, Body;
public:
    ForExprAST(std::string VarName, std::unique_ptr<ExprAST> Start, std::unique_ptr<ExprAST> End, 
               std::unique_ptr<ExprAST> Step, std::unique_ptr<ExprAST> Body)
        : VarName(std::move(VarName)), Start(std::move(Start)), End(std::move(End)), Step(std::move(Step)), Body(std::move(Body)) {}
    llvm::Value* codegen() override;
};

#endif
