/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CCalculationTableauCompletionTaskHandleAlgorithm.h"







#ifdef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED
#define KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(a)
#define KONCLUCE_TASK_ALGORITHM_CLASH_STRING_INSTRUCTION(a)

#define KONCLUCE_TASK_ALGORITHM_TIME_MEASURE_INSTRUCTION(a)

#else

//#define KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(a) a
#define KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(a)

//#define KONCLUCE_TASK_ALGORITHM_CLASH_STRING_INSTRUCTION(a) a
#define KONCLUCE_TASK_ALGORITHM_CLASH_STRING_INSTRUCTION(a) 

#define KONCLUCE_TASK_ALGORITHM_TIME_MEASURE_INSTRUCTION(a) a
//#define KONCLUCE_TASK_ALGORITHM_TIME_MEASURE_INSTRUCTION(a)

#endif









namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CCalculationTableauCompletionTaskHandleAlgorithm::CCalculationTableauCompletionTaskHandleAlgorithm(CUnsatisfiableCacheHandler* unsatCacheHandler, CSatisfiableExpanderCacheHandler* satExpCacheHandler, CReuseCompletionGraphCacheHandler* reuseCompGraphCacheHandler, CSaturationNodeExpansionCacheHandler* satNodeExpCacheHandler, CComputedConsequencesCacheHandler* compConsCacheHandler) {
					mConceptPriorityStrategy = new CConcreteConceptProcessingOperatorPriorityStrategy();
					mIndiAncDepthMasConProcPriStr = new CIndividualAncestorDepthMaximumConceptProcessingPriorityStrategy();
					//mTaskProcessingStrategy = new CEqualDepthTaskProcessingPriorityStrategy();
					mTaskProcessingStrategy = new CEqualDepthCacheOrientatedProcessingPriorityStrategy();
					mUnsatCachRetStrategy = new CGenerativeNonDeterministicUnsatisfiableCacheRetrievalStrategy();
					mGroundingHandler = new CConceptNominalSchemaGroundingHandler();
					mDatatypeHandler = new CDatatypeIndividualProcessNodeHandler();
					mProcessingDataBox = nullptr;
					mCalcAlgContext = nullptr;

					mClashDesFactory = new CClashDescriptorFactory();
					mIndiNodeManager = new CIndividualNodeManager();
					mDependencyFactory = new CDependencyFactory();
					mSatExpCacheHandler = satExpCacheHandler;
					mUnsatCacheHandler = unsatCacheHandler;
					mSatNodeExpCacheHandler = satNodeExpCacheHandler;
					mReuseCompGraphCacheHandler = reuseCompGraphCacheHandler;
					mCompConsCacheHandler = compConsCacheHandler;

					mCompGraphCacheHandler = new CCompletionGraphCacheHandler();


					mIndividualPriorityStrategy = mIndiAncDepthMasConProcPriStr;

					mPosJumpFuncVec = &mPosTableauRuleJumpFuncVec[mRuleFuncCount/2];
					mNegJumpFuncVec = &mNegTableauRuleJumpFuncVec[mRuleFuncCount/2];

					for (cint64 i = 0; i < mRuleFuncCount; ++i) {
						mPosTableauRuleJumpFuncVec[i] = nullptr;
						mNegTableauRuleJumpFuncVec[i] = nullptr;
					}

					mConfSpecializedAutomateRules = true;
					mConfSubSetBlocking = false;
					mConfOptimizedSubSetBlocking = true;
					mConfEqualSetBlocking = false;
					mConfPairwiseEqualSetBlocking = false;
					mConfAncestorBlockingSearch = false;
					mConfAnywhereBlockingSearch = false;
					mConfAnywhereBlockingCandidateHashSearch = false;
					mConfAnywhereBlockingLinkedCandidateHashSearch = true;
					mConfSaveCoreBlockingConceptsCandidates = true;
					mConfSemanticBranching = false;
					mConfAtomicSemanticBranching = true;
					mConfBranchTriggering = true;
					mConfStrictIndiNodeProcessing = true;
					mConfIDIndiPriorization = true;
					mConfPropagateNodeProcessed = false;
					mConfDirectRulePreprocessing = true;
					mConfLazyNewNominalGeneration = true;
					mConfConsRestrictedNonStrictIndiNodeProcessing = true;
					mConfUniqueNameAssumption = false;

					mConfBuildDependencies = true;
					mConfDependencyBacktracking = true;
					mConfDependencyBackjumping = true;

					mConfWriteUnsatCaching = true;
					mConfTestOccurUnsatCached = true;
					mConfTestPrecheckUnsatCached = true;
					mConfMinimizeMerging = true;
					mConfUnsatCachingUseFullNodeDependency = false;
					mConfUnsatCachingUseNodeSignatureSet = false;

					mConfSatExpCacheRetrieval = true;
					mConfSatExpCacheConceptExpansion = true;
					mConfSatExpCacheSatisfiableBlocking = true;
					mConfSatExpCacheWriting = true;

					mConfSignatureSaving = false;
					mConfSignatureMirroringBlocking = false;

					mConfSatExpCachedDisjAbsorp = true;
					mConfSatExpCachedMergAbsorp = true;
					mConfSatExpCachedSuccAbsorp = true;

					mConfAnywhereBlockingSomeInitializationHashing = false;
					mConfAnywhereBlockingLazyExactHashing = true;

					mConfSkipANDConcepts = false;
					mConfPairwiseMerging = false;

					mConfDepthOrientatedProcessing = true;
					mConfLateBlockingResolving = true;
					mConfUnsatBranchSatisfiableCaching = true;
					mConfAtleastAtmostFastClashCheck = true;

					mOptSignatureMirroringBlockingForceSubset = false;
					mOptSignatureMirroringBlockingInBlocking = false;
					mConfIndividualReusingFromSignatureBlocking = false;

					mConfCompGraphReuseCacheRetrieval = false;
					mConfCompGraphDeterministicReuse = true;
					mConfCompGraphNonDeterministicReuse = true;

					mConfRepresentativePropagationRules = true;
					mMapComparisonDirectLookupFactor = 20;
					mConfCurrentIndividualQueuing = false;

					mConfDebuggingWriteData = false;


					mConfExpandCreatedSuccessorsFromSaturation = true;
					mConfCachingBlockingFromSaturation = true;

					mConfExactNominalDependencyTracking = true;
					mConfSaturationCachingWithNominals = true;
					mConfConceptUnsatisfiabilitySaturatedTesting = false;
					mConfSaturationConceptUnsatisfiabilitySaturatedCacheWriting = true;
					mConfSaturationSatisfiabilitiyExpansionCacheWriting = false;
					mConfSaturationCachingTestingDuringBlockingTests = true;
					mConfSaturationExpansionCacheReading = true;
					mConfSaturationIncompleteExpansionFromCache = false;

					mConfDatatypeReasoning = true;
					mConfCompletionGraphCaching = true;


					mConfAddCachedComputedConsequences = true;
					mConfCacheComputedConsequences = true;

					mIndiAncDepthMasConProcPriStr->configureStrategy(mConfStrictIndiNodeProcessing,mConfIDIndiPriorization);

					mPosJumpFuncVec[CCTOP] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCBOTTOM] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyBOTTOMRule;
					mPosJumpFuncVec[CCAND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCAQAND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCIMPLAQAND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCBRANCHAQAND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyANDRule;
					if (mConfSpecializedAutomateRules) {
						mPosJumpFuncVec[CCAQAND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyAutomatANDRule;
						mPosJumpFuncVec[CCIMPLAQAND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyAutomatANDRule;
						mPosJumpFuncVec[CCBRANCHAQAND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyAutomatANDRule;
					}

					mPosJumpFuncVec[CCDATATYPE] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyDATATYPERule;
					mNegJumpFuncVec[CCDATATYPE] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyDATATYPERule;
					mPosJumpFuncVec[CCDATALITERAL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyDATALITERALRule;
					mNegJumpFuncVec[CCDATALITERAL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyDATALITERALRule;
					mPosJumpFuncVec[CCDATARESTRICTION] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyDATARESTRICTIONRule;
					mNegJumpFuncVec[CCDATARESTRICTION] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyDATARESTRICTIONRule;

					mPosJumpFuncVec[CCSUB] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCEQ] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCOR] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyORRule;
					mPosJumpFuncVec[CCALL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyALLRule;
					mPosJumpFuncVec[CCAQALL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyALLRule;
					mPosJumpFuncVec[CCIMPLAQALL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyALLRule;
					mPosJumpFuncVec[CCBRANCHAQALL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyALLRule;
					mPosJumpFuncVec[CCSOME] = &CCalculationTableauCompletionTaskHandleAlgorithm::applySOMERule;
					mPosJumpFuncVec[CCAQSOME] = &CCalculationTableauCompletionTaskHandleAlgorithm::applySOMERule;
					mPosJumpFuncVec[CCAQCHOOCE] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyAutomatChooseRule;
					mPosJumpFuncVec[CCNOT] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyNegANDRule;
					mPosJumpFuncVec[CCSELF] = &CCalculationTableauCompletionTaskHandleAlgorithm::applySELFRule;
					mPosJumpFuncVec[CCATLEAST] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyATLEASTRule;
					mPosJumpFuncVec[CCATMOST] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyATMOSTRule;
					mPosJumpFuncVec[CCNOMINAL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyNOMINALRule;
					mPosJumpFuncVec[CCVALUE] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyVALUERule;

					mPosJumpFuncVec[CCIMPL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyIMPLICATIONRule;
					mPosJumpFuncVec[CCIMPLTRIG] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCBRANCHTRIG] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCIMPLALL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyALLRule;
					mPosJumpFuncVec[CCBRANCHALL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyALLRule;

					mNegJumpFuncVec[CCAND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyORRule;
					mNegJumpFuncVec[CCOR] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyANDRule;
					mNegJumpFuncVec[CCEQ] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyORRule;
					mNegJumpFuncVec[CCALL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applySOMERule;
					mNegJumpFuncVec[CCNOT] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyNegANDRule;
					mNegJumpFuncVec[CCSOME] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyALLRule;
					mNegJumpFuncVec[CCAQCHOOCE] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyAutomatChooseRule;
					mNegJumpFuncVec[CCSELF] = &CCalculationTableauCompletionTaskHandleAlgorithm::applySELFRule;
					mNegJumpFuncVec[CCATMOST] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyATLEASTRule;
					mNegJumpFuncVec[CCATLEAST] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyATMOSTRule;
					mNegJumpFuncVec[CCNOMINAL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyNOMINALRule;
					mNegJumpFuncVec[CCVALUE] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyVALUERule;

					mPosJumpFuncVec[CCPBINDVARIABLE] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDVARIABLERule;
					mPosJumpFuncVec[CCPBINDTRIG] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDPROPAGATEANDRule;
					mPosJumpFuncVec[CCPBINDAND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDPROPAGATEANDRule;
					mPosJumpFuncVec[CCPBINDAQAND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDPROPAGATEANDRule;
					mPosJumpFuncVec[CCPBINDIMPL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDPROPAGATEIMPLICATIONRule;
					mPosJumpFuncVec[CCPBINDALL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDPROPAGATEALLRule;
					mPosJumpFuncVec[CCPBINDAQALL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDPROPAGATEALLRule;
					mPosJumpFuncVec[CCPBINDCYCLE] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDPROPAGATECYCLERule;
					mPosJumpFuncVec[CCPBINDGROUND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDPROPAGATEGROUNDINGRule;
					mNegJumpFuncVec[CCPBINDGROUND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDPROPAGATEGROUNDINGRule;

					mPosJumpFuncVec[CCVARBINDVARIABLE] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyVARBINDVARIABLERule;
					mPosJumpFuncVec[CCVARBINDTRIG] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyVARIABLEBINDINGANDRule;
					mPosJumpFuncVec[CCVARBINDAND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyVARIABLEBINDINGANDRule;
					mPosJumpFuncVec[CCVARBINDAQAND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyVARIABLEBINDINGANDRule;
					mPosJumpFuncVec[CCVARBINDIMPL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyVARBINDPROPAGATEIMPLICATIONRule;
					mPosJumpFuncVec[CCVARBINDALL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyVARBINDPROPAGATEALLRule;
					mPosJumpFuncVec[CCVARBINDAQALL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyVARBINDPROPAGATEALLRule;
					mPosJumpFuncVec[CCVARBINDJOIN] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyVARBINDPROPAGATEJOINRule;
					mPosJumpFuncVec[CCVARBINDGROUND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyVARBINDPROPAGATEGROUNDINGRule;
					mNegJumpFuncVec[CCVARBINDGROUND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyVARBINDPROPAGATEGROUNDINGRule;

					if (mConfRepresentativePropagationRules) {
						mPosJumpFuncVec[CCVARBINDVARIABLE] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEBINDVARIABLERule;
						mPosJumpFuncVec[CCVARBINDTRIG] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEANDRule;
						mPosJumpFuncVec[CCVARBINDAND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEANDRule;
						mPosJumpFuncVec[CCVARBINDAQAND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEANDRule;
						mPosJumpFuncVec[CCVARBINDIMPL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEIMPLICATIONRule;
						mPosJumpFuncVec[CCVARBINDALL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEALLRule;
						mPosJumpFuncVec[CCVARBINDAQALL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEALLRule;
						mPosJumpFuncVec[CCVARBINDJOIN] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEJOINRule;
						mPosJumpFuncVec[CCVARBINDGROUND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEGROUNDINGRule;
						mNegJumpFuncVec[CCVARBINDGROUND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEGROUNDINGRule;
					}


					mPosJumpFuncVec[CCBACKACTIVTRIG] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDPROPAGATEANDFLAGALLRule;
					mPosJumpFuncVec[CCVARPBACKTRIG] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDPROPAGATEANDRule;
					mPosJumpFuncVec[CCVARPBACKALL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDPROPAGATEALLRule;
					mPosJumpFuncVec[CCVARPBACKAQAND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDPROPAGATEANDRule;
					mPosJumpFuncVec[CCVARPBACKAQALL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDPROPAGATEALLRule;
					mPosJumpFuncVec[CCBACKACTIVIMPL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDPROPAGATEIMPLICATIONRule;

					mLastConfig = nullptr;

					mAppliedALLRuleCount = 0;
					mAppliedSOMERuleCount = 0;
					mAppliedANDRuleCount = 0;
					mAppliedORRuleCount = 0;
					mAppliedATLEASTRuleCount = 0;
					mAppliedATMOSTRuleCount = 0;
					mAppliedTOTALRuleCount = 0;

					mStatVarBindingCreatedCount = 0;
					mStatVarBindingGroundingCount = 0;
					mStatVarBindingImplicationCount = 0;
					mStatVarBindingJoinCombinesCount = 0;
					mStatVarBindingPropagateSuccCount = 0;
					mStatVarBindingPropagateSuccFreshCount = 0;
					mStatVarBindingPropagateSuccInitialCount = 0;
					mStatVarBindingPropagateCount = 0;
					mStatVarBindingPropagateFreshCount = 0;
					mStatVarBindingPropagateInitialCount = 0;


					mStatRepresentativeCreatedCount = 0;
					mStatRepresentativeGroundingCount = 0;
					mStatRepresentativeImplicationCount = 0;
					mStatRepresentativeJoinCombinesCount = 0;
					mStatRepresentativePropagateSuccCount = 0;
					mStatRepresentativePropagateCount = 0;
					mStatRepresentativePropagateNewRepresentativeCount = 0;
					mStatRepresentativePropagateReusedRepresentativeCount = 0;
					mStatRepresentativePropagateUseRepresentativeCount = 0;
					mStatRepresentativeJoinCount = 0;
					mStatRepresentativeJoinedCount = 0;
					mStatRepresentativeJoinQuickFailCount = 0;

					mStatBackPropActivationCount = 0;

					mCurrentRecProcDepthLimit = 500;

					mMaxBlockingCachingSavedCandidateCount = 50;

					//bool debug = true;
					//if (debug) {
					//	QStringList debugStringList;
					//	QFile debugTestFile("debugStop.txt");
					//	if (debugTestFile.open(QIODevice::ReadOnly)) {
					//		while (!debugTestFile.atEnd()) {
					//			QByteArray lineArray(debugTestFile.readLine());
					//			debugStringList.append(QString(lineArray));
					//		}
					//	}
					//	foreach (const QString& debugTestString, debugStringList) {
					//		QString testString(debugTestString);
					//		testString = testString.remove("^");
					//		testString = testString.remove("0");
					//		testString = testString.remove("1");
					//		testString = testString.remove("2");
					//		testString = testString.remove("3");
					//		testString = testString.remove("4");
					//		testString = testString.remove("5");
					//		testString = testString.remove("6");
					//		testString = testString.remove("7");
					//		testString = testString.remove("8");
					//		testString = testString.remove("9");
					//		testString = testString.trimmed();

					//		mCriticalConceptSetStringSet.insert(testString);
					//	}
					//	mFoundCriticalConceptSet = false;
					//}
					//mCriticalConceptSetStringSet.insert("� := � ( ALL  http://www.owllink.org/testsuite/galen#hasState. �~http://www.owllink.org/testsuite/galen#actuallyHollow)   ^");
					//mCriticalConceptSetStringSet.insert(" :=  (~http://www.owllink.org/testsuite/galen#Topology AND � ( ALL  http://www.owllink.org/testsuite/galen#hasState. �~http://www.owllink.org/testsuite/galen#actuallyHollow)  )   ^");
					//mCriticalConceptSetStringSet.insert("� := � ( ALL  http://www.owllink.org/testsuite/galen#hasState. �~http://www.owllink.org/testsuite/galen#trulyHollow)   ^");
					//mCriticalConceptSetStringSet.insert("~http://www.owllink.org/testsuite/galen#Feature ^");
					//mCriticalConceptSetStringSet.insert("~http://www.owllink.org/testsuite/galen#StructuralFeature ^");
					//mCriticalConceptSetStringSet.insert("~http://www.owllink.org/testsuite/galen#Morphology ^");
					//mCriticalConceptSetStringSet.insert("~http://www.owllink.org/testsuite/galen#Appearance ^");
					//mCriticalConceptSetStringSet.insert("~http://www.owllink.org/testsuite/galen#Topology ^");
					//mCriticalConceptSetStringSet.insert(" :=  (~http://www.owllink.org/testsuite/galen#Topology AND � ( ALL  http://www.owllink.org/testsuite/galen#hasState. �~http://www.owllink.org/testsuite/galen#trulyHollow)  )   ^");
					//mCriticalConceptSetStringSet.insert("~http://www.owllink.org/testsuite/galen#TopCategory ^");
					//mCriticalConceptSetStringSet.insert("~http://www.owllink.org/testsuite/galen#DomainCategory ^");
					//mCriticalConceptSetStringSet.insert("~http://www.owllink.org/testsuite/galen#ModifierConcept ^");
					//mCriticalConceptSetStringSet.insert("~http://www.owllink.org/testsuite/galen#Aspect ^");
					//mCriticalConceptSetStringSet.insert("~http://www.owllink.org/testsuite/galen#Status ^");
					//mCriticalConceptSetStringSet.insert("~http://www.owllink.org/testsuite/galen#AbstractStatus ^");
					//mCriticalConceptSetStringSet.insert("~http://www.owllink.org/testsuite/galen#CountabilityStatus ^");

					//mCriticalConceptSetStringSet.insert("~http://www.owllink.org/testsuite/galen#StomachToDuodenum");


					for (cint64 i = 0; i < mDebugTaskIDVectorSize; ++i) {
						mDebugTaskIDVector[i] = 0;
					}
					mBacktrackingStep = 0;

					mFoundCriticalConceptSet = false;
					mDebug = false;
					mBacktrackDebug = true;

					mNominalMerged = false;
					mNominalMergedCount = 0;
				}

				CCalculationTableauCompletionTaskHandleAlgorithm::~CCalculationTableauCompletionTaskHandleAlgorithm() {
					delete mConceptPriorityStrategy;
				}

				void CCalculationTableauCompletionTaskHandleAlgorithm::readCalculationConfig(CSatisfiableCalculationTask* satCalcTask) {
					CCalculationConfigurationExtension *config = satCalcTask->getCalculationConfiguration();
					if (config != mLastConfig) {
						if (config) {
							mConfSpecializedAutomateRules = config->isSpecializedAutomateRuleActivated();
							mConfSubSetBlocking = config->isSubSetBlockingActivated();
							mConfOptimizedSubSetBlocking = config->isOptimizedBlockingActivated();
							mConfEqualSetBlocking = config->isEqualSetBlockingActivated();
							mConfPairwiseEqualSetBlocking = config->isPairwiseEqualSetBlockingActivated();
							mConfAncestorBlockingSearch = config->isAncestorBlockingSearchActivated();
							mConfAnywhereBlockingSearch = config->isAnywhereBlockingSearchActivated();
							mConfAnywhereBlockingCandidateHashSearch = config->isAnywhereBlockingCandidateHashSearchActivated();
							mConfSemanticBranching = config->isSemanticBranchingActivated();
							mConfAtomicSemanticBranching = config->isAtomicSemanticBranchingActivated();
							mConfBranchTriggering = config->isBranchTriggeringActivated();
							mConfStrictIndiNodeProcessing = config->isStrictIndiNodeProcessingActivated();
							mConfIDIndiPriorization = config->isIDIndiPriorizationActivated();
							mConfPropagateNodeProcessed = config->isPropagateNodeProcessedActivated();
							mConfDirectRulePreprocessing = config->isDirectRulePreprocessingActivated();
							mConfLazyNewNominalGeneration = config->isLazyNewNominalCreationActivated();
							mConfConsRestrictedNonStrictIndiNodeProcessing = config->isConsistenceRestrictedNonStictProcessingActivated();
							mConfUniqueNameAssumption = config->isUniqueNameAssumptionActivated();
							mConfDependencyBacktracking = config->isDependencyTrackingActivated();
							mConfBuildDependencies = mConfDependencyBacktracking;
							mConfDependencyBackjumping = config->isBackjumpingActivated();
							mConfWriteUnsatCaching = config->isSingleLevelUnsatisfiableCacheWritingActivated();
							mConfTestOccurUnsatCached = config->isUnsatisfiableCacheRetrievalActivated();
							mConfTestPrecheckUnsatCached = mConfTestOccurUnsatCached;
							mConfMinimizeMerging = config->isMinimizeMergingBranchesActivated();
							mConfSatExpCacheRetrieval = config->isSatisfiableExpansionCacheRetrievalActivated();
							mConfSatExpCacheConceptExpansion = config->isSatisfiableExpansionCacheConceptExpansionActivated();
							mConfSatExpCacheSatisfiableBlocking = config->isSatisfiableExpansionCacheSatisfiableBlockingActivated();
							mConfSatExpCacheWriting = config->isSatisfiableExpansionCacheWritingActivated();
							mConfSignatureSaving = config->isSignatureSavingActivated();
							mConfSignatureMirroringBlocking = config->isSignatureMirroringBlockingActivated();
							mConfUnsatCachingUseFullNodeDependency = config->isUnsatisfiableCachingFullDependencyActivated();
							mConfUnsatCachingUseNodeSignatureSet = config->isUnsatisfiableCachingFullSignatureActivated();
							mConfPairwiseMerging = config->isPairwiseMergingActivated();
							mConfCompGraphReuseCacheRetrieval = config->isCompletionGraphReuseCachingRetrievalActivated();
							mConfCompGraphDeterministicReuse = config->isCompletionGraphDeterministicReuseActivated();
							mConfCompGraphNonDeterministicReuse = config->isCompletionGraphNonDeterministicReuseActivated();
							mConfAnywhereBlockingLinkedCandidateHashSearch = config->isAnywhereBlockingCoreConceptCandidateHashSearchActivated();
							mConfRepresentativePropagationRules = config->isRepresentativePropagationActivated();
							mConfDebuggingWriteData = config->isDebuggingWriteDataActivated();
							mConfExpandCreatedSuccessorsFromSaturation = config->isSuccessorConceptSaturationExpansionActivated();
							mConfCachingBlockingFromSaturation = config->isSaturationCachingActivated();
							mConfSaturationCachingWithNominals = config->isSaturationCachingWithNominalsByReactivationActivated();
							mConfSaturationConceptUnsatisfiabilitySaturatedCacheWriting = config->isSaturationUnsatisfiabilityCacheWritingActivated();
							mConfSaturationSatisfiabilitiyExpansionCacheWriting = config->isSaturationExpansionSatisfiabilityCacheWritingActivated();
							mConfDatatypeReasoning = config->isDatatypeReasoningActivated();

							mConfCompletionGraphCaching = config->isCompletionGraphCachingActivated();

						} else {
							mConfCompletionGraphCaching = true;
							mConfSpecializedAutomateRules = true;
							mConfSubSetBlocking = false;
							mConfOptimizedSubSetBlocking = true;
							mConfEqualSetBlocking = false;
							mConfPairwiseEqualSetBlocking = false;
							mConfAncestorBlockingSearch = false;
							mConfAnywhereBlockingSearch = false;
							mConfAnywhereBlockingCandidateHashSearch = false;
							mConfAnywhereBlockingLinkedCandidateHashSearch = true;
							mConfSemanticBranching = false;
							mConfAtomicSemanticBranching = true;
							mConfBranchTriggering = true;
							mConfStrictIndiNodeProcessing = true;
							mConfIDIndiPriorization = true;
							mConfPropagateNodeProcessed = false;
							mConfDirectRulePreprocessing = true;
							mConfLazyNewNominalGeneration = true;
							mConfConsRestrictedNonStrictIndiNodeProcessing = true;
							mConfUniqueNameAssumption = false;
							mConfBuildDependencies = true;
							mConfDependencyBacktracking = true;
							mConfDependencyBackjumping = true;
							mConfWriteUnsatCaching = true;
							mConfTestOccurUnsatCached = true;
							mConfTestPrecheckUnsatCached = true;
							mConfMinimizeMerging = true;
							mConfSatExpCacheRetrieval = true;
							mConfSatExpCacheConceptExpansion = true;
							mConfSatExpCacheSatisfiableBlocking = true;
							mConfSatExpCacheWriting = true;
							mConfSignatureSaving = false;
							mConfSignatureMirroringBlocking = false;
							mConfUnsatCachingUseFullNodeDependency = false;
							mConfUnsatCachingUseNodeSignatureSet = false;
							mConfCompGraphReuseCacheRetrieval = false;
							mConfCompGraphDeterministicReuse = true;
							mConfCompGraphNonDeterministicReuse = true;
							mConfRepresentativePropagationRules = true;
							mConfDebuggingWriteData = false;
							mConfExpandCreatedSuccessorsFromSaturation = true;
							mConfCachingBlockingFromSaturation = true;
							mConfSaturationCachingWithNominals = true;
							mConfSaturationConceptUnsatisfiabilitySaturatedCacheWriting = true;
							mConfSaturationSatisfiabilitiyExpansionCacheWriting = false;
							mConfDatatypeReasoning = true;

						}
						mLastConfig = config;

						mPosJumpFuncVec[CCAQAND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyANDRule;
						if (mConfSpecializedAutomateRules) {
							mPosJumpFuncVec[CCAQAND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyAutomatANDRule;
						}


						if (mConfRepresentativePropagationRules) {
							mPosJumpFuncVec[CCVARBINDVARIABLE] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEBINDVARIABLERule;
							mPosJumpFuncVec[CCVARBINDTRIG] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEANDRule;
							mPosJumpFuncVec[CCVARBINDAND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEANDRule;
							mPosJumpFuncVec[CCVARBINDAQAND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEANDRule;
							mPosJumpFuncVec[CCVARBINDIMPL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEIMPLICATIONRule;
							mPosJumpFuncVec[CCVARBINDALL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEALLRule;
							mPosJumpFuncVec[CCVARBINDAQALL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEALLRule;
							mPosJumpFuncVec[CCVARBINDJOIN] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEJOINRule;
							mPosJumpFuncVec[CCVARBINDGROUND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEGROUNDINGRule;
							mNegJumpFuncVec[CCVARBINDGROUND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEGROUNDINGRule;
						} else {
							mPosJumpFuncVec[CCVARBINDVARIABLE] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyVARBINDVARIABLERule;
							mPosJumpFuncVec[CCVARBINDTRIG] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyVARIABLEBINDINGANDRule;
							mPosJumpFuncVec[CCVARBINDAND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyVARIABLEBINDINGANDRule;
							mPosJumpFuncVec[CCVARBINDAQAND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyVARIABLEBINDINGANDRule;
							mPosJumpFuncVec[CCVARBINDIMPL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyVARBINDPROPAGATEIMPLICATIONRule;
							mPosJumpFuncVec[CCVARBINDALL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyVARBINDPROPAGATEALLRule;
							mPosJumpFuncVec[CCVARBINDAQALL] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyVARBINDPROPAGATEALLRule;
							mPosJumpFuncVec[CCVARBINDJOIN] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyVARBINDPROPAGATEJOINRule;
							mPosJumpFuncVec[CCVARBINDGROUND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyVARBINDPROPAGATEGROUNDINGRule;
							mNegJumpFuncVec[CCVARBINDGROUND] = &CCalculationTableauCompletionTaskHandleAlgorithm::applyVARBINDPROPAGATEGROUNDINGRule;
						}

					}


					bool consPrepProcessing = satCalcTask->getConsistenceAdapter() != nullptr && satCalcTask->getTaskDepth() <= 0;
					mConfSaveCoreBlockingConceptsCandidates = mConfAnywhereBlockingLinkedCandidateHashSearch;


					mOptConsistenceNodeMarking = consPrepProcessing && mConfConsRestrictedNonStrictIndiNodeProcessing;
					mOptDetExpPreporcessing = consPrepProcessing;

					//mOptProcessedNodePropagation = mConfPropagateNodeProcessed || !mConfStrictIndiNodeProcessing;
					//mOptProcessedConsNodePropagation = mConfConsRestrictedNonStrictIndiNodeProcessing;
					//mOptProcessingBlockingTests = !mConfStrictIndiNodeProcessing;
					//mOptConsNodeProcessingBlockingTests = mConfConsRestrictedNonStrictIndiNodeProcessing;
					
					//mOptNonStrictIndiNodeProcessing = !mConfStrictIndiNodeProcessing || consPrepProcessing && mConfConsRestrictedNonStrictIndiNodeProcessing;
					//mIndiAncDepthMasConProcPriStr->configureStrategy(!mOptNonStrictIndiNodeProcessing,mConfIDIndiPriorization);
					mOptNonStrictIndiNodeProcessing = true;
				}


				CCalculationAlgorithmContextBase* CCalculationTableauCompletionTaskHandleAlgorithm::createCalculationAlgorithmContext(CTaskProcessorContext *processorContext, CProcessContext* processContext, CSatisfiableCalculationTask* satCalcTask) {
					CCalculationAlgorithmContextBase* calcAlgContext = CObjectAllocator< CCalculationAlgorithmContextBase >::allocateAndConstruct(processContext->getUsedMemoryAllocationManager());
					calcAlgContext->initTaskProcessContext(processContext,satCalcTask);
					calcAlgContext->initCalculationAlgorithmContext(processorContext,mConceptPriorityStrategy,mIndividualPriorityStrategy,mTaskProcessingStrategy,mUnsatCachRetStrategy,
								mIndiNodeManager,mClashDesFactory,mDependencyFactory,mUnsatCacheHandler,mSatExpCacheHandler,mSatNodeExpCacheHandler);
					return calcAlgContext;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::handleTask(CTaskProcessorContext *processorContext, CTask* task) {

					//lastBranchingMergingProcRest = nullptr;

					CTaskHandleMemoryAllocationManager* processorMemoryManager = processorContext->getTaskHandleMemoryAllocationManager();
					CTaskProcessorCommunicator* processorCommunicator = processorContext->getTaskProcessorCommunicator();

					CTaskHandleMemoryAllocationManager* taskHandleMemMan = processorContext->getTaskHandleMemoryAllocationManager();
					taskHandleMemMan->releaseAllMemory();

					CSatisfiableCalculationTask* satCalcTask = dynamic_cast<CSatisfiableCalculationTask*>(task);
					if (satCalcTask) {

						if (!processorCommunicator->verifyContinueTaskProcessing(satCalcTask)) {
							if (!satCalcTask->getTaskStatus()->isProcessable()) {
								processorCommunicator->communicateTaskComplete(satCalcTask);
								return false;
							} else {
								// continue later
								return true;
							}
						}

						readCalculationConfig(satCalcTask);


						mLastUnsatCacheTestedIndiNode = nullptr;

						CProcessContext* processContext = satCalcTask->getProcessContext(processorContext);
						CCalculationAlgorithmContextBase* calcAlgContext = createCalculationAlgorithmContext(processorContext,processContext,satCalcTask);
						mCalcAlgContext = calcAlgContext;
						mProcessingDataBox = satCalcTask->getProcessingDataBox();
						bool clashed = false;
						bool satisfiable = false;
						bool completed = false;
						bool paused = false;
						bool error = false;
						cint64 errorCode = 0;

						CProcessTagger* processTagger = calcAlgContext->getUsedProcessTagger();

						CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();

						CNodeSwitchHistory* nodeSwitchHistory = processingDataBox->getNodeSwitchHistory(true);

						mProcessingQueue = nullptr;
						mDepthFirstProcessingQueue = nullptr;
						mSigBlockRevSet = nullptr; 
						mReusingReviewData = nullptr; 
						mIndiImmediateProQueue = nullptr; 
						mIndiDetExpProQueue = nullptr; 
						mValueSpaceTriggeringProQueue = nullptr; 
						mValueSpaceSatCheckingQueue = nullptr; 
						mNominalProcessingQueue = nullptr; 
						mDepthProcessingQueue = nullptr; 
						mIndiReactProcessingQueue = nullptr; 
						mIndiBlockReactProQueue = nullptr; 
						mIndiSigBlockUpdProQueue = nullptr; 
						mDelayedNominalProcessingQueue = nullptr; 
						mVarBindConBatchProcessingQueue = nullptr; 
						mRoleAssertionProcessingQueue = nullptr;
						mMinConceptProcessingPriorityLevel = 0;
						mIndiNodeConcludeUnsatCaching = false;



						//CSatisfiableTaskClassificationMessageAdapter* adapter = satCalcTask->getClassificationMessageAdapter();
						//if (adapter) {
						//	CConcept* concept = adapter->getTestingConcept();
						//	if (concept && adapter->hasExtractionFlags(CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTSUBSUMERSROOTNODE)) {
						//		QString className = CIRIName::getRecentIRIName(concept->getClassNameLinker());
						//		if (className == "http://www.owllink.org/testsuite/galen#KneeJointRecessus") {
						//			bool bug = true;
						//		}
						//	}
						//}


						if (processingDataBox->isReapplicationLastConceptDesciptorOnLastIndividualNodeRequired()) {
							CIndividualProcessNode* lastProcessingIndiNode = nullptr;
							CConceptProcessDescriptor* lastProcessingConDes = nullptr;
							if (processingDataBox->getLastProcessingIndividualNodeAndConceptDescriptor(lastProcessingIndiNode,lastProcessingConDes)) {
								addCopiedConceptToProcessingQueue(lastProcessingConDes,lastProcessingIndiNode,calcAlgContext);
							}
						}



						CIndividualProcessNode* indiProcNode = takeNextProcessIndividual(calcAlgContext);
						CConceptProcessDescriptor* conProcDes = nullptr;

						mProcessRuleToTaskProcessingVerificationCount = 80;
						mRemainProcessRuleToTaskProcessingVerification = mProcessRuleToTaskProcessingVerificationCount;

						KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(mBeginTaskDebugIndiModelString = generateDebugIndiModelStringList(calcAlgContext));


						CIndividualProcessNode* lastIndiProcNode = nullptr;
						try {
							STATINC(TASKPROCESSCHANGECOUNT,mCalcAlgContext);


							while (indiProcNode) {
								lastIndiProcNode = indiProcNode;
								conProcDes = nullptr;


								// TODO: signature cached, but empty processing queue
								//CConceptProcessingQueue* prevConProcQueue = indiProcNode->getConceptProcessingQueue(false);
								//if (!prevConProcQueue || !prevConProcQueue->isEmpty()) {
								if (true) {


									processTagger->incNodeSwitchTag();
									nodeSwitchHistory->addIndividualProcessNodeSwitch(indiProcNode,processTagger->getCurrentNodeSwitchTag());
									mCalcAlgContext->setMinModificationIndividual(indiProcNode);
									STATINC(INDIVIDUALNODESWITCHCOUNT,mCalcAlgContext);

									//testCompletionGraphCachingAndBlocking(calcAlgContext);

									// initialize individual
									if (individualNodeInitializing(indiProcNode,calcAlgContext)) {


										bool continueProcessingIndividual = continueIndividualProcessing(indiProcNode,calcAlgContext);

										while (continueProcessingIndividual) {
											CConceptProcessingQueue* conProcQueue = indiProcNode->getConceptProcessingQueue(true);
											conProcDes = conProcQueue->takeNextConceptDescriptorProcess();

											processingDataBox->setLastProcessingIndividualNodeAndConceptDescriptor(indiProcNode,conProcDes);

											// process concept
											mCurrentRecProcDepth = 0;
											++mAppliedTOTALRuleCount;
											STATINC(RULEAPPLICATIONCOUNT,mCalcAlgContext);
											KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(mBeforeRuleTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext));
											KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(mBeforeRuleDebugIndiModelString = generateDebugIndiModelStringList(calcAlgContext));
											
											continueProcessingIndividual = tableauRuleProcessing(indiProcNode,conProcDes,mCalcAlgContext);
											
											KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(mAfterRuleDebugIndiModelString = generateDebugIndiModelStringList(calcAlgContext));

											if (continueProcessingIndividual) {												
												continueProcessingIndividual = continueIndividualProcessing(indiProcNode,calcAlgContext);
											} else {
												addConceptToProcessingQueue(conProcDes,conProcQueue,indiProcNode,calcAlgContext);
											}


											if (--mRemainProcessRuleToTaskProcessingVerification <= 0) {
												mRemainProcessRuleToTaskProcessingVerification = mProcessRuleToTaskProcessingVerificationCount;
												if (!processorCommunicator->verifyContinueTaskProcessing(satCalcTask)) {
													STATINC(TASKPROCESSPAUSECOUNT,mCalcAlgContext);
													paused = true;
													calcAlgContext->setCurrentIndividualNode(nullptr);
													addIndividualToProcessingQueue(indiProcNode,calcAlgContext);
													throw CCalculationStopProcessingException(false);
												}
											}


										}

										individualNodeConclusion(indiProcNode,calcAlgContext);

									}

									if (mCalcAlgContext->isMinModificationUpdated()) {
										// update history
										nodeSwitchHistory->updateLastIndividualProcessNodeSwitch(mCalcAlgContext->getMinModificationAncestorDepth(),mCalcAlgContext->getMinModificationIndividualID());
									}
								}


								indiProcNode = takeNextProcessIndividual(calcAlgContext);

							}

							//if (task->getTaskDepth() < 100) {

							//	CSatisfiableCalculationTask* tasks = createDependendBranchingTaskList(2,calcAlgContext);

							//	processorCommunicator->communicateTaskCreation(tasks);
							//	//throw CCalculationStopProcessingException(true);
							//	completed = true;
							//} else {
							//	//CClashedConceptDescriptor* clashDes = CObjectAllocator<CClashedConceptDescriptor>::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
							//	//throw clashDes;
							//	clashed = true;
							//}

							KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(mSatTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext));

 							satisfiable = true;


							//lastBranchingMergingProcRest = lastBranchingMergingProcRest;
							//if (lastBranchingMergingProcRest) {
							//	lastBranchingMergingProcRest->printIndividualContainer();
							//}

							//analyzeCompletionGraphStatistics(calcAlgContext);
							//writeCompletionGraphStatistics(calcAlgContext);

							//testCompletionGraphCachingAndBlocking(calcAlgContext);

						} catch (const CClashedConceptDescriptor*& clashConLinker) {
							clashed = true;
							KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(mClashedDebugIndiModelString = generateDebugIndiModelStringList(calcAlgContext));
						} catch (const CCalculationClashProcessingException& calcClashProcException) {
							clashed = true;
							CClashedDependencyDescriptor* clashConLinker = calcClashProcException.getClashedDependencyDescriptor();
							KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(mClashedDebugIndiModelString = generateDebugIndiModelStringList(calcAlgContext));

							//mClashedDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);

							//QFile file = QString("clashed-model.txt");
							//if (file.open(QIODevice::WriteOnly)) {
							//	file.write(mClashedDebugIndiModelString.replace("<br>","\r\n").toLocal8Bit());
							//	file.close();
							//}

							if (mConfDependencyBacktracking) {
								clashedBacktracking(clashConLinker,calcAlgContext);
							}
						} catch (const CCalculationStopProcessingException& calcStopProcException) {
							if (calcStopProcException.isTaskCompletedProcessed()) {
								completed = true;
							}
						} catch (const CCalculationErrorProcessingException& calcErrorProcException) {
							if (calcErrorProcException.hasError()) {
								error = true;
								errorCode = (cint64)calcErrorProcException.getErrorCode();
								LOG(ERROR,"::Konclude::Reasoner::Kernel::Algorithm::TableauCompletionAlgorihm",logTr("Error occured, computation stopped."),this);
							}
						} catch (const CMemoryAllocationException& memAllocException) {
							error = true;
							errorCode = 2;
						} catch (...) {
							error = true;
							errorCode = 3;
						}

						KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(mEndTaskDebugIndiModelString = generateDebugIndiModelStringList(calcAlgContext));



						bool xDebug = false;
						if (xDebug || mConfDebuggingWriteData) {

							CSatisfiableTaskClassificationMessageAdapter* adapter = satCalcTask->getClassificationMessageAdapter();
							if (adapter && satisfiable) {
								mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);

								CConcept* concept = adapter->getTestingConcept();
								if (concept && adapter->hasExtractionFlags(CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTSUBSUMERSROOTNODE)) {
									QString className = CIRIName::getRecentIRIName(concept->getClassNameLinker());
									if (className.contains("/")) {
										className = className.mid(className.lastIndexOf("/")+1);
									}
									QFile file(QString("./SATCT/%1.txt").arg(className));
									if (file.open(QIODevice::WriteOnly)) {
										file.write(mEndTaskDebugIndiModelString.replace("<br>","\r\n").toLocal8Bit());
										file.close();
									}
								}
							} else {
								if (mDebug || satisfiable /*|| satCalcTask->getTaskDepth() <= 0*/) {
									mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
									QString satStopString = QString("stopped");
									if (satisfiable) {
										satStopString = QString("satisfied");
									}
									if (clashed) {
										satStopString = QString("clashed");
									}
									QFile file(QString("./SATCT/Tasks/task-%1-%2-%3.txt").arg(satCalcTask->getTaskDepth()).arg(satCalcTask->getTaskID()).arg(satStopString));
									if (file.open(QIODevice::WriteOnly)) {
										file.write(mEndTaskDebugIndiModelString.replace("<br>","\r\n").toLocal8Bit());
										file.close();
									}
								}

							}

							//mEndTaskDebugIndiModelString = generateDebugIndiModelStringList(calcAlgContext);
						}


						//if (satisfiable) {
						//	QString modelString = generateDebugIndiModelStringList(calcAlgContext);
						//	if (modelString.indexOf("http://www.owllink.org/testsuite/galen#Haem") != -1) {
						//		QString extModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
						//		CIndividualProcessNodeVector* indiVec = mProcessingDataBox->getIndividualProcessNodeVector();
						//		cint64 indiId = 1;
						//		CIndividualProcessNode* indiNode1 = indiVec->getData(indiId);
						//		isSatisfiableCachedProcessingBlocked(indiNode1,calcAlgContext);
						//		bool bug = true;
						//	}
						//	if (modelString.indexOf("http://www.owllink.org/testsuite/galen#PulmonaryHeartDisease") != -1 && modelString.indexOf("http://www.owllink.org/testsuite/galen#LungDisease") == -1) {
						//		QString extModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
						//		CIndividualProcessNodeVector* indiVec = mProcessingDataBox->getIndividualProcessNodeVector();
						//		cint64 indiId = 1;
						//		CIndividualProcessNode* indiNode1 = indiVec->getData(indiId);
						//		isSatisfiableCachedProcessingBlocked(indiNode1,calcAlgContext);
						//		bool bug = true;
						//	}
						//	if (modelString.indexOf("http://www.owllink.org/testsuite/galen#HeartDisease") != -1) {
						//		QString extModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
						//		CIndividualProcessNodeVector* indiVec = mProcessingDataBox->getIndividualProcessNodeVector();
						//		cint64 indiId = 1;
						//		CIndividualProcessNode* indiNode1 = indiVec->getData(indiId);
						//		bool bug = true;
						//	}
						//}


						//QString modelString = generateDebugIndiModelStringList(calcAlgContext);
						//QString extModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);


						// print individual node concept set signature count statistics 
						//QList<cint64> sigCountList;
						//cint64 totalCount = 0;
						//for (QHash<cint64,cint64>::const_iterator it = mIndiNodeInitConceptSigCountHash.constBegin(), itEnd = mIndiNodeInitConceptSigCountHash.constEnd(); it != itEnd; ++it) {
						//	cint64 sigCount = it.value();
						//	sigCountList.append(sigCount);
						//	totalCount += sigCount;
						//}
						//qSort(sigCountList);
						//QListIterator<cint64> sigCountIt(sigCountList);
						//sigCountIt.toBack();
						//cint64 sigCountSize = sigCountList.size();
						//QString sigCountString(QString("\r\nnode signature counts %1, total count %2\r\n").arg(sigCountSize).arg(totalCount));
						//cint64 printCount = qMin<cint64>(sigCountSize,50);
						//for (cint64 i = 0; i < printCount; ++i) {
						//	sigCountString += QString(" n: %1, %2\t").arg(i).arg(sigCountIt.previous());
						//}
						//LOG(INFO,"::Reasoner::Kernel::Algorithm",sigCountString,this);

						//mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);

						CBooleanTaskResult* satResult = satCalcTask->getSatisfiableCalculationTaskResult();
						if (clashed) {
							if (!satResult->hasResult()) {
								satResult->installResult(false);
							}

							//writeSatisfiableCachedIndividualNodesOfUnsatisfiableBranch(mCalcAlgContext);

							completed = true;
						}
						if (satisfiable) {
							STATINC(ROOTTASKSATISFIABLECOUNT,calcAlgContext);
							satResult->installResult(true);

							if (mSatTaskConsAnalyser.analyseSatisfiableTask(satCalcTask,mCalcAlgContext)) {
								rebuildSignatureBlockingCandidateHash(mCalcAlgContext);
							}

							//testProblematicConceptSet(mCalcAlgContext);

							mSatTaskSubSumIdenAnalyser.analyseSatisfiableTask(satCalcTask,mCalcAlgContext);

							mClassMessAnalyser.readCalculationConfig(satCalcTask->getCalculationConfiguration());
							mClassMessAnalyser.analyseSatisfiableTask(satCalcTask,mCalcAlgContext);


							cacheSatisfiableIndividualNodes(mCalcAlgContext);

							completed = true;
						}
						commitCacheMessages(calcAlgContext);

						if (error) {
							if (satCalcTask->hasActiveReferencedTask()) {
								satCalcTask->clearUninitializedReferenceTasks();
							}
							satCalcTask->getTaskStatus()->setError(error,errorCode);
							processorCommunicator->communicateTaskError(satCalcTask);
							return false;
						}

						if (completed) {
							processorCommunicator->communicateTaskComplete(satCalcTask);
							return false;
						} 
						return true;

					}

					return false;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::continueIndividualProcessing(CIndividualProcessNode*& indiProcNode, CCalculationAlgorithmContextBase* calcAlgContext) {

					bool purgedIndiBlocked = indiProcNode->hasIndirectBlockedProcessingRestrictionFlags() || indiProcNode->hasPurgedBlockedProcessingRestrictionFlags();
					if (purgedIndiBlocked) {
						return false;
					}

					CConceptProcessingQueue* conProQue = indiProcNode->getConceptProcessingQueue(false);

					if (conProQue && !conProQue->isEmpty()) {
						CConceptProcessPriority conProPri;
						if (conProQue->getNextConceptProcessPriority(&conProPri)) {
							double priority = conProPri.getPriority();
							if (priority < mMinConceptProcessingPriorityLevel) {
								return false;
							}
						}
						return true;
					}
					return false;
				}





				bool CCalculationTableauCompletionTaskHandleAlgorithm::installSaturationCachingReactivation(CIndividualProcessNode* indiProcNode, CSaturationNodeAssociatedDependentNominalSet* nominalSet, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (nominalSet) {
						CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
						for (CSaturationNodeAssociatedDependentNominalSet::const_iterator it = nominalSet->constBegin(), itEnd = nominalSet->constEnd(); it != itEnd; ++it) {
							cint64 nominalID(*it);
							CIndividualProcessNode* nominalNode = getUpToDateIndividual(nominalID,calcAlgContext);
							nominalNode->setCachingLossNodeReactivationInstalled(true);
							CIndividualProcessNode* locNominalNode = getLocalizedIndividual(nominalNode,true,calcAlgContext);
							CNominalCachingLossReactivationData* locNominalReactivationData = locNominalNode->getNominalCachingLossReactivationData(true);
							locNominalReactivationData->addReactivationIndividualNode(indiProcNode);
						}
					}
					return true;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::tryInstallSaturationCachingReactivation(CIndividualProcessNode* indiProcNode, CSuccessorConnectedNominalSet* nominalSet, CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					for (CSuccessorConnectedNominalSet::const_iterator it = nominalSet->constBegin(), itEnd = nominalSet->constEnd(); it != itEnd; ++it) {
						cint64 nominalID(*it);
						CIndividualProcessNode* nominalNode = getUpToDateIndividual(nominalID,calcAlgContext);
						if (nominalNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALID | CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED)) {
							return false;
						}
						nominalNode->setCachingLossNodeReactivationInstalled(true);
						CNominalCachingLossReactivationData* nominalReactivationData = nominalNode->getNominalCachingLossReactivationData(false);
						if (nominalReactivationData && nominalReactivationData->hasReactivated()) {
							return false;
						}
						CIndividualProcessNode* locNominalNode = getLocalizedIndividual(nominalNode,true,calcAlgContext);
						CNominalCachingLossReactivationData* locNominalReactivationData = locNominalNode->getNominalCachingLossReactivationData(true);
						locNominalReactivationData->addReactivationIndividualNode(indiProcNode);
					}
					return true;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::reactivateIndividualNodesDueToNominalCachingLoss(CIndividualProcessNode* nominalProcNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool nodesReactivated = false;
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					CNominalCachingLossReactivationData* nominalReactivationData = nominalProcNode->getNominalCachingLossReactivationData(false);
					CIndividualUnsortedProcessingQueue* reactivationQueue = nullptr;
					if (nominalReactivationData) {
						if (!nominalReactivationData->hasReactivated()) {
							nominalReactivationData = nominalProcNode->getNominalCachingLossReactivationData(true);
							nominalReactivationData->setReactivated(true);
							for (CXLinker<CIndividualProcessNode*>* reactivationLinkerIt = nominalReactivationData->takeReactivationIndividualNodeLinker(); reactivationLinkerIt; reactivationLinkerIt = reactivationLinkerIt->getNext()) {
								CIndividualProcessNode* reactivationIndiNode = reactivationLinkerIt->getData();
								if (!reactivationQueue) {
									reactivationQueue = processingDataBox->getNominalCachingLossReactivationProcessingQueue(true);
								}
								reactivationQueue->insertIndiviudalProcessNode(reactivationIndiNode);
								nodesReactivated = true;
							}
						}
					}
					return nodesReactivated;
				}



				bool individualIDGreaterThan(CXLinker<CIndividualProcessNode*>*& s1, CXLinker<CIndividualProcessNode*>*& s2) {
					return s1->getData()->getIndividualID() > s2->getData()->getIndividualID();
				}


				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::takeNextProcessIndividual(CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualProcessNode* indiProcNode = nullptr;
					mIndiNodeConcludeUnsatCaching = false;

					if (!indiProcNode) {
						mMinConceptProcessingPriorityLevel = mImmediatelyProcessPriority;
						if (mProcessingDataBox->hasCacheTestingIndividualNodes()) {
							indiProcNode = mProcessingDataBox->takeNextCacheTestingIndividualNode();
							mIndiNodeConcludeUnsatCaching = true;
							mIndiNodeFromQueueType = INQT_CACHETEST;
						}
					}
					if (!indiProcNode) {
						mIndiImmediateProQueue = mProcessingDataBox->getIndividualImmediatelyProcessingQueue(false);
						if (mIndiImmediateProQueue && !mIndiImmediateProQueue->isEmpty()) {
							mIndiImmediateProQueue = mProcessingDataBox->getIndividualImmediatelyProcessingQueue(true);
							indiProcNode = mIndiImmediateProQueue->takeNextProcessIndividualNode();
							mIndiNodeFromQueueType = INQT_IMMEDIATE;
						}
					}
					if (!indiProcNode) {
						mRoleAssertionProcessingQueue = mProcessingDataBox->getRoleAssertionProcessingQueue(false);
						if (mRoleAssertionProcessingQueue && !mRoleAssertionProcessingQueue->isEmpty()) {
							mRoleAssertionProcessingQueue = mProcessingDataBox->getRoleAssertionProcessingQueue(true);
							indiProcNode = mRoleAssertionProcessingQueue->takeNextProcessIndividualNode();
							mIndiNodeFromQueueType = INQT_ROLEASS;
						}
					}
					if (!indiProcNode) {
						mMinConceptProcessingPriorityLevel = mDeterministicProcessPriority;
						mIndiDetExpProQueue = mProcessingDataBox->getIndividualDepthDeterministicExpansionPreprocessingQueue(false);
						if (mIndiDetExpProQueue && !mIndiDetExpProQueue->isEmpty()) {
							mIndiDetExpProQueue = mProcessingDataBox->getIndividualDepthDeterministicExpansionPreprocessingQueue(true);
							indiProcNode = mIndiDetExpProQueue->takeNextProcessIndividual();
							mIndiNodeFromQueueType = INQT_DETEXP;
						}
					}
					if (!indiProcNode) {
						mValueSpaceSatCheckingQueue = mProcessingDataBox->getDistinctValueSpaceSatisfiabilityCheckingQueue(false);
						if (mValueSpaceSatCheckingQueue && !mValueSpaceSatCheckingQueue->isEmpty()) {
							mValueSpaceSatCheckingQueue = mProcessingDataBox->getDistinctValueSpaceSatisfiabilityCheckingQueue(true);
							indiProcNode = mValueSpaceSatCheckingQueue->takeNextProcessIndividual();
							mIndiNodeFromQueueType = INQT_VSTSATTESTING;
							if (indiProcNode) {
								indiProcNode = getLocalizedIndividual(indiProcNode,true,calcAlgContext);
							}
						}
					}
					if (!indiProcNode) {
						mValueSpaceTriggeringProQueue = mProcessingDataBox->getValueSpaceTriggeringProcessingQueue(false);
						if (mValueSpaceTriggeringProQueue && !mValueSpaceTriggeringProQueue->isEmpty()) {
							mValueSpaceTriggeringProQueue = mProcessingDataBox->getValueSpaceTriggeringProcessingQueue(true);
							indiProcNode = mValueSpaceTriggeringProQueue->takeNextProcessIndividual();
							mIndiNodeFromQueueType = INQT_VSTRIGGERING;
							if (indiProcNode) {
								indiProcNode = getLocalizedIndividual(indiProcNode,true,calcAlgContext);
							}
						}
					}

					if (!indiProcNode) {
						mVarBindConBatchProcessingQueue = mProcessingDataBox->getVariableBindingConceptBatchProcessingQueue(false);
						if (mVarBindConBatchProcessingQueue && !mVarBindConBatchProcessingQueue->isEmpty()) {
							mVarBindConBatchProcessingQueue = mProcessingDataBox->getVariableBindingConceptBatchProcessingQueue(true);

							CConcept* varBindConcept = nullptr;
							CConceptProcessDescriptor* conProDes = nullptr;
							indiProcNode = nullptr;
							mVarBindConBatchProcessingQueue->takeNextConceptProcessIndividual(varBindConcept,indiProcNode,conProDes);
							if (indiProcNode) {
								indiProcNode = getLocalizedIndividual(indiProcNode,true,calcAlgContext);

								indiProcNode->getConceptProcessingQueue(true)->insertConceptProcessDescriptor(conProDes);

								mIndiNodeFromQueueType = INQT_VARBINDBATCHQUE;
							}
						}
					}
					if (!indiProcNode) {
						mIndiReactProcessingQueue = mProcessingDataBox->getIndividualReactivationProcessingQueue(false);
						if (mIndiReactProcessingQueue && !mIndiReactProcessingQueue->isEmpty()) {
							mIndiReactProcessingQueue = mProcessingDataBox->getIndividualReactivationProcessingQueue(true);
							CIndividualProcessNode* reactIndiNode = nullptr;
							bool forceReactivation = false;
							mIndiReactProcessingQueue->takeNextReactivationIndividual(reactIndiNode,forceReactivation);
							indiProcNode = getLocalizedIndividual(reactIndiNode,true,calcAlgContext);
							if (forceReactivation) {
								if (indiProcNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHED)) {
									indiProcNode->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHED);
									reapplySatisfiableCachedAbsorbedDisjunctionConcepts(indiProcNode,calcAlgContext);
									reapplySatisfiableCachedAbsorbedGeneratingConcepts(indiProcNode,calcAlgContext);
								}
								indiProcNode->addProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED);
							}
							mIndiNodeFromQueueType = INQT_COMPCACHEDREACT;
						}
					}
					if (!indiProcNode) {
						if (!mProcessingDataBox->hasNominalNonDeterministicProcessingNodesSorted()) {
							// sort nominal processing nodes
							CXLinker<CIndividualProcessNode*>* nomNonDetProLinker = mProcessingDataBox->getSortedNominalNonDeterministicProcessingNodeLinker();
							CPROCESSINGLIST< CXLinker<CIndividualProcessNode*>* > nomNonDetProLinkerList(calcAlgContext->getUsedTaskProcessorContext());
							while (nomNonDetProLinker) {
								CXLinker<CIndividualProcessNode*>* tmpNomNonDetProLinker = nomNonDetProLinker;
								nomNonDetProLinker = nomNonDetProLinker->getNext();
								tmpNomNonDetProLinker->clearNext();
								nomNonDetProLinkerList.append(tmpNomNonDetProLinker);
							}
							mProcessingDataBox->clearSortedNominalNonDeterministicProcessingNodeLinker();
							qSort(nomNonDetProLinkerList.begin(),nomNonDetProLinkerList.end(),individualIDGreaterThan);
							for (CPROCESSINGLIST< CXLinker<CIndividualProcessNode*>* >::const_iterator it = nomNonDetProLinkerList.constBegin(), itEnd = nomNonDetProLinkerList.constEnd(); it != itEnd; ++it) {
								CXLinker<CIndividualProcessNode*>* tmpNomNonDetProLinker(*it);
								mProcessingDataBox->addSortedNominalNonDeterministicProcessingNodeLinker(tmpNomNonDetProLinker);
							}
							mProcessingDataBox->setNominalNonDeterministicProcessingNodesSorted(true);
						}
					}

					if (!indiProcNode) {
						mMinConceptProcessingPriorityLevel = 0;
						mProcessingQueue = mProcessingDataBox->getIndividualProcessingQueue(false);
						if (mProcessingQueue && !mProcessingQueue->isEmpty()) {
							mProcessingQueue = mProcessingDataBox->getIndividualProcessingQueue(true);
							CIndividualProcessNodeDescriptor* indiProcessNodeDes = mProcessingQueue->takeNextProcessIndividualDescriptor();
							indiProcNode = indiProcessNodeDes->getIndividual();
							mIndiNodeFromQueueType = INQT_OUTDATED;
						}
					}
					if (!indiProcNode) {
						// process remaining concepts on nominal nodes
						mNominalProcessingQueue = mProcessingDataBox->getNominalProcessingQueue(false);
						if (mNominalProcessingQueue && !mNominalProcessingQueue->isEmpty()) {
							mNominalProcessingQueue = mProcessingDataBox->getNominalProcessingQueue(true);
							indiProcNode = mNominalProcessingQueue->takeNextProcessIndividual();
							mIndiNodeFromQueueType = INQT_NOMINAL;
						}
					}
					if (!indiProcNode) {
						if (mProcessingDataBox->hasSortedNominalNonDeterministicProcessingNodes()) {
							indiProcNode = mProcessingDataBox->takeSortedNominalNonDeterministicProcessingNode();
							mIndiNodeFromQueueType = INQT_NOMINAL;
						}
					}
					if (!indiProcNode) {
						// depth based processing for blockable node
						mDepthProcessingQueue = mProcessingDataBox->getIndividualDepthProcessingQueue(false);
						if (mDepthProcessingQueue && !mDepthProcessingQueue->isEmpty()) {
							mDepthProcessingQueue = mProcessingDataBox->getIndividualDepthProcessingQueue(true);
							indiProcNode = mDepthProcessingQueue->takeNextProcessIndividual();
							mIndiNodeFromQueueType = INQT_DEPTHNORMAL;
						}
					}
					if (!indiProcNode) {
						// processing of new nodes with depth first
						mNominalCachingLossReactivationProcessingQueue = mProcessingDataBox->getNominalCachingLossReactivationProcessingQueue(false);
						if (mNominalCachingLossReactivationProcessingQueue && !mNominalCachingLossReactivationProcessingQueue->isEmpty()) {
							mNominalCachingLossReactivationProcessingQueue = mProcessingDataBox->getNominalCachingLossReactivationProcessingQueue(true);
							CIndividualProcessNode* tmpIndiProcNode = mNominalCachingLossReactivationProcessingQueue->takeNextProcessIndividualNode();
							tmpIndiProcNode = getUpToDateIndividual(tmpIndiProcNode,calcAlgContext);
							if (tmpIndiProcNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONBLOCKINGCACHED)) {
								indiProcNode = getLocalizedIndividual(tmpIndiProcNode,false,calcAlgContext);
								indiProcNode->addProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONBLOCKINGCACHEDINVALIDATED);
							}
							mIndiNodeFromQueueType = INQT_NOMINALCACHINGLOSSREACTIVATION;
						}
					}
					if (!indiProcNode) {
						// processing of new nodes with depth first
						mDepthFirstProcessingQueue = mProcessingDataBox->getIndividualDepthFirstProcessingQueue(false);
						if (mDepthFirstProcessingQueue && !mDepthFirstProcessingQueue->isEmpty()) {
							mDepthFirstProcessingQueue = mProcessingDataBox->getIndividualDepthFirstProcessingQueue(true);
							indiProcNode = mDepthFirstProcessingQueue->takeNextProcessIndividualNode();
							mIndiNodeFromQueueType = INQT_DEPTHFIRST;
						}
					}
					if (!indiProcNode) {
						mOptDetExpPreporcessing = false;
						mIndiSigBlockUpdProQueue = mProcessingDataBox->getBlockingUpdateReviewProcessingQueue(false);
						if (mIndiSigBlockUpdProQueue && !mIndiSigBlockUpdProQueue->isEmpty()) {
							mIndiSigBlockUpdProQueue = mProcessingDataBox->getBlockingUpdateReviewProcessingQueue(true);
							indiProcNode = mIndiSigBlockUpdProQueue->takeNextProcessIndividual();
							mIndiNodeFromQueueType = INQT_BLOCKUP;
						}
					}
					if (!indiProcNode) {
						mIndiBlockReactProQueue = mProcessingDataBox->getBlockedReactivationProcessingQueue(false);
						if (mIndiBlockReactProQueue && !mIndiBlockReactProQueue->isEmpty()) {
							mIndiBlockReactProQueue = mProcessingDataBox->getBlockedReactivationProcessingQueue(true);
							indiProcNode = mIndiBlockReactProQueue->takeNextProcessIndividual();
							mIndiNodeFromQueueType = INQT_BLOCKREACT;
						}
					}

					if (!indiProcNode) {
						mSigBlockRevSet = mProcessingDataBox->getSignatureBlockingReviewSet(false);
						if (mSigBlockRevSet && !mSigBlockRevSet->isEmpty()) {
							mSigBlockRevSet = mProcessingDataBox->getSignatureBlockingReviewSet(true);
							bool isNonSubSetData = true;
							CSignatureBlockingReviewData* reviewData = mSigBlockRevSet->getNonSubsetReviewData();
							if (reviewData->isEmpty()) {
								isNonSubSetData = false;
								reviewData = mSigBlockRevSet->getSubsetReviewData();
							}

							CSignatureBlockingReviewDataIterator revIt = reviewData->getIterator();

							if (revIt.hasNext()) {

								cint64 blockedIndiID = revIt.next(false);
								revIt.removeAndMoveNext();

								indiProcNode = getLocalizedIndividual(blockedIndiID,calcAlgContext);

								if (!isNonSubSetData && mConfIndividualReusingFromSignatureBlocking) {
									// try to establish individual reusing
									upgradeSignatureBlockingToIndividualReusing(indiProcNode,calcAlgContext);
								}

								CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
								CSignatureBlockingIndividualNodeConceptExpansionData* locSigBlockingData = indiProcNode->getSignatureBlockingIndividualNodeConceptExpansionData(true);
								if (!locSigBlockingData) {
									CSignatureBlockingIndividualNodeConceptExpansionData* sigBlockingData = indiProcNode->getSignatureBlockingIndividualNodeConceptExpansionData(false);
									locSigBlockingData = CObjectAllocator<CSignatureBlockingIndividualNodeConceptExpansionData>::allocateAndConstruct(taskMemMan);
									locSigBlockingData->initBlockingExpansionData(sigBlockingData);
									indiProcNode->setSignatureBlockingIndividualNodeConceptExpansionData(locSigBlockingData);
								}
								if (!locSigBlockingData->isIdenticConceptSetRequired()) {
									locSigBlockingData->setIdenticConceptSetRequired(true);
									indiProcNode->setLastSearchBlockerCandidateCount(0);
								}
							}

						}
					}
					if (!indiProcNode) {
						mReusingReviewData = calcAlgContext->getProcessingDataBox()->getReusingReviewData(false);
						if (mReusingReviewData && !mReusingReviewData->isEmpty()) {
							mReusingReviewData = calcAlgContext->getProcessingDataBox()->getReusingReviewData(false);

							while (mReusingReviewData->hasNextIndividualID() && !indiProcNode) {
								cint64 indiNodeID = mReusingReviewData->takeNextIndividualID();

								indiProcNode = getLocalizedIndividual(indiNodeID,calcAlgContext);
								CReusingIndividualNodeConceptExpansionData* reuseData = indiProcNode->getReusingIndividualNodeConceptExpansionData(false);
								if (!reuseData->isConceptSetStillSubset()) {
									removeIndividualReusing(indiProcNode,calcAlgContext);
								} else {
									indiProcNode = nullptr;
								}
							}
						}
					}
					if (!indiProcNode) {
						mDelayedNominalProcessingQueue = mProcessingDataBox->getDelayingNominalProcessingQueue(false);
						if (mDelayedNominalProcessingQueue) {
							mDelayedNominalProcessingQueue = mProcessingDataBox->getDelayingNominalProcessingQueue(true);
							indiProcNode = mDelayedNominalProcessingQueue->takeNextProcessIndividualNode();
							mIndiNodeFromQueueType = INQT_DELAYEDNOMINAL;
						}
					}

					return indiProcNode;
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::analyzeCompletionGraphStatistics(CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* processingDataBox = calcAlgContext->getProcessingDataBox();
					CIndividualProcessNodeVector* indiNodeVec = processingDataBox->getIndividualProcessNodeVector();
					cint64 indiCount = indiNodeVec->getItemCount();
					for (cint64 indiIdx = 0; indiIdx < indiCount; ++indiIdx) {
						CIndividualProcessNode* indiNode = getLocalizedIndividual(indiIdx,calcAlgContext);
						if (indiNode) {
							cint64 conSigValue = indiNode->getReapplyConceptLabelSet(false)->getConceptSignatureValue();
							mSignatureIndiNodeStatusHash.insertMulti(conSigValue,indiNode->getProcessingRestrictionFlags());

							if (!mSignatureIndiNodePredDepHash.contains(conSigValue)) {
								CReapplyConceptLabelSet* conSet = indiNode->getReapplyConceptLabelSet(false);
								if (conSet && indiNode->getIndividualAncestorDepth() > 0) {
									cint64 conFromPredCount = 0;
									CConceptDescriptor* conDesIt = conSet->getAddingSortedConceptDescriptionLinker();
									while (conDesIt) {
										if (conDesIt->getConceptTag() != 1) {
											if (isConceptFromPredecessorDependent(indiNode,conDesIt,conDesIt->getDependencyTrackPoint(),calcAlgContext)) {
												++conFromPredCount;
											}
										}
										conDesIt = conDesIt->getNext();
									}
									mSignatureIndiNodePredDepHash.insert(conSigValue,conFromPredCount);
								}
							}
						}
					}
					mIndiNodeCountMap.insert(indiCount,mIndiNodeCountMap.value(indiCount,0)+1);
					mIndiNodeCountList.append(indiCount);
				}




				bool CCalculationTableauCompletionTaskHandleAlgorithm::isConceptFromPredecessorDependent(CIndividualProcessNode*& individualNode, CConceptDescriptor* conDes, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					cint64 ancDepth = individualNode->getIndividualAncestorDepth();
					bool dependencyToAncestor = false;
					CDependencyNode* depNode = depTrackPoint->getDependencyNode();
					if (depTrackPoint->isPointingToIndependentDependencyNode()) {
						dependencyToAncestor = true;
					} else if (depNode->hasAppropriateIndividualNode()) {
						CIndividualProcessNode* appIndiNode = depNode->getAppropriateIndividualNode();
						cint64 appIndiAncDepth = appIndiNode->getIndividualAncestorDepth();
						if (appIndiAncDepth < ancDepth) {
							dependencyToAncestor = true;
						}
					} 
					return dependencyToAncestor;
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::testCompletionGraphCachingAndBlocking(CCalculationAlgorithmContextBase* calcAlgContext, CIndividualProcessNode* exceptIndividualNode) {
					CProcessingDataBox* processingDataBox = calcAlgContext->getProcessingDataBox();
					CIndividualProcessNodeVector* indiNodeVec = processingDataBox->getIndividualProcessNodeVector();
					cint64 indiCount = indiNodeVec->getItemCount();
					for (cint64 indiIdx = 0; indiIdx < indiCount; ++indiIdx) {
						CIndividualProcessNode* indiNode = getLocalizedIndividual(indiIdx,calcAlgContext);
						if (indiNode && indiNode != exceptIndividualNode) {
							CConceptProcessingQueue* conProQue = indiNode->getConceptProcessingQueue(false);
							if (conProQue) {
								if (!conProQue->isEmpty()) {
									if (!indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATISFIABLECACHED | CIndividualProcessNode::PRFANCESTORSIGNATUREBLOCKINGCACHED | CIndividualProcessNode::PRFINDIRECTBLOCKED | CIndividualProcessNode::PRFPURGEDBLOCKED | CIndividualProcessNode::PRFANCESTORSIGNATUREBLOCKINGCACHED | CIndividualProcessNode::PRFDIRECTBLOCKED) ) {
										if (!indiNode->isImmediatelyProcessingQueued() && !indiNode->isBlockedReactivationProcessingQueued() && !indiNode->isDeterministicExpandingProcessingQueued() && !indiNode->isRegularDepthProcessingQueued() && !indiNode->isProcessingQueued()) {
											mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
											QFile file(QString("caching-error.txt"));
											if (file.open(QIODevice::WriteOnly)) {
												file.write(mEndTaskDebugIndiModelString.replace("<br>","\r\n").toLocal8Bit());
												file.close();
											}
											bool bug = true;
										}
									}  
										
									if (indiNode->getIndividualID() <= calcAlgContext->getMaxCompletionGraphCachedIndividualNodeID() && indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED)) {
										if (!indiNode->isImmediatelyProcessingQueued() && !indiNode->isBlockedReactivationProcessingQueued() && !indiNode->isDeterministicExpandingProcessingQueued() && !indiNode->isRegularDepthProcessingQueued() && !indiNode->isProcessingQueued() && !calcAlgContext->getProcessingDataBox()->getIndividualReactivationProcessingQueue(false)->hasQueuedIndividual(indiNode)) {
											mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
											QFile file(QString("caching-error.txt"));
											if (file.open(QIODevice::WriteOnly)) {
												file.write(mEndTaskDebugIndiModelString.replace("<br>","\r\n").toLocal8Bit());
												file.close();
											}
											bool bug = true;
										}
									}
								}
							}
							if (indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATISFIABLECACHED)) {
								CIndividualProcessNode* ancNode = getAncestorIndividual(indiNode,calcAlgContext);
								if (!ancNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATISFIABLECACHED | CIndividualProcessNode::PRFSATISFIABLECACHED)) {
									mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
									bool bug = true;
								}
							}
						}
					}
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::writeCompletionGraphStatistics(CCalculationAlgorithmContextBase* calcAlgContext) {
					QFile nodeCountMap("nodeCountMap.txt");
					if (nodeCountMap.open(QIODevice::WriteOnly)) {
						for (QMap<cint64,cint64>::const_iterator it = mIndiNodeCountMap.constBegin(), itEnd = mIndiNodeCountMap.constEnd(); it != itEnd; ++it) {
							cint64 indiNodeCount = it.key();
							cint64 occurenceCount = it.value();
							QString writeString = QString("%1\t%2\r\n").arg(indiNodeCount).arg(occurenceCount);
							nodeCountMap.write(writeString.toLocal8Bit());
						}
						nodeCountMap.close();
					}
					QFile nodeCountList("nodeCountList.txt");
					if (nodeCountList.open(QIODevice::WriteOnly)) {
						cint64 number = 1;
						for (QList<cint64>::const_iterator it = mIndiNodeCountList.constBegin(), itEnd = mIndiNodeCountList.constEnd(); it != itEnd; ++it) {
							cint64 indiNodeCount = *it;
							QString writeString = QString("%1\t%2\r\n").arg(number++).arg(indiNodeCount);
							nodeCountList.write(writeString.toLocal8Bit());
						}
						nodeCountList.close();
					}
					QFile nodeStats("nodeStats.txt");
					if (nodeStats.open(QIODevice::WriteOnly)) {
						QString writeString;

						cint64 satCachedAncCount = 0;
						cint64 satCachedCount = 0;

						cint64 mirrBlockAncCount = 0;
						cint64 mirrBlockCount = 0;

						cint64 normalBlockCount = 0;
						cint64 indirectBlockCount = 0;

						cint64 prunedBlockCount = 0;
						cint64 elseCount = 0;
						cint64 totalCount = 0;

						for (QHash<cint64,cint64>::const_iterator it = mSignatureIndiNodeStatusHash.constBegin(), itEnd = mSignatureIndiNodeStatusHash.constEnd(); it != itEnd; ++it) {
							cint64 indiNodeSigt = it.key();
							cint64 indiNodeFlags = it.value();

							if ((indiNodeFlags & CIndividualProcessNode::PRFSATISFIABLECACHED) != 0) {
								satCachedCount++;
							} else if ((indiNodeFlags & CIndividualProcessNode::PRFANCESTORSATISFIABLECACHED) != 0) {
								satCachedAncCount++;
							} else if ((indiNodeFlags & CIndividualProcessNode::PRFSIGNATUREBLOCKINGCACHED) != 0) {
								mirrBlockCount++;
							} else if ((indiNodeFlags & CIndividualProcessNode::PRFANCESTORSIGNATUREBLOCKINGCACHED) != 0) {
								mirrBlockAncCount++;
							} else if ((indiNodeFlags & CIndividualProcessNode::PRFDIRECTBLOCKED) != 0) {
								normalBlockCount++;
							} else if ((indiNodeFlags & CIndividualProcessNode::PRFINDIRECTBLOCKED) != 0) {
								indirectBlockCount++;
							} else if ((indiNodeFlags & CIndividualProcessNode::PRFPURGEDBLOCKED) != 0) {
								prunedBlockCount++;
							} else {
								elseCount++;
							}
							++totalCount;
						}

						cint64 sig1ConFromPredDependentCount = 0;
						cint64 sig2ConFromPredDependentCount = 0;
						cint64 sig3ConFromPredDependentCount = 0;
						cint64 sig4ConFromPredDependentCount = 0;
						cint64 sig5ConFromPredDependentCount = 0;

						for (QHash<cint64,cint64>::const_iterator it = mSignatureIndiNodePredDepHash.constBegin(), itEnd = mSignatureIndiNodePredDepHash.constEnd(); it != itEnd; ++it) {
							cint64 indiNodeSigt = it.key();
							cint64 indiNodePredDepCount = it.value();
							if (indiNodePredDepCount == 1) {
								sig1ConFromPredDependentCount++;
							} else if (indiNodePredDepCount == 2) {
								sig2ConFromPredDependentCount++;
							} else if (indiNodePredDepCount == 3) {
								sig3ConFromPredDependentCount++;
							} else if (indiNodePredDepCount == 4) {
								sig4ConFromPredDependentCount++;
							} else if (indiNodePredDepCount == 5) {
								sig5ConFromPredDependentCount++;
							}
						}


						writeString = QString("total-nodes-count\t%1\r\n").arg(totalCount);
						nodeStats.write(writeString.toLocal8Bit());
						writeString = QString("different-signatures-count\t%1\r\n").arg(mSignatureIndiNodeStatusHash.uniqueKeys().count());
						nodeStats.write(writeString.toLocal8Bit());
						writeString = QString("signatures-from-1-predecessor-concept-dependent-count\t%1\r\n").arg(sig1ConFromPredDependentCount);
						nodeStats.write(writeString.toLocal8Bit());
						writeString = QString("signatures-from-2-predecessor-concepts-dependent-count\t%1\r\n").arg(sig2ConFromPredDependentCount);
						nodeStats.write(writeString.toLocal8Bit());
						writeString = QString("signatures-from-3-predecessor-concepts-dependent-count\t%1\r\n").arg(sig3ConFromPredDependentCount);
						nodeStats.write(writeString.toLocal8Bit());
						writeString = QString("signatures-from-4-predecessor-concepts-dependent-count\t%1\r\n").arg(sig4ConFromPredDependentCount);
						nodeStats.write(writeString.toLocal8Bit());
						writeString = QString("signatures-from-5-predecessor-concepts-dependent-count\t%1\r\n").arg(sig5ConFromPredDependentCount);
						nodeStats.write(writeString.toLocal8Bit());
						writeString = QString("normal-blocked-count\t%1\r\n").arg(normalBlockCount);
						nodeStats.write(writeString.toLocal8Bit());
						writeString = QString("indirect-blocked-count\t%1\r\n").arg(indirectBlockCount);
						nodeStats.write(writeString.toLocal8Bit());
						writeString = QString("satisfiable-blocked-count\t%1\r\n").arg(satCachedCount);
						nodeStats.write(writeString.toLocal8Bit());
						writeString = QString("indirect-satisfiable-blocked-count\t%1\r\n").arg(satCachedAncCount);
						nodeStats.write(writeString.toLocal8Bit());
						writeString = QString("mirroring-blocked-count\t%1\r\n").arg(mirrBlockCount);
						nodeStats.write(writeString.toLocal8Bit());
						writeString = QString("indirect-mirroring-blocked-count\t%1\r\n").arg(mirrBlockAncCount);
						nodeStats.write(writeString.toLocal8Bit());
						writeString = QString("other-node-count\t%1\r\n").arg(elseCount);
						nodeStats.write(writeString.toLocal8Bit());

						nodeStats.close();
					}
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::isIndividualNodeValidBlocker(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (individualNode->isNominalIndividual()) {
						return false;
					}
					if (individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINVALIDATEBLOCKERFLAGSCOMPINATION)) {
						return false;
					}

					if (calcAlgContext->hasCompletionGraphCachedIndividualNodes() && !individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED) && 
							individualNode->getIndividualID() <= calcAlgContext->getMaxCompletionGraphCachedIndividualNodeID()) {
						return false;
					}
					return true;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::isIndividualNodeCompletionGraphCached(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					detectIndividualNodeCompletionGraphCached(individualNode,calcAlgContext);
					return false;
				}




				bool CCalculationTableauCompletionTaskHandleAlgorithm::detectIndividualNodeCompletionGraphCached(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool indiNodeCompGraphCached = false;
					if (mCompGraphCacheHandler && mConfCompletionGraphCaching) {
						if (!individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED) && individualNode->getIndividualID() <= calcAlgContext->getMaxCompletionGraphCachedIndividualNodeID()) {
							individualNode->addProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHEDNODELOCATED);

							if (individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFRETESTCOMPLETIONGRAPHCACHEDDUEDIRECTMODIFIED)) {
								individualNode->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFRETESTCOMPLETIONGRAPHCACHEDDUEDIRECTMODIFIED);
							}

							if (!individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALID | CIndividualProcessNode::PRFINVALIDBLOCKINGORCACHING)) {
								bool conceptSetExtended = false;
								indiNodeCompGraphCached = mCompGraphCacheHandler->isIndividualNodeCompletionGraphConsistenceBlocked(individualNode,conceptSetExtended,calcAlgContext);
								if (conceptSetExtended) {
									individualNode->addProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHEDNODEEXTENDED);
								}
							}
							if (!indiNodeCompGraphCached) {
								if (individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHED)) {
									individualNode->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHED);
									reapplySatisfiableCachedAbsorbedDisjunctionConcepts(individualNode,calcAlgContext);
									reapplySatisfiableCachedAbsorbedGeneratingConcepts(individualNode,calcAlgContext);
								}
								individualNode->addProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED);

								if (individualNode->isCachingLossNodeReactivationInstalled()) {
									reactivateIndividualNodesDueToNominalCachingLoss(individualNode,calcAlgContext);
								}


								CProcessingDataBox* processingDataBox = calcAlgContext->getProcessingDataBox();
								CIndividualReactivationProcessingQueue* reactProcQueue = processingDataBox->getIndividualReactivationProcessingQueue(true);
								bool reactivatedIndis = mCompGraphCacheHandler->getReactivationIndividuals(individualNode,reactProcQueue,calcAlgContext);

							} else {
								individualNode->addProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHED);
							}
						}
					}
					return indiNodeCompGraphCached;
				}





				void CCalculationTableauCompletionTaskHandleAlgorithm::commitCacheMessages(CCalculationAlgorithmContextBase* calcAlgContext) {
					CSatisfiableExpanderCacheHandler* satExpHandler = calcAlgContext->getUsedSatisfiableExpanderCacheHandler();
					if (satExpHandler) {
						satExpHandler->commitCacheMessages(calcAlgContext);
					}
					CSaturationNodeExpansionCacheHandler* satNodeExpHandler = calcAlgContext->getUsedSaturationNodeExpansionCacheHandler();
					if (satNodeExpHandler) {
						satNodeExpHandler->commitCacheMessages(calcAlgContext);
					}
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::testIndividualNodeUnsatisfiableCached(CIndividualProcessNode* individualNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (mConfTestOccurUnsatCached) {
						cint64 conSetSize = 0;
						if (mLastUnsatCacheTestedIndiNode != individualNode && (conSetSize = individualNode->getReapplyConceptLabelSet(false)->getConceptCount()) != mLastUnsatCacheTestedIndiNodeConceptSetSize) {
							CUnsatisfiableCacheHandler* unsatCacheHandler = calcAlgContext->getUsedUnsatisfiableCacheHandler();
							CClashedDependencyDescriptor* clashDescriptors = nullptr;

							if (unsatCacheHandler) {
								KONCLUCE_TASK_ALGORITHM_TIME_MEASURE_INSTRUCTION(mUnsatCacheRetrieval.start());
								bool unsatCached = unsatCacheHandler->isIndividualNodeUnsatisfiableCached(individualNode,clashDescriptors,calcAlgContext);
								KONCLUCE_TASK_ALGORITHM_TIME_MEASURE_INSTRUCTION(
									cint64 timeElapsed = mUnsatCacheRetrieval.elapsed();
									STATINCM(TIMEUNSATCACHERETRIVAL,timeElapsed,calcAlgContext);
								);

								if (unsatCached) {
									if (!mConfUnsatCachingUseNodeSignatureSet || mUnsatCachingSignatureSet.contains(individualNode->getReapplyConceptLabelSet(false)->getConceptSignatureValue())) {
										if (mConfUnsatCachingUseFullNodeDependency) {
											clashDescriptors = createClashedIndividualNodeDescriptor(nullptr,individualNode,calcAlgContext);
										}
										STATINC(UNSATCACHEUSEDCOUNT,calcAlgContext);
										throw CCalculationClashProcessingException(clashDescriptors);
									}
								}
							}
							mLastUnsatCacheTestedIndiNode = individualNode;
							mLastUnsatCacheTestedIndiNodeConceptSetSize = conSetSize;
						}
					}
				}


				CClashedDependencyDescriptor* CCalculationTableauCompletionTaskHandleAlgorithm::createClashedIndividualNodeDescriptor(CClashedDependencyDescriptor* prevClashes, CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CClashedDependencyDescriptor* clashDes = prevClashes;
					CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(false);
					CConceptDescriptor* conDesIt = conSet->getAddingSortedConceptDescriptionLinker();
					while (conDesIt) {
						CConceptDescriptor* conDes = conDesIt;
						clashDes = createClashedConceptDescriptor(clashDes,processIndi,conDes,conDes->getDependencyTrackPoint(),calcAlgContext);
						conDesIt = conDesIt->getNext();
					}
					return clashDes;
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::testProblematicConceptSet(CCalculationAlgorithmContextBase* calcAlgContext) {
					if (true) {
						CProcessingDataBox* processingDataBox = calcAlgContext->getProcessingDataBox();
						CIndividualProcessNodeVector* indiNodeVec = processingDataBox->getIndividualProcessNodeVector();
						cint64 indiCount = indiNodeVec->getItemCount();
						cint64 indiIdx = processingDataBox->getOntology()->getABox()->getIndividualCount();
						for (; indiIdx < indiCount; ++indiIdx) {
							CIndividualProcessNode* indiNode = getUpToDateIndividual(indiIdx,calcAlgContext);
							if (indiNode) {
								if (!indiNode->isNominalIndividual()) {
									if (!indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINVALIDATEBLOCKERFLAGSCOMPINATION | CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION)) {
										if (!calcAlgContext->hasCompletionGraphCachedIndividualNodes() || indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED) || indiNode->getIndividualID() > calcAlgContext->getMaxCompletionGraphCachedIndividualNodeID()) {
											indiNode = getLocalizedIndividual(indiNode,false,calcAlgContext);


											if (true) {
												bool base1Con = false;
												bool base2Con = false;
												CConceptDescriptor* conDes = indiNode->getReapplyConceptLabelSet(false)->getAddingSortedConceptDescriptionLinker();
												for (CConceptDescriptor* conDesIt = conDes; conDesIt; conDesIt = conDesIt->getNext()) {
													if (!conDesIt->isNegated() && CIRIName::getRecentIRIName(conDesIt->getData()->getClassNameLinker()) == "http://www.w3.org/TR/2003/CR-owl-guide-20030818/food#SweetFruitCourse") {
														base1Con = true;
													}
													if (!conDesIt->isNegated() && CIRIName::getRecentIRIName(conDesIt->getData()->getClassNameLinker()) == "http://www.w3.org/TR/2003/CR-owl-guide-20030818/food#FruitCourse") {
														base2Con = true;
													}
												}
												if (!base2Con && base1Con) {
													mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
													QFile file(QString("caching-error.txt"));
													if (file.open(QIODevice::WriteOnly)) {
														file.write(mEndTaskDebugIndiModelString.replace("<br>","\r\n").toLocal8Bit());
														file.close();
													}
													bool bug = true;
												}
											}


											//QStringList conSetList = generateExtendedDebugConceptSetStringList(indiNode->getReapplyConceptLabelSet(false),nullptr,nullptr,calcAlgContext);
											//debugTestCriticalConceptSet(conSetList,calcAlgContext);

										}
									}
								}
							}
						}
					}
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::cacheSatisfiableIndividualNodes(CCalculationAlgorithmContextBase* calcAlgContext) {
					bool nodeCached = false;
					CSatisfiableExpanderCacheHandler* satisfiableExpHandler = calcAlgContext->getUsedSatisfiableExpanderCacheHandler();
					CSaturationNodeExpansionCacheHandler* saturationExpHandler = calcAlgContext->getUsedSaturationNodeExpansionCacheHandler();
					if (mConfSatExpCacheWriting && satisfiableExpHandler || mConfSaturationSatisfiabilitiyExpansionCacheWriting && saturationExpHandler) {
						CProcessingDataBox* processingDataBox = calcAlgContext->getProcessingDataBox();
						CIndividualProcessNodeVector* indiNodeVec = processingDataBox->getIndividualProcessNodeVector();
						cint64 indiCount = indiNodeVec->getItemCount();
						cint64 indiIdx = processingDataBox->getOntology()->getABox()->getIndividualCount();
						for (; indiIdx < indiCount; ++indiIdx) {
							CIndividualProcessNode* indiNode = getUpToDateIndividual(indiIdx,calcAlgContext);
							if (indiNode) {
								if (!indiNode->isNominalIndividual()) {
									if (mConfSatExpCacheWriting && satisfiableExpHandler && !indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINVALIDATEBLOCKERFLAGSCOMPINATION | CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION | CIndividualProcessNode::PRFSATURATIONBLOCKINGCACHED)) {
										if (!calcAlgContext->hasCompletionGraphCachedIndividualNodes() || indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED) || indiNode->getIndividualID() > calcAlgContext->getMaxCompletionGraphCachedIndividualNodeID()) {
											indiNode = getLocalizedIndividual(indiNode,false,calcAlgContext);


											//CConceptProcessingQueue* conProQue = indiNode->getConceptProcessingQueue(false);
											//if (conProQue) {
											//	if (!conProQue->isEmpty()) {
											//		mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
											//		QFile file(QString("caching-error.txt"));
											//		if (file.open(QIODevice::WriteOnly)) {
											//			file.write(mEndTaskDebugIndiModelString.replace("<br>","\r\n").toLocal8Bit());
											//			file.close();
											//		}
											//		bool bug = true;
											//	}
											//}
											 

											//CConceptDescriptor* conDes = indiNode->getReapplyConceptLabelSet(false)->getAddingSortedConceptDescriptionLinker();
											//bool amountSubstanceFound = false;
											//for (CConceptDescriptor* conDesIt = conDes; conDesIt; conDesIt = conDesIt->getNext()) {
											//	CConcept* concept = conDesIt->getConcept();
											//	if (concept->getOperatorCode() == CCIMPLALL) {
											//		if (CIRIName::getRecentIRIName(concept->getRole()->getPropertyNameLinker()) == "http://purl.org/biotop/biotop.owl#granularPartOf") {
											//			CConcept* orCocnept = concept->getOperandList()->getData();
											//			if (orCocnept->getOperatorCode() == CCOR) {
											//				for (CSortedNegLinker<CConcept*>* opLinker = orCocnept->getOperandList(); opLinker; opLinker = opLinker->getNext()) {
											//					if (opLinker->getData()->getOperatorCode() == CCSUB && CIRIName::getRecentIRIName(opLinker->getData()->getClassNameLinker()) == "http://purl.org/imbi/dco/dco#AmountOfSubstance") {
											//						amountSubstanceFound = true;
											//					}
											//				}
											//			}
											//		}
											//	}
											//}
											//if (amountSubstanceFound) {
											//	bool base1Con = false;
											//	bool base2Con = false;
											//	CConceptDescriptor* conDes = indiNode->getReapplyConceptLabelSet(false)->getAddingSortedConceptDescriptionLinker();
											//	for (CConceptDescriptor* conDesIt = conDes; conDesIt; conDesIt = conDesIt->getNext()) {
											//		if (!conDesIt->isNegated() && CIRIName::getRecentIRIName(conDesIt->getData()->getClassNameLinker()) == "http://purl.org/biotop/biotop.owl#AmountOfPureSubstance") {
											//			base1Con = true;
											//		}
											//		if (!conDesIt->isNegated() && CIRIName::getRecentIRIName(conDesIt->getData()->getClassNameLinker()) == "http://purl.org/imbi/dco/dco#AmountOfSubstance") {
											//			base2Con = true;
											//		}
											//	}
											//	if (!base2Con && base1Con) {
											//		mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
											//		QFile file(QString("caching-error.txt"));
											//		if (file.open(QIODevice::WriteOnly)) {
											//			file.write(mEndTaskDebugIndiModelString.replace("<br>","\r\n").toLocal8Bit());
											//			file.close();
											//		}
											//		bool bug = true;
											//	}
											//}


											//QStringList conSetList = generateExtendedDebugConceptSetStringList(indiNode->getReapplyConceptLabelSet(false),nullptr,nullptr,calcAlgContext);
											//debugTestCriticalConceptSet(conSetList,calcAlgContext);

											nodeCached |= satisfiableExpHandler->cacheIndividualNodeSatisfiable(indiNode,calcAlgContext);
										}
									}



									if (mConfSaturationSatisfiabilitiyExpansionCacheWriting && saturationExpHandler && !indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINVALIDATEBLOCKERFLAGSCOMPINATION | CIndividualProcessNode::PRFSUCCESSORNEWNOMINALCONNECTION | CIndividualProcessNode::PRFSATURATIONBLOCKINGCACHED)) {
										if (!calcAlgContext->hasCompletionGraphCachedIndividualNodes() || indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED) || indiNode->getIndividualID() > calcAlgContext->getMaxCompletionGraphCachedIndividualNodeID()) {
											nodeCached |= saturationExpHandler->tryNodeSatisfiableCaching(indiNode,calcAlgContext);
										}
									}
								}
							}
						}
					}
					return nodeCached;
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::debugTestCriticalConceptSet(QStringList& conSetList, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (conSetList.count() == 67) {
						bool containAll = true;
						QSet<QString> testConceptSetStringSet;
						for (QStringList::const_iterator it = conSetList.constBegin(), itEnd = conSetList.constEnd(); containAll && it != itEnd; ++it) {
							const QString& conString = *it;
							QString testString = conString;
							testString = testString.remove("^");
							testString = testString.remove("0");
							testString = testString.remove("1");
							testString = testString.remove("2");
							testString = testString.remove("3");
							testString = testString.remove("4");
							testString = testString.remove("5");
							testString = testString.remove("6");
							testString = testString.remove("7");
							testString = testString.remove("8");
							testString = testString.remove("9");
							testString = testString.trimmed();
							testConceptSetStringSet.insert(testString);
						}

						if (testConceptSetStringSet.count() >= mCriticalConceptSetStringSet.count()) {
							foreach (const QString& testString, mCriticalConceptSetStringSet) {
								if (!testConceptSetStringSet.contains(testString)) {
									containAll = false;
								}
							}
						} else {
							containAll = false;
						}

						if (containAll && !mFoundCriticalConceptSet) {
							mFoundCriticalConceptSet = true;
							mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
							mFoundCriticalConceptSet = false;
							bool debug = true;
						}
					}
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::testAllSuccessorsProcessedAndWriteSatisfiableCache(CIndividualProcessNode* indiNode, CPROCESSINGSET<CIndividualProcessNode*>* processedNodeSet, CSatisfiableExpanderCacheHandler* satExpHandler, CCalculationAlgorithmContextBase* calcAlgContext) {

					if (!indiNode->isNominalIndividual() && !indiNode->hasBackwardDependencyToAncestorIndividualNode()) {
						if (!indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINVALIDATEBLOCKERFLAGSCOMPINATION | CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION)) {
							if (!calcAlgContext->hasCompletionGraphCachedIndividualNodes() || indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED) || indiNode->getIndividualID() > calcAlgContext->getMaxCompletionGraphCachedIndividualNodeID()) {
								CConceptProcessingQueue* conProQue = indiNode->getConceptProcessingQueue(false);
								if (!conProQue || conProQue->isEmpty()) {

									if (!processedNodeSet->contains(indiNode)) {
										processedNodeSet->insert(indiNode);

										CIndividualProcessNode* ancIndi = getAncestorIndividual(indiNode,calcAlgContext);
										CSuccessorIterator succIt(indiNode->getSuccessorIterator());
										while (succIt.hasNext()) {
											CIndividualLinkEdge* succLink = succIt.nextLink();
											CIndividualProcessNode* succIndi = getSuccessorIndividual(indiNode,succLink,calcAlgContext);
											if (!ancIndi || succIndi->getIndividualID() != ancIndi->getIndividualID()) {
												if (!testAllSuccessorsProcessedAndWriteSatisfiableCache(succIndi,processedNodeSet,satExpHandler,calcAlgContext)) {
													return false;
												}
											}
										}

										indiNode = getLocalizedIndividual(indiNode,false,calcAlgContext);
										satExpHandler->cacheIndividualNodeSatisfiable(indiNode,calcAlgContext);

										return true;
									}
								}
							}
						}
					}
					return false;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::writeSatisfiableCachedIndividualNodesOfUnsatisfiableBranch(CCalculationAlgorithmContextBase* calcAlgContext) {
					bool nodeCached = false;
					CSatisfiableExpanderCacheHandler* satExpHandler = calcAlgContext->getUsedSatisfiableExpanderCacheHandler();
					if (mConfUnsatBranchSatisfiableCaching && mConfSatExpCacheWriting && satExpHandler) {
						CProcessingDataBox* processingDataBox = calcAlgContext->getProcessingDataBox();
						CIndividualProcessNodeVector* indiNodeVec = processingDataBox->getIndividualProcessNodeVector();

						CPROCESSINGSET<CIndividualProcessNode*>* processedNodeSet = CObjectParameterizingAllocator< CPROCESSINGSET<CIndividualProcessNode*>,CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getUsedTaskProcessorContext());
						cint64 indiCount = indiNodeVec->getItemCount()-1;
						for (cint64 indiIdx = 0; indiIdx < indiCount; ++indiIdx) {
							CIndividualProcessNode* indiNode = getUpToDateIndividual(indiIdx,calcAlgContext);
							if (indiNode) {
								if (!indiNode->isNominalIndividual()) {
									if (!indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINVALIDATEBLOCKERFLAGSCOMPINATION | CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION)) {
										if (!calcAlgContext->hasCompletionGraphCachedIndividualNodes() || indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED) || indiNode->getIndividualID() > calcAlgContext->getMaxCompletionGraphCachedIndividualNodeID()) {
											CConceptProcessingQueue* conProQue = indiNode->getConceptProcessingQueue(false);
											if (!conProQue || conProQue->isEmpty()) {
												// test whether all successor nodes are already processed
												nodeCached |= testAllSuccessorsProcessedAndWriteSatisfiableCache(indiNode,processedNodeSet,satExpHandler,calcAlgContext);
											}
										}
									}
								}
							}
						}
					}
					return nodeCached;
				}




				bool CCalculationTableauCompletionTaskHandleAlgorithm::isSaturationCachedProcessingBlocked(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool processingBlocked = false;
					bool saturationCached = detectIndividualNodeSaturationCached(individualNode,calcAlgContext);
					if (saturationCached) {
						// block processing only for successors of saturation cached nodes
						processingBlocked = individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATURATIONBLOCKINGCACHED);
					}
					return processingBlocked;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::detectIndividualNodeSaturationCached(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext) {

					if (individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONBLOCKINGCACHED) && !individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFRETESTSATURATIONBLOCKINGCACHEDDUEDIRECTMODIFIED)) {
						return true;
					}

					bool prevSatCached = individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONBLOCKINGCACHED | CIndividualProcessNode::PRFANCESTORSATURATIONBLOCKINGCACHED);
					bool prevSatSuccCreationBlocked = individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONSUCCESSORCREATIONBLOCKINGCACHED);


					if (individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATURATIONBLOCKINGCACHED)) {
						if (individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATURATIONBLOCKINGCACHEDABOLISHED)) {
							individualNode->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATURATIONBLOCKINGCACHEDABOLISHED);
							individualNode->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATURATIONBLOCKINGCACHED);
						} else {
							return true;
						}
					}
					if (individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFRETESTSATURATIONBLOCKINGCACHEDDUEDIRECTMODIFIED)) {
						individualNode->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFRETESTSATURATIONBLOCKINGCACHEDDUEDIRECTMODIFIED);
					}
					bool stillSaturationCached = false;
					if (!individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONBLOCKINGCACHEDINVALIDATED)) {

						CSaturationNodeExpansionCacheHandler* satNodeExpCacheHandler = calcAlgContext->getSaturationNodeExpansionCacheHandler();
						if (!stillSaturationCached && satNodeExpCacheHandler && mConfSaturationExpansionCacheReading) {
							CSaturationNodeAssociatedConceptExpansion* expansion = nullptr;

							if (individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONBLOCKINGCACHEDRETESTDUETOMODIFICATION)) {
								individualNode->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONBLOCKINGCACHEDRETESTDUETOMODIFICATION);
								if (satNodeExpCacheHandler->isNodeSatisfiableCached(individualNode,expansion,calcAlgContext)) {
									stillSaturationCached = true;
									if (expansion && expansion->getDependentNominalSet(false) && !mConfSaturationCachingWithNominals) {
										stillSaturationCached = false;
									} else if (expansion) {
										installSaturationCachingReactivation(individualNode,expansion->getDependentNominalSet(false),calcAlgContext);

										if (prevSatSuccCreationBlocked && expansion->getHasTightAtMostRestriction()) {
											individualNode->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONSUCCESSORCREATIONBLOCKINGCACHED);
											reapplySatisfiableCachedAbsorbedGeneratingConcepts(individualNode,calcAlgContext);
										}
										if (!prevSatSuccCreationBlocked && !expansion->getHasTightAtMostRestriction()) {
											individualNode->addProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONSUCCESSORCREATIONBLOCKINGCACHED);
										}
									}
								}
							}
						}
					}
					if (!stillSaturationCached) {
						if (prevSatCached) {
							STATINC(SATURATIONCACHELOSECOUNT,calcAlgContext);
							individualNode->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONBLOCKINGCACHED);
							reactivateIndirectSaturationCachedSuccessors(individualNode,false,calcAlgContext);
						}
						if (prevSatSuccCreationBlocked) {
							individualNode->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONSUCCESSORCREATIONBLOCKINGCACHED);
							reapplySatisfiableCachedAbsorbedGeneratingConcepts(individualNode,calcAlgContext);
						}
					} else {
						if (!prevSatCached) {
							STATINC(SATURATIONCACHEESTABLISHCOUNT,calcAlgContext);
							individualNode->addProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONBLOCKINGCACHED);
							propagateIndirectSuccessorSaturationBlocked(individualNode,calcAlgContext);
						}
					}
					return stillSaturationCached;
				}




				bool CCalculationTableauCompletionTaskHandleAlgorithm::isSatisfiableCachedProcessingBlocked(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool processingBlocked = false;
					bool satisfiableCached = detectIndividualNodeSatisfiableExpandedCached(individualNode,calcAlgContext);
					if (satisfiableCached) {
						// block processing only for successors of satisfiable cached nodes
						processingBlocked = individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATISFIABLECACHED);
					}
					return processingBlocked;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::detectIndividualNodeSatisfiableExpandedCached(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext) {

					bool prevSatCached = individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATISFIABLECACHED | CIndividualProcessNode::PRFANCESTORSATISFIABLECACHED);
					
					
					if (individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATISFIABLECACHED)) {
						if (individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATISFIABLECACHEDABOLISHED)) {
							individualNode->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATISFIABLECACHEDABOLISHED);
							individualNode->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATISFIABLECACHED);
						} else {
							return true;
						}
					}
					if (individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFRETESTSATISFIABLECACHEDDUEDIRECTMODIFIED)) {
						individualNode->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFRETESTSATISFIABLECACHEDDUEDIRECTMODIFIED);
					}


					//QStringList conSetList = generateExtendedDebugConceptSetStringList(individualNode->getReapplyConceptLabelSet(false),nullptr,nullptr,calcAlgContext);
					//debugTestCriticalConceptSet(conSetList,calcAlgContext);


					bool newSatCached = false;

					CSatisfiableExpanderCacheHandler* satExpHandler = calcAlgContext->getUsedSatisfiableExpanderCacheHandler();
					if (!individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION | CIndividualProcessNode::PRFINVALIDBLOCKINGORCACHING | CIndividualProcessNode::PRFSATURATIONBLOCKINGCACHED)) {
						// can only satisfiable cached if there is no nominal
						if (mConfSatExpCacheRetrieval && satExpHandler) {
							bool satisfiableCached = false;
							CSignatureSatisfiableExpanderCacheEntry* entry = nullptr;
							STATINC(SATEXPCACHERETRIEVALCOUNT,calcAlgContext);
							if (satExpHandler->isIndividualNodeExpandCached(individualNode,&satisfiableCached,&entry,calcAlgContext)) {
								STATINC(SATEXPCACHERETRIEVALSUCCESSCOUNT,calcAlgContext);
								if (mConfSatExpCacheConceptExpansion) {
									expandCachedConcepts(individualNode,entry,calcAlgContext);
									if (mConfSatExpCacheSatisfiableBlocking && satisfiableCached && !individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINVALIDBLOCKINGORCACHING)) {
										STATINC(SATEXPCACHERETRIEVALFOUNDSATISFIABLECOUNT,calcAlgContext);
										bool satCompatible = false;
										CExpanderBranchedLinker* satBranchCompatibleLinker = nullptr;
										if (entry->isSatisfiableWithoutBranchedConcepts()) {
											satCompatible = true;
										} else {
											CExpanderBranchedLinker* satBranchLinker = entry->getExpanderBranchedLinker();
											if (!satBranchLinker) {
												satCompatible = true;
											} else {
												CIndividualProcessNode* ancestorIndiNode = getAncestorIndividual(individualNode,calcAlgContext);
												CExpanderBranchedLinker* satBranchLinkerIt = satBranchLinker;
												while (satBranchLinkerIt && !satCompatible) {
													bool allSatCompatible = true;
													STATINC(SATEXPCACHERETRIEVALCOMPATIBILITYTESTCOUNT,calcAlgContext);
													allSatCompatible = isSatisfiableCachedCompatible(individualNode,satBranchLinkerIt,ancestorIndiNode,calcAlgContext);		
													if (allSatCompatible) {
														satBranchCompatibleLinker = satBranchLinkerIt;
														satCompatible = true;
													}
													satBranchLinkerIt = satBranchLinkerIt->getNext();
												}									
											}
										}

										if (satCompatible) {

											//QStringList conSetList = generateExtendedDebugConceptSetStringList(individualNode->getReapplyConceptLabelSet(false),nullptr,nullptr,calcAlgContext);
											//debugTestCriticalConceptSet(conSetList,calcAlgContext);

											STATINC(SATEXPCACHERETRIEVALCOMPATIBLESATCOUNT,calcAlgContext);
											newSatCached = true;
											if (!prevSatCached) {
												individualNode->addProcessingRestrictionFlags(CIndividualProcessNode::PRFSATISFIABLECACHED);
												propagateIndirectSuccessorSatisfiableCached(individualNode,calcAlgContext);
											}
										} else {
											STATINC(SATEXPCACHERETRIEVALINCOMPATIBLESATCOUNT,calcAlgContext);
										}
									}
								}
							}
						}
						if (mConfSatExpCacheWriting && !newSatCached) {
							if (!individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSIGNATUREBLOCKINGCACHED | CIndividualProcessNode::PRFANCESTORSIGNATUREBLOCKINGCACHED)) {
								//mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
								//QFile file(QString("caching-error.txt"));
								//if (file.open(QIODevice::WriteOnly)) {
								//	file.write(mEndTaskDebugIndiModelString.replace("<br>","\r\n").toLocal8Bit());
								//	file.close();
								//}
								satExpHandler->cacheIndividualNodeExpansion(individualNode,calcAlgContext);
							}
						}
					}
					if (!newSatCached) {
						if (prevSatCached) {
							individualNode->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFSATISFIABLECACHED);
							reactivateIndirectSatisfiableCachedSuccessors(individualNode,false,calcAlgContext);

							reapplySatisfiableCachedAbsorbedDisjunctionConcepts(individualNode,calcAlgContext);
							reapplySatisfiableCachedAbsorbedGeneratingConcepts(individualNode,calcAlgContext);
						}
					}


					//if (satExpHandler) {
					//	bool satisfiableCached = false;
					//	CSignatureSatisfiableExpanderCacheEntry* entry = nullptr;
					//	satExpHandler->isIndividualNodeExpandCached(individualNode,&satisfiableCached,&entry,calcAlgContext);
					//	if (entry && !individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINVALIDBLOCKINGORCACHING)) {
					//		CReapplyConceptLabelSet* conSet = individualNode->getReapplyConceptLabelSet(false);
					//		cint64 conSetCount = conSet->getConceptCount();
					//		cint64 expandCount = entry->getExpanderCacheValueCount();
					//		if (conSetCount != expandCount) {
					//			bool bug = true;
					//		}
					//	}
					//}
					return newSatCached;
				}





				bool CCalculationTableauCompletionTaskHandleAlgorithm::hasCompatibleConceptSetReuse(CIndividualProcessNode* indiNode, CReapplyConceptLabelSet* subConceptSet, CIndividualProcessNode* reuseNodeCand, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReapplyConceptLabelSet* superConceptSet = reuseNodeCand->getReapplyConceptLabelSet(false);
					bool isSubset = isLabelConceptSubSet(subConceptSet,superConceptSet,nullptr,nullptr,calcAlgContext);
					if (!isSubset) {
						return false;
					}

					CReapplyConceptLabelSetIterator superConSetIt = superConceptSet->getConceptLabelSetIterator(false,false,false);
					while (superConSetIt.hasNext()) {
						CConceptDescriptor* conDes = superConSetIt.next();
						if (isConceptSignatureBlockingCritical(reuseNodeCand,conDes,conDes->getDependencyTrackPoint(),calcAlgContext)) {
							indiNode->setInvalidSignatureBlocking(true);
							return false;
						}
					}

					return true;
				}




				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::searchSignatureReusingIndividualNode(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* processingDataBox = calcAlgContext->getProcessingDataBox();
					CSignatureBlockingCandidateHash* sigBlockCandHash = processingDataBox->getSignatureBlockingCandidateHash(false);
					CReapplyConceptLabelSet* conSet = individualNode->getReapplyConceptLabelSet(false);
					if (sigBlockCandHash && conSet) {
						cint64 conCount = conSet->getConceptCount();
						if (!individualNode->isInvalidSignatureBlocking() && individualNode->getLastConceptCountSearchBlockingCandidate() != conCount) {
							individualNode->setLastConceptCountSearchBlockingCandidate(conCount);

							cint64 conSig = conSet->getConceptSignatureValue();
							cint64 newCandCount = sigBlockCandHash->getBlockingCandidatesCount(conSig);
							cint64 lastCandCount = individualNode->getLastSearchBlockerCandidateCount();
							cint64 lastCandSignature = individualNode->getLastSearchBlockerCandidateSignature();
							if (lastCandSignature != conSig) {
								lastCandCount = 0;
							}
							individualNode->setLastSearchBlockerCandidateSignature(conSig);
							if (newCandCount != lastCandCount) {

								cint64 candDiffCount = newCandCount - lastCandCount;

								CSignatureBlockingCandidateIterator candIt(sigBlockCandHash->getBlockingCandidatesIterator(conSig));
								while (candIt.hasNext() && candDiffCount-- > 0 && !individualNode->isInvalidSignatureBlocking()) {
									cint64 candIndiID = candIt.next(true);
									if (candIndiID != individualNode->getIndividualID()) {
										CIndividualProcessNode* candIndiNode = getUpToDateIndividual(candIndiID,calcAlgContext);
										if (isIndividualNodeValidBlocker(candIndiNode,calcAlgContext)) {
											bool compatible = hasCompatibleConceptSetReuse(individualNode,conSet,candIndiNode,calcAlgContext);
											if (compatible) {
												individualNode->setLastSearchBlockerCandidateCount(newCandCount-candDiffCount);
												return candIndiNode;
											}
										}
									}
								}
								individualNode->setLastSearchBlockerCandidateCount(newCandCount);

							}
						}
					}
					return nullptr;
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::removeIndividualReusing(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					individualNode->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFREUSINGINDIVIDUAL);
					removeReusingBlockerFollowing(individualNode,calcAlgContext);
					reactivateIndirectReuseSuccessors(individualNode,true,calcAlgContext);
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::updateIndividualReusing(CIndividualProcessNode* processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (processIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFREUSINGINDIVIDUAL)) {
						// 1. update non-deterministic expansions, 2. update deterministic expansions
						CReusingIndividualNodeConceptExpansionData* reusingData = processIndi->getReusingIndividualNodeConceptExpansionData(false);
						CIndividualProcessNode* reuseIndi = reusingData->getBlockerIndividualNode();
						reuseIndi = getUpToDateIndividual(reuseIndi,calcAlgContext);

						// test whether reused individual is now invalid or blocked
						if (isIndividualNodeValidBlocker(reuseIndi,calcAlgContext)) {
							// search other reuseable individual
							removeReusingBlockerFollowing(processIndi,calcAlgContext);

							reuseIndi = searchSignatureReusingIndividualNode(processIndi,calcAlgContext);
							if (reuseIndi) {

								CReusingIndividualNodeConceptExpansionData* locReusingData = processIndi->getReusingIndividualNodeConceptExpansionData(true);
								if (!locReusingData) {
									CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
									locReusingData = CObjectAllocator<CReusingIndividualNodeConceptExpansionData>::allocateAndConstruct(taskMemMan);
									locReusingData->initBlockingExpansionData(reusingData);
									reusingData = locReusingData;
									processIndi->setReusingIndividualNodeConceptExpansionData(locReusingData);
								}
								locReusingData->setBlockerIndividualNode(reuseIndi);
								locReusingData->setBlockingConceptCount(0);
								locReusingData->setBlockingConceptSignature(0);
								locReusingData->setLastSubsetTestedConceptDescriptor(nullptr);
								locReusingData->setContinuousExpandedContainedConceptCount(0);
								locReusingData->setLastUpdatedConceptCount(0);
								locReusingData->setLastUpdatedConceptExpansionCount(0);
								locReusingData->setLastNonDeterministicExpansionLinker(nullptr);

								addReusingBlockerFollowing(processIndi,calcAlgContext);
							} else {

								reactivateIndirectReuseSuccessors(processIndi,true,calcAlgContext);
								processIndi->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFREUSINGINDIVIDUAL);
								if (reusingData->isBlockingReviewMarked()) {
									CReusingReviewData* reusingReviewData = calcAlgContext->getProcessingDataBox()->getReusingReviewData(true);
									reusingReviewData->remove(processIndi->getIndividualID());
								}

							}
						}

						bool reusingIndiCompatible = true;

						if (reuseIndi) {
							anlyzeIndiviudalNodesConceptExpansion(reuseIndi,calcAlgContext);


							CIndividualNodeAnalizedConceptExpansionData* blockerAnalizedConExpData = reuseIndi->getAnalizedConceptExpansionData(false);
							CXLinker<CConceptDescriptor*>* nonDetExpLinker = blockerAnalizedConExpData->getAnalysedNonDeterministicConceptExpansionLinker();
							reusingIndiCompatible &= !blockerAnalizedConExpData->isInvalidBlocker();

							CXLinker<CConceptDescriptor*>* lastNonDetExpLinker = reusingData->getLastNonDeterministicExpansionLinker();
							CReapplyConceptLabelSet* processIndiLabelSet = processIndi->getReapplyConceptLabelSet(false);

							if (nonDetExpLinker != lastNonDetExpLinker) {
								processIndiLabelSet = processIndi->getReapplyConceptLabelSet(true);

								CReusingIndividualNodeConceptExpansionData* locReusingData = processIndi->getReusingIndividualNodeConceptExpansionData(true);
								if (!locReusingData) {
									CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
									locReusingData = CObjectAllocator<CReusingIndividualNodeConceptExpansionData>::allocateAndConstruct(taskMemMan);
									locReusingData->initBlockingExpansionData(reusingData);
									reusingData = locReusingData;
									processIndi->setReusingIndividualNodeConceptExpansionData(locReusingData);
								}

								CNonDeterministicDependencyTrackPoint* lastReuseConceptsDepTrackPoint = locReusingData->getReuseConceptsDependencyTrackPoint();


								CREUSECONCEPTSDependencyNode* reuseConceptsDepNode = createREUSECONCEPTSDependency(processIndi,nullptr,lastReuseConceptsDepTrackPoint,calcAlgContext);
								CNonDeterministicDependencyTrackPoint* reuseConceptsDependencyTrackPoint = createNonDeterministicDependencyTrackPointBranch(reuseConceptsDepNode,true,calcAlgContext);

								for (CXLinker<CConceptDescriptor*>* nonDetExpLinkerIt = nonDetExpLinker; nonDetExpLinkerIt; nonDetExpLinkerIt = nonDetExpLinkerIt->getNext()) {
									CConceptDescriptor* reusingConDes = nonDetExpLinkerIt->getData();

									if (!processIndiLabelSet->containsConceptDescriptor(reusingConDes)) {
										// add non-deterministically the missing concept
										addConceptToIndividual(reusingConDes->getConcept(),reusingConDes->isNegated(),processIndi,reuseConceptsDependencyTrackPoint,false,true,calcAlgContext);
									}
								}

								locReusingData->setReuseConceptsDependencyTrackPoint(reuseConceptsDependencyTrackPoint);
								locReusingData->setLastNonDeterministicExpansionLinker(nonDetExpLinker);

							}

							CConceptDescriptor* addingSortedConDes = processIndiLabelSet->getAddingSortedConceptDescriptionLinker();
							CConceptDescriptor* lastSubSetTestConDes = reusingData->getLastSubsetTestedConceptDescriptor();

							if (addingSortedConDes != lastSubSetTestConDes || blockerAnalizedConExpData->getExpansionConceptCount() != reusingData->getLastUpdatedConceptExpansionCount()) {
								CReusingIndividualNodeConceptExpansionData* locReusingData = processIndi->getReusingIndividualNodeConceptExpansionData(true);
								if (!locReusingData) {
									CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
									locReusingData = CObjectAllocator<CReusingIndividualNodeConceptExpansionData>::allocateAndConstruct(taskMemMan);
									locReusingData->initBlockingExpansionData(reusingData);
									reusingData = locReusingData;
									processIndi->setReusingIndividualNodeConceptExpansionData(locReusingData);
								}
								updateSignatureBlockingConceptExpansion(processIndi,locReusingData,reuseIndi,blockerAnalizedConExpData,calcAlgContext);

								CReapplyConceptLabelSet* reuseIndiLabelSet = reuseIndi->getReapplyConceptLabelSet(false);

								// validate still subset
								bool stillSubset = locReusingData->isConceptSetStillSubset();
								if (stillSubset) {
									CConceptDescriptor* addingSortedConDesIt = addingSortedConDes;
									while (addingSortedConDesIt != lastSubSetTestConDes && stillSubset) {
										CConcept* concept = addingSortedConDesIt->getConcept();
										bool conNegation = addingSortedConDesIt->isNegated();
										if (!processIndiLabelSet->containsConcept(concept,conNegation)) {
											stillSubset = false;
										}
										addingSortedConDesIt = addingSortedConDesIt->getNext();
									}
									locReusingData->setConceptSetStillSubset(stillSubset);
								}
								locReusingData->setLastSubsetTestedConceptDescriptor(addingSortedConDes);

								if (!stillSubset) {
									reusingIndiCompatible = false;
								}
							}
						}

						if (reuseIndi) {
							if (!reusingIndiCompatible) {
								CReusingIndividualNodeConceptExpansionData* locReusingData = processIndi->getReusingIndividualNodeConceptExpansionData(true);
								if (!locReusingData) {
									CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
									locReusingData = CObjectAllocator<CReusingIndividualNodeConceptExpansionData>::allocateAndConstruct(taskMemMan);
									locReusingData->initBlockingExpansionData(reusingData);
									reusingData = locReusingData;
									processIndi->setReusingIndividualNodeConceptExpansionData(locReusingData);
								}


								if (!locReusingData->isBlockingReviewMarked()) {
									locReusingData->setBlockingReviewMarked(true);
									CReusingReviewData* reusingReviewData = calcAlgContext->getProcessingDataBox()->getReusingReviewData(true);
									reusingReviewData->insert(processIndi->getIndividualAncestorDepth(),processIndi->getIndividualID());
								}
							}
						}

						
					}
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::upgradeSignatureBlockingToIndividualReusing(CIndividualProcessNode* processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSignatureBlockingIndividualNodeConceptExpansionData* sigBlockData = processIndi->getSignatureBlockingIndividualNodeConceptExpansionData(false);
					CIndividualProcessNode* reuseIndi = sigBlockData->getBlockerIndividualNode();

					processIndi->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFSIGNATUREBLOCKINGCACHED);
					reactivateIndirectSignatureBlockedSuccessors(processIndi,false,calcAlgContext);
					reapplySatisfiableCachedAbsorbedGeneratingConcepts(processIndi,calcAlgContext);

					establishIndividualReusing(processIndi,reuseIndi,calcAlgContext);
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::establishIndividualReusing(CIndividualProcessNode* processIndi, CIndividualProcessNode* reuseIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CREUSEINDIVIDUALDependencyNode* reuseDepNode = createREUSEINDIVIDUALDependency(processIndi,nullptr,nullptr,calcAlgContext);

					CSatisfiableCalculationTask* newTaskList = createDependendBranchingTaskList(2,calcAlgContext);
					CTaskProcessorContext* processorContext = calcAlgContext->getUsedTaskProcessorContext();

					anlyzeIndiviudalNodesConceptExpansion(reuseIndi,calcAlgContext);
					CIndividualNodeAnalizedConceptExpansionData* blockerAnalizedConExpData = reuseIndi->getAnalizedConceptExpansionData(false);
					if (blockerAnalizedConExpData) {

						CXLinker<CConceptDescriptor*>* nonDetExpLinker = blockerAnalizedConExpData->getAnalysedNonDeterministicConceptExpansionLinker();

						CSatisfiableCalculationTask* newTaskIt = newTaskList;
						for (cint64 i = 0; i < 2; ++i) {

							bool reusingAlternative = i == 0;

							CSatisfiableCalculationTask* newSatCalcTask = newTaskIt;
							CProcessContext* newProcessContext = newSatCalcTask->getProcessContext(processorContext);

							CCalculationAlgorithmContextBase* newCalcAlgContext = createCalculationAlgorithmContext(processorContext,newProcessContext,newSatCalcTask);
							CProcessingDataBox* newProcessingDataBox = newSatCalcTask->getProcessingDataBox();

							CProcessTagger* newProcessTagger = newCalcAlgContext->getUsedProcessTagger();
							newProcessTagger->incBranchingTag();
							newProcessTagger->incLocalizationTag();

							CIndividualProcessNode* newLocIndiNode = getLocalizedIndividual(processIndi,false,newCalcAlgContext);
							CConceptProcessingQueue* newConProcQueue = newLocIndiNode->getConceptProcessingQueue(true);


							CReusingIndividualNodeConceptExpansionData* locReusingData = newLocIndiNode->getReusingIndividualNodeConceptExpansionData(true);
							if (!locReusingData) {
								CMemoryAllocationManager* taskMemMan = newCalcAlgContext->getUsedProcessTaskMemoryAllocationManager();
								CReusingIndividualNodeConceptExpansionData* reusingData = newLocIndiNode->getReusingIndividualNodeConceptExpansionData(false);
								locReusingData = CObjectAllocator<CReusingIndividualNodeConceptExpansionData>::allocateAndConstruct(taskMemMan);
								locReusingData->initBlockingExpansionData(reusingData);
								newLocIndiNode->setReusingIndividualNodeConceptExpansionData(locReusingData);
							}
							locReusingData->incReusingTriedCount();

							locReusingData->setBlockingConceptCount(0);
							locReusingData->setBlockingConceptSignature(0);
							locReusingData->setLastSubsetTestedConceptDescriptor(nullptr);
							locReusingData->setContinuousExpandedContainedConceptCount(0);
							locReusingData->setBlockerIndividualNode(nullptr);
							locReusingData->setLastUpdatedConceptCount(0);
							locReusingData->setLastUpdatedConceptExpansionCount(0);
							locReusingData->setBlockingReviewMarked(false);



							if (reusingAlternative) {

								CNonDeterministicDependencyTrackPoint* newDependencyTrackPoint = createNonDeterministicDependencyTrackPointBranch(reuseDepNode,false,newCalcAlgContext);

								CREUSECONCEPTSDependencyNode* reuseConceptsDepNode = createREUSECONCEPTSDependency(processIndi,nullptr,newDependencyTrackPoint,newCalcAlgContext);
								CNonDeterministicDependencyTrackPoint* reuseConceptsDependencyTrackPoint = createNonDeterministicDependencyTrackPointBranch(reuseConceptsDepNode,true,newCalcAlgContext);

								locReusingData->setReuseConceptsDependencyTrackPoint(reuseConceptsDependencyTrackPoint);

								CReapplyConceptLabelSet* indiLabelSet = newLocIndiNode->getReapplyConceptLabelSet(true);


								locReusingData->setBlockingConceptCount(indiLabelSet->getConceptCount());
								locReusingData->setBlockingConceptSignature(indiLabelSet->getConceptSignatureValue());
								locReusingData->setLastSubsetTestedConceptDescriptor(indiLabelSet->getAddingSortedConceptDescriptionLinker());
								locReusingData->setContinuousExpandedContainedConceptCount(0);
								locReusingData->setBlockerIndividualNode(reuseIndi);
								locReusingData->setLastUpdatedConceptCount(0);
								locReusingData->setLastUpdatedConceptExpansionCount(0);


								for (CXLinker<CConceptDescriptor*>* nonDetExpLinkerIt = nonDetExpLinker; nonDetExpLinkerIt; nonDetExpLinkerIt = nonDetExpLinkerIt->getNext()) {
									CConceptDescriptor* reusingConDes = nonDetExpLinkerIt->getData();

									if (!indiLabelSet->containsConceptDescriptor(reusingConDes)) {
										// add non-deterministically the missing concept
										addConceptToIndividual(reusingConDes->getConcept(),reusingConDes->isNegated(),newLocIndiNode,reuseConceptsDependencyTrackPoint,false,true,newCalcAlgContext);
									}
								}

								newLocIndiNode->addProcessingRestrictionFlags(CIndividualProcessNode::PRFREUSINGINDIVIDUAL);

								addReusingBlockerFollowing(newLocIndiNode,newCalcAlgContext);
								propagateIndirectSuccessorReuseBlocked(newLocIndiNode,newCalcAlgContext);
							} else {
								CNonDeterministicDependencyTrackPoint* newDependencyTrackPoint = createNonDeterministicDependencyTrackPointBranch(reuseDepNode,false,newCalcAlgContext);
								locReusingData->incReusingFailedCount();
								locReusingData->addReusingFailedSignatureAndIndividual(reuseIndi->getReapplyConceptLabelSet(false)->getConceptSignatureValue(),reuseIndi->getIndividualID());
							}

							prepareBranchedTaskProcessing(newLocIndiNode,newTaskIt,newCalcAlgContext);

							// set new task priority
							double newTaskPriority = calcAlgContext->getUsedTaskPriorityStrategy()->getPriorityForTaskReusing(newSatCalcTask,calcAlgContext->getUsedSatisfiableCalculationTask(),reusingAlternative);
							newSatCalcTask->setTaskPriority(newTaskPriority);

							newTaskIt = (CSatisfiableCalculationTask*)newTaskIt->getNext();
						}

						processorContext->getTaskProcessorCommunicator()->communicateTaskCreation(newTaskList);

						throw CCalculationStopProcessingException(true);
					}
				}




				bool CCalculationTableauCompletionTaskHandleAlgorithm::addReusingBlockerFollowing(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReusingIndividualNodeConceptExpansionData* reusingData = individualNode->getReusingIndividualNodeConceptExpansionData(false);
					if (reusingData) {
						CIndividualProcessNode* blockerIndividualNode = reusingData->getBlockerIndividualNode();
						CIndividualProcessNode* locBlockerIndividualNode = getLocalizedIndividual(blockerIndividualNode,true,calcAlgContext);
						CBlockingFollowSet* followSet = locBlockerIndividualNode->getBlockingFollowSet(true);
						followSet->insert(individualNode->getIndividualID());
						return true;
					}
					return false;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::removeReusingBlockerFollowing(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReusingIndividualNodeConceptExpansionData* reusingData = individualNode->getReusingIndividualNodeConceptExpansionData(false);
					if (reusingData) {
						CIndividualProcessNode* blockerIndividualNode = reusingData->getBlockerIndividualNode();
						CIndividualProcessNode* locBlockerIndividualNode = getLocalizedIndividual(blockerIndividualNode,true,calcAlgContext);
						CBlockingFollowSet* followSet = locBlockerIndividualNode->getBlockingFollowSet(true);
						followSet->remove(individualNode->getIndividualID());
						return true;
					}
					return false;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::isSignatureBlockedProcessingBlocked(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool processingBlocked = false;
					bool sigBlocked = detectIndividualNodeSignatureBlockingStatus(individualNode,calcAlgContext);
					if (sigBlocked) {
						// block processing only for successors of satisfiable cached nodes
						processingBlocked = individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSIGNATUREBLOCKINGCACHED);
					}
					return processingBlocked;
				}




				bool CCalculationTableauCompletionTaskHandleAlgorithm::testAlternativeBlocked(CIndividualProcessNode*& individualNode, CBlockingAlternativeData* blockAltData, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool blocked = false;

					if (blockAltData->getBlockingAlternativeDataType() == CBlockingAlternativeData::BADSIGNATUREBLOCKINGCANDIDATE) {
						if (mConfSignatureMirroringBlocking) {
							CBlockingAlternativeSignatureBlockingCandidateData* sigBlockCandData = (CBlockingAlternativeSignatureBlockingCandidateData*)blockAltData;
							CIndividualProcessNode* blockerNode = sigBlockCandData->getSignatureBlockingCandidateNode();


							STATINC(SIGNATUREMIRRORINGBLOCKINGESTABLISHCOUNT,calcAlgContext);
							cint64 prevBlockerConSetCount = blockerNode->getReapplyConceptLabelSet(false)->getConceptCount();
							if (establishIndividualNodeSignatureBlocking(individualNode,blockerNode,calcAlgContext)) {

								//if (blockerNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION)) {
								//	propagateIndividualNodeNominalConnectionToAncestors(individualNode,calcAlgContext);
								//}
								if (blockerNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION)) {
									propagateIndividualNodeNominalConnectionStatusToAncestors(individualNode,blockerNode,calcAlgContext);
								}

								if (prevBlockerConSetCount != blockerNode->getReapplyConceptLabelSet(false)->getConceptCount()) {
									addIndividualToBlockingUpdateReviewProcessingQueue(individualNode,calcAlgContext);
								}
								addSignatureBlockingBlockerFollowing(individualNode,calcAlgContext);
								STATINC(SIGNATUREMIRRORINGBLOCKINGADDFOLLOWINGCOUNT,calcAlgContext);
								calcAlgContext->getProcessTagger()->incCurrentBlockingFollowTag();

								individualNode->addProcessingRestrictionFlags(CIndividualProcessNode::PRFSIGNATUREBLOCKINGCACHED);
								propagateIndirectSuccessorSignatureBlocked(individualNode,calcAlgContext);

								updateBlockingReviewMarking(individualNode,true,calcAlgContext);
								blocked = true;
							}
						}
					}

					return blocked;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::detectIndividualNodeSignatureBlockingStatus(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					// TODO: 1. search nodes with identical signatures, 2. check signature compatibility, 3. expand concepts and establish blocking status, 4. block successor generation, 
					// 5. hold blocking status as long subset, 6. validate or remove blocking status at the end of completion graph construction


					bool wasBlockingCached = individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSIGNATUREBLOCKINGCACHED | CIndividualProcessNode::PRFANCESTORSIGNATUREBLOCKINGCACHED);

					if (individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSIGNATUREBLOCKINGCACHED)) {
						if (individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSIGNATUREBLOCKINGCACHEDABOLISHED)) {
							individualNode->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSIGNATUREBLOCKINGCACHEDABOLISHED);
							individualNode->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSIGNATUREBLOCKINGCACHED);
						} else {
							return true;
						}
					}
					if (individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFRETESTSIGNATUREBLOCKINGCACHEDDUEDIRECTMODIFIED)) {
						individualNode->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFRETESTSIGNATUREBLOCKINGCACHEDDUEDIRECTMODIFIED);
					}

					bool newBlockingCached = wasBlockingCached;

					if (mConfSignatureMirroringBlocking) {
						bool continueBlockerSearch = true;
						if (individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINVALIDBLOCKINGORCACHING | CIndividualProcessNode::PRFSATURATIONBLOCKINGCACHED)) {
							continueBlockerSearch = false;
						}
						if (wasBlockingCached) {
							STATINC(SIGNATUREMIRRORINGBLOCKINGREFRESHCOUNT,calcAlgContext);
							if (continueBlockerSearch) {
								newBlockingCached = refreshIndividualNodeSignatureBlocking(individualNode,calcAlgContext);
							}
							if (!newBlockingCached) {
								// remove connection from blocker node
								STATINC(SIGNATUREMIRRORINGBLOCKINGREFRESHLOSEDCOUNT,calcAlgContext);
								STATINC(SIGNATUREMIRRORINGBLOCKINGREMOVEFOLLOWINGCOUNT,calcAlgContext);
								removeSignatureBlockingBlockerFollowing(individualNode,calcAlgContext);
							} else {
								calcAlgContext->getProcessTagger()->incCurrentBlockingFollowTag();
							}
						}

						while (continueBlockerSearch && !newBlockingCached) {
							STATINC(SIGNATUREMIRRORINGBLOCKINGSEARCHCOUNT,calcAlgContext);
							CIndividualProcessNode* blockerNode = searchSignatureIndividualNodeBlocker(individualNode,calcAlgContext);
							if (blockerNode) {
								STATINC(SIGNATUREMIRRORINGBLOCKINGESTABLISHCOUNT,calcAlgContext);
								cint64 prevBlockerConSetCount = blockerNode->getReapplyConceptLabelSet(false)->getConceptCount();
								if (establishIndividualNodeSignatureBlocking(individualNode,blockerNode,calcAlgContext)) {

									//if (blockerNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION)) {
									//	propagateIndividualNodeNominalConnectionToAncestors(individualNode,calcAlgContext);
									//}
									if (blockerNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION)) {
										propagateIndividualNodeNominalConnectionStatusToAncestors(individualNode,blockerNode,calcAlgContext);
									}

									newBlockingCached = true;
									if (prevBlockerConSetCount != blockerNode->getReapplyConceptLabelSet(false)->getConceptCount()) {
										addIndividualToBlockingUpdateReviewProcessingQueue(individualNode,calcAlgContext);
									}
									addSignatureBlockingBlockerFollowing(individualNode,calcAlgContext);
									STATINC(SIGNATUREMIRRORINGBLOCKINGADDFOLLOWINGCOUNT,calcAlgContext);
									calcAlgContext->getProcessTagger()->incCurrentBlockingFollowTag();
								}
							} else {
								continueBlockerSearch = false;
							}
						}

						updateBlockingReviewMarking(individualNode,newBlockingCached,calcAlgContext);

						if (newBlockingCached && !wasBlockingCached) {
							// activate caching status
							individualNode->addProcessingRestrictionFlags(CIndividualProcessNode::PRFSIGNATUREBLOCKINGCACHED);
							propagateIndirectSuccessorSignatureBlocked(individualNode,calcAlgContext);
						} else if (wasBlockingCached && !newBlockingCached) {
							// deactivate caching status
							individualNode->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFSIGNATUREBLOCKINGCACHED);
							reactivateIndirectSignatureBlockedSuccessors(individualNode,false,calcAlgContext);
							reapplySatisfiableCachedAbsorbedGeneratingConcepts(individualNode,calcAlgContext);
						}
					}
					return newBlockingCached;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::addSignatureBlockingBlockerFollowing(CIndividualProcessNode*& blockingIndividualNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSignatureBlockingIndividualNodeConceptExpansionData* sigBlockingData = blockingIndividualNode->getSignatureBlockingIndividualNodeConceptExpansionData(false);
					if (sigBlockingData) {
						CIndividualProcessNode* blockerIndividualNode = sigBlockingData->getBlockerIndividualNode();
						CIndividualProcessNode* locBlockerIndividualNode = getLocalizedIndividual(blockerIndividualNode,true,calcAlgContext);
						CBlockingFollowSet* sigFollowSet = locBlockerIndividualNode->getBlockingFollowSet(true);
						sigFollowSet->insert(blockingIndividualNode->getIndividualID());
						return true;
					}
					return false;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::removeSignatureBlockingBlockerFollowing(CIndividualProcessNode*& blockingIndividualNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSignatureBlockingIndividualNodeConceptExpansionData* sigBlockingData = blockingIndividualNode->getSignatureBlockingIndividualNodeConceptExpansionData(false);
					if (sigBlockingData) {
						CIndividualProcessNode* blockerIndividualNode = sigBlockingData->getBlockerIndividualNode();
						CIndividualProcessNode* locBlockerIndividualNode = getLocalizedIndividual(blockerIndividualNode,true,calcAlgContext);
						CBlockingFollowSet* sigFollowSet = locBlockerIndividualNode->getBlockingFollowSet(true);
						sigFollowSet->remove(blockingIndividualNode->getIndividualID());
						return true;
					}
					return false;
				}




				void CCalculationTableauCompletionTaskHandleAlgorithm::rebuildSignatureBlockingCandidateHash(CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CProcessContext* processContext = calcAlgContext->getUsedProcessContext();


					CProcessingDataBox* processingDataBox = calcAlgContext->getProcessingDataBox();
					CSignatureBlockingCandidateHash* sigBlockCandHash = processingDataBox->getSignatureBlockingCandidateHash(false);

					if (sigBlockCandHash) {

						CSignatureBlockingCandidateHash* newSigBlockCandHash = CObjectParameterizingAllocator< CSignatureBlockingCandidateHash,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,processContext);

						CSignatureIterator sigIt(sigBlockCandHash->getSignatureIterator());
						while (sigIt.hasNext()) {
							cint64 signature = sigIt.getSignature();
							CXLinker<cint64>* newCandidateLinker = nullptr;
							CXLinker<cint64>* candidateLinker = sigIt.getCandidateLinker();
							for (CXLinker<cint64>* candidateLinkerIt = candidateLinker; candidateLinkerIt; candidateLinkerIt = candidateLinkerIt->getNext()) {
								cint64 candIndiID = candidateLinkerIt->getData();
								CIndividualProcessNode* candIndiNode = getUpToDateIndividual(candIndiID,calcAlgContext);
								if (isIndividualNodeValidBlocker(candIndiNode,calcAlgContext)) {
									CXLinker<cint64>* tmpNewCandLinker = CObjectAllocator< CXLinker<cint64> >::allocateAndConstruct(taskMemMan);
									newCandidateLinker = tmpNewCandLinker->initLinker(candIndiNode->getIndividualID(),newCandidateLinker);
								}
							}
							if (newCandidateLinker) {
								newSigBlockCandHash->insertSignatureBlockingCandidates(signature,newCandidateLinker);
							}
							sigIt.moveNext();
						}
						processingDataBox->setSignatureBlockingCandidateHash(newSigBlockCandHash);
					}
				}


				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::searchSignatureIndividualNodeBlocker(CIndividualProcessNode*& blockingNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* processingDataBox = calcAlgContext->getProcessingDataBox();
					CSignatureBlockingCandidateHash* sigBlockCandHash = processingDataBox->getSignatureBlockingCandidateHash(false);
					CReapplyConceptLabelSet* conSet = blockingNode->getReapplyConceptLabelSet(false);
					if (sigBlockCandHash && conSet) {
						cint64 conCount = conSet->getConceptCount();
						if (!blockingNode->isInvalidSignatureBlocking() && blockingNode->getLastConceptCountSearchBlockingCandidate() != conCount) {
							blockingNode->setLastConceptCountSearchBlockingCandidate(conCount);

							cint64 conSig = conSet->getConceptSignatureValue();
							cint64 newCandCount = sigBlockCandHash->getBlockingCandidatesCount(conSig);
							cint64 lastCandCount = blockingNode->getLastSearchBlockerCandidateCount();
							cint64 lastCandSignature = blockingNode->getLastSearchBlockerCandidateSignature();
							if (lastCandSignature != conSig) {
								lastCandCount = 0;
							}
							blockingNode->setLastSearchBlockerCandidateSignature(conSig);
							if (newCandCount != lastCandCount) {

								cint64 candDiffCount = newCandCount - lastCandCount;

								CSignatureBlockingCandidateIterator candIt(sigBlockCandHash->getBlockingCandidatesIterator(conSig));
								while (candIt.hasNext() && candDiffCount-- > 0 && !blockingNode->isInvalidSignatureBlocking()) {
									cint64 candIndiID = candIt.next(true);
									if (candIndiID != blockingNode->getIndividualID()) {
										CIndividualProcessNode* candIndiNode = getUpToDateIndividual(candIndiID,calcAlgContext);
										STATINC(SIGNATUREMIRRORINGBLOCKINGCANDIDATEREGARDEDCOUNT,calcAlgContext);
										if (isIndividualNodeValidBlocker(candIndiNode,calcAlgContext)) {
											bool compatible = hasCompatibleConceptSetSignature(blockingNode,conSet,candIndiNode,calcAlgContext);
											if (compatible) {
												blockingNode->setLastSearchBlockerCandidateCount(newCandCount-candDiffCount);
												return candIndiNode;
											} else {
												STATINC(SIGNATUREMIRRORINGBLOCKINGCANDIDATEREGARDEDINCOMPATIBLECOUNT,calcAlgContext);
											}
										} else {
											STATINC(SIGNATUREMIRRORINGBLOCKINGCANDIDATEREGARDEDINVALIDCOUNT,calcAlgContext);
										}
									}
								}
								blockingNode->setLastSearchBlockerCandidateCount(newCandCount);

							}
						}
					}
					return nullptr;
				}





				bool CCalculationTableauCompletionTaskHandleAlgorithm::addSignatureIndividualNodeBlockerCandidate(CIndividualProcessNode*& indiNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (indiNode->getBlockingCachingSavedCandidateCount() <= mMaxBlockingCachingSavedCandidateCount) {
						indiNode->incBlockingCachingSavedCandidateCount();
						if (isIndividualNodeValidBlocker(indiNode,calcAlgContext)) {
							CReapplyConceptLabelSet* conSet = indiNode->getReapplyConceptLabelSet(false);
							if (conSet) {
								cint64 conCount = conSet->getConceptCount();
								if (indiNode->getLastConceptCountCachedBlockingCandidate() != conCount) { 
									STATINC(SIGNATURESAVINGCOUNT,calcAlgContext);
									cint64 conSig = conSet->getConceptSignatureValue();
									CProcessingDataBox* processingDataBox = calcAlgContext->getProcessingDataBox();
									CSignatureBlockingCandidateHash* sigBlockCandHash = processingDataBox->getSignatureBlockingCandidateHash(true);
									sigBlockCandHash->insertSignatureBlockingCandidate(conSig,indiNode);
									indiNode->setLastConceptCountCachedBlockingCandidate(conCount);
									return true;
								}
							}
						}
					}
					return false;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::establishIndividualNodeSignatureBlocking(CIndividualProcessNode*& blockingIndividualNode, CIndividualProcessNode*& blockerIndividualNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool blockingEstablished = false;
					anlyzeIndiviudalNodesConceptExpansion(blockerIndividualNode,calcAlgContext);
					CIndividualNodeAnalizedConceptExpansionData* blockerAnalizedConExpData = blockerIndividualNode->getAnalizedConceptExpansionData(false);
					if (blockerAnalizedConExpData) {
						if (!blockerAnalizedConExpData->isInvalidBlocker()) {
							CReapplyConceptLabelSet* blockingConSet = blockingIndividualNode->getReapplyConceptLabelSet(true);
							cint64 blockingConSetCount = blockingConSet->getConceptCount();
							cint64 blockingConSetSignature = blockingConSet->getConceptSignatureValue();
							CConceptDescriptor* blockingLastConDes = blockingConSet->getAddingSortedConceptDescriptionLinker();
							CSignatureBlockingIndividualNodeConceptExpansionData* locSigBlockingData = blockingIndividualNode->getSignatureBlockingIndividualNodeConceptExpansionData(true);
							if (!locSigBlockingData) {
								CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
								CSignatureBlockingIndividualNodeConceptExpansionData* sigBlockingData = blockingIndividualNode->getSignatureBlockingIndividualNodeConceptExpansionData(false);
								locSigBlockingData = CObjectAllocator<CSignatureBlockingIndividualNodeConceptExpansionData>::allocateAndConstruct(taskMemMan);
								locSigBlockingData->initBlockingExpansionData(sigBlockingData);
								blockingIndividualNode->setSignatureBlockingIndividualNodeConceptExpansionData(locSigBlockingData);
							}
							locSigBlockingData->setBlockingConceptCount(blockingConSetCount);
							locSigBlockingData->setBlockingConceptSignature(blockingConSetSignature);
							locSigBlockingData->setLastSubsetTestedConceptDescriptor(blockingLastConDes);
							locSigBlockingData->setContinuousExpandedContainedConceptCount(0);
							locSigBlockingData->setBlockerIndividualNode(blockerIndividualNode);
							locSigBlockingData->setLastUpdatedConceptCount(0);
							locSigBlockingData->setLastUpdatedConceptExpansionCount(0);

							updateSignatureBlockingConceptExpansion(blockingIndividualNode,locSigBlockingData,blockerIndividualNode,blockerAnalizedConExpData,calcAlgContext);


							// set blocking status
							blockingEstablished = true;

							// is still subset after added expansions concepts
							CReapplyConceptLabelSet* blockerConSet = blockerIndividualNode->getReapplyConceptLabelSet(false);
							if (locSigBlockingData->isIdenticConceptSetRequired()) {
								if (blockingConSet->getConceptSignatureValue() != blockerConSet->getConceptSignatureValue()) {
									return false;
								}
								if (blockingConSet->getConceptCount() != blockerConSet->getConceptCount()) {
									return false;
								}
							}
							CConceptDescriptor* lastSubSetTestConDes = locSigBlockingData->getLastSubsetTestedConceptDescriptor();
							CConceptDescriptor* addingSortedConDes = blockingConSet->getAddingSortedConceptDescriptionLinker();
							if (addingSortedConDes != lastSubSetTestConDes) {
								bool stillSubset = true;
								CConceptDescriptor* addingSortedConDesIt = addingSortedConDes;
								while (blockingEstablished && addingSortedConDesIt != lastSubSetTestConDes && stillSubset) {
									CConcept* concept = addingSortedConDesIt->getConcept();
									if (!blockerConSet->containsConcept(concept)) {
										stillSubset = false;
									}
									addingSortedConDesIt = addingSortedConDesIt->getNext();
								}
								if (!stillSubset && mOptSignatureMirroringBlockingForceSubset) {
									blockingEstablished = false;
								}
								locSigBlockingData->setConceptSetStillSubset(stillSubset);
								if (stillSubset) {
									locSigBlockingData->setLastSubsetTestedConceptDescriptor(addingSortedConDes);
								}
							}			
						} else {
							blockingIndividualNode->setInvalidSignatureBlocking(true);
							blockingEstablished = false;
						}
					}
					return blockingEstablished;						 
				}




				bool CCalculationTableauCompletionTaskHandleAlgorithm::refreshIndividualNodeSignatureBlocking(CIndividualProcessNode*& blockingIndividualNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSignatureBlockingIndividualNodeConceptExpansionData* sigBlockingData = blockingIndividualNode->getSignatureBlockingIndividualNodeConceptExpansionData(false);
					if (sigBlockingData) {
						CIndividualProcessNode* blockerIndividualNode = sigBlockingData->getBlockerIndividualNode();
						blockerIndividualNode = getUpToDateIndividual(blockerIndividualNode,calcAlgContext);
						if (!isIndividualNodeValidBlocker(blockerIndividualNode,calcAlgContext)) {
							return false;
						}
						if (blockingIndividualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINVALIDBLOCKINGORCACHING)) {
							return false;
						}
						anlyzeIndiviudalNodesConceptExpansion(blockerIndividualNode,calcAlgContext);
						// test is still subset?
						CReapplyConceptLabelSet* blockingConSet = blockingIndividualNode->getReapplyConceptLabelSet(false);
						CReapplyConceptLabelSet* blockerConSet = blockerIndividualNode->getReapplyConceptLabelSet(false);
						CConceptDescriptor* lastSubSetTestConDes = sigBlockingData->getLastSubsetTestedConceptDescriptor();
						CConceptDescriptor* addingSortedConDes = blockingConSet->getAddingSortedConceptDescriptionLinker();
						CIndividualNodeAnalizedConceptExpansionData* blockerAnalizedConExpData = blockerIndividualNode->getAnalizedConceptExpansionData(false);
						if (!blockerAnalizedConExpData->isInvalidBlocker()) {
							if (addingSortedConDes != lastSubSetTestConDes || blockerAnalizedConExpData->getExpansionConceptCount() != sigBlockingData->getLastUpdatedConceptExpansionCount()) {
								bool stillSubset = true;
								CConceptDescriptor* addingSortedConDesIt = addingSortedConDes;
								while (addingSortedConDesIt != lastSubSetTestConDes && stillSubset) {
									CConcept* concept = addingSortedConDesIt->getConcept();
									bool conNegation = addingSortedConDesIt->isNegated();
									if (!blockerConSet->containsConcept(concept,conNegation)) {
										stillSubset = false;
									}
									addingSortedConDesIt = addingSortedConDesIt->getNext();
								}
								if (!stillSubset && mOptSignatureMirroringBlockingForceSubset) {
									return false;
								}
								CSignatureBlockingIndividualNodeConceptExpansionData* locSigBlockingData = blockingIndividualNode->getSignatureBlockingIndividualNodeConceptExpansionData(true);
								if (!locSigBlockingData) {
									CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
									CSignatureBlockingIndividualNodeConceptExpansionData* sigBlockingData = blockingIndividualNode->getSignatureBlockingIndividualNodeConceptExpansionData(false);
									locSigBlockingData = CObjectAllocator<CSignatureBlockingIndividualNodeConceptExpansionData>::allocateAndConstruct(taskMemMan);
									locSigBlockingData->initBlockingExpansionData(sigBlockingData);
									blockingIndividualNode->setSignatureBlockingIndividualNodeConceptExpansionData(locSigBlockingData);
								}
								locSigBlockingData->setLastSubsetTestedConceptDescriptor(addingSortedConDes);

								// update expansions concepts
								updateSignatureBlockingConceptExpansion(blockingIndividualNode,locSigBlockingData,blockerIndividualNode,blockerAnalizedConExpData,calcAlgContext);

								if (mConfDirectRulePreprocessing || locSigBlockingData->isIdenticConceptSetRequired()) {
									blockingConSet = blockingIndividualNode->getReapplyConceptLabelSet(false);
									if (locSigBlockingData->isIdenticConceptSetRequired()) {
										if (blockingConSet->getConceptSignatureValue() != blockerConSet->getConceptSignatureValue()) {
											return false;
										}
										if (blockingConSet->getConceptCount() != blockerConSet->getConceptCount()) {
											return false;
										}
									}
									// validate still subset
									addingSortedConDes = blockingConSet->getAddingSortedConceptDescriptionLinker();
									lastSubSetTestConDes = locSigBlockingData->getLastSubsetTestedConceptDescriptor();
									if (addingSortedConDes != lastSubSetTestConDes) {
										CConceptDescriptor* addingSortedConDesIt = addingSortedConDes;
										while (addingSortedConDesIt != lastSubSetTestConDes && stillSubset) {
											CConcept* concept = addingSortedConDesIt->getConcept();
											bool conNegation = addingSortedConDesIt->isNegated();
											if (!blockerConSet->containsConcept(concept,conNegation)) {
												stillSubset = false;
											}
											addingSortedConDesIt = addingSortedConDesIt->getNext();
										}
										locSigBlockingData->setConceptSetStillSubset(stillSubset);
										if (stillSubset) {
											locSigBlockingData->setLastSubsetTestedConceptDescriptor(addingSortedConDes);
										} else {
											if (mOptSignatureMirroringBlockingForceSubset) {
												return false;
											}
										}
									}
								}
							}
							return true;
						} else {
							blockingIndividualNode->setInvalidSignatureBlocking(true);
							return false;
						}
					}
					return false;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::updateBlockingReviewMarking(CIndividualProcessNode*& blockingIndividualNode, bool isBlocked, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSignatureBlockingIndividualNodeConceptExpansionData* sigBlockingData = blockingIndividualNode->getSignatureBlockingIndividualNodeConceptExpansionData(false);
					cint64 indiID = blockingIndividualNode->getIndividualID();
					if (sigBlockingData) {
						if (isBlocked && !sigBlockingData->isBlockingReviewMarked()) {
							CIndividualProcessNode* blockerNode = getUpToDateIndividual(sigBlockingData->getBlockerIndividualNode(),calcAlgContext);
							if (blockerNode->getReapplyConceptLabelSet(false)->getConceptCount() != blockingIndividualNode->getReapplyConceptLabelSet(false)->getConceptCount() || !sigBlockingData->isConceptSetStillSubset()) {
								CProcessingDataBox* processingDataBox = calcAlgContext->getProcessingDataBox();
								CSignatureBlockingReviewSet* revSet = processingDataBox->getSignatureBlockingReviewSet(true);
								
								revSet->getReviewData(sigBlockingData->isConceptSetStillSubset())->insert(blockingIndividualNode->getIndividualAncestorDepth(),indiID);

								CSignatureBlockingIndividualNodeConceptExpansionData* locSigBlockingData = blockingIndividualNode->getSignatureBlockingIndividualNodeConceptExpansionData(true);
								if (!locSigBlockingData) {
									CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
									locSigBlockingData = CObjectAllocator<CSignatureBlockingIndividualNodeConceptExpansionData>::allocateAndConstruct(taskMemMan);
									locSigBlockingData->initBlockingExpansionData(sigBlockingData);
									blockingIndividualNode->setSignatureBlockingIndividualNodeConceptExpansionData(locSigBlockingData);
								}
								locSigBlockingData->setBlockingReviewMarked(true);
								locSigBlockingData->setBlockingSubsetReviewMarked(sigBlockingData->isConceptSetStillSubset());
								return true;
							}
						} 
						if (sigBlockingData->isBlockingReviewMarked()) {
							CIndividualProcessNode* blockerNode = getUpToDateIndividual(sigBlockingData->getBlockerIndividualNode(),calcAlgContext);
							if (!isBlocked || blockerNode->getReapplyConceptLabelSet(false)->getConceptCount() == blockingIndividualNode->getReapplyConceptLabelSet(false)->getConceptCount() && sigBlockingData->isConceptSetStillSubset()) {
								CProcessingDataBox* processingDataBox = calcAlgContext->getProcessingDataBox();
								CSignatureBlockingReviewSet* revSet = processingDataBox->getSignatureBlockingReviewSet(true);

								revSet->getReviewData(sigBlockingData->isBlockingSubsetReviewMarked())->remove(indiID);

								CSignatureBlockingIndividualNodeConceptExpansionData* locSigBlockingData = blockingIndividualNode->getSignatureBlockingIndividualNodeConceptExpansionData(true);
								if (!locSigBlockingData) {
									CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
									locSigBlockingData = CObjectAllocator<CSignatureBlockingIndividualNodeConceptExpansionData>::allocateAndConstruct(taskMemMan);
									locSigBlockingData->initBlockingExpansionData(sigBlockingData);
									blockingIndividualNode->setSignatureBlockingIndividualNodeConceptExpansionData(locSigBlockingData);
								}
								locSigBlockingData->setBlockingReviewMarked(false);
								locSigBlockingData->setBlockingSubsetReviewMarked(sigBlockingData->isConceptSetStillSubset());

								return true;

							} else if (sigBlockingData->isBlockingSubsetReviewMarked() != sigBlockingData->isConceptSetStillSubset()) {
								CProcessingDataBox* processingDataBox = calcAlgContext->getProcessingDataBox();
								CSignatureBlockingReviewSet* revSet = processingDataBox->getSignatureBlockingReviewSet(true);

								revSet->getReviewData(sigBlockingData->isBlockingSubsetReviewMarked())->remove(indiID);
								revSet->getReviewData(sigBlockingData->isConceptSetStillSubset())->insert(blockingIndividualNode->getIndividualAncestorDepth(),indiID);

								CSignatureBlockingIndividualNodeConceptExpansionData* locSigBlockingData = blockingIndividualNode->getSignatureBlockingIndividualNodeConceptExpansionData(true);
								if (!locSigBlockingData) {
									CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
									locSigBlockingData = CObjectAllocator<CSignatureBlockingIndividualNodeConceptExpansionData>::allocateAndConstruct(taskMemMan);
									locSigBlockingData->initBlockingExpansionData(sigBlockingData);
									blockingIndividualNode->setSignatureBlockingIndividualNodeConceptExpansionData(locSigBlockingData);
								}
								locSigBlockingData->setBlockingReviewMarked(false);
								locSigBlockingData->setBlockingSubsetReviewMarked(sigBlockingData->isConceptSetStillSubset());

								return true;
							}
						}
					}
					return false;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::updateSignatureBlockingConceptExpansion(CIndividualProcessNode*& blockingIndividualNode, CSignatureBlockingIndividualNodeConceptExpansionData* sigBlockingData, CIndividualProcessNode*& blockerIndividualNode, CIndividualNodeAnalizedConceptExpansionData* blockerAnalizedConExpData, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReapplyConceptLabelSet* blockingConSet = blockingIndividualNode->getReapplyConceptLabelSet(true);
					cint64 lastUpdatedConExpCount = sigBlockingData->getLastUpdatedConceptExpansionCount();

					bool updateDueChangedConcepts = blockingConSet->getConceptCount() != sigBlockingData->getLastUpdatedConceptCount();
					bool updateDueChangedExpansions = blockerAnalizedConExpData->getExpansionConceptCount() > lastUpdatedConExpCount;

					if (updateDueChangedConcepts || updateDueChangedExpansions) {
						bool retestAllExpansionsConcepts = true;
						if (lastUpdatedConExpCount > 0 && updateDueChangedExpansions && !updateDueChangedConcepts) {
							retestAllExpansionsConcepts = false;
						}
						cint64 continuousExpConConceptCount = sigBlockingData->getContinuousExpandedContainedConceptCount();
						CAnalizedConceptExpansionLinker* analizedConExpLinkerIt = blockerAnalizedConExpData->getReverseAnalizedConceptExpansionLinker();
						cint64 skipExpConCount = continuousExpConConceptCount;
						if (!retestAllExpansionsConcepts) {
							skipExpConCount = lastUpdatedConExpCount;
						}
						cint64 conExpSearchCount = blockerAnalizedConExpData->getExpansionConceptCount() - skipExpConCount;

						//for (cint64 i = 0; i < skipExpConCount; ++i) {
						//	analizedConExpLinkerIt = analizedConExpLinkerIt->getNext();
						//}
						cint64 updateContinuousExpandedContainedConceptCount = -1;
						if (!retestAllExpansionsConcepts && sigBlockingData->getContinuousExpandedContainedConceptCount() != sigBlockingData->getLastUpdatedConceptExpansionCount()) {
							updateContinuousExpandedContainedConceptCount = 0;
						}
						sigBlockingData->setLastUpdatedConceptExpansionCount(blockerAnalizedConExpData->getExpansionConceptCount());
						cint64 expandedContainedConceptCount = 0;

						if (sigBlockingData->isConceptSetStillSubset() && blockingConSet->getConceptCount() == blockerAnalizedConExpData->getLastConceptCount()) {
							// no expansion required
							sigBlockingData->setContinuousExpandedContainedConceptCount(blockerAnalizedConExpData->getExpansionConceptCount());
							sigBlockingData->setLastUpdatedConceptCount(blockingConSet->getConceptCount());
						} else {

							CPROCESSINGLIST<CAnalizedConceptExpansionLinker*> revAnalizedConExpList(calcAlgContext->getTaskProcessorContext());
							while (conExpSearchCount-- > 0 && analizedConExpLinkerIt) {
								revAnalizedConExpList.prepend(analizedConExpLinkerIt);
								analizedConExpLinkerIt = analizedConExpLinkerIt->getNext();
							}

							for (CPROCESSINGLIST<CAnalizedConceptExpansionLinker*>::const_iterator it = revAnalizedConExpList.constBegin(), itEnd = revAnalizedConExpList.constEnd(); it != itEnd; ++it) {
								CAnalizedConceptExpansionLinker* nextAnalizedConExpLinker = *it;
								CConceptDescriptor* expConDes = nextAnalizedConExpLinker->getConceptDescriptor();
								CConcept* expConcept = expConDes->getConcept();
								bool expConNegation = expConDes->getNegation();
								if (!blockingConSet->containsConcept(expConcept,expConNegation)) {

									bool allDependenciesExistings = true;
									CDependency* dependencies = nullptr;
									CDependencyTrackPoint* firstDepTrackPoint = nullptr;

									CXLinker<CConceptDescriptor*>* depLinker = nextAnalizedConExpLinker->getDependendConceptDescriptorLinker();
									CXLinker<CConceptDescriptor*>* depLinkerIt = depLinker;
									while (allDependenciesExistings && depLinkerIt) {
										CConceptDescriptor* depExpConDes = depLinkerIt->getData();
										cint64 depConTag = depExpConDes->getConceptTag();
										CConceptDescriptor* depConDes = nullptr;
										CDependencyTrackPoint* depDepTrackPoint = nullptr;
										if (blockingConSet->getConceptDescriptor(depConTag,depConDes,depDepTrackPoint)) {
											if (depConDes->isNegated() == depExpConDes->isNegated()) {
												KONCLUDE_ASSERT_X(depDepTrackPoint,"expandCachedConcepts","missing dependency");

												CCONNECTIONDependencyNode* connDepNode = createCONNECTIONDependency(blockingIndividualNode,depConDes,depDepTrackPoint,calcAlgContext);
												if (!firstDepTrackPoint) {
													firstDepTrackPoint = connDepNode;
												} else {
													dependencies = connDepNode->append(dependencies);
												}
											} else {
												allDependenciesExistings = false;
											}

										} else {
											allDependenciesExistings = false;
										}
										depLinkerIt = depLinkerIt->getNext();
									}

									if (allDependenciesExistings) {
										STATINC(SIGNATUREMIRRORINGBLOCKINGCONCEPTEXPANSIONCOUNT,calcAlgContext);
										KONCLUDE_ASSERT_X(firstDepTrackPoint,"expandCachedConcepts","missing dependency");

										CDependencyTrackPoint* expDepTrackPoint = nullptr;
										CEXPANDEDDependencyNode* expDepNode = createEXPANDEDDependency(expDepTrackPoint,blockingIndividualNode,firstDepTrackPoint,dependencies,calcAlgContext);

										addConceptToIndividualSkipANDProcessing(expConcept,expConNegation,blockingIndividualNode,expDepTrackPoint,true,false,calcAlgContext);
										++expandedContainedConceptCount;
									} else {
										if (updateContinuousExpandedContainedConceptCount < 0) {
											updateContinuousExpandedContainedConceptCount = expandedContainedConceptCount;
										}
									}
								} else {
									++expandedContainedConceptCount;
								}
							}
							if (updateContinuousExpandedContainedConceptCount < 0) {
								updateContinuousExpandedContainedConceptCount = expandedContainedConceptCount;
							}
							sigBlockingData->setContinuousExpandedContainedConceptCount(continuousExpConConceptCount+updateContinuousExpandedContainedConceptCount);
							sigBlockingData->setLastUpdatedConceptCount(blockingConSet->getConceptCount());
						}

						return true;

					}
					return false;
				}




				bool CCalculationTableauCompletionTaskHandleAlgorithm::hasCompatibleConceptSetSignature(CIndividualProcessNode*& blockingNode, CReapplyConceptLabelSet* conSet, CIndividualProcessNode* compatibleTestNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					// is conSet a compatible concept set to compatibleTestNode
					CReapplyConceptLabelSet* compTestConSet = compatibleTestNode->getReapplyConceptLabelSet(false);
					cint64 conCount = conSet->getConceptCount();
					cint64 compTestConCount = compTestConSet->getConceptCount();
					if (conCount <= 0 || compTestConCount < conCount) {
						return false;
					}
					cint64 diffCount = compTestConCount - conCount;
					CConceptDescriptor* compTestConDesIt = compTestConSet->getAddingSortedConceptDescriptionLinker();
					CConceptDescriptor* conDesIt = conSet->getAddingSortedConceptDescriptionLinker();
					while (diffCount-- > 0) {
						compTestConDesIt = compTestConDesIt->getNext();
					}
					bool orderingCompatible = true;
					while (orderingCompatible && conDesIt) {
						CConcept* concept = conDesIt->getConcept();
						bool conNeg = conDesIt->getNegation();
						if (compTestConDesIt->getData() == concept && compTestConDesIt->getNegation() == conNeg) {							
							if (isConceptSignatureBlockingCritical(blockingNode,conDesIt,conDesIt->getDependencyTrackPoint(),calcAlgContext)) {
								blockingNode->setInvalidSignatureBlocking(true);
								return false;
							}
							conDesIt = conDesIt->getNext();
							compTestConDesIt = compTestConDesIt->getNext();
						} else {
							orderingCompatible = false;
						}
					}
					if (!orderingCompatible) {
						while (conDesIt) {
							CConcept* concept = conDesIt->getConcept();
							bool conNeg = conDesIt->getNegation();
							if (isConceptSignatureBlockingCritical(blockingNode,conDesIt,conDesIt->getDependencyTrackPoint(),calcAlgContext)) {
								blockingNode->setInvalidSignatureBlocking(true);
								return false;
							}
							if (!conSet->containsConcept(concept,conNeg)) {
								return false;
							}
							conDesIt = conDesIt->getNext();
						}
					}
					return true;
				}




				bool CCalculationTableauCompletionTaskHandleAlgorithm::anlyzeIndiviudalNodesConceptExpansion(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CReapplyConceptLabelSet* conSet = individualNode->getReapplyConceptLabelSet(false);
					if (conSet) {
						CConceptDescriptor* lastAddedConDes = conSet->getAddingSortedConceptDescriptionLinker();
						CIndividualNodeAnalizedConceptExpansionData* anlConExpData = individualNode->getAnalizedConceptExpansionData(false);
						bool update = false;
						if (anlConExpData) {
							if (!anlConExpData->isInvalidBlocker()) {
								CConceptDescriptor* lastAnalyzedConDes = anlConExpData->getLastConceptDescriptor();
								if (lastAddedConDes != lastAnalyzedConDes) {
									update = true;
								}
							}
						} else {
							if (lastAddedConDes) {
								update = true;
							}
						}
						if (update) {
							cint64 conSignature = conSet->getConceptSignatureValue();
							cint64 conceptCount = conSet->getConceptCount();
							individualNode = getLocalizedIndividual(individualNode,false,calcAlgContext);
							CIndividualNodeAnalizedConceptExpansionData* locAnlConExpData = individualNode->getAnalizedConceptExpansionData(true);
							cint64 minValidConCount = locAnlConExpData->getMinimalValidConceptCountLimit();
							CConceptDescriptor* lastAnalizedConDes = locAnlConExpData->getLastConceptDescriptor();


							CAnalizedConceptExpansionLinker* lastAnaConExpLinker = nullptr;
							CAnalizedConceptExpansionLinker* firstAnaConExpLinker = nullptr;

							CConceptDescriptor* conDesIt = lastAddedConDes;
							cint64 currAnalizingCount = conceptCount;
							bool invalidBlocking = locAnlConExpData->isInvalidBlocker();
							CXLinker<CConceptDescriptor*>* nonDetConLinker = nullptr;
							while (conDesIt && conDesIt != lastAnalizedConDes && !invalidBlocking) {
								CConceptDescriptor* conDes = conDesIt;
								CDependencyTrackPoint* depTrackPoint = conDes->getDependencyTrackPoint();
								if (isConceptSignatureBlockingCritical(individualNode,conDes,depTrackPoint,calcAlgContext)) {
									invalidBlocking = true;
								} else {
									bool directDependentFlag = false;
									if (isConceptFromDirectOrPredecessorOrNondeterminismusDependent(individualNode,conDes,depTrackPoint,&directDependentFlag,calcAlgContext)) {
										if (!directDependentFlag) {
											// do not add to expansions concepts
											CXLinker<CConceptDescriptor*>* newNonDetConLinker = CObjectAllocator< CXLinker<CConceptDescriptor*> >::allocateAndConstruct(taskMemMan);
											newNonDetConLinker->initLinker(conDes);
											nonDetConLinker = newNonDetConLinker->append(nonDetConLinker);
										}
									} else {
										// try to find dependencies
										CXLinker<CConceptDescriptor*>* depLinker = nullptr;
										if (getConceptDependenciesToSameIndividualNode(individualNode,conDes,depTrackPoint,depLinker,calcAlgContext)) {
											CAnalizedConceptExpansionLinker* analizedConExpLinker = CObjectAllocator<CAnalizedConceptExpansionLinker>::allocateAndConstruct(taskMemMan);
											analizedConExpLinker->initAnalizedConceptExpansion(depLinker,conDes);		
											if (lastAnaConExpLinker) {
												lastAnaConExpLinker->setNext(analizedConExpLinker);
												lastAnaConExpLinker = analizedConExpLinker;
											} else {
												lastAnaConExpLinker = analizedConExpLinker;
												firstAnaConExpLinker = analizedConExpLinker;
											}
										} else {
											CXLinker<CConceptDescriptor*>* newNonDetConLinker = CObjectAllocator< CXLinker<CConceptDescriptor*> >::allocateAndConstruct(taskMemMan);
											newNonDetConLinker->initLinker(conDes);
											nonDetConLinker = newNonDetConLinker->append(nonDetConLinker);
										}
									}
								}
								conDesIt = conDesIt->getNextConceptDesciptor();
								--currAnalizingCount;
							}

							if (firstAnaConExpLinker) {
								locAnlConExpData->addAnalizedConceptExpansionLinker(firstAnaConExpLinker);
							}
							locAnlConExpData->addAnalysedNonDeterministicConceptExpansionLinker(nonDetConLinker);
							locAnlConExpData->setInvalidBlocker(invalidBlocking);

							locAnlConExpData->setLastConceptDescriptor(lastAddedConDes);
							locAnlConExpData->setLastConceptSignature(conSignature);
							locAnlConExpData->setLastConceptCount(conceptCount);
							return true;
						}
					}
					return false;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::isConceptSignatureBlockingCritical(CIndividualProcessNode*& individualNode, CConceptDescriptor* conDes, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConcept* concept = conDes->getConcept();
					bool conNeg = conDes->getNegation();
					cint64 opCode = concept->getOperatorCode();
					cint64 param = concept->getParameter();
					cint64 cardinality = concept->getParameter() + 1*conNeg;
					if (cardinality > 1 && (!conNeg && opCode == CCATMOST || conNeg && opCode == CCATLEAST)) {
						return true;
					}
					return false;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::isConceptFromDirectOrPredecessorOrNondeterminismusDependent(CIndividualProcessNode*& individualNode, CConceptDescriptor* conDes, CDependencyTrackPoint* depTrackPoint, bool* directDependentFlag, CCalculationAlgorithmContextBase* calcAlgContext) {
					cint64 ancDepth = individualNode->getIndividualAncestorDepth();
					bool dependencyToAncestor = false;
					CDependencyNode* depNode = depTrackPoint->getDependencyNode();
					if (!depTrackPoint->isPointingToDeterministicDependencyNode()) {
						dependencyToAncestor = true;
					} else {
						if (depTrackPoint->isPointingToIndependentDependencyNode()) {
							dependencyToAncestor = true;
						} else if (depNode->hasAppropriateIndividualNode()) {
							CIndividualProcessNode* appIndiNode = depNode->getAppropriateIndividualNode();
							cint64 appIndiAncDepth = appIndiNode->getIndividualAncestorDepth();
							if (appIndiAncDepth < ancDepth) {
								dependencyToAncestor = true;
							} else if (/*mConfDirectRulePreprocessing && */appIndiAncDepth == ancDepth) {
								dependencyToAncestor = !depNode->hasAdditionalDependencies();
								if (*directDependentFlag) {
									*directDependentFlag = true;
								}
							}
						} else {
							CDependencyNode::DEPENDENCNODEYTYPE depType = depNode->getDependencyType();
							if (depType == CDependencyNode::DNTMERGEDCONCEPT) {
								dependencyToAncestor = true;
							}
						}
					}
					return dependencyToAncestor;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::getConceptDependenciesToSameIndividualNode(CIndividualProcessNode*& individualNode, CConceptDescriptor* conDes, CDependencyTrackPoint* depTrackPoint, CXLinker<CConceptDescriptor*>*& depLinker, CCalculationAlgorithmContextBase* calcAlgContext) {					
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					CDependencyNode* depNode = depTrackPoint->getDependencyNode();
					CConceptDescriptor* dependendConDes = depNode->getConceptDescriptor();

					bool simpleSameNodeDeps = false;
					if (depNode->hasAppropriateIndividualNode()) {
						CIndividualProcessNode* appIndiNode = depNode->getAppropriateIndividualNode();
						cint64 appIndiDepth = appIndiNode->getIndividualAncestorDepth();
						cint64 baseDepth = individualNode->getIndividualAncestorDepth();
						if (appIndiDepth == baseDepth) {
							simpleSameNodeDeps = true;
						}
					} else {
						simpleSameNodeDeps = true;
					}
					if (simpleSameNodeDeps) {
						if (!depTrackPoint->isPointingToIndependentDependencyNode()) {
							if (!dependendConDes || depNode->hasAdditionalDependencies()) {
								simpleSameNodeDeps = false;
							}
						}
					}

					if (simpleSameNodeDeps) {
						if (/*!mConfDirectRulePreprocessing*/true) {
							CXLinker<CConceptDescriptor*>* depedendConDesLinker = CObjectAllocator< CXLinker<CConceptDescriptor*> >::allocateAndConstruct(taskMemMan);
							depedendConDesLinker->initLinker(dependendConDes,depLinker);
							depLinker = depedendConDesLinker;
							return true;
						} else {
							return false;
						}
					} else {
						cint64 baseAncDepth = individualNode->getIndividualAncestorDepth();
						typedef QPair<cint64,CDependencyTrackPoint*> CDepthDependencyPair;
						CPROCESSINGSET<CDepthDependencyPair> depSet(calcAlgContext->getTaskProcessorContext());
						CPROCESSINGLIST<CDepthDependencyPair> depList(calcAlgContext->getTaskProcessorContext());
						depSet.insert(CDepthDependencyPair(baseAncDepth,depTrackPoint));
						depList.append(CDepthDependencyPair(baseAncDepth,depTrackPoint));


						while (!depList.isEmpty()) {
							CDepthDependencyPair depthDepPair(depList.takeFirst());
							cint64 ancDepth = depthDepPair.first;
							CDependencyTrackPoint* depTrackPoint = depthDepPair.second;

							if (!depTrackPoint->isPointingToDeterministicDependencyNode()) {
								return false;
							}

							CDependencyNode* depNode = depTrackPoint->getDependencyNode();
							CIndividualProcessNode* appIndiNode = depNode->getAppropriateIndividualNode();
							cint64 newAncDepth = ancDepth;
							bool continueDepLoading = true;
							if (appIndiNode) {
								newAncDepth = appIndiNode->getIndividualAncestorDepth();
							}
							if (newAncDepth == baseAncDepth) {
								CConceptDescriptor* nextConDes = depNode->getConceptDescriptor();
								if (nextConDes) {
									continueDepLoading = false;

									CXLinker<CConceptDescriptor*>* depedendConDesLinker = CObjectAllocator< CXLinker<CConceptDescriptor*> >::allocateAndConstruct(taskMemMan);
									depedendConDesLinker->initLinker(nextConDes,depLinker);
									depLinker = depedendConDesLinker;
								}
							}
							if (newAncDepth < baseAncDepth || depTrackPoint->isPointingToIndependentDependencyNode()) {
								return false;
							}
							if (continueDepLoading) {
								CDependencyTrackPoint* prevDepTrackPoint = depNode->getPreviousDependencyTrackPoint();
								cint64 nextAncDepth = newAncDepth;
								CDependencyNode* nextDepNode = prevDepTrackPoint->getDependencyNode();
								if (nextDepNode && nextDepNode->hasAppropriateIndividualNode()) {
									nextAncDepth = nextDepNode->getAppropriateIndividualNode()->getIndividualAncestorDepth();
								}
								if (!depSet.contains(CDepthDependencyPair(nextAncDepth,prevDepTrackPoint))) {
									depSet.insert(CDepthDependencyPair(nextAncDepth,prevDepTrackPoint));
									depList.append(CDepthDependencyPair(nextAncDepth,prevDepTrackPoint));
								}
							}

							CDependencyIterator depIt = depNode->getAdditionalDependencyIterator();
							while (depIt.hasNext()) {
								CDependency* dependency = depIt.nextDependency();
								// load additional dependencies
								CDependencyTrackPoint* prevDepTrackPoint = dependency->getPreviousDependencyTrackPoint();
								cint64 nextAncDepth = ancDepth;
								CDependencyNode* nextDepNode = prevDepTrackPoint->getDependencyNode();
								if (nextDepNode && nextDepNode->hasAppropriateIndividualNode()) {
									nextAncDepth = nextDepNode->getAppropriateIndividualNode()->getIndividualAncestorDepth();
								}
								if (!depSet.contains(CDepthDependencyPair(ancDepth,prevDepTrackPoint))) {
									depSet.insert(CDepthDependencyPair(ancDepth,prevDepTrackPoint));
									depList.append(CDepthDependencyPair(ancDepth,prevDepTrackPoint));
								}
							}
						}
						return true;
					}
				}




				bool CCalculationTableauCompletionTaskHandleAlgorithm::reapplySatisfiableCachedAbsorbedDisjunctionConcepts(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool conceptsReapplyed = false;

					CConceptProcessingQueue* conProQueue = individualNode->getConceptProcessingQueue(true);
					CReapplyConceptDescriptor* absorbedReapplyConDesLinker = individualNode->getSatisfiableCachedAbsorbedDisjunctionsLinker();
					while (absorbedReapplyConDesLinker) {

						conceptsReapplyed = true;

						CConceptDescriptor* conDes = absorbedReapplyConDesLinker->getConceptDescriptor();
						CDependencyTrackPoint* depTrackPoint = absorbedReapplyConDesLinker->getDependencyTrackPoint();
						CProcessingRestrictionSpecification* procRest = absorbedReapplyConDesLinker->getReapplyProcessingRestriction();

						addConceptRestrictedToProcessingQueue(conDes,depTrackPoint,conProQueue,individualNode,absorbedReapplyConDesLinker->isStaticDescriptor(),procRest,calcAlgContext);

						absorbedReapplyConDesLinker = absorbedReapplyConDesLinker->getNext();
					}
					individualNode->clearSatisfiableCachedAbsorbedDisjunctionsLinker();

					return conceptsReapplyed;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::reapplySatisfiableCachedAbsorbedGeneratingConcepts(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool conceptsReapplyed = false;

					CConceptProcessingQueue* conProQueue = individualNode->getConceptProcessingQueue(true);
					CReapplyConceptDescriptor* absorbedReapplyConDesLinker = individualNode->getSatisfiableCachedAbsorbedGeneratingLinker();
					while (absorbedReapplyConDesLinker) {

						conceptsReapplyed = true;

						CConceptDescriptor* conDes = absorbedReapplyConDesLinker->getConceptDescriptor();
						CDependencyTrackPoint* depTrackPoint = absorbedReapplyConDesLinker->getDependencyTrackPoint();

						addConceptToProcessingQueue(conDes,depTrackPoint,conProQueue,individualNode,false,calcAlgContext);

						absorbedReapplyConDesLinker = absorbedReapplyConDesLinker->getNext();
					}
					individualNode->clearSatisfiableCachedAbsorbedGeneratingLinker();

					return conceptsReapplyed;
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::addSatisfiableCachedAbsorbedDisjunctionConcept(CConceptDescriptor *conceptDescriptor, CIndividualProcessNode*& processIndi, CProcessingRestrictionSpecification* procRest, CDependencyTrackPoint* dependencyTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CReapplyConceptDescriptor* reapplyConDes = CObjectAllocator<CReapplyConceptDescriptor>::allocateAndConstruct(taskMemMan);
					reapplyConDes->initReapllyDescriptor(conceptDescriptor,dependencyTrackPoint,procRest);
					processIndi->addSatisfiableCachedAbsorbedDisjunctionsLinker(reapplyConDes);
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::addSatisfiableCachedAbsorbedGeneratingConcept(CConceptDescriptor *conceptDescriptor, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* dependencyTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CReapplyConceptDescriptor* reapplyConDes = CObjectAllocator<CReapplyConceptDescriptor>::allocateAndConstruct(taskMemMan);
					reapplyConDes->initReapllyDescriptor(conceptDescriptor,dependencyTrackPoint);
					processIndi->addSatisfiableCachedAbsorbedGeneratingLinker(reapplyConDes);
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateIndirectSuccessorSignatureBlocked(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext) {
					propagateAddingBlockedProcessingRestrictionToSuccessors(indi,CIndividualProcessNode::PRFANCESTORSIGNATUREBLOCKINGCACHED,true,CIndividualProcessNode::PRFANCESTORSIGNATUREBLOCKINGCACHED,calcAlgContext);
				}

				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateIndirectSuccessorSatisfiableCached(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext) {
					propagateAddingBlockedProcessingRestrictionToSuccessors(indi,CIndividualProcessNode::PRFANCESTORSATISFIABLECACHED,true,CIndividualProcessNode::PRFANCESTORSATISFIABLECACHED,calcAlgContext);
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateIndirectSuccessorReuseBlocked(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext) {
					propagateAddingBlockedProcessingRestrictionToSuccessors(indi,CIndividualProcessNode::PRFREUSINGINDIVIDUAL,true,CIndividualProcessNode::PRFANCESTORREUSINGINDIVIDUALBLOCKED,calcAlgContext);
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::isSatisfiableCachedAutomatConceptCompatible(CIndividualProcessNode*& individualNode, CConcept* concept, bool negated, CIndividualProcessNode* ancestorIndiNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					cint64 opCode = concept->getOperatorCode();
					CConceptOperator* conOperator = concept->getConceptOperator();
					CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();
					if (!negated && (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQALL_TYPE))) {
						CRole* role = concept->getRole();
						CReapplyConceptLabelSet* ancConSet = ancestorIndiNode->getReapplyConceptLabelSet(false);
						if (individualNode->hasRoleSuccessorToIndividual(role,ancestorIndiNode,true)) {
							if (!containsIndividualNodeConcepts(ancConSet,opConLinker,false,calcAlgContext)) {
								return false;
							}
						}
					} else if (!negated && (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQAND_TYPE))) {
						CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
						while (opConLinkerIt) {
							CConcept* opCon = opConLinkerIt->getData();
							bool opNeg = opConLinkerIt->isNegated();
							if (!isSatisfiableCachedAutomatConceptCompatible(individualNode,opCon,opNeg,ancestorIndiNode,calcAlgContext)) {
								return false;
							}
							opConLinkerIt = opConLinkerIt->getNext();
						}
					}
					return true;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::isSatisfiableCachedCompatible(CIndividualProcessNode*& individualNode, CExpanderBranchedLinker* satBranchLinker, CIndividualProcessNode* ancestorIndiNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (ancestorIndiNode) {
						CSuccessorRoleIterator ancRoleIt = individualNode->getSuccessorRoleIterator(ancestorIndiNode);
						if (!ancRoleIt.hasNext()) {
							return true;
						}
						CReapplyConceptLabelSet* ancConSet = ancestorIndiNode->getReapplyConceptLabelSet(false);
						CCACHINGLIST<CCacheValue>* cacheValueList = satBranchLinker->getCacheValueList();
						for (CCACHINGLIST<CCacheValue>::const_iterator it = cacheValueList->constBegin(), itEnd = cacheValueList->constEnd(); it != itEnd; ++it) {
							CCacheValue cacheValue(*it);
							CConcept* concept = (CConcept*)cacheValue.getIdentification();
							bool conceptNeg = cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT;

							CRole* role = concept->getRole();
							cint64 opCode = concept->getOperatorCode();
							CConceptOperator* conOperator = concept->getConceptOperator();
							CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();
							if (!conceptNeg && (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_TYPE)) || conceptNeg && opCode == CCSOME) {
								bool operandNeg = opCode == CCSOME;
								if (individualNode->hasRoleSuccessorToIndividual(role,ancestorIndiNode,true)) {
									if (!containsIndividualNodeConcepts(ancConSet,opConLinker,operandNeg,calcAlgContext)) {
										return false;
									}
								}
							} else if (!conceptNeg && opCode == CCATMOST || conceptNeg && opCode == CCATLEAST) {
								if (!opConLinker) {
									if (individualNode->hasRoleSuccessorToIndividual(role,ancestorIndiNode,true)) {
										return false;
									}
								} else {
									if (individualNode->hasRoleSuccessorToIndividual(role,ancestorIndiNode,true)) {
										if (!containsIndividualNodeConcepts(ancConSet,opConLinker,true,calcAlgContext)) {
											return false;
										}
									}
								}
							} else if (!conceptNeg && (opCode == CCSOME || opCode == CCATLEAST || opCode == CCAQSOME) || conceptNeg && (opCode == CCALL || opCode == CCATMOST)) {
								CRole* minSuperRole = nullptr;
								cint64 minSuperRoleCount = 0;
								CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList();
								while (superRoleIt) {
									CRole* superRole = superRoleIt->getData();
									cint64 superRoleCount = superRole->getIndirectSuperRoleList()->getCount();
									if (!minSuperRole || superRoleCount < minSuperRoleCount) {
										minSuperRoleCount = superRoleCount;
										minSuperRole = superRole;
									}
									superRoleIt = superRoleIt->getNext();
								}
								if (individualNode->hasRoleSuccessorToIndividual(minSuperRole,ancestorIndiNode,true)) {
									return false;
								}
							} else if (!conceptNeg && (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQAND_AQALL_TYPE))) {
								if (!isSatisfiableCachedAutomatConceptCompatible(individualNode,concept,conceptNeg,ancestorIndiNode,calcAlgContext)) {
									return false;
								}
							}

						}
					}
					return true;
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::expandCachedConcepts(CIndividualProcessNode*& individualNode, CSignatureSatisfiableExpanderCacheEntry* entry, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (entry) {
						CReapplyConceptLabelSet* conSet = individualNode->getReapplyConceptLabelSet(true);
						CMemoryAllocationManager* memMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
						CDependencyFactory* depFactory = calcAlgContext->getDependencyFactory();
						cint64 conSetCount = conSet->getConceptCount();
						cint64 expandCount = entry->getExpanderCacheValueCount();
						CExpanderCacheValueLinker* expLinker = entry->getExpanderCacheValueLinker();
						CCACHINGHASH<cint64,CExpanderCacheValueLinker*>* expTagHash = entry->getTagExpanderCacheValueHash();
						for (cint64 i = 0; i < conSetCount; ++i) {
							expLinker = expLinker->getNext();
						}
						CExpanderCacheValueLinker* expLinkerIt = expLinker;
						bool expanded = false;
						while (expLinkerIt) {

							CCacheValue* cacheValue = expLinkerIt->getCacheValue();
							CConcept* concept = (CConcept*)cacheValue->getIdentification();
							bool conceptNeg = cacheValue->getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT;

							if (!conSet->hasConcept(concept)) {

								STATINC(SATEXPCACHERETRIEVALEXPANDEDCONCEPTSCOUNT,calcAlgContext);
								if (!expanded) {
									STATINC(SATEXPCACHERETRIEVALFOUNDEXPANSIONCOUNT,calcAlgContext);
									expanded = true;
								}
								CDependency* dependencies = nullptr;
								CDependencyTrackPoint* firstDepTrackPoint = nullptr;
								CCACHINGLIST<CExpanderCacheValueLinker*>* depList = expLinkerIt->getExpanderDependencyList();
								for (CCACHINGLIST<CExpanderCacheValueLinker*>::const_iterator depIt = depList->constBegin(), depEndIt = depList->constEnd(); depIt != depEndIt; ++depIt) {
									CExpanderCacheValueLinker* depLinker(*depIt);
									cint64 depTag = depLinker->getCacheValue()->getTag();

									CDependencyTrackPoint* depTrackPoint = nullptr;
									CConceptDescriptor* depConDes = nullptr;
									conSet->getConceptDescriptor(depTag,depConDes,depTrackPoint);

									KONCLUDE_ASSERT_X(depTrackPoint,"expandCachedConcepts","missing dependency");

									CCONNECTIONDependencyNode* connDepNode = createCONNECTIONDependency(individualNode,depConDes,depTrackPoint,calcAlgContext);
									if (!firstDepTrackPoint) {
										firstDepTrackPoint = connDepNode;
									} else {
										dependencies = connDepNode->append(dependencies);
									}
								}

								KONCLUDE_ASSERT_X(firstDepTrackPoint,"expandCachedConcepts","missing dependency");

								CDependencyTrackPoint* expDepTrackPoint = nullptr;
								CEXPANDEDDependencyNode* expDepNode = createEXPANDEDDependency(expDepTrackPoint,individualNode,firstDepTrackPoint,dependencies,calcAlgContext);

								addConceptToIndividualSkipANDProcessing(concept,conceptNeg,individualNode,expDepTrackPoint,true,false,calcAlgContext);
							}

							expLinkerIt = expLinkerIt->getNext();
						}
					}				
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::reactivateIndirectReuseSuccessors(CIndividualProcessNode*& indi, bool recursive, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSuccessorIterator succIt = indi->getSuccessorIterator();
					cint64 ancDepth = indi->getIndividualAncestorDepth();
					while (succIt.hasNext()) {
						CIndividualLinkEdge* succLink = succIt.nextLink(true);
						CIndividualProcessNode* succIndi = getSuccessorIndividual(indi,succLink,calcAlgContext);						
						cint64 succAncDepth = succIndi->getIndividualAncestorDepth();
						if (succAncDepth > ancDepth) {
							if (succIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORREUSINGINDIVIDUALBLOCKED)) {

								if (!succIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORREUSINGINDIVIDUALBLOCKEDABOLISHED)) {
									CIndividualProcessNode* locIndiNode = getLocalizedIndividual(succIndi,false,calcAlgContext);
									locIndiNode->addProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORREUSINGINDIVIDUALBLOCKEDABOLISHED);
								}
							}
						}
					}
				}

				void CCalculationTableauCompletionTaskHandleAlgorithm::reactivateIndirectSignatureBlockedSuccessors(CIndividualProcessNode*& indi, bool recursive, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSuccessorIterator succIt = indi->getSuccessorIterator();
					cint64 ancDepth = indi->getIndividualAncestorDepth();
					while (succIt.hasNext()) {
						CIndividualLinkEdge* succLink = succIt.nextLink(true);
						CIndividualProcessNode* succIndi = getSuccessorIndividual(indi,succLink,calcAlgContext);						
						cint64 succAncDepth = succIndi->getIndividualAncestorDepth();
						if (succAncDepth > ancDepth) {
							if (succIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSIGNATUREBLOCKINGCACHED)) {

								if (!succIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSIGNATUREBLOCKINGCACHEDABOLISHED)) {
									CIndividualProcessNode* locIndiNode = getLocalizedIndividual(succIndi,false,calcAlgContext);
									locIndiNode->addProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSIGNATUREBLOCKINGCACHEDABOLISHED);
									addIndividualToProcessingQueue(locIndiNode,calcAlgContext);
								}

							}
						}
					}
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::reactivateIndirectSatisfiableCachedSuccessors(CIndividualProcessNode*& indi, bool recursive, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSuccessorIterator succIt = indi->getSuccessorIterator();
					cint64 ancDepth = indi->getIndividualAncestorDepth();
					while (succIt.hasNext()) {
						CIndividualLinkEdge* succLink = succIt.nextLink(true);
						CIndividualProcessNode* succIndi = getSuccessorIndividual(indi,succLink,calcAlgContext);						
						cint64 succAncDepth = succIndi->getIndividualAncestorDepth();
						if (succAncDepth > ancDepth) {
							if (succIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATISFIABLECACHED)) {

								if (!succIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATISFIABLECACHEDABOLISHED)) {
									CIndividualProcessNode* locIndiNode = getLocalizedIndividual(succIndi,false,calcAlgContext);
									locIndiNode->addProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATISFIABLECACHEDABOLISHED);
									addIndividualToProcessingQueue(locIndiNode,calcAlgContext);
								}

							}
						}
					}
				}

				void CCalculationTableauCompletionTaskHandleAlgorithm::reactivateIndirectSaturationCachedSuccessors(CIndividualProcessNode*& indi, bool recursive, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSuccessorIterator succIt = indi->getSuccessorIterator();
					cint64 ancDepth = indi->getIndividualAncestorDepth();
					while (succIt.hasNext()) {
						CIndividualLinkEdge* succLink = succIt.nextLink(true);
						CIndividualProcessNode* succIndi = getSuccessorIndividual(indi,succLink,calcAlgContext);						
						cint64 succAncDepth = succIndi->getIndividualAncestorDepth();
						if (succAncDepth > ancDepth) {
							if (succIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATURATIONBLOCKINGCACHED)) {

								if (!succIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATURATIONBLOCKINGCACHEDABOLISHED)) {
									CIndividualProcessNode* locIndiNode = getLocalizedIndividual(succIndi,false,calcAlgContext);
									locIndiNode->addProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATURATIONBLOCKINGCACHEDABOLISHED);
									addIndividualToProcessingQueue(locIndiNode,calcAlgContext);
								}

							}
						}
					}
				}

				QString CCalculationTableauCompletionTaskHandleAlgorithm::generateDebugTrackedClashedDescriptorSummaryString(CTrackedClashedDescriptor* trackedClashDescriptors, CCalculationAlgorithmContextBase* calcAlgContext) {
					QString clashString;
					CTrackedClashedDescriptor* trackedClashDescriptorIt = trackedClashDescriptors;
					while (trackedClashDescriptorIt) {
						CConceptDescriptor* conDes = trackedClashDescriptorIt->getConceptDescriptor();
						QString conceptString("null");
						if (conDes) {
							conceptString = CConceptTextFormater::getConceptString(conDes->getConcept(),conDes->isNegated());
						}
						if (!clashString.isEmpty()) {
							clashString += QString(", ");
						}
						clashString += conceptString;
						trackedClashDescriptorIt = trackedClashDescriptorIt->getNextDescriptor();
					}
					return clashString;
				}


				QString CCalculationTableauCompletionTaskHandleAlgorithm::generateDebugTrackedClashedDescriptorString(CTrackedClashedDescriptor* trackedClashDescriptors, CCalculationAlgorithmContextBase* calcAlgContext) {
					QString clashListString;
					CTrackedClashedDescriptor* trackedClashDescriptorIt = trackedClashDescriptors;
					while (trackedClashDescriptorIt) {
						CConceptDescriptor* conDes = trackedClashDescriptorIt->getConceptDescriptor();
						QString conceptString("null");
						if (conDes) {
							conceptString = CConceptTextFormater::getConceptString(conDes->getConcept(),conDes->isNegated());
						}

						CDependencyTrackPoint* depTrackPoint = trackedClashDescriptorIt->getDependencyTrackPoint();

						QString dependencyString = QString("null");
						if (depTrackPoint) {
							QString depTypeString;
							CDependencyNode* depNode = depTrackPoint->getDependencyNode();
							switch (depNode->getDependencyType()) {
								case CDependencyNode::DNTINDEPENDENTBASE: {
									depTypeString = QString("INDEPENDENT");
									break;
								}
								case CDependencyNode::DNTALLDEPENDENCY: {
									depTypeString = QString("ALL");
									break;
								}
								case CDependencyNode::DNTSOMEDEPENDENCY: {
									depTypeString = QString("SOME");
									break;
								}
								case CDependencyNode::DNTANDDEPENDENCY: {
									depTypeString = QString("AND");
									break;
								}
								case CDependencyNode::DNTORDEPENDENCY: {
									depTypeString = QString("OR");
									break;
								}
								case CDependencyNode::DNTATLEASTDEPENDENCY: {
									depTypeString = QString("ATLEAST");
									break;
								}
								case CDependencyNode::DNTAUTOMATCHOOSEDEPENDENCY: {
									depTypeString = QString("AUTOMATCHOOSE");
									break;
								}
								case CDependencyNode::DNTAUTOMATTRANSACTIONDEPENDENCY: {
									depTypeString = QString("AUTOMATTRANSACTION");
									break;
								}
								case CDependencyNode::DNTSELFDEPENDENCY: {
									depTypeString = QString("SELF");
									break;
								}
								case CDependencyNode::DNTVALUEDEPENDENCY: {
									depTypeString = QString("VALUE");
									break;
								}
								case CDependencyNode::DNTNEGVALUEDEPENDENCY: {
									depTypeString = QString("NEGVALUE");
									break;
								}
								case CDependencyNode::DNTDISTINCTDEPENDENCY: {
									depTypeString = QString("DISTINCT");
									break;
								}
								case CDependencyNode::DNTMERGEDCONCEPT: {
									depTypeString = QString("MERGEDCONCEPT");
									break;
								}
								case CDependencyNode::DNTMERGEDLINK: {
									depTypeString = QString("MERGEDLINK");
									break;
								}
								case CDependencyNode::DNTMERGEDEPENDENCY: {
									depTypeString = QString("MERGE");
									break;
								}
								case CDependencyNode::DNTATMOSTDEPENDENCY: {
									depTypeString = QString("ATMOST");
									break;
								}
								case CDependencyNode::DNTQUALIFYDEPENDENCY: {
									depTypeString = QString("QUALIFY");
									break;
								}
								case CDependencyNode::DNTFUNCTIONALDEPENDENCY: {
									depTypeString = QString("FUNCTIONAL");
									break;
								}
								case CDependencyNode::DNTNOMINALDEPENDENCY: {
									depTypeString = QString("NOMINAL");
									break;
								}
								case CDependencyNode::DNTIMPLICATIONDEPENDENCY: {
									depTypeString = QString("IMPLICATION");
									break;
								}
								case CDependencyNode::DNTEXPANDEDDEPENDENCY: {
									depTypeString = QString("EXPANDED");
									break;
								}
								case CDependencyNode::DNTDATATYPETRIGGERDEPENDENCY: {
									depTypeString = QString("DATATYPETRIGGER");
									break;
								}

							}

							CConceptDescriptor* depNodeConDes = depNode->getConceptDescriptor();
							QString conceptDepNodeString("null"); 
							if (depNodeConDes) {
								conceptDepNodeString = CConceptTextFormater::getConceptString(depNodeConDes->getConcept(),depNodeConDes->isNegated());
							}
							QString depInfoString;
							if (depNode->isNonDeterministiDependencyNode()) {
								CNonDeterministicDependencyNode* nonDetDepNode = (CNonDeterministicDependencyNode*)depNode;
								depInfoString += QString(" NonDetDep, <%1/%2>").arg(nonDetDepNode->getOpenedDependencyTrackingPointsCount()).arg(nonDetDepNode->getBranchTrackPoints()->getCount());
							}
							depInfoString += QString(" + ...(%1)").arg(depNode->getAdditionalDependencyCount());
							dependencyString = QString("%1-Dependency: {%2}%3").arg(depTypeString).arg(conceptDepNodeString).arg(depInfoString);
						}

						QString clashString = QString("\t[ID:%1 / L:%2 | B:%3]: {%4}  -->  %5\r\n").arg(trackedClashDescriptorIt->getAppropriatedIndividualID()).arg(trackedClashDescriptorIt->getAppropriatedIndividualLevel()).arg(trackedClashDescriptorIt->getBranchingLevelTag()).arg(conceptString).arg(dependencyString);

						clashListString += clashString;

						trackedClashDescriptorIt = trackedClashDescriptorIt->getNextDescriptor();
					}
					clashListString.replace("\r\n","<br>");
					return clashListString;
				}




				QString CCalculationTableauCompletionTaskHandleAlgorithm::writeDebugTrackingLineStringToFile(const QString& debugDataString, const QString& fileNameString, CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext) {
					QString tmpDebugDataString = debugDataString;
					QFile file(QString("./SATCT/Tasks/backtrack-task-%1-%2-%3.txt").arg(calcAlgContext->getUsedSatisfiableCalculationTask()->getTaskDepth()).arg(calcAlgContext->getUsedSatisfiableCalculationTask()->getTaskID()).arg(fileNameString));
					if (file.open(QIODevice::WriteOnly)) {
						file.write(tmpDebugDataString.replace("<br>","\r\n").toLocal8Bit());
						file.close();
					}
					return debugDataString;
				}




				QString CCalculationTableauCompletionTaskHandleAlgorithm::generateDebugTrackingLineString(CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext) {
					QString trackLineString = QString("branching level: %1, individual node level: %2\r\n").arg(trackingLine->getBranchingLevel()).arg(trackingLine->getIndividualNodeLevel());

					CTrackedClashedDescriptor* trackedClashDescriptors = nullptr;
					
					trackedClashDescriptors = trackingLine->getIndependentTrackedClashedDescriptors();
					trackLineString += QString("\r\n$ tracked clashes, independent\r\n%1").arg(generateDebugTrackedClashedDescriptorString(trackedClashDescriptors,calcAlgContext));
					
					trackedClashDescriptors = trackingLine->getLevelTrackedBranchingClashedDescriptors();
					trackLineString += QString("\r\n$ tracked clashes, current branching level %2, current individual node level %3\r\n%1").arg(generateDebugTrackedClashedDescriptorString(trackedClashDescriptors,calcAlgContext)).arg(trackingLine->getBranchingLevel()).arg(trackingLine->getIndividualNodeLevel());
					
					trackedClashDescriptors = trackingLine->getLevelTrackedClashedDescriptors();
					trackLineString += QString("\r\n$ tracked clashes, current individual node level %3\r\n%1").arg(generateDebugTrackedClashedDescriptorString(trackedClashDescriptors,calcAlgContext)).arg(trackingLine->getIndividualNodeLevel());
					
					trackedClashDescriptors = trackingLine->getPerviousLevelTrackedClashedDescriptors();
					trackLineString += QString("\r\n$ tracked clashes, previous individual node levels, not non-deterministic\r\n%1").arg(generateDebugTrackedClashedDescriptorString(trackedClashDescriptors,calcAlgContext));
					
					trackedClashDescriptors = trackingLine->getPerviousLevelTrackedNonDeterministicBranchingClashedDescriptors();
					trackLineString += QString("\r\n$ tracked clashes, previous individual node levels, non-deterministic current branching level %2\r\n%1").arg(generateDebugTrackedClashedDescriptorString(trackedClashDescriptors,calcAlgContext)).arg(trackingLine->getBranchingLevel());
					
					trackedClashDescriptors = trackingLine->getPerviousLevelTrackedNonDeterministicClashedDescriptors();
					trackLineString += QString("\r\n$ tracked clashes, previous individual node levels, non-deterministic, not current branching level\r\n%1").arg(generateDebugTrackedClashedDescriptorString(trackedClashDescriptors,calcAlgContext));

					trackLineString.replace("\r\n","<br>");

					return trackLineString;
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::clashedBacktracking(CClashedDependencyDescriptor* clashes, CCalculationAlgorithmContextBase* calcAlgContext) {

					calcAlgContext->getProcessingDataBox()->setClashedDescriptorLinker(clashes);

					//if (!mConfDependencyBackjumping) {
					//	if (!calcAlgContext->getSatisfiableCalculationTask()->getSatisfiableCalculationTaskResult()->hasResult()) {
					//		calcAlgContext->getSatisfiableCalculationTask()->getSatisfiableCalculationTaskResult()->installResult(false);
					//	}
					//	return;

					//}

					KONCLUCE_TASK_ALGORITHM_TIME_MEASURE_INSTRUCTION(mTimerBacktracing.start());

					//mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);

					STATINC(CLASHEDBACKTRACKINGSTARTCOUNT,calcAlgContext);
					CMemoryAllocationManager* tmpMemMan = calcAlgContext->getUsedTemporaryMemoryAllocationManager();
					CTrackedClashedDescriptor* trackedClashDescriptors = createTrackedClashesDescriptors(clashes,calcAlgContext);
					
					KONCLUCE_TASK_ALGORITHM_CLASH_STRING_INSTRUCTION(
						if (mBacktrackDebug) {
							mBeginBacktrackingClashString = generateDebugTrackedClashedDescriptorSummaryString(trackedClashDescriptors,calcAlgContext);
						}
					)

					CPROCESSINGSET<CTrackedClashedDescriptorHasher> clashedSet(calcAlgContext->getUsedTaskProcessorContext());
					CTrackedClashedDependencyLine trackingLine(&clashedSet);
					if (initializeTrackingLine(&trackingLine,trackedClashDescriptors,calcAlgContext)) {

						KONCLUCE_TASK_ALGORITHM_CLASH_STRING_INSTRUCTION(
							if (mBacktrackDebug) {
								mBeginBacktrackingTracklineString = writeDebugTrackingLineStringToFile(generateDebugTrackingLineString(&trackingLine,calcAlgContext),"begin",&trackingLine,calcAlgContext);
							}
						)

						if (trackingLine.getBranchingLevel() == 0) {
							// jump back to root task / cancellation
							cancellationRootTask(calcAlgContext);
						}

						if (trackingLine.hasOnlyCurrentIndividualNodeLevelClashesDescriptors()) {
							writeClashDescriptorsToCache(&trackingLine,calcAlgContext);
						}

						backtrackFromTrackingLine(&trackingLine,calcAlgContext);

						KONCLUCE_TASK_ALGORITHM_CLASH_STRING_INSTRUCTION(
							if (mBacktrackDebug) {
								mEndBacktrackingTracklineString = writeDebugTrackingLineStringToFile(generateDebugTrackingLineString(&trackingLine,calcAlgContext),"end",&trackingLine,calcAlgContext);
							}
						)
					}

					KONCLUCE_TASK_ALGORITHM_TIME_MEASURE_INSTRUCTION(
						cint64 timeElapsed = mTimerBacktracing.elapsed();
						STATINCM(TIMEBACKTRACING,timeElapsed,calcAlgContext);
					);

				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::rootUnsatisfiabilityWriteCaches(CSatisfiableCalculationTask* task, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSatisfiableTaskClassificationMessageAdapter* adapter = task->getClassificationMessageAdapter();
					if (adapter) {
						CConcept* concept = adapter->getTestingConcept();
						if (concept && adapter->hasExtractionFlags(CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTSUBSUMERSROOTNODE)) {
							CUnsatisfiableCacheHandler* unsatCacheHandler = calcAlgContext->getUsedUnsatisfiableCacheHandler();
							if (unsatCacheHandler && mConfWriteUnsatCaching) {
								unsatCacheHandler->writeUnsatisfiableClashedConcept(concept,calcAlgContext);
							}

							CSaturationNodeExpansionCacheHandler* satNodeExpanderCacheHandler = calcAlgContext->getUsedSaturationNodeExpansionCacheHandler();
							if (satNodeExpanderCacheHandler && mConfSaturationConceptUnsatisfiabilitySaturatedCacheWriting) {
								satNodeExpanderCacheHandler->cacheUnsatisfiableConcept(concept,calcAlgContext);
							}

						}
					}
					CProcessingDataBox* processingDataBox = calcAlgContext->getProcessingDataBox();
					CIndividualProcessNode* constIndiNode = processingDataBox->getConstructedIndividualNode();
					if (!processingDataBox->hasMultipleConstructionIndividualNodes() && constIndiNode && constIndiNode->isNominalIndividual()) {
						CXSortedNegLinker<CConcept*>* initConLinker = constIndiNode->getInitializingConceptLinkerIt();
						CIndividual* individual = constIndiNode->getNominalIndividual();
						if (initConLinker && !initConLinker->hasNext()) {
							CConcept* initConcept = initConLinker->getData();
							bool conNegation = initConLinker->isNegated();

							if (mConfCacheComputedConsequences && mCompConsCacheHandler) {
								mCompConsCacheHandler->tryCacheTypeConcept(individual,initConcept,!conNegation,calcAlgContext);
							}
						}
					}
					return false;
				}




				bool CCalculationTableauCompletionTaskHandleAlgorithm::cancellationRootTask(CCalculationAlgorithmContextBase* calcAlgContext) {
					CSatisfiableCalculationTask* rootTask = (CSatisfiableCalculationTask*)calcAlgContext->getSatisfiableCalculationTask()->getRootTask();
					STATINC(TASKROOTBACKJUMPINGCOUNT,calcAlgContext);
					STATINC(ROOTTASKUNSATISFIABLECOUNT,calcAlgContext);


					rootUnsatisfiabilityWriteCaches(rootTask,calcAlgContext);


					//mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
					//QFile file(QString("./SATCT/Tasks/task-root-clashed.txt"));
					//if (file.open(QIODevice::WriteOnly)) {
					//	file.write(mEndTaskDebugIndiModelString.replace("<br>","\r\n").toLocal8Bit());
					//	file.close();
					//}

					//mBranchLevelClosedCountString.clear();
					//cint64 maxBraLevel = mClosedBranchLevelCountHash.size();
					//cint64 foundLevel = 0;
					//for (cint64 i = 0; foundLevel < maxBraLevel; ++i) {
					//	cint64 levelCount = mClosedBranchLevelCountHash.value(i,0);
					//	if (levelCount > 0) {
					//		++foundLevel;
					//	}
					//	mBranchLevelClosedCountString += QString("%1: %2\n").arg(i).arg(levelCount);
					//}

					return cancellationTask(rootTask,calcAlgContext);
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::cancellationTask(CSatisfiableCalculationTask* task, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (!task->getSatisfiableCalculationTaskResult()->hasResult()) {
						task->getSatisfiableCalculationTaskResult()->installResult(false);
						if (mConfDependencyBackjumping) {
							STATINC(TASKBACKJUMPINGCOUNT,calcAlgContext);
							if (calcAlgContext->getUsedSatisfiableCalculationTask() != task) {
								STATINC(TASKAWAYBACKJUMPINGCOUNT,calcAlgContext);
							}
							CTaskProcessorContext* processorContext = calcAlgContext->getUsedTaskProcessorContext();
							processorContext->getTaskProcessorCommunicator()->communicateTaskStatusUpdate(task);
							return true;
						}
					}
					return false;
				}


				CTrackedClashedDescriptor* CCalculationTableauCompletionTaskHandleAlgorithm::getFreeTrackedClashedDescriptor(CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext) {
					CTrackedClashedDescriptor* des = trackingLine->takeNextFreeTrackedClashedDescriptor();
					if (!des) {
						CMemoryAllocationManager* tmpMemMan = calcAlgContext->getUsedTemporaryMemoryAllocationManager();
						des = CObjectAllocator< CTrackedClashedDescriptor >::allocateAndConstruct(tmpMemMan);
					}
					return des;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::backtrackFromTrackingLine(CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext) {
					cint64 backtrackStep = 0;
					while (backtrackFromTrackingLineStep(trackingLine,calcAlgContext)) {
						// until backtracking failed
						KONCLUCE_TASK_ALGORITHM_CLASH_STRING_INSTRUCTION(
							if (mBacktrackDebug) {
								mFileBacktrackingStepTracklineString = writeDebugTrackingLineStringToFile(generateDebugTrackingLineString(trackingLine,calcAlgContext),QString("step-%1").arg(backtrackStep++),trackingLine,calcAlgContext);
							}
						)
					}
					return false;
				}

				bool CCalculationTableauCompletionTaskHandleAlgorithm::backtrackFromTrackingLineStep(CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool trackingSuccess = true;
					bool prevLevelBacktracked = false;
					STATINC(CLASHEDBACKTRACKINGSTEPSCOUNT,calcAlgContext);

					KONCLUCE_TASK_ALGORITHM_CLASH_STRING_INSTRUCTION(
						if (mBacktrackDebug) {
							mBeginBacktrackingStepTracklineString = generateDebugTrackingLineString(trackingLine,calcAlgContext);
						}
					)

					// step 1: backtrack all deterministic dependencies in previous individual node levels
					while (trackingLine->hasPerviousLevelTrackedClashedDescriptors() && trackingSuccess) {
						CTrackedClashedDescriptor* trackedClashedDes = trackingLine->takeNextPerviousLevelTrackedClashedDescriptor();
						STATINC(CLASHEDBACKTRACKINGPREVNODELEVELCOUNT,calcAlgContext);

						KONCLUCE_TASK_ALGORITHM_CLASH_STRING_INSTRUCTION(
							if (mBacktrackDebug) {
								mBeginDetPrevBacktrackingStepTracklineString = generateDebugTrackingLineString(trackingLine,calcAlgContext);
							}
						)
						trackingSuccess &= backtrackDeterministicClashedDescriptorFromPreviousIndividualNodeLevels(trackedClashedDes,trackingLine,calcAlgContext);
						KONCLUCE_TASK_ALGORITHM_CLASH_STRING_INSTRUCTION(
							if (mBacktrackDebug) {
								mEndDetPrevBacktrackingStepTracklineString = generateDebugTrackingLineString(trackingLine,calcAlgContext);
							}
						)
					
						prevLevelBacktracked = true;
					}
					if (prevLevelBacktracked) {
						if (trackingLine->hasOnlyCurrentIndividualNodeLevelClashesDescriptors()) {
							// write clashes to cache
							writeClashDescriptorsToCache(trackingLine,calcAlgContext);
						}
					}
					// step 2: backtrack non-deterministic dependencies in previous individual node levels with current branching level
					if (trackingLine->hasPerviousLevelTrackedNonDeterministicBranchingClashedDescriptors()) {
						CTrackedClashedDescriptor* trackedClashedDes = trackingLine->takeNextPerviousLevelTrackedNonDeterministicBranchingClashedDescriptor();
						STATINC(CLASHEDBACKTRACKINGPREVNODELEVELBRANCHCOUNT,calcAlgContext);

						KONCLUCE_TASK_ALGORITHM_CLASH_STRING_INSTRUCTION(
							if (mBacktrackDebug) {
								mBeginNonDetPrevBacktrackingStepTracklineString = generateDebugTrackingLineString(trackingLine,calcAlgContext);
							}
						)
						trackingSuccess &= backtrackNonDeterministicBranchingClashedDescriptorFromPreviousIndividualNodeLevel(trackedClashedDes,trackingLine,calcAlgContext);
						KONCLUCE_TASK_ALGORITHM_CLASH_STRING_INSTRUCTION(
							if (mBacktrackDebug) {
								mEndNonDetPrevBacktrackingStepTracklineString = generateDebugTrackingLineString(trackingLine,calcAlgContext);
							}
						)
					
					} else
					// step 3: backtrack to next individual node level
					if (trackingLine->hasLevelTrackedBranchingClashedDescriptors()) {
						CTrackedClashedDescriptor* trackedClashedDes = trackingLine->takeNextLevelTrackedBranchingClashedDescriptor();
						STATINC(CLASHEDBACKTRACKINGCURRNODELEVELBRANCHCOUNT,calcAlgContext);
						if (trackedClashedDes->isPointingToNonDeterministicDependencyNode()) {

							KONCLUCE_TASK_ALGORITHM_CLASH_STRING_INSTRUCTION(
								if (mBacktrackDebug) {
									mBeginNonDetPrevBacktrackingStepTracklineString = generateDebugTrackingLineString(trackingLine,calcAlgContext);
								}
							)
							trackingSuccess &= backtrackNonDeterministicBranchingClashedDescriptorFromCurrentIndividualNodeLevel(trackedClashedDes,trackingLine,calcAlgContext);
							KONCLUCE_TASK_ALGORITHM_CLASH_STRING_INSTRUCTION(
								if (mBacktrackDebug) {
									mEndNonDetPrevBacktrackingStepTracklineString = generateDebugTrackingLineString(trackingLine,calcAlgContext);
								}
							)
						
						} else {
							trackingSuccess &= backtrackDeterministicBranchingClashedDescriptorFromCurrentIndividualNodeLevel(trackedClashedDes,trackingLine,calcAlgContext);
						}
					} else {
						// check all dependencies independent
						if (trackingLine->hasOnlyIndependentTrackedClashedDescriptorsRemaining()) {
							STATINC(CLASHEDBACKTRACKINGINDEPENDENTCOUNT,calcAlgContext);
							// caching
							writeClashDescriptorsToCache(trackingLine,calcAlgContext);
							// no more tracking
							trackingSuccess = false;
						} else {

							// something failed, should never happen
							trackingSuccess = false;
						}
					}

					KONCLUCE_TASK_ALGORITHM_CLASH_STRING_INSTRUCTION(
						if (mBacktrackDebug) {
							mEndBacktrackingStepTracklineString = generateDebugTrackingLineString(trackingLine,calcAlgContext);
						}
					)

					return trackingSuccess;
				}

				bool CCalculationTableauCompletionTaskHandleAlgorithm::backtrackNonDeterministicBranchingClashedDescriptorFromCurrentIndividualNodeLevel(CTrackedClashedDescriptor* trackedClashedDes, CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext) {
					return backtrackNonDeterministicBranchingClashedDescriptor(trackedClashedDes,trackingLine,calcAlgContext);
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::backtrackNonDeterministicBranchingClashedDescriptorFromPreviousIndividualNodeLevel(CTrackedClashedDescriptor* trackedClashedDes, CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext) {
					return backtrackNonDeterministicBranchingClashedDescriptor(trackedClashedDes,trackingLine,calcAlgContext);
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::backtrackNonDeterministicBranchingClashedDescriptor(CTrackedClashedDescriptor* trackedClashedDes, CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(CLASHEDBACKTRACKGNONDETERMINISTICCOUNT,calcAlgContext);
					CDependencyTrackPoint* depTrackPoint = trackedClashedDes->getDependencyTrackPoint();
					CNonDeterministicDependencyTrackPoint* nonDetDepTrackPoint = (CNonDeterministicDependencyTrackPoint*)depTrackPoint;
					CNonDeterministicDependencyNode* nonDetDependencyNode = (CNonDeterministicDependencyNode*)nonDetDepTrackPoint->getDependencyNode();

					CSatisfiableCalculationTask* nonDetAccTask = nonDetDependencyNode->getBranchNode()->getSatisfiableCalculationTask();
					CSatisfiableCalculationTask* branchAccTask = nonDetDepTrackPoint->getBranchNode()->getSatisfiableCalculationTask();

					cancellationTask(branchAccTask,calcAlgContext);

					if (nonDetDepTrackPoint->isClashedOrIrelevantBranch()) {
						// perhaps clash set by another thread
						STATINC(CLASHEDBACKTRACKGNONDETALREADYCLASHFAILEDCOUNT,calcAlgContext);
						return false;
					}

					CMemoryTemporaryAllocationManager* tmpMemMan = calcAlgContext->getUsedTemporaryMemoryAllocationManager();
					CTaskProcessorContext* processorContext = calcAlgContext->getUsedTaskProcessorContext();



					KONCLUCE_TASK_ALGORITHM_CLASH_STRING_INSTRUCTION(
						if (mBacktrackDebug) {
							mNonDetDependencyTrackPointReasonString = generateDebugTrackedClashedDescriptorString(trackedClashedDes,calcAlgContext);
						}
					)


					cint64 procTag = nonDetDependencyNode->getProcessingTag();
					// backtrack all clashes before the processing tag of the non-deterministic dependency for current branch
					CTrackedClashedDescriptor* trackedClashedDescriptorBeforeProcTagList = nullptr;
					while (trackingLine->hasMoreTrackedClashedList()) {
						CTrackedClashedDescriptor* trackedClashedDesList = trackingLine->takeNextTrackedClashedList();
						CTrackedClashedDescriptor* trackedClashedDescriptorsBeforeProcTag = getBacktrackedDeterministicClashedDescriptorsBeforeProcessingTag(trackedClashedDesList,procTag,trackingLine,calcAlgContext);
						if (trackedClashedDescriptorsBeforeProcTag) {
							trackedClashedDescriptorBeforeProcTagList = trackedClashedDescriptorsBeforeProcTag->append(trackedClashedDescriptorBeforeProcTagList);
						}
					}

					KONCLUCE_TASK_ALGORITHM_CLASH_STRING_INSTRUCTION(
						if (mBacktrackDebug) {
							mNonDetDependencyBeforeProcessedTrackedString = generateDebugTrackedClashedDescriptorString(trackedClashedDescriptorBeforeProcTagList,calcAlgContext);
						}
					)


					writeClashDescriptorsToCache(trackedClashedDescriptorBeforeProcTagList,trackedClashedDes,trackingLine,calcAlgContext);

					// copy clashes to memory container
					CMemoryPoolContainer memoryContainer;
					CTaskMemoryPoolAllocationManager conBranchMemMan(&memoryContainer,calcAlgContext->getTemporaryMemoryAllocationManager());

					// create branch clash descriptors with memory pool which will sent to corresponding task, because other threads could be using the clash descriptors after installation
					CTrackedClashedDescriptor* branchMemConClashedDesList = createTrackedClashesDescriptors(trackedClashedDescriptorBeforeProcTagList,calcAlgContext,&conBranchMemMan);


					if (nonDetDepTrackPoint->isClashedOrIrelevantBranch()) {
						// perhaps clash set by another thread, release memory pools
						calcAlgContext->getTemporaryMemoryAllocationManager()->releaseMemoryPoolContainer(&memoryContainer);
						STATINC(CLASHEDBACKTRACKGNONDETALREADYCLASHFAILEDCOUNT,calcAlgContext);
						return false;
					}
					// otherwise install clash descriptors
					nonDetDepTrackPoint->setClashes(branchMemConClashedDesList,true);


					if (trackedClashedDescriptorBeforeProcTagList) {
						// and send memory pools to corresponding task
						processorContext->getTaskProcessorCommunicator()->communicateTaskAdditionalAllocation(nonDetAccTask,memoryContainer.takeMemoryPools());
					}

					// test whether all branches has clash descriptors
					bool otherOpenedTrackPoints = nonDetDependencyNode->hasOtherOpenedDependencyTrackingPoints(nonDetDepTrackPoint);
					if (!otherOpenedTrackPoints) {
						// collect clash descriptors of all branches and add dependency of closed dependency node
						STATINC(CLASHEDBACKTRACKGNONDETCLOSEDCOUNT,calcAlgContext);
						CTrackedClashedDescriptor* collectedTrackedClashedDes = getCollectedFilteredClashedDescriptorsFromBranch(trackedClashedDes,nonDetDependencyNode,trackingLine,calcAlgContext);
						
						KONCLUCE_TASK_ALGORITHM_CLASH_STRING_INSTRUCTION(
							if (mBacktrackDebug) {
								mNonDetDependencyCollectedTrackedString = generateDebugTrackedClashedDescriptorString(collectedTrackedClashedDes,calcAlgContext);
							}
						)
						
						if (initializeTrackingLine(trackingLine,collectedTrackedClashedDes,calcAlgContext)) {
							if (trackingLine->getBranchingLevel() == 0) {
								// jump back to root task / cancellation
								cancellationRootTask(calcAlgContext);
							}

							if (trackingLine->hasOnlyCurrentIndividualNodeLevelClashesDescriptors()) {
								writeClashDescriptorsToCache(trackingLine,calcAlgContext);
							}

							//// TODO: delete
							//cint64 depBraLevel = nonDetDependencyNode->getBranchNode()->getBranchingLevel();
							//cint64 depBraLevelCount = mClosedBranchLevelCountHash.value(depBraLevel,0)+1;
							//mClosedBranchLevelCountHash.insert(depBraLevel,depBraLevelCount);

							return true;
						}
					} else {
						// mark all dependencies and corresponding tasks as relevant

						trackedClashedDescriptorBeforeProcTagList;

					}
					return false;
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::markRelevanceForTrackedClashedDescriptors(CTrackedClashedDescriptor* descriptors, CCalculationAlgorithmContextBase* calcAlgContext) {
					for (CTrackedClashedDescriptor* desIt = descriptors; desIt; desIt = desIt->getNextDescriptor()) {
						CDependencyTrackPoint* depTrackPoint = desIt->getDependencyTrackPoint();
						markDependencyRelevance(depTrackPoint,calcAlgContext);
					}
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::markDependencyRelevance(CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (!depTrackPoint->isDependencyRelevant()) {
						depTrackPoint->setDependencyRelevance(true);

						CDependencyNode* depNode = depTrackPoint->getDependencyNode();
						if (depNode) {
							CDependencyIterator depIt(depNode->getAdditionalDependencyIterator(true,true));
							while (depIt.hasNext()) {
								CDependency* dep = depIt.nextDependency(true);
								CDependencyTrackPoint* prevDepTrackPoint = dep->getPreviousDependencyTrackPoint();
								markDependencyRelevance(prevDepTrackPoint,calcAlgContext);
							}
							CDependencyTrackPoint* prevDepTrackPoint = depNode->getPreviousDependencyTrackPoint();
							markDependencyRelevance(prevDepTrackPoint,calcAlgContext);
						}

						if (depNode->isNonDeterministiDependencyNode()) {
							CNonDeterministicDependencyNode* nonDetDepNode = (CNonDeterministicDependencyNode*)depNode;
							CBranchTreeNode* branchTreeNode = nonDetDepNode->getBranchNode();
							CSatisfiableCalculationTask* branchSatCalcTask = branchTreeNode->getSatisfiableCalculationTask();

							if (!branchSatCalcTask->isTaskRelevant()) {
								// TODO: add statistic for marked tasks as relevant
								CTaskProcessorContext* processorContext = calcAlgContext->getUsedTaskProcessorContext();
								processorContext->getTaskProcessorCommunicator()->communicateTaskRelevant(branchSatCalcTask);
							}
						}
					}
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::addIndividualNodeForCacheUnsatisfiableRetrieval(CIndividualProcessNode*& indiNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CXLinker<CIndividualProcessNode*>* indiNodeLinker = CObjectAllocator< CXLinker<CIndividualProcessNode*> >::allocateAndConstruct(taskMemMan);
					indiNodeLinker->initLinker(indiNode);
					calcAlgContext->getUsedProcessingDataBox()->addIndividualNodeCacheTestingLinker(indiNodeLinker);
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::writeClashDescriptorsToCache(CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext) {
					CTrackedClashedDescriptor* trackedClashedDesList = nullptr;
					while (trackingLine->hasMoreTrackedClashedList()) {
						trackedClashedDesList = trackingLine->takeNextTrackedClashedList()->append(trackedClashedDesList);
					}
					bool cacheWrite = writeClashDescriptorsToCache(trackedClashedDesList,trackingLine,calcAlgContext);
					trackingLine->sortInTrackedClashedDescriptors(trackedClashedDesList,true);
					return cacheWrite;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::writeClashDescriptorsToCache(CTrackedClashedDescriptor*& trackedClashedDes, CTrackedClashedDescriptor* additionalTrackedClashedDes, CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext) {
					CTrackedClashedDescriptor* separatTrackedClashedDes = additionalTrackedClashedDes;
					trackedClashedDes = additionalTrackedClashedDes->append(trackedClashedDes);
					bool cacheWrite = writeClashDescriptorsToCache(trackedClashedDes,trackingLine,calcAlgContext);
					if (!cacheWrite) {
						trackedClashedDes = trackedClashedDes->getNextDescriptor();
					} else {
						trackedClashedDes = (CTrackedClashedDescriptor*)trackedClashedDes->removeOne(additionalTrackedClashedDes);
					}
					additionalTrackedClashedDes->clearNext();
					return cacheWrite;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::writeClashDescriptorsToCache(CTrackedClashedDescriptor*& trackedClashedDes, CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (mConfWriteUnsatCaching && trackedClashedDes) {
						STATINC(UNSATCACHEWRITINGREQUSTCOUNT,calcAlgContext);
						KONCLUCE_TASK_ALGORITHM_CLASH_STRING_INSTRUCTION(
							if (mBacktrackDebug) {
								mCachingClashString  = generateDebugTrackedClashedDescriptorString(trackedClashedDes,calcAlgContext);
							}
						)
						CTrackedClashedDescriptor* trackedClashedDesIt = trackedClashedDes;
						bool nominalOccured = trackedClashedDesIt->isAppropriatedIndividualNominal();
						cint64 minIndiID = trackedClashedDesIt->getAppropriatedIndividualID();
						bool hasOtherIndiID = false;
						cint64 minIndiLevel = trackedClashedDesIt->getAppropriatedIndividualLevel();
						bool hasOtherIndiLevel = false;
						bool hasNoInvalidConDes = trackedClashedDesIt->getConceptDescriptor() != nullptr;

						trackedClashedDesIt = trackedClashedDesIt->getNextDescriptor();
						while (trackedClashedDesIt) {
							CConceptDescriptor* conDes = trackedClashedDesIt->getConceptDescriptor();
							hasNoInvalidConDes &= conDes != nullptr;
							if (conDes) {
								CConcept* concept = conDes->getConcept();
								cint64 opCode = concept->getOperatorCode();
								CConceptOperator* conOperator = concept->getConceptOperator();
								if (concept->getTerminology() == nullptr || conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_PROPAGATION_TYPE)) {
									hasNoInvalidConDes = false;
								}
							}
							nominalOccured |= trackedClashedDesIt->isAppropriatedIndividualNominal();
							if (trackedClashedDesIt->getAppropriatedIndividualID() != minIndiID) {
								hasOtherIndiID = true;
								if (nominalOccured) {
									STATINC(UNSATCACHEWRITINGREQUSTDIFFNOMINALFAILEDCOUNT,calcAlgContext);
									return false;
								}
							}
							if (trackedClashedDesIt->getAppropriatedIndividualLevel() != minIndiLevel) {
								STATINC(UNSATCACHEWRITINGREQUSTDIFFNODELEVELFAILEDCOUNT,calcAlgContext);
								hasOtherIndiLevel = true;
								return false;
							}
							trackedClashedDesIt = trackedClashedDesIt->getNextDescriptor();
						}
						// TODO: unsatisfiability caching with nominals, currently deactivateds
						if (hasNoInvalidConDes && !nominalOccured) {
							bool writeCacheLine = false;
							if (!nominalOccured) {
								writeCacheLine = !hasOtherIndiLevel;
							} else {
								writeCacheLine = !hasOtherIndiID;
							}
							bool atomicClash = false;
							if (writeCacheLine) {
								CTrackedClashedDescriptor* trackedClashedDesIt = trackedClashedDes;
								while (trackedClashedDesIt && !atomicClash) {
									CConceptDescriptor* conDes = trackedClashedDesIt->getConceptDescriptor();
									CConcept* concept = conDes->getConcept();
									bool conNeg = conDes->getNegation();
									CTrackedClashedDescriptor* atomClashedDesIt = trackedClashedDesIt->getNextDescriptor();
									while (atomClashedDesIt && !atomicClash) {
										CConceptDescriptor* othConDes = atomClashedDesIt->getConceptDescriptor();
										if (othConDes->getConcept() == concept && othConDes->getNegation() != conNeg) {
											STATINC(UNSATCACHEWRITINGREQUSTATOMICCLASHFAILEDCOUNT,calcAlgContext);
											atomicClash = true;
											return false;
										}
										atomClashedDesIt = atomClashedDesIt->getNextDescriptor();
									}
									trackedClashedDesIt = trackedClashedDesIt->getNextDescriptor();
								}
							}
							writeCacheLine &= !atomicClash;
							if (writeCacheLine) {


								//CConcept* orConcept = nullptr;
								//for (CTrackedClashedDescriptor* cIt = trackedClashedDes; cIt; cIt = cIt->getNextDescriptor()) {
								//	if (cIt->getConceptDescriptor()->getConcept()->getOperatorCode() == CCAND && cIt->getConceptDescriptor()->isNegated() && cIt->getConceptDescriptor()->getConcept()->getOperandCount() > 2) {
								//		orConcept = cIt->getConceptDescriptor()->getConcept();
								//	}
								//}
								//if (orConcept) {
								//	for (CSortedNegLinker<CConcept*>* opConIt = orConcept->getOperandList(); opConIt; opConIt = opConIt->getNext()) {
								//		bool found = false;
								//		for (CTrackedClashedDescriptor* cIt = trackedClashedDes; cIt; cIt = cIt->getNextDescriptor()) {
								//			if (cIt->getConceptDescriptor()->getConcept() == opConIt->getData()) {
								//				found = true;
								//			}
								//		}
								//		if (!found) {
								//			mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
								//			QFile file(QString("caching-error.txt"));
								//			if (file.open(QIODevice::WriteOnly)) {
								//				file.write(mEndTaskDebugIndiModelString.replace("<br>","\r\n").toLocal8Bit());
								//				file.close();
								//			}
								//			bool bug = true;
								//		}
								//	}
								//}

								if (mConfUnsatCachingUseNodeSignatureSet) {
									addIndiNodeSignatureOfUnsatisfiableClashedDescriptors(trackedClashedDes,calcAlgContext);
								}
								trackedClashedDes = getSortedClashedDescriptors(trackedClashedDes,calcAlgContext);
								KONCLUCE_TASK_ALGORITHM_CLASH_STRING_INSTRUCTION(
									if (mBacktrackDebug) {
										mSortedCachingClashString  = generateDebugTrackedClashedDescriptorString(trackedClashedDes,calcAlgContext);
									}
								)
								writeUnsatisfiableClashedDescriptors(trackedClashedDes,calcAlgContext);
								return true;
							}
						}
					}
					return false;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::addIndiNodeSignatureOfUnsatisfiableClashedDescriptors(CTrackedClashedDescriptor* trackedClashedDes, CCalculationAlgorithmContextBase* calcAlgContext) {
					cint64 indiID = trackedClashedDes->getAppropriatedIndividualID();
					CIndividualProcessNode* indi = getCorrectedNominalIndividualNode(indiID,calcAlgContext);
					CReapplyConceptLabelSet* conSet = indi->getReapplyConceptLabelSet(false);
					cint64 conSig = conSet->getConceptSignatureValue();
					mUnsatCachingSignatureSet.insert(conSig);
					return true;
				}

				bool CCalculationTableauCompletionTaskHandleAlgorithm::isClashedDescriptorSortedBefore(CTrackedClashedDescriptor* trackedClashedDesBefore, CTrackedClashedDescriptor* trackedClashedDesAfter, CCalculationAlgorithmContextBase* calcAlgContext) {
					return !trackedClashedDesAfter || trackedClashedDesBefore->getConceptDescriptor()->getConceptTag() <= trackedClashedDesAfter->getConceptDescriptor()->getConceptTag();
				}


				CTrackedClashedDescriptor* CCalculationTableauCompletionTaskHandleAlgorithm::getSortedClashedDescriptors(CTrackedClashedDescriptor* trackedClashedDes, CCalculationAlgorithmContextBase* calcAlgContext) {
					CTrackedClashedDescriptor* sortedTrackedClashedDes = trackedClashedDes;
					trackedClashedDes = trackedClashedDes->getNextDescriptor();
					sortedTrackedClashedDes->clearNext();
					while (trackedClashedDes) {
						CTrackedClashedDescriptor* tmpTrackedClashedDes = trackedClashedDes;
						trackedClashedDes = trackedClashedDes->getNextDescriptor();
						tmpTrackedClashedDes->clearNext();

						if (isClashedDescriptorSortedBefore(tmpTrackedClashedDes,sortedTrackedClashedDes,calcAlgContext)) {
							sortedTrackedClashedDes = tmpTrackedClashedDes->append(sortedTrackedClashedDes);
						} else {
							CTrackedClashedDescriptor* sortedInsertPosIt = sortedTrackedClashedDes;
							while (sortedInsertPosIt) {
								CTrackedClashedDescriptor* nextSortedPosDes = sortedInsertPosIt->getNextDescriptor();
								if (isClashedDescriptorSortedBefore(tmpTrackedClashedDes,nextSortedPosDes,calcAlgContext)) {
									sortedInsertPosIt->insertNext(tmpTrackedClashedDes);
									break;
								}
								sortedInsertPosIt = nextSortedPosDes;
							}
						}
					}
					return sortedTrackedClashedDes;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::writeUnsatisfiableClashedDescriptors(CTrackedClashedDescriptor* trackedClashedDes, CCalculationAlgorithmContextBase* calcAlgContext) {
					CUnsatisfiableCacheHandler* unsatCacheHandler = calcAlgContext->getUsedUnsatisfiableCacheHandler();
					if (unsatCacheHandler) {
						return unsatCacheHandler->writeUnsatisfiableClashedDescriptors(trackedClashedDes,calcAlgContext);
					}
					return false;
				}


				CTrackedClashedDescriptor* CCalculationTableauCompletionTaskHandleAlgorithm::getCollectedFilteredClashedDescriptorsFromBranch(CTrackedClashedDescriptor* nonDetClashedPointingDes, CNonDeterministicDependencyNode* nonDetBranchDepNode, CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext, CMemoryAllocationManager* tmpMemMan) {
					CPROCESSINGSET<CTrackedClashedDescriptorHasher> testClashedSet(calcAlgContext->getUsedTaskProcessorContext());
					CNonDeterministicDependencyTrackPoint* trackPointIt = nonDetBranchDepNode->getBranchTrackPoints();
					CTrackedClashedDescriptor* newTrackedClashedDescriptorList = nullptr;

					CTrackedClashedDescriptor* nonDetPointingFirstTrackedClashedDescriptor = nonDetClashedPointingDes;

					while (trackPointIt) {
						CClashedDependencyDescriptor* clashedDepDescriptors = trackPointIt->getClashes();						

						CClashedDependencyDescriptor* clashedDepDescriptorsIt = clashedDepDescriptors;
						while (clashedDepDescriptorsIt) {
							CClashedDependencyDescriptor* clashedDepDescriptor = clashedDepDescriptorsIt;
							if (clashedDepDescriptor->getDependencyTrackPoint()->getDependencyNode() != nonDetBranchDepNode) {
								CTrackedClashedDescriptor* netTrackedClashDes = createTrackedClashesDescriptor(clashedDepDescriptor,calcAlgContext,tmpMemMan);
								CTrackedClashedDescriptorHasher hasher(netTrackedClashDes);
								if (!testClashedSet.contains(hasher)) {
									// each dependency only once
									testClashedSet.insert(hasher);
									newTrackedClashedDescriptorList = netTrackedClashDes->append(newTrackedClashedDescriptorList);
								}
							} else {
								if (!nonDetPointingFirstTrackedClashedDescriptor) {
									nonDetPointingFirstTrackedClashedDescriptor = createTrackedClashesDescriptor(clashedDepDescriptor,calcAlgContext,tmpMemMan);
								}
							}
							clashedDepDescriptorsIt = clashedDepDescriptorsIt->getNext();
						}
						trackPointIt = trackPointIt->getNext();
					}

					KONCLUDE_ASSERT_X(nonDetPointingFirstTrackedClashedDescriptor,"getCollectedFilteredClashedDescriptorsFromBranch","track point for non-deterministic dependency not found");

					CTrackedClashedDescriptor* nonDetBacktrackedClashedDes = getBacktrackedDeterministicClashedDescriptors(nonDetPointingFirstTrackedClashedDescriptor,trackingLine,nullptr,calcAlgContext);
					CTrackedClashedDescriptor* nonDetBacktrackedClashedDesIt = nonDetBacktrackedClashedDes;
					while (nonDetBacktrackedClashedDesIt) {
						nonDetBacktrackedClashedDes = nonDetBacktrackedClashedDesIt;
						nonDetBacktrackedClashedDesIt = nonDetBacktrackedClashedDesIt->getNextDescriptor();
						nonDetBacktrackedClashedDes->clearNext();

						CTrackedClashedDescriptorHasher hasher(nonDetBacktrackedClashedDes);
						if (!testClashedSet.contains(hasher)) {
							// each dependency only once
							testClashedSet.insert(hasher);
							newTrackedClashedDescriptorList = nonDetBacktrackedClashedDes->append(newTrackedClashedDescriptorList);
						}
					}


					return newTrackedClashedDescriptorList;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::backtrackDeterministicBranchingClashedDescriptorFromCurrentIndividualNodeLevel(CTrackedClashedDescriptor* trackedClashedDes, CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext) {
					cint64 minIndiLevel = CINT64_MAX;
					CTrackedClashedDescriptor* newTrackedClashedDesList = getBacktrackedDeterministicClashedDescriptors(trackedClashedDes,trackingLine,&minIndiLevel,calcAlgContext);
					trackingLine->addFreeTrackedClashedDescriptor(trackedClashedDes);
					if (minIndiLevel < trackingLine->getIndividualNodeLevel()) {
						// cache with old tracked clash descriptors
						trackingLine->moveToNextIndividualNodeLevel(minIndiLevel);
					}
					trackingLine->sortInTrackedClashedDescriptors(newTrackedClashedDesList);
					return true;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::backtrackDeterministicClashedDescriptorFromPreviousIndividualNodeLevels(CTrackedClashedDescriptor* trackedClashedDes, CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext) {
					CTrackedClashedDescriptor* newTrackedClashedDesList = getBacktrackedDeterministicClashedDescriptors(trackedClashedDes,trackingLine,nullptr,calcAlgContext);
					trackingLine->addFreeTrackedClashedDescriptor(trackedClashedDes);
					trackingLine->sortInTrackedClashedDescriptors(newTrackedClashedDesList);
					return true;
				}


				CTrackedClashedDescriptor* CCalculationTableauCompletionTaskHandleAlgorithm::getBacktrackedDeterministicClashedDescriptorsBeforeProcessingTag(CTrackedClashedDescriptor* trackedClashedDescriptors, cint64 processingTag, CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext) {
					CPROCESSINGSET<CTrackedClashedDescriptorHasher>* testClashedSet = trackingLine->getTrackedClashedDescriptorSet();
					CTrackedClashedDescriptor* newTrackedClashedDescriptorList = nullptr;
					CTrackedClashedDescriptor* trackedClashedDescriptorIt = trackedClashedDescriptors;
					while (trackedClashedDescriptorIt) {
						CTrackedClashedDescriptor* trackedClashedDescriptor = trackedClashedDescriptorIt;
						trackedClashedDescriptorIt = trackedClashedDescriptorIt->getNextDescriptor();
						trackedClashedDescriptor->clearNext();
						if (trackedClashedDescriptor->isPointingToNonDeterministicDependencyNode()) {
							KONCLUDE_ASSERT_X(!trackedClashedDescriptor->isProcessedAfter(processingTag),"getBacktrackedDeterministicClashedDescriptorsBeforeProcessingTag","non-deterministic dependency is processed after max branching leveled dependency");
							newTrackedClashedDescriptorList = trackedClashedDescriptor->append(newTrackedClashedDescriptorList);
						} else {
							if (trackedClashedDescriptor->isProcessedAfter(processingTag)) {
								CDependencyTrackPoint* depTrackPoint = trackedClashedDescriptor->getDependencyTrackPoint();
								//if (depTrackPoint->isPointingToIndependentDependencyNode()) {
								//	CTrackedClashedDescriptor* newTrackedClashedDescriptor = getFreeTrackedClashedDescriptor(trackingLine,calcAlgContext);
								//	newTrackedClashedDescriptor->initTrackedClashedDescriptor(trackedClashedDescriptor,nullptr,depTrackPoint);	

								//	CTrackedClashedDescriptorHasher hasher(newTrackedClashedDescriptor);
								//	if (!testClashedSet->contains(hasher)) {
								//		// each dependency only once
								//		testClashedSet->insert(hasher);

								//		trackedClashedDescriptorIt = newTrackedClashedDescriptor->append(trackedClashedDescriptorIt);
								//	} else {
								//		trackingLine->addFreeTrackedClashedDescriptor(newTrackedClashedDescriptor);
								//	}

								//} else {
									CTrackedClashedDescriptor* newTrackedClashedDescriptorIt = getBacktrackedDeterministicClashedDescriptors(trackedClashedDescriptor,trackingLine,nullptr,calcAlgContext);
									while (newTrackedClashedDescriptorIt) {
										CTrackedClashedDescriptor* newTrackedClashedDescriptor = newTrackedClashedDescriptorIt;
										newTrackedClashedDescriptorIt = newTrackedClashedDescriptorIt->getNextDescriptor();
										newTrackedClashedDescriptor->clearNext();

										CTrackedClashedDescriptorHasher hasher(newTrackedClashedDescriptor);
										if (!testClashedSet->contains(hasher)) {
											// each dependency only once
											testClashedSet->insert(hasher);

											trackedClashedDescriptorIt = newTrackedClashedDescriptor->append(trackedClashedDescriptorIt);
										} else {
											trackingLine->addFreeTrackedClashedDescriptor(newTrackedClashedDescriptor);
										}
									}
								//}
								trackingLine->addFreeTrackedClashedDescriptor(trackedClashedDescriptor);
							} else if (!trackedClashedDescriptor->getConceptDescriptor() && !trackedClashedDescriptor->isPointingToIndependentDependencyNode()) {
								// try backtrack 
								cint64 continuedIndiID = trackedClashedDescriptor->getAppropriatedIndividualID();
								bool allIndiIDContinued = true;
								CTrackedClashedDescriptor* newTrackedClashedDescriptors = getBacktrackedDeterministicClashedDescriptors(trackedClashedDescriptor,trackingLine,nullptr,calcAlgContext);
								CTrackedClashedDescriptor* newTrackedClashedDescriptorIt = newTrackedClashedDescriptors;
								while (newTrackedClashedDescriptorIt && allIndiIDContinued) {
									if (newTrackedClashedDescriptorIt->getAppropriatedIndividualID() != continuedIndiID) {
										allIndiIDContinued = false;
									}
									newTrackedClashedDescriptorIt = newTrackedClashedDescriptorIt->getNextDescriptor();
								}
								if (!allIndiIDContinued) {
									newTrackedClashedDescriptorList = trackedClashedDescriptor->append(newTrackedClashedDescriptorList);
									newTrackedClashedDescriptorIt = newTrackedClashedDescriptors;
									while (newTrackedClashedDescriptorIt) {
										CTrackedClashedDescriptor* newTrackedClashedDescriptor = newTrackedClashedDescriptorIt;
										newTrackedClashedDescriptorIt = newTrackedClashedDescriptorIt->getNextDescriptor();
										newTrackedClashedDescriptor->clearNext();
										trackingLine->addFreeTrackedClashedDescriptor(newTrackedClashedDescriptor);
									}
								} else {
									// 
									newTrackedClashedDescriptorIt = newTrackedClashedDescriptors;
									while (newTrackedClashedDescriptorIt) {
										CTrackedClashedDescriptor* newTrackedClashedDescriptor = newTrackedClashedDescriptorIt;
										newTrackedClashedDescriptorIt = newTrackedClashedDescriptorIt->getNextDescriptor();
										newTrackedClashedDescriptor->clearNext();

										CTrackedClashedDescriptorHasher hasher(newTrackedClashedDescriptor);
										if (!testClashedSet->contains(hasher)) {
											// each dependency only once
											testClashedSet->insert(hasher);

											trackedClashedDescriptorIt = newTrackedClashedDescriptor->append(trackedClashedDescriptorIt);
										} else {
											trackingLine->addFreeTrackedClashedDescriptor(newTrackedClashedDescriptor);
										}
									}
									trackingLine->addFreeTrackedClashedDescriptor(trackedClashedDescriptor);
								}

							} else {
								newTrackedClashedDescriptorList = trackedClashedDescriptor->append(newTrackedClashedDescriptorList);
							}
						}
					}
					return newTrackedClashedDescriptorList;
				}






				CTrackedClashedDescriptor* CCalculationTableauCompletionTaskHandleAlgorithm::getBacktrackedDeterministicClashedDescriptors(CTrackedClashedDescriptor* trackedClashedDes, CTrackedClashedDependencyLine* trackingLine, cint64* minIndiLevel, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(CLASHEDBACKTRACKEDDESCRIPTORSCOUNT,calcAlgContext);
					CDependencyNode* depNode = trackedClashedDes->getDependencyTrackPoint()->getDependencyNode();
					CConceptDescriptor* conDes = depNode->getConceptDescriptor();
					CIndividualProcessNode* indiNode = nullptr;
					CIndividualProcessNode* newIndiNode = nullptr;
					CTrackedClashedDescriptor* newTrackedClashedDesList = nullptr;
					if (minIndiLevel) {
						*minIndiLevel = trackedClashedDes->getAppropriatedIndividualLevel();
					}
					if (depNode->hasAppropriateIndividualNode()) {
						newIndiNode = getCoresspondingIndividualNodeFromDependency(depNode,calcAlgContext);
						if (minIndiLevel) {
							*minIndiLevel = qMin(*minIndiLevel,newIndiNode->getIndividualNominalLevelOrAncestorDepth());
						}
					}
					CDependencyTrackPoint* depTrackPoint = depNode->getPreviousDependencyTrackPoint();


					CVariableBindingPath* varBindPath = trackedClashedDes->getVariableBindingPath();
					if (depNode->isRepresentativeSelectDependencyNode()) {			
						CRepresentativeSelectDependencyNode* repSelDepNode = dynamic_cast<CRepresentativeSelectDependencyNode*>(depNode);
						varBindPath = repSelDepNode->getSelectedVariableBindingPath();
					} else if (depNode->isRepresentativeResolveDependencyNode()) {
						CRepresentativeResolveDependencyNode* repResDepNode = dynamic_cast<CRepresentativeResolveDependencyNode*>(depNode);		

						CRepresentativeVariableBindingPathMap* repVarBindPathMap = repResDepNode->getResolveRepresentativeVariableBindingPathMap();
						CRepresentativePropagationMap* repPropMap = repResDepNode->getResolveRepresentativePropagationMap();

						const CRepresentativeVariableBindingPathMapData& repVarBindPathMapData = repVarBindPathMap->value(varBindPath->getPropagationID());
						varBindPath = repVarBindPathMapData.getResolveVariableBindingPath();

						cint64 resolveRepID = repVarBindPathMapData.getResolveRepresentativeVariableBindingPathSetDataID();

						const CRepresentativePropagationMapData& repPropMapData = repPropMap->value(resolveRepID);
						CRepresentativePropagationDescriptor* repPropDes = repPropMapData.getRepresentativePropagationDescriptor();

						depTrackPoint = repPropDes->getDependencyTrackPoint();
					}


					CTrackedClashedDescriptor* newTrackedClashedDes = getFreeTrackedClashedDescriptor(trackingLine,calcAlgContext);
					if (newIndiNode) {
						newTrackedClashedDes->initTrackedClashedDescriptor(newIndiNode,conDes,varBindPath,depTrackPoint);
					} else {
						newTrackedClashedDes->initTrackedClashedDescriptor(trackedClashedDes,conDes,varBindPath,depTrackPoint);
					}
					newTrackedClashedDes = tryGetInvalidSameIndividualNodeLevelBacktrackedDeterministicClashedDescriptors(newTrackedClashedDes,trackingLine,nullptr,calcAlgContext);
					newTrackedClashedDesList = newTrackedClashedDes->append(newTrackedClashedDesList);


					CDependencyIterator depIt = depNode->getAdditionalDependencyIterator();
					while (depIt.hasNext()) {
						CDependency* dependency = depIt.nextDependency();
						// load additional dependencies
						CDependencyTrackPoint* depTrackPoint = dependency->getPreviousDependencyTrackPoint();
						CIndividualProcessNode* addDepNewIndiNode = getCoresspondingIndividualNodeFromDependency(depTrackPoint,calcAlgContext);
						CTrackedClashedDescriptor* newTrackedClashedDes = getFreeTrackedClashedDescriptor(trackingLine,calcAlgContext);
						if (!addDepNewIndiNode) {
							newTrackedClashedDes->initTrackedClashedDescriptor(trackedClashedDes,nullptr,varBindPath,depTrackPoint);
						} else {
							if (minIndiLevel) {
								*minIndiLevel = qMin(*minIndiLevel,addDepNewIndiNode->getIndividualNominalLevelOrAncestorDepth());
							}
							newTrackedClashedDes->initTrackedClashedDescriptor(addDepNewIndiNode,nullptr,varBindPath,depTrackPoint);
						}
						newTrackedClashedDes = tryGetInvalidSameIndividualNodeLevelBacktrackedDeterministicClashedDescriptors(newTrackedClashedDes,trackingLine,nullptr,calcAlgContext);
						newTrackedClashedDesList = newTrackedClashedDes->append(newTrackedClashedDesList);
					}
					return newTrackedClashedDesList;
				}


				CTrackedClashedDescriptor* CCalculationTableauCompletionTaskHandleAlgorithm::tryGetInvalidSameIndividualNodeLevelBacktrackedDeterministicClashedDescriptors(CTrackedClashedDescriptor* trackedClashedDes, CTrackedClashedDependencyLine* trackingLine, cint64* minIndiLevel, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (trackedClashedDes->getConceptDescriptor() == nullptr && trackedClashedDes->isPointingToDeterministicDependencyNode() && !trackedClashedDes->isPointingToIndependentDependencyNode()) {
						CDependencyNode* depNode = trackedClashedDes->getDependencyTrackPoint()->getDependencyNode();
						cint64 currLevel = trackedClashedDes->getAppropriatedIndividualLevel();
						CIndividualProcessNode* indiNode = nullptr;
						CTrackedClashedDescriptor* newTrackedClashedDesList = nullptr;
						if (minIndiLevel) {
							*minIndiLevel = trackedClashedDes->getAppropriatedIndividualLevel();
						}
						if (depNode->hasAppropriateIndividualNode()) {
							indiNode = getCoresspondingIndividualNodeFromDependency(depNode,calcAlgContext);
							if (indiNode->getIndividualNominalLevelOrAncestorDepth() != currLevel) {
								return trackedClashedDes;
							}
						}

						CDependencyIterator depIt = depNode->getAdditionalDependencyIterator();
						while (depIt.hasNext()) {
							CDependency* dependency = depIt.nextDependency();
							// load additional dependencies
							CDependencyTrackPoint* depTrackPoint = dependency->getPreviousDependencyTrackPoint();
							CIndividualProcessNode* addDepNewIndiNode = getCoresspondingIndividualNodeFromDependency(depTrackPoint,calcAlgContext);
							if (addDepNewIndiNode && addDepNewIndiNode->getIndividualNominalLevelOrAncestorDepth() != currLevel) {
								return trackedClashedDes;
							}
						}
						return getBacktrackedDeterministicClashedDescriptors(trackedClashedDes,trackingLine,minIndiLevel,calcAlgContext);
					}
					return trackedClashedDes;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::initializeTrackingLine(CTrackedClashedDependencyLine* trackingLine, CTrackedClashedDescriptor* trackingClashes, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool trackingError = false;
					bool nominalOccured = false;
					cint64 maxBranchingLevel = CINT64_MIN;
					cint64 minIndividualLevel = CINT64_MAX;
					CTrackedClashedDescriptor* trackingClashesIt = trackingClashes;
					while (trackingClashesIt && !trackingError) {
						trackingClashesIt->candidateTracking(&trackingError,&nominalOccured,&maxBranchingLevel,&minIndividualLevel);
						trackingClashesIt = trackingClashesIt->getNextDescriptor();
					}
					if (trackingError) {
						return false;
					}
					trackingLine->initTrackedClashedDependencyLine(nominalOccured,minIndividualLevel,maxBranchingLevel);
					trackingLine->sortInTrackedClashedDescriptors(trackingClashes,true);
					return true;
				}



				CTrackedClashedDescriptor* CCalculationTableauCompletionTaskHandleAlgorithm::createTrackedClashesDescriptors(CClashedDependencyDescriptor* clashes, CCalculationAlgorithmContextBase* calcAlgContext, CMemoryAllocationManager* tmpMemMan) {
					if (!tmpMemMan) {
						tmpMemMan = calcAlgContext->getUsedTemporaryMemoryAllocationManager();
					}

					CTrackedClashedDescriptor* trackingClashes = nullptr;
					CClashedDependencyDescriptor* nextClash = clashes;
					while (nextClash) {
						CTrackedClashedDescriptor* newTrackingClash = nullptr;
						newTrackingClash = createTrackedClashesDescriptor(nextClash,calcAlgContext,tmpMemMan);
						trackingClashes = newTrackingClash->append(trackingClashes);
						nextClash = nextClash->getNext();
					}
					return trackingClashes;
				}



				CTrackedClashedDescriptor* CCalculationTableauCompletionTaskHandleAlgorithm::createTrackedClashesDescriptor(CClashedDependencyDescriptor* clashDes, CCalculationAlgorithmContextBase* calcAlgContext, CMemoryAllocationManager* tmpMemMan) {
					if (!tmpMemMan) {
						tmpMemMan = calcAlgContext->getUsedTemporaryMemoryAllocationManager();
					}

					CTrackedClashedDescriptor* newTrackingClash = nullptr;
					CTrackedClashedDescriptor* clashedTrackDes = dynamic_cast<CTrackedClashedDescriptor*>(clashDes);
					if (clashedTrackDes) {	
						newTrackingClash = CObjectAllocator< CTrackedClashedDescriptor >::allocateAndConstruct(tmpMemMan);
						newTrackingClash->initTrackedClashedDescriptor(clashedTrackDes);
					} else {
						CClashedConceptDescriptor* clashedConDes = dynamic_cast<CClashedConceptDescriptor*>(clashDes);
						if (clashedConDes) {
							newTrackingClash = CObjectAllocator< CTrackedClashedDescriptor >::allocateAndConstruct(tmpMemMan);
							newTrackingClash->initTrackedClashedDescriptor(clashedConDes->getAppropriatedIndividual(),clashedConDes->getConceptDescriptor(),nullptr,clashedConDes->getDependencyTrackPoint());								
						} else {
							CClashedDatatypeValueSpaceExclusionDescriptor* clashedDataVSExDes = dynamic_cast<CClashedDatatypeValueSpaceExclusionDescriptor*>(clashDes);
							if (clashedDataVSExDes) {
								newTrackingClash = CObjectAllocator< CTrackedClashedDescriptor >::allocateAndConstruct(tmpMemMan);
								newTrackingClash->initTrackedClashedDescriptor(clashedDataVSExDes->getAppropriatedIndividual(),nullptr,nullptr,clashedDataVSExDes->getDependencyTrackPoint());								
							} else {							
								CIndividualProcessNode* indiNode = getCoresspondingIndividualNodeFromDependency(clashDes->getDependencyTrackPoint(),calcAlgContext);
								newTrackingClash = CObjectAllocator< CTrackedClashedDescriptor >::allocateAndConstruct(tmpMemMan);
								newTrackingClash->initTrackedClashedDescriptor(indiNode,nullptr,nullptr,clashDes->getDependencyTrackPoint());			
							}
						}
					}
					return newTrackingClash;
				}

				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::getCoresspondingIndividualNodeFromDependency(CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CDependencyNode* depNode = depTrackPoint->getDependencyNode();
					return getCoresspondingIndividualNodeFromDependency(depNode,calcAlgContext);
				}


				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::getCoresspondingIndividualNodeFromDependency(CDependencyNode* depNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualProcessNode* indi = nullptr;
					indi = depNode->getAppropriateIndividualNode();
					//KONCLUDE_ASSERT_X(indi,"getCoresspondingIndividualNodeFromDependency","individual node not available");
					if (indi) {
						indi = getUpToDateIndividual(indi,calcAlgContext);
						if (indi->hasMergedIntoIndividualNodeID()) {
							indi = getCorrectedNominalIndividualNode(indi->getMergedIntoIndividualNodeID(),calcAlgContext);
						}
					}
					return indi;
				}



				QString CCalculationTableauCompletionTaskHandleAlgorithm::generateDebugDependentNominalsString(CIndividualProcessNode* indi, CCalculationAlgorithmContextBase* calcAlgContext) {
					QStringList nominalDependentStringList;
					CSuccessorConnectedNominalSet* connNomSet = indi->getSuccessorNominalConnectionSet(false);
					if (connNomSet) {
						for (CSuccessorConnectedNominalSet::const_iterator it = connNomSet->constBegin(), itEnd = connNomSet->constEnd(); it != itEnd; ++it) {
							cint64 nominalID = *it;
							QString nominalString = QString("%1").arg(nominalID);
							nominalDependentStringList.append(nominalString);
						}
					}
					return nominalDependentStringList.join(", ");
				}


				QString CCalculationTableauCompletionTaskHandleAlgorithm::generateDebugIndiStatusString(CIndividualProcessNode* indi, CCalculationAlgorithmContextBase* calcAlgContext) {
					QStringList statusStringList;
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINVALIDATEBLOCKERFLAGSCOMPINATION)) {
						statusStringList.append("invalid-blocker");
					} 
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPURGEDBLOCKED)) {
						statusStringList.append("pruned");
					} 
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPROCESSINGBLOCKED)) {
						statusStringList.append("processing blocked");
					} 
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATISFIABLECACHED)) {
						statusStringList.append("satisfiable-cached");
					} 
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATISFIABLECACHED)) {
						statusStringList.append("ancestor-satisfiable-cached");
					}
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFDIRECTBLOCKED)) {
						cint64 blockerIndiID = indi->mDebugBlockerIndi->getIndividualID();
						cint64 lastConTag = indi->mDebugBlockerLastConceptDes->getConceptTag();
						statusStringList.append(QString("direct-blocked by %1 (%2)").arg(blockerIndiID).arg(lastConTag));
					}
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINDIRECTBLOCKED)) {
						statusStringList.append("indirect-blocked");
					}
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSIGNATUREBLOCKINGCACHED)) {
						statusStringList.append("signature-blocking-cached");
					}
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSIGNATUREBLOCKINGCACHED)) {
						statusStringList.append("ancestor-signature-blocking-cached");
					}
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFREUSINGINDIVIDUAL)) {
						statusStringList.append("reusing-individual-blocked");
					}
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORREUSINGINDIVIDUALBLOCKED)) {
						statusStringList.append("ancestor-reusing-individual-blocked");
					}
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONBLOCKINGCACHED)) {
						statusStringList.append("saturation-cached-blocked");
					}
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATURATIONBLOCKINGCACHED)) {
						statusStringList.append("ancestor-saturation-cached-blocked");
					}
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHED)) {
						statusStringList.append("completion-graph-cached-blocked");
					}
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALID)) {
						statusStringList.append("completion-graph-caching-invalid");
					}
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHEDNODELOCATED)) {
						statusStringList.append("completion-graph-caching-node-located");
					}
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHEDNODEEXTENDED)) {
						statusStringList.append("completion-graph-caching-node-extended");
					}
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED)) {
						statusStringList.append("completion-graph-caching-invalidated");
					}
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFRETESTCOMPLETIONGRAPHCACHEDDUEDIRECTMODIFIED)) {
						statusStringList.append("completion-graph-caching-retest-due-to-modification");
					}
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION)) {
						statusStringList.append("successor-nominal-connection");
					}
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSUCCESSORNEWNOMINALCONNECTION)) {
						statusStringList.append("successor-new-nominal-connection");
					}
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCONCRETEDATAINDINODE)) {
						statusStringList.append("data-node");
					}
					if (indi->isNominalIndividual()) {
						statusStringList.append("nominal");
					}

					CConceptProcessingQueue* conProQue = indi->getConceptProcessingQueue(false);
					if (conProQue) {
						if (!conProQue->isEmpty() && !indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINVALIDATEBLOCKERFLAGSCOMPINATION)) {
							statusStringList.append("processing");
						}
					}

					QString statusString;
					statusString = statusStringList.join(", ");
					return statusString;
				}

				QString CCalculationTableauCompletionTaskHandleAlgorithm::generateDebugDependencyString(CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					QString dependencyString = QString("null");
					if (depTrackPoint) {
						QString depTypeString;
						CDependencyNode* depNode = depTrackPoint->getDependencyNode();
						switch (depNode->getDependencyType()) {
								case CDependencyNode::DNTINDEPENDENTBASE: {
									depTypeString = QString("INDEPENDENT");
									break;
																		  }
								case CDependencyNode::DNTALLDEPENDENCY: {
									depTypeString = QString("ALL");
									break;
																		}
								case CDependencyNode::DNTSOMEDEPENDENCY: {
									depTypeString = QString("SOME");
									break;
																		 }
								case CDependencyNode::DNTANDDEPENDENCY: {
									depTypeString = QString("AND");
									break;
																		}
								case CDependencyNode::DNTORDEPENDENCY: {
									depTypeString = QString("OR");
									break;
																	   }
								case CDependencyNode::DNTATLEASTDEPENDENCY: {
									depTypeString = QString("ATLEAST");
									break;
																			}
								case CDependencyNode::DNTAUTOMATCHOOSEDEPENDENCY: {
									depTypeString = QString("AUTOMATCHOOSE");
									break;
																				  }
								case CDependencyNode::DNTAUTOMATTRANSACTIONDEPENDENCY: {
									depTypeString = QString("AUTOMATTRANSACTION");
									break;
																					   }
								case CDependencyNode::DNTSELFDEPENDENCY: {
									depTypeString = QString("SELF");
									break;
																		 }
								case CDependencyNode::DNTVALUEDEPENDENCY: {
									depTypeString = QString("VALUE");
									break;
																		  }
								case CDependencyNode::DNTNEGVALUEDEPENDENCY: {
									depTypeString = QString("NEGVALUE");
									break;
																			 }
								case CDependencyNode::DNTDISTINCTDEPENDENCY: {
									depTypeString = QString("DISTINCT");
									break;
																			 }
								case CDependencyNode::DNTMERGEDCONCEPT: {
									depTypeString = QString("MERGEDCONCEPT");
									break;
																		}
								case CDependencyNode::DNTMERGEDLINK: {
									depTypeString = QString("MERGEDLINK");
									break;
																	 }
								case CDependencyNode::DNTMERGEDEPENDENCY: {
									depTypeString = QString("MERGE");
									break;
																		  }
								case CDependencyNode::DNTATMOSTDEPENDENCY: {
									depTypeString = QString("ATMOST");
									break;
																		   }
								case CDependencyNode::DNTQUALIFYDEPENDENCY: {
									depTypeString = QString("QUALIFY");
									break;
																			}
								case CDependencyNode::DNTFUNCTIONALDEPENDENCY: {
									depTypeString = QString("FUNCTIONAL");
									break;
																			   }
								case CDependencyNode::DNTNOMINALDEPENDENCY: {
									depTypeString = QString("NOMINAL");
									break;
																			}
								case CDependencyNode::DNTIMPLICATIONDEPENDENCY: {
									depTypeString = QString("IMPLICATION");
									break;
																				}
								case CDependencyNode::DNTEXPANDEDDEPENDENCY: {
									depTypeString = QString("EXPANDED");
									break;
																			 }
								case CDependencyNode::DNTDATATYPETRIGGERDEPENDENCY: {
									depTypeString = QString("DATATYPETRIGGER");
									break;
																					}

						}

						CConceptDescriptor* depNodeConDes = depNode->getConceptDescriptor();
						QString conceptDepNodeString("null"); 
						if (depNodeConDes) {
							conceptDepNodeString = CConceptTextFormater::getConceptString(depNodeConDes->getConcept(),depNodeConDes->isNegated());
						}
						QString depIndiNodeString;
						CIndividualProcessNode* depIndiNode = depNode->getAppropriateIndividualNode();
						if (depIndiNode) {
							depIndiNodeString = QString("@%1 ").arg(depIndiNode->getIndividualID());
						}
						QString depInfoString;
						if (depNode->isNonDeterministiDependencyNode()) {
							CNonDeterministicDependencyNode* nonDetDepNode = (CNonDeterministicDependencyNode*)depNode;
							depInfoString += QString(" NonDetDep, <%1/%2>").arg(nonDetDepNode->getOpenedDependencyTrackingPointsCount()).arg(nonDetDepNode->getBranchTrackPoints()->getCount());
						}
						depInfoString += QString(" + ...(%1)").arg(depNode->getAdditionalDependencyCount());
						dependencyString = QString("%1-Dependency: {%2}%3%4").arg(depTypeString).arg(conceptDepNodeString).arg(depIndiNodeString).arg(depInfoString);
					}

					cint64 branchTag = depTrackPoint->getBranchingTag();
					dependencyString = QString(" ^%1  --->  %2").arg(branchTag).arg(dependencyString);
					return dependencyString;
				}


				QStringList CCalculationTableauCompletionTaskHandleAlgorithm::generateExtendedDebugConceptSetStringList(CReapplyConceptLabelSet* conSet, CConceptPropagationBindingSetHash* propBindSetHash, CConceptVariableBindingPathSetHash* varBindPathSetHash, CCalculationAlgorithmContextBase* calcAlgContext) {
					QStringList conSetStringList;
					CReapplyConceptLabelSetIterator conSetIt = conSet->getConceptLabelSetIterator(false,false,false);
					while (conSetIt.hasNext()) {
						CConceptDescriptor* conDes = conSetIt.next();
						CConcept* concept = conDes->getConcept();
						cint64 conTag = conDes->getConceptTag();
						if (conTag != 1) {
							QString conceptString("null");
							if (conDes) {
								conceptString = CConceptTextFormater::getConceptString(conDes->getConcept(),conDes->isNegated());
							}
							if (propBindSetHash) {
								CPropagationBindingSet* propBindSet = propBindSetHash->getPropagationBindingSet(concept,false);
								if (propBindSet) {
									QString bindingString;
									CPropagationBindingMap* map = propBindSet->getPropagationBindingMap();
									for (CPropagationBindingMap::const_iterator it = map->constBegin(), itEnd = map->constEnd(); it != itEnd; ++it) {
										cint64 propBindingID = it.key();
										if (!bindingString.isEmpty()) {
											bindingString += QString(", ");
										}
										bindingString += QString::number(propBindingID);
									}
									conceptString += QString(" ~{%1}").arg(bindingString);
								}
							}
							if (varBindPathSetHash) {
								CVariableBindingPathSet* varBindPathSet = varBindPathSetHash->getVariableBindingPathSet(concept,false);
								if (varBindPathSet) {
									QString bindingString;
									CVariableBindingPathMap* map = varBindPathSet->getVariableBindingPathMap();
									for (CVariableBindingPathMap::const_iterator it = map->constBegin(), itEnd = map->constEnd(); it != itEnd; ++it) {
										cint64 varBindingPathID = it.key();
										if (!bindingString.isEmpty()) {
											bindingString += QString(", ");
										}
										QString bindString;
										CVariableBindingPathDescriptor* varBindPathDes = it.value().getVariableBindingPathDescriptor();
										CVariableBindingPath* varBindPath = varBindPathDes->getVariableBindingPath();
										CVariableBindingDescriptor* varBindDes = varBindPath->getVariableBindingDescriptorLinker();
										for (CVariableBindingDescriptor* varBindDesIt = varBindDes; varBindDesIt; varBindDesIt = varBindDesIt->getNext()) {
											CVariableBinding* varBinding = varBindDesIt->getVariableBinding();
											if (!bindString.isEmpty()) {
												bindString += QString(",  ");
											}
											bindString += QString("v-%1/i-%2").arg(varBinding->getBindedVariable()->getPathVariableID()).arg(varBinding->getBindedIndividual()->getIndividualID());
										}
										bindingString += QString("{%1:%2}").arg(varBindingPathID).arg(bindString);
									}
									conceptString += QString(" ~{%1}").arg(bindingString);
								}
							}
							CDependencyTrackPoint* depTrackPoint = conDes->getDependencyTrackPoint();
							if (depTrackPoint) {
								conceptString += generateDebugDependencyString(depTrackPoint,calcAlgContext);
							}
							conSetStringList.append(conceptString);
						}
					}
					return conSetStringList;
				}


				QString CCalculationTableauCompletionTaskHandleAlgorithm::generateExtendedDebugIndiModelStringList(CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					CIndividualProcessNodeVector* indiVec = procDataBox->getIndividualProcessNodeVector();
					cint64 indiCount = indiVec->getItemCount();
					QStringList indiStringList;
					for (cint64 i = 0; i < indiCount; ++i) {
						CIndividualProcessNode* indi = getUpToDateIndividual(i,calcAlgContext);
						if (indi) {
							CIndividualProcessNode* ancIndi = getAncestorIndividual(indi,calcAlgContext);
							CReapplyConceptLabelSet* conSet = indi->getReapplyConceptLabelSet(false);

							CConceptPropagationBindingSetHash* propBindSetHash = indi->getConceptPropagationBindingSetHash(false);
							CConceptVariableBindingPathSetHash* varBindPathSetHash = indi->getConceptVariableBindingPathSetHash(false);

							if (conSet) {
								QString nominalString = QString("");
								if (indi->getNominalIndividual()) {
									nominalString = QString(", ")+CIRIName::getRecentIRIName(indi->getNominalIndividual()->getNameLinker());
								}
								QString indiString = QString("[ %1%2%3 ] = <br>").arg(ancIndi?QString("%1->").arg(ancIndi->getIndividualID()):"").arg(indi->getIndividualID()).arg(nominalString);

								QString statusString = QString("%1").arg(generateDebugIndiStatusString(indi,calcAlgContext));
								indiString += QString("{{%1}d%2}<br>").arg(statusString).arg(indi->getIndividualNominalLevelOrAncestorDepth());
								QStringList conSetStringList = generateExtendedDebugConceptSetStringList(conSet,propBindSetHash,varBindPathSetHash,calcAlgContext);
								//debugTestCriticalConceptSet(conSetStringList,calcAlgContext);

								QString depNomString = generateDebugDependentNominalsString(indi,calcAlgContext);
								if (!depNomString.isEmpty()) {
									indiString += QString("SuccessorDependentNominals: %1<br>\r\n").arg(depNomString);
								}


								QString conSetString(conSetStringList.join("<br>"));
								indiString += QString("{%1} ").arg(conSetString);
								indiStringList.append(indiString);
							}
						}
					}
					for (cint64 i = 0; i < indiCount; ++i) {
						CIndividualProcessNode* indi = getUpToDateIndividual(i,calcAlgContext);
						if (indi && indi->hasMergedIntoIndividualNodeID()) {
							cint64 mergedIntoID = indi->getMergedIntoIndividualNodeID();
							QString meIndiString = indiStringList.value(mergedIntoID);
							meIndiString += QString("+%1").arg(i);
							indiStringList.replace(mergedIntoID,meIndiString);
						}
					}
					for (cint64 i = 0; i < indiCount; ++i) {
						CIndividualProcessNode* indi = getUpToDateIndividual(i,calcAlgContext);
						if (indi && indi->hasMergedIntoIndividualNodeID()) {
							cint64 mergedIntoID = indi->getMergedIntoIndividualNodeID();
							QString indiString = indiStringList.value(i);
							indiString += QString(" -> %1").arg(mergedIntoID);
							indiStringList.replace(i,indiString);
						}
					}

					cint64 succInsertions = 0;
					for (cint64 i = 0; i < indiCount; ++i) {
						CIndividualProcessNode* indi = getUpToDateIndividual(i,calcAlgContext);
						if (indi) {
							CSuccessorIterator succIt = indi->getSuccessorIterator();
							while (succIt.hasNext()) {
								cint64 succIndi = succIt.nextIndividualID();
								QString succString = QString("\t--> %1: ").arg(succIndi);
								CSuccessorRoleIterator succRoleIt = indi->getSuccessorRoleIterator(succIndi);
								while (succRoleIt.hasNext()) {
									CIndividualLinkEdge* link = succRoleIt.next();
									CRole* role = link->getLinkRole();
									succString += QString::number(role->getRoleTag());
									if (role->hasPropertyName()) {
										succString += CIRIName::getRecentIRIName(role->getPropertyNameLinker());
									}
									CDependencyTrackPoint* depTrackPoint = link->getDependencyTrackPoint();
									if (depTrackPoint) {
										succString += generateDebugDependencyString(depTrackPoint,calcAlgContext);
									}
									if (succRoleIt.hasNext()) {
										succString += QString(", ");
									}
								}
								indiStringList.insert(i+1+succInsertions++,succString);
							}
						}
					}
					mDebugIndiModelStringList = indiStringList;
					mDebugIndiModelString = mDebugIndiModelStringList.join("<br><p><br>\r\n");


					if (calcAlgContext->getProcessingDataBox()->hasClashedDescriptorLinker()) {
						CTrackedClashedDescriptor* trackedClashDescriptors = createTrackedClashesDescriptors(calcAlgContext->getProcessingDataBox()->getClashedDescriptorLinker(),calcAlgContext);
						QString clashedString = generateDebugTrackedClashedDescriptorSummaryString(trackedClashDescriptors,calcAlgContext);
						mDebugIndiModelString = mDebugIndiModelString + QString("<br><p><br>\r\n<br><p><br>\r\nClashes<br><p><br>\r\n%1").arg(clashedString);
					}
					return mDebugIndiModelString;
				}



				QString CCalculationTableauCompletionTaskHandleAlgorithm::generateDebugIndiModelStringList(CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					CIndividualProcessNodeVector* indiVec = procDataBox->getIndividualProcessNodeVector();
					cint64 indiCount = indiVec->getItemCount();
					QStringList indiStringList;
					for (cint64 i = 0; i < indiCount; ++i) {
						CIndividualProcessNode* indi = getUpToDateIndividual(i,calcAlgContext);
						if (indi) {
							CIndividualProcessNode* ancIndi = getAncestorIndividual(indi,calcAlgContext);
							CReapplyConceptLabelSet* conSet = indi->getReapplyConceptLabelSet(false);
							if (conSet) {
								QString nominalString = QString("");
								if (indi->getNominalIndividual()) {
									nominalString = QString(", ")+CIRIName::getRecentIRIName(indi->getNominalIndividual()->getNameLinker());
								}
								QString indiString = QString("[ %1%2%3 ] = ").arg(ancIndi?QString("%1->").arg(ancIndi->getIndividualID()):"").arg(indi->getIndividualID()).arg(nominalString);
								QString conSetString;
								CReapplyConceptLabelSetIterator conSetIt = conSet->getConceptLabelSetIterator(false,false,false);
								while (conSetIt.hasNext()) {
									CConceptDescriptor* conDes = conSetIt.next();
									CConcept* concept = conDes->getConcept();
									cint64 conTag = conDes->getConceptTag();
									if (conTag != 1) {
										QString conString = QString("%1%2").arg((conDes->isNegated())?"-":"").arg(conTag);
										if (concept->hasClassName()) {
											conString += CIRIName::getRecentIRIName(concept->getClassNameLinker());
										}
										if (!conSetString.isEmpty()) {
											conSetString += QString(", ");
										}
										conSetString += conString;
									}
								}
								indiString += QString("{%1} ").arg(conSetString);
								indiStringList.append(indiString);
							}
						}
					}
					for (cint64 i = 0; i < indiCount; ++i) {
						CIndividualProcessNode* indi = getUpToDateIndividual(i,calcAlgContext);
						if (indi && indi->hasMergedIntoIndividualNodeID()) {
							cint64 mergedIntoID = indi->getMergedIntoIndividualNodeID();
							QString meIndiString = indiStringList.value(mergedIntoID);
							meIndiString += QString("+%1").arg(i);
							indiStringList.replace(mergedIntoID,meIndiString);
						}
					}
					for (cint64 i = 0; i < indiCount; ++i) {
						CIndividualProcessNode* indi = getUpToDateIndividual(i,calcAlgContext);
						if (indi && indi->hasMergedIntoIndividualNodeID()) {
							cint64 mergedIntoID = indi->getMergedIntoIndividualNodeID();
							QString indiString = indiStringList.value(i);
							indiString += QString(" -> %1").arg(mergedIntoID);
							indiStringList.replace(i,indiString);
						}
					}

					cint64 succInsertions = 0;
					for (cint64 i = 0; i < indiCount; ++i) {
						CIndividualProcessNode* indi = getUpToDateIndividual(i,calcAlgContext);
						if (indi) {
							CSuccessorIterator succIt = indi->getSuccessorIterator();
							while (succIt.hasNext()) {
								cint64 succIndi = succIt.nextIndividualID();
								QString succString = QString("\t--> %1: ").arg(succIndi);
								CSuccessorRoleIterator succRoleIt = indi->getSuccessorRoleIterator(succIndi);
								while (succRoleIt.hasNext()) {
									CIndividualLinkEdge* link = succRoleIt.next();
									CRole* role = link->getLinkRole();
									succString += QString::number(role->getRoleTag());
									if (role->hasPropertyName()) {
										succString += CIRIName::getRecentIRIName(role->getPropertyNameLinker());
									}
									if (succRoleIt.hasNext()) {
										succString += QString(", ");
									}
								}
								indiStringList.insert(i+1+succInsertions++,succString);
							}
						}
					}
					mDebugIndiModelStringList = indiStringList;
					mDebugIndiModelString = mDebugIndiModelStringList.join("\n");
					return mDebugIndiModelString;
				}

				bool CCalculationTableauCompletionTaskHandleAlgorithm::initialNodeInitialize(CIndividualProcessNode*& indiProcNode, bool allowPreprocess, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptAssertionLinker* assertionConceptLinker = indiProcNode->getAssertionConceptLinkerIt();
					CRoleAssertionLinker* assertionRoleLinkerIt = indiProcNode->getAssertionRoleLinkerIt();
					CXSortedNegLinker<CConcept*>* initConceptLinker = indiProcNode->getProcessInitializingConceptLinkerIt();
					CReapplyConceptLabelSet* conSet = indiProcNode->getReapplyConceptLabelSet(false);

					bool initialized = false;

					CConceptSetSignature assInitSignature;
					bool assInitSignatureBuilded = false;


					if (assertionConceptLinker || initConceptLinker || !conSet) {
						CDependencyTrackPoint* depTrackPoint = calcAlgContext->getBaseDependencyNode()->getContinueDependencyTrackPoint();
						CConcept* topConcept = calcAlgContext->getProcessingDataBox()->getOntologyTopConcept();
						addConceptToIndividual(topConcept,false,indiProcNode,depTrackPoint,allowPreprocess,true,calcAlgContext);
						initialized = true;
					}
					if (indiProcNode->isNominalIndividual()) {
						if (!indiProcNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION)) {
							propagateIndividualNodeNominalConnectionToAncestors(indiProcNode,calcAlgContext);
						}
						//propagateIndividualNodeNominalConnectionStatusToAncestors(indiProcNode,indiProcNode,calcAlgContext);
					}
					if (assertionConceptLinker || initConceptLinker) {
						CDependencyTrackPoint* depTrackPoint = calcAlgContext->getBaseDependencyNode()->getContinueDependencyTrackPoint();
						CConcept* univConnNomValueConcept = calcAlgContext->getProcessingDataBox()->getOntology()->getTBox()->getUniversalConnectionNominalValueConcept();
						if (univConnNomValueConcept) {
							addConceptToIndividual(univConnNomValueConcept,false,indiProcNode,depTrackPoint,allowPreprocess,true,calcAlgContext);
						}
					}

					if (assertionConceptLinker) {
						CDependencyTrackPoint* depTrackPoint = calcAlgContext->getBaseDependencyNode()->getContinueDependencyTrackPoint();
						addConceptsToIndividual(assertionConceptLinker,false,indiProcNode,depTrackPoint,allowPreprocess,true,nullptr,calcAlgContext);
						indiProcNode->clearAssertionConcepts();
						initialized = true;
						for (CConceptAssertionLinker* assertionConceptLinkerIt = assertionConceptLinker; assertionConceptLinkerIt; assertionConceptLinkerIt = assertionConceptLinkerIt->getNext()) {
							CConcept* assertionConcept = assertionConceptLinkerIt->getData();
							bool assertionConceptNegation = assertionConceptLinkerIt->isNegated();
							if (assertionConcept->getOperatorCode() != CCNOMINAL) {
								assInitSignature.addConceptSignature(assertionConcept,assertionConceptNegation);
								assInitSignatureBuilded = true;
							}
						}
					}
					if (initConceptLinker) {

						if (!calcAlgContext->getUsedProcessingDataBox()->hasConstructedIndividualNodeInitialized()) {
							calcAlgContext->getUsedProcessingDataBox()->setConstructedIndividualNodeInitialized(true);
							tryCompletionGraphReuse(indiProcNode,calcAlgContext);
						}

						CDependencyTrackPoint* depTrackPoint = calcAlgContext->getBaseDependencyNode()->getContinueDependencyTrackPoint();
						addConceptsToIndividual(initConceptLinker,false,indiProcNode,depTrackPoint,allowPreprocess,true,nullptr,calcAlgContext);
						indiProcNode->clearProcessInitializingConcepts();
						initialized = true;
						for (CXSortedNegLinker<CConcept*>* initConceptLinkerIt = initConceptLinker; initConceptLinkerIt; initConceptLinkerIt = initConceptLinkerIt->getNext()) {
							CConcept* initConcept = initConceptLinkerIt->getData();
							bool initConceptNegation = initConceptLinkerIt->isNegated();
							if (initConcept->getOperatorCode() != CCNOMINAL) {
								assInitSignature.addConceptSignature(initConcept,initConceptNegation);
								assInitSignatureBuilded = true;
							}
						}
					}
					if (assertionRoleLinkerIt) {

						if (allowPreprocess || mCurrentRecProcDepth < mCurrentRecProcDepthLimit) {
							CDependencyTrackPoint* depTrackPoint = calcAlgContext->getBaseDependencyNode()->getContinueDependencyTrackPoint();
							indiProcNode->clearAssertionRoles();
							while (assertionRoleLinkerIt) {
								addRoleAssertion(indiProcNode,assertionRoleLinkerIt,depTrackPoint,calcAlgContext);
								assertionRoleLinkerIt = assertionRoleLinkerIt->getNext();
							}
						} else {
							calcAlgContext->getProcessingDataBox()->getRoleAssertionProcessingQueue(true)->insertIndiviudalProcessNode(indiProcNode);
						}

						initialized = true;
					}

					if (assInitSignatureBuilded && mOptDetExpPreporcessing) {
						cint64 assInitSigValue = assInitSignature.getSignatureValue();
						indiProcNode->setAssertionInitialisationSignatureValue(assInitSigValue);

						STATINC(SIGNATURESAVINGCOUNT,calcAlgContext);
						CProcessingDataBox* processingDataBox = calcAlgContext->getProcessingDataBox();
						CSignatureBlockingCandidateHash* sigNomDelCandHash = processingDataBox->getSignatureNominalDelayingCandidateHash(true);
						sigNomDelCandHash->insertSignatureBlockingCandidate(assInitSigValue,indiProcNode);
					}

					if (mConfAddCachedComputedConsequences && indiProcNode->isNominalIndividual()) {
						addCachedComputedTypes(indiProcNode,calcAlgContext);
					}

					return initialized;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::addCachedComputedTypes(CIndividualProcessNode*& indiProcNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool addedConcepts = false;
					CIndividual* individual = indiProcNode->getNominalIndividual();
					if (individual && mCompConsCacheHandler && !indiProcNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCACHEDCOMPUTEDTYPESADDED)) {
						indiProcNode->addProcessingRestrictionFlags(CIndividualProcessNode::PRFCACHEDCOMPUTEDTYPESADDED);
						CSortedNegLinker<CConcept*>* typeConceptLinker = mCompConsCacheHandler->getCachedTypesConceptLinker(individual,calcAlgContext);
						if (typeConceptLinker) {
							CDependencyTrackPoint* depTrackPoint = calcAlgContext->getBaseDependencyNode()->getContinueDependencyTrackPoint();
							CDependencyTrackPoint* expDepTrackPoint = nullptr;
							createANDDependency(expDepTrackPoint,indiProcNode,nullptr,depTrackPoint,calcAlgContext);
							addConceptsToIndividual(typeConceptLinker,false,indiProcNode,expDepTrackPoint,true,false,nullptr,calcAlgContext);
							addedConcepts = true;
						}
					}
					return addedConcepts;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::individualNodeInitializing(CIndividualProcessNode*& indiProcNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					indiProcNode = getLocalizedIndividual(indiProcNode,true,calcAlgContext);

					indiProcNode->setProcessingQueued(false);
					indiProcNode->setExtendedQueueProcessing(true);
					calcAlgContext->setCurrentIndividualNode(indiProcNode);


					if (mIndiNodeFromQueueType == INQT_IMMEDIATE) {
						indiProcNode->setImmediatelyProcessingQueued(false);
					} else if (mIndiNodeFromQueueType == INQT_DETEXP) {
						indiProcNode->setDeterministicExpandingProcessingQueued(false);
					} else if (mIndiNodeFromQueueType == INQT_DEPTHNORMAL || mIndiNodeFromQueueType == INQT_NOMINAL) {
						indiProcNode->setRegularDepthProcessingQueued(false);
					} else if (mIndiNodeFromQueueType == INQT_BLOCKREACT) {
						indiProcNode->setBlockedReactivationProcessingQueued(false);
					} else if (mIndiNodeFromQueueType == INQT_DELAYEDNOMINAL) {
						indiProcNode->setDelayedNominalProcessingQueued(false);
					}


					indiProcNode->resetLastProcessingPriority();

					//if (indiProcNode->getNominalIndividual()) {
					//	if (CIRIName::getRecentIRIName(indiProcNode->getNominalIndividual()->getIndividualNameLinker()) == "#robert_david_bright_1965") {
					//		bool bug = true;
					//	}
					//}
					//} 
					initialNodeInitialize(indiProcNode,true,calcAlgContext);


					if (mIndiNodeFromQueueType == INQT_CACHETEST) {
						testIndividualNodeUnsatisfiableCached(indiProcNode,calcAlgContext);
					} else if (mIndiNodeFromQueueType == INQT_VSTSATTESTING) {
						checkValueSpaceDistinctSatisfiability(indiProcNode,calcAlgContext);
					} else if (mIndiNodeFromQueueType == INQT_VSTRIGGERING) {
						triggerValueSpaceConcepts(indiProcNode,calcAlgContext);
					}



					if (isIndividualNodeProcessingBlocked(indiProcNode,calcAlgContext)) {
						eliminiateBlockedIndividuals(indiProcNode,calcAlgContext);
						return false;
					}


					// counting same concept signature
					//conSet = indiProcNode->getReapplyConceptLabelSet(false);
					//if (conSet) {
					//	cint64 sigVal = conSet->getConceptSignature()->getSignatureValue();
					//	cint64 prevCountVal = mIndiNodeInitConceptSigCountHash.value(sigVal,0);
					//	mIndiNodeInitConceptSigCountHash.insert(sigVal,prevCountVal+1);
					//}

					if (mConfSignatureSaving) {
						addSignatureIndividualNodeBlockerCandidate(indiProcNode,calcAlgContext);
					}
					return true;
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::checkValueSpaceDistinctSatisfiability(CIndividualProcessNode* processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (mDatatypeHandler && mConfDatatypeReasoning) {

						//mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
						//QFile file(QString("datatype-error.txt"));
						//if (file.open(QIODevice::WriteOnly)) {
						//	file.write(mEndTaskDebugIndiModelString.replace("<br>","\r\n").toLocal8Bit());
						//	file.close();
						//}


						CTaskProcessorContext* taskProcessorContext = calcAlgContext->getUsedTaskProcessorContext();
						CPROCESSINGSET<CIndividualProcessNode*>* distinctIndividualNodeCollectionSet = CObjectParameterizingAllocator< CPROCESSINGSET<CIndividualProcessNode*>,CContext* >::allocateAndConstructAndParameterize(taskProcessorContext->getMemoryAllocationManager(),taskProcessorContext);
						CPROCESSINGLIST<CIndividualProcessNode*>* distinctIndividualNodeCollectionList = CObjectParameterizingAllocator< CPROCESSINGLIST<CIndividualProcessNode*>,CContext* >::allocateAndConstructAndParameterize(taskProcessorContext->getMemoryAllocationManager(),taskProcessorContext);

						if (mDatatypeHandler->requiresSatisfiabilityChecking(processIndi,calcAlgContext)) {
							distinctIndividualNodeCollectionSet->insert(processIndi);
							distinctIndividualNodeCollectionList->append(processIndi);

							while (!distinctIndividualNodeCollectionList->isEmpty()) {
								CIndividualProcessNode* distinctIndiNode = distinctIndividualNodeCollectionList->takeFirst();
								CDistinctHash* distinctHash = distinctIndiNode->getDistinctHash(false);
								if (distinctHash) {
									CDistinctIterator distinctIterator = distinctHash->getDistinctIterator();
									while (distinctIterator.hasNext()) {
										cint64 nextDistinctIndiID = distinctIterator.nextDistinctIndividualID();
										CIndividualProcessNode* nextDistinctIndiNode = getLocalizedIndividual(nextDistinctIndiID,calcAlgContext);
										if (mDatatypeHandler->involveDistinctNodeForSatisfiabilityChecking(nextDistinctIndiNode,calcAlgContext)) {
											if (!distinctIndividualNodeCollectionSet->contains(nextDistinctIndiNode)) {
												distinctIndividualNodeCollectionSet->insert(nextDistinctIndiNode);
												distinctIndividualNodeCollectionList->append(nextDistinctIndiNode);
											}
										}
									}
								}
							}		
						}

						mDatatypeHandler->checkSatisfiability(processIndi,distinctIndividualNodeCollectionSet,calcAlgContext);
					}
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::triggerValueSpaceConcepts(CIndividualProcessNode* processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (mDatatypeHandler && mConfDatatypeReasoning) {
						CConceptDescriptor* triggeredConceptLinker = nullptr;


						//mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
						//QFile file(QString("debug-model.txt"));
						//if (file.open(QIODevice::WriteOnly)) {
						//	file.write(mEndTaskDebugIndiModelString.replace("<br>","\r\n").toLocal8Bit());
						//	file.close();
						//}

						mDatatypeHandler->triggerValueSpaceConcepts(processIndi,triggeredConceptLinker,calcAlgContext);

						CConceptDescriptor* invTriggeredConceptLinker = nullptr;
						while (triggeredConceptLinker) {
							CConceptDescriptor* tmpTriggeredConceptLinker = triggeredConceptLinker;
							triggeredConceptLinker = triggeredConceptLinker->getNextConceptDesciptor();
							tmpTriggeredConceptLinker->clearNext();
							invTriggeredConceptLinker = tmpTriggeredConceptLinker->append(invTriggeredConceptLinker);
						}


						for (CConceptDescriptor* triggeredConceptLinkerIt = invTriggeredConceptLinker; triggeredConceptLinkerIt; triggeredConceptLinkerIt = triggeredConceptLinkerIt->getNext()) {
							CConcept* triggeredConcept = triggeredConceptLinkerIt->getConcept();
							bool triggeredNegation = triggeredConceptLinkerIt->isNegated();
							CDependencyTrackPoint* triggeredDependencyTrackPoint = triggeredConceptLinkerIt->getDependencyTrackPoint();
							addConceptToIndividual(triggeredConcept,triggeredNegation,processIndi,triggeredDependencyTrackPoint,true,false,calcAlgContext);
						}

					}
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::tryCompletionGraphReuse(CIndividualProcessNode* processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool minimalCompletionGraph = false;
					bool minimalCompletionGraphConnection = false;
					if (mReuseCompGraphCacheHandler && mConfCompGraphReuseCacheRetrieval) {
						STATINC(COMPLETIONGRAPHREUSECACHERETRIEVALCOUNT,calcAlgContext);

						KONCLUCE_TASK_ALGORITHM_TIME_MEASURE_INSTRUCTION(
							mComplGraphReuseCacheRetrieval.start();
						);

						CReuseCompletionGraphCacheEntry* reuseCompGraphCacheEntry = mReuseCompGraphCacheHandler->getReuseCompletionGraphEntry(processIndi,&minimalCompletionGraph,&minimalCompletionGraphConnection,calcAlgContext);

						KONCLUCE_TASK_ALGORITHM_TIME_MEASURE_INSTRUCTION(
							cint64 timeElapsed = mComplGraphReuseCacheRetrieval.elapsed();
							STATINCM(TIMECOMPLETIONGRAPHREUSERETRIVAL,timeElapsed,calcAlgContext);
						);

						if (reuseCompGraphCacheEntry) {
							STATINC(COMPLETIONGRAPHREUSECACHERETRIEVALSUCCESSCOUNT,calcAlgContext);
							CXSortedNegLinker<CConcept*>* initConceptLinkerIt = processIndi->getProcessInitializingConceptLinkerIt();
							CTaskProcessorContext* processorContext = calcAlgContext->getUsedTaskProcessorContext();
							CSatisfiableCalculationTask* reuseSatCalcTask = (CSatisfiableCalculationTask*)reuseCompGraphCacheEntry->getJobInstantiation();
							CProcessingDataBox* reuseProcessingDataBox = reuseSatCalcTask->getProcessingDataBox();
							cint64 reuseBranchingTag = reuseProcessingDataBox->getProcessContext()->getUsedProcessTagger()->getCurrentBranchingTag();
							bool deterministicReuse = reuseBranchingTag == 0 && minimalCompletionGraph && minimalCompletionGraphConnection;


							if (!deterministicReuse && mConfCompGraphNonDeterministicReuse || deterministicReuse && mConfCompGraphDeterministicReuse) {


								cint64 taskCreationCount = 2;
								if (deterministicReuse) {
									STATINC(COMPLETIONGRAPHREUSECACHEREUSINGDETCOUNT,calcAlgContext);
									taskCreationCount = 1;
								} else {
									STATINC(COMPLETIONGRAPHREUSECACHEREUSINGNONDETCOUNT,calcAlgContext);
								}

								CSatisfiableCalculationTask* newTaskList = createDependendBranchingTaskList(taskCreationCount,calcAlgContext);

								CSatisfiableCalculationTask* newTaskIt = newTaskList;

								CREUSECOMPLETIONGRAPHDependencyNode* reuseDepNode = createREUSECOMPLETIONGRAPHDependency(processIndi,nullptr,nullptr,calcAlgContext);


								for (cint64 i = 0; i < taskCreationCount; ++i) {

									CSatisfiableCalculationTask* newSatCalcTask = newTaskIt;

									bool reusingAlternative = i == 0;

									if (reusingAlternative) {

										CDependencyTrackPoint* newDependencyTrackPoint = nullptr;
										if (deterministicReuse) {
											newDependencyTrackPoint = calcAlgContext->getBaseDependencyNode()->getContinueDependencyTrackPoint();
										} else {
											newDependencyTrackPoint = createNonDeterministicDependencyTrackPointBranch(reuseDepNode,false,calcAlgContext);
										}


										CProcessingDataBox* newProcessingDataBox = newSatCalcTask->getProcessingDataBox();
										newProcessingDataBox->initProcessingDataBox(reuseProcessingDataBox);

										CProcessContext* newProcessContext = newSatCalcTask->getProcessContext(processorContext);
										CCalculationAlgorithmContextBase* newCalcAlgContext = createCalculationAlgorithmContext(processorContext,newProcessContext,newSatCalcTask);
										CMemoryAllocationManager* newAllocMemMan = newCalcAlgContext->getUsedProcessTaskMemoryAllocationManager();

										CProcessTagger* newProcessTagger = newCalcAlgContext->getUsedProcessTagger();
										if (!deterministicReuse) {
											newProcessTagger->incBranchingTag();
											if (!minimalCompletionGraphConnection) {
												newProcessingDataBox->setMaximumDeterministicBranchTag(-1);
											}
										}
										newProcessTagger->incLocalizationTag();


										CIndividualProcessNodeVector* indiNodeVec = newProcessingDataBox->getIndividualProcessNodeVector();
										cint64 nextIndiID = indiNodeVec->getItemCount();


										CIndividualProcessNode* newIndi = CObjectParameterizingAllocator< CIndividualProcessNode,CProcessContext* >::allocateAndConstructAndParameterize(newAllocMemMan,newProcessContext);
										newIndi->setIndividualID(nextIndiID);
										newIndi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFINVALIDBLOCKINGORCACHING);
										newProcessingDataBox->setConstructedIndividualNode(newIndi);
										indiNodeVec->setData(nextIndiID,newIndi);

										addConceptsToIndividual(initConceptLinkerIt,false,newIndi,newDependencyTrackPoint,false,true,nullptr,newCalcAlgContext);

										CConcept* topConcept = calcAlgContext->getProcessingDataBox()->getOntologyTopConcept();
										addConceptToIndividual(topConcept,false,newIndi,newDependencyTrackPoint,false,true,newCalcAlgContext);

										prepareBranchedTaskProcessing(newIndi,newSatCalcTask,newCalcAlgContext);


									} else {

										CProcessContext* newProcessContext = newSatCalcTask->getProcessContext(processorContext);
										CCalculationAlgorithmContextBase* newCalcAlgContext = createCalculationAlgorithmContext(processorContext,newProcessContext,newSatCalcTask);
										CMemoryAllocationManager* newAllocMemMan = newCalcAlgContext->getUsedProcessTaskMemoryAllocationManager();

										CIndividualProcessNode* newLocIndiNode = getLocalizedIndividual(processIndi,false,newCalcAlgContext);
										prepareBranchedTaskProcessing(newLocIndiNode,newSatCalcTask,newCalcAlgContext);

									}

									// set new task priority
									double newTaskPriority = calcAlgContext->getUsedTaskPriorityStrategy()->getPriorityForTaskReusing(newSatCalcTask,calcAlgContext->getUsedSatisfiableCalculationTask(),reusingAlternative);
									newSatCalcTask->setTaskPriority(newTaskPriority);

									newTaskIt = (CSatisfiableCalculationTask*)newTaskIt->getNext();

								}


								processorContext->getTaskProcessorCommunicator()->communicateTaskCreation(newTaskList);

								throw CCalculationStopProcessingException(true);
							}
						} else {
							STATINC(COMPLETIONGRAPHREUSECACHERETRIEVALFAILEDCOUNT,calcAlgContext);
						}
					}
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::eliminiateBlockedIndividuals(CIndividualProcessNode*& indiProcNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					//if (indiProcNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINVALIDATEBLOCKERFLAGSCOMPINATION)) {
					if (indiProcNode->hasBlockedIndividualsLinker()) {
						// reactivate all blocked individuals
						reactivateBlockedIndividuals(indiProcNode,calcAlgContext);
					}
					if (indiProcNode->hasBlockingFollower()) {
						CBlockingFollowSet* followerSet = indiProcNode->getBlockingFollowSet(false);
						for (CPROCESSSET<cint64>::const_iterator it = followerSet->constBegin(), itEnd = followerSet->constEnd(); it != itEnd; ++it) {
							cint64 blockingIndiNodeID = *it;
							CIndividualProcessNode* blockingIndiNode = getUpToDateIndividual(blockingIndiNodeID,calcAlgContext);
							addIndividualToBlockingUpdateReviewProcessingQueue(blockingIndiNode,calcAlgContext);
						}
					}
					CXLinker<CIndividualProcessNode*>* processingBlockedNodeLinker = indiProcNode->getProcessingBlockedIndividualsLinker();
					while (processingBlockedNodeLinker) {
						CIndividualProcessNode* blockedNode = processingBlockedNodeLinker->getData();
						CIndividualProcessNode* locBlockedNode = getLocalizedIndividual(blockedNode,true,calcAlgContext);
						locBlockedNode->addProcessingRestrictionFlags(CIndividualProcessNode::PRFBLOCKINGRETESTDUEPROCESSINGCOMPLETED);
						addIndividualToProcessingQueue(locBlockedNode,calcAlgContext);
						processingBlockedNodeLinker = processingBlockedNodeLinker->getNext();
					}
					indiProcNode->clearBlockedIndividualsLinker();
					//}
				}




				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::getDelayProcessingBlockingNominalNode(CIndividualProcessNode* testIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualProcessNode* nominalBlockerIndividual = nullptr;
					CProcessingDataBox* processingDataBox = calcAlgContext->getProcessingDataBox();
					CSignatureBlockingCandidateHash* sigNomDelCandHash = processingDataBox->getSignatureNominalDelayingCandidateHash(false);

					CReapplyConceptLabelSet* conSet = testIndi->getReapplyConceptLabelSet(false);
					if (conSet && sigNomDelCandHash) {					
						cint64 assConSig = testIndi->getAssertionInitialisationSignatureValue();
						CSignatureBlockingCandidateIterator candIt(sigNomDelCandHash->getBlockingCandidatesIterator(assConSig));
						while (!nominalBlockerIndividual && candIt.hasNext()) {
							cint64 candIndiID = candIt.next(true);
							if (candIndiID != testIndi->getIndividualID()) {
								CIndividualProcessNode* candIndiNode = getUpToDateIndividual(candIndiID,calcAlgContext);
								if (candIndiNode->isNominalIndividual() && candIndiNode->getNominalIndividual() && !candIndiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPURGEDBLOCKED)) {
									CReapplyConceptLabelSet* blockerConSet = candIndiNode->getReapplyConceptLabelSet(false);
									if (isLabelConceptSubSetIgnoreNominals(conSet,blockerConSet,calcAlgContext)) {
										nominalBlockerIndividual = candIndiNode;
									}
								}
							}
						}
					}
					return nominalBlockerIndividual;
				}




				bool CCalculationTableauCompletionTaskHandleAlgorithm::tryDelayNominalProcessing(CConceptProcessDescriptor* conProDes, CIndividualProcessNode* testIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (canDelayNominalProcessing(conProDes,testIndi,calcAlgContext)) {
						testIndi->setNominalProcessingDelayingChecked(true);

						bool notConnectedNominal = false;
						CSuccessorIterator succIt(testIndi->getSuccessorIterator());
						if (!succIt.hasNext()) {
							notConnectedNominal = true;
						}

						if (notConnectedNominal) {
							CIndividualProcessNode* blockerNominalIndiNode = getDelayProcessingBlockingNominalNode(testIndi,calcAlgContext);
							if (blockerNominalIndiNode) {
								CProcessingDataBox* processingDataBox = calcAlgContext->getProcessingDataBox();
								CIndividualUnsortedProcessingQueue* delayingNominalProcessingQueu = processingDataBox->getDelayingNominalProcessingQueue(true);
								delayingNominalProcessingQueu->insertIndiviudalProcessNode(testIndi);
								testIndi->setDelayedNominalProcessingQueued(true);
								return true;
							}
						}
					}
					return false;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::canDelayNominalProcessing(CConceptProcessDescriptor* conProDes, CIndividualProcessNode* testIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (testIndi->isNominalIndividual() && testIndi->getNominalIndividual()) {
						if (conProDes->getProcessPriority() < mImmediatelyProcessPriority) {
							if (!testIndi->hasNominalProcessingDelayingChecked()) {
								return true;
							}
						}
					}
					return false;
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::individualNodeConclusion(CIndividualProcessNode*& indiProcNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (mIndiNodeConcludeUnsatCaching) {
						testIndividualNodeUnsatisfiableCached(indiProcNode,calcAlgContext);
					}

					calcAlgContext->setCurrentIndividualNode(nullptr);
					addIndividualToProcessingQueue(indiProcNode,calcAlgContext);

					//CConceptProcessingQueue* conProQue = indiProcNode->getConceptProcessingQueue(false);
					//if (nodeProcessingBreaked && conProQue && !conProQue->isEmpty()) {
					//	addIndividualToProcessingQueue(indiProcNode,false,calcAlgContext);
					//} else if (!conProQue || conProQue->isEmpty()) {
					//	propagateIndividualProcessedAndReactivate(indiProcNode,calcAlgContext);
					//}
				}

				bool CCalculationTableauCompletionTaskHandleAlgorithm::tableauRuleProcessing(CIndividualProcessNode*& indiProcNode,CConceptProcessDescriptor*& conProcDes, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* conDes = conProcDes->getConceptDescriptor();
					bool conNeg = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					cint64 conOpCode = concept->getOperatorCode();

					if (tryDelayNominalProcessing(conProcDes,indiProcNode,calcAlgContext)) {
						return false;
					}

					if (needsIndividualNodeExpansionBlockingTest(conProcDes,indiProcNode,calcAlgContext)) {
						if (isIndividualNodeExpansionBlocked(indiProcNode,calcAlgContext)) {
							return false;
						}
					}


					tableauRuleChoice(indiProcNode,conProcDes,calcAlgContext);

					return true;
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::tableauRuleChoice(CIndividualProcessNode*& indiProcNode,CConceptProcessDescriptor*& conProcDes, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* conDes = conProcDes->getConceptDescriptor();
					bool conNeg = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					cint64 conOpCode = concept->getOperatorCode();

					++mCurrentRecProcDepth;


					TableauRuleFunction func = nullptr;
					if (!conNeg) {
						func = mPosJumpFuncVec[conOpCode];
						if (func) {
							(this->*func)(indiProcNode,conProcDes,false,calcAlgContext);
						}
						mLastJumpFunc = func;
					} else {
						func = mNegJumpFuncVec[conOpCode];
						if (func) {
							(this->*func)(indiProcNode,conProcDes,true,calcAlgContext);
						}
						mLastJumpFunc = func;
					}


					--mCurrentRecProcDepth;

				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::applyNegAutomatChooseRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					applyAutomatChooseRule(processIndi,conProDes,!negate,calcAlgContext);
				}

				void CCalculationTableauCompletionTaskHandleAlgorithm::applyNegANDRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					applyANDRule(processIndi,conProDes,!negate,calcAlgContext);
				}

				void CCalculationTableauCompletionTaskHandleAlgorithm::applyNegSOMERule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					applySOMERule(processIndi,conProDes,!negate,calcAlgContext);
				}

				void CCalculationTableauCompletionTaskHandleAlgorithm::applyNegALLRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					applyALLRule(processIndi,conProDes,!negate,calcAlgContext);
				}

				void CCalculationTableauCompletionTaskHandleAlgorithm::applyNegORRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					applyORRule(processIndi,conProDes,!negate,calcAlgContext);
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::applyNegATMOSTRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					applyATLEASTRule(processIndi,conProDes,negate,calcAlgContext);
				}

				void CCalculationTableauCompletionTaskHandleAlgorithm::applyNegATLEASTRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					applyATMOSTRule(processIndi,conProDes,negate,calcAlgContext);
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::applyAutomatChooseRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(AUTOMATEINITCOUNT,calcAlgContext);
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					bool conceptNegation = negate;
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();

					// create dependency
					CDependencyTrackPoint* nextDepTrackPoint = nullptr;
					CAUTOMATCHOOSEDependencyNode* andDepNode = createAUTOMATCHOOSEDependency(nextDepTrackPoint,processIndi,conDes,depTrackPoint,calcAlgContext);

					while (conceptOpLinkerIt) {
						CConcept* opConcept = conceptOpLinkerIt->getData();
						bool opNegation = conceptOpLinkerIt->isNegated();
						if (opNegation == negate) {
							addConceptToIndividual(opConcept,false,processIndi,nextDepTrackPoint,true,false,calcAlgContext);
						}
						conceptOpLinkerIt = conceptOpLinkerIt->getNext();
					}
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::applyAutomatANDRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					bool conceptNegation = negate;
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();

					applyAutomatTransactions(processIndi,conProDes,concept,negate,calcAlgContext);
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::applyAutomatTransactions(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, CConcept* concept, bool negated, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(AUTOMATERULEAPPLICATIONCOUNT,calcAlgContext);

					CConceptDescriptor* baseConDes = conProDes->getConceptDescriptor();
					bool reapplied = conProDes->isConceptReapplied();
					CIndividualLinkEdge* restLink = getLinkProcessingRestriction(conProDes);
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();


					cint64 opCode = concept->getOperatorCode();
					CSortedNegLinker<CConcept*>* opConcepts = concept->getOperandList();
					if (opCode == CCAQAND || opCode == CCIMPLAQAND || opCode == CCBRANCHAQAND) {
						while (opConcepts) {
							++mAppliedALLRuleCount;
							STATINC(AUTOMATESTATECOUNT,calcAlgContext);
							CConcept* opCon = opConcepts->getData();
							bool opConNeg = opConcepts->isNegated();
							applyAutomatTransactions(processIndi,conProDes,opCon,opConNeg,calcAlgContext);
							opConcepts = opConcepts->getNext();
						}
					} else if (opCode == CCAQALL || opCode == CCIMPLAQALL || opCode == CCBRANCHAQALL) {
						CRole* role = concept->getRole();


						if (restLink) {
							if (restLink->getLinkRole() == role) {
								++mAppliedALLRuleCount;
								STATINC(AUTOMATETRANSACTIONCOUNT,calcAlgContext);

								CDependencyTrackPoint* nextDepTrackPoint = nullptr;
								CAUTOMATTRANSACTIONDependencyNode* allDepNode = nullptr;
								bool allDepNodeCreated = false;

								CIndividualProcessNode* succIndi = getSuccessorIndividual(processIndi,restLink,calcAlgContext);
								CIndividualProcessNode* locSuccIndi = nullptr;
								CReapplyConceptLabelSet* conLabelSet = succIndi->getReapplyConceptLabelSet(false);
								CSortedNegLinker<CConcept*>* conceptOpLinkerIt = opConcepts;
								while (conceptOpLinkerIt) {
									CConcept* opConcept = conceptOpLinkerIt->getData();
									bool opConNeg = conceptOpLinkerIt->isNegated() ^ negated;
									if (!conLabelSet->containsConcept(opConcept,opConNeg)) {

										// create dependency
										if (!allDepNodeCreated) {
											allDepNodeCreated = true;
											allDepNode = createAUTOMATTRANSACTIONDependency(nextDepTrackPoint,processIndi,baseConDes,depTrackPoint,restLink->getDependencyTrackPoint(),calcAlgContext);
										}

										// add concept
										if (!locSuccIndi) {
											locSuccIndi = getLocalizedIndividual(succIndi,false,calcAlgContext);
											conLabelSet = locSuccIndi->getReapplyConceptLabelSet(true);
										}
										addConceptToIndividual(opConcept,opConNeg,locSuccIndi,nextDepTrackPoint,true,true,calcAlgContext);
									}
									conceptOpLinkerIt = conceptOpLinkerIt->getNext();
								}
								if (locSuccIndi) {
									addIndividualToProcessingQueue(locSuccIndi,calcAlgContext);
								}
							}
						} else {
							CReapplyRoleSuccessorHash* roleSuccHash = processIndi->getReapplyRoleSuccessorHash(false);
							if (roleSuccHash) {
								CRoleSuccessorLinkIterator roleSuccIt = roleSuccHash->getRoleSuccessorLinkIterator(role);
								while (roleSuccIt.hasNext()) {
									++mAppliedALLRuleCount;
									STATINC(AUTOMATETRANSACTIONCOUNT,calcAlgContext);

									CDependencyTrackPoint* nextDepTrackPoint = nullptr;
									CAUTOMATTRANSACTIONDependencyNode* allDepNode = nullptr;
									bool allDepNodeCreated = false;

									CIndividualLinkEdge* link = roleSuccIt.next(true);
									CIndividualProcessNode* succIndi = getSuccessorIndividual(processIndi,link,calcAlgContext);
									CIndividualProcessNode* locSuccIndi = nullptr;
									CReapplyConceptLabelSet* conLabelSet = succIndi->getReapplyConceptLabelSet(false);
									CSortedNegLinker<CConcept*>* conceptOpLinkerIt = opConcepts;
									while (conceptOpLinkerIt) {
										CConcept* opConcept = conceptOpLinkerIt->getData();
										bool opConNeg = conceptOpLinkerIt->isNegated() ^ negated;
										if (!conLabelSet->containsConcept(opConcept,opConNeg)) {

											// create dependency
											if (!allDepNodeCreated) {
												allDepNodeCreated = true;
												allDepNode = createAUTOMATTRANSACTIONDependency(nextDepTrackPoint,processIndi,baseConDes,depTrackPoint,link->getDependencyTrackPoint(),calcAlgContext);
											}

											// add concept
											if (!locSuccIndi) {
												locSuccIndi = getLocalizedIndividual(succIndi,false,calcAlgContext);
												conLabelSet = locSuccIndi->getReapplyConceptLabelSet(true);
											}
											addConceptToIndividual(opConcept,opConNeg,locSuccIndi,nextDepTrackPoint,true,true,calcAlgContext);
										}
										conceptOpLinkerIt = conceptOpLinkerIt->getNext();
									}
									if (locSuccIndi) {
										addIndividualToProcessingQueue(locSuccIndi,calcAlgContext);
									}
								}
							}
						}
						if (!reapplied) {
							// add to reapply queue
							addConceptToReapplyQueue(baseConDes,role,processIndi,true,depTrackPoint,calcAlgContext);
						}
					} else if (!reapplied) {
						addConceptToIndividual(concept,negated,processIndi,depTrackPoint,true,true,calcAlgContext);
					}

				}


				CREPRESENTATIVEGROUNDINGDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createREPRESENTATIVEGROUNDINGDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CVariableBindingPath* selectVarBindPath, CCalculationAlgorithmContextBase* calcAlgContext) {
					CREPRESENTATIVEGROUNDINGDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createREPRESENTATIVEGROUNDINGDependency(implContinueDepTrackPoint,processIndi,conDes,prevDepTrackPoint,selectVarBindPath,calcAlgContext);
					}
					return depNode;
				}

				CREPRESENTATIVEJOINDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createREPRESENTATIVEJOINDependency(CDependencyTrackPoint*& joinContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* otherDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CREPRESENTATIVEJOINDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createREPRESENTATIVEJOINDependency(joinContinueDepTrackPoint,processIndi,conDes,prevDepTrackPoint,otherDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CREPRESENTATIVEBINDVARIABLEDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createREPRESENTATIVEBINDVARIABLEDependency(CDependencyTrackPoint*& andDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CREPRESENTATIVEBINDVARIABLEDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createREPRESENTATIVEBINDVARIABLEDependency(andDepTrackPoint,processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CREPRESENTATIVEIMPLICATIONDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createREPRESENTATIVEIMPLICATIONDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContextBase* calcAlgContext) {
					CREPRESENTATIVEIMPLICATIONDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createREPRESENTATIVEIMPLICATIONDependency(implContinueDepTrackPoint,processIndi,conDes,prevDepTrackPoint,prevOtherDependencies,calcAlgContext);
					}
					return depNode;
				}

				CREPRESENTATIVEALLDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createREPRESENTATIVEALLDependency(CDependencyTrackPoint*& allDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CREPRESENTATIVEALLDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createREPRESENTATIVEALLDependency(allDepTrackPoint,processIndi,conDes,prevDepTrackPoint,linkDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CREPRESENTATIVEANDDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createREPRESENTATIVEANDDependency(CDependencyTrackPoint*& andDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CREPRESENTATIVEANDDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createREPRESENTATIVEANDDependency(andDepTrackPoint,processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CRESOLVEREPRESENTATIVEDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createRESOLVEREPRESENTATIVEDependency(CDependencyTrackPoint*& andDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CRepresentativeVariableBindingPathMap* resolveVarBindPathMap, CRepresentativePropagationMap* resolveRepPropMap, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* additionalDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CRESOLVEREPRESENTATIVEDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createRESOLVEREPRESENTATIVEDependency(andDepTrackPoint,processIndi,conDes,resolveVarBindPathMap,resolveRepPropMap,prevDepTrackPoint,additionalDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}










				CPROPAGATEVARIABLECONNECTIONDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createPROPAGATEVARIABLECONNECTIONDependency(CIndividualProcessNode* processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CPROPAGATEVARIABLECONNECTIONDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createPROPAGATEVARIABLECONNECTIONDependency(processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CVARBINDPROPAGATEIMPLICATIONDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createVARBINDPROPAGATEIMPLICATIONDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContextBase* calcAlgContext) {
					CVARBINDPROPAGATEIMPLICATIONDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createVARBINDPROPAGATEIMPLICATIONDependency(implContinueDepTrackPoint,processIndi,conDes,prevDepTrackPoint,prevOtherDependencies,calcAlgContext);
					}
					return depNode;
				}

				CVARBINDPROPAGATEGROUNDINGDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createVARBINDPROPAGATEGROUNDINGDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContextBase* calcAlgContext) {
					CVARBINDPROPAGATEGROUNDINGDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createVARBINDPROPAGATEGROUNDINGDependency(implContinueDepTrackPoint,processIndi,conDes,prevDepTrackPoint,prevOtherDependencies,calcAlgContext);
					}
					return depNode;
				}

				CVARBINDPROPAGATEALLDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createVARBINDPROPAGATEALLDependency(CDependencyTrackPoint*& allDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CVARBINDPROPAGATEALLDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createVARBINDPROPAGATEALLDependency(allDepTrackPoint,processIndi,conDes,prevDepTrackPoint,linkDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CVARBINDPROPAGATEANDDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createVARBINDPROPAGATEANDDependency(CDependencyTrackPoint*& andDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CVARBINDPROPAGATEANDDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createVARBINDPROPAGATEANDDependency(andDepTrackPoint,processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CPROPAGATEVARIABLEBINDINGDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createPROPAGATEVARIABLEBINDINGDependency(CDependencyTrackPoint*& andDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContextBase* calcAlgContext) {
					CPROPAGATEVARIABLEBINDINGDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createPROPAGATEVARIABLEBINDINGDependency(andDepTrackPoint,processIndi,conDes,prevDepTrackPoint,prevOtherDependencies,calcAlgContext);
					}
					return depNode;
				}

				CPROPAGATEVARIABLEBINDINGSSUCCESSORDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createPROPAGATEVARIABLEBINDINGSSUCCESSORDependency(CDependencyTrackPoint*& allDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CPROPAGATEVARIABLEBINDINGSSUCCESSORDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createPROPAGATEVARIABLEBINDINGSSUCCESSORDependency(allDepTrackPoint,processIndi,conDes,prevDepTrackPoint,linkDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CVARBINDVARIABLEDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createVARBINDVARIABLEDependency(CDependencyTrackPoint*& andDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CVARBINDVARIABLEDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createVARBINDVARIABLEDependency(andDepTrackPoint,processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CVARBINDPROPAGATEJOINDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createVARBINDPROPAGATEJOINDependency(CDependencyTrackPoint*& continueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* otherDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CVARBINDPROPAGATEJOINDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createVARBINDPROPAGATEJOINDependency(continueDepTrackPoint,processIndi,conDes,prevDepTrackPoint,otherDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}






				CBINDPROPAGATEGROUNDINGDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createBINDPROPAGATEGROUNDINGDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContextBase* calcAlgContext) {
					CBINDPROPAGATEGROUNDINGDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createBINDPROPAGATEGROUNDINGDependency(implContinueDepTrackPoint,processIndi,conDes,prevDepTrackPoint,prevOtherDependencies,calcAlgContext);
					}
					return depNode;
				}

				CPROPAGATECONNECTIONAWAYDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createPROPAGATECONNECTIONAWAYDependency(CIndividualProcessNode* processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CPROPAGATECONNECTIONAWAYDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createPROPAGATECONNECTIONAWAYDependency(processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CPROPAGATECONNECTIONDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createPROPAGATECONNECTIONDependency(CIndividualProcessNode* processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CPROPAGATECONNECTIONDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createPROPAGATECONNECTIONDependency(processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CBINDPROPAGATECYCLEDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createBINDPROPAGATECYCLEDependency(CDependencyTrackPoint*& continueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* triggDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CBINDPROPAGATECYCLEDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createBINDPROPAGATECYCLEDependency(continueDepTrackPoint,processIndi,conDes,prevDepTrackPoint,triggDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CBINDPROPAGATEALLDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createBINDPROPAGATEALLDependency(CDependencyTrackPoint*& allDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CBINDPROPAGATEALLDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createBINDPROPAGATEALLDependency(allDepTrackPoint,processIndi,conDes,prevDepTrackPoint,linkDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CPROPAGATEBINDINGSSUCCESSORDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createPROPAGATEBINDINGSSUCCESSORDependency(CDependencyTrackPoint*& allDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CPROPAGATEBINDINGSSUCCESSORDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createPROPAGATEBINDINGSSUCCESSORDependency(allDepTrackPoint,processIndi,conDes,prevDepTrackPoint,linkDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CBINDPROPAGATEIMPLICATIONDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createBINDPROPAGATEIMPLICATIONDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContextBase* calcAlgContext) {
					CBINDPROPAGATEIMPLICATIONDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createBINDPROPAGATEIMPLICATIONDependency(implContinueDepTrackPoint,processIndi,conDes,prevDepTrackPoint,prevOtherDependencies,calcAlgContext);
					}
					return depNode;
				}

				CANDDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createANDDependency(CDependencyTrackPoint*& andDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CANDDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createANDDependency(andDepTrackPoint,processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CBINDPROPAGATEANDDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createBINDPROPAGATEANDDependency(CDependencyTrackPoint*& andDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CBINDPROPAGATEANDDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createBINDPROPAGATEANDDependency(andDepTrackPoint,processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CPROPAGATEBINDINGDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createPROPAGATEBINDINGDependency(CDependencyTrackPoint*& andDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContextBase* calcAlgContext) {
					CPROPAGATEBINDINGDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createPROPAGATEBINDINGDependency(andDepTrackPoint,processIndi,conDes,prevDepTrackPoint,prevOtherDependencies,calcAlgContext);
					}
					return depNode;
				}

				CBINDVARIABLEDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createBINDVARIABLEDependency(CDependencyTrackPoint*& andDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CBINDVARIABLEDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createBINDVARIABLEDependency(andDepTrackPoint,processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CNOMINALDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createNOMINALDependency(CDependencyTrackPoint*& nominalContDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* nominalDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CNOMINALDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createNOMINALDependency(nominalContDepTrackPoint,processIndi,conDes,prevDepTrackPoint,nominalDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CAUTOMATCHOOSEDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createAUTOMATCHOOSEDependency(CDependencyTrackPoint*& andDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CAUTOMATCHOOSEDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createAUTOMATCHOOSEDependency(andDepTrackPoint,processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CSOMEDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createSOMEDependency(CDependencyTrackPoint*& someDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSOMEDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createSOMEDependency(someDepTrackPoint,processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CSELFDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createSELFDependency(CDependencyTrackPoint*& someDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSELFDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createSELFDependency(someDepTrackPoint,processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CVALUEDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createVALUEDependency(CDependencyTrackPoint*& valueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* nominalDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CVALUEDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createVALUEDependency(valueDepTrackPoint,processIndi,conDes,prevDepTrackPoint,nominalDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CNEGVALUEDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createNEGVALUEDependency(CDependencyTrackPoint*& negValueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* nominalDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CNEGVALUEDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createNEGVALUEDependency(negValueDepTrackPoint,processIndi,conDes,prevDepTrackPoint,nominalDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CALLDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createALLDependency(CDependencyTrackPoint*& allDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CALLDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createALLDependency(allDepTrackPoint,processIndi,conDes,prevDepTrackPoint,linkDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CMERGEDCONCEPTDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createMERGEDCONCEPTDependency(CDependencyTrackPoint*& mergedConceptContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* mergePrevDepTrackPoint, CDependencyTrackPoint* conceptPrevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMERGEDCONCEPTDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createMERGEDCONCEPTDependency(mergedConceptContinueDepTrackPoint,processIndi,conDes,mergePrevDepTrackPoint,conceptPrevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CMERGEDLINKDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createMERGEDLINKDependency(CDependencyTrackPoint*& mergedLinkContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* mergePrevDepTrackPoint, CDependencyTrackPoint* linkPrevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMERGEDLINKDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createMERGEDLINKDependency(mergedLinkContinueDepTrackPoint,processIndi,mergePrevDepTrackPoint,linkPrevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CFUNCTIONALDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createFUNCTIONALDependency(CDependencyTrackPoint*& functionalContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* prevLink1DependencyTrackPoint, CDependencyTrackPoint* prevLink2DependencyTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CFUNCTIONALDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createFUNCTIONALDependency(functionalContinueDepTrackPoint,processIndi,conDes,prevDepTrackPoint,prevLink1DependencyTrackPoint,prevLink2DependencyTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CDISTINCTDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createDISTINCTDependency(CDependencyTrackPoint*& distinctDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CDISTINCTDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createDISTINCTDependency(distinctDepTrackPoint,processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CAUTOMATTRANSACTIONDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createAUTOMATTRANSACTIONDependency(CDependencyTrackPoint*& allDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CAUTOMATTRANSACTIONDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createAUTOMATTRANSACTIONDependency(allDepTrackPoint,processIndi,conDes,prevDepTrackPoint,linkDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CATLEASTDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createATLEASTDependency(CDependencyTrackPoint*& atleastDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CATLEASTDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createATLEASTDependency(atleastDepTrackPoint,processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CORDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createORDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CORDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createORDependency(processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CATMOSTDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createATMOSTDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CATMOSTDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createATMOSTDependency(processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CMERGEDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createMERGEDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMERGEDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createMERGEDependency(processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CREUSEINDIVIDUALDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createREUSEINDIVIDUALDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CREUSEINDIVIDUALDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createREUSEINDIVIDUALDependency(processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CREUSECOMPLETIONGRAPHDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createREUSECOMPLETIONGRAPHDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CREUSECOMPLETIONGRAPHDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createREUSECOMPLETIONGRAPHDependency(processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CREUSECONCEPTSDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createREUSECONCEPTSDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CREUSECONCEPTSDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createREUSECONCEPTSDependency(processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}

				CQUALIFYDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createQUALIFYDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CQUALIFYDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createQUALIFYDependency(processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}



				CIMPLICATIONDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createIMPLICATIONDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIMPLICATIONDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createIMPLICATIONDependency(implContinueDepTrackPoint,processIndi,conDes,prevDepTrackPoint,prevOtherDependencies,calcAlgContext);
					}
					return depNode;
				}


				CEXPANDEDDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createEXPANDEDDependency(CDependencyTrackPoint*& expContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContextBase* calcAlgContext) {
					CEXPANDEDDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createEXPANDEDDependency(expContinueDepTrackPoint,processIndi,prevDepTrackPoint,prevOtherDependencies,calcAlgContext);
					}
					return depNode;
				}


				CCONNECTIONDependencyNode* CCalculationTableauCompletionTaskHandleAlgorithm::createCONNECTIONDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CCONNECTIONDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = calcAlgContext->getUsedDependencyFactory()->createCONNECTIONDependency(processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					}
					return depNode;
				}






















































				void CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEGROUNDINGRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					CConcept* concept = conDes->getConcept();
					bool negated = conDes->isNegated();
					cint64 opCount = concept->getOperandCount();

					CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(false);

					CConceptRepresentativePropagationSetHash* repPropSetHash = processIndi->getConceptRepresentativePropagationSetHash(false);
					CRepresentativePropagationSet* repPropSet = repPropSetHash->getRepresentativePropagationSet(concept,false);

					STATINC(VARBINDRULEGROUNDINGAPPLICATIONCOUNT,calcAlgContext);

					if (repPropSet) {

						CRepresentativePropagationDescriptor* outRepPropDes = repPropSet->getOutgoingRepresentativePropagationDescriptorLinker();
						if (outRepPropDes) {
							CRepresentativeVariableBindingPathSetData* repVarBindPathSetData = outRepPropDes->getRepresentativeVariableBindingPathSetData();
							if (repVarBindPathSetData) {
								CRepresentativeVariableBindingPathSetMigrateData* repVarBindPathSetMigData = repVarBindPathSetData->getMigrateData(false);
								if (repVarBindPathSetMigData) {
									CRepresentativeVariableBindingPathMap* repVarBindPathSetMap = repVarBindPathSetMigData->getRepresentativeVariableBindingPathMap();

									KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(mBeforeGroundingDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext));

									CPROCESSINGHASH<CConcept*,CVariableBindingPath*>* groundedConVarBindPathHash = nullptr;
									CSortedNegLinker<CConcept*>* newGroundedLinker = mGroundingHandler->getGroundingConceptLinker(processIndi,repVarBindPathSetMap,concept,negated,groundedConVarBindPathHash,calcAlgContext);

									if (newGroundedLinker) {
										for (CSortedNegLinker<CConcept*>* newGroundedLinkerIt = newGroundedLinker; newGroundedLinkerIt; newGroundedLinkerIt = newGroundedLinkerIt->getNext()) {
											STATINC(VARBINDGROUNDINGCOUNT,calcAlgContext);
											++mStatRepresentativeGroundingCount;
											CConcept* newGroundedCocnept = newGroundedLinkerIt->getData();
											bool newGroundedConceptNegation = newGroundedLinkerIt->isNegated();

											CVariableBindingPath* selectedVarBindPath = groundedConVarBindPathHash->value(newGroundedCocnept);

											CDependencyTrackPoint* nextDepTrackPoint = nullptr;
											CREPRESENTATIVEGROUNDINGDependencyNode* groundingDepNode = createREPRESENTATIVEGROUNDINGDependency(nextDepTrackPoint,processIndi,conDes,outRepPropDes->getDependencyTrackPoint(),selectedVarBindPath,calcAlgContext);

											addConceptToIndividual(newGroundedCocnept,newGroundedConceptNegation,processIndi,nextDepTrackPoint,true,false,calcAlgContext);

										}
									}

									KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(mAfterGroundingDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext));

								}
							}
						}
					}
				}

				void CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEJOINRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					cint64 opCount = concept->getOperandCount();
					CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();


					CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(false);
					CConceptDescriptor* joinConDes = nullptr;
					CDependencyTrackPoint* joinDepTrackPoint = nullptr;
					CCondensedReapplyQueue* reapplyQueue = nullptr;

					CConcept* joinConcept = opLinker->getData();
					bool joinConceptNegation = opLinker->isNegated();
					CSortedNegLinker<CConcept*>* triggerLinker = opLinker->getNext();

					CSortedLinker<CVariable*>* varLinker = concept->getVariableLinker();

					STATINC(VARBINDRULEJOINAPPLICATIONCOUNT,calcAlgContext);

					bool propagateJoins = false;
					bool createJoinConcept = false;
					if (!conSet->getConceptDescriptorAndReapplyQueue(joinConcept,joinConDes,joinDepTrackPoint,reapplyQueue)) {
						// search next not existing trigger
						bool allTriggersAvailable = true;
						conSet = processIndi->getReapplyConceptLabelSet(true);
						CSortedNegLinker<CConcept*>* triggerLinkerIt = nullptr;
						for (triggerLinkerIt = triggerLinker; triggerLinkerIt && allTriggersAvailable; triggerLinkerIt = triggerLinkerIt->getNext()) {
							CSortedNegLinker<CConcept*>* nextTrigger = triggerLinkerIt;
							CConcept* triggerConcept = nextTrigger->getData();
							CConceptDescriptor* triggerConDes = nullptr;
							CDependencyTrackPoint* triggerDepTrackPoint = nullptr;
							if (conSet->getConceptDescriptor(triggerConcept,triggerConDes,triggerDepTrackPoint)) {
								if (triggerConDes->isNegated() == nextTrigger->isNegated()) {
									return;
								}
							} else {
								allTriggersAvailable = false;
								break;
							}
						}

						if (!allTriggersAvailable) {
							// install to trigger
							CSortedNegLinker<CConcept*>* nextTrigger = triggerLinkerIt;
							CConcept* triggerConcept = nextTrigger->getData();
							bool triggerNegation = !nextTrigger->isNegated();
							if (!isConceptInReapplyQueue(conDes,triggerConcept,triggerNegation,processIndi,calcAlgContext)) {
								addConceptToReapplyQueue(conDes,triggerConcept,triggerNegation,processIndi,(CProcessingRestrictionSpecification*)nullptr,depTrackPoint,calcAlgContext);
							}
						} else {
							propagateJoins = true;
							createJoinConcept = true;
						}
					} else {
						propagateJoins = true;
					}

					//mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);

					bool propagationsDone = false;
					if (propagateJoins) {

						for (CSortedNegLinker<CConcept*>* triggerLinkerIt = triggerLinker; triggerLinkerIt; triggerLinkerIt = triggerLinkerIt->getNext()) {
							CSortedNegLinker<CConcept*>* nextTrigger = triggerLinkerIt;
							CConcept* triggerConcept = nextTrigger->getData();
							if (!isConceptInReapplyQueue(conDes,triggerConcept,false,processIndi,calcAlgContext)) {
								addConceptToReapplyQueue(conDes,triggerConcept,false,processIndi,(CProcessingRestrictionSpecification*)nullptr,depTrackPoint,calcAlgContext);
							}
						}

						CConceptPropagationBindingSetHash* conPropBindingSetHash = processIndi->getConceptPropagationBindingSetHash(false);
						CConceptRepresentativePropagationSetHash* repPropSetHash = processIndi->getConceptRepresentativePropagationSetHash(false);
						CPropagationBindingSet* propBindingSet = nullptr;
						if (conPropBindingSetHash && repPropSetHash) {
							propBindingSet = conPropBindingSetHash->getPropagationBindingSet(concept,false);
							if (propBindingSet) {

								CPropagationRepresentativeTransitionExtension* propRepTransExt = propBindingSet->getPropagationRepresentativeTransitionExtension(false);

								CSortedNegLinker<CConcept*>* leftTriggerLinker = triggerLinker;
								CSortedNegLinker<CConcept*>* rightTriggerLinker = triggerLinker->getNext();

								CConcept* leftConcept = leftTriggerLinker->getData();
								CConcept* rightConcept = rightTriggerLinker->getData();

								CRepresentativePropagationSet* leftRepPropSet = repPropSetHash->getRepresentativePropagationSet(leftConcept,false);
								CRepresentativePropagationSet* rightRepPropSet = repPropSetHash->getRepresentativePropagationSet(rightConcept,false);

								bool examineTransExt = false;

								CRepresentativePropagationDescriptor* leftRepPropDes = leftRepPropSet->getOutgoingRepresentativePropagationDescriptorLinker();
								CRepresentativePropagationDescriptor* rightRepPropDes = rightRepPropSet->getOutgoingRepresentativePropagationDescriptorLinker();

								if (leftRepPropSet && rightRepPropSet && leftRepPropDes && rightRepPropDes) {

									if (!propRepTransExt) {
										examineTransExt = true;
									} else {
										if (propRepTransExt->getLastAnalysedPropagateAllFlag() != propBindingSet->getPropagateAllFlag() ||
													propRepTransExt->getLastAnalysedPropagationBindingDescriptor() != propBindingSet->getPropagationBindingDescriptorLinker() || 
													propRepTransExt->getLeftLastRepresentativeJoiningDescriptor() != leftRepPropDes || 
													propRepTransExt->getRightLastRepresentativeJoiningDescriptor() != rightRepPropDes) {

											examineTransExt = true;
										}
									}
								}

								if (examineTransExt) {
									++mStatRepresentativeJoinCount;

									conPropBindingSetHash = processIndi->getConceptPropagationBindingSetHash(true);
									repPropSetHash = processIndi->getConceptRepresentativePropagationSetHash(true);
									propBindingSet = conPropBindingSetHash->getPropagationBindingSet(concept,true);
									propRepTransExt = propBindingSet->getPropagationRepresentativeTransitionExtension(true);

									CRepresentativePropagationSet* joinRepPropSet = repPropSetHash->getRepresentativePropagationSet(joinConcept,true);

									CPropagationBindingDescriptor* lastAnalyPropBindDes = propRepTransExt->getLastAnalysedPropagationBindingDescriptor();
									CPropagationBindingDescriptor* propBindDes = propBindingSet->getPropagationBindingDescriptorLinker();

									bool propAllFlag = propBindingSet->hasPropagateAllFlag();

									CRepresentativeVariableBindingPathSetData* leftRepData = leftRepPropDes->getRepresentativeVariableBindingPathSetData();
									CRepresentativeVariableBindingPathSetData* rightRepData = rightRepPropDes->getRepresentativeVariableBindingPathSetData();


									if (areRepresentativesJoinable(processIndi,leftRepData,rightRepData,varLinker,calcAlgContext)) {

										CRepresentativeJoiningData* joinData = nullptr;
										CRepresentativeJoiningData* locJoinData = nullptr;
										CRepresentativeJoiningHash* repJoiningHash = procDataBox->getRepresentativeJoiningHash(false);
										if (repJoiningHash) {
											joinData = repJoiningHash->getRepresentativeJoiningData(leftRepData,rightRepData,false);
										}

										if (!joinData) {

											CRepresentativeVariableBindingPathSetHash* repVarBindPathSetHash = nullptr;
											if (!leftRepData->hasJoiningData(concept)) {
												if (!repVarBindPathSetHash) {
													repVarBindPathSetHash = procDataBox->getRepresentativeVariableBindingPathSetHash(true);
												}
												leftRepData = repVarBindPathSetHash->getRepresentativeVariableBindingPathSetData(leftRepData,true);
											}
											if (!rightRepData->hasJoiningData(concept)) {
												if (!repVarBindPathSetHash) {
													repVarBindPathSetHash = procDataBox->getRepresentativeVariableBindingPathSetHash(true);
												}
												rightRepData = repVarBindPathSetHash->getRepresentativeVariableBindingPathSetData(rightRepData,true);
											}

											if (!locJoinData) {
												repJoiningHash = procDataBox->getRepresentativeJoiningHash(true);
												locJoinData = repJoiningHash->getRepresentativeJoiningData(leftRepData,rightRepData,true);
												joinData = locJoinData;
											}

											CRepresentativeVariableBindingPathSetJoiningKeyMap* leftJoiningKeyMap = getRepresentativeJoiningKeyData(leftRepData,concept,calcAlgContext);
											CRepresentativeVariableBindingPathSetJoiningKeyMap* rightJoiningKeyMap = getRepresentativeJoiningKeyData(rightRepData,concept,calcAlgContext);

											CRepresentativeJoiningCommonKeyMap* repJoinCommonKeyMap = joinData->getRepresentativeJoiningCommonKeyMap();

											createCommonJoiningKeyMap(repJoinCommonKeyMap,leftJoiningKeyMap,leftRepData,rightJoiningKeyMap,rightRepData,true,calcAlgContext);
										}

										CRepresentativeJoiningCommonKeyMap* repJoinCommonKeyMap = joinData->getRepresentativeJoiningCommonKeyMap();
										if (!repJoinCommonKeyMap->isEmpty()) {
											++mStatRepresentativeJoinedCount;

											if (propAllFlag) {
												CRepresentativeJoiningAllDataExtension* joinAllExtData = joinData->getJoiningAllExtension(false);
												if (!joinAllExtData) {
													if (!locJoinData) {
														repJoiningHash = procDataBox->getRepresentativeJoiningHash(true);
														locJoinData = repJoiningHash->getRepresentativeJoiningData(leftRepData,rightRepData,true);
														joinData = locJoinData;
													}

													joinAllExtData = joinData->getJoiningAllExtension(true);

													createCommonJoiningAll(joinData->getRepresentativeJoiningCommonKeyMap(),joinAllExtData,leftRepData,rightRepData,calcAlgContext);
												}

												CRepresentativeVariableBindingPathSetData* joinedRepData = joinAllExtData->getRepresentativeVariableBindingPathSetData();

												CRepresentativePropagationMap* leftRepPropMap = propRepTransExt->getLeftRepresentativePropagationMap();
												CRepresentativePropagationMap* rightRepPropMap = propRepTransExt->getRightRepresentativePropagationMap();
												if (!leftRepPropMap->contains(leftRepData->getRepresentativeID())) {
													CDependencyTrackPoint* nextDepTrackPoint = nullptr;
													CREPRESENTATIVEANDDependencyNode* repPropDepNode = createREPRESENTATIVEANDDependency(nextDepTrackPoint,processIndi,conDes,leftRepPropDes->getDependencyTrackPoint(),calcAlgContext);
													CRepresentativePropagationDescriptor* propagateRepDes = CObjectAllocator<CRepresentativePropagationDescriptor>::allocateAndConstruct(taskMemMan);
													propagateRepDes->initRepresentativeDescriptor(leftRepData,nextDepTrackPoint);
													leftRepPropMap->insert(leftRepData->getRepresentativeID(),CRepresentativePropagationMapData(propagateRepDes));
												}
												if (!rightRepPropMap->contains(rightRepData->getRepresentativeID())) {
													CDependencyTrackPoint* nextDepTrackPoint = nullptr;
													CREPRESENTATIVEANDDependencyNode* repPropDepNode = createREPRESENTATIVEANDDependency(nextDepTrackPoint,processIndi,conDes,rightRepPropDes->getDependencyTrackPoint(),calcAlgContext);
													CRepresentativePropagationDescriptor* propagateRepDes = CObjectAllocator<CRepresentativePropagationDescriptor>::allocateAndConstruct(taskMemMan);
													propagateRepDes->initRepresentativeDescriptor(rightRepData,nextDepTrackPoint);
													rightRepPropMap->insert(rightRepData->getRepresentativeID(),CRepresentativePropagationMapData(propagateRepDes));
												}

												CDependencyTrackPoint* leftNextResolveDepTrackPoint = nullptr;
												CRESOLVEREPRESENTATIVEDependencyNode* leftResolveRepNode = createRESOLVEREPRESENTATIVEDependency(leftNextResolveDepTrackPoint,processIndi,nullptr,joinAllExtData->getLeftResolveVariableBindingPathMap(false),leftRepPropMap,leftRepPropDes->getDependencyTrackPoint(),nullptr,calcAlgContext);
												
												CDependencyTrackPoint* rightNextResolveDepTrackPoint = nullptr;
												CRESOLVEREPRESENTATIVEDependencyNode* rightResolveRepNode = createRESOLVEREPRESENTATIVEDependency(rightNextResolveDepTrackPoint,processIndi,nullptr,joinAllExtData->getRightResolveVariableBindingPathMap(false),rightRepPropMap,rightRepPropDes->getDependencyTrackPoint(),nullptr,calcAlgContext);

												CDependencyTrackPoint* joinNextDepTrackPoint = nullptr;
												CREPRESENTATIVEJOINDependencyNode* joinResolveRepNode = createREPRESENTATIVEJOINDependency(joinNextDepTrackPoint,processIndi,conDes,leftNextResolveDepTrackPoint,rightNextResolveDepTrackPoint,calcAlgContext);

												CRepresentativePropagationDescriptor* propagateRepDes = CObjectAllocator<CRepresentativePropagationDescriptor>::allocateAndConstruct(taskMemMan);
												propagateRepDes->initRepresentativeDescriptor(joinedRepData,joinNextDepTrackPoint);
												joinRepPropSet->addIncomingRepresentativePropagation(propagateRepDes);
												updateRepresentativePropagationSet(processIndi,joinRepPropSet,calcAlgContext);

												if (!joinConDes) {
													joinDepTrackPoint = joinNextDepTrackPoint;
													joinConDes = addConceptToIndividualReturnConceptDescriptor(joinConcept,false,processIndi,joinNextDepTrackPoint,false,false,calcAlgContext);
												}
											} else {
												// ToDo!
											}

											propagationsDone = true;
										}
									}

									propRepTransExt->setLeftLastRepresentativeJoiningDescriptor(leftRepPropDes);
									propRepTransExt->setRightLastRepresentativeJoiningDescriptor(rightRepPropDes);
									propRepTransExt->setLastAnalysedPropagationBindingDescriptor(propBindDes);
									propRepTransExt->setLastAnalysedPropagateAllFlag(propBindingSet->hasPropagateAllFlag());
								}
							}
						}
					}

					if (propagationsDone) {
						if (!createJoinConcept) {
							reapplyConceptUpdatedRepresentative(processIndi,joinConDes,joinDepTrackPoint,conSet,reapplyQueue,calcAlgContext);
						}
					}
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::hasCommonVariableBindings(CIndividualProcessNode*& processIndi, CRepresentativeVariableBindingPathMap* leftRepVarBindMap, CRepresentativeVariableBindingPathMap* rightRepVarBindMap, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (rightRepVarBindMap->count() < leftRepVarBindMap->count()) {
						return hasCommonVariableBindings(processIndi,rightRepVarBindMap,leftRepVarBindMap,calcAlgContext);
					}

					if (leftRepVarBindMap->count()*mMapComparisonDirectLookupFactor < rightRepVarBindMap->count()) {
						for (CRepresentativeVariableBindingPathMap::const_iterator it1 = leftRepVarBindMap->constBegin(), it1End = leftRepVarBindMap->constEnd(); it1 != it1End; ++it1) {
							if (rightRepVarBindMap->contains(it1.key())) {
								return true;
							}
						}
						return false;
					} else {
						CRepresentativeVariableBindingPathMap::const_iterator it1 = leftRepVarBindMap->constBegin(), it1End = leftRepVarBindMap->constEnd();
						CRepresentativeVariableBindingPathMap::const_iterator it2 = rightRepVarBindMap->constBegin(), it2End = rightRepVarBindMap->constEnd();

						while (it1 != it1End && it2 != it2End) {
							cint64 key1 = it1.key();
							cint64 key2 = it2.key();
							if (key1 == key2) {
								return true;
							}
							if (key1 < key2) {
								++it1;
							} else if (key2 < key1) {
								++it2;
							}
						}
						return false;
					}
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::areRepresentativesJoinable(CIndividualProcessNode*& processIndi, CRepresentativeVariableBindingPathSetData* leftRepData, CRepresentativeVariableBindingPathSetData* rightRepData, CSortedLinker<CVariable*>* varLinker, CCalculationAlgorithmContextBase* calcAlgContext) {
					CRepresentativeVariableBindingPathSetMigrateData* leftRepMigData = leftRepData->getMigrateData();
					CRepresentativeVariableBindingPathSetMigrateData* rightRepMigData = rightRepData->getMigrateData();
					if (varLinker && leftRepMigData && rightRepMigData) {
						CRepresentativeVariableBindingPathMap* leftRepVarBindMap = leftRepMigData->getRepresentativeVariableBindingPathMap();
						CRepresentativeVariableBindingPathMap* rightRepVarBindMap = rightRepMigData->getRepresentativeVariableBindingPathMap();

						const CRepresentativeVariableBindingPathMapData& leftRepVarBindData = leftRepVarBindMap->constBegin().value();
						const CRepresentativeVariableBindingPathMapData& rightRepVarBindData = rightRepVarBindMap->constBegin().value();
						CVariableBindingPath* leftVarBindPath = leftRepVarBindData.getVariableBindingPath();
						CVariableBindingPath* rightVarBindPath = rightRepVarBindData.getVariableBindingPath();
						if (leftVarBindPath->getVariableBindingCount() == 1 && rightVarBindPath->getVariableBindingCount() == 1) {
							if (!hasCommonVariableBindings(processIndi,leftRepVarBindMap,rightRepVarBindMap,calcAlgContext)) {
								++mStatRepresentativeJoinQuickFailCount;
								return false;
							}
						}
					}
					return true;
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::createCommonJoiningAll(CRepresentativeJoiningCommonKeyMap* repJoinCommonKeyMap, CRepresentativeJoiningAllDataExtension* joinAllExtData, CRepresentativeVariableBindingPathSetData* leftRepData, CRepresentativeVariableBindingPathSetData* rightRepData, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					CProcessContext* processContext = calcAlgContext->getUsedProcessContext();
					CRepresentativeVariableBindingPathSetData* repData = CObjectParameterizingAllocator< CRepresentativeVariableBindingPathSetData,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,processContext);
					repData->initRepresentativeVariableBindingPathData(nullptr);
					repData->setRepresentativeID(procDataBox->getNextRepresentativeVariableBindingPathID(true));
					repData->setMigratable(false);
					repData->incUseCount();
					repData->incShareCount();
					CRepresentativeVariableBindingPathSetMigrateData* repMigData = repData->getMigrateData(true);
					CRepresentativeVariableBindingPathMap* varBindPathResolveMap = repMigData->getRepresentativeVariableBindingPathMap();

					CRepresentativeVariableBindingPathMap* leftRepVarBindPathResolveMap = joinAllExtData->getLeftResolveVariableBindingPathMap(true);
					CRepresentativeVariableBindingPathMap* rightRepVarBindPathResolveMap = joinAllExtData->getRightResolveVariableBindingPathMap(true);

					for (CRepresentativeJoiningCommonKeyMap::const_iterator it = repJoinCommonKeyMap->constBegin(), itEnd = repJoinCommonKeyMap->constEnd(); it != itEnd; ++it) {
						cint64 joiningKey = it.key();
						const CRepresentativeJoiningCommonKeyData& commonKeyData = it.value();
						CRepresentativeVariableBindingPathSetJoiningKeyDataMap* leftKeyDataMap = commonKeyData.getLeftJoiningDataMap();
						CRepresentativeVariableBindingPathSetJoiningKeyDataMap* rightKeyDataMap = commonKeyData.getRightJoiningDataMap();

						for (CRepresentativeVariableBindingPathSetJoiningKeyDataMap::const_iterator it1 = leftKeyDataMap->constBegin(), it1End = leftKeyDataMap->constEnd(); it1 != it1End; ++it1) {
							for (CRepresentativeVariableBindingPathSetJoiningKeyDataMap::const_iterator it2 = rightKeyDataMap->constBegin(), it2End = rightKeyDataMap->constEnd(); it2 != it2End; ++it2) {
								CVariableBindingPath* varBindPath1 = it1.value();
								CVariableBindingPath* varBindPath2 = it2.value();

								CVariableBindingPath* mergedVarBindPath = getJoinedVariableBindingPath(varBindPath1,varBindPath2,calcAlgContext);

								leftRepVarBindPathResolveMap->insert(mergedVarBindPath->getPropagationID(),CRepresentativeVariableBindingPathMapData(mergedVarBindPath,varBindPath1,leftRepData));
								rightRepVarBindPathResolveMap->insert(mergedVarBindPath->getPropagationID(),CRepresentativeVariableBindingPathMapData(mergedVarBindPath,varBindPath2,rightRepData));

								varBindPathResolveMap->insert(mergedVarBindPath->getPropagationID(),CRepresentativeVariableBindingPathMapData(mergedVarBindPath,repData));
							}
						}
					}

					++mStatRepresentativeJoinCombinesCount;
					repMigData->getRepresentativeContainingMap()->insertContainedRepresentative(repData,false);
					repData->addKeySignatureValue(repData->getRepresentativeID());
					CRepresentativeVariableBindingPathSetHash* repVarBindPathSetHash = procDataBox->getRepresentativeVariableBindingPathSetHash(true);
					repVarBindPathSetHash->insertRepresentativeVariableBindingPathSetData(repData);
					joinAllExtData->setRepresentativeVariableBindingPathSetData(repData);
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::createCommonJoiningKeyMap(CRepresentativeJoiningCommonKeyMap* repJoinCommonKeyMap, CRepresentativeVariableBindingPathSetJoiningKeyMap* firstJoiningKeyMap, CRepresentativeVariableBindingPathSetData* firstRepData,
							CRepresentativeVariableBindingPathSetJoiningKeyMap* secJoiningKeyMap, CRepresentativeVariableBindingPathSetData* secRepData, bool firstLeft, CCalculationAlgorithmContextBase* calcAlgContext) {

					if (secJoiningKeyMap->count() < firstJoiningKeyMap->count()) {
						createCommonJoiningKeyMap(repJoinCommonKeyMap,secJoiningKeyMap,secRepData,firstJoiningKeyMap,firstRepData,!firstLeft,calcAlgContext);
					}
					if (firstJoiningKeyMap->count()*mMapComparisonDirectLookupFactor < secJoiningKeyMap->count()) {
						for (CRepresentativeVariableBindingPathSetJoiningKeyMap::const_iterator it = firstJoiningKeyMap->constBegin(), itEnd = firstJoiningKeyMap->constEnd(); it != itEnd; ++it) {
							cint64 joiningKey = it.key();
							CRepresentativeVariableBindingPathSetJoiningKeyDataMap* firstJoiningDataMap = it.value().getRepresentativeVariableBindingPathSetJoiningKeyDataMap();
							CRepresentativeVariableBindingPathSetJoiningKeyDataMap* secJoiningDataMap = secJoiningKeyMap->value(joiningKey).getRepresentativeVariableBindingPathSetJoiningKeyDataMap();
							if (secJoiningDataMap) {
								CRepresentativeVariableBindingPathSetJoiningKeyDataMap* leftJoiningDataMap = firstJoiningDataMap;
								CRepresentativeVariableBindingPathSetJoiningKeyDataMap* rightJoiningDataMap = secJoiningDataMap;
								if (!firstLeft) {
									leftJoiningDataMap = secJoiningDataMap;
									rightJoiningDataMap = firstJoiningDataMap;
								}
								repJoinCommonKeyMap->insert(joiningKey,CRepresentativeJoiningCommonKeyData(leftJoiningDataMap,rightJoiningDataMap));
							}
						}
					} else {
						CRepresentativeVariableBindingPathSetJoiningKeyMap::const_iterator it1 = firstJoiningKeyMap->constBegin(), it1End = firstJoiningKeyMap->constEnd();
						CRepresentativeVariableBindingPathSetJoiningKeyMap::const_iterator it2 = secJoiningKeyMap->constBegin(), it2End = secJoiningKeyMap->constEnd();
						while (it1 != it1End && it2 != it2End) {
							cint64 joiningKey1 = it1.key();
							cint64 joiningKey2 = it2.key();
							if (joiningKey1 == joiningKey2) {
								CRepresentativeVariableBindingPathSetJoiningKeyDataMap* firstJoiningDataMap = it1.value().getRepresentativeVariableBindingPathSetJoiningKeyDataMap();
								CRepresentativeVariableBindingPathSetJoiningKeyDataMap* secJoiningDataMap = it2.value().getRepresentativeVariableBindingPathSetJoiningKeyDataMap();
								if (firstJoiningDataMap && secJoiningDataMap) {
									CRepresentativeVariableBindingPathSetJoiningKeyDataMap* leftJoiningDataMap = firstJoiningDataMap;
									CRepresentativeVariableBindingPathSetJoiningKeyDataMap* rightJoiningDataMap = secJoiningDataMap;
									if (!firstLeft) {
										leftJoiningDataMap = secJoiningDataMap;
										rightJoiningDataMap = firstJoiningDataMap;
									}
									repJoinCommonKeyMap->insert(joiningKey1,CRepresentativeJoiningCommonKeyData(leftJoiningDataMap,rightJoiningDataMap));
								}
								++it1;
								++it2;
							} else if (joiningKey1 < joiningKey2) {
								++it1;
							} else if (joiningKey2 < joiningKey1) {
								++it2;
							}
						}
					}
				}



				CRepresentativeVariableBindingPathSetJoiningKeyMap* CCalculationTableauCompletionTaskHandleAlgorithm::getRepresentativeJoiningKeyData(CRepresentativeVariableBindingPathSetData* repVarBindPathSetData, CConcept* joinConcept, CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessContext* processContext = calcAlgContext->getUsedProcessContext();
					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CSortedLinker<CVariable*>* keyVarLinker = joinConcept->getVariableLinker();

					CRepresentativeVariableBindingPathSetJoiningData* joiningData = nullptr;

					CRepresentativeVariableBindingPathSetJoiningHash* joiningHash = repVarBindPathSetData->getJoiningHash(false);
					if (joiningHash) {
						joiningData = joiningHash->getRepresentativeVariableBindingPathSetJoiningData(joinConcept,false);
					}
					if (!joiningData) {
						joiningHash = repVarBindPathSetData->getJoiningHash(true);
						joiningData = joiningHash->getRepresentativeVariableBindingPathSetJoiningData(joinConcept,true);

						CRepresentativeVariableBindingPathJoiningKeyHash* varBindPathJoiningKeyHash = procDataBox->getRepresentativeVariableBindingPathJoiningKeyHash(true);

						CRepresentativeVariableBindingPathSetJoiningKeyMap* joiningKeyMap = joiningData->getJoiningKeyMap();
						CRepresentativeVariableBindingPathMap* varBindPathMap = repVarBindPathSetData->getMigrateData(false)->getRepresentativeVariableBindingPathMap();
						for (CRepresentativeVariableBindingPathMap::const_iterator it = varBindPathMap->constBegin(), itEnd = varBindPathMap->constEnd(); it != itEnd; ++it) {
							CVariableBindingPath* varBindPath = it.value().getVariableBindingPath();
							cint64 joinKey = varBindPathJoiningKeyHash->getRepresentativeVariableBindingPathJoiningKey(varBindPath,keyVarLinker,true);			
							CRepresentativeVariableBindingPathSetJoiningKeyDataMap* joiningKeyDataMap = joiningKeyMap->getJoiningKeyDataMap(joinKey,true);
							joiningKeyDataMap->insert(varBindPath->getPropagationID(),varBindPath);
						}
					}
					return joiningData->getJoiningKeyMap();
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEBINDVARIABLERule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {

					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CVariable* variable = concept->getVariable();
					bool conceptNegation = negate;
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();

					CConcept* bindingTriggerConcept = opConLinker->getData();
					bool bindingTriggerConceptNegation = opConLinker->isNegated();

					CProcessContext* processContext = calcAlgContext->getUsedProcessContext();
					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(false);
					CConceptDescriptor* bindingConDes = nullptr;
					CDependencyTrackPoint* bindingDepTrackPoint = nullptr;
					CCondensedReapplyQueue* reapplyQueue = nullptr;

					STATINC(VARBINDRULEBINDAPPLICATIONCOUNT,calcAlgContext);

					bool updateExt = false;


					CConceptPropagationBindingSetHash* conPropBindingSetHash = processIndi->getConceptPropagationBindingSetHash(false);
					if (conPropBindingSetHash) {
						CPropagationBindingSet* propBindingSet = conPropBindingSetHash->getPropagationBindingSet(concept,false);

						if (propBindingSet) {
							CPropagationVariableBindingTransitionExtension* propVarBindTransExt = propBindingSet->getPropagationVariableBindingTransitionExtension(false);
							if (!propVarBindTransExt || !propVarBindTransExt->isProcessingCompleted()) {
								if (!propVarBindTransExt || propBindingSet->hasPropagateAllFlag()) {
									updateExt = true;
								} else {
									CPropagationBindingDescriptor* lastAnalyPropBindDes = propVarBindTransExt->getLastAnalysedPropagationBindingDescriptor();
									CPropagationBindingDescriptor* propBindDes = propBindingSet->getPropagationBindingDescriptorLinker();
									if (lastAnalyPropBindDes != propBindDes) {
										updateExt = true;
									}
								}
							}
						}
					}



					if (updateExt) {

						CConceptPropagationBindingSetHash* conPropBindingSetHash = processIndi->getConceptPropagationBindingSetHash(true);
						CPropagationBindingSet* propBindingSet = conPropBindingSetHash->getPropagationBindingSet(concept,true);
						CPropagationVariableBindingTransitionExtension* propVarBindTransExt = propBindingSet->getPropagationVariableBindingTransitionExtension(true);

						CPropagationBindingDescriptor* lastAnalyPropBindDes = propVarBindTransExt->getLastAnalysedPropagationBindingDescriptor();
						CPropagationBindingDescriptor* propBindDes = propBindingSet->getPropagationBindingDescriptorLinker();

						propVarBindTransExt->setTriggeredVariableIndividualPair(variable,processIndi);
						bool createVarBinding = false;
						createVarBinding = propBindingSet->hasPropagateAllFlag();
						for (CPropagationBindingDescriptor* propBindDesIt = propBindDes; propBindDesIt != lastAnalyPropBindDes; propBindDesIt = propBindDesIt->getNext()) {
							if (propVarBindTransExt->addAnalysedPropagationBindingDescriptorReturnMatched(propBindDesIt)) {
								createVarBinding = true;
							}
						}
						propVarBindTransExt->setLastAnalysedPropagationBindingDescriptor(propBindDes);
						propVarBindTransExt->setLastAnalysedPropagateAllFlag(propBindingSet->hasPropagateAllFlag());

						if (createVarBinding) {
							++mStatRepresentativeCreatedCount;
							STATINC(VARBINDVARIABLEBINDCOUNT,calcAlgContext);
							propVarBindTransExt->setProcessingCompleted(true);

							CConceptRepresentativePropagationSetHash* conRepPropSetHash = processIndi->getConceptRepresentativePropagationSetHash(true);
							CRepresentativePropagationSet* repPropSet = conRepPropSetHash->getRepresentativePropagationSet(bindingTriggerConcept,true);

							CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
							cint64 nextPathPropID = processingDataBox->getNextVariableBindingPathID(true);

							CDependencyTrackPoint* nextDepTrackPoint = nullptr;
							CREPRESENTATIVEBINDVARIABLEDependencyNode* bindDepNode = createREPRESENTATIVEBINDVARIABLEDependency(nextDepTrackPoint,processIndi,conDes,processIndi->getDependencyTrackPoint(),calcAlgContext);


							if (!conSet->getConceptDescriptorAndReapplyQueue(bindingTriggerConcept,bindingConDes,bindingDepTrackPoint,reapplyQueue)) {
								bindingConDes = addConceptToIndividualReturnConceptDescriptor(bindingTriggerConcept,bindingTriggerConceptNegation,processIndi,nextDepTrackPoint,false,false,calcAlgContext);
							} else {
								reapplyConceptUpdatedRepresentative(processIndi,bindingConDes,bindingDepTrackPoint,conSet,reapplyQueue,calcAlgContext);
							}

							repPropSet->setConceptDescriptor(bindingConDes);
							CVariableBinding* varBinding = CObjectAllocator<CVariableBinding>::allocateAndConstruct(taskMemMan);
							varBinding->initVariableBinding(nextDepTrackPoint,processIndi,variable);
							CVariableBindingDescriptor* varBindingDes = CObjectAllocator<CVariableBindingDescriptor>::allocateAndConstruct(taskMemMan);
							varBindingDes->initVariableBindingDescriptor(varBinding);
							CVariableBindingPath* varBindingPath = CObjectAllocator<CVariableBindingPath>::allocateAndConstruct(taskMemMan);
							varBindingPath->initVariableBindingPath(nextPathPropID,varBindingDes);

							CRepresentativeVariableBindingPathHash* repVarBindPathHash = procDataBox->getRepresentativeVariableBindingPathHash(true);
							CRepresentativeVariableBindingPathSetData* repData = repVarBindPathHash->getRepresentativeVariableBindingPathSetData(varBindingPath,true);
							repData->setRepresentativeID(procDataBox->getNextRepresentativeVariableBindingPathID(true));
							repData->setMigratable(false);
							repData->incUseCount();
							repData->incShareCount();
							CRepresentativeVariableBindingPathSetMigrateData* repMigData = repData->getMigrateData(true);
							repMigData->getRepresentativeVariableBindingPathMap()->insert(varBindingPath->getPropagationID(),CRepresentativeVariableBindingPathMapData(varBindingPath,repData));
							repMigData->getRepresentativeContainingMap()->insertContainedRepresentative(repData,false);
							repData->addKeySignatureValue(repData->getRepresentativeID());

							CRepresentativeVariableBindingPathSetHash* repVarBindPathSetHash = procDataBox->getRepresentativeVariableBindingPathSetHash(true);
							repVarBindPathSetHash->insertRepresentativeVariableBindingPathSetData(repData);


							CRepresentativePropagationDescriptor* repPropDes = CObjectAllocator<CRepresentativePropagationDescriptor>::allocateAndConstruct(taskMemMan);
							repPropDes->initRepresentativeDescriptor(repData,nextDepTrackPoint);

							repPropSet->addIncomingRepresentativePropagationDescriptorLinker(repPropDes);
							updateRepresentativePropagationSet(processIndi,repPropSet,calcAlgContext);

						}
					}
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEIMPLICATIONRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					cint64 opCount = concept->getOperandCount();
					CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();


					CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(false);
					CConceptDescriptor* bindingConDes = nullptr;
					CDependencyTrackPoint* bindingDepTrackPoint = nullptr;
					CCondensedReapplyQueue* reapplyQueue = nullptr;

					CConcept* bindingTriggerConcept = opLinker->getData();
					bool bindingTriggerConceptNegation = opLinker->isNegated();
					CSortedNegLinker<CConcept*>* triggerLinker = opLinker->getNext();

					STATINC(VARBINDRULEIMPLICATIONAPPLICATIONCOUNT,calcAlgContext);

					if (!conSet->getConceptDescriptorAndReapplyQueue(bindingTriggerConcept,bindingConDes,bindingDepTrackPoint,reapplyQueue)) {

						// search next not existing trigger
						bool allTriggersAvailable = true;
						conSet = processIndi->getReapplyConceptLabelSet(true);
						CSortedNegLinker<CConcept*>* triggerLinkerIt = nullptr;
						for (triggerLinkerIt = triggerLinker; triggerLinkerIt; triggerLinkerIt = triggerLinkerIt->getNext()) {
							CSortedNegLinker<CConcept*>* nextTrigger = triggerLinkerIt;
							CConcept* triggerConcept = nextTrigger->getData();
							CConceptDescriptor* triggerConDes = nullptr;
							CDependencyTrackPoint* triggerDepTrackPoint = nullptr;
							if (conSet->getConceptDescriptor(triggerConcept,triggerConDes,triggerDepTrackPoint)) {
								if (triggerConDes->isNegated() == nextTrigger->isNegated()) {
									return;
								}
							} else {
								allTriggersAvailable = false;
								break;
							}
						}

						if (!allTriggersAvailable) {
							// install to trigger
							CSortedNegLinker<CConcept*>* nextTrigger = triggerLinkerIt;
							CConcept* triggerConcept = nextTrigger->getData();
							bool triggerNegation = !nextTrigger->isNegated();
							if (!isConceptInReapplyQueue(conDes,triggerConcept,triggerNegation,processIndi,calcAlgContext)) {
								addConceptToReapplyQueue(conDes,triggerConcept,triggerNegation,processIndi,(CProcessingRestrictionSpecification*)nullptr,depTrackPoint,calcAlgContext);
							}
						} else {
							CDependency* triggerDeps = nullptr;
							for (CSortedNegLinker<CConcept*>* triggerLinkerIt = triggerLinker; triggerLinkerIt; triggerLinkerIt = triggerLinkerIt->getNext()) {
								CConcept* triggerConcept = triggerLinkerIt->getData();
								CConceptDescriptor* triggerConDes = nullptr;
								CDependencyTrackPoint* triggerDepTrackPoint = nullptr;
								conSet->getConceptDescriptor(triggerConcept,triggerConDes,triggerDepTrackPoint);
								CCONNECTIONDependencyNode* connDep = createCONNECTIONDependency(processIndi,triggerConDes,triggerDepTrackPoint,calcAlgContext);
								connDep->setNext(triggerDeps);
								triggerDeps = connDep;
							}

							++mStatRepresentativeImplicationCount;

							CConceptRepresentativePropagationSetHash* conRepPropSetHash = processIndi->getConceptRepresentativePropagationSetHash(true);
							CRepresentativePropagationSet* prevRepPropSet = conRepPropSetHash->getRepresentativePropagationSet(concept,false);
							CRepresentativePropagationSet* repPropSet = conRepPropSetHash->getRepresentativePropagationSet(bindingTriggerConcept,true);
							CRepresentativePropagationDescriptor* procRepPropDes = nullptr;
							CDependencyTrackPoint* propDepTrackPoint = nullptr;
							if (prevRepPropSet) {
								procRepPropDes = prevRepPropSet->getOutgoingRepresentativePropagationDescriptorLinker();
							}
							if (procRepPropDes) {
								propDepTrackPoint = procRepPropDes->getDependencyTrackPoint();

								repPropSet->setConceptDescriptor(bindingConDes);

								CDependencyTrackPoint* nextDepTrackPoint = nullptr;
								CREPRESENTATIVEIMPLICATIONDependencyNode* implDepNode = createREPRESENTATIVEIMPLICATIONDependency(nextDepTrackPoint,processIndi,conDes,propDepTrackPoint,triggerDeps,calcAlgContext);

								bindingConDes = addConceptToIndividualReturnConceptDescriptor(bindingTriggerConcept,bindingTriggerConceptNegation,processIndi,nextDepTrackPoint,true,false,calcAlgContext);


								propagateRepresentative(processIndi,procRepPropDes,repPropSet,nextDepTrackPoint,calcAlgContext);
							}

						}
					} else {

						CConceptRepresentativePropagationSetHash* conRepPropSetHash = processIndi->getConceptRepresentativePropagationSetHash(true);
						CRepresentativePropagationSet* prevRepPropSet = conRepPropSetHash->getRepresentativePropagationSet(concept,false);
						CRepresentativePropagationSet* repPropSet = conRepPropSetHash->getRepresentativePropagationSet(bindingTriggerConcept,true);
						CRepresentativePropagationDescriptor* procRepPropDes = nullptr;
						CDependencyTrackPoint* propDepTrackPoint = nullptr;
						if (prevRepPropSet) {
							procRepPropDes = prevRepPropSet->getOutgoingRepresentativePropagationDescriptorLinker();
						}
						if (procRepPropDes) {
							propDepTrackPoint = procRepPropDes->getDependencyTrackPoint();

							if (requiresRepresentativePropagation(processIndi,procRepPropDes,repPropSet,calcAlgContext)) {
								CDependency* triggerDeps = nullptr;
								for (CSortedNegLinker<CConcept*>* triggerLinkerIt = triggerLinker; triggerLinkerIt; triggerLinkerIt = triggerLinkerIt->getNext()) {
									CConcept* triggerConcept = triggerLinkerIt->getData();
									CConceptDescriptor* triggerConDes = nullptr;
									CDependencyTrackPoint* triggerDepTrackPoint = nullptr;
									conSet->getConceptDescriptor(triggerConcept,triggerConDes,triggerDepTrackPoint);
									CCONNECTIONDependencyNode* connDep = createCONNECTIONDependency(processIndi,triggerConDes,triggerDepTrackPoint,calcAlgContext);
									connDep->setNext(triggerDeps);
									triggerDeps = connDep;
								}

								++mStatRepresentativeImplicationCount;
								CDependencyTrackPoint* nextDepTrackPoint = nullptr;
								CREPRESENTATIVEIMPLICATIONDependencyNode* implDepNode = createREPRESENTATIVEIMPLICATIONDependency(nextDepTrackPoint,processIndi,conDes,propDepTrackPoint,triggerDeps,calcAlgContext);

								propagateRepresentative(processIndi,procRepPropDes,repPropSet,nextDepTrackPoint,calcAlgContext);
								reapplyConceptUpdatedRepresentative(processIndi,bindingConDes,bindingDepTrackPoint,conSet,reapplyQueue,calcAlgContext);
							}
						}
					}
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateRepresentativeToSuccessor(CIndividualProcessNode* processIndi, CIndividualProcessNode*& succIndi, CSortedNegLinker<CConcept*>* conceptOpLinker, bool negate, CConceptDescriptor* conDes, CIndividualLinkEdge* restLink, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CDependencyTrackPoint* depTrackPoint = conDes->getDependencyTrackPoint();
					CConcept* concept = conDes->getConcept();


					succIndi = getLocalizedIndividual(succIndi,false,calcAlgContext);
					CReapplyConceptLabelSet* conSet = succIndi->getReapplyConceptLabelSet(false);

					// create dependency
					CDependencyTrackPoint* nextDepTrackPoint = nullptr;
					bool continuePropagation = false;

					for (CSortedNegLinker<CConcept*>* conceptOpLinkerIt = conceptOpLinker; conceptOpLinkerIt; conceptOpLinkerIt = conceptOpLinkerIt->getNext()) {
						CConcept* opConcept = conceptOpLinkerIt->getData();
						bool opConNeg = conceptOpLinkerIt->isNegated() ^ negate;

						CConceptDescriptor* bindingConDes = nullptr;
						CDependencyTrackPoint* bindingDepTrackPoint = nullptr;
						CCondensedReapplyQueue* reapplyQueue = nullptr;

						CConceptRepresentativePropagationSetHash* conRepPropSetHash = processIndi->getConceptRepresentativePropagationSetHash(true);
						CRepresentativePropagationSet* prevRepPropSet = conRepPropSetHash->getRepresentativePropagationSet(concept,false);
						CConceptRepresentativePropagationSetHash* succConRepPropSetHash = succIndi->getConceptRepresentativePropagationSetHash(true);
						CRepresentativePropagationSet* succRepPropSet = succConRepPropSetHash->getRepresentativePropagationSet(opConcept,true);
						CRepresentativePropagationDescriptor* procRepPropDes = nullptr;
						CDependencyTrackPoint* propDepTrackPoint = nullptr;
						if (prevRepPropSet) {
							procRepPropDes = prevRepPropSet->getOutgoingRepresentativePropagationDescriptorLinker();
						}
						if (procRepPropDes) {
							propDepTrackPoint = procRepPropDes->getDependencyTrackPoint();

							if (!conSet->getConceptDescriptorAndReapplyQueue(opConcept,bindingConDes,bindingDepTrackPoint,reapplyQueue)) {

								++mStatRepresentativePropagateSuccCount;
								if (!nextDepTrackPoint) {
									conSet = succIndi->getReapplyConceptLabelSet(true);
									CREPRESENTATIVEALLDependencyNode* repAllDepNode = createREPRESENTATIVEALLDependency(nextDepTrackPoint,processIndi,conDes,propDepTrackPoint,restLink->getDependencyTrackPoint(),calcAlgContext);
								}

								bindingConDes = addConceptToIndividualReturnConceptDescriptor(opConcept,opConNeg,succIndi,nextDepTrackPoint,false,false,calcAlgContext);
								succRepPropSet->setConceptDescriptor(bindingConDes);

								propagateRepresentative(succIndi,procRepPropDes,succRepPropSet,nextDepTrackPoint,calcAlgContext);
								continuePropagation = true;
							} else {

								if (requiresRepresentativePropagation(succIndi,procRepPropDes,succRepPropSet,calcAlgContext)) {
									++mStatRepresentativePropagateSuccCount;
									if (!nextDepTrackPoint) {
										conSet = succIndi->getReapplyConceptLabelSet(true);
										CREPRESENTATIVEALLDependencyNode* repAllDepNode = createREPRESENTATIVEALLDependency(nextDepTrackPoint,processIndi,conDes,propDepTrackPoint,restLink->getDependencyTrackPoint(),calcAlgContext);
									}

									propagateRepresentative(succIndi,procRepPropDes,succRepPropSet,nextDepTrackPoint,calcAlgContext);
									cint64 varCount = succRepPropSet->getOutgoingRepresentativePropagationDescriptorLinker()->getRepresentativeVariableBindingPathSetData()->getRepresentatedVariableCount();
									reapplyConceptUpdatedRepresentative(succIndi,bindingConDes,bindingDepTrackPoint,varCount,conSet,reapplyQueue,calcAlgContext);
									continuePropagation = true;
								}
							}
						}
					}

					if (continuePropagation) {
						addIndividualToProcessingQueue(succIndi,calcAlgContext);
					}
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEALLRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					cint64 opCount = concept->getOperandCount();
					CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					CSortedNegLinker<CConcept*>* conceptOpLinker = concept->getOperandList();

					STATINC(VARBINDRULEALLAPPLICATIONCOUNT,calcAlgContext);

					CIndividualLinkEdge* restLink = getLinkProcessingRestriction(conProDes);
					if (restLink) {
						CIndividualProcessNode* succIndi = getSuccessorIndividual(processIndi,restLink,calcAlgContext);

						propagateRepresentativeToSuccessor(processIndi,succIndi,opLinker,negate,conDes,restLink,calcAlgContext);

					} else {
						CReapplyRoleSuccessorHash* roleSuccHash = processIndi->getReapplyRoleSuccessorHash(false);
						if (roleSuccHash) {
							CRoleSuccessorLinkIterator roleSuccIt = roleSuccHash->getRoleSuccessorLinkIterator(role);
							while (roleSuccIt.hasNext()) {
								CIndividualLinkEdge* link = roleSuccIt.next(true);
								CIndividualProcessNode* succIndi = getSuccessorIndividual(processIndi,link,calcAlgContext);

								propagateRepresentativeToSuccessor(processIndi,succIndi,opLinker,negate,conDes,link,calcAlgContext);
							}
						}
					}
					if (!conProDes->isConceptReapplied()) {
						if (!isConceptInReapplyQueue(conDes,role,processIndi,calcAlgContext)) {
							addConceptToReapplyQueue(conDes,role,processIndi,true,depTrackPoint,calcAlgContext);
						}
					}
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::applyREPRESENTATIVEANDRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {

					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					bool conceptNegation = negate;
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();


					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(false);
					CDependencyTrackPoint* nextDepTrackPoint = nullptr;

					CConceptRepresentativePropagationSetHash* conRepPropHash = nullptr;
					CRepresentativePropagationSet* prevRepPropSet = nullptr;
					CRepresentativePropagationDescriptor* procRepPropDes = nullptr;
					CDependencyTrackPoint* propDepTrackPoint = nullptr;

					STATINC(VARBINDRULEANDAPPLICATIONCOUNT,calcAlgContext);

					for (CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker; opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
						CConcept* bindingTriggerConcept = opConLinkerIt->getData();
						bool bindingTriggerConceptNegation = opConLinkerIt->isNegated()^conceptNegation;

						if (!conRepPropHash) {
							conRepPropHash = processIndi->getConceptRepresentativePropagationSetHash(true);
						}
						if (!prevRepPropSet) {
							prevRepPropSet = conRepPropHash->getRepresentativePropagationSet(concept,false);
						}
						if (prevRepPropSet) {
							procRepPropDes = prevRepPropSet->getOutgoingRepresentativePropagationDescriptorLinker();
						}
						if (procRepPropDes) {
							propDepTrackPoint = procRepPropDes->getDependencyTrackPoint();
							CRepresentativePropagationSet* repPropSet = conRepPropHash->getRepresentativePropagationSet(bindingTriggerConcept,true);

							CConceptDescriptor* bindingConDes = nullptr;
							CDependencyTrackPoint* bindingDepTrackPoint = nullptr;
							CCondensedReapplyQueue* reapplyQueue = nullptr;

							if (!conSet->getConceptDescriptorAndReapplyQueue(bindingTriggerConcept,bindingConDes,bindingDepTrackPoint,reapplyQueue)) {

								++mStatRepresentativePropagateCount;
								if (!nextDepTrackPoint) {
									conSet = processIndi->getReapplyConceptLabelSet(true);
									CREPRESENTATIVEANDDependencyNode* repPropDepNode = createREPRESENTATIVEANDDependency(nextDepTrackPoint,processIndi,conDes,propDepTrackPoint,calcAlgContext);
								}
								bindingConDes = addConceptToIndividualReturnConceptDescriptor(bindingTriggerConcept,bindingTriggerConceptNegation,processIndi,nextDepTrackPoint,false,false,calcAlgContext);
								repPropSet->setConceptDescriptor(bindingConDes);

								propagateRepresentative(processIndi,procRepPropDes,repPropSet,nextDepTrackPoint,calcAlgContext);

							} else {

								if (requiresRepresentativePropagation(processIndi,procRepPropDes,repPropSet,calcAlgContext)) {
									++mStatRepresentativePropagateCount;
									if (!nextDepTrackPoint) {
										conSet = processIndi->getReapplyConceptLabelSet(true);
										CREPRESENTATIVEANDDependencyNode* repPropDepNode = createREPRESENTATIVEANDDependency(nextDepTrackPoint,processIndi,conDes,propDepTrackPoint,calcAlgContext);
									}

									propagateRepresentative(processIndi,procRepPropDes,repPropSet,nextDepTrackPoint,calcAlgContext);
									cint64 varCount = repPropSet->getOutgoingRepresentativePropagationDescriptorLinker()->getRepresentativeVariableBindingPathSetData()->getRepresentatedVariableCount();
									reapplyConceptUpdatedRepresentative(processIndi,bindingConDes,bindingDepTrackPoint,varCount,conSet,reapplyQueue,calcAlgContext);
								}
							}
						}
					}
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::reapplyConceptUpdatedRepresentative(CIndividualProcessNode*& processIndi, CConceptDescriptor* bindingConDes, CDependencyTrackPoint* bindingDepTrackPoint, CReapplyConceptLabelSet* conSet, CCondensedReapplyQueue* reapplyQueue, CCalculationAlgorithmContextBase* calcAlgContext) {
					setIndividualNodeConceptLabelSetModified(processIndi,calcAlgContext);
					CConceptProcessingQueue* conProQueue = processIndi->getConceptProcessingQueue(true);
					addConceptPreprocessedToProcessingQueue(bindingConDes,bindingDepTrackPoint,conProQueue,processIndi,true,calcAlgContext);
					if (!reapplyQueue->isEmpty()) {
						CCondensedReapplyQueueIterator reapplyQueueIt(conSet->getConceptReapplyIterator(bindingConDes));
						applyReapplyQueueConcepts(processIndi,&reapplyQueueIt,calcAlgContext);
					}
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::reapplyConceptUpdatedRepresentative(CIndividualProcessNode*& processIndi, CConceptDescriptor* bindingConDes, CDependencyTrackPoint* bindingDepTrackPoint, cint64 bindingCount, CReapplyConceptLabelSet* conSet, CCondensedReapplyQueue* reapplyQueue, CCalculationAlgorithmContextBase* calcAlgContext) {
					setIndividualNodeConceptLabelSetModified(processIndi,calcAlgContext);
					CConceptProcessingQueue* conProQueue = processIndi->getConceptProcessingQueue(true);
					addConceptPreprocessedToProcessingQueue(bindingConDes,bindingDepTrackPoint,conProQueue,processIndi,bindingCount,calcAlgContext);
					if (!reapplyQueue->isEmpty()) {
						CCondensedReapplyQueueIterator reapplyQueueIt(conSet->getConceptReapplyIterator(bindingConDes));
						applyReapplyQueueConcepts(processIndi,&reapplyQueueIt,calcAlgContext);
					}
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::updateRepresentativePropagationSet(CIndividualProcessNode*& processIndi, CRepresentativePropagationSet* repPropSet, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (repPropSet->getLastProcessedIncomingRepresentativePropagationDescriptorLinker() != repPropSet->getIncomingRepresentativePropagationDescriptorLinker()) {
						CRepresentativePropagationDescriptor* lastRepPropDes = repPropSet->getLastProcessedIncomingRepresentativePropagationDescriptorLinker();		
						CRepresentativePropagationDescriptor* lastIncRepPropDes = repPropSet->getIncomingRepresentativePropagationDescriptorLinker();
						CRepresentativePropagationDescriptor* lastOutRepPropDes = repPropSet->getOutgoingRepresentativePropagationDescriptorLinker();
						repPropSet->setLastProcessedIncomingRepresentativePropagationDescriptorLinker(lastIncRepPropDes);

						
						if (!repPropSet->getOutgoingRepresentativePropagationDescriptorLinker() && !lastIncRepPropDes->hasNext()) {
							repPropSet->setOutgoingRepresentativePropagationDescriptorLinker(lastIncRepPropDes);
							if (lastIncRepPropDes->getRepresentativeVariableBindingPathSetData()->isLocalizationTagUpToDate(calcAlgContext->getUsedProcessTagger())) {
								lastIncRepPropDes->getRepresentativeVariableBindingPathSetData()->incShareCount();
							}
						} else {
							++mStatRepresentativePropagateUseRepresentativeCount;

							CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
							CRepresentativeVariableBindingPathSetHash* repVarBindPathSetHash = procDataBox->getRepresentativeVariableBindingPathSetHash(true);

							CRepresentativeVariableBindingPathSetData* lastRepVarBindPathSetData = nullptr;
							bool migrateable = false;
							if (lastOutRepPropDes) {
								lastRepVarBindPathSetData = lastOutRepPropDes->getRepresentativeVariableBindingPathSetData();
							}
							if (lastRepVarBindPathSetData) {
								if (lastRepVarBindPathSetData->isLocalizationTagUpToDate(calcAlgContext->getUsedProcessTagger())) {
									lastRepVarBindPathSetData->decShareCount();
									if (lastRepVarBindPathSetData->isMigratable() && lastRepVarBindPathSetData->getShareCount() <= 0 && lastRepVarBindPathSetData->getUseCount() <= 20) {
										migrateable = true;
									}
								}
							}

							CRepresentativeVariableBindingPathSetData* repVarBindPathSetData = repVarBindPathSetHash->getRepresentativeVariableBindingPathSetData(repPropSet,true);
							repVarBindPathSetData->incShareCount();
							repVarBindPathSetData->incUseCount();
							if (!repVarBindPathSetData->hasMigrateData()) {
								++mStatRepresentativePropagateNewRepresentativeCount;
								repVarBindPathSetData->setRepresentativeID(procDataBox->getNextRepresentativeVariableBindingPathID(true));
								bool updateNewOnly = false;
								if (migrateable) {
									updateNewOnly = true;
									repVarBindPathSetData->takeMigrateDataFrom(lastRepVarBindPathSetData);
								} else if (lastRepVarBindPathSetData) {
									updateNewOnly = true;
									repVarBindPathSetData->copyMigrateDataFrom(lastRepVarBindPathSetData);
								} 

								CRepresentativeVariableBindingPathSetMigrateData* repMigrateData = repVarBindPathSetData->getMigrateData(true);
								CRepresentativeVariableBindingPathMap* repVarBindPathMap = repMigrateData->getRepresentativeVariableBindingPathMap();
								CRepresentativePropagationDescriptor* untilUpdateRepPropDes = nullptr;
								if (updateNewOnly) {
									untilUpdateRepPropDes = lastRepPropDes;
								}
								for (CRepresentativePropagationDescriptor* newRepPropDesIt = lastIncRepPropDes; newRepPropDesIt != untilUpdateRepPropDes; newRepPropDesIt = newRepPropDesIt->getNext()) {
									CRepresentativeVariableBindingPathSetData* newRepVarBindPathSetData = newRepPropDesIt->getRepresentativeVariableBindingPathSetData();
									CRepresentativeVariableBindingPathSetMigrateData* newRepMigrateData = newRepVarBindPathSetData->getMigrateData(true);
									repMigrateData->getRepresentativeContainingMap()->insertContainedRepresentative(newRepVarBindPathSetData,true);
									repVarBindPathSetData->addKeySignatureValue(newRepVarBindPathSetData->getRepresentativeKey());

									CRepresentativeVariableBindingPathMap* newRepVarBindPathMap = newRepMigrateData->getRepresentativeVariableBindingPathMap();

									if (newRepVarBindPathMap->count()*mMapComparisonDirectLookupFactor <= repVarBindPathMap->count()) {
										for (CRepresentativeVariableBindingPathMap::const_iterator propIt = newRepVarBindPathMap->constBegin(), propItEnd = newRepVarBindPathMap->constEnd(); propIt != propItEnd; ++propIt) {
											cint64 propVarBindPathID = propIt.key();
											if (!repVarBindPathMap->contains(propVarBindPathID)) {
												repVarBindPathMap->insert(propVarBindPathID,CRepresentativeVariableBindingPathMapData(propIt.value().getVariableBindingPath(),newRepVarBindPathSetData));
											}
										}
									} else {

										CRepresentativeVariableBindingPathMap::const_iterator availIt = repVarBindPathMap->begin(), availItEnd = repVarBindPathMap->end();
										CRepresentativeVariableBindingPathMap::const_iterator propIt = newRepVarBindPathMap->constBegin(), propItEnd = newRepVarBindPathMap->constEnd();

										while (propIt != propItEnd) {
											cint64 propID = propIt.key();
											if (availIt == availItEnd) {
												availIt = repVarBindPathMap->insert(propID,CRepresentativeVariableBindingPathMapData(propIt.value().getVariableBindingPath(),newRepVarBindPathSetData));
												++propIt;
												++availIt;
											} else {
												cint64 availID = availIt.key();
												if (availID < propID) {
													++availIt;
												} else if (availID > propID) {
													availIt = repVarBindPathMap->insert(propID,CRepresentativeVariableBindingPathMapData(propIt.value().getVariableBindingPath(),newRepVarBindPathSetData));
													++propIt;
													++availIt;
												} else {
													++propIt;
													++availIt;
												}
											}
										}
									}
								}

							} else {
								++mStatRepresentativePropagateReusedRepresentativeCount;
							}

							CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
							CRepresentativePropagationDescriptor* outPropRepDes = CObjectAllocator<CRepresentativePropagationDescriptor>::allocateAndConstruct(taskMemMan);


							CDependencyTrackPoint* nextDepTrackPoint = nullptr;
							CRepresentativeVariableBindingPathMap* repVarBindPathMap = repVarBindPathSetData->getMigrateData(false)->getRepresentativeVariableBindingPathMap();
							CRepresentativeContainingMap* repConMap = repVarBindPathSetData->getMigrateData(false)->getRepresentativeContainingMap();
							CRESOLVEREPRESENTATIVEDependencyNode* resolveRepNode = createRESOLVEREPRESENTATIVEDependency(nextDepTrackPoint,processIndi,repPropSet->getConceptDescriptor(),repVarBindPathMap,repPropSet->getRepresentativePropagationMap(),lastIncRepPropDes->getDependencyTrackPoint(),lastOutRepPropDes->getDependencyTrackPoint(),calcAlgContext);
							
							outPropRepDes->initRepresentativeDescriptor(repVarBindPathSetData,nextDepTrackPoint);
							repPropSet->addOutgoingRepresentativePropagationDescriptorLinker(outPropRepDes);

						}
					}
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateRepresentative(CIndividualProcessNode*& processIndi, CRepresentativePropagationDescriptor* repPropDes, CRepresentativePropagationSet* repPropSet, CDependencyTrackPoint* nextDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CRepresentativePropagationDescriptor* propagateRepDes = CObjectAllocator<CRepresentativePropagationDescriptor>::allocateAndConstruct(taskMemMan);

					propagateRepDes->initRepresentativeDescriptor(repPropDes->getRepresentativeVariableBindingPathSetData(),nextDepTrackPoint);
					repPropSet->addIncomingRepresentativePropagation(propagateRepDes);

					updateRepresentativePropagationSet(processIndi,repPropSet,calcAlgContext);
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::requiresRepresentativePropagation(CIndividualProcessNode*& processIndi, CRepresentativePropagationDescriptor* repPropDes, CRepresentativePropagationSet* testRepPropSet, CCalculationAlgorithmContextBase* calcAlgContext) {
					cint64 propRepID = repPropDes->getRepresentativeVariableBindingPathSetData()->getRepresentativeID();
					if (testRepPropSet->containsRepresentativePropagation(propRepID)) {
						return false;
					}

					CRepresentativePropagationDescriptor* lastRepPropDes = testRepPropSet->getOutgoingRepresentativePropagationDescriptorLinker();
					if (lastRepPropDes) {
						CRepresentativeVariableBindingPathSetData* availRepData = lastRepPropDes->getRepresentativeVariableBindingPathSetData();
						CRepresentativeVariableBindingPathSetMigrateData* availMigData = availRepData->getMigrateData(false);
						if (availMigData) {
							CRepresentativeContainingMap* repContMap = availMigData->getRepresentativeContainingMap();
							if (repContMap->contains(propRepID)) {
								return false;
							}
							CRepresentativeVariableBindingPathMap* availVarBindPathMap = availMigData->getRepresentativeVariableBindingPathMap();
							CRepresentativeVariableBindingPathSetMigrateData* propMigData = repPropDes->getRepresentativeVariableBindingPathSetData()->getMigrateData(false);
							CRepresentativeVariableBindingPathMap* propVarBindPathMap = propMigData->getRepresentativeVariableBindingPathMap();


							if (propVarBindPathMap->count()*mMapComparisonDirectLookupFactor <= availVarBindPathMap->count()) {
								for (CRepresentativeVariableBindingPathMap::const_iterator propIt = propVarBindPathMap->constBegin(), propItEnd = propVarBindPathMap->constEnd(); propIt != propItEnd; ++propIt) {
									cint64 propVarBindPathID = propIt.key();
									if (!availVarBindPathMap->contains(propVarBindPathID)) {
										return true;
									}
								}
								return false;
							} else {
								CRepresentativeVariableBindingPathMap::const_iterator availIt = availVarBindPathMap->constBegin(), availItEnd = availVarBindPathMap->constEnd();
								CRepresentativeVariableBindingPathMap::const_iterator propIt = propVarBindPathMap->constBegin(), propItEnd = propVarBindPathMap->constEnd();

								while (propIt != propItEnd) {
									cint64 propID = propIt.key();
									if (availIt == availItEnd) {
										return true;
									}
									cint64 availID = availIt.key();
									if (availID < propID) {
										++availIt;
									} else  if (propID < availID) {
										return true;
									} else {
										++availIt;
										++propIt;
									}
								}
								return false;

							}
						}

					}
					return true;
				}





































































				void CCalculationTableauCompletionTaskHandleAlgorithm::applyVARIABLEBINDINGANDRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {

					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					bool conceptNegation = negate;
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();


					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(false);

					CDependencyTrackPoint* nextDepTrackPoint = nullptr;

					STATINC(VARBINDRULEANDAPPLICATIONCOUNT,calcAlgContext);

					for (CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker; opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
						CConcept* bindingTriggerConcept = opConLinkerIt->getData();
						bool bindingTriggerConceptNegation = opConLinkerIt->isNegated()^conceptNegation;

						CConceptDescriptor* bindingConDes = nullptr;
						CDependencyTrackPoint* bindingDepTrackPoint = nullptr;
						CCondensedReapplyQueue* reapplyQueue = nullptr;

						if (!conSet->getConceptDescriptorAndReapplyQueue(bindingTriggerConcept,bindingConDes,bindingDepTrackPoint,reapplyQueue)) {
							if (!nextDepTrackPoint) {
								conSet = processIndi->getReapplyConceptLabelSet(true);
								CVARBINDPROPAGATEANDDependencyNode* bindDepNode = createVARBINDPROPAGATEANDDependency(nextDepTrackPoint,processIndi,conDes,depTrackPoint,calcAlgContext);
							}
							bindingConDes = addConceptToIndividualReturnConceptDescriptor(bindingTriggerConcept,bindingTriggerConceptNegation,processIndi,nextDepTrackPoint,false,false,calcAlgContext);

							CConceptVariableBindingPathSetHash* conVarBindingSetHash = processIndi->getConceptVariableBindingPathSetHash(true);
							CVariableBindingPathSet* prevVarBindingPathSet = conVarBindingSetHash->getVariableBindingPathSet(concept,false);
							CVariableBindingPathSet* varBindingPathSet = conVarBindingSetHash->getVariableBindingPathSet(bindingTriggerConcept,true);
							varBindingPathSet->setConceptDescriptor(bindingConDes);

							propagateInitialVariableBindings(processIndi,bindingConDes,varBindingPathSet,prevVarBindingPathSet,nullptr,calcAlgContext);



						} else {

							CConceptVariableBindingPathSetHash* conVarBindingSetHash = processIndi->getConceptVariableBindingPathSetHash(true);
							CVariableBindingPathSet* prevVarBindingPathSet = conVarBindingSetHash->getVariableBindingPathSet(concept,false);
							CVariableBindingPathSet* varBindingPathSet = conVarBindingSetHash->getVariableBindingPathSet(bindingTriggerConcept,true);



							if (propagateFreshVariableBindings(processIndi,conDes,varBindingPathSet,prevVarBindingPathSet,nullptr,calcAlgContext)) {
								setIndividualNodeConceptLabelSetModified(processIndi,calcAlgContext);
								CConceptProcessingQueue* conProQueue = processIndi->getConceptProcessingQueue(true);;
								cint64 bindingCount = varBindingPathSet->getVariableBindingPathMap()->count();
								addConceptPreprocessedToProcessingQueue(bindingConDes,bindingDepTrackPoint,conProQueue,processIndi,true,calcAlgContext);
								if (!reapplyQueue->isEmpty()) {
									CCondensedReapplyQueueIterator reapplyQueueIt(conSet->getConceptReapplyIterator(bindingConDes));
									applyReapplyQueueConcepts(processIndi,&reapplyQueueIt,calcAlgContext);
								}
							}
						}
					}
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::propagateInitialVariableBindings(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CVariableBindingPathSet* newVarBindingSet, CVariableBindingPathSet* prevVarBindingSet, CDependency* otherDependencies, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					bool propagations = false;
					CVariableBindingPathDescriptor* newVarBindPathDesLinker = nullptr;
					if (prevVarBindingSet) {
						newVarBindingSet->copyVariableBindingPaths(prevVarBindingSet->getVariableBindingPathMap());
						CVariableBindingPathMap* varBindMap = newVarBindingSet->getVariableBindingPathMap();
						for (CVariableBindingPathMap::iterator it = varBindMap->begin(), itEnd = varBindMap->end(); it != itEnd; ++it) {
							++mStatVarBindingPropagateCount;
							++mStatVarBindingPropagateInitialCount;
							STATINC(VARBINDPROPAGATEDCOUNT,calcAlgContext);
							STATINC(VARBINDPROPAGATEDINITIALCOUNT,calcAlgContext);
							CVariableBindingPathMapData& varBindPathMapData = it.value();
							CVariableBindingPathDescriptor* prevVarBindPathDes = varBindPathMapData.getVariableBindingPathDescriptor();
							CVariableBindingPathDescriptor* newVarBindPathDes = CObjectAllocator<CVariableBindingPathDescriptor>::allocateAndConstruct(taskMemMan);
							CDependencyTrackPoint* newDepTrackPoint = nullptr;
							CPROPAGATEVARIABLEBINDINGDependencyNode* bindDepNode = createPROPAGATEVARIABLEBINDINGDependency(newDepTrackPoint,processIndi,conDes,prevVarBindPathDes->getDependencyTrackPoint(),otherDependencies,calcAlgContext);
							newVarBindPathDes->initVariableBindingPathDescriptor(prevVarBindPathDes->getVariableBindingPath(),newDepTrackPoint);
							varBindPathMapData.setVariableBindingPathDescriptor(newVarBindPathDes);
							newVarBindPathDesLinker = newVarBindPathDes->append(newVarBindPathDesLinker);
							propagations = true;
						}
						if (newVarBindPathDesLinker) {
							newVarBindingSet->addVariableBindingPathDescriptorLinker(newVarBindPathDesLinker);
						}
					}
					return propagations;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::propagateFreshVariableBindings(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CVariableBindingPathSet* newVarBindingSet, CVariableBindingPathSet* prevVarBindingSet, CDependency* otherDependencies, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					bool propagations = false;
					if (prevVarBindingSet) {
						CVariableBindingPathMap* prevVarBindPathMap = prevVarBindingSet->getVariableBindingPathMap();
						CVariableBindingPathMap* newVarBindPathMap = newVarBindingSet->getVariableBindingPathMap();

						CVariableBindingPathMap::iterator itNew = newVarBindPathMap->begin();
						CVariableBindingPathMap::const_iterator itPrev = prevVarBindPathMap->constBegin(), itPrevEnd = prevVarBindPathMap->constEnd();

						CVariableBindingPathDescriptor* newVarBindPathDesLinker = nullptr;

						while (itPrev != itPrevEnd) {
							cint64 prevPropID = itPrev.key();
							bool doPropagation = false;
							if (itNew == newVarBindPathMap->end()) {
								doPropagation = true;
							} else {
								cint64 newPropID = itNew.key();
								if (newPropID < prevPropID) {
									++itNew;
								} else if (newPropID == prevPropID) {
									++itNew;
									++itPrev;
								} else {
									doPropagation = true;
								}
							}
							if (doPropagation) {
								++mStatVarBindingPropagateCount;
								++mStatVarBindingPropagateFreshCount;
								STATINC(VARBINDPROPAGATEDCOUNT,calcAlgContext);
								STATINC(VARBINDPROPAGATEDFRESHCOUNT,calcAlgContext);
								const CVariableBindingPathMapData& prevVarBindPathMapData = itPrev.value();
								CVariableBindingPathDescriptor* prevVarBindPathDes = prevVarBindPathMapData.getVariableBindingPathDescriptor();

								CVariableBindingPathDescriptor* newVarBindPathDes = CObjectAllocator<CVariableBindingPathDescriptor>::allocateAndConstruct(taskMemMan);
								CDependencyTrackPoint* newDepTrackPoint = nullptr;
								CPROPAGATEVARIABLEBINDINGDependencyNode* bindDepNode = createPROPAGATEVARIABLEBINDINGDependency(newDepTrackPoint,processIndi,conDes,prevVarBindPathDes->getDependencyTrackPoint(),otherDependencies,calcAlgContext);
								CVariableBindingPath* varBindingPath = prevVarBindPathDes->getVariableBindingPath();
								newVarBindPathDes->initVariableBindingPathDescriptor(varBindingPath,newDepTrackPoint);

								itNew = newVarBindPathMap->insert(varBindingPath->getPropagationID(),CVariableBindingPathMapData(newVarBindPathDes));

								newVarBindPathDesLinker = newVarBindPathDes->append(newVarBindPathDesLinker);
								propagations = true;
							}
						}
						if (newVarBindPathDesLinker) {
							newVarBindingSet->addVariableBindingPathDescriptorLinker(newVarBindPathDesLinker);
						}
					}
					return propagations;
				}




				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateVariableBindingsToSuccessor(CIndividualProcessNode* processIndi, CIndividualProcessNode*& succIndi, CSortedNegLinker<CConcept*>* conceptOpLinker, bool negate, CConceptDescriptor* conDes, CIndividualLinkEdge* restLink, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CDependencyTrackPoint* depTrackPoint = conDes->getDependencyTrackPoint();
					CConcept* concept = conDes->getConcept();


					succIndi = getLocalizedIndividual(succIndi,false,calcAlgContext);
					CReapplyConceptLabelSet* conSet = succIndi->getReapplyConceptLabelSet(false);

					// create dependency
					CDependencyTrackPoint* nextDepTrackPoint = nullptr;
					bool continuePropagation = false;

					for (CSortedNegLinker<CConcept*>* conceptOpLinkerIt = conceptOpLinker; conceptOpLinkerIt; conceptOpLinkerIt = conceptOpLinkerIt->getNext()) {
						CConcept* opConcept = conceptOpLinkerIt->getData();
						bool opConNeg = conceptOpLinkerIt->isNegated() ^ negate;

						CConceptDescriptor* bindingConDes = nullptr;
						CDependencyTrackPoint* bindingDepTrackPoint = nullptr;
						CCondensedReapplyQueue* reapplyQueue = nullptr;
						if (!conSet->getConceptDescriptorAndReapplyQueue(opConcept,bindingConDes,bindingDepTrackPoint,reapplyQueue)) {


							if (!nextDepTrackPoint) {
								conSet = processIndi->getReapplyConceptLabelSet(true);
								CVARBINDPROPAGATEALLDependencyNode* bindDepNode = createVARBINDPROPAGATEALLDependency(nextDepTrackPoint,processIndi,conDes,depTrackPoint,restLink->getDependencyTrackPoint(),calcAlgContext);
							}
							bindingConDes = addConceptToIndividualReturnConceptDescriptor(opConcept,opConNeg,succIndi,nextDepTrackPoint,false,false,calcAlgContext);

							CConceptVariableBindingPathSetHash* conVarBindingPathSetHash = processIndi->getConceptVariableBindingPathSetHash(true);
							CVariableBindingPathSet* prevVarBindingPathSet = conVarBindingPathSetHash->getVariableBindingPathSet(concept,false);
							CConceptVariableBindingPathSetHash* succConVarBindingPathSetHash = succIndi->getConceptVariableBindingPathSetHash(true);
							CVariableBindingPathSet* succVarBindingPathSet = succConVarBindingPathSetHash->getVariableBindingPathSet(opConcept,true);
							succVarBindingPathSet->setConceptDescriptor(bindingConDes);

							propagateInitialVariableBindingsToSuccessor(processIndi,succIndi,bindingConDes,succVarBindingPathSet,prevVarBindingPathSet,restLink,calcAlgContext);
							continuePropagation = true;
						} else {

							CConceptVariableBindingPathSetHash* conVarBindingPathSetHash = processIndi->getConceptVariableBindingPathSetHash(true);
							CVariableBindingPathSet* prevVarBindingPathSet = conVarBindingPathSetHash->getVariableBindingPathSet(concept,false);
							CConceptVariableBindingPathSetHash* succConVarBindingPathSetHash = succIndi->getConceptVariableBindingPathSetHash(true);
							CVariableBindingPathSet* succVarBindingPathSet = succConVarBindingPathSetHash->getVariableBindingPathSet(opConcept,true);

							if (propagateFreshVariableBindingsToSuccessor(processIndi,succIndi,conDes,succVarBindingPathSet,prevVarBindingPathSet,restLink,calcAlgContext)) {
								setIndividualNodeConceptLabelSetModified(succIndi,calcAlgContext);
								CConceptProcessingQueue* conProQueue = succIndi->getConceptProcessingQueue(true);;
								cint64 bindingCount = succVarBindingPathSet->getVariableBindingPathMap()->count();
								addConceptPreprocessedToProcessingQueue(bindingConDes,bindingDepTrackPoint,conProQueue,succIndi,true,calcAlgContext);
								if (!reapplyQueue->isEmpty()) {
									CCondensedReapplyQueueIterator reapplyQueueIt(conSet->getConceptReapplyIterator(bindingConDes));
									applyReapplyQueueConcepts(succIndi,&reapplyQueueIt,calcAlgContext);
								}
								continuePropagation = true;
							}
						}
					}

					if (continuePropagation) {
						addIndividualToProcessingQueue(succIndi,calcAlgContext);
					}
				}






				bool CCalculationTableauCompletionTaskHandleAlgorithm::propagateInitialVariableBindingsToSuccessor(CIndividualProcessNode*& processIndi, CIndividualProcessNode* succIndi, CConceptDescriptor* conDes, CVariableBindingPathSet* newVarBindingPathSet, CVariableBindingPathSet* prevVarBindingPathSet, CIndividualLinkEdge* restLink, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					bool propagations = false;
					CVariableBindingPathDescriptor* newVarBindPathDesLinker = nullptr;
					if (prevVarBindingPathSet) {
						newVarBindingPathSet->copyVariableBindingPaths(prevVarBindingPathSet->getVariableBindingPathMap());
						CVariableBindingPathMap* varBindPathMap = newVarBindingPathSet->getVariableBindingPathMap();
						for (CVariableBindingPathMap::iterator it = varBindPathMap->begin(), itEnd = varBindPathMap->end(); it != itEnd; ++it) {
							STATINC(VARBINDPROPAGATEDCOUNT,calcAlgContext);
							STATINC(VARBINDPROPAGATEDINITIALCOUNT,calcAlgContext);
							++mStatVarBindingPropagateSuccCount;
							++mStatVarBindingPropagateSuccInitialCount;
							CVariableBindingPathMapData& varBindPathMapData = it.value();
							CVariableBindingPathDescriptor* prevVarBindPathDes = varBindPathMapData.getVariableBindingPathDescriptor();
							CVariableBindingPathDescriptor* newVarBindPathDes = CObjectAllocator<CVariableBindingPathDescriptor>::allocateAndConstruct(taskMemMan);
							CDependencyTrackPoint* newDepTrackPoint = nullptr;
							CPROPAGATEVARIABLEBINDINGSSUCCESSORDependencyNode* bindDepNode = createPROPAGATEVARIABLEBINDINGSSUCCESSORDependency(newDepTrackPoint,processIndi,conDes,prevVarBindPathDes->getDependencyTrackPoint(),restLink->getDependencyTrackPoint(),calcAlgContext);
							newVarBindPathDes->initVariableBindingPathDescriptor(prevVarBindPathDes->getVariableBindingPath(),newDepTrackPoint);
							varBindPathMapData.setVariableBindingPathDescriptor(newVarBindPathDes);
							newVarBindPathDesLinker = newVarBindPathDes->append(newVarBindPathDesLinker);
							propagations = true;
						}
						if (newVarBindPathDesLinker) {
							newVarBindingPathSet->addVariableBindingPathDescriptorLinker(newVarBindPathDesLinker);
						}
					}
					return propagations;
				}




				bool CCalculationTableauCompletionTaskHandleAlgorithm::propagateFreshVariableBindingsToSuccessor(CIndividualProcessNode*& processIndi, CIndividualProcessNode* succIndi, CConceptDescriptor* conDes, CVariableBindingPathSet* newVarBindingPathSet, CVariableBindingPathSet* prevVarBindingPathSet, CIndividualLinkEdge* restLink, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					bool propagations = false;
					if (prevVarBindingPathSet) {
						CVariableBindingPathMap* prevVarBindPathMap = prevVarBindingPathSet->getVariableBindingPathMap();
						CVariableBindingPathMap* newVarBindPathMap = newVarBindingPathSet->getVariableBindingPathMap();

						CVariableBindingPathMap::iterator itNew = newVarBindPathMap->begin();
						CVariableBindingPathMap::const_iterator itPrev = prevVarBindPathMap->constBegin(), itPrevEnd = prevVarBindPathMap->constEnd();

						CVariableBindingPathDescriptor* newVarBindPathDesLinker = nullptr;

						while (itPrev != itPrevEnd) {
							cint64 prevPropID = itPrev.key();
							bool doPropagation = false;
							if (itNew == newVarBindPathMap->end()) {
								doPropagation = true;
							} else {
								cint64 newPropID = itNew.key();
								if (newPropID < prevPropID) {
									++itNew;
								} else if (newPropID == prevPropID) {
									++itNew;
									++itPrev;
								} else {
									doPropagation = true;
								}
							}
							if (doPropagation) {
								STATINC(VARBINDPROPAGATEDCOUNT,calcAlgContext);
								STATINC(VARBINDPROPAGATEDFRESHCOUNT,calcAlgContext);
								++mStatVarBindingPropagateSuccCount;
								++mStatVarBindingPropagateSuccFreshCount;
								const CVariableBindingPathMapData& prevVarBindPathMapData = itPrev.value();
								CVariableBindingPathDescriptor* prevVarBindPathDes = prevVarBindPathMapData.getVariableBindingPathDescriptor();

								CVariableBindingPathDescriptor* newVarBindPathDes = CObjectAllocator<CVariableBindingPathDescriptor>::allocateAndConstruct(taskMemMan);

								CDependencyTrackPoint* newDepTrackPoint = nullptr;
								CPROPAGATEVARIABLEBINDINGSSUCCESSORDependencyNode* bindDepNode = createPROPAGATEVARIABLEBINDINGSSUCCESSORDependency(newDepTrackPoint,processIndi,conDes,prevVarBindPathDes->getDependencyTrackPoint(),restLink->getDependencyTrackPoint(),calcAlgContext);
								CVariableBindingPath* varBindingPath = prevVarBindPathDes->getVariableBindingPath();
								newVarBindPathDes->initVariableBindingPathDescriptor(varBindingPath,newDepTrackPoint);

								itNew = newVarBindPathMap->insert(varBindingPath->getPropagationID(),CVariableBindingPathMapData(newVarBindPathDes));

								newVarBindPathDesLinker = newVarBindPathDes->append(newVarBindPathDesLinker);
								propagations = true;
							}
						}
						if (newVarBindPathDesLinker) {
							newVarBindingPathSet->addVariableBindingPathDescriptorLinker(newVarBindPathDesLinker);
						}
					}
					return propagations;
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::applyVARBINDPROPAGATEALLRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					cint64 opCount = concept->getOperandCount();
					CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					CSortedNegLinker<CConcept*>* conceptOpLinker = concept->getOperandList();

					STATINC(VARBINDRULEALLAPPLICATIONCOUNT,calcAlgContext);

					CIndividualLinkEdge* restLink = getLinkProcessingRestriction(conProDes);
					if (restLink) {
						CIndividualProcessNode* succIndi = getSuccessorIndividual(processIndi,restLink,calcAlgContext);

						propagateVariableBindingsToSuccessor(processIndi,succIndi,opLinker,negate,conDes,restLink,calcAlgContext);

					} else {
						CReapplyRoleSuccessorHash* roleSuccHash = processIndi->getReapplyRoleSuccessorHash(false);
						if (roleSuccHash) {
							CRoleSuccessorLinkIterator roleSuccIt = roleSuccHash->getRoleSuccessorLinkIterator(role);
							while (roleSuccIt.hasNext()) {
								CIndividualLinkEdge* link = roleSuccIt.next(true);
								CIndividualProcessNode* succIndi = getSuccessorIndividual(processIndi,link,calcAlgContext);

								propagateVariableBindingsToSuccessor(processIndi,succIndi,opLinker,negate,conDes,link,calcAlgContext);
							}
						}
					}
					if (!conProDes->isConceptReapplied()) {
						if (!isConceptInReapplyQueue(conDes,role,processIndi,calcAlgContext)) {
							addConceptToReapplyQueue(conDes,role,processIndi,true,depTrackPoint,calcAlgContext);
						}
					}
				}




				void CCalculationTableauCompletionTaskHandleAlgorithm::applyVARBINDVARIABLERule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {

					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CVariable* variable = concept->getVariable();
					bool conceptNegation = negate;
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();

					CConcept* bindingTriggerConcept = opConLinker->getData();
					bool bindingTriggerConceptNegation = opConLinker->isNegated();

					CProcessContext* processContext = calcAlgContext->getUsedProcessContext();
					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(false);
					CConceptDescriptor* bindingConDes = nullptr;
					CDependencyTrackPoint* bindingDepTrackPoint = nullptr;
					CCondensedReapplyQueue* reapplyQueue = nullptr;

					STATINC(VARBINDRULEBINDAPPLICATIONCOUNT,calcAlgContext);

					bool updateExt = false;


					CConceptPropagationBindingSetHash* conPropBindingSetHash = processIndi->getConceptPropagationBindingSetHash(false);
					if (conPropBindingSetHash) {
						CPropagationBindingSet* propBindingSet = conPropBindingSetHash->getPropagationBindingSet(concept,false);

						if (propBindingSet) {
							CPropagationVariableBindingTransitionExtension* propVarBindTransExt = propBindingSet->getPropagationVariableBindingTransitionExtension(false);
							if (!propVarBindTransExt || !propVarBindTransExt->isProcessingCompleted()) {
								if (!propVarBindTransExt || propBindingSet->hasPropagateAllFlag()) {
									updateExt = true;
								} else {
									CPropagationBindingDescriptor* lastAnalyPropBindDes = propVarBindTransExt->getLastAnalysedPropagationBindingDescriptor();
									CPropagationBindingDescriptor* propBindDes = propBindingSet->getPropagationBindingDescriptorLinker();
									if (lastAnalyPropBindDes != propBindDes) {
										updateExt = true;
									}
								}
							}
						}
					}



					if (updateExt) {

						CConceptPropagationBindingSetHash* conPropBindingSetHash = processIndi->getConceptPropagationBindingSetHash(true);
						CPropagationBindingSet* propBindingSet = conPropBindingSetHash->getPropagationBindingSet(concept,true);
						CPropagationVariableBindingTransitionExtension* propVarBindTransExt = propBindingSet->getPropagationVariableBindingTransitionExtension(true);

						CPropagationBindingDescriptor* lastAnalyPropBindDes = propVarBindTransExt->getLastAnalysedPropagationBindingDescriptor();
						CPropagationBindingDescriptor* propBindDes = propBindingSet->getPropagationBindingDescriptorLinker();

						propVarBindTransExt->setTriggeredVariableIndividualPair(variable,processIndi);
						bool createVarBinding = false;
						createVarBinding = propBindingSet->hasPropagateAllFlag();
						for (CPropagationBindingDescriptor* propBindDesIt = propBindDes; propBindDesIt != lastAnalyPropBindDes; propBindDesIt = propBindDesIt->getNext()) {
							if (propVarBindTransExt->addAnalysedPropagationBindingDescriptorReturnMatched(propBindDesIt)) {
								createVarBinding = true;
							}
						}
						propVarBindTransExt->setLastAnalysedPropagationBindingDescriptor(propBindDes);
						propVarBindTransExt->setLastAnalysedPropagateAllFlag(propBindingSet->hasPropagateAllFlag());

						if (createVarBinding) {
							++mStatVarBindingCreatedCount;
							STATINC(VARBINDVARIABLEBINDCOUNT,calcAlgContext);
							propVarBindTransExt->setProcessingCompleted(true);

							CConceptVariableBindingPathSetHash* conVarBindingPathSetHash = processIndi->getConceptVariableBindingPathSetHash(true);
							CVariableBindingPathSet* varBindPathSet = conVarBindingPathSetHash->getVariableBindingPathSet(bindingTriggerConcept,true);

							CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
							cint64 nextPathPropID = processingDataBox->getNextVariableBindingPathID(true);

							CDependencyTrackPoint* nextDepTrackPoint = nullptr;
							CVARBINDVARIABLEDependencyNode* bindDepNode = createVARBINDVARIABLEDependency(nextDepTrackPoint,processIndi,conDes,processIndi->getDependencyTrackPoint(),calcAlgContext);


							if (!conSet->getConceptDescriptorAndReapplyQueue(bindingTriggerConcept,bindingConDes,bindingDepTrackPoint,reapplyQueue)) {
								bindingConDes = addConceptToIndividualReturnConceptDescriptor(bindingTriggerConcept,bindingTriggerConceptNegation,processIndi,nextDepTrackPoint,false,false,calcAlgContext);
							} else {
								setIndividualNodeConceptLabelSetModified(processIndi,calcAlgContext);
								CConceptProcessingQueue* conProQueue = processIndi->getConceptProcessingQueue(true);
								addConceptPreprocessedToProcessingQueue(bindingConDes,bindingDepTrackPoint,conProQueue,processIndi,true,calcAlgContext);
								if (!reapplyQueue->isEmpty()) {
									CCondensedReapplyQueueIterator reapplyQueueIt(conSet->getConceptReapplyIterator(bindingConDes));
									applyReapplyQueueConcepts(processIndi,&reapplyQueueIt,calcAlgContext);
								}
							}

							varBindPathSet->setConceptDescriptor(bindingConDes);
							CVariableBinding* varBinding = CObjectAllocator<CVariableBinding>::allocateAndConstruct(taskMemMan);
							varBinding->initVariableBinding(nextDepTrackPoint,processIndi,variable);
							CVariableBindingDescriptor* varBindingDes = CObjectAllocator<CVariableBindingDescriptor>::allocateAndConstruct(taskMemMan);
							varBindingDes->initVariableBindingDescriptor(varBinding);
							CVariableBindingPath* varBindingPath = CObjectAllocator<CVariableBindingPath>::allocateAndConstruct(taskMemMan);
							varBindingPath->initVariableBindingPath(nextPathPropID,varBindingDes);
							CVariableBindingPathDescriptor* varBindingPathDes = CObjectAllocator<CVariableBindingPathDescriptor>::allocateAndConstruct(taskMemMan);
							varBindingPathDes->initVariableBindingPathDescriptor(varBindingPath,nextDepTrackPoint);
							varBindPathSet->addVariableBindingPath(varBindingPathDes);

						}
					}
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::applyVARBINDPROPAGATEJOINRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					cint64 opCount = concept->getOperandCount();
					CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();


					CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(false);
					CConceptDescriptor* joinConDes = nullptr;
					CDependencyTrackPoint* joinDepTrackPoint = nullptr;
					CCondensedReapplyQueue* reapplyQueue = nullptr;

					CConcept* joinConcept = opLinker->getData();
					bool joinConceptNegation = opLinker->isNegated();
					CSortedNegLinker<CConcept*>* triggerLinker = opLinker->getNext();

					CSortedLinker<CVariable*>* varLinker = concept->getVariableLinker();

					STATINC(VARBINDRULEJOINAPPLICATIONCOUNT,calcAlgContext);

					bool propagateJoins = false;
					bool createJoinConcept = false;
					if (!conSet->getConceptDescriptorAndReapplyQueue(joinConcept,joinConDes,joinDepTrackPoint,reapplyQueue)) {
						// search next not existing trigger
						bool allTriggersAvailable = true;
						conSet = processIndi->getReapplyConceptLabelSet(true);
						CSortedNegLinker<CConcept*>* triggerLinkerIt = nullptr;
						for (triggerLinkerIt = triggerLinker; triggerLinkerIt && allTriggersAvailable; triggerLinkerIt = triggerLinkerIt->getNext()) {
							CSortedNegLinker<CConcept*>* nextTrigger = triggerLinkerIt;
							CConcept* triggerConcept = nextTrigger->getData();
							CConceptDescriptor* triggerConDes = nullptr;
							CDependencyTrackPoint* triggerDepTrackPoint = nullptr;
							if (conSet->getConceptDescriptor(triggerConcept,triggerConDes,triggerDepTrackPoint)) {
								if (triggerConDes->isNegated() == nextTrigger->isNegated()) {
									return;
								}
							} else {
								allTriggersAvailable = false;
								break;
							}
						}

						if (!allTriggersAvailable) {
							// install to trigger
							CSortedNegLinker<CConcept*>* nextTrigger = triggerLinkerIt;
							CConcept* triggerConcept = nextTrigger->getData();
							bool triggerNegation = !nextTrigger->isNegated();
							if (!isConceptInReapplyQueue(conDes,triggerConcept,triggerNegation,processIndi,calcAlgContext)) {
								addConceptToReapplyQueue(conDes,triggerConcept,triggerNegation,processIndi,(CProcessingRestrictionSpecification*)nullptr,depTrackPoint,calcAlgContext);
							}
						} else {
							propagateJoins = true;
							createJoinConcept = true;
						}
					} else {
						propagateJoins = true;
					}

					//mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);

					bool propagationsDone = false;
					if (propagateJoins) {

						for (CSortedNegLinker<CConcept*>* triggerLinkerIt = triggerLinker; triggerLinkerIt; triggerLinkerIt = triggerLinkerIt->getNext()) {
							CSortedNegLinker<CConcept*>* nextTrigger = triggerLinkerIt;
							CConcept* triggerConcept = nextTrigger->getData();
							if (!isConceptInReapplyQueue(conDes,triggerConcept,false,processIndi,calcAlgContext)) {
								addConceptToReapplyQueue(conDes,triggerConcept,false,processIndi,(CProcessingRestrictionSpecification*)nullptr,depTrackPoint,calcAlgContext);
							}
						}

						CConceptPropagationBindingSetHash* conPropBindingSetHash = processIndi->getConceptPropagationBindingSetHash(false);
						CConceptVariableBindingPathSetHash* varBindingPathSetHash = processIndi->getConceptVariableBindingPathSetHash(false);
						CPropagationBindingSet* propBindingSet = nullptr;
						if (conPropBindingSetHash && varBindingPathSetHash) {
							propBindingSet = conPropBindingSetHash->getPropagationBindingSet(concept,false);
							if (propBindingSet) {

								CPropagationVariableBindingTransitionExtension* propVarBindTransExt = propBindingSet->getPropagationVariableBindingTransitionExtension(false);

								CSortedNegLinker<CConcept*>* leftTriggerLinker = triggerLinker;
								CSortedNegLinker<CConcept*>* rightTriggerLinker = triggerLinker->getNext();

								CConcept* leftConcept = leftTriggerLinker->getData();
								CConcept* rightConcept = rightTriggerLinker->getData();

								CVariableBindingPathSet* leftVarBindPathSet = varBindingPathSetHash->getVariableBindingPathSet(leftConcept,false);
								CVariableBindingPathSet* rightVarBindPathSet = varBindingPathSetHash->getVariableBindingPathSet(rightConcept,false);

								bool examineTransExt = false;

								if (leftVarBindPathSet && rightVarBindPathSet) {

									if (!propVarBindTransExt) {
										examineTransExt = true;
									} else {
										if (propVarBindTransExt->getLastAnalysedPropagateAllFlag() != propBindingSet->getPropagateAllFlag() ||
													propVarBindTransExt->getLastAnalysedPropagationBindingDescriptor() != propBindingSet->getPropagationBindingDescriptorLinker() || 
													propVarBindTransExt->getLeftLastVariableBindingPathJoiningDescriptor() != leftVarBindPathSet->getVariableBindingPathDescriptorLinker() || 
													propVarBindTransExt->getRightLastVariableBindingPathJoiningDescriptor() != rightVarBindPathSet->getVariableBindingPathDescriptorLinker()) {

											examineTransExt = true;
										}
									}
								}

								if (examineTransExt) {
									conPropBindingSetHash = processIndi->getConceptPropagationBindingSetHash(true);
									varBindingPathSetHash = processIndi->getConceptVariableBindingPathSetHash(true);
									propBindingSet = conPropBindingSetHash->getPropagationBindingSet(concept,true);
									propVarBindTransExt = propBindingSet->getPropagationVariableBindingTransitionExtension(true);


									CVariableBindingPathSet* joinVarBindingPathSet = nullptr;

									CVariableBindingTriggerHash* varBindTriggerHash = propVarBindTransExt->getVariableBindingTriggerHash(true);
									CVariableBindingPathJoiningHash* varBindPathJoinHash = propVarBindTransExt->getVariableBindingPathJoiningHash(true);

									CPropagationBindingDescriptor* lastAnalyPropBindDes = propVarBindTransExt->getLastAnalysedPropagationBindingDescriptor();
									CPropagationBindingDescriptor* propBindDes = propBindingSet->getPropagationBindingDescriptorLinker();

									bool propAllFlag = propBindingSet->hasPropagateAllFlag();

									if (propAllFlag) {

										for (CVariableBindingTriggerHash::iterator it = varBindTriggerHash->begin(), itEnd = varBindTriggerHash->end(); it != itEnd; ++it) {
											CVariableBindingTriggerData& data = it.value();
											CVariableBindingTriggerLinker* varBindTriggerLinker = data.getVariableBindingTriggerLinker();
											data.setTriggered(true);
											data.clearVariableBindingTriggerLinker();
											for (CVariableBindingTriggerLinker* varBindTriggerIt = varBindTriggerLinker; varBindTriggerIt; varBindTriggerIt = varBindTriggerIt->getNext()) {
												STATINC(VARBINDJOINTRIGGEREXECUTECOUNT,calcAlgContext);

												CVariableBindingPathDescriptor* varBindPathDes = varBindTriggerIt->getVariableBindingPathDescriptor();
												CVariableBindingDescriptor* varBindTriggerDes = varBindTriggerIt->getNextTriggerVariableBindingDescriptor();
												bool leftTriggered = varBindTriggerIt->isLeftTriggered();

												propagationsDone |= propagateVariableBindingsJoins(processIndi,conDes,joinConcept,varBindPathDes,leftTriggered,varBindPathJoinHash,varBindingPathSetHash,joinConDes,joinVarBindingPathSet,calcAlgContext);
											}
										}

									} else {

										for (CPropagationBindingDescriptor* propBindDesIt = propBindDes; propBindDesIt != lastAnalyPropBindDes; propBindDesIt = propBindDesIt->getNext()) {
											CVariableBindingTriggerLinker* varBindTriggerLinker = nullptr;
											if (propVarBindTransExt->addAnalysedPropagationBindingDescriptorReturnMatched(propBindDesIt,&varBindTriggerLinker)) {
												for (CVariableBindingTriggerLinker* varBindTriggerIt = varBindTriggerLinker; varBindTriggerIt; varBindTriggerIt = varBindTriggerIt->getNext()) {
													STATINC(VARBINDJOINTRIGGEREXECUTECOUNT,calcAlgContext);

													CVariableBindingPathDescriptor* varBindPathDes = varBindTriggerIt->getVariableBindingPathDescriptor();
													CVariableBindingDescriptor* varBindTriggerDes = varBindTriggerIt->getNextTriggerVariableBindingDescriptor();
													bool leftTriggered = varBindTriggerIt->isLeftTriggered();

													if (!triggerVariableBindingPathJoining(processIndi,varBindPathDes,varBindTriggerDes,leftTriggered,varBindTriggerHash,calcAlgContext)) {
														propagationsDone |= propagateVariableBindingsJoins(processIndi,conDes,joinConcept,varBindPathDes,leftTriggered,varBindPathJoinHash,varBindingPathSetHash,joinConDes,joinVarBindingPathSet,calcAlgContext);
													}
												}
											}
										}

									}
									propVarBindTransExt->setLastAnalysedPropagationBindingDescriptor(propBindDes);
									propVarBindTransExt->setLastAnalysedPropagateAllFlag(propBindingSet->hasPropagateAllFlag());



									if (propVarBindTransExt->getLeftLastVariableBindingPathJoiningDescriptor() != leftVarBindPathSet->getVariableBindingPathDescriptorLinker()) {
										for (CVariableBindingPathDescriptor* varBindPathDes = leftVarBindPathSet->getVariableBindingPathDescriptorLinker(); varBindPathDes != propVarBindTransExt->getLeftLastVariableBindingPathJoiningDescriptor(); varBindPathDes = varBindPathDes->getNext()) {
											CVariableBindingPath* varBindPath = varBindPathDes->getVariableBindingPath();

											CVariableBindingDescriptor* varBindTriggerDes = varBindPath->getVariableBindingDescriptorLinker();
											bool leftTriggered = true;

											if (propAllFlag || !triggerVariableBindingPathJoining(processIndi,varBindPathDes,varBindTriggerDes,leftTriggered,varBindTriggerHash,calcAlgContext)) {
												propagationsDone |= propagateVariableBindingsJoins(processIndi,conDes,joinConcept,varBindPathDes,leftTriggered,varBindPathJoinHash,varBindingPathSetHash,joinConDes,joinVarBindingPathSet,calcAlgContext);
											}
										}
										propVarBindTransExt->setLeftLastVariableBindingPathJoiningDescriptor(leftVarBindPathSet->getVariableBindingPathDescriptorLinker());
									}



									if (propVarBindTransExt->getRightLastVariableBindingPathJoiningDescriptor() != rightVarBindPathSet->getVariableBindingPathDescriptorLinker()) {
										for (CVariableBindingPathDescriptor* varBindPathDes = rightVarBindPathSet->getVariableBindingPathDescriptorLinker(); varBindPathDes != propVarBindTransExt->getRightLastVariableBindingPathJoiningDescriptor(); varBindPathDes = varBindPathDes->getNext()) {
											CVariableBindingPath* varBindPath = varBindPathDes->getVariableBindingPath();

											CVariableBindingDescriptor* varBindTriggerDes = varBindPath->getVariableBindingDescriptorLinker();
											bool leftTriggered = false;

											if (propAllFlag || !triggerVariableBindingPathJoining(processIndi,varBindPathDes,varBindTriggerDes,leftTriggered,varBindTriggerHash,calcAlgContext)) {
												propagationsDone |= propagateVariableBindingsJoins(processIndi,conDes,joinConcept,varBindPathDes,leftTriggered,varBindPathJoinHash,varBindingPathSetHash,joinConDes,joinVarBindingPathSet,calcAlgContext);
											}
										}
										propVarBindTransExt->setRightLastVariableBindingPathJoiningDescriptor(rightVarBindPathSet->getVariableBindingPathDescriptorLinker());
									}


								}
							}
						}


					}

					if (propagationsDone) {
						if (!createJoinConcept) {
							setIndividualNodeConceptLabelSetModified(processIndi,calcAlgContext);
							CConceptProcessingQueue* conProQueue = processIndi->getConceptProcessingQueue(true);
							addConceptPreprocessedToProcessingQueue(joinConDes,joinDepTrackPoint,conProQueue,processIndi,true,calcAlgContext);
							if (!reapplyQueue->isEmpty()) {
								CCondensedReapplyQueueIterator reapplyQueueIt(conSet->getConceptReapplyIterator(joinConDes));
								applyReapplyQueueConcepts(processIndi,&reapplyQueueIt,calcAlgContext);
							}
						}
					}
				}





				bool CCalculationTableauCompletionTaskHandleAlgorithm::propagateVariableBindingsJoins(CIndividualProcessNode* processIndi, CConceptDescriptor* joiningConDes, CConcept* joinConcept, CVariableBindingPathDescriptor* varBindPathDes, bool leftTriggerPath,  
								CVariableBindingPathJoiningHash* varBindPathJoinHash, CConceptVariableBindingPathSetHash* varBindingPathSetHash, CConceptDescriptor*& joinConDes, CVariableBindingPathSet*& varBindingPathSet, CCalculationAlgorithmContextBase* calcAlgContext) {

					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CConcept* joiningConcept = joiningConDes->getConcept();
					CSortedLinker<CVariable*>* varLinker = joiningConcept->getVariableLinker();

					CVariableBindingPath* varBindPath = varBindPathDes->getVariableBindingPath();
					CVariableBindingPathJoiningData* varBindPathJoinData = varBindPathJoinHash->getVariableBindingPathJoiningData(CVariableBindingPathJoiningHasher(varBindPath,varLinker),true);
					CVariableBindingDescriptor* varBindDes = varBindPath->getVariableBindingDescriptorLinker();

					if (!varBindPathJoinData) {
						varBindPathJoinData = CObjectAllocator< CVariableBindingPathJoiningData >::allocateAndConstruct(taskMemMan);
						CVariableBindingDescriptor* keyVarBindDesLinker = createVariableBindingPathKey(processIndi,varLinker,varBindDes,calcAlgContext);
						varBindPathJoinData->initVariableBindingPathJoiningData(keyVarBindDesLinker,nullptr,nullptr);
						varBindPathJoinHash->insertVariableBindingPathJoiningData(CVariableBindingPathJoiningHasher(varBindPathJoinData),varBindPathJoinData);
					}


					CVariableBindingPathDescriptor* otherVarBindPathDes = nullptr;
					if (leftTriggerPath) {
						otherVarBindPathDes = varBindPathJoinData->getRightVariableBindingPathDescriptorLinker();
					} else {
						otherVarBindPathDes = varBindPathJoinData->getLeftVariableBindingPathDescriptorLinker();
					}


					bool addedVarBindPath = false;
					for (CVariableBindingPathDescriptor* otherVarBindPathDesIt = otherVarBindPathDes; otherVarBindPathDesIt; otherVarBindPathDesIt = otherVarBindPathDesIt->getNext()) {
						STATINC(VARBINDJOINCOMBINECOUNT,calcAlgContext);
						++mStatVarBindingJoinCombinesCount;

						CVariableBindingPath* mergedVarBindPath = getJoinedVariableBindingPath(varBindPathDes->getVariableBindingPath(),otherVarBindPathDesIt->getVariableBindingPath(),calcAlgContext);
						CVariableBindingPathDescriptor* mergedVarBindPathDes = CObjectAllocator< CVariableBindingPathDescriptor >::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());

						CDependencyTrackPoint* mergedDependencyTrackPoint = nullptr;
						CVARBINDPROPAGATEJOINDependencyNode* bindJoinDepNode = createVARBINDPROPAGATEJOINDependency(mergedDependencyTrackPoint,processIndi,joiningConDes,varBindPathDes->getDependencyTrackPoint(),otherVarBindPathDesIt->getDependencyTrackPoint(),calcAlgContext);

						forceVariableBindingJoinCreated(processIndi,joiningConDes,joinConcept,joinConDes,mergedDependencyTrackPoint,varBindingPathSet,varBindingPathSetHash,calcAlgContext);

						mergedVarBindPathDes->initVariableBindingPathDescriptor(mergedVarBindPath,mergedDependencyTrackPoint);
						varBindingPathSet->addVariableBindingPath(mergedVarBindPathDes);

						addedVarBindPath = true;
					}

					CVariableBindingPathDescriptor* newVarBindPathDes = CObjectAllocator< CVariableBindingPathDescriptor >::allocateAndConstruct(taskMemMan);
					newVarBindPathDes->initVariableBindingPathDescriptor(varBindPath,varBindPathDes->getDependencyTrackPoint());


					if (leftTriggerPath) {
						varBindPathJoinData->addLeftVariableBindingPathDescriptorLinker(newVarBindPathDes);
					} else {
						varBindPathJoinData->addRightVariableBindingPathDescriptorLinker(newVarBindPathDes);
					}

					return addedVarBindPath;
				}





				CVariableBindingDescriptor* CCalculationTableauCompletionTaskHandleAlgorithm::createVariableBindingPathKey(CIndividualProcessNode* processIndi, CSortedLinker<CVariable*>* varLinker, CVariableBindingDescriptor* varBindDes, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					CVariableBindingDescriptor* keyVarBindDesLinker = nullptr;
					CVariableBindingDescriptor* lastKeyVarBindDesLinker = nullptr;
					CSortedLinker<CVariable*>* varLinkerIt = varLinker;
					CVariableBindingDescriptor* varBindDesIt = varBindDes;
					while (varLinkerIt) {
						CVariableBinding* varBind = varBindDesIt->getVariableBinding();
						if (varBind->getBindedVariable() == varLinkerIt->getData()) {
							CVariableBindingDescriptor* nextKeyVarBindDesLinker = CObjectAllocator< CVariableBindingDescriptor >::allocateAndConstruct(taskMemMan);
							nextKeyVarBindDesLinker->initVariableBindingDescriptor(varBind);

							if (lastKeyVarBindDesLinker) {
								lastKeyVarBindDesLinker->setNext(nextKeyVarBindDesLinker);
								lastKeyVarBindDesLinker = nextKeyVarBindDesLinker;
							} else {
								keyVarBindDesLinker = lastKeyVarBindDesLinker = nextKeyVarBindDesLinker;
							}
							varLinkerIt = varLinkerIt->getNext();
							varBindDesIt = varBindDesIt->getNext();
						} else {
							varBindDesIt = varBindDesIt->getNext();
						}
					}
					return keyVarBindDesLinker;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::triggerVariableBindingPathJoining(CIndividualProcessNode* processIndi, CVariableBindingPathDescriptor* varBindPathDes, CVariableBindingDescriptor* varBindDes, bool leftTriggered, CVariableBindingTriggerHash* varBindTriggerHash, CCalculationAlgorithmContextBase* calcAlgContext) {

					CVariableBindingDescriptor* nextVarBindDesTrigger = varBindDes;
					if (nextVarBindDesTrigger) {
						while (nextVarBindDesTrigger) {
							CVariableBinding* varBind = nextVarBindDesTrigger->getVariableBinding();
							nextVarBindDesTrigger = nextVarBindDesTrigger->getNext();
							if (!varBindTriggerHash->tryInsertVariableBindingTrigger(varBind->getBindedVariable(),varBind->getBindedIndividual(),varBindPathDes,nextVarBindDesTrigger,leftTriggered)) {
							} else {
								STATINC(VARBINDJOINTRIGGERINSTALLCOUNT,calcAlgContext);
								return true;
							}
						}
					}
					return false;
				}




				void CCalculationTableauCompletionTaskHandleAlgorithm::forceVariableBindingJoinCreated(CIndividualProcessNode* processIndi, CConceptDescriptor* joiningConDes, CConcept* joinConcept, CConceptDescriptor*& joinConDes, CDependencyTrackPoint* mergedDependencyTrackPoint, CVariableBindingPathSet*& varBindingPathSet, CConceptVariableBindingPathSetHash* varBindingPathSetHash, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (!joinConDes) {
						joinConDes = addConceptToIndividualReturnConceptDescriptor(joinConcept,false,processIndi,mergedDependencyTrackPoint,false,false,calcAlgContext);
					}
					if (!varBindingPathSet) {
						varBindingPathSet = varBindingPathSetHash->getVariableBindingPathSet(joinConcept,true);
						varBindingPathSet->setConceptDescriptor(joinConDes);
					}
				}



				CVariableBindingPath* CCalculationTableauCompletionTaskHandleAlgorithm::getJoinedVariableBindingPath(CVariableBindingPath* leftVarBindPath, CVariableBindingPath* rightVarBindPath, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					CVariableBindingPathMergingHash* varBindPathMergingHash = processingDataBox->getVariableBindingPathMergingHash(true);

					CVariableBindingPathMergingHashData* mergeHashData = varBindPathMergingHash->getMergedVariableBindingPathData(leftVarBindPath,rightVarBindPath);
					CVariableBindingPath* mergedVarBindPath = mergeHashData->getVariableBindingPath();
					if (!mergedVarBindPath) {
						STATINC(VARBINDJOINCREATENEWCOUNT,calcAlgContext);

						CVariableBindingDescriptor* leftVarBindDesIt = leftVarBindPath->getVariableBindingDescriptorLinker();
						CVariableBindingDescriptor* rightVarBindDesIt = rightVarBindPath->getVariableBindingDescriptorLinker();

						CVariableBindingDescriptor* mergedVarBindDes = nullptr;
						CVariableBindingDescriptor* lastMergedVarBindDes = nullptr;

						while (leftVarBindDesIt || rightVarBindDesIt) {
							CVariableBindingDescriptor* nextMergedVarBindDes = nullptr;
							if (leftVarBindDesIt && rightVarBindDesIt) {
								bool leftLE = *leftVarBindDesIt->getVariableBinding() <= *rightVarBindDesIt->getVariableBinding();
								bool rightLE = *rightVarBindDesIt->getVariableBinding() <= *leftVarBindDesIt->getVariableBinding();
								if (leftLE && rightLE) {
									nextMergedVarBindDes = CObjectAllocator< CVariableBindingDescriptor >::allocateAndConstruct(taskMemMan);
									nextMergedVarBindDes->initVariableBindingDescriptor(leftVarBindDesIt->getVariableBinding());
									leftVarBindDesIt = leftVarBindDesIt->getNext();
									rightVarBindDesIt = rightVarBindDesIt->getNext();
								} else if (rightLE) {
									nextMergedVarBindDes = CObjectAllocator< CVariableBindingDescriptor >::allocateAndConstruct(taskMemMan);
									nextMergedVarBindDes->initVariableBindingDescriptor(rightVarBindDesIt->getVariableBinding());
									rightVarBindDesIt = rightVarBindDesIt->getNext();
								} else if (leftLE) {
									nextMergedVarBindDes = CObjectAllocator< CVariableBindingDescriptor >::allocateAndConstruct(taskMemMan);
									nextMergedVarBindDes->initVariableBindingDescriptor(leftVarBindDesIt->getVariableBinding());
									leftVarBindDesIt = leftVarBindDesIt->getNext();
								} 
							} else if (leftVarBindDesIt) {
								nextMergedVarBindDes = CObjectAllocator< CVariableBindingDescriptor >::allocateAndConstruct(taskMemMan);
								nextMergedVarBindDes->initVariableBindingDescriptor(leftVarBindDesIt->getVariableBinding());
								leftVarBindDesIt = leftVarBindDesIt->getNext();
							} else if (rightVarBindDesIt) {
								nextMergedVarBindDes = CObjectAllocator< CVariableBindingDescriptor >::allocateAndConstruct(taskMemMan);
								nextMergedVarBindDes->initVariableBindingDescriptor(rightVarBindDesIt->getVariableBinding());
								rightVarBindDesIt = rightVarBindDesIt->getNext();
							}

							if (nextMergedVarBindDes) {
								if (lastMergedVarBindDes) {
									lastMergedVarBindDes->setNext(nextMergedVarBindDes);
									lastMergedVarBindDes = nextMergedVarBindDes;
								} else {
									mergedVarBindDes = lastMergedVarBindDes = nextMergedVarBindDes;
								}
							}
						}

						mergedVarBindPath = CObjectAllocator< CVariableBindingPath >::allocateAndConstruct(taskMemMan);
						mergedVarBindPath->initVariableBindingPath(processingDataBox->getNextVariableBindingPathID(true),mergedVarBindDes);
						mergeHashData->setVariableBindingPath(mergedVarBindPath);
					}
					return mergedVarBindPath;
				}




				void CCalculationTableauCompletionTaskHandleAlgorithm::applyVARBINDPROPAGATEGROUNDINGRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					CConcept* concept = conDes->getConcept();
					bool negated = conDes->isNegated();
					cint64 opCount = concept->getOperandCount();

					CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(false);

					CConceptVariableBindingPathSetHash* conVarBindPathSetHash = processIndi->getConceptVariableBindingPathSetHash(false);
					CVariableBindingPathSet* varBindPathSet = conVarBindPathSetHash->getVariableBindingPathSet(concept,false);

					STATINC(VARBINDRULEGROUNDINGAPPLICATIONCOUNT,calcAlgContext);

					if (varBindPathSet) {

						KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(mBeforeGroundingDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext));

						CPROCESSINGHASH<CConcept*,CVariableBindingPathDescriptor*>* groundedConVarBindPathDesHash = nullptr;
						CSortedNegLinker<CConcept*>* newGroundedLinker = mGroundingHandler->getGroundingConceptLinker(processIndi,varBindPathSet,concept,negated,groundedConVarBindPathDesHash,calcAlgContext);

						if (newGroundedLinker) {
							for (CSortedNegLinker<CConcept*>* newGroundedLinkerIt = newGroundedLinker; newGroundedLinkerIt; newGroundedLinkerIt = newGroundedLinkerIt->getNext()) {
								STATINC(VARBINDGROUNDINGCOUNT,calcAlgContext);
								++mStatVarBindingGroundingCount;
								CConcept* newGroundedCocnept = newGroundedLinkerIt->getData();
								bool newGroundedConceptNegation = newGroundedLinkerIt->isNegated();

								CDependencyTrackPoint* baseDependencyTrackPoint = nullptr;
								CDependency* additionalsDependencies = nullptr;
								// collect dependencies

								CPROCESSINGHASH<CConcept*,CVariableBindingPathDescriptor*>::const_iterator it = groundedConVarBindPathDesHash->constFind(newGroundedCocnept), itEnd = groundedConVarBindPathDesHash->constEnd();
								while (it != itEnd && it.key() == newGroundedCocnept) {
									CVariableBindingPathDescriptor* varBindPathDes = it.value();
									CDependencyTrackPoint* propVarDepTrackPoint = varBindPathDes->getDependencyTrackPoint();
									if (propVarDepTrackPoint) {
										if (!baseDependencyTrackPoint) {
											baseDependencyTrackPoint = propVarDepTrackPoint;
										}
									}
									++it;
								}

								if (!baseDependencyTrackPoint) {
									baseDependencyTrackPoint = depTrackPoint;
								}

								CDependencyTrackPoint* nextDepTrackPoint = nullptr;
								CVARBINDPROPAGATEGROUNDINGDependencyNode* groundingDepNode = createVARBINDPROPAGATEGROUNDINGDependency(nextDepTrackPoint,processIndi,conDes,baseDependencyTrackPoint,additionalsDependencies,calcAlgContext);

								addConceptToIndividual(newGroundedCocnept,newGroundedConceptNegation,processIndi,nextDepTrackPoint,true,false,calcAlgContext);

							}
						}

						KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(mAfterGroundingDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext));
					}
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::applyVARBINDPROPAGATEIMPLICATIONRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					cint64 opCount = concept->getOperandCount();
					CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();


					CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(false);
					CConceptDescriptor* bindingConDes = nullptr;
					CDependencyTrackPoint* bindingDepTrackPoint = nullptr;
					CCondensedReapplyQueue* reapplyQueue = nullptr;

					CConcept* bindingTriggerConcept = opLinker->getData();
					bool bindingTriggerConceptNegation = opLinker->isNegated();
					CSortedNegLinker<CConcept*>* triggerLinker = opLinker->getNext();

					STATINC(VARBINDRULEIMPLICATIONAPPLICATIONCOUNT,calcAlgContext);

					if (!conSet->getConceptDescriptorAndReapplyQueue(bindingTriggerConcept,bindingConDes,bindingDepTrackPoint,reapplyQueue)) {

						// search next not existing trigger
						bool allTriggersAvailable = true;
						conSet = processIndi->getReapplyConceptLabelSet(true);
						CSortedNegLinker<CConcept*>* triggerLinkerIt = nullptr;
						for (triggerLinkerIt = triggerLinker; triggerLinkerIt; triggerLinkerIt = triggerLinkerIt->getNext()) {
							CSortedNegLinker<CConcept*>* nextTrigger = triggerLinkerIt;
							CConcept* triggerConcept = nextTrigger->getData();
							CConceptDescriptor* triggerConDes = nullptr;
							CDependencyTrackPoint* triggerDepTrackPoint = nullptr;
							if (conSet->getConceptDescriptor(triggerConcept,triggerConDes,triggerDepTrackPoint)) {
								if (triggerConDes->isNegated() == nextTrigger->isNegated()) {
									return;
								}
							} else {
								allTriggersAvailable = false;
								break;
							}
						}

						if (!allTriggersAvailable) {
							// install to trigger
							CSortedNegLinker<CConcept*>* nextTrigger = triggerLinkerIt;
							CConcept* triggerConcept = nextTrigger->getData();
							bool triggerNegation = !nextTrigger->isNegated();
							if (!isConceptInReapplyQueue(conDes,triggerConcept,triggerNegation,processIndi,calcAlgContext)) {
								addConceptToReapplyQueue(conDes,triggerConcept,triggerNegation,processIndi,(CProcessingRestrictionSpecification*)nullptr,depTrackPoint,calcAlgContext);
							}
						} else {
							CDependency* triggerDeps = nullptr;
							for (CSortedNegLinker<CConcept*>* triggerLinkerIt = triggerLinker; triggerLinkerIt; triggerLinkerIt = triggerLinkerIt->getNext()) {
								CConcept* triggerConcept = triggerLinkerIt->getData();
								CConceptDescriptor* triggerConDes = nullptr;
								CDependencyTrackPoint* triggerDepTrackPoint = nullptr;
								conSet->getConceptDescriptor(triggerConcept,triggerConDes,triggerDepTrackPoint);
								CCONNECTIONDependencyNode* connDep = createCONNECTIONDependency(processIndi,triggerConDes,triggerDepTrackPoint,calcAlgContext);
								connDep->setNext(triggerDeps);
								triggerDeps = connDep;
							}

							++mStatVarBindingImplicationCount;
							CDependencyTrackPoint* nextDepTrackPoint = nullptr;
							CVARBINDPROPAGATEIMPLICATIONDependencyNode* implDepNode = createVARBINDPROPAGATEIMPLICATIONDependency(nextDepTrackPoint,processIndi,conDes,depTrackPoint,triggerDeps,calcAlgContext);

							bindingConDes = addConceptToIndividualReturnConceptDescriptor(bindingTriggerConcept,bindingTriggerConceptNegation,processIndi,nextDepTrackPoint,true,false,calcAlgContext);

							CConceptVariableBindingPathSetHash* conVarBindingSetHash = processIndi->getConceptVariableBindingPathSetHash(true);
							CVariableBindingPathSet* prevVarBindingPathSet = conVarBindingSetHash->getVariableBindingPathSet(concept,false);
							CVariableBindingPathSet* varBindingPathSet = conVarBindingSetHash->getVariableBindingPathSet(bindingTriggerConcept,true);
							varBindingPathSet->setConceptDescriptor(bindingConDes);

							propagateInitialVariableBindings(processIndi,bindingConDes,varBindingPathSet,prevVarBindingPathSet,triggerDeps,calcAlgContext);


						}
					} else {
						CDependency* triggerDeps = nullptr;
						for (CSortedNegLinker<CConcept*>* triggerLinkerIt = triggerLinker; triggerLinkerIt; triggerLinkerIt = triggerLinkerIt->getNext()) {
							CConcept* triggerConcept = triggerLinkerIt->getData();
							CConceptDescriptor* triggerConDes = nullptr;
							CDependencyTrackPoint* triggerDepTrackPoint = nullptr;
							conSet->getConceptDescriptor(triggerConcept,triggerConDes,triggerDepTrackPoint);
							CCONNECTIONDependencyNode* connDep = createCONNECTIONDependency(processIndi,triggerConDes,triggerDepTrackPoint,calcAlgContext);
							connDep->setNext(triggerDeps);
							triggerDeps = connDep;
						}
						++mStatVarBindingImplicationCount;

						CConceptVariableBindingPathSetHash* conVarBindingSetHash = processIndi->getConceptVariableBindingPathSetHash(true);
						CVariableBindingPathSet* prevVarBindingPathSet = conVarBindingSetHash->getVariableBindingPathSet(concept,false);
						CVariableBindingPathSet* varBindingPathSet = conVarBindingSetHash->getVariableBindingPathSet(bindingTriggerConcept,true);

						if (propagateFreshVariableBindings(processIndi,conDes,varBindingPathSet,prevVarBindingPathSet,triggerDeps,calcAlgContext)) {
							setIndividualNodeConceptLabelSetModified(processIndi,calcAlgContext);
							CConceptProcessingQueue* conProQueue = processIndi->getConceptProcessingQueue(true);
							cint64 bindingCount = varBindingPathSet->getVariableBindingPathMap()->count();
							addConceptPreprocessedToProcessingQueue(bindingConDes,bindingDepTrackPoint,conProQueue,processIndi,bindingCount,calcAlgContext);
							if (!reapplyQueue->isEmpty()) {
								CCondensedReapplyQueueIterator reapplyQueueIt(conSet->getConceptReapplyIterator(bindingConDes));
								applyReapplyQueueConcepts(processIndi,&reapplyQueueIt,calcAlgContext);
							}
						}
					}
				}







































































































				void CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDPROPAGATEGROUNDINGRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					CConcept* concept = conDes->getConcept();
					bool negated = conDes->isNegated();
					cint64 opCount = concept->getOperandCount();
					CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();

					CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(false);

					CConceptPropagationBindingSetHash* conPropBindSetHash = processIndi->getConceptPropagationBindingSetHash(false);
					CPropagationBindingSet* propBindSet = conPropBindSetHash->getPropagationBindingSet(concept,false);

					if (propBindSet) {
						STATINC(PBINDRULEGROUNDINGAPPLICATIONCOUNT,calcAlgContext);

						if (!opConLinker) {

							KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(mBeforeGroundingDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext));

							CPROCESSINGHASH<CConcept*,CPropagationBindingDescriptor*>* groundedConPropBindDesHash = nullptr;
							CPROCESSINGHASH<CVariable*,CPropagationBindingDescriptor*>* additionalPropBindDesHash = nullptr;
							CSortedNegLinker<CConcept*>* newGroundedLinker = mGroundingHandler->getGroundingConceptLinker(processIndi,propBindSet,concept,negated,groundedConPropBindDesHash,additionalPropBindDesHash,calcAlgContext);

							if (newGroundedLinker) {
								for (CSortedNegLinker<CConcept*>* newGroundedLinkerIt = newGroundedLinker; newGroundedLinkerIt; newGroundedLinkerIt = newGroundedLinkerIt->getNext()) {
									CConcept* newGroundedCocnept = newGroundedLinkerIt->getData();
									bool newGroundedConceptNegation = newGroundedLinkerIt->isNegated();

									STATINC(PBINDGROUNDINGCOUNT,calcAlgContext);

									CDependencyTrackPoint* baseDependencyTrackPoint = nullptr;
									CDependency* additionalsDependencies = nullptr;
									// collect dependencies


									CPROCESSINGHASH<CConcept*,CPropagationBindingDescriptor*>::const_iterator it = groundedConPropBindDesHash->constFind(newGroundedCocnept), itEnd = groundedConPropBindDesHash->constEnd();
									while (it != itEnd && it.key() == newGroundedCocnept) {
										CPropagationBindingDescriptor* propBindDes = it.value();
										CDependencyTrackPoint* propBindDepTrackPoint = propBindDes->getDependencyTrackPoint();
										if (propBindDepTrackPoint) {
											if (!baseDependencyTrackPoint) {
												baseDependencyTrackPoint = propBindDepTrackPoint;
											} else {
												CPROPAGATECONNECTIONDependencyNode* connDep = createPROPAGATECONNECTIONDependency(nullptr,conDes,propBindDepTrackPoint,calcAlgContext);
												additionalsDependencies = connDep->append(additionalsDependencies);
											}
										}
										++it;
									}

									for (CPROCESSINGHASH<CVariable*,CPropagationBindingDescriptor*>::const_iterator it = additionalPropBindDesHash->constBegin(), itEnd = additionalPropBindDesHash->constEnd(); it != itEnd; ++it) {
										CPropagationBindingDescriptor* propBindDes = *it;
										CDependencyTrackPoint* propBindDepTrackPoint = propBindDes->getDependencyTrackPoint();
										if (propBindDepTrackPoint) {
											if (!baseDependencyTrackPoint) {
												baseDependencyTrackPoint = propBindDepTrackPoint;
											} else {
												CPROPAGATECONNECTIONDependencyNode* connDep = createPROPAGATECONNECTIONDependency(nullptr,conDes,propBindDepTrackPoint,calcAlgContext);
												additionalsDependencies = connDep->append(additionalsDependencies);
											}
										}
									}

									if (!baseDependencyTrackPoint) {
										baseDependencyTrackPoint = depTrackPoint;
									}

									CDependencyTrackPoint* nextDepTrackPoint = nullptr;
									CBINDPROPAGATEGROUNDINGDependencyNode* groundingDepNode = createBINDPROPAGATEGROUNDINGDependency(nextDepTrackPoint,processIndi,conDes,baseDependencyTrackPoint,additionalsDependencies,calcAlgContext);
									addConceptToIndividual(newGroundedCocnept,newGroundedConceptNegation,processIndi,nextDepTrackPoint,true,false,calcAlgContext);
								}
							}
							KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(mAfterGroundingDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext));
						} else {


							CPROCESSINGHASH<CConcept*,CPropagationBindingDescriptor*>* groundedConPropBindDesHash = nullptr;
							CPROCESSINGHASH<CVariable*,CPropagationBindingDescriptor*>* additionalPropBindDesHash = nullptr;
							CSortedNegLinker<CConcept*>* newGroundedLinker = mGroundingHandler->getGroundingConceptLinker(processIndi,propBindSet,concept,negated,groundedConPropBindDesHash,additionalPropBindDesHash,calcAlgContext);

							if (newGroundedLinker) {

								for (CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker; opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
									CConcept* bindingTriggerConcept = opConLinkerIt->getData();
									bool bindingTriggerConceptNegation = opConLinkerIt->isNegated();

									CConceptDescriptor* bindingConDes = nullptr;
									CDependencyTrackPoint* bindingDepTrackPoint = nullptr;
									CCondensedReapplyQueue* reapplyQueue = nullptr;

									CPropagationBindingSet* prevPropBindingSet = propBindSet;
									CPropagationBindingSet* newPropBindingSet = conPropBindSetHash->getPropagationBindingSet(bindingTriggerConcept,true);

									bool createdBindConcept = false;
									bool doneBindPropagations = false;

									if (!conSet->getConceptDescriptorAndReapplyQueue(bindingTriggerConcept,bindingConDes,bindingDepTrackPoint,reapplyQueue)) {
										CConcept* newGroundedCocnept = newGroundedLinker->getData();
										bool newGroundedConceptNegation = newGroundedLinker->isNegated();

										CDependencyTrackPoint* baseDependencyTrackPoint = nullptr;
										CDependency* additionalsDependencies = nullptr;
										CPROCESSINGHASH<CConcept*,CPropagationBindingDescriptor*>::const_iterator it = groundedConPropBindDesHash->constFind(newGroundedCocnept), itEnd = groundedConPropBindDesHash->constEnd();
										while (it != itEnd && it.key() == newGroundedCocnept) {
											CPropagationBindingDescriptor* propBindDes = it.value();
											CDependencyTrackPoint* propBindDepTrackPoint = propBindDes->getDependencyTrackPoint();
											if (propBindDepTrackPoint) {
												if (!baseDependencyTrackPoint) {
													baseDependencyTrackPoint = propBindDepTrackPoint;
												} else {
													CPROPAGATECONNECTIONDependencyNode* connDep = createPROPAGATECONNECTIONDependency(nullptr,conDes,propBindDepTrackPoint,calcAlgContext);
													additionalsDependencies = connDep->append(additionalsDependencies);
												}
											}
											++it;
										}

										for (CPROCESSINGHASH<CVariable*,CPropagationBindingDescriptor*>::const_iterator it = additionalPropBindDesHash->constBegin(), itEnd = additionalPropBindDesHash->constEnd(); it != itEnd; ++it) {
											CPropagationBindingDescriptor* propBindDes = *it;
											CDependencyTrackPoint* propBindDepTrackPoint = propBindDes->getDependencyTrackPoint();
											if (propBindDepTrackPoint) {
												if (!baseDependencyTrackPoint) {
													baseDependencyTrackPoint = propBindDepTrackPoint;
												} else {
													CPROPAGATECONNECTIONDependencyNode* connDep = createPROPAGATECONNECTIONDependency(nullptr,conDes,propBindDepTrackPoint,calcAlgContext);
													additionalsDependencies = connDep->append(additionalsDependencies);
												}
											}
										}

										if (!baseDependencyTrackPoint) {
											baseDependencyTrackPoint = depTrackPoint;
										}
										CDependencyTrackPoint* nextDepTrackPoint = nullptr;
										CBINDPROPAGATEGROUNDINGDependencyNode* groundingDepNode = createBINDPROPAGATEGROUNDINGDependency(nextDepTrackPoint,processIndi,conDes,baseDependencyTrackPoint,additionalsDependencies,calcAlgContext);

										bindingConDes = addConceptToIndividualReturnConceptDescriptor(bindingTriggerConcept,bindingTriggerConceptNegation,processIndi,nextDepTrackPoint,false,false,calcAlgContext);
										conSet = processIndi->getReapplyConceptLabelSet(true);

										createdBindConcept = true;
									}



									for (CSortedNegLinker<CConcept*>* newGroundedLinkerIt = newGroundedLinker; newGroundedLinkerIt; newGroundedLinkerIt = newGroundedLinkerIt->getNext()) {
										CConcept* newGroundedCocnept = newGroundedLinkerIt->getData();
										bool newGroundedConceptNegation = newGroundedLinkerIt->isNegated();

										CPROCESSINGHASH<CConcept*,CPropagationBindingDescriptor*>::const_iterator it = groundedConPropBindDesHash->constFind(newGroundedCocnept), itEnd = groundedConPropBindDesHash->constEnd();
										while (it != itEnd && it.key() == newGroundedCocnept) {
											CPropagationBindingDescriptor* propBindDes = it.value();
											CPropagationBinding* propBinding = propBindDes->getPropagationBinding();
											if (!newPropBindingSet->containsPropagationBinding(propBinding)) {
												STATINC(PBINDGROUNDINGCOUNT,calcAlgContext);
												CDependencyTrackPoint* propBindDepTrackPoint = propBindDes->getDependencyTrackPoint();

												CDependencyTrackPoint* newDepTrackPoint = nullptr;
												CPROPAGATEBINDINGDependencyNode* bindDepNode = createPROPAGATEBINDINGDependency(newDepTrackPoint,processIndi,conDes,propBindDepTrackPoint,nullptr,calcAlgContext);
												CPropagationBindingDescriptor* newPropBindDes = CObjectAllocator<CPropagationBindingDescriptor>::allocateAndConstruct(taskMemMan);
												newPropBindDes->initPropagationBindingDescriptor(propBinding,newDepTrackPoint);
												newPropBindingSet->addPropagationBinding(newPropBindDes,false);

												doneBindPropagations = true;
											}
											++it;
										}
										for (CPROCESSINGHASH<CVariable*,CPropagationBindingDescriptor*>::const_iterator it = additionalPropBindDesHash->constBegin(), itEnd = additionalPropBindDesHash->constEnd(); it != itEnd; ++it) {
											CPropagationBindingDescriptor* propBindDes = *it;
											CDependencyTrackPoint* propBindDepTrackPoint = propBindDes->getDependencyTrackPoint();
											CPropagationBinding* propBinding = propBindDes->getPropagationBinding();
											if (!newPropBindingSet->containsPropagationBinding(propBinding)) {
												STATINC(PBINDGROUNDINGCOUNT,calcAlgContext);
												CDependencyTrackPoint* propBindDepTrackPoint = propBindDes->getDependencyTrackPoint();

												CDependencyTrackPoint* newDepTrackPoint = nullptr;
												CPROPAGATEBINDINGDependencyNode* bindDepNode = createPROPAGATEBINDINGDependency(newDepTrackPoint,processIndi,conDes,propBindDepTrackPoint,nullptr,calcAlgContext);
												CPropagationBindingDescriptor* newPropBindDes = CObjectAllocator<CPropagationBindingDescriptor>::allocateAndConstruct(taskMemMan);
												newPropBindDes->initPropagationBindingDescriptor(propBinding,newDepTrackPoint);
												newPropBindingSet->addPropagationBinding(newPropBindDes,false);

												doneBindPropagations = true;
											}
										}


									}


									if (doneBindPropagations) {
										if (!createdBindConcept) {
											if (propagateFreshPropagationBindings(processIndi,conDes,newPropBindingSet,prevPropBindingSet,nullptr,calcAlgContext)) {
												setIndividualNodeConceptLabelSetModified(processIndi,calcAlgContext);
												CConceptProcessingQueue* conProQueue = processIndi->getConceptProcessingQueue(true);;
												addConceptPreprocessedToProcessingQueue(bindingConDes,bindingDepTrackPoint,conProQueue,processIndi,true,calcAlgContext);
												if (!reapplyQueue->isEmpty()) {
													CCondensedReapplyQueueIterator reapplyQueueIt(conSet->getConceptReapplyIterator(bindingConDes));
													applyReapplyQueueConcepts(processIndi,&reapplyQueueIt,calcAlgContext);
												}
											}
										}
									}


								}
							}
						}

					}
				}







				void CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDPROPAGATECYCLERule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					CConcept* concept = conDes->getConcept();
					cint64 opCount = concept->getOperandCount();
					CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();


					CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(false);
					CConceptDescriptor* bindingConDes = nullptr;
					CDependencyTrackPoint* bindingDepTrackPoint = nullptr;
					CCondensedReapplyQueue* reapplyQueue = nullptr;

					CConcept* bindingTriggerConcept = opLinker->getData();
					bool bindingTriggerConceptNegation = opLinker->isNegated();
					CSortedNegLinker<CConcept*>* triggerLinker = opLinker->getNext();
					CConcept* cycleTriggerConcept = triggerLinker->getData();
					bool cycleTriggerConceptNegation = triggerLinker->isNegated();
					CConceptDescriptor* cycleTriggerConDes = nullptr;
					CDependencyTrackPoint* cycleTriggerDepTrackPoint = nullptr;



					bool testCycleBindingClosed = false;
					bool addCycleCloseConcept = false;
					if (!conSet->getConceptDescriptorAndReapplyQueue(bindingTriggerConcept,bindingConDes,bindingDepTrackPoint,reapplyQueue)) {
						bool cycleTriggersAvailable = true;
						if (conSet->getConceptDescriptor(cycleTriggerConcept,cycleTriggerConDes,cycleTriggerDepTrackPoint)) {
							if (cycleTriggerConDes->isNegated() == cycleTriggerConceptNegation) {
								return;
							}
						} else {
							cycleTriggersAvailable = false;
						}

						if (cycleTriggersAvailable) {
							addCycleCloseConcept = true;
							testCycleBindingClosed = true;
						}
					} else {
						testCycleBindingClosed = true;
					}


					CPropagationBindingDescriptor* cycleTriggerConceptNewBindingDescriptor = nullptr;
					CPropagationBindingDescriptor* cyclePropagationConceptDescriptor = nullptr;
					CPropagationBindingSet* cycleTriggerConceptBindingSet = nullptr;
					CPropagationBindingSet* cyclePropConceptBindingSet = nullptr;
					CConceptPropagationBindingSetHash* conPropBindingSetHash = nullptr;
					CPropagationBinding* cycleTriggerConceptNewBinding = nullptr;
					bool cycleBindingPropagation = false;
					if (testCycleBindingClosed) {
						conPropBindingSetHash = processIndi->getConceptPropagationBindingSetHash(false);
						if (conPropBindingSetHash) {
							cycleTriggerConceptBindingSet = conPropBindingSetHash->getPropagationBindingSet(cycleTriggerConcept,false);
							if (cycleTriggerConceptBindingSet) {
								cycleTriggerConceptNewBindingDescriptor = cycleTriggerConceptBindingSet->getNewSepcialPropagationBindingDescriptor();
								cycleTriggerConceptNewBinding = cycleTriggerConceptNewBindingDescriptor->getPropagationBinding();
								cint64 bindingID = cycleTriggerConceptNewBinding->getPropagationID();

								cyclePropConceptBindingSet = conPropBindingSetHash->getPropagationBindingSet(concept,false);
								if (cyclePropConceptBindingSet) {
									cyclePropagationConceptDescriptor = cyclePropConceptBindingSet->getPropagationBindingDescriptor(cycleTriggerConceptNewBinding);
									if (cyclePropagationConceptDescriptor) {
										cycleBindingPropagation = true;
									}
								}
							}
						}
					}


					bool propagations = false;
					if (cycleBindingPropagation) {
						STATINC(PBINDRULECYCLEAPPLICATIONCOUNT,calcAlgContext);

						if (addCycleCloseConcept) {
							conSet = processIndi->getReapplyConceptLabelSet(true);
							CBINDPROPAGATECYCLEDependencyNode* cycleDepNode = createBINDPROPAGATECYCLEDependency(bindingDepTrackPoint,processIndi,conDes,cyclePropagationConceptDescriptor->getDependencyTrackPoint(),cycleTriggerConceptNewBindingDescriptor->getDependencyTrackPoint(),calcAlgContext);

							bindingConDes = addConceptToIndividualReturnConceptDescriptor(bindingTriggerConcept,bindingTriggerConceptNegation,processIndi,bindingDepTrackPoint,true,false,calcAlgContext);
						}

						// propagate bindings
						CPropagationBindingMap* cyclePropConceptBindingMap = cyclePropConceptBindingSet->getPropagationBindingMap();
						CPropagationBindingMap* cycleTriggerConceptBindingMap = cycleTriggerConceptBindingSet->getPropagationBindingMap();

						CPropagationBindingMap::iterator itCycle = cyclePropConceptBindingMap->begin(), itCycleEnd = cyclePropConceptBindingMap->end();
						CPropagationBindingMap::iterator itTrigger = cycleTriggerConceptBindingMap->begin(), itTriggerEnd = cycleTriggerConceptBindingMap->end();

						CPropagationBindingSet* newPropBindSet = conPropBindingSetHash->getPropagationBindingSet(bindingTriggerConcept,false);
						CPropagationBindingMap* newPropBindMap = nullptr;
						CPropagationBindingDescriptor* newPropBindDesLinker = nullptr;
						if (newPropBindSet) {
							newPropBindMap = newPropBindSet->getPropagationBindingMap();
						}

						while (itCycle != itCycleEnd) {
							cint64 cycleID = itCycle.key();
							cint64 triggerID = -1;
							if (itTrigger != itTriggerEnd) {
								triggerID = itTrigger.key();
							}
							if (triggerID != -1 && triggerID < cycleID) {
								++itTrigger;
							} else if (triggerID == cycleID) {

								if (!newPropBindSet || !newPropBindSet->containsPropagationBinding(cycleID)) {
									STATINC(PBINDCYCLEPROPAGATECOUNT,calcAlgContext);
									if (!newPropBindSet) {
										newPropBindSet = conPropBindingSetHash->getPropagationBindingSet(bindingTriggerConcept,true);
										newPropBindMap = newPropBindSet->getPropagationBindingMap();
										newPropBindSet->setConceptDescriptor(bindingConDes);
									}

									CPropagationBindingMapData& cyclePropBindMapData = itCycle.value();
									CPropagationBindingDescriptor* cyclePropBindDes = cyclePropBindMapData.getPropagationBindingDescriptor();

									CPropagationBindingMapData& triggerPropBindMapData = itTrigger.value();
									CPropagationBindingDescriptor* triggerPropBindDes = triggerPropBindMapData.getPropagationBindingDescriptor();

									CPROPAGATECONNECTIONDependencyNode* connDep1 = createPROPAGATECONNECTIONDependency(nullptr,cycleTriggerConceptBindingSet->getConceptDescriptor(),triggerPropBindDes->getDependencyTrackPoint(),calcAlgContext);
									CPROPAGATECONNECTIONDependencyNode* connDep2 = createPROPAGATECONNECTIONDependency(nullptr,conDes,cyclePropagationConceptDescriptor->getDependencyTrackPoint(),calcAlgContext);
									CDependency* connDep = connDep1->setNext(connDep2);

									CDependencyTrackPoint* newDepTrackPoint = nullptr;
									CPROPAGATEBINDINGDependencyNode* bindDepNode = createPROPAGATEBINDINGDependency(newDepTrackPoint,processIndi,conDes,cyclePropBindDes->getDependencyTrackPoint(),connDep,calcAlgContext);
									CPropagationBinding* propBinding = cyclePropBindDes->getPropagationBinding();
									CPropagationBindingDescriptor* newPropBindDes = CObjectAllocator<CPropagationBindingDescriptor>::allocateAndConstruct(taskMemMan);
									newPropBindDes->initPropagationBindingDescriptor(propBinding,newDepTrackPoint);

									CPropagationBindingMapData& data = (*newPropBindMap)[propBinding->getPropagationID()];
									data.setPropagationBindingDescriptor(newPropBindDes);
									CPropagationBindingReapplyConceptDescriptor* reapplyDes = data.getReapplyConceptDescriptor();
									if (reapplyDes) {
										applyReapplyQueueConcepts(processIndi,reapplyDes,calcAlgContext);
									}
									newPropBindDesLinker = newPropBindDes->append(newPropBindDesLinker);

									propagations = true;
								}

								++itCycle;
								++itTrigger;

							} else {

								if (!newPropBindSet || !newPropBindSet->containsPropagationBinding(cycleID)) {

									CPropagationBindingMapData& cyclePropBindMapData = itCycle.value();
									CPropagationBindingDescriptor* cyclePropBindDes = cyclePropBindMapData.getPropagationBindingDescriptor();
									CPropagationBinding* propBinding = cyclePropBindDes->getPropagationBinding();
									CIndividualProcessNode* bindedIndiNode = propBinding->getBindedIndividual();
									CConceptDescriptor* bindedConDes = propBinding->getBindedConceptDescriptor();

									bindedIndiNode = getUpToDateIndividual(bindedIndiNode,calcAlgContext);
									bindedIndiNode = getCorrectedMergedIntoIndividualNode(bindedIndiNode,calcAlgContext);

									CConceptPropagationBindingSetHash* bindedIndiConPropBindingSetHash = bindedIndiNode->getConceptPropagationBindingSetHash(false);
									if (bindedIndiConPropBindingSetHash) {
										CPropagationBindingSet* bindedIndiConPropBindingSet = bindedIndiConPropBindingSetHash->getPropagationBindingSet(bindedConDes->getConcept(),false);
										if (bindedIndiConPropBindingSet) {

											CPropagationBindingDescriptor* triggerPropBindDes = bindedIndiConPropBindingSet->getPropagationBindingDescriptor(cycleTriggerConceptNewBinding);
											if (triggerPropBindDes) {
												STATINC(PBINDCYCLEPROPAGATECOUNT,calcAlgContext);
												if (!newPropBindSet) {
													newPropBindSet = conPropBindingSetHash->getPropagationBindingSet(bindingTriggerConcept,true);
													newPropBindMap = newPropBindSet->getPropagationBindingMap();
												}

												// do propagation
												CPROPAGATECONNECTIONDependencyNode* connDep1 = createPROPAGATECONNECTIONDependency(bindedIndiNode,cycleTriggerConceptBindingSet->getConceptDescriptor(),triggerPropBindDes->getDependencyTrackPoint(),calcAlgContext);
												CPROPAGATECONNECTIONDependencyNode* connDep2 = createPROPAGATECONNECTIONDependency(nullptr,conDes,cyclePropagationConceptDescriptor->getDependencyTrackPoint(),calcAlgContext);
												CDependency* connDep = connDep1->setNext(connDep2);

												CDependencyTrackPoint* newDepTrackPoint = nullptr;
												CPROPAGATEBINDINGDependencyNode* bindDepNode = createPROPAGATEBINDINGDependency(newDepTrackPoint,processIndi,conDes,cyclePropBindDes->getDependencyTrackPoint(),connDep,calcAlgContext);
												CPropagationBindingDescriptor* newPropBindDes = CObjectAllocator<CPropagationBindingDescriptor>::allocateAndConstruct(taskMemMan);
												newPropBindDes->initPropagationBindingDescriptor(propBinding,newDepTrackPoint);

												CPropagationBindingMapData& data = (*newPropBindMap)[propBinding->getPropagationID()];
												data.setPropagationBindingDescriptor(newPropBindDes);
												CPropagationBindingReapplyConceptDescriptor* reapplyDes = data.getReapplyConceptDescriptor();
												if (reapplyDes) {
													applyReapplyQueueConcepts(processIndi,reapplyDes,calcAlgContext);
												}
												newPropBindDesLinker = newPropBindDes->append(newPropBindDesLinker);
												propagations = true;
											} else {
												// install rule reapplication if variable bindings will change
												TIndividualConceptPair indiConPair(bindedIndiNode->getIndividualID(),concept);
												CPropagationBindingReapplyConceptHash* reapplyHash = bindedIndiConPropBindingSet->getPropagationBindingReapplyConceptHash(false);
												if (reapplyHash) {
													if (!reapplyHash->hasPropagationBindingReapplyConceptDescriptor(indiConPair)) {
														STATINC(PBINDCYCLEREAPPLYINSTALLCOUNT,calcAlgContext);
														bindedIndiNode = getLocalizedIndividual(bindedIndiNode,false,calcAlgContext);
														bindedIndiConPropBindingSetHash = bindedIndiNode->getConceptPropagationBindingSetHash(true);
														bindedIndiConPropBindingSet = bindedIndiConPropBindingSetHash->getPropagationBindingSet(bindedConDes->getConcept(),true);

														CPropagationBindingReapplyConceptDescriptor* propBindReapplyConDes = CObjectAllocator<CPropagationBindingReapplyConceptDescriptor>::allocateAndConstruct(taskMemMan);
														propBindReapplyConDes->initReapllyDescriptor(processIndi,cycleTriggerConceptNewBinding,conDes,depTrackPoint);

														bindedIndiConPropBindingSet->addPropagationBindingReapplyConceptDescriptor(propBindReapplyConDes);
													}
												}
											}
										}
									}
								}
								++itCycle;

							}
						}
						if (newPropBindSet && newPropBindDesLinker) {
							newPropBindSet->addPropagationBindingDescriptorLinker(newPropBindDesLinker);
						}


					}


					if (propagations && !addCycleCloseConcept) {
						setIndividualNodeConceptLabelSetModified(processIndi,calcAlgContext);
						CConceptProcessingQueue* conProQueue = processIndi->getConceptProcessingQueue(true);;
						addConceptPreprocessedToProcessingQueue(bindingConDes,bindingDepTrackPoint,conProQueue,processIndi,true,calcAlgContext);
						if (!reapplyQueue->isEmpty()) {
							CCondensedReapplyQueueIterator reapplyQueueIt(conSet->getConceptReapplyIterator(bindingConDes));
							applyReapplyQueueConcepts(processIndi,&reapplyQueueIt,calcAlgContext);
						}
					}



					if (!isConceptInReapplyQueue(conDes,cycleTriggerConcept,!cycleTriggerConceptNegation,processIndi,calcAlgContext)) {
						addConceptToReapplyQueue(conDes,cycleTriggerConcept,!cycleTriggerConceptNegation,processIndi,(CProcessingRestrictionSpecification*)nullptr,depTrackPoint,calcAlgContext);
					}

				}





				void CCalculationTableauCompletionTaskHandleAlgorithm::propagatePropagationBindingsToSuccessor(CIndividualProcessNode* processIndi, CIndividualProcessNode*& succIndi, CSortedNegLinker<CConcept*>* conceptOpLinker, bool negate, CConceptDescriptor* conDes, CIndividualLinkEdge* restLink, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CDependencyTrackPoint* depTrackPoint = conDes->getDependencyTrackPoint();
					CConcept* concept = conDes->getConcept();


					succIndi = getLocalizedIndividual(succIndi,false,calcAlgContext);
					CReapplyConceptLabelSet* conSet = succIndi->getReapplyConceptLabelSet(false);

					// create dependency
					CDependencyTrackPoint* nextDepTrackPoint = nullptr;
					bool continuePropagation = false;

					for (CSortedNegLinker<CConcept*>* conceptOpLinkerIt = conceptOpLinker; conceptOpLinkerIt; conceptOpLinkerIt = conceptOpLinkerIt->getNext()) {
						CConcept* opConcept = conceptOpLinkerIt->getData();
						bool opConNeg = conceptOpLinkerIt->isNegated() ^ negate;

						CConceptDescriptor* bindingConDes = nullptr;
						CDependencyTrackPoint* bindingDepTrackPoint = nullptr;
						CCondensedReapplyQueue* reapplyQueue = nullptr;
						if (!conSet->getConceptDescriptorAndReapplyQueue(opConcept,bindingConDes,bindingDepTrackPoint,reapplyQueue)) {


							if (!nextDepTrackPoint) {
								conSet = processIndi->getReapplyConceptLabelSet(true);
								CBINDPROPAGATEALLDependencyNode* bindDepNode = createBINDPROPAGATEALLDependency(nextDepTrackPoint,processIndi,conDes,depTrackPoint,restLink->getDependencyTrackPoint(),calcAlgContext);
							}
							bindingConDes = addConceptToIndividualReturnConceptDescriptor(opConcept,opConNeg,succIndi,nextDepTrackPoint,false,false,calcAlgContext);

							CConceptPropagationBindingSetHash* conPropBindingSetHash = processIndi->getConceptPropagationBindingSetHash(true);
							CPropagationBindingSet* prevPropBindingSet = conPropBindingSetHash->getPropagationBindingSet(concept,false);
							CConceptPropagationBindingSetHash* succConPropBindingSetHash = succIndi->getConceptPropagationBindingSetHash(true);
							CPropagationBindingSet* propBindingSet = succConPropBindingSetHash->getPropagationBindingSet(opConcept,true);
							propBindingSet->setConceptDescriptor(bindingConDes);

							propagateInitialPropagationBindingsToSuccessor(processIndi,succIndi,bindingConDes,propBindingSet,prevPropBindingSet,restLink,calcAlgContext);
							continuePropagation = true;
						} else {

							CConceptPropagationBindingSetHash* conPropBindingSetHash = processIndi->getConceptPropagationBindingSetHash(true);
							CPropagationBindingSet* prevPropBindingSet = conPropBindingSetHash->getPropagationBindingSet(concept,false);
							CConceptPropagationBindingSetHash* succConPropBindingSetHash = succIndi->getConceptPropagationBindingSetHash(true);
							CPropagationBindingSet* propBindingSet = succConPropBindingSetHash->getPropagationBindingSet(opConcept,true);

							if (propagateFreshPropagationBindingsToSuccessor(processIndi,succIndi,conDes,propBindingSet,prevPropBindingSet,restLink,calcAlgContext)) {
								setIndividualNodeConceptLabelSetModified(succIndi,calcAlgContext);
								CConceptProcessingQueue* conProQueue = succIndi->getConceptProcessingQueue(true);;
								addConceptPreprocessedToProcessingQueue(bindingConDes,bindingDepTrackPoint,conProQueue,succIndi,true,calcAlgContext);
								if (!reapplyQueue->isEmpty()) {
									CCondensedReapplyQueueIterator reapplyQueueIt(conSet->getConceptReapplyIterator(bindingConDes));
									applyReapplyQueueConcepts(succIndi,&reapplyQueueIt,calcAlgContext);
								}
								continuePropagation = true;
							}
						}
					}

					if (continuePropagation) {
						addIndividualToProcessingQueue(succIndi,calcAlgContext);
					}
				}






				bool CCalculationTableauCompletionTaskHandleAlgorithm::propagateInitialPropagationBindingsToSuccessor(CIndividualProcessNode*& processIndi, CIndividualProcessNode* succIndi, CConceptDescriptor* conDes, CPropagationBindingSet* newPropBindingSet, CPropagationBindingSet* prevPropBindingSet, CIndividualLinkEdge* restLink, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					bool propagations = false;
					CPropagationBindingDescriptor* newPropBindDesLinker = nullptr;
					if (prevPropBindingSet) {
						propagations |= newPropBindingSet->adoptPropagateAllFlag(prevPropBindingSet);

						newPropBindingSet->copyPropagationBindings(prevPropBindingSet->getPropagationBindingMap());
						CPropagationBindingMap* propBindMap = newPropBindingSet->getPropagationBindingMap();
						for (CPropagationBindingMap::iterator it = propBindMap->begin(), itEnd = propBindMap->end(); it != itEnd; ++it) {
							STATINC(PBINDPROPAGATEDCOUNT,calcAlgContext);
							STATINC(PBINDPROPAGATEDINITIALCOUNT,calcAlgContext);
							CPropagationBindingMapData& propBindMapData = it.value();
							propBindMapData.clearReapplyConceptDescriptor();
							CPropagationBindingDescriptor* prevPropBindDes = propBindMapData.getPropagationBindingDescriptor();
							CPropagationBindingDescriptor* newPropBindDes = CObjectAllocator<CPropagationBindingDescriptor>::allocateAndConstruct(taskMemMan);
							CDependencyTrackPoint* newDepTrackPoint = nullptr;
							CPROPAGATEBINDINGSSUCCESSORDependencyNode* bindDepNode = createPROPAGATEBINDINGSSUCCESSORDependency(newDepTrackPoint,processIndi,conDes,prevPropBindDes->getDependencyTrackPoint(),restLink->getDependencyTrackPoint(),calcAlgContext);
							newPropBindDes->initPropagationBindingDescriptor(prevPropBindDes->getPropagationBinding(),newDepTrackPoint);
							propBindMapData.setPropagationBindingDescriptor(newPropBindDes);
							newPropBindDesLinker = newPropBindDes->append(newPropBindDesLinker);
							propagations = true;
						}
						if (newPropBindDesLinker) {
							newPropBindingSet->addPropagationBindingDescriptorLinker(newPropBindDesLinker);
						}
					}
					return propagations;
				}




				bool CCalculationTableauCompletionTaskHandleAlgorithm::propagateFreshPropagationBindingsToSuccessor(CIndividualProcessNode*& processIndi, CIndividualProcessNode* succIndi, CConceptDescriptor* conDes, CPropagationBindingSet* newPropBindingSet, CPropagationBindingSet* prevPropBindingSet, CIndividualLinkEdge* restLink, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					bool propagations = false;
					if (prevPropBindingSet) {
						propagations |= newPropBindingSet->adoptPropagateAllFlag(prevPropBindingSet);

						CPropagationBindingMap* prevPropBindMap = prevPropBindingSet->getPropagationBindingMap();
						CPropagationBindingMap* newPropBindMap = newPropBindingSet->getPropagationBindingMap();

						CPropagationBindingMap::iterator itNew = newPropBindMap->begin();
						CPropagationBindingMap::const_iterator itPrev = prevPropBindMap->constBegin(), itPrevEnd = prevPropBindMap->constEnd();
						
						CPropagationBindingDescriptor* newPropBindDesLinker = nullptr;

						while (itPrev != itPrevEnd) {
							cint64 prevPropID = itPrev.key();
							bool doPropagation = false;
							bool updateExisting = false;
							if (itNew == newPropBindMap->end()) {
								doPropagation = true;
							} else {
								cint64 newPropID = itNew.key();
								if (newPropID < prevPropID) {
									++itNew;
								} else if (newPropID == prevPropID) {
									if (!itNew.value().hasPropagationBindingDescriptor()) {
										doPropagation = true;
										updateExisting = true;
									} else {
										++itNew;
										++itPrev;
									}
								} else {
									doPropagation = true;
								}
							}
							if (doPropagation) {
								STATINC(PBINDPROPAGATEDCOUNT,calcAlgContext);
								STATINC(PBINDPROPAGATEDFRESHCOUNT,calcAlgContext);
								const CPropagationBindingMapData& prevPropBindMapData = itPrev.value();
								CPropagationBindingDescriptor* prevPropBindDes = prevPropBindMapData.getPropagationBindingDescriptor();

								CPropagationBindingDescriptor* newPropBindDes = CObjectAllocator<CPropagationBindingDescriptor>::allocateAndConstruct(taskMemMan);

								CDependencyTrackPoint* newDepTrackPoint = nullptr;
								CPROPAGATEBINDINGSSUCCESSORDependencyNode* bindDepNode = createPROPAGATEBINDINGSSUCCESSORDependency(newDepTrackPoint,processIndi,conDes,prevPropBindDes->getDependencyTrackPoint(),restLink->getDependencyTrackPoint(),calcAlgContext);
								CPropagationBinding* propBinding = prevPropBindDes->getPropagationBinding();
								newPropBindDes->initPropagationBindingDescriptor(propBinding,newDepTrackPoint);

								if (updateExisting) {
									CPropagationBindingMapData& data = (*newPropBindMap)[propBinding->getPropagationID()];
									data.setPropagationBindingDescriptor(newPropBindDes);
									CPropagationBindingReapplyConceptDescriptor* reapplyDes = data.getReapplyConceptDescriptor();
									if (reapplyDes) {
										applyReapplyQueueConcepts(succIndi,reapplyDes,calcAlgContext);
									}
								} else {
									itNew = newPropBindMap->insert(propBinding->getPropagationID(),CPropagationBindingMapData(newPropBindDes));
								}
								newPropBindDesLinker = newPropBindDes->append(newPropBindDesLinker);
								propagations = true;
							}
						}
						if (newPropBindDesLinker) {
							newPropBindingSet->addPropagationBindingDescriptorLinker(newPropBindDesLinker);
						}
					}
					return propagations;
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDPROPAGATEALLRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					cint64 opCount = concept->getOperandCount();
					CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					CSortedNegLinker<CConcept*>* conceptOpLinker = concept->getOperandList();

					STATINC(PBINDRULEALLAPPLICATIONCOUNT,calcAlgContext);

					CIndividualLinkEdge* restLink = getLinkProcessingRestriction(conProDes);
					if (restLink) {
						CIndividualProcessNode* succIndi = getSuccessorIndividual(processIndi,restLink,calcAlgContext);

						propagatePropagationBindingsToSuccessor(processIndi,succIndi,opLinker,negate,conDes,restLink,calcAlgContext);
						
					} else {
						CReapplyRoleSuccessorHash* roleSuccHash = processIndi->getReapplyRoleSuccessorHash(false);
						if (roleSuccHash) {
							CRoleSuccessorLinkIterator roleSuccIt = roleSuccHash->getRoleSuccessorLinkIterator(role);
							while (roleSuccIt.hasNext()) {
								CIndividualLinkEdge* link = roleSuccIt.next(true);
								CIndividualProcessNode* succIndi = getSuccessorIndividual(processIndi,link,calcAlgContext);

								propagatePropagationBindingsToSuccessor(processIndi,succIndi,opLinker,negate,conDes,link,calcAlgContext);
							}
						}
					}
					if (!conProDes->isConceptReapplied()) {
						if (!isConceptInReapplyQueue(conDes,role,processIndi,calcAlgContext)) {
							addConceptToReapplyQueue(conDes,role,processIndi,true,depTrackPoint,calcAlgContext);
						}
					}
				}






				void CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDPROPAGATEIMPLICATIONRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					cint64 opCount = concept->getOperandCount();
					CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					STATINC(PBINDRULEIMPLICATIONAPPLICATIONCOUNT,calcAlgContext);

					CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(false);
					CConceptDescriptor* bindingConDes = nullptr;
					CDependencyTrackPoint* bindingDepTrackPoint = nullptr;
					CCondensedReapplyQueue* reapplyQueue = nullptr;

					CConcept* bindingTriggerConcept = opLinker->getData();
					bool bindingTriggerConceptNegation = opLinker->isNegated();
					CSortedNegLinker<CConcept*>* triggerLinker = opLinker->getNext();

					if (!conSet->getConceptDescriptorAndReapplyQueue(bindingTriggerConcept,bindingConDes,bindingDepTrackPoint,reapplyQueue)) {

						// search next not existing trigger
						bool allTriggersAvailable = true;
						conSet = processIndi->getReapplyConceptLabelSet(true);
						CSortedNegLinker<CConcept*>* triggerLinkerIt = nullptr;
						for (triggerLinkerIt = triggerLinker; triggerLinkerIt; triggerLinkerIt = triggerLinkerIt->getNext()) {
							CSortedNegLinker<CConcept*>* nextTrigger = triggerLinkerIt;
							CConcept* triggerConcept = nextTrigger->getData();
							CConceptDescriptor* triggerConDes = nullptr;
							CDependencyTrackPoint* triggerDepTrackPoint = nullptr;
							if (conSet->getConceptDescriptor(triggerConcept,triggerConDes,triggerDepTrackPoint)) {
								if (triggerConDes->isNegated() == nextTrigger->isNegated()) {
									return;
								}
							} else {
								allTriggersAvailable = false;
								break;
							}
						}

						if (!allTriggersAvailable) {
							// install to trigger
							CSortedNegLinker<CConcept*>* nextTrigger = triggerLinkerIt;
							CConcept* triggerConcept = nextTrigger->getData();
							bool triggerNegation = !nextTrigger->isNegated();
							if (!isConceptInReapplyQueue(conDes,triggerConcept,triggerNegation,processIndi,calcAlgContext)) {
								addConceptToReapplyQueue(conDes,triggerConcept,triggerNegation,processIndi,(CProcessingRestrictionSpecification*)nullptr,depTrackPoint,calcAlgContext);
							}
						} else {
							CDependency* triggerDeps = nullptr;
							for (CSortedNegLinker<CConcept*>* triggerLinkerIt = triggerLinker; triggerLinkerIt; triggerLinkerIt = triggerLinkerIt->getNext()) {
								CConcept* triggerConcept = triggerLinkerIt->getData();
								CConceptDescriptor* triggerConDes = nullptr;
								CDependencyTrackPoint* triggerDepTrackPoint = nullptr;
								conSet->getConceptDescriptor(triggerConcept,triggerConDes,triggerDepTrackPoint);
								CCONNECTIONDependencyNode* connDep = createCONNECTIONDependency(processIndi,triggerConDes,triggerDepTrackPoint,calcAlgContext);
								connDep->setNext(triggerDeps);
								triggerDeps = connDep;
							}

							CDependencyTrackPoint* nextDepTrackPoint = nullptr;
							CBINDPROPAGATEIMPLICATIONDependencyNode* implDepNode = createBINDPROPAGATEIMPLICATIONDependency(nextDepTrackPoint,processIndi,conDes,depTrackPoint,triggerDeps,calcAlgContext);

							bindingConDes = addConceptToIndividualReturnConceptDescriptor(bindingTriggerConcept,bindingTriggerConceptNegation,processIndi,nextDepTrackPoint,true,false,calcAlgContext);

							CConceptPropagationBindingSetHash* conPropBindingSetHash = processIndi->getConceptPropagationBindingSetHash(true);
							CPropagationBindingSet* prevPropBindingSet = conPropBindingSetHash->getPropagationBindingSet(concept,false);
							CPropagationBindingSet* propBindingSet = conPropBindingSetHash->getPropagationBindingSet(bindingTriggerConcept,true);
							propBindingSet->setConceptDescriptor(bindingConDes);

							propagateInitialPropagationBindings(processIndi,bindingConDes,propBindingSet,prevPropBindingSet,triggerDeps,calcAlgContext);

						}
					} else {
						CDependency* triggerDeps = nullptr;
						for (CSortedNegLinker<CConcept*>* triggerLinkerIt = triggerLinker; triggerLinkerIt; triggerLinkerIt = triggerLinkerIt->getNext()) {
							CConcept* triggerConcept = triggerLinkerIt->getData();
							CConceptDescriptor* triggerConDes = nullptr;
							CDependencyTrackPoint* triggerDepTrackPoint = nullptr;
							conSet->getConceptDescriptor(triggerConcept,triggerConDes,triggerDepTrackPoint);
							CCONNECTIONDependencyNode* connDep = createCONNECTIONDependency(processIndi,triggerConDes,triggerDepTrackPoint,calcAlgContext);
							connDep->setNext(triggerDeps);
							triggerDeps = connDep;
						}

						CConceptPropagationBindingSetHash* conPropBindingSetHash = processIndi->getConceptPropagationBindingSetHash(true);
						CPropagationBindingSet* prevPropBindingSet = conPropBindingSetHash->getPropagationBindingSet(concept,false);
						CPropagationBindingSet* propBindingSet = conPropBindingSetHash->getPropagationBindingSet(bindingTriggerConcept,true);

						if (propagateFreshPropagationBindings(processIndi,conDes,propBindingSet,prevPropBindingSet,triggerDeps,calcAlgContext)) {
							setIndividualNodeConceptLabelSetModified(processIndi,calcAlgContext);
							CConceptProcessingQueue* conProQueue = processIndi->getConceptProcessingQueue(true);;
							addConceptPreprocessedToProcessingQueue(bindingConDes,bindingDepTrackPoint,conProQueue,processIndi,true,calcAlgContext);
							if (!reapplyQueue->isEmpty()) {
								CCondensedReapplyQueueIterator reapplyQueueIt(conSet->getConceptReapplyIterator(bindingConDes));
								applyReapplyQueueConcepts(processIndi,&reapplyQueueIt,calcAlgContext);
							}
						}
					}
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDPROPAGATEANDRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					propagatePropagationBindings(processIndi,conProDes,negate,false,calcAlgContext);
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDPROPAGATEANDFLAGALLRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					++mStatBackPropActivationCount;
					propagatePropagationBindings(processIndi,conProDes,negate,true,calcAlgContext);
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::propagatePropagationBindings(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, bool propagateAllFlag, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					bool conceptNegation = negate;
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();

					STATINC(PBINDRULEANDAPPLICATIONCOUNT,calcAlgContext);

					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(false);

					CDependencyTrackPoint* nextDepTrackPoint = nullptr;

					for (CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker; opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
						CConcept* bindingTriggerConcept = opConLinkerIt->getData();
						bool bindingTriggerConceptNegation = opConLinkerIt->isNegated()^conceptNegation;

						CConceptDescriptor* bindingConDes = nullptr;
						CDependencyTrackPoint* bindingDepTrackPoint = nullptr;
						CCondensedReapplyQueue* reapplyQueue = nullptr;

						if (!conSet->getConceptDescriptorAndReapplyQueue(bindingTriggerConcept,bindingConDes,bindingDepTrackPoint,reapplyQueue)) {
							if (!nextDepTrackPoint) {
								conSet = processIndi->getReapplyConceptLabelSet(true);
								CBINDPROPAGATEANDDependencyNode* bindDepNode = createBINDPROPAGATEANDDependency(nextDepTrackPoint,processIndi,conDes,depTrackPoint,calcAlgContext);
							}
							bindingConDes = addConceptToIndividualReturnConceptDescriptor(bindingTriggerConcept,bindingTriggerConceptNegation,processIndi,nextDepTrackPoint,false,false,calcAlgContext);

							CConceptPropagationBindingSetHash* conPropBindingSetHash = processIndi->getConceptPropagationBindingSetHash(true);
							CPropagationBindingSet* prevPropBindingSet = conPropBindingSetHash->getPropagationBindingSet(concept,false);
							CPropagationBindingSet* propBindingSet = conPropBindingSetHash->getPropagationBindingSet(bindingTriggerConcept,true);
							propBindingSet->setConceptDescriptor(bindingConDes);

							if (propagateAllFlag) {
								propBindingSet->setPropagateAllFlag(true);
							}

							propagateInitialPropagationBindings(processIndi,bindingConDes,propBindingSet,prevPropBindingSet,nullptr,calcAlgContext);
						} else {

							CConceptPropagationBindingSetHash* conPropBindingSetHash = processIndi->getConceptPropagationBindingSetHash(true);
							CPropagationBindingSet* prevPropBindingSet = conPropBindingSetHash->getPropagationBindingSet(concept,false);
							CPropagationBindingSet* propBindingSet = conPropBindingSetHash->getPropagationBindingSet(bindingTriggerConcept,true);

							if (propagateFreshPropagationBindings(processIndi,conDes,propBindingSet,prevPropBindingSet,nullptr,calcAlgContext) || propagateAllFlag && !propBindingSet->hasPropagateAllFlag()) {
								if (propagateAllFlag) {
									propBindingSet->setPropagateAllFlag(true);
								}
								setIndividualNodeConceptLabelSetModified(processIndi,calcAlgContext);
								CConceptProcessingQueue* conProQueue = processIndi->getConceptProcessingQueue(true);;
								addConceptPreprocessedToProcessingQueue(bindingConDes,bindingDepTrackPoint,conProQueue,processIndi,true,calcAlgContext);
								if (!reapplyQueue->isEmpty()) {
									CCondensedReapplyQueueIterator reapplyQueueIt(conSet->getConceptReapplyIterator(bindingConDes));
									applyReapplyQueueConcepts(processIndi,&reapplyQueueIt,calcAlgContext);
								}
							}
						}
					}
				}





				void CCalculationTableauCompletionTaskHandleAlgorithm::applyBINDVARIABLERule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {

					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CVariable* variable = concept->getVariable();
					bool conceptNegation = negate;
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();

					CConcept* bindingTriggerConcept = opConLinker->getData();
					bool bindingTriggerConceptNegation = opConLinker->isNegated();

					CProcessContext* processContext = calcAlgContext->getUsedProcessContext();
					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(false);
					CConceptDescriptor* bindingConDes = nullptr;
					CDependencyTrackPoint* bindingDepTrackPoint = nullptr;
					CCondensedReapplyQueue* reapplyQueue = nullptr;

					STATINC(PBINDRULEBINDNAPPLICATIONCOUNT,calcAlgContext);

					if (!conSet->getConceptDescriptorAndReapplyQueue(bindingTriggerConcept,bindingConDes,bindingDepTrackPoint,reapplyQueue)) {
						STATINC(PBINDVARIABLEBINDCOUNT,calcAlgContext);
						CDependencyTrackPoint* nextDepTrackPoint = nullptr;
						CBINDVARIABLEDependencyNode* bindDepNode = createBINDVARIABLEDependency(nextDepTrackPoint,processIndi,conDes,depTrackPoint,calcAlgContext);
						conSet = processIndi->getReapplyConceptLabelSet(true);

						bindingConDes = addConceptToIndividualReturnConceptDescriptor(bindingTriggerConcept,bindingTriggerConceptNegation,processIndi,nextDepTrackPoint,false,false,calcAlgContext);

						CConceptPropagationBindingSetHash* conPropBindingSetHash = processIndi->getConceptPropagationBindingSetHash(true);
						CPropagationBindingSet* prevPropBindingSet = conPropBindingSetHash->getPropagationBindingSet(concept,false);
						CPropagationBindingSet* propBindingSet = conPropBindingSetHash->getPropagationBindingSet(bindingTriggerConcept,true);
						propBindingSet->setConceptDescriptor(bindingConDes);
						propagateInitialPropagationBindings(processIndi,bindingConDes,propBindingSet,prevPropBindingSet,nullptr,calcAlgContext);


						CPropagationBinding* propVarBinding = CObjectAllocator<CPropagationBinding>::allocateAndConstruct(taskMemMan);
						cint64 nextPropBindingID = procDataBox->getNextBindingPropagationID(true);
						propVarBinding->initPropagationBinding(nextPropBindingID,nextDepTrackPoint,processIndi,bindingConDes,variable);
						CPropagationBindingDescriptor* propBindDes = CObjectAllocator<CPropagationBindingDescriptor>::allocateAndConstruct(taskMemMan);
						propBindDes->initPropagationBindingDescriptor(propVarBinding,nextDepTrackPoint);
						propBindingSet->addPropagationBinding(propBindDes,true);

					} else {
						CConceptPropagationBindingSetHash* conPropBindingSetHash = processIndi->getConceptPropagationBindingSetHash(true);
						CPropagationBindingSet* prevPropBindingSet = conPropBindingSetHash->getPropagationBindingSet(concept,false);
						CPropagationBindingSet* propBindingSet = conPropBindingSetHash->getPropagationBindingSet(bindingTriggerConcept,true);

						bool newVarBindCreated = false;
						if (!propBindingSet->getNewSepcialPropagationBindingDescriptor()) {
							STATINC(PBINDVARIABLEBINDCOUNT,calcAlgContext);
							CDependencyTrackPoint* nextDepTrackPoint = nullptr;
							CBINDVARIABLEDependencyNode* bindDepNode = createBINDVARIABLEDependency(nextDepTrackPoint,processIndi,conDes,depTrackPoint,calcAlgContext);
							CPropagationBinding* propVarBinding = CObjectAllocator<CPropagationBinding>::allocateAndConstruct(taskMemMan);
							cint64 nextPropBindingID = procDataBox->getNextBindingPropagationID(true);
							propVarBinding->initPropagationBinding(nextPropBindingID,nextDepTrackPoint,processIndi,bindingConDes,variable);
							CPropagationBindingDescriptor* propBindDes = CObjectAllocator<CPropagationBindingDescriptor>::allocateAndConstruct(taskMemMan);
							propBindDes->initPropagationBindingDescriptor(propVarBinding,nextDepTrackPoint);
							propBindingSet->addPropagationBinding(propBindDes,true);
							newVarBindCreated = true;
						}

						if (propagateFreshPropagationBindings(processIndi,conDes,propBindingSet,prevPropBindingSet,nullptr,calcAlgContext) || newVarBindCreated) {
							setIndividualNodeConceptLabelSetModified(processIndi,calcAlgContext);
							CConceptProcessingQueue* conProQueue = processIndi->getConceptProcessingQueue(true);;
							addConceptPreprocessedToProcessingQueue(bindingConDes,bindingDepTrackPoint,conProQueue,processIndi,true,calcAlgContext);
							if (!reapplyQueue->isEmpty()) {
								CCondensedReapplyQueueIterator reapplyQueueIt(conSet->getConceptReapplyIterator(bindingConDes));
								applyReapplyQueueConcepts(processIndi,&reapplyQueueIt,calcAlgContext);
							}
						}
					}
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::propagateInitialPropagationBindings(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CPropagationBindingSet* newPropBindingSet, CPropagationBindingSet* prevPropBindingSet, CDependency* otherDependencies, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					bool propagations = false;
					CPropagationBindingDescriptor* newPropBindDesLinker = nullptr;
					if (prevPropBindingSet) {
						propagations |= newPropBindingSet->adoptPropagateAllFlag(prevPropBindingSet);

						newPropBindingSet->copyPropagationBindings(prevPropBindingSet->getPropagationBindingMap());
						CPropagationBindingMap* propBindMap = newPropBindingSet->getPropagationBindingMap();
						for (CPropagationBindingMap::iterator it = propBindMap->begin(), itEnd = propBindMap->end(); it != itEnd; ++it) {
							STATINC(PBINDPROPAGATEDCOUNT,calcAlgContext);
							STATINC(PBINDPROPAGATEDINITIALCOUNT,calcAlgContext);
							CPropagationBindingMapData& propBindMapData = it.value();
							propBindMapData.clearReapplyConceptDescriptor();
							CPropagationBindingDescriptor* prevPropBindDes = propBindMapData.getPropagationBindingDescriptor();
							CPropagationBindingDescriptor* newPropBindDes = CObjectAllocator<CPropagationBindingDescriptor>::allocateAndConstruct(taskMemMan);
							CDependencyTrackPoint* newDepTrackPoint = nullptr;
							CPROPAGATEBINDINGDependencyNode* bindDepNode = createPROPAGATEBINDINGDependency(newDepTrackPoint,processIndi,conDes,prevPropBindDes->getDependencyTrackPoint(),otherDependencies,calcAlgContext);
							newPropBindDes->initPropagationBindingDescriptor(prevPropBindDes->getPropagationBinding(),newDepTrackPoint);
							propBindMapData.setPropagationBindingDescriptor(newPropBindDes);
							newPropBindDesLinker = newPropBindDes->append(newPropBindDesLinker);
							propagations = true;
						}
						if (newPropBindDesLinker) {
							newPropBindingSet->addPropagationBindingDescriptorLinker(newPropBindDesLinker);
						}
					}
					return propagations;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::propagateFreshPropagationBindings(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CPropagationBindingSet* newPropBindingSet, CPropagationBindingSet* prevPropBindingSet, CDependency* otherDependencies, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					bool propagations = false;
					if (prevPropBindingSet) {
						propagations |= newPropBindingSet->adoptPropagateAllFlag(prevPropBindingSet);
						CPropagationBindingMap* prevPropBindMap = prevPropBindingSet->getPropagationBindingMap();
						CPropagationBindingMap* newPropBindMap = newPropBindingSet->getPropagationBindingMap();

						CPropagationBindingMap::iterator itNew = newPropBindMap->begin();
						CPropagationBindingMap::const_iterator itPrev = prevPropBindMap->constBegin(), itPrevEnd = prevPropBindMap->constEnd();

						CPropagationBindingDescriptor* newPropBindDesLinker = nullptr;

						while (itPrev != itPrevEnd) {
							cint64 prevPropID = itPrev.key();
							bool doPropagation = false;
							bool updateExisting = false;
							if (itNew == newPropBindMap->end()) {
								doPropagation = true;
							} else {
								cint64 newPropID = itNew.key();
								if (newPropID < prevPropID) {
									++itNew;
								} else if (newPropID == prevPropID) {
									if (!itNew.value().hasPropagationBindingDescriptor()) {
										doPropagation = true;
										updateExisting = true;
									} else {
										++itNew;
										++itPrev;
									}
								} else {
									doPropagation = true;
								}
							}
							if (doPropagation) {
								STATINC(PBINDPROPAGATEDCOUNT,calcAlgContext);
								STATINC(PBINDPROPAGATEDFRESHCOUNT,calcAlgContext);
								const CPropagationBindingMapData& prevPropBindMapData = itPrev.value();
								CPropagationBindingDescriptor* prevPropBindDes = prevPropBindMapData.getPropagationBindingDescriptor();

								CPropagationBindingDescriptor* newPropBindDes = CObjectAllocator<CPropagationBindingDescriptor>::allocateAndConstruct(taskMemMan);

								CDependencyTrackPoint* newDepTrackPoint = nullptr;
								CPROPAGATEBINDINGDependencyNode* bindDepNode = createPROPAGATEBINDINGDependency(newDepTrackPoint,processIndi,conDes,prevPropBindDes->getDependencyTrackPoint(),otherDependencies,calcAlgContext);
								CPropagationBinding* propBinding = prevPropBindDes->getPropagationBinding();
								newPropBindDes->initPropagationBindingDescriptor(propBinding,newDepTrackPoint);

								if (updateExisting) {
									CPropagationBindingMapData& data = (*newPropBindMap)[propBinding->getPropagationID()];
									data.setPropagationBindingDescriptor(newPropBindDes);
									CPropagationBindingReapplyConceptDescriptor* reapplyDes = data.getReapplyConceptDescriptor();
									if (reapplyDes) {
										applyReapplyQueueConcepts(processIndi,reapplyDes,calcAlgContext);
									}
								} else {
									itNew = newPropBindMap->insert(propBinding->getPropagationID(),CPropagationBindingMapData(newPropBindDes));
								}
								newPropBindDesLinker = newPropBindDes->append(newPropBindDesLinker);
								propagations = true;
							}
						}
						if (newPropBindDesLinker) {
							newPropBindingSet->addPropagationBindingDescriptorLinker(newPropBindDesLinker);
						}
					}
					return propagations;
				}




				void CCalculationTableauCompletionTaskHandleAlgorithm::applyReapplyQueueConcepts(CIndividualProcessNode*& processIndi, CPropagationBindingReapplyConceptDescriptor* reapplyDesLinker, CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingRestrictionSpecification* procRest = nullptr;
					for (CPropagationBindingReapplyConceptDescriptor* reapplyDesLinkerIt = reapplyDesLinker; reapplyDesLinkerIt; reapplyDesLinkerIt = reapplyDesLinkerIt->getNext()) {
						STATINC(PBINDREAPPLICATIONCOUNT,calcAlgContext);
						CPropagationBindingReapplyConceptDescriptor* reapplyDes = reapplyDesLinkerIt;
						CConceptDescriptor* conDes = reapplyDes->getConceptDescriptor();
						CDependencyTrackPoint* depTrackPoint = conDes->getDependencyTrackPoint();
						CIndividualProcessNode* indiNode = reapplyDes->getReapllyIndividualNode();
						CIndividualProcessNode* locIndiNode = indiNode;

						if (processIndi->getIndividualID() != indiNode->getIndividualID()) {
							locIndiNode = getLocalizedIndividual(indiNode,true,calcAlgContext);
						}

						CConceptProcessingQueue* conProQueue = locIndiNode->getConceptProcessingQueue(true);
						addConceptRestrictedToProcessingQueue(conDes,depTrackPoint,conProQueue,locIndiNode,false,nullptr,calcAlgContext);

						if (processIndi->getIndividualID() != indiNode->getIndividualID()) {
							addIndividualToProcessingQueue(locIndiNode,calcAlgContext);
						}
					}
				}














































































































				void CCalculationTableauCompletionTaskHandleAlgorithm::applyDATATYPERule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();


					//mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
					//QFile file(QString("debug-model.txt"));
					//if (file.open(QIODevice::WriteOnly)) {
					//	file.write(mEndTaskDebugIndiModelString.replace("<br>","\r\n").toLocal8Bit());
					//	file.close();
					//}

					if (mDatatypeHandler && mConfDatatypeReasoning) {
						mDatatypeHandler->addDatatype(processIndi,concept,negate,depTrackPoint,calcAlgContext);
					}

					if (!negate || concept->getOperandCount() <= 1) {
						applyANDRule(processIndi,conProDes,negate,calcAlgContext);
					} else {
						applyORRule(processIndi,conProDes,negate,calcAlgContext);
					}
				}





				void CCalculationTableauCompletionTaskHandleAlgorithm::applyDATARESTRICTIONRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();


					//mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
					//QFile file(QString("debug-model.txt"));
					//if (file.open(QIODevice::WriteOnly)) {
					//	file.write(mEndTaskDebugIndiModelString.replace("<br>","\r\n").toLocal8Bit());
					//	file.close();
					//}

					if (mDatatypeHandler && mConfDatatypeReasoning) {
						mDatatypeHandler->addDataRestriction(processIndi,concept,negate,depTrackPoint,calcAlgContext);
					}
				}




				void CCalculationTableauCompletionTaskHandleAlgorithm::applyDATALITERALRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CDataLiteral* dataLiteral = concept->getDataLiteral();
					if (dataLiteral) {
						CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();


						//mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
						//QFile file(QString("debug-model.txt"));
						//if (file.open(QIODevice::WriteOnly)) {
						//	file.write(mEndTaskDebugIndiModelString.replace("<br>","\r\n").toLocal8Bit());
						//	file.close();
						//}

						if (mDatatypeHandler && mConfDatatypeReasoning) {
							mDatatypeHandler->addDataLiteral(processIndi,dataLiteral,negate,depTrackPoint,calcAlgContext);
						}
					}
				}




				void CCalculationTableauCompletionTaskHandleAlgorithm::applyBOTTOMRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					++mAppliedANDRuleCount;
					STATINC(ANDRULEAPPLICATIONCOUNT,calcAlgContext);
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();

					CConcept* topConcept = calcAlgContext->getProcessingDataBox()->getOntologyTopConcept();

					// create dependency
					CDependencyTrackPoint* nextDepTrackPoint = nullptr;
					CANDDependencyNode* andDepNode = createANDDependency(nextDepTrackPoint,processIndi,conDes,depTrackPoint,calcAlgContext);

					addConceptToIndividual(topConcept,true,processIndi,nextDepTrackPoint,true,false,calcAlgContext);
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::applyANDRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					++mAppliedANDRuleCount;
					STATINC(ANDRULEAPPLICATIONCOUNT,calcAlgContext);
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					bool conceptNegation = negate;
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();

					// create dependency
					CDependencyTrackPoint* nextDepTrackPoint = nullptr;
					CANDDependencyNode* andDepNode = createANDDependency(nextDepTrackPoint,processIndi,conDes,depTrackPoint,calcAlgContext);

					CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList();

					addConceptsToIndividual(opConLinkerIt,conceptNegation,processIndi,nextDepTrackPoint,true,false,nullptr,calcAlgContext);
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::isGeneratingConceptSatisfiableCachedAbsorpable(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualProcessNode* ancestorIndiNode = getAncestorIndividual(processIndi,calcAlgContext);
					if (ancestorIndiNode) {
						CConcept* concept = conDes->getConcept();
						CRole* role = concept->getRole();
						CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList();
						while (superRoleIt) {
							bool superRoleNeg = superRoleIt->isNegated();
							if (!superRoleNeg) {
								CRole* superRole = superRoleIt->getData();
								if (processIndi->hasRoleSuccessorToIndividual(superRole,ancestorIndiNode,true)) {
									if (superRole->isFunctional()) {
										return false;
									}
									// check additional for ATMOST restriction
									CReapplyRoleSuccessorHash* reapplyRoleSuccHash = processIndi->getReapplyRoleSuccessorHash(false);
									CReapplyQueueIterator reapplyQueueIt = reapplyRoleSuccHash->getRoleReapplyIterator(superRole,false);
									while (reapplyQueueIt.hasNext()) {
										CReapplyConceptDescriptor* reapplyConceptDes = reapplyQueueIt.next();
										CConceptDescriptor* reapplyConDes = reapplyConceptDes->getConceptDescriptor();
										CConcept* reapplyConcept = reapplyConDes->getConcept();
										bool reapplyConNeg = reapplyConDes->getNegation();
										cint64 opCode = reapplyConcept->getOperatorCode();
										if (opCode == CCATMOST || opCode == CCATLEAST) {
											cint64 cardinality = concept->getParameter() + 1*reapplyConNeg;
											if (cardinality <= 1) {
												return false;
											}
										}
									}
								}
							}
							superRoleIt = superRoleIt->getNext();
						}
					}
					return true;
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::applySOMERule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();

					if (!conceptOpLinkerIt->hasNext())  {
						if (!conceptOpLinkerIt->isNegated()^negate) {
							CConcept* nominalConcept = conceptOpLinkerIt->getData();
							if (nominalConcept->getOperatorCode() == CCNOMINAL) {
								STATINC(VALUERULEAPPLICATIONCOUNT,calcAlgContext);
								CIndividual* indi = nominalConcept->getNominalIndividual();
								CIndividualProcessNode* nominalIndi = getCorrectedNominalIndividualNode(indi->getIndividualID(),calcAlgContext);
								CIndividualProcessNode* locNominalIndi = getLocalizedIndividual(nominalIndi,false,calcAlgContext);
								// force initialized
								if (locNominalIndi != processIndi) {
									locNominalIndi = getForcedInitializedNominalIndividualNode(locNominalIndi,calcAlgContext);
								}
								CReapplyConceptLabelSet* locNominalNodeConSet = locNominalIndi->getReapplyConceptLabelSet(true);
								CConcept* nominalConcept = indi->getIndividualNominalConcept();
								CConceptDescriptor* nominalConDes = nullptr;
								CDependencyTrackPoint* nominalConDepTrackPoint = nullptr;
								if (indi->getIndividualID() != locNominalIndi->getIndividualID()) {
									locNominalNodeConSet->getConceptDescriptor(nominalConcept,nominalConDes,nominalConDepTrackPoint);
								}

								if (!hasIndividualsLink(processIndi,locNominalIndi,role,true,calcAlgContext)) {
									// create dependency
									CDependencyTrackPoint* nextDepTrackPoint = nullptr;
									CVALUEDependencyNode* valueDepNode = createVALUEDependency(nextDepTrackPoint,processIndi,conDes,depTrackPoint,nominalConDepTrackPoint,calcAlgContext);

									// create link
									createNewIndividualsLinksReapplyed(processIndi,locNominalIndi,role->getIndirectSuperRoleList(),role,nextDepTrackPoint,true,calcAlgContext);
									
									//propagateIndividualNodeNominalConnectionToAncestors(processIndi,calcAlgContext);
									if (!processIndi->isNominalIndividual()) {
										propagateIndividualNodeNominalConnectionStatusToAncestors(processIndi,locNominalIndi,calcAlgContext);
									}

									propagateIndividualNodeModified(locNominalIndi,calcAlgContext);

									addIndividualToProcessingQueue(locNominalIndi,calcAlgContext);

								}
								return;
							}
						}
					} 

					STATINC(SOMERULEAPPLICATIONCOUNT,calcAlgContext);
					CIndividualProcessNode* alreadyExistSuitableSuccessor = getRoleSuccessorWithConcepts(processIndi,role,conceptOpLinkerIt,negate,calcAlgContext);

					if (!alreadyExistSuitableSuccessor) {
						if (mConfSatExpCachedSuccAbsorp && processIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATISFIABLECACHED | CIndividualProcessNode::PRFSIGNATUREBLOCKINGCACHED | CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHED | CIndividualProcessNode::PRFSATURATIONSUCCESSORCREATIONBLOCKINGCACHED)) {
							if (isGeneratingConceptSatisfiableCachedAbsorpable(processIndi,conDes,calcAlgContext)) {
								STATINC(SATCACHEDABSORBEDGENERATINGCONCEPTSCOUNT,calcAlgContext);
								return addSatisfiableCachedAbsorbedGeneratingConcept(conDes,processIndi,depTrackPoint,calcAlgContext);
							}
						}

						++mAppliedSOMERuleCount;
						STATINC(SEPARATESUCCESSORINDINODECREATIONCOUNT,calcAlgContext);

						if (calcAlgContext->getUsedUnsatisfiableCacheRetrievalStrategy()->testUnsatisfiableCacheForSuccessorGeneration(conProDes,processIndi)) {
							testIndividualNodeUnsatisfiableCached(processIndi,calcAlgContext);
						}


						// generate new role successor
						CIndividualProcessNode* succIndi = tryExtendFunctionalSuccessorIndividual(processIndi,conDes,role->getIndirectSuperRoleList(),role,conceptOpLinkerIt,negate,depTrackPoint,calcAlgContext);
						if (!succIndi) {
							succIndi = createSuccessorIndividual(processIndi,conDes,role->getIndirectSuperRoleList(),role,conceptOpLinkerIt,negate,depTrackPoint,calcAlgContext);
							if (processIndi->isNominalIndividual() && processIndi->getIndividualNominalLevel() <= 0) {
								succIndi->setExtendedQueueProcessing(true);
							}
						}
						// check for backward dependencies
						if (processIndi->isIndividualAncestor(succIndi)) {
							if (mConfSatExpCachedSuccAbsorp && processIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATISFIABLECACHED | CIndividualProcessNode::PRFSIGNATUREBLOCKINGCACHED | CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHED | CIndividualProcessNode::PRFSATURATIONSUCCESSORCREATIONBLOCKINGCACHED)) {
								reapplySatisfiableCachedAbsorbedGeneratingConcepts(processIndi,calcAlgContext);
							}

							CXLinker<CIndividualProcessNode*>* newBackwardDepLinker = CObjectAllocator< CXLinker<CIndividualProcessNode*> >::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
							newBackwardDepLinker->initLinker(processIndi);
							succIndi->addSuccessorIndividualNodeBackwardDependencyLinker(newBackwardDepLinker);
							processIndi->setBackwardDependencyToAncestorIndividualNode(true);
							if (succIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION)) {
								//propagateIndividualNodeNominalConnectionToAncestors(processIndi,calcAlgContext);
								if (!processIndi->isNominalIndividual()) {
									propagateIndividualNodeNominalConnectionStatusToAncestors(processIndi,succIndi,calcAlgContext);
								}
							}
						}

						addIndividualToProcessingQueue(succIndi,calcAlgContext);

						if (mConfAnywhereBlockingSomeInitializationHashing) {
							addIndividualNodeCandidateForConcept(succIndi,conceptOpLinkerIt,negate,calcAlgContext);
						}
					} else {
						// check for backward dependencies
						if (processIndi->isIndividualAncestor(alreadyExistSuitableSuccessor)) {
							CIndividualProcessNode* locAncestorIndiNode = getLocalizedIndividual(alreadyExistSuitableSuccessor,false,calcAlgContext);
							CXLinker<CIndividualProcessNode*>* newBackwardDepLinker = CObjectAllocator< CXLinker<CIndividualProcessNode*> >::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
							newBackwardDepLinker->initLinker(processIndi);
							locAncestorIndiNode->addSuccessorIndividualNodeBackwardDependencyLinker(newBackwardDepLinker);
							processIndi->setBackwardDependencyToAncestorIndividualNode(true);
							if (locAncestorIndiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION)) {
								//propagateIndividualNodeNominalConnectionToAncestors(processIndi,calcAlgContext);
								if (!processIndi->isNominalIndividual()) {
									propagateIndividualNodeNominalConnectionStatusToAncestors(processIndi,locAncestorIndiNode,calcAlgContext);
								}
							}
						}
					}
				}







				void CCalculationTableauCompletionTaskHandleAlgorithm::addRoleAssertion(CIndividualProcessNode*& processIndi, CRoleAssertionLinker* roleAssertionLinker, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(VALUERULEAPPLICATIONCOUNT,calcAlgContext);
					CRole* role = roleAssertionLinker->getRole();
					CIndividual* indi = roleAssertionLinker->getIndividual();

					if (indi) {
						CIndividualProcessNode* nominalIndi = getCorrectedNominalIndividualNode(indi->getIndividualID(),calcAlgContext);
						CIndividualProcessNode* locNominalIndi = getLocalizedIndividual(nominalIndi,false,calcAlgContext);

						if (locNominalIndi != processIndi) {
							locNominalIndi = getForcedInitializedNominalIndividualNode(locNominalIndi,calcAlgContext);
						}

						CReapplyConceptLabelSet* locNominalNodeConSet = locNominalIndi->getReapplyConceptLabelSet(true);
						CConcept* nominalConcept = indi->getIndividualNominalConcept();
						CConceptDescriptor* nominalConDes = nullptr;
						CDependencyTrackPoint* nominalConDepTrackPoint = nullptr;
						if (indi->getIndividualID() != locNominalIndi->getIndividualID()) {
							locNominalNodeConSet->getConceptDescriptor(nominalConcept,nominalConDes,nominalConDepTrackPoint);
						}


						if (!hasIndividualsLink(processIndi,locNominalIndi,role,true,calcAlgContext)) {
							// create dependency
							CDependencyTrackPoint* nextDepTrackPoint = nullptr;
							CVALUEDependencyNode* valueDepNode = createVALUEDependency(nextDepTrackPoint,processIndi,nullptr,depTrackPoint,nominalConDepTrackPoint,calcAlgContext);

							// create link
							createNewIndividualsLinksReapplyed(processIndi,locNominalIndi,role->getIndirectSuperRoleList(),role,nextDepTrackPoint,true,calcAlgContext);
							//propagateIndividualNodeNominalConnectionToAncestors(processIndi,calcAlgContext);
							//propagateIndividualNodeNominalConnectionStatusToAncestors(processIndi,locNominalIndi,calcAlgContext);

							propagateIndividualNodeModified(locNominalIndi,calcAlgContext);
							addIndividualToProcessingQueue(locNominalIndi,calcAlgContext);
						}
					}
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::applyVALUERule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(VALUERULEAPPLICATIONCOUNT,calcAlgContext);
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					CIndividual* indi = concept->getNominalIndividual();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();

					if (indi) {

						CIndividualProcessNode* nominalIndi = getCorrectedNominalIndividualNode(indi->getIndividualID(),calcAlgContext);

						CIndividualProcessNode* locNominalIndi = getLocalizedIndividual(nominalIndi,false,calcAlgContext);
						// force initialized
						if (locNominalIndi != processIndi) {
							locNominalIndi = getForcedInitializedNominalIndividualNode(locNominalIndi,calcAlgContext);
						}
						CReapplyConceptLabelSet* locNominalNodeConSet = locNominalIndi->getReapplyConceptLabelSet(true);
						CConcept* nominalConcept = indi->getIndividualNominalConcept();
						CConceptDescriptor* nominalConDes = nullptr;
						CDependencyTrackPoint* nominalConDepTrackPoint = nullptr;
						if (indi->getIndividualID() != locNominalIndi->getIndividualID()) {
							locNominalNodeConSet->getConceptDescriptor(nominalConcept,nominalConDes,nominalConDepTrackPoint);
						}


						if (!negate) {

							if (!hasIndividualsLink(processIndi,locNominalIndi,role,true,calcAlgContext)) {
								// create dependency
								CDependencyTrackPoint* nextDepTrackPoint = nullptr;
								CVALUEDependencyNode* valueDepNode = createVALUEDependency(nextDepTrackPoint,processIndi,conDes,depTrackPoint,nominalConDepTrackPoint,calcAlgContext);

								// create link
								createNewIndividualsLinksReapplyed(processIndi,locNominalIndi,role->getIndirectSuperRoleList(),role,nextDepTrackPoint,true,calcAlgContext);
								//propagateIndividualNodeNominalConnectionToAncestors(processIndi,calcAlgContext);
								if (!processIndi->isNominalIndividual()) {
									propagateIndividualNodeNominalConnectionStatusToAncestors(processIndi,locNominalIndi,calcAlgContext);
								}

								propagateIndividualNodeModified(locNominalIndi,calcAlgContext);
								addIndividualToProcessingQueue(locNominalIndi,calcAlgContext);
							}
						} else {

							// create dependency
							CDependencyTrackPoint* nextDepTrackPoint = nullptr;
							CNEGVALUEDependencyNode* negValueDepNode = createNEGVALUEDependency(nextDepTrackPoint,processIndi,conDes,depTrackPoint,nominalConDepTrackPoint,calcAlgContext);

							// create negation/disjoint link
							createIndividualNodeNegationLink(processIndi,locNominalIndi,role,nextDepTrackPoint,calcAlgContext);
							//propagateIndividualNodeNominalConnectionToAncestors(processIndi,calcAlgContext);
							if (!processIndi->isNominalIndividual()) {
								propagateIndividualNodeNominalConnectionStatusToAncestors(processIndi,locNominalIndi,calcAlgContext);
							}

							addIndividualToProcessingQueue(locNominalIndi,calcAlgContext);
						}
					}
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::applyFUNCTIONALRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(FUNCTIONALRULEAPPLICATIONCOUNT,calcAlgContext);
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();
					bool reapplied = conProDes->isConceptReapplied();

					CRoleSuccessorLinkIterator roleSuccIt(processIndi->getRoleSuccessorLinkIterator(role));
					CIndividualProcessNode* firstSuccNode = nullptr;
					CIndividualLinkEdge* firstLink = nullptr;
					CDependencyTrackPoint* nnRequireDepTrackPoint = nullptr;
					bool nominalNode = processIndi->isNominalIndividual();
					bool requiresNNRule = false;
					bool hasAlreadyMergingNominal = false;
					if (nominalNode && roleSuccIt.hasNext()) {
						// has to apply NN-rule?
						CRoleSuccessorLinkIterator checkNNRoleSuccIt(roleSuccIt);
						while (!hasAlreadyMergingNominal && checkNNRoleSuccIt.hasNext()) {
							CIndividualLinkEdge* link = checkNNRoleSuccIt.next();
							if (!requiresNNRule) {
								if (link->getCreatorIndividualID() != processIndi->getIndividualID()) {
									requiresNNRule = true;
									nnRequireDepTrackPoint = link->getDependencyTrackPoint();
								}
							}
							if (!hasAlreadyMergingNominal) {
								CIndividualProcessNode* succIndi = getSuccessorIndividual(processIndi,link,calcAlgContext);
								if (succIndi->isNominalIndividual()) {
									hasAlreadyMergingNominal = true;
									firstSuccNode = succIndi;
									firstLink = link;
								}
							}
						}
					}
					if (!hasAlreadyMergingNominal) {
						if (requiresNNRule) {

							createNominalsSuccessorIndividuals(processIndi,role->getIndirectSuperRoleList(),role,conceptOpLinkerIt,false,nnRequireDepTrackPoint,1,calcAlgContext);
							CRoleSuccessorLinkIterator lastRoleSuccIt = processIndi->getRoleSuccessorHistoryLinkIterator(role,nullptr);
							if (lastRoleSuccIt.hasNext()) {
								CIndividualLinkEdge* link = lastRoleSuccIt.next();
								CIndividualProcessNode* nominalSuccIndi = getSuccessorIndividual(processIndi,link,calcAlgContext);
								firstSuccNode = nominalSuccIndi;
								firstLink = link;
							}

						} else {
							while (!firstSuccNode && roleSuccIt.hasNext()) {
								firstLink = roleSuccIt.next();
								if (roleSuccIt.hasNext()) {
									CIndividualProcessNode* succIndi = getSuccessorIndividual(processIndi,firstLink,calcAlgContext);
									firstSuccNode = succIndi;
								}
							}
						}
					}
					if (firstSuccNode) {
						CIndividualProcessNode* locFirstSuccNode = nullptr;
						while (roleSuccIt.hasNext()) {
							CIndividualLinkEdge* link = roleSuccIt.next();
							if (link != firstLink) {
								CIndividualProcessNode* succIndi = getSuccessorIndividual(processIndi,link,calcAlgContext);
								CClashedDependencyDescriptor* clashDescriptors = nullptr;
								if (isIndividualNodesMergeable(firstSuccNode,succIndi,clashDescriptors,calcAlgContext)) {
									if (!locFirstSuccNode) {
										locFirstSuccNode = getLocalizedIndividual(firstSuccNode,link,calcAlgContext);
										firstSuccNode = locFirstSuccNode;
									}
									CIndividualProcessNode* locSuccIndiNode = getLocalizedIndividual(succIndi,false,calcAlgContext);

									// create dependency
									CDependencyTrackPoint* nextDepTrackPoint = nullptr;
									CFUNCTIONALDependencyNode* funcDepNode = createFUNCTIONALDependency(nextDepTrackPoint,processIndi,conDes,depTrackPoint,firstLink->getDependencyTrackPoint(),link->getDependencyTrackPoint(),calcAlgContext);

									// merge to first successor
									locFirstSuccNode = getMergedIndividualNodes(locFirstSuccNode,locSuccIndiNode,nextDepTrackPoint,calcAlgContext);
									if (locFirstSuccNode == locSuccIndiNode) {
										firstLink = link;
									}

									if (mConfSatExpCachedSuccAbsorp && processIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATISFIABLECACHED | CIndividualProcessNode::PRFSIGNATUREBLOCKINGCACHED | CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHED | CIndividualProcessNode::PRFSATURATIONSUCCESSORCREATIONBLOCKINGCACHED)) {
										if (locFirstSuccNode == getAncestorIndividual(processIndi,calcAlgContext)) {
											reapplySatisfiableCachedAbsorbedGeneratingConcepts(processIndi,calcAlgContext);
										}
									}

								} else {
									// clash
									clashDescriptors = createClashedConceptDescriptor(clashDescriptors,processIndi,conDes,depTrackPoint,calcAlgContext);
									clashDescriptors = createIndividualMergeCausingDescriptors(clashDescriptors,firstSuccNode,firstLink,nullptr,calcAlgContext);
									clashDescriptors = createIndividualMergeCausingDescriptors(clashDescriptors,succIndi,link,nullptr,calcAlgContext);
									throw CCalculationClashProcessingException(clashDescriptors);
								}
							}
						}
					}
					if (!reapplied) {
						CDependencyTrackPoint* contDepTrackPoint = depTrackPoint;
						addConceptToReapplyQueue(conDes,role,processIndi,true,contDepTrackPoint,calcAlgContext);
					}
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::hasIdenticalConceptOperands(CSortedNegLinker<CConcept*>* opConLinker1, CSortedNegLinker<CConcept*>* opConLinker2) {
					if (opConLinker1->getCount() != opConLinker2->getCount()) {
						return false;
					}
					for (CSortedNegLinker<CConcept*>* opConLinker1It = opConLinker1; opConLinker1It; opConLinker1It = opConLinker1It->getNext()) {
						CConcept* con1 = opConLinker1It->getData();
						bool neg1 = opConLinker1It->isNegated();
						bool foundOperand = false;
						for (CSortedNegLinker<CConcept*>* opConLinker2It = opConLinker2; opConLinker2It && !foundOperand; opConLinker2It = opConLinker2It->getNext()) {
							CConcept* con2 = opConLinker2It->getData();
							bool neg2 = opConLinker2It->isNegated();
							if (con1 == con2 && neg1 == neg2) {
								foundOperand = true;
							}
						}
						if (!foundOperand) {
							return false;
						}
					}
					for (CSortedNegLinker<CConcept*>* opConLinker2It = opConLinker2; opConLinker2It; opConLinker2It = opConLinker2It->getNext()) {
						CConcept* con1 = opConLinker2It->getData();
						bool neg1 = opConLinker2It->isNegated();
						bool foundOperand = false;
						for (CSortedNegLinker<CConcept*>* opConLinker1It = opConLinker1; opConLinker1It && !foundOperand; opConLinker1It = opConLinker1It->getNext()) {
							CConcept* con2 = opConLinker1It->getData();
							bool neg2 = opConLinker1It->isNegated();
							if (con1 == con2 && neg1 == neg2) {
								foundOperand = true;
							}
						}
						if (!foundOperand) {
							return false;
						}
					}
					return true;
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::applyATMOSTRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(ATMOSTRULEAPPLICATIONCOUNT,calcAlgContext);
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CProcessContext* processContext = calcAlgContext->getUsedProcessContext();
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();
					CProcessingRestrictionSpecification* procRest = conProDes->getProcessingRestrictionSpecification();

					cint64 cardinality = concept->getParameter() - 1*negate;

					bool installReapplication = false;
					if (cardinality < 0) {
						// clash
						CClashedDependencyDescriptor* clashConDesLinker = nullptr;
						clashConDesLinker = createClashedConceptDescriptor(clashConDesLinker,processIndi,conDes,depTrackPoint,calcAlgContext);
						throw CCalculationClashProcessingException(clashConDesLinker);
					}
					if (cardinality == 1 && !conceptOpLinkerIt) {
						return applyFUNCTIONALRule(processIndi,conProDes,negate,calcAlgContext);
					}


					if (mConfSatExpCachedMergAbsorp && processIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATISFIABLECACHED | CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHED)) {
						STATINC(SATCACHEDABSORBEDMERGINGCONCEPTSCOUNT,calcAlgContext);
						return addSatisfiableCachedAbsorbedDisjunctionConcept(conDes,processIndi,procRest,depTrackPoint,calcAlgContext);
					}

					CReapplyRoleSuccessorHash* roleSuccHash = processIndi->getReapplyRoleSuccessorHash(false);
					CBranchingMergingProcessingRestrictionSpecification* branchingMergingProcRest = nullptr;
					CRoleSuccessorLinkIterator roleSuccIt;
					CIndividualLinkEdge* usingLastLink = nullptr;
					cint64 linkCount = 0;
					if (!procRest) {
						if (roleSuccHash) {
							roleSuccIt = roleSuccHash->getRoleSuccessorLinkIterator(role,&linkCount,usingLastLink);
						}
						if (!roleSuccIt.hasNext()) {
							// no need for merging
						} else if (cardinality <= 0 && !conceptOpLinkerIt) {
							// clash
							CClashedDependencyDescriptor* clashConDesLinker = nullptr;
							clashConDesLinker = createClashedConceptDescriptor(clashConDesLinker,processIndi,conDes,depTrackPoint,calcAlgContext);
							CIndividualLinkEdge* link = roleSuccIt.next(false);
							clashConDesLinker = createClashedIndividualLinkDescriptor(clashConDesLinker,link,link->getDependencyTrackPoint(),calcAlgContext);
							throw CCalculationClashProcessingException(clashConDesLinker);
						}


						if (mConfAtleastAtmostFastClashCheck) {
							CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(false);
							if (conSet) {
								CConceptDescriptor* conDesIt = conSet->getAddingSortedConceptDescriptionLinker();
								while (conDesIt) {
									CConcept* atleastConcept = conDesIt->getData();
									bool conNegated = conDesIt->isNegated();
									cint64 opCode = atleastConcept->getOperatorCode();
									if (!conNegated && (opCode == CCATLEAST) || conNegated && (opCode == CCATMOST)) {
										cint64 param = atleastConcept->getParameter();
										cint64 atleastCardinality = param + 1*conNegated;

										if (atleastCardinality > cardinality && (hasIdenticalConceptOperands(atleastConcept->getOperandList(),conceptOpLinkerIt) || !conceptOpLinkerIt)) {
											CRole* atmostRole = role;
											CRole* atleastRole = atleastConcept->getRole();


											CSortedNegLinker<CRole*>* superRoleIt = atleastRole->getIndirectSuperRoleList();
											while (superRoleIt) {
												if (!superRoleIt->isNegated()) {
													if (superRoleIt->getData() == atmostRole) {
														CClashedDependencyDescriptor* clashConDesLinker = nullptr;
														clashConDesLinker = createClashedConceptDescriptor(clashConDesLinker,processIndi,conDes,depTrackPoint,calcAlgContext);
														clashConDesLinker = createClashedConceptDescriptor(clashConDesLinker,processIndi,conDesIt,conDesIt->getDependencyTrackPoint(),calcAlgContext);
														throw CCalculationClashProcessingException(clashConDesLinker);
													}
												}
												superRoleIt = superRoleIt->getNext();
											}
										}
									}
									conDesIt = conDesIt->getNext();
								}
							}
						}

						// create basis dependency
						CATMOSTDependencyNode* atMostDepNode = createATMOSTDependency(processIndi,conDes,depTrackPoint,calcAlgContext);
						CNonDeterministicDependencyTrackPoint* atMostNonDetDepTrackPoint = createNonDeterministicDependencyTrackPointBranch(atMostDepNode,true,calcAlgContext);

						// initialize qualifying and merging
						branchingMergingProcRest = CObjectParameterizingAllocator< CBranchingMergingProcessingRestrictionSpecification,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,processContext);
						branchingMergingProcRest->initBranchingMergingProcessingRestriction();
						branchingMergingProcRest->initDependencyTracker(atMostNonDetDepTrackPoint);
						branchingMergingProcRest->initMergingDependencyNode(atMostDepNode);

						if (calcAlgContext->getUsedUnsatisfiableCacheRetrievalStrategy()->testUnsatisfiableCacheForMergingInitialization(conProDes,processIndi)) {
							testIndividualNodeUnsatisfiableCached(processIndi,calcAlgContext);
						}

					} else {
						CBranchingMergingProcessingRestrictionSpecification* prevBranchingMergingProcRest = (CBranchingMergingProcessingRestrictionSpecification*)procRest;

						roleSuccIt = roleSuccHash->getRoleSuccessorHistoryLinkIterator(role,prevBranchingMergingProcRest->getLastIndividualLink(),&linkCount);

						if (cardinality <= 0 && linkCount > 0 && !conceptOpLinkerIt) {
							// clash
							CClashedDependencyDescriptor* clashConDesLinker = nullptr;
							clashConDesLinker = createClashedConceptDescriptor(clashConDesLinker,processIndi,conDes,depTrackPoint,calcAlgContext);
							CIndividualLinkEdge* link = roleSuccIt.next(false);
							clashConDesLinker = createClashedIndividualLinkDescriptor(clashConDesLinker,link,link->getDependencyTrackPoint(),calcAlgContext);
							throw CCalculationClashProcessingException(clashConDesLinker);
						}

						CBranchingMergingProcessingRestrictionSpecification* newBranchingMergingProcRest = CObjectParameterizingAllocator< CBranchingMergingProcessingRestrictionSpecification,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,processContext);
						newBranchingMergingProcRest->initBranchingMergingProcessingRestriction(prevBranchingMergingProcRest);

						branchingMergingProcRest = newBranchingMergingProcRest;
					}

					//lastBranchingMergingProcRest = branchingMergingProcRest;


					//CIndividual* nominalIndividual = processIndi->getNominalIndividual();
					//if (nominalIndividual && cardinality == 2) {
					//	if (CIRIName::getRecentIRIName(nominalIndividual->getIndividualNameLinker()) == "http://www.cs.man.ac.uk/~stevensr/ontology/fhkb.owl#richard_john_bright_1962") {
					//		bool bug = true;
					//	}
					//}

					
					initializeMergingIndividualNodes(processIndi,conProDes,&roleSuccIt,usingLastLink,conceptOpLinkerIt,branchingMergingProcRest,calcAlgContext);
					qualifyMergingIndividualNodes(processIndi,conProDes,branchingMergingProcRest,calcAlgContext);
					if (mConfPairwiseMerging) {
						mergeMergingIndividualNodesPairwise(processIndi,conProDes,linkCount,cardinality,branchingMergingProcRest,calcAlgContext);
					} else {
						mergeMergingIndividualNodes(processIndi,conProDes,linkCount,cardinality,branchingMergingProcRest,calcAlgContext);
					}

					installReapplication = true;
					if (installReapplication) {
						CDependencyTrackPoint* contDepTrackPoint = depTrackPoint;
						addConceptToReapplyQueue(conDes,role,processIndi,branchingMergingProcRest,contDepTrackPoint,calcAlgContext);
					}
				}


				QString CCalculationTableauCompletionTaskHandleAlgorithm::generateDebugMergingQueueString(CBranchingMergingProcessingRestrictionSpecification* branchingMergingProcRest, CCalculationAlgorithmContextBase* calcAlgContext) {
					QString mergingString;
					CBranchingMergingIndividualNodeCandidateLinker* mergCandLinker = branchingMergingProcRest->getMergingCandidateNodeLinker();
					while (mergCandLinker) {
						CIndividualProcessNode* indiNode = mergCandLinker->getMergingIndividualNodeCandidate();

						QString indiString = QString("[ %1 ] = ").arg(indiNode->getIndividualID());
						QString conSetString;
						CReapplyConceptLabelSet* conSet = indiNode->getReapplyConceptLabelSet(false);
						CReapplyConceptLabelSetIterator conSetIt = conSet->getConceptLabelSetIterator(false,false,false);
						while (conSetIt.hasNext()) {
							CConceptDescriptor* conDes = conSetIt.next();
							CConcept* concept = conDes->getConcept();
							cint64 conTag = conDes->getConceptTag();
							if (conTag != 1) {
								QString conString = QString("%1%2").arg((conDes->isNegated())?"-":"").arg(conTag);
								if (concept->hasClassName()) {
									conString += CIRIName::getRecentIRIName(concept->getClassNameLinker());
								}
								if (!conSetString.isEmpty()) {
									conSetString += QString(", ");
								}
								conSetString += conString;
							}
						}
						indiString += QString("{%1} \n").arg(conSetString);
						mergingString += indiString;

						mergCandLinker = mergCandLinker->getNext();
					}
					return mergingString;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::mergeMergingIndividualNodesPairwise(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, cint64 linkCount, cint64 cardinality, CBranchingMergingProcessingRestrictionSpecification* branchingMergingProcRest, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();
					CDependencyTrackPoint* baseDepTrackPoint = branchingMergingProcRest->getDependencyTrackPoint();
					cint64 indiCount = processIndi->getRoleSuccessorCount(role);
					if (indiCount > cardinality) {
						CSatisfiableCalculationTask* newTaskList = nullptr;

						CMERGEDependencyNode* mergeDependencyNode = createMERGEDependency(processIndi,nullptr,baseDepTrackPoint,calcAlgContext);
						CClashedDependencyDescriptor* clashDescriptors = nullptr;

						CRoleSuccessorLinkIterator roleSuccIt1 = processIndi->getRoleSuccessorLinkIterator(role);
						while (roleSuccIt1.hasNext()) {
							CIndividualLinkEdge* link1 = roleSuccIt1.next();
							CIndividualProcessNode* succNode1 = getSuccessorIndividual(processIndi,link1,calcAlgContext);

							clashDescriptors = createIndividualMergeCausingDescriptors(clashDescriptors,succNode1,link1,conceptOpLinkerIt,calcAlgContext);

							CRoleSuccessorLinkIterator roleSuccIt2 = roleSuccIt1;
							while (roleSuccIt2.hasNext()) {
								CIndividualLinkEdge* link2 = roleSuccIt2.next();
								CIndividualProcessNode* succNode2 = getSuccessorIndividual(processIndi,link2,calcAlgContext);

								clashDescriptors = createIndividualMergeCausingDescriptors(clashDescriptors,succNode2,link2,conceptOpLinkerIt,calcAlgContext);

								if (isIndividualNodesMergeable(succNode1,succNode2,clashDescriptors,calcAlgContext)) {
									CSatisfiableCalculationTask* newTask = createMergeBranchingTask(processIndi,conProDes,succNode1,succNode2,mergeDependencyNode,branchingMergingProcRest,calcAlgContext);
									newTaskList = (CSatisfiableCalculationTask*)newTask->append(newTaskList);
								}
							}
						}

						if (mergeDependencyNode) {
							mergeDependencyNode->addBranchClashes(clashDescriptors);
						}

						if (newTaskList) {
							CTaskProcessorContext* processorContext = calcAlgContext->getUsedTaskProcessorContext();
							processorContext->getTaskProcessorCommunicator()->communicateTaskCreation(newTaskList);
						} else {
							clashDescriptors = createClashedConceptDescriptor(clashDescriptors,processIndi,nullptr,baseDepTrackPoint,calcAlgContext);
							throw CCalculationClashProcessingException(clashDescriptors);
						}

						throw CCalculationStopProcessingException(true);
					}
					return false;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::mergeMergingIndividualNodes(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, cint64 linkCount, cint64 cardinality, CBranchingMergingProcessingRestrictionSpecification* branchingMergingProcRest, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();
					CPROCESSSET<cint64>* distinctMergedSet = branchingMergingProcRest->getDistinctMergedNodesSet(false);
					CPROCESSSET<cint64>* locDistinctMergedSet = nullptr;

					cint64 deterministicMergingAttempt = 0;
					cint64 deterministicMergingStep = 0;

					cint64 distinctCount = 0;
					if (distinctMergedSet) {
						distinctCount = distinctMergedSet->count();
					}
					cint64 linkAndCandidateCount = branchingMergingProcRest->getRemainingLinkerMergingCandidateIndividualNodeCount() + distinctCount;
					if (linkCount != linkAndCandidateCount) {
						// update 
						branchingMergingProcRest->setRemainingValidMergingCandidateIndividualNodeCount(linkCount-distinctCount);
					}

					CDependencyTrackPoint* baseDepTrackPoint = branchingMergingProcRest->getDependencyTrackPoint();

					CDependencyTrackPoint* nextDepTrackPoint = nullptr;

					cint64 tmpCardinality = cardinality;


					if (cardinality <= 0 && branchingMergingProcRest->getRemainingLinkerMergingCandidateIndividualNodeCount() > 0) {
						// clash
						CBranchingMergingIndividualNodeCandidateLinker* mergeCandLinker = branchingMergingProcRest->takeNextMergingCandidateNodeLinker();
						while (mergeCandLinker) {
							CIndividualProcessNode* mergeCandIndiNode = mergeCandLinker->getMergingIndividualNodeCandidate();
							if (branchingMergingProcRest->hasValidRemainingMergingCandidates() || processIndi->hasRoleSuccessorToIndividual(role,mergeCandIndiNode,true)) {

								CClashedDependencyDescriptor* clashDescriptors = nullptr;

								CIndividualProcessNode* mergingIndiNode = getUpToDateIndividual(mergeCandIndiNode,calcAlgContext);
								clashDescriptors = createIndividualMergeCausingDescriptors(clashDescriptors,mergingIndiNode,mergeCandLinker->getMergingIndividualLink(),conceptOpLinkerIt,calcAlgContext);
								clashDescriptors = createClashedConceptDescriptor(clashDescriptors,processIndi,nullptr,baseDepTrackPoint,calcAlgContext);
								throw CCalculationClashProcessingException(clashDescriptors);

							} else {
								// invalid link
								branchingMergingProcRest->incRemainingValidMergingCandidateIndividualNodeCount();
							}
							mergeCandLinker = branchingMergingProcRest->takeNextMergingCandidateNodeLinker();
						}
					}



					bool fixedNominalMerging = processIndi->isNominalIndividual() && branchingMergingProcRest->hasAddedBlockablePredecessorMergingNodeCandidate();
					bool requiresNNOperating = false;
					if (fixedNominalMerging) {
						cint64 fixDisCount = distinctCount + branchingMergingProcRest->getRemainingNominalCreationCount();
						if (fixDisCount > 0) {
							tmpCardinality = distinctCount;
							if (branchingMergingProcRest->hasRemainingMergingCandidates()) {
								requiresNNOperating = true;
							}
						} else if (linkAndCandidateCount > 0) {
							requiresNNOperating = true;
						}
					}

					CClashedDependencyDescriptor* initClashDescriptors = nullptr;

					if (linkAndCandidateCount > tmpCardinality || requiresNNOperating) {
						// needs merging
						if (fixedNominalMerging) {
							bool requiresNNInitialization = fixedNominalMerging && !branchingMergingProcRest->isDistinctSetFixed();
							if (requiresNNInitialization) {
								STATINC(INDINODEMERGENEWNOMINALINITCOUNT,calcAlgContext);

								if (!locDistinctMergedSet) {
									locDistinctMergedSet = branchingMergingProcRest->getDistinctMergedNodesSet(true);
									distinctMergedSet = locDistinctMergedSet;
									locDistinctMergedSet->clear();
									distinctCount = 0;
								}

								if (mConfLazyNewNominalGeneration) {
									branchingMergingProcRest->setRemainingNominalCreationCount(tmpCardinality);
									tmpCardinality = 0;
								} else {
									// generate new nominal successor nodes
									createNominalsSuccessorIndividuals(processIndi,role->getIndirectSuperRoleList(),role,conceptOpLinkerIt,false,branchingMergingProcRest->getAddedBlockablePredecessorDependencyTrackPoint(),tmpCardinality,calcAlgContext);
									distinctCount = tmpCardinality;

									cint64 disIndiIdx = 0;

									CIndividualLinkEdge* newLastLink = nullptr;
									CRoleSuccessorLinkIterator roleSuccIt(processIndi->getRoleSuccessorHistoryLinkIterator(role,branchingMergingProcRest->getLastIndividualLink()));
									while (roleSuccIt.hasNext()) {
										CIndividualLinkEdge* link = roleSuccIt.next();
										if (!newLastLink) {
											newLastLink = link;
										}
										CIndividualProcessNode* nominalSuccIndi = getSuccessorIndividual(processIndi,link,calcAlgContext);
										distinctMergedSet->insert(nominalSuccIndi->getIndividualID());
										++disIndiIdx;
									}
									branchingMergingProcRest->setLastIndividualLink(newLastLink);
								}
								branchingMergingProcRest->setDistinctSetFixed(true);
							}

						} else {
							while (distinctCount <= 0) {
								if (!locDistinctMergedSet) {
									locDistinctMergedSet = branchingMergingProcRest->getDistinctMergedNodesSet(true);
									distinctMergedSet = locDistinctMergedSet;
								}
								initClashDescriptors = branchingMergingProcRest->getMultipleMergingNodesInitializationClashesDescriptors();
								CBranchingMergingIndividualNodeCandidateLinker* mergeCandLinker = branchingMergingProcRest->takeNextMergingCandidateNodeLinker();
								while (mergeCandLinker) {
									CIndividualProcessNode* mergeCandIndiNode = mergeCandLinker->getMergingIndividualNodeCandidate();

									if (!branchingMergingProcRest->hasValidRemainingMergingCandidates()) {
										mergeCandIndiNode = getCorrectedNominalIndividualNode(mergeCandIndiNode->getIndividualID(),calcAlgContext);
									}

									CIndividualProcessNode* mergingIndiNode = getUpToDateIndividual(mergeCandIndiNode,calcAlgContext);
									initClashDescriptors = createIndividualMergeCausingDescriptors(initClashDescriptors,mergingIndiNode,mergeCandLinker->getMergingIndividualLink(),conceptOpLinkerIt,calcAlgContext);


									distinctMergedSet->insert(mergeCandIndiNode->getIndividualID());

									++distinctCount;
									if (distinctCount > tmpCardinality) {
										// clash, not able to merge 
										if (!initClashDescriptors) {
											initClashDescriptors = branchingMergingProcRest->getMergingNodesInitializationClashesDescriptors();
										}
										initClashDescriptors = createClashedConceptDescriptor(initClashDescriptors,processIndi,nullptr,baseDepTrackPoint,calcAlgContext);
										throw CCalculationClashProcessingException(initClashDescriptors);
									}
									mergeCandLinker = branchingMergingProcRest->takeNextMergingInitializationCandidateNodeLinker();
								}
							}
						}


						if (!branchingMergingProcRest->hasValidRemainingMergingCandidates()) {
							// update distinct hash
							if (!distinctMergedSet) {
								locDistinctMergedSet = branchingMergingProcRest->getDistinctMergedNodesSet(true);
								distinctMergedSet = locDistinctMergedSet;
							}

							for (CPROCESSSET<cint64>::const_iterator disIt = distinctMergedSet->constBegin(), disItEnd = distinctMergedSet->constEnd(); disIt != disItEnd; ++disIt) {
								cint64 distinctIndiID = *disIt;

								CIndividualLinkEdge* disIndiLink = processIndi->getRoleSuccessorToIndividualLink(role,distinctIndiID,true);
								if (!disIndiLink) {
									CIndividualProcessNode* mergedIntoNode = getCorrectedNominalIndividualNode(distinctIndiID,calcAlgContext);
									if (!locDistinctMergedSet) {
										locDistinctMergedSet = branchingMergingProcRest->getDistinctMergedNodesSet(true);
										distinctMergedSet = locDistinctMergedSet;
									}
									locDistinctMergedSet->remove(distinctIndiID);
									if (locDistinctMergedSet->contains(mergedIntoNode->getIndividualID())) {
										--distinctCount;
										if (fixedNominalMerging) {
											tmpCardinality = distinctCount;
										}
									}
									locDistinctMergedSet->insert(mergedIntoNode->getIndividualID());
								} else if (locDistinctMergedSet) {
									locDistinctMergedSet->insert(distinctIndiID);
								}
							}
						}



						while (branchingMergingProcRest->hasRemainingMergingCandidates()) {

							++deterministicMergingAttempt;
							bool createNewNodesAsNominals = false;

							if (mConfLazyNewNominalGeneration) {
								cint64 remainingNewNominalCreationCount = branchingMergingProcRest->getRemainingNominalCreationCount();
								if (remainingNewNominalCreationCount > 0) {
									// generate new nominal

									//createNominalsSuccessorIndividuals(processIndi,role->getIndirectSuperRoleList(),role,conceptOpLinkerIt,false,branchingMergingProcRest->getAddedBlockablePredecessorDependencyTrackPoint(),1,calcAlgContext);
									//if (!locDistinctMergedSet) {
									//	locDistinctMergedSet = branchingMergingProcRest->getDistinctMergedNodesSet(true);
									//	distinctMergedSet = locDistinctMergedSet;
									//}
									//++distinctCount;
									++tmpCardinality;
									createNewNodesAsNominals = true;
									//CRoleSuccessorLinkIterator roleSuccIt(processIndi->getRoleSuccessorHistoryLinkIterator(role,branchingMergingProcRest->getLastIndividualLink()));
									//if (roleSuccIt.hasNext()) {
									//	CIndividualLinkEdge* link = roleSuccIt.next();
									//	branchingMergingProcRest->setLastIndividualLink(link);
									//	CIndividualProcessNode* nominalSuccIndi = getSuccessorIndividual(processIndi,link,calcAlgContext);
									//	distinctMergedSet->insert(nominalSuccIndi->getIndividualID());
									//}

									//branchingMergingProcRest->setRemainingNominalCreationCount(remainingNewNominalCreationCount-1);
								}
							}

							CBranchingMergingIndividualNodeCandidateLinker* mergeCandLinker = branchingMergingProcRest->takeNextMergingCandidateNodeLinker();
							CIndividualProcessNode* mergeCandIndiNode = mergeCandLinker->getMergingIndividualNodeCandidate();

							KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(mDebugIndiModelString = generateDebugIndiModelStringList(calcAlgContext));

							if ((branchingMergingProcRest->hasValidRemainingMergingCandidates() || processIndi->hasRoleSuccessorToIndividual(role,mergeCandIndiNode,true)) && !distinctMergedSet->contains(mergeCandIndiNode->getIndividualID())) {
								++deterministicMergingStep;

								CIndividualProcessNode* mergingIndiNode = getUpToDateIndividual(mergeCandIndiNode,calcAlgContext);

								// TODO: collect clashes and update distinct node set
								CClashedDependencyDescriptor* clashDescriptors = initClashDescriptors;
								initClashDescriptors = nullptr;
								clashDescriptors = createIndividualMergeCausingDescriptors(clashDescriptors,mergingIndiNode,mergeCandLinker->getMergingIndividualLink(),conceptOpLinkerIt,calcAlgContext);

								CPROCESSSET<cint64>* itDistinctMergedSet = distinctMergedSet;
								CPROCESSSET<cint64>::const_iterator disItEnd = distinctMergedSet->constEnd();
								CPROCESSSET<cint64>::const_iterator firstMergeableIt = disItEnd;
								CPROCESSSET<cint64>::const_iterator secondMergeableIt = disItEnd;

								cint64 firstContIndex = 0;
								cint64 secondContIndex = 0;


								cint64 contIndex = 0;
								for (CPROCESSSET<cint64>::const_iterator disIt = distinctMergedSet->constBegin(); disIt != disItEnd;) {
									++contIndex;
									cint64 distinctIndiID = *disIt;

									CIndividualLinkEdge* disIndiLink = processIndi->getRoleSuccessorToIndividualLink(role,distinctIndiID,true);
									//if (!disIndiLink) {
									//	mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
									//	QFile file(QString("./SATCT/Tasks/task-root-clashed.txt"));
									//	if (file.open(QIODevice::WriteOnly)) {
									//		file.write(mEndTaskDebugIndiModelString.replace("<br>","\r\n").toLocal8Bit());
									//		file.close();
									//	}
									//	bool bug = true;

									//}
									KONCLUDE_ASSERT_X(disIndiLink,"merging individual nodes","individual node link for distinct hashed individual node not found");
									if (disIndiLink) {
										CIndividualProcessNode* disIndiNode = getSuccessorIndividual(processIndi,disIndiLink,calcAlgContext);

										clashDescriptors = createIndividualMergeCausingDescriptors(clashDescriptors,disIndiNode,disIndiLink,conceptOpLinkerIt,calcAlgContext);

										if (isIndividualNodesMergeable(disIndiNode,mergingIndiNode,clashDescriptors,calcAlgContext)) {
											if (firstMergeableIt == disItEnd) {
												firstMergeableIt = disIt;
												firstContIndex = contIndex;
											} else {
												secondMergeableIt = disIt;
												secondContIndex = contIndex;
												break;
											}
										} 
										++disIt;
									} 							
								}

								//KONCLUCE_TASK_ALGORITHM_CLASH_STRING_INSTRUCTION(mMergingClashString = generateDebugTrackedClashedDescriptorString(createTrackedClashesDescriptors(clashDescriptors,calcAlgContext),calcAlgContext));

								CMERGEDependencyNode* mergeDependencyNode = createMERGEDependency(processIndi,nullptr,baseDepTrackPoint,calcAlgContext);
								if (mergeDependencyNode) {
									mergeDependencyNode->addBranchClashes(clashDescriptors);
								}


								//KONCLUCE_TASK_ALGORITHM_BRANCH_STRING_INSTRUCTION(mergeDependencyNode->getBranchNode()->setBranchString(new QString(QString("%1").arg(mergingIndiNode->getIndividualID()))));


								if (firstMergeableIt == disItEnd) {
									if (distinctCount < tmpCardinality) {
										++mAppliedATMOSTRuleCount;

										// add individual to distinct set
										if (!locDistinctMergedSet) {
											locDistinctMergedSet = branchingMergingProcRest->getDistinctMergedNodesSet(true);
											distinctMergedSet = locDistinctMergedSet;
										}
										CNonDeterministicDependencyTrackPoint* mergeNonDetDepTrackPoint = createNonDeterministicDependencyTrackPointBranch(mergeDependencyNode,true,calcAlgContext);
										if ((!mConfMinimizeMerging && !createNewNodesAsNominals) || mergingIndiNode->isNominalIndividual()) {
											distinctMergedSet->insert(mergingIndiNode->getIndividualID());

											branchingMergingProcRest->initMergingDependencyNode(mergeDependencyNode);
											branchingMergingProcRest->initDependencyTracker(mergeNonDetDepTrackPoint);
											baseDepTrackPoint = mergeNonDetDepTrackPoint;
										} else {
											CIndividualProcessNode* locMergingIndiNode = getLocalizedIndividual(mergingIndiNode,false,calcAlgContext);

											if (createNewNodesAsNominals) {
												branchingMergingProcRest->decRemainingNominalCreationCount();
											}

											CIndividualProcessNode* mergedIntoEmptyIndiNode = getIntoEmptyMergedIndividualNode(locMergingIndiNode,createNewNodesAsNominals,processIndi,mergeNonDetDepTrackPoint,calcAlgContext);
											distinctMergedSet->insert(mergedIntoEmptyIndiNode->getIndividualID());

											CRoleSuccessorLinkIterator roleSuccIt(processIndi->getRoleSuccessorHistoryLinkIterator(role,branchingMergingProcRest->getLastIndividualLink()));
											if (roleSuccIt.hasNext()) {
												CIndividualLinkEdge* link = roleSuccIt.next();
												branchingMergingProcRest->setLastIndividualLink(link);
											}
											addIndividualToProcessingQueue(mergedIntoEmptyIndiNode,calcAlgContext);

											KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(mDebugIndiModelString = generateDebugIndiModelStringList(calcAlgContext));

										}


										++distinctCount;
									} else {
										// clash, not able to merge 
										clashDescriptors = createClashedConceptDescriptor(clashDescriptors,processIndi,nullptr,baseDepTrackPoint,calcAlgContext);

										throw CCalculationClashProcessingException(clashDescriptors);
									}
								} else if (secondMergeableIt == disItEnd && distinctCount >= tmpCardinality) {
									++mAppliedATMOSTRuleCount;
									// only one possibility to merge
									CIndividualProcessNode* locDisIndiNode = getLocalizedIndividual(*firstMergeableIt,calcAlgContext);
									CIndividualProcessNode* locMergingIndiNode = getLocalizedIndividual(mergingIndiNode,false,calcAlgContext);

									CNonDeterministicDependencyTrackPoint* mergeNonDetDepTrackPoint = createNonDeterministicDependencyTrackPointBranch(mergeDependencyNode,true,calcAlgContext);

									CIndividualProcessNode* locMergedIndiNode = getMergedIndividualNodes(locDisIndiNode,locMergingIndiNode,mergeNonDetDepTrackPoint,calcAlgContext);
									if (locMergedIndiNode->getIndividualID() != locDisIndiNode->getIndividualID()) {
										if (!locDistinctMergedSet) {
											locDistinctMergedSet = branchingMergingProcRest->getDistinctMergedNodesSet(true);
											distinctMergedSet = locDistinctMergedSet;
										}
										distinctMergedSet->remove(locDisIndiNode->getIndividualID());
										distinctMergedSet->insert(locMergedIndiNode->getIndividualID());
									}

									if (calcAlgContext->getUsedUnsatisfiableCacheRetrievalStrategy()->testUnsatisfiableCacheForMergedIndividualNodes(conProDes,processIndi,locMergedIndiNode)) {
										testIndividualNodeUnsatisfiableCached(locMergedIndiNode,calcAlgContext);
									}


									CReapplyRoleSuccessorHash* roleSuccHash = processIndi->getReapplyRoleSuccessorHash(false);
									CRoleSuccessorLinkIterator roleSuccIt = roleSuccHash->getRoleSuccessorHistoryLinkIterator(role,branchingMergingProcRest->getLastIndividualLink(),&linkCount);
									if (roleSuccIt.hasNext()) {
										initializeMergingIndividualNodes(processIndi,conProDes,&roleSuccIt,nullptr,conceptOpLinkerIt,branchingMergingProcRest,calcAlgContext);
										qualifyMergingIndividualNodes(processIndi,conProDes,branchingMergingProcRest,calcAlgContext);
									}

								} else {
									++mAppliedATMOSTRuleCount;
									CSatisfiableCalculationTask* newTaskList = nullptr;

									if (firstMergeableIt != disItEnd) {
										CIndividualProcessNode* firstDistinctIndiNode = getUpToDateIndividual(*firstMergeableIt,calcAlgContext);
										CSatisfiableCalculationTask* newTask = createMergeBranchingTask(processIndi,conProDes,firstDistinctIndiNode,mergingIndiNode,mergeDependencyNode,branchingMergingProcRest,calcAlgContext);
										newTaskList = (CSatisfiableCalculationTask*)newTask->append(newTaskList);
									}
									if (secondMergeableIt != disItEnd) {
										CIndividualProcessNode* secondDistinctIndiNode = getUpToDateIndividual(*secondMergeableIt,calcAlgContext);
										CSatisfiableCalculationTask* newTask = createMergeBranchingTask(processIndi,conProDes,secondDistinctIndiNode,mergingIndiNode,mergeDependencyNode,branchingMergingProcRest,calcAlgContext);
										newTaskList = (CSatisfiableCalculationTask*)newTask->append(newTaskList);

										CPROCESSSET<cint64>::const_iterator mergeableIt = secondMergeableIt;
										while (++mergeableIt != disItEnd) {
											cint64 distinctIndiID = *mergeableIt;

											CIndividualLinkEdge* disIndiLink = processIndi->getRoleSuccessorToIndividualLink(role,distinctIndiID,true);
											KONCLUDE_ASSERT_X(disIndiLink,"merging individual nodes","individual node link for distinct hashed individual node not found");
											if (disIndiLink) {
												CIndividualProcessNode* distinctIndiNode = getSuccessorIndividual(processIndi,disIndiLink,calcAlgContext);

												CClashedDependencyDescriptor* moreClashDescriptors = createIndividualMergeCausingDescriptors(nullptr,distinctIndiNode,disIndiLink,conceptOpLinkerIt,calcAlgContext);

												if (isIndividualNodesMergeable(distinctIndiNode,mergingIndiNode,moreClashDescriptors,calcAlgContext)) {
													CSatisfiableCalculationTask* newTask = createMergeBranchingTask(processIndi,conProDes,distinctIndiNode,mergingIndiNode,mergeDependencyNode,branchingMergingProcRest,calcAlgContext);
													newTaskList = (CSatisfiableCalculationTask*)newTask->append(newTaskList);
												}

												if (mergeDependencyNode) {
													mergeDependencyNode->addBranchClashes(moreClashDescriptors);
												}
											}
										}
									}

									if (distinctCount < tmpCardinality) {
										CSatisfiableCalculationTask* newTask = createDistinctBranchingTask(processIndi,conProDes,mergingIndiNode,createNewNodesAsNominals,mergeDependencyNode,branchingMergingProcRest,calcAlgContext);
										newTaskList = (CSatisfiableCalculationTask*)newTask->append(newTaskList);
									}


									CTaskProcessorContext* processorContext = calcAlgContext->getUsedTaskProcessorContext();
									processorContext->getTaskProcessorCommunicator()->communicateTaskCreation(newTaskList);

									throw CCalculationStopProcessingException(true);
								}

							} else {
								// invalid link
								branchingMergingProcRest->incRemainingValidMergingCandidateIndividualNodeCount();
							}
						}

					}
					return false;
				}



				CSatisfiableCalculationTask* CCalculationTableauCompletionTaskHandleAlgorithm::createDistinctBranchingTask(CIndividualProcessNode*& processIndiNode, CConceptProcessDescriptor*& conProDes, CIndividualProcessNode*& distinctIndiNode, bool createAsNominal, CNonDeterministicDependencyNode* mergeDependencyNode, CBranchingMergingProcessingRestrictionSpecification* branchingMergingProcRest, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(TASKDISTINCTMERGEBRANCHCREATIONCOUNT,calcAlgContext);
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CRole* role = conDes->getConcept()->getRole();
					CSatisfiableCalculationTask* newSatCalcTask = createDependendBranchingTaskList(1,calcAlgContext);

					CTaskProcessorContext* processorContext = calcAlgContext->getUsedTaskProcessorContext();

					CProcessContext* newProcessContext = newSatCalcTask->getProcessContext(processorContext);

					CCalculationAlgorithmContextBase* newCalcAlgContext = createCalculationAlgorithmContext(processorContext,newProcessContext,newSatCalcTask);
					CProcessingDataBox* newProcessingDataBox = newSatCalcTask->getProcessingDataBox();

					CMemoryAllocationManager* newTaskMemMan = newCalcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					CNonDeterministicDependencyTrackPoint* mergeNonDetDepTrackPoint = createNonDeterministicDependencyTrackPointBranch(mergeDependencyNode,false,newCalcAlgContext);

					CBranchingMergingProcessingRestrictionSpecification* newBranchingMergingProcRest = CObjectParameterizingAllocator< CBranchingMergingProcessingRestrictionSpecification,CProcessContext* >::allocateAndConstructAndParameterize(newTaskMemMan,newProcessContext);
					newBranchingMergingProcRest->initBranchingMergingProcessingRestriction(branchingMergingProcRest);

					CProcessTagger* newProcessTagger = newCalcAlgContext->getUsedProcessTagger();
					newProcessTagger->incBranchingTag();
					newProcessTagger->incLocalizationTag();

					CIndividualProcessNode* newLocDistinctIndiNode = getLocalizedIndividual(distinctIndiNode,false,newCalcAlgContext);

					CIndividualProcessNode* locProcessIndiNode = getLocalizedIndividual(processIndiNode,false,newCalcAlgContext);

					// make distinct to all other 
					CPROCESSSET<cint64>* locDistinctMergedSet = newBranchingMergingProcRest->getDistinctMergedNodesSet(true);


					if ((mConfMinimizeMerging || createAsNominal) && !newLocDistinctIndiNode->isNominalIndividual()) {
						if (createAsNominal) {
							newBranchingMergingProcRest->decRemainingNominalCreationCount();
						}

						CIndividualProcessNode* mergedIntoEmptyIndiNode = getIntoEmptyMergedIndividualNode(newLocDistinctIndiNode,createAsNominal,processIndiNode,mergeNonDetDepTrackPoint,newCalcAlgContext);
						locDistinctMergedSet->insert(mergedIntoEmptyIndiNode->getIndividualID());

						CRoleSuccessorLinkIterator roleSuccIt(locProcessIndiNode->getRoleSuccessorHistoryLinkIterator(role,newBranchingMergingProcRest->getLastIndividualLink()));
						if (roleSuccIt.hasNext()) {
							CIndividualLinkEdge* link = roleSuccIt.next();
							newBranchingMergingProcRest->setLastIndividualLink(link);
						}
						addIndividualToProcessingQueue(mergedIntoEmptyIndiNode,newCalcAlgContext);

						KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(mDebugIndiModelString = generateDebugIndiModelStringList(newCalcAlgContext));

					} else {
						newBranchingMergingProcRest->initMergingDependencyNode(mergeDependencyNode);
						newBranchingMergingProcRest->initDependencyTracker(mergeNonDetDepTrackPoint);

						locDistinctMergedSet->insert(newLocDistinctIndiNode->getIndividualID());
					}


					// continue merging
					CConceptProcessingQueue* conProQueu = locProcessIndiNode->getConceptProcessingQueue(true);
					addConceptRestrictedToProcessingQueue(conDes,mergeNonDetDepTrackPoint,conProQueu,locProcessIndiNode,true,newBranchingMergingProcRest,newCalcAlgContext);

					prepareBranchedTaskProcessing(locProcessIndiNode,newSatCalcTask,newCalcAlgContext);

					KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(mDebugIndiModelString = generateDebugIndiModelStringList(newCalcAlgContext));

					// set new task priority
					double newTaskPriority = calcAlgContext->getUsedTaskPriorityStrategy()->getPriorityForTaskMerging(newSatCalcTask,calcAlgContext->getUsedSatisfiableCalculationTask());
					newSatCalcTask->setTaskPriority(newTaskPriority);

					return newSatCalcTask;
				}



				CSatisfiableCalculationTask* CCalculationTableauCompletionTaskHandleAlgorithm::createMergeBranchingTask(CIndividualProcessNode*& processIndiNode, CConceptProcessDescriptor*& conProDes, CIndividualProcessNode*& distinctIndiNode, CIndividualProcessNode*& mergingIndiNode, CNonDeterministicDependencyNode* mergeDependencyNode, CBranchingMergingProcessingRestrictionSpecification* branchingMergingProcRest, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(TASKINDINODEMERGEBRANCHCREATIONCOUNT,calcAlgContext);

					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CSatisfiableCalculationTask* newSatCalcTask = createDependendBranchingTaskList(1,calcAlgContext);

					CTaskProcessorContext* processorContext = calcAlgContext->getUsedTaskProcessorContext();

					CProcessContext* newProcessContext = newSatCalcTask->getProcessContext(processorContext);

					CCalculationAlgorithmContextBase* newCalcAlgContext = createCalculationAlgorithmContext(processorContext,newProcessContext,newSatCalcTask);
					CProcessingDataBox* newProcessingDataBox = newSatCalcTask->getProcessingDataBox();

					CMemoryAllocationManager* newTaskMemMan = newCalcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					CNonDeterministicDependencyTrackPoint* mergeNonDetDepTrackPoint = createNonDeterministicDependencyTrackPointBranch(mergeDependencyNode,false,newCalcAlgContext);

					CBranchingMergingProcessingRestrictionSpecification* newBranchingMergingProcRest = CObjectParameterizingAllocator< CBranchingMergingProcessingRestrictionSpecification,CProcessContext* >::allocateAndConstructAndParameterize(newTaskMemMan,newProcessContext);
					newBranchingMergingProcRest->initBranchingMergingProcessingRestriction(branchingMergingProcRest);

					CProcessTagger* newProcessTagger = newCalcAlgContext->getUsedProcessTagger();
					newProcessTagger->incBranchingTag();
					newProcessTagger->incLocalizationTag();

					CIndividualProcessNode* newLocDistinctIndiNode = getLocalizedIndividual(distinctIndiNode,false,newCalcAlgContext);
					CIndividualProcessNode* newLocMergingIndiNode = getLocalizedIndividual(mergingIndiNode,false,newCalcAlgContext);


					CIndividualProcessNode* locMergedIndiNode = getMergedIndividualNodes(newLocDistinctIndiNode,newLocMergingIndiNode,mergeNonDetDepTrackPoint,newCalcAlgContext);
					if (locMergedIndiNode->getIndividualID() != newLocDistinctIndiNode->getIndividualID()) {
						CPROCESSSET<cint64>* locDistinctMergedSet = newBranchingMergingProcRest->getDistinctMergedNodesSet(true);
						locDistinctMergedSet->remove(newLocDistinctIndiNode->getIndividualID());
						locDistinctMergedSet->insert(locMergedIndiNode->getIndividualID());
					}


					// continue merging
					CIndividualProcessNode* locProcessIndiNode = getLocalizedIndividual(processIndiNode,true,newCalcAlgContext);
					CConceptProcessingQueue* conProQueu = locProcessIndiNode->getConceptProcessingQueue(true);
					addConceptRestrictedToProcessingQueue(conDes,mergeNonDetDepTrackPoint,conProQueu,locProcessIndiNode,true,newBranchingMergingProcRest,newCalcAlgContext);


					if (calcAlgContext->getUsedUnsatisfiableCacheRetrievalStrategy()->testUnsatisfiableCacheForMergedIndividualNodes(conProDes,locProcessIndiNode,locMergedIndiNode)) {
						addIndividualNodeForCacheUnsatisfiableRetrieval(locMergedIndiNode,newCalcAlgContext);
					}

					prepareBranchedTaskProcessing(locProcessIndiNode,newSatCalcTask,newCalcAlgContext);

					KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(mDebugIndiModelString = generateDebugIndiModelStringList(newCalcAlgContext));

					// set new task priority
					double newTaskPriority = calcAlgContext->getUsedTaskPriorityStrategy()->getPriorityForTaskMerging(newSatCalcTask,calcAlgContext->getUsedSatisfiableCalculationTask());
					newSatCalcTask->setTaskPriority(newTaskPriority);

					return newSatCalcTask;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::qualifyMergingIndividualNodes(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, CBranchingMergingProcessingRestrictionSpecification* branchingMergingProcRest, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CProcessContext* processContext = calcAlgContext->getUsedProcessContext();
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					cint64 cardinality = concept->getParameter() - 1*conDes->isNegated();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();

					bool qualifing = false;

					CBranchingMergingIndividualNodeCandidateLinker* qualifyPosNegCandLinker = branchingMergingProcRest->getBothQualifyCandidateNodeLinker();
					while (qualifyPosNegCandLinker) {

						CIndividualProcessNode* qualifyIndiNode = qualifyPosNegCandLinker->getMergingIndividualNodeCandidate();

						// check still valid
						CIndividualLinkEdge* link = processIndi->getRoleSuccessorToIndividualLink(role,qualifyIndiNode,true);
						if (link) {
							CIndividualProcessNode* upQualifyIndiNode = getUpToDateIndividual(qualifyIndiNode,calcAlgContext);
							bool negated = false;
							if (containsIndividualNodeConcepts(upQualifyIndiNode,conceptOpLinkerIt,&negated,calcAlgContext)) {
								if (!negated) {
									if (cardinality <= 0) {
										// clash
									}
									CBranchingMergingIndividualNodeCandidateLinker* qualifiedMovedCandLinker = CObjectParameterizingAllocator< CBranchingMergingIndividualNodeCandidateLinker,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,calcAlgContext->getUsedProcessContext());
									qualifiedMovedCandLinker->initBranchingMergingIndividualNodeCandidate(qualifyPosNegCandLinker);
									branchingMergingProcRest->addMergingCandidateNodeLinker(qualifiedMovedCandLinker);
								}

							} else {
								CClashedDependencyDescriptor* clashDes = createClashedIndividualLinkDescriptor(nullptr,link,link->getDependencyTrackPoint(),calcAlgContext);

								// create dependency
								CQUALIFYDependencyNode* qualifyDepNode = createQUALIFYDependency(processIndi,nullptr,branchingMergingProcRest->getDependencyTrackPoint(),calcAlgContext);
								if (qualifyDepNode) {
									qualifyDepNode->addBranchClashes(clashDes);
								}
								branchingMergingProcRest->setBothQualifyCandidateNodeLinker(qualifyPosNegCandLinker->getNext());


								if (cardinality <= 0) {
									++mAppliedATMOSTRuleCount;
									STATINC(INDINODEQUALIFYCHOOCECOUNT,calcAlgContext);
									CDependencyTrackPoint* newDependencyTrackPoint = createNonDeterministicDependencyTrackPointBranch(qualifyDepNode,true,calcAlgContext);

									CIndividualProcessNode* locQualifyIndiNode =  getLocalizedIndividual(upQualifyIndiNode,false,calcAlgContext);
									// qualify only negated
									addConceptsToIndividual(conceptOpLinkerIt,true,locQualifyIndiNode,newDependencyTrackPoint,false,true,nullptr,calcAlgContext);
									addIndividualToProcessingQueue(locQualifyIndiNode,calcAlgContext);

									if (calcAlgContext->getUsedUnsatisfiableCacheRetrievalStrategy()->testUnsatisfiableCacheForQualifiedIndividualNodes(conProDes,processIndi,locQualifyIndiNode)) {
										addIndividualNodeForCacheUnsatisfiableRetrieval(locQualifyIndiNode,calcAlgContext);
									}


								} else {
									++mAppliedATMOSTRuleCount;
									STATINC(INDINODEQUALIFYCHOOCECOUNT,calcAlgContext);

									qualifing = true;


									CSatisfiableCalculationTask* newTaskList = createDependendBranchingTaskList(2,calcAlgContext);
									CTaskProcessorContext* processorContext = calcAlgContext->getUsedTaskProcessorContext();

									CSatisfiableCalculationTask* newTaskIt = newTaskList;
									cint64 branchNumber = 1;
									bool qualNeg = true;
									while (newTaskIt) {
										STATINC(TASKQUALIFYCHOOSEBRANCHCREATIONCOUNT,calcAlgContext);

										CSatisfiableCalculationTask* newSatCalcTask = newTaskIt;
										CProcessContext* newProcessContext = newSatCalcTask->getProcessContext(processorContext);

										CCalculationAlgorithmContextBase* newCalcAlgContext = createCalculationAlgorithmContext(processorContext,newProcessContext,newSatCalcTask);
										CProcessingDataBox* newProcessingDataBox = newSatCalcTask->getProcessingDataBox();

										CProcessTagger* newProcessTagger = newCalcAlgContext->getUsedProcessTagger();
										newProcessTagger->incBranchingTag();
										newProcessTagger->incLocalizationTag();

										CIndividualProcessNode* newLocIndiNode = getLocalizedIndividual(processIndi,false,newCalcAlgContext);
										CConceptProcessingQueue* newConProcQueue = newLocIndiNode->getConceptProcessingQueue(true);


										CIndividualProcessNode* newLocQualifyIndiNode =  getLocalizedIndividual(upQualifyIndiNode,false,newCalcAlgContext);


										// create dependency track point
										CDependencyTrackPoint* newDependencyTrackPoint = createNonDeterministicDependencyTrackPointBranch(qualifyDepNode,false,newCalcAlgContext);

										if (!qualNeg) {
											CMemoryAllocationManager* newTaskMemMan = newCalcAlgContext->getUsedProcessTaskMemoryAllocationManager();
											CBranchingMergingProcessingRestrictionSpecification* newBranchingMergingProcRest = CObjectParameterizingAllocator< CBranchingMergingProcessingRestrictionSpecification,CProcessContext* >::allocateAndConstructAndParameterize(newTaskMemMan,newProcessContext);
											newBranchingMergingProcRest->initBranchingMergingProcessingRestriction(branchingMergingProcRest);
											CBranchingMergingIndividualNodeCandidateLinker* qualifiedMovedCandLinker = CObjectParameterizingAllocator< CBranchingMergingIndividualNodeCandidateLinker,CProcessContext* >::allocateAndConstructAndParameterize(newTaskMemMan,newProcessContext);
											qualifiedMovedCandLinker->initBranchingMergingIndividualNodeCandidate(qualifyPosNegCandLinker);
											newBranchingMergingProcRest->addMergingCandidateNodeLinker(qualifiedMovedCandLinker);

											branchingMergingProcRest = newBranchingMergingProcRest;
										}

										// ATMOST reapplication in new tasks
										addConceptRestrictedToProcessingQueue(conDes,depTrackPoint,newConProcQueue,newLocIndiNode,true,branchingMergingProcRest,newCalcAlgContext);

										// qualify
										addConceptsToIndividual(conceptOpLinkerIt,qualNeg,newLocQualifyIndiNode,newDependencyTrackPoint,false,true,nullptr,newCalcAlgContext);
										addIndividualToProcessingQueue(newLocQualifyIndiNode,newCalcAlgContext);

										if (calcAlgContext->getUsedUnsatisfiableCacheRetrievalStrategy()->testUnsatisfiableCacheForQualifiedIndividualNodes(conProDes,newLocIndiNode,newLocQualifyIndiNode)) {
											addIndividualNodeForCacheUnsatisfiableRetrieval(newLocQualifyIndiNode,newCalcAlgContext);
										}

										prepareBranchedTaskProcessing(newLocIndiNode,newTaskIt,newCalcAlgContext);

										// set new task priority
										double newTaskPriority = calcAlgContext->getUsedTaskPriorityStrategy()->getPriorityForTaskQualifing(newSatCalcTask,calcAlgContext->getUsedSatisfiableCalculationTask(),qualNeg);
										newSatCalcTask->setTaskPriority(newTaskPriority);

										++branchNumber;
										qualNeg = !qualNeg;
										newTaskIt = (CSatisfiableCalculationTask*)newTaskIt->getNext();
									}

									processorContext->getTaskProcessorCommunicator()->communicateTaskCreation(newTaskList);

									throw CCalculationStopProcessingException(true);
								}
							}
						}

						qualifyPosNegCandLinker = qualifyPosNegCandLinker->getNext();
					}
					branchingMergingProcRest->setBothQualifyCandidateNodeLinker(nullptr);
					return qualifing;
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::initializeMergingIndividualNodes(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, CRoleSuccessorLinkIterator* roleSuccIt, CIndividualLinkEdge* usingLastLink, CSortedNegLinker<CConcept*>* conceptOpLinkerIt, CBranchingMergingProcessingRestrictionSpecification* branchingMergingProcRest, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CProcessContext* processContext = calcAlgContext->getUsedProcessContext();


					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					bool negate = conDes->getNegation();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					CConcept* concept = conDes->getConcept();

					cint64 cardinality = concept->getParameter() - 1*negate;

					CIndividualLinkEdge* firstLink = usingLastLink;
					CBranchingMergingIndividualNodeCandidateLinker* disIndiNodeCandLinker = nullptr;
					CBranchingMergingIndividualNodeCandidateLinker* maxDisIndiNodeCandLinker = nullptr;
					cint64 maxDisIndiNodeCandCount = 0;
					cint64 disIndiNodeCandCount = 0;
					bool collectMaxDistinctMergingNodes = true;
					bool canInitMerging = !branchingMergingProcRest->hasMergingInitializationCandidates();
					while (roleSuccIt->hasNext()) {
						STATINC(INDINODEMERGEINITCOUNT,calcAlgContext);
						CIndividualLinkEdge* link = roleSuccIt->next();
						CIndividualProcessNode* succIndi = getSuccessorIndividual(processIndi,link,calcAlgContext);

						CBranchingMergingIndividualNodeCandidateLinker* mergeIndiNodeCandLinker = CObjectParameterizingAllocator< CBranchingMergingIndividualNodeCandidateLinker,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,processContext);
						mergeIndiNodeCandLinker->initBranchingMergingIndividualNodeCandidate(succIndi,link);


						bool containsNegation = false;
						if (containsIndividualNodeConcepts(succIndi,conceptOpLinkerIt,&containsNegation,calcAlgContext)) {

							if (!containsNegation) {
								bool delayedCandAdded = false;
								CDistinctHash* disHash = succIndi->getDistinctHash(false);
								if (collectMaxDistinctMergingNodes && disHash) {
									if (disIndiNodeCandLinker) {
										bool distinctToAllPrev = true;
										CBranchingMergingIndividualNodeCandidateLinker* disIndiNodeCandLinkerIt = disIndiNodeCandLinker;
										while (disIndiNodeCandLinkerIt) {
											if (!disHash->isIndividualDistinct(disIndiNodeCandLinkerIt->getMergingIndividualNodeCandidate()->getIndividualID())) {
												distinctToAllPrev = false;
												break;
											}
											disIndiNodeCandLinkerIt = disIndiNodeCandLinkerIt->getNext();
										}
										if (distinctToAllPrev) {
											disIndiNodeCandLinker = mergeIndiNodeCandLinker->append(disIndiNodeCandLinker);
											++disIndiNodeCandCount;
											delayedCandAdded = true;
										} else {
											if (disIndiNodeCandCount > maxDisIndiNodeCandCount) {
												maxDisIndiNodeCandCount = disIndiNodeCandCount;
												branchingMergingProcRest->addMergingCandidateNodeLinker(maxDisIndiNodeCandLinker);
												maxDisIndiNodeCandLinker = disIndiNodeCandLinker;
											} else {
												branchingMergingProcRest->addMergingCandidateNodeLinker(disIndiNodeCandLinker);
											}
											disIndiNodeCandCount = 0;
											disIndiNodeCandLinker = nullptr;
										}
									}


									if (!disIndiNodeCandLinker) {
										if (disHash->getDistinctCount() >= maxDisIndiNodeCandCount) {
											disIndiNodeCandLinker = mergeIndiNodeCandLinker;
											disIndiNodeCandCount = 1;
											delayedCandAdded = true;
										}
									}
								}
								if (!delayedCandAdded) {
									branchingMergingProcRest->addMergingCandidateNodeLinker(mergeIndiNodeCandLinker);
								}
							}

						} else {
							// TODO: check cache, add to only pos or neg qualify candidate node linker
							branchingMergingProcRest->addBothQualifyCandidateNodeLinker(mergeIndiNodeCandLinker);
						}
						if (!firstLink) {
							firstLink = link;
						}
					}
					if (disIndiNodeCandLinker) {
						if (disIndiNodeCandCount > maxDisIndiNodeCandCount) {
							maxDisIndiNodeCandCount = disIndiNodeCandCount;
							branchingMergingProcRest->addMergingCandidateNodeLinker(maxDisIndiNodeCandLinker);
							maxDisIndiNodeCandLinker = disIndiNodeCandLinker;
						} else {
							branchingMergingProcRest->addMergingCandidateNodeLinker(disIndiNodeCandLinker);
						}
					}
					if (maxDisIndiNodeCandLinker) {
						cint64 disIndiCollCount = 0;
						if (canInitMerging || maxDisIndiNodeCandCount > cardinality) {
							CPROCESSINGSET<CDependencyTrackPoint*> distinctInitDepTrackPointSet(calcAlgContext->getUsedTaskProcessorContext());
							bool multipleDistinctInitClashes = false;
							CClashedDependencyDescriptor* distinctInitClashes = nullptr;
							CBranchingMergingIndividualNodeCandidateLinker* initMergingIndiNodeCandIt = maxDisIndiNodeCandLinker;
							while (initMergingIndiNodeCandIt && disIndiCollCount <= cardinality) {
								CIndividualProcessNode* indiNodeMergeCand = initMergingIndiNodeCandIt->getMergingIndividualNodeCandidate();

								CBranchingMergingIndividualNodeCandidateLinker* initMergingIndiNodeCandIt2 = initMergingIndiNodeCandIt->getNext();
								while (initMergingIndiNodeCandIt2) {
									CIndividualProcessNode* otherIndiNodeMergeCand = initMergingIndiNodeCandIt2->getMergingIndividualNodeCandidate();
									CDistinctEdge* disEdge = indiNodeMergeCand->getDistinctHash(false)->getIndividualDistinctEdge(otherIndiNodeMergeCand->getIndividualID());
									CDependencyTrackPoint* disEdgeDepTrackPoint = disEdge->getDependencyTrackPoint();
									if (!distinctInitDepTrackPointSet.contains(disEdgeDepTrackPoint)) {
										distinctInitDepTrackPointSet.insert(disEdgeDepTrackPoint);
										if (distinctInitClashes) {
											multipleDistinctInitClashes = true;
										}
										distinctInitClashes = createClashedIndividualDistinctDescriptor(distinctInitClashes,disEdge,disEdgeDepTrackPoint,calcAlgContext);
									}
									initMergingIndiNodeCandIt2 = initMergingIndiNodeCandIt2->getNext();
								}

								distinctInitClashes = createIndividualMergeCausingDescriptors(distinctInitClashes,indiNodeMergeCand,initMergingIndiNodeCandIt->getMergingIndividualLink(),conceptOpLinkerIt,calcAlgContext);
								++disIndiCollCount;

								initMergingIndiNodeCandIt = initMergingIndiNodeCandIt->getNext();
							}

							if (maxDisIndiNodeCandCount > cardinality) {
								distinctInitClashes = createClashedConceptDescriptor(distinctInitClashes,processIndi,conDes,depTrackPoint,calcAlgContext);
								throw CCalculationClashProcessingException(distinctInitClashes);
							}

							if (multipleDistinctInitClashes) {
								branchingMergingProcRest->setMultipleMergingNodesInitializationClashesDescriptors(distinctInitClashes);
							} else {
								branchingMergingProcRest->setMergingNodesInitializationClashesDescriptors(distinctInitClashes);
							}

							branchingMergingProcRest->addMergingInitializationCandidateNodeLinker(maxDisIndiNodeCandLinker);
						} else {
							branchingMergingProcRest->addMergingCandidateNodeLinker(maxDisIndiNodeCandLinker);
						}
					}



					if (firstLink) {
						branchingMergingProcRest->setLastIndividualLink(firstLink);

						//KONCLUCE_TASK_ALGORITHM_BRANCH_STRING_INSTRUCTION(mMergingQueueString = generateDebugMergingQueueString(branchingMergingProcRest,calcAlgContext));
					}

					for (cint64 i = 0; i < 2; ++i) {
						bool qualPosNeg = false;
						CBranchingMergingIndividualNodeCandidateLinker* qualifyPosNegCandLinker = nullptr;
						if (i == 0) {
							qualPosNeg = false;
							qualifyPosNegCandLinker = branchingMergingProcRest->getOnlyPosQualifyCandidateNodeLinker();
						} else {
							qualPosNeg = true;
							qualifyPosNegCandLinker = branchingMergingProcRest->getOnlyNegQualifyCandidateNodeLinker();
						}
						while (qualifyPosNegCandLinker) {
							STATINC(INDINODEQUALIFYCHOOCECOUNT,calcAlgContext);
							CIndividualProcessNode* qualifyIndiNode = qualifyPosNegCandLinker->getMergingIndividualNodeCandidate();
							CIndividualProcessNode* locQualifyIndiNode = getLocalizedIndividual(qualifyIndiNode,true,calcAlgContext);
							CDependencyTrackPoint* nextDepTrackPoint = nullptr;
							addConceptsToIndividual(conceptOpLinkerIt,qualPosNeg,locQualifyIndiNode,nextDepTrackPoint,true,true,nullptr,calcAlgContext);
							addIndividualToProcessingQueue(locQualifyIndiNode,calcAlgContext);
							if (!qualPosNeg) {
								branchingMergingProcRest->addMergingCandidateNodeLinker(qualifyPosNegCandLinker);
							}
							qualifyPosNegCandLinker = qualifyPosNegCandLinker->getNext();
						}
					}
					branchingMergingProcRest->clearOnlyNegQualifyCandidateNodeLinker();
					branchingMergingProcRest->clearOnlyPosQualifyCandidateNodeLinker();

				}




				void CCalculationTableauCompletionTaskHandleAlgorithm::applyATLEASTRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(ATLEASTRULEAPPLICATIONCOUNT,calcAlgContext);
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();

					cint64 cardinality = concept->getParameter() + 1*negate;


					if (cardinality <= 0) {
						return;
					} else if (cardinality == 1) {
						applySOMERule(processIndi,conProDes,false,calcAlgContext);
					}


					if (mConfSatExpCachedSuccAbsorp && processIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATISFIABLECACHED | CIndividualProcessNode::PRFSIGNATUREBLOCKINGCACHED | CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHED | CIndividualProcessNode::PRFSATURATIONSUCCESSORCREATIONBLOCKINGCACHED)) {
						STATINC(SATCACHEDABSORBEDGENERATINGCONCEPTSCOUNT,calcAlgContext);
						return addSatisfiableCachedAbsorbedGeneratingConcept(conDes,processIndi,depTrackPoint,calcAlgContext);
					}



					bool alreadyExistSuitableSuccessors = hasDistinctRoleSuccessorConcepts(processIndi,role,conceptOpLinkerIt,false,cardinality,calcAlgContext);
					if (!alreadyExistSuitableSuccessors) {

						if (mConfAtleastAtmostFastClashCheck) {
							CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(false);
							if (conSet) {
								CConceptDescriptor* conDesIt = conSet->getAddingSortedConceptDescriptionLinker();
								while (conDesIt) {
									CConcept* atmostConcept = conDesIt->getData();
									bool conNegated = conDesIt->isNegated();
									cint64 opCode = atmostConcept->getOperatorCode();
									if (!conNegated && (opCode == CCATMOST) || conNegated && (opCode == CCATLEAST)) {
										cint64 param = atmostConcept->getParameter();
										cint64 atmostCardinality = param - 1*conNegated;

										if (atmostCardinality < cardinality && (hasIdenticalConceptOperands(atmostConcept->getOperandList(),conceptOpLinkerIt) || !atmostConcept->getOperandList())) {
											CRole* atmostRole = atmostConcept->getRole();


											CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList();
											while (superRoleIt) {
												if (!superRoleIt->isNegated()) {
													if (superRoleIt->getData() == atmostRole) {
														CClashedDependencyDescriptor* clashConDesLinker = nullptr;
														clashConDesLinker = createClashedConceptDescriptor(clashConDesLinker,processIndi,conDes,depTrackPoint,calcAlgContext);
														clashConDesLinker = createClashedConceptDescriptor(clashConDesLinker,processIndi,conDesIt,conDesIt->getDependencyTrackPoint(),calcAlgContext);
														throw CCalculationClashProcessingException(clashConDesLinker);
													}
												}
												superRoleIt = superRoleIt->getNext();
											}
										}
									}
									conDesIt = conDesIt->getNext();
								}
							}
						}

						if (calcAlgContext->getUsedUnsatisfiableCacheRetrievalStrategy()->testUnsatisfiableCacheForSuccessorGeneration(conProDes,processIndi)) {
							testIndividualNodeUnsatisfiableCached(processIndi,calcAlgContext);
						}

						++mAppliedATLEASTRuleCount;

						// create dependency
						CDependencyTrackPoint* nextDepTrackPoint = nullptr;
						CATLEASTDependencyNode* atleastDepNode = createATLEASTDependency(nextDepTrackPoint,processIndi,conDes,depTrackPoint,calcAlgContext);

						CPROCESSINGLIST<CIndividualProcessNode*> indiList(calcAlgContext->getUsedTaskProcessorContext());
						createDistinctSuccessorIndividuals(processIndi,indiList,role->getIndirectSuperRoleList(),role,conceptOpLinkerIt,false,nextDepTrackPoint,cardinality,calcAlgContext);

						for (CPROCESSINGLIST<CIndividualProcessNode*>::const_iterator it = indiList.constBegin(), itEnd = indiList.constEnd(); it != itEnd; ++it) {
							STATINC(DISTINCTSUCCESSORINDINODECREATIONCOUNT,calcAlgContext);
							CIndividualProcessNode* succIndi = *it;
							if (processIndi->isNominalIndividual() && processIndi->getIndividualNominalLevel() <= 0) {
								succIndi->setExtendedQueueProcessing(true);
							}
							addIndividualToProcessingQueue(succIndi,calcAlgContext);
						}
					}
				}








				void CCalculationTableauCompletionTaskHandleAlgorithm::applyNOMINALRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(NOMINALRULEAPPLICATIONCOUNT,calcAlgContext);
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CIndividual* indi = concept->getNominalIndividual();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();


					CIndividualProcessNode* nominalNode = getCorrectedNominalIndividualNode(indi->getIndividualID(),calcAlgContext);
					if (nominalNode->getIndividualID() != processIndi->getIndividualID()) {
						CIndividualProcessNode* locNominalNode = getLocalizedIndividual(nominalNode,false,calcAlgContext);
						if (!negate) {

							// force initialized
							locNominalNode = getForcedInitializedNominalIndividualNode(locNominalNode,calcAlgContext);
							CReapplyConceptLabelSet* locNominalNodeConSet = locNominalNode->getReapplyConceptLabelSet(true);
							CConcept* nominalConcept = indi->getIndividualNominalConcept();
							CConceptDescriptor* nominalConDes = nullptr;
							CDependencyTrackPoint* nominalConDepTrackPoint = nullptr;
							if (indi->getIndividualID() != locNominalNode->getIndividualID()) {
								locNominalNodeConSet->getConceptDescriptor(nominalConcept,nominalConDes,nominalConDepTrackPoint);
							}

							STATINC(INDINODENOMINALMERGECOUNT,calcAlgContext);
							CClashedDependencyDescriptor* clashDescriptors = nullptr;
							// test if merging is possible
							if (isIndividualNodesMergeable(processIndi,locNominalNode,clashDescriptors,calcAlgContext)) {

								// create dependency
								CDependencyTrackPoint* nextDepTrackPoint = nullptr;
								CNOMINALDependencyNode* nominalDepNode = createNOMINALDependency(nextDepTrackPoint,processIndi,conDes,depTrackPoint,nominalConDepTrackPoint,calcAlgContext);

								CIndividualProcessNode* mergedNode = getMergedIndividualNodes(processIndi,locNominalNode,nextDepTrackPoint,calcAlgContext);

								propagateIndividualNodeNeighboursNominalConnectionToAncestors(mergedNode,calcAlgContext);
							} else {
								// clash
								clashDescriptors = createClashedConceptDescriptor(clashDescriptors,processIndi,conDes,depTrackPoint,calcAlgContext);
								if (nominalConDes) {
									clashDescriptors = createClashedConceptDescriptor(clashDescriptors,locNominalNode,nominalConDes,nominalConDepTrackPoint,calcAlgContext);
								}
								throw CCalculationClashProcessingException(clashDescriptors);
							}
						}
					}
				}



				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::getForcedInitializedNominalIndividualNode(CIndividualProcessNode* indi, CCalculationAlgorithmContextBase* calcAlgContext) {
					++mCurrentRecProcDepth;
					CIndividualProcessNode* nominalIndividual = indi;
					initialNodeInitialize(nominalIndividual,false,calcAlgContext);
					--mCurrentRecProcDepth;
					return nominalIndividual;
				}



				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::getCorrectedMergedIntoIndividualNode(CIndividualProcessNode* indi, CCalculationAlgorithmContextBase* calcAlgContext) {
					while (indi->hasMergedIntoIndividualNodeID()) {
						cint64 mergedIntoID = indi->getMergedIntoIndividualNodeID();
						indi = getUpToDateIndividual(mergedIntoID,calcAlgContext);
					}
					return indi;
				}



				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::getCorrectedNominalIndividualNode(cint64 indiID, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualProcessNode* indi = getUpToDateIndividual(indiID,calcAlgContext);
					if (indi) {
						while (indi->hasMergedIntoIndividualNodeID()) {
							STATINC(INDINODENOMINALCORRECTIDCOUNT,calcAlgContext);
							cint64 mergedIntoID = indi->getMergedIntoIndividualNodeID();
							indi = getUpToDateIndividual(mergedIntoID,calcAlgContext);
							// TODO: path compression? -> update merged into IDs
						}
					} else {
						// throw error
						throw CCalculationErrorProcessingException::getNominalMissingErrorException();
					}
					return indi;
				}




				void CCalculationTableauCompletionTaskHandleAlgorithm::applyALLRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(ALLRULEAPPLICATIONCOUNT,calcAlgContext);
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					CSortedNegLinker<CConcept*>* conceptOpLinker = concept->getOperandList();


					CIndividualLinkEdge* restLink = getLinkProcessingRestriction(conProDes);
					if (restLink) {
						++mAppliedALLRuleCount;
						STATINC(ALLROLERESTRICTIONCOUNT,calcAlgContext);
						CIndividualProcessNode* succIndi = getSuccessorIndividual(processIndi,restLink,calcAlgContext);
						CIndividualProcessNode* locSuccIndi = nullptr;
						CReapplyConceptLabelSet* conLabelSet = succIndi->getReapplyConceptLabelSet(false);

						// create dependency
						CDependencyTrackPoint* nextDepTrackPoint = nullptr;
						CALLDependencyNode* allDepNode = nullptr;
						bool allDepNodeCreated = false;

						CSortedNegLinker<CConcept*>* conceptOpLinkerIt = conceptOpLinker;
						while (conceptOpLinkerIt) {
							CConcept* opConcept = conceptOpLinkerIt->getData();
							bool opConNeg = conceptOpLinkerIt->isNegated() ^ negate;
							if (!conLabelSet->hasConcept(opConcept,opConNeg)) {
								// add concept
								if (!allDepNodeCreated) {
									allDepNodeCreated = true;
									allDepNode = createALLDependency(nextDepTrackPoint,processIndi,conDes,depTrackPoint,restLink->getDependencyTrackPoint(),calcAlgContext);
								}
								if (!locSuccIndi) {
									locSuccIndi = getLocalizedIndividual(succIndi,false,calcAlgContext);
									conLabelSet = locSuccIndi->getReapplyConceptLabelSet(true);
								}
								addConceptToIndividual(opConcept,opConNeg,locSuccIndi,nextDepTrackPoint,true,true,calcAlgContext);
							}
							conceptOpLinkerIt = conceptOpLinkerIt->getNext();
						}
						if (locSuccIndi) {
							addIndividualToProcessingQueue(locSuccIndi,calcAlgContext);
						}
					} else {
						CReapplyRoleSuccessorHash* roleSuccHash = processIndi->getReapplyRoleSuccessorHash(false);
						if (roleSuccHash) {
							CRoleSuccessorLinkIterator roleSuccIt = roleSuccHash->getRoleSuccessorLinkIterator(role);
							while (roleSuccIt.hasNext()) {
								++mAppliedALLRuleCount;
								STATINC(ALLROLERESTRICTIONCOUNT,calcAlgContext);
								CIndividualLinkEdge* link = roleSuccIt.next(true);
								CIndividualProcessNode* succIndi = getSuccessorIndividual(processIndi,link,calcAlgContext);
								CIndividualProcessNode* locSuccIndi = nullptr;
								CReapplyConceptLabelSet* conLabelSet = succIndi->getReapplyConceptLabelSet(false);

								// create dependency
								CDependencyTrackPoint* nextDepTrackPoint = nullptr;
								CALLDependencyNode* allDepNode = nullptr;
								bool allDepNodeCreated = false;

								CSortedNegLinker<CConcept*>* conceptOpLinkerIt = conceptOpLinker;
								while (conceptOpLinkerIt) {
									CConcept* opConcept = conceptOpLinkerIt->getData();
									bool opConNeg = conceptOpLinkerIt->isNegated() ^ negate;
									if (!conLabelSet->hasConcept(opConcept,opConNeg)) {
										// add concept
										if (!allDepNodeCreated) {
											allDepNodeCreated = true;
											allDepNode = createALLDependency(nextDepTrackPoint,processIndi,conDes,depTrackPoint,link->getDependencyTrackPoint(),calcAlgContext);
										}
										if (!locSuccIndi) {
											locSuccIndi = getLocalizedIndividual(succIndi,false,calcAlgContext);
											conLabelSet = locSuccIndi->getReapplyConceptLabelSet(true);
										}
										addConceptToIndividual(opConcept,opConNeg,locSuccIndi,nextDepTrackPoint,true,true,calcAlgContext);
									}
									conceptOpLinkerIt = conceptOpLinkerIt->getNext();
								}
								if (locSuccIndi) {
									addIndividualToProcessingQueue(locSuccIndi,calcAlgContext);
								}
							}
						}
					}
					if (!conProDes->isConceptReapplied()) {
						addConceptToReapplyQueue(conDes,role,processIndi,true,depTrackPoint,calcAlgContext);
					}
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::initializeORProcessing(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CBranchingORProcessingRestrictionSpecification** plannedBranchingProcessRestriction, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();

					if (conProDes->getProcessPriority().getPriority() >= mDeterministicProcessPriority) {
						CConceptProcessData* conProData = (CConceptProcessData*)concept->getConceptData();
						if (conProData) {
							CConcept* impConcept = nullptr;
							CReplacementData* repData = conProData->getReplacementData();
							CXNegLinker<CConcept*>* commDisConLinker = nullptr;
							if (repData) {
								impConcept = repData->getImplicationReplacementConcept();
								commDisConLinker = repData->getCommonDisjunctConceptLinker();
							}
							if (commDisConLinker) {
								addConceptsToIndividual(commDisConLinker,false,processIndi,depTrackPoint,true,false,nullptr,calcAlgContext);
							}
							if (impConcept) {
								STATINC(ORREPLACEMENTIMPLICATIONCOUNT,calcAlgContext);
								addConceptToIndividual(impConcept,false,processIndi,depTrackPoint,true,false,calcAlgContext);
								return true;
							}
						}

						CConceptProcessingQueue* conProQueue = processIndi->getConceptProcessingQueue(true);
						double priorityOffset = calcAlgContext->getUsedConceptPriorityStrategy()->getPriorityOffsetForDisjunctionDelayedConsidering(conDes,processIndi);
						addConceptRestrictedToProcessingQueue(conDes,depTrackPoint,conProQueue,processIndi,true,nullptr,priorityOffset,calcAlgContext);
						return true;
					}
					return false;
				}







				bool CCalculationTableauCompletionTaskHandleAlgorithm::hasSaturatedClashedFlagForConcept(CConcept* concept, bool negation, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptData* conceptData = concept->getConceptData();
					CIndividualSaturationProcessNode* saturationIndiNode = nullptr;
					if (conceptData) {
						CConceptProcessData* conProcData = (CConceptProcessData*)conceptData;
						CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
						if (conRefLinking) {
							CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
							CSaturationConceptReferenceLinking* satCalcRefLinkData = confSatRefLinkingData->getConceptSaturationReferenceLinkingData(negation);
							if (satCalcRefLinkData) {
								saturationIndiNode = (CIndividualSaturationProcessNode*)satCalcRefLinkData->getIndividualProcessNodeForConcept();
							}
						}
					}

					if (saturationIndiNode) {
						if (saturationIndiNode->getIndirectStatusFlags()->hasClashedFlag()) {
							return true;
						}
					}
					return false;
				}






				bool CCalculationTableauCompletionTaskHandleAlgorithm::planORProcessing(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CBranchingORProcessingRestrictionSpecification** plannedBranchingProcessRestriction, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					CProcessingRestrictionSpecification* procRest = conProDes->getProcessingRestrictionSpecification();


					if (initializeORProcessing(processIndi,conProDes,negate,plannedBranchingProcessRestriction,calcAlgContext)) {
						return true;
					}

					// plan OR processing
					if (!procRest) {
						STATINC(OREXECUTIONINITCOUNT,calcAlgContext);
						// concept is processed first time
						CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(false);

						CSortedNegLinker<CConcept*>* firstNotPosAndNegContainedOperand = nullptr;
						CSortedNegLinker<CConcept*>* secondNotPosAndNegContainedOperand = nullptr;
						CSortedNegLinker<CConcept*>* containedOperand = nullptr;

						CClashedDependencyDescriptor* clashConDesLinker = nullptr;

						CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
						while (opLinker && !containedOperand) {
							CConcept* opConcept = opLinker->getData();
							bool opConNegation = opLinker->isNegated() ^ negate;
							bool containsNegation = false;

							CConceptDescriptor* containedConDes = nullptr;
							CDependencyTrackPoint* containedConDepTrackPoint = nullptr;
							bool contains = conSet->getConceptDescriptor(opConcept,containedConDes,containedConDepTrackPoint);
							if (!contains) {
								if (!hasSaturatedClashedFlagForConcept(opConcept,opConNegation,calcAlgContext)) {
									if (!firstNotPosAndNegContainedOperand) {
										firstNotPosAndNegContainedOperand = opLinker;
									} else if (!secondNotPosAndNegContainedOperand) {
										secondNotPosAndNegContainedOperand = opLinker;
									}
								}
							} else {
								containsNegation = containedConDes->isNegated();
								if (containsNegation == opConNegation) {									
									containedOperand = opLinker;
								} else {
									clashConDesLinker = createClashedConceptDescriptor(clashConDesLinker,processIndi,containedConDes,containedConDepTrackPoint,calcAlgContext);
								}
							}
							opLinker = opLinker->getNext();
						}


						if (containedOperand || !secondNotPosAndNegContainedOperand) {
							STATINC(ORINSTANTCONTAINSEXECUTEDCOUNT,calcAlgContext);
							// only one branch is applicable, execute OR rule
							if (plannedBranchingProcessRestriction) {
								CBranchingORProcessingRestrictionSpecification* branchORProcRest = CObjectAllocator< CBranchingORProcessingRestrictionSpecification >::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
								branchORProcRest->initBranchingORProcessingRestriction();
								branchORProcRest->setContainedOperand(containedOperand);
								branchORProcRest->setFirstNotPosAndNegContainedOperand(firstNotPosAndNegContainedOperand);
								branchORProcRest->setSecondNotPosAndNegContainedOperand(secondNotPosAndNegContainedOperand);
								branchORProcRest->addClashedConceptDescriptors(clashConDesLinker);
								*plannedBranchingProcessRestriction = branchORProcRest;
							}
							return false;
						} else {
							// try to trigger branching

							CConceptProcessData* conProcessData = (CConceptProcessData*)concept->getConceptData();
							CConceptRoleBranchingTrigger* conRoleBranchTriggerLinkerIt = nullptr;
							CConceptRoleBranchingTrigger* nextConRoleBranchTriggerLinkerIt = nullptr;
							CBranchingORProcessingRestrictionSpecification* branchORProcRest = CObjectAllocator< CBranchingORProcessingRestrictionSpecification >::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
							branchORProcRest->initBranchingORProcessingRestriction();
							branchORProcRest->setFirstNotPosAndNegContainedOperand(firstNotPosAndNegContainedOperand);
							branchORProcRest->setSecondNotPosAndNegContainedOperand(secondNotPosAndNegContainedOperand);
							branchORProcRest->addClashedConceptDescriptors(clashConDesLinker);
							if (conProcessData) {
								if (mConfBranchTriggering) {
									conRoleBranchTriggerLinkerIt = conProcessData->getConceptRoleBranchTrigger();
								}
							}
							if (conRoleBranchTriggerLinkerIt) {
								// search trigger
								STATINC(ORTRIGGERSEARCHCOUNT,calcAlgContext);
								conRoleBranchTriggerLinkerIt = searchNextConceptRoleBranchTrigger(processIndi,conRoleBranchTriggerLinkerIt,calcAlgContext);
							}
							if (conRoleBranchTriggerLinkerIt) {
								// install trigger
								STATINC(ORTRIGGEREDDELAYEDEXECUTEDCOUNT,calcAlgContext);
								nextConRoleBranchTriggerLinkerIt = conRoleBranchTriggerLinkerIt->getNextBranchingTrigger();
								branchORProcRest->setConceptRoleBranchingTrigger(nextConRoleBranchTriggerLinkerIt);
								installConceptRoleBranchTrigger(processIndi,conDes,depTrackPoint,branchORProcRest,conRoleBranchTriggerLinkerIt,calcAlgContext);
								return true;
							} else {
								// branch triggering is not anymore possible, determine priority for this OR branch
								// determine additional OR priority offset
								STATINC(ORQUEUEDDELAYEDEXECUTEDCOUNT,calcAlgContext);
								double priorityOffset = calcAlgContext->getUsedConceptPriorityStrategy()->getPriorityOffsetForDisjunctionDelayedProcessing(conDes,processIndi);
								branchORProcRest->setPriorityOffset(priorityOffset);
								CConceptProcessingQueue* conProQueue = processIndi->getConceptProcessingQueue(true);
								addConceptRestrictedToProcessingQueue(conDes,depTrackPoint,conProQueue,processIndi,true,branchORProcRest,priorityOffset,calcAlgContext);
								return true;
							}
						} 

					} else {
						STATINC(OREXECUTIONCONTINUECOUNT,calcAlgContext);
						// OR branching concept has already been considered before
						// try triggering again
						CBranchingORProcessingRestrictionSpecification* branchORProcRest = (CBranchingORProcessingRestrictionSpecification*)procRest;
						if (branchORProcRest->getContainedOperand()) {
							*plannedBranchingProcessRestriction = branchORProcRest;
							return false;
						}
						CConceptRoleBranchingTrigger* conRoleBranchTriggerLinkerIt = branchORProcRest->getConceptRoleBranchingTrigger();
						CConceptRoleBranchingTrigger* nextConRoleBranchTriggerLinkerIt = nullptr;
						if (!conRoleBranchTriggerLinkerIt) {
							// OR branch was not triggered, execute OR rule
							if (plannedBranchingProcessRestriction) {
								*plannedBranchingProcessRestriction = branchORProcRest;
							}
							return false;
						} else {
							// search trigger
							STATINC(ORTRIGGERSEARCHCOUNT,calcAlgContext);
							conRoleBranchTriggerLinkerIt = searchNextConceptRoleBranchTrigger(processIndi,conRoleBranchTriggerLinkerIt,calcAlgContext);
						}
						CBranchingORProcessingRestrictionSpecification* nextBranchORProcRest = CObjectAllocator< CBranchingORProcessingRestrictionSpecification >::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
						nextBranchORProcRest->initBranchingORProcessingRestriction(branchORProcRest);
						if (conRoleBranchTriggerLinkerIt) {
							// install trigger
							STATINC(ORTRIGGEREDDELAYEDEXECUTEDCOUNT,calcAlgContext);
							nextConRoleBranchTriggerLinkerIt = conRoleBranchTriggerLinkerIt->getNextBranchingTrigger();
							nextBranchORProcRest->setConceptRoleBranchingTrigger(nextConRoleBranchTriggerLinkerIt);
							installConceptRoleBranchTrigger(processIndi,conDes,depTrackPoint,nextBranchORProcRest,conRoleBranchTriggerLinkerIt,calcAlgContext);
							return true;
						} else {
							STATINC(ORQUEUEDDELAYEDEXECUTEDCOUNT,calcAlgContext);
							double priorityOffset = calcAlgContext->getUsedConceptPriorityStrategy()->getPriorityOffsetForDisjunctionDelayedProcessing(conDes,processIndi);
							nextBranchORProcRest->setPriorityOffset(priorityOffset);
							nextBranchORProcRest->setConceptRoleBranchingTrigger(nullptr);
							CConceptProcessingQueue* conProQueue = processIndi->getConceptProcessingQueue(true);
							addConceptRestrictedToProcessingQueue(conDes,depTrackPoint,conProQueue,processIndi,true,nextBranchORProcRest,priorityOffset,calcAlgContext);
							return true;
						}
					}
				}




				CNonDeterministicDependencyTrackPoint* CCalculationTableauCompletionTaskHandleAlgorithm::createNonDeterministicDependencyTrackPointBranch(CNonDeterministicDependencyNode* dependencyNode, bool singleBranch, CCalculationAlgorithmContextBase* calcAlgContext) {
					CNonDeterministicDependencyTrackPoint* nonDepTrackPoint = nullptr;
					if (mConfBuildDependencies && dependencyNode) {
						if (singleBranch) {
							CBranchTreeNode* branchNode = calcAlgContext->getNewBranchTreeNode();
							nonDepTrackPoint = dependencyNode->getDependencyTrackPointBranch();
							branchNode->branchingIncrement(nonDepTrackPoint);
							nonDepTrackPoint->initBranch(branchNode);
						} else {
							CBranchTreeNode* branchNode = calcAlgContext->getUsedBranchTreeNode();
							nonDepTrackPoint = dependencyNode->getDependencyTrackPointBranch();
							branchNode->branchingIncrement(nonDepTrackPoint);
							nonDepTrackPoint->initBranch(branchNode);
						}
					}
					return nonDepTrackPoint;
				}




				CClashedDependencyDescriptor* CCalculationTableauCompletionTaskHandleAlgorithm::createIndividualMergeCausingDescriptors(CClashedDependencyDescriptor* prevClashes, CIndividualProcessNode*& processIndi, CIndividualLinkEdge* link, CSortedNegLinker<CConcept*>* conceptAddLinker, CCalculationAlgorithmContextBase* calcAlgContext) {
					CClashedDependencyDescriptor* clashDes = prevClashes;
					
					if (link->getDependencyTrackPoint() != processIndi->getDependencyTrackPoint()) {
						clashDes = createClashedIndividualLinkDescriptor(clashDes,link,link->getDependencyTrackPoint(),calcAlgContext);
					}

					CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(false);

					CSortedNegLinker<CConcept*>* conceptAddLinkerIt = conceptAddLinker;
					while (conceptAddLinkerIt) {
						CConcept* concept = conceptAddLinkerIt->getData();
						bool conNegation = conceptAddLinkerIt->isNegated();

						CConceptDescriptor* containedConDes = nullptr;
						CDependencyTrackPoint* containedDepTrackPoint = nullptr;
						conSet->getConceptDescriptor(concept,containedConDes,containedDepTrackPoint);

						clashDes = createClashedConceptDescriptor(clashDes,processIndi,containedConDes,containedDepTrackPoint,calcAlgContext);

						conceptAddLinkerIt = conceptAddLinkerIt->getNext();
					}
					return clashDes;
				}



				CClashedDependencyDescriptor* CCalculationTableauCompletionTaskHandleAlgorithm::createClashedConceptDescriptor(CClashedDependencyDescriptor* prevClashes, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CClashedDependencyDescriptor* clashDes = calcAlgContext->getClashDescriptorFactory()->createClashedConceptDescriptor(prevClashes,processIndi,conDes,prevDepTrackPoint,calcAlgContext);
					return clashDes;
				}

				CClashedDependencyDescriptor* CCalculationTableauCompletionTaskHandleAlgorithm::createClashedIndividualLinkDescriptor(CClashedDependencyDescriptor* prevClashes, CIndividualLinkEdge* link, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CClashedDependencyDescriptor* clashDes = calcAlgContext->getClashDescriptorFactory()->createClashedIndividualLinkDescriptor(prevClashes,link,prevDepTrackPoint,calcAlgContext);
					return clashDes;
				}

				CClashedDependencyDescriptor* CCalculationTableauCompletionTaskHandleAlgorithm::createClashedIndividualDistinctDescriptor(CClashedDependencyDescriptor* prevClashes, CDistinctEdge* distinct, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CClashedDependencyDescriptor* clashDes = calcAlgContext->getClashDescriptorFactory()->createClashedIndividualDistinctDescriptor(prevClashes,distinct,prevDepTrackPoint,calcAlgContext);
					return clashDes;
				}

				CClashedDependencyDescriptor* CCalculationTableauCompletionTaskHandleAlgorithm::createClashedNegationDisjointDescriptor(CClashedDependencyDescriptor* prevClashes, CNegationDisjointEdge* disjointNegLink, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CClashedDependencyDescriptor* clashDes = calcAlgContext->getClashDescriptorFactory()->createClashedNegationDisjointDescriptor(prevClashes,disjointNegLink,prevDepTrackPoint,calcAlgContext);
					return clashDes;
				}





				void CCalculationTableauCompletionTaskHandleAlgorithm::executeORBranching(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CBranchingORProcessingRestrictionSpecification* plannedBranchingProcessRestriction, CCalculationAlgorithmContextBase* calcAlgContext) {
					++mAppliedORRuleCount;
					STATINC(OREXECUTIONCOUNT,calcAlgContext);
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(false);

					CSortedNegLinker<CConcept*>* containedOperand = nullptr;
					CSortedNegLinker<CConcept*>* firstNotPosAndNegContainedOperand = nullptr;
					CSortedNegLinker<CConcept*>* secondNotPosAndNegContainedOperand = nullptr;

					CPROCESSINGLIST< CSortedNegLinker<CConcept*>* > notContainedOperandsList(calcAlgContext->getUsedTaskProcessorContext());

					cint64 notPosAndNegContainedOperandCount = 0;
					if (plannedBranchingProcessRestriction) {
						containedOperand = plannedBranchingProcessRestriction->getContainedOperand();
						firstNotPosAndNegContainedOperand = plannedBranchingProcessRestriction->getFirstNotPosAndNegContainedOperand();
						secondNotPosAndNegContainedOperand = plannedBranchingProcessRestriction->getSecondNotPosAndNegContainedOperand();
					}

					CClashedDependencyDescriptor* clashConDesLinker = plannedBranchingProcessRestriction->getClashedConceptDescriptors();

					if (!containedOperand) {
						// check if one operand is already in the concept set
						if (firstNotPosAndNegContainedOperand) {
							bool containsNegation = false;
							bool correctNegation = firstNotPosAndNegContainedOperand->isNegated() ^ negate;
							CConceptDescriptor* containedConDes = nullptr;
							CDependencyTrackPoint* containedConDepTrackPoint = nullptr;
							bool contains = conSet->getConceptDescriptor(firstNotPosAndNegContainedOperand->getData(),containedConDes,containedConDepTrackPoint);
							if (contains) {
								containsNegation = containedConDes->isNegated();
								if (containsNegation == correctNegation) {
									containedOperand = firstNotPosAndNegContainedOperand;
									// update first/second not contained operands
									firstNotPosAndNegContainedOperand = secondNotPosAndNegContainedOperand;
								} else {
									firstNotPosAndNegContainedOperand = nullptr;
									clashConDesLinker = createClashedConceptDescriptor(clashConDesLinker,processIndi,containedConDes,containedConDepTrackPoint,calcAlgContext);
								}
							} else {
								++notPosAndNegContainedOperandCount;								
								notContainedOperandsList.append(firstNotPosAndNegContainedOperand);
							}
							if (!containedOperand && secondNotPosAndNegContainedOperand) {
								bool remainingDisjunctsUseless = false;
								CSortedNegLinker<CConcept*>* containsOperandCheckIt = secondNotPosAndNegContainedOperand;
								secondNotPosAndNegContainedOperand = nullptr;
								while (containsOperandCheckIt) {
									bool containsNegation = false;
									bool correctNegation = containsOperandCheckIt->isNegated() ^ negate;
									CConceptDescriptor* containedConDes = nullptr;
									CDependencyTrackPoint* containedConDepTrackPoint = nullptr;
									bool contains = conSet->getConceptDescriptor(containsOperandCheckIt->getData(),containedConDes,containedConDepTrackPoint);
									if (contains) {
										containsNegation = containedConDes->isNegated();
										if (containsNegation == correctNegation) {
											containedOperand = containsOperandCheckIt;
											break;
										} else {
											clashConDesLinker = createClashedConceptDescriptor(clashConDesLinker,processIndi,containedConDes,containedConDepTrackPoint,calcAlgContext);
										}
									} else {
										if (!hasSaturatedClashedFlagForConcept(containsOperandCheckIt->getData(),correctNegation,calcAlgContext)) {
											bool criticalWithOtherOperand = false;
											if (remainingDisjunctsUseless) {
												criticalWithOtherOperand = true;
											}
											for (CPROCESSINGLIST< CSortedNegLinker<CConcept*>* >::const_iterator opIt = notContainedOperandsList.constBegin(), opItEnd = notContainedOperandsList.constEnd(); opIt != opItEnd && !criticalWithOtherOperand; ++opIt) {
												CSortedNegLinker<CConcept*>* notContOpLinker = *opIt;
												if (containsOperandCheckIt->getData() == notContOpLinker->getData()) {
													if (containsOperandCheckIt->isNegated() == notContOpLinker->isNegated()) {
														criticalWithOtherOperand = true;
													} else {
														remainingDisjunctsUseless = true;
													}
												}
											}

											if (!criticalWithOtherOperand) {
												++notPosAndNegContainedOperandCount;
												notContainedOperandsList.append(containsOperandCheckIt);
												// update first/second not contained operands
												if (!firstNotPosAndNegContainedOperand) {
													firstNotPosAndNegContainedOperand = containsOperandCheckIt;
												} else {
													if (!secondNotPosAndNegContainedOperand) {
														secondNotPosAndNegContainedOperand = containsOperandCheckIt;
													}
												}
											}
										}
									}
									containsOperandCheckIt = containsOperandCheckIt->getNext();
								}
							}
						}
					}


					if (!containedOperand) {
						// collect clashes
						// do branching

						if (calcAlgContext->getUsedUnsatisfiableCacheRetrievalStrategy()->testUnsatisfiableCacheForDisjunctionBranching(conProDes,processIndi,&notContainedOperandsList)) {
							testIndividualNodeUnsatisfiableCached(processIndi,calcAlgContext);
						}


						CORDependencyNode* orDependencyNode = createORDependency(processIndi,conDes,depTrackPoint,calcAlgContext);
						if (orDependencyNode && clashConDesLinker) {
							orDependencyNode->addBranchClashes(clashConDesLinker);
						}

						if (notPosAndNegContainedOperandCount == 1) {
							STATINC(ORSINGLEBRANCHCOUNT,calcAlgContext);

							CNonDeterministicDependencyTrackPoint* newDependencyTrackPoint = createNonDeterministicDependencyTrackPointBranch(orDependencyNode,true,calcAlgContext);
							CPROCESSINGLIST< CSortedNegLinker<CConcept*>* >::const_iterator opIt = notContainedOperandsList.constBegin();
							CSortedNegLinker<CConcept*>* operandConcept = *opIt;
							bool addOpNegated = operandConcept->isNegated() ^ negate;
							addConceptToIndividual(operandConcept->getData(),addOpNegated,processIndi,newDependencyTrackPoint,true,false,calcAlgContext);

							if (calcAlgContext->getUsedUnsatisfiableCacheRetrievalStrategy()->testUnsatisfiableCacheForBranchedDisjuncts(conProDes,processIndi,operandConcept)) {
								testIndividualNodeUnsatisfiableCached(processIndi,calcAlgContext);
							}


						} else if (notPosAndNegContainedOperandCount > 1) {
							STATINC(ORMULTIPLEBRANCHCOUNT,calcAlgContext);

							CSatisfiableCalculationTask* newTaskList = createDependendBranchingTaskList(notPosAndNegContainedOperandCount,calcAlgContext);
							CTaskProcessorContext* processorContext = calcAlgContext->getUsedTaskProcessorContext();

							// semantic branching
							CPROCESSINGLIST< CSortedNegLinker<CConcept*>* >::const_iterator branchOpConIt = notContainedOperandsList.constBegin();
							CSatisfiableCalculationTask* newTaskIt = newTaskList;
							cint64 branchNumber = 1;
							while (newTaskIt) {

								CSatisfiableCalculationTask* newSatCalcTask = newTaskIt;
								CProcessContext* newProcessContext = newSatCalcTask->getProcessContext(processorContext);

								CCalculationAlgorithmContextBase* newCalcAlgContext = createCalculationAlgorithmContext(processorContext,newProcessContext,newSatCalcTask);
								CProcessingDataBox* newProcessingDataBox = newSatCalcTask->getProcessingDataBox();

								CProcessTagger* newProcessTagger = newCalcAlgContext->getUsedProcessTagger();
								newProcessTagger->incBranchingTag();
								newProcessTagger->incLocalizationTag();

								CIndividualProcessNode* newLocIndiNode = getLocalizedIndividual(processIndi,false,newCalcAlgContext);
								CConceptProcessingQueue* newConProcQueue = newLocIndiNode->getConceptProcessingQueue(true);

								CNonDeterministicDependencyTrackPoint* newDependencyTrackPoint = createNonDeterministicDependencyTrackPointBranch(orDependencyNode,false,newCalcAlgContext);

								CSortedNegLinker<CConcept*>* branchingOperandConcept = nullptr;
								CPROCESSINGLIST< CSortedNegLinker<CConcept*>* >::const_iterator opIt = notContainedOperandsList.constBegin();
								do {
									STATINC(TASKORBRANCHCREATIONCOUNT,calcAlgContext);

									CSortedNegLinker<CConcept*>* operandConcept = *opIt;
									bool posOperand = (opIt == branchOpConIt);
									if (posOperand) {
										branchingOperandConcept = operandConcept;
									}

									bool addOpNegated = !posOperand ^ operandConcept->isNegated() ^ negate;

									if (posOperand || mConfSemanticBranching || mConfAtomicSemanticBranching && isConceptAdditionAtomaric(operandConcept->getData(),addOpNegated,newCalcAlgContext)) {
										addConceptToIndividual(operandConcept->getData(),addOpNegated,newLocIndiNode,newDependencyTrackPoint,false,true,newCalcAlgContext);
									}


								} while (opIt++ != branchOpConIt);
								++branchOpConIt;

								if (calcAlgContext->getUsedUnsatisfiableCacheRetrievalStrategy()->testUnsatisfiableCacheForBranchedDisjuncts(conProDes,processIndi,branchingOperandConcept)) {
									addIndividualNodeForCacheUnsatisfiableRetrieval(newLocIndiNode,newCalcAlgContext);
								}


								prepareBranchedTaskProcessing(newLocIndiNode,newTaskIt,newCalcAlgContext);

								// set new task priority
								double newTaskPriority = calcAlgContext->getUsedTaskPriorityStrategy()->getPriorityForTaskBranching(newSatCalcTask,calcAlgContext->getUsedSatisfiableCalculationTask(),newLocIndiNode,conDes,branchingOperandConcept,branchNumber);
								newSatCalcTask->setTaskPriority(newTaskPriority);

								++branchNumber;
								newTaskIt = (CSatisfiableCalculationTask*)newTaskIt->getNext();
							}

							processorContext->getTaskProcessorCommunicator()->communicateTaskCreation(newTaskList);

							throw CCalculationStopProcessingException(true);
						} else {
							// throw clash
							clashConDesLinker = createClashedConceptDescriptor(clashConDesLinker,processIndi,conDes,depTrackPoint,calcAlgContext);
							throw CCalculationClashProcessingException(clashConDesLinker);
						}

					} else {
						// contains at least one operand, branching is not necessary, ignoring or concept
					}
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::isConceptAdditionAtomaric(CConcept* addingConcept, bool negated, CCalculationAlgorithmContextBase* calcAlgContext) {
					cint64 opCode = addingConcept->getOperatorCode();
					if (negated && (opCode == CCSUB || opCode == CCIMPLTRIG) || opCode == CCATOM) {
						return true;
					}
					return false;
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::applyORRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(ORRULEAPPLICATIONCOUNT,calcAlgContext);
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();

					cint64 opCount = concept->getOperandCount();

					if (opCount <= 0) {

						// throw clash
						CClashedDependencyDescriptor* clashConDesLinker = nullptr;
						clashConDesLinker = createClashedConceptDescriptor(clashConDesLinker,processIndi,conDes,depTrackPoint,calcAlgContext);
						throw CCalculationClashProcessingException(clashConDesLinker);

					} else if (opCount == 1) {
						// apply AND rule
						applyANDRule(processIndi,conProDes,negate,calcAlgContext);
					} else {
						CBranchingORProcessingRestrictionSpecification* plannedBranchingProcessRestriction = nullptr;
						if (!planORProcessing(processIndi,conProDes,negate,&plannedBranchingProcessRestriction,calcAlgContext)) {
							if (mConfSatExpCachedDisjAbsorp && processIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATISFIABLECACHED | CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHED)) {
								STATINC(SATCACHEDABSORBEDDISJUNCTIONCONCEPTSCOUNT,calcAlgContext);
								addSatisfiableCachedAbsorbedDisjunctionConcept(conDes,processIndi,plannedBranchingProcessRestriction,depTrackPoint,calcAlgContext);
							} else {
								// delaying failed, execute OR rule
								executeORBranching(processIndi,conProDes,negate,plannedBranchingProcessRestriction,calcAlgContext);
							}
						}
					}
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::applyIMPLICATIONRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(IMPLICATIONRULEAPPLICATIONCOUNT,calcAlgContext);
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					CProcessingRestrictionSpecification* procRest = conProDes->getProcessingRestrictionSpecification();
					cint64 opCount = concept->getOperandCount();
					CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
					CTriggeredImplicationProcessingRestrictionSpecification* triggImpProcRes = nullptr;
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					if (!procRest) {
						triggImpProcRes = CObjectAllocator< CTriggeredImplicationProcessingRestrictionSpecification >::allocateAndConstruct(taskMemMan);
						triggImpProcRes->setConceptImplicationTrigger(opLinker->getNext());

					} else {
						CTriggeredImplicationProcessingRestrictionSpecification* lastTriggImpProcRes = (CTriggeredImplicationProcessingRestrictionSpecification*)procRest;
						triggImpProcRes = CObjectAllocator< CTriggeredImplicationProcessingRestrictionSpecification >::allocateAndConstruct(taskMemMan);
						triggImpProcRes->initImplicationTriggeringProcessingRestriction(lastTriggImpProcRes);
					}

					// search next not existing trigger
					bool allTriggersAvailable = false;
					CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(true);
					while (triggImpProcRes->hasConceptImplicationTrigger()) {
						CSortedNegLinker<CConcept*>* nextTrigger = triggImpProcRes->getConceptImplicationTrigger();
						CConcept* triggerConcept = nextTrigger->getData();
						CConceptDescriptor* triggerConDes = nullptr;
						CDependencyTrackPoint* triggerDepTrackPoint = nullptr;
						if (conSet->getConceptDescriptor(triggerConcept,triggerConDes,triggerDepTrackPoint)) {
							if (triggerConDes->isNegated() == nextTrigger->isNegated()) {
								return;
							} else {
								if (triggerDepTrackPoint != depTrackPoint) {
									// add dependency track point
									CCONNECTIONDependencyNode* connDep = createCONNECTIONDependency(processIndi,triggerConDes,triggerDepTrackPoint,calcAlgContext);
									connDep->setNext(triggImpProcRes->getImplicationDependency());
									triggImpProcRes->setImplicationDependency(connDep);
								}
								triggImpProcRes->setConceptImplicationTrigger(nextTrigger->getNext());
							}
						} else {
							break;
						}
					}
					if (!triggImpProcRes->hasConceptImplicationTrigger()) {
						allTriggersAvailable = true;
					}

					if (!allTriggersAvailable) {
						// install to trigger
						STATINC(IMPLICATIONTRIGGERINGCOUNT,calcAlgContext);
						CSortedNegLinker<CConcept*>* nextTrigger = triggImpProcRes->getConceptImplicationTrigger();
						CConcept* triggerConcept = nextTrigger->getData();
						bool triggerNegation = !nextTrigger->isNegated();
						addConceptToReapplyQueue(conDes,triggerConcept,triggerNegation,processIndi,triggImpProcRes,depTrackPoint,calcAlgContext);
					} else {
						STATINC(IMPLICATIONEXECUTINGCOUNT,calcAlgContext);
						CDependency* triggerDeps = triggImpProcRes->getImplicationDependency();
						CDependencyTrackPoint* nextDepTrackPoint = nullptr;
						CIMPLICATIONDependencyNode* implDepNode = createIMPLICATIONDependency(nextDepTrackPoint,processIndi,conDes,depTrackPoint,triggerDeps,calcAlgContext);
						CConcept* implConcept = opLinker->getData();
						bool impConNeg = opLinker->isNegated();
						addConceptToIndividual(implConcept,impConNeg,processIndi,nextDepTrackPoint,true,false,calcAlgContext);
					}


				}





				CSatisfiableCalculationTask* CCalculationTableauCompletionTaskHandleAlgorithm::createDependendBranchingTaskList(cint64 newTaskCount, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSatisfiableCalculationTask* taskList = nullptr;
					CMemoryTemporaryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedTemporaryMemoryAllocationManager();
					for (cint64 i = 0; i < newTaskCount; ++i) {
						STATINC(TASKCREATIONCOUNT,calcAlgContext);
						CSatisfiableCalculationTask* satCalcTask = CObjectMemoryPoolAllocator<CSatisfiableCalculationTask>::allocateAndConstructWithMemroyPool(taskMemMan);
						satCalcTask->initBranchDependedSatisfiableCalculationTask(calcAlgContext->getUsedSatisfiableCalculationTask(),calcAlgContext->getTaskProcessorContext());

						if (calcAlgContext->getUsedSatisfiableCalculationTask()->getTaskDepth() < 90) {
							satCalcTask->setTaskID(mDebugTaskIDVector[calcAlgContext->getUsedSatisfiableCalculationTask()->getTaskDepth()+1]++);
						}

						taskList = (CSatisfiableCalculationTask*)satCalcTask->append(taskList);
					}
					return taskList;
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::prepareBranchedTaskProcessing(CIndividualProcessNode*& individual, CSatisfiableCalculationTask* newTask, CCalculationAlgorithmContextBase* calcAlgContext) {
					addIndividualToProcessingQueue(individual,calcAlgContext);
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::installConceptRoleBranchTrigger(CIndividualProcessNode*& processIndi, CConceptDescriptor* conceptDescriptor, CDependencyTrackPoint* depTrackPoint, CProcessingRestrictionSpecification* procRest, CConceptRoleBranchingTrigger* trigger, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (trigger->isConceptTrigger()) {
						STATINC(CONCEPTTRIGGERINSTALLCOUNT,calcAlgContext);
						CConcept* triggerConcept = trigger->getTriggerConcept();
						bool triggerNegation = trigger->getTriggerNegation();
						addConceptToReapplyQueue(conceptDescriptor,triggerConcept,triggerNegation,processIndi,procRest,depTrackPoint,calcAlgContext);
					} else {
						STATINC(ROLETRIGGERINSTALLCOUNT,calcAlgContext);
						CRole* role = trigger->getTriggerRole();
						addConceptToReapplyQueue(conceptDescriptor,role,processIndi,procRest,depTrackPoint,calcAlgContext);
					}
				}



				CConceptRoleBranchingTrigger* CCalculationTableauCompletionTaskHandleAlgorithm::searchNextConceptRoleBranchTrigger(CIndividualProcessNode*& processIndi, CConceptRoleBranchingTrigger* triggers, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReapplyRoleSuccessorHash* reapplySuccHash = processIndi->getReapplyRoleSuccessorHash(false);
					CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(false);
					while (triggers) {
						if (triggers->isConceptTrigger()) {
							CConcept* triggerConcept = triggers->getTriggerConcept();
							bool triggerNegation = triggers->getTriggerNegation();
							if (!conSet || !conSet->containsConcept(triggerConcept,triggerNegation)) {
								return triggers;
							}
						} else {
							CRole* role = triggers->getTriggerRole();
							if (!reapplySuccHash || !reapplySuccHash->hasRoleSuccessor(role)) {
								return triggers;
							}
						}
						triggers = triggers->getNextBranchingTrigger();
					}
					return nullptr;
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::applySELFRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(SELFRULEAPPLICATIONCOUNT,calcAlgContext);
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					CDependencyTrackPoint* depTrackPoint = conProDes->getDependencyTrackPoint();
					bool reapplied = conProDes->isConceptReapplied();
					CIndividualLinkEdge* restLink = getLinkProcessingRestriction(conProDes);
					if (!negate) {
						CIndividualLinkEdge* link = getIndividualNodeLink(processIndi,processIndi,role,calcAlgContext);
						if (!link) {
							// self edge/link does not exist
							// create dependency
							CDependencyTrackPoint* nextDepTrackPoint = nullptr;
							CSELFDependencyNode* selfDepNode = createSELFDependency(nextDepTrackPoint,processIndi,conDes,depTrackPoint,calcAlgContext);
							createNewIndividualsLinksReapplyed(processIndi,processIndi,role->getIndirectSuperRoleList(),role,nextDepTrackPoint,true,calcAlgContext);
						}
					} else {
						if (restLink) {
							if (restLink->isDestinationIndividual(processIndi) && restLink->isSourceIndividual(processIndi)) {
								// throw clash
								CClashedDependencyDescriptor* clashDes = nullptr;
								clashDes = createClashedIndividualLinkDescriptor(clashDes,restLink,restLink->getDependencyTrackPoint(),calcAlgContext);
								clashDes = createClashedConceptDescriptor(clashDes,processIndi,conDes,depTrackPoint,calcAlgContext);
								throw CCalculationClashProcessingException(clashDes);
							}
						} else {
							CIndividualLinkEdge* link = getIndividualNodeLink(processIndi,processIndi,role,calcAlgContext);
							if (link) {
								// throw clash
								CClashedDependencyDescriptor* clashDes = nullptr;
								clashDes = createClashedIndividualLinkDescriptor(clashDes,link,link->getDependencyTrackPoint(),calcAlgContext);
								clashDes = createClashedConceptDescriptor(clashDes,processIndi,conDes,depTrackPoint,calcAlgContext);
								throw CCalculationClashProcessingException(clashDes);
							}
						}
						if (!reapplied) {
							addConceptToReapplyQueue(conDes,role,processIndi,true,depTrackPoint,calcAlgContext);
						}
					}
				}


				CIndividualLinkEdge* CCalculationTableauCompletionTaskHandleAlgorithm::getLinkProcessingRestriction(CConceptProcessDescriptor*& conProDes) {
					CProcessingRestrictionSpecification* procRestSpec = conProDes->getProcessingRestrictionSpecification();
					CIndividualLinkEdge* restLink = nullptr;
					if (procRestSpec) {
						CLinkProcessingRestrictionSpecification* linkProcRestSpec = (CLinkProcessingRestrictionSpecification*)procRestSpec;
						restLink = linkProcRestSpec->getLinkRestriction();
					}
					return restLink;
				}












				CIndividualLinkEdge* CCalculationTableauCompletionTaskHandleAlgorithm::getIndividualNodeLink(CIndividualProcessNode*& indiSource, CIndividualProcessNode*& indiDestination, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSuccessorRoleHash* succRoleHash = indiSource->getSuccessorRoleHash(false);
					if (succRoleHash) {
						CSuccessorRoleIterator succRoleIt = succRoleHash->getSuccessorRoleIterator(indiSource->getIndividualID());
						while (succRoleIt.hasNext()) {
							CIndividualLinkEdge* link = succRoleIt.next(true);
							if (link->getLinkRole() == role) {
								return link;
							}
						}
					}
					return nullptr;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::isLabelConceptSubSetIgnoreNominals(CReapplyConceptLabelSet* subConceptSet, CReapplyConceptLabelSet* superConceptSet, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(LABELCONCEPTSUBSETTESTCOUNT,calcAlgContext);
					cint64 subConSetCount = subConceptSet->getConceptCount();
					cint64 superConSetCount = superConceptSet->getConceptCount();
					cint64 thresholdFactor = mMapComparisonDirectLookupFactor;
					if (subConSetCount * thresholdFactor < superConSetCount) {
						CReapplyConceptLabelSetIterator subConSetIt = subConceptSet->getConceptLabelSetIterator(true,false,false);
						while (subConSetIt.hasValue()) {
							CConceptDescriptor* subConDes = subConSetIt.getConceptDescriptor();
							if (subConDes->getConcept()->getOperatorCode() != CCNOMINAL) {
								if (!superConceptSet->containsConceptDescriptor(subConDes)) {
									return false;
								}
							}
							subConSetIt.moveNext();
						}
					} else {
						CReapplyConceptLabelSetIterator subConSetIt(subConceptSet->getConceptLabelSetIterator(true,false,false));
						CReapplyConceptLabelSetIterator superConSetIt(superConceptSet->getConceptLabelSetIterator(true,false,false));
						CConceptDescriptor* superConDes = superConSetIt.getConceptDescriptor();
						cint64 superConTag = superConDes->getConceptTag();
						superConSetIt.moveNext();
						while (subConSetIt.hasValue()) {
							CConceptDescriptor* subConDes = subConSetIt.getConceptDescriptor();
							if (subConDes->getConcept()->getOperatorCode() != CCNOMINAL) {
								cint64 subConTag = subConDes->getConceptTag();

								while (superConTag < subConTag) {
									if (!superConSetIt.hasValue()) {
										return false;
									}
									superConDes = superConSetIt.getConceptDescriptor();
									superConTag = superConDes->getConceptTag();
									superConSetIt.moveNext();
								}
								if (subConTag != superConTag) {
									return false;
								} else {
									if (subConDes->isNegated() != superConDes->isNegated()) {
										return false;
									}
								}
							}
							subConSetIt.moveNext();
						}
					}					
					return true;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::isLabelConceptSubSet(CReapplyConceptLabelSet* subConceptSet, CReapplyConceptLabelSet* superConceptSet, CConceptDescriptor** firstNotEntailedConDes, bool* equalConSet, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(LABELCONCEPTSUBSETTESTCOUNT,calcAlgContext);
					cint64 subConSetCount = subConceptSet->getConceptCount();
					cint64 superConSetCount = superConceptSet->getConceptCount();
					if (equalConSet) {
						if (subConSetCount != superConSetCount) {
							*equalConSet = false;
						} else {
							*equalConSet = true;
						}
					}
					if (subConSetCount > superConSetCount) {
						return false;
					}
					if (superConSetCount == 0) {
						return true;
					}
					cint64 thresholdFactor = 10;
					if (subConSetCount * thresholdFactor < superConSetCount) {
						CReapplyConceptLabelSetIterator subConSetIt = subConceptSet->getConceptLabelSetIterator(true,false,false);
						while (subConSetIt.hasValue()) {
							CConceptDescriptor* subConDes = subConSetIt.getConceptDescriptor();
							if (!superConceptSet->containsConceptDescriptor(subConDes)) {
								if (firstNotEntailedConDes) {
									*firstNotEntailedConDes = subConDes;
								}
								return false;
							}
							subConSetIt.moveNext();
						}
					} else {
						CReapplyConceptLabelSetIterator subConSetIt = subConceptSet->getConceptLabelSetIterator(true,false,false);
						CReapplyConceptLabelSetIterator superConSetIt = superConceptSet->getConceptLabelSetIterator(true,false,false);
						CConceptDescriptor* superConDes = superConSetIt.getConceptDescriptor();
						cint64 superConTag = superConDes->getConceptTag();
						superConSetIt.moveNext();
						while (subConSetIt.hasValue()) {
							CConceptDescriptor* subConDes = subConSetIt.getConceptDescriptor();
							cint64 subConTag = subConDes->getConceptTag();
							
							while (superConTag < subConTag) {
								if (!superConSetIt.hasValue()) {
									if (firstNotEntailedConDes) {
										*firstNotEntailedConDes = subConDes;
									}
									return false;
								}
								superConDes = superConSetIt.getConceptDescriptor();
								superConTag = superConDes->getConceptTag();
								superConSetIt.moveNext();
								if (equalConSet && superConTag < subConTag) {
									*equalConSet = false;
								}
							}
							if (subConTag != superConTag) {
								if (firstNotEntailedConDes) {
									*firstNotEntailedConDes = subConDes;
								}
								if (equalConSet) {
									*equalConSet = false;
								}
								return false;
							} else {
								if (subConDes->isNegated() != superConDes->isNegated()) {
									if (firstNotEntailedConDes) {
										*firstNotEntailedConDes = subConDes;
									}
									if (equalConSet) {
										*equalConSet = false;
									}
									return false;
								}
							}
							subConSetIt.moveNext();
						}
					}
					return true;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::isLabelConceptEqualSet(CReapplyConceptLabelSet* conceptSet1, CReapplyConceptLabelSet* conceptSet2, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(LABELCONCEPTEQUALSETTESTCOUNT,calcAlgContext);
					cint64 conceptSet1Count = conceptSet1->getConceptCount();
					cint64 conceptSet2Count = conceptSet2->getConceptCount();
					if (conceptSet1Count != conceptSet2Count) {
						return false;
					}
					if (!conceptSet1->getConceptSignature()->isSignatureEquivalent(conceptSet2->getConceptSignature())) {
						return false;
					}
					CReapplyConceptLabelSetIterator conSet1It = conceptSet1->getConceptLabelSetIterator(true,false,false);
					CReapplyConceptLabelSetIterator conSet2It = conceptSet2->getConceptLabelSetIterator(true,false,false);
					while (conSet1It.hasValue()) {
						if (!conSet2It.hasValue()) {
							return false;
						}
						CConceptDescriptor* conDes1 = conSet1It.getConceptDescriptor();
						CConceptDescriptor* conDes2 = conSet2It.getConceptDescriptor();
						if (conDes1->getConcept() != conDes2->getConcept()) {
							return false;
						}
						if (conDes1->isNegated() != conDes2->isNegated()) {
							return false;
						}
						conSet1It.moveNext();
						conSet2It.moveNext();
					}
					return true;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::isPairwiseLabelConceptEqualSet(CReapplyConceptLabelSet* conceptSet1, CReapplyConceptLabelSet* conceptSet1Pair, CReapplyConceptLabelSet* conceptSet2, CReapplyConceptLabelSet* conceptSet2Pair, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(LABELCONCEPTPAIRWISEEQUALSETTESTCOUNT,calcAlgContext);
					cint64 conceptSet1Count = conceptSet1->getConceptCount();
					cint64 conceptSet1PCount = conceptSet1Pair->getConceptCount();
					if (conceptSet1Count != conceptSet1PCount) {
						return false;
					}
					cint64 conceptSet2Count = conceptSet2->getConceptCount();
					cint64 conceptSet2PCount = conceptSet2Pair->getConceptCount();
					if (conceptSet2Count != conceptSet2PCount) {
						return false;
					}
					if (!conceptSet1->getConceptSignature()->isSignatureEquivalent(conceptSet1Pair->getConceptSignature())) {
						return false;
					}
					if (!conceptSet2->getConceptSignature()->isSignatureEquivalent(conceptSet2Pair->getConceptSignature())) {
						return false;
					}
					CReapplyConceptLabelSetIterator conSet1It = conceptSet1->getConceptLabelSetIterator(true,false,false);
					CReapplyConceptLabelSetIterator conSet1PairIt = conceptSet1Pair->getConceptLabelSetIterator(true,false,false);
					while (conSet1It.hasValue()) {
						if (!conSet1PairIt.hasValue()) {
							return false;
						}
						CConceptDescriptor* conDes1 = conSet1It.getConceptDescriptor();
						CConceptDescriptor* conDes1Pair = conSet1PairIt.getConceptDescriptor();
						if (conDes1->getConcept() != conDes1Pair->getConcept()) {
							return false;
						}
						if (conDes1->isNegated() != conDes1Pair->isNegated()) {
							return false;
						}
						conSet1It.moveNext();
						conSet1PairIt.moveNext();
					}
					CReapplyConceptLabelSetIterator conSet2It = conceptSet2->getConceptLabelSetIterator(true,false);
					CReapplyConceptLabelSetIterator conSet2PairIt = conceptSet2Pair->getConceptLabelSetIterator(true,false,false);
					while (conSet2It.hasValue()) {
						if (!conSet2PairIt.hasValue()) {
							return false;
						}
						CConceptDescriptor* conDes2 = conSet2It.getConceptDescriptor();
						CConceptDescriptor* conDes2Pair = conSet2PairIt.getConceptDescriptor();
						if (conDes2->getConcept() != conDes2Pair->getConcept()) {
							return false;
						}
						if (conDes2->isNegated() != conDes2Pair->isNegated()) {
							return false;
						}
						conSet2It.moveNext();
						conSet2PairIt.moveNext();
					}
					return true;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::hasOptimizedBlockingB2AutomateTransitionOperands(CConcept* concept, CRole* role, CReapplyConceptLabelSet* vConSet, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(OPTIMIZEDBLOCKINGB2AUTOMATETRANSACTIONTESTCOUNT,calcAlgContext);
					cint64 opCode = concept->getOperatorCode();
					CConceptOperator* conOperator = concept->getConceptOperator();
					if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQAND_TYPE)) {
						// recursive
						CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList();
						while (opLinkerIt) {
							CConcept* opConcept = opLinkerIt->getData();
							if (!hasOptimizedBlockingB2AutomateTransitionOperands(opConcept,role,vConSet,calcAlgContext)) {
								return false;
							}
							opLinkerIt = opLinkerIt->getNext();
						}
					} else if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQALL_TYPE)) {
						CRole* conRole = concept->getRole();
						if (conRole == role) {
							CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList();
							while (opLinkerIt) {
								CConcept* opConcept = opLinkerIt->getData();
								if (!vConSet->containsConcept(opConcept,false)) {
									return false;
								}
								opLinkerIt = opLinkerIt->getNext();
							}
						}
					}
					return true;
				}





				bool CCalculationTableauCompletionTaskHandleAlgorithm::isNominalVariablePropagationBindingSubSet(CIndividualProcessNode*& testIndi, CIndividualProcessNode*& blockingIndi, CIndividualNodeBlockingTestData* blockData, bool testContinueBlocking, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualProcessNode* ancestorIndiNode = nullptr;
					CConceptPropagationBindingSetHash* conPropBindSetHash = testIndi->getConceptPropagationBindingSetHash(false);
					CConceptPropagationBindingSetHash* blockerConPropBindSetHash = blockerConPropBindSetHash = blockingIndi->getConceptPropagationBindingSetHash(false);
					if (conPropBindSetHash) {
						for (CConceptPropagationBindingSetHash::iterator it1 = conPropBindSetHash->begin(), it1End = conPropBindSetHash->end(); it1 != it1End; ++it1) {
							CConceptPropagationBindingSetHashData& hashData = it1.value();
							CPropagationBindingSet* propBindSet = hashData.mUsePropBindingSet;
							CConceptDescriptor* conDes = propBindSet->getConceptDescriptor();

							if (propBindSet && conDes) {
								CConcept* concept = conDes->getConcept();
								CPropagationBindingSet* blockerPropBindSet = nullptr;

								CPropagationBindingMap* propBindMap = propBindSet->getPropagationBindingMap();
								for (CPropagationBindingMap::iterator it2 = propBindMap->begin(), it2End = propBindMap->end(); it2 != it2End; ++it2) {
									CPropagationBindingMapData& mapData = it2.value();
									CPropagationBindingDescriptor* propBindDes = mapData.getPropagationBindingDescriptor();
									if (propBindDes) {
										CPropagationBinding* propBind = propBindDes->getPropagationBinding();
										if (propBind) {
											CVariable* propBindVar = propBind->getBindedVariable();
											if (propBindVar) {
												if (propBindVar->isNominalVariable()) {

													// propagation variable binding is also required in blocker node
													if (!blockerPropBindSet) {
														if (!blockerConPropBindSetHash) {
															return false;
														}
														blockerPropBindSet = blockerConPropBindSetHash->getPropagationBindingSet(concept,false);
														if (!blockerPropBindSet) {
															return false;
														}
													}
													bool blockingPossible = false;
													CPropagationBindingDescriptor* blockerPropBindDes = blockerPropBindSet->getPropagationBindingDescriptor(propBind);
													if (blockerPropBindDes) {
														blockingPossible = true;
													}

													if (!blockingPossible) {
														return false;
													}
												}
											}
										}
									}
								}
							}
						}
					}
					if (blockerConPropBindSetHash) {
						if (!ancestorIndiNode) {
							ancestorIndiNode = getAncestorIndividual(testIndi,calcAlgContext);
						}
						if (ancestorIndiNode) {
							CConceptPropagationBindingSetHash* ancConPropBindSetHash = ancestorIndiNode->getConceptPropagationBindingSetHash(false);
							for (CConceptPropagationBindingSetHash::iterator it1 = blockerConPropBindSetHash->begin(), it1End = blockerConPropBindSetHash->end(); it1 != it1End; ++it1) {
								CConceptPropagationBindingSetHashData& hashData = it1.value();
								CPropagationBindingSet* blockerPropBindSet = hashData.mUsePropBindingSet;
								CConceptDescriptor* conDes = blockerPropBindSet->getConceptDescriptor();

								if (blockerPropBindSet && conDes) {
									CConcept* concept = conDes->getConcept();
									cint64 opCode = concept->getOperatorCode();
									bool negation = conDes->isNegated();

									if (!negation && (opCode == CCPBINDALL || opCode == CCPBINDAQALL)) {
										CRole* role = concept->getRole();
										for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
											CConcept* opConcept = opConLinkerIt->getData();
											if (testIndi->hasRoleSuccessorToIndividual(role,ancestorIndiNode,true)) {
												if (!ancConPropBindSetHash) {
													return false;
												} else {
													CPropagationBindingSet* ancPropBindSet = nullptr;
													CPropagationBindingMap* propBindMap = blockerPropBindSet->getPropagationBindingMap();
													for (CPropagationBindingMap::iterator it2 = propBindMap->begin(), it2End = propBindMap->end(); it2 != it2End; ++it2) {
														CPropagationBindingMapData& mapData = it2.value();
														CPropagationBindingDescriptor* blockerPropBindDes = mapData.getPropagationBindingDescriptor();
														if (blockerPropBindDes) {
															CPropagationBinding* blockerPropBind = blockerPropBindDes->getPropagationBinding();
															if (blockerPropBind) {
																CVariable* blockerPropBindVar = blockerPropBind->getBindedVariable();
																if (blockerPropBindVar) {
																	if (blockerPropBindVar->isNominalVariable()) {

																		bool blockingPossible = false;
																		if (!ancPropBindSet) {
																			ancPropBindSet = ancConPropBindSetHash->getPropagationBindingSet(opConcept);
																			if (!ancPropBindSet) {
																				return false;
																			}
																		}
																		CPropagationBindingDescriptor* ancPropBindDes = ancPropBindSet->getPropagationBindingDescriptor(blockerPropBind);
																		if (ancPropBindDes) {
																			blockingPossible = true;
																		}
																		if (!blockingPossible) {
																			return false;
																		}

																	}
																}
															}
														}
													}

												}
											}
										}
									}

								}
							}
						}
					}




					CConceptVariableBindingPathSetHash* conVarBindSetHash = testIndi->getConceptVariableBindingPathSetHash(false);
					CConceptVariableBindingPathSetHash* blockerConVarBindSetHash = blockerConVarBindSetHash = blockingIndi->getConceptVariableBindingPathSetHash(false);
					if (conVarBindSetHash) {
						for (CConceptVariableBindingPathSetHash::iterator it1 = conVarBindSetHash->begin(), it1End = conVarBindSetHash->end(); it1 != it1End; ++it1) {
							CConceptVariableBindingPathSetHashData& hashData = it1.value();
							CVariableBindingPathSet* varBindSet = hashData.mUseVariableBindingPathSet;
							CConceptDescriptor* conDes = varBindSet->getConceptDescriptor();

							if (varBindSet && conDes) {
								CConcept* concept = conDes->getConcept();
								CVariableBindingPathSet* blockerVarBindSet = nullptr;

								CVariableBindingPathMap* varBindMap = varBindSet->getVariableBindingPathMap();
								for (CVariableBindingPathMap::iterator it2 = varBindMap->begin(), it2End = varBindMap->end(); it2 != it2End; ++it2) {
									CVariableBindingPathMapData& mapData = it2.value();
									CVariableBindingPathDescriptor* varBindDes = mapData.getVariableBindingPathDescriptor();
									if (varBindDes) {
										CVariableBindingPath* varBindPath = varBindDes->getVariableBindingPath();
										if (varBindPath) {

											// propagation variable binding is also required in blocker node
											if (!blockerVarBindSet) {
												if (!blockerConVarBindSetHash) {
													return false;
												}
												blockerVarBindSet = blockerConVarBindSetHash->getVariableBindingPathSet(concept,false);
												if (!blockerVarBindSet) {
													return false;
												}
											}
											bool blockingPossible = false;
											CVariableBindingPathDescriptor* blockerVarBindDes = blockerVarBindSet->getVariableBindingPathDescriptor(varBindPath);
											if (blockerVarBindDes) {
												blockingPossible = true;
											}

											if (!blockingPossible) {
												return false;
											}
										}
									}
								}
							}
						}
					}
					if (blockerConVarBindSetHash) {
						if (!ancestorIndiNode) {
							ancestorIndiNode = getAncestorIndividual(testIndi,calcAlgContext);
						}
						if (ancestorIndiNode) {
							CConceptVariableBindingPathSetHash* ancConVarBindSetHash = ancestorIndiNode->getConceptVariableBindingPathSetHash(false);
							for (CConceptVariableBindingPathSetHash::iterator it1 = blockerConVarBindSetHash->begin(), it1End = blockerConVarBindSetHash->end(); it1 != it1End; ++it1) {
								CConceptVariableBindingPathSetHashData& hashData = it1.value();
								CVariableBindingPathSet* blockerVarBindSet = hashData.mUseVariableBindingPathSet;
								CConceptDescriptor* conDes = blockerVarBindSet->getConceptDescriptor();

								if (blockerVarBindSet && conDes) {
									CConcept* concept = conDes->getConcept();
									cint64 opCode = concept->getOperatorCode();
									bool negation = conDes->isNegated();

									if (!negation && (opCode == CCVARBINDALL || opCode == CCVARBINDAQALL)) {
										CRole* role = concept->getRole();
										for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
											CConcept* opConcept = opConLinkerIt->getData();
											if (testIndi->hasRoleSuccessorToIndividual(role,ancestorIndiNode,true)) {
												if (!ancConVarBindSetHash) {
													return false;
												} else {
													CVariableBindingPathSet* ancVarBindSet = nullptr;
													CVariableBindingPathMap* varBindMap = blockerVarBindSet->getVariableBindingPathMap();
													for (CVariableBindingPathMap::iterator it2 = varBindMap->begin(), it2End = varBindMap->end(); it2 != it2End; ++it2) {
														CVariableBindingPathMapData& mapData = it2.value();
														CVariableBindingPathDescriptor* blockerVarBindDes = mapData.getVariableBindingPathDescriptor();
														if (blockerVarBindDes) {
															CVariableBindingPath* blockerVarBindPath = blockerVarBindDes->getVariableBindingPath();
															if (blockerVarBindPath) {

																bool blockingPossible = false;
																if (!ancVarBindSet) {
																	ancVarBindSet = ancConVarBindSetHash->getVariableBindingPathSet(opConcept);
																	if (!ancVarBindSet) {
																		return false;
																	}
																}
																CVariableBindingPathDescriptor* ancVarBindPathDes = ancVarBindSet->getVariableBindingPathDescriptor(blockerVarBindPath);
																if (ancVarBindPathDes) {
																	blockingPossible = true;
																}
																if (!blockingPossible) {
																	return false;
																}

															}
														}
													}

												}
											}
										}
									}

								}
							}
						}
					}

					return true;
				}




				bool CCalculationTableauCompletionTaskHandleAlgorithm::isLabelConceptOptimizedBlocking(CIndividualProcessNode*& testIndi, CIndividualProcessNode*& blockingIndi, CIndividualNodeBlockingTestData* blockData, bool testContinueBlocking, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(OPTIMIZEDBLOCKINGTESTCOUNT,calcAlgContext);
					CIndividualProcessNode* wNode = testIndi;
					CIndividualProcessNode* wPredNode = blockingIndi;

					CReapplyConceptLabelSet* wSubConSet = wNode->getReapplyConceptLabelSet(false);
					CReapplyConceptLabelSet* wPredSuperConSet = wPredNode->getReapplyConceptLabelSet(false);
					CConceptDescriptor* firstNotEntailedConDes = nullptr;
					// B1 test
					bool equalSet = false;
					bool subSet = isLabelConceptSubSet(wSubConSet,wPredSuperConSet,&firstNotEntailedConDes,&equalSet,calcAlgContext);
					if (!subSet) {
						return false;
					}


					CIndividualProcessNode* vNode = getAncestorIndividual(wNode,calcAlgContext);
					CReapplyConceptLabelSet* vConSet = vNode->getReapplyConceptLabelSet(false);
					// B2 test

					CSuccessorRoleHash* ancRoleHash = wNode->getSuccessorRoleHash(false);
					if (!ancRoleHash) {
						// no inverse roles
						return true;
					}
					// assume blocking is possible
					bool blocked = true;

					cint64 violatingB2Restrictions = 0;
					cint64 violatingNonDetB2Restrictions = 0;

					CSuccessorRoleIterator baseAncRoleIt = ancRoleHash->getSuccessorRoleIterator(vNode->getIndividualID());
					CSuccessorRoleIterator ancRoleIt1 = baseAncRoleIt;
					while (ancRoleIt1.hasNext() && blocked) {
						CIndividualLinkEdge* link = ancRoleIt1.next();
						// from w to v is a edge labeled with r, w is an inv(r)-successor of v
						CRole* role = link->getLinkRole();
						// B2, (ALL r. C) in w'
						CReapplyQueueIterator reapplyQuIt = wPredNode->getRoleReapplyIterator(role,false);
						while (reapplyQuIt.hasNext() && blocked) {
							CReapplyConceptDescriptor* reapplyConDes = reapplyQuIt.next();
							CConceptDescriptor* conDes = reapplyConDes->getConceptDescriptor();
							CConcept* concept = conDes->getConcept();
							bool conNeg = conDes->isNegated();
							cint64 conOpCode = concept->getOperatorCode();
							CConceptOperator* conOperator = concept->getConceptOperator();
							if (!conNeg && (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_AQALL_TYPE)) || conNeg && conOpCode == CCSOME) {
								// B2a
								CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList();
								while (opLinkerIt && blocked) {
									CConcept* opConcept = opLinkerIt->getData();
									bool opConNeg = opLinkerIt->isNegated() ^ conNeg;
									if (!vConSet->containsConcept(opConcept,opConNeg)) {
										blocked = false;
										CDependencyTrackPoint* depTrackPoint = conDes->getDependencyTrackPoint();
										bool nonDetDependency = depTrackPoint->getBranchingTag() > 0;
										++violatingB2Restrictions;
										if (nonDetDependency) {
											++violatingNonDetB2Restrictions;
										}
									}
									opLinkerIt = opLinkerIt->getNext();
								}
							} else if (!conNeg && (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQAND_TYPE))) {
								// B2b, transitive and automate transitions
								if (!hasOptimizedBlockingB2AutomateTransitionOperands(concept,role,vConSet,calcAlgContext)) {
									blocked = false;
									CDependencyTrackPoint* depTrackPoint = conDes->getDependencyTrackPoint();
									bool nonDetDependency = depTrackPoint->getBranchingTag() > 0;
									++violatingB2Restrictions;
									if (nonDetDependency) {
										++violatingNonDetB2Restrictions;
									}
								}
							}
						}
					}

					CReapplyRoleSuccessorHash* wPredSuccHash = wPredNode->getReapplyRoleSuccessorHash(false);
					if (blocked) {

						// test c-blocked and a-blocked specific parts
						// B3, B5

						bool cBlocked = true;
						bool aBlocked = true;
						CSuccessorRoleIterator ancRoleIt2 = baseAncRoleIt;
						while (ancRoleIt2.hasNext() && blocked) {
							CIndividualLinkEdge* link = ancRoleIt2.next();
							CRole* role = link->getLinkRole();
							// B�, (ATMOST n r. C) in w'
							CReapplyQueueIterator reapplyQuIt = wPredNode->getRoleReapplyIterator(role,false);
							while (reapplyQuIt.hasNext() && blocked) {
								CReapplyConceptDescriptor* reapplyConDes = reapplyQuIt.next();
								CConceptDescriptor* conDes = reapplyConDes->getConceptDescriptor();
								CConcept* concept = conDes->getConcept();
								bool conNeg = conDes->isNegated();
								cint64 conOpCode = concept->getOperatorCode();

								bool checkBlockerRoleCardinality = false;
								cint64 blockerMinSuccCardinality = 0;
								CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();

								if (!conNeg && conOpCode == CCATMOST || conNeg && conOpCode == CCATLEAST) {
									cint64 cardinality = concept->getParameter();
									if (conNeg) {
										--cardinality;
									}

									CSortedNegLinker<CConcept*>* opLinkerIt = opLinker;
									if (!opLinkerIt) {
										cBlocked = false;
										if (!checkBlockerRoleCardinality) {
											checkBlockerRoleCardinality = true;
											blockerMinSuccCardinality = cardinality;
										}
									}
									while (opLinkerIt) {
										CConcept* opConcept = opLinkerIt->getData();
										bool opConNeg = opLinkerIt->isNegated();
										bool containsNeg = false;
										if (!vConSet->containsConcept(opConcept,&containsNeg)) {
											// contains not positive and negative
											cBlocked = false;
											if (!checkBlockerRoleCardinality) {
												checkBlockerRoleCardinality = true;
												blockerMinSuccCardinality = cardinality;
											}
										}
										if (containsNeg == opConNeg) {
											cBlocked = false;
											if (!checkBlockerRoleCardinality) {
												checkBlockerRoleCardinality = true;
												blockerMinSuccCardinality = cardinality;
											}
										}
										opLinkerIt = opLinkerIt->getNext();
									}
								}


								if (checkBlockerRoleCardinality) {
									// count
									cint64 minRoleCardinality = 0;
									if (wPredSuccHash) {
										CRoleSuccessorLinkIterator succIt = wPredSuccHash->getRoleSuccessorLinkIterator(role);
										while (succIt.hasNext() && blocked) {
											CIndividualLinkEdge* succLink = succIt.next();
											CIndividualProcessNode* succIndiNode = getSuccessorIndividual(wPredNode,succLink,calcAlgContext);
											if (succIndiNode->getIndividualAncestorDepth() > wPredNode->getIndividualAncestorDepth()) {
												if (!opLinker) {
													++minRoleCardinality;
													if (minRoleCardinality >= blockerMinSuccCardinality) {
														blocked = false;
													}
												} else {
													if (containsIndividualNodeConcepts(succIndiNode,opLinker,false,calcAlgContext)) {
														++minRoleCardinality;
														if (minRoleCardinality >= blockerMinSuccCardinality) {
															blocked = false;
														}
													}
												}
											}
										}
									}
								}
							}
						}

						if (cBlocked) {
							// test whether B6 holds
							CReapplyConceptLabelSetIterator vConSetIt = vConSet->getConceptLabelSetIterator(false,false,false);
							while (cBlocked && vConSetIt.hasNext()) {
								CConceptDescriptor* conDes = vConSetIt.next();
								CConcept* concept = conDes->getConcept();
								bool conNeg = conDes->isNegated();
								cint64 conOpCode = concept->getOperatorCode();
								if (!conNeg && conOpCode == CCATLEAST || conNeg && conOpCode == CCATMOST) {
									cint64 cardinality = concept->getParameter() + 1*conNeg;
									if (cardinality > 1) {
										CRole* role = concept->getRole();
										if (hasIndividualsLink(vNode,wNode,role,false,calcAlgContext)) {
											CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
											if (opLinker) {
												if (!containsIndividualNodeConcepts(wSubConSet,opLinker,!conNeg,calcAlgContext)) {
													cBlocked = false;
												}
											} else {
												cBlocked = false;
											}
										}
									}
								}
							}
							if (cBlocked) {
								return true;
							}
						}
					}

					if (blocked) {
						// test whether B4 holds
						CReapplyConceptLabelSetIterator blockerConLabSetIt = wPredSuperConSet->getConceptLabelSetIterator(false,false,false);
						while (blockerConLabSetIt.hasNext() && blocked) {
							CConceptDescriptor* conDes = blockerConLabSetIt.next();
							CConcept* concept = conDes->getConcept();
							bool conNeg = conDes->isNegated();
							cint64 conOpCode = concept->getOperatorCode();
							CRole* role = concept->getRole();
							cint64 cardinality = concept->getParameter();
							CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
							bool needsRestrictionTest = false;
							bool negateOps = false;
							if (!conNeg && conOpCode == CCATLEAST || conNeg && conOpCode == CCATMOST) {
								if (conNeg) {
									++cardinality;
								}
								needsRestrictionTest = true;
							} else if (!conNeg && (conOpCode == CCSOME || conOpCode == CCAQSOME) || conNeg && conOpCode == CCALL) {
								cardinality = 1;
								needsRestrictionTest = true;
								negateOps = conNeg;
							}

							if (needsRestrictionTest) {
								// B4b
								bool restrictionHolds = false;
								if (hasIndividualsLink(wNode,vNode,role,false,calcAlgContext)) {
									if (containsIndividualNodeConcepts(vConSet,opLinker,negateOps,calcAlgContext)) {
										restrictionHolds = true;
									}
								}
								if (!restrictionHolds) {
									// B4a
									// w' needs m role-successors
									cint64 minRoleCardinality = 0;
									if (wPredSuccHash) {
										CRoleSuccessorLinkIterator succIt = wPredSuccHash->getRoleSuccessorLinkIterator(role);
										while (succIt.hasNext()) {
											CIndividualLinkEdge* link = succIt.next();
											CIndividualProcessNode* succIndi = getSuccessorIndividual(wPredNode,link,calcAlgContext);
											if (succIndi->getIndividualAncestorDepth() > wPredNode->getIndividualAncestorDepth()) {
												if (!opLinker) {
													++minRoleCardinality;
												} else {
													if (containsIndividualNodeConcepts(succIndi,opLinker,negateOps,calcAlgContext)) {
														++minRoleCardinality;
													}
												}
											}
										}
									}
									if (minRoleCardinality < cardinality) {
										blocked = false;
									}
								}
							}
						}
					}


					if (!blocked && blockAltData && mConfSignatureMirroringBlocking && mOptSignatureMirroringBlockingInBlocking) {
						cint64 diffConceptCount = wPredSuperConSet->getConceptCount() - wSubConSet->getConceptCount();

						CBlockingAlternativeSignatureBlockingCandidateData* sigBlockCandData = nullptr;
						if (*blockAltData) {
							sigBlockCandData = dynamic_cast<CBlockingAlternativeSignatureBlockingCandidateData*>(*blockAltData);
						}
						if (!sigBlockCandData) {
							CMemoryAllocationManager* tempMemMan = calcAlgContext->getUsedTemporaryMemoryAllocationManager();
							sigBlockCandData = CObjectAllocator<CBlockingAlternativeSignatureBlockingCandidateData>::allocateAndConstruct(tempMemMan);
							sigBlockCandData->initSignatureBlockingCandidateData(wPredNode,violatingB2Restrictions,violatingNonDetB2Restrictions,diffConceptCount);
							*blockAltData = sigBlockCandData;
						} else {
							bool lessViolNonDetRes = violatingNonDetB2Restrictions < sigBlockCandData->getViolatedNonDeterministicRestrictionCount();
							bool lessViolRes = violatingB2Restrictions < sigBlockCandData->getViolatedRestrictionCount();
							bool lessConcepts = diffConceptCount < sigBlockCandData->getConceptDifferenceCount();

							double newScore = violatingNonDetB2Restrictions*1.2 + violatingB2Restrictions + diffConceptCount*0.1;
							double oldScore = sigBlockCandData->getViolatedNonDeterministicRestrictionCount()*1.2 + sigBlockCandData->getViolatedRestrictionCount() + sigBlockCandData->getConceptDifferenceCount()*0.1;

							if (newScore > oldScore) {
								sigBlockCandData->initSignatureBlockingCandidateData(wPredNode,violatingB2Restrictions,violatingNonDetB2Restrictions,diffConceptCount);
							}
						}
						


					}

					return blocked;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::containsIndividualNodeConcepts(CReapplyConceptLabelSet* conLabelSet, CSortedNegLinker<CConcept*>* conTestLinkerIt, bool* containsNegation, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool containsAllNegated = true;
					bool containsAllNonNegated = true;
					if (!conTestLinkerIt) {
						// interpret as top
						if (containsNegation) {
							*containsNegation = false;
						}
						containsAllNegated = false; 
					}
					while (conTestLinkerIt && (containsAllNegated || containsAllNonNegated)) {
						CConcept* concept = conTestLinkerIt->getData();
						bool conNeg = conTestLinkerIt->isNegated();
						bool containsNeg = false;
						if (!conLabelSet->containsConcept(concept,&containsNeg)) {
							return false;
						}
						if (containsNeg == conNeg) {
							containsAllNegated = false;
						} else {
							containsAllNonNegated = false;
						}
						conTestLinkerIt = conTestLinkerIt->getNext();
					}
					if (containsNegation) {
						if (containsAllNegated) {
							*containsNegation = true;
							return true;
						} 
						if (containsAllNonNegated) {
							*containsNegation = false;
							return true;
						}
					}
					if (containsAllNonNegated) {
						return true;
					}
					if (containsAllNegated) {
						return true;
					}
					return false;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::containsIndividualNodeConcepts(CReapplyConceptLabelSet* conLabelSet, CSortedNegLinker<CConcept*>* conTestLinkerIt, bool negated, CCalculationAlgorithmContextBase* calcAlgContext) {
					while (conTestLinkerIt) {
						CConcept* concept = conTestLinkerIt->getData();
						bool conNeg = conTestLinkerIt->isNegated() ^ negated;
						if (!conLabelSet->containsConcept(concept,conNeg)) {
							return false;
						}
						conTestLinkerIt = conTestLinkerIt->getNext();
					}
					return true;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::containsIndividualNodeConcepts(CIndividualProcessNode*& testIndi, CSortedNegLinker<CConcept*>* conTestLinkerIt, bool negated, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReapplyConceptLabelSet* conLabelSet = testIndi->getReapplyConceptLabelSet(false);
					return containsIndividualNodeConcepts(conLabelSet,conTestLinkerIt,negated,calcAlgContext);
				}

				bool CCalculationTableauCompletionTaskHandleAlgorithm::containsIndividualNodeConcepts(CIndividualProcessNode*& testIndi, CSortedNegLinker<CConcept*>* conTestLinkerIt, bool* containsNegation, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReapplyConceptLabelSet* conLabelSet = testIndi->getReapplyConceptLabelSet(false);
					return containsIndividualNodeConcepts(conLabelSet,conTestLinkerIt,containsNegation,calcAlgContext);
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::containsIndividualNodeConcepts(CIndividualProcessNode*& testIndi, CSortedNegLinker<CConcept*>* conTestLinkerIt, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReapplyConceptLabelSet* conLabelSet = testIndi->getReapplyConceptLabelSet(false);
					return containsIndividualNodeConcepts(conLabelSet,conTestLinkerIt,(bool*)nullptr,calcAlgContext);
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::isLabelConceptSubSetBlocking(CIndividualProcessNode*& testIndi, CIndividualProcessNode*& blockingIndi, CIndividualNodeBlockingTestData* blockData, bool testContinueBlocking, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(SUBSETBLOCKINGTESTCOUNT,calcAlgContext);
					if (testContinueBlocking) {
						// is still sub set when added something to blocker individual node
						return true;
					}
					CReapplyConceptLabelSet* subConSet = testIndi->getReapplyConceptLabelSet(false);
					CReapplyConceptLabelSet* superConSet = blockingIndi->getReapplyConceptLabelSet(false);
					CConceptDescriptor* firstNotEntailedConDes = nullptr;
					bool subSet = isLabelConceptSubSet(subConSet,superConSet,&firstNotEntailedConDes,nullptr,calcAlgContext);
					return subSet;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::isLabelConceptEqualBlocking(CIndividualProcessNode*& testIndi, CIndividualProcessNode*& blockingIndi, CIndividualNodeBlockingTestData* blockData, bool testContinueBlocking, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(EQUALSETBLOCKINGTESTCOUNT,calcAlgContext);
					CReapplyConceptLabelSet* conSet1 = testIndi->getReapplyConceptLabelSet(false);
					CReapplyConceptLabelSet* conSet2 = blockingIndi->getReapplyConceptLabelSet(false);
					return isLabelConceptEqualSet(conSet1,conSet2,calcAlgContext);
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::isLabelConceptEqualPairwiseBlocking(CIndividualProcessNode*& testIndi, CIndividualProcessNode*& blockingIndi, CIndividualNodeBlockingTestData* blockData, bool testContinueBlocking, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(PAIRWISEEQUALSETBLOCKINGTESTCOUNT,calcAlgContext);
					CIndividualProcessNode* ancTestIndi = getAncestorIndividual(testIndi,calcAlgContext);
					if (!ancTestIndi) {
						return false;
					}
					CIndividualProcessNode* ancBlockingIndi = getAncestorIndividual(blockingIndi,calcAlgContext);
					if (!ancBlockingIndi) {
						return false;
					}
					CIndividualLinkEdge* testIndiAncLink = testIndi->getAncestorLink();
					CIndividualLinkEdge* blockingIndiAncLink = testIndi->getAncestorLink();
					if (testIndiAncLink->getLinkRole() != blockingIndiAncLink->getLinkRole()) {
						return false;
					}
					CReapplyConceptLabelSet* conSet1 = testIndi->getReapplyConceptLabelSet(false);
					CReapplyConceptLabelSet* conSet1Pair = blockingIndi->getReapplyConceptLabelSet(false);
					CReapplyConceptLabelSet* conSet2 = ancTestIndi->getReapplyConceptLabelSet(false);
					CReapplyConceptLabelSet* conSet2Pair = ancBlockingIndi->getReapplyConceptLabelSet(false);
					return isPairwiseLabelConceptEqualSet(conSet1,conSet1Pair,conSet2,conSet2Pair,calcAlgContext);
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::isIndividualNodeBlocking(CIndividualProcessNode*& testIndi, CIndividualProcessNode*& blockingIndi, CIndividualNodeBlockingTestData* blockData, bool testContinueBlocking, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(INDINODEBLOCKINGTESTCOUNT,calcAlgContext);
					// TODO: check config, first test concept set sizes

					CReapplyConceptLabelSet* testConSet = testIndi->getReapplyConceptLabelSet(false);
					CReapplyConceptLabelSet* blockingConSet = blockingIndi->getReapplyConceptLabelSet(false);
					cint64 testConSetCount = testConSet->getConceptCount();
					cint64 blockingConSetCount = blockingConSet->getConceptCount();
					if (testConSetCount > blockingConSetCount) {
						return false;
					}

					if (!testContinueBlocking) {
						CConceptDescriptor* initConDes = testIndi->getIndividualInitializationConcept();
						if (initConDes) {
							if (!blockingIndi->getReapplyConceptLabelSet(false)->containsConceptDescriptor(initConDes)) {
								return false;
							}
						}
					}



					bool testProcessingBlocking = false;
					if (mOptDetExpPreporcessing) {
						testProcessingBlocking = true;
					}


					bool blockingConcepts = false;

					if (testProcessingBlocking || mConfSubSetBlocking) {
						if (isLabelConceptSubSetBlocking(testIndi,blockingIndi,blockData,testContinueBlocking,blockAltData,calcAlgContext)) {
							blockingConcepts = true;
						}
					} else if (mConfOptimizedSubSetBlocking) {
						if (isLabelConceptOptimizedBlocking(testIndi,blockingIndi,blockData,testContinueBlocking,blockAltData,calcAlgContext)) {
							blockingConcepts = true;
						}
					} else if (mConfEqualSetBlocking) {
						if (isLabelConceptEqualBlocking(testIndi,blockingIndi,blockData,testContinueBlocking,blockAltData,calcAlgContext)) {
							blockingConcepts = true;
						}
					} else if (mConfPairwiseEqualSetBlocking) {
						if (isLabelConceptEqualPairwiseBlocking(testIndi,blockingIndi,blockData,testContinueBlocking,blockAltData,calcAlgContext)) {
							blockingConcepts = true;
						}
					}

					bool blockingPropBindings = false;
					if (blockingConcepts) {
						blockingPropBindings = isNominalVariablePropagationBindingSubSet(testIndi,blockingIndi,blockData,testContinueBlocking,blockAltData,calcAlgContext);
					}

					return blockingConcepts && blockingPropBindings;
				}





				bool CCalculationTableauCompletionTaskHandleAlgorithm::detectIndividualNodeBlockedStatus(CIndividualProcessNode*& testIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(DETECTINDINODEBLOCKINGSTATUSCOUNT,calcAlgContext);
					bool previousProcessingBlocked = false;
					bool previousBlocked = testIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFDIRECTBLOCKED | CIndividualProcessNode::PRFINDIRECTBLOCKED | CIndividualProcessNode::PRFPROCESSINGBLOCKED);
					if (!mOptDetExpPreporcessing && testIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPROCESSINGBLOCKED)) {
						testIndi->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFPROCESSINGBLOCKED);
						previousProcessingBlocked = true;
					} else if (mOptDetExpPreporcessing && testIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPROCESSINGBLOCKED)) {
						return true;
					} else {
						if (!testIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFBLOCKINGRETESTDUEANCESTORMODIFIED | CIndividualProcessNode::PRFBLOCKINGRETESTDUEDIRECTMODIFIED | CIndividualProcessNode::PRFBLOCKINGRETESTDUEBLOCKERMODIFIED | CIndividualProcessNode::PRFBLOCKINGRETESTDUEINDIRECTBLOCKERLOSS)) {
							return previousBlocked;
						}
					}


					bool previousIndirectBlocked = testIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINDIRECTBLOCKED);
					if (previousIndirectBlocked) {
						if (!testIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFBLOCKINGRETESTDUEINDIRECTBLOCKERLOSS)) {
							testIndi->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFBLOCKINGRETESTDUEANCESTORMODIFIED | CIndividualProcessNode::PRFBLOCKINGRETESTDUEDIRECTMODIFIED | CIndividualProcessNode::PRFBLOCKINGRETESTDUEBLOCKERMODIFIED);
							return true;
						}
					}

					bool previousDirectBlocked = testIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFDIRECTBLOCKED);

					CIndividualProcessNode* ancTestIndi = testIndi;
					CIndividualProcessNode* blockingIndi = nullptr;

					CBlockingAlternativeData* blockAltData = nullptr;

					bool blocked = false;




					// test each modified ancestor
					while (!blocked && ancTestIndi && ancTestIndi->isBlockableIndividual() && 
								ancTestIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFBLOCKINGRETESTDUEANCESTORMODIFIED | CIndividualProcessNode::PRFBLOCKINGRETESTDUEDIRECTMODIFIED | CIndividualProcessNode::PRFBLOCKINGRETESTDUEBLOCKERMODIFIED | CIndividualProcessNode::PRFBLOCKINGRETESTDUEINDIRECTBLOCKERLOSS) && 
								!ancTestIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINVALIDBLOCKINGORCACHING)) {
						STATINC(DETECTANCINDINODEBLOCKINGSTATUSCOUNT,calcAlgContext);
						CIndividualProcessNode* locAncTestIndi = getLocalizedIndividual(ancTestIndi,false,calcAlgContext);
						
						// search blocker node
						blockAltData = nullptr;
						blockingIndi = nullptr;

						if (mConfSaturationCachingTestingDuringBlockingTests) {
							detectIndividualNodeSaturationCached(locAncTestIndi,calcAlgContext);
						}

						bool skipBlockerSearch = false;
						if (locAncTestIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONBLOCKINGCACHED) && locAncTestIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONSUCCESSORCREATIONBLOCKINGCACHED)) {
							skipBlockerSearch = true;
						}

						if (!skipBlockerSearch) {
							blockingIndi = getBlockingIndividualNode(locAncTestIndi,&blockAltData,calcAlgContext);
						}

						locAncTestIndi->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFBLOCKINGRETESTDUEANCESTORMODIFIED | CIndividualProcessNode::PRFBLOCKINGRETESTDUEDIRECTMODIFIED | CIndividualProcessNode::PRFBLOCKINGRETESTDUEBLOCKERMODIFIED 
								| CIndividualProcessNode::PRFBLOCKINGRETESTDUEINDIRECTBLOCKERLOSS | CIndividualProcessNode::PRFDIRECTBLOCKED | CIndividualProcessNode::PRFINDIRECTBLOCKED | CIndividualProcessNode::PRFPROCESSINGBLOCKED);

						if (!blockingIndi) {
							STATINC(FAILEDBLOCKINGSTATUSDETECTIONCOUNT,calcAlgContext);
							if (blockAltData) {
								blocked = testAlternativeBlocked(locAncTestIndi,blockAltData,calcAlgContext);
							}
							if (!blocked) {
								ancTestIndi = getAncestorIndividual(locAncTestIndi,calcAlgContext);
							}
						} else {
							STATINC(SUCCESSBLOCKINGSTATUSDETECTIONCOUNT,calcAlgContext);

							//if (blockingIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION)) {
							//	propagateIndividualNodeNominalConnectionToAncestors(locAncTestIndi,calcAlgContext);
							//}
							if (blockingIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION)) {
								propagateIndividualNodeNominalConnectionStatusToAncestors(locAncTestIndi,blockingIndi,calcAlgContext);
							}


							bool testProcessingBlocking = false;
							if (mOptDetExpPreporcessing) {
								testProcessingBlocking = true;
							}


							locAncTestIndi->mDebugBlockerIndi = blockingIndi;
							locAncTestIndi->mDebugBlockerLastConceptDes = blockingIndi->getReapplyConceptLabelSet(false)->getAddingSortedConceptDescriptionLinker();


							if (!testProcessingBlocking) {
								CIndividualProcessNode* locBlockingNode = getLocalizedIndividual(blockingIndi,false,calcAlgContext);
								CXLinker<CIndividualProcessNode*>* blockedIndiNodeLinker = CObjectAllocator< CXLinker<CIndividualProcessNode*> >::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
								blockedIndiNodeLinker->initLinker(locAncTestIndi);
								locBlockingNode->addBlockedIndividualsLinker(blockedIndiNodeLinker);
								locAncTestIndi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFDIRECTBLOCKED);
							} else {
								locAncTestIndi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFPROCESSINGBLOCKED);
								addIndividualToBlockingUpdateReviewProcessingQueue(blockingIndi,calcAlgContext);
							}
							propagateIndirectSuccessorBlocking(locAncTestIndi,calcAlgContext);
							blocked = true;
						}
					}

					if (!blockingIndi && previousBlocked) {
						reactivateIndirectBlockedSuccessors(testIndi,false,calcAlgContext);
					}
					return blocked;
				}


				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::getBlockingIndividualNode(CIndividualProcessNode* blockingTestIndi, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualProcessNode* blockingIndi = nullptr;
					if (!blockingIndi && mConfAnywhereBlockingLinkedCandidateHashSearch) {
						blockingIndi = getAnywhereBlockingIndividualNodeLinkedCanidateHashed(blockingTestIndi,blockAltData,calcAlgContext);
					}
					if (!blockingIndi && mConfAnywhereBlockingCandidateHashSearch) {
						blockingIndi = getAnywhereBlockingIndividualNodeCanidateHashed(blockingTestIndi,blockAltData,calcAlgContext);
					}
					if (!blockingIndi && mConfAnywhereBlockingSearch) {
						blockingIndi = getAnywhereBlockingIndividualNode(blockingTestIndi,blockAltData,calcAlgContext);
					} 
					if (!blockingIndi && mConfAncestorBlockingSearch) {
						blockingIndi = getAncestorBlockingIndividualNode(blockingTestIndi,blockAltData,calcAlgContext);
					}
					return blockingIndi;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::continueIndividualNodeBlock(CIndividualProcessNode*& indi, CIndividualNodeBlockingTestData* blockData, CIndividualProcessNode*& blockerIndiNode, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualProcessNode* blockingIndiNode = nullptr;
					if (blockData) {
						blockingIndiNode = blockData->getBlockingIndividualNode();
						if (blockingIndiNode) {
							STATINC(CONTINUEBLOCKINGTESTCOUNT,calcAlgContext);
							blockingIndiNode = getUpToDateIndividual(blockingIndiNode,calcAlgContext);
							if (blockingIndiNode->isBlockableIndividual() && isIndividualNodeValidBlocker(blockingIndiNode,calcAlgContext)) {
								if (isIndividualNodeBlocking(indi,blockingIndiNode,blockData,true,blockAltData,calcAlgContext)) {
									blockerIndiNode = blockingIndiNode;
									return true;
								} else {
									blockData->clearBlockingIndividualNode();
								}
							}
						}
					}
					return false;
				}




				bool CCalculationTableauCompletionTaskHandleAlgorithm::signatureCachedIndividualNodeBlock(CIndividualProcessNode*& indi, CIndividualNodeBlockingTestData* blockData, CIndividualProcessNode*& blockerIndiNode, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSIGNATUREBLOCKINGCACHED)) {
						CSignatureBlockingIndividualNodeConceptExpansionData* sigBlockExpData = indi->getSignatureBlockingIndividualNodeConceptExpansionData(false);
						if (sigBlockExpData) {
							CIndividualProcessNode* blockingIndiNode = sigBlockExpData->getBlockerIndividualNode();
							if (blockingIndiNode) {
								blockingIndiNode = getUpToDateIndividual(blockingIndiNode,calcAlgContext);
								if (blockingIndiNode->isBlockableIndividual() && isIndividualNodeValidBlocker(blockingIndiNode,calcAlgContext)) {
									if (isIndividualNodeBlocking(indi,blockingIndiNode,blockData,false,blockAltData,calcAlgContext)) {
										blockerIndiNode = blockingIndiNode;
										return true;
									}
								}
							}
						}
					}
					return false;
				}



				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::getAncestorBlockingIndividualNode(CIndividualProcessNode* blockingTestIndi, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					CIndividualProcessNode* blockerNode = nullptr;
					CNodeSwitchHistory* nodeSwitchHistory = calcAlgContext->getUsedProcessingDataBox()->getNodeSwitchHistory(false);
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CIndividualNodeBlockingTestData* blockData = (CIndividualNodeBlockingTestData*)blockingTestIndi->getIndividualBlockData(false);
					CIndividualNodeBlockingTestData* locBlockData = (CIndividualNodeBlockingTestData*)blockingTestIndi->getIndividualBlockData(true);
					if (!locBlockData) {
						locBlockData = CObjectAllocator<CIndividualNodeBlockingTestData>::allocateAndConstruct(taskMemMan);
						locBlockData->initBlockData(blockData);
						blockingTestIndi->setIndividualBlockData(locBlockData);
					}
					cint64 prevNodeSwitchTag = locBlockData->getNodeSwitchTag();
					cint64 prevNodeConceptLabelModTag = locBlockData->getConceptLabelSetModificationTag();
					cint64 minTestIndiNodeID = 0;
					cint64 minTestAncIndiDepth = 0;
					locBlockData->updateNodeSwitchTag(calcAlgContext->getUsedProcessTagger());
					locBlockData->updateConceptLabelSetModificationTag(calcAlgContext->getUsedProcessTagger());

					CIndividualProcessNode* continueBlockingIndiNode = nullptr;
					if (continueIndividualNodeBlock(blockingTestIndi,locBlockData,continueBlockingIndiNode,blockAltData,calcAlgContext)) {
						blockerNode = continueBlockingIndiNode;
					} else {
						if (signatureCachedIndividualNodeBlock(blockingTestIndi,locBlockData,continueBlockingIndiNode,blockAltData,calcAlgContext)) {	
							blockerNode = continueBlockingIndiNode;
						} else {
							if (nodeSwitchHistory && locBlockData && prevNodeSwitchTag > 0) {
								nodeSwitchHistory->getMinIndividualAncestorDepthAndNodeID(prevNodeSwitchTag,minTestAncIndiDepth,minTestIndiNodeID);
								minTestIndiNodeID = qMax(minTestIndiNodeID,(cint64)0);
								minTestAncIndiDepth = qMax(minTestAncIndiDepth,(cint64)0);
							}
							CIndividualProcessNode* ancIndiNode = getAncestorIndividual(blockingTestIndi,calcAlgContext);
							while (!blockerNode && ancIndiNode && isIndividualNodeValidBlocker(ancIndiNode,calcAlgContext) && ancIndiNode->getIndividualAncestorDepth() >= minTestAncIndiDepth) {
								if (continueBlockingIndiNode != ancIndiNode) {
									STATINC(ANCESTORBLOCKINGSEARCHINDINODECOUNT,calcAlgContext);
									if (isIndividualNodeConceptLabelSetModified(ancIndiNode,prevNodeConceptLabelModTag,calcAlgContext) && isIndividualNodeBlocking(blockingTestIndi,ancIndiNode,locBlockData,false,blockAltData,calcAlgContext)) {
										blockerNode = ancIndiNode;
									}
								}
								if (!blockerNode) {
									ancIndiNode = getAncestorIndividual(ancIndiNode,calcAlgContext);
								}
							}
						}
					}
					locBlockData->setBlockingIndividualNode(blockerNode);
					return blockerNode;
				}


				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::getAnywhereBlockingIndividualNode(CIndividualProcessNode* blockingTestIndi, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					CIndividualProcessNode* blockerNode = nullptr;
					CNodeSwitchHistory* nodeSwitchHistory = calcAlgContext->getUsedProcessingDataBox()->getNodeSwitchHistory(false);
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CIndividualNodeBlockingTestData* blockData = (CIndividualNodeBlockingTestData*)blockingTestIndi->getIndividualBlockData(false);
					CIndividualNodeBlockingTestData* locBlockData = (CIndividualNodeBlockingTestData*)blockingTestIndi->getIndividualBlockData(true);
					if (!locBlockData) {
						locBlockData = CObjectAllocator<CIndividualNodeBlockingTestData>::allocateAndConstruct(taskMemMan);
						locBlockData->initBlockData(blockData);
						blockingTestIndi->setIndividualBlockData(locBlockData);
					}
					cint64 prevNodeSwitchTag = locBlockData->getNodeSwitchTag();
					cint64 prevNodeConceptLabelModTag = locBlockData->getConceptLabelSetModificationTag();
					cint64 minTestIndiNodeID = 0;
					cint64 minTestAncIndiDepth = 0;
					locBlockData->updateNodeSwitchTag(calcAlgContext->getUsedProcessTagger());
					locBlockData->updateConceptLabelSetModificationTag(calcAlgContext->getUsedProcessTagger());

					CIndividualProcessNode* continueBlockingIndiNode = nullptr;
					if (continueIndividualNodeBlock(blockingTestIndi,locBlockData,continueBlockingIndiNode,blockAltData,calcAlgContext)) {
						blockerNode = continueBlockingIndiNode;
					} else {
						if (signatureCachedIndividualNodeBlock(blockingTestIndi,locBlockData,continueBlockingIndiNode,blockAltData,calcAlgContext)) {	
							blockerNode = continueBlockingIndiNode;
						} else {
							cint64 continueBlockingIndiNodeID = -1;
							if (continueBlockingIndiNode) {
								continueBlockingIndiNodeID = continueBlockingIndiNode->getIndividualID();
							}
							if (nodeSwitchHistory && locBlockData && prevNodeSwitchTag > 0) {
								nodeSwitchHistory->getMinIndividualAncestorDepthAndNodeID(prevNodeSwitchTag,minTestAncIndiDepth,minTestIndiNodeID);
								minTestIndiNodeID = qMax(minTestIndiNodeID,(cint64)0);
								minTestAncIndiDepth = qMax(minTestAncIndiDepth,(cint64)0);
							}
							cint64 prevIndiID = blockingTestIndi->getIndividualID() - 1;
							CIndividualProcessNode* prevIndiNode = nullptr;
							while (!blockerNode && prevIndiID > 0 && prevIndiID >= minTestIndiNodeID) {
								if (prevIndiID != continueBlockingIndiNodeID) {
									prevIndiNode = getUpToDateIndividual(prevIndiID,calcAlgContext);
									if (prevIndiNode && isIndividualNodeValidBlocker(prevIndiNode,calcAlgContext) && isIndividualNodeConceptLabelSetModified(prevIndiNode,prevNodeConceptLabelModTag,calcAlgContext)) {
										STATINC(ANYWHEREBLOCKINGSEARCHINDINODECOUNT,calcAlgContext);
										if (isIndividualNodeBlocking(blockingTestIndi,prevIndiNode,locBlockData,false,blockAltData,calcAlgContext)) {
											blockerNode = prevIndiNode;
										}
									}
								}
								--prevIndiID;
							}
						}
					}
					locBlockData->setBlockingIndividualNode(blockerNode);
					return blockerNode;
				}



				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::getAnywhereBlockingIndividualNodeLinkedCanidateHashed(CIndividualProcessNode* blockingTestIndi, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CIndividualProcessNode* blockerNode = nullptr;
					CIndividualNodeBlockingTestData* blockData = (CIndividualNodeBlockingTestData*)blockingTestIndi->getIndividualBlockData(false);
					CIndividualNodeBlockingTestData* locBlockData = (CIndividualNodeBlockingTestData*)blockingTestIndi->getIndividualBlockData(true);
					if (!locBlockData) {
						locBlockData = CObjectAllocator<CIndividualNodeBlockingTestData>::allocateAndConstruct(taskMemMan);
						locBlockData->initBlockData(blockData);
						blockingTestIndi->setIndividualBlockData(locBlockData);
					}

					CIndividualProcessNode* continueBlockingIndiNode = nullptr;
					if (continueIndividualNodeBlock(blockingTestIndi,locBlockData,continueBlockingIndiNode,blockAltData,calcAlgContext)) {
						blockerNode = continueBlockingIndiNode;
					} else {
						if (signatureCachedIndividualNodeBlock(blockingTestIndi,locBlockData,continueBlockingIndiNode,blockAltData,calcAlgContext)) {	
							blockerNode = continueBlockingIndiNode;
						} else {
							cint64 continueBlockingIndiNodeID = -1;
							if (continueBlockingIndiNode) {
								continueBlockingIndiNodeID = continueBlockingIndiNode->getIndividualID();
							}

							CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
							CBlockingIndividualNodeLinkedCandidateHash* blockingCandHash = procDataBox->getBlockingIndividualNodeLinkedCandidateHash(false);
							CReapplyConceptLabelSet* conSet = blockingTestIndi->getReapplyConceptLabelSet(false);
							CCoreConceptDescriptor* coreConDesLinker = conSet->getCoreConceptDescriptorLinker();
							cint64 blockingTestIndiID = blockingTestIndi->getIndividualID();


							if (!coreConDesLinker) {

								blockerNode = getAnywhereBlockingIndividualNodeCanidateHashed(blockingTestIndi,blockAltData,calcAlgContext);

							} else {

								CBlockingIndividualNodeLinkedCandidateData* minBlockingIndNodeCandData = nullptr;
								cint64 minBlockingIndNodeCandDataCount = 0;
								CConceptDescriptor* minBlockingConDes = nullptr;

								CCoreConceptDescriptor* lastAddedCoreConDes = locBlockData->getLastAddedCoreConceptDescriptor();
								CConceptDescriptor* lastConDes = locBlockData->getLastCoreBlockingCandidateConceptDescriptor();
								cint64 lastNodeDiff = locBlockData->getLastCoreBlockingCandidateConceptNodeDifference();
								if (lastAddedCoreConDes != coreConDesLinker) {
									lastConDes = nullptr;
								}

								CBlockingIndividualNodeLinkedCandidateData* lastMinBlockingIndNodeCandData = nullptr;

								if (lastConDes) {
									CBlockingIndividualNodeLinkedCandidateData* blockingCandData = blockingCandHash->getBlockingIndividualCandidateData(lastConDes,false);
									cint64 blockingIndNodeCandDataCount = blockingCandData->getCandidateCount();
									if (blockingIndNodeCandDataCount <= lastNodeDiff) {
										minBlockingIndNodeCandData = blockingCandData;
										minBlockingIndNodeCandDataCount = blockingIndNodeCandDataCount;
									}
								}

								if (!minBlockingIndNodeCandData) {

									for (CCoreConceptDescriptor* coreConDesLinkerIt = coreConDesLinker; coreConDesLinkerIt; coreConDesLinkerIt = coreConDesLinkerIt->getNext()) {
										CConceptDescriptor* conDes = coreConDesLinkerIt->getConceptDesciptor();
										CBlockingIndividualNodeLinkedCandidateData* blockingCandData = blockingCandHash->getBlockingIndividualCandidateData(conDes,false);
										if (blockingCandData) {
											cint64 blockingIndNodeCandDataCount = blockingCandData->getCandidateCount();
											if (!minBlockingIndNodeCandData || blockingIndNodeCandDataCount < minBlockingIndNodeCandDataCount) {

												lastMinBlockingIndNodeCandData = minBlockingIndNodeCandData;

												minBlockingIndNodeCandDataCount = blockingIndNodeCandDataCount;
												minBlockingIndNodeCandData = blockingCandData;
												minBlockingConDes = conDes;
											} else if (!lastMinBlockingIndNodeCandData) {
												lastMinBlockingIndNodeCandData = blockingCandData;
											}
										}
									}
								}

								locBlockData->setLastAddedCoreConceptDescriptor(coreConDesLinker);
								locBlockData->setLastCoreBlockingCandidateConceptDescriptor(minBlockingConDes);
								locBlockData->setLastCoreBlockingCandidateConceptNodeDifference(0);
								if (lastMinBlockingIndNodeCandData) {
									cint64 diffCount = lastMinBlockingIndNodeCandData->getCandidateCount();
									locBlockData->setLastCoreBlockingCandidateConceptNodeDifference(diffCount);
								}

								CBlockingIndividualNodeLinkedCandidateData* blockingCandData = minBlockingIndNodeCandData;
								if (blockingCandData) {
									CBlockingIndividualNodeLinker* blockingIndNodeLinker = blockingCandData->getBlockingCandidatesIndividualNodeLinker();
									while (blockingIndNodeLinker && !blockerNode) {
										CIndividualProcessNode* blockerCandIndiNode = blockingIndNodeLinker->getCandidateIndividualNode();
										cint64 blockerCandIndiNodeID = blockerCandIndiNode->getIndividualID();

										if (blockerCandIndiNodeID != continueBlockingIndiNodeID && blockerCandIndiNodeID != blockingTestIndiID) {

											STATINC(ANYWHERECORECONCEPTBLOCKINGCANDIDATEHASHSEARCHINDINODECOUNT,calcAlgContext);
											CIndividualProcessNode* upBlockerCandIndiNode = getUpToDateIndividual(blockerCandIndiNode,calcAlgContext);

											if (isIndividualNodeValidBlocker(upBlockerCandIndiNode,calcAlgContext)) {
												if (isIndividualNodeBlocking(blockingTestIndi,upBlockerCandIndiNode,locBlockData,false,blockAltData,calcAlgContext)) {
													blockerNode = upBlockerCandIndiNode;
												}
											}

										}

										blockingIndNodeLinker = blockingIndNodeLinker->getNext();
									}
								}

							}
						}
					}

					return blockerNode;
				}



				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::getAnywhereBlockingIndividualNodeCanidateHashed(CIndividualProcessNode* blockingTestIndi, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					CIndividualProcessNode* blockerNode = nullptr;
					CNodeSwitchHistory* nodeSwitchHistory = calcAlgContext->getUsedProcessingDataBox()->getNodeSwitchHistory(false);
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CIndividualNodeBlockingTestData* blockData = (CIndividualNodeBlockingTestData*)blockingTestIndi->getIndividualBlockData(false);
					CIndividualNodeBlockingTestData* locBlockData = (CIndividualNodeBlockingTestData*)blockingTestIndi->getIndividualBlockData(true);
					if (!locBlockData) {
						locBlockData = CObjectAllocator<CIndividualNodeBlockingTestData>::allocateAndConstruct(taskMemMan);
						locBlockData->initBlockData(blockData);
						blockingTestIndi->setIndividualBlockData(locBlockData);
					}
					cint64 prevNodeSwitchTag = locBlockData->getNodeSwitchTag();
					cint64 prevNodeConceptLabelModTag = locBlockData->getConceptLabelSetModificationTag();
					cint64 minTestIndiNodeID = 0;
					cint64 minTestAncIndiDepth = 0;
					locBlockData->updateNodeSwitchTag(calcAlgContext->getUsedProcessTagger());
					locBlockData->updateConceptLabelSetModificationTag(calcAlgContext->getUsedProcessTagger());

					CIndividualProcessNode* continueBlockingIndiNode = nullptr;
					if (continueIndividualNodeBlock(blockingTestIndi,locBlockData,continueBlockingIndiNode,blockAltData,calcAlgContext)) {
						blockerNode = continueBlockingIndiNode;
					} else {
						cint64 continueBlockingIndiNodeID = -1;
						if (continueBlockingIndiNode) {
							continueBlockingIndiNodeID = continueBlockingIndiNode->getIndividualID();
						}
						if (nodeSwitchHistory && locBlockData && prevNodeSwitchTag > 0) {
							nodeSwitchHistory->getMinIndividualAncestorDepthAndNodeID(prevNodeSwitchTag,minTestAncIndiDepth,minTestIndiNodeID);
							minTestIndiNodeID = qMax(minTestIndiNodeID,(cint64)0);
							minTestAncIndiDepth = qMax(minTestAncIndiDepth,(cint64)0);
						}
						if (blockingTestIndi->getIndividualInitializationConcept()) {
							CBlockingIndividualNodeCandidateIterator indiNodeCandIt(getBlockingIndividualNodeCandidateIterator(blockingTestIndi,calcAlgContext));
							while (!blockerNode && indiNodeCandIt.hasNext()) {
								CIndividualProcessNode* indiNode = indiNodeCandIt.nextIndividualCandidate(true);
								CIndividualProcessNode* upIndiNode = getUpToDateIndividual(indiNode,calcAlgContext);
								cint64 upIndiNodeID = upIndiNode->getIndividualID();
								if (upIndiNodeID != continueBlockingIndiNodeID) {
									if (upIndiNode->hasPurgedBlockedProcessingRestrictionFlags() || !upIndiNode->isBlockableIndividual()) {
										STATINC(ANYWHEREBLOCKINGCANDIDATEHASHUDATEREMOVECOUNT,calcAlgContext);
										indiNodeCandIt.removeLastIndividualCandidate();
									} else if (isIndividualNodeValidBlocker(upIndiNode,calcAlgContext) && isIndividualNodeConceptLabelSetModified(upIndiNode,prevNodeConceptLabelModTag,calcAlgContext)) {
										STATINC(ANYWHEREBLOCKINGCANDIDATEHASHSEARCHINDINODECOUNT,calcAlgContext);
										if (isIndividualNodeBlocking(blockingTestIndi,upIndiNode,locBlockData,false,blockAltData,calcAlgContext)) {
											blockerNode = upIndiNode;
										}
									}
								}
							}
						} else {
							cint64 prevIndiID = blockingTestIndi->getIndividualID() - 1;
							CIndividualProcessNode* prevIndiNode = nullptr;
							while (!blockerNode && prevIndiID > 0 && prevIndiID >= minTestIndiNodeID) {
								if (prevIndiID != continueBlockingIndiNodeID) {
									prevIndiNode = getUpToDateIndividual(prevIndiID,calcAlgContext);
									if (prevIndiNode && isIndividualNodeValidBlocker(prevIndiNode,calcAlgContext) && isIndividualNodeConceptLabelSetModified(prevIndiNode,prevNodeConceptLabelModTag,calcAlgContext)) {
										STATINC(ANYWHEREBLOCKINGCANDIDATEHASHSEARCHINDINODECOUNT,calcAlgContext);
										if (isIndividualNodeBlocking(blockingTestIndi,prevIndiNode,locBlockData,false,blockAltData,calcAlgContext)) {
											blockerNode = prevIndiNode;
										}
									}
								}
								--prevIndiID;
							}
						}
					}
					locBlockData->setBlockingIndividualNode(blockerNode);
					return blockerNode;
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::addIndividualNodeCandidateForConcept(CIndividualProcessNode*& indi, CConceptDescriptor* conDes, CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					CBlockingIndividualNodeCandidateHash* blockingCandHash = procDataBox->getBlockingIndividualNodeCandidateHash(true);
					CBlockingIndividualNodeCandidateData* blockingCandData = blockingCandHash->getBlockingIndividualCandidateData(conDes,true);
					STATINC(ANYWHEREBLOCKINGCANDIDATEHASHUDATEADDCOUNT,calcAlgContext);
					blockingCandData->insertBlockingCandidateIndividualNode(indi);
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::addIndividualNodeCandidateForConcept(CIndividualProcessNode*& indi, CSortedNegLinker<CConcept*>* concepts, bool negated, CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					CBlockingIndividualNodeCandidateHash* blockingCandHash = procDataBox->getBlockingIndividualNodeCandidateHash(true);
					CSortedNegLinker<CConcept*>* conceptIt = concepts;
					while (conceptIt) {
						CConcept* concept = conceptIt->getData();
						bool conceptNeg = conceptIt->isNegated()^negated;
						CBlockingIndividualNodeCandidateData* blockingCandData = blockingCandHash->getBlockingIndividualCandidateData(concept,conceptNeg,true);
						STATINC(ANYWHEREBLOCKINGCANDIDATEHASHUDATEADDCOUNT,calcAlgContext);
						blockingCandData->insertBlockingCandidateIndividualNode(indi);
						cint64 opCode = concept->getOperatorCode();
						if (!conceptNeg && opCode == CCAND || conceptNeg && opCode == CCOR) {
							addIndividualNodeCandidateForConcept(indi,concept->getOperandList(),conceptNeg,calcAlgContext);
						}
						conceptIt = conceptIt->getNext();
					}
				}


				CBlockingIndividualNodeCandidateIterator CCalculationTableauCompletionTaskHandleAlgorithm::getBlockingIndividualNodeCandidateIterator(CIndividualProcessNode* blockingTestIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					cint64 testingIndiID = blockingTestIndi->getIndividualID();
					CConceptDescriptor* initializationConceptDes = blockingTestIndi->getIndividualInitializationConcept();
					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					CNodeSwitchHistory* nodeSwitchHistory = procDataBox->getNodeSwitchHistory(false);
					// test whether hash has to be updated
					bool needsHashUpdate = true;
					CBlockingIndividualNodeCandidateHash* blockingCandHash = procDataBox->getBlockingIndividualNodeCandidateHash(true);
					CBlockingIndividualNodeCandidateData* blockingCandData = blockingCandHash->getBlockingIndividualCandidateData(initializationConceptDes,true);
						
					if (mConfAnywhereBlockingLazyExactHashing) {
						cint64 maxValidIndiID = blockingCandData->getMaxValidIndividualID()+1;
						cint64 conLabelSetModTag = blockingCandData->getConceptLabelSetModificationTag();
						cint64 nodeSwitchTag = blockingCandData->getNodeSwitchTag();
						cint64 minTestIndiNodeID = 0;
						cint64 minTestAncIndiDepth = 0;
						if (nodeSwitchHistory && nodeSwitchTag > 0) {
							nodeSwitchHistory->getMinIndividualAncestorDepthAndNodeID(nodeSwitchTag,minTestAncIndiDepth,minTestIndiNodeID);
							minTestIndiNodeID = qMax(minTestIndiNodeID,(cint64)0);
							minTestAncIndiDepth = qMax(minTestAncIndiDepth,(cint64)0);
						}
						if (maxValidIndiID >= testingIndiID && minTestIndiNodeID >= testingIndiID) {
							needsHashUpdate = false;
						}
						if (needsHashUpdate) {
							STATINC(ANYWHEREBLOCKINGCANDIDATEHASHUDATECOUNT,calcAlgContext);
							if (maxValidIndiID != testingIndiID) {
								// insert testing node
								STATINC(ANYWHEREBLOCKINGCANDIDATEHASHUDATEADDCOUNT,calcAlgContext);
								blockingCandData->insertBlockingCandidateIndividualNode(blockingTestIndi);
							}
							cint64 destIndiID = qMin(maxValidIndiID,minTestIndiNodeID);
							destIndiID = qMax(destIndiID,(cint64)0);
							for (cint64 indiID = testingIndiID-1; indiID >= destIndiID; --indiID) {
								CIndividualProcessNode* indi = getUpToDateIndividual(indiID,calcAlgContext);
								if (indi->isBlockableIndividual() && !indi->hasPurgedBlockedProcessingRestrictionFlags()) {
									if (indiID >= maxValidIndiID || isIndividualNodeConceptLabelSetModified(indi,conLabelSetModTag,calcAlgContext)) {
										if (indi->getReapplyConceptLabelSet(false)->containsConceptDescriptor(initializationConceptDes)) {
											// add to candidate hash
											STATINC(ANYWHEREBLOCKINGCANDIDATEHASHUDATEADDCOUNT,calcAlgContext);
											blockingCandData->insertBlockingCandidateIndividualNode(indi);
										}
									}
								}
							}
							CProcessTagger* processTagger = calcAlgContext->getUsedProcessTagger();
							blockingCandData->updateConceptLabelSetModificationTag(processTagger);
							blockingCandData->updateNodeSwitchTag(processTagger);
							blockingCandData->setMaxValidIndividualID(qMax(maxValidIndiID,testingIndiID));
						}
					}

					return blockingCandData->getBlockingCandidatesIndividualNodeIterator(blockingTestIndi);
				}








				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateIndividualNodeModified(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool addIndividualToProcessingQueueDueToModification = false;
					if (!indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONBLOCKINGCACHEDRETESTDUETOMODIFICATION)) {
						indi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONBLOCKINGCACHEDRETESTDUETOMODIFICATION);
					}
					if (!indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFBLOCKINGRETESTDUEDIRECTMODIFIED)) {
						indi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFBLOCKINGRETESTDUEDIRECTMODIFIED);
						propagateProcessingRestrictionToSuccessors(indi,CIndividualProcessNode::PRFBLOCKINGRETESTDUEANCESTORMODIFIED,true,
								CIndividualProcessNode::PRFDIRECTBLOCKED | CIndividualProcessNode::PRFINDIRECTBLOCKED | CIndividualProcessNode::PRFPROCESSINGBLOCKED,calcAlgContext);
						addIndividualToProcessingQueueDueToModification = true;
					}
					eliminiateBlockedIndividuals(indi,calcAlgContext);
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATISFIABLECACHED) && !indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFRETESTSATISFIABLECACHEDDUEDIRECTMODIFIED)) {
						indi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFRETESTSATISFIABLECACHEDDUEDIRECTMODIFIED);
						addIndividualToProcessingQueueDueToModification = true;
					}
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSIGNATUREBLOCKINGCACHED) && !indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFRETESTSIGNATUREBLOCKINGCACHEDDUEDIRECTMODIFIED)) {
						indi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFRETESTSIGNATUREBLOCKINGCACHEDDUEDIRECTMODIFIED);
						addIndividualToProcessingQueueDueToModification = true;
					}
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHED) && !indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFRETESTCOMPLETIONGRAPHCACHEDDUEDIRECTMODIFIED)) {
						indi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFRETESTCOMPLETIONGRAPHCACHEDDUEDIRECTMODIFIED);
						addIndividualToProcessingQueueDueToModification = true;
					}
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONBLOCKINGCACHED) && !indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFRETESTSATURATIONBLOCKINGCACHEDDUEDIRECTMODIFIED)) {
						indi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFRETESTSATURATIONBLOCKINGCACHEDDUEDIRECTMODIFIED);
						addIndividualToProcessingQueueDueToModification = true;
					}

					if (addIndividualToProcessingQueueDueToModification) {
						addIndividualToBlockingUpdateReviewProcessingQueue(indi,calcAlgContext);
					}

					if (indi->isNominalIndividual() && indi->getNominalIndividual() && indi->isDelayedNominalProcessingQueued()) {
						indi->setDelayedNominalProcessingQueued(false);
						addIndividualToProcessingQueue(indi,calcAlgContext);
					}
				}

				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateIndirectSuccessorBlocking(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext) {
					propagateAddingBlockedProcessingRestrictionToSuccessors(indi,CIndividualProcessNode::PRFINDIRECTBLOCKED,true,CIndividualProcessNode::PRFINDIRECTBLOCKED,calcAlgContext);
				}





				void CCalculationTableauCompletionTaskHandleAlgorithm::pruneSuccessors(CIndividualProcessNode*& indi, CIndividualProcessNode* ancestorIndi, bool removeNominalLinks, CCalculationAlgorithmContextBase* calcAlgContext) {
					indi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFPURGEDBLOCKED);

					eliminiateBlockedIndividuals(indi,calcAlgContext);

					if (removeNominalLinks && ancestorIndi) {
						cint64 ancIndiID = ancestorIndi->getIndividualID();
						CConnectionSuccessorSet* connSuccSet = indi->getConnectionSuccessorSet(false);
						if (connSuccSet) {
							if (connSuccSet->getConnectionSuccessorCount() > 0) {
								CConnectionSuccessorSetIterator conSuccIt = connSuccSet->getConnectionSuccessorIterator();
								while (conSuccIt.hasNext()) {
									cint64 connID = conSuccIt.next(true);
									if (ancIndiID != connID) {
										CIndividualProcessNode* nomIndi = getUpToDateIndividual(connID,calcAlgContext);
										if (nomIndi->isNominalIndividual()) {
											CIndividualProcessNode* locNomIndi = getLocalizedIndividual(nomIndi,false,calcAlgContext);
											CSuccessorRoleIterator succRoleIt = locNomIndi->getSuccessorRoleIterator(indi);
											while (succRoleIt.hasNext()) {
												CIndividualLinkEdge* link = succRoleIt.next(true);
												locNomIndi->removeIndividualLink(link);
											}
											locNomIndi->removeIndividualConnection(indi);
										}
									}
								}
							}
						}
						CSuccessorIterator succIt = indi->getSuccessorIterator();
						while (succIt.hasNext()) {
							CIndividualLinkEdge* succLink = succIt.nextLink(true);							
							CIndividualProcessNode* succIndi = getSuccessorIndividual(indi,succLink,calcAlgContext);
							if (succIndi->isNominalIndividual() && succIndi->getIndividualID() != ancIndiID) {
								CIndividualProcessNode* locNomIndi = getLocalizedIndividual(succIndi,false,calcAlgContext);
								CSuccessorRoleIterator succRoleIt = locNomIndi->getSuccessorRoleIterator(indi);
								while (succRoleIt.hasNext()) {
									CIndividualLinkEdge* link = succRoleIt.next(true);
									locNomIndi->removeIndividualLink(link);
								}
								locNomIndi->removeIndividualConnection(indi);
							}
						}

					}
					CSuccessorIterator succIt = indi->getSuccessorIterator();
					cint64 ancDepth = indi->getIndividualAncestorDepth();
					while (succIt.hasNext()) {
						CIndividualLinkEdge* succLink = succIt.nextLink(true);
						CIndividualProcessNode* succIndi = getSuccessorIndividual(indi,succLink,calcAlgContext);
						cint64 succAncDepth = succIndi->getIndividualAncestorDepth();
						if (succAncDepth > ancDepth) {
							if (succIndi->isBlockableIndividual() && !succIndi->hasPurgedBlockedProcessingRestrictionFlags()) {
								CIndividualProcessNode* locSuccIndi = getLocalizedIndividual(succIndi,false,calcAlgContext);
								pruneSuccessors(locSuccIndi,indi,true,calcAlgContext);
							}
						}
					}
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateProcessingRestrictionToAncestor(CIndividualProcessNode*& indi, cint64 addRestrictionFlags, bool recursive, cint64 whileNotContainsFlags, CCalculationAlgorithmContextBase* calcAlgContext) {
					propagateAddingProcessingRestrictionToAncestor(indi,addRestrictionFlags,recursive,whileNotContainsFlags,calcAlgContext);
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateAddingProcessingRestrictionToAncestor(CIndividualProcessNode*& indi, cint64 addRestrictionFlags, bool recursive, cint64 whileNotContainsFlags, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (indi->hasIndividualAncestor()) {
						CIndividualProcessNode* ancIndi = getAncestorIndividual(indi,calcAlgContext);
						if (!ancIndi->hasPartialProcessingRestrictionFlags(whileNotContainsFlags)) {
							CIndividualProcessNode* locAncIndi = getLocalizedIndividual(ancIndi,false,calcAlgContext);
							locAncIndi->addProcessingRestrictionFlags(addRestrictionFlags);
							if (recursive) {
								propagateAddingProcessingRestrictionToAncestor(locAncIndi,addRestrictionFlags,recursive,whileNotContainsFlags,calcAlgContext);
							}
						}
					}
				}




				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateProcessingRestrictionToSuccessors(CIndividualProcessNode*& indi, cint64 addRestrictionFlags, bool recursive, cint64 whileNotContainsFlags, CCalculationAlgorithmContextBase* calcAlgContext) {
					propagateAddingProcessingRestrictionToSuccessors(indi,addRestrictionFlags,recursive,whileNotContainsFlags,calcAlgContext);
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateAddingBlockedProcessingRestrictionToSuccessors(CIndividualProcessNode*& indi, cint64 addRestrictionFlags, bool recursive, cint64 whileNotContainsFlags, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSuccessorIterator succIt = indi->getSuccessorIterator();
					cint64 ancDepth = indi->getIndividualAncestorDepth();
					while (succIt.hasNext()) {
						CIndividualLinkEdge* succLink = succIt.nextLink(true);
						CIndividualProcessNode* succIndi = getSuccessorIndividual(indi,succLink,calcAlgContext);						
						cint64 succAncDepth = succIndi->getIndividualAncestorDepth();
						if (succAncDepth > ancDepth) {
							if (!succIndi->hasPartialProcessingRestrictionFlags(whileNotContainsFlags)) {
								CIndividualProcessNode* locSuccIndi = getLocalizedIndividual(succIndi,false,calcAlgContext);
								locSuccIndi->addProcessingRestrictionFlags(addRestrictionFlags);
								if (recursive) {
									propagateAddingBlockedProcessingRestrictionToSuccessors(locSuccIndi,addRestrictionFlags,recursive,whileNotContainsFlags,calcAlgContext);
								}
								eliminiateBlockedIndividuals(locSuccIndi,calcAlgContext);
							}
						}
					}
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateAddingProcessingRestrictionToSuccessors(CIndividualProcessNode*& indi, cint64 addRestrictionFlags, bool recursive, cint64 whileNotContainsFlags, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSuccessorIterator succIt = indi->getSuccessorIterator();
					cint64 ancDepth = indi->getIndividualAncestorDepth();
					while (succIt.hasNext()) {
						CIndividualLinkEdge* succLink = succIt.nextLink(true);
						CIndividualProcessNode* succIndi = getSuccessorIndividual(indi,succLink,calcAlgContext);						
						cint64 succAncDepth = succIndi->getIndividualAncestorDepth();
						if (succAncDepth > ancDepth) {
							if (!succIndi->hasPartialProcessingRestrictionFlags(whileNotContainsFlags)) {
								CIndividualProcessNode* locSuccIndi = getLocalizedIndividual(succIndi,false,calcAlgContext);
								locSuccIndi->addProcessingRestrictionFlags(addRestrictionFlags);
								if (recursive) {
									propagateAddingProcessingRestrictionToSuccessors(locSuccIndi,addRestrictionFlags,recursive,whileNotContainsFlags,calcAlgContext);
								}
							}
						}
					}
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateClearingProcessingRestrictionToSuccessors(CIndividualProcessNode*& indi, cint64 clearRestrictionFlags, bool recursive, cint64 whileContainsFlags, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSuccessorIterator succIt = indi->getSuccessorIterator();
					cint64 ancDepth = indi->getIndividualAncestorDepth();
					while (succIt.hasNext()) {
						CIndividualLinkEdge* succLink = succIt.nextLink(true);
						CIndividualProcessNode* succIndi = getSuccessorIndividual(indi,succLink,calcAlgContext);						
						cint64 succAncDepth = succIndi->getIndividualAncestorDepth();
						if (succAncDepth > ancDepth) {
							if (!succIndi->hasPartialProcessingRestrictionFlags(whileContainsFlags)) {
								CIndividualProcessNode* locSuccIndi = getLocalizedIndividual(succIndi,false,calcAlgContext);
								locSuccIndi->clearProcessingRestrictionFlags(clearRestrictionFlags);
								if (recursive) {
									propagateClearingProcessingRestrictionToSuccessors(locSuccIndi,clearRestrictionFlags,recursive,whileContainsFlags,calcAlgContext);
								}
							}
						}
					}
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::reactivateIndirectBlockedSuccessors(CIndividualProcessNode*& indi, bool recursive, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSuccessorIterator succIt = indi->getSuccessorIterator();
					cint64 ancDepth = indi->getIndividualAncestorDepth();
					while (succIt.hasNext()) {
						CIndividualLinkEdge* succLink = succIt.nextLink(true);
						CIndividualProcessNode* succIndi = getSuccessorIndividual(indi,succLink,calcAlgContext);						
						cint64 succAncDepth = succIndi->getIndividualAncestorDepth();
						if (succAncDepth > ancDepth) {
							if (succIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINDIRECTBLOCKED)) {
								if (!succIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFBLOCKINGRETESTDUEINDIRECTBLOCKERLOSS)) {
									CIndividualProcessNode* locBlockedIndiNode = getLocalizedIndividual(succIndi,true,calcAlgContext);
									locBlockedIndiNode->addProcessingRestrictionFlags(CIndividualProcessNode::PRFBLOCKINGRETESTDUEINDIRECTBLOCKERLOSS);
									addIndividualToProcessingQueue(locBlockedIndiNode,calcAlgContext);
								}
							}
						}
					}
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::reactivateBlockedIndividuals(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CXLinker<CIndividualProcessNode*>* blockedIndiNodeIt = indi->getBlockedIndividualsLinker();
					while (blockedIndiNodeIt) {
						CIndividualProcessNode* blockedIndiNode = blockedIndiNodeIt->getData();
						if (!blockedIndiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFBLOCKINGRETESTDUEBLOCKERMODIFIED)) {
							CIndividualProcessNode* locBlockedIndiNode = getLocalizedIndividual(blockedIndiNode,true,calcAlgContext);
							locBlockedIndiNode->addProcessingRestrictionFlags(CIndividualProcessNode::PRFBLOCKINGRETESTDUEBLOCKERMODIFIED);
							addIndividualToProcessingQueue(locBlockedIndiNode,calcAlgContext);
						}
						blockedIndiNodeIt = blockedIndiNodeIt->getNext();
					}
					indi->clearBlockedIndividualsLinker();
					return true;
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateIndividualProcessedAndReactivate(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (mOptProcessedNodePropagation || mOptProcessedConsNodePropagation && indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCONSNODEPREPARATIONINDINODE)) {
						if (!indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPROCESSINGCOMPLETED)) {
							indi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFPROCESSINGCOMPLETED);

							if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORALLPROCESSED) || !hasAncestorIndividualNode(indi,calcAlgContext)) {
								// wake up next individual nodes

								searchReactivateIndividualsProcessedPropagated(indi,calcAlgContext);
							}
						}
					}
				}

				void CCalculationTableauCompletionTaskHandleAlgorithm::searchReactivateIndividualsProcessedPropagated(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext) {


					CXLinker<CIndividualProcessNode*>* processingBlockedNodeLinker = indi->getProcessingBlockedIndividualsLinker();
					while (processingBlockedNodeLinker) {
						CIndividualProcessNode* blockedNode = processingBlockedNodeLinker->getData();
						CIndividualProcessNode* locBlockedNode = getLocalizedIndividual(blockedNode,true,calcAlgContext);
						locBlockedNode->addProcessingRestrictionFlags(CIndividualProcessNode::PRFBLOCKINGRETESTDUEPROCESSINGCOMPLETED);
						addIndividualToProcessingQueue(locBlockedNode,calcAlgContext);
						// set blocking retest flag, clear processing blocked flag
						processingBlockedNodeLinker = processingBlockedNodeLinker->getNext();
					}
					indi->clearBlockedIndividualsLinker();

					cint64 indiID = indi->getIndividualID();

					// TODO: check multiple not processed ancestors
					CSuccessorIterator succIt = indi->getSuccessorIterator();
					cint64 ancDepth = indi->getIndividualAncestorDepth();
					while (succIt.hasNext()) {
						CIndividualLinkEdge* succLink = succIt.nextLink(true);
						CIndividualProcessNode* succIndi = getSuccessorIndividual(indi,succLink,calcAlgContext);						
						cint64 succAncDepth = succIndi->getIndividualAncestorDepth();
						if (succAncDepth > ancDepth) {
							if (!succIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORALLPROCESSED)) {
								// test whether have unprocessed nominals or ancestor
								bool allProcessedAncestor = true;
								CConnectionSuccessorSetIterator connIt = succIndi->getConnectionSuccessorIterator();
								while (allProcessedAncestor && connIt.hasNext()) {
									cint64 connIndiNodeID = connIt.next();
									if (connIndiNodeID != indiID) {
										CIndividualProcessNode* ancNomIndi = getUpToDateIndividual(connIndiNodeID,calcAlgContext);
										if (ancNomIndi->getIndividualAncestorDepth() >= ancDepth || ancNomIndi->isNominalIndividual()) {
											if (!ancNomIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPROCESSINGCOMPLETED | CIndividualProcessNode::PRFANCESTORALLPROCESSED)) {
												allProcessedAncestor = false;
											}
										}
									}
								}
								if (allProcessedAncestor) {
									CIndividualProcessNode* locSuccIndi = getLocalizedIndividual(succIndi,false,calcAlgContext);
									locSuccIndi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORALLPROCESSED);
									if (!locSuccIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPROCESSINGCOMPLETED)) {
										if (locSuccIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPROCESSINGBLOCKED)) {
											locSuccIndi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFBLOCKINGRETESTDUEPROCESSINGCOMPLETED);
											addIndividualToProcessingQueue(locSuccIndi,calcAlgContext);
										}
									} else {
										// search recursive all nodes which has to be reactivated
										searchReactivateIndividualsProcessedPropagated(locSuccIndi,calcAlgContext);
									}
								}
							}
						}
					}
				}

				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateIndividualUnprocessed(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (mOptProcessedNodePropagation) {
						propagateIndividualUnprocessed(indi,false,calcAlgContext);
					} else if (mOptProcessedConsNodePropagation) {
						propagateIndividualUnprocessed(indi,true,calcAlgContext);
					}
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateIndividualUnprocessed(CIndividualProcessNode*& indi, bool requiresConsFlag, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (!requiresConsFlag || indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCONSNODEPREPARATIONINDINODE)) {
						if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPROCESSINGCOMPLETED)) {
							indi->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFPROCESSINGCOMPLETED);
							if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORALLPROCESSED)) {

								CSuccessorIterator succIt = indi->getSuccessorIterator();
								cint64 ancDepth = indi->getIndividualAncestorDepth();
								while (succIt.hasNext()) {
									CIndividualLinkEdge* succLink = succIt.nextLink(true);
									CIndividualProcessNode* succIndi = getSuccessorIndividual(indi,succLink,calcAlgContext);						
									cint64 succAncDepth = succIndi->getIndividualAncestorDepth();
									if (succAncDepth > ancDepth) {
										if (!succIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORALLPROCESSED)) {
											CIndividualProcessNode* locSuccIndi = getLocalizedIndividual(succIndi,false,calcAlgContext);
											locSuccIndi->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORALLPROCESSED);
											propagateIndividualUnprocessed(locSuccIndi,CIndividualProcessNode::PRFANCESTORALLPROCESSED,calcAlgContext);
										}
									}
								}

							}
						}
					}
				}




				bool CCalculationTableauCompletionTaskHandleAlgorithm::isIndividualNodeProcessingBlocked(CIndividualProcessNode* blockingTestIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(INDINODEPROCESSINGBLOCKINGTESTCOUNT,calcAlgContext);
					if (blockingTestIndi->hasPurgedBlockedProcessingRestrictionFlags()) {
						return true;
					}
					if (isSaturationCachedProcessingBlocked(blockingTestIndi,calcAlgContext)) {
						return true;
					}
					if (isIndividualNodeCompletionGraphCached(blockingTestIndi,calcAlgContext)) {
						return true;
					}
					if (isSatisfiableCachedProcessingBlocked(blockingTestIndi,calcAlgContext)) {
						return true;
					}
					if (isSignatureBlockedProcessingBlocked(blockingTestIndi,calcAlgContext)) {
						return true;
					}
					if (blockingTestIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPROCESSINGBLOCKED)) {
						if (mOptDetExpPreporcessing) {
							return true;
						} else {
							return detectIndividualNodeBlockedStatus(blockingTestIndi,calcAlgContext);
						}
					}
					if (blockingTestIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINDIRECTBLOCKED)) {
						if (blockingTestIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFBLOCKINGRETESTDUEINDIRECTBLOCKERLOSS)) {
							return detectIndividualNodeBlockedStatus(blockingTestIndi,calcAlgContext);
						} else {
							return true;
						}
					}
					if (blockingTestIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFDIRECTBLOCKED)) {
						if (blockingTestIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFBLOCKINGRETESTDUEDIRECTMODIFIED | CIndividualProcessNode::PRFBLOCKINGRETESTDUEBLOCKERMODIFIED)) {
							return detectIndividualNodeBlockedStatus(blockingTestIndi,calcAlgContext);
						} else {
							return true;
						}
					}
					return false;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::isIndividualNodeExpansionBlocked(CIndividualProcessNode* blockingTestIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(INDINODEEXPANSIONBLOCKINGTESTCOUNT,calcAlgContext);
					return detectIndividualNodeBlockedStatus(blockingTestIndi,calcAlgContext);
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::needsIndividualNodeExpansionBlockingTest(CConceptProcessDescriptor* conProDes, CIndividualProcessNode* blockingTestIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* conDes = conProDes->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					double conPriority = conProDes->getProcessPriority().getPriority();
					bool conNeg = conDes->getNegation();
					cint64 opCode = concept->getOperatorCode();
					cint64 opCount = concept->getOperandCount();;
					cint64 parameter = concept->getParameter();
					bool needsBlockingTest = false;
					if (!conNeg) {
						switch (opCode) {
							case CCSOME : case CCATLEAST : case CCAQSOME: {
								needsBlockingTest = true; 
								break;
							}
							case CCOR : {
								if (mOptNonStrictIndiNodeProcessing) {
									if (opCount >= 1) {
										needsBlockingTest = true; 
									}
								}
								break;
							}
							case CCATMOST : {
								if (mOptNonStrictIndiNodeProcessing) {
									if (parameter > 1) {
										needsBlockingTest = true; 
									}
								}
								break;
							}
						}
					} else {
						switch (opCode) {
							case CCALL : case CCATMOST : {
								needsBlockingTest = true; 
								break;
							}
							case CCAND : case CCEQ : {
								if (mOptNonStrictIndiNodeProcessing) {
									if (opCount >= 1) {
										needsBlockingTest = true; 
									}
								}
								break;
							}
							case CCATLEAST : {
								if (mOptNonStrictIndiNodeProcessing) {
									if (parameter > 2) {
										needsBlockingTest = true; 
									}
								}
								break;
							}
						}

					}
					return needsBlockingTest;
				}









				bool CCalculationTableauCompletionTaskHandleAlgorithm::hasAncestorIndividualNode(CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualProcessNode* ancIndi = nullptr;
					CIndividualLinkEdge* ancLink = processIndi->getAncestorLink();
					return ancLink != nullptr;

				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::hasRoleSuccessorConcept(CIndividualProcessNode*& processIndi, CRole* role, CConcept* concept, bool conceptNegation, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReapplyRoleSuccessorHash* roleSuccHash = processIndi->getReapplyRoleSuccessorHash(false);
					if (roleSuccHash) {
						CRoleSuccessorLinkIterator roleSuccIt = roleSuccHash->getRoleSuccessorLinkIterator(role);
						while (roleSuccIt.hasNext()) {
							CIndividualLinkEdge* link = roleSuccIt.next();
							CIndividualProcessNode* succIndi = getSuccessorIndividual(processIndi,link,calcAlgContext);
							CReapplyConceptLabelSet* conLabelSet = succIndi->getReapplyConceptLabelSet(false);
							bool contained = conLabelSet->hasConcept(concept,conceptNegation);
							if (contained) {
								return true;
							}
						}
					}
					return false;
				}

				bool CCalculationTableauCompletionTaskHandleAlgorithm::hasRoleSuccessorConcepts(CIndividualProcessNode*& processIndi, CRole* role, CSortedNegLinker<CConcept*>* conceptLinker, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReapplyRoleSuccessorHash* roleSuccHash = processIndi->getReapplyRoleSuccessorHash(false);
					if (roleSuccHash) {
						CRoleSuccessorLinkIterator roleSuccIt = roleSuccHash->getRoleSuccessorLinkIterator(role);
						while (roleSuccIt.hasNext()) {
							bool allContained = true;
							CIndividualLinkEdge* link = roleSuccIt.next();
							CIndividualProcessNode* succIndi = getSuccessorIndividual(processIndi,link,calcAlgContext);
							CReapplyConceptLabelSet* conLabelSet = succIndi->getReapplyConceptLabelSet(false);
							CSortedNegLinker<CConcept*>* conceptLinkerIt = conceptLinker;
							while (conceptLinkerIt && allContained) {
								CConcept* concept = conceptLinkerIt->getData();
								bool conceptNegation = conceptLinkerIt->isNegated() ^ negate;
								bool contained = conLabelSet->hasConcept(concept,conceptNegation);
								allContained &= contained;
								conceptLinkerIt = conceptLinkerIt->getNext();
							}
							if (allContained) { 
								if (processIndi->isIndividualAncestor(succIndi)) {
								}
								return true;
							}
						}
					}
					return false;
				}


				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::getRoleSuccessorWithConcepts(CIndividualProcessNode*& processIndi, CRole* role, CSortedNegLinker<CConcept*>* conceptLinker, bool negate, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReapplyRoleSuccessorHash* roleSuccHash = processIndi->getReapplyRoleSuccessorHash(false);
					if (roleSuccHash) {
						CRoleSuccessorLinkIterator roleSuccIt = roleSuccHash->getRoleSuccessorLinkIterator(role);
						while (roleSuccIt.hasNext()) {
							bool allContained = true;
							CIndividualLinkEdge* link = roleSuccIt.next();
							CIndividualProcessNode* succIndi = getSuccessorIndividual(processIndi,link,calcAlgContext);
							CReapplyConceptLabelSet* conLabelSet = succIndi->getReapplyConceptLabelSet(false);
							CSortedNegLinker<CConcept*>* conceptLinkerIt = conceptLinker;
							while (conceptLinkerIt && allContained) {
								CConcept* concept = conceptLinkerIt->getData();
								bool conceptNegation = conceptLinkerIt->isNegated() ^ negate;
								bool contained = conLabelSet->hasConcept(concept,conceptNegation);
								allContained &= contained;
								conceptLinkerIt = conceptLinkerIt->getNext();
							}
							if (allContained) { 
								return succIndi;
							}
						}
					}
					return nullptr;
				}




				bool CCalculationTableauCompletionTaskHandleAlgorithm::hasDistinctRoleSuccessorConcepts(CIndividualProcessNode*& processIndi, CRole* role, CSortedNegLinker<CConcept*>* conceptLinker, bool negate, cint64 distinctCount, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReapplyRoleSuccessorHash* roleSuccHash = processIndi->getReapplyRoleSuccessorHash(false);
					if (roleSuccHash) {
						CRoleSuccessorLinkIterator roleSuccIt = roleSuccHash->getRoleSuccessorLinkIterator(role);
						while (roleSuccIt.hasNext()) {
							bool allContained = true;
							CIndividualLinkEdge* link = roleSuccIt.next();
							CIndividualProcessNode* succIndi = getSuccessorIndividual(processIndi,link,calcAlgContext);
							cint64 succIndiID = succIndi->getIndividualID();
							CDistinctHash* disHash = succIndi->getDistinctHash(false);
							if (disHash) {
								cint64 maxSuccDisCount = disHash->getDistinctCount()+1;
								if (maxSuccDisCount >= distinctCount && containsIndividualNodeConcepts(succIndi,conceptLinker,negate,calcAlgContext)) {
									cint64 succDisCount = 1;
									cint64 failDisCount = 0;
									CDistinctIterator disIt = disHash->getDistinctIterator();
									while (disIt.hasNext() && maxSuccDisCount-failDisCount >= distinctCount && succDisCount < distinctCount) {
										cint64 disIndiID = disIt.nextDistinctIndividualID();
										if (disIndiID != succIndiID && processIndi->hasRoleSuccessorToIndividual(role,disIndiID,true)) {
											// check has necessary concept
											if (disIndiID < succIndiID) {
												// the combination has been already checked, where successor individual node had the smaller ID
												break;
											}											
											CIndividualProcessNode* disIndi = getUpToDateIndividual(disIndiID,calcAlgContext);
											if (containsIndividualNodeConcepts(disIndi,conceptLinker,negate,calcAlgContext)) {
												++succDisCount;
											} else {
												++failDisCount;
											}
										}
									}
									if (succDisCount >= distinctCount) {
										return true;
									}
								}
							}
						}
					}
					return false;
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::createIndividualNodeDisjointRolesLinks(CIndividualProcessNode*& sourceIndi, CIndividualProcessNode*& destinationIndi, CSortedNegLinker<CRole*>* disjointRoleLinker, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (disjointRoleLinker) {
						CMemoryAllocationManager* taskMemMan = nullptr;
						taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

						CSortedNegLinker<CRole*>* disjointRoleLinkerIt = disjointRoleLinker;
						while (disjointRoleLinkerIt) {
							CRole* disjointRole = disjointRoleLinkerIt->getData();

							// add negation / disjoint edge
							CNegationDisjointEdge* negDisEdge = CObjectParameterizingAllocator< CNegationDisjointEdge,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,calcAlgContext->getUsedProcessContext());
							negDisEdge->initNegationDisjointEdge(sourceIndi,destinationIndi,disjointRole,depTrackPoint);

							CIndividualLinkEdge* linkIndi = sourceIndi->getRoleSuccessorToIndividualLink(disjointRole,destinationIndi,true);
							if (linkIndi) {
								// create clash
								CClashedDependencyDescriptor* clashDes = nullptr;
								clashDes = createClashedIndividualLinkDescriptor(clashDes,linkIndi,linkIndi->getDependencyTrackPoint(),calcAlgContext);
								clashDes = createClashedNegationDisjointDescriptor(clashDes,negDisEdge,depTrackPoint,calcAlgContext);
								throw CCalculationClashProcessingException(clashDes);
							} else {
								sourceIndi->installDisjointLink(negDisEdge);
							}
							disjointRoleLinkerIt = disjointRoleLinkerIt->getNext();
						}
						// it is not necessary to install to the destination individual node the connection from the source node, the disjoint roles have a role link which does this already
					}
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::createIndividualNodeNegationLink(CIndividualProcessNode*& sourceIndi, CIndividualProcessNode*& destinationIndi, CRole* negationRole, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					CNegationDisjointEdge* negDisEdge = CObjectParameterizingAllocator< CNegationDisjointEdge,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,calcAlgContext->getUsedProcessContext());
					negDisEdge->initNegationDisjointEdge(sourceIndi,destinationIndi,negationRole,depTrackPoint);

					CIndividualLinkEdge* linkIndi = sourceIndi->getRoleSuccessorToIndividualLink(negationRole,destinationIndi,true);
					if (linkIndi) {
						// create clash
						CClashedDependencyDescriptor* clashDes = nullptr;
						clashDes = createClashedIndividualLinkDescriptor(clashDes,linkIndi,linkIndi->getDependencyTrackPoint(),calcAlgContext);
						clashDes = createClashedNegationDisjointDescriptor(clashDes,negDisEdge,depTrackPoint,calcAlgContext);
						throw CCalculationClashProcessingException(clashDes);
					} else {
						// add negation / disjoint edge
						sourceIndi->installDisjointLink(negDisEdge);
						destinationIndi->getConnectionSuccessorSet(true)->insertConnectionSuccessor(sourceIndi->getIndividualID());
					}
				}


		




				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateIndividualNodeNewNominalConnectionToAncestors(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext) {
					propagateIndividualNodeNominalConnectionFlagsToAncestors(indi,CIndividualProcessNode::PRFSUCCESSORNEWNOMINALCONNECTION | CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION,calcAlgContext);
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateIndividualNodeNominalConnectionToAncestors(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext) {
					propagateIndividualNodeNominalConnectionFlagsToAncestors(indi,CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION,calcAlgContext);
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateIndividualNodeNominalConnectionFlagsToAncestors(CIndividualProcessNode*& indi, cint64 nominalPropagationFlags, CCalculationAlgorithmContextBase* calcAlgContext) {

					CIndividualProcessNode* ancIndi = indi;
					while (ancIndi && !ancIndi->hasPartialProcessingRestrictionFlags(nominalPropagationFlags)) {
						ancIndi->addProcessingRestrictionFlags(nominalPropagationFlags);

						CXLinker<CIndividualProcessNode*>* procBlockIndiLinkerIt = ancIndi->getProcessingBlockedIndividualsLinker();
						while (procBlockIndiLinkerIt) {
							CIndividualProcessNode* blockedIndiNode = procBlockIndiLinkerIt->getData();
							if (!blockedIndiNode->hasPartialProcessingRestrictionFlags(nominalPropagationFlags)) {
								CIndividualProcessNode* locBlockedIndiNode = getLocalizedIndividual(blockedIndiNode,true,calcAlgContext);
								propagateIndividualNodeNominalConnectionFlagsToAncestors(locBlockedIndiNode,nominalPropagationFlags,calcAlgContext);
							}
							procBlockIndiLinkerIt = procBlockIndiLinkerIt->getNext();
						}
						CXLinker<CIndividualProcessNode*>* blockedIndiLinkerIt = ancIndi->getBlockedIndividualsLinker();
						while (blockedIndiLinkerIt) {
							CIndividualProcessNode* blockedIndiNode = blockedIndiLinkerIt->getData();
							if (!blockedIndiNode->hasPartialProcessingRestrictionFlags(nominalPropagationFlags)) {
								CIndividualProcessNode* locBlockedIndiNode = getLocalizedIndividual(blockedIndiNode,true,calcAlgContext);
								propagateIndividualNodeNominalConnectionFlagsToAncestors(locBlockedIndiNode,nominalPropagationFlags,calcAlgContext);
							}
							blockedIndiLinkerIt = blockedIndiLinkerIt->getNext();
						}
						CBlockingFollowSet* followSet = ancIndi->getBlockingFollowSet(false);
						if (followSet) {
							for (CBlockingFollowSet::const_iterator it = followSet->constBegin(), itEnd = followSet->constEnd(); it != itEnd; ++it) {
								cint64 blockedIndiNodeID = *it;
								CIndividualProcessNode* locBlockedIndiNode = getLocalizedIndividual(blockedIndiNodeID,calcAlgContext);
								propagateIndividualNodeNominalConnectionFlagsToAncestors(locBlockedIndiNode,nominalPropagationFlags,calcAlgContext);
							}
						}
						if (ancIndi->hasSuccessorIndividualNodeBackwardDependencyLinker()) {
							CXLinker<CIndividualProcessNode*>* succIndiNodeBackwardDepLinkerIt = ancIndi->getSuccessorIndividualNodeBackwardDependencyLinker();
							while (succIndiNodeBackwardDepLinkerIt) {
								CIndividualProcessNode* succIndiNodeBackwardDep = succIndiNodeBackwardDepLinkerIt->getData();
								if (ancIndi->hasSuccessorIndividualNode(succIndiNodeBackwardDep) && !succIndiNodeBackwardDep->hasPartialProcessingRestrictionFlags(nominalPropagationFlags)) {
									CIndividualProcessNode* locSuccIndiNodeBackwardDep = getLocalizedIndividual(succIndiNodeBackwardDep,true,calcAlgContext);
									propagateIndividualNodeNominalConnectionFlagsToAncestors(locSuccIndiNodeBackwardDep,nominalPropagationFlags,calcAlgContext);
								}
								succIndiNodeBackwardDepLinkerIt = succIndiNodeBackwardDepLinkerIt->getNext();
							}
						}

						if (ancIndi->hasIndividualAncestor()) {
							ancIndi = getAncestorIndividual(ancIndi,calcAlgContext);
							CIndividualProcessNode* locAncIndi = getLocalizedIndividual(ancIndi,false,calcAlgContext);
							ancIndi = locAncIndi;
						} else {
							ancIndi = nullptr;
						}
					}
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateIndividualNodeNominalConnectionStatusToAncestors(CIndividualProcessNode*& indi, CIndividualProcessNode* copyFromIndiNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (copyFromIndiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION) || copyFromIndiNode->isNominalIndividual()) {
						if (mConfExactNominalDependencyTracking) {
							CSuccessorConnectedNominalSet* copySuccConnNomSet = copyFromIndiNode->getSuccessorNominalConnectionSet(false);
							if (copySuccConnNomSet) {
								for (CSuccessorConnectedNominalSet::const_iterator it = copySuccConnNomSet->constBegin(), itEnd = copySuccConnNomSet->constEnd(); it != itEnd; ++it) {
									cint64 nominalID = *it;
									if (!indi->hasSuccessorConnectionToNominal(nominalID)) {
										propagateIndividualNodeConnectedNominalToAncestors(indi,nominalID,calcAlgContext);
									}
								}
							}
						}
						if (copyFromIndiNode->isNominalIndividual()) {
							CIndividual* nominalIndi = copyFromIndiNode->getNominalIndividual();
							if (mConfExactNominalDependencyTracking && nominalIndi) {
								cint64 nominalID = nominalIndi->getIndividualID();
								if (!indi->hasSuccessorConnectionToNominal(nominalID)) {
									propagateIndividualNodeConnectedNominalToAncestors(indi,nominalID,calcAlgContext);
								}
							}
							cint64 level = copyFromIndiNode->getIndividualNominalLevelOrAncestorDepth();
							if (!nominalIndi || level > 0) {
								propagateIndividualNodeNewNominalConnectionToAncestors(indi,calcAlgContext);
							}
						}
						if (!indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION)) {
							propagateIndividualNodeNominalConnectionToAncestors(indi,calcAlgContext);
						}


						if (copyFromIndiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSUCCESSORNEWNOMINALCONNECTION)) {
							propagateIndividualNodeNewNominalConnectionToAncestors(indi,calcAlgContext);
						}
					}
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateIndividualNodeConnectedNominalToAncestors(CIndividualProcessNode*& indi, cint64 nominalID, CCalculationAlgorithmContextBase* calcAlgContext) {
					cint64 nominalPropagationFlags = CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION;
					CIndividualProcessNode* ancIndi = indi;
					while (ancIndi && !ancIndi->hasSuccessorConnectionToNominal(nominalID)) {
						ancIndi->addProcessingRestrictionFlags(nominalPropagationFlags);
						ancIndi->addSuccessorConnectionToNominal(nominalID);

						CXLinker<CIndividualProcessNode*>* procBlockIndiLinkerIt = ancIndi->getProcessingBlockedIndividualsLinker();
						while (procBlockIndiLinkerIt) {
							CIndividualProcessNode* blockedIndiNode = procBlockIndiLinkerIt->getData();
							if (!blockedIndiNode->hasSuccessorConnectionToNominal(nominalID)) {
								CIndividualProcessNode* locBlockedIndiNode = getLocalizedIndividual(blockedIndiNode,true,calcAlgContext);
								propagateIndividualNodeConnectedNominalToAncestors(locBlockedIndiNode,nominalID,calcAlgContext);
							}
							procBlockIndiLinkerIt = procBlockIndiLinkerIt->getNext();
						}
						CXLinker<CIndividualProcessNode*>* blockedIndiLinkerIt = ancIndi->getBlockedIndividualsLinker();
						while (blockedIndiLinkerIt) {
							CIndividualProcessNode* blockedIndiNode = blockedIndiLinkerIt->getData();
							if (!blockedIndiNode->hasSuccessorConnectionToNominal(nominalID)) {
								CIndividualProcessNode* locBlockedIndiNode = getLocalizedIndividual(blockedIndiNode,true,calcAlgContext);
								propagateIndividualNodeConnectedNominalToAncestors(locBlockedIndiNode,nominalID,calcAlgContext);
							}
							blockedIndiLinkerIt = blockedIndiLinkerIt->getNext();
						}
						CBlockingFollowSet* followSet = ancIndi->getBlockingFollowSet(false);
						if (followSet) {
							for (CBlockingFollowSet::const_iterator it = followSet->constBegin(), itEnd = followSet->constEnd(); it != itEnd; ++it) {
								cint64 blockedIndiNodeID = *it;
								CIndividualProcessNode* locBlockedIndiNode = getLocalizedIndividual(blockedIndiNodeID,calcAlgContext);
								propagateIndividualNodeConnectedNominalToAncestors(locBlockedIndiNode,nominalID,calcAlgContext);
							}
						}
						if (ancIndi->hasSuccessorIndividualNodeBackwardDependencyLinker()) {
							CXLinker<CIndividualProcessNode*>* succIndiNodeBackwardDepLinkerIt = ancIndi->getSuccessorIndividualNodeBackwardDependencyLinker();
							while (succIndiNodeBackwardDepLinkerIt) {
								CIndividualProcessNode* succIndiNodeBackwardDep = succIndiNodeBackwardDepLinkerIt->getData();
								if (ancIndi->hasSuccessorIndividualNode(succIndiNodeBackwardDep) && !succIndiNodeBackwardDep->hasSuccessorConnectionToNominal(nominalID)) {
									CIndividualProcessNode* locSuccIndiNodeBackwardDep = getLocalizedIndividual(succIndiNodeBackwardDep,true,calcAlgContext);
									propagateIndividualNodeConnectedNominalToAncestors(locSuccIndiNodeBackwardDep,nominalID,calcAlgContext);
								}
								succIndiNodeBackwardDepLinkerIt = succIndiNodeBackwardDepLinkerIt->getNext();
							}
						}

						if (ancIndi->hasIndividualAncestor()) {
							ancIndi = getAncestorIndividual(ancIndi,calcAlgContext);
							CIndividualProcessNode* locAncIndi = getLocalizedIndividual(ancIndi,false,calcAlgContext);
							ancIndi = locAncIndi;
						} else {
							ancIndi = nullptr;
						}
					}
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateIndividualNodeNeighboursNominalConnectionToAncestors(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConnectionSuccessorSetIterator conSuccIt = indi->getConnectionSuccessorIterator();
					while (conSuccIt.hasNext()) {
						cint64 neighbourID = conSuccIt.nextSuccessorConnectionID();
						CIndividualProcessNode* neighbourIndiNode = getUpToDateIndividual(neighbourID,calcAlgContext);
						if (!neighbourIndiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION)) {
							CIndividualProcessNode* locNeighbourIndiNode = getLocalizedIndividual(neighbourIndiNode,false,calcAlgContext);
							propagateIndividualNodeNominalConnectionStatusToAncestors(locNeighbourIndiNode,indi,calcAlgContext);
						}
					}
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::isIndividualNodesMergeable(CIndividualProcessNode* indi1, CIndividualProcessNode* indi2, CClashedDependencyDescriptor*& clashDescriptors, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(INDINODESMERGEABLETESTCOUNT,calcAlgContext);
					CDistinctHash* disHash = indi1->getDistinctHash(false);
					STATINC(INDINODESMERGEABLEDISTINCTTESTCOUNT,calcAlgContext);

					if (disHash) {
						CDistinctEdge* disEdge = disHash->getIndividualDistinctEdge(indi2->getIndividualID());
						if (disEdge) {
							clashDescriptors = createClashedIndividualDistinctDescriptor(clashDescriptors,disEdge,disEdge->getDependencyTrackPoint(),calcAlgContext);
							return false;
						}
					}

					if (mConfUniqueNameAssumption) {
						if (indi1->isNominalIndividual() && indi2->isNominalIndividual()) {
							if (indi1->getNominalIndividual() != indi2->getNominalIndividual()) {
								// TODO: clashes correct?
								clashDescriptors = createClashedConceptDescriptor(clashDescriptors,indi1,nullptr,indi1->getDependencyTrackPoint(),calcAlgContext);
								clashDescriptors = createClashedConceptDescriptor(clashDescriptors,indi2,nullptr,indi2->getDependencyTrackPoint(),calcAlgContext);
								return false;
							}
						}
					}


					if (isLabelConceptClashSet(indi1,indi2,clashDescriptors,calcAlgContext)) {
						return false;
					}

					if (!areIndividualNodesDisjointRolesMergeable(indi1,indi2,clashDescriptors,calcAlgContext)) {
						return false;
					}

					STATINC(INDINODESMERGEABLETESTSUCCESSCOUNT,calcAlgContext);
					return true;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::areIndividualNodesDisjointRolesMergeable(CIndividualProcessNode* indi1, CIndividualProcessNode* indi2, CClashedDependencyDescriptor*& clashDescriptors, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (!isIndividualNodeDisjointRolesMergeable(indi1,indi2,clashDescriptors,calcAlgContext)) {
						return false;
					}
					if (!isIndividualNodeDisjointRolesMergeable(indi2,indi1,clashDescriptors,calcAlgContext)) {
						return false;
					}
					return true;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::isIndividualNodeDisjointRolesMergeable(CIndividualProcessNode* indi1, CIndividualProcessNode* indi2, CClashedDependencyDescriptor*& clashDescriptors, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(INDINODESMERGEABLEDISJOINTROLETESTCOUNT,calcAlgContext);
					CConnectionSuccessorSet* connSuccSet1 = indi1->getConnectionSuccessorSet(false);
					CIndividualProcessNode* indi = indi1;
					CIndividualProcessNode* othIndi = indi2;
					CConnectionSuccessorSet* connSuccSet = connSuccSet1;

					cint64 indiID = indi->getIndividualID();
					cint64 othIndiID = othIndi->getIndividualID();

					if (connSuccSet) {
						// links which point to adding individual node
						CConnectionSuccessorSetIterator connIt = connSuccSet->getConnectionSuccessorIterator();
						while (connIt.hasNext()) {
							cint64 connID = connIt.next();

							if (connID == indiID) {
								// test self links
								CDisjointSuccessorRoleIterator connDisSuccRoleIt(indi->getDisjointSuccessorRoleIterator(indi));
								while (connDisSuccRoleIt.hasNext()) {
									CNegationDisjointEdge* disNegLink = connDisSuccRoleIt.next();
									CRole* role = disNegLink->getLinkRole();
									CIndividualLinkEdge* link = othIndi->getRoleSuccessorToIndividualLink(role,othIndi,false);
									if (link) {
										clashDescriptors = createClashedIndividualLinkDescriptor(clashDescriptors,link,link->getDependencyTrackPoint(),calcAlgContext);
										clashDescriptors = createClashedNegationDisjointDescriptor(clashDescriptors,disNegLink,disNegLink->getDependencyTrackPoint(),calcAlgContext);
										return false;
									}
								}
							} else if (othIndiID == connID) {
								// test ancestor/successor merging link clashes
								CDisjointSuccessorRoleIterator connDisSuccRoleIt(othIndi->getDisjointSuccessorRoleIterator(indi));
								while (connDisSuccRoleIt.hasNext()) {
									CNegationDisjointEdge* disNegLink = connDisSuccRoleIt.next();
									CRole* role = disNegLink->getLinkRole();
									CIndividualLinkEdge* link = nullptr;
									// ancestor self test
									link = othIndi->getRoleSuccessorToIndividualLink(role,othIndi,false);
									if (link) {
										clashDescriptors = createClashedIndividualLinkDescriptor(clashDescriptors,link,link->getDependencyTrackPoint(),calcAlgContext);
										clashDescriptors = createClashedNegationDisjointDescriptor(clashDescriptors,disNegLink,disNegLink->getDependencyTrackPoint(),calcAlgContext);
										return false;
									}
									// successor self test
									link = indi->getRoleSuccessorToIndividualLink(role,indi,false);
									if (link) {
										clashDescriptors = createClashedIndividualLinkDescriptor(clashDescriptors,link,link->getDependencyTrackPoint(),calcAlgContext);
										clashDescriptors = createClashedNegationDisjointDescriptor(clashDescriptors,disNegLink,disNegLink->getDependencyTrackPoint(),calcAlgContext);
										return false;
									}
									// ancestor to successor test
									link = othIndi->getRoleSuccessorToIndividualLink(role,indi,false);
									if (link) {
										clashDescriptors = createClashedIndividualLinkDescriptor(clashDescriptors,link,link->getDependencyTrackPoint(),calcAlgContext);
										clashDescriptors = createClashedNegationDisjointDescriptor(clashDescriptors,disNegLink,disNegLink->getDependencyTrackPoint(),calcAlgContext);
										return false;
									}
									// successor to ancestor test
									link = indi->getRoleSuccessorToIndividualLink(role,othIndi,false);
									if (link) {
										clashDescriptors = createClashedIndividualLinkDescriptor(clashDescriptors,link,link->getDependencyTrackPoint(),calcAlgContext);
										clashDescriptors = createClashedNegationDisjointDescriptor(clashDescriptors,disNegLink,disNegLink->getDependencyTrackPoint(),calcAlgContext);
										return false;
									}
								}

							} else {

								CIndividualProcessNode* connIndi = getUpToDateIndividual(connID,calcAlgContext);

								CDisjointSuccessorRoleIterator connDisSuccRoleIt(connIndi->getDisjointSuccessorRoleIterator(indi));
								while (connDisSuccRoleIt.hasNext()) {
									CNegationDisjointEdge* disNegLink = connDisSuccRoleIt.next();
									CRole* role = disNegLink->getLinkRole();
									CIndividualLinkEdge* link = connIndi->getRoleSuccessorToIndividualLink(role,othIndi,false);
									if (link) {
										clashDescriptors = createClashedIndividualLinkDescriptor(clashDescriptors,link,link->getDependencyTrackPoint(),calcAlgContext);
										clashDescriptors = createClashedNegationDisjointDescriptor(clashDescriptors,disNegLink,disNegLink->getDependencyTrackPoint(),calcAlgContext);
										return false;
									}
								}
								CDisjointSuccessorRoleIterator disSuccRoleIt(indi->getDisjointSuccessorRoleIterator(connIndi));
								while (disSuccRoleIt.hasNext()) {
									CNegationDisjointEdge* disNegLink = disSuccRoleIt.next();
									CRole* role = disNegLink->getLinkRole();
									CIndividualLinkEdge* link = othIndi->getRoleSuccessorToIndividualLink(role,connIndi,false);
									if (link) {
										clashDescriptors = createClashedIndividualLinkDescriptor(clashDescriptors,link,link->getDependencyTrackPoint(),calcAlgContext);
										clashDescriptors = createClashedNegationDisjointDescriptor(clashDescriptors,disNegLink,disNegLink->getDependencyTrackPoint(),calcAlgContext);
										return false;
									}
								}
							}
						}
					}
					return true;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::isLabelConceptClashSet(CIndividualProcessNode* indi1, CIndividualProcessNode* indi2, CClashedDependencyDescriptor*& clashDescriptors, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(INDINODESMERGEABLECONCEPTSETTESTCOUNT,calcAlgContext);
					CReapplyConceptLabelSet* conSet1 = indi1->getReapplyConceptLabelSet(false);
					CReapplyConceptLabelSet* conSet2 = indi2->getReapplyConceptLabelSet(false);
					if (conSet2->getConceptCount() < conSet1->getConceptCount()) {
						CReapplyConceptLabelSet* tmpConSet = conSet1;
						conSet1 = conSet2;
						conSet2 = tmpConSet;
						CIndividualProcessNode* tmpIndi = indi1;
						indi1 = indi2;
						indi2 = tmpIndi;
					}
					CReapplyConceptLabelSetIterator conSet1It = conSet1->getConceptLabelSetIterator(true,false,false);
					CReapplyConceptLabelSetIterator conSet2It = conSet2->getConceptLabelSetIterator(true,false,false);
					CConceptDescriptor* conDes2 = conSet2It.getConceptDescriptor();
					CDependencyTrackPoint* depTrackPoint2 = conSet2It.getDependencyTrackPoint();
					cint64 conTag2 = conDes2->getConceptTag();
					conSet2It.moveNext();
					while (conSet1It.hasValue()) {
						CConceptDescriptor* conDes1 = conSet1It.getConceptDescriptor();
						CDependencyTrackPoint* depTrackPoint1 = conSet1It.getDependencyTrackPoint();
						cint64 conTag1 = conDes1->getConceptTag();

						while (conTag2 < conTag1) {
							if (!conSet2It.hasValue()) {
								return false;
							}
							conDes2 = conSet2It.getConceptDescriptor();
							depTrackPoint2 = conSet2It.getDependencyTrackPoint();
							conTag2 = conDes2->getConceptTag();
							conSet2It.moveNext();
						}
						if (conTag1 == conTag2) {
							if (conDes1->isNegated() != conDes2->isNegated()) {
								clashDescriptors = createClashedConceptDescriptor(clashDescriptors,indi1,conDes1,depTrackPoint1,calcAlgContext);
								clashDescriptors = createClashedConceptDescriptor(clashDescriptors,indi2,conDes2,depTrackPoint2,calcAlgContext);
								return true;
							}
						}
						conSet1It.moveNext();
					}
					return false;
				}



				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::getMergedIndividualNodes(CIndividualProcessNode*& preferedMergeIntoIndividualNode, CIndividualProcessNode*& individual2, CDependencyTrackPoint* mergeDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {


					CIndividualProcessNode* mergedIndividual = nullptr;
					CIndividualProcessNode* addingIndividual = nullptr;
					bool switchNodes = false;
					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					if (procDataBox->getConstructedIndividualNode()->getIndividualID() == preferedMergeIntoIndividualNode->getIndividualID()) {
						switchNodes = false;
					} else if (procDataBox->getConstructedIndividualNode()->getIndividualID() == individual2->getIndividualID()) {
						switchNodes = true;
					} else if (preferedMergeIntoIndividualNode->isNominalIndividual() && individual2->isNominalIndividual()) {
						if (individual2->getIndividualNominalLevel() < preferedMergeIntoIndividualNode->getIndividualNominalLevel()) {
							switchNodes = true;
						}
					} else if (preferedMergeIntoIndividualNode->isNominalIndividual()) {
						switchNodes = false;
					} else if (individual2->isNominalIndividual()) {
						switchNodes = true;
					} else if (preferedMergeIntoIndividualNode->isIndividualAncestor(individual2)) {
						switchNodes = false;
					} else if (individual2->isIndividualAncestor(preferedMergeIntoIndividualNode)) {
						switchNodes = true;
					} else {
						if (individual2->getIndividualAncestorDepth() < preferedMergeIntoIndividualNode->getIndividualAncestorDepth()) {
							switchNodes = true;
						} 
						// TODO: check is this if useful?
						//else if (individual2->getIndividualID() < preferedMergeIntoIndividualNode->getIndividualID()) {
						//	switchNodes = true;
						//}
					}

					if (!switchNodes) {
						mergedIndividual = preferedMergeIntoIndividualNode;
						addingIndividual = individual2;
					} else {
						mergedIndividual = individual2;
						addingIndividual = preferedMergeIntoIndividualNode;
					}
					mergeIndividualNodeInto(mergedIndividual,addingIndividual,mergeDepTrackPoint,calcAlgContext);
					return mergedIndividual;
				}




				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::getIntoEmptyMergedIndividualNode(CIndividualProcessNode*& mergingIndividualNode, bool createAsNominal, CIndividualProcessNode* mergerNode, CDependencyTrackPoint* mergeDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualProcessNode* newEmptyIndiNode = createNewEmptyIndividual(calcAlgContext);
					if (!createAsNominal) {
						newEmptyIndiNode->setIndividualNominalLevel(mergingIndividualNode->getIndividualNominalLevel());
						newEmptyIndiNode->setIndividualAncestorDepth(mergingIndividualNode->getIndividualAncestorDepth());
						if (mergingIndividualNode->isNominalIndividual()) {
							newEmptyIndiNode->setIndividualType(CIndividualProcessNode::NOMINALINDIVIDUALTYPE);
						} else {
							newEmptyIndiNode->setIndividualType(CIndividualProcessNode::BLOCKABLEINDIVIDUALTYPE);
						}
					} else {
						newEmptyIndiNode->setIndividualType(CIndividualProcessNode::NOMINALINDIVIDUALTYPE);
						newEmptyIndiNode->setIndividualNominalLevel(mergerNode->getIndividualNominalLevel()+1);
					}
					mergeIndividualNodeInto(newEmptyIndiNode,mergingIndividualNode,mergeDepTrackPoint,calcAlgContext);
					return newEmptyIndiNode;
				}

				
	
				void CCalculationTableauCompletionTaskHandleAlgorithm::mergeIndividualNodeInto(CIndividualProcessNode*& mergeIntoIndividualNode, CIndividualProcessNode*& individual, CDependencyTrackPoint* mergeDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					STATINC(INDINODEMERGECOUNT,calcAlgContext);

					KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(mBeforeMergingTaskDebugIndiModelString = generateDebugIndiModelStringList(calcAlgContext));

					//mMergedStringList.append(QString("merged %1 into %2").arg(individual->getIndividualID()).arg(mergeIntoIndividualNode->getIndividualID()));


					//if (individual->isNominalIndividual()) {
					//	++mNominalMergedCount;
					//	cout<<" start-nominal-merge ";

					//	mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
					//	QFile file(QString("./SATCT/Tasks/task-start-nominal-merge-%1-%2-%3.txt").arg(mNominalMergedCount).arg(individual->getIndividualID()).arg(mergeIntoIndividualNode->getIndividualID()));
					//	if (file.open(QIODevice::WriteOnly)) {
					//		file.write(mEndTaskDebugIndiModelString.replace("<br>","\r\n").toLocal8Bit());
					//		file.close();
					//	}

					//}

					bool newLinksAdded = false;

					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					// merge concept label set
					CReapplyConceptLabelSet* addingConceptLabelSet = individual->getReapplyConceptLabelSet(false);
					CConceptProcessingQueue* mergeIntoConProQueue = mergeIntoIndividualNode->getConceptProcessingQueue(true);
					if (addingConceptLabelSet) {
						CReapplyConceptLabelSet* mergeIntoConceptLabelSet = mergeIntoIndividualNode->getReapplyConceptLabelSet(true);
						CReapplyConceptLabelSetIterator prevSetIt = mergeIntoConceptLabelSet->getConceptLabelSetIterator(true,true,false);
						CReapplyConceptLabelSetIterator conSetIt = addingConceptLabelSet->getConceptLabelSetIterator(true,true,false);
						cint64 prevDataTag = 0;
						bool hasPrevData = false;
						if (prevSetIt.hasNext()) {
							prevDataTag = prevSetIt.getDataTag();
							hasPrevData = true;
							prevSetIt.moveNext();
						}
						while (conSetIt.hasNext()) {

							cint64 conTag = conSetIt.getDataTag();
							CConceptDescriptor* conDes = conSetIt.getConceptDescriptor();
							CDependencyTrackPoint* depTrackPoint = conSetIt.getDependencyTrackPoint();
 							const CCondensedReapplyQueue* reapplyQueue = conSetIt.getPosNegReapplyQueue();
							if (conDes) {
								while (hasPrevData && prevDataTag < conTag) {
									if (prevSetIt.hasNext()) {
										prevDataTag = prevSetIt.getDataTag();
										hasPrevData = true;
										prevSetIt.moveNext();
									} else {
										hasPrevData = false;
									}
								}
								if (!hasPrevData || (hasPrevData && prevDataTag != conTag)) {
									STATINC(INDINODEMERGECONCEPTSADDCOUNT,calcAlgContext);

									// create dependency
									CDependencyTrackPoint* newDepTrackPoint = nullptr;
									CMERGEDCONCEPTDependencyNode* mergedConDepNode = createMERGEDCONCEPTDependency(newDepTrackPoint,mergeIntoIndividualNode,conDes,mergeDepTrackPoint,depTrackPoint,calcAlgContext);

									addConceptToIndividual(conDes->getConcept(),conDes->getNegation(),mergeIntoIndividualNode,newDepTrackPoint,false,true,calcAlgContext);
								}
							}
							conSetIt.moveNext();
						}
					}


					CPROCESSINGHASH<CDependencyTrackPoint*,CDependencyTrackPoint*> depTrackPointHash(calcAlgContext->getUsedTaskProcessorContext());

					// move all connected incoming links
					CConnectionSuccessorSet* connSuccSet = individual->getConnectionSuccessorSet(false);
					if (connSuccSet) {

						// links which point to adding individual node
						CConnectionSuccessorSetIterator connIt = connSuccSet->getConnectionSuccessorIterator();
						while (connIt.hasNext()) {
							cint64 connID = connIt.next();
							if (connID != individual->getIndividualID()) {
								CIndividualProcessNode* locConnIndi = getLocalizedIndividual(connID,calcAlgContext);
								if (individual->isNominalIndividual() || locConnIndi->isNominalIndividual() || individual->isIndividualAncestor(locConnIndi) || mergeIntoIndividualNode->hasSuccessorIndividualNode(locConnIndi)) {

									if (!individual->isNominalIndividual() || !locConnIndi->isIndividualAncestor(individual)) {

										// links from connected individual node
										CSuccessorRoleIterator connRoleIt(locConnIndi->getSuccessorRoleIterator(individual));
										while (connRoleIt.hasNext()) {
											CIndividualLinkEdge* link = connRoleIt.next();
											CRole* role = link->getLinkRole();
											CIndividualProcessNode* creatorIndiNode = mergeIntoIndividualNode;
											if (link->isCreatorIndividualID(locConnIndi)) {
												creatorIndiNode = locConnIndi;
											}
											if (!locConnIndi->hasRoleSuccessorToIndividual(role,mergeIntoIndividualNode,true)) {
												STATINC(INDINODEMERGELINKSADDCOUNT,calcAlgContext);

												// create dependency
												CDependencyTrackPoint* newDepTrackPoint = nullptr;
												CDependencyTrackPoint* prevLinkDepTrackPoint = link->getDependencyTrackPoint();
												if (depTrackPointHash.contains(prevLinkDepTrackPoint)) {
													newDepTrackPoint = depTrackPointHash.value(prevLinkDepTrackPoint);
												} else {
													CMERGEDLINKDependencyNode* mergedLinkDepNode = createMERGEDLINKDependency(newDepTrackPoint,mergeIntoIndividualNode,mergeDepTrackPoint,prevLinkDepTrackPoint,calcAlgContext);
													depTrackPointHash.insert(prevLinkDepTrackPoint,newDepTrackPoint);
												}

												// generate new link
												newLinksAdded = true;
												createNewIndividualsLinkReapplyed(creatorIndiNode,locConnIndi,mergeIntoIndividualNode,role,newDepTrackPoint,calcAlgContext);
											}
											locConnIndi->removeIndividualLink(link);
										}
										// and reverse
										CSuccessorRoleIterator succRoleIt(individual->getSuccessorRoleIterator(locConnIndi));
										while (succRoleIt.hasNext()) {
											CIndividualLinkEdge* link = succRoleIt.next();
											CIndividualProcessNode* creatorIndiNode = mergeIntoIndividualNode;
											if (link->isCreatorIndividualID(locConnIndi)) {
												creatorIndiNode = locConnIndi;
											}
											CRole* role = link->getLinkRole();
											if (!mergeIntoIndividualNode->hasRoleSuccessorToIndividual(role,locConnIndi,true)) {
												STATINC(INDINODEMERGELINKSADDCOUNT,calcAlgContext);

												// create dependency
												CDependencyTrackPoint* newDepTrackPoint = nullptr;
												CDependencyTrackPoint* prevLinkDepTrackPoint = link->getDependencyTrackPoint();
												if (depTrackPointHash.contains(prevLinkDepTrackPoint)) {
													newDepTrackPoint = depTrackPointHash.value(prevLinkDepTrackPoint);
												} else {
													CMERGEDLINKDependencyNode* mergedLinkDepNode = createMERGEDLINKDependency(newDepTrackPoint,mergeIntoIndividualNode,mergeDepTrackPoint,link->getDependencyTrackPoint(),calcAlgContext);
													depTrackPointHash.insert(prevLinkDepTrackPoint,newDepTrackPoint);
												}

												// generate new link
												newLinksAdded = true;
												createNewIndividualsLinkReapplyed(creatorIndiNode,mergeIntoIndividualNode,locConnIndi,role,newDepTrackPoint,calcAlgContext);
											}
										}


										// neg/disjoint links from connected individual node
										CDisjointSuccessorRoleIterator connNegDisIt(locConnIndi->getDisjointSuccessorRoleIterator(individual));
										while (connNegDisIt.hasNext()) {
											CNegationDisjointEdge* negDisLink = connNegDisIt.next();
											CRole* role = negDisLink->getLinkRole();
											if (!locConnIndi->hasNegationDisjointToIndividual(role,mergeIntoIndividualNode)) {
												// TODO: change stat to neg/dis link
												STATINC(INDINODEMERGELINKSADDCOUNT,calcAlgContext);

												// create dependency
												CDependencyTrackPoint* newDepTrackPoint = nullptr;
												CMERGEDLINKDependencyNode* mergedLinkDepNode = createMERGEDLINKDependency(newDepTrackPoint,mergeIntoIndividualNode,mergeDepTrackPoint,negDisLink->getDependencyTrackPoint(),calcAlgContext);

												// generate new negation link
												createIndividualNodeNegationLink(locConnIndi,mergeIntoIndividualNode,role,newDepTrackPoint,calcAlgContext);
											}
										}
										locConnIndi->removeDisjointLinks(individual->getIndividualID());
										// and reverse
										CDisjointSuccessorRoleIterator succNegDisIt(individual->getDisjointSuccessorRoleIterator(locConnIndi));
										while (succNegDisIt.hasNext()) {
											CNegationDisjointEdge* negDisLink = succNegDisIt.next();
											CRole* role = negDisLink->getLinkRole();
											if (!mergeIntoIndividualNode->hasNegationDisjointToIndividual(role,locConnIndi)) {
												// TODO: change stat to neg/dis link
												STATINC(INDINODEMERGELINKSADDCOUNT,calcAlgContext);

												// create dependency
												CDependencyTrackPoint* newDepTrackPoint = nullptr;
												CMERGEDLINKDependencyNode* mergedLinkDepNode = createMERGEDLINKDependency(newDepTrackPoint,mergeIntoIndividualNode,mergeDepTrackPoint,negDisLink->getDependencyTrackPoint(),calcAlgContext);

												// generate new negation link
												createIndividualNodeNegationLink(mergeIntoIndividualNode,locConnIndi,role,newDepTrackPoint,calcAlgContext);
											}
										}


										locConnIndi->removeIndividualConnection(individual);
									}
								}
							}
						}
					}

					if (individual->isNominalIndividual()) {
						if (!mergeIntoIndividualNode->isNominalIndividual()) {
							mergeIntoIndividualNode->setIndividualType(CIndividualProcessNode::NOMINALINDIVIDUALTYPE);
							mergeIntoIndividualNode->setNominalIndividual(individual->getNominalIndividual());
						}
					}

					if (!mergeIntoIndividualNode->getDependencyTrackPoint()) {
						mergeIntoIndividualNode->setDependencyTrackPoint(depTrackPointHash.value(individual->getDependencyTrackPoint()));
					}
					if (mConfMinimizeMerging) {
						if (mergeIntoIndividualNode->isBlockableIndividual() && !mergeIntoIndividualNode->hasIndividualAncestor()) {
							CIndividualLinkEdge* ancLink = individual->getAncestorLink();
							if (ancLink) {
								CIndividualProcessNode* ancIndiNode = getAncestorIndividual(individual,calcAlgContext);
								CIndividualLinkEdge* mergedNewAncLink = ancIndiNode->getRoleSuccessorToIndividualLink(ancLink->getLinkRole(),mergeIntoIndividualNode,false);
								mergeIntoIndividualNode->setAncestorLink(mergedNewAncLink);
							}
						}
					}


					// add distinct information
					bool mergedNodeDatatypeDistinctChangeNotified = false;
					CDistinctHash* addDisHash = individual->getDistinctHash(false);
					if (addDisHash) {
						CDistinctHash* mergeDisHash = mergeIntoIndividualNode->getDistinctHash(true);
						CDistinctIterator disIt = addDisHash->getDistinctIterator();
						while (disIt.hasNext()) {
							CDependencyTrackPoint* depTrackPoint = nullptr;
							cint64 disIndiID = disIt.nextDistinctIndividualID(depTrackPoint);
							CIndividualProcessNode* locDisIndiNode = getLocalizedIndividual(disIndiID,calcAlgContext);
							CDistinctHash* disHash = locDisIndiNode->getDistinctHash(true);
							disHash->removeDistinctIndividual(individual->getIndividualID());
							if (!mergeDisHash->isIndividualDistinct(disIndiID)) {
								STATINC(INDINODEMERGEDISTINCTADDCOUNT,calcAlgContext);
								STATINC(DISTINCTCREATIONCOUNT,calcAlgContext);

								// create dependency
								CDependencyTrackPoint* newDepTrackPoint = nullptr;
								CMERGEDLINKDependencyNode* mergedLinkDepNode = createMERGEDLINKDependency(newDepTrackPoint,mergeIntoIndividualNode,mergeDepTrackPoint,depTrackPoint,calcAlgContext);


								CDistinctEdge* disEdge = CObjectParameterizingAllocator< CDistinctEdge,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,calcAlgContext->getUsedProcessContext());
								disEdge->initDistinctEdge(locDisIndiNode,mergeIntoIndividualNode,newDepTrackPoint);

								mergeDisHash->insertDistinctIndividual(disIndiID,disEdge);
								disHash->insertDistinctIndividual(mergeIntoIndividualNode->getIndividualID(),disEdge);

								if (mDatatypeHandler) {
									mDatatypeHandler->notifyDistinctChanges(locDisIndiNode,calcAlgContext);
									if (!mergedNodeDatatypeDistinctChangeNotified) {
										mergedNodeDatatypeDistinctChangeNotified = true;
										mDatatypeHandler->notifyDistinctChanges(mergeIntoIndividualNode,calcAlgContext);
									}
								}

							}
						}
					}

					// prune nodes
					individual->setMergedIntoIndividualNodeID(mergeIntoIndividualNode->getIndividualID());
					pruneSuccessors(individual,nullptr,false,calcAlgContext);


					if (individual->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINVALIDBLOCKINGORCACHING)) {
						mergeIntoIndividualNode->addProcessingRestrictionFlags(CIndividualProcessNode::PRFINVALIDBLOCKINGORCACHING);
					}


					if (mConfExactNominalDependencyTracking) {
						if (individual->isNominalIndividual() && mergeIntoIndividualNode->isNominalIndividual()) {
							CSuccessorConnectedNominalSet* copySuccConnNomSet = individual->getSuccessorNominalConnectionSet(false);
							if (copySuccConnNomSet) {
								for (CSuccessorConnectedNominalSet::const_iterator it = copySuccConnNomSet->constBegin(), itEnd = copySuccConnNomSet->constEnd(); it != itEnd; ++it) {
									cint64 nominalID = *it;
									mergeIntoIndividualNode->addSuccessorConnectionToNominal(nominalID);
								}
							}
							CIndividual* nominalIndi = individual->getNominalIndividual();
							if (nominalIndi) {
								cint64 nominalID = nominalIndi->getIndividualID();
								mergeIntoIndividualNode->addSuccessorConnectionToNominal(nominalID);
							}
						}
					}

					// is not necessary since rules are applied again for new added concepts
					//if (individual->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION)) {
					//	propagateIndividualNodeNominalConnectionToAncestors(mergeIntoIndividualNode,calcAlgContext);
					//}

					if (newLinksAdded) {
						propagateIndividualNodeModified(mergeIntoIndividualNode,calcAlgContext);
					}
					addIndividualToProcessingQueue(mergeIntoIndividualNode,calcAlgContext);


					//if (individual->isNominalIndividual()) {
					//	++mNominalMergedCount;
					//	cout<<" end-nominal-merge ";

					//	mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
					//	QFile file(QString("./SATCT/Tasks/task-end-nominal-merge-%1-%2-%3.txt").arg(mNominalMergedCount).arg(individual->getIndividualID()).arg(mergeIntoIndividualNode->getIndividualID()));
					//	if (file.open(QIODevice::WriteOnly)) {
					//		file.write(mEndTaskDebugIndiModelString.replace("<br>","\r\n").toLocal8Bit());
					//		file.close();
					//	}

					//}


					KONCLUCE_TASK_ALGORITHM_MODEL_STRING_INSTRUCTION(mAfterMergingTaskDebugIndiModelString = generateDebugIndiModelStringList(calcAlgContext));

				}


				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::tryExtendFunctionalSuccessorIndividual(CIndividualProcessNode*& indi, CConceptDescriptor* conDes, CSortedNegLinker<CRole*>* roleLinkerIt, CRole* ancRole, CSortedNegLinker<CConcept*>* conceptLinkerIt, bool negate, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CIndividualProcessNode* succIndi = nullptr;
					CIndividualSaturationProcessNode* saturationIndiNode = nullptr;


					// TODO: test super roles for functional
					CReapplyRoleSuccessorHash* roleSucc = indi->getReapplyRoleSuccessorHash(false);
					CIndividualLinkEdge* mergeLink = nullptr;
					if (roleSucc) {
						CSortedNegLinker<CRole*>* funTestRoleLinkerIt = roleLinkerIt;
						while (funTestRoleLinkerIt && !succIndi) {
							CRole* role = funTestRoleLinkerIt->getData();
							bool invRole = funTestRoleLinkerIt->isNegated();
							bool functionalRole = !invRole && role->isFunctional();
							if (functionalRole) {
								CRoleSuccessorLinkIterator roleSuccIt = roleSucc->getRoleSuccessorLinkIterator(role);
								if (roleSuccIt.hasNext()) {
									CIndividualLinkEdge* link = roleSuccIt.next(false);
									succIndi = getLocalizedSuccessorIndividual(indi,link,calcAlgContext);
									mergeLink = link;
								}
							}
							funTestRoleLinkerIt = funTestRoleLinkerIt->getNext();
						}
					}
					if (succIndi) {
						// create dependency
						CDependencyTrackPoint* nextAllDepTrackPoint = nullptr;
						CALLDependencyNode* allDepNode = createALLDependency(nextAllDepTrackPoint,indi,conDes,depTrackPoint,mergeLink->getDependencyTrackPoint(),calcAlgContext);

						bool satCachingPossible;
						CConceptDescriptor* lastSatCachPossibleConDes = nullptr;
						if (mConfExpandCreatedSuccessorsFromSaturation) {
							tryExpansionFromSaturatedData(indi,succIndi,conDes,nextAllDepTrackPoint,saturationIndiNode,&satCachingPossible,&lastSatCachPossibleConDes,calcAlgContext);
						}
						// update successor links
						bool newLinksAdded = false;
						CSortedNegLinker<CRole*>* tmpRoleLinkerIt = roleLinkerIt;
						while (tmpRoleLinkerIt) {
							CRole* role = tmpRoleLinkerIt->getData();
							bool invRole = tmpRoleLinkerIt->isNegated();
							if (!invRole) {
								if (!hasIndividualsLink(indi,succIndi,role,true,calcAlgContext)) {
									// add link
									createNewIndividualsLinkReapplyed(indi,indi,succIndi,role,nextAllDepTrackPoint,calcAlgContext);
									newLinksAdded = true;
								}
							} else {
								if (!hasIndividualsLink(succIndi,indi,role,true,calcAlgContext)) {
									// add link
									createNewIndividualsLinkReapplyed(indi,succIndi,indi,role,nextAllDepTrackPoint,calcAlgContext);
									newLinksAdded = true;
								}
							}
							tmpRoleLinkerIt = tmpRoleLinkerIt->getNext();
						}
						if (newLinksAdded) {
							propagateIndividualNodeModified(succIndi,calcAlgContext);
						}

						addConceptsToIndividual(conceptLinkerIt,negate,succIndi,nextAllDepTrackPoint,true,true,nullptr,calcAlgContext);
						if (mConfCachingBlockingFromSaturation) {
							tryEstablishSaturationCaching(indi,succIndi,saturationIndiNode,&satCachingPossible,&lastSatCachPossibleConDes,calcAlgContext);
						}
					}
					return succIndi;
				}


				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::createSuccessorIndividual(CIndividualProcessNode*& indi, CConceptDescriptor* conDes, CSortedNegLinker<CRole*>* roleLinkerIt, CRole* ancRole, CSortedNegLinker<CConcept*>* conceptLinkerIt, bool negate, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CIndividualProcessNode* succIndi = nullptr;
					CIndividualSaturationProcessNode* saturationIndiNode = nullptr;

					// create dependency
					CDependencyTrackPoint* nextDepTrackPoint = nullptr;
					CSOMEDependencyNode* someDepNode = createSOMEDependency(nextDepTrackPoint,indi,conDes,depTrackPoint,calcAlgContext);

					// generate new role successor
					succIndi = createNewIndividual(nextDepTrackPoint,ancRole->isDataRole(),calcAlgContext);

					bool satCachingPossible = true;
					CConceptDescriptor* lastSatCachPossibleConDes = nullptr;
					if (mConfExpandCreatedSuccessorsFromSaturation) {
						tryExpansionFromSaturatedData(indi,succIndi,conDes,nextDepTrackPoint,saturationIndiNode,&satCachingPossible,&lastSatCachPossibleConDes,calcAlgContext);
					}
					CIndividualLinkEdge* ancLink = createNewIndividualsLinksReapplyed(indi,succIndi,roleLinkerIt,ancRole,nextDepTrackPoint,false,calcAlgContext);
					succIndi->setAncestorLink(ancLink);
					succIndi->setIndividualAncestorDepth(indi->getIndividualAncestorDepth()+1);
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATISFIABLECACHED | CIndividualProcessNode::PRFANCESTORSATISFIABLECACHED)) {
						succIndi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATISFIABLECACHED);
					}
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSIGNATUREBLOCKINGCACHED | CIndividualProcessNode::PRFANCESTORSIGNATUREBLOCKINGCACHED)) {
						succIndi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSIGNATUREBLOCKINGCACHED);
					}
					if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONBLOCKINGCACHED | CIndividualProcessNode::PRFANCESTORSATURATIONBLOCKINGCACHED)) {
						succIndi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATURATIONBLOCKINGCACHED);
					}

					addConceptsToIndividual(conceptLinkerIt,negate,succIndi,nextDepTrackPoint,true,true,nullptr,calcAlgContext);
					if (mConfCachingBlockingFromSaturation) {
						tryEstablishSaturationCaching(indi,succIndi,saturationIndiNode,&satCachingPossible,&lastSatCachPossibleConDes,calcAlgContext);
					}
					return succIndi;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::tryEstablishSaturationCaching(CIndividualProcessNode*& indi, CIndividualProcessNode* succIndi, CIndividualSaturationProcessNode* saturationIndiNode, bool* satCachingPossible, CConceptDescriptor** lastSatCachPossibleConDes, CCalculationAlgorithmContextBase* calcAlgContext) {

					if (saturationIndiNode && saturationIndiNode->isInitialized() && saturationIndiNode->isCompleted()) {
						bool cachingEstablishmentPossible = false;

						CIndividualSaturationProcessNodeStatusFlags* flags = saturationIndiNode->getIndirectStatusFlags();
						if (!flags->hasFlags(CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT | CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCLASHED,false)) {
							if (validateSaturationCachingPossible(succIndi,saturationIndiNode,satCachingPossible,lastSatCachPossibleConDes,nullptr,false,calcAlgContext)) {

								bool nominalNodesCompatible = true;

								if (flags->hasFlags(CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGNOMINALCONNECTION,false)) {
									if (!mConfSaturationCachingWithNominals) {
										nominalNodesCompatible = false;
									}
									CSuccessorConnectedNominalSet* succConnNominalSet = saturationIndiNode->getSuccessorConnectedNominalSet(false);
									if (!succConnNominalSet) {
										nominalNodesCompatible = false;
									} else if (!tryInstallSaturationCachingReactivation(succIndi,succConnNominalSet,calcAlgContext)) {
										nominalNodesCompatible = false;
									}
								}

								if (nominalNodesCompatible) {
									cachingEstablishmentPossible = true;
								}
							}
						}

						cint64 succIndiConCount = succIndi->getReapplyConceptLabelSet(false)->getConceptCount();
						CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
						CIndividualNodeSaturationBlockingData* satBlockingData = CObjectAllocator< CIndividualNodeSaturationBlockingData >::allocateAndConstruct(taskMemMan);
						satBlockingData->initSaturationBlockingData(succIndiConCount,*lastSatCachPossibleConDes,saturationIndiNode);
						succIndi->setIndividualSaturationBlockingData(satBlockingData);

						if (cachingEstablishmentPossible) {
							STATINC(SATURATIONCACHEESTABLISHCOUNT,calcAlgContext);
							succIndi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONBLOCKINGCACHED);
							propagateIndirectSuccessorSaturationBlocked(succIndi,calcAlgContext);

							if (!flags->hasFlags(CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCARDINALITYPROPLEMATIC,false)) {
								succIndi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONSUCCESSORCREATIONBLOCKINGCACHED);
							}
							return true;
						}
					}


					return false;
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::propagateIndirectSuccessorSaturationBlocked(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext) {
					propagateAddingBlockedProcessingRestrictionToSuccessors(indi,CIndividualProcessNode::PRFANCESTORSATURATIONBLOCKINGCACHED,true,CIndividualProcessNode::PRFANCESTORSATURATIONBLOCKINGCACHED,calcAlgContext);
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::validateSaturationCachingPossible(CIndividualProcessNode* indi, CIndividualSaturationProcessNode*& saturationIndiNode, bool* satCachingPossible, CConceptDescriptor** lastSatCachPossibleConDes, CConcept* addedConcept, bool addedConceptNegation, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool satCachingStillPossible = true;
					if (satCachingPossible) {
						satCachingStillPossible = *satCachingPossible;
					}
					if (satCachingStillPossible) {
						if (!saturationIndiNode->isCompleted()) {
							satCachingStillPossible = false;
						} else {
							CReapplyConceptLabelSet* conSet = indi->getReapplyConceptLabelSet(false);
							CReapplyConceptSaturationLabelSet* satConSet = saturationIndiNode->getReapplyConceptSaturationLabelSet(false);
							if (conSet && satConSet) {
								CConceptDescriptor* conDesLinker = conSet->getAddingSortedConceptDescriptionLinker();
								CConceptDescriptor* conDesIt = conDesLinker;
								CConceptDescriptor* lastTestedConDesIt = nullptr;
								if (lastSatCachPossibleConDes) {
									lastTestedConDesIt = *lastSatCachPossibleConDes;
								}
								if (addedConcept && conDesIt != lastTestedConDesIt) {
									if (conDesIt->getConcept() == addedConcept || conDesIt->isNegated() == addedConceptNegation) {
										conDesIt = conDesIt->getNext();
									}
								}
								//while (conDesIt != lastTestedConDesIt && fromSatDataConceptsAddedCount > 0) {
								//	--fromSatDataConceptsAddedCount;
								//	conDesIt = conDesIt->getNext();
								//}
								while (conDesIt != lastTestedConDesIt && satCachingStillPossible) {
									if (!satConSet->hasConcept(conDesIt->getConcept(),conDesIt->isNegated())) {
										satCachingStillPossible = false;
									}
									conDesIt = conDesIt->getNext();
								}
								if (lastSatCachPossibleConDes) {
									*lastSatCachPossibleConDes = conDesLinker;
								}
							} else {
								satCachingStillPossible = false;
							}
						}
					}
					if (satCachingPossible) {
						*satCachingPossible = satCachingStillPossible;
					}
					return satCachingStillPossible;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::tryExpansionFromSaturatedData(CIndividualProcessNode*& indi, CIndividualProcessNode* createdSuccIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* depTrackPoint, CIndividualSaturationProcessNode*& saturationIndiNode, bool* satCachingPossible, CConceptDescriptor** lastSatCachPossibleConDes, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConcept* concept = conDes->getConcept();
					bool conceptNegation = conDes->isNegated();
					CConceptData* conceptData = concept->getConceptData();
					CIndividualSaturationProcessNode* existIndiNode = nullptr;
					if (conceptData) {
						CConceptProcessData* conProcData = (CConceptProcessData*)conceptData;
						CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
						if (conRefLinking) {
							CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
							CSaturationConceptReferenceLinking* extSatCalcRefLinkData = confSatRefLinkingData->getExistentialSuccessorConceptSaturationReferenceLinkingData();
							if (extSatCalcRefLinkData) {
								existIndiNode = (CIndividualSaturationProcessNode*)extSatCalcRefLinkData->getIndividualProcessNodeForConcept();
							}
						}
					}

					if (!existIndiNode) {
						CSortedNegLinker<CConcept*>* conceptOpLinker = concept->getOperandList();
						if (!conceptOpLinker->hasNext()) {
							CConcept* opConcept = conceptOpLinker->getData();
							bool opConNegation = conceptOpLinker->isNegated()^conceptNegation;
							CConceptData* opConceptData = opConcept->getConceptData();
							if (opConceptData) {
								CConceptProcessData* conProcData = (CConceptProcessData*)opConceptData;
								CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
								if (conRefLinking) {
									CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
									CSaturationConceptReferenceLinking* satCalcRefLinkData = confSatRefLinkingData->getConceptSaturationReferenceLinkingData(opConNegation);
									if (satCalcRefLinkData) {
										existIndiNode = (CIndividualSaturationProcessNode*)satCalcRefLinkData->getIndividualProcessNodeForConcept();
									}	
								}
							}
						}
					}


					if (existIndiNode && existIndiNode->isInitialized()) {
						CReapplyConceptSaturationLabelSet* satConSet = existIndiNode->getReapplyConceptSaturationLabelSet(false);
						if (satConSet) {
							saturationIndiNode = existIndiNode;
							if (saturationIndiNode->getIndirectStatusFlags()->hasClashedFlag()) {
								CClashedDependencyDescriptor* clashDes = nullptr;
								clashDes = createClashedConceptDescriptor(clashDes,indi,conDes,depTrackPoint,calcAlgContext);
								throw CCalculationClashProcessingException(clashDes);
							} else {

								bool nominalConnectionFlag = saturationIndiNode->getIndirectStatusFlags()->hasNominalConnectionFlag();
								if (nominalConnectionFlag) {
									propagateIndividualNodeNominalConnectionToAncestors(createdSuccIndi,calcAlgContext);
									if (mConfExactNominalDependencyTracking) {
										CSuccessorConnectedNominalSet* satSuccConnNomSet = saturationIndiNode->getSuccessorConnectedNominalSet(false);
										if (satSuccConnNomSet) {
											CSuccessorConnectedNominalSet* succConnNomSet = createdSuccIndi->getSuccessorNominalConnectionSet(true);
											for (CSuccessorConnectedNominalSet::const_iterator it = satSuccConnNomSet->constBegin(), itEnd = satSuccConnNomSet->constEnd(); it != itEnd; ++it) {
												cint64 nominalID = *it;
												succConnNomSet->insert(nominalID);
											}
										}
									}
									propagateIndividualNodeNominalConnectionStatusToAncestors(indi,createdSuccIndi,calcAlgContext);
								}

								for (CConceptSaturationDescriptor* conSatDesIt = satConSet->getConceptSaturationDescriptionLinker(); conSatDesIt; conSatDesIt = conSatDesIt->getNext()) {
									CConcept* satConcept = conSatDesIt->getConcept();
									bool satConceptNegation = conSatDesIt->isNegated();
									STATINC(SATURATIONCACHECONCEPTEXPANSIONCOUNT,calcAlgContext);
									addConceptToIndividualSkipANDProcessing(satConcept,satConceptNegation,createdSuccIndi,depTrackPoint,true,false,calcAlgContext);
									validateSaturationCachingPossible(createdSuccIndi,saturationIndiNode,satCachingPossible,lastSatCachPossibleConDes,satConcept,satConceptNegation,calcAlgContext);
								}

								CSaturationNodeAssociatedDeterministicConceptExpansion* detExpansion = nullptr;
								CSaturationNodeExpansionCacheHandler* satCacheHandler = calcAlgContext->getUsedSaturationNodeExpansionCacheHandler();
								if (satCacheHandler) {
									if (satCacheHandler->getCachedDeterministicExpansion(saturationIndiNode,detExpansion,calcAlgContext)) {
										if (detExpansion && (mConfSaturationIncompleteExpansionFromCache || !detExpansion->requiresNonDeterministicExpansion())) {
											for (CSaturationNodeAssociatedConceptLinker* conceptExpansionLinkerIt = detExpansion->getConceptExpansionLinker(); conceptExpansionLinkerIt; conceptExpansionLinkerIt = conceptExpansionLinkerIt->getNext()) {
												CCacheValue* cacheValue = conceptExpansionLinkerIt->getCacheValue();

												CConcept* expansionConcept = (CConcept*)cacheValue->getIdentification();
												bool expansionConceptNeg = cacheValue->getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT;
												addConceptToIndividualSkipANDProcessing(expansionConcept,expansionConceptNeg,createdSuccIndi,depTrackPoint,true,false,calcAlgContext);

											}
										}
									}
								}

							}
							return true;
						}
					}
					return false;
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::createDistinctSuccessorIndividuals(CIndividualProcessNode*& indi, CPROCESSINGLIST<CIndividualProcessNode*>& indiList, CSortedNegLinker<CRole*>* roleLinkerIt, CRole* ancRole, CSortedNegLinker<CConcept*>* conceptLinkerIt, bool negate, CDependencyTrackPoint* depTrackPoint, cint64 succCardCount, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					for (cint64 i = 0; i < succCardCount; ++i) {
						CIndividualProcessNode* succIndi = nullptr;
						succIndi = createNewIndividual(depTrackPoint,ancRole->isDataRole(),calcAlgContext);
						indiList.append(succIndi);
					}

					createIndividualsDistinct(indiList,depTrackPoint,calcAlgContext);

					for (CPROCESSINGLIST<CIndividualProcessNode*>::const_iterator it = indiList.constBegin(), itEnd = indiList.constEnd(); it != itEnd; ++it) {
						CIndividualProcessNode* succIndi = *it;
						CIndividualLinkEdge* ancLink = createNewIndividualsLinksReapplyed(indi,succIndi,roleLinkerIt,ancRole,depTrackPoint,false,calcAlgContext);
						succIndi->setAncestorLink(ancLink);
						succIndi->setIndividualAncestorDepth(indi->getIndividualAncestorDepth()+1);
						if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATISFIABLECACHED | CIndividualProcessNode::PRFANCESTORSATISFIABLECACHED)) {
							succIndi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATISFIABLECACHED);
						}
						if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSIGNATUREBLOCKINGCACHED | CIndividualProcessNode::PRFANCESTORSIGNATUREBLOCKINGCACHED)) {
							succIndi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSIGNATUREBLOCKINGCACHED);
						}
						if (indi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATURATIONBLOCKINGCACHED | CIndividualProcessNode::PRFANCESTORSATURATIONBLOCKINGCACHED)) {
							succIndi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATURATIONBLOCKINGCACHED);
						}
						addConceptsToIndividual(conceptLinkerIt,negate,succIndi,depTrackPoint,true,true,nullptr,calcAlgContext);
					}
				}





				void CCalculationTableauCompletionTaskHandleAlgorithm::createNominalsSuccessorIndividuals(CIndividualProcessNode*& indi, CSortedNegLinker<CRole*>* roleLinkerIt, CRole* ancRole, CSortedNegLinker<CConcept*>* conceptLinkerIt, bool negate, CDependencyTrackPoint* depTrackPoint, cint64 succCardCount, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					for (cint64 i = 0; i < succCardCount; ++i) {
						STATINC(NOMINALSUCCESSORINDINODECREATIONCOUNT,calcAlgContext);
						CIndividualProcessNode* succIndi = nullptr;
						succIndi = createNewIndividual(depTrackPoint,false,calcAlgContext);
						CIndividualLinkEdge* ancLink = createNewIndividualsLinksReapplyed(indi,succIndi,roleLinkerIt,ancRole,depTrackPoint,false,calcAlgContext);
						succIndi->setIndividualType(CIndividualProcessNode::NOMINALINDIVIDUALTYPE);
						succIndi->setIndividualNominalLevel(indi->getIndividualNominalLevel()+1);

						addIndividualToProcessingQueue(succIndi,calcAlgContext);
					}
				}





				CIndividualLinkEdge* CCalculationTableauCompletionTaskHandleAlgorithm::createNewIndividualsLinks(CIndividualProcessNode*& indiSource, CIndividualProcessNode*& indiDestination, CSortedNegLinker<CRole*>* roleLinkerIt, CRole* ancRole, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualLinkEdge* ancRoleLink = nullptr;
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					bool generatedInvLink = false;
					while (roleLinkerIt) {
						CRole* role = roleLinkerIt->getData();
						bool invRole = roleLinkerIt->isNegated();
						STATINC(LINKSCREATIONCOUNT,calcAlgContext);
						CIndividualLinkEdge* individualLink = CObjectParameterizingAllocator< CIndividualLinkEdge,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,calcAlgContext->getUsedProcessContext());
						if (!invRole) {
							createIndividualNodeDisjointRolesLinks(indiSource,indiDestination,role->getDisjointRoleList(),depTrackPoint,calcAlgContext);
							individualLink->initIndividualLinkEdge(indiSource,indiSource,indiDestination,role,depTrackPoint);
							//indiSource->installIndividualLink(individualLink);
							installIndividualNodeRoleLink(indiSource,indiDestination,individualLink,calcAlgContext);
						} else {
							generatedInvLink = true;
							createIndividualNodeDisjointRolesLinks(indiDestination,indiSource,role->getDisjointRoleList(),depTrackPoint,calcAlgContext);
							individualLink->initIndividualLinkEdge(indiSource,indiDestination,indiSource,role,depTrackPoint);
							//indiDestination->installIndividualLink(individualLink);
							installIndividualNodeRoleLink(indiDestination,indiSource,individualLink,calcAlgContext);
						}
						if (ancRole == role) {
							ancRoleLink = individualLink;
						}
						roleLinkerIt = roleLinkerIt->getNext();
					}
					if (generatedInvLink) {
						indiSource->getConnectionSuccessorSet(true)->insertConnectionSuccessor(indiDestination->getIndividualID());
					}
					indiDestination->getConnectionSuccessorSet(true)->insertConnectionSuccessor(indiSource->getIndividualID());
					return ancRoleLink;
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::installIndividualNodeRoleLink(CIndividualProcessNode*& sourceIndi, CIndividualProcessNode*& destinationIndi, CIndividualLinkEdge* individualLink, CCalculationAlgorithmContextBase* calcAlgContext) {
					CNegationDisjointEdge* negDisEdge = nullptr;
					CDisjointSuccessorRoleHash* disSuccRoleHash = sourceIndi->getDisjointSuccessorRoleHash(false);
					if (disSuccRoleHash) {
						negDisEdge = disSuccRoleHash->getDisjointSuccessorRoleLink(destinationIndi->getIndividualID(),individualLink->getLinkRole());
						if (negDisEdge) {
							CClashedDependencyDescriptor* clashDes = nullptr;
							clashDes = createClashedIndividualLinkDescriptor(clashDes,individualLink,individualLink->getDependencyTrackPoint(),calcAlgContext);
							clashDes = createClashedNegationDisjointDescriptor(clashDes,negDisEdge,negDisEdge->getDependencyTrackPoint(),calcAlgContext);
							throw CCalculationClashProcessingException(clashDes);
						}
					}
					sourceIndi->installIndividualLink(individualLink);
				}


				CReapplyQueueIterator CCalculationTableauCompletionTaskHandleAlgorithm::installIndividualNodeRoleLinkReapplied(CIndividualProcessNode*& sourceIndi, CIndividualProcessNode*& destinationIndi, CIndividualLinkEdge* individualLink, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReapplyQueueIterator reapplyIterator;
					CNegationDisjointEdge* negDisEdge = nullptr;
					CDisjointSuccessorRoleHash* disSuccRoleHash = sourceIndi->getDisjointSuccessorRoleHash(false);
					if (disSuccRoleHash) {
						negDisEdge = disSuccRoleHash->getDisjointSuccessorRoleLink(destinationIndi->getIndividualID(),individualLink->getLinkRole());
						if (negDisEdge) {
							CClashedDependencyDescriptor* clashDes = nullptr;
							clashDes = createClashedIndividualLinkDescriptor(clashDes,individualLink,individualLink->getDependencyTrackPoint(),calcAlgContext);
							clashDes = createClashedNegationDisjointDescriptor(clashDes,negDisEdge,negDisEdge->getDependencyTrackPoint(),calcAlgContext);
							throw CCalculationClashProcessingException(clashDes);
						}
					}
					sourceIndi->installIndividualLink(individualLink,&reapplyIterator);
					return reapplyIterator;
				}


				CIndividualLinkEdge* CCalculationTableauCompletionTaskHandleAlgorithm::createNewIndividualsLinksReapplyed(CIndividualProcessNode*& indiSource, CIndividualProcessNode*& indiDestination, CSortedNegLinker<CRole*>* roleLinkerIt, CRole* ancRole, CDependencyTrackPoint* depTrackPoint, bool checkRoleExisting, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualLinkEdge* ancRoleLink = nullptr;
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					bool generatedInvLink = false;
					while (roleLinkerIt) {
						CRole* role = roleLinkerIt->getData();
						bool invRole = roleLinkerIt->isNegated();
						CSortedNegLinker<CConcept *> *rangeConLinkerIt = role->getRangeConceptList();
						STATINC(LINKSCREATIONCOUNT,calcAlgContext);
						CIndividualLinkEdge* individualLink = CObjectParameterizingAllocator< CIndividualLinkEdge,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,calcAlgContext->getUsedProcessContext());
						if (!invRole) {
							if (!checkRoleExisting || !hasIndividualsLink(indiSource,indiDestination,role,true,calcAlgContext)) {
								createIndividualNodeDisjointRolesLinks(indiSource,indiDestination,role->getDisjointRoleList(),depTrackPoint,calcAlgContext);
								individualLink->initIndividualLinkEdge(indiSource,indiSource,indiDestination,role,depTrackPoint);
								CReapplyQueueIterator reapplyIterator;
								//indiSource->installIndividualLink(individualLink,&reapplyIterator);
								reapplyIterator = installIndividualNodeRoleLinkReapplied(indiSource,indiDestination,individualLink,calcAlgContext);
								if (rangeConLinkerIt) {
									addConceptsToIndividual(rangeConLinkerIt,false,indiDestination,depTrackPoint,true,false,nullptr,calcAlgContext);
								}
								CSortedNegLinker<CConcept *> *domainConLinkerIt = role->getDomainConceptList();
								if (domainConLinkerIt) {
									addConceptsToIndividual(domainConLinkerIt,false,indiSource,depTrackPoint,true,false,nullptr,calcAlgContext);
								}
								applyReapplyQueueConceptsRestricted(indiSource,&reapplyIterator,individualLink,calcAlgContext);
							}
						} else {
							if (!checkRoleExisting || !hasIndividualsLink(indiDestination,indiSource,role,true,calcAlgContext)) {
								generatedInvLink = true;
								createIndividualNodeDisjointRolesLinks(indiDestination,indiSource,role->getDisjointRoleList(),depTrackPoint,calcAlgContext);
								individualLink->initIndividualLinkEdge(indiSource,indiDestination,indiSource,role,depTrackPoint);
								//indiDestination->installIndividualLink(individualLink);
								CReapplyQueueIterator reapplyIterator;
								reapplyIterator = installIndividualNodeRoleLinkReapplied(indiDestination,indiSource,individualLink,calcAlgContext);
								if (rangeConLinkerIt) {
									addConceptsToIndividual(rangeConLinkerIt,false,indiSource,depTrackPoint,true,false,nullptr,calcAlgContext);
								}
								CSortedNegLinker<CConcept *> *domainConLinkerIt = role->getDomainConceptList();
								if (domainConLinkerIt) {
									addConceptsToIndividual(domainConLinkerIt,false,indiDestination,depTrackPoint,true,false,nullptr,calcAlgContext);
								}
								applyReapplyQueueConceptsRestricted(indiDestination,&reapplyIterator,individualLink,calcAlgContext);
							}
						}
						if (ancRole == role) {
							ancRoleLink = individualLink;
						}
						roleLinkerIt = roleLinkerIt->getNext();
					}
					if (generatedInvLink) {
						indiSource->getConnectionSuccessorSet(true)->insertConnectionSuccessor(indiDestination->getIndividualID());
					}
					indiDestination->getConnectionSuccessorSet(true)->insertConnectionSuccessor(indiSource->getIndividualID());
					return ancRoleLink;
				}


				CIndividualLinkEdge* CCalculationTableauCompletionTaskHandleAlgorithm::createNewIndividualsLink(CIndividualProcessNode*& indiCreator, CIndividualProcessNode*& indiSource, CIndividualProcessNode*& indiDestination, CRole* role, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					STATINC(LINKSCREATIONCOUNT,calcAlgContext);
					createIndividualNodeDisjointRolesLinks(indiSource,indiDestination,role->getDisjointRoleList(),depTrackPoint,calcAlgContext);
					CIndividualLinkEdge* individualLink = CObjectParameterizingAllocator< CIndividualLinkEdge,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,calcAlgContext->getUsedProcessContext());
					individualLink->initIndividualLinkEdge(indiCreator,indiSource,indiDestination,role,depTrackPoint);
					//indiSource->installIndividualLink(individualLink);
					installIndividualNodeRoleLink(indiSource,indiDestination,individualLink,calcAlgContext);
					indiDestination->getConnectionSuccessorSet(true)->insertConnectionSuccessor(indiSource->getIndividualID());
					return individualLink;
				}


				CIndividualLinkEdge* CCalculationTableauCompletionTaskHandleAlgorithm::createNewIndividualsLinkReapplyed(CIndividualProcessNode*& indiCreator, CIndividualProcessNode*& indiSource, CIndividualProcessNode*& indiDestination, CRole* role, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					createIndividualNodeDisjointRolesLinks(indiSource,indiDestination,role->getDisjointRoleList(),depTrackPoint,calcAlgContext);
					CReapplyQueueIterator reapplyIterator;
					STATINC(LINKSCREATIONCOUNT,calcAlgContext);
					CIndividualLinkEdge* individualLink = CObjectParameterizingAllocator< CIndividualLinkEdge,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,calcAlgContext->getUsedProcessContext());
					individualLink->initIndividualLinkEdge(indiCreator,indiSource,indiDestination,role,depTrackPoint);
					//indiSource->installIndividualLink(individualLink,&reapplyIterator);
					reapplyIterator = installIndividualNodeRoleLinkReapplied(indiSource,indiDestination,individualLink,calcAlgContext);
					CSortedNegLinker<CConcept *> *rangeConLinkerIt = role->getRangeConceptList();
					if (rangeConLinkerIt) {
						// no preprocessing, because of possible not intercepted clashes while merging
						addConceptsToIndividual(rangeConLinkerIt,false,indiDestination,depTrackPoint,false,false,nullptr,calcAlgContext);
					}
					CSortedNegLinker<CConcept *> *domainConLinkerIt = role->getDomainConceptList();
					if (domainConLinkerIt) {
						// no preprocessing, because of possible not intercepted clashes while merging
						addConceptsToIndividual(domainConLinkerIt,false,indiSource,depTrackPoint,false,false,nullptr,calcAlgContext);
					}
					applyReapplyQueueConceptsRestricted(indiSource,&reapplyIterator,individualLink,calcAlgContext);
					indiDestination->getConnectionSuccessorSet(true)->insertConnectionSuccessor(indiSource->getIndividualID());
					return individualLink;
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::createIndividualsDistinct(CIndividualProcessNode*& indiSource, CIndividualProcessNode*& indiDestination, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(DISTINCTCREATIONCOUNT,calcAlgContext);

					CDistinctEdge* disEdge = CObjectParameterizingAllocator< CDistinctEdge,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
					disEdge->initDistinctEdge(indiSource,indiDestination,depTrackPoint);

					indiSource->getDistinctHash(true)->insertDistinctIndividual(indiDestination->getIndividualID(),disEdge);
					indiDestination->getDistinctHash(true)->insertDistinctIndividual(indiSource->getIndividualID(),disEdge);
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::createIndividualsDistinct(CPROCESSINGLIST<CIndividualProcessNode*>& indiList, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					for (CPROCESSINGLIST<CIndividualProcessNode*>::const_iterator it = indiList.constBegin(), itEnd = indiList.constEnd(); it != itEnd; ++it) {
						CIndividualProcessNode* indi1 = *it;
						CDistinctHash* disHash1 = indi1->getDistinctHash(true);
						CPROCESSINGLIST<CIndividualProcessNode*>::const_iterator it2 = it;
						for (++it2; it2 != itEnd; ++it2) {
							CIndividualProcessNode* indi2 = *it2;
							CDistinctHash* disHash2 = indi2->getDistinctHash(true);
							STATINC(DISTINCTCREATIONCOUNT,calcAlgContext);

							CDistinctEdge* disEdge = CObjectParameterizingAllocator< CDistinctEdge,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
							disEdge->initDistinctEdge(indi1,indi2,depTrackPoint);

							disHash1->insertDistinctIndividual(indi2->getIndividualID(),disEdge);
							disHash2->insertDistinctIndividual(indi1->getIndividualID(),disEdge);
						}
					}
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::hasIndividualsLink(CIndividualProcessNode*& indiSource, CIndividualProcessNode*& indiDestination, CRole* role, bool locateable, CCalculationAlgorithmContextBase* calcAlgContext) {
					return indiSource->hasRoleSuccessorToIndividual(role,indiDestination,locateable);
				}



				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::createNewEmptyIndividual(CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					STATINC(SUCCESSORINDINODECREATIONCOUNT,calcAlgContext);
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CIndividualProcessNodeVector* indiProcNodeVec = calcAlgContext->getProcessingDataBox()->getIndividualProcessNodeVector();
					CIndividualProcessNode* newIndividual = CObjectParameterizingAllocator< CIndividualProcessNode,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,calcAlgContext->getUsedProcessContext());
					cint64 newIndividualID = indiProcNodeVec->getItemCount();
					newIndividual->setIndividualID(newIndividualID);
					indiProcNodeVec->setLocalData(newIndividualID,newIndividual);
					return newIndividual;
				}



				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::createNewIndividual(CDependencyTrackPoint* depTrackPoint, bool dataNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualProcessNode* newIndividual = createNewEmptyIndividual(calcAlgContext);
					if (mOptConsistenceNodeMarking) {
						newIndividual->addProcessingRestrictionFlags(CIndividualProcessNode::PRFCONSNODEPREPARATIONINDINODE);
					}
					newIndividual->initDependencyTracker(depTrackPoint);
					if (!dataNode) {
						CConcept* topConcept = calcAlgContext->getProcessingDataBox()->getOntologyTopConcept();
						addConceptToIndividual(topConcept,false,newIndividual,depTrackPoint,true,false,calcAlgContext);
					} else {
						newIndividual->setExtendedQueueProcessing(true);
						CConcept* topDataRangeConcept = calcAlgContext->getProcessingDataBox()->getOntologyTopDataRangeConcept();
						addConceptToIndividual(topDataRangeConcept,false,newIndividual,depTrackPoint,true,false,calcAlgContext);
						newIndividual->addProcessingRestrictionFlags(CIndividualProcessNode::PRFCONSNODEPREPARATIONINDINODE);
					}
					return newIndividual;
				}



				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::getUpToDateIndividual(CIndividualProcessNode* indi, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (!indi->isLocalizationTagUpToDate(calcAlgContext->getUsedProcessTagger()->getCurrentLocalizationTag()) && indi->isRelocalized()) {
						STATINC(INDINODEUPDATELOADCOUNT,calcAlgContext);
						CIndividualProcessNodeVector* indiProcNodeVec = calcAlgContext->getProcessingDataBox()->getIndividualProcessNodeVector();
						CIndividualProcessNode* upToDateIndi = indiProcNodeVec->getData(indi->getIndividualID());
						return upToDateIndi;
					}
					return indi;
				}

				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::getUpToDateIndividual(cint64 indiID, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(INDINODEUPDATELOADCOUNT,calcAlgContext);
					CIndividualProcessNodeVector* indiProcNodeVec = calcAlgContext->getProcessingDataBox()->getIndividualProcessNodeVector();
					CIndividualProcessNode* upToDateIndi = indiProcNodeVec->getData(indiID);
					return upToDateIndi;
				}

				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::getLocalizedIndividual(cint64 indiID, CCalculationAlgorithmContextBase* calcAlgContext) {
					return getLocalizedIndividual(getUpToDateIndividual(indiID,calcAlgContext),false,calcAlgContext);
				}

				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::getLocalizedIndividual(CIndividualProcessNode* indi, bool updateIndividual, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (!indi->isLocalizationTagUpToDate(calcAlgContext->getUsedProcessTagger()->getCurrentLocalizationTag())) {
						if (updateIndividual) {
							indi = getUpToDateIndividual(indi,calcAlgContext);
						}
						if (!indi->isLocalizationTagUpToDate(calcAlgContext->getUsedProcessTagger()->getCurrentLocalizationTag())) {
							STATINC(INDINODELOCALIZEDLOADCOUNT,calcAlgContext);
							CMemoryAllocationManager* taskMemMan = nullptr;
							taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
							CIndividualProcessNodeVector* indiProcNodeVec = calcAlgContext->getProcessingDataBox()->getIndividualProcessNodeVector();
							CIndividualProcessNode* localicedIndi = CObjectParameterizingAllocator< CIndividualProcessNode,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,calcAlgContext->getUsedProcessContext());
							localicedIndi->initIndividualProcessNode(indi);
							indiProcNodeVec->setLocalData(localicedIndi->getIndividualID(),localicedIndi);

							if (calcAlgContext->hasCompletionGraphCachedIndividualNodes()) {
								if (localicedIndi->getIndividualID() <= calcAlgContext->getMaxCompletionGraphCachedIndividualNodeID()) {
									if (indi->getLocalizationTag() <= calcAlgContext->getCompletionGraphCachedLocalizationTag()) {
										localicedIndi->clearProcessingQueued();
										localicedIndi->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFCACHEDCOMPUTEDTYPESADDED);
									}
								}
							}
							return localicedIndi;
						}
					}
					return indi;
				}

				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::getSuccessorIndividual(CIndividualProcessNode*& indi, CIndividualLinkEdge* link, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualProcessNode* succIndi = nullptr;
					
					if (link->isLocalizationTagUpToDate(calcAlgContext->getUsedProcessTagger()->getCurrentLocalizationTag())) {
						succIndi = link->getOppositeIndividual(indi);
					} else {
						STATINC(INDINODEUPDATELOADCOUNT,calcAlgContext);
						succIndi = link->getOppositeIndividual(indi);
						if (!succIndi->isLocalizationTagUpToDate(calcAlgContext->getUsedProcessTagger()->getCurrentLocalizationTag()) && succIndi->isRelocalized()) {
							cint64 succIndiId = link->getOppositeIndividualID(indi);
							CIndividualProcessNodeVector* indiProcNodeVec = calcAlgContext->getProcessingDataBox()->getIndividualProcessNodeVector();
							succIndi = indiProcNodeVec->getData(succIndiId);
						}
					}
					return succIndi;
				}


				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::getLocalizedSuccessorIndividual(CIndividualProcessNode*& indi, CIndividualLinkEdge* link, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualProcessNode* succIndi = nullptr;

					if (link->isLocalizationTagUpToDate(calcAlgContext->getUsedProcessTagger()->getCurrentLocalizationTag())) {
						succIndi = link->getOppositeIndividual(indi);
					} else {
						STATINC(INDINODEUPDATELOADCOUNT,calcAlgContext);
						cint64 succIndiId = link->getOppositeIndividualID(indi);
						CIndividualProcessNodeVector* indiProcNodeVec = calcAlgContext->getProcessingDataBox()->getIndividualProcessNodeVector();
						succIndi = indiProcNodeVec->getData(succIndiId);
						succIndi = getLocalizedIndividual(succIndi,false,calcAlgContext);
					}
					return succIndi;
				}


				CIndividualProcessNode* CCalculationTableauCompletionTaskHandleAlgorithm::getAncestorIndividual(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualProcessNode* ancIndi = nullptr;

					CIndividualLinkEdge* ancLink = indi->getAncestorLink();
					if (ancLink) {
						ancIndi = getSuccessorIndividual(indi,ancLink,calcAlgContext);
					}
					return ancIndi;
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::applyReapplyQueueConcepts(CIndividualProcessNode*& processIndi, CConcept* concept, bool negation, CCalculationAlgorithmContextBase* calcAlgContext) {					
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CCondensedReapplyQueueIterator reapplyQueueIt = processIndi->getConceptReapplyIterator(concept,negation,true);
					CConceptProcessingQueue* conProQueue = nullptr;
					CProcessingRestrictionSpecification* procRest = nullptr;
					while (reapplyQueueIt.hasNext()) {
						STATINC(REAPPLIEDCONCEPTSCOUNT,calcAlgContext);
						CReapplyConceptDescriptor* reapplyConceptDes = reapplyQueueIt.next();
						CConceptDescriptor* conDes = reapplyConceptDes->getConceptDescriptor();
						CDependencyTrackPoint* depTrackPoint = reapplyConceptDes->getDependencyTrackPoint();
						procRest = reapplyConceptDes->getReapplyProcessingRestriction();
						if (!conProQueue) {
							conProQueue = processIndi->getConceptProcessingQueue(true);
						}
						addConceptRestrictedToProcessingQueue(conDes,depTrackPoint,conProQueue,processIndi,reapplyConceptDes->isStaticDescriptor(),procRest,calcAlgContext);
					}
					if (conProQueue) {
						addIndividualToProcessingQueue(processIndi,calcAlgContext);
					}
				}

				void CCalculationTableauCompletionTaskHandleAlgorithm::applyReapplyQueueConcepts(CIndividualProcessNode*& processIndi, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext) {					
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CReapplyQueueIterator reapplyQueueIt = processIndi->getRoleReapplyIterator(role,true);
					CConceptProcessingQueue* conProQueue = nullptr;
					CProcessingRestrictionSpecification* procRest = nullptr;
					while (reapplyQueueIt.hasNext()) {
						STATINC(REAPPLIEDCONCEPTSCOUNT,calcAlgContext);
						CReapplyConceptDescriptor* reapplyConceptDes = reapplyQueueIt.next();
						CConceptDescriptor* conDes = reapplyConceptDes->getConceptDescriptor();
						CDependencyTrackPoint* depTrackPoint = reapplyConceptDes->getDependencyTrackPoint();
						procRest = reapplyConceptDes->getReapplyProcessingRestriction();
						if (!conProQueue) {
							conProQueue = processIndi->getConceptProcessingQueue(true);
						}
						addConceptRestrictedToProcessingQueue(conDes,depTrackPoint,conProQueue,processIndi,reapplyConceptDes->isStaticDescriptor(),procRest,calcAlgContext);
					}
					if (conProQueue) {
						addIndividualToProcessingQueue(processIndi,calcAlgContext);
					}
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::applyReapplyQueueConceptsRestricted(CIndividualProcessNode*& processIndi, CReapplyQueueIterator* reapplyQueueIt, CIndividualLinkEdge* restrictedLink, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CConceptProcessingQueue* conProQueue = nullptr;
					CLinkProcessingRestrictionSpecification* linkProcRest = nullptr;
					CProcessingRestrictionSpecification* procRest = nullptr;
					while (reapplyQueueIt->hasNext()) {
						STATINC(REAPPLIEDCONCEPTSCOUNT,calcAlgContext);
						CReapplyConceptDescriptor* reapplyConceptDes = reapplyQueueIt->next();
						CConceptDescriptor* conDes = reapplyConceptDes->getConceptDescriptor();
						CDependencyTrackPoint* depTrackPoint = reapplyConceptDes->getDependencyTrackPoint();
						procRest = reapplyConceptDes->getReapplyProcessingRestriction();
						if (!conProQueue) {
							conProQueue = processIndi->getConceptProcessingQueue(true);
						}
						if (!procRest) {
							if (!linkProcRest) {
								linkProcRest = CObjectAllocator< CLinkProcessingRestrictionSpecification >::allocateAndConstruct(taskMemMan);
								linkProcRest->initLinkRestriction(restrictedLink);
							}
							procRest = linkProcRest;
						}
						addConceptRestrictedToProcessingQueue(conDes,depTrackPoint,conProQueue,processIndi,reapplyConceptDes->isStaticDescriptor(),procRest,calcAlgContext);
					}
					if (conProQueue) {
						addIndividualToProcessingQueue(processIndi,calcAlgContext);
					}
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::applyReapplyQueueConcepts(CIndividualProcessNode*& processIndi, CCondensedReapplyQueueIterator* reapplyQueueIt, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CConceptProcessingQueue* conProQueue = nullptr;
					CProcessingRestrictionSpecification* procRest = nullptr;
					while (reapplyQueueIt->hasNext()) {
						STATINC(REAPPLIEDCONCEPTSCOUNT,calcAlgContext);
						CReapplyConceptDescriptor* reapplyConceptDes = reapplyQueueIt->next();
						CConceptDescriptor* conDes = reapplyConceptDes->getConceptDescriptor();
						CDependencyTrackPoint* depTrackPoint = reapplyConceptDes->getDependencyTrackPoint();
						procRest = reapplyConceptDes->getReapplyProcessingRestriction();
						if (!conProQueue) {
							conProQueue = processIndi->getConceptProcessingQueue(true);
						}
						addConceptRestrictedToProcessingQueue(conDes,depTrackPoint,conProQueue,processIndi,reapplyConceptDes->isStaticDescriptor(),procRest,calcAlgContext);
					}
					if (conProQueue) {
						addIndividualToProcessingQueue(processIndi,calcAlgContext);
					}
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::addConceptToReapplyQueue(CConceptDescriptor *conceptDescriptor, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* dependencyTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConcept* concept = conceptDescriptor->getConcept();
					CRole* role = concept->getRole();
					return addConceptToReapplyQueue(conceptDescriptor,role,processIndi,true,dependencyTrackPoint,calcAlgContext);
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::addConceptToReapplyQueue(CConceptDescriptor *conceptDescriptor, CRole* role, CIndividualProcessNode*& processIndi, bool isStaticDes, CDependencyTrackPoint* dependencyTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					STATINC(INSERTCONCEPTREAPPLICATIONSCOUNT,calcAlgContext);
					CReapplyQueue* reapplyQueue = processIndi->getRoleReapplyQueue(role,true);
					CReapplyConceptDescriptor* reapplyConDes = CObjectAllocator<CReapplyConceptDescriptor>::allocateAndConstruct(taskMemMan);
					reapplyConDes->initReapllyDescriptor(conceptDescriptor,dependencyTrackPoint,isStaticDes);
					reapplyQueue->addReapplyConceptDescriptor(reapplyConDes);
				}

				void CCalculationTableauCompletionTaskHandleAlgorithm::addConceptToReapplyQueue(CConceptDescriptor *conceptDescriptor, CConcept* concept, bool negation, CIndividualProcessNode*& processIndi, bool isStaticDes, CDependencyTrackPoint* dependencyTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					STATINC(INSERTCONCEPTREAPPLICATIONSCOUNT,calcAlgContext);
					CCondensedReapplyQueue* reapplyQueue = processIndi->getConceptReapplyQueue(concept,negation,true);
					CCondensedReapplyConceptDescriptor* reapplyConDes = CObjectAllocator<CCondensedReapplyConceptDescriptor>::allocateAndConstruct(taskMemMan);
					reapplyConDes->initReapllyDescriptor(conceptDescriptor,dependencyTrackPoint,!negation);
					reapplyQueue->addReapplyConceptDescriptor(reapplyConDes);
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::addConceptToReapplyQueue(CConceptDescriptor *conceptDescriptor, CRole* role, CIndividualProcessNode*& processIndi, CProcessingRestrictionSpecification* procRest, CDependencyTrackPoint* dependencyTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					STATINC(INSERTCONCEPTREAPPLICATIONSCOUNT,calcAlgContext);
					CReapplyQueue* reapplyQueue = processIndi->getRoleReapplyQueue(role,true);
					CReapplyConceptDescriptor* reapplyConDes = CObjectAllocator<CReapplyConceptDescriptor>::allocateAndConstruct(taskMemMan);
					reapplyConDes->initReapllyDescriptor(conceptDescriptor,dependencyTrackPoint,procRest);
					reapplyQueue->addReapplyConceptDescriptor(reapplyConDes);
				}

				void CCalculationTableauCompletionTaskHandleAlgorithm::addConceptToReapplyQueue(CConceptDescriptor *conceptDescriptor, CConcept* concept, bool negation, CIndividualProcessNode*& processIndi, CProcessingRestrictionSpecification* procRest, CDependencyTrackPoint* dependencyTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					STATINC(INSERTCONCEPTREAPPLICATIONSCOUNT,calcAlgContext);
					CCondensedReapplyQueue* reapplyQueue = processIndi->getConceptReapplyQueue(concept,negation,true);
					CCondensedReapplyConceptDescriptor* reapplyConDes = CObjectAllocator<CCondensedReapplyConceptDescriptor>::allocateAndConstruct(taskMemMan);
					reapplyConDes->initReapllyDescriptor(conceptDescriptor,dependencyTrackPoint,!negation,procRest);
					reapplyQueue->addReapplyConceptDescriptor(reapplyConDes);
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::isConceptInReapplyQueue(CConceptDescriptor* conceptDescriptor, CConcept* concept, bool negation, CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CCondensedReapplyQueue* reapplyQueue = processIndi->getConceptReapplyQueue(concept,negation,false);
					if (reapplyQueue) {
						return reapplyQueue->hasConceptDescriptor(conceptDescriptor);
					}
					return false;
				}

				bool CCalculationTableauCompletionTaskHandleAlgorithm::isConceptInReapplyQueue(CConceptDescriptor* conceptDescriptor, CRole* role, CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReapplyQueue* reapplyQueue = processIndi->getRoleReapplyQueue(role,false);
					if (reapplyQueue) {
						return reapplyQueue->hasConceptDescriptor(conceptDescriptor);
					}
					return false;
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::addConceptToIndividualSkipANDProcessing(CConcept* addingConcept, bool negate, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* dependencyTrackPoint, bool allowPreprocessing, bool allowInitalization, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					CConceptProcessingQueue* conProQueue = processIndi->getConceptProcessingQueue(true);
					CReapplyConceptLabelSet* conLabelSet = processIndi->getReapplyConceptLabelSet(true);

					KONCLUDE_ASSERT_X(dependencyTrackPoint,"adding concept to individual","dependency track point missing");
					KONCLUDE_ASSERT_X(addingConcept,"adding concept to individual","concept missing");

					CConceptDescriptor* conceptDescriptor = CObjectAllocator<CConceptDescriptor>::allocateAndConstruct(taskMemMan);
					conceptDescriptor->initConceptDescriptor(addingConcept,negate,dependencyTrackPoint);
					CCondensedReapplyQueueIterator reapplyIt;
					bool contained = insertConceptsToIndividualConceptSet(conceptDescriptor,dependencyTrackPoint,processIndi,conLabelSet,&reapplyIt,allowInitalization,calcAlgContext);
					if (!contained) {
						STATINC(CONCEPTSADDEDINDINODELABELSETCOUNT,calcAlgContext);
						addBlockingCoreConcept(conceptDescriptor,processIndi,conLabelSet,calcAlgContext);
						setIndividualNodeConceptLabelSetModified(processIndi,calcAlgContext);
						addConceptPreprocessedToProcessingQueue(conceptDescriptor,dependencyTrackPoint,conProQueue,processIndi,allowPreprocessing,calcAlgContext,&CCalculationTableauCompletionTaskHandleAlgorithm::applyANDRule);
						if (reapplyIt.hasNext()) {
							// reapply reapplying concept
							applyReapplyQueueConcepts(processIndi,&reapplyIt,calcAlgContext);
						}
					}
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::addConceptToIndividual(CConcept* addingConcept, bool negate, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* dependencyTrackPoint, bool allowPreprocessing, bool allowInitalization, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					CConceptProcessingQueue* conProQueue = processIndi->getConceptProcessingQueue(true);
					CReapplyConceptLabelSet* conLabelSet = processIndi->getReapplyConceptLabelSet(true);

					KONCLUDE_ASSERT_X(dependencyTrackPoint,"adding concept to individual","dependency track point missing");
					KONCLUDE_ASSERT_X(addingConcept,"adding concept to individual","concept missing");

					CConceptDescriptor* conceptDescriptor = CObjectAllocator<CConceptDescriptor>::allocateAndConstruct(taskMemMan);
					conceptDescriptor->initConceptDescriptor(addingConcept,negate,dependencyTrackPoint);
					CCondensedReapplyQueueIterator reapplyIt;
					bool contained = insertConceptsToIndividualConceptSet(conceptDescriptor,dependencyTrackPoint,processIndi,conLabelSet,&reapplyIt,allowInitalization,calcAlgContext);
					if (!contained) {
						STATINC(CONCEPTSADDEDINDINODELABELSETCOUNT,calcAlgContext);
						addBlockingCoreConcept(conceptDescriptor,processIndi,conLabelSet,calcAlgContext);
						setIndividualNodeConceptLabelSetModified(processIndi,calcAlgContext);
						addConceptPreprocessedToProcessingQueue(conceptDescriptor,dependencyTrackPoint,conProQueue,processIndi,allowPreprocessing,calcAlgContext);
						if (reapplyIt.hasNext()) {
							// reapply reapplying concept
							applyReapplyQueueConcepts(processIndi,&reapplyIt,calcAlgContext);
						}
					}
				}



				CConceptDescriptor* CCalculationTableauCompletionTaskHandleAlgorithm::addConceptToIndividualReturnConceptDescriptor(CConcept* addingConcept, bool negate, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* dependencyTrackPoint, bool allowPreprocessing, bool allowInitalization, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					CConceptProcessingQueue* conProQueue = processIndi->getConceptProcessingQueue(true);
					CReapplyConceptLabelSet* conLabelSet = processIndi->getReapplyConceptLabelSet(true);

					KONCLUDE_ASSERT_X(dependencyTrackPoint,"adding concept to individual","dependency track point missing");
					KONCLUDE_ASSERT_X(addingConcept,"adding concept to individual","concept missing");

					CConceptDescriptor* conceptDescriptor = CObjectAllocator<CConceptDescriptor>::allocateAndConstruct(taskMemMan);
					conceptDescriptor->initConceptDescriptor(addingConcept,negate,dependencyTrackPoint);
					CCondensedReapplyQueueIterator reapplyIt;
					insertConceptsToIndividualConceptSet(conceptDescriptor,dependencyTrackPoint,processIndi,conLabelSet,&reapplyIt,allowInitalization,calcAlgContext);

					STATINC(CONCEPTSADDEDINDINODELABELSETCOUNT,calcAlgContext);
					addBlockingCoreConcept(conceptDescriptor,processIndi,conLabelSet,calcAlgContext);
					setIndividualNodeConceptLabelSetModified(processIndi,calcAlgContext);
					addConceptPreprocessedToProcessingQueue(conceptDescriptor,dependencyTrackPoint,conProQueue,processIndi,allowPreprocessing,calcAlgContext);
					if (reapplyIt.hasNext()) {
						// reapply reapplying concept
						applyReapplyQueueConcepts(processIndi,&reapplyIt,calcAlgContext);
					}
					return conceptDescriptor;
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::setIndividualNodeAncestorConnectionModified(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext) {
					propagateIndividualNodeModified(indi,calcAlgContext);
				}

				void CCalculationTableauCompletionTaskHandleAlgorithm::setIndividualNodeConceptLabelSetModified(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessTagger* processTagger = calcAlgContext->getUsedProcessTagger();
					processTagger->incConceptLabelSetModificationTag();
					indi->getReapplyConceptLabelSet(true)->updateConceptLabelSetModificationTag(processTagger);
					calcAlgContext->setMinModificationIndividualCandidate(indi);
					propagateIndividualNodeModified(indi,calcAlgContext);
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::isIndividualNodeConceptLabelSetModified(CIndividualProcessNode*& indi, cint64 modTag, CCalculationAlgorithmContextBase* calcAlgContext) {
					return indi->getReapplyConceptLabelSet(false)->isConceptLabelSetModificationTagUpToDate(modTag);
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::addConceptsToIndividual(CSortedNegLinker<CConcept*>* conceptAddLinkerIt, bool negate, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* dependencyTrackPoint, bool allowPreprocessing, bool allowInitalization, cint64* conceptCount, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CConceptProcessingQueue* conProQueue = processIndi->getConceptProcessingQueue(true);

					KONCLUDE_ASSERT_X(dependencyTrackPoint,"adding concept to individual","dependency track point missing");

					cint64 conCount = 0;
					CReapplyConceptLabelSet* conLabelSet = processIndi->getReapplyConceptLabelSet(true);
					while (conceptAddLinkerIt) {
						CConcept* concept = conceptAddLinkerIt->getData();
						bool conNeg = conceptAddLinkerIt->isNegated()^negate;


						KONCLUDE_ASSERT_X(concept,"adding concept to individual","concept missing");

						CConceptDescriptor* conceptDescriptor = CObjectAllocator<CConceptDescriptor>::allocateAndConstruct(taskMemMan);
						conceptDescriptor->initConceptDescriptor(concept,conNeg,dependencyTrackPoint);
						CCondensedReapplyQueueIterator reapplyIt;
						bool contained = insertConceptsToIndividualConceptSet(conceptDescriptor,dependencyTrackPoint,processIndi,conLabelSet,&reapplyIt,allowInitalization,calcAlgContext);
						if (!contained) {
							STATINC(CONCEPTSADDEDINDINODELABELSETCOUNT,calcAlgContext);
							addBlockingCoreConcept(conceptDescriptor,processIndi,conLabelSet,calcAlgContext);
							setIndividualNodeConceptLabelSetModified(processIndi,calcAlgContext);
							addConceptPreprocessedToProcessingQueue(conceptDescriptor,dependencyTrackPoint,conProQueue,processIndi,allowPreprocessing,calcAlgContext);
							if (reapplyIt.hasNext()) {
								// reapply reapplying concept
								applyReapplyQueueConcepts(processIndi,&reapplyIt,calcAlgContext);
							}
						}
						++conCount;
						conceptAddLinkerIt = conceptAddLinkerIt->getNext();
					}
					if (conceptCount) {
						*conceptCount = conCount;
					}
				}

				bool CCalculationTableauCompletionTaskHandleAlgorithm::addBlockingCoreConcept(CConceptDescriptor* conceptDescriptor, CIndividualProcessNode*& processIndi, CReapplyConceptLabelSet* conLabelSet, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (mConfSaveCoreBlockingConceptsCandidates) {
						CConceptData* conData = conceptDescriptor->getConcept()->getConceptData();
						if (conData) {
							CConceptProcessData* conProData = (CConceptProcessData*)conData;
							if (conProData->isCoreBlockingConcept(conceptDescriptor->isNegated())) {

								if (!processIndi->isNominalIndividual()) {
									STATINC(CORECONCEPTSADDEDINDINODELABELSETCOUNT,calcAlgContext);
									CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
									CCoreConceptDescriptor* coreConceptDescriptor = CObjectAllocator<CCoreConceptDescriptor>::allocateAndConstruct(taskMemMan);
									coreConceptDescriptor->initCoreConceptDescriptor(conceptDescriptor);
									conLabelSet->addCoreConceptDescriptor(coreConceptDescriptor);

									CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
									CBlockingIndividualNodeLinkedCandidateHash* blockingCandHash = procDataBox->getBlockingIndividualNodeLinkedCandidateHash(true);
									CBlockingIndividualNodeLinkedCandidateData* blockingCandData = blockingCandHash->getBlockingIndividualCandidateData(conceptDescriptor,true);
									blockingCandData->addBlockingCandidateIndividualNode(processIndi);
								}

								return true;
							}
						}
					}
					return false;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::isConceptUnsatisfiabilitySaturated(CConcept* concept, bool negation, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptData* conceptData = concept->getConceptData();
					CIndividualSaturationProcessNode* saturationIndiNode = nullptr;
					if (conceptData) {
						CConceptProcessData* conProcData = (CConceptProcessData*)conceptData;
						CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
						if (conRefLinking) {
							CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
							CSaturationConceptReferenceLinking* satCalcRefLinkData = confSatRefLinkingData->getConceptSaturationReferenceLinkingData(negation);
							if (satCalcRefLinkData) {
								saturationIndiNode = (CIndividualSaturationProcessNode*)satCalcRefLinkData->getIndividualProcessNodeForConcept();
							}
						}
					}

					if (saturationIndiNode) {
						if (saturationIndiNode->getIndirectStatusFlags()->hasClashedFlag()) {
							return true;
						}
					}
					return false;
				}



				bool CCalculationTableauCompletionTaskHandleAlgorithm::insertConceptsToIndividualConceptSet(CConceptDescriptor* conceptDescriptor, CDependencyTrackPoint* dependencyTrackPoint, CIndividualProcessNode*& processIndi, CReapplyConceptLabelSet* conLabelSet, CCondensedReapplyQueueIterator* reapplyIt, bool allowInitalization, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptDescriptor* clashedConceptDescriptor = nullptr;
					CDependencyTrackPoint* clashedDependencyTrackPoint = nullptr;

					if (mConfConceptUnsatisfiabilitySaturatedTesting) {
						if (isConceptUnsatisfiabilitySaturated(conceptDescriptor->getConcept(),conceptDescriptor->isNegated(),calcAlgContext)) {
							CClashedDependencyDescriptor* clashConDesLinker = nullptr;
							clashConDesLinker = createClashedConceptDescriptor(clashConDesLinker,processIndi,conceptDescriptor,dependencyTrackPoint,calcAlgContext);
							throw CCalculationClashProcessingException(clashConDesLinker);
						}
					}

					bool contained = conLabelSet->insertConceptGetClash(conceptDescriptor,dependencyTrackPoint,reapplyIt,&clashedConceptDescriptor,&clashedDependencyTrackPoint);
					if (allowInitalization) {
						if (!processIndi->getIndividualInitializationConcept()) {
							processIndi->setIndividualInitializationConcept(conceptDescriptor);
						}
					}
					if (contained && clashedConceptDescriptor) {

						//mClashedDebugIndiModelString = generateDebugIndiModelStringList(calcAlgContext);

						//QFile file = QString("model.txt");
						//if (file.open(QIODevice::WriteOnly)) {
						//	file.write(mClashedDebugIndiModelString.replace("<br>","\r\n").toLocal8Bit());
						//	file.close();
						//}

						CClashedDependencyDescriptor* clashConDesLinker = nullptr;
						clashConDesLinker = createClashedConceptDescriptor(clashConDesLinker,processIndi,clashedConceptDescriptor,clashedDependencyTrackPoint,calcAlgContext);
						clashConDesLinker = createClashedConceptDescriptor(clashConDesLinker,processIndi,conceptDescriptor,dependencyTrackPoint,calcAlgContext);
						throw CCalculationClashProcessingException(clashConDesLinker);
					}



					//CConceptDescriptor* conDes = processIndi->getReapplyConceptLabelSet(false)->getAddingSortedConceptDescriptionLinker();
					//bool amountSubstanceFound = false;
					//for (CConceptDescriptor* conDesIt = conDes; conDesIt; conDesIt = conDesIt->getNext()) {
					//	CConcept* concept = conDesIt->getConcept();
					//	if (concept->getOperatorCode() == CCIMPLALL) {
					//		if (CIRIName::getRecentIRIName(concept->getRole()->getPropertyNameLinker()) == "http://purl.org/biotop/biotop.owl#granularPartOf") {
					//			CConcept* orCocnept = concept->getOperandList()->getData();
					//			if (orCocnept->getOperatorCode() == CCOR) {
					//				for (CSortedNegLinker<CConcept*>* opLinker = orCocnept->getOperandList(); opLinker; opLinker = opLinker->getNext()) {
					//					if (opLinker->getData()->getOperatorCode() == CCSUB && CIRIName::getRecentIRIName(opLinker->getData()->getClassNameLinker()) == "http://purl.org/imbi/dco/dco#AmountOfSubstance") {
					//						amountSubstanceFound = true;
					//					}
					//				}
					//			}
					//		}
					//	}
					//}
					//if (amountSubstanceFound) {
					//	bool base1Con = false;
					//	bool base2Con = false;
					//	for (CConceptDescriptor* conDesIt = conDes; conDesIt; conDesIt = conDesIt->getNext()) {
					//		if (CIRIName::getRecentIRIName(conDesIt->getData()->getClassNameLinker()) == "http://purl.org/biotop/biotop.owl#Atom") {
					//			base1Con = true;
					//		}
					//		if (CIRIName::getRecentIRIName(conDesIt->getData()->getClassNameLinker()) == "http://purl.org/biotop/biotop.owl#MonoMolecularEntity") {
					//			base2Con = true;
					//		}
					//	}
					//	if (!base2Con && !base1Con) {
					//		mEndTaskDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
					//		QFile file(QString("caching-error.txt"));
					//		if (file.open(QIODevice::WriteOnly)) {
					//			file.write(mEndTaskDebugIndiModelString.replace("<br>","\r\n").toLocal8Bit());
					//			file.close();
					//		}
					//		bool bug = true;
					//	}
					//}


					return contained;
				}




				void CCalculationTableauCompletionTaskHandleAlgorithm::addConceptsToIndividual(CConceptAssertionLinker* conceptAddLinkerIt, bool negate, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* dependencyTrackPoint, bool allowPreprocessing, bool allowInitalization, cint64* conceptCount, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CConceptProcessingQueue* conProQueue = processIndi->getConceptProcessingQueue(true);

					KONCLUDE_ASSERT_X(dependencyTrackPoint,"adding concept to individual","dependency track point missing");

					cint64 conCount = 0;
					CReapplyConceptLabelSet* conLabelSet = processIndi->getReapplyConceptLabelSet(true);
					while (conceptAddLinkerIt) {
						CConcept* concept = conceptAddLinkerIt->getData();
						bool conNeg = conceptAddLinkerIt->isNegated()^negate;
						KONCLUDE_ASSERT_X(concept,"adding concept to individual","concept missing");


						CConceptDescriptor* conceptDescriptor = CObjectAllocator<CConceptDescriptor>::allocateAndConstruct(taskMemMan);
						conceptDescriptor->initConceptDescriptor(concept,conNeg,dependencyTrackPoint);
						CCondensedReapplyQueueIterator reapplyIt;
						bool contained = insertConceptsToIndividualConceptSet(conceptDescriptor,dependencyTrackPoint,processIndi,conLabelSet,&reapplyIt,allowInitalization,calcAlgContext);
						if (!contained) {
							STATINC(CONCEPTSADDEDINDINODELABELSETCOUNT,calcAlgContext);
							addBlockingCoreConcept(conceptDescriptor,processIndi,conLabelSet,calcAlgContext);
							setIndividualNodeConceptLabelSetModified(processIndi,calcAlgContext);
							addConceptPreprocessedToProcessingQueue(conceptDescriptor,dependencyTrackPoint,conProQueue,processIndi,allowPreprocessing,calcAlgContext);
							if (reapplyIt.hasNext()) {
								// reapply reapplying concept
								applyReapplyQueueConcepts(processIndi,&reapplyIt,calcAlgContext);
							}
						}
						conceptAddLinkerIt = conceptAddLinkerIt->getNext();
					}
					if (conceptCount) {
						*conceptCount = conCount;
					}
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::addConceptsToIndividual(CXNegLinker<CConcept*>* conceptAddLinkerIt, bool negate, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* dependencyTrackPoint, bool allowPreprocessing, bool allowInitalization, cint64* conceptCount, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CConceptProcessingQueue* conProQueue = processIndi->getConceptProcessingQueue(true);

					KONCLUDE_ASSERT_X(dependencyTrackPoint,"adding concept to individual","dependency track point missing");

					cint64 conCount = 0;
					CReapplyConceptLabelSet* conLabelSet = processIndi->getReapplyConceptLabelSet(true);
					while (conceptAddLinkerIt) {
						CConcept* concept = conceptAddLinkerIt->getData();
						bool conNeg = conceptAddLinkerIt->isNegated()^negate;
						KONCLUDE_ASSERT_X(concept,"adding concept to individual","concept missing");


						CConceptDescriptor* conceptDescriptor = CObjectAllocator<CConceptDescriptor>::allocateAndConstruct(taskMemMan);
						conceptDescriptor->initConceptDescriptor(concept,conNeg,dependencyTrackPoint);
						CCondensedReapplyQueueIterator reapplyIt;
						bool contained = insertConceptsToIndividualConceptSet(conceptDescriptor,dependencyTrackPoint,processIndi,conLabelSet,&reapplyIt,allowInitalization,calcAlgContext);
						if (!contained) {
							STATINC(CONCEPTSADDEDINDINODELABELSETCOUNT,calcAlgContext);
							addBlockingCoreConcept(conceptDescriptor,processIndi,conLabelSet,calcAlgContext);
							setIndividualNodeConceptLabelSetModified(processIndi,calcAlgContext);
							addConceptPreprocessedToProcessingQueue(conceptDescriptor,dependencyTrackPoint,conProQueue,processIndi,allowPreprocessing,calcAlgContext);
							if (reapplyIt.hasNext()) {
								// reapply reapplying concept
								applyReapplyQueueConcepts(processIndi,&reapplyIt,calcAlgContext);
							}
						}
						conceptAddLinkerIt = conceptAddLinkerIt->getNext();
					}
					if (conceptCount) {
						*conceptCount = conCount;
					}
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::addConceptsToIndividual(CXSortedNegLinker<CConcept*>* conceptAddLinkerIt, bool negate, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* dependencyTrackPoint, bool allowPreprocessing, bool allowInitalization, cint64* conceptCount, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CConceptProcessingQueue* conProQueue = processIndi->getConceptProcessingQueue(true);

					KONCLUDE_ASSERT_X(dependencyTrackPoint,"adding concept to individual","dependency track point missing");

					cint64 conCount = 0;
					CReapplyConceptLabelSet* conLabelSet = processIndi->getReapplyConceptLabelSet(true);
					while (conceptAddLinkerIt) {
						CConcept* concept = conceptAddLinkerIt->getData();
						KONCLUDE_ASSERT_X(concept,"adding concept to individual","concept missing");
						bool conNeg = conceptAddLinkerIt->isNegated()^negate;


						CConceptDescriptor* conceptDescriptor = CObjectAllocator<CConceptDescriptor>::allocateAndConstruct(taskMemMan);
						conceptDescriptor->initConceptDescriptor(concept,conNeg,dependencyTrackPoint);
						CCondensedReapplyQueueIterator reapplyIt;
						bool contained = insertConceptsToIndividualConceptSet(conceptDescriptor,dependencyTrackPoint,processIndi,conLabelSet,&reapplyIt,allowInitalization,calcAlgContext);
						if (!contained) {
							STATINC(CONCEPTSADDEDINDINODELABELSETCOUNT,calcAlgContext);
							addBlockingCoreConcept(conceptDescriptor,processIndi,conLabelSet,calcAlgContext);
							setIndividualNodeConceptLabelSetModified(processIndi,calcAlgContext);
							addConceptPreprocessedToProcessingQueue(conceptDescriptor,dependencyTrackPoint,conProQueue,processIndi,allowPreprocessing,calcAlgContext);
							if (reapplyIt.hasNext()) {
								// reapply reapplying concept
								applyReapplyQueueConcepts(processIndi,&reapplyIt,calcAlgContext);
							}
						}
						conceptAddLinkerIt = conceptAddLinkerIt->getNext();
					}
					if (conceptCount) {
						*conceptCount = conCount;
					}
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::insertConceptProcessDescriptorToProcessingQueue(CConceptProcessDescriptor* conProDes, CConceptProcessingQueue*& conceptProcessingQueue, CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {

					CConcept* concept = conProDes->getConceptDescriptor()->getConcept();
					CConceptOperator* conOperator = concept->getConceptOperator();

					if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_PROPAGATION_TYPE)) {
						CProcessingDataBox* procDataBox = calcAlgContext->getProcessingDataBox();
						CIndividualConceptBatchProcessingQueue* varBindConBatchProcQueue = procDataBox->getVariableBindingConceptBatchProcessingQueue(true);
						varBindConBatchProcQueue->insertIndiviudalForConcept(concept,processIndi,conProDes);
					} else {
						conceptProcessingQueue->insertConceptProcessDescriptor(conProDes);
					}
				}

				void CCalculationTableauCompletionTaskHandleAlgorithm::insertConceptProcessDescriptorToProcessingQueue(CConceptProcessDescriptor* conProDes, CConceptProcessingQueue*& conceptProcessingQueue, cint64 bindingCount, CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {

					CConcept* concept = conProDes->getConceptDescriptor()->getConcept();
					CConceptOperator* conOperator = concept->getConceptOperator();

					if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_PROPAGATION_ALL_TYPE | CConceptOperator::CCFS_PROPAGATION_AND_TYPE)) {
						CProcessingDataBox* procDataBox = calcAlgContext->getProcessingDataBox();
						CIndividualConceptBatchProcessingQueue* varBindConBatchProcQueue = procDataBox->getVariableBindingConceptBatchProcessingQueue(true);
						varBindConBatchProcQueue->insertIndiviudalForBindingCount(concept,bindingCount,processIndi,conProDes);
					} else if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_PROPAGATION_TYPE)) {
						CProcessingDataBox* procDataBox = calcAlgContext->getProcessingDataBox();
						CIndividualConceptBatchProcessingQueue* varBindConBatchProcQueue = procDataBox->getVariableBindingConceptBatchProcessingQueue(true);
						varBindConBatchProcQueue->insertIndiviudalForConcept(concept,processIndi,conProDes);
					} else {
						conceptProcessingQueue->insertConceptProcessDescriptor(conProDes);
					}
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::addConceptToProcessingQueue(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint* depTrackPoint, CConceptProcessingQueue*& conceptProcessingQueue, CIndividualProcessNode*& processIndi, bool reapplied, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					if (needsProcessingForConcept(conceptDescriptor,depTrackPoint,processIndi,calcAlgContext)) {

						STATINC(CONCEPTSADDEDINDINODEPROCESSINGQUEUECOUNT,calcAlgContext);
						CConceptProcessDescriptor* conProDes = CObjectAllocator<CConceptProcessDescriptor>::allocateAndConstruct(taskMemMan);
						CConceptProcessPriority conProPriority = calcAlgContext->getUsedConceptPriorityStrategy()->getPriorityForConcept(conceptDescriptor,processIndi);
						conProDes->init(conceptDescriptor,conProPriority,reapplied,depTrackPoint);
						
						insertConceptProcessDescriptorToProcessingQueue(conProDes,conceptProcessingQueue,processIndi,calcAlgContext);

						//propagateIndividualUnprocessed(processIndi,calcAlgContext);
					}
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::needsProcessingForConcept(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint* depTrackPoint, CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					TableauRuleFunction func = nullptr;
					cint64 opCode = conceptDescriptor->getConcept()->getOperatorCode();
					bool conNeg = conceptDescriptor->isNegated();
					if (conNeg) {
						func = mNegJumpFuncVec[opCode];
					} else {
						func = mPosJumpFuncVec[opCode];
					}
					return func != nullptr;
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::addConceptPreprocessedToProcessingQueue(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint* depTrackPoint, CConceptProcessingQueue*& conceptProcessingQueue, CIndividualProcessNode*& processIndi, cint64 bindingCount, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					STATINC(CONCEPTSADDEDINDINODEPROCESSINGQUEUECOUNT,calcAlgContext);
					CConceptProcessPriority conProPriority = calcAlgContext->getUsedConceptPriorityStrategy()->getPriorityForConcept(conceptDescriptor,processIndi);					
					
					CConceptProcessDescriptor* conProDes = CObjectAllocator<CConceptProcessDescriptor>::allocateAndConstruct(taskMemMan);
					conProDes->init(conceptDescriptor,conProPriority,false,depTrackPoint);
					insertConceptProcessDescriptorToProcessingQueue(conProDes,conceptProcessingQueue,bindingCount,processIndi,calcAlgContext);
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::addConceptPreprocessedToProcessingQueue(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint* depTrackPoint, CConceptProcessingQueue*& conceptProcessingQueue, CIndividualProcessNode*& processIndi, bool allowPreprocessing, CCalculationAlgorithmContextBase* calcAlgContext, TableauRuleFunction skipFunction) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					// TODO: check necessary 
					TableauRuleFunction func = nullptr;
					cint64 opCode = conceptDescriptor->getConcept()->getOperatorCode();
					bool conNeg = conceptDescriptor->isNegated();
					if (conNeg) {
						func = mNegJumpFuncVec[opCode];
					} else {
						func = mPosJumpFuncVec[opCode];
					}
					if (!func || func == skipFunction) {
						return;
					}


					allowPreprocessing &= mConfDirectRulePreprocessing;
					if (mCurrentRecProcDepth > mCurrentRecProcDepthLimit) {
						allowPreprocessing = false;
					}


					CConcept* concept = conceptDescriptor->getConcept();
					CConceptOperator* conOperator = concept->getConceptOperator();

					if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_PROPAGATION_TYPE)) {
						allowPreprocessing = false;
					} 


					STATINC(CONCEPTSADDEDINDINODEPROCESSINGQUEUECOUNT,calcAlgContext);
					CConceptProcessPriority conProPriority = calcAlgContext->getUsedConceptPriorityStrategy()->getPriorityForConcept(conceptDescriptor,processIndi);
					
					
					CConceptProcessDescriptor* conProDes = CObjectAllocator<CConceptProcessDescriptor>::allocateAndConstruct(taskMemMan);
					conProDes->init(conceptDescriptor,conProPriority,false,depTrackPoint);
					if (allowPreprocessing && conProPriority.getPriority() >= mImmediatelyProcessPriority) {
						STATINC(RULEAPPLICATIONCOUNT,calcAlgContext);
						tableauRuleChoice(processIndi,conProDes,calcAlgContext);
					} else {
						insertConceptProcessDescriptorToProcessingQueue(conProDes,conceptProcessingQueue,processIndi,calcAlgContext);
						//propagateIndividualUnprocessed(processIndi,calcAlgContext);
					}
				}




				void CCalculationTableauCompletionTaskHandleAlgorithm::addConceptToProcessingQueue(CConceptProcessDescriptor* reinsertConProDes, CConceptProcessingQueue*& conceptProcessingQueue, CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(CONCEPTSADDEDINDINODEPROCESSINGQUEUECOUNT,calcAlgContext);
					conceptProcessingQueue->reinsertConceptProcessDescriptor(reinsertConProDes);
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::addCopiedConceptToProcessingQueue(CConceptProcessDescriptor* copyConProDes, CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					CIndividualProcessNode* locProcessNode = getLocalizedIndividual(processIndi,true,calcAlgContext);
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CConceptProcessDescriptor* conProDes = CObjectAllocator<CConceptProcessDescriptor>::allocateAndConstruct(taskMemMan);
					CConceptProcessPriority conProPriority = calcAlgContext->getUsedConceptPriorityStrategy()->getPriorityForConcept(copyConProDes->getConceptDescriptor(),locProcessNode);
					conProDes->initCopy(copyConProDes);
					CConceptProcessingQueue* conceptProcessingQueue = locProcessNode->getConceptProcessingQueue(true);
					insertConceptProcessDescriptorToProcessingQueue(conProDes,conceptProcessingQueue,processIndi,calcAlgContext);
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::addConceptRestrictedToProcessingQueue(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint* depTrackPoint, CConceptProcessingQueue*& conceptProcessingQueue, CIndividualProcessNode*& processIndi, bool reapplied, CProcessingRestrictionSpecification* procRestriction, CCalculationAlgorithmContextBase* calcAlgContext) {

					bool insertedWithPriorityOffset = false;
					if (procRestriction) {
						double priorityOffset = procRestriction->getPriorityOffset();
						insertedWithPriorityOffset = true;
						addConceptRestrictedToProcessingQueue(conceptDescriptor,depTrackPoint,conceptProcessingQueue,processIndi,reapplied,procRestriction,priorityOffset,calcAlgContext);
					}
					if (!insertedWithPriorityOffset) {
						addConceptRestrictedToProcessingQueue(conceptDescriptor,depTrackPoint,conceptProcessingQueue,processIndi,reapplied,procRestriction,0,calcAlgContext);
					}
				}


				void CCalculationTableauCompletionTaskHandleAlgorithm::addConceptRestrictedToProcessingQueue(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint* depTrackPoint, CConceptProcessingQueue*& conceptProcessingQueue, CIndividualProcessNode*& processIndi, bool reapplied, CProcessingRestrictionSpecification* procRestriction, double priorityOffset, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					if (needsProcessingForConcept(conceptDescriptor,depTrackPoint,processIndi,calcAlgContext)) {

						STATINC(CONCEPTSADDEDINDINODEPROCESSINGQUEUECOUNT,calcAlgContext);
						CConceptProcessDescriptor* conProDes = CObjectAllocator<CConceptProcessDescriptor>::allocateAndConstruct(taskMemMan);
						CConceptProcessPriority conProPriority = calcAlgContext->getUsedConceptPriorityStrategy()->getPriorityForConcept(conceptDescriptor,processIndi);
						conProPriority.addPriorityOffset(priorityOffset);
						conProDes->init(conceptDescriptor,conProPriority,reapplied,depTrackPoint,procRestriction);
						insertConceptProcessDescriptorToProcessingQueue(conProDes,conceptProcessingQueue,processIndi,calcAlgContext);


						//propagateIndividualUnprocessed(processIndi,calcAlgContext);
					}
				}



				void CCalculationTableauCompletionTaskHandleAlgorithm::addConceptRestrictedFixedPriorityToProcessingQueue(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint* depTrackPoint, CConceptProcessingQueue*& conceptProcessingQueue, CIndividualProcessNode*& processIndi, bool reapplied, CProcessingRestrictionSpecification* procRestriction, double priority, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = nullptr;
					taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();

					STATINC(CONCEPTSADDEDINDINODEPROCESSINGQUEUECOUNT,calcAlgContext);
					CConceptProcessDescriptor* conProDes = CObjectAllocator<CConceptProcessDescriptor>::allocateAndConstruct(taskMemMan);
					CConceptProcessPriority conProPriority(priority);
					conProDes->init(conceptDescriptor,conProPriority,reapplied,depTrackPoint,procRestriction);
					insertConceptProcessDescriptorToProcessingQueue(conProDes,conceptProcessingQueue,processIndi,calcAlgContext);

					//propagateIndividualUnprocessed(processIndi,calcAlgContext);
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::addIndividualToProcessingQueueBasedOnProcessingConcepts(CIndividualProcessNode* individual, CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* processingDataBox = calcAlgContext->getProcessingDataBox();
					bool individualInserted = false;
					CConceptProcessingQueue* conProQueue = individual->getConceptProcessingQueue(false);
					if (!conProQueue) {
						if (!mConfCurrentIndividualQueuing && calcAlgContext->getCurrentIndividualNode() == individual) {
							individualInserted = true;
						} else if (!individual->isImmediatelyProcessingQueued()) {
							individual->setImmediatelyProcessingQueued(true);
							CIndividualUnsortedProcessingQueue* unPrQueue = processingDataBox->getIndividualImmediatelyProcessingQueue(true);
							unPrQueue->insertIndiviudalProcessNode(individual);
							individualInserted = true;
						}
					} else if (!conProQueue->isEmpty()) {
						CConceptProcessPriority conProPri;
						if (conProQueue->getNextConceptProcessPriority(&conProPri)) {
							double priority = conProPri.getPriority();
							if (priority >= mImmediatelyProcessPriority) {
								if (!mConfCurrentIndividualQueuing && calcAlgContext->getCurrentIndividualNode() == individual) {
									individualInserted = true;
								} else if (!individual->isImmediatelyProcessingQueued()) {
									individual->setImmediatelyProcessingQueued(true);
									CIndividualUnsortedProcessingQueue* unPrQueue = processingDataBox->getIndividualImmediatelyProcessingQueue(true);
									unPrQueue->insertIndiviudalProcessNode(individual);
									individualInserted = true;
								}
							} else if (mOptDetExpPreporcessing && priority >= mDeterministicProcessPriority) {
								if (!individual->isDeterministicExpandingProcessingQueued()) {
									individual->setDeterministicExpandingProcessingQueued(true);
									CIndividualDepthProcessingQueue* unPrQueue = processingDataBox->getIndividualDepthDeterministicExpansionPreprocessingQueue(true);
									unPrQueue->insertProcessIndiviudal(individual);
									individualInserted = true;
								}
							} else {
								if (!individual->isRegularDepthProcessingQueued()) {
									individual->setRegularDepthProcessingQueued(true);

									if (individual->isNominalIndividual() && individual->getIndividualNominalLevelOrAncestorDepth() <= 0) {
										if (!processingDataBox->hasNominalNonDeterministicProcessingNodesSorted()) {
											CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
											CXLinker<CIndividualProcessNode*>* linker = CObjectAllocator< CXLinker<CIndividualProcessNode*> >::allocateAndConstruct(taskMemMan);
											linker->initLinker(individual);
											processingDataBox->addSortedNominalNonDeterministicProcessingNodeLinker(linker);
										} else {
											CIndividualDepthProcessingQueue* nominalProQueue = processingDataBox->getNominalProcessingQueue(true);										
											nominalProQueue->insertProcessIndiviudal(individual);
										}
									} else {
										CIndividualDepthProcessingQueue* inDepthProQueue = processingDataBox->getIndividualDepthProcessingQueue(true);
										inDepthProQueue->insertProcessIndiviudal(individual);
									}
									individualInserted = true;
								}
							}
						}
					}
					return individualInserted;
				}


				bool CCalculationTableauCompletionTaskHandleAlgorithm::addIndividualToProcessingQueue(CIndividualProcessNode* individual, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					bool individualInserted = false;

					CProcessingDataBox* processingDataBox = calcAlgContext->getProcessingDataBox();

					if (mConfDepthOrientatedProcessing && !individual->isNominalIndividual() && !individual->isExtendedQueueProcessing()) {
						if (!individual->isProcessingQueued()) {
							CIndividualUnsortedProcessingQueue* depthProcessingQueue = processingDataBox->getIndividualDepthFirstProcessingQueue(true);
							depthProcessingQueue->insertIndiviudalProcessNode(individual);
							individual->setProcessingQueued(true);
							individualInserted = true;
						}

					} else {

						bool insertIndividual = true;
						bool individualBlocked = false;
						if (insertIndividual && individual->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFDIRECTBLOCKED)) {
							individualBlocked = true;
							if (!individual->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFBLOCKINGRETESTDUEDIRECTMODIFIED | CIndividualProcessNode::PRFBLOCKINGRETESTDUEBLOCKERMODIFIED)) {
								insertIndividual = false;
							}
						}
						//if (insertIndividual && individual->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSATISFIABLECACHED)) {
						//	individualBlocked = true;
						//	if (!individual->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFRETESTSATISFIABLECACHEDDUEDIRECTMODIFIED)) {
						//		insertIndividual = false;
						//	}
						//}
						//if (insertIndividual && individual->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSIGNATUREBLOCKINGCACHED)) {
						//	individualBlocked = true;
						//	if (!individual->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFRETESTSIGNATUREBLOCKINGCACHEDDUEDIRECTMODIFIED)) {
						//		insertIndividual = false;
						//	}
						//}
						if (insertIndividual && individual->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHED)) {
							individualBlocked = true;
							if (!individual->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFRETESTCOMPLETIONGRAPHCACHEDDUEDIRECTMODIFIED | CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED | CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALID)) {
								insertIndividual = false;
							}
						}

						if (insertIndividual && individual->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINDIRECTBLOCKED)) {
							individualBlocked = true;
							if (!individual->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFBLOCKINGRETESTDUEINDIRECTBLOCKERLOSS)) {
								insertIndividual = false;
							}
						}
						if (insertIndividual && individual->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATISFIABLECACHED)) {
							individualBlocked = true;
							if (!individual->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATISFIABLECACHEDABOLISHED)) {
								insertIndividual = false;
							}
						}
						if (insertIndividual && individual->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSIGNATUREBLOCKINGCACHED)) {
							individualBlocked = true;
							if (!individual->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSIGNATUREBLOCKINGCACHEDABOLISHED)) {
								insertIndividual = false;
							}
						}
						if (insertIndividual && individual->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATURATIONBLOCKINGCACHED)) {
							individualBlocked = true;
							if (!individual->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFANCESTORSATURATIONBLOCKINGCACHEDABOLISHED)) {
								insertIndividual = false;
							}
						}
						if (insertIndividual && individual->isDelayedNominalProcessingQueued()) {
							individualBlocked = true;
							insertIndividual = false;
						}

						if (individualBlocked && insertIndividual) {
							if (mConfLateBlockingResolving) {
								if (!individual->isBlockedReactivationProcessingQueued()) {
									individual->setBlockedReactivationProcessingQueued(true);

									CIndividualDepthProcessingQueue* blockReactProQueue = processingDataBox->getBlockedReactivationProcessingQueue(true);
									blockReactProQueue->insertProcessIndiviudal(individual);

									//CXLinker<CIndividualProcessNode*>* indiNodeLinker = CObjectAllocator< CXLinker<CIndividualProcessNode*> >::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
									//indiNodeLinker->initLinker(individual);
									//processingDataBox->addIndividualNodeBlockedResolveLinker(indiNodeLinker);
									individualInserted = true;
								}
							} else {
								individualInserted = addIndividualToProcessingQueueBasedOnProcessingConcepts(individual,calcAlgContext);
								if (!individualInserted) {
									if (!individual->isRegularDepthProcessingQueued()) {
										individual->setRegularDepthProcessingQueued(true);
										CIndividualDepthProcessingQueue* inDepthProQueue = processingDataBox->getIndividualDepthProcessingQueue(true);
										inDepthProQueue->insertProcessIndiviudal(individual);
										individualInserted = true;
									}
								}
							}							
						}

						if (!individualBlocked) {
							individualInserted = addIndividualToProcessingQueueBasedOnProcessingConcepts(individual,calcAlgContext);
						}


					}
					if (individualInserted) {
						STATINC(INDINODESADDEDPROCESSINGQUEUECOUNT,calcAlgContext);
					}
					return individualInserted;
				}





				bool CCalculationTableauCompletionTaskHandleAlgorithm::addIndividualToBlockingUpdateReviewProcessingQueue(CIndividualProcessNode* individual, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualDepthProcessingQueue* sigBlockUpdProQueue = calcAlgContext->getProcessingDataBox()->getBlockingUpdateReviewProcessingQueue(true);
					sigBlockUpdProQueue->insertProcessIndiviudal(individual);
					STATINC(INDINODESADDEDPROCESSINGQUEUECOUNT,calcAlgContext);
					return true;
				}

				cint64 CCalculationTableauCompletionTaskHandleAlgorithm::getAppliedANDRuleCount() {
					return mAppliedANDRuleCount;
				}

				cint64 CCalculationTableauCompletionTaskHandleAlgorithm::getAppliedORRuleCount() {
					return mAppliedORRuleCount;
				}

				cint64 CCalculationTableauCompletionTaskHandleAlgorithm::getAppliedSOMERuleCount() {
					return mAppliedSOMERuleCount;
				}

				cint64 CCalculationTableauCompletionTaskHandleAlgorithm::getAppliedATLEASTRuleCount() {
					return mAppliedATLEASTRuleCount;
				}

				cint64 CCalculationTableauCompletionTaskHandleAlgorithm::getAppliedALLRuleCount() {
					return mAppliedALLRuleCount;
				}

				cint64 CCalculationTableauCompletionTaskHandleAlgorithm::getAppliedATMOSTRuleCount() {
					return mAppliedATMOSTRuleCount;
				}

				cint64 CCalculationTableauCompletionTaskHandleAlgorithm::getAppliedTotalRuleCount() {
					return mAppliedTOTALRuleCount;
				}



			}; // end namespace Algorithm 

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
