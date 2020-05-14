// ECE 5984 S20 Assignment 3: dataflow.cpp
// Group: Naga Sanjana Bikonda, Balvansh Heerekar
////////////////////////////////////////////////////////////////////////////////

#include "dataflow.h"

namespace llvm {

  // Add code for your dataflow abstraction here.

  void printbitVector(BitVector bitvector) {
    for (unsigned i = 0; i < bitvector.size(); i++) {
      if (bitvector[i])
        errs() << "1 ";
      else
        errs() << "0 ";
    }
    errs() << "\n";
  }

  result DataFlowAnalysis::run(Function &Func, BitVector boundaryValues, BitVector initialValues, std::vector<void*> domain){

    result finalResult;
    bool modified = false;

    //Boundary BasicBlock

    std::vector<BasicBlock*> bbBoundary;

    if(direction)
      bbBoundary.push_back(&Func.front());
    else{
      for (Function::iterator I = Func.begin(), E = Func.end(); I != E ; ++I ){
        if(isa<ReturnInst>(I->getTerminator()))
          bbBoundary.push_back(dyn_cast<BasicBlock>(&*I));
      }
    }

    BBResult bbBoundaryResult = BBResult();

    //Initializing Boundary BasicBlocks In and Out values

    if(direction){
      bbBoundaryResult.inBB = boundaryValues;
    }
    else{
      bbBoundaryResult.outBB = boundaryValues;
    }
    bbBoundaryResult.outputValue = boundaryValues;

    for(std::vector<BasicBlock*>::iterator I = bbBoundary.begin(), E = bbBoundary.end(); I !=E; ++I){
        finalResult[*I] = bbBoundaryResult;
    }

    //Initializing Internal BasicBlocks

    BBResult insideBlocks = BBResult();
    if(direction){
      insideBlocks.outBB = initialValues;
    } else {
      insideBlocks.inBB = initialValues;
    }

    for(Function::iterator BB = Func.begin(), BE = Func.end(); BB != BE; ++BB){
      if (find(bbBoundary.begin(), bbBoundary.end(), dyn_cast<BasicBlock>(&*BB)) == bbBoundary.end()) {
        finalResult[dyn_cast<BasicBlock>(&*BB)] = insideBlocks;

      }
    }

    //Finding the BasicBlock Neighbors
    std::map<BasicBlock*, std::vector<BasicBlock*>> blockNeighbors;

    for (Function::iterator BB = Func.begin(), BE = Func.end(); BB != BE; ++BB) {
	     std::vector<BasicBlock*> neighborList;
        if (direction) {
          for (pred_iterator neighbor = pred_begin(dyn_cast<BasicBlock>(&*BB)), E = pred_end(dyn_cast<BasicBlock>(&*BB)); neighbor != E; ++neighbor){
	      neighborList.push_back(*neighbor);
            }
        } else {
          for (succ_iterator neighbor = succ_begin(dyn_cast<BasicBlock>(&*BB)), E = succ_end(dyn_cast<BasicBlock>(&*BB)); neighbor != E; ++neighbor){
	           neighborList.push_back(*neighbor);
          }
        }
	blockNeighbors[dyn_cast<BasicBlock>(&*BB)] = neighborList;
    }

    //Finding the BB Traversal order
    std::vector<BasicBlock*> traversal;

    if (direction){
      ReversePostOrderTraversal<Function*> traverse(&Func);
      for (ReversePostOrderTraversal<Function*>::rpo_iterator I = traverse.begin(), E = traverse.end(); I != E; ++I) {
	      traversal.push_back(*I);
      }
    } else{
      for (po_iterator<BasicBlock*> I = po_begin(&Func.getEntryBlock()), E = po_end(&Func.getEntryBlock()); I != E; ++I) {
          traversal.push_back(*I);
      }
    }

    int count = 1;
    bool converge = false;
    while(!converge){
      converge = true;

      // errs()<<"\nIteration :"<< count++<<"\n\n";

      for (std::vector<BasicBlock*>::iterator BB = traversal.begin(), BE = traversal.end(); BB != BE; ++BB) {

          BBResult& bbRes = finalResult[*BB];

          BBResult prevRes = bbRes;

          BitVector prevVal;
          if(direction){
            prevVal = bbRes.outBB;
          } else {
            prevVal = bbRes.inBB;
          }

	  std::vector<BitVector> meetIn;

          for (std::vector<BasicBlock*>::iterator NI = blockNeighbors[*BB].begin(), NE = blockNeighbors[*BB].end(); NI != NE; ++NI) {
              BBResult& resultNeighbors = finalResult[*NI];
              BitVector neighborValues;
              if(direction){
                neighborValues = resultNeighbors.outBB;
              }else{
                neighborValues = resultNeighbors.inBB;
              }

              meetIn.push_back(neighborValues);
          }
          BitVector* blockInValues;
          if (direction)
            blockInValues = &bbRes.inBB;
          else
            blockInValues = &bbRes.outBB;
          if (!meetIn.empty()){
            *blockInValues = MeetOperation(meetIn);
          }else{
            *blockInValues = boundaryValues;
          }

          BitVector tmpIn = bbRes.inBB;
          if(direction){
            tmpIn = bbRes.inBB;
          }else{
            tmpIn = bbRes.outBB;
          }

          bbRes = transferFunction(*blockInValues, *BB, domain);

          BitVector* blockOutValues;
          if(direction){
            blockOutValues = &bbRes.outBB;
          } else{
            blockOutValues = &bbRes.inBB;
          }
          *blockOutValues = bbRes.outputValue;

          if(direction){
            bbRes.inBB = tmpIn;
          }else{
            bbRes.outBB = tmpIn;
          }

          if(converge){
            if(*blockOutValues != prevVal)
              converge = false;
          }
      }
    }

    return finalResult;
  }

  BitVector DataFlowAnalysis::MeetOperation(std::vector<BitVector> inputs){
    BitVector output;

    if(!inputs.empty()) {
      for (int i = 0; i < inputs.size(); i++) {
          if (i == 0) {
              output = inputs[i];
          }
          else {
              if (meetOperator) {
                  output |= inputs[i];
              }
              else {
                  output &= inputs[i];
              }
          }
      }
    }

    return output;
  }


}
