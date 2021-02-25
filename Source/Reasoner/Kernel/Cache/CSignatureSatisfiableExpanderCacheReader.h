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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CSIGNATURESATISFIABLEEXPANDERCACHEREADER_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CSIGNATURESATISFIABLEEXPANDERCACHEREADER_H

// Libraries includes
#include <QAtomicPointer>

// Namespace includes
#include "CacheSettings.h"
#include "CSignatureSatisfiableExpanderCacheRedirectionItem.h"
#include "CSignatureSatisfiableExpanderCacheHasher.h"
#include "CSignatureSatisfiableExpanderCacheSlotItem.h"
#include "CSignatureSatisfiableExpanderCacheEntry.h"


// Other includes
#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		CSignatureSatisfiableExpanderCacheReader
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSignatureSatisfiableExpanderCacheReader : public CLinkerBase<CSignatureSatisfiableExpanderCacheReader*,CSignatureSatisfiableExpanderCacheReader> {
					// public methods
					public:
						//! Constructor
						CSignatureSatisfiableExpanderCacheReader();


						CSignatureSatisfiableExpanderCacheReader* updateSlot(CSignatureSatisfiableExpanderCacheSlotItem* updatedSlot);

						bool hasCacheEntry(cint64 signature);
						CSignatureSatisfiableExpanderCacheEntry* getCacheEntry(CCACHINGSET<CCacheValue>* cacheValueSet);
						CSignatureSatisfiableExpanderCacheEntry* getCacheEntry(cint64 signature);

					// protected methods
					protected:
						bool hasUpdatedSlotItem();
						bool switchToUpdatedSlotItem();

					// protected variables
					protected:
						CSignatureSatisfiableExpanderCacheSlotItem* mCurrentSlot;
						QAtomicPointer<CSignatureSatisfiableExpanderCacheSlotItem> mUpdatedSlot;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CSIGNATURESATISFIABLEEXPANDERCACHEREADER_H
