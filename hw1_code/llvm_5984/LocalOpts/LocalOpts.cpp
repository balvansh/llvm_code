// ECE-5984 S20 Assignment 1: LocalOpts.cpp
// PID:balvansh
////////////////////////////////////////////////////////////////////////////////

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"

#include <iostream>

using namespace llvm;

namespace {

  struct localOpts{
  	localOpts(): algebraicId(0), constantFold(0), strengthReduce(0){};
	int algebraicId;
	int constantFold;
	int strengthReduce;
  }localOptsData;	  
  class LocalOpts : public FunctionPass {
  public:
    static char ID;
    LocalOpts() : FunctionPass(ID) { }
    ~LocalOpts() { }

    // We don't modify the program, so we preserve all analyses
    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.setPreservesAll();
    }

    // Do some initialization
    bool doInitialization(Module &M) override {
      errs() << "5984 Local Optimizations Pass\n";
      //outs() << "Name,\tArgs,\tCalls,\tBlocks,\tInsns,\tAdd/Sub,\tMul/Div,\tBr(Cond),\tBr(UnCond)\n";

      return false;
    }

    int64_t logValue(int64_t power_value){
    	int64_t log_value=0;
	if(power_value == 0)
		return 0;
	while(((power_value &1) == 0) && power_value>1){
		power_value>>=1;
		log_value++;
	}
	return log_value;
    }

    int64_t requiredLog(int64_t power_value, int64_t &exp){
	    if(power_value <= 0)
		    return 0;
	    if((exp=logValue(power_value)) >= 0)
		    return 0;
	    else if((exp=logValue(power_value-1)) >= 0)
		    return 1;
	    else if((exp=logValue(power_value+1)) >= 0)
		    return 2;
	    else 
		    return -1;
    }

    int algebraicOptimization(BasicBlock &BB){
	    for(BasicBlock::iterator b=BB.begin(), e=BB.end(); b!=e; ++b){
		    Instruction& I(*b);
		    //outs() <<I.getOperand(0)<<"\n";
		    if(BinaryOperator *op = dyn_cast<BinaryOperator>(&I)){
			    //outs() <<"Binary!\n";
			    Value *v1(op->getOperand(0));
			    Value *v2(op->getOperand(1));
			    //outs()<< v1<<"  "<< v2<<"\n";
			    IntegerType *itype = dyn_cast<IntegerType>(b->getType());
			    //outs() << op->getOpcode() <<"\n";
			    ConstantInt *value1, *value2;
			    ConstantInt *const_zero = ConstantInt::get(itype, 0, false);
			    ConstantInt *const_one = ConstantInt::get(itype, 1, false);
			    //outs() <<"Constants: "<<const_zero<<" "<<const_one<<"\n";
			    switch(op->getOpcode()){

				    case Instruction::Add:
					    //outs() <<"Add\n";
					    if(ConstantInt::classof(v1)){
					    	value1 = dyn_cast<ConstantInt>(v1);
						if(value1->isZero()){
							//outs() <<"0+x\n";
							ReplaceInstWithValue(BB.getInstList(), b, v2);
							localOptsData.algebraicId++;
						}

					    }
					    else if (ConstantInt::classof(v2)){
					    	value2 = dyn_cast<ConstantInt>(v2);
						if(value2->isZero()){
							//outs()<< "x+0\n";
							ReplaceInstWithValue(BB.getInstList(), b, v1);
							localOptsData.algebraicId++;
						}
					    }
					    break;
				    case Instruction::Sub:
					    //outs() <<"Sub\n";
					    if(ConstantInt::classof(v2)){
					    	value1 = dyn_cast<ConstantInt>(v1);
						value2 = dyn_cast<ConstantInt>(v2);
						if(value1 == value2){
							//outs()<< "x-0\n";
							ReplaceInstWithValue(BB.getInstList(), b, const_zero);
							localOptsData.algebraicId++;
						} else if(value2->isZero()){
							//outs()<< "x-0\n";
							ReplaceInstWithValue(BB.getInstList(), b, v1);
							localOptsData.algebraicId++;
						}
					    }
					    else{
					    	if(value1 == value2){
							//outs()<< "x-x\n";
							ReplaceInstWithValue(BB.getInstList(), b, const_zero);
							localOptsData.algebraicId++;
						}
					    }
					    break;
				    case Instruction::Mul:
					    //outs() <<"Mul\n";
					    value1 = dyn_cast<ConstantInt>(v1);
					    value2 = dyn_cast<ConstantInt>(v2);
					    if(ConstantInt::classof(v1)){
					    	if(value1->isOne()){
							//outs()<< "1*x\n";
							ReplaceInstWithValue(BB.getInstList(), b, v2);
							localOptsData.algebraicId++;
						} else if(value1->isZero()){
							//outs()<< "0*x\n";
							ReplaceInstWithValue(BB.getInstList(), b, const_zero);
							localOptsData.algebraicId++;
						}
					    } else if(ConstantInt::classof(v2)){
					    	if(value2->isOne()){
							//outs()<< "x*1\n";
							ReplaceInstWithValue(BB.getInstList(), b, v1);
							localOptsData.algebraicId++;
						} else if(value2->isZero()){
							//outs()<< "x*0\n";
							ReplaceInstWithValue(BB.getInstList(), b, const_zero);
							localOptsData.algebraicId++;
						}
					    }
					    break;
			            case Instruction::UDiv:
			    	    case Instruction::SDiv:
					    //outs() <<"Div\n";
					    value1 = dyn_cast<ConstantInt>(v1);
					    value2 = dyn_cast<ConstantInt>(v2);
					    if(ConstantInt::classof(v2)){
					    	if(value2->isOne()){
							//outs()<< "x/1\n";
							ReplaceInstWithValue(BB.getInstList(), b, v1);
							localOptsData.algebraicId++;
						}
					    } else if (ConstantInt::classof(v1)){
					    	if(value1->isZero()){
							//outs()<< "0/x\n";
							ReplaceInstWithValue(BB.getInstList(), b, const_zero);
							localOptsData.algebraicId++;
						}
					    } else {
					    	if(value1 == value2){
							//outs()<< "x/x";
							ReplaceInstWithValue(BB.getInstList(), b, const_one);
							localOptsData.algebraicId++;
						}
					    }
					    break;
				    default:
					    outs() <<"Ignore\n";		    
			    }
		    }
	    }
	    return 0;
    }

