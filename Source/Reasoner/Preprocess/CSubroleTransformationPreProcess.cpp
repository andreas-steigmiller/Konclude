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

#include "CSubroleTransformationPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CSubroleTransformationPreProcess::CSubroleTransformationPreProcess() {
			}


			CSubroleTransformationPreProcess::~CSubroleTransformationPreProcess() {
			}


			CConcreteOntology *CSubroleTransformationPreProcess::preprocess(CConcreteOntology *ontology, CPreProcessContext* context) {
				if (ontology) {
					mTBox = ontology->getDataBoxes()->getTBox();
					mABox = ontology->getDataBoxes()->getABox();
					mRBox = ontology->getDataBoxes()->getRBox();
					mMBox = ontology->getDataBoxes()->getMBox();



					mRolesVector = mRBox->getRoleVector();
					mConceptVector = mTBox->getConceptVector();
					mIndiVec = mABox->getIndividualVector();

					mMemMan = ontology->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();
					mOntology = ontology;

					mTopObjectRole = nullptr;
					mBottomObjectRole = nullptr;

					mTopDataRole = nullptr;
					mBottomDataRole = nullptr;

					QSet<CRole*> mProcessedRoleSet;
					QSet<cint64> locRoleTagSet;
					QList<cint64> subRoleUpdateTagList;

					COntologyBuildConstructFlags* construcFlags = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getBuildConstructFlags();
					bool topObjectRuleUsed = construcFlags->isTopObjectRoleUsed();
					bool bottomObjectRuleUsed = construcFlags->isBottomObjectRoleUsed();

					if (topObjectRuleUsed) {
						mRoleChainVector = mRBox->getRoleChainVector();
						makeUniversalSuperRole();
					}
					if (bottomObjectRuleUsed) {
						makeBottomObjectRoleDomain();
					}

					bool topDataRuleUsed = construcFlags->isTopDataRoleUsed();
					bool bottomDataRuleUsed = construcFlags->isBottomDataRoleUsed();

					if (topDataRuleUsed) {
						makeDataSuperRole();
					}
					if (bottomDataRuleUsed) {
						makeBottomDataRoleDomain();
					}

					qint64 itemCounts = mRolesVector->getItemCount();
					for (qint64 i = 0; i < itemCounts; ++i) {
						CRole *role = mRolesVector->getLocalData(i);
						if (role) {
							locRoleTagSet.insert(role->getRoleTag());
							subRoleUpdateTagList.append(role->getRoleTag());
							if (!mProcessedRoleSet.contains(role)) {
								// update all direct super roles 

								QSet< QPair<CRole*,bool> > inEqRoleSet;
								collectInverseEquivalentRoles(role,false,&inEqRoleSet);

								for (QSet< QPair<CRole*,bool> >::const_iterator it = inEqRoleSet.constBegin(), itEnd = inEqRoleSet.constEnd(); it != itEnd; ++it) {
									QPair<CRole*,bool> roleNegPair(*it);
									CRole* upRole = roleNegPair.first;
									bool upNeg = roleNegPair.second;
									if (!mProcessedRoleSet.contains(upRole)) {
										mProcessedRoleSet.insert(upRole);
										updateDirectSuperRoles(upRole,upNeg,&inEqRoleSet);
									}
								}
							}
						}
					}

					QHash<qint64,qint64> roleTagSubRoleTagHash;


					for (qint64 i = 0; i < itemCounts; ++i) {
						CRole *role = mRolesVector->getData(i);
						if (role) {
							cint64 roleTag = role->getRoleTag();
							CSortedNegLinker<CRole*>* superRoleLinkerIt = role->getSuperRoleList();
							while (superRoleLinkerIt) {
								CRole* superRole = superRoleLinkerIt->getData();
								cint64 superRoleTag = superRole->getRoleTag();
								roleTagSubRoleTagHash.insertMulti(superRoleTag,roleTag);
								superRoleLinkerIt = superRoleLinkerIt->getNext();
							}
						}
					}

					FOREACHIT (cint64 locRoleTag, locRoleTagSet) {
						if (!locRoleTagSet.contains(locRoleTag)) {
							locRoleTagSet.insert(locRoleTag);
							subRoleUpdateTagList.append(locRoleTag);
						}
						FOREACHIT (cint64 subRoleTag, roleTagSubRoleTagHash.values(locRoleTag)) {
							if (!locRoleTagSet.contains(subRoleTag)) {
								locRoleTagSet.insert(subRoleTag);
								subRoleUpdateTagList.append(subRoleTag);
							}
						}
					}

					while (!subRoleUpdateTagList.isEmpty()) {
						cint64 updateTag = subRoleUpdateTagList.takeFirst();
						CRole *role = mRolesVector->getData(updateTag);
						role = getLocalizedRole(role);

						if (role) {
							// update indirect super role list
							QSet< QPair<CRole*,bool> > singleSuperRoleAddSet;
							singleSuperRoleAddSet.insert(QPair<CRole*,bool>(role,false));
							CSortedNegLinker<CRole *> *superRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemMan);
							superRoleLinker->init(role,false);
							role->setIndirectSuperRoleLinker(superRoleLinker);

							addIndirectSuperRoles(false,role,role->getSuperRoleList(),&singleSuperRoleAddSet);

							FOREACHIT (cint64 subRoleTag, roleTagSubRoleTagHash.values(updateTag)) {
								if (!locRoleTagSet.contains(subRoleTag)) {
									locRoleTagSet.insert(subRoleTag);
									subRoleUpdateTagList.append(subRoleTag);
								}
							}

						}
					}

					for (qint64 i = 0; i < itemCounts; ++i) {
						CRole *role = mRolesVector->getLocalData(i);
						if (role) {
							if (role->getRoleChainSuperSharingLinker() != nullptr) {
								role->setRoleComplexity(true);
								// all super roles must be complex, too

								CSortedNegLinker<CRole*>* superRoleLinkerIt = role->getIndirectSuperRoleList();
								while (superRoleLinkerIt) {
									CRole* superRole = superRoleLinkerIt->getData();
									superRole = getLocalizedRole(superRole);
									superRole->setRoleComplexity(true);
									superRoleLinkerIt = superRoleLinkerIt->getNext();
								}
							}
						}
					}

					if (topObjectRuleUsed && mABox->getIndividualCount() > 0 && hasUniversalRolePropagation()) {
						makeUniversalConnectionIndividual();
					}
				}

				return ontology;
			}

			void CSubroleTransformationPreProcess::makeUniversalConnectionIndividual() {
				cint64 univConnIndiID = 0;
				CIndividual* universalConnIndi = nullptr;
				CConcept* universalConnNominalValueCon = nullptr;
				CConcept* universalConnNominalCon = nullptr;
				if (!mABox->hasUniversalConnectionIndividual()) {
					univConnIndiID = mIndiVec->getItemCount();
					mABox->setUniversalConnectionIndividualID(univConnIndiID);
					mABox->setHasUniversalConnectionIndividual(true);
					universalConnIndi = CObjectAllocator< CIndividual >::allocateAndConstruct(mMemMan);
					universalConnIndi->initIndividual(univConnIndiID);
					mIndiVec->setData(univConnIndiID,universalConnIndi);

					cint64 valueConTag = mConceptVector->getItemCount();
					universalConnNominalValueCon = CObjectAllocator< CConcept >::allocateAndConstruct(mMemMan);
					universalConnNominalValueCon->initConcept();
					universalConnNominalValueCon->setConceptTag(valueConTag);
					universalConnNominalValueCon->setOperatorCode(CCVALUE);
					universalConnNominalValueCon->setNominalIndividual(universalConnIndi);
					universalConnNominalValueCon->setRole(mRolesVector->getData(1));
					mConceptVector->setData(valueConTag,universalConnNominalValueCon);
					mTBox->setUniversalConnectionNominalValueConcept(universalConnNominalValueCon);


					cint64 nominalConTag = mConceptVector->getItemCount();
					universalConnNominalCon = CObjectAllocator< CConcept >::allocateAndConstruct(mMemMan);
					universalConnNominalCon->initConcept();
					universalConnNominalCon->setConceptTag(nominalConTag);
					universalConnNominalCon->setOperatorCode(CCNOMINAL);
					universalConnNominalCon->setNominalIndividual(universalConnIndi);
					mConceptVector->setData(nominalConTag,universalConnNominalCon);
					universalConnIndi->setIndividualNominalConcept(universalConnNominalCon);

				}
			}


			void CSubroleTransformationPreProcess::makeBottomObjectRoleDomain() {
				mBottomObjectRole = mRolesVector->getData(mRBox->getBottomObjectRoleIndex());
				makeBottomRoleDomain(mBottomObjectRole);
			}


			void CSubroleTransformationPreProcess::makeBottomDataRoleDomain() {
				mBottomDataRole = mRolesVector->getData(mRBox->getBottomDataRoleIndex());
				makeBottomRoleDomain(mBottomDataRole);
			}


			void CSubroleTransformationPreProcess::makeBottomRoleDomain(CRole* role) {
				bool foundBottom = false;
				for (CSortedNegLinker<CConcept*>* domainConIt = role->getDomainConceptList(); !foundBottom && domainConIt; domainConIt = domainConIt->getNext()) {
					CConcept* domainConcept = domainConIt->getData();
					bool domainConNegation = domainConIt->isNegated();
					if (domainConNegation && domainConcept->getOperatorCode() == CCTOP) {
						foundBottom = true;
					} else if (!domainConNegation && domainConcept->getOperatorCode() == CCBOTTOM) {
						foundBottom = true;
					}
				}
				if (!foundBottom) {
					role = getLocalizedRole(role);
					CSortedNegLinker<CConcept*>* domainConLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
					domainConLinker->init(mTBox->getTopConcept(),true);
					role->addDomainConceptLinker(domainConLinker);
				}
			}


			void CSubroleTransformationPreProcess::makeUniversalSuperRole() {
				mOntology->getDataBoxes()->getExpressionDataBoxMapping()->getBuildConstructFlags()->setComplexRoleUsed();
				mTopObjectRole = getLocalizedRole(mRolesVector->getData(mRBox->getTopObjectRoleIndex()));
				if (!mTopObjectRole->isTransitive()) {
					mTopObjectRole->setTransitive(true);
					cint64 roleChainCount = mRoleChainVector->getItemCount();
					CRoleChain* roleChain = CObjectAllocator< CRoleChain >::allocateAndConstruct(mMemMan);
					roleChain->initRoleChain();
					CXLinker<CRole*>* roleLinker1 = CObjectAllocator< CXLinker<CRole*> >::allocateAndConstruct(mMemMan);
					CXLinker<CRole*>* roleLinker2 = CObjectAllocator< CXLinker<CRole*> >::allocateAndConstruct(mMemMan);
					roleLinker1->initLinker(mTopObjectRole);
					roleLinker2->initLinker(mTopObjectRole);
					roleChain->appendRoleChainLinker(roleLinker1);
					roleChain->appendRoleChainLinker(roleLinker2);
					roleChain->initTag(roleChainCount);
					mRoleChainVector->setData(roleChainCount,roleChain);

					CXLinker<CRoleChain*>* superRoleChainLinker = CObjectAllocator< CXLinker<CRoleChain*> >::allocateAndConstruct(mMemMan);
					superRoleChainLinker->initLinker(roleChain);
					mTopObjectRole->addRoleChainSuperSharingLinker(superRoleChainLinker);
					CXLinker<CRoleChain*>* subRoleChainLinker = CObjectAllocator< CXLinker<CRoleChain*> >::allocateAndConstruct(mMemMan);
					subRoleChainLinker->initLinker(roleChain);
					mTopObjectRole->addRoleChainSubSharingLinker(subRoleChainLinker);
				}
				if (!mTopObjectRole->isSymmetric()) {
					mTopObjectRole->setSymmetric(true);

					CSortedNegLinker<CRole*>* roleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemMan);
					roleLinker->init(mTopObjectRole,true);
					mTopObjectRole->addInverseRoleLinker(roleLinker);
				}
				cint64 roleCount = mRolesVector->getItemCount();
				for (cint64 roleIdx = 0; roleIdx < roleCount; ++roleIdx) {
					CRole* role = mRolesVector->getData(roleIdx);
					if (role && role->isObjectRole() && !role->hasSuperRoleTag(mTopObjectRole->getRoleTag())) {
						role = getLocalizedRole(role);

						CSortedNegLinker<CRole*>* roleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemMan);
						roleLinker->init(mTopObjectRole,true);
						mTopObjectRole->addSuperRoleLinker(roleLinker);
					}
				}
				// make reflexive -> add \exists u.SELF to TOP
				bool topConLocal = false;
				CConcept* topConcept = mConceptVector->getData(1,&topConLocal);
				if (!topConLocal) {
					topConcept = CConceptRoleIndividualLocator::getLocatedConcept(topConcept,mOntology);
				}
				bool hasTopExistsUniversalSelf = false;
				for (CSortedNegLinker<CConcept*>* opLinker = topConcept->getOperandList(); !hasTopExistsUniversalSelf && opLinker; opLinker = opLinker->getNext()) {
					if (!opLinker->isNegated()) {
						CConcept* opConcept = opLinker->getData();
						if (opConcept->getOperatorCode() == CCSELF && opConcept->getRole()->getRoleTag() == mTopObjectRole->getRoleTag()) {
							hasTopExistsUniversalSelf = true;
						}
					}
				}
				if (!hasTopExistsUniversalSelf) {
					CConcept* univSelfCon = CObjectAllocator< CConcept >::allocateAndConstruct(mMemMan);
					cint64 conCount = mConceptVector->getItemCount();
					univSelfCon->initConcept();
					univSelfCon->setConceptTag(conCount);
					univSelfCon->setRole(mTopObjectRole);
					univSelfCon->setOperatorCode(CCSELF);
					mConceptVector->setData(conCount,univSelfCon);

					CSortedNegLinker<CConcept*>* topOpLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
					topOpLinker->init(univSelfCon,false);
					topConcept->addOperandLinker(topOpLinker);
					topConcept->incOperandCount(1);
				}
				
			}





			void CSubroleTransformationPreProcess::makeDataSuperRole() {
				mTopDataRole = getLocalizedRole(mRolesVector->getData(mRBox->getTopDataRoleIndex()));
				cint64 roleCount = mRolesVector->getItemCount();
				for (cint64 roleIdx = 0; roleIdx < roleCount; ++roleIdx) {
					CRole* role = mRolesVector->getData(roleIdx);
					if (role && role->isDataRole() && !role->hasSuperRoleTag(mTopDataRole->getRoleTag())) {
						role = getLocalizedRole(role);

						CSortedNegLinker<CRole*>* roleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemMan);
						roleLinker->init(mTopDataRole,true);
						mTopDataRole->addSuperRoleLinker(roleLinker);
					}
				}
			}



			CIndividual* CSubroleTransformationPreProcess::getLocalizedIndividual(CIndividual* individual) {
				cint64 indiID = individual->getIndividualID();
				if (!mIndiVec->hasLocalData(indiID)) {
					individual = CConceptRoleIndividualLocator::getLocatedIndividual(mIndiVec->getLocalData(indiID),mOntology);
				} else {
					individual = mIndiVec->getLocalData(indiID);
				}
				return individual;
			}

			CRole* CSubroleTransformationPreProcess::getLocalizedRole(CRole* role) {
				cint64 roleTag = role->getRoleTag();
				if (!mRolesVector->hasLocalData(roleTag)) {
					role = CConceptRoleIndividualLocator::getLocatedRole(role,mOntology);
				} else {
					role = mRolesVector->getLocalData(roleTag);
				}
				return role;
			}



			CSubroleTransformationPreProcess* CSubroleTransformationPreProcess::updateDirectSuperRoles(CRole* role, bool negated, QSet< QPair<CRole*,bool> >* roleSet) {
				role = getLocalizedRole(role);
				for (QSet< QPair<CRole*,bool> >::const_iterator it = roleSet->constBegin(), itEnd = roleSet->constEnd(); it != itEnd; ++it) {
					QPair<CRole*,bool> roleNegPair(*it);
					CRole* upRole = roleNegPair.first;
					bool upNeg = roleNegPair.second^negated;
					cint64 upRoleTag = upRole->getRoleTag();
					if (upRole != role || upNeg) {
						if (!role->hasSuperRoleTag(upRoleTag,upNeg)) {
							CSortedNegLinker<CRole*>* superRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemMan);
							superRoleLinker->init(upRole,upNeg);
							role->addSuperRoleLinker(superRoleLinker);
						}
					}
				}
				return this;
			}



			CSubroleTransformationPreProcess* CSubroleTransformationPreProcess::collectInverseEquivalentRoles(CRole* role, bool negated, QSet< QPair<CRole*,bool> >* roleSet) {
				if (!roleSet->contains(QPair<CRole*,bool>(role,negated))) {
					roleSet->insert(QPair<CRole*,bool>(role,negated));
					CSortedNegLinker<CRole*>* inEqRoleLinker = role->getInverseEquivalentRoleList();
					while (inEqRoleLinker) {
						CRole* inEqRole = inEqRoleLinker->getData();
						inEqRole = mRolesVector->getData(inEqRole->getRoleTag());
						bool inEqRoleNeg = inEqRoleLinker->isNegated();
						collectInverseEquivalentRoles(inEqRole,inEqRoleNeg^negated,roleSet);
						inEqRoleLinker = inEqRoleLinker->getNext();
					}
				}
				return this;
			}




			bool CSubroleTransformationPreProcess::hasUniversalRolePropagation() {
				QSet<CRole*> topSuperRoleSet;
				topSuperRoleSet.insert(mTopObjectRole);
				for (CSortedNegLinker<CRole*>* topSuperRoleIt = mTopObjectRole->getIndirectSuperRoleList(); topSuperRoleIt; topSuperRoleIt = topSuperRoleIt->getNext()) {
					CRole* topSuperRole = topSuperRoleIt->getData();
					topSuperRoleSet.insert(topSuperRole);
				}
				cint64 conCount = mConceptVector->getItemCount();
				for (cint64 conIdx = 0; conIdx < conCount; ++conIdx) {
					CConcept* concept = mConceptVector->getData(conIdx);
					if (concept && concept->getOperatorCode() != CCSELF) {
						CRole* role = concept->getRole();
						if (role && topSuperRoleSet.contains(role)) {
							return true;
						}
					}
				}
				return false;
			}


			void CSubroleTransformationPreProcess::addIndirectSuperRoles(bool inverse, CRole *modifingSubRole, CSortedNegLinker<CRole *> *superRoleLinker, QSet< QPair<CRole*,bool> > *singleSuperRoleAddSet) {

				CSortedNegLinker<CRole *> *superRoleIt = superRoleLinker;
				while (superRoleIt) {
					CRole *superRole = superRoleIt->getData();
					superRole = mRolesVector->getData(superRole->getRoleTag());
					bool inversed = superRoleIt->isNegated()^inverse;

					if (!singleSuperRoleAddSet->contains(QPair<CRole*,bool>(superRole,inversed))) {
						singleSuperRoleAddSet->insert(QPair<CRole*,bool>(superRole,inversed));

						CSortedNegLinker<CRole *> *nextSuperRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemMan);
						nextSuperRoleLinker->init(superRole,inversed);
						modifingSubRole->addIndirectSuperRoleLinker(nextSuperRoleLinker);

						addIndirectSuperRoles(inversed,modifingSubRole,superRole->getSuperRoleList(),singleSuperRoleAddSet);
					} 

					superRoleIt = (CSortedNegLinker<CRole *> *)superRoleIt->getNext();
				}
			}




		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
