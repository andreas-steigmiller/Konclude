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

#include "CPreconditionCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {


			CPreconditionCommand::CPreconditionCommand(CCommand *parentSuperCommand) 
					: CCommand(parentSuperCommand) {
				commandPreconditionLinker = 0;
			}


			CPreconditionCommand::~CPreconditionCommand() {
			}


			CLinker<CCommandPrecondition *> *CPreconditionCommand::getCommandPreconditionLinker() {
				return commandPreconditionLinker;
			}

			CPreconditionCommand *CPreconditionCommand::addCommandPrecondition(CCommandPrecondition *takeCommandPrecondition) {
				commandPreconditionLinker = new CLinker<CCommandPrecondition *>(takeCommandPrecondition,commandPreconditionLinker);
				return this;
			}


			bool CPreconditionCommand::isProcessable() {
				if (isProcessed()) {
					return false;
				}
				CLinker<CCommandPrecondition *> *preconIt = commandPreconditionLinker;
				while (preconIt) {
					if (!preconIt->getData()->isPreconditionFullfilled()) {
						return false;
					}
					preconIt = preconIt->getNext();
				}
				return true;
			}


			CCommand *CPreconditionCommand::deleteDependencies() {
				deletePreconditions();
				return this;
			}

			CPreconditionCommand *CPreconditionCommand::deletePreconditions() {
				while (commandPreconditionLinker) {
					CCommandPrecondition *pre = commandPreconditionLinker->getData();
					delete pre;
					CLinker<CCommandPrecondition *> *tmpPreIt = commandPreconditionLinker;
					commandPreconditionLinker = (CLinker<CCommandPrecondition *> *)commandPreconditionLinker->getNext();
					delete tmpPreIt;
				}
				return this;
			}



		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude