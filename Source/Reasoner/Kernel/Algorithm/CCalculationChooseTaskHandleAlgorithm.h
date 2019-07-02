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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CCALCULATIONCHOOSETASKHANDLEALGORITHM_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CCALCULATIONCHOOSETASKHANDLEALGORITHM_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"


// Other includes
#include "Scheduler/CTask.h"
#include "Scheduler/CTaskHandleAlgorithm.h"
#include "Scheduler/CTaskProcessorContext.h"

#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Scheduler;

		namespace Kernel {

			using namespace Task;

			namespace Algorithm {


				/*! 
				 *
				 *		\class		CCalculationChooseTaskHandleAlgorithm
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCalculationChooseTaskHandleAlgorithm : public CTaskHandleAlgorithm {
					// public methods
					public:
						//! Constructor
						CCalculationChooseTaskHandleAlgorithm(CTaskHandleAlgorithm* tableauCompTaskHandleAlg = nullptr, CTaskHandleAlgorithm* tableauDefaultSaturTaskHandleAlg = nullptr, CTaskHandleAlgorithm* tableauPilingSaturTaskHandleAlg = nullptr, CTaskHandleAlgorithm* tableauApproxSaturTaskHandleAlg = nullptr);

						virtual bool handleTask(CTaskProcessorContext *processorContext, CTask* task);

					// protected methods
					protected:

					// protected variables
					protected:
						CTaskHandleAlgorithm* mTableauCompTaskHandleAlg;
						CTaskHandleAlgorithm* mTableauDefaultSaturTaskHandleAlg;
						CTaskHandleAlgorithm* mTableauPilingSaturTaskHandleAlg;
						CTaskHandleAlgorithm* mTableauApproxSaturTaskHandleAlg;
						

					// private methods
					private:

					// private variables
					private:

				};



			}; // end namespace Algorithm

		}; // end namespace Kernel 

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CCALCULATIONCHOOSETASKHANDLEALGORITHM_H