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

#include <iostream>

using namespace llvm;
using namespace polly;

bool SPGenScheduler::runOnScop(Scop &S) {
  if (S.isToBeSkipped()) {
    return false;
  }

  if (S.getSize() == 0) {
    S.markAsOptimized();
    return false;
  }

  const Dependences &D =
    getAnalysis<DependenceInfo>().getDependences(Dependences::AL_Statement);

  // FIXME when does this happen?
  if (D.getSharedIslCtx() != S.getSharedIslCtx()) {
    llvm_unreachable("DependenceInfo for another SCoP/isl_ctx");
  }

  if (!D.hasValidDependences()) {
    llvm_unreachable("SCoP has no valid dependence");
  }

  SPDIR *IR = getAnalysis<SPDIRInfo>().getIR(&S);
  if (IR) {
    IR->dump();
  }
  else {
    llvm_unreachable("cannot find an SPDIR");
  }

  std::cerr << "OLD -------------------------------------------------------\n";
  isl_schedule_dump(S.getScheduleTree().get());

  int ValidityKinds =
    Dependences::TYPE_RAW | Dependences::TYPE_WAR | Dependences::TYPE_WAW;
  int ProximityKinds =
    Dependences::TYPE_RAW | Dependences::TYPE_WAR | Dependences::TYPE_WAW;

  isl::union_set Domain = S.getDomains();
  if (!Domain) {
    llvm_unreachable("SCoP has no domain");
  }

  isl::union_map Validity = isl::manage(D.getDependences(ValidityKinds));
  isl::union_map Proximity = isl::manage(D.getDependences(ProximityKinds));

  isl_ctx *Ctx = S.getIslCtx().get();
  isl_options_set_schedule_outer_coincidence(Ctx, 0);
  isl_options_set_schedule_serialize_sccs(Ctx, 1);
  isl_options_set_schedule_maximize_band_depth(Ctx, 1);
  isl_options_set_schedule_max_constant_term(Ctx, 20);
  isl_options_set_schedule_max_coefficient(Ctx, 20);
  isl_options_set_tile_scale_tile_loops(Ctx, 0);

  auto SC = isl::schedule_constraints::on_domain(Domain);
  SC = SC.set_proximity(Proximity);
  SC = SC.set_validity(Validity);
  SC = SC.set_coincidence(Validity);

  auto Schedule = SC.compute_schedule();
  if (!Schedule) {
    llvm_unreachable("cannot compute a new schedule");
  }

  std::cerr << "\nNEW -------------------------------------------------------\n";
  Schedule.dump();

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
