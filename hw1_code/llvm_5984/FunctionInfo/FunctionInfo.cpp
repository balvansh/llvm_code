// ECE-5984 S20 Assignment 1: FunctionInfo.cpp
// PID:balvansh
////////////////////////////////////////////////////////////////////////////////

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Support/raw_ostream.h"

#include <iostream>

using namespace llvm;

namespace {
  class FunctionInfo : public FunctionPass {
  public:
    static char ID;
    FunctionInfo() : FunctionPass(ID) { }
    ~FunctionInfo() { }

    // We don't modify the program, so we preserve all analyses
    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.setPreservesAll();
    }

    // Do some initialization
    bool doInitialization(Module &M) override {
      errs() << "5984 Function Information Pass\n";
      outs() << "Name,\tArgs,\tCalls,\tBlocks,\tInsns,\tAdd/Sub,\tMul/Div,\tBr(Cond),\tBr(UnCond)\n";

      return false;
    }

    // Print output for each function
    bool runOnFunction(Function &F) override {
      unsigned count = 0, calls = 0, size=0;
      unsigned addsub = 0, muldiv = 0;
      unsigned ops = 0, brnch= 0, ubrnch= 0;
      //calls = F.getNumUses();
      
      for(Function::iterator bb=F.begin(), e=F.end();bb!=e; ++bb){
	      
	      /*Calculating the number of basic blocks*/
	      count++;

	      /*Calculating the number of instructions*/
	      size = size+bb->size();
	      
	      for(BasicBlock::iterator BBI=bb->begin(), BBE=bb->end(); BBI!=BBE; ++BBI){
		      
		      /* Getting the total number of calls*/
		      if(isa<CallInst>(&(*BBI)) ){
			calls = calls+1;	      	
		      } 

		      /*Counting the number of conditional branched and unconditional branched instructions*/
		      if (BranchInst *BI = dyn_cast<BranchInst>(BBI)){
			ops = BI->getNumOperands();
			if(ops>1){
				brnch = brnch+1;
			} else {
				ubrnch = ubrnch+1;
			}
		      }
		      
		      /*Counting the number of arithmetic instructions*/
		      switch(BBI->getOpcode()){
			      case Instruction::Add:
				      addsub++;
				      break;
			      case Instruction::Sub:
				      addsub++;
				      break;
			      case Instruction::Mul:
				      muldiv++;
				      break;
			      case Instruction::UDiv:
			      case Instruction::SDiv:
				      muldiv++;
				      break;
			      default:
				      break;
		      }
	      }
      }
      outs() <<F.getName()<< ",\t" << F.arg_size()<< ",\t"<<calls<<"\t"<<count<<"\t"<<size<<"\t"<<addsub<<"\t"<<muldiv<<"\t"<<brnch<<"\t"<<ubrnch<<"\n";
      return false;
    }
  };
}

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char FunctionInfo::ID = 0;
static RegisterPass<FunctionInfo> X("function-info", "5984: Function Information", false, false);
