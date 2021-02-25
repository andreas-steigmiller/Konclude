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

#ifndef KONCLUDE_REASONER_KERNEL_MANAGER_CREASONINGTASKDATA_H
#define KONCLUDE_REASONER_KERNEL_MANAGER_CREASONINGTASKDATA_H

// Libraries includes
#include <QTime>
#include <QString>

// Namespace includes
#include "ManagerSettings.h"
#include "COntologyRequirementPreparingData.h"
#include "COntologyRequirementPair.h"

// Other includes
#include "Reasoner/Kernel/Manager/Events/CCalcedQueryEvent.h"

#include "Concurrent/Callback/CCallbackData.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent::Callback;

	namespace Reasoner {

		namespace Kernel {

			namespace Manager {

				using namespace Events;


				/*! 
				 *
				 *		\class		CReasoningTaskData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CReasoningTaskData {
					// public methods
					public:
						//! Constructor
						CReasoningTaskData();

						//! Destructor
						~CReasoningTaskData();

						CCallbackData* mCallback;
						CCalcedQueryEvent* mFinQueryCallback;
						QTime mStartTime;
						cint64 mDepCount;
						bool mAnswered;
						QHash<QString,cint64>* mCalcStat;
						CRequirementPreparingData* mReqPrepData;

					// protected methods
					protected:

					// protected variables
					protected:

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Manager

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_MANAGER_CREASONINGTASKDATA_H
