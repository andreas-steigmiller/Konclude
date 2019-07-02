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

#ifndef KONCLUDE_REASONER_CONSISTISER_CTOTALLYONTOLOGYPRECOMPUTATIONITEM_H
#define KONCLUDE_REASONER_CONSISTISER_CTOTALLYONTOLOGYPRECOMPUTATIONITEM_H

// Libraries includes
#include <QAtomicPointer>

// Namespace includes
#include "ConsistiserSettings.h"
#include "CConsistenceObserver.h"
#include "CSaturationObserver.h"
#include "COntologyPrecomputationItem.h"
#include "CPrecomputationTestingStep.h"
#include "CSaturationConceptDataItem.h"


// Other includes
#include "Reasoner/Consistence/CConsistenceData.h"
#include "Reasoner/Consistence/CSaturationData.h"
#include "Reasoner/Consistence/CConsistence.h"

#include "Reasoner/Ontology/COntologyProcessingStepRequirement.h"
#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptSaturationReferenceLinkingData.h"
#include "Reasoner/Ontology/COntologyProcessingConceptSaturationRequirement.h"

#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"

#include "Reasoner/Query/CApproximatedSaturationCalculationJob.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Kernel::Task;
		using namespace Consistence;
		using namespace Ontology;
		using namespace Query;

		namespace Consistiser {


			typedef QPair<CConcept*,bool> TConceptNegPair;
			typedef QPair<CRole*,TConceptNegPair> TRoleConceptNegTriple;


			/*! 
			 *
			 *		\class		CTotallyOntologyPrecomputationItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CTotallyOntologyPrecomputationItem : public COntologyPrecomputationItem, public CConsistenceObserver, public CSaturationObserver {
				// public methods
				public:
					//! Constructor
					CTotallyOntologyPrecomputationItem();
					~CTotallyOntologyPrecomputationItem();

					CTotallyOntologyPrecomputationItem* initTotallyPrecomputationItem(CConcreteOntology* ontology, CConfigurationBase* config);

					virtual COntologyPrecomputationItem* addPrecomputationRequirement(COntologyProcessingRequirement* ontoRequirement);

					CTotallyOntologyPrecomputationItem* setConsistenceCheched(bool consistenceChecked);
					bool hasConsistenceCheched();

					CTotallyOntologyPrecomputationItem* setConsistenceCheckCreated(bool consistenceCheckCreated);
					bool hasConsistenceCheckCreated();

					virtual CConsistenceObserver* tellConsistenceData(CConsistenceData* consData);
					CConsistenceData* getConsistenceData();
					virtual CSaturationObserver* tellSaturationData(CSaturationData* saturationData);
					CSaturationData* getSaturationData();

					CConsistence* getConsistence();
					CTotallyOntologyPrecomputationItem* setConsistence(CConsistence* consistence);

					CCalculationConfigurationExtension* getCalculationConfiguration();

					CTotallyOntologyPrecomputationItem* setPrecomputation(CPrecomputation* precomputation);
					CPrecomputation* getPrecomputation();
					CTotallyOntologyPrecomputationItem* setConceptCyclePrecomputationInitialized(bool initialized);
					bool hasConceptCyclePrecomputationInitialized();
					CTotallyOntologyPrecomputationItem* setConceptCyclePrecomputationChecked(bool checked);
					bool hasConceptCyclePrecomputationChecked();
					QList<CConceptCycleData*>* getRemainingConceptCyclePrecomputationList();
					CTotallyOntologyPrecomputationItem* addRemainingConceptCyclePrecomputation(CConceptCycleData* conceptCycleData);
					bool hasRemainingConceptCyclePrecomputation();

					CTotallyOntologyPrecomputationItem* setConceptSaturationPrecomputationCreated(bool initialized);
					bool hasConceptSaturationPrecomputationCreated();
					CTotallyOntologyPrecomputationItem* setConceptSaturationPrecomputationChecked(bool checked);
					bool hasConceptSaturationPrecomputationChecked();



					bool failAfterConsistencyConceptSaturation();
					bool failAfterConsistencyChecking();
					bool failAfterConceptSaturation();



					bool requiresAllConceptsSaturation();
					bool hasAllConceptsSaturationOrdered();
					CTotallyOntologyPrecomputationItem* setAllConceptsSaturationOrdered(bool ordered);
					QList<CSaturationConceptDataItem*>* getSaturationOrderedItemList();
					bool hasRemainingRequiredSaturationConcepts();
					QList<TConceptNegPair>* getRemainingRequiredSaturationConceptList();
					CTotallyOntologyPrecomputationItem* addRequiredSaturationConcept(CConcept* concept, bool negation);

					bool requiresNominalDelayedConceptsSaturationUpdate();
					CTotallyOntologyPrecomputationItem* setNominalDelayedConceptsSaturationUpdateRequired(bool updateRequired);


					bool hasRemainingConsistencyRequiredSaturationConcepts();
					QList<TConceptNegPair>* getRemainingConsistencyRequiredSaturationConceptList();
					CTotallyOntologyPrecomputationItem* addConsistencyRequiredSaturationConcept(CConcept* concept, bool negation);


					cint64 getMinConceptCycleTestSize();
					CTotallyOntologyPrecomputationItem* setMinConceptCycleTestSize(cint64 minSize);

					CPrecomputationTestingStep* getConsistencePrecomputationStep();
					bool isConsistenceStepFinished();
					bool isConsistenceStepRequired();
					bool areConsistenceStepProcessingRequirementSatisfied();


					CPrecomputationTestingStep* getCyclePrecomputationStep();
					bool isCycleStepFinished();
					bool isCycleStepRequired();
					bool areCycleStepProcessingRequirementSatisfied();

					CPrecomputationTestingStep* getSaturationPrecomputationStep();
					bool isSaturationStepRunning();
					bool isSaturationStepFinished();
					bool isSaturationStepRequired();
					bool areSaturationStepProcessingRequirementSatisfied();

					CTotallyOntologyPrecomputationItem* setSaturationStepRunning(bool satStepRunning);
				
					bool isSaturationComputationRunning();
					CTotallyOntologyPrecomputationItem* setSaturationComputationRunning(bool satCompRunning);

					bool areAllStepFinished();
					bool hasRemainingProcessingRequirements();

					CSaturationConceptDataItem* getSaturationConceptDataItem(CConcept* satConcept, bool negated, bool create);
					CSaturationConceptDataItem* getSaturationRoleSuccessorConceptDataItem(CRole* role, CConcept* satConcept, bool negated, bool create);

					CApproximatedSaturationCalculationJob* getSaturationCalculationJob();
					CTotallyOntologyPrecomputationItem* setSaturationCalculationJob(CApproximatedSaturationCalculationJob* calculationJob);


					QSet<CConcept*>* getSaturatedDisjunctionSet();
					CTotallyOntologyPrecomputationItem* addSaturatedDisjunction(CConcept* disjunctionConcept);


				// protected methods
				protected:

				// protected variables
				protected:
					CConsistence* mConsistence;
					bool mConsistenceCheckCreated;
					bool mConsistenceCheched;
					QAtomicPointer<CConsistenceData> mConsData;
					QAtomicPointer<CSaturationData> mSaturData;
					QMutex mSaturDataAddMutex;
					QList<CSaturationData*> mSaturDataList;
					CSaturationData* mLastSaturData;

					CPrecomputation* mPrecomputation;
					bool mConceptCyclePrecomputationInitialized;
					bool mConceptCyclePrecomputationChecked;
					QList<CConceptCycleData*> mRemainingConceptCyclePrecomputationList;
					cint64 mMinConceptCycleTestSize;

					bool mFailAfterConsistencyConceptSaturation;
					bool mFailAfterConsistencyChecking;
					bool mFailAfterConceptSaturation;

					bool mSaturationTestRunning;
					bool mSaturationComputationRunning;

					CPrecomputationTestingStep* mConsistencePrecomputationStep;
					CPrecomputationTestingStep* mCyclePrecomputationStep;
					CPrecomputationTestingStep* mSaturationPrecomputationStep;
					QList<CPrecomputationTestingStep*> mProcessingSteps;

					QSet<CConcept*> mDisjunctionSaturationSet;


					bool mConceptSaturationPrecomputationCreated;
					bool mConceptSaturationPrecomputationChecked;
					QHash<TConceptNegPair,CSaturationConceptDataItem*> mConceptNegationSatItemHash;
					QHash<TRoleConceptNegTriple,CSaturationConceptDataItem*> mRoleConceptNegationSatItemHash;
					QList<CSaturationConceptDataItem*> mSatItemContainer;
					QList<CSaturationConceptDataItem*> mSatOrderedItemList;
					bool mAllConSatRequired;
					bool mAllConSatOrdered;
					QList<TConceptNegPair> mRemainingReqSatConList;
					QList<TConceptNegPair> mRemainingConsReqSatConList;
					bool mNomDelayedConSatUp;


					CApproximatedSaturationCalculationJob* mApproxSatCalcJob;

					CCalculationConfigurationExtension* mCalculationConfig;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CONSISTISER_CTOTALLYONTOLOGYPRECOMPUTATIONITEM_H