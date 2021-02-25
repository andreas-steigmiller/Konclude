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

#include "CExtractKnowledgeBaseEntitiesCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CExtractKnowledgeBaseEntitiesCommand::CExtractKnowledgeBaseEntitiesCommand(CExtractedKnowledgeBaseEntitiesContainer* extractionContainer, CKnowledgeBaseEntitiesExtractionSpecification* extractSpec, CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, CCommand *parentSuperCommand) 
						: CPreconditionCommand(parentSuperCommand) {

					revProvider = ontologyRevisionProviderCommand; 
					if (revProvider) {
						addCommandPrecondition(new CCommandProcessedPrecondition(revProvider->getCommand()));
					}
					mExtractSpec = extractSpec;
					mExtractionContainer = extractionContainer;
				}


				CExtractKnowledgeBaseEntitiesCommand::~CExtractKnowledgeBaseEntitiesCommand() {
				}

				qint64 CExtractKnowledgeBaseEntitiesCommand::getCommandTag() {
					return EXTRACTKNOWLEDGEBASEENTITIESCOMMAND;
				}


				QString CExtractKnowledgeBaseEntitiesCommand::getBriefCommandDescription() {
					return QString("Extract-Knowledge-Base-Entities Command");
				}


				CCommand *CExtractKnowledgeBaseEntitiesCommand::getCommand() {
					return this;
				}


				COntologyRevision *CExtractKnowledgeBaseEntitiesCommand::getOntologyRevision() {
					COntologyRevision *rev = 0;
					if (revProvider) {
						rev = revProvider->getOntologyRevision();
					}
					return rev;
				}

				CKnowledgeBaseEntitiesExtractionSpecification* CExtractKnowledgeBaseEntitiesCommand::getEntitiesExtractionSpecification() {
					return mExtractSpec;
				}


				CExtractedKnowledgeBaseEntitiesContainer* CExtractKnowledgeBaseEntitiesCommand::getExtractionContainer() {
					return mExtractionContainer;
				}



			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude
