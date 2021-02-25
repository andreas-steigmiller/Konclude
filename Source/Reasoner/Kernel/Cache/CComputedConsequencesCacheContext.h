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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CCOMPUTEDCONSEQUENCESCACHECONTEXT_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CCOMPUTEDCONSEQUENCESCACHECONTEXT_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"


// Other includes
#include "Context/CContext.h"

#include "Utilities/Memory/CMemoryPoolProvider.h"
#include "Utilities/Memory/CMemoryPoolAllocationManager.h"
#include "Utilities/Memory/CNewAllocationMemoryPoolProvider.h"
#include "Utilities/Memory/CLimitedReserveMemoryPoolAllocationManager.h"

#include "Reasoner/Ontology/CConcept.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Context;
	using namespace Utilities::Memory;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		CComputedConsequencesCacheContext
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CComputedConsequencesCacheContext : public CContext {
					// public methods
					public:
						//! Constructor
						CComputedConsequencesCacheContext();

						~CComputedConsequencesCacheContext();

						virtual CMemoryAllocationManager* getMemoryAllocationManager();
						virtual CMemoryPoolAllocationManager* getMemoryPoolAllocationManager();

						virtual CMemoryPoolProvider* getMemoryPoolProvider();

						cint64 getMemoryConsumption();
						CComputedConsequencesCacheContext* releaseTemporaryMemoryPools(CMemoryPool* memoryPools);
						
						
						
						CSortedNegLinker<CConcept*>* createConceptLinker(bool create = true);
						CComputedConsequencesCacheContext* addConceptLinker(CSortedNegLinker<CConcept*>* linker);

					// protected methods
					protected:

					// protected variables
					protected:
						CMemoryPoolAllocationManager* mMemMan;
						CNewAllocationMemoryPoolProvider* mMemoryPoolProvider;

						CSortedNegLinker<CConcept*>* mConLinker;

						cint64 mAddRelMemory;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CCOMPUTEDCONSEQUENCESCACHECONTEXT_H
