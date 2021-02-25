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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CCONCEPTOFINTERESTACTIVATIONTRIGGERINGDATA_H
#define KONCLUDE_REASONER_ONTOLOGY_CCONCEPTOFINTERESTACTIVATIONTRIGGERINGDATA_H

// Libraries includes


// Namespace includes
#include "CConcept.h"
#include "CConceptOfInterestActivationLinker.h"


// Other includes
#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CConceptOfInterestActivationTriggeringData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConceptOfInterestActivationTriggeringData {
				// public methods
				public:
					//! Constructor
					CConceptOfInterestActivationTriggeringData();

					CConceptOfInterestActivationLinker* getActivationLinker();
					CConceptOfInterestActivationTriggeringData* addActivationLinker(CConceptOfInterestActivationLinker* activationLinker);

					CConcept* getCandidateConcept();
					CConcept* getTriggeringConcept();

					CConceptOfInterestActivationTriggeringData* setCandidateConcept(CConcept* candidateConcept);
					CConceptOfInterestActivationTriggeringData* setTriggeringConcept(CConcept* triggeringConcept);

				// protected methods
				protected:

				// protected variables
				protected:
					CConceptOfInterestActivationLinker* mActivationLinker;
					CConcept* mCandidateConcept;
					CConcept* mTriggeringConcept;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CCONCEPTOFINTERESTACTIVATIONTRIGGERINGDATA_H
