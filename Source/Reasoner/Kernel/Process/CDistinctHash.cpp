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

#include "CDistinctHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDistinctHash::CDistinctHash(CContext* context) : CPROCESSHASH<cint64,CDistinctEdge*>(context) {
				}

				CDistinctHash::CDistinctHash(const CDistinctHash &disHash, CContext* context) : CPROCESSHASH<cint64,CDistinctEdge*>(disHash,context) {
				}


				CDistinctHash::~CDistinctHash() {
				}


				CDistinctHash* CDistinctHash::initDistinctHash(CDistinctHash* prevHash) {
					if (prevHash) {
						*this = *prevHash;
					} else {
						clear();
					}
					return this;
				}



				CDistinctEdge* CDistinctHash::getIndividualDistinctEdge(cint64 indiID) {
					return CPROCESSHASH<cint64,CDistinctEdge*>::value(indiID,nullptr);
				}


				bool CDistinctHash::isIndividualDistinct(cint64 indiID) {
					return CPROCESSHASH<cint64,CDistinctEdge*>::contains(indiID);
				}

				CDistinctHash *CDistinctHash::insertDistinctIndividual(cint64 indiID, CDistinctEdge* disEdge) {
					CPROCESSHASH<cint64,CDistinctEdge*>::insert(indiID,disEdge);
					return this;
				}

				CDistinctHash* CDistinctHash::removeDistinctIndividual(cint64 indiID) {
					CPROCESSHASH<cint64,CDistinctEdge*>::remove(indiID);
					return this;
				}

				cint64 CDistinctHash::getDistinctCount() {
					return CPROCESSHASH<cint64,CDistinctEdge*>::count();
				}

				CDistinctIterator CDistinctHash::getDistinctIterator() {
					return CDistinctIterator(CPROCESSHASH<cint64,CDistinctEdge*>::begin(),CPROCESSHASH<cint64,CDistinctEdge*>::end());
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
