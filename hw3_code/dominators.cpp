// ECE 5984 S20 Assignment 3: dominators.cpp
// Group: nagasanjana, balvansh
////////////////////////////////////////////////////////////////////////////////
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"

#include "dominators.h"
#include "dataflow.h"
using namespace llvm;
using namespace std;
namespace {
  class Dominators : public LoopPass {
  public:
    static char ID;

    Dominators() : LoopPass(ID) {

      bool direction = true;
      bool meet_op = false;

      domPass = DominatorsAnalysis(direction, meet_op);

    }

    virtual void getAnalysisUsage(AnalysisUsage& AU) const {
      AU.setPreservesAll();
      AU.addRequired<LoopInfoWrapperPass>();
    }
    // std::map<BasicBlock*, BasicBlock*> computeDom(Function* F){
    //
    //   result output;
    //   Function::BasicBlockListType &BBs = F->getBasicBlockList();
    //   for (Function::BasicBlockListType::iterator IT = BBs.begin(), IE = BBs.end(); IT != IE; ++IT){
    //     domain.push_back((void* )&(*IT));
    //   }
    //   for (int i = 0; i < domain.size(); i++){
    //     domainToIndex[i] = domain[i];
    //   }
    //
    //   BitVector boundCond(domain.size(), false);
    //   BitVector initCond(domain.size(), true);
    //
    //
    //
    //   std::map<BasicBlock*, BasicBlock*>domMap;
    //
    //   output = domPass.run(*F, boundCond, initCond);
    //
    //   for(int block2 = 0; block2 < domain.size(); ++block2 ){
    //
    //     BasicBlock* B = (BasicBlock*)domain[block2];
    //
    //     for(int block1 = 0; block1 < domain.size(); ++block1){
    //
    //       if(block1 == block2){
    //         continue;
    //       }
    //       BasicBlock* A = (BasicBlock*)domain[block1];
    //
    //       if(output[B].inBB[block1] == false)
    //         continue;
    //
    //       bool immediateDOM = true;
    //
    //       for(int block3=0; block3 < domain.size(); ++block3){
    //
    //         if (block3 == block1 || block3 == block2)
    //             continue;
    //         BasicBlock* C = (BasicBlock*)domain[block3];
    //         if (output[C].inBB[block1] && output[B].inBB[block3]) {
    //             immediateDOM = false;
    //             break;
    //         }
    //       }
    //
    //       if (immediateDOM) {
    //           domMap[B] = A;
    //       }
    //     }
    //
    //   }
    //
    //   return domMap;
    //
    // }
    //
    // void printIdom(std::map<BasicBlock*, BasicBlock*> domMap, Loop* L) {
    //     std::vector<BasicBlock*> BB = L->getBlocks();
    //     errs() << "\n\nNew Loop\n";
    //     for (int i=0; i < BB.size(); ++i) {
    //         errs() << domMap[BB[i]]->getName() << " idom " << BB[i]->getName() << "\n";
    //     }
    //     return;
    // }


  private:

    void printbitVector(BitVector bitvector) {
      for (unsigned i = 0; i < bitvector.size(); i++) {
        if (bitvector[i])
          errs() << "1 ";
        else
          errs() << "0 ";
      }
      errs() << "\n";
    }

    DominatorsAnalysis domPass;

    virtual bool runOnLoop(Loop* L, LPPassManager &no_use){

      result output;
      map<BasicBlock*, BasicBlock*> finalDominance;
      std::vector<void*> domain;

      BasicBlock* preheader = L->getLoopPreheader();
			if (!preheader) {
				return false;
			}

      LoopInfo& LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();


      Function* F = L->getBlocks().front()->getParent();

        for (Function::iterator FI = F->begin(), FE = F->end(); FI != FE; ++FI) {
          BasicBlock* block = &*FI;
          domain.push_back(block);
        }

      finalDominance = domPass.computeDom(F, domain);

      domPass.printIdom(finalDominance,L);

      // BitVector boundCond(domain.size(), false);
      // BitVector initCond(domain.size(), true);
      //
      //
      // std::map<BasicBlock*, BasicBlock*>domMap;
      //
      // output = domPass.run(*F, boundCond, initCond);
      //
      // std::vector<BasicBlock*> domain1(domainToIndex.size());  // Reconstruct index -> BasicBlock* mapping from domainToIndex
      // for (std::map<int,void*>::iterator it = domainToIndex.begin(); it != domainToIndex.end(); ++it ) {
      //     domain1[it->first] = (BasicBlock*)it->second;
      // }
      //
      // for(int block2 = 0; block2 < domain1.size(); ++block2 ){
      //
      //   BasicBlock* B = domain1[block2];
      //
      //   for(int block1 = 0; block1 < domain1.size(); ++block1){
      //
      //     if(block1 == block2){
      //       continue;
      //     }
      //     BasicBlock* A = domain1[block1];
      //
      //     if(output[B].inBB[block1] == false)
      //       continue;
      //
      //     bool immediateDOM = true;
      //
      //     for(int block3=0; block3 < domain1.size(); ++block3){
      //
      //       if (block3 == block1 || block3 == block2)
      //           continue;
      //       BasicBlock* C = domain1[block3];
      //       if (output[C].inBB[block1] && output[B].inBB[block3]) {
      //           immediateDOM = false;
      //           break;
      //       }
      //     }
      //
      //     if (immediateDOM) {
      //         if(domMap.find(B) != domMap.end()) {
      //             // errs() << "IDOM ALREADY PRESENT!! " << B->getName() << " idom " << domMap.find(B)->second->getName() << "\n";
      //             // errs() << "NOW ADDING!! " << B->getName() << " idom " << A->getName() << "\n";
      //             // //assert(0);
      //         }
      //         domMap[B] = A;
      //         // break;
      //     }
      //   }
      //
      // }
      //
      // printIdom(domMap,L);
      return false;
    }

    // virtual bool runOnFunction(Function& F) {
    //
    //   result output;
    //
    //   for (Function::iterator FI = F.begin(), FE = F.end(); FI != FE; ++FI) {
    //     BasicBlock* block = &*FI;
    //     domain.push_back(block);
    //   }
    //
	  //   outs()<<"size: "<<domain.size()<<"\n";
    //   for (int i = 0; i < domain.size(); i++){
		//     //errs()<<"debug: "<<domain[i]<<"\n";
    //     domainToIndex[i] = domain[i];
    //   }
    //   BitVector boundCond(domain.size(), false);
    //   BitVector initCond(domain.size(), true);
    //
    //   output = domPass.run(F, boundCond, initCond);
    //
    //   return false;
    // }

  };






  char Dominators::ID = 0;
  RegisterPass<Dominators> X("dominators",
       "ECE 5984 Dominators");
}
