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

#ifndef KONCLUDE_UTILITIES_MEMORY_CCENTRALIZEDALLOCATIONLIMITATOR_H
#define KONCLUDE_UTILITIES_MEMORY_CCENTRALIZEDALLOCATIONLIMITATOR_H

// Library includes
#include <qglobal.h>

// Namespace includes
#include "MemorySettings.h"
#include "CAllocationLimitator.h"
#include "CCentralizedAllocationLimitation.h"


// Other includes
#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Utilities {

		using namespace Container;

		namespace Memory {


			/*! 
			 *
			 *		\class		CCentralizedAllocationLimitator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CCentralizedAllocationLimitator : public CAllocationLimitator, public CLinkerBase<CCentralizedAllocationLimitator*,CCentralizedAllocationLimitator> {
				// public methods
				public:
					//! Constructor
					CCentralizedAllocationLimitator(CCentralizedAllocationLimitation* limitation);

					//! Destructor
					virtual ~CCentralizedAllocationLimitator();


					virtual cint64 getMemoryAllocatedSize();
					virtual cint64 getMaximumMemoryAllocationSize();
					virtual cint64 getRemainingMemoryAllocationSize();

					virtual bool addAllocatingMemorySize(cint64 allocatedMemorySize);
					virtual CCentralizedAllocationLimitator* addReleasedMemorySize(cint64 allocatedMemorySize);

					virtual CCentralizedAllocationLimitator* notifyRequireUpdateLimit();


				// protected methods
				protected:

				// protected variables
				protected:
					bool mReqUpdateFlag;
					cint64 mMemAllocated;
					cint64 mMaxMemAllocSize;

					CCentralizedAllocationLimitation* mAllocLimitation;



				// private methods
				private:

				// private variables
				private:
			};


		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_MEMORY_CCENTRALIZEDALLOCATIONLIMITATOR_H
