//===- SPGenScheduler.cpp - SCoP Schedule Pass for SPGen ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// SCoP Schedule Pass for SPGen
//
//===----------------------------------------------------------------------===//

#include "polly/DependenceInfo.h"
#include "polly/LinkAllPasses.h"
#include "polly/SPDIRInfo.h"
#include "polly/SPGenScheduler.h"

#define DEBUG_TYPE "polly-ast"

// FIXME
#include <iostream>

using namespace llvm;
using namespace polly;

bool SPGenScheduler::runOnScop(Scop &S) {
  if (S.isToBeSkipped())
    return false;

  SPDIR *IR = getAnalysis<SPDIRInfo>().getIR(&S);
  if (IR) {
    std::cerr << "SPDIR found:\n";
    IR->dump();
  }
  else {
    llvm_unreachable("cannot find a SPDIR");
  }

  return false;
}

void SPGenScheduler::getAnalysisUsage(AnalysisUsage &AU) const {
  ScopPass::getAnalysisUsage(AU);
  AU.addRequired<DependenceInfo>();
  AU.addRequired<SPDIRInfo>();

  AU.addPreserved<DependenceInfo>();
}

char SPGenScheduler::ID = 0;

Pass *polly::createSPGenSchedulerPass() {
  return new SPGenScheduler();
}

INITIALIZE_PASS_BEGIN(SPGenScheduler, "polly-spgenscheduler",
                      "Polly - Schedule SCoP for SPGen",
                      false, false);
INITIALIZE_PASS_DEPENDENCY(DependenceInfo);
INITIALIZE_PASS_DEPENDENCY(ScopInfoRegionPass);
INITIALIZE_PASS_DEPENDENCY(SPDIRInfo);
INITIALIZE_PASS_END(SPGenScheduler, "polly-spgenscheduler",
                    "Polly - Schedule SCoP for SPGen",
                    false, false)
