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

#ifndef KONCLUDE_REASONER_KERNEL_TASK_CSatisfiableCalculationTaskStatusPropagator_H
#define KONCLUDE_REASONER_KERNEL_TASK_CSatisfiableCalculationTaskStatusPropagator_H

// Libraries includes


// Namespace includes
#include "CSatisfiableCalculationTask.h"

// Other includes
#include "Scheduler/CTaskStatusPropagator.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Scheduler;

	namespace Reasoner {

		namespace Kernel {

			namespace Task {

				/*! 
				 *
				 *		\class		CSatisfiableCalculationTaskStatusPropagator
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSatisfiableCalculationTaskStatusPropagator : public CTaskStatusPropagator {
					// public methods
					public:
						//! Constructor
						CSatisfiableCalculationTaskStatusPropagator();

						//! Destructor
						virtual ~CSatisfiableCalculationTaskStatusPropagator();

						virtual bool updateTaskStatus(CTask* task, bool& moreDownPropagation, bool& moreUpPropagation);
						virtual bool completeTaskStatus(CTask* task, bool& moreUpPropagation);


					// protected methods
					protected:

					// protected variables
					protected:

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Task

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_TASK_CSatisfiableCalculationTaskStatusPropagator_H