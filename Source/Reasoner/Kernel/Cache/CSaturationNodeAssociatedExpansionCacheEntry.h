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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CSATURATIONNODEASSOCIATEDEXPANSIONCACHEENTRY_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CSATURATIONNODEASSOCIATEDEXPANSIONCACHEENTRY_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CCacheEntry.h"
#include "CSaturationNodeAssociatedExpansionCacheContext.h"
#include "CSaturationNodeAssociatedDeterministicConceptExpansion.h"
#include "CSaturationNodeAssociatedNondeterministicConceptExpansion.h"


// Other includes
#include "Reasoner/Kernel/Process/ProcessSettings.h"
#include "Reasoner/Kernel/Process/CIndividualSaturationProcessNodeCacheData.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			using namespace Process;

			namespace Cache {

				/*! 
				 *
				 *		\class		CSaturationNodeAssociatedExpansionCacheEntry
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSaturationNodeAssociatedExpansionCacheEntry : public CIndividualSaturationProcessNodeCacheData, public CCacheEntry {
					// public methods
					public:
						//! Constructor
						CSaturationNodeAssociatedExpansionCacheEntry(CSaturationNodeAssociatedExpansionCacheContext* context);


						CSaturationNodeAssociatedExpansionCacheEntry* initCacheEntry(CIndividualSaturationProcessNode* saturationNode, cint64 remainingAllowedNonDetExpansionCount);

						CIndividualSaturationProcessNode* getSaturationIndividualNode();

						CSaturationNodeAssociatedDeterministicConceptExpansion* getDeterministicConceptExpansion();
						CSaturationNodeAssociatedNondeterministicConceptExpansion* getNondeterministicConceptExpansionLinker();
						bool hasDeterministicConceptExpansion();


						CSaturationNodeAssociatedExpansionCacheEntry* setDeterministicConceptExpansion(CSaturationNodeAssociatedDeterministicConceptExpansion* detConceptExpansion);
						CSaturationNodeAssociatedExpansionCacheEntry* addNondeterministicConceptExpansion(CSaturationNodeAssociatedNondeterministicConceptExpansion* nondetConceptExpansion);


						cint64 getRemainingAllowedNondeterministicExpansionCount();
						bool areMoreNondeterministicExpansionAllowed();
						CSaturationNodeAssociatedExpansionCacheEntry* decRemainingAllowedNondeterministicExpansionCount(cint64 decCount = 1);


					// protected methods
					protected:

					// protected variables
					protected:
						CSaturationNodeAssociatedExpansionCacheContext* mContext;

						CIndividualSaturationProcessNode* mSaturationNode;

						cint64 mRemainingAllowedNonDetExpansionCount;

						CSaturationNodeAssociatedDeterministicConceptExpansion* mDetExpansion;
						CSaturationNodeAssociatedNondeterministicConceptExpansion* mNondetExpansionLinker;



					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CSATURATIONNODEASSOCIATEDEXPANSIONCACHEENTRY_H
