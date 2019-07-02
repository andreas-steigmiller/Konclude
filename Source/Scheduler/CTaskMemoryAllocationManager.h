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

#ifndef KONCLUDE_SCHEDULER_CTASKMEMORYALLOCATIONMANAGER_H
#define KONCLUDE_SCHEDULER_CTASKMEMORYALLOCATIONMANAGER_H

// Libraries includes


// Namespace includes
#include "SchedulerSettings.h"


// Other includes
#include "Utilities/Memory/CMemoryAllocationManager.h"
#include "Utilities/Memory/CConsiderateMemoryPoolProvider.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Memory;

	namespace Scheduler {


		/*! 
		 *
		 *		\class		CTaskMemoryAllocationManager
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CTaskMemoryAllocationManager : virtual public CMemoryAllocationManager {
			// public methods
			public:
				//! Constructor
				CTaskMemoryAllocationManager();

				//! Destructor
				virtual ~CTaskMemoryAllocationManager();

				virtual CTaskMemoryAllocationManager* updateMemoryProvider(CConsiderateMemoryPoolProvider* memoryPoolProvider) = 0;

			// protected methods
			protected:

			// protected variables
			protected:


			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_CTASKMEMORYALLOCATIONMANAGER_H
