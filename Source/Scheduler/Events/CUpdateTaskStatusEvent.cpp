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

#include "CUpdateTaskStatusEvent.h"


namespace Konclude {

	namespace Scheduler {

		namespace Events {


			CUpdateTaskStatusEvent::CUpdateTaskStatusEvent(CMemoryPool* memoryPool) 
					: CEvent(EVENTTYPEID,memoryPool) {

				mParentTask = nullptr;
				mUpdateTask = nullptr;
			}


			CUpdateTaskStatusEvent::~CUpdateTaskStatusEvent() {
			}

			CUpdateTaskStatusEvent* CUpdateTaskStatusEvent::setParentTask(CTask* task) {
				mParentTask = task;
				return this;
			}

			CTask* CUpdateTaskStatusEvent::getParentTask() {
				return mParentTask;
			}

			CUpdateTaskStatusEvent* CUpdateTaskStatusEvent::setUpdateTask(CTask* task) {
				mUpdateTask = task;
				return this;
			}

			CTask* CUpdateTaskStatusEvent::getUpdateTask() {
				return mUpdateTask;
			}



		}; // end namespace Events

	}; // end namespace Scheduler

}; // end namespace Konclude