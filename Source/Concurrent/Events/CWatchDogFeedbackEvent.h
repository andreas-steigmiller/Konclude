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
#ifndef KONCLUDE_CONCURRENT_EVENTS_CWATCHDOGFEEDBACKEVENT_H
#define KONCLUDE_CONCURRENT_EVENTS_CWATCHDOGFEEDBACKEVENT_H

#include <QEvent>

#include "CControlEvent.h"

#include "../ConcurrentSettings.h"

namespace Konclude {

	namespace Concurrent {

		namespace Events {


			/*! 
			 *	\class		CWatchDogFeedbackEvent
			 *	\author		Andreas Steigmiller
			 *	\version	0.1
			 *	\brief		
			 */
			class CWatchDogFeedbackEvent : public CControlEvent {
				// public methods
				public:
					//! Constructor
					CWatchDogFeedbackEvent(qint64 threadID = 0, qint64 updateNumber = 0);

					//! Destructor
					virtual ~CWatchDogFeedbackEvent();

					qint64 getThreadID();
					void seThreadID(qint64 threadID);

					void setUpdateNumber(qint64 updateNumber);
					qint64 getUpdateNumber();


				// protected methods
				protected:


				// private members
				private:
					qint64 id;
					qint64 uNr;

			};

		}; // end namespace Events

	}; // end namespace Concurrent

}; // end namespace Konclude

#endif // KONCLUDE_CONCURRENT_EVENTS_CWATCHDOGFEEDBACKEVENT_H
