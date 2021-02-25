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

#include "CTaskProcessingUpdateReader.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {

			namespace Reader {

				CTaskProcessingUpdateReader::CTaskProcessingUpdateReader(CTaskProcessorThreadBase* taskProcessor, const QString& threadName) {
					mTaskProcessor = taskProcessor;
					mThreadName = threadName;

					mReserveScheduleTaskCountValueDescription = nullptr;


					if (mTaskProcessor) {
						mReserveScheduleTaskCountValueDescription = new CAnalyseValueDescription();
						mReserveScheduleTaskCountValueDescription->setValueName(QString("%1::ReserveScheduleTasks").arg(mThreadName));
						mReserveScheduleTaskCountValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));
					}
				}

				CTaskProcessingUpdateReader::~CTaskProcessingUpdateReader() {
					delete mReserveScheduleTaskCountValueDescription;
				}



				CAnalyseValueDescription* CTaskProcessingUpdateReader::getReserveScheduleTaskCountValueDescription() {
					return mReserveScheduleTaskCountValueDescription;
				}

				CAnalyseValue *CTaskProcessingUpdateReader::getAnalyseValueUpdate(CAnalyseValueDescription* valueDes) {
					CAnalyseValue *val = 0;

					if (valueDes == mReserveScheduleTaskCountValueDescription) {
						val = new CIntegerAnalyseValue(mTaskProcessor->countScheduleableProcessingTasks());
					}
					return val;
				}



			}; // end namespace Reader

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
