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

#include "CDatatypeStringValueSpaceData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeStringValueSpaceData::CDatatypeStringValueSpaceData(CProcessContext* processContext) : CDatatypeValueSpaceData(processContext),mValueSpaceMap(processContext),mProcessContext(processContext) {
				}


				CDatatypeStringValueSpaceData* CDatatypeStringValueSpaceData::initStringValueSpaceData(CDatatypeValueSpaceStringType* valueSpaceType) {
					CDatatypeValueSpaceData::initValueSpaceData(nullptr);
					mValueSpaceMap.initDatatypeStringValueSpaceMap(valueSpaceType);
					return this;
				}


				CDatatypeStringValueSpaceData* CDatatypeStringValueSpaceData::copyStringValueSpaceData(CDatatypeStringValueSpaceData* spaceData) {
					CDatatypeValueSpaceData::initValueSpaceData(spaceData);
					mValueSpaceMap.initDatatypeStringValueSpaceMap(spaceData->getValueSpaceMap());
					return this;
				}

				CDatatypeStringValueSpaceMap* CDatatypeStringValueSpaceData::getValueSpaceMap() {
					return &mValueSpaceMap;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude