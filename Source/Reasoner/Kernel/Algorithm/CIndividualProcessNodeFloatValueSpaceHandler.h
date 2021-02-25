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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CINDIVIDUALPROCESSNODEFLOATVALUESPACEHANDLER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CINDIVIDUALPROCESSNODEFLOATVALUESPACEHANDLER_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContext.h"
#include "CIndividualProcessNodeCompareValueSpaceHandler.h"

// Other includes
#include "Reasoner/Kernel/Process/CIndividualProcessNode.h"
#include "Reasoner/Kernel/Process/CProcessingDataBox.h"

#include "Reasoner/Kernel/Process/Dependency/CDependencyTrackPoint.h"


#include "Context/CContextBase.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Context;

	namespace Reasoner {

		namespace Kernel {

			using namespace Process;
			using namespace Dependency;

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CIndividualProcessNodeFloatValueSpaceHandler
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualProcessNodeFloatValueSpaceHandler : public CIndividualProcessNodeCompareValueSpaceHandler {
					// public methods
					public:
						//! Constructor
						CIndividualProcessNodeFloatValueSpaceHandler(CDatatypeValueSpaceType* valueSpaceType);
					// protected methods
					protected:

					// protected variables
					protected:


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CINDIVIDUALPROCESSNODEFLOATVALUESPACEHANDLER_H
