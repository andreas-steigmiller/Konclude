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

#include "CSatisfiableCalculationConstruct.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CSatisfiableCalculationConstruct::CSatisfiableCalculationConstruct() : CLinkerBase<CSatisfiableCalculationConstruct*,CSatisfiableCalculationConstruct>(this) {
				mIndividual = nullptr;
				mIndiID = -1;
				mRelNodeID = 0;
			}


			CSatisfiableCalculationConstruct::~CSatisfiableCalculationConstruct() {
			}

			CSatisfiableCalculationConstruct* CSatisfiableCalculationConstruct::getNextConstruct() {
				return getNext();
			}



			CSatisfiableCalculationConstruct* CSatisfiableCalculationConstruct::setIndividual(CIndividual* individual) {
				mIndividual = individual;
				return this;
			}

			CIndividual* CSatisfiableCalculationConstruct::getIndividual() {
				return mIndividual;
			}

			CSatisfiableCalculationConstruct* CSatisfiableCalculationConstruct::setIndividualID(cint64 individualID) {
				mIndiID = individualID;
				return this;
			}

			cint64 CSatisfiableCalculationConstruct::getIndividualID() {
				return mIndiID;
			}

			CSatisfiableCalculationConstruct* CSatisfiableCalculationConstruct::setRelativeNewNodeID(cint64 nodeID) {
				return this;
			}

			cint64 CSatisfiableCalculationConstruct::getRelativeNewNodeID() {
				return mRelNodeID;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude