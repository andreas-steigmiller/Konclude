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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CSATURATIONNODEASSOCIATEDEXPANSIONCACHECONTEXT_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CSATURATIONNODEASSOCIATEDEXPANSIONCACHECONTEXT_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"


// Other includes
#include "Context/CContext.h"

#include "Utilities/Memory/CMemoryPoolProvider.h"
#include "Utilities/Memory/CMemoryPoolAllocationManager.h"
#include "Utilities/Memory/CNewAllocationMemoryPoolProvider.h"
#include "Utilities/Memory/CLimitedReserveMemoryPoolAllocationManager.h"

#include "Reasoner/Kernel/Process/CIndividualSaturationProcessNodeStatusUpdateLinker.h"
#include "Reasoner/Kernel/Process/CIndividualSaturationProcessNode.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Context;
	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Kernel {

			using namespace Process;

			namespace Cache {

				/*! 
				 *
				 *		\class		CSaturationNodeAssociatedExpansionCacheContext
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSaturationNodeAssociatedExpansionCacheContext : public CContext {
					// public methods
					public:
						//! Constructor
						CSaturationNodeAssociatedExpansionCacheContext();

						~CSaturationNodeAssociatedExpansionCacheContext();

						virtual CMemoryAllocationManager* getMemoryAllocationManager();
						virtual CMemoryPoolAllocationManager* getMemoryPoolAllocationManager();

						virtual CMemoryPoolProvider* getMemoryPoolProvider();

						cint64 getMemoryConsumption();
						CSaturationNodeAssociatedExpansionCacheContext* releaseTemporaryMemoryPools(CMemoryPool* memoryPools);
						
						
						
						CIndividualSaturationProcessNodeStatusUpdateLinker* getIndividualSaturationUpdateLinker(bool create = true);
						CSaturationNodeAssociatedExpansionCacheContext* addIndividualSaturationUpdateLinker(CIndividualSaturationProcessNodeStatusUpdateLinker* linker);

					// protected methods
					protected:

					// protected variables
					protected:
						CMemoryPoolAllocationManager* mMemMan;
						CNewAllocationMemoryPoolProvider* mMemoryPoolProvider;

						CIndividualSaturationProcessNodeStatusUpdateLinker* mConSatUpdateLinker;

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

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CSATURATIONNODEASSOCIATEDEXPANSIONCACHECONTEXT_H
