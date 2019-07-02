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

#include "CTestTaskThroughputReader.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {

			namespace Reader {

				CTestTaskThroughputReader::CTestTaskThroughputReader(CThroughputTestTaskTester *throughputTaskTester) {
					mThroughputTaskTester = throughputTaskTester;

					mProcessedTaskCountValueDescription = 0;

					if (mThroughputTaskTester) {
						mProcessedTaskCountValueDescription = new CAnalyseValueDescription();
						mProcessedTaskCountValueDescription->setValueName(QString("ThroughputTaskTester::TaskProcessedCount"));
						mProcessedTaskCountValueDescription->setTemplateValue(new CIntegerAnalyseValue(0));
					}
				}

				CTestTaskThroughputReader::~CTestTaskThroughputReader() {
					if (mProcessedTaskCountValueDescription) {
						delete mProcessedTaskCountValueDescription;
					}
				}



				CAnalyseValueDescription *CTestTaskThroughputReader::getProcessedTaskCountValueDescription() {
					return mProcessedTaskCountValueDescription;
				}



				CAnalyseValue *CTestTaskThroughputReader::getAnalyseValueUpdate(CAnalyseValueDescription *valueDes) {
					CAnalyseValue *val = 0;

					if (valueDes == mProcessedTaskCountValueDescription && mThroughputTaskTester) {
						val = new CIntegerAnalyseValue(mThroughputTaskTester->getTaskProcessedCount());
					}
					return val;
				}



			}; // end namespace Reader

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude