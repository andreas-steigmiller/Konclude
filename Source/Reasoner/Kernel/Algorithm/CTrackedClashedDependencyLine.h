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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CTRACKEDCLASHEDDEPENDENCYLINE_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CTRACKEDCLASHEDDEPENDENCYLINE_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CTrackedClashedDescriptor.h"
#include "CTrackedClashedDescriptorHasher.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CTrackedClashedDependencyLine
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CTrackedClashedDependencyLine {
					// public methods
					public:
						//! Constructor
						CTrackedClashedDependencyLine(CPROCESSINGSET<CTrackedClashedDescriptorHasher>* clashedSet);


						CTrackedClashedDependencyLine* initTrackedClashedDependencyLine(bool exactIndiNodeTracking, cint64 individualNodeTrackLevel, cint64 branchingLevel);

						CTrackedClashedDependencyLine* sortInTrackedClashedDescriptors(CTrackedClashedDescriptor* clashedDes, bool forceInsertion = false);
						CTrackedClashedDependencyLine* moveToNextIndividualNodeLevel(cint64 newLevel);

						bool hasLevelTrackedClashedDescriptors();
						CTrackedClashedDescriptor* takeNextLevelTrackedClashedDescriptor();
						CTrackedClashedDescriptor* getLevelTrackedClashedDescriptors();

						bool hasLevelTrackedBranchingClashedDescriptors();
						CTrackedClashedDescriptor* takeNextLevelTrackedBranchingClashedDescriptor();
						CTrackedClashedDescriptor* getLevelTrackedBranchingClashedDescriptors();

						bool hasPerviousLevelTrackedClashedDescriptors();
						CTrackedClashedDescriptor* takeNextPerviousLevelTrackedClashedDescriptor();
						CTrackedClashedDescriptor* getPerviousLevelTrackedClashedDescriptors();


						bool hasPerviousLevelTrackedNonDeterministicClashedDescriptors();
						CTrackedClashedDescriptor* takeNextPerviousLevelTrackedNonDeterministicClashedDescriptor();
						CTrackedClashedDescriptor* getPerviousLevelTrackedNonDeterministicClashedDescriptors();

						bool hasPerviousLevelTrackedNonDeterministicBranchingClashedDescriptors();
						CTrackedClashedDescriptor* takeNextPerviousLevelTrackedNonDeterministicBranchingClashedDescriptor();
						CTrackedClashedDescriptor* getPerviousLevelTrackedNonDeterministicBranchingClashedDescriptors();

						bool hasIndependentTrackedClashedDescriptors();
						CTrackedClashedDescriptor* takeNextIndependentTrackedClashedDescriptor();
						CTrackedClashedDescriptor* getIndependentTrackedClashedDescriptors();


						CTrackedClashedDescriptor* takeNextTrackedClashedList();
						bool hasMoreTrackedClashedList();


						CTrackedClashedDescriptor* takeNextFreeTrackedClashedDescriptor();
						CTrackedClashedDependencyLine* addFreeTrackedClashedDescriptor(CTrackedClashedDescriptor* clashDes);



						CPROCESSINGSET<CTrackedClashedDescriptorHasher>* getTrackedClashedDescriptorSet();

						cint64 getBranchingLevel();
						cint64 getIndividualNodeLevel();

						bool hasOnlyIndependentTrackedClashedDescriptorsRemaining();
						bool hasOnlyCurrentIndividualNodeLevelClashesDescriptors();

					// protected methods
					protected:

					// protected variables
					protected:
						bool mExactIndividualTracking;
						cint64 mIndividualTrackLevel;
						cint64 mBranchingLevel;
						CTrackedClashedDescriptor* mIndependentTrackedClashes;
						CTrackedClashedDescriptor* mLevelTrackedClashes;
						CTrackedClashedDescriptor* mLevelTrackedBranchingClashes;
						CTrackedClashedDescriptor* mPrevLevelsTrackedClashes;
						CTrackedClashedDescriptor* mPrevLevelsTrackedNonDetClashes;
						CTrackedClashedDescriptor* mPrevLevelsTrackedNonDetBranchingClashes;

						CTrackedClashedDescriptor* mFreeTrackedClashedDescriptors;

						CPROCESSINGSET<CTrackedClashedDescriptorHasher>* mClashedSet;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CTRACKEDCLASHEDDEPENDENCYLINE_H