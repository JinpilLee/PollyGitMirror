//===- SPGenScheduler.h - SCoP Schedule Pass for SPGen ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// SCoP Scheduler Pass for SPGen
//
//===----------------------------------------------------------------------===//

#ifndef POLLY_SPGEN_SCHEDULER
#define POLLY_SPGEN_SCHEDULER

#include "polly/ScopPass.h"
#include "isl/isl-noexceptions.h"

namespace polly {
class Scop;

class SPGenScheduler : public ScopPass {
public:
  static char ID;
  SPGenScheduler() : ScopPass(ID) {}

  bool runOnScop(Scop &S) override;
  void getAnalysisUsage(AnalysisUsage &AU) const override;
};
} // namespace polly

namespace llvm {
void initializeSPGenSchedulerPass(llvm::PassRegistry &);
} // namespace llvm

#endif // POLLY_SPGEN_SCHEDULER
