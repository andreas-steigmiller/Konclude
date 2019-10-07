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

#include "CSatisfiableTaskIncrementalConsistencyTestingAdapter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Task {

				CSatisfiableTaskIncrementalConsistencyTestingAdapter::CSatisfiableTaskIncrementalConsistencyTestingAdapter(CConcreteOntology* testingOntology, CConcreteOntology* prevConsOntology, cint64 incRevID, CConsistenceObserver* observer) {
					mOntology = testingOntology;
					mConsObserver = observer;
					mPrevConsOntology = prevConsOntology;
					mIncrementalRevisionID = incRevID;
				}

				CConcreteOntology *CSatisfiableTaskIncrementalConsistencyTestingAdapter::getTestingOntology() {
					return mOntology;
				}

				CConcreteOntology *CSatisfiableTaskIncrementalConsistencyTestingAdapter::getPreviousConsistentOntology() {
					return mPrevConsOntology;
				}

				CConsistenceObserver *CSatisfiableTaskIncrementalConsistencyTestingAdapter::getConsistenceObserver() {
					return mConsObserver;
				}


				cint64 CSatisfiableTaskIncrementalConsistencyTestingAdapter::getIncrementalRevisionID() {
					return mIncrementalRevisionID;
				}

			}; // end namespace Task

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude