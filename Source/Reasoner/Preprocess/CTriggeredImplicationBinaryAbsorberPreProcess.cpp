/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CTriggeredImplicationBinaryAbsorberPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


#ifdef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED
	#define KONCLUCE_PREPROCESSING_TRIGGERED_IMPLICATION_STRING_INSTRUCTION(a)
#else
	//#define KONCLUCE_PREPROCESSING_TRIGGERED_IMPLICATION_STRING_INSTRUCTION(a) a
	#define KONCLUCE_PREPROCESSING_TRIGGERED_IMPLICATION_STRING_INSTRUCTION(a)
#endif



			CTriggeredImplicationBinaryAbsorberPreProcess::CTriggeredImplicationBinaryAbsorberPreProcess() {
				mConfRequiresExistingInverseRole = false;
				mConfAbsorbEqClassDefinitions = true;
				mConfAbsorbDisjunctions = true;
				mConfAbsorbSimpleDisjunctions = true;
				mConfPartialGCIAbsorption = true;
				mConfPartialAbsorbEqClassDefinitions = true;
				mConfPartialDisjunctionsAbsorption = true;
				mConfOnlyPositiveDisjunctionsAbsorption = true;
				mConfNominalSchemaPathPreabsorption = true;
				mConfNominalSchemaBackPropagation = true;
				mConfNominalSchemaBackPropagationMultiTrigger = true;
				mConfImplicationsForTriggerReusue = true;

				mConfOftenTriggerUseCountPunishment = 20;
				mConfPunishOftenSameTriggerUse = true;
				mConfDatatypeAbsorption = true;

				mConfCardinalityQualificationTriggerAbsorption = true;
				mConfPartialCardinalityQualificationTriggerAbsorption = true;
				mConfOnlyPositiveCardinalityQualificationTriggerAbsorption = false;

				mConfGlobalNegatedSelfRestrictionsToRoleDomainAbsorption = true;

				mLastDisjunctId = 0;
				mOnto = nullptr;
			}


			CTriggeredImplicationBinaryAbsorberPreProcess::~CTriggeredImplicationBinaryAbsorberPreProcess() {
				qDeleteAll(mTriggerContainer);
				mTriggerContainer.clear();
			}


			CConcreteOntology *CTriggeredImplicationBinaryAbsorberPreProcess::preprocess(CConcreteOntology *ontology, CPreProcessContext* context) {
				CMBox *mBox = ontology->getDataBoxes()->getMBox();
				CTBox *tBox = ontology->getDataBoxes()->getTBox();
				CRBox *rBox = ontology->getDataBoxes()->getRBox();
				CABox *aBox = ontology->getDataBoxes()->getABox();
				mMBox = mBox;
				mTBox = tBox;

				mTmpTriggerLinker = nullptr;
				mLastDisjunctId = 0;

				mConceptVec = tBox->getConceptVector();
				cint64 conceptCount = mConceptVec->getItemCount();

				CRoleVector *roles = rBox->getRoleVector();

				CConcept *topConcept = tBox->getTopConcept();
				mBottomConcept = tBox->getBottomConcept();
				mTopConcept = tBox->getTopConcept();

				mMemMan = ontology->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();
				mOnto = ontology;


				CConfiguration* config = context->getConfiguration();
				mConfAbsorbDisjunctions = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.DisjunctionToImplicationAbsorption",true);
				mConfAbsorbSimpleDisjunctions = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.DisjunctionToImplicationAbsorptionByExistingTriggers",true);
				mConfAbsorbEqClassDefinitions = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.EquivalentDefinitionToSubclassImplicationAbsorption",true);
				mConfPartialGCIAbsorption = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.PartialGCIImplicationAbsorption",true);
				mConfPartialAbsorbEqClassDefinitions = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.PartialEquivalentDefinitionToCandidatesAbsorption",true);
				mConfPartialDisjunctionsAbsorption = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.PartialDisjunctionAbsorption",true);

				mConfNominalSchemaBackPropagation = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.NominalSchemaBackPropagation",true);
				mConfNominalSchemaPathPreabsorption = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.NominalSchemaPathPreabsorption",true);
				mConfDatatypeAbsorption = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.DatatypeAbsorption",true);
				mConfNominalSchemaPathPreabsorption = false;

				mConfCardinalityQualificationTriggerAbsorption = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.CardinalityQualificationTriggerAbsorption",true);
				mConfPartialCardinalityQualificationTriggerAbsorption = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.CardinalityQualificationTriggerPartialAbsorption",true);
				mConfOnlyPositiveCardinalityQualificationTriggerAbsorption = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.CardinalityQualificationTriggerOnlyPositiveAbsorption",false);

				
				if (mConfAbsorbDisjunctions || mConfAbsorbSimpleDisjunctions) {
					mImpRepVec = mBox->getImplicationReplacementVector(true);
				}

				mTriggerImplHash = tBox->getTriggerImplicationHash(true);
				mEquivConCandidateHash = tBox->getEquivalentConceptCandidateHash(true);
				mCandidateEquivConHash = tBox->getCandidateEquivalentConceptHash(true);
				mEquivConNonCandidateSet = tBox->getEquivalentConceptNonCandidateSet(true);
				mRoleDomainTriggerConceptHash = tBox->getRoleDomainTriggerConceptHash(false);
				mIndividualTriggerConceptHash = tBox->getIndividualTriggerConceptHash(false);

				mBranchTriggVec = mBox->getBranchingTriggerVector();
				mNomSchemaTemplVec = mBox->getNominalSchemaTemplateVector(false);

				mNextVariableID = 0;

				mStatInvRolesGenerated = 0;
				mStatGCIAbsorbed = 0;
				mStatGCINormalAbsorbed = 0;
				mStatGCINormalSchemaAbsorbed = 0;
				mStatGCIAssertionAbsorbed = 0;
				mStatGCITotal = 0;
				mStatEqAbsorbed = 0;
				mStatEqTotal = 0;
				mStatImplConceptsGenerated = 0;
				mStatGCIAbsorptionPartialExtended = 0;
				mStatGCIAbsorptionAssuring = 0;
				mStatGCIPartialAbsorbed = 0;
				mStatReusedImplications = 0;
				mStatOptimizedRemovedTriggers = 0;


				if (ontology->getIncrementalRevisionData()->isIncrementalOntology()) {
					if (ontology->getIncrementalRevisionData()->getAxiomChangeData()->hasChangedABoxAxioms()) {
						CIndividualVector* indiVec = aBox->getIndividualVector(false);
						if (indiVec && mIndividualTriggerConceptHash) {
							for (CBOXHASH<cint64,CConcept*>::const_iterator it = mIndividualTriggerConceptHash->constBegin(), itEnd = mIndividualTriggerConceptHash->constEnd(); it != itEnd; ++it) {
								cint64 indiID = it.key();
								CConcept* triggerConcept = it.value();
								CIndividual* locIndi = indiVec->getLocalData(indiID);
								if (locIndi) {
									CConceptAssertionLinker* assLinker = CObjectAllocator< CConceptAssertionLinker >::allocateAndConstruct(mMemMan);
									assLinker->initNegLinker(triggerConcept,false);
									locIndi->addAssertionConceptLinker(assLinker);
								}
							}
						}
					}					
				}


				if (mConfAbsorbEqClassDefinitions) {

					QList<CAbsorpEquivalentClassItem> equivConceptAbsorbItemList;
					QList<CConcept*> equivConceptNotAbsorbList;
					// try to absorb equivalent classes expressions
					for (cint64 conIdx = 0; conIdx < conceptCount; conIdx++) {
						CConcept* concept = mConceptVec->getLocalData(conIdx);
						if (concept) {
							cint64 opCode = concept->getOperatorCode();
							if (opCode == CCEQ) {
								++mStatEqTotal;

								if (isEquivalenceConceptTriggeredImplicationAbsorbable(concept,false)) {
									// absorb to subclass and implication trigger
									concept->setOperatorCode(CCSUB);
									equivConceptAbsorbItemList.append(CAbsorpEquivalentClassItem(concept,concept->getOperandList()));
									++mStatEqAbsorbed;
								} else {
									equivConceptNotAbsorbList.append(concept);
								}
							}
						}
					}

					//foreach (CConcept* concept, equivConceptAbsorbList) {
					//	createEquivalenceAbsorbedTriggeredImplication(concept,false);
					//}
					foreach (const CAbsorpEquivalentClassItem& eqConceptAbsItem, equivConceptAbsorbItemList) {
						createEquivalenceAbsorbedTriggeredImplication(eqConceptAbsItem.mEqConcept,false,eqConceptAbsItem.mAbsorpOpLinker);
					}

					foreach (CConcept* concept, equivConceptNotAbsorbList) {

						//QString iriClassNameString = CIRIName::getRecentIRIName(concept->getClassNameLinker());
						//if (iriClassNameString == "http://rewerse.net/I5/NS/2006/r3#LanguageResource") {
						//	COntologyTextFormater::writeOntologyToFile(ontology,"xyztTBox.txt");
						//	bool bug = true;
						//}

						if (mConfPartialAbsorbEqClassDefinitions && isEquivalenceConceptCandidateExtractable(concept,false)) {
							CConcept* candidateTriggerConcept = getEquivalenceConceptCandidateTriggeredImplication(concept,false);
							CConcept* eqConCand = createCandidateEquivalentConcept(concept);
							addUnfoldingConceptForConcept(candidateTriggerConcept,eqConCand,false);
							mEquivConCandidateHash->insert(concept,eqConCand);
							mCandidateEquivConHash->insert(eqConCand,concept);
						} else {
							mEquivConNonCandidateSet->insert(concept);
						}
					}

				}

				mFirstNewConceptTag = mConceptVec->getItemCount();
				mLastNewConceptTag = mFirstNewConceptTag;


				if (topConcept) {
					CSortedNegLinker<CConcept*>* gciConLinker = topConcept->getOperandList();

					QList< TConceptNegationPair > replaceConceptList;
					CSortedNegLinker<CConcept*>* lastgciConLinker = nullptr;
					CSortedNegLinker<CConcept*>* gciConLinkerIt = gciConLinker;
					while (gciConLinkerIt) {
						CConcept* opCon = gciConLinkerIt->getData();
						bool opNegation = gciConLinkerIt->isNegated();
						bool removed = false;
						cint64 opCode = opCon->getOperatorCode();
						if (!opNegation && opCode == CCAND || opNegation && opCode == CCOR) {
							collectConjunctions(opCon,opNegation,replaceConceptList);
							removed = true;
						}
						if (removed) {
							if (lastgciConLinker) {
								lastgciConLinker->setNext(gciConLinkerIt->getNext());
							} else {
								topConcept->setOperandList(gciConLinkerIt->getNext());
							}
							topConcept->incOperandCount(-1);
						} else {
							lastgciConLinker = gciConLinkerIt;
						}

						gciConLinkerIt = gciConLinkerIt->getNext();
					}

					for (QList< TConceptNegationPair >::const_iterator it = replaceConceptList.constBegin(), itEnd = replaceConceptList.constEnd(); it != itEnd; ++it) {
						TConceptNegationPair conNegPair(*it);
						addConceptToImplied(topConcept,conNegPair.first,conNegPair.second);
					}

					lastgciConLinker = nullptr;
					gciConLinker = topConcept->getOperandList();
					gciConLinkerIt = gciConLinker;
					while (gciConLinkerIt) {
						CConcept* opCon = gciConLinkerIt->getData();
						bool opNegation = gciConLinkerIt->isNegated();
						++mStatGCITotal;
						bool absorbed = false;

						mFirstNewConceptTag = mLastNewConceptTag;


						//QString iriClassNameString = CIRIName::getRecentIRIName(opCon->getOperandList()->getData()->getClassNameLinker());
						//if (iriClassNameString == "http://purl.org/imbi/dco/dco#BacterialInfectiousPathologicalProcess") {
						//	COntologyTextFormater::writeOntologyToFile(ontology,"xyztTBox.txt");
						//	bool bug = true;
						//}


						KONCLUCE_PREPROCESSING_TRIGGERED_IMPLICATION_STRING_INSTRUCTION(mDebugLastGCIString = generateDebugGCIConceptString(opCon,opNegation));
						

						if (!absorbed && absorbNominalConceptsToAssertions(opCon, opNegation)) {
							++mStatGCIAssertionAbsorbed;
							absorbed = true;
						}


						if (!absorbed && mConfGlobalNegatedSelfRestrictionsToRoleDomainAbsorption && absorbNegatedSelfToRoleDomain(opCon, opNegation)) {
							// better for saturation
							++mStatGCIAssertionAbsorbed;
							absorbed = true;
						}


						if (!absorbed && absorbNominalSchemaGCIConceptsToTriggeredImplications(opCon,opNegation)) {
							++mStatGCINormalSchemaAbsorbed;
							absorbed = true;
						}

						if (!absorbed && absorbGCIConceptsToTriggeredImplications(opCon,opNegation)) {
							++mStatGCINormalAbsorbed;
							KONCLUCE_PREPROCESSING_TRIGGERED_IMPLICATION_STRING_INSTRUCTION(mDebugLastAbsorbedGCITechniqueString = QString("implication absorbed"));
							absorbed = true;
						}


						if (!absorbed && mConfPartialGCIAbsorption) {
							if (absorbPartialGCIConceptsToTriggeredImplications(opCon,opNegation)) {
								++mStatGCIPartialAbsorbed;
								KONCLUCE_PREPROCESSING_TRIGGERED_IMPLICATION_STRING_INSTRUCTION(mDebugLastAbsorbedGCITechniqueString = QString("partial implication absorbed"));
								absorbed = true;
							}
						}


						if (absorbed) {
							++mStatGCIAbsorbed;
							mLastNewConceptTag = mConceptVec->getItemCount();
							KONCLUCE_PREPROCESSING_TRIGGERED_IMPLICATION_STRING_INSTRUCTION(mDebugLastAbsorbedGCIConceptString = generateDebugGeneratedAbsorbedImplicationString(mFirstNewConceptTag,mLastNewConceptTag));
							KONCLUCE_PREPROCESSING_TRIGGERED_IMPLICATION_STRING_INSTRUCTION(mDebugLastAbsorbedGCIString = QString("GCI:<br>\r\n<br>\r\n%1<br>\r\n%2 to<br>\r\n<br>\r\n%3<p>\r\n\r\n").arg(mDebugLastGCIString).arg(mDebugLastAbsorbedGCITechniqueString).arg(mDebugLastAbsorbedGCIConceptString));
							KONCLUCE_PREPROCESSING_TRIGGERED_IMPLICATION_STRING_INSTRUCTION(mDebugTotalAbsorbedGCIString += mDebugLastAbsorbedGCIString);
							if (lastgciConLinker) {
								lastgciConLinker->setNext(gciConLinkerIt->getNext());
							} else {
								topConcept->setOperandList(gciConLinkerIt->getNext());
							}
							topConcept->incOperandCount(-1);
						} else {
							lastgciConLinker = gciConLinkerIt;
						}

						gciConLinkerIt = gciConLinkerIt->getNext();
					}
				}



				if (mConfOnlyPositiveDisjunctionsAbsorption) {
					for (cint64 conIdx = 0; conIdx < conceptCount; conIdx++) {
						CConcept* concept = mConceptVec->getData(conIdx);
						if (concept && concept->hasClassName()) {
							collectPositiveConcepts(concept,false,&mPositiveConceptReferencedSet);
						}
					}
				}


				if (mConfCardinalityQualificationTriggerAbsorption) {
					for (cint64 conIdx = 0; conIdx < conceptCount; conIdx++) {
						CConcept* concept = mConceptVec->getLocalData(conIdx);
						if (concept) {
							cint64 opCode = concept->getOperatorCode();
							bool absorbable = false;
							bool absorbNeg = false;
							bool absorbPos = false;
							bool partialAbsorbable = false;
							if (opCode == CCATMOST) {
								absorbNeg = true;
								absorbPos = true;
							} else if (opCode == CCATLEAST) {
								absorbNeg = false;
								absorbPos = true;
							}
							if (absorbPos) {
								if (!mConfOnlyPositiveCardinalityQualificationTriggerAbsorption || mPositiveConceptReferencedSet.contains(TConceptNegationPair(concept,absorbNeg))) {
									if (isCardinalityConceptQualificationTriggerAbsorbable(concept,absorbNeg)) {
										absorbable = true;
									} else if (mConfPartialCardinalityQualificationTriggerAbsorption && isCardinalityConceptPartialQualificationTriggerAbsorbable(concept,absorbNeg)) {
										absorbable = true;
									}
								}
							}
							CConcept* absorbedImpCon = nullptr;
							if (absorbable) {
								CConceptRoleBranchingTrigger* branchingTriggers = createCardinalityQualificationPartialAbsorbedTriggers(concept,absorbNeg);
								mBranchTriggVec->setData(conIdx,branchingTriggers);
							}
						}
					}
				}


				createConceptOfInterestAbsorptions(false);
				createDisjunctionAbsorptions(false);

				for (QList<CSortedNegLinker<CConcept*>*>::const_iterator it = mLaterTopConceptTriggerAddList.constBegin(), itEnd = mLaterTopConceptTriggerAddList.constEnd(); it != itEnd; ++it) {
					CSortedNegLinker<CConcept*>* newOpLinker = *it;
					topConcept->addOperandLinker(newOpLinker);
					topConcept->incOperandCount();
				}


				asorbForallsToRanges(topConcept);




				mStatInvRolesGenerated = mRoleInverseRoleCreateHash.count();
				CConcreteOntologyInverseRoleBuilder::createDelayedInverseRoles(&mRoleInverseRoleCreateHash,mOnto);


				optimizeTriggerStructure();


				LOG(INFO,"::Konclude::Reasoner::Preprocess::TriggeredImplicationAbsorber",logTr("Absorbed %1 of %2 equivalence class definitions.").arg(mStatEqAbsorbed).arg(mStatEqTotal),this);
				LOG(INFO,"::Konclude::Reasoner::Preprocess::TriggeredImplicationAbsorber",logTr("Absorbed %1 of %2 GCI definitions.").arg(mStatGCIAbsorbed).arg(mStatGCITotal),this);
				if (mStatGCITotal > 0) {
					LOG(INFO,"::Konclude::Reasoner::Preprocess::TriggeredImplicationAbsorber",logTr("Absorbed %1 GCI definitions only partially and extended %2 absorptions partially.").arg(mStatGCIPartialAbsorbed).arg(mStatGCIAbsorptionPartialExtended),this);
				}
				if (mStatGCINormalSchemaAbsorbed > 0) {
					LOG(INFO,"::Konclude::Reasoner::Preprocess::TriggeredImplicationAbsorber",logTr("Absorbed %1 GCI with nominal schemas.").arg(mStatGCINormalSchemaAbsorbed),this);
				}
				if (mStatGCIAssertionAbsorbed > 0) {
					LOG(INFO, "::Konclude::Reasoner::Preprocess::TriggeredImplicationAbsorber", logTr("Absorbed %1 GCI to assertions.").arg(mStatGCIAssertionAbsorbed), this);
				}
				LOG(INFO,"::Konclude::Reasoner::Preprocess::TriggeredImplicationAbsorber",logTr("Generated %1 triggered implication concepts and %2 additional inverse roles.").arg(mStatImplConceptsGenerated).arg(mStatInvRolesGenerated),this);
	
				return ontology;
			}




			CConcreteOntology* CTriggeredImplicationBinaryAbsorberPreProcess::continuePreprocessing() {
				if (mOnto) {
					createConceptOfInterestAbsorptions(true);
					createDisjunctionAbsorptions(true);
				}
				return mOnto;
			}


			void CTriggeredImplicationBinaryAbsorberPreProcess::createConceptOfInterestAbsorptions(bool continuationExtension) {
				CBOXLIST<TConceptNegPair>* conIntList = mTBox->getConceptOfInterestList(false);
				if (conIntList && !conIntList->isEmpty()) {
					CBOXSET<TConceptNegPair>* triggeredConIntSet = mTBox->getTriggeredConceptOfInterestSet(true);
					cint64 conIntCount = conIntList->count();
					for (cint64 triggeredCount = triggeredConIntSet->count(); triggeredCount < conIntCount; ++triggeredCount) {
						TConceptNegPair conNegPair(conIntList->at(triggeredCount));
						CConcept* concept = conNegPair.first;						
						bool negation = conNegPair.second;
						if (!triggeredConIntSet->contains(conNegPair)) {
							createConceptOfInterestAbsorption(concept, negation, continuationExtension);
							triggeredConIntSet->insert(conNegPair);
							collectPositiveConcepts(concept, negation, &mPositiveConceptReferencedSet);
						}
					}
				}
			}



			void CTriggeredImplicationBinaryAbsorberPreProcess::createDisjunctionAbsorptions(bool continuationExtension) {
				cint64 conceptCount = mConceptVec->getItemCount();
				if (mConfAbsorbDisjunctions || mConfPartialDisjunctionsAbsorption || mConfAbsorbSimpleDisjunctions) {
					for (cint64 conIdx = mLastDisjunctId; conIdx < conceptCount; conIdx++) {
						CConcept* concept = mConceptVec->getLocalData(conIdx);
						if (concept) {

							bool requiresBackpropagation = false;
							if (continuationExtension && mLastDisjunctId > 0) {
								requiresBackpropagation = true;
							}

							cint64 opCode = concept->getOperatorCode();
							bool absorb = false;
							bool absorbable = false;
							bool absorbNeg = false;
							bool absorbPos = false;
							bool partialAbsorbable = false;
							cint64 opCount = concept->getOperandCount();
							if (opCount > 1) {
								if (opCode == CCAND) {
									absorbNeg = true;
									absorbPos = true;
									absorb = true;
								} else if (opCode == CCOR) {
									absorbNeg = false;
									absorbPos = true;
									absorb = true;
								}
								if (absorb) {
									bool localRepData = false;
									CReplacementData* prevReplData = mImpRepVec->getData(conIdx, &localRepData);
									if ((!prevReplData || !localRepData)) {
										if (absorbPos) {
											if (!mConfOnlyPositiveDisjunctionsAbsorption || mPositiveConceptReferencedSet.contains(TConceptNegationPair(concept, absorbNeg))) {
												if (mConfAbsorbSimpleDisjunctions && isDisjunctionConceptSimpleTriggeredImplicationAbsorbable(concept, absorbNeg)) {
													absorbable = true;
												} else if (mConfAbsorbDisjunctions && isDisjunctionConceptTriggeredImplicationAbsorbable(concept, absorbNeg)) {
													// add implication trigger absorption
													absorbable = true;
												} else if (mConfPartialDisjunctionsAbsorption && isDisjunctionConceptPartialTriggeredImplicationAbsorbable(concept, absorbNeg)) {
													partialAbsorbable = true;
												}
											}
										}


										CConcept* absorbedImpCon = nullptr;
										if (!requiresBackpropagation) {
											if (absorbable) {
												absorbedImpCon = createDisjunctionAbsorbedTriggeredImplication(concept, absorbNeg);
											} else if (partialAbsorbable) {
												CConceptRoleBranchingTrigger* branchingTriggers = createDisjunctionPartialAbsorbedBranchTriggers(concept, absorbNeg);
												mBranchTriggVec->setData(concept->getConceptTag(), branchingTriggers);
											}
										} else {
											if (absorbable || partialAbsorbable) {
												if (!mCreatedImpliedDisjunctions.contains(concept)) {
													CConcept* disjBackPropTrigger = createTriggerConcept(true);
													absorbedImpCon = createDisjunctionAbsorbedBackpropagationTriggeredImplication(concept, absorbNeg, disjBackPropTrigger);
													addUnfoldingConceptForConcept(disjBackPropTrigger, absorbedImpCon, false);
													absorbedImpCon = disjBackPropTrigger;
												}
											}
										}

										if (absorbedImpCon) {

											CReplacementData* replData = prevReplData;
											if (!localRepData) {
												replData = CObjectAllocator<CReplacementData>::allocateAndConstruct(mMemMan);
												replData->initReplacementData(prevReplData);
												mImpRepVec->setData(concept->getConceptTag(), replData);
											}
											replData->setImplicationReplacementConcept(absorbedImpCon);
										}

									}
								}
							}
						}
					}
					mLastDisjunctId = conceptCount;
				}
			}


			void CTriggeredImplicationBinaryAbsorberPreProcess::asorbForallsToRanges(CConcept* topConcept) {
				CSortedNegLinker<CConcept*>* gciConLinker = topConcept->getOperandList();
				CSortedNegLinker<CConcept*>* lastgciConLinker = nullptr;
				CSortedNegLinker<CConcept*>* gciConLinkerIt = gciConLinker;
				while (gciConLinkerIt) {
					CConcept* opCon = gciConLinkerIt->getData();
					bool opNegation = gciConLinkerIt->isNegated();
					bool removed = false;
					cint64 opCode = opCon->getOperatorCode();
					if (!opNegation && (opCode == CCALL || opCode == CCIMPLALL) || opNegation && (opCode == CCSOME)) {
						CRole* role = opCon->getRole();
						CRole* locatedRole = CConceptRoleIndividualLocator::getLocatedRole(role,mOnto);
						for (CSortedNegLinker<CConcept*>* opLinkerIt = opCon->getOperandList(); opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
							addRoleRangeConcept(locatedRole,opLinkerIt->getData(),opLinkerIt->isNegated()^opNegation);
						}
						removed = true;
					}
					if (removed) {
						if (lastgciConLinker) {
							lastgciConLinker->setNext(gciConLinkerIt->getNext());
						} else {
							topConcept->setOperandList(gciConLinkerIt->getNext());
						}
						topConcept->incOperandCount(-1);
					} else {
						lastgciConLinker = gciConLinkerIt;
					}

					gciConLinkerIt = gciConLinkerIt->getNext();
				}
			}


			bool CTriggeredImplicationBinaryAbsorberPreProcess::collectConjunctions(CConcept* concept, bool negated, QList< TConceptNegationPair >& conjList) {
				cint64 opCode = concept->getOperatorCode();
				if (!negated && opCode == CCAND || negated && opCode == CCOR) {
					for (CSortedNegLinker<CConcept*>* opLinkIt = concept->getOperandList(); opLinkIt; opLinkIt = opLinkIt->getNext()) {
						CConcept* opConcept = opLinkIt->getData();
						bool opNegation = opLinkIt->isNegated()^negated;
						collectConjunctions(opConcept,opNegation,conjList);
					}
				} else {
					conjList.append(TConceptNegationPair(concept,negated));
				}
				return true;
			}


			void CTriggeredImplicationBinaryAbsorberPreProcess::replaceTrigger(CConcept* baseConcept, CConcept* replaceConcept, bool negated) {
				for (CSortedNegLinker<CConcept*>* opLinker = replaceConcept->getOperandList(); opLinker; opLinker = opLinker->getNext()) {
					CConcept* opConcept = opLinker->getData();
					bool replaceOperand = true;
					bool negatedReplacement = opLinker->isNegated() ^ negated;
					if (opConcept->getConceptOperator()->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_TRIG_TYPE)/* == CCIMPLTRIG*/) {
						if (mTriggCountHash[opConcept].mCount <= 1) {
							replaceOperand = false;
							replaceTrigger(baseConcept,opConcept,negatedReplacement);
						}
					}
					if (replaceOperand) {
						++mStatOptimizedRemovedTriggers;
						addUnfoldingConceptForConcept(baseConcept,opConcept,negatedReplacement);
					}
				}
			}


			void CTriggeredImplicationBinaryAbsorberPreProcess::replaceOneTrigger(CConcept* baseConcept, CConcept* replaceConcept, bool negated) {
				for (CSortedNegLinker<CConcept*>* opLinker = replaceConcept->getOperandList(); opLinker; opLinker = opLinker->getNext()) {
					CConcept* opConcept = opLinker->getData();
					bool replaceOperand = true;
					bool negatedReplacement = opLinker->isNegated() ^ negated;
					if (opConcept->getConceptOperator()->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_TRIG_TYPE)/* == CCIMPLTRIG*/) {
						if (mTriggCountHash[opConcept].mCount <= 1) { 
							CSortedNegLinker<CConcept*>* opOpLinker = opConcept->getOperandList();
							if (opOpLinker && !opOpLinker->getNext() && !opOpLinker->getData()->getConceptOperator()->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_IMPL_TYPE | CConceptOperator::CCF_VARBINDJOIN)/* != CCIMPL*/) {
								replaceOperand = false;
								replaceTrigger(baseConcept,opConcept,negatedReplacement);
							}
						}
					}
					if (replaceOperand) {
						++mStatOptimizedRemovedTriggers;
						addUnfoldingConceptForConcept(baseConcept,opConcept,negatedReplacement);
					}
				}
			}


			void CTriggeredImplicationBinaryAbsorberPreProcess::optimizeTriggerStructure() {

				cint64 optimisingRemovedTrigger = 0;

				cint64 conceptCount = mConceptVec->getItemCount();
				for (cint64 conIdx = 0; conIdx < conceptCount; conIdx++) {
					CConcept* concept = mConceptVec->getData(conIdx);
					if (concept) {
						for (CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList(); opLinker; opLinker = opLinker->getNext()) {
							CConcept* opConcept = opLinker->getData();
							if (opConcept->getConceptOperator()->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_TRIG_TYPE)/* == CCIMPLTRIG*/) {
								mTriggCountHash[opConcept].incCount(1);
							}
						}
					}
				}
				cint64 brCount = mBranchTriggVec->getItemCount();
				for (cint64 brIdx = 0; brIdx < brCount; ++brIdx) {
					CConceptRoleBranchingTrigger* brTrigger = mBranchTriggVec->getData(brIdx);
					if (brTrigger) {
						if (brTrigger->isConceptTrigger()) {
							CConcept* concept = brTrigger->getTriggerConcept();
							if (concept->getConceptOperator()->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_TRIG_TYPE)/* == CCIMPLTRIG*/) {
								mTriggCountHash[concept].incCount(2);
							}
						}
					}
				}

				for (cint64 conIdx = 0; conIdx < conceptCount; conIdx++) {
					CConcept* concept = mConceptVec->getLocalData(conIdx);
					if (concept) {
						cint64 opCode = concept->getOperatorCode();
						if (opCode == CCSUB || opCode == CCAND || concept->getConceptOperator()->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_TRIG_TYPE)/*opCode == CCIMPLTRIG*/) {
							CSortedNegLinker<CConcept*>* lastLinker = nullptr;
							for (CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList(); opLinker; opLinker = opLinker->getNext()) {
								CConcept* opConcept = opLinker->getData();
								bool opNegated = opLinker->isNegated();
								if (opConcept->getConceptOperator()->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_TRIG_TYPE)/* == CCIMPLTRIG*/) {
									if (mTriggCountHash[opConcept].mCount <= 1) {
										++optimisingRemovedTrigger;
										concept->incOperandCount(-1);
										if (lastLinker) {
											lastLinker->setNext(opLinker->getNext());
											replaceTrigger(concept,opConcept,opNegated);
										} else {
											replaceTrigger(concept,opConcept,opNegated);
											CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList();
											while (opLinkerIt && opLinkerIt->getNext() != opLinker) {
												opLinkerIt = opLinkerIt->getNext();
											}
											opLinkerIt->setNext(opLinker->getNext());
											lastLinker = opLinkerIt;
										}
									} else {
										lastLinker = opLinker;
									}
								}
							}
						} else if (concept->getConceptOperator()->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_IMPL_TYPE | CConceptOperator::CCF_VARBINDJOIN | CConceptOperator::CCF_VARBINDGROUND)/*opCode == CCIMPL*/) {
							CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
							if (opLinker) {
								CConcept* opConcept = opLinker->getData();
								bool opNegated = opLinker->isNegated();
								if (opConcept->getConceptOperator()->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_TRIG_TYPE)/* == CCIMPLTRIG*/) {
									if (mTriggCountHash[opConcept].mCount <= 1) {
										CSortedNegLinker<CConcept*>* opOpLinker = opConcept->getOperandList();
										if (opOpLinker && !opOpLinker->getNext() && !opOpLinker->getData()->getConceptOperator()->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_IMPL_TYPE | CConceptOperator::CCF_VARBINDJOIN)/* != CCIMPL*/) {
											++optimisingRemovedTrigger;
											concept->setOperandList(opLinker->getNext());
											concept->incOperandCount(-1);
											replaceOneTrigger(concept,opConcept,opNegated);
										}
									}
								}
							}
						} else {
							if (concept->getOperandCount() == 1) {
								CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
								CConcept* opConcept = opLinker->getData();
								bool opNegated = opLinker->isNegated();
								if (opConcept->getConceptOperator()->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_TRIG_TYPE)/* == CCIMPLTRIG*/) {
									if (mTriggCountHash[opConcept].mCount <= 1) {
										CSortedNegLinker<CConcept*>* opOpLinker = opConcept->getOperandList();
										if (opOpLinker && !opOpLinker->getNext() && !opOpLinker->getData()->getConceptOperator()->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_IMPL_TYPE | CConceptOperator::CCF_VARBINDJOIN)/* != CCIMPL*/) {
											++optimisingRemovedTrigger;
											concept->setOperandList(nullptr);
											concept->incOperandCount(-1);
											replaceOneTrigger(concept,opConcept,opNegated);
										}
									}
								}
							}
						}
					}
				}


			}



			void CTriggeredImplicationBinaryAbsorberPreProcess::collectPositiveConcepts(CConcept* initalConcept, bool initalNegated, QSet<TConceptNegationPair>* positiveConceptSet, bool recursiveDefinition) {
				TConceptNegationPair processConNegPair(initalConcept,initalNegated);
				if (!positiveConceptSet->contains(processConNegPair)) {
					positiveConceptSet->insert(processConNegPair);
					QList<TConceptNegationPair> processConceptList;
					processConceptList.append(processConNegPair);
					while (!processConceptList.isEmpty()) {
						TConceptNegationPair conNegPair(processConceptList.takeFirst());

						CConcept* concept = conNegPair.first;
						bool negated = conNegPair.second;

						cint64 opCode = concept->getOperatorCode();
						CConceptOperator* conOperator = concept->getConceptOperator();
						CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();
						if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AND_AQAND_TYPE) || conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_AQALL_TYPE) || (recursiveDefinition && opCode == CCEQ || !negated && opCode == CCSUB) || opCode == CCAQSOME || opCode == CCSOME) {
							CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
							while (opConLinkerIt) {
								CConcept* opConcept = opConLinkerIt->getData();
								bool opNegation = opConLinkerIt->isNegated()^negated;
								TConceptNegationPair opConNegPair(opConcept,opNegation);
								if (!positiveConceptSet->contains(opConNegPair)) {
									positiveConceptSet->insert(opConNegPair);
									processConceptList.append(opConNegPair);
								}
								opConLinkerIt = opConLinkerIt->getNext();
							}
						} else if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_IMPL_TYPE)) {
							CSortedNegLinker<CConcept*>* firstOpConLinker = opConLinker;
							if (firstOpConLinker) {
								CConcept* opConcept = firstOpConLinker->getData();
								TConceptNegationPair opConNegPair(opConcept,false);
								if (!positiveConceptSet->contains(opConNegPair)) {
									positiveConceptSet->insert(opConNegPair);
									processConceptList.append(opConNegPair);
								}
							}
						} else if (opCode == CCAQCHOOCE) {
							CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
							while (opConLinkerIt) {
								CConcept* opConcept = opConLinkerIt->getData();
								if (opConLinkerIt->isNegated() == negated) {
									TConceptNegationPair opConNegPair(opConcept,false);
									if (!positiveConceptSet->contains(opConNegPair)) {
										positiveConceptSet->insert(opConNegPair);
										processConceptList.append(opConNegPair);
									}
								}
								opConLinkerIt = opConLinkerIt->getNext();
							}
						} else if (opCode == CCATMOST || opCode == CCATLEAST) {
							CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
							while (opConLinkerIt) {
								CConcept* opConcept = opConLinkerIt->getData();
								TConceptNegationPair opConNegPair(opConcept,opConLinkerIt->isNegated());
								if (!positiveConceptSet->contains(opConNegPair)) {
									positiveConceptSet->insert(opConNegPair);
									processConceptList.append(opConNegPair);
								}
								opConLinkerIt = opConLinkerIt->getNext();
							}
						} else if (!negated && (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_TRIG_TYPE))) {
							CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
							while (opConLinkerIt) {
								CConcept* opConcept = opConLinkerIt->getData();
								TConceptNegationPair opConNegPair(opConcept,opConLinkerIt->isNegated());
								if (!positiveConceptSet->contains(opConNegPair)) {
									positiveConceptSet->insert(opConNegPair);
									processConceptList.append(opConNegPair);
								}
								opConLinkerIt = opConLinkerIt->getNext();
							}
						}
					}
				}
			}


			QString CTriggeredImplicationBinaryAbsorberPreProcess::generateDebugGCIConceptString(CConcept* concept, bool conceptNegation) {
				mDebugGCIString = CConceptTextFormater::getConceptString(concept,conceptNegation);
				return mDebugGCIString;
			}


			QString CTriggeredImplicationBinaryAbsorberPreProcess::generateDebugGeneratedAbsorbedImplicationString(cint64 minNewConceptTag, cint64 lastNewConceptTag) {
				QString absorbedString;
				for (cint64 tag = minNewConceptTag; tag <= lastNewConceptTag; ++tag) {
					CConcept* concept = mConceptVec->getData(tag);
					if (concept) {
						QString tmpString = CConceptTextFormater::getConceptString(concept,false);
						absorbedString += QString("%1<br>\r\n").arg(tmpString);
					}
				}
				mDebugAbsorbedString = absorbedString;
				return absorbedString;
			}


			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createImpliedConcept() {
				CConcept* concept = createNewConcept();
				concept->setOperatorCode(CCAND);
				return concept;
			}


			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createCandidateEquivalentConcept(CConcept* eqConcept) {
				CConcept* concept = createNewConcept();
				addUnfoldingConceptForConcept(concept,eqConcept,false);
				concept->setOperatorCode(CCEQCAND);
				return concept;
			}


			bool CTriggeredImplicationBinaryAbsorberPreProcess::addConceptToImplied(CConcept* impliedConcept, CConcept* concept, bool negated) {
				CSortedNegLinker<CConcept*>* newBaseOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon->init(concept,negated,impliedConcept->getOperandList());
				impliedConcept->setOperandList(newBaseOpCon);
				impliedConcept->incOperandCount();
				return true;
			}

			bool CTriggeredImplicationBinaryAbsorberPreProcess::addSubClassConcept(CConcept* subclassConcept, CConcept* addedConcept, bool negated) {
				CSortedNegLinker<CConcept*>* newBaseOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon->init(addedConcept,negated,subclassConcept->getOperandList());
				subclassConcept->setOperandList(newBaseOpCon);
				subclassConcept->incOperandCount();
				subclassConcept->setOperatorCode(CCSUB);
				return true;
			}


			bool CTriggeredImplicationBinaryAbsorberPreProcess::addRoleDomainConcept(CRole* role, CConcept* addedConcept, bool negated) {
				CSortedNegLinker<CConcept*>* newBaseOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon->init(addedConcept,negated,role->getDomainConceptList());
				role->setDomainConceptList(newBaseOpCon);
				return true;
			}



			bool CTriggeredImplicationBinaryAbsorberPreProcess::addRoleRangeConcept(CRole* role, CConcept* addedConcept, bool negated) {
				CSortedNegLinker<CConcept*>* newBaseOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon->init(addedConcept,negated,role->getRangeConceptList());
				role->setRangeConceptList(newBaseOpCon);
				return true;
			}

			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createNewConcept() {
				CConcept* concept = CObjectAllocator< CConcept >::allocateAndConstruct(mMemMan);
				concept->initConcept();
				cint64 nextConTag = mTBox->getNextConceptID();
				concept->initTag(nextConTag);
				mConceptVec->setLocalData(nextConTag, concept);
				return concept;
			}


			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createImplicationConcept(CConcept* impliedConcept, bool negated) {
				CConcept* concept = createNewConcept();
				concept->setOperatorCode(CCIMPL);

				CSortedNegLinker<CConcept*>* newBaseOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon->init(impliedConcept,negated);
				concept->setOperandList(newBaseOpCon);
				concept->incOperandCount();

				++mStatImplConceptsGenerated;
				return concept;
			}



			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createImplicationTriggerConcept(CConcept* impConcept, bool negated) {
				CConcept* concept = createNewConcept();
				concept->setOperatorCode(CCIMPLTRIG);
				if (impConcept) {
					addConceptToImplied(concept,impConcept,negated);
				}
				return concept;
			}

			bool CTriggeredImplicationBinaryAbsorberPreProcess::addImplicationTrigger(CConcept* implConcept, CConcept* triggerConcept, bool negated) {
				CSortedNegLinker<CConcept*>* newTriggerOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newTriggerOpCon->init(triggerConcept,negated,implConcept->getOperandList()->getNext());
				implConcept->getOperandList()->setNext(newTriggerOpCon);
				implConcept->incOperandCount();
				mTriggerImplHash->insertMulti(triggerConcept,implConcept);
				return true;
			}


			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createTriggerPropagationConcept(CConcept* destConcept, CRole* backwardPropRole, bool branchTiggerCreation, bool invsereRolePropagation) {
				CConcept* concept = createNewConcept();
				CRole* propRole = backwardPropRole;
				if (invsereRolePropagation) {
					propRole = getInverseRole(backwardPropRole);
				}
				if (branchTiggerCreation) {
					concept->setOperatorCode(CCBRANCHALL);
				} else {
					concept->setOperatorCode(CCIMPLALL);
				}
				concept->setRole(propRole);
				CSortedNegLinker<CConcept*>* newTriggerOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newTriggerOpCon->init(destConcept,false);
				concept->setOperandList(newTriggerOpCon);
				concept->incOperandCount();
				return concept;
			}



			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createNominalImplication(CConcept* destConcept, CIndividual* nominalIndi) {
				CConcept* concept = createNewConcept();
				concept->setOperatorCode(CCNOMINALIMPLI);
				concept->setNominalIndividual(nominalIndi);
				CSortedNegLinker<CConcept*>* newTriggerOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newTriggerOpCon->init(destConcept, false);
				concept->setOperandList(newTriggerOpCon);
				concept->incOperandCount();
				return concept;
			}





			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createDatatypeImplication(CConcept* destConcept, CDatatype* datatype) {
				CConcept* concept = createNewConcept();
				concept->setOperatorCode(CCDATATYPEIMPLI);
				concept->setDatatype(datatype);
				CSortedNegLinker<CConcept*>* newTriggerOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newTriggerOpCon->init(destConcept, false);
				concept->setOperandList(newTriggerOpCon);
				concept->incOperandCount();
				return concept;
			}


			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createDataLiteralImplication(CConcept* destConcept, CDataLiteral* dataLiteral) {
				CConcept* concept = createNewConcept();
				concept->setOperatorCode(CCDATALITERALIMPLI);
				concept->setDataLiteral(dataLiteral);
				CSortedNegLinker<CConcept*>* newTriggerOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newTriggerOpCon->init(destConcept, false);
				concept->setOperandList(newTriggerOpCon);
				concept->incOperandCount();
				return concept;
			}



			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createDataRestrictionImplication(CConcept* destConcept, CDatatype* datatype, CDataLiteral* dataliteral, cint64 restrictionCode) {
				CConcept* concept = createNewConcept();
				concept->setOperatorCode(CCDATARESTRICTIONIMPLI);
				concept->setDatatype(datatype);
				concept->setDataLiteral(dataliteral);
				concept->setParameter(restrictionCode);
				CSortedNegLinker<CConcept*>* newTriggerOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newTriggerOpCon->init(destConcept, false);
				concept->setOperandList(newTriggerOpCon);
				concept->incOperandCount();
				return concept;
			}


			CRole* CTriggeredImplicationBinaryAbsorberPreProcess::getInverseRole(CRole* role) {
				CSortedNegLinker<CRole*>* eqInvRoleIt = role->getInverseEquivalentRoleList();
				while (eqInvRoleIt) {
					if (eqInvRoleIt->isNegated()) {
						return eqInvRoleIt->getData();
					}
					eqInvRoleIt = eqInvRoleIt->getNext();
				}
				CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList();
				while (superRoleIt) {
					if (superRoleIt->isNegated()) {
						CRole* superRole = superRoleIt->getData();
						CSortedNegLinker<CRole*>* superRoleSuperRoleIt = superRole->getIndirectSuperRoleList();
						while (superRoleSuperRoleIt) {
							if (superRoleSuperRoleIt->isNegated() && superRoleSuperRoleIt->getData() == role) {
								return superRole;
							}
							superRoleSuperRoleIt = superRoleSuperRoleIt->getNext();
						}
					}
					superRoleIt = superRoleIt->getNext();
				}

				// create inverse role
				CRole* invRole = CConcreteOntologyInverseRoleBuilder::createInverseRoleDelayed(role,&mRoleInverseRoleCreateHash,mOnto);
				++mStatInvRolesGenerated;
				return invRole;
			}



			void CTriggeredImplicationBinaryAbsorberPreProcess::createEquivalenceAbsorbedTriggeredImplication(CConcept* eqConcept, bool negated, CSortedNegLinker<CConcept*>* absorpOpLinker) {
				CConceptTriggerLinker* triggers = nullptr;
				for (CSortedNegLinker<CConcept*>* opConLinkerIt = absorpOpLinker; opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
					CConcept* concept = opConLinkerIt->getData();
					bool negated = opConLinkerIt->isNegated();
					CConceptTriggerLinker* tmpTriggers = copyTriggerLinkers(getTriggersForConcept(concept,negated));
					if (tmpTriggers) {
						triggers = tmpTriggers->append(triggers);
					}
				}
				CConceptTriggerLinker* impliedConceptTrigger = getImplicationTriggeredConceptForTriggers(triggers,nullptr);
				addUnfoldingConceptForConcept(impliedConceptTrigger->getTriggerConcept(),eqConcept,false);

			}


			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::getEquivalenceConceptCandidateTriggeredImplication(CConcept* eqConcept, bool negated) {
				CConcept* triggeredConcept = nullptr;

				cint64 opCode = eqConcept->getOperatorCode();
				if (!negated && opCode == CCEQ) {
					QList<TConceptNegationPair> candList;
					if (addAbsorbableDisjunctCandidates(eqConcept,!negated,candList)) {
						if (candList.count() >= 1) {
							QList<TConceptNegationPair> absorbList(getPartialAbsorbableDisjuncts(candList));
							if (!absorbList.isEmpty()) {	
								triggeredConcept = createPartialExtendedAbsorbedImpliedTriggerCocnept(absorbList);
							}
						}
					}
				}
				return triggeredConcept;
			}



			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createDisjunctionAbsorbedTriggeredImplication(CConcept* orConcept, bool negated) {
				CConcept* implicationConcept = nullptr;

				cint64 opCode = orConcept->getOperatorCode();
				if (!negated && opCode == CCOR || negated && opCode == CCAND) {
					QList<TConceptNegationPair> candList;
					if (addAbsorbableDisjunctCandidates(orConcept,negated,candList)) {
						if (candList.count() >= 1) {
							QList<TConceptNegationPair> absorbList(splitAbsorbableDisjuncts(candList));
							if (!absorbList.isEmpty()) {
								implicationConcept = createDisjunctionAbsorbedTriggeredImplication(orConcept,negated,absorbList,candList);
							}
						}
					}
				}
				return implicationConcept;
			}




			CConceptRoleBranchingTrigger* CTriggeredImplicationBinaryAbsorberPreProcess::createCardinalityQualificationPartialAbsorbedTriggers(CConcept* cardConcept, bool negated) {
				CConceptRoleBranchingTrigger* branchTriggers = nullptr;
				QSet<CConcept*> conceptEqConAbsorbed;
				for (CSortedNegLinker<CConcept*>* opConLinker = cardConcept->getOperandList(); opConLinker; opConLinker = opConLinker->getNext()) {
					CConcept* concept = opConLinker->getData();
					bool negated = opConLinker->isNegated();
					if (isConceptPartialImplicationTriggerable(concept,!negated,&conceptEqConAbsorbed)) {
						CConceptRoleBranchingTrigger* tmpBranchTrigger = getSimpleBranchTriggersForConcept(concept,negated);
						if (tmpBranchTrigger) {
							branchTriggers = tmpBranchTrigger->append(branchTriggers);
						} else {
							CConceptRoleBranchingTrigger* tmpBranchTrigger = getBranchTiggers(getPartialTriggersForConcept(concept,negated));
							branchTriggers = tmpBranchTrigger->append(branchTriggers);
						}
					}
				}
				return branchTriggers;
			}



			CConceptRoleBranchingTrigger* CTriggeredImplicationBinaryAbsorberPreProcess::createDisjunctionPartialAbsorbedBranchTriggers(CConcept* orConcept, bool negated) {
				CConceptRoleBranchingTrigger* branchTriggers = nullptr;
				cint64 opCode = orConcept->getOperatorCode();
				if (!negated && opCode == CCOR || negated && (opCode == CCAND)) {
					QList<TConceptNegationPair> candList;
					if (addAbsorbableDisjunctCandidates(orConcept,negated,candList)) {
						if (candList.count() >= 1) {
							QList<TConceptNegationPair> absorbList(getPartialAbsorbableDisjuncts(candList));
							if (!absorbList.isEmpty()) {								
								branchTriggers = createDisjunctionPartialAbsorbedBranchTriggers(orConcept,negated,absorbList);
							}
						}
					}
				}
				return branchTriggers;
			}



			CConceptRoleBranchingTrigger* CTriggeredImplicationBinaryAbsorberPreProcess::createDisjunctionPartialAbsorbedBranchTriggers(CConcept* orConcept, bool negated, const QList<TConceptNegationPair>& absorbList) {
				CConceptRoleBranchingTrigger* branchTriggers = nullptr;
				for (QList<TConceptNegationPair>::const_iterator it = absorbList.constBegin(), itEnd = absorbList.constEnd(); it != itEnd; ++it) {
					TConceptNegationPair conNegPair(*it);
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					CConceptRoleBranchingTrigger* tmpBranchTrigger = getSimpleBranchTriggersForConcept(concept,!negated);
					if (tmpBranchTrigger) {
						branchTriggers = tmpBranchTrigger->append(branchTriggers);
					} else {
						CConceptRoleBranchingTrigger* tmpBranchTrigger = getBranchTiggers(getPartialTriggersForConcept(concept,!negated));
						branchTriggers = tmpBranchTrigger->append(branchTriggers);
					}
				}
				return branchTriggers;
			}



			bool CTriggeredImplicationBinaryAbsorberPreProcess::createConceptOfInterestAbsorption(CConcept* intConcept, bool negated, bool continuationExtension) {
				bool absorbed = false;
				bool propagationActivationAbsorbable = isConceptPropagationTriggerable(intConcept, negated);

				if (propagationActivationAbsorbable) {
					return createPropagationConceptOfInterestAbsorption(intConcept, negated, continuationExtension);
				} else {
					return createDirectConceptOfInterestAbsorption(intConcept, negated, continuationExtension);
				}

			}


			bool CTriggeredImplicationBinaryAbsorberPreProcess::createPropagationConceptOfInterestAbsorption(CConcept* intConcept, bool negated, bool continuationExtension) {

				TConceptNegPair intConceptNegationPair = TConceptNegPair(intConcept, negated);
				CConcept* candidateBackPropTriggerCon = nullptr;
				QHash<CConcept*, TOccuredAbsorbablePair> conceptEqConAbsorbed;
				bool completlyAbsorbable = isConceptImplicationTriggerable(intConcept, negated, &conceptEqConAbsorbed);
				bool partiallyAbsorbable = completlyAbsorbable;
				if (!partiallyAbsorbable) {
					QSet<CConcept*> conceptEqConPartialAbsorbed;
					partiallyAbsorbable = isConceptPartialImplicationTriggerable(intConcept, negated, &conceptEqConPartialAbsorbed);
				}

				CConceptOfInterestActivationTriggeringData& activationTriggeringData = mTBox->getConceptOfInterestActivationTriggerDataHash()->operator[](TConceptNegPair(intConcept, negated));

				CConcept* markerConcept = createMarkerConcept();

				CConceptTriggerLinker* triggers = getPropagationTriggersForConcept(intConcept, negated, intConceptNegationPair, completlyAbsorbable);
				if (triggers->hasNext()) {
					triggers = getImplicationTriggeredConceptForTriggers(triggers, nullptr);
				}
				CConcept* triggerConcept = triggers->getTriggerConcept();
				addUnfoldingConceptForConcept(triggerConcept, markerConcept, false);

				if (completlyAbsorbable) {
					mTBox->getTriggerConceptOfInterestHash()->insert(markerConcept, intConceptNegationPair);
					activationTriggeringData.setTriggeringConcept(markerConcept);
				} else {
					mTBox->getCandidateConceptOfInterestHash()->insert(markerConcept, intConceptNegationPair);
					activationTriggeringData.setCandidateConcept(markerConcept);
				}

				return true;

			}



			CConceptTriggerLinker* CTriggeredImplicationBinaryAbsorberPreProcess::getPropagationTriggersForConceptOperandList(CSortedNegLinker<CConcept*>* opConLinker, bool operandsNegation, bool chooseInsteadOfNegation, CConcept* concept, bool negated, TConceptNegPair& intConceptNegationPair, bool completelyAbsorbable) {

				CConceptTriggerLinker* triggers = nullptr;

				QList<TConceptNegPair> propagationAbsorptionList;
				QList<TConceptNegPair> backpropagationAbsorptionList;
				for (CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker; opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
					CConcept* opConcept = opConLinkerIt->getData();
					bool opNegated = opConLinkerIt->isNegated();
					if (!chooseInsteadOfNegation && operandsNegation) {
						opNegated = !opNegated;
					}
					TConceptNegPair conceptNegationPair(opConcept, opNegated);

					if (!chooseInsteadOfNegation || operandsNegation == opNegated) {
						if (isConceptPropagationTriggerable(opConcept, opNegated)) {
							propagationAbsorptionList.append(conceptNegationPair);
						} else {
							backpropagationAbsorptionList.append(conceptNegationPair);
						}
					}
				}

				CConceptTriggerLinker* propagationTriggers = nullptr;
				for (TConceptNegationPair conceptNegationPair : propagationAbsorptionList) {
					CConcept* opConcept = conceptNegationPair.first;
					bool opNegated = conceptNegationPair.second;

					CConceptTriggerLinker* newTriggers = copyTriggerLinkers(getPropagationTriggersForConcept(opConcept, opNegated, intConceptNegationPair, completelyAbsorbable));
					propagationTriggers = newTriggers->append(propagationTriggers);
				}

				CConceptTriggerLinker* implPropagationTriggeredConceptLinker = nullptr;
				if (propagationTriggers->hasNext()) {
					implPropagationTriggeredConceptLinker = getImplicationTriggeredConceptForTriggers(propagationTriggers, nullptr);
				} else {
					implPropagationTriggeredConceptLinker = propagationTriggers;
				}
				CConcept* backPropConcept = implPropagationTriggeredConceptLinker->getTriggerConcept();

				CConceptTriggerLinker* backPropTriggers = implPropagationTriggeredConceptLinker;
				for (TConceptNegationPair conceptNegationPair : backpropagationAbsorptionList) {
					CConcept* opConcept = conceptNegationPair.first;
					bool opNegated = conceptNegationPair.second;

					CConceptTriggerLinker* newTriggers = copyTriggerLinkers(getPartialTriggersForConcept(opConcept, opNegated, implPropagationTriggeredConceptLinker->getTriggerConcept()));
					backPropTriggers = newTriggers->append(backPropTriggers);
				}

				if (backPropTriggers) {
					if (!backPropTriggers->hasNext()) {
						triggers = backPropTriggers;
					} else {
						triggers = getImplicationTriggeredConceptForTriggers(backPropTriggers, nullptr);
					}
				} else {
					triggers = implPropagationTriggeredConceptLinker;
				}

				return triggers;
			}




			CConceptTriggerLinker* CTriggeredImplicationBinaryAbsorberPreProcess::getPropagationTriggersForConcept(CConcept* concept, bool negated, TConceptNegPair& intConceptNegationPair, bool completelyAbsorbable) {

				CConceptTriggerLinker* triggers = nullptr;


				TConceptNegationPair conNegPair(concept, negated);


				cint64 opCode = concept->getOperatorCode();
				cint64 parm = concept->getParameter();
				cint64 opCount = concept->getOperandCount();
				CRole* role = concept->getRole();
				CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();

				if (!negated && (opCode == CCNOMINAL)) {

					CIndividual* nominalIndi = concept->getNominalIndividual();
					CConcept* individualActivationTriggerCon = createTriggerConcept(true);

					CConceptOfInterestActivationTriggeringData& activationTriggeringData = mTBox->getConceptOfInterestActivationTriggerDataHash()->operator[](intConceptNegationPair);
					CConceptOfInterestActivationLinker* activationLinker = CObjectAllocator<CConceptOfInterestActivationLinker>::allocateAndConstruct(mMemMan);
					activationLinker->initConceptOfInterestActivationData(individualActivationTriggerCon, nominalIndi);
					activationTriggeringData.addActivationLinker(activationLinker);

					CConceptTriggerLinker* newTrigger = createTriggerLinker();
					newTrigger->initConceptTriggerLinker(individualActivationTriggerCon, 1);
					triggers = newTrigger->append(triggers);

				} else if (!negated && (opCode == CCVALUE)) {

					CIndividual* nominalIndi = concept->getNominalIndividual();
					CRole* role = concept->getRole();

					CConcept* propagatedTriggerConcept = createTriggerConcept(true);
					CConcept* individualActivationTriggerCon = createTriggerPropagationConcept(propagatedTriggerConcept, role, true);

					CConceptOfInterestActivationTriggeringData& activationTriggeringData = mTBox->getConceptOfInterestActivationTriggerDataHash()->operator[](intConceptNegationPair);
					CConceptOfInterestActivationLinker* activationLinker = CObjectAllocator<CConceptOfInterestActivationLinker>::allocateAndConstruct(mMemMan);
					activationLinker->initConceptOfInterestActivationData(individualActivationTriggerCon, nominalIndi);
					activationTriggeringData.addActivationLinker(activationLinker);

					CConceptTriggerLinker* newTrigger = createTriggerLinker();
					newTrigger->initConceptTriggerLinker(propagatedTriggerConcept, 2);
					triggers = newTrigger->append(triggers);

				} else if (negated && (opCode == CCOR) || !negated && (opCode == CCAND)) {
					// AND, collect triggers from operands
					CConceptTriggerLinker* andTriggers = getPropagationTriggersForConceptOperandList(opConLinker, negated, false, concept, negated, intConceptNegationPair, completelyAbsorbable);
					triggers = andTriggers->append(triggers);

				} else if (opCode == CCAQCHOOCE) {

					CConceptTriggerLinker* andTriggers = getPropagationTriggersForConceptOperandList(opConLinker, negated, true, concept, negated, intConceptNegationPair, completelyAbsorbable);
					triggers = andTriggers->append(triggers);

				} else if (!negated && (opCode == CCSOME || opCode == CCAQSOME) || negated && (opCode == CCALL) ||
						!negated && (opCode == CCATLEAST && parm == 1) || negated && (opCode == CCATMOST && parm == 0)) {

					cint64 triggerComplexity = 1;
					bool negateOperands = negated && (opCode == CCSOME || opCode == CCALL);

					CConceptTriggerLinker* succTriggers = getPropagationTriggersForConceptOperandList(opConLinker, negateOperands, false, concept, negated, intConceptNegationPair, completelyAbsorbable);

					CConcept* rolePropagatedCon = createTriggerConcept(true);
					CConcept* rolePropagationCon = createTriggerPropagationConcept(rolePropagatedCon, role, true);
					addUnfoldingConceptForConcept(succTriggers->getTriggerConcept(), rolePropagationCon, false);
					triggerComplexity += succTriggers->getTriggerComplexity();

					CConceptTriggerLinker* newTrigger = createTriggerLinker();
					newTrigger->initConceptTriggerLinker(rolePropagatedCon, triggerComplexity);

					triggers = newTrigger->append(triggers);
				}

				return triggers;
			}




			bool CTriggeredImplicationBinaryAbsorberPreProcess::createDirectConceptOfInterestAbsorption(CConcept* intConcept, bool negated, bool continuationExtension) {
				bool absorbed = false;
				CConcept* candidateBackPropTriggerCon = nullptr;
				QHash<CConcept*, TOccuredAbsorbablePair> conceptEqConAbsorbed;
				QSet<CConcept*> conceptEqConPartialAbsorbed;
				bool completlyAbsorbable = isConceptImplicationTriggerable(intConcept, negated, &conceptEqConAbsorbed);
				bool partiallyAbsorbable = completlyAbsorbable;
				if (!partiallyAbsorbable) {
					partiallyAbsorbable = isConceptPartialImplicationTriggerable(intConcept, negated, &conceptEqConPartialAbsorbed);
				}
				CConcept* markerConcept = nullptr;
				if (partiallyAbsorbable) {
					markerConcept = createMarkerConcept();
				}

				CConceptOfInterestActivationTriggeringData& activationTriggeringData = mTBox->getConceptOfInterestActivationTriggerDataHash()->operator[](TConceptNegPair(intConcept, negated));
				if (continuationExtension && partiallyAbsorbable) {
					candidateBackPropTriggerCon = createTriggerConcept(true);
					CConceptOfInterestActivationLinker* activationLinker = CObjectAllocator<CConceptOfInterestActivationLinker>::allocateAndConstruct(mMemMan);
					activationLinker->initConceptOfInterestActivationData(candidateBackPropTriggerCon);
					activationTriggeringData.addActivationLinker(activationLinker);
					absorbed = true;
				}
				if (completlyAbsorbable) {
					CConcept* triggerConcept = getConceptOfInterestTrigger(intConcept, negated, candidateBackPropTriggerCon);
					addUnfoldingConceptForConcept(triggerConcept, markerConcept, false);
					mTBox->getTriggerConceptOfInterestHash()->insert(markerConcept, TConceptNegPair(intConcept, negated));
					activationTriggeringData.setTriggeringConcept(markerConcept);
					absorbed = true;
				} else if (partiallyAbsorbable) {
					CConcept* candidateConcept = getConceptOfInterestCandidate(intConcept, negated, candidateBackPropTriggerCon);
					addUnfoldingConceptForConcept(candidateConcept, markerConcept, false);
					activationTriggeringData.setCandidateConcept(markerConcept);
					mTBox->getCandidateConceptOfInterestHash()->insert(markerConcept, TConceptNegPair(intConcept, negated));
					absorbed = true;
				}

				return absorbed;
			}





			bool CTriggeredImplicationBinaryAbsorberPreProcess::isConceptPropagationTriggerable(CConcept* concept, bool negated) {
				TConceptNegationPair conNegPair(concept, negated);
				if (mConceptPropagationAbsorbableHash.contains(conNegPair)) {
					return mConceptPropagationAbsorbableHash.value(conNegPair);
				}
				cint64 opCode = concept->getOperatorCode();
				cint64 parm = concept->getParameter();
				cint64 opCount = concept->getOperandCount();
				CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();

				bool absorbable = false;

				if (!negated && (opCode == CCNOMINAL)) {
					absorbable = true;
				} else if (!negated && (opCode == CCVALUE)) {
					absorbable = true;
				} else if (!negated && (opCode == CCSOME || opCode == CCAQSOME) || negated && (opCode == CCALL)) {
					if (!opConLinker) {
						absorbable = false;
					} else {
						bool oneOpTriggerable = false;
						CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
						while (opConLinkerIt && !oneOpTriggerable) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegated = opConLinkerIt->isNegated() ^ negated;
							oneOpTriggerable  |= isConceptPropagationTriggerable(opConcept, opNegated);
							opConLinkerIt = opConLinkerIt->getNext();
						}
						absorbable = oneOpTriggerable;
					}
				} else  if (!negated && (opCode == CCATLEAST && parm == 1) || negated && (opCode == CCATMOST && parm == 0)) {
					if (!opConLinker) {
						absorbable = false;
					} else {
						bool oneOpTriggerable = true;
						CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
						while (opConLinkerIt && !oneOpTriggerable) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegated = opConLinkerIt->isNegated();
							oneOpTriggerable |= isConceptPropagationTriggerable(opConcept, opNegated);
							opConLinkerIt = opConLinkerIt->getNext();
						}
						absorbable = oneOpTriggerable;
					}
				} else if (opCode == CCAQCHOOCE) {
					bool oneOpTriggerable = false;
					CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
					while (opConLinkerIt && !oneOpTriggerable) {
						CConcept* opConcept = opConLinkerIt->getData();
						bool opNegated = opConLinkerIt->isNegated();
						if (opNegated == negated) {
							oneOpTriggerable |= isConceptPropagationTriggerable(opConcept, opNegated);
						}
						opConLinkerIt = opConLinkerIt->getNext();
					}
					absorbable = oneOpTriggerable;
				} else if (!negated && opCode == CCAND || negated && opCode == CCOR) {
					bool oneOpTriggerable = false;
					CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
					while (opConLinkerIt && !oneOpTriggerable) {
						CConcept* opConcept = opConLinkerIt->getData();
						bool opNegated = opConLinkerIt->isNegated() ^ negated;
						oneOpTriggerable |= isConceptPropagationTriggerable(opConcept, opNegated);
						opConLinkerIt = opConLinkerIt->getNext();
					}
					absorbable = oneOpTriggerable;
				} else {
					absorbable = false;
				}
				mConceptPropagationAbsorbableHash.insert(conNegPair, absorbable);
				return absorbable;
			}














			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createDisjunctionAbsorbedBackpropagationTriggeredImplication(CConcept* orConcept, bool negated, CConcept* backPropTriggerCon) {
				CConcept* implicationConcept = nullptr;

				cint64 opCode = orConcept->getOperatorCode();
				if (!negated && opCode == CCOR || negated && opCode == CCAND) {
					QList<TConceptNegationPair> candList;
					if (addAbsorbableDisjunctCandidates(orConcept, negated, candList)) {
						if (candList.count() >= 1) {
							QList<TConceptNegationPair> absorbList(splitAbsorbableDisjuncts(candList));
							QList<TConceptNegationPair> partialAbsorbList(getPartialAbsorbableDisjuncts(candList));
							if (!absorbList.isEmpty() || !partialAbsorbList.isEmpty()) {
								implicationConcept = createDisjunctionAbsorbedBackpropagationTriggeredImplication(orConcept, negated, absorbList, partialAbsorbList, candList, backPropTriggerCon);
							}
						}
					}
				}
				return implicationConcept;
			}




			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createDisjunctionAbsorbedBackpropagationTriggeredImplication(CConcept* orConcept, bool negated, QList<TConceptNegationPair>& absorbList, QList<TConceptNegationPair>& partialAbsorbList, QList<TConceptNegationPair>& candList, CConcept* backPropTriggerCon) {
				CConcept* impliedConcept = nullptr;
				CConcept* implicationConcept = nullptr;
				bool impliedNegation = false;

				if (candList.isEmpty()) {
					candList.append(absorbList.takeFirst());
				}

				CConceptTriggerLinker* triggers = nullptr;
				for (QList<TConceptNegationPair>::const_iterator it = absorbList.constBegin(), itEnd = absorbList.constEnd(); it != itEnd; ++it) {
					TConceptNegationPair conNegPair(*it);
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					CConceptTriggerLinker* tmpTriggers = copyTriggerLinkers(getTriggersForConcept(concept, !negated, backPropTriggerCon));
					triggers = tmpTriggers->append(triggers);
				}
				for (QList<TConceptNegationPair>::const_iterator it = partialAbsorbList.constBegin(), itEnd = partialAbsorbList.constEnd(); it != itEnd; ++it) {
					TConceptNegationPair conNegPair(*it);
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					CConceptTriggerLinker* tmpTriggers = copyTriggerLinkers(getPartialTriggersForConcept(concept, !negated, backPropTriggerCon));
					triggers = tmpTriggers->append(triggers);
				}

				CConceptTriggerLinker* impliedConceptTrigger = getImplicationTriggeredConceptForTriggers(triggers, &implicationConcept);
				impliedConcept = impliedConceptTrigger->getTriggerConcept();
				if (candList.count() > 1) {
					CConcept* orImpliedConcept = createImpliedConcept();
					for (QList<TConceptNegationPair>::const_iterator it = candList.constBegin(), itEnd = candList.constEnd(); it != itEnd; ++it) {
						TConceptNegationPair conNegPair(*it);
						CConcept* concept = conNegPair.first;
						bool negated = conNegPair.second;
						addConceptToImplied(orImpliedConcept, concept, negated);
					}
					orImpliedConcept->setOperatorCode(CCOR);
					mCreatedImpliedDisjunctions.insert(orImpliedConcept);
					addUnfoldingConceptForConcept(impliedConcept, orImpliedConcept, false);

				} else {
					TConceptNegationPair conNegPair(candList.first());
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					addUnfoldingConceptForConcept(impliedConcept, concept, negated);
				}

				return implicationConcept;
			}




			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createPartialBackpropagationExtendedAbsorbedImpliedTriggerCocnept(const QList<TConceptNegationPair>& absorbList, CConcept* backPropTriggerCon) {
				CConceptTriggerLinker* trigger = createPartialBackpropagationExtendedAbsorbedImpliedTrigger(absorbList, backPropTriggerCon);
				if (trigger) {
					return trigger->getTriggerConcept();
				}
				return nullptr;
			}


			CConceptTriggerLinker* CTriggeredImplicationBinaryAbsorberPreProcess::createPartialBackpropagationExtendedAbsorbedImpliedTrigger(const QList<TConceptNegationPair>& absorbList, CConcept* backPropTriggerCon) {
				CConceptTriggerLinker* triggers = nullptr;
				for (QList<TConceptNegationPair>::const_iterator it = absorbList.constBegin(), itEnd = absorbList.constEnd(); it != itEnd; ++it) {
					TConceptNegationPair conNegPair(*it);
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					CConceptTriggerLinker* tmpTriggers = copyTriggerLinkers(getPartialTriggersForConcept(concept, !negated, backPropTriggerCon));
					triggers = tmpTriggers->append(triggers);
				}
				CConcept* implicationConcept = nullptr;
				CConcept** implicationConceptPointer = nullptr;
				if (backPropTriggerCon) {
					implicationConceptPointer = &implicationConcept;
				}
				CConceptTriggerLinker* impliedConceptTrigger = getImplicationTriggeredConceptForTriggers(triggers, implicationConceptPointer);
				if (implicationConcept) {
					addUnfoldingConceptForConcept(backPropTriggerCon, implicationConcept, false);
				}
				return impliedConceptTrigger;
			}


			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::getConceptOfInterestCandidate(CConcept* intConcept, bool negated, CConcept* backPropTriggerCon) {
				CConcept* triggeredConcept = nullptr;

				QList<TConceptNegationPair> candList;
				if (addAbsorbableDisjunctCandidates(intConcept, !negated, candList)) {
					if (candList.count() >= 1) {
						QList<TConceptNegationPair> absorbList(getPartialAbsorbableDisjuncts(candList));
						if (!absorbList.isEmpty()) {
							triggeredConcept = createBackpropPartialExtendedAbsorbedImpliedTrigger(absorbList, backPropTriggerCon)->getTriggerConcept();
						}
					}
				}
				return triggeredConcept;
			}




			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::getConceptOfInterestTrigger(CConcept* intConcept, bool negated, CConcept* backPropTriggerCon) {
				CConcept* triggeredConcept = nullptr;

				QList<TConceptNegationPair> candList;
				if (addAbsorbableDisjunctCandidates(intConcept, !negated, candList)) {
					if (candList.count() >= 1) {
						QList<TConceptNegationPair> absorbList(splitAbsorbableDisjuncts(candList));
						if (!absorbList.isEmpty()) {
							triggeredConcept = createBackpropExtendedAbsorbedImpliedTrigger(absorbList, backPropTriggerCon)->getTriggerConcept();
						}
					}
				}
				return triggeredConcept;
			}



			CConceptTriggerLinker* CTriggeredImplicationBinaryAbsorberPreProcess::createBackpropPartialExtendedAbsorbedImpliedTrigger(const QList<TConceptNegationPair>& absorbList, CConcept* backPropTriggerCon) {
				CConceptTriggerLinker* triggers = nullptr;
				for (QList<TConceptNegationPair>::const_iterator it = absorbList.constBegin(), itEnd = absorbList.constEnd(); it != itEnd; ++it) {
					TConceptNegationPair conNegPair(*it);
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					CConceptTriggerLinker* tmpTriggers = copyTriggerLinkers(getPartialTriggersForConcept(concept, !negated, backPropTriggerCon));
					triggers = tmpTriggers->append(triggers);
				}
				CConcept* backPropImpConcept = nullptr;
				CConceptTriggerLinker* impliedConceptTrigger = getImplicationTriggeredConceptForTriggers(triggers, &backPropImpConcept);
				addUnfoldingConceptForConcept(backPropTriggerCon, backPropImpConcept, false);
				return impliedConceptTrigger;
			}

			



			CConceptTriggerLinker* CTriggeredImplicationBinaryAbsorberPreProcess::createBackpropExtendedAbsorbedImpliedTrigger(const QList<TConceptNegationPair>& absorbList, CConcept* backPropTriggerCon) {
				CConceptTriggerLinker* triggers = nullptr;
				for (QList<TConceptNegationPair>::const_iterator it = absorbList.constBegin(), itEnd = absorbList.constEnd(); it != itEnd; ++it) {
					TConceptNegationPair conNegPair(*it);
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					CConceptTriggerLinker* tmpTriggers = copyTriggerLinkers(getTriggersForConcept(concept, !negated, backPropTriggerCon));
					triggers = tmpTriggers->append(triggers);
				}
				CConcept* backPropImpConcept = nullptr;
				CConceptTriggerLinker* impliedConceptTrigger = getImplicationTriggeredConceptForTriggers(triggers, &backPropImpConcept);
				addUnfoldingConceptForConcept(backPropTriggerCon, backPropImpConcept, false);
				return impliedConceptTrigger;
			}






			bool CTriggeredImplicationBinaryAbsorberPreProcess::absorbPartialGCIConceptsToTriggeredImplications(CConcept* orConcept, bool negated) {
				cint64 opCode = orConcept->getOperatorCode();
				if (!negated && opCode == CCOR || negated && opCode == CCAND) {
					QList<TConceptNegationPair> candList;
					if (addAbsorbableDisjunctCandidates(orConcept,negated,candList)) {
						if (candList.count() >= 1) {
							QList<TConceptNegationPair> absorbList(getPartialAbsorbableDisjuncts(candList));
							if (!absorbList.isEmpty()) {
								createGCIPartialAbsorbedTriggeredImplication(orConcept,negated,absorbList);

								if (absorbList.count() + 1 == candList.count()) {
									QList<TConceptNegationPair> assurList(getAssuringAbsorbableDisjuncts(absorbList));
									if (assurList.count() == absorbList.count()) {
										createGCIAssuringAbsorbedTriggeredImplication(orConcept, negated, assurList, candList);
									}
								}

								return true;
							}
						}
					}
				}
				return false;
			}







			void CTriggeredImplicationBinaryAbsorberPreProcess::createGCIAssuringAbsorbedTriggeredImplication(CConcept* orConcept, bool negated, const QList<TConceptNegationPair>& absorbList, const QList<TConceptNegationPair>& disjList) {


				CConcept* remainingDisConcept = nullptr;
				bool remainingDisConNegation = false;

				QSet<TConceptNegationPair> absorbDisjSet = absorbList.toSet();
				for (TConceptNegationPair disConNegPair : disjList) {
					if (!absorbDisjSet.contains(disConNegPair)) {
						remainingDisConcept = disConNegPair.first;
						remainingDisConNegation = disConNegPair.second;
						break;
					}
				}

				CConceptTriggerLinker* partialImpliedTrigger = createAssuringExtendedAbsorbedImpliedTrigger(absorbList);
				CConcept* partialImpliedTriggerConcept = partialImpliedTrigger->getTriggerConcept();
				addUnfoldingConceptForConcept(partialImpliedTriggerConcept, remainingDisConcept, remainingDisConNegation);

			}




			CConceptTriggerLinker* CTriggeredImplicationBinaryAbsorberPreProcess::createAssuringExtendedAbsorbedImpliedTrigger(const QList<TConceptNegationPair>& absorbList) {
				CConceptTriggerLinker* triggers = nullptr;
				for (QList<TConceptNegationPair>::const_iterator it = absorbList.constBegin(), itEnd = absorbList.constEnd(); it != itEnd; ++it) {
					TConceptNegationPair conNegPair(*it);
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					CConceptTriggerLinker* tmpTriggers = copyTriggerLinkers(getAssuringTriggersForConcept(concept, !negated));
					triggers = tmpTriggers->append(triggers);
				}
				CConceptTriggerLinker* impliedConceptTrigger = getImplicationTriggeredConceptForTriggers(triggers, nullptr);
				return impliedConceptTrigger;
			}



			bool CTriggeredImplicationBinaryAbsorberPreProcess::absorbNominalSchemaGCIConceptsToTriggeredImplications(CConcept* concept, bool negated) {

				if (concept->getOperatorCode() == CCNOMTEMPLREF) {

					cint64 nomSchTemplIndex = concept->getParameter();
					CNominalSchemaTemplate* nsTemplate = mNomSchemaTemplVec->getData(nomSchTemplIndex);

					if (nsTemplate) {
						CConcept* templConcept = nsTemplate->getTemplateConcept();

						cint64 opCode = templConcept->getOperatorCode();
						if (!negated && opCode == CCOR || negated && opCode == CCAND) {
							QList<TConceptNegationPair> candList;
							if (addAbsorbableDisjunctCandidates(templConcept,negated,candList)) {
								if (candList.count() >= 1) {
									QList<TConceptNegationPair> absorbList(splitAbsorbableDisjuncts(candList));
									QList<TConceptNegationPair> partialAbsorbList(getPartialAbsorbableDisjuncts(candList));
									if (!absorbList.isEmpty() || !partialAbsorbList.isEmpty()) {
										createGCINominalSchemaAbsorbedTriggeredImplication(templConcept,negated,absorbList,partialAbsorbList,candList,nsTemplate);
										return true;
									}
								}
							}
						}

					}
				}
				return false;
			}







			bool CTriggeredImplicationBinaryAbsorberPreProcess::absorbNominalConceptsToAssertions(CConcept* orConcept, bool negated) {
				bool absorbed = false;
				cint64 opCode = orConcept->getOperatorCode();
				if (!negated && opCode == CCOR || negated && opCode == CCAND) {

					if (orConcept->getOperandCount() == 2) {
						CConcept* nominalCon = orConcept->getOperandList()->getData();
						bool nominalConNeg = orConcept->getOperandList()->isNegated() ^ negated;

						CConcept* impliedCon = orConcept->getOperandList()->getNext()->getData();
						bool impliedConNeg = orConcept->getOperandList()->getNext()->isNegated() ^ negated;


						if (!nominalConNeg || nominalCon->getOperatorCode() != CCNOMINAL) {
							CConcept* tmpCon = nominalCon;
							bool tmpConNeg = nominalConNeg;

							nominalCon = impliedCon;
							nominalConNeg = impliedConNeg;
							impliedCon = tmpCon;
							impliedConNeg = tmpConNeg;
						}


						if (nominalConNeg && nominalCon->getOperatorCode() == CCNOMINAL) {
							CIndividual* nomIndi = nominalCon->getNominalIndividual();

							cint64 impliedConCode = impliedCon->getOperatorCode();
							if (!impliedConNeg && impliedConCode == CCSOME && impliedCon->getOperandCount() == 1) {

								CConcept* someQuantifiedCon = impliedCon->getOperandList()->getData();
								bool someQuantifiedConNeg = impliedCon->getOperandList()->isNegated() ^ impliedConNeg;
								cint64 someQuantifiedConCode = someQuantifiedCon->getOperatorCode();

								if (!someQuantifiedConNeg && someQuantifiedConCode == CCNOMINAL) {
									absorbed = true;
									// absorb as role assertion
									CIndividual* connNomIndi = someQuantifiedCon->getNominalIndividual();
									CIndividual* locatedIndividual = CConceptRoleIndividualLocator::getLocatedIndividual(nomIndi, mOnto);

									CRoleAssertionLinker* roleAssLinker = CObjectAllocator< CRoleAssertionLinker >::allocateAndConstruct(mMemMan);
									roleAssLinker->initRoleAssertionLinker(impliedCon->getRole(), connNomIndi);
									locatedIndividual->addAssertionRoleLinker(roleAssLinker);
								}
							}

							if (!absorbed) {
								absorbed = true;
								// absorb as concept assertion
								CIndividual* locatedIndividual = CConceptRoleIndividualLocator::getLocatedIndividual(nomIndi, mOnto);
								CConceptAssertionLinker* assLinker = CObjectAllocator< CConceptAssertionLinker >::allocateAndConstruct(mMemMan);
								assLinker->initNegLinker(impliedCon, impliedConNeg);
								locatedIndividual->addAssertionConceptLinker(assLinker);
							}


						}
					}


				}
				return absorbed;
			}


			bool CTriggeredImplicationBinaryAbsorberPreProcess::absorbNegatedSelfToRoleDomain(CConcept* concept, bool negated) {
				bool absorbed = false;
				cint64 opCode = concept->getOperatorCode();
				if (negated && opCode == CCSELF) {

					CRole* role = concept->getRole();
					CRole* locatedRole = CConceptRoleIndividualLocator::getLocatedRole(role, mOnto);
					CSortedNegLinker<CConcept*>* domainConLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
					domainConLinker->init(concept, true);
					locatedRole->addDomainConceptLinker(domainConLinker);

					absorbed = true;
				}
				return absorbed;
			}





			void CTriggeredImplicationBinaryAbsorberPreProcess::createGCINominalSchemaAbsorbedTriggeredImplication(CConcept* orConcept, bool negated, QList<TConceptNegationPair>& absorbList, QList<TConceptNegationPair>& partialAbsorbList, QList<TConceptNegationPair>& candList, CNominalSchemaTemplate* nsTemplate) {
				CConcept* impliedConcept = nullptr;
				bool impliedNegation = false; 
				QList<TConceptNegationPair> partialAbsorbableList;


				CConceptTriggerLinker* triggers = nullptr;
				for (QList<TConceptNegationPair>::const_iterator it = absorbList.constBegin(), itEnd = absorbList.constEnd(); it != itEnd; ++it) {
					TConceptNegationPair conNegPair(*it);
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					CConceptTriggerLinker* tmpTriggers = copyTriggerLinkers(getPartialTriggersForConcept(concept,!negated));
					triggers = tmpTriggers->append(triggers);
				}
				for (QList<TConceptNegationPair>::const_iterator it = partialAbsorbList.constBegin(), itEnd = partialAbsorbList.constEnd(); it != itEnd; ++it) {
					TConceptNegationPair conNegPair(*it);
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					CConceptTriggerLinker* tmpTriggers = copyTriggerLinkers(getPartialTriggersForConcept(concept,!negated));
					triggers = tmpTriggers->append(triggers);
				}
				CConceptTriggerLinker* impliedConceptTrigger = getImplicationTriggeredConceptForTriggers(triggers,nullptr);
				impliedConcept = impliedConceptTrigger->getTriggerConcept();


				QList<TConceptNegationPair> absorpConceptList(absorbList);
				absorpConceptList.append(partialAbsorbList);

				QSet<TConceptNegationPair> conceptSet;
				for (QList<TConceptNegationPair>::const_iterator it = absorpConceptList.constBegin(), itEnd = absorpConceptList.constEnd(); it != itEnd; ++it) {
					TConceptNegationPair conNegPair(*it);
					CConcept* operandConcept = conNegPair.first;
					bool operandConceptNegation = conNegPair.second;
					conceptSet.insert(TConceptNegationPair(operandConcept,!operandConceptNegation));
				}


				QHash<CConcept*,CVariable*> nomSchConceptVariableHash;
				CBOXSET<CConcept*>* nominalSchemasSet = nsTemplate->getNominalSchemaConceptSet();
				for (CBOXSET<CConcept*>::const_iterator it = nominalSchemasSet->constBegin(), itEnd = nominalSchemasSet->constEnd(); it != itEnd; ++it) {
					CConcept* nominalSchemaConcept = *it;
					CVariable* variable = CObjectAllocator< CVariable >::allocateAndConstruct(mMemMan);
					variable->initVariable(nominalSchemaConcept,mNextVariableID++);
					nomSchConceptVariableHash.insert(nominalSchemaConcept,variable);
				}

				QList<TConceptNegationPair> groundConceptList(candList);

				CConcept* nextAbsorptionConcept = impliedConcept;

				bool pathPreabsorption = false;
				if (mConfNominalSchemaPathPreabsorption) {
					if (nominalSchemasSet->count() >= 3) {
						pathPreabsorption = true;
					}
				}

				if (pathPreabsorption) {
					// create nominal schema absorption path
					CNominalSchemaAbsorptionPathLinker* absorptionPathLinker = new CNominalSchemaAbsorptionPathLinker();
					absorptionPathLinker->initNominalSchemaAbsorptionPathLinker(nullptr);
					absorptionPathLinker->setLastPathTriggerConcept(nextAbsorptionConcept);

					absorptionPathLinker = getAbsorbedNominalSchemaPaths(conceptSet,absorptionPathLinker,&nomSchConceptVariableHash,nsTemplate);
					nextAbsorptionConcept = createBindingBackPropagationTriggerConcept();

					for (CNominalSchemaAbsorptionPathLinker* absorptionPathLinkerIt = absorptionPathLinker; absorptionPathLinkerIt; absorptionPathLinkerIt = absorptionPathLinkerIt->getNext()) {
						CConcept* groundingConcept = nullptr;
						bool groundingConNegation = false;
						CNominalSchemaTemplate* grNSTemplate = nsTemplate;
						createBindingGroundConcept(groundingConcept,groundingConNegation,groundConceptList,grNSTemplate,CCPBINDGROUND);
						CConcept* lastPathTriggerConcept = absorptionPathLinkerIt->getLastPathTriggerConcept();
						addConceptOperand(lastPathTriggerConcept,groundingConcept,groundingConNegation);

						addConceptOperand(groundingConcept,nextAbsorptionConcept,false);
					}
				} else {

					CConcept* tmpNextAbsorptionConcept = createBindingBackPropagationActivationTriggerConcept();
					addConceptOperand(nextAbsorptionConcept,tmpNextAbsorptionConcept,false);
					nextAbsorptionConcept = tmpNextAbsorptionConcept;

				}


				CNominalSchemaAbsorptionBranchLinker* absorptionBranchLinker = getAbsorbedJoinedNominalSchemaBranchLinker(conceptSet,nextAbsorptionConcept,&nomSchConceptVariableHash,nsTemplate);

				for (CNominalSchemaAbsorptionBranchLinker* absorptionBranchLinkerIt = absorptionBranchLinker; absorptionBranchLinkerIt; absorptionBranchLinkerIt = absorptionBranchLinkerIt->getNext()) {
					CConcept* groundingConcept = nullptr;
					bool groundingConNegation = false;
					CNominalSchemaTemplate* grNSTemplate = nsTemplate;
					createBindingGroundConcept(groundingConcept,groundingConNegation,groundConceptList,grNSTemplate,CCVARBINDGROUND);
					CConcept* lastPathTriggerConcept = absorptionBranchLinkerIt->getLastBranchTriggerConcept();
					addConceptOperand(lastPathTriggerConcept,groundingConcept,groundingConNegation);
				}

			}



			void CTriggeredImplicationBinaryAbsorberPreProcess::createBindingGroundConcept(CConcept*& groundingConcept, bool& groundingConNegation, const QList<TConceptNegationPair>& groundConceptList, CNominalSchemaTemplate* nsTemplate, cint64 groundOpCode) {
				CConcept* groundConcept = nullptr;
				bool groundConNegation = false;
				bool createNewTemplate = false;
				if (groundConceptList.count() > 1) {

					CConcept* orConcept = createNewConcept();
					orConcept->setOperatorCode(CCOR);


					for (QList<TConceptNegationPair>::const_iterator it = groundConceptList.constBegin(), itEnd = groundConceptList.constEnd(); it != itEnd; ++it) {
						TConceptNegationPair conNegPair(*it);
						CConcept* operandConcept = conNegPair.first;
						bool operandConceptNegation = conNegPair.second;
						addConceptOperand(orConcept,operandConcept,operandConceptNegation);
					}

					groundConcept = orConcept;
					groundConNegation = false;
					createNewTemplate = true;
				} else if (groundConceptList.count() == 1) {
					TConceptNegationPair conNegPair(groundConceptList.first());
					groundConcept = conNegPair.first;
					groundConNegation = conNegPair.second;
					createNewTemplate = true;
				} else {
					groundConcept = mTopConcept;
					groundConNegation = true;
				}
 

				if (createNewTemplate) {
					nsTemplate = CObjectAllocator< CNominalSchemaTemplate >::allocateAndConstruct(mMemMan);
					CBOXSET<CConcept*>* nominalSchemaConceptSet = CObjectAllocator< CBOXSET<CConcept*> >::allocateAndConstruct(mMemMan);
					CBOXHASH<CConcept*,CConcept*>* conceptNominalSchemaConceptHash = CObjectAllocator< CBOXHASH<CConcept*,CConcept*> >::allocateAndConstruct(mMemMan);
					CBOXHASH<CConcept*,CConcept*>* absorbableConceptNominalSchemaConceptHash = CObjectAllocator< CBOXHASH<CConcept*,CConcept*> >::allocateAndConstruct(mMemMan);
					nsTemplate->initNominalSchemaTemplate(nominalSchemaConceptSet,conceptNominalSchemaConceptHash,absorbableConceptNominalSchemaConceptHash);
					if (!mNomSchemaTemplVec) {
						mNomSchemaTemplVec = mMBox->getNominalSchemaTemplateVector();
					}
					cint64 nextNomSchTemplID = mNomSchemaTemplVec->getItemCount();
					nsTemplate->setNominalSchemaTemplateTag(nextNomSchTemplID);
					mNomSchemaTemplVec->setData(nextNomSchTemplID,nsTemplate);
					nsTemplate->setTemplateConcept(groundConcept);


					collectNominalSchemaConcepts(groundConcept,nominalSchemaConceptSet);
					FOREACHIT (CConcept* nomSchConcept, *nominalSchemaConceptSet) {
						QSet<CConcept*> testedNomSchemConSet;
						hasNominalSchemaConceptFillHash(groundConcept,conceptNominalSchemaConceptHash,nomSchConcept,&testedNomSchemConSet);
						QSet<CConcept*> testedNomSchemAbsorbedConSet;
						hasNominalSchemaAbsorbableConceptFillHash(groundConcept,groundingConNegation,absorbableConceptNominalSchemaConceptHash,nomSchConcept,&testedNomSchemAbsorbedConSet);
					}

					groundingConcept = createBindingGroundingConcept(nsTemplate,groundOpCode);
				} else {
					groundingConcept = groundConcept;
				}
				groundingConNegation = groundConNegation;

			}

			bool CTriggeredImplicationBinaryAbsorberPreProcess::hasNominalSchemaAbsorbableConceptFillHash(CConcept* concept, bool negation, CBOXHASH<CConcept*,CConcept*>* absorbableConceptNominalSchemaConceptHash, CConcept* nominalSchemaConcept, QSet<CConcept*>* testedConceptSet) {
				cint64 conOpCode = concept->getOperatorCode();
				if (conOpCode == CCSUB || conOpCode == CCEQ || conOpCode == CCTOP || conOpCode == CCBOTTOM || concept->getConceptOperator()->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_TRIG_TYPE)) {
					return false;
				}
				bool foundAbsorbableNominalSchemaConcept = false;

				if (negation && concept == nominalSchemaConcept) {
					foundAbsorbableNominalSchemaConcept = true;
				} else {
					cint64 conCode = concept->getOperatorCode();
					if (negation && (conCode == CCSOME) || !negation && (conCode == CCALL)) {
						for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
							CConcept* opConcept = opConLinkerIt->getData();
							foundAbsorbableNominalSchemaConcept |= hasNominalSchemaAbsorbableConceptFillHash(opConcept,negation^opConLinkerIt->isNegated(),absorbableConceptNominalSchemaConceptHash,nominalSchemaConcept,testedConceptSet);
						}
					} else if (!negation && (conCode == CCAND || conCode == CCOR) || negation && (conCode == CCOR || conCode == CCAND)) {
						for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
							CConcept* opConcept = opConLinkerIt->getData();
							foundAbsorbableNominalSchemaConcept |= hasNominalSchemaAbsorbableConceptFillHash(opConcept,negation^opConLinkerIt->isNegated(),absorbableConceptNominalSchemaConceptHash,nominalSchemaConcept,testedConceptSet);
						}
					}
				}

				if (foundAbsorbableNominalSchemaConcept) {
					if (!testedConceptSet->contains(concept)) {
						testedConceptSet->insert(concept);
						absorbableConceptNominalSchemaConceptHash->insertMulti(concept,nominalSchemaConcept);
					}
				}
				return foundAbsorbableNominalSchemaConcept;			
			}



			bool CTriggeredImplicationBinaryAbsorberPreProcess::hasNominalSchemaConceptFillHash(CConcept* concept, CBOXHASH<CConcept*,CConcept*>* conceptNominalSchemaConceptHash, CConcept* nominalSchemaConcept, QSet<CConcept*>* testedConceptSet) {
				cint64 conOpCode = concept->getOperatorCode();
				if (conOpCode == CCSUB || conOpCode == CCEQ || conOpCode == CCTOP || conOpCode == CCBOTTOM || concept->getConceptOperator()->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_TRIG_TYPE)) {
					return false;
				}
				bool foundNominalSchemaConcept = false;

				if (concept == nominalSchemaConcept) {
					foundNominalSchemaConcept = true;
				} else {
					for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
						CConcept* opConcept = opConLinkerIt->getData();
						foundNominalSchemaConcept |= hasNominalSchemaConceptFillHash(opConcept,conceptNominalSchemaConceptHash,nominalSchemaConcept,testedConceptSet);
					}
				}

				if (foundNominalSchemaConcept) {
					if (!testedConceptSet->contains(concept)) {
						testedConceptSet->insert(concept);
						conceptNominalSchemaConceptHash->insertMulti(concept,nominalSchemaConcept);
					}
				}
				return foundNominalSchemaConcept;
			}


			void CTriggeredImplicationBinaryAbsorberPreProcess::collectNominalSchemaConcepts(CConcept* concept, CBOXSET<CConcept*>* nominalSchemaConceptSet) {
				cint64 conOpCode = concept->getOperatorCode();
				if (conOpCode == CCSUB || conOpCode == CCEQ || conOpCode == CCTOP || conOpCode == CCBOTTOM || concept->getConceptOperator()->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_TRIG_TYPE)) {
				} else if (conOpCode == CCNOMVAR) {
					nominalSchemaConceptSet->insert(concept);
				} else {
					for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
						CConcept* opConcept = opConLinkerIt->getData();
						collectNominalSchemaConcepts(opConcept,nominalSchemaConceptSet);
					}
				}
			}


			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createBindingGroundingConcept(CNominalSchemaTemplate* nsTemplate, cint64 groundOpCode) {
				CConcept* groundingConcept = createNewConcept();
				groundingConcept->setOperatorCode(groundOpCode);
				groundingConcept->setParameter(nsTemplate->getNominalSchemaTemplateTag());
				return groundingConcept;
			}



			CNominalSchemaAbsorptionPathLinker* CTriggeredImplicationBinaryAbsorberPreProcess::getAbsorbedNominalSchemaPaths(const QSet<TConceptNegationPair>& conceptSet, CNominalSchemaAbsorptionPathLinker* absorptionPathLinker, QHash<CConcept*,CVariable*>* nomSchConceptVariableHash, CNominalSchemaTemplate* nsTemplate) {

				CConceptTriggerLinker* triggers = nullptr;

				QSet<CConcept*> conceptEqConAbsorbed;
				QSet<TConceptNegationPair> relatedConceptSet;
				for (QSet<TConceptNegationPair>::const_iterator it = conceptSet.constBegin(), itEnd = conceptSet.constEnd(); it != itEnd; ++it) {
					TConceptNegationPair conNegPair(*it);
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;

					collectNominalSchemaRelatedConcepts(concept,negated,relatedConceptSet,nsTemplate);

					isConceptPartialImplicationTriggerable(concept,negated,&conceptEqConAbsorbed);
					CConceptTriggerLinker* tmpTriggers = copyTriggerLinkers(getPartialTriggersForConcept(concept,negated));
					if (tmpTriggers) {
						triggers = tmpTriggers->append(triggers);
					}
				}

				QList<TConceptNegationPair> nomSchConList;
				QList<TConceptNegationPair> otherConList;
				QList<TConceptNegationPair> orConList;
				sortNominalSchemaRelatedConcepts(relatedConceptSet,nsTemplate,nomSchConList,otherConList,orConList);

				CConceptTriggerLinker* impliedConceptTrigger = getImplicationTriggeredConceptForTriggers(triggers,nullptr);
				CConcept* triggerConcept = impliedConceptTrigger->getTriggerConcept();



				CNominalSchemaAbsorptionPathLinker* newAbsorptionPathLinker = nullptr;

				CNominalSchemaAbsorptionPathLinker* absorptionPathLinkerIt = absorptionPathLinker;
				while (absorptionPathLinkerIt) {
					CNominalSchemaAbsorptionPathLinker* absorptionPath = absorptionPathLinkerIt;
					absorptionPathLinkerIt = absorptionPathLinkerIt->getNext();
					absorptionPath->clearNext();


					CConcept* lastPathTriggerConcept = absorptionPath->getLastPathTriggerConcept();
					QList<TConceptNegationPair> initNomSchConList;
					CConcept* levelTriggerConcept = nullptr;


					if (triggerConcept != lastPathTriggerConcept) {
						CConcept* nextPathTriggerConcept = createBindingTriggerConcept();
						CConcept* implicationConcept = createBindingImplicationTriggeredConcept(triggerConcept,nextPathTriggerConcept);
						addConceptOperand(lastPathTriggerConcept,implicationConcept,false);
						lastPathTriggerConcept = nextPathTriggerConcept;
					}


					// find already binded nominal schema variables
					QHash<CConcept*,CConcept*>* nomSchConTrigHash = absorptionPath->getNominalSchemaBindedConceptTriggerHash();
					for (QList<TConceptNegationPair>::const_iterator it = nomSchConList.constBegin(), itEnd = nomSchConList.constEnd(); it != itEnd; ++it) {
						TConceptNegationPair nomSchConNegPair(*it);
						CConcept* nomSchConcept = nomSchConNegPair.first;

						CConcept* nominalBindConceptTrigger = nomSchConTrigHash->value(nomSchConcept,nullptr);
						if (nominalBindConceptTrigger) {
							CConcept* nextPathTriggerConcept = createBindingTriggerConcept();
							if (!levelTriggerConcept) {
								levelTriggerConcept = nominalBindConceptTrigger;
							}

							CConcept* cycleCloseConcept = createBindingCycleCloseConcept(nominalBindConceptTrigger,nextPathTriggerConcept);
							addConceptOperand(lastPathTriggerConcept,cycleCloseConcept,false);
							lastPathTriggerConcept = nextPathTriggerConcept;
						} else {
							initNomSchConList.append(nomSchConNegPair);
						}
					}

					//if (!levelTriggerConcept) {
					//	levelTriggerConcept = createBindingTriggerConcept();
					//	cint64 nextVariableID = 0;
					//	if (absorptionPath->hasVariables()) {
					//		nextVariableID = absorptionPath->getLastVariable()->getPathVariableID()+1;
					//	}
					//	CConcept* nominalConcept = nullptr;
					//	if (!initNomSchConList.isEmpty()) {
					//		TConceptNegationPair nomSchConNegPair(initNomSchConList.first());
					//		CConcept* nomSchConcept = nomSchConNegPair.first;
					//		nominalConcept = nomSchConcept;
					//	}
					//	CVariable* variable = CObjectAllocator< CVariable >::allocateAndConstruct(mMemMan);
					//	variable->initVariable(nominalConcept,nextVariableID);
					//	absorptionPath->addVariable(variable);
					//	CConcept* varBindConcept = createBindingVariableConcept(levelTriggerConcept,variable);
					//	addConceptOperand(lastPathTriggerConcept,varBindConcept,false);
					//	lastPathTriggerConcept = levelTriggerConcept;
					//}

					for (QList<TConceptNegationPair>::const_iterator it = initNomSchConList.constBegin(), itEnd = initNomSchConList.constEnd(); it != itEnd; ++it) {
						TConceptNegationPair nomSchConNegPair(*it);
						CConcept* nomSchConcept = nomSchConNegPair.first;

						CConcept* nextTriggerConcept = createBindingTriggerConcept();

						CVariable* variable = nomSchConceptVariableHash->value(nomSchConcept);
						absorptionPath->addVariable(variable);
						CConcept* varBindConcept = createBindingVariableConcept(nextTriggerConcept,variable);
						addConceptOperand(lastPathTriggerConcept,varBindConcept,false);

						nomSchConTrigHash->insert(nomSchConcept,nextTriggerConcept);
						lastPathTriggerConcept = nextTriggerConcept;

						if (!levelTriggerConcept) {
							levelTriggerConcept = nextTriggerConcept;
						}
					}

					absorptionPath->setLastPathTriggerConcept(lastPathTriggerConcept);

					// build absorption path recursive
					CNominalSchemaAbsorptionPathLinker* nextAbsorptionPathLinker = absorptionPath;
					for (QList<TConceptNegationPair>::const_iterator itRec = otherConList.constBegin(), itRecEnd = otherConList.constEnd(); itRec != itRecEnd; ++itRec) {
						TConceptNegationPair succConNegPair(*itRec);
						CConcept* succConcept = succConNegPair.first;
						bool succConNegation = succConNegPair.second;
						cint64 succConOpCode = succConcept->getOperatorCode();
						cint64 param = succConcept->getParameter();

						if (!succConNegation && (succConOpCode == CCSOME || succConOpCode == CCATLEAST && param == 1) || succConNegation && (succConOpCode == CCALL || succConOpCode == CCATMOST && param == 0)) {
							CRole* role = succConcept->getRole();
							bool succOpNegate = succConOpCode == CCALL;

							if (succConcept->getOperandList()) {

								for (CNominalSchemaAbsorptionPathLinker* nextAbsorptionPathLinkerIt = nextAbsorptionPathLinker; nextAbsorptionPathLinkerIt; nextAbsorptionPathLinkerIt = nextAbsorptionPathLinkerIt->getNext()) {
									CConcept* nextAbsorptionPathTriggerConcept = createBindingTriggerConcept();
									CConcept* lastAbsorptionPathTriggerConcept = nextAbsorptionPathLinkerIt->getLastPathTriggerConcept();
									CConcept* propagationPathTriggerConcept = createBindingToSuccessorPropagationConcept(nextAbsorptionPathTriggerConcept,role);
									addConceptOperand(lastAbsorptionPathTriggerConcept,propagationPathTriggerConcept,false);
									nextAbsorptionPathLinkerIt->setLastPathTriggerConcept(nextAbsorptionPathTriggerConcept);
								}

								QSet<TConceptNegationPair> operandConceptSet;
								for (CSortedNegLinker<CConcept*>* succOpConLinkerIt = succConcept->getOperandList(); succOpConLinkerIt; succOpConLinkerIt = succOpConLinkerIt->getNext()) {
									CConcept* succOpCon = succOpConLinkerIt->getData();
									bool succOpConNeg = succOpConLinkerIt->isNegated() ^ succOpNegate;
									operandConceptSet.insert(TConceptNegationPair(succOpCon,succOpConNeg));
								}

								nextAbsorptionPathLinker = getAbsorbedNominalSchemaPaths(operandConceptSet,nextAbsorptionPathLinker,nomSchConceptVariableHash,nsTemplate);

								for (CNominalSchemaAbsorptionPathLinker* nextAbsorptionPathLinkerIt = nextAbsorptionPathLinker; nextAbsorptionPathLinkerIt; nextAbsorptionPathLinkerIt = nextAbsorptionPathLinkerIt->getNext()) {
									CConcept* propNextAbsorptionPathTriggerConcept = createBindingTriggerConcept();
									CConcept* lastAbsorptionPathTriggerConcept = nextAbsorptionPathLinkerIt->getLastPathTriggerConcept();
									CConcept* propagationPathTriggerConcept = createBindingFromSuccessorPropagationConcept(propNextAbsorptionPathTriggerConcept,role);
									addConceptOperand(lastAbsorptionPathTriggerConcept,propagationPathTriggerConcept,false);

									if (levelTriggerConcept) {
										CConcept* cylceNextAbsorptionPathTriggerConcept = createBindingTriggerConcept();
										CConcept* cycleCloseConcept = createBindingCycleCloseConcept(levelTriggerConcept,cylceNextAbsorptionPathTriggerConcept);
										addConceptOperand(propNextAbsorptionPathTriggerConcept,cycleCloseConcept,false);

										nextAbsorptionPathLinkerIt->setLastPathTriggerConcept(cylceNextAbsorptionPathTriggerConcept);
									} else {
										nextAbsorptionPathLinkerIt->setLastPathTriggerConcept(propNextAbsorptionPathTriggerConcept);
									}
								}
							}

						}
					}


					for (QList<TConceptNegationPair>::const_iterator itOr = orConList.constBegin(), itOrEnd = orConList.constEnd(); itOr != itOrEnd; ++itOr) {
						TConceptNegationPair orConNegPair(*itOr);
						CConcept* orConcept = orConNegPair.first;
						bool orConNegation = orConNegPair.second;
						cint64 orConOpCode = orConcept->getOperatorCode();

						if (!orConNegation && (orConOpCode == CCOR) || orConNegation && (orConOpCode == CCAND)) {

							cint64 opCount = orConcept->getOperandCount();
							if (opCount > 1) {

								CNominalSchemaAbsorptionPathLinker* newNextAbsorptionPathLinker = nullptr;

								CNominalSchemaAbsorptionPathLinker* nextAbsorptionPathLinkerIt = nextAbsorptionPathLinker;
								while (nextAbsorptionPathLinkerIt) {
									nextAbsorptionPathLinker = nextAbsorptionPathLinkerIt;
									nextAbsorptionPathLinkerIt = nextAbsorptionPathLinkerIt->getNext();
									nextAbsorptionPathLinker->clearNext();

									CNominalSchemaAbsorptionPathLinker* dublicateAbsorptionPathLinkerIt = duplicateNominalSchemaAbsorptionPaths(nextAbsorptionPathLinker,opCount);

									for (CSortedNegLinker<CConcept*>* orOpConLinkerIt = orConcept->getOperandList(); orOpConLinkerIt; orOpConLinkerIt = orOpConLinkerIt->getNext()) {
										CNominalSchemaAbsorptionPathLinker* dublicateAbsorptionPathLinker = dublicateAbsorptionPathLinkerIt;
										dublicateAbsorptionPathLinkerIt = dublicateAbsorptionPathLinkerIt->getNext();
										dublicateAbsorptionPathLinker->clearNext();
										CConcept* orOpCon = orOpConLinkerIt->getData();
										bool orOpConNeg = orOpConLinkerIt->isNegated();

										QSet<TConceptNegationPair> operandConceptSet;
										operandConceptSet.insert(TConceptNegationPair(orOpCon,orOpConNeg));
										dublicateAbsorptionPathLinker = getAbsorbedNominalSchemaPaths(operandConceptSet,dublicateAbsorptionPathLinker,nomSchConceptVariableHash,nsTemplate);

										newNextAbsorptionPathLinker = dublicateAbsorptionPathLinker->append(newNextAbsorptionPathLinker);
									}

								}

								nextAbsorptionPathLinker = newNextAbsorptionPathLinker;

							}
						}
					}

					newAbsorptionPathLinker = nextAbsorptionPathLinker->append(newAbsorptionPathLinker);


				}

				return newAbsorptionPathLinker;
			}



			CNominalSchemaAbsorptionPathLinker* CTriggeredImplicationBinaryAbsorberPreProcess::duplicateNominalSchemaAbsorptionPaths(CNominalSchemaAbsorptionPathLinker* abPathLinker, cint64 duplicateCount) {
				CNominalSchemaAbsorptionPathLinker* duplicateLinker = abPathLinker;
				for (cint64 i = 1; i < duplicateCount; ++i) {
					CNominalSchemaAbsorptionPathLinker* linker = new CNominalSchemaAbsorptionPathLinker();
					linker->initNominalSchemaAbsorptionPathLinker(abPathLinker);
					duplicateLinker = linker->append(duplicateLinker);
				}
				return duplicateLinker;
			}



			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createBindingToSuccessorPropagationConcept(CConcept* followingConcept, CRole* role) {
				CConcept* succPropConcept = createNewConcept();
				succPropConcept->setOperatorCode(CCPBINDALL);
				succPropConcept->setRole(role);
				addConceptOperand(succPropConcept,followingConcept,false);
				return succPropConcept;
			}

			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createBindingFromSuccessorPropagationConcept(CConcept* followingConcept, CRole* role) {
				CRole* inverseRole = getInverseRole(role);
				CConcept* succPropConcept = createNewConcept();
				succPropConcept->setOperatorCode(CCPBINDALL);
				succPropConcept->setRole(inverseRole);
				addConceptOperand(succPropConcept,followingConcept,false);
				return succPropConcept;
			}


			void CTriggeredImplicationBinaryAbsorberPreProcess::addConceptOperand(CConcept* concept, CConcept* operandConcept, bool operandNegation) {
				CSortedNegLinker<CConcept*>* newBaseOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon->init(operandConcept,operandNegation,concept->getOperandList());
				concept->setOperandList(newBaseOpCon);
				concept->incOperandCount();
			}



			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createBindingTriggerConcept() {
				CConcept* triggConcept = createNewConcept();
				triggConcept->setOperatorCode(CCPBINDTRIG);
				return triggConcept;
			}




			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createBindingImplicationTriggeredConcept(CConcept* triggeredConcept, CConcept* impliedConcept) {
				CConcept* implicationConcept = createNewConcept();
				implicationConcept->setOperatorCode(CCPBINDIMPL);

				CSortedNegLinker<CConcept*>* newBaseOpCon1 = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon1->init(triggeredConcept,true);
				CSortedNegLinker<CConcept*>* newBaseOpCon2 = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon2->init(impliedConcept,false,newBaseOpCon1);
				implicationConcept->setOperandList(newBaseOpCon2);
				implicationConcept->incOperandCount(2);

				return implicationConcept;
			}



			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createBindingCycleCloseConcept(CConcept* cycleCloseTriggerConcept, CConcept* impliedTriggerConcept) {
				CConcept* cycleConcept = createNewConcept();
				cycleConcept->setOperatorCode(CCPBINDCYCLE);

				CSortedNegLinker<CConcept*>* newBaseOpCon1 = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon1->init(cycleCloseTriggerConcept,true);
				CSortedNegLinker<CConcept*>* newBaseOpCon2 = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon2->init(impliedTriggerConcept,false,newBaseOpCon1);
				cycleConcept->setOperandList(newBaseOpCon2);
				cycleConcept->incOperandCount(2);

				return cycleConcept;
			}


			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createBindingVariableConcept(CConcept* followingConcept, CVariable* variable) {
				CConcept* bindConcept = createNewConcept();
				bindConcept->setOperatorCode(CCPBINDVARIABLE);
				bindConcept->setVariableLinker(CObjectAllocator< CSortedLinker<CVariable*> >::allocateAndConstruct(mMemMan)->init(variable));

				addConceptOperand(bindConcept,followingConcept,false);

				return bindConcept;
			}


			void CTriggeredImplicationBinaryAbsorberPreProcess::sortNominalSchemaRelatedConcepts(QSet<TConceptNegationPair>& conceptSet, CNominalSchemaTemplate* nsTemplate, QList<TConceptNegationPair>& nomSchConList, QList<TConceptNegationPair>& otherConList, QList<TConceptNegationPair>& orConList) {
				for (QSet<TConceptNegationPair>::const_iterator it = conceptSet.constBegin(), itEnd = conceptSet.constEnd(); it != itEnd; ++it) {
					TConceptNegationPair conNegPair(*it);
					CConcept* concept = conNegPair.first;
					bool conNegation = conNegPair.second;
					cint64 opCode = concept->getOperatorCode();
					if (conNegation && (opCode == CCAND) || !conNegation && (opCode == CCOR)) {
						orConList.append(conNegPair);
					} else if (!conNegation && (opCode == CCNOMVAR)) {
						nomSchConList.append(conNegPair);
					} else {
						otherConList.append(conNegPair);
					}
				}
			}


			void CTriggeredImplicationBinaryAbsorberPreProcess::collectNominalSchemaRelatedConcepts(CConcept* concept, bool negated, QSet<TConceptNegationPair>& conceptSet, CNominalSchemaTemplate* nsTemplate) {
				CBOXHASH<CConcept*,CConcept*>* conceptNomSchemaAbsorbableConceptsHash = nsTemplate->getTemplateAbsorbableConceptNominalSchemaConceptHash();
				if (conceptNomSchemaAbsorbableConceptsHash->contains(concept)) {
					cint64 opCode = concept->getOperatorCode();
					if (!negated && (opCode == CCAND) || negated && (opCode == CCOR)) {
						for (CSortedNegLinker<CConcept*>* opLinkIt = concept->getOperandList(); opLinkIt; opLinkIt = opLinkIt->getNext()) {
							CConcept* opConcept = opLinkIt->getData();
							bool opConNegation = opLinkIt->isNegated() ^ negated;
							collectNominalSchemaRelatedConcepts(opConcept,opConNegation,conceptSet,nsTemplate);
						}
					} else {
						conceptSet.insert(TConceptNegationPair(concept,negated));
					}
				}
			}







			CNominalSchemaAbsorptionBranchLinker* CTriggeredImplicationBinaryAbsorberPreProcess::getAbsorbedJoinedNominalSchemaBranchLinker(CConcept* absorbingConcept, bool absorbingNegation, CConcept* backPropConTrigger, QHash<CConcept*,CVariable*>* nomSchConceptVariableHash, CNominalSchemaTemplate* nsTemplate) {

				TConceptNegationPair absConNegPair(absorbingConcept,absorbingNegation);
				CNominalSchemaAbsorptionBranchLinker* abBranchLinker = mConceptNomSchAbsorpBranchLinkerHash.value(absConNegPair);

				if (!abBranchLinker) {
					QSet<TConceptNegationPair> relatedConceptSet;
					relatedConceptSet.insert(TConceptNegationPair(absorbingConcept,absorbingNegation));

					abBranchLinker = getAbsorbedJoinedNominalSchemaBranchLinker(relatedConceptSet,backPropConTrigger,nomSchConceptVariableHash,nsTemplate);

					mConceptNomSchAbsorpBranchLinkerHash.insert(absConNegPair,abBranchLinker);
				}
				return abBranchLinker;
			}




			CNominalSchemaAbsorptionBranchLinker* CTriggeredImplicationBinaryAbsorberPreProcess::getAbsorbedJoinedNominalSchemaBranchLinker(const QSet<TConceptNegationPair>& conceptSet, CConcept* backPropConTrigger, QHash<CConcept*,CVariable*>* nomSchConceptVariableHash, CNominalSchemaTemplate* nsTemplate) {
				CNominalSchemaAbsorptionBranchLinker* newAbLinker = nullptr;


				CConceptTriggerLinker* triggers = nullptr;
				QSet<TConceptNegationPair> relatedConceptSet;
				for (QSet<TConceptNegationPair>::const_iterator it = conceptSet.constBegin(), itEnd = conceptSet.constEnd(); it != itEnd; ++it) {
					TConceptNegationPair conNegPair(*it);
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					collectNominalSchemaRelatedConcepts(concept,negated,relatedConceptSet,nsTemplate);
					CConceptTriggerLinker* tmpTriggers = copyTriggerLinkers(getPartialTriggersForConcept(concept,negated));
					if (tmpTriggers) {
						triggers = tmpTriggers->append(triggers);
					}
				}

				QList<TConceptNegationPair> nomSchConList;
				QList<TConceptNegationPair> otherConList;
				QList<TConceptNegationPair> orConList;
				sortNominalSchemaRelatedConcepts(relatedConceptSet,nsTemplate,nomSchConList,otherConList,orConList);
				QList<TConceptNegationPair> allConList(otherConList);
				allConList += nomSchConList;
				allConList += orConList;

				CConcept* triggerConcept = nullptr;
				if (triggers) {
					CConceptTriggerLinker* impliedConceptTrigger = getImplicationTriggeredConceptForTriggers(triggers,nullptr);
					triggerConcept = impliedConceptTrigger->getTriggerConcept();
				}


				CConcept* activationTriggerConcept = nullptr;
				if (!activationTriggerConcept && mConfNominalSchemaBackPropagationMultiTrigger && mConfNominalSchemaBackPropagation) {
					CBackPropTrigHashData* backPropTrigHashData = nullptr;
					for (QList<TConceptNegationPair>::const_iterator nomSchIt = nomSchConList.constBegin(), nomSchItEnd = nomSchConList.constEnd(); nomSchIt != nomSchItEnd && !backPropTrigHashData; ++nomSchIt) {
						CConcept* nomSchCon = nomSchIt->first;
						backPropTrigHashData = mVarBackPropTrigHash.value(nomSchCon);
					}
					if (backPropTrigHashData) {
						activationTriggerConcept = backPropTrigHashData->mActivationTriggerConcept;
						if (triggerConcept) {
							addBindingBackPropagationTriggeredToImplicationConcept(backPropTrigHashData->mImplicationConcept,triggerConcept);
						}
						addBindingBackPropagationTriggeredToImplicationConcept(backPropTrigHashData->mImplicationConcept,backPropConTrigger);
					}
				}
				if (!activationTriggerConcept) {
					activationTriggerConcept = createBindingBackPropagationTriggerConcept();
					if (!mConfNominalSchemaBackPropagation && triggerConcept) {
						CConcept* tmpNextAbsorptionConcept = createBindingBackPropagationActivationTriggerConcept();
						addConceptOperand(triggerConcept,tmpNextAbsorptionConcept,false);
						addConceptOperand(tmpNextAbsorptionConcept,activationTriggerConcept,false);
					} else {
						if (!triggerConcept) {
							addConceptOperand(backPropConTrigger,activationTriggerConcept,false);
						} else {					
							CConcept* implBindBackPropTrigCon = createBindingBackPropagationImplicationTriggeredConcept(triggerConcept,activationTriggerConcept);
							addConceptOperand(backPropConTrigger,implBindBackPropTrigCon,false);
							if (!nomSchConList.isEmpty() && mConfNominalSchemaBackPropagationMultiTrigger) {
								CBackPropTrigHashData* backPropTrigHashData = new CBackPropTrigHashData(activationTriggerConcept,implBindBackPropTrigCon);
								mVarBackPropTrigContainer.append(backPropTrigHashData);
								foreach (TConceptNegationPair conNegPair, nomSchConList) {
									CConcept* nomSchCon = conNegPair.first;
									mVarBackPropTrigHash.insert(nomSchCon,backPropTrigHashData);
								}
							}
						}
					}
				}



				QList<CNominalSchemaAbsorptionBranchLinker*> newBranchConList;
				QList<CNominalSchemaAbsorptionBranchLinker*> newMultiBranchConList;
				QList<CNominalSchemaAbsorptionBranchLinker*> newSingleBranchConList;

				for (QList<TConceptNegationPair>::const_iterator itRec = allConList.constBegin(), itRecEnd = allConList.constEnd(); itRec != itRecEnd; ++itRec) {
					TConceptNegationPair succConNegPair(*itRec);
					CConcept* succConcept = succConNegPair.first;
					bool succConNegation = succConNegPair.second;
					CNominalSchemaAbsorptionBranchLinker* newBranchLinker = getAbsorbedNominalSchemaBranchLinker(succConcept,succConNegation,activationTriggerConcept,nomSchConceptVariableHash,nsTemplate);
					if (newBranchLinker && newBranchLinker->hasNext()) {
						newMultiBranchConList.append(newBranchLinker);
					} else if (newBranchLinker) {
						newSingleBranchConList.append(newBranchLinker);
					}
				}

				newBranchConList.append(newSingleBranchConList);
				newBranchConList.append(newMultiBranchConList);		



				QList<CNominalSchemaAbsorptionBranchLinker*> newBranchConItList;
				for (QList<CNominalSchemaAbsorptionBranchLinker*>::const_iterator it = newBranchConList.constBegin(), branchConListItEnd = newBranchConList.constEnd(); it != branchConListItEnd; ++it) {
					newBranchConItList.append(*it);
				}
				QList<CNominalSchemaAbsorptionBranchLinker*> orNewBranchConItList(newBranchConItList);


				QList< CNominalSchemaAbsorptionBranchLinker* >::iterator nextChangeEndIt = newBranchConItList.end();

				bool iterationFinished = false;
				while (!iterationFinished) {



					QList<CNominalSchemaAbsorptionBranchLinker*> aborpBranchList;
					for (QList<CNominalSchemaAbsorptionBranchLinker*>::const_iterator it = newBranchConItList.constBegin(), itEnd = newBranchConItList.constEnd(); it != itEnd; ++it) {
						aborpBranchList.append(*it);
					}

					bool removedOne = true;
					while (removedOne) {
						removedOne = false;
						QList<CNominalSchemaAbsorptionBranchLinker*>::iterator it1 = aborpBranchList.begin();
						QList<CNominalSchemaAbsorptionBranchLinker*>::iterator it2 = it1;
						QList<CNominalSchemaAbsorptionBranchLinker*>::iterator itEnd = aborpBranchList.end();
						for (; it1 != itEnd && !removedOne; ++it1) {
							it2 = it1;
							for (++it2; it2 != itEnd && !removedOne; ++it2) {
								CNominalSchemaAbsorptionBranchLinker* abLinker1 = qMin(*it1,*it2);;
								CNominalSchemaAbsorptionBranchLinker* abLinker2 = qMax(*it1,*it2);;

								TAbsorptionBranchPair absBranchPair(abLinker1,abLinker2);
								CNominalSchemaAbsorptionBranchLinker* newAbsBranchLinker = mAbsorptionBranchPairAbsorptionBranchHash.value(absBranchPair,nullptr);
								if (newAbsBranchLinker) {
									removedOne = true;
									aborpBranchList.removeOne(abLinker1);
									aborpBranchList.removeOne(abLinker2);
									aborpBranchList.append(newAbsBranchLinker);

									CConcept* joiningConcept = newAbsBranchLinker->getBackPropagationBranchTriggerConcept();
									addConceptOperand(activationTriggerConcept,joiningConcept,false);

								}
							}
						}
					}

					CNominalSchemaAbsorptionBranchLinker* nextAbLinker = aborpBranchList.takeFirst();
					while (!aborpBranchList.isEmpty()) {
						CNominalSchemaAbsorptionBranchLinker* abLinker = aborpBranchList.takeFirst();

						CConcept* branchTrigger1 = nextAbLinker->getLastBranchTriggerConcept();
						CConcept* branchTrigger2 = abLinker->getLastBranchTriggerConcept();

						CConcept* newImpliedBranchTrigger = createVariableBindingPropagationTriggerConcept();

						CConcept* joiningConcept = createVariableBindingPropagationJoiningConcept(branchTrigger1,branchTrigger2,newImpliedBranchTrigger);
						addConceptOperand(activationTriggerConcept,joiningConcept,false);

						CNominalSchemaAbsorptionBranchLinker* newTmpAbLinker = new CNominalSchemaAbsorptionBranchLinker();
						newTmpAbLinker->joinNominalSchemaAbsorptionPathLinker(nextAbLinker,abLinker);
						newTmpAbLinker->setLastBranchTriggerConcept(newImpliedBranchTrigger);
						newTmpAbLinker->setBackPropagationBranchTriggerConcept(joiningConcept);
						createJoiningConceptVariableList(joiningConcept,nextAbLinker,abLinker);

						TAbsorptionBranchPair absBranchPair(qMin(nextAbLinker,abLinker),qMax(nextAbLinker,abLinker));
						mAbsorptionBranchPairAbsorptionBranchHash.insert(absBranchPair,newTmpAbLinker);

						nextAbLinker = newTmpAbLinker;
					}


					if (triggerConcept) {
						CConcept* nextVariableBindTrigger = createVariableBindingPropagationTriggerConcept();
						CConcept* varImplConcept = createVariableBindingPropagationImplicationTriggeredConcept(triggerConcept,nextVariableBindTrigger);
						CNominalSchemaAbsorptionBranchLinker* newTmpAbLinker = new CNominalSchemaAbsorptionBranchLinker();
						newTmpAbLinker->initNominalSchemaAbsorptionPathLinker(nextAbLinker);
						addConceptOperand(nextAbLinker->getLastBranchTriggerConcept(),varImplConcept,false);
						newTmpAbLinker->setLastBranchTriggerConcept(nextVariableBindTrigger);
						newTmpAbLinker->setBackPropagationBranchTriggerConcept(activationTriggerConcept);
						nextAbLinker = newTmpAbLinker;
					}

					newAbLinker = nextAbLinker->append(newAbLinker);



					QList< CNominalSchemaAbsorptionBranchLinker* >::iterator nextChangeIt = newBranchConItList.begin();
					QList< CNominalSchemaAbsorptionBranchLinker* >::iterator nextOriginalIt = orNewBranchConItList.begin();
					CNominalSchemaAbsorptionBranchLinker* nomSchAbsBranchLinkerIt = *nextChangeIt;

					nomSchAbsBranchLinkerIt = nomSchAbsBranchLinkerIt->getNext();
					bool continueIteration = true;

					while (!nomSchAbsBranchLinkerIt && !iterationFinished) {
						continueIteration = false;

						*nextChangeIt = *nextOriginalIt;

						++nextChangeIt;
						++nextOriginalIt;

						if (nextChangeIt == nextChangeEndIt) {
							iterationFinished = true;
						} else {
							nomSchAbsBranchLinkerIt = *nextChangeIt;
							nomSchAbsBranchLinkerIt = nomSchAbsBranchLinkerIt->getNext();
							if (!nomSchAbsBranchLinkerIt) {
								continueIteration = true;
							}
						}
					}
					if (!iterationFinished) {
						*nextChangeIt = nomSchAbsBranchLinkerIt;
					}

				}


				return newAbLinker;
			}




			CNominalSchemaAbsorptionBranchLinker* CTriggeredImplicationBinaryAbsorberPreProcess::getAbsorbedNominalSchemaBranchLinker(CConcept* absorbingConcept, bool absorbingNegation, CConcept* backPropConTrigger, QHash<CConcept*,CVariable*>* nomSchConceptVariableHash, CNominalSchemaTemplate* nsTemplate) {
				CNominalSchemaAbsorptionBranchLinker* newBranchLinker = nullptr;

				TConceptNegationPair absConNegPair(absorbingConcept,absorbingNegation);
				newBranchLinker = mConceptNomSchAbsorpBranchLinkerHash.value(absConNegPair);

				if (!newBranchLinker) {
					cint64 absConOpCode = absorbingConcept->getOperatorCode();
					cint64 param = absorbingConcept->getParameter();

					if (!absorbingNegation && (absConOpCode == CCSOME || absConOpCode == CCATLEAST && param == 1) || absorbingNegation && (absConOpCode == CCALL || absConOpCode == CCATMOST && param == 0)) {
						CRole* role = absorbingConcept->getRole();
						bool succOpNegate = absConOpCode == CCALL;

						if (absorbingConcept->getOperandList()) {

							CConcept* nextAbsorptionBranchTriggerConcept = createBindingBackPropagationTriggerConcept();
							CConcept* propagationPathTriggerConcept = createBindingBackPropagationToSuccessorConcept(nextAbsorptionBranchTriggerConcept,role);
							addConceptOperand(backPropConTrigger,propagationPathTriggerConcept,false);


							QSet<TConceptNegationPair> operandConceptSet;
							for (CSortedNegLinker<CConcept*>* succOpConLinkerIt = absorbingConcept->getOperandList(); succOpConLinkerIt; succOpConLinkerIt = succOpConLinkerIt->getNext()) {
								CConcept* succOpCon = succOpConLinkerIt->getData();
								bool succOpConNeg = succOpConLinkerIt->isNegated() ^ succOpNegate;
								operandConceptSet.insert(TConceptNegationPair(succOpCon,succOpConNeg));
							}

							CNominalSchemaAbsorptionBranchLinker* nextAbsorptionBranchLinker = getAbsorbedJoinedNominalSchemaBranchLinker(operandConceptSet,nextAbsorptionBranchTriggerConcept,nomSchConceptVariableHash,nsTemplate);

							for (CNominalSchemaAbsorptionBranchLinker* nextAbsorptionBranchLinkerIt = nextAbsorptionBranchLinker; nextAbsorptionBranchLinkerIt; nextAbsorptionBranchLinkerIt = nextAbsorptionBranchLinkerIt->getNext()) {

								CNominalSchemaAbsorptionBranchLinker* tmpNewAbsBranchLinker = new CNominalSchemaAbsorptionBranchLinker();
								tmpNewAbsBranchLinker->initNominalSchemaAbsorptionPathLinker(nextAbsorptionBranchLinkerIt);

								tmpNewAbsBranchLinker->setBackPropagationBranchTriggerConcept(propagationPathTriggerConcept);

								CConcept* nextVariableBindPropTriggerConcept = createVariableBindingPropagationTriggerConcept();
								CConcept* propagationVariableBindPropConcept = createVariableBindingPropagationFromSuccessorConcept(nextVariableBindPropTriggerConcept,role);
								CConcept* lastVariableBindPropTriggerConcept = tmpNewAbsBranchLinker->getLastBranchTriggerConcept();
								addConceptOperand(lastVariableBindPropTriggerConcept,propagationVariableBindPropConcept,false);
								tmpNewAbsBranchLinker->setLastBranchTriggerConcept(nextVariableBindPropTriggerConcept);

								newBranchLinker = tmpNewAbsBranchLinker->append(newBranchLinker);
							}
						}

					} else if (!absorbingNegation && (absConOpCode == CCOR) || absorbingNegation && (absConOpCode == CCAND)) {

						for (CSortedNegLinker<CConcept*>* opConLinkerIt = absorbingConcept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
							CConcept* opCon = opConLinkerIt->getData();
							bool opConNeg = opConLinkerIt->isNegated() ^ absorbingNegation;

							CNominalSchemaAbsorptionBranchLinker* tmpNewAbsBranchLinker = getAbsorbedJoinedNominalSchemaBranchLinker(opCon,opConNeg,backPropConTrigger,nomSchConceptVariableHash,nsTemplate);
							newBranchLinker = tmpNewAbsBranchLinker->append(newBranchLinker);
						}

					} else if (!absorbingNegation && (absConOpCode == CCNOMVAR)) {
						CVariable* variable = nomSchConceptVariableHash->value(absorbingConcept,nullptr);
						if (!variable) {
							variable = CObjectAllocator< CVariable >::allocateAndConstruct(mMemMan);
							variable->initVariable(absorbingConcept,mNextVariableID++);
							nomSchConceptVariableHash->insert(absorbingConcept,variable);
						}

						CConcept* varBindTriggerConcept = createVariableBindingPropagationTriggerConcept();						
						CConcept* bindVarConcept = createVariableBindingPropagationBindVariableConcept(varBindTriggerConcept,variable);
						CNominalSchemaAbsorptionBranchLinker* newAbsBranchLinker = new CNominalSchemaAbsorptionBranchLinker();
						newAbsBranchLinker->initNominalSchemaAbsorptionPathLinker(nullptr);
						newAbsBranchLinker->setLastBranchTriggerConcept(varBindTriggerConcept);
						newAbsBranchLinker->setBackPropagationBranchTriggerConcept(bindVarConcept);
						newAbsBranchLinker->addVariable(variable);
						if (!mConfNominalSchemaBackPropagation) {
							CConcept* nominalTriggerConcept = mTBox->getIndividualTriggerConcept();
							CConcept* tmpNextAbsorptionConcept = createBindingBackPropagationActivationTriggerConcept();
							addConceptOperand(nominalTriggerConcept,tmpNextAbsorptionConcept,false);
							addConceptOperand(tmpNextAbsorptionConcept,bindVarConcept,false);
						} else {
							addConceptOperand(backPropConTrigger,bindVarConcept,false);
						}

						newBranchLinker = newAbsBranchLinker;						
					}

					mConceptNomSchAbsorpBranchLinkerHash.insert(absConNegPair,newBranchLinker);
				} else {
					for (CNominalSchemaAbsorptionBranchLinker* nextAbsorptionBranchLinkerIt = newBranchLinker; nextAbsorptionBranchLinkerIt; nextAbsorptionBranchLinkerIt = nextAbsorptionBranchLinkerIt->getNext()) {
						addConceptOperand(backPropConTrigger,nextAbsorptionBranchLinkerIt->getBackPropagationBranchTriggerConcept(),false);
					}
				}

				return newBranchLinker;
			}















			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createVariableBindingPropagationBindVariableConcept(CConcept* followingConcept, CVariable* variable) {
				CConcept* bindConcept = createNewConcept();
				bindConcept->setOperatorCode(CCVARBINDVARIABLE);
				bindConcept->setVariableLinker(CObjectAllocator< CSortedLinker<CVariable*> >::allocateAndConstruct(mMemMan)->init(variable));

				addConceptOperand(bindConcept,followingConcept,false);

				return bindConcept;
			}



			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createBindingBackPropagationActivationTriggerConcept() {
				CConcept* triggConcept = createNewConcept();
				triggConcept->setOperatorCode(CCBACKACTIVTRIG);
				return triggConcept;
			}

			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createBindingBackPropagationTriggerConcept() {
				CConcept* triggConcept = createNewConcept();
				triggConcept->setOperatorCode(CCVARPBACKTRIG);
				return triggConcept;
			}


			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createBindingBackPropagationToSuccessorConcept(CConcept* followingConcept, CRole* role) {
				CConcept* succPropConcept = createNewConcept();
				succPropConcept->setOperatorCode(CCVARPBACKALL);
				succPropConcept->setRole(role);
				addConceptOperand(succPropConcept,followingConcept,false);
				return succPropConcept;
			}

			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createVariableBindingPropagationFromSuccessorConcept(CConcept* followingConcept, CRole* role) {
				CRole* inverseRole = getInverseRole(role);
				CConcept* succPropConcept = createNewConcept();
				succPropConcept->setOperatorCode(CCVARBINDALL);
				succPropConcept->setRole(inverseRole);
				addConceptOperand(succPropConcept,followingConcept,false);
				return succPropConcept;
			}



			bool CTriggeredImplicationBinaryAbsorberPreProcess::addBindingBackPropagationTriggeredToImplicationConcept(CConcept* implicationConcept, CConcept* triggeredConcept) {
				CSortedNegLinker<CConcept*>* lastOpConLinker = nullptr;
				for (CSortedNegLinker<CConcept*>* opConLinker = implicationConcept->getOperandList(); opConLinker; opConLinker = opConLinker->getNext()) {
					lastOpConLinker = opConLinker;
					if (lastOpConLinker->getData() == triggeredConcept) {
						return false;
					}
				}
				CSortedNegLinker<CConcept*>* newBaseOpCon1 = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon1->init(triggeredConcept,true);
				lastOpConLinker->setNext(newBaseOpCon1);
				implicationConcept->incOperandCount();
				return true;
			}



			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createBindingBackPropagationImplicationTriggeredConcept(CConcept* triggeredConcept, CConcept* impliedConcept) {
				CConcept* implicationConcept = createNewConcept();
				implicationConcept->setOperatorCode(CCBACKACTIVIMPL);

				CSortedNegLinker<CConcept*>* newBaseOpCon1 = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon1->init(triggeredConcept,true);
				CSortedNegLinker<CConcept*>* newBaseOpCon2 = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon2->init(impliedConcept,false,newBaseOpCon1);
				implicationConcept->setOperandList(newBaseOpCon2);
				implicationConcept->incOperandCount(2);

				return implicationConcept;
			}


			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createVariableBindingPropagationImplicationTriggeredConcept(CConcept* triggeredConcept, CConcept* impliedConcept) {
				CConcept* implicationConcept = createNewConcept();
				implicationConcept->setOperatorCode(CCVARBINDIMPL);

				CSortedNegLinker<CConcept*>* newBaseOpCon1 = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon1->init(triggeredConcept,true);
				CSortedNegLinker<CConcept*>* newBaseOpCon2 = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon2->init(impliedConcept,false,newBaseOpCon1);
				implicationConcept->setOperandList(newBaseOpCon2);
				implicationConcept->incOperandCount(2);

				return implicationConcept;
			}


			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createVariableBindingPropagationTriggerConcept() {
				CConcept* triggConcept = createNewConcept();
				triggConcept->setOperatorCode(CCVARBINDTRIG);
				return triggConcept;
			}




			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createVariableBindingPropagationJoiningConcept(CConcept* joinConcept1, CConcept* joinConcept2, CConcept* impliedConcept) {
				CConcept* joiningConcept = createNewConcept();
				joiningConcept->setOperatorCode(CCVARBINDJOIN);

				CSortedNegLinker<CConcept*>* newBaseOpCon1 = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon1->init(joinConcept2,true);
				CSortedNegLinker<CConcept*>* newBaseOpCon2 = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon2->init(joinConcept1,true,newBaseOpCon1);
				CSortedNegLinker<CConcept*>* newBaseOpCon3 = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon3->init(impliedConcept,false,newBaseOpCon2);
				joiningConcept->setOperandList(newBaseOpCon3);
				joiningConcept->incOperandCount(3);

				return joiningConcept;
			}


			void CTriggeredImplicationBinaryAbsorberPreProcess::createJoiningConceptVariableList(CConcept* joiningConcept, CNominalSchemaAbsorptionBranchLinker* abLinker1, CNominalSchemaAbsorptionBranchLinker* abLinker2) {
				QSet<CVariable*>* varSet1 = abLinker1->getVariableSet();
				QSet<CVariable*>* varSet2 = abLinker2->getVariableSet();
				CSortedLinker<CVariable*>* varLinker = nullptr;
				for (QSet<CVariable*>::const_iterator it = varSet1->constBegin(), itEnd = varSet1->constEnd(); it != itEnd; ++it) {
					if (varSet2->contains(*it)) {
						CVariable* variable = *it;
						CSortedLinker<CVariable*>* tmpVarLinker = CObjectAllocator< CSortedLinker<CVariable*> >::allocateAndConstruct(mMemMan);
						tmpVarLinker->init(variable);
						if (varLinker) {
							varLinker = varLinker->insertNextSorted(tmpVarLinker);
						} else {
							varLinker = tmpVarLinker;
						}
					}
				}
				joiningConcept->setVariableLinker(varLinker);
			}










			void CTriggeredImplicationBinaryAbsorberPreProcess::createGCIPartialAbsorbedTriggeredImplication(CConcept* orConcept, bool negated, const QList<TConceptNegationPair>& absorbList) {
				//CConceptRoleBranchingTrigger* branchTriggers = nullptr;
				//for (QList<TConceptNegationPair>::const_iterator it = absorbList.constBegin(), itEnd = absorbList.constEnd(); it != itEnd; ++it) {
				//	TConceptNegationPair conNegPair(*it);
				//	CConcept* concept = conNegPair.first;
				//	bool negated = conNegPair.second;
				//	CConceptRoleBranchingTrigger* tmpBranchTrigger = getSimpleBranchTriggersForConcept(concept,!negated);
				//	if (tmpBranchTrigger) {
				//		branchTriggers = tmpBranchTrigger->append(branchTriggers);
				//	} else {
				//		CConceptRoleBranchingTrigger* tmpBranchTrigger = getBranchTiggers(getPartialTriggersForConcept(concept,!negated));
				//		branchTriggers = tmpBranchTrigger->append(branchTriggers);
				//	}
				//}
				CConceptTriggerLinker* partialImpliedTrigger = createPartialExtendedAbsorbedImpliedTrigger(absorbList);
				CConcept* partialImpliedTriggerConcept = partialImpliedTrigger->getTriggerConcept();
				addUnfoldingConceptForConcept(partialImpliedTriggerConcept,orConcept,negated);

				CConceptRoleBranchingTrigger* branchTriggers = getSimpleBranchTriggersForConcept(partialImpliedTriggerConcept,true);
				mBranchTriggVec->setData(orConcept->getConceptTag(),branchTriggers);
			}



			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createDisjunctionAbsorbedTriggeredImplication(CConcept* orConcept, bool negated, QList<TConceptNegationPair>& absorbList, QList<TConceptNegationPair>& candList) {
				CConcept* impliedConcept = nullptr;
				CConcept* implicationConcept = nullptr;
				bool impliedNegation = false; 

				if (candList.isEmpty()) {
					candList.append(absorbList.takeFirst());
				}

				CConceptTriggerLinker* triggers = nullptr;
				for (QList<TConceptNegationPair>::const_iterator it = absorbList.constBegin(), itEnd = absorbList.constEnd(); it != itEnd; ++it) {
					TConceptNegationPair conNegPair(*it);
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					CConceptTriggerLinker* tmpTriggers = copyTriggerLinkers(getTriggersForConcept(concept,!negated));
					triggers = tmpTriggers->append(triggers);
				}
				CConceptTriggerLinker* impliedConceptTrigger = getImplicationTriggeredConceptForTriggers(triggers,&implicationConcept);
				impliedConcept = impliedConceptTrigger->getTriggerConcept();
				if (candList.count() > 1) {
					CConcept* orImpliedConcept = createImpliedConcept();
					for (QList<TConceptNegationPair>::const_iterator it = candList.constBegin(), itEnd = candList.constEnd(); it != itEnd; ++it) {
						TConceptNegationPair conNegPair(*it);
						CConcept* concept = conNegPair.first;
						bool negated = conNegPair.second;
						addConceptToImplied(orImpliedConcept,concept,negated);
					}
					orImpliedConcept->setOperatorCode(CCOR);
					addUnfoldingConceptForConcept(impliedConcept,orImpliedConcept,false);

					if (mConfPartialDisjunctionsAbsorption) {
						QList<TConceptNegationPair> partialAbsorbableList;
						partialAbsorbableList = getPartialAbsorbableDisjuncts(candList);
						if (!partialAbsorbableList.isEmpty()) {
							CConcept* partialImpliedTriggerConcept = createPartialExtendedAbsorbedImpliedTriggerCocnept(partialAbsorbableList);
							CConceptRoleBranchingTrigger* branchTriggers = getSimpleBranchTriggersForConcept(partialImpliedTriggerConcept,true);
							mBranchTriggVec->setData(impliedConcept->getConceptTag(),branchTriggers);
							++mStatGCIAbsorptionPartialExtended;
						} 
					}
				} else {
					TConceptNegationPair conNegPair(candList.first());
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					addUnfoldingConceptForConcept(impliedConcept,concept,negated);
				}

				return implicationConcept;
			}





			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::getRoleDomainTriggerConcept(CRole* role) {
				if (!mRoleDomainTriggerConceptHash) {
					mRoleDomainTriggerConceptHash = mTBox->getRoleDomainTriggerConceptHash(true);
				}
				CConcept*& triggerConcept = (*mRoleDomainTriggerConceptHash)[role];
				if (!triggerConcept) {
					triggerConcept = createTriggerConcept();
					addRoleDomainConcept(role,triggerConcept,false);
				}
				return triggerConcept;
			}







			CConceptTriggerLinker* CTriggeredImplicationBinaryAbsorberPreProcess::getTriggersForConcept(CConcept* concept, bool negated, CConcept* backPropConcept) {
				CConceptTriggerLinker* triggers = nullptr;

				CConcept* backPropImpConcept = nullptr;
				CConcept* backPropTriggerConcept = nullptr;
				CConcept** backPropImpConceptPointer = nullptr;
				bool branchTiggerCreation = false;
				if (backPropConcept) {
					branchTiggerCreation = true;
					backPropImpConceptPointer = &backPropImpConcept;
				}
				TConceptNegationPair conNegPair(concept,negated);
				triggers = mConceptTriggerLinkerHash.value(conNegPair);
				if (triggers && backPropConcept) {
					CConcept* backPropActivCon = mBackPropActivationConHash.value(conNegPair);
					if (backPropActivCon) {
						addUnfoldingConceptForConcept(backPropConcept, backPropActivCon, false);
					}
				}
				if (!triggers) {
					if (backPropConcept) {
						backPropTriggerConcept = createTriggerConcept(branchTiggerCreation);
						addUnfoldingConceptForConcept(backPropConcept, backPropTriggerConcept, false);
					}
					cint64 opCode = concept->getOperatorCode();
					cint64 parm = concept->getParameter();
					cint64 opCount = concept->getOperandCount();
					CRole* role = concept->getRole();
					CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();

					if (!negated && (opCode == CCATOM || opCode == CCSUB || opCode == CCTOP || opCode == CCIMPLTRIG)) {
						CConceptTriggerLinker* newTrigger = createTriggerLinker();
						newTrigger->initConceptTriggerLinker(concept, 1);
						if (backPropConcept) {
							CConceptTriggerLinker* implTriggeredConcept = getImplicationTriggeredConceptForTriggers(newTrigger, backPropImpConceptPointer);
							addUnfoldingConceptForConcept(backPropTriggerConcept, backPropImpConcept, false);
							newTrigger = implTriggeredConcept;
						}
						triggers = newTrigger->append(triggers);


					} else if (negated && (opCode == CCOR) || !negated && (opCode == CCAND || opCode == CCEQ)) {
						// AND, collect triggers from operands
						CConceptTriggerLinker* andTriggers = nullptr;
						for (CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker; opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegated = opConLinkerIt->isNegated()^negated;

							CConceptTriggerLinker* newTriggers = copyTriggerLinkers(getTriggersForConcept(opConcept,opNegated, backPropTriggerConcept));
							andTriggers = newTriggers->append(andTriggers);
						}
						if (backPropConcept) {
							CConceptTriggerLinker* implTriggeredConcept = getImplicationTriggeredConceptForTriggers(andTriggers, &backPropImpConcept);
							addUnfoldingConceptForConcept(backPropTriggerConcept, backPropImpConcept, false);
							andTriggers = implTriggeredConcept;
						}
						triggers = andTriggers->append(triggers);

					} else if (opCode == CCAQCHOOCE) {
						CConceptTriggerLinker* andTriggers = nullptr;
						for (CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker; opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegated = opConLinkerIt->isNegated() ^ negated;

							if (opNegated == negated) {
								CConceptTriggerLinker* newTriggers = copyTriggerLinkers(getTriggersForConcept(opConcept, opNegated, backPropTriggerConcept));
								andTriggers = newTriggers->append(andTriggers);
							}
						}
						if (backPropConcept) {
							CConceptTriggerLinker* implTriggeredConcept = getImplicationTriggeredConceptForTriggers(andTriggers, &backPropImpConcept);
							addUnfoldingConceptForConcept(backPropTriggerConcept, backPropImpConcept, false);
							andTriggers = implTriggeredConcept;
						}
						triggers = andTriggers->append(triggers);


					} else if (!negated && (opCode == CCOR) || negated && (opCode == CCAND || opCode == CCEQ)) {
						// or trigger
						CConceptTriggerLinker* orTriggers = nullptr;
						for (CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker; opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegated = opConLinkerIt->isNegated()^negated;

							CConceptTriggerLinker* newTriggers = copyTriggerLinkers(getTriggersForConcept(opConcept,opNegated, backPropTriggerConcept));
							if (newTriggers->hasNext()) {
								CConceptTriggerLinker* implTriggeredConcept = getImplicationTriggeredConceptForTriggers(newTriggers, backPropImpConceptPointer);
								if (backPropImpConceptPointer) {
									addUnfoldingConceptForConcept(backPropTriggerConcept, backPropImpConcept, false);
								}
								orTriggers = implTriggeredConcept->append(orTriggers);
							} else {
								orTriggers = newTriggers->append(orTriggers);
							}
						}
						if (orTriggers) {
							if (orTriggers->hasNext()) {
								// more than one trigger
								cint64 triggerComplexity = 0;
								cint64 triggerCount = 0;
								CConcept* triggerConcept = createTriggerConcept(branchTiggerCreation);
								for (CConceptTriggerLinker* orTriggerIt = orTriggers; orTriggerIt; orTriggerIt = orTriggerIt->getNext()) {
									triggerComplexity += orTriggerIt->getTriggerComplexity();
									triggerCount++;
									addUnfoldingConceptForConcept(orTriggerIt->getTriggerConcept(),triggerConcept,false);
								}
								CConceptTriggerLinker* newTrigger = createTriggerLinker();
								newTrigger->initConceptTriggerLinker(triggerConcept,(triggerComplexity+1)/triggerCount);
								triggers = newTrigger->append(triggers);
								//releaseTriggerLinkers(orTriggers);
							} else {
								// only one trigger, add simply to other triggers
								triggers = orTriggers->append(triggers);
							}
						}


					} else if (!negated && (opCode == CCSOME || opCode == CCSELF || opCode == CCAQSOME) || negated && (opCode == CCALL) ||
								!negated && (opCode == CCATLEAST && parm == 1) || negated && (opCode == CCATMOST && parm == 0)) {
						CConcept* propagatedTriggerConcept = nullptr;
						cint64 triggerComplexity = 1;
						bool negateOperands = negated && (opCode == CCSOME || opCode == CCSELF || opCode == CCALL);
						CConceptTriggerLinker* allTriggers = nullptr;
						CConcept* roleBackPropTriggerConcept = nullptr;
						if (backPropConcept) {
							roleBackPropTriggerConcept = createTriggerConcept(branchTiggerCreation);
							CConcept* roleBackPropConcept = createTriggerPropagationConcept(roleBackPropTriggerConcept, role, branchTiggerCreation, false);
							addUnfoldingConceptForConcept(backPropTriggerConcept, roleBackPropConcept, false);
						}
						for (CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker; opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegated = opConLinkerIt->isNegated()^negateOperands;
							if (!(!opNegated && opConcept == mTopConcept || opNegated && opConcept == mBottomConcept)) {
								CConceptTriggerLinker* newTrigger = copyTriggerLinkers(getTriggersForConcept(opConcept,opNegated, roleBackPropTriggerConcept));
								allTriggers = newTrigger->append(allTriggers);
							}
						}
						if (allTriggers) {
							propagatedTriggerConcept = createTriggerConcept(branchTiggerCreation);
							CConcept* nextLevelTriggerConcept = createTriggerPropagationConcept(propagatedTriggerConcept,role, branchTiggerCreation);
							if (allTriggers->hasNext()) {
								// requires a generation of an implication concept
								CConceptTriggerLinker* implTriggeredConcept = getImplicationTriggeredConceptForTriggers(allTriggers, backPropImpConceptPointer);
								if (backPropImpConceptPointer) {
									addUnfoldingConceptForConcept(roleBackPropTriggerConcept, backPropImpConcept, false);
								}
								triggerComplexity += implTriggeredConcept->getTriggerComplexity();
								addUnfoldingConceptForConcept(implTriggeredConcept->getTriggerConcept(),nextLevelTriggerConcept,false);
							} else {
								triggerComplexity += allTriggers->getTriggerComplexity();
								addUnfoldingConceptForConcept(allTriggers->getTriggerConcept(),nextLevelTriggerConcept,false);
								releaseTriggerLinkers(allTriggers);
							}
						} else {
							if (backPropConcept) {
								propagatedTriggerConcept = createTriggerConcept(branchTiggerCreation);
								CConcept* nextLevelTriggerConcept = createTriggerPropagationConcept(propagatedTriggerConcept, role, branchTiggerCreation);
								addUnfoldingConceptForConcept(roleBackPropTriggerConcept, nextLevelTriggerConcept, false);
							}
							else {
								CRole* locatedRole = CConceptRoleIndividualLocator::getLocatedRole(role, mOnto);
								propagatedTriggerConcept = getRoleDomainTriggerConcept(locatedRole);
							}
						}
						CConceptTriggerLinker* newTrigger = createTriggerLinker();
						newTrigger->initConceptTriggerLinker(propagatedTriggerConcept,triggerComplexity);
						triggers = newTrigger->append(triggers);


					} else if (!negated && (opCode == CCVALUE)) {
						CConcept* propagatedTriggerConcept = createTriggerConcept(branchTiggerCreation);
						CConcept* nextLevelTriggerConcept = createTriggerPropagationConcept(propagatedTriggerConcept,role, branchTiggerCreation);

						cint64 triggerComplexity = 2;

						CIndividual* individual = concept->getNominalIndividual();

						if (!backPropConcept) {
							CIndividual* locatedIndividual = CConceptRoleIndividualLocator::getLocatedIndividual(individual, mOnto);
							CConceptAssertionLinker* assLinker = CObjectAllocator< CConceptAssertionLinker >::allocateAndConstruct(mMemMan);
							assLinker->initNegLinker(nextLevelTriggerConcept, false);
							locatedIndividual->addAssertionConceptLinker(assLinker);

							if (!mIndividualTriggerConceptHash) {
								mIndividualTriggerConceptHash = mTBox->getIndividualTriggerConceptHash(true);
							}
							mIndividualTriggerConceptHash->insertMulti(individual->getIndividualID(), nextLevelTriggerConcept);
						} else {
							CConcept* roleBackPropTriggerConcept = createTriggerConcept(branchTiggerCreation);
							CConcept* roleBackPropConcept = createTriggerPropagationConcept(roleBackPropTriggerConcept, role, branchTiggerCreation, false);
							addUnfoldingConceptForConcept(backPropTriggerConcept, roleBackPropConcept, false);

							CConcept* nomImpConcept = createNominalImplication(nextLevelTriggerConcept, individual);
							addUnfoldingConceptForConcept(roleBackPropTriggerConcept, nomImpConcept, false);

						}


						CConceptTriggerLinker* newTrigger = createTriggerLinker();
						newTrigger->initConceptTriggerLinker(propagatedTriggerConcept,triggerComplexity);
						triggers = newTrigger->append(triggers);


					} else if (!negated && (opCode == CCNOMVAR)) {
						cint64 triggerComplexity = 1;
						CConcept* nominalTriggerConcept = mTBox->getIndividualTriggerConcept();
						CConceptTriggerLinker* newTrigger = createTriggerLinker();
						newTrigger->initConceptTriggerLinker(nominalTriggerConcept,triggerComplexity);
						triggers = newTrigger->append(triggers);


					} else if (!negated && (opCode == CCNOMINAL)) {
						CConcept* nominalTriggerConcept = createTriggerConcept(branchTiggerCreation);
						cint64 triggerComplexity = 1;

						CIndividual* individual = concept->getNominalIndividual();
						if (!backPropConcept) {
							CIndividual* locatedIndividual = CConceptRoleIndividualLocator::getLocatedIndividual(individual, mOnto);
							CConceptAssertionLinker* assLinker = CObjectAllocator< CConceptAssertionLinker >::allocateAndConstruct(mMemMan);
							assLinker->initNegLinker(nominalTriggerConcept,false);
							locatedIndividual->addAssertionConceptLinker(assLinker);

							if (!mIndividualTriggerConceptHash) {
								mIndividualTriggerConceptHash = mTBox->getIndividualTriggerConceptHash(true);
							}
							mIndividualTriggerConceptHash->insertMulti(individual->getIndividualID(),nominalTriggerConcept);
						} else {
							CConcept* nomImpConcept = createNominalImplication(nominalTriggerConcept, individual);
							addUnfoldingConceptForConcept(backPropTriggerConcept, nomImpConcept, false);
						}

						CConceptTriggerLinker* newTrigger = createTriggerLinker();
						newTrigger->initConceptTriggerLinker(nominalTriggerConcept,triggerComplexity);
						triggers = newTrigger->append(triggers);
					}


					mConceptTriggerLinkerHash.insert(conNegPair,triggers);
					if (backPropTriggerConcept) {
						mBackPropActivationConHash.insert(conNegPair, backPropTriggerConcept);
					}
				}
				return triggers;
			}


			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::getImplicationImpliedConcept(CConcept* implicationConcept) {
				CConcept* impliedConcept = nullptr;
				cint64 param = implicationConcept->getParameter();
				if (param >= 1) {
					CSortedNegLinker<CConcept*>* opConLinkerIt = implicationConcept->getOperandList();
					while (opConLinkerIt && param-- >= 0 && !impliedConcept) {
						if (!opConLinkerIt->isNegated()) {
							CConcept* concept = opConLinkerIt->getData();
							if (concept->getOperatorCode() == CCIMPLTRIG) {
								impliedConcept = concept;
							}
						}
						opConLinkerIt = opConLinkerIt->getNext();
					}
				}
				return impliedConcept;
			}

			bool CTriggeredImplicationBinaryAbsorberPreProcess::findAndReplaceImplicationFromTriggers(CConceptTriggerLinker* trigger, CConceptTriggerLinker*& otherTriggers) {
				if (!mConfImplicationsForTriggerReusue) {
					return false;
				}
				if (otherTriggers) {
					CConcept* concept1 = trigger->getTriggerConcept();
					CConceptTriggerLinker* triggerIt = otherTriggers;
					CConceptTriggerLinker* lastTriggerIt = nullptr;
					while (triggerIt) {
						CConcept* concept2 = triggerIt->getTriggerConcept();
						CConcept* minConcept1 = qMin(concept1,concept2);
						CConcept* maxConcept1 = qMax(concept1,concept2);
						TConceptPair conceptPair(minConcept1,maxConcept1);
						CConceptTriggerLinker* impliedConceptTrigger = mConceptImplicationImpliedHash.value(conceptPair);
						if (impliedConceptTrigger) {
							CConceptTriggerLinker* relTrigger = triggerIt;
							triggerIt = triggerIt->getNext();
							relTrigger->clearNext();
							*trigger = *impliedConceptTrigger;
							if (lastTriggerIt) {
								lastTriggerIt->setNext(triggerIt);
							} else {
								otherTriggers = triggerIt;
							}
							releaseTriggerLinkers(relTrigger);
							return true;
						} else {
							lastTriggerIt = triggerIt;
							triggerIt = triggerIt->getNext();
						}
					}
				}
				return false;
			}



			CConceptTriggerLinker* CTriggeredImplicationBinaryAbsorberPreProcess::getUpdatedTriggerComplexities(CConceptTriggerLinker* triggers) {
				if (mConfPunishOftenSameTriggerUse) {
					for (CConceptTriggerLinker* triggerIt = triggers; triggerIt; triggerIt = triggerIt->getNext()) {
						cint64 triggerOpCount = triggerIt->getTriggerConcept()->getOperandCount();
						if (triggerOpCount > mConfOftenTriggerUseCountPunishment) {
							cint64 triggerComplexityPunishment = triggerOpCount / mConfOftenTriggerUseCountPunishment;
							triggerIt->setTriggerComplexity(triggerIt->getTriggerComplexity()-triggerComplexityPunishment);
						}
					}
				}
				return triggers;
			}



			CConceptTriggerLinker* CTriggeredImplicationBinaryAbsorberPreProcess::getImplicationTriggeredConceptForTriggers(CConceptTriggerLinker* triggers, CConcept** firstImplicationConcept) {
				CConceptTriggerLinker* implicationTriggeredConcept = nullptr;
				CConceptTriggerLinker* sortedTriggers = nullptr;
				CConceptTriggerLinker* triggerIt = getUpdatedTriggerComplexities(triggers);
				while (triggerIt) {
					CConceptTriggerLinker* trigger = triggerIt;
					triggerIt = triggerIt->getNext();
					trigger->clearNext();

					if ((triggerIt || sortedTriggers) && trigger->getTriggerConcept() == mTopConcept) {
						// ignore trigger
					} else {
						bool replaced = true;
						bool checkSorted = false;
						while (replaced) {
							replaced = false;
							if (findAndReplaceImplicationFromTriggers(trigger,triggerIt)) {
								++mStatReusedImplications;
								replaced = true;
								checkSorted = true;
							} else {
								if (checkSorted) {
									if (findAndReplaceImplicationFromTriggers(trigger,sortedTriggers)) {
										++mStatReusedImplications;
										replaced = true;
									}
								}
							}
						}

						if (sortedTriggers) {
							sortedTriggers = sortedTriggers->insertNextSorted(trigger);
						} else {
							sortedTriggers = trigger;
						}
					}

				}
				if (firstImplicationConcept) {
					*firstImplicationConcept = nullptr;
					if (sortedTriggers) {
						CConceptTriggerLinker* trigger1 = sortedTriggers;

						CConcept* newImpliedTriggerConcept = createTriggerConcept(firstImplicationConcept != nullptr);
						CConcept* newImplicationConcept = createImplicationConcept(newImpliedTriggerConcept,false);
						addImplicationTrigger(newImplicationConcept,trigger1->getTriggerConcept(),true);
						trigger1->initConceptTriggerLinker(newImpliedTriggerConcept,trigger1->getTriggerComplexity());
						*firstImplicationConcept = newImplicationConcept;

						implicationTriggeredConcept = trigger1;

					}
				}

				if (sortedTriggers) {
					if (!sortedTriggers->hasNext()) {
						implicationTriggeredConcept = sortedTriggers;
					} else {
						CConceptTriggerLinker* trigger1 = sortedTriggers;
						sortedTriggers = sortedTriggers->getNext();
						triggerIt = sortedTriggers;
						trigger1->clearNext();
						while (triggerIt) {
							CConceptTriggerLinker* trigger2 = triggerIt;
							triggerIt = triggerIt->getNext();
							trigger2->clearNext();

							CConcept* newImpliedTriggerConcept = createTriggerConcept(firstImplicationConcept != nullptr);
							CConcept* newImplicationConcept = createImplicationConcept(newImpliedTriggerConcept,false);

							CConcept* concept1 = trigger1->getTriggerConcept();
							CConcept* concept2 = trigger2->getTriggerConcept();

							addImplicationTrigger(newImplicationConcept,concept2,true);
							addUnfoldingConceptForConcept(concept1,newImplicationConcept,false);
							cint64 newTriggerComplexity = trigger1->getTriggerComplexity()+trigger2->getTriggerComplexity();

							CConcept* minConcept1 = qMin(concept1,concept2);
							CConcept* maxConcept1 = qMax(concept1,concept2);
							TConceptPair conceptPair(minConcept1,maxConcept1);
							trigger2->initConceptTriggerLinker(newImpliedTriggerConcept,newTriggerComplexity);
							mConceptImplicationImpliedHash.insert(conceptPair,trigger2);

							trigger1->initConceptTriggerLinker(newImpliedTriggerConcept,newTriggerComplexity);
						}
						implicationTriggeredConcept = trigger1;
					}
					if (!mTriggerImplHash->contains(implicationTriggeredConcept->getTriggerConcept())) {
						mTriggerImplHash->insertMulti(implicationTriggeredConcept->getTriggerConcept(),nullptr);
					}
				}
				return implicationTriggeredConcept;
			}


			void CTriggeredImplicationBinaryAbsorberPreProcess::addUnfoldingConceptForConcept(CConcept* concept, CConcept* addingUnfoldingConcept, bool addingNegation) {
				cint64 opCode = concept->getOperatorCode();
				if (opCode == CCTOP) {
					CSortedNegLinker<CConcept*>* newOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
					newOpCon->init(addingUnfoldingConcept,addingNegation);
					mLaterTopConceptTriggerAddList.append(newOpCon);
				} else {
					CSortedNegLinker<CConcept*>* newOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
					newOpCon->init(addingUnfoldingConcept,addingNegation,concept->getOperandList());
					concept->setOperandList(newOpCon);
					concept->incOperandCount();
					if (opCode == CCATOM) {
						concept->setOperatorCode(CCSUB);
					}
				}
			}


			void CTriggeredImplicationBinaryAbsorberPreProcess::addUnfoldingConceptForImplicationConcept(CConcept* implicationConcept, CConcept* addingUnfoldingConcept, bool addingNegation) {
				CSortedNegLinker<CConcept*>* newOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newOpCon->init(addingUnfoldingConcept,addingNegation,implicationConcept->getOperandList());
				implicationConcept->setOperandList(newOpCon);
				implicationConcept->incOperandCount();
				implicationConcept->setParameter(implicationConcept->getParameter()+1);
			}



			CConceptTriggerLinker* CTriggeredImplicationBinaryAbsorberPreProcess::createTriggerLinker() {
				CConceptTriggerLinker* newTrigger = mTmpTriggerLinker;
				if (newTrigger) {
					mTmpTriggerLinker = mTmpTriggerLinker->getNext();
				} else {
					newTrigger = new CConceptTriggerLinker();
					mTriggerContainer.append(newTrigger);
				}
				newTrigger->clearNext();
				return newTrigger;
			}

			void CTriggeredImplicationBinaryAbsorberPreProcess::releaseTriggerLinkers(CConceptTriggerLinker* triggers) {
				if (triggers) {
					mTmpTriggerLinker = triggers->append(mTmpTriggerLinker);
				}
			}


			CConceptTriggerLinker* CTriggeredImplicationBinaryAbsorberPreProcess::copyTriggerLinkers(CConceptTriggerLinker* triggers) {
				CConceptTriggerLinker* newTriggers = nullptr;
				for (CConceptTriggerLinker* triggerIt = triggers; triggerIt; triggerIt = triggerIt->getNext()) {
					CConceptTriggerLinker* newTrigger = createTriggerLinker()->initConceptTriggerLinker(triggerIt->getTriggerConcept(),triggerIt->getTriggerComplexity());
					newTriggers = newTrigger->append(newTriggers);
				}
				return newTriggers;
			}


			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createMarkerConcept() {
				CConcept* markerConcept = createNewConcept();
				markerConcept->setOperatorCode(CCMARKER);
				return markerConcept;
			}


			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createTriggerConcept(bool branchTrigger) {
				CConcept* triggerConcept = createNewConcept();
				if (branchTrigger) {
					triggerConcept->setOperatorCode(CCBRANCHTRIG);
				} else {
					triggerConcept->setOperatorCode(CCIMPLTRIG);
				}
				return triggerConcept;
			}



			bool CTriggeredImplicationBinaryAbsorberPreProcess::isDisjunctionConceptSimpleTriggeredImplicationAbsorbable(CConcept* orConcept, bool negated) {
				cint64 notAbsorbableCount = 0;
				cint64 absorbableCount = 0;
				CSortedNegLinker<CConcept*>* opConLinkerIt = orConcept->getOperandList();
				while (opConLinkerIt) {
					CConcept* opConcept = opConLinkerIt->getData();
					bool opNegated = opConLinkerIt->isNegated();
					if (isConceptSimpleImplicationTriggerable(opConcept,!opNegated^negated)) {
						absorbableCount++;
					} else {
						notAbsorbableCount++;
					}
					opConLinkerIt = opConLinkerIt->getNext();
				}
				if (notAbsorbableCount <= 1) {
					return true;
				}
				return false;
			}




			bool CTriggeredImplicationBinaryAbsorberPreProcess::isDisjunctionConceptTriggeredImplicationAbsorbable(CConcept* orConcept, bool negated) {
				cint64 notAbsorbableCount = 0;
				cint64 absorbableCount = 0;
				QHash<CConcept*,TOccuredAbsorbablePair> conceptEqConAbsorbed;
				CSortedNegLinker<CConcept*>* opConLinkerIt = orConcept->getOperandList();
				while (opConLinkerIt) {
					CConcept* opConcept = opConLinkerIt->getData();
					bool opNegated = opConLinkerIt->isNegated();
					if (isConceptImplicationTriggerable(opConcept,!opNegated^negated,&conceptEqConAbsorbed)) {
						absorbableCount++;
					} else {
						notAbsorbableCount++;
					}
					opConLinkerIt = opConLinkerIt->getNext();
				}
				if (notAbsorbableCount <= 1) {
					return true;
				}
				return false;
			}

			bool CTriggeredImplicationBinaryAbsorberPreProcess::isDisjunctionConceptPartialTriggeredImplicationAbsorbable(CConcept* orConcept, bool negated) {
				CSortedNegLinker<CConcept*>* opConLinkerIt = orConcept->getOperandList();
				QSet<CConcept*> conceptEqConAbsorbed;
				bool partialTriggerable = false;
				while (opConLinkerIt) {
					CConcept* opConcept = opConLinkerIt->getData();
					bool opNegated = opConLinkerIt->isNegated();
					if (isConceptPartialImplicationTriggerable(opConcept,!opNegated^negated,&conceptEqConAbsorbed)) {
						partialTriggerable = true;
					}
					opConLinkerIt = opConLinkerIt->getNext();
				}
				return partialTriggerable;
			}








			bool CTriggeredImplicationBinaryAbsorberPreProcess::isCardinalityConceptQualificationTriggerAbsorbable(CConcept* cardConcept, bool negated) {
				bool triggerable = false;
				QHash<CConcept*,TOccuredAbsorbablePair> conceptEqConAbsorbed;
				CSortedNegLinker<CConcept*>* opConLinkerIt = cardConcept->getOperandList();
				while (opConLinkerIt) {
					CConcept* opConcept = opConLinkerIt->getData();
					bool opNegated = opConLinkerIt->isNegated();
					if (isConceptImplicationTriggerable(opConcept,opNegated,&conceptEqConAbsorbed)) {
						triggerable = true;
					}
					opConLinkerIt = opConLinkerIt->getNext();
				}
				return triggerable;
			}

			bool CTriggeredImplicationBinaryAbsorberPreProcess::isCardinalityConceptPartialQualificationTriggerAbsorbable(CConcept* cardConcept, bool negated) {
				CSortedNegLinker<CConcept*>* opConLinkerIt = cardConcept->getOperandList();
				QSet<CConcept*> conceptEqConAbsorbed;
				bool partialTriggerable = false;
				while (opConLinkerIt) {
					CConcept* opConcept = opConLinkerIt->getData();
					bool opNegated = opConLinkerIt->isNegated();
					if (isConceptPartialImplicationTriggerable(opConcept,opNegated,&conceptEqConAbsorbed)) {
						partialTriggerable = true;
					}
					opConLinkerIt = opConLinkerIt->getNext();
				}
				return partialTriggerable;
			}







			bool CTriggeredImplicationBinaryAbsorberPreProcess::isEquivalenceConceptTriggeredImplicationAbsorbable(CConcept* orConcept, bool negated, cint64* notAbsorbableOperandsCount) {
				cint64 notAbsorbableCount = 0;
				cint64 opCode = orConcept->getOperatorCode();
				QHash<CConcept*,TOccuredAbsorbablePair> conceptEqConAbsorbed;
				if (!negated && opCode == CCEQ) {
					CSortedNegLinker<CConcept*>* opConLinkerIt = orConcept->getOperandList();
					while (opConLinkerIt) {
						CConcept* opConcept = opConLinkerIt->getData();
						bool opNegated = opConLinkerIt->isNegated();
						if (!isConceptImplicationTriggerable(opConcept,opNegated,&conceptEqConAbsorbed)) {
							return false;
						}
						opConLinkerIt = opConLinkerIt->getNext();
					}
				}
				return true;
			}


			bool CTriggeredImplicationBinaryAbsorberPreProcess::isEquivalenceConceptCandidateExtractable(CConcept* orConcept, bool negated) {
				cint64 notAbsorbableCount = 0;
				cint64 opCode = orConcept->getOperatorCode();
				QSet<CConcept*> conceptEqConAbsorbed;
				bool partialTriggerable = false;
				if (!negated && opCode == CCEQ) {
					CSortedNegLinker<CConcept*>* opConLinkerIt = orConcept->getOperandList();
					while (opConLinkerIt) {
						CConcept* opConcept = opConLinkerIt->getData();
						bool opNegated = opConLinkerIt->isNegated();
						if (isConceptPartialImplicationTriggerable(opConcept,opNegated,&conceptEqConAbsorbed)) {
							partialTriggerable = true;
						}
						opConLinkerIt = opConLinkerIt->getNext();
					}
				}
				return partialTriggerable;
			}


			bool CTriggeredImplicationBinaryAbsorberPreProcess::isGCIConceptTriggeredImplicationAbsorbable(CConcept* orConcept, bool negated, cint64* notAbsorbableOperandsCount) {
				bool absorbable = false;
				cint64 notAbsorbableCount = 0;
				cint64 opCode = orConcept->getOperatorCode();
				QHash<CConcept*,TOccuredAbsorbablePair> conceptEqConAbsorbed;
				if (!negated && opCode == CCOR || negated && opCode == CCAND) {
					CSortedNegLinker<CConcept*>* opConLinkerIt = orConcept->getOperandList();
					while (opConLinkerIt) {
						CConcept* opConcept = opConLinkerIt->getData();
						bool opNegated = opConLinkerIt->isNegated();
						if (isConceptImplicationTriggerable(opConcept,!opNegated,&conceptEqConAbsorbed)) {
							absorbable = true;
						} else {
							notAbsorbableCount++;
						}
						opConLinkerIt = opConLinkerIt->getNext();
					}
				}
				if (notAbsorbableOperandsCount) {
					*notAbsorbableOperandsCount = notAbsorbableCount;
				}
				return absorbable;
			}


			bool CTriggeredImplicationBinaryAbsorberPreProcess::isConceptSimpleImplicationTriggerable(CConcept* concept, bool negated) {
				cint64 opCode = concept->getOperatorCode();
				cint64 parm = concept->getParameter();
				cint64 opCount = concept->getOperandCount();
				CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();
				if (!negated && (opCode == CCSUB || opCode == CCATOM || opCode == CCIMPLTRIG)) {
					return true;
				} else if (!negated && (opCode == CCAND || opCode == CCEQ) || negated && opCode == CCOR) {
					if (opCount >= 1) {
						bool allOpsTriggerable = true;
						CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
						while (opConLinkerIt && allOpsTriggerable) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegated = opConLinkerIt->isNegated()^negated;
							allOpsTriggerable &= isConceptSimpleImplicationTriggerable(opConcept,opNegated);
							opConLinkerIt = opConLinkerIt->getNext();
						}
						return allOpsTriggerable;
					}
				} else {
					if (mConceptTriggerLinkerHash.contains(TConceptNegationPair(concept,negated))) {
						return true;
					}
				}
				return false;
			}

			void CTriggeredImplicationBinaryAbsorberPreProcess::mergeDependingCacheList(QList<TConceptNegationPair>** dependingCacheList, QList<TConceptNegationPair>** subDependingCacheList) {
				if (*subDependingCacheList) {
					if (!*dependingCacheList) {
						*dependingCacheList = *subDependingCacheList;
					} else {
						(*dependingCacheList)->append(**subDependingCacheList);
						delete *subDependingCacheList;
					}
					*subDependingCacheList = nullptr;
				}
			}


			bool CTriggeredImplicationBinaryAbsorberPreProcess::isConceptImplicationTriggerable(CConcept* concept, bool negated, QHash<CConcept*,TOccuredAbsorbablePair>* conceptEqConAbsorbed) {
				cint64 openMultipleOccurCount = 0;
				QList<TConceptNegationPair>* dependingCacheList = nullptr;
				return isConceptImplicationTriggerable(concept,negated,conceptEqConAbsorbed,&openMultipleOccurCount,&dependingCacheList);
			}


			bool CTriggeredImplicationBinaryAbsorberPreProcess::isConceptImplicationTriggerable(CConcept* concept, bool negated, QHash<CConcept*,TOccuredAbsorbablePair>* conceptEqConAbsorbed, cint64* openMultipleOccurCount, QList<TConceptNegationPair>** dependingCacheList) {
				TConceptNegationPair conNegPair(concept,negated);
				if (mConceptTotalAbsorbableHash.contains(conNegPair)) {
					return mConceptTotalAbsorbableHash.value(conNegPair);
				}
				cint64 opCode = concept->getOperatorCode();
				cint64 parm = concept->getParameter();
				cint64 opCount = concept->getOperandCount();
				CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();

				bool absorbable = false;
				QList<TConceptNegationPair>* subDependingCacheList = nullptr;

				if (!negated && (opCode == CCSUB || opCode == CCATOM || opCode == CCTOP || opCode == CCIMPLTRIG)) {
					absorbable = true;
				} else if (!negated && (opCode == CCNOMINAL)) {
					absorbable = true;
				} else if (!negated && (opCode == CCNOMVAR)) {
					absorbable = true;
				} else if (!negated && (opCode == CCVALUE)) {
					absorbable = true;
				}
				else if (opCode == CCAQCHOOCE) {
					absorbable = true;
					CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
					while (opConLinkerIt) {
						CConcept* opConcept = opConLinkerIt->getData();
						bool opNegated = opConLinkerIt->isNegated();
						if (opNegated == negated) {
							absorbable &= isConceptImplicationTriggerable(opConcept, opNegated, conceptEqConAbsorbed, openMultipleOccurCount, &subDependingCacheList);
							mergeDependingCacheList(dependingCacheList, &subDependingCacheList);
						}
						opConLinkerIt = opConLinkerIt->getNext();
					}

				} else if (!negated && (opCode == CCSOME || opCode == CCAQSOME) || negated && (opCode == CCALL)) {
					if (!opConLinker) {
						absorbable = true;
					} else {
						bool allOpsTriggerable = true;
						CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
						while (opConLinkerIt && allOpsTriggerable) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegated = opConLinkerIt->isNegated()^negated;
							allOpsTriggerable &= isConceptImplicationTriggerable(opConcept,opNegated,conceptEqConAbsorbed,openMultipleOccurCount,&subDependingCacheList);
							mergeDependingCacheList(dependingCacheList,&subDependingCacheList);
							opConLinkerIt = opConLinkerIt->getNext();
						}
						absorbable = allOpsTriggerable;
					}
				} else  if (!negated && (opCode == CCATLEAST && parm == 1) || negated && (opCode == CCATMOST && parm == 0)) {
					if (!opConLinker) {
						absorbable = true;
					} else {
						bool allOpsTriggerable = true;
						CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
						while (opConLinkerIt && allOpsTriggerable) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegated = opConLinkerIt->isNegated();
							allOpsTriggerable &= isConceptImplicationTriggerable(opConcept,opNegated,conceptEqConAbsorbed,openMultipleOccurCount,&subDependingCacheList);
							mergeDependingCacheList(dependingCacheList,&subDependingCacheList);
							opConLinkerIt = opConLinkerIt->getNext();
						}
						absorbable = allOpsTriggerable;
					}
				} else  if (opCode == CCAND || opCode == CCOR) {
					bool allOpsTriggerable = true;
					CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
					while (opConLinkerIt && allOpsTriggerable) {
						CConcept* opConcept = opConLinkerIt->getData();
						bool opNegated = opConLinkerIt->isNegated()^negated;
						allOpsTriggerable &= isConceptImplicationTriggerable(opConcept,opNegated,conceptEqConAbsorbed,openMultipleOccurCount,&subDependingCacheList);
						mergeDependingCacheList(dependingCacheList,&subDependingCacheList);
						opConLinkerIt = opConLinkerIt->getNext();
					}
					absorbable = allOpsTriggerable;
				} else if (opCode == CCEQ) {
					if (negated) {
						absorbable = false;
					} else {
						TOccuredAbsorbablePair& occAbsPair = (*conceptEqConAbsorbed)[concept];
						if (occAbsPair.first) {
							if (!occAbsPair.second) {
								occAbsPair.second = true;
								++(*openMultipleOccurCount);
							}
							if (!*dependingCacheList) {
								*dependingCacheList = new QList<TConceptNegationPair>();
							}
							absorbable = true;
						} else {

							occAbsPair.first = true;
							bool allOpsTriggerable = true;
							CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
							while (opConLinkerIt && allOpsTriggerable) {
								CConcept* opConcept = opConLinkerIt->getData();
								bool opNegated = opConLinkerIt->isNegated()^negated;
								allOpsTriggerable &= isConceptImplicationTriggerable(opConcept,opNegated,conceptEqConAbsorbed,openMultipleOccurCount,&subDependingCacheList);
								mergeDependingCacheList(dependingCacheList,&subDependingCacheList);
								opConLinkerIt = opConLinkerIt->getNext();
							}

							if (*dependingCacheList) {
								if (!allOpsTriggerable) {
									for (QList<TConceptNegationPair>::const_iterator it = (*dependingCacheList)->constBegin(), itEnd = (*dependingCacheList)->constEnd(); it != itEnd; ++it) {
										TConceptNegationPair upConNegPair(*it);
										mConceptTotalAbsorbableHash.insert(upConNegPair,false);
									}
									delete *dependingCacheList;
									*dependingCacheList = nullptr;
								}
							}

							if (occAbsPair.second) {
								if (--(*openMultipleOccurCount) <= 0) {
									if (*dependingCacheList && allOpsTriggerable) {
										for (QList<TConceptNegationPair>::const_iterator it = (*dependingCacheList)->constBegin(), itEnd = (*dependingCacheList)->constEnd(); it != itEnd; ++it) {
											TConceptNegationPair upConNegPair(*it);
											mConceptTotalAbsorbableHash.insert(upConNegPair,true);
										}
										delete *dependingCacheList;
										*dependingCacheList = nullptr;
									}
								}
							}

							absorbable = allOpsTriggerable;		
						}
					}
				} else {
					absorbable = false;
				}
				if (absorbable) {
					if (*dependingCacheList) {
						(*dependingCacheList)->append(conNegPair);
					} else {
						mConceptTotalAbsorbableHash.insert(conNegPair,absorbable);
					}
				} else {
					mConceptTotalAbsorbableHash.insert(conNegPair,absorbable);
				}
				return absorbable;
			}








			bool CTriggeredImplicationBinaryAbsorberPreProcess::absorbGCIConceptsToTriggeredImplications(CConcept* orConcept, bool negated) {
				cint64 opCode = orConcept->getOperatorCode();
				if (!negated && opCode == CCOR || negated && opCode == CCAND) {
					QList<TConceptNegationPair> candList;
					if (addAbsorbableDisjunctCandidates(orConcept,negated,candList)) {
						if (candList.count() >= 1) {
							QList<TConceptNegationPair> absorbList(splitAbsorbableDisjuncts(candList));
							if (!absorbList.isEmpty()) {
								createGCIAbsorbedTriggeredImplication(orConcept,negated,absorbList,candList);
								return true;
							}
						}
					}
				}
				return false;
			}




			bool CTriggeredImplicationBinaryAbsorberPreProcess::isConceptPartialImplicationTriggerable(CConcept* concept, bool negated, QSet<CConcept*>* conceptEqConAbsorbed) {
				TConceptNegationPair conNegPair(concept,negated);
				if (mConceptPartialAbsorbableHash.contains(conNegPair)) {
					return mConceptPartialAbsorbableHash.value(conNegPair);
				}

				cint64 opCode = concept->getOperatorCode();
				cint64 parm = concept->getParameter();
				cint64 opCount = concept->getOperandCount();
				CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();

				bool absorbable = false;

				if (!negated && (opCode == CCSUB || opCode == CCATOM || opCode == CCTOP || opCode == CCIMPLTRIG)) {
					absorbable = true;
				} else if (!negated && (opCode == CCNOMINAL)) {
					absorbable = true;
				} else if (!negated && (opCode == CCNOMVAR)) {
					absorbable = true;
				} else if (!negated && (opCode == CCVALUE)) {
					absorbable = true;
				} else if (!negated && (opCode == CCSELF)) {
					absorbable = true;
				} else if (opCode == CCAQCHOOCE) {
					CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
					while (opConLinkerIt) {
						CConcept* opConcept = opConLinkerIt->getData();
						bool opNegated = opConLinkerIt->isNegated();
						if (opNegated == negated) {
							absorbable |= isConceptPartialImplicationTriggerable(opConcept, opNegated, conceptEqConAbsorbed);
						}
						opConLinkerIt = opConLinkerIt->getNext();
					}
				} else  if (!negated && (opCode == CCSOME || opCode == CCAQSOME) || negated && (opCode == CCALL)) {
					CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
					while (opConLinkerIt) {
						CConcept* opConcept = opConLinkerIt->getData();
						bool opNegated = opConLinkerIt->isNegated()^negated;
						isConceptPartialImplicationTriggerable(opConcept,opNegated,conceptEqConAbsorbed);
						opConLinkerIt = opConLinkerIt->getNext();
					}
					absorbable = true;
				} else  if (!negated && (opCode == CCATLEAST && parm >= 1) || negated && (opCode == CCATMOST && parm >= 0)) {
					CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
					while (opConLinkerIt) {
						CConcept* opConcept = opConLinkerIt->getData();
						bool opNegated = opConLinkerIt->isNegated()^negated;
						isConceptPartialImplicationTriggerable(opConcept,opNegated,conceptEqConAbsorbed);
						opConLinkerIt = opConLinkerIt->getNext();
					}
					absorbable = true;
				} else  if (!negated && (opCode == CCAND) || negated && opCode == CCOR) {
					// AND, at least one operand has to be triggerable
					if (opCount >= 1) {
						CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
						while (opConLinkerIt) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegated = opConLinkerIt->isNegated()^negated;
							if (isConceptPartialImplicationTriggerable(opConcept,opNegated,conceptEqConAbsorbed)) {
								absorbable = true;
							}
							opConLinkerIt = opConLinkerIt->getNext();
						}
					}
				} else  if (negated && (opCode == CCAND) || !negated && opCode == CCOR) {
					// OR, all operands have to be triggerable
					if (opCount >= 1) {
						bool allOpsTriggerable = true;
						CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
						while (opConLinkerIt && allOpsTriggerable) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegated = opConLinkerIt->isNegated()^negated;
							allOpsTriggerable &= isConceptPartialImplicationTriggerable(opConcept,opNegated,conceptEqConAbsorbed);
							opConLinkerIt = opConLinkerIt->getNext();
						}
						absorbable = allOpsTriggerable;
					}
				} else if (opCode == CCEQ) {
					if (negated) {
						absorbable = false;
					} else {
						if (conceptEqConAbsorbed->contains(concept)) {
							mEQNotConceptPartialAbsorbableSet.insert(concept);
							return false;
						} else {
							// AND, at least one operand has to be triggerable
							if (opCount >= 1) {
								conceptEqConAbsorbed->insert(concept);
								CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
								while (opConLinkerIt) {
									CConcept* opConcept = opConLinkerIt->getData();
									bool opNegated = opConLinkerIt->isNegated()^negated;
									if (isConceptPartialImplicationTriggerable(opConcept,opNegated,conceptEqConAbsorbed)) {
										absorbable = true;
									}
									opConLinkerIt = opConLinkerIt->getNext();
								}
							}							
							if (mEQNotConceptPartialAbsorbableSet.contains(concept)) {
								absorbable = false;
							}
						}
					}
				} else if (!negated && (opCode == CCDATALITERAL)) {
					if (mConfDatatypeAbsorption) {
						CDataLiteral* dataLiteral = concept->getDataLiteral();
						if (dataLiteral) {
							CDataLiteralValue* dataLitValue = dataLiteral->getDataLiteralValue();
							if (dataLitValue) {
								if (dataLitValue->getDataValueType() == CDataLiteralValue::DLVT_REAL) {
									absorbable = true;
								}
								if (dataLitValue->getDataValueType() == CDataLiteralValue::DLVT_STRING) {
									absorbable = true;
								}
								if (dataLitValue->getDataValueType() == CDataLiteralValue::DLVT_BOOLEAN) {
									absorbable = true;
								}
								if (dataLitValue->getDataValueType() == CDataLiteralValue::DLVT_DOUBLE) {
									absorbable = true;
								}
								if (dataLitValue->getDataValueType() == CDataLiteralValue::DLVT_FLOAT) {
									absorbable = true;
								}
								if (dataLitValue->getDataValueType() == CDataLiteralValue::DLVT_IRI) {
									absorbable = true;
								}
								if (dataLitValue->getDataValueType() == CDataLiteralValue::DLVT_XML) {
									absorbable = true;
								}
								if (dataLitValue->getDataValueType() == CDataLiteralValue::DLVT_HEXBINARY) {
									absorbable = true;
								}
								if (dataLitValue->getDataValueType() == CDataLiteralValue::DLVT_BASE64BINARY) {
									absorbable = true;
								}
								if (dataLitValue->getDataValueType() == CDataLiteralValue::DLVT_DATETIME) {
									absorbable = true;
								}
							}
						}
					}
				} else if (!negated && (opCode == CCDATARESTRICTION)) {
					if (mConfDatatypeAbsorption) {
						cint64 dataRestrictionType = parm;
						CDatatype* datatype = concept->getDatatype();
						if (datatype->isBasicDatatype()) {
							CDatatype::DATATYPE_BASIC_TYPE datatypeType = datatype->getBasicDatatypeType();
							if (datatypeType == CDatatype::DBT_REAL) {
								if (dataRestrictionType == CDFC_RATIONAL) {
									absorbable = true;
								} else if (dataRestrictionType == CDFC_DECIMAL) {
									absorbable = true;
								} else if (dataRestrictionType == CDFC_INTEGER) {
									absorbable = true;
								}
							} 
							if (datatypeType == CDatatype::DBT_STRING) {
								if (dataRestrictionType == CDFC_STRING) {
									absorbable = true;
								}
							}
							if (datatypeType == CDatatype::DBT_REAL || datatypeType == CDatatype::DBT_STRING || datatypeType == CDatatype::DBT_BOOLEAN || datatypeType == CDatatype::DBT_DOUBLE || datatypeType == CDatatype::DBT_DATETIME
										|| datatypeType == CDatatype::DBT_FLOAT || datatypeType == CDatatype::DBT_IRI || datatypeType == CDatatype::DBT_XML || datatypeType == CDatatype::DBT_HEXBINARY || datatypeType == CDatatype::DBT_BASE64BINARY) {

								CDataLiteral* dataLiteral = concept->getDataLiteral();
								if (!dataLiteral || !dataLiteral->getDatatype() || !dataLiteral->getDataLiteralValue() || dataLiteral->getDatatype()->getValueSpaceType() != datatype->getValueSpaceType()) {
									absorbable = false;
								} else {
									if (dataRestrictionType == CDFC_MIN_EXCLUSIVE) {
										absorbable = true;
									} else if (dataRestrictionType == CDFC_MIN_INCLUSIVE) {
										absorbable = true;
									} else if (dataRestrictionType == CDFC_MAX_INCLUSIVE) {
										absorbable = true;
									} else if (dataRestrictionType == CDFC_MAX_EXCLUSIVE) {
										absorbable = true;
									}
								}
							}
						}
					}
				} else if (!negated && (opCode == CCDATATYPE)) {

					if (mConfDatatypeAbsorption) {
						CDatatype* datatype = concept->getDatatype();
						if (datatype->isBasicDatatype()) {
							CDatatype::DATATYPE_BASIC_TYPE datatypeType = datatype->getBasicDatatypeType();
							if (datatypeType == CDatatype::DBT_REAL || datatypeType == CDatatype::DBT_STRING || datatypeType == CDatatype::DBT_BOOLEAN || datatypeType == CDatatype::DBT_DOUBLE || datatypeType == CDatatype::DBT_DATETIME
										|| datatypeType == CDatatype::DBT_FLOAT || datatypeType == CDatatype::DBT_IRI || datatypeType == CDatatype::DBT_XML || datatypeType == CDatatype::DBT_HEXBINARY || datatypeType == CDatatype::DBT_BASE64BINARY) {
								absorbable = true;
							}
						} else {

							if (opCount >= 1) {
								CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
								while (opConLinkerIt) {
									CConcept* opConcept = opConLinkerIt->getData();
									bool opNegated = opConLinkerIt->isNegated()^negated;
									if (isConceptPartialImplicationTriggerable(opConcept,opNegated,conceptEqConAbsorbed)) {
										absorbable = true;
									}
									opConLinkerIt = opConLinkerIt->getNext();
								}
							}
						}
					}


				} else {
					absorbable = false;
				}
				mConceptPartialAbsorbableHash.insert(conNegPair,absorbable);
				return absorbable;
			}





			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createPartialExtendedAbsorbedImpliedTriggerCocnept(const QList<TConceptNegationPair>& absorbList) {
				CConceptTriggerLinker* trigger = createPartialExtendedAbsorbedImpliedTrigger(absorbList);
				if (trigger) {
					return trigger->getTriggerConcept();
				}
				return nullptr;
			}


			CConceptTriggerLinker* CTriggeredImplicationBinaryAbsorberPreProcess::createPartialExtendedAbsorbedImpliedTrigger(const QList<TConceptNegationPair>& absorbList) {
				CConceptTriggerLinker* triggers = nullptr;
				for (QList<TConceptNegationPair>::const_iterator it = absorbList.constBegin(), itEnd = absorbList.constEnd(); it != itEnd; ++it) {
					TConceptNegationPair conNegPair(*it);
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					CConceptTriggerLinker* tmpTriggers = copyTriggerLinkers(getPartialTriggersForConcept(concept,!negated));
					triggers = tmpTriggers->append(triggers);
				}
				CConceptTriggerLinker* impliedConceptTrigger = getImplicationTriggeredConceptForTriggers(triggers,nullptr);
				return impliedConceptTrigger;
			}


			CConcept* CTriggeredImplicationBinaryAbsorberPreProcess::createPartialExtendedAbsorbedTriggeredImplication(CConcept* impliedConcept, bool impliedNegation, const QList<TConceptNegationPair>& absorbList) {
				CConcept* implicationConcept = nullptr;
				CConceptTriggerLinker* triggers = nullptr;
				for (QList<TConceptNegationPair>::const_iterator it = absorbList.constBegin(), itEnd = absorbList.constEnd(); it != itEnd; ++it) {
					TConceptNegationPair conNegPair(*it);
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					CConceptTriggerLinker* tmpTriggers = copyTriggerLinkers(getPartialTriggersForConcept(concept,!negated));
					triggers = tmpTriggers->append(triggers);
				}
				CConceptTriggerLinker* impliedConceptTrigger = getImplicationTriggeredConceptForTriggers(triggers,&implicationConcept);
				addUnfoldingConceptForConcept(impliedConceptTrigger->getTriggerConcept(),impliedConcept,impliedNegation);
				return implicationConcept;
			}



			void CTriggeredImplicationBinaryAbsorberPreProcess::createGCIAbsorbedTriggeredImplication(CConcept* orConcept, bool negated, QList<TConceptNegationPair>& absorbList, QList<TConceptNegationPair>& candList) {
				CConcept* impliedConcept = nullptr;
				bool impliedNegation = false; 

				if (candList.isEmpty()) {
					candList.append(TConceptNegationPair(mTopConcept,true));
				}

				CConceptTriggerLinker* triggers = nullptr;
				for (QList<TConceptNegationPair>::const_iterator it = absorbList.constBegin(), itEnd = absorbList.constEnd(); it != itEnd; ++it) {
					TConceptNegationPair conNegPair(*it);
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					CConceptTriggerLinker* tmpTriggers = copyTriggerLinkers(getTriggersForConcept(concept,!negated));
					triggers = tmpTriggers->append(triggers);
				}

				CConcept* partialImpliedTriggerConcept = nullptr;
				QList<TConceptNegationPair> partialAbsorbableList;
				if (candList.count() > 1) {
					if (mConfPartialGCIAbsorption) {
						partialAbsorbableList = getPartialAbsorbableDisjuncts(candList);
						if (!partialAbsorbableList.isEmpty()) {
							CConceptTriggerLinker* partialImpliedTrigger = createPartialExtendedAbsorbedImpliedTrigger(partialAbsorbableList);
							partialImpliedTriggerConcept = partialImpliedTrigger->getTriggerConcept();
							triggers = partialImpliedTrigger->append(triggers);
						}
					}
				}

				CConceptTriggerLinker* impliedConceptTrigger = getImplicationTriggeredConceptForTriggers(triggers,nullptr);
				impliedConcept = impliedConceptTrigger->getTriggerConcept();
				if (candList.count() > 1) {


					CConcept* orImpliedConcept = createImpliedConcept();
					for (QList<TConceptNegationPair>::const_iterator it = candList.constBegin(), itEnd = candList.constEnd(); it != itEnd; ++it) {
						TConceptNegationPair conNegPair(*it);
						CConcept* concept = conNegPair.first;
						bool negated = conNegPair.second;
						addConceptToImplied(orImpliedConcept,concept,negated);
					}
					orImpliedConcept->setOperatorCode(CCOR);
					addUnfoldingConceptForConcept(impliedConcept,orImpliedConcept,false);

					if (partialImpliedTriggerConcept) {
						CConceptRoleBranchingTrigger* branchTriggers = getSimpleBranchTriggersForConcept(partialImpliedTriggerConcept,true);
						mBranchTriggVec->setData(impliedConcept->getConceptTag(),branchTriggers);
						++mStatGCIAbsorptionPartialExtended;
						
						if (partialAbsorbableList.count() + 1 == candList.count()) {
							QList<TConceptNegationPair> allList(candList);
							allList.append(absorbList);
							QList<TConceptNegationPair> assurList(getAssuringAbsorbableDisjuncts(allList));
							if (assurList.count() + 1 == allList.count()) {
								++mStatGCIAbsorptionAssuring;
								createGCIAssuringAbsorbedTriggeredImplication(orConcept, negated, assurList, candList);
							}
						}

					}
				} else {
					TConceptNegationPair conNegPair(candList.first());
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					addUnfoldingConceptForConcept(impliedConcept,concept,negated);
				}
			}



			CConceptRoleBranchingTrigger* CTriggeredImplicationBinaryAbsorberPreProcess::getBranchTiggers(CConceptTriggerLinker* triggers) {
				CConceptRoleBranchingTrigger* branchingTriggerLinker = nullptr;
				for (CConceptTriggerLinker* triggerIt = triggers; triggerIt; triggerIt = triggerIt->getNext()) {
					CConceptRoleBranchingTrigger* branchingTrigger = nullptr;
					branchingTrigger = CObjectAllocator< CConceptRoleBranchingTrigger >::allocateAndConstruct(mMemMan);
					branchingTrigger->initConceptBranchingTrigger(triggerIt->getTriggerConcept(),false);
					branchingTriggerLinker = branchingTrigger->append(branchingTriggerLinker);
				}
				return branchingTriggerLinker;
			}


			CConceptRoleBranchingTrigger* CTriggeredImplicationBinaryAbsorberPreProcess::getSimpleBranchTriggersForConcept(CConcept* concept, bool negated) {
				CConceptRoleBranchingTrigger* branchingTrigger = nullptr;
				cint64 conOpCount = concept->getOperandCount();

				qint64 conOpCode = concept->getOperatorCode();

				if ((conOpCode == CCSUB || conOpCode == CCATOM || conOpCode == CCIMPLTRIG) && !negated) {
					branchingTrigger = CObjectAllocator< CConceptRoleBranchingTrigger >::allocateAndConstruct(mMemMan);
					branchingTrigger->initConceptBranchingTrigger(concept,false);
				} else if (conOpCode == CCALL && !negated || conOpCode == CCSOME && negated || conOpCode == CCATMOST && !negated || conOpCode == CCATLEAST && negated && conOpCount > 0) {
					CRole* role = concept->getRole();
					CRole* locatedRole = CConceptRoleIndividualLocator::getLocatedRole(role,mOnto);
					CConcept* triggerConcept = getRoleDomainTriggerConcept(locatedRole);
					branchingTrigger = CObjectAllocator< CConceptRoleBranchingTrigger >::allocateAndConstruct(mMemMan);
					branchingTrigger->initConceptBranchingTrigger(triggerConcept,false);
				}

				return branchingTrigger;
			}



			CConceptTriggerLinker* CTriggeredImplicationBinaryAbsorberPreProcess::getPartialTriggersForConcept(CConcept* concept, bool negated, CConcept* backPropConcept) {
				CConceptTriggerLinker* triggers = nullptr;

				CConcept* backPropImpConcept = nullptr;
				CConcept* backPropTriggerConcept = nullptr;
				CConcept** backPropImpConceptPointer = nullptr;
				bool branchTiggerCreation = false;
				if (backPropConcept) {
					backPropImpConceptPointer = &backPropImpConcept;
					branchTiggerCreation = true;
				}
				TConceptNegationPair conNegPair(concept,negated);
				triggers = mConceptTriggerLinkerHash.value(conNegPair);
				if (triggers && backPropConcept) {
					CConcept* backPropActivCon = mBackPropActivationConHash.value(conNegPair);
					if (backPropActivCon) {
						addUnfoldingConceptForConcept(backPropConcept, backPropActivCon, false);
					}
				}
				if (!triggers) {
					if (backPropConcept) {
						backPropTriggerConcept = createTriggerConcept(branchTiggerCreation);
						addUnfoldingConceptForConcept(backPropConcept, backPropTriggerConcept, false);
					}

					cint64 opCode = concept->getOperatorCode();
					cint64 parm = concept->getParameter();
					cint64 opCount = concept->getOperandCount();
					CRole* role = concept->getRole();
					CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();

					if (!negated && (opCode == CCATOM || opCode == CCSUB|| opCode == CCTOP || opCode == CCIMPLTRIG)) {
						CConceptTriggerLinker* newTrigger = createTriggerLinker();
						newTrigger->initConceptTriggerLinker(concept, 1);
						if (backPropConcept) {
							CConceptTriggerLinker* implTriggeredConcept = getImplicationTriggeredConceptForTriggers(newTrigger, backPropImpConceptPointer);
							addUnfoldingConceptForConcept(backPropTriggerConcept, backPropImpConcept, false);
							newTrigger = implTriggeredConcept;
						}
						triggers = newTrigger->append(triggers);


					} else if (negated && (opCode == CCOR) || !negated && (opCode == CCAND || opCode == CCEQ)) {
						// AND, collect triggers from operands

						if (opCode != CCEQ || !mEQNotConceptPartialAbsorbableSet.contains(concept)) {

							CConceptTriggerLinker* andTriggers = nullptr;
							for (CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker; opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
								CConcept* opConcept = opConLinkerIt->getData();
								bool opNegated = opConLinkerIt->isNegated()^negated;

								if (mConceptPartialAbsorbableHash.value(TConceptNegationPair(opConcept,opNegated),false) || mConceptTotalAbsorbableHash.value(TConceptNegationPair(opConcept,opNegated),false)) {
									CConceptTriggerLinker* newTriggers = copyTriggerLinkers(getPartialTriggersForConcept(opConcept,opNegated, backPropTriggerConcept));
									if (newTriggers) {
										andTriggers = newTriggers->append(andTriggers);
									}
								}
							}
							if (opCode == CCEQ || backPropConcept) {
								CConceptTriggerLinker* implTriggers = getImplicationTriggeredConceptForTriggers(andTriggers,backPropImpConceptPointer);
								if (backPropConcept) {
									addUnfoldingConceptForConcept(backPropTriggerConcept, backPropImpConcept, false);
								}
								andTriggers = implTriggers;
							}
							if (andTriggers) {
								triggers = andTriggers->append(triggers);
							}
						}


					} else if (!negated && (opCode == CCOR) || negated && (opCode == CCAND || opCode == CCEQ)) {
						// or trigger
						CConceptTriggerLinker* orTriggers = nullptr;
						for (CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker; opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegated = opConLinkerIt->isNegated()^negated;

							CConceptTriggerLinker* newTriggers = copyTriggerLinkers(getPartialTriggersForConcept(opConcept,opNegated, backPropTriggerConcept));
							if (newTriggers) {
								CConceptTriggerLinker* implTriggeredConcept = getImplicationTriggeredConceptForTriggers(newTriggers, backPropImpConceptPointer);
								if (backPropImpConceptPointer) {
									addUnfoldingConceptForConcept(backPropTriggerConcept, backPropImpConcept, false);
								}
								orTriggers = implTriggeredConcept->append(orTriggers);
							}
						}
						if (orTriggers) {
							if (orTriggers->hasNext()) {
								// more than one trigger
								cint64 minTriggerComplexity = 0;
								CConcept* triggerConcept = createTriggerConcept(branchTiggerCreation);
								for (CConceptTriggerLinker* orTriggerIt = orTriggers; orTriggerIt; orTriggerIt = orTriggerIt->getNext()) {
									minTriggerComplexity = qMin(orTriggerIt->getTriggerComplexity(),minTriggerComplexity);
									addUnfoldingConceptForConcept(orTriggerIt->getTriggerConcept(),triggerConcept,false);
								}
								CConceptTriggerLinker* newTrigger = createTriggerLinker();
								newTrigger->initConceptTriggerLinker(triggerConcept,minTriggerComplexity);
								triggers = newTrigger->append(triggers);
								//releaseTriggerLinkers(orTriggers);
							} else {
								// only one trigger, add simply to other triggers
								triggers = orTriggers->append(triggers);
							}
						}

					} else if (opCode == CCAQCHOOCE) {

						CConceptTriggerLinker* andTriggers = nullptr;
						for (CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker; opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegated = opConLinkerIt->isNegated();

							if (opNegated == negated) {
								if (mConceptPartialAbsorbableHash.value(TConceptNegationPair(opConcept, opNegated), false) || mConceptTotalAbsorbableHash.value(TConceptNegationPair(opConcept, opNegated), false)) {
									CConceptTriggerLinker* newTriggers = copyTriggerLinkers(getPartialTriggersForConcept(opConcept, opNegated, backPropTriggerConcept));
									if (newTriggers) {
										andTriggers = newTriggers->append(andTriggers);
									}
								}
							}
						}
						if (backPropConcept) {
							CConceptTriggerLinker* implTriggers = getImplicationTriggeredConceptForTriggers(andTriggers, backPropImpConceptPointer);
							if (backPropConcept) {
								addUnfoldingConceptForConcept(backPropTriggerConcept, backPropImpConcept, false);
							}
							andTriggers = implTriggers;
						}
						if (andTriggers) {
							triggers = andTriggers->append(triggers);
						}


					} else if (!negated && (opCode == CCSOME || opCode == CCSELF || opCode == CCAQSOME) || negated && (opCode == CCALL) ||
						!negated && (opCode == CCATLEAST) || negated && (opCode == CCATMOST)) {
							CConcept* propagatedTriggerConcept = nullptr;
							cint64 triggerComplexity = 1;
							bool negateOperands = negated && (opCode == CCSOME || opCode == CCSELF || opCode == CCALL);
							CConceptTriggerLinker* allTriggers = nullptr;
							CConcept* roleBackPropTriggerConcept = nullptr;
							if (backPropConcept) {
								roleBackPropTriggerConcept = createTriggerConcept(branchTiggerCreation);
								CConcept* roleBackPropConcept = createTriggerPropagationConcept(roleBackPropTriggerConcept, role, branchTiggerCreation, false);
								addUnfoldingConceptForConcept(backPropTriggerConcept, roleBackPropConcept, false);
							}
							for (CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker; opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
								CConcept* opConcept = opConLinkerIt->getData();
								bool opNegated = opConLinkerIt->isNegated()^negateOperands;
								if (!(!opNegated && opConcept == mTopConcept || opNegated && opConcept == mBottomConcept)) {
									if (mConceptPartialAbsorbableHash.value(TConceptNegationPair(opConcept,opNegated),false) || mConceptTotalAbsorbableHash.value(TConceptNegationPair(opConcept,opNegated),false)) {
										CConceptTriggerLinker* newTrigger = copyTriggerLinkers(getPartialTriggersForConcept(opConcept,opNegated, roleBackPropTriggerConcept));
										if (newTrigger) {
											allTriggers = newTrigger->append(allTriggers);
										}
									}
								}
							}
							if (allTriggers) {
								propagatedTriggerConcept = createTriggerConcept(branchTiggerCreation);
								CConcept* nextLevelTriggerConcept = createTriggerPropagationConcept(propagatedTriggerConcept,role, branchTiggerCreation);
								if (allTriggers->hasNext()) {
									// requires a generation of an implication concept
									CConceptTriggerLinker* implTriggeredConcept = getImplicationTriggeredConceptForTriggers(allTriggers, backPropImpConceptPointer);
									if (backPropImpConceptPointer) {
										addUnfoldingConceptForConcept(roleBackPropTriggerConcept, backPropImpConcept, false);
									}
									triggerComplexity += implTriggeredConcept->getTriggerComplexity();
									addUnfoldingConceptForConcept(implTriggeredConcept->getTriggerConcept(),nextLevelTriggerConcept,false);
								} else {
									triggerComplexity += allTriggers->getTriggerComplexity();
									addUnfoldingConceptForConcept(allTriggers->getTriggerConcept(),nextLevelTriggerConcept,false);
									releaseTriggerLinkers(allTriggers);
								}
							} else {
								if (backPropConcept) {
									propagatedTriggerConcept = createTriggerConcept(branchTiggerCreation);
									CConcept* nextLevelTriggerConcept = createTriggerPropagationConcept(propagatedTriggerConcept, role, branchTiggerCreation);
									addUnfoldingConceptForConcept(roleBackPropTriggerConcept, nextLevelTriggerConcept, false);
								} else {
									CRole* locatedRole = CConceptRoleIndividualLocator::getLocatedRole(role, mOnto);
									propagatedTriggerConcept = getRoleDomainTriggerConcept(locatedRole);
								}
							}
							CConceptTriggerLinker* newTrigger = createTriggerLinker();
							newTrigger->initConceptTriggerLinker(propagatedTriggerConcept,triggerComplexity);
							triggers = newTrigger->append(triggers);


					} else if (!negated && (opCode == CCVALUE)) {


						CIndividual* individual = concept->getNominalIndividual();

						CConcept* propagatedTriggerConcept = createTriggerConcept(branchTiggerCreation);
						CConcept* nextLevelTriggerConcept = createTriggerPropagationConcept(propagatedTriggerConcept,role, branchTiggerCreation);

						cint64 triggerComplexity = 2;



						if (!backPropConcept) {
							CIndividual* locatedIndividual = CConceptRoleIndividualLocator::getLocatedIndividual(individual, mOnto);
							CConceptAssertionLinker* assLinker = CObjectAllocator< CConceptAssertionLinker >::allocateAndConstruct(mMemMan);
							assLinker->initNegLinker(nextLevelTriggerConcept,false);
							locatedIndividual->addAssertionConceptLinker(assLinker);

							if (!mIndividualTriggerConceptHash) {
								mIndividualTriggerConceptHash = mTBox->getIndividualTriggerConceptHash(true);
							}
							mIndividualTriggerConceptHash->insertMulti(individual->getIndividualID(),nextLevelTriggerConcept);
						} else {
							CConcept* roleBackPropTriggerConcept = createTriggerConcept(branchTiggerCreation);
							CConcept* roleBackPropConcept = createTriggerPropagationConcept(roleBackPropTriggerConcept, role, branchTiggerCreation, false);
							addUnfoldingConceptForConcept(backPropTriggerConcept, roleBackPropConcept, false);

							CConcept* nomImpConcept = createNominalImplication(nextLevelTriggerConcept, individual);
							addUnfoldingConceptForConcept(roleBackPropTriggerConcept, nomImpConcept, false);

						}

						CConceptTriggerLinker* newTrigger = createTriggerLinker();
						newTrigger->initConceptTriggerLinker(propagatedTriggerConcept,triggerComplexity);
						triggers = newTrigger->append(triggers);

					} else if (!negated && (opCode == CCNOMVAR)) {
						cint64 triggerComplexity = 1;
						CConcept* nominalTriggerConcept = mTBox->getIndividualTriggerConcept();
						CConceptTriggerLinker* newTrigger = createTriggerLinker();
						newTrigger->initConceptTriggerLinker(nominalTriggerConcept,triggerComplexity);
						triggers = newTrigger->append(triggers);

					} else if (!negated && (opCode == CCNOMINAL)) {
						CConcept* nominalTriggerConcept = createTriggerConcept(branchTiggerCreation);
						cint64 triggerComplexity = 1;

						CIndividual* individual = concept->getNominalIndividual();
						if (!backPropConcept) {
							CIndividual* locatedIndividual = CConceptRoleIndividualLocator::getLocatedIndividual(individual, mOnto);
							CConceptAssertionLinker* assLinker = CObjectAllocator< CConceptAssertionLinker >::allocateAndConstruct(mMemMan);
							assLinker->initNegLinker(nominalTriggerConcept,false);
							locatedIndividual->addAssertionConceptLinker(assLinker);

							if (!mIndividualTriggerConceptHash) {
								mIndividualTriggerConceptHash = mTBox->getIndividualTriggerConceptHash(true);
							}
							mIndividualTriggerConceptHash->insertMulti(individual->getIndividualID(),nominalTriggerConcept);
						} else {
							CConcept* nomImpConcept = createNominalImplication(nominalTriggerConcept, individual);
							addUnfoldingConceptForConcept(backPropTriggerConcept, nomImpConcept, false);
						}


						CConceptTriggerLinker* newTrigger = createTriggerLinker();
						newTrigger->initConceptTriggerLinker(nominalTriggerConcept,triggerComplexity);
						triggers = newTrigger->append(triggers);


					} else if (!negated && (opCode == CCDATALITERAL)) {
						if (mConfDatatypeAbsorption) {
							CDataLiteral* dataLiteral = concept->getDataLiteral();
							if (dataLiteral) {
								CDataLiteralValue* dataLitValue = dataLiteral->getDataLiteralValue();
								if (dataLitValue) {

									cint64 triggerComplexity = 2;
									CConcept* valueTriggerConcept = createTriggerConcept(branchTiggerCreation);
									CConceptTriggerLinker* valueTrigger = createTriggerLinker();
									valueTrigger->initConceptTriggerLinker(valueTriggerConcept,triggerComplexity);
									CDatatypeValueSpaceConceptTriggerLinker* valueSpaceConceptTriggerLinker = createValueSpaceConceptTriggerLinker();
									valueSpaceConceptTriggerLinker->initConceptTrigger(valueTriggerConcept);


									if (backPropConcept) {
										CConcept* datatypeImpConcept = createDataLiteralImplication(valueTriggerConcept, dataLiteral);
										addUnfoldingConceptForConcept(backPropTriggerConcept, datatypeImpConcept, false);
									} else {


										CDatatypeValueSpaceTriggers* valueSpaceTrigger = nullptr;
										CDataLiteralValue* dataLitValue = dataLiteral->getDataLiteralValue();
										if (dataLitValue) {
											CDataLiteralCompareValue* compareDataLitValue = dynamic_cast<CDataLiteralCompareValue*>(dataLitValue);
											if (compareDataLitValue) {
												CDatatypeValueSpaceType* valueSpaceType = nullptr;
												CDataLiteralValue::DATA_LITERAL_VALUE_TYPE dataLiteralValueType = compareDataLitValue->getDataValueType();
												CDatatypeValueSpaceTypes* valueSpaceTypes = CDatatypeValueSpaceTypes::getValueSpaceTypes();
												if (dataLiteralValueType == CDataLiteralValue::DLVT_REAL) {
													valueSpaceType = valueSpaceTypes->getValueSpaceRealType();
												} else if (dataLiteralValueType == CDataLiteralValue::DLVT_STRING) {
													valueSpaceType = valueSpaceTypes->getValueSpaceStringType();
												} else if (dataLiteralValueType == CDataLiteralValue::DLVT_BOOLEAN) {
													valueSpaceType = valueSpaceTypes->getValueSpaceBooleanType();
												} else if (dataLiteralValueType == CDataLiteralValue::DLVT_DOUBLE) {
													valueSpaceType = valueSpaceTypes->getValueSpaceDoubleType();
												} else if (dataLiteralValueType == CDataLiteralValue::DLVT_FLOAT) {
													valueSpaceType = valueSpaceTypes->getValueSpaceFloatType();
												} else if (dataLiteralValueType == CDataLiteralValue::DLVT_IRI) {
													valueSpaceType = valueSpaceTypes->getValueSpaceIRIType();
												} else if (dataLiteralValueType == CDataLiteralValue::DLVT_XML) {
													valueSpaceType = valueSpaceTypes->getValueSpaceXMLType();
												} else if (dataLiteralValueType == CDataLiteralValue::DLVT_HEXBINARY) {
													valueSpaceType = valueSpaceTypes->getValueSpaceBinaryHexDataType();
												} else if (dataLiteralValueType == CDataLiteralValue::DLVT_BASE64BINARY) {
													valueSpaceType = valueSpaceTypes->getValueSpaceBinaryBase64DataType();
												} else if (dataLiteralValueType == CDataLiteralValue::DLVT_DATETIME) {
													valueSpaceType = valueSpaceTypes->getValueSpaceDateTimeType();
												}

												if (valueSpaceType) {
													CDatatypeValueSpacesTriggers* valueSpacesTriggers = mMBox->getValueSpacesTriggers(true);
													CDatatypeValueSpaceCompareTriggers* compareValueSpaceConceptTrigger = (CDatatypeValueSpaceCompareTriggers*)valueSpacesTriggers->getValueSpaceTriggers(valueSpaceType);
													if (compareValueSpaceConceptTrigger) {
														compareValueSpaceConceptTrigger->addPartialValueConceptTrigger(compareDataLitValue, valueSpaceConceptTriggerLinker);
													}
												}
											}
										}
									}

									triggers = valueTrigger->append(triggers);

								}
							}
						}



					} else if (!negated && (opCode == CCDATATYPE)) {
						if (mConfDatatypeAbsorption) {
							CDatatype* datatype = concept->getDatatype();
							if (datatype && datatype->isBasicDatatype()) {

								cint64 triggerComplexity = 1;
								CConcept* valueTriggerConcept = createTriggerConcept(branchTiggerCreation);
								CConceptTriggerLinker* valueTrigger = createTriggerLinker();
								valueTrigger->initConceptTriggerLinker(valueTriggerConcept,triggerComplexity);
								CDatatypeValueSpaceConceptTriggerLinker* valueSpaceConceptTriggerLinker = createValueSpaceConceptTriggerLinker();
								valueSpaceConceptTriggerLinker->initConceptTrigger(valueTriggerConcept);


								if (backPropConcept) {
									CConcept* datatypeImpConcept = createDatatypeImplication(valueTriggerConcept, datatype);
									addUnfoldingConceptForConcept(backPropTriggerConcept, datatypeImpConcept, false);
								}  else {


									CDatatypeValueSpaceTriggers* valueSpaceTrigger = nullptr;

									CDatatypeValueSpaceType* valueSpaceType = datatype->getValueSpaceType();

									if (valueSpaceType) {
										CDatatypeValueSpacesTriggers* valueSpacesTriggers = mMBox->getValueSpacesTriggers(true);
										CDatatypeValueSpaceCompareTriggers* compareValueSpaceConceptTrigger = (CDatatypeValueSpaceCompareTriggers*)valueSpacesTriggers->getValueSpaceTriggers(valueSpaceType);
										if (compareValueSpaceConceptTrigger) {
											compareValueSpaceConceptTrigger->addValueSpaceConceptTrigger(valueSpaceConceptTriggerLinker);
										}
									}
								}

								triggers = valueTrigger->append(triggers);
							} else {

								CConceptTriggerLinker* andTriggers = nullptr;
								for (CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker; opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
									CConcept* opConcept = opConLinkerIt->getData();
									bool opNegated = opConLinkerIt->isNegated()^negated;

									if (mConceptPartialAbsorbableHash.value(TConceptNegationPair(opConcept,opNegated),false) || mConceptTotalAbsorbableHash.value(TConceptNegationPair(opConcept,opNegated),false)) {
										CConceptTriggerLinker* newTriggers = copyTriggerLinkers(getPartialTriggersForConcept(opConcept,opNegated, backPropTriggerConcept));
										if (newTriggers) {
											CConceptTriggerLinker* implTriggers = getImplicationTriggeredConceptForTriggers(newTriggers, backPropImpConceptPointer);
											if (backPropConcept) {
												addUnfoldingConceptForConcept(backPropTriggerConcept, backPropImpConcept, false);
											}
											andTriggers = newTriggers->append(andTriggers);
										}
									}
								}
								if (andTriggers) {
									triggers = andTriggers->append(triggers);
								}
							}
						}



					} else if (!negated && (opCode == CCDATARESTRICTION)) {
						if (mConfDatatypeAbsorption) {
							CDatatype* datatype = concept->getDatatype();
							if (datatype && datatype->isBasicDatatype()) {
								cint64 dataRestrictionCode = parm;

								cint64 triggerComplexity = 1;
								CConcept* valueTriggerConcept = createTriggerConcept(branchTiggerCreation);
								CConceptTriggerLinker* valueTrigger = createTriggerLinker();
								valueTrigger->initConceptTriggerLinker(valueTriggerConcept, triggerComplexity);
								CDatatypeValueSpaceConceptTriggerLinker* valueSpaceConceptTriggerLinker = createValueSpaceConceptTriggerLinker();
								valueSpaceConceptTriggerLinker->initConceptTrigger(valueTriggerConcept);
								CDataLiteral* dataLiteral = concept->getDataLiteral();

								if (backPropConcept) {									
									CConcept* datatypeImpConcept = createDataRestrictionImplication(valueTriggerConcept, datatype, dataLiteral, dataRestrictionCode);
									addUnfoldingConceptForConcept(backPropTriggerConcept, datatypeImpConcept, false);
								} else {

									CDatatypeValueSpacesTriggers* valueSpacesTriggers = mMBox->getValueSpacesTriggers(true);

									CDatatype::DATATYPE_BASIC_TYPE basicDatatype = datatype->getBasicDatatypeType();
									if (basicDatatype == CDatatype::DBT_REAL) {
										CDatatypeValueSpaceRealTriggers* realValueSpaceConceptTrigger = valueSpacesTriggers->getRealValueSpaceTriggers();
										if (dataRestrictionCode == CDFC_RATIONAL) {
											realValueSpaceConceptTrigger->addRationalConceptTrigger(valueSpaceConceptTriggerLinker);
										} else if (dataRestrictionCode == CDFC_DECIMAL) {
											realValueSpaceConceptTrigger->addDecimalConceptTrigger(valueSpaceConceptTriggerLinker);
										} else if (dataRestrictionCode == CDFC_INTEGER) {
											realValueSpaceConceptTrigger->addIntegerConceptTrigger(valueSpaceConceptTriggerLinker);
										}
									}
									if (basicDatatype == CDatatype::DBT_STRING) {
										CDatatypeValueSpaceStringTriggers* stringValueSpaceConceptTrigger = valueSpacesTriggers->getStringValueSpaceTriggers();
										if (dataRestrictionCode == CDFC_STRING) {
											stringValueSpaceConceptTrigger->addNonLanguageTagConceptTrigger(valueSpaceConceptTriggerLinker);
										}
									}
									if (dataLiteral) {
										CDataLiteralValue* dataLitValue = dataLiteral->getDataLiteralValue();
										if (dataLitValue) {
											CDataLiteralCompareValue* compareDataLitValue = dynamic_cast<CDataLiteralCompareValue*>(dataLitValue);
											if (compareDataLitValue) {

												CDatatypeValueSpaceType* valueSpaceType = datatype->getValueSpaceType();

												if (valueSpaceType) {
													CDatatypeValueSpaceCompareTriggers* compareValueSpaceConceptTrigger = (CDatatypeValueSpaceCompareTriggers*)valueSpacesTriggers->getValueSpaceTriggers(valueSpaceType);

													if (compareValueSpaceConceptTrigger) {
														bool minRestriction = false;
														bool inclusive = false;

														if (dataRestrictionCode == CDFC_MIN_EXCLUSIVE) {
															minRestriction = true;
															inclusive = false;
														} else if (dataRestrictionCode == CDFC_MIN_INCLUSIVE) {
															minRestriction = true;
															inclusive = true;
														} else if (dataRestrictionCode == CDFC_MAX_EXCLUSIVE) {
															minRestriction = false;
															inclusive = false;
														} else if (dataRestrictionCode == CDFC_MAX_INCLUSIVE) {
															minRestriction = false;
															inclusive = true;
														}

														compareValueSpaceConceptTrigger->addMinMaxValueConceptTrigger(compareDataLitValue, minRestriction, inclusive, valueSpaceConceptTriggerLinker);
													}
												}
											}

										}
									}
								}
								triggers = valueTrigger->append(triggers);
							}
						}
					}


					mConceptTriggerLinkerHash.insert(conNegPair,triggers);
					if (backPropTriggerConcept) {
						mBackPropActivationConHash.insert(conNegPair, backPropTriggerConcept);
					}
				}
				return triggers;
			}




			CDatatypeValueSpaceConceptTriggerLinker* CTriggeredImplicationBinaryAbsorberPreProcess::createValueSpaceConceptTriggerLinker() {
				CDatatypeValueSpaceConceptTriggerLinker* valueSpaceConceptTriggerLinker = CObjectAllocator<CDatatypeValueSpaceConceptTriggerLinker>::allocateAndConstruct(mMemMan);
				return valueSpaceConceptTriggerLinker;
			}



			bool CTriggeredImplicationBinaryAbsorberPreProcess::addAbsorbableDisjunctCandidates(CConcept* orConcept, bool negated, QList<TConceptNegationPair>& list) {
				bool absorbableDisjuncts = false;
				cint64 opCode = orConcept->getOperatorCode();
				if (!negated && opCode == CCOR || negated && (opCode == CCAND || opCode == CCEQ)) {
					CSortedNegLinker<CConcept*>* opConLinkerIt = orConcept->getOperandList();
					while (opConLinkerIt) {
						CConcept* opConcept = opConLinkerIt->getData();
						bool opNegated = opConLinkerIt->isNegated()^negated;

						cint64 opOpCode = opConcept->getOperatorCode();
						if (!opNegated && opOpCode == CCOR || opNegated && (opOpCode == CCAND)) {
							absorbableDisjuncts |= addAbsorbableDisjunctCandidates(opConcept,opNegated,list);
						} else {
							list.append(TConceptNegationPair(opConcept,opNegated));
							absorbableDisjuncts = true;
						}

						opConLinkerIt = opConLinkerIt->getNext();
					}
				}
				return absorbableDisjuncts;
			}

			QList<CTriggeredImplicationBinaryAbsorberPreProcess::TConceptNegationPair> CTriggeredImplicationBinaryAbsorberPreProcess::splitAbsorbableDisjuncts(QList<TConceptNegationPair>& list) {
				QList<TConceptNegationPair> absorbableList;
				cint64 count = list.count();
				QHash<CConcept*,TOccuredAbsorbablePair> conceptEqConAbsorbed;
				for (cint64 nr = 0; nr < count; ++nr) {
					TConceptNegationPair conNegPair(list.takeFirst());
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					if (isConceptImplicationTriggerable(concept,!negated,&conceptEqConAbsorbed)) {
						absorbableList.append(conNegPair);
					} else {
						list.append(conNegPair);
					}
				}
				return absorbableList;
			}

			QList<CTriggeredImplicationBinaryAbsorberPreProcess::TConceptNegationPair> CTriggeredImplicationBinaryAbsorberPreProcess::getPartialAbsorbableDisjuncts(QList<TConceptNegationPair>& list) {
				QList<TConceptNegationPair> absorbableList;
				cint64 count = list.count();
				QSet<CConcept*> conceptEqConAbsorbed;
				for (QList<TConceptNegationPair>::const_iterator it = list.constBegin(), itEnd = list.constEnd(); it != itEnd; ++it) {
					TConceptNegationPair conNegPair(*it);
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					if (isConceptPartialImplicationTriggerable(concept,!negated,&conceptEqConAbsorbed)) {
						absorbableList.append(conNegPair);
					}
				}
				return absorbableList;
			}




			QList<CTriggeredImplicationBinaryAbsorberPreProcess::TConceptNegationPair> CTriggeredImplicationBinaryAbsorberPreProcess::getAssuringAbsorbableDisjuncts(QList<TConceptNegationPair>& list) {
				QList<TConceptNegationPair> absorbableList;
				cint64 count = list.count();
				QSet<CConcept*> conceptEqConAbsorbed;
				for (QList<TConceptNegationPair>::const_iterator it = list.constBegin(), itEnd = list.constEnd(); it != itEnd; ++it) {
					TConceptNegationPair conNegPair(*it);
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					if (isConceptAssuringImplicationTriggerable(concept, !negated, &conceptEqConAbsorbed)) {
						absorbableList.append(conNegPair);
					}
				}
				return absorbableList;
			}





			bool CTriggeredImplicationBinaryAbsorberPreProcess::isConceptAssuringImplicationTriggerable(CConcept* concept, bool negated, QSet<CConcept*>* conceptEqConAbsorbed) {
				TConceptNegationPair conNegPair(concept, negated);
				if (mConceptAssuringAbsorbableHash.contains(conNegPair)) {
					return mConceptAssuringAbsorbableHash.value(conNegPair);
				}
				cint64 opCode = concept->getOperatorCode();
				cint64 parm = concept->getParameter();
				cint64 opCount = concept->getOperandCount();
				CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();

				bool absorbable = false;

				QHash<CConcept*, TOccuredAbsorbablePair> compConceptEqConAbsorbed;
				if (isConceptImplicationTriggerable(concept, negated, &compConceptEqConAbsorbed)) {
					absorbable = true;
				} else {


					if (!negated && (opCode == CCSOME || opCode == CCAQSOME) || negated && (opCode == CCALL)) {
						absorbable = true;
						CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
						while (opConLinkerIt && absorbable) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegated = opConLinkerIt->isNegated() ^ negated;
							if (!isConceptAssuringImplicationTriggerable(opConcept, opNegated, conceptEqConAbsorbed)) {
								absorbable = false;
							}
							opConLinkerIt = opConLinkerIt->getNext();
						}
					} else  if (!negated && (opCode == CCAND) || negated && opCode == CCOR) {
						// AND, at least one operand has to be triggerable
						if (opCount >= 1) {
							CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
							absorbable = true;
							while (opConLinkerIt && absorbable) {
								CConcept* opConcept = opConLinkerIt->getData();
								bool opNegated = opConLinkerIt->isNegated() ^ negated;
								if (!isConceptAssuringImplicationTriggerable(opConcept, opNegated, conceptEqConAbsorbed)) {
									absorbable = false;
								}
								opConLinkerIt = opConLinkerIt->getNext();
							}
						}
					} else if (!negated && (opCode == CCDATALITERAL)) {
						if (mConfDatatypeAbsorption) {
							CDataLiteral* dataLiteral = concept->getDataLiteral();
							if (dataLiteral) {
								CDataLiteralValue* dataLitValue = dataLiteral->getDataLiteralValue();
								if (dataLitValue) {
									if (dataLitValue->getDataValueType() == CDataLiteralValue::DLVT_REAL) {
										absorbable = true;
									}
									if (dataLitValue->getDataValueType() == CDataLiteralValue::DLVT_STRING) {
										absorbable = true;
									}
									if (dataLitValue->getDataValueType() == CDataLiteralValue::DLVT_BOOLEAN) {
										absorbable = true;
									}
									if (dataLitValue->getDataValueType() == CDataLiteralValue::DLVT_DOUBLE) {
										absorbable = true;
									}
									if (dataLitValue->getDataValueType() == CDataLiteralValue::DLVT_FLOAT) {
										absorbable = true;
									}
									if (dataLitValue->getDataValueType() == CDataLiteralValue::DLVT_IRI) {
										absorbable = true;
									}
									if (dataLitValue->getDataValueType() == CDataLiteralValue::DLVT_XML) {
										absorbable = true;
									}
									if (dataLitValue->getDataValueType() == CDataLiteralValue::DLVT_HEXBINARY) {
										absorbable = true;
									}
									if (dataLitValue->getDataValueType() == CDataLiteralValue::DLVT_BASE64BINARY) {
										absorbable = true;
									}
									if (dataLitValue->getDataValueType() == CDataLiteralValue::DLVT_DATETIME) {
										absorbable = true;
									}
								}
							}
						}


					} else {
						absorbable = false;
					}
				}
				mConceptAssuringAbsorbableHash.insert(conNegPair, absorbable);
				return absorbable;
			}






			CConceptTriggerLinker* CTriggeredImplicationBinaryAbsorberPreProcess::getAssuringTriggersForConcept(CConcept* concept, bool negated) {
				CConceptTriggerLinker* triggers = nullptr;

				bool branchTiggerCreation = false;
				TConceptNegationPair conNegPair(concept, negated);
				triggers = mAssuringConceptTriggerLinkerHash.value(conNegPair);
				if (!triggers) {

					cint64 opCode = concept->getOperatorCode();
					cint64 parm = concept->getParameter();
					cint64 opCount = concept->getOperandCount();
					CRole* role = concept->getRole();
					CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();


					QHash<CConcept*, TOccuredAbsorbablePair> compConceptEqConAbsorbed;
					if (isConceptImplicationTriggerable(concept, negated, &compConceptEqConAbsorbed)) {

						triggers = copyTriggerLinkers(getTriggersForConcept(concept, negated));


					} else if (!negated && (opCode == CCATOM || opCode == CCSUB || opCode == CCTOP || opCode == CCIMPLTRIG)) {
						CConceptTriggerLinker* newTrigger = createTriggerLinker();
						newTrigger->initConceptTriggerLinker(concept, 1);
						triggers = newTrigger->append(triggers);

					} else if (!negated && (opCode == CCSOME) || negated && (opCode == CCALL)) {
						CConcept* propagatedTriggerConcept = nullptr;
						cint64 triggerComplexity = 1;
						bool negateOperands = negated && (opCode == CCSOME || opCode == CCSELF || opCode == CCALL);
						CConceptTriggerLinker* allTriggers = nullptr;
						for (CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker; opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegated = opConLinkerIt->isNegated() ^ negateOperands;
							CConceptTriggerLinker* newTrigger = copyTriggerLinkers(getAssuringTriggersForConcept(opConcept, opNegated));
							allTriggers = newTrigger->append(allTriggers);
						}
						if (allTriggers) {
							propagatedTriggerConcept = createTriggerConcept(branchTiggerCreation);
							CConcept* nextLevelTriggerConcept = createTriggerPropagationConcept(propagatedTriggerConcept, role, branchTiggerCreation);
							if (allTriggers->hasNext()) {
								// requires a generation of an implication concept
								CConceptTriggerLinker* implTriggeredConcept = getImplicationTriggeredConceptForTriggers(allTriggers, nullptr);
								triggerComplexity += implTriggeredConcept->getTriggerComplexity();
								addUnfoldingConceptForConcept(implTriggeredConcept->getTriggerConcept(), nextLevelTriggerConcept, false);
							} else {
								triggerComplexity += allTriggers->getTriggerComplexity();
								addUnfoldingConceptForConcept(allTriggers->getTriggerConcept(), nextLevelTriggerConcept, false);
								releaseTriggerLinkers(allTriggers);
							}
						} else {
							CRole* locatedRole = CConceptRoleIndividualLocator::getLocatedRole(role, mOnto);
							propagatedTriggerConcept = getRoleDomainTriggerConcept(locatedRole);
						}
						CConceptTriggerLinker* newTrigger = createTriggerLinker();
						newTrigger->initConceptTriggerLinker(propagatedTriggerConcept, triggerComplexity);
						triggers = newTrigger->append(triggers);


					} else if (negated && (opCode == CCOR) || !negated && (opCode == CCAND || opCode == CCEQ)) {
						// AND, collect triggers from operands
						CConceptTriggerLinker* andTriggers = nullptr;
						for (CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker; opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegated = opConLinkerIt->isNegated() ^ negated;

							CConceptTriggerLinker* newTriggers = copyTriggerLinkers(getAssuringTriggersForConcept(opConcept, opNegated));
							andTriggers = newTriggers->append(andTriggers);
						}
						triggers = andTriggers->append(triggers);

					} else if (!negated && (opCode == CCDATALITERAL)) {
						if (mConfDatatypeAbsorption) {
							CDataLiteral* dataLiteral = concept->getDataLiteral();
							if (dataLiteral) {
								CDataLiteralValue* dataLitValue = dataLiteral->getDataLiteralValue();
								if (dataLitValue) {

									cint64 triggerComplexity = 2;
									CConcept* valueTriggerConcept = createTriggerConcept(branchTiggerCreation);
									CConceptTriggerLinker* valueTrigger = createTriggerLinker();
									valueTrigger->initConceptTriggerLinker(valueTriggerConcept, triggerComplexity);
									CDatatypeValueSpaceConceptTriggerLinker* valueSpaceConceptTriggerLinker = createValueSpaceConceptTriggerLinker();
									valueSpaceConceptTriggerLinker->initConceptTrigger(valueTriggerConcept);


									CDatatypeValueSpaceTriggers* valueSpaceTrigger = nullptr;
									CDataLiteralValue* dataLitValue = dataLiteral->getDataLiteralValue();
									if (dataLitValue) {
										CDataLiteralCompareValue* compareDataLitValue = dynamic_cast<CDataLiteralCompareValue*>(dataLitValue);
										if (compareDataLitValue) {
											CDatatypeValueSpaceType* valueSpaceType = nullptr;
											CDataLiteralValue::DATA_LITERAL_VALUE_TYPE dataLiteralValueType = compareDataLitValue->getDataValueType();
											CDatatypeValueSpaceTypes* valueSpaceTypes = CDatatypeValueSpaceTypes::getValueSpaceTypes();
											if (dataLiteralValueType == CDataLiteralValue::DLVT_REAL) {
												valueSpaceType = valueSpaceTypes->getValueSpaceRealType();
											} else if (dataLiteralValueType == CDataLiteralValue::DLVT_STRING) {
												valueSpaceType = valueSpaceTypes->getValueSpaceStringType();
											} else if (dataLiteralValueType == CDataLiteralValue::DLVT_BOOLEAN) {
												valueSpaceType = valueSpaceTypes->getValueSpaceBooleanType();
											} else if (dataLiteralValueType == CDataLiteralValue::DLVT_DOUBLE) {
												valueSpaceType = valueSpaceTypes->getValueSpaceDoubleType();
											} else if (dataLiteralValueType == CDataLiteralValue::DLVT_FLOAT) {
												valueSpaceType = valueSpaceTypes->getValueSpaceFloatType();
											} else if (dataLiteralValueType == CDataLiteralValue::DLVT_IRI) {
												valueSpaceType = valueSpaceTypes->getValueSpaceIRIType();
											} else if (dataLiteralValueType == CDataLiteralValue::DLVT_XML) {
												valueSpaceType = valueSpaceTypes->getValueSpaceXMLType();
											} else if (dataLiteralValueType == CDataLiteralValue::DLVT_HEXBINARY) {
												valueSpaceType = valueSpaceTypes->getValueSpaceBinaryHexDataType();
											} else if (dataLiteralValueType == CDataLiteralValue::DLVT_BASE64BINARY) {
												valueSpaceType = valueSpaceTypes->getValueSpaceBinaryBase64DataType();
											} else if (dataLiteralValueType == CDataLiteralValue::DLVT_DATETIME) {
												valueSpaceType = valueSpaceTypes->getValueSpaceDateTimeType();
											}

											if (valueSpaceType) {
												CDatatypeValueSpacesTriggers* valueSpacesTriggers = mMBox->getValueSpacesTriggers(true);
												CDatatypeValueSpaceCompareTriggers* compareValueSpaceConceptTrigger = (CDatatypeValueSpaceCompareTriggers*)valueSpacesTriggers->getValueSpaceTriggers(valueSpaceType);
												if (compareValueSpaceConceptTrigger) {
													compareValueSpaceConceptTrigger->addCompleteValueConceptTrigger(compareDataLitValue, valueSpaceConceptTriggerLinker);
												}
											}
										}
									}
									
									triggers = valueTrigger->append(triggers);

								}
							}
						}
					}

					mAssuringConceptTriggerLinkerHash.insert(conNegPair, triggers);
				}
				return triggers;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
