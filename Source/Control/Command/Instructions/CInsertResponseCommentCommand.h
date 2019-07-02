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

#ifndef KONCLUDE_Control_COMMAND_INSTRUCTIONS_CINSERTRESPONSECOMMENTCOMMAND_H
#define KONCLUDE_Control_COMMAND_INSTRUCTIONS_CINSERTRESPONSECOMMENTCOMMAND_H

// Libraries includes
#include <QString>

// Namespace includes


// Other includes

#include "Control/Command/CommandSettings.h"
#include "Control/Command/CCommand.h"
#include "Control/Command/CPreconditionCommand.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				/*! 
				 *
				 *		\class		CInsertResponseCommentCommand
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CInsertResponseCommentCommand : public CPreconditionCommand {
					// public methods
					public:
						//! Constructor
						CInsertResponseCommentCommand(const QString &responseComment, CCommand *parentSuperCommand = 0);

						//! Destructor
						virtual ~CInsertResponseCommentCommand();

						virtual qint64 getCommandTag();
						virtual QString getBriefCommandDescription();

						virtual QString getResponseComment();

					// protected methods
					protected:

					// protected variables
					protected:
						QString comment;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_INSTRUCTIONS_CINSERTRESPONSECOMMENTCOMMAND_H
