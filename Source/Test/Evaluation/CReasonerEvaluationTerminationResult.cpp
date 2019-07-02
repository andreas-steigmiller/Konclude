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

#include "CReasonerEvaluationTerminationResult.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationTerminationResult::CReasonerEvaluationTerminationResult() {
				mInTimeTerminated = false;
				mTerminationForced = false;
				mProcessError = false;
			}

			CReasonerEvaluationTerminationResult* CReasonerEvaluationTerminationResult::initResult(bool processError, bool inTimeTerminated, bool forcedTermination, const QString& errorText, const QString& stnOutText, const QString& errOutText) {
				mProcessError = processError;
				mInTimeTerminated = inTimeTerminated;
				mTerminationForced = forcedTermination;
				mStandardOutputText = stnOutText;
				mErrorOutputText = errOutText;
				mErrorText = errorText;
				return this;
			}


			bool CReasonerEvaluationTerminationResult::isInTimeTerminated() {
				return mInTimeTerminated;
			}

			bool CReasonerEvaluationTerminationResult::isTerminationForced() {
				return mTerminationForced;
			}

			bool CReasonerEvaluationTerminationResult::hasProcessError() {
				return mProcessError;
			}

			QString CReasonerEvaluationTerminationResult::getStandardOutputText() {
				return mStandardOutputText;
			}

			QString CReasonerEvaluationTerminationResult::getErrorOutputText() {
				return mErrorOutputText;
			}

			QString CReasonerEvaluationTerminationResult::getErrorText() {
				return mErrorText;
			}

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude