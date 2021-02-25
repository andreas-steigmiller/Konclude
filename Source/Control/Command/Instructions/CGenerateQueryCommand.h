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

#ifndef KONCLUDE_CONTROL_COMMAND_CGENERATEQUERYCOMMAND_H
#define KONCLUDE_CONTROL_COMMAND_CGENERATEQUERYCOMMAND_H

// Libraries includes
#include <QString>


// Namespace includes
#include "CQueryCommandProvider.h"
#include "CQueryGeneratorCommandProvider.h"

// Other includes
#include "Control/Command/CommandSettings.h"
#include "Control/Command/CCommand.h"
#include "Control/Command/CPreconditionCommand.h"
#include "Control/Command/CCommandProcessedPrecondition.h"

#include "Reasoner/Ontology/COntologyProcessingRequirement.h"
#include "Reasoner/Ontology/COntologyProcessingStepVector.h"
#include "Reasoner/Ontology/COntologyProcessingRequirementExpander.h"
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Reasoner/Generator/CQueryBuilderGenerator.h"

// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Reasoner::Ontology;
	using namespace Reasoner::Generator;

	namespace Control {

		namespace Command {

			namespace Instructions {

				/*! 
				 *
				 *		\class		CGenerateQueryCommand
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CGenerateQueryCommand : public CPreconditionCommand, public CQueryCommandProvider {
					// public methods
					public:
						//! Constructor
						CGenerateQueryCommand(CQueryGeneratorCommandProvider* queryCommandProvider, CCommand *parentSuperCommand = 0);

						//! Destructor
						virtual ~CGenerateQueryCommand();

						virtual qint64 getCommandTag();

						virtual QString getBriefCommandDescription();

						virtual CQueryBuilderGenerator* getQueryGenerator();

						virtual CCommand* getCommand();

						virtual CQuery *getQuery();
						virtual CGenerateQueryCommand *setQuery(CQuery *knowledgeBaseQuery);

					// protected methods
					protected:

					// protected variables
					protected:
						CQueryGeneratorCommandProvider* mQueryCommandProvider;
						CQuery* mQuery;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_COMMAND_CGENERATEQUERYCOMMAND_H