    int constantFolding(BasicBlock &BB){
	    for(BasicBlock::iterator b=BB.begin(), e=BB.end(); b!=e; ++b){
		    //outs() <<"Entered BB: \n";
                    Instruction& I(*b);
		    if(BinaryOperator *op = dyn_cast<BinaryOperator>(&I)){
                            //outs() <<"Binary!\n";
                            Value *v1(op->getOperand(0));
                            Value *v2(op->getOperand(1));
                            //outs()<< v1<<"  "<< v2<<"\n";
                            IntegerType *itype = dyn_cast<IntegerType>(b->getType());
			    ConstantInt *value1, *value2, *final_value;
                            ConstantInt *const_zero = ConstantInt::get(itype, 0, false);
			    //outs() <<"Constant Folding: \n"<<op->getOpcode();
			    switch(op->getOpcode()){
				    case Instruction::Add:
					    //outs() <<"Add\n";
					    if(ConstantInt::classof(v1) && ConstantInt::classof(v2)){
						value1 = dyn_cast<ConstantInt>(v1);
                                            	value2 = dyn_cast<ConstantInt>(v2);
						final_value = ConstantInt::get(value1->getType(), (value1->getSExtValue() + value2->getSExtValue()));
					    	ReplaceInstWithValue(BB.getInstList(), b, final_value);
					    	localOptsData.constantFold++;
					    }
					    break;
				    case Instruction::Sub:
					    //outs() <<"Sub\n";
					    if(ConstantInt::classof(v1) && ConstantInt::classof(v2)){
						value1 = dyn_cast<ConstantInt>(v1);
                                                value2 = dyn_cast<ConstantInt>(v2);
                                                final_value = ConstantInt::get(value1->getType(), (value1->getSExtValue() - value2->getSExtValue()));
						ReplaceInstWithValue(BB.getInstList(), b, final_value);
						localOptsData.constantFold++;
					    }
					    break;
			   	    case Instruction::Mul:
					    //outs() <<"Mul\n";
					    if(ConstantInt::classof(v1) && ConstantInt::classof(v2)){
						value1 = dyn_cast<ConstantInt>(v1);
                                                value2 = dyn_cast<ConstantInt>(v2);
                                                final_value = ConstantInt::get(value1->getType(), (value1->getSExtValue() * value2->getSExtValue()));
						ReplaceInstWithValue(BB.getInstList(), b, final_value);
						localOptsData.constantFold++;
					    }
					    break;
				    case Instruction::UDiv:
				    case Instruction::SDiv:
					    //outs() <<"Div\n";
					    if(ConstantInt::classof(v1) && ConstantInt::classof(v2)){
					    	if(!value1->isZero()){
							value1 = dyn_cast<ConstantInt>(v1);
        	                                        value2 = dyn_cast<ConstantInt>(v2);
	                                                final_value = ConstantInt::get(value1->getType(), (value1->getSExtValue() / value2->getSExtValue()));
							ReplaceInstWithValue(BB.getInstList(), b, final_value);
							localOptsData.constantFold++;
						} else {
							ReplaceInstWithValue(BB.getInstList(), b, const_zero);
							localOptsData.constantFold++;
						}
					    }
					    break;
				    default:
					    outs() <<"Ignore\n";
			    }
		    }
	    }
	    return 0;
    }

    int strengthReductions(BasicBlock &BB){
            for(BasicBlock::iterator b=BB.begin(), e=BB.end(); b!=e; ++b){
                    Instruction& I(*b);
                    if(BinaryOperator *op = dyn_cast<BinaryOperator>(&I)){
                            //outs() <<"Binary!\n";
                            Value *v1(op->getOperand(0));
                            Value *v2(op->getOperand(1));
                            //outs()<< v1<<"  "<< v2<<"\n";
                            //IntegerType *itype = dyn_cast<IntegerType>(b->getType());
			    ConstantInt *power_value, *final_value;
			    switch(op->getOpcode()){
			    	case Instruction::Mul:
					//outs() <<"Mul\n";
					if(ConstantInt::classof(v1)){
						Value *temp(v1);
						v1 = v2;
						v2 = temp;
					}
					if(ConstantInt::classof(v2)){
						power_value = dyn_cast<ConstantInt>(v2);	
						BinaryOperator *modified_inst;
						int64_t log_value = 0;
						int64_t p = requiredLog(power_value->getSExtValue(), log_value);
						final_value = ConstantInt::get(power_value->getType(), log_value);
						modified_inst = BinaryOperator::Create(Instruction::Shl, v1, final_value);
						if(p!=-1){
							if(p==0){
								ReplaceInstWithInst(BB.getInstList(), b, modified_inst);
								localOptsData.strengthReduce++;
							} else if(p == 1){
								BB.getInstList().insert(b, modified_inst);
								BinaryOperator *final_inst(BinaryOperator::Create(Instruction::Add, modified_inst, v1));
								ReplaceInstWithInst(BB.getInstList(), b, final_inst);
								localOptsData.strengthReduce++;
							} else if(p==2){
								BB.getInstList().insert(b, modified_inst);
                                                                BinaryOperator *final_inst(BinaryOperator::Create(Instruction::Sub, modified_inst, v1));
                                                                ReplaceInstWithInst(BB.getInstList(), b, final_inst);
                                                                localOptsData.strengthReduce++;							
							}
						}
					}
					break;
				case Instruction::UDiv:
				case Instruction::SDiv:
					//outs() <<"Div\n";
					if(ConstantInt::classof(v2)){
						power_value = dyn_cast<ConstantInt>(v2);
						BinaryOperator *modified_inst;
						int64_t log_value = logValue(power_value->getSExtValue());
						final_value = ConstantInt::get(power_value->getType(), log_value);
						modified_inst = BinaryOperator::Create(Instruction::LShr, v1, final_value);
						if(log_value>=0){
							ReplaceInstWithInst(BB.getInstList(), b, modified_inst);
							localOptsData.strengthReduce++;
						}
					}
					break;
				default:
					//outs() <<"Unsupported op\n";
					break;
			    }
		    }
	    }	    
	    return 0;
    }

    // Print output for each function
    bool runOnFunction(Function &F) override {
      outs() << "Inside Function: "<<F.getName()<<"\n";
      for(Function:: iterator f=F.begin(), end=F.end(); f!=end; ++f){
	      BasicBlock& BB(*f);
	      algebraicOptimization(BB);
	      constantFolding(BB);
	      strengthReductions(BB);
	      outs()<<"Algebraic Optimizations: "<< localOptsData.algebraicId <<"\n";
	      outs()<<"Constant Foldings: "<<localOptsData.constantFold << "\n";
	      outs()<<"Strength Reduction: "<<localOptsData.strengthReduce << "\n";
      }
      return false;
    }
  };
}

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char LocalOpts::ID = 0;
static RegisterPass<LocalOpts> X("local-opts", "5984: Local Optimizations", false, false);
