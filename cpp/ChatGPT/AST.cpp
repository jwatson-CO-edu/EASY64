#include "AST.h"

#include "AST.h"

std::map<std::string, llvm::Value*> NamedValues; // Variable storage

// Codegen for variables (e.g., `x`)
llvm::Value* VariableExprAST::codegen() {
    llvm::Value* V = NamedValues[Name];
    if (!V) {
        std::cerr << "Unknown variable: " << Name << std::endl;
        return nullptr;
    }
    return Builder.CreateLoad(llvm::Type::getDoubleTy(TheContext), V, Name.c_str());
}

// Codegen for variable assignment (e.g., `let x = 5;`)
llvm::Value* VariableAssignAST::codegen() {
    llvm::Value* ValueCode = Value->codegen();
    if (!ValueCode) return nullptr;

    llvm::AllocaInst* Alloca = Builder.CreateAlloca(llvm::Type::getDoubleTy(TheContext), 0, Name.c_str());
    Builder.CreateStore(ValueCode, Alloca);
    NamedValues[Name] = Alloca;
    return ValueCode;
}

// Codegen for functions
llvm::Function* FunctionAST::codegen() {
    std::vector<llvm::Type*> ArgTypes(Args.size(), llvm::Type::getDoubleTy(TheContext));
    llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getDoubleTy(TheContext), ArgTypes, false);
    llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name, TheModule.get());

    // Create function body
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "entry", F);
    Builder.SetInsertPoint(BB);

    // Store arguments in NamedValues map
    NamedValues.clear();
    unsigned Idx = 0;
    for (auto &Arg : F->args()) {
        llvm::AllocaInst* Alloca = Builder.CreateAlloca(llvm::Type::getDoubleTy(TheContext), 0, Args[Idx].c_str());
        Builder.CreateStore(&Arg, Alloca);
        NamedValues[Args[Idx]] = Alloca;
        ++Idx;
    }

    if (llvm::Value* RetVal = Body->codegen()) {
        Builder.CreateRet(RetVal);
        verifyFunction(*F);
        return F;
    }

    F->eraseFromParent();
    return nullptr;
}


llvm::Value* IfExprAST::codegen() {
    llvm::Value* CondV = Cond->codegen();
    if (!CondV) return nullptr;

    // Convert condition to a boolean (0.0 -> false, anything else -> true)
    CondV = Builder.CreateFCmpONE(CondV, llvm::ConstantFP::get(TheContext, llvm::APFloat(0.0)), "ifcond");

    llvm::Function* TheFunction = Builder.GetInsertBlock()->getParent();

    // Create basic blocks for then, else, and merge
    llvm::BasicBlock* ThenBB = llvm::BasicBlock::Create(TheContext, "then", TheFunction);
    llvm::BasicBlock* ElseBB = llvm::BasicBlock::Create(TheContext, "else");
    llvm::BasicBlock* MergeBB = llvm::BasicBlock::Create(TheContext, "ifcont");

    Builder.CreateCondBr(CondV, ThenBB, ElseBB);

    // Emit then block
    Builder.SetInsertPoint(ThenBB);
    llvm::Value* ThenV = Then->codegen();
    if (!ThenV) return nullptr;
    Builder.CreateBr(MergeBB);
    
    // Emit else block
    TheFunction->insert(TheFunction->end(), ElseBB);
    Builder.SetInsertPoint(ElseBB);
    llvm::Value* ElseV = Else->codegen();
    if (!ElseV) return nullptr;
    Builder.CreateBr(MergeBB);

    // Emit merge block
    TheFunction->insert(TheFunction->end(), MergeBB);
    Builder.SetInsertPoint(MergeBB);

    return llvm::ConstantFP::get(TheContext, llvm::APFloat(0.0));
}

llvm::Value* WhileExprAST::codegen() {
    llvm::Function* TheFunction = Builder.GetInsertBlock()->getParent();

    llvm::BasicBlock* LoopCondBB = llvm::BasicBlock::Create(TheContext, "loopcond", TheFunction);
    llvm::BasicBlock* LoopBodyBB = llvm::BasicBlock::Create(TheContext, "loopbody");
    llvm::BasicBlock* AfterLoopBB = llvm::BasicBlock::Create(TheContext, "afterloop");

    Builder.CreateBr(LoopCondBB);
    Builder.SetInsertPoint(LoopCondBB);

    llvm::Value* CondV = Cond->codegen();
    if (!CondV) return nullptr;
    CondV = Builder.CreateFCmpONE(CondV, llvm::ConstantFP::get(TheContext, llvm::APFloat(0.0)), "loopcond");
    Builder.CreateCondBr(CondV, LoopBodyBB, AfterLoopBB);

    TheFunction->insert(TheFunction->end(), LoopBodyBB);
    Builder.SetInsertPoint(LoopBodyBB);
    if (!Body->codegen()) return nullptr;
    Builder.CreateBr(LoopCondBB);

    TheFunction->insert(TheFunction->end(), AfterLoopBB);
    Builder.SetInsertPoint(AfterLoopBB);

    return llvm::ConstantFP::get(TheContext, llvm::APFloat(0.0));
}

llvm::Value* ForExprAST::codegen() {
    llvm::Function* TheFunction = Builder.GetInsertBlock()->getParent();

    llvm::Value* StartVal = Start->codegen();
    if (!StartVal) return nullptr;

    llvm::AllocaInst* VarAlloca = Builder.CreateAlloca(llvm::Type::getDoubleTy(TheContext), 0, VarName.c_str());
    Builder.CreateStore(StartVal, VarAlloca);
    NamedValues[VarName] = VarAlloca;

    llvm::BasicBlock* LoopBB = llvm::BasicBlock::Create(TheContext, "loop", TheFunction);
    llvm::BasicBlock* AfterLoopBB = llvm::BasicBlock::Create(TheContext, "afterloop");

    Builder.CreateBr(LoopBB);
    Builder.SetInsertPoint(LoopBB);

    llvm::Value* OldVal = NamedValues[VarName];
    if (!Body->codegen()) return nullptr;

    llvm::Value* StepVal = Step ? Step->codegen() : llvm::ConstantFP::get(TheContext, llvm::APFloat(1.0));
    llvm::Value* CurVal = Builder.CreateLoad(llvm::Type::getDoubleTy(TheContext), VarAlloca, VarName.c_str());
    llvm::Value* NextVal = Builder.CreateFAdd(CurVal, StepVal, "nextvar");
    Builder.CreateStore(NextVal, VarAlloca);

    llvm::Value* EndVal = End->codegen();
    if (!EndVal) return nullptr;
    EndVal = Builder.CreateFCmpULT(NextVal, EndVal, "loopcond");
    Builder.CreateCondBr(EndVal, LoopBB, AfterLoopBB);

    Builder.SetInsertPoint(AfterLoopBB);
    if (OldVal) NamedValues[VarName] = OldVal;
    return llvm::ConstantFP::get(TheContext, llvm::APFloat(0.0));
}
