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

#include "CReasonerEvaluationResponseTimeExtractor.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationResponseTimeExtractor::CReasonerEvaluationResponseTimeExtractor(double timelimit) {
				mTimelimit = timelimit;
			}


			CReasonerEvaluationResponseTimeExtractor::~CReasonerEvaluationResponseTimeExtractor() {
			}

			CReasonerEvaluationDataValue* CReasonerEvaluationResponseTimeExtractor::extractEvaluationData(const QString& responseFileString) {
				CReasonerEvaluationDoubleDataValue* doubleEvalValue = nullptr;
				double doubleValue = 0;
				QFile responseFile(responseFileString);
				if (responseFile.open(QIODevice::ReadOnly)) {
					QDomDocument document;
					document.setContent(&responseFile,false);
					QDomElement rootEl = document.documentElement();
					QString respTimeString = rootEl.attribute("response-time","timeout");
					bool validRespTime = false;
					doubleValue = respTimeString.toDouble(&validRespTime);
					if (validRespTime) {
						if (mTimelimit >= 0) {
							doubleValue = qMin(mTimelimit,doubleValue);
						}
						doubleEvalValue = new CReasonerEvaluationDoubleDataValue(doubleValue);
					}
				}
				return doubleEvalValue;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude