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

#ifndef KONCLUDE_REASONER_GENERATOR_CAPPROXIMATEDSATURATIONCALCULATIONJOBGENERATOR_H
#define KONCLUDE_REASONER_GENERATOR_CAPPROXIMATEDSATURATIONCALCULATIONJOBGENERATOR_H

// Libraries includes


// Namespace includes


// Other includes
#include "Reasoner/Query/CApproximatedSaturationCalculationJob.h"

#include "Reasoner/Consistiser/CSaturationConceptReferenceLinking.h"

#include "Reasoner/Consistence/CSaturationData.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Query;
		using namespace Consistiser;
		using namespace Consistence;

		namespace Generator {

			/*! 
			 *
			 *		\class		CApproximatedSaturationCalculationJobGenerator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CApproximatedSaturationCalculationJobGenerator {
				// public methods
				public:
					//! Constructor
					CApproximatedSaturationCalculationJobGenerator(CConcreteOntology *ontology);

					CApproximatedSaturationCalculationJob* getApproximatedSaturationCalculationJob(cint64 approximationSaturationMode, CSaturationData* saturationData = nullptr);
					CApproximatedSaturationCalculationJob* extendApproximatedSaturationCalculationJobConstruction(CRole* role, CConcept* concept, bool conceptNegation, bool processing, CSaturationConceptReferenceLinking* satConRefLinking, CApproximatedSaturationCalculationJob* calculationJob = nullptr);
					CApproximatedSaturationCalculationJob* extendApproximatedSaturationCalculationJobProcessing(CSaturationConceptReferenceLinking* satConRefLinking, CApproximatedSaturationCalculationJob* calculationJob = nullptr);
					CApproximatedSaturationCalculationJob* extendApproximatedSaturationCalculationJobProcessing(CIndividual* individual, CSaturationIndividualReferenceLinking* satIndiRefLinking, CApproximatedSaturationCalculationJob* calculationJob = nullptr);
					CApproximatedSaturationCalculationJob* extendApproximatedSaturationCalculationJobProcessing(cint64 indiId, CSaturationIndividualReferenceLinking* satIndiRefLinking, CApproximatedSaturationCalculationJob* calculationJob = nullptr);

				// protected methods
				protected:

				// protected variables
				protected:		
					CConcreteOntology *mOntology;

				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_GENERATOR_CAPPROXIMATEDSATURATIONCALCULATIONJOBGENERATOR_H
