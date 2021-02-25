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

#include "CAnalyseDataUpdateItem.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {


			CAnalyseDataUpdateItem::CAnalyseDataUpdateItem(CAnalyseDataSeries *analyseDataSeries, qint64 interval, qint64 timerID) {
				dataSeries = analyseDataSeries;
				updateInterval = interval;
				timerUpdateID = timerID;
			}


			CAnalyseDataUpdateItem::~CAnalyseDataUpdateItem() {
			}


			CAnalyseDataSeries *CAnalyseDataUpdateItem::getAnalyseDataSeries() {
				return dataSeries;
			}

			qint64 CAnalyseDataUpdateItem::getUpdateInterval() {
				return updateInterval;
			}

			qint64 CAnalyseDataUpdateItem::getTimerID() {
				return timerUpdateID;
			}


			CAnalyseDataUpdateItem *CAnalyseDataUpdateItem::addUpdateObserver(CAnalyseDataObserver *dataUpdateObserver) {
				observerList.append(dataUpdateObserver);
				return this;
			}

			CAnalyseDataUpdateItem *CAnalyseDataUpdateItem::notifyObserversDataUpdate() {
				foreach (CAnalyseDataObserver *observer, observerList) {
					observer->notifyDataUpdate(dataSeries);
				}
				return this;
			}



		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
