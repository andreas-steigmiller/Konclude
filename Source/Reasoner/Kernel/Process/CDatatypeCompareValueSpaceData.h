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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPECOMPAREVALUESPACEDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPECOMPAREVALUESPACEDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CDatatypeValueSpaceData.h"
#include "CDatatypeCompareValueSpaceData.h"
#include "CDatatypeCompareValueSpaceMap.h"


// Other includes
#include "Utilities/Container/CLinker.h"

#include "Reasoner/Ontology/CDatatypeValueSpaceCompareType.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CDatatypeCompareValueSpaceData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CDatatypeCompareValueSpaceData : public CDatatypeValueSpaceData {
					// public methods
					public:
						//! Constructor
						CDatatypeCompareValueSpaceData(CProcessContext* processContext);


						CDatatypeCompareValueSpaceData* copyCompareValueSpaceData(CDatatypeCompareValueSpaceData* spaceData);
						CDatatypeCompareValueSpaceData* initCompareValueSpaceData(CDatatypeValueSpaceCompareType* valueSpaceType);

						CDatatypeCompareValueSpaceMap* getValueSpaceMap();

					// protected methods
					protected:

					// protected variables
					protected:
						CDatatypeCompareValueSpaceMap* mValueSpaceMap;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPECOMPAREVALUESPACEDATA_H