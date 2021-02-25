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

#include "COntologyCoreConceptCyclesData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyCoreConceptCyclesData::COntologyCoreConceptCyclesData(COntologyContext* ontoContext) : mContext(ontoContext),mConceptCycleHash(ontoContext),mCycleList(ontoContext) {
			}

			COntologyCoreConceptCyclesData::~COntologyCoreConceptCyclesData() {
			}


			CBOXHASH<TConceptNegPair,CConceptCycleData*>* COntologyCoreConceptCyclesData::getConceptCycleHash() {
				return &mConceptCycleHash;
			}
			CBOXLIST<CConceptCycleData*>* COntologyCoreConceptCyclesData::getConceptCycleList() {
				return &mCycleList;
			}

			cint64 COntologyCoreConceptCyclesData::getConceptCycleCount() {
				return mCycleList.count();
			}

			COntologyCoreConceptCyclesData* COntologyCoreConceptCyclesData::installCycle(CConceptCycleData* cycleData) {
				mCycleList.append(cycleData);
				CBOXSET<TConceptNegPair>* conSet = cycleData->getCycleConceptSet();
				for (CBOXSET<TConceptNegPair>::const_iterator it = conSet->constBegin(), itEnd = conSet->constEnd(); it != itEnd; ++it) {
					TConceptNegPair conNegPair(*it);
					mConceptCycleHash.insert(conNegPair,cycleData);
				}
				return this;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
