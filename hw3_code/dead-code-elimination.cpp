// ECE 5984 S20 Assignment 3: dead-code-elimination.cpp
// Group: nagasanjana, balvansh
////////////////////////////////////////////////////////////////////////////////
//#include "available-support.h"

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/IntrinsicInst.h"

#include "available-support.h"
#include "dataflow.h"
using namespace llvm;
using namespace std;
namespace {

  std::map<void*, int> domainToIndex;
  vector<void*> domain;

  class DCE : public FunctionPass {
  public:
    static char ID;

    DCE() : FunctionPass(ID) {
      /* direction: true - forward; false - backward
       *meetOperator: true - Union; false - Intersection
       */
      bool direction = false;
      bool meet_op = false;

      dcePass = DCEAnalysis(direction, meet_op);

      //domPass = DominatorsAnalysis(direction, meet_op);
    }

    virtual void getAnalysisUsage(AnalysisUsage& AU) const {
      AU.setPreservesAll();
    }

  private:
    class DCEAnalysis : public DataFlowAnalysis {
    public:
      DCEAnalysis() : DataFlowAnalysis(false, false) {     }
      DCEAnalysis(bool direction, bool meet_op) : DataFlowAnalysis(direction, meet_op){}

      void printbitVector(BitVector bitvector) {
        for (unsigned i = 0; i < bitvector.size(); i++) {
          if (bitvector[i])
            errs() << "1 ";
          else
            errs() << "0 ";
        }
        errs() << "\n";
      }

      BBResult transferFunction(BitVector input, BasicBlock * BB, std::vector<void*> domain) {
        int domain_size = domain.size();
        BitVector tmp(domain_size,false);
        BBResult bbresult;
        bbresult.outputValue = tmp;
        BitVector in(domain_size,false);
        in = input;

        for (BasicBlock::reverse_iterator insn = BB->rbegin(); insn != BB->rend(); ++insn) {
            BitVector GenSet(domain_size);
            BitVector KillSet(domain_size);

            Instruction *I = &(*insn);

          //PhiNodes
          if(PHINode *phi_insn = dyn_cast<PHINode>(&*I)){

            for (int ind = 0; ind < phi_insn->getNumIncomingValues(); ind++) {

                Value* val = phi_insn->getIncomingValue(ind);

                if (isa<Instruction>(val)) {
                    BasicBlock* valBlock = phi_insn->getIncomingBlock(ind);

                    // neighborVals has no mapping for this block, then create one
                    if (bbresult.neighbors.find(valBlock) == bbresult.neighbors.end())
                        bbresult.neighbors[valBlock] = BitVector(domain_size);

                    int valInd = domainToIndex[(void*)val];
                    bbresult.neighbors[valBlock].set(valInd);
                }
            }
          } else {
            //ConstKill
            if(isa<TerminatorInst>(I) ||  isa<DbgInfoIntrinsic>(I) ||
               isa<LandingPadInst>(I) ||  I->mayHaveSideEffects())
            {
                //errs()<<"TerminatorInst: "<<&(*I)<<"\n";
                std::map<void*, int>::iterator iter = domainToIndex.find((void*)I);

                if(iter != domainToIndex.end())
                {
                    int valInd = domainToIndex[(void*)I];
                    KillSet.set(valInd);
                    //in.set(valInd);
                }
                //kill the operands also
                for (User::op_iterator opnd = insn->op_begin(), opE = insn->op_end(); opnd != opE; ++opnd) {
                    Value* val = *opnd;

                    if (isa<Instruction>(val)) {
                        int valInd = domainToIndex[(void*)val];
                        KillSet.set(valInd);
                    }
                }

            } else {
              //DeptKill
              std::map<void*, int>::iterator iter = domainToIndex.find((void*)(&(*insn)));

              if(iter != domainToIndex.end())
              {
                  int valInd = domainToIndex[(void*)(&(*insn))];
                  if(in[valInd] == false){
                    for (User::op_iterator opnd = insn->op_begin(), opE = insn->op_end(); opnd != opE; ++opnd) {
                        Value* val = *opnd;

                        if (isa<Instruction>(val)) {
                            int valInd = domainToIndex[(void*)val];
                            KillSet.set(valInd);
                        }
                    }
                  }

              //ConstGen
              if (iter != domainToIndex.end()){
                if(in[valInd] == true){
                  GenSet.set((*iter).second);
                }
              }
            }
            }
          }
          bbresult.outputValue = KillSet;
          bbresult.outputValue.flip();
          bbresult.outputValue &= in;
          bbresult.outputValue |= GenSet;

          in = bbresult.outputValue;
        }

      return bbresult;
    }
    };

    DCEAnalysis dcePass;


    virtual bool runOnFunction(Function& F) {

      // Here's some code to familarize you with the Expression
      // class and pretty printing code we've provided:

      result output;
      std::vector<Expression> expressions;

      for(inst_iterator II = inst_begin(F), IE = inst_end(F); II!=IE; ++II) {
          Instruction& insn(*II);

          if(std::find(domain.begin(),domain.end(),(&(*II))) == domain.end()){
            //errs()<<"domain: "<<*((Value*)&*II)<<"\n";
            domain.push_back((void*)(&(*II)));
          }

      }

	    // errs()<<"size: "<<domain.size()<<"\n";
      // for(void* element : domain)
      // {
      //     errs() << "Element : " << *((Value*) element) << "\n"; // Could also use getShortValueName((Value*) element)
      // }
      for (int i = 0; i < domain.size(); i++){
		    //errs()<<"debug: "<<domain[i]<<"\n";
        domainToIndex[(void*)domain[i]] = i;
      }
      BitVector boundCond(domain.size(), true);
      BitVector initCond(domain.size(), true);
      bool modified = false;

      output = dcePass.run(F, boundCond, initCond, domain);

      for (Function::iterator BI = F.begin(), BE = F.end(); BI != BE; ++BI) {
          BasicBlock* block = dyn_cast<BasicBlock>(&*BI);

          // liveness at OUT
          BitVector FV = output[block].inBB;
          //BitVector faintValues = liveValues.flip();
          // errs()<<"faintValues: ";
          //dcePass.printbitVector(liveValues);

          // dcePass.printbitVector(FV);
          std::vector<Instruction *> deleteSet;

          // Figure out instructions to delete
          for (BasicBlock::reverse_iterator insn = BI->rbegin(); insn != BI->rend(); ++insn) {
              Instruction* I = &(*insn);

              // Check if insn is live
              if(isa<TerminatorInst>(I) ||  isa<DbgInfoIntrinsic>(I) ||
                 isa<LandingPadInst>(I) ||  I->mayHaveSideEffects())
                   continue;

              if(std::find(domain.begin(), domain.end(), I) != domain.end())
              {
                  int valInd = domainToIndex[(void*)I];

                  if(FV[valInd] == true)
                  {
                      errs() << "Trying to delete instruction :: " <<*(Value*)&*I << "\n";
                      deleteSet.push_back(I);
                      modified = true;
                  }
              }
          }

          // do actual deletion
          for(auto I : deleteSet){
              if(!I->use_empty())
                  continue;
              errs() << "Deleting instruction :: " << *(Value*)&*I << "\n";
              I->eraseFromParent();
          }

      }
      return false;
    }

  };

  char DCE::ID = 0;
  RegisterPass<DCE> X("dead-code-elimination",
       "ECE 5984 Dead Code Elimination");
}
