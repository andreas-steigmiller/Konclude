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

#ifndef KONCLUDE_SCHEDULER_EVENTS_CSENDTASKPROCESSEVENT_H
#define KONCLUDE_SCHEDULER_EVENTS_CSENDTASKPROCESSEVENT_H



// Libraries includes


// Namespace includes
#include "../SchedulerSettings.h"
#include "../CTask.h"


// Other includes
#include "Concurrent/CEvent.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent;

	namespace Scheduler {

		namespace Events {



			/*! 
			 *	\class		CSendTaskProcessEvent
			 *	\author		Andreas Steigmiller
			 *	\version	0.1
			 *	\brief		
			 */
			class CSendTaskProcessEvent : public CEvent {
				// public methods
				public:
					//! Constructor
					CSendTaskProcessEvent(CMemoryPool* memoryPool = nullptr);

					//! Destructor
					virtual ~CSendTaskProcessEvent();


					static const cint64 EVENTTYPEID = CSENDTASKPROCESSEVENTTYPEID;

					CSendTaskProcessEvent* setTask(CTask* task);
					CTask* getTask();

					CSendTaskProcessEvent* setSchedulingTask(bool schedulable);
					bool isSchedulingTask();


				// protected methods
				protected:

				// protected variables
				protected:
					CTask* mTask;
					bool mSchedulingTask;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Events

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_EVENTS_CSENDTASKPROCESSEVENT_H
