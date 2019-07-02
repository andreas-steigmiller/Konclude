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

#include "COntologyProcessingStatus.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyProcessingStatus::COntologyProcessingStatus() {
				mProcessingFlags = PSUNPROCESSED;
				mErrorFlags = 0;
			}


			cint64 COntologyProcessingStatus::getProcessingFlags() {
				return mProcessingFlags;
			}

			COntologyProcessingStatus* COntologyProcessingStatus::setProcessingFlags(cint64 flags) {
				mProcessingFlags = flags;
				return this;
			}


			cint64 COntologyProcessingStatus::getErrorFlags() {
				return mErrorFlags;
			}

			COntologyProcessingStatus* COntologyProcessingStatus::setErrorFlags(cint64 flags) {
				mErrorFlags = flags;
				return this;
			}


			bool COntologyProcessingStatus::hasPartialProcessingFlags(cint64 flags) {
				return (mProcessingFlags & flags) != 0;
			}

			bool COntologyProcessingStatus::hasPartialErrorFlags(cint64 flags) {
				return (mErrorFlags & flags) != 0;
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude