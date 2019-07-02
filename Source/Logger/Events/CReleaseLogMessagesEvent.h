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
#ifndef KONCLUDE_LOGGER_EVENTS_CRELEASELOGMESSAGESEVENT_H
#define KONCLUDE_LOGGER_EVENTS_CRELEASELOGMESSAGESEVENT_H

#include <QEvent>
#include <QLinkedList>
#include <QStringList>

#include "Concurrent/Events/CCustomEvent.h"

#include "../LoggerSettings.h"
#include "../CLogMessage.h"


namespace Konclude {

	using namespace Concurrent;
	using namespace Events;

	namespace Logger {

		namespace Events {


			/*! 
			 *	\class		CReleaseLogMessagesEvent
			 *	\author		Andreas Steigmiller
			 *	\version	0.1
			 *	\brief		
			 */
			class CReleaseLogMessagesEvent : public CCustomEvent {
				// public methods
				public:
					//! Constructor
					CReleaseLogMessagesEvent(QLinkedList<CLogMessage *> &logMessages);
					CReleaseLogMessagesEvent();

					//! Destructor
					virtual ~CReleaseLogMessagesEvent();				


					void setLogMessages(QLinkedList<CLogMessage *> &logMessages);
					QLinkedList<CLogMessage *> getLogMessages();


				// protected methods
				protected:


				// private members
				private:
					QLinkedList<CLogMessage *> messages;


			};

		}; // end namespace Events

	}; // end namespace Logger

}; // end namespace Konclude

#endif // KONCLUDE_LOGGER_EVENTS_CRELEASELOGMESSAGESEVENT_H
