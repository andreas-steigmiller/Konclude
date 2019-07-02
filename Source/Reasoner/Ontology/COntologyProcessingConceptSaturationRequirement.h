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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYPROCESSINGCONCEPTSATURATIONREQUIREMENT_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYPROCESSINGCONCEPTSATURATIONREQUIREMENT_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "COntologyProcessingRequirement.h"
#include "CConcreteOntology.h"
#include "COntologyProcessingStatus.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		COntologyProcessingConceptSaturationRequirement
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyProcessingConceptSaturationRequirement : public COntologyProcessingRequirement {
				// public methods
				public:
					//! Constructor
					COntologyProcessingConceptSaturationRequirement(CConcept* concept, bool conceptNegation, cint64 requiredProcessingFlags = COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, cint64 forbiddenProcessingFlags = 0, cint64 requiredErrorFlags = COntologyProcessingStatus::PSSUCESSFULL, cint64 forbiddenErrorFlags = 0);


					virtual bool isRequirementSatisfied(COntology* ontology);

					virtual COntologyProcessingConceptSaturationRequirement* submitRequirementUpdate(cint64 processingFlags, cint64 errorFlags);
					COntologyProcessingStatus* getProcessingStatus();
					
					CConcept* getConcept();
					bool getConceptNegation();

					virtual const QString getRequirementFailureString(COntology* ontology);

					virtual bool isDynamicRequirement();

				// protected methods
				protected:

				// protected variables
				protected:
					COntologyProcessingStatus mProcessingStatus;

					cint64 mRequiredProcessingFlags;
					cint64 mForbiddenProcessingFlags;

					cint64 mRequiredErrorFlags;
					cint64 mForbiddenErrorFlags;

					CConcept* mConcept;
					bool mConceptNegation;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYPROCESSINGCONCEPTSATURATIONREQUIREMENT_H