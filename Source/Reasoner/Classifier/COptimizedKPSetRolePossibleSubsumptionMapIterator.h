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

#ifndef KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDKPSETROLEPOSSIBLESUBSUMPTIONMAPITERATOR_H
#define KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDKPSETROLEPOSSIBLESUBSUMPTIONMAPITERATOR_H

// Libraries includes
#include <QMap>

// Namespace includes
#include "ClassifierSettings.h"
#include "CPropertyPossibleSubsumptionMapIterator.h"
#include "COptimizedKPSetRolePossibleSubsumptionData.h"


// Other includes
#include "Reasoner/Ontology/CRoleTagComparer.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Classifier {


			/*! 
			 *
			 *		\class		COptimizedKPSetRolePossibleSubsumptionMapIterator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedKPSetRolePossibleSubsumptionMapIterator : public CPropertyPossibleSubsumptionMapIterator {
				// public methods
				public:
					// Constructor
					COptimizedKPSetRolePossibleSubsumptionMapIterator();
					COptimizedKPSetRolePossibleSubsumptionMapIterator(QMap<CRoleTagComparer,COptimizedKPSetRolePossibleSubsumptionData*>::iterator itBegin, QMap<CRoleTagComparer,COptimizedKPSetRolePossibleSubsumptionData*>::iterator itEnd);

					bool hasNext();

					bool isSubsumptionConfirmed();
					bool isSubsumptionInvalided();
					CRole* getSubsumptionRole();

					bool invalidateSubsumption();
					bool confirmSubsumption();

					bool moveNext();

				// protected methods
				protected:

				// protected variables
				protected:
					QMap<CRoleTagComparer,COptimizedKPSetRolePossibleSubsumptionData*>::iterator mItBegin;
					QMap<CRoleTagComparer,COptimizedKPSetRolePossibleSubsumptionData*>::iterator mItEnd;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDKPSETROLEPOSSIBLESUBSUMPTIONMAPITERATOR_H