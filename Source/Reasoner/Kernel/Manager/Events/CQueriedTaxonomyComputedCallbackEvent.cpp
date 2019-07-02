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

#include "CQueriedTaxonomyComputedCallbackEvent.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Manager {

				namespace Events {


					CQueriedTaxonomyComputedCallbackEvent::CQueriedTaxonomyComputedCallbackEvent(CThread *receiverThread, CTaxonomyPremisingQuery *query) 
							: CCustomEvent(EVENTQUERIEDTAXCOMPUTEDCALLBACK) {

						this->query = query;
						queryString = query->getQueryString();
						queryName = query->getQueryName();
						recThread = receiverThread;
					}


					CQueriedTaxonomyComputedCallbackEvent::~CQueriedTaxonomyComputedCallbackEvent() {
					}


					void CQueriedTaxonomyComputedCallbackEvent::doCallback() {
						if (recThread) {
							recThread->postEvent(this);
						}
					}

					QString CQueriedTaxonomyComputedCallbackEvent::getQueryString() {
						return queryString;
					}

					QString CQueriedTaxonomyComputedCallbackEvent::getQueryName() {
						return queryName;
					}

					CTaxonomyPremisingQuery *CQueriedTaxonomyComputedCallbackEvent::getQuery() {
						return query;
					}

				}; // end namespace Events

			}; // end namespace Manager

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude