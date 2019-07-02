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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CSUBSUMPTIONCLASSIFIERFACTORY_H
#define KONCLUDE_REASONER_CLASSIFIER_CSUBSUMPTIONCLASSIFIERFACTORY_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "CSubsumptionClassifier.h"

// Other includes
#include "Config/CConfigurationBase.h"

#include "Reasoner/Ontology/COntology.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		using namespace Ontology;

		namespace Classifier {


			/*! 
			 *
			 *		\class		CSubsumptionClassifierFactory
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CSubsumptionClassifierFactory {
				// public methods
				public:
					//! Constructor
					CSubsumptionClassifierFactory();

					//! Destructor
					virtual ~CSubsumptionClassifierFactory();


					virtual CSubsumptionClassifier *createClassifier(CConcreteOntology *ontology, CConfigurationBase *config) = 0;
					virtual CSubsumptionClassifier *createBackgroundClassifier(CConcreteOntology *ontology, CConfigurationBase *config) = 0;


				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CSUBSUMPTIONCLASSIFIERFACTORY_H
