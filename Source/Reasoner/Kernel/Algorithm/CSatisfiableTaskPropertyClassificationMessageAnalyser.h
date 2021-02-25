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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATISFIABLETASKPROPERTYCLASSIFICATIONMESSAGEANALYSER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATISFIABLETASKPROPERTYCLASSIFICATIONMESSAGEANALYSER_H

// Libraries includes


// Namespace includes
#include "CCalculationAlgorithmContext.h"

// Other includes
#include "Context/CContextBase.h"

#include "Reasoner/Classifier/CClassificationMessageDataObserver.h"
#include "Reasoner/Classifier/CClassificationPropertySubsumptionMessageData.h"
#include "Reasoner/Classifier/CClassificationInitializePossiblePropertySubsumptionData.h"
#include "Reasoner/Classifier/CClassificationInitializePossiblePropertySubsumptionMessageData.h"
#include "Reasoner/Classifier/CClassificationMessageData.h"

#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"

#include "Reasoner/Kernel/Process/Dependency/CDependencyTrackPoint.h"
#include "Reasoner/Kernel/Process/Dependency/CDependencyNode.h"
#include "Reasoner/Kernel/Process/CIndividualProcessNode.h"
#include "Reasoner/Kernel/Process/CIndividualSaturationProcessNode.h"

#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptSatisfiableReferenceLinkingData.h"
#include "Reasoner/Ontology/CConceptSaturationReferenceLinkingData.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Context;

	namespace Reasoner {

		using namespace Consistiser;
		using namespace Classifier;
		using namespace Ontology;

		namespace Kernel {

			using namespace Task;
			using namespace Process::Dependency;

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CSatisfiableTaskPropertyClassificationMessageAnalyser
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSatisfiableTaskPropertyClassificationMessageAnalyser {
					// public methods
					public:
						//! Constructor
						CSatisfiableTaskPropertyClassificationMessageAnalyser();

						bool readCalculationConfig(CCalculationConfigurationExtension *config);
						bool analyseSatisfiableTask(CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContext* calcAlgContext);

						CIndividualProcessNode* getCorrectedIndividualID(CIndividualProcessNode* baseIndiNode, CIndividualProcessNodeVector* indiNodeVec, bool* nondetMergedFlag);



					// protected methods
					protected:
						CClassificationMessageData* extractPossibleSubsumptionInformation(CRole* testingRole, CIndividualProcessNode* indiNode, CReapplyConceptLabelSet* conSet, CCalculationAlgorithmContext* calcAlgContext);

					// protected variables
					protected:


						CTaskMemoryPoolAllocationManager* mTempMemAllocMan;
						CContextBase* mTmpContext;
						CSatisfiableCalculationTask* mStatCalcTask;





					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATISFIABLETASKPROPERTYCLASSIFICATIONMESSAGEANALYSER_H
