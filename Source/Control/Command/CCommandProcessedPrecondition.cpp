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

#include "CCommandProcessedPrecondition.h"


namespace Konclude {

	namespace Control {

		namespace Command {


			CCommandProcessedPrecondition::CCommandProcessedPrecondition(CCommand *command) {
				comm = command;
			}


			CCommandProcessedPrecondition::~CCommandProcessedPrecondition() {
			}


			bool CCommandProcessedPrecondition::isPreconditionFullfilled() {
				return !comm || comm->isProcessed();
			}

			CCommandProcessedPrecondition *CCommandProcessedPrecondition::addFullfilledCallback(CCallbackData *callback) {
				if (comm) {
					comm->addProcessedCallback(callback);
				} else {
					callback->doCallback();
				}
				return this;
			}

			CCommand *CCommandProcessedPrecondition::getCommand() {
				return comm;
			}

			CCommandProcessedPrecondition *CCommandProcessedPrecondition::setCommand(CCommand *command) {
				comm = command;
				return this;
			}



		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude