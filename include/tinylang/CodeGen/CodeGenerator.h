#ifndef  TINYLANG_CODEGEN_CODEGENERATOR_H
#define  TINYLANG_CODEGEN_CODEGENERATOR_H

#include "tinylang/AST/AST.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/TargetParser/Triple.h"
#include "llvm/IR/Module.h"

#include <string>

namespace tinylang{

class CodeGenerator{
    llvm::LLVMContext& Ctx;
    llvm::TargetMachine* TM;
protected:
    CodeGenerator(llvm::LLVMContext& Ctx,llvm::TargetMachine* TM): Ctx{Ctx}, TM{TM} {}
public:
    static CodeGenerator* Create(llvm::LLVMContext& Ctx, llvm::TargetMachine* TM);
    std::unique_ptr<llvm::Module> run(ModuleDeclaration* CM, std::string FileName);

};
}
#endif
