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

#include "CConceptProcessLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CConceptProcessLinker::CConceptProcessLinker() : CLinkerBase<CConceptDescriptor*,CConceptProcessLinker>(nullptr) {
				}

				CConceptProcessLinker *CConceptProcessLinker::initConceptProcessLinker(CConceptDescriptor *conceptDescriptor, CProcessingRestrictionSpecification* processingRestriction) {
					initLinker(conceptDescriptor);
					mProcRest = processingRestriction;
					return this;
				}

				CConceptDescriptor *CConceptProcessLinker::getConceptDescriptor() {
					return getData();
				}

				CProcessingRestrictionSpecification* CConceptProcessLinker::getProcessingRestriction() {
					return mProcRest;
				}


			}; // end namespace Kernel

		}; // end namespace Process

	}; // end namespace Reasoner

}; // end namespace Konclude
