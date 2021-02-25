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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CNODECHILDRENORDERCLASSIFICATIONITEM_H
#define KONCLUDE_REASONER_CLASSIFIER_CNODECHILDRENORDERCLASSIFICATIONITEM_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "CClassifierStatistics.h"

// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Reasoner/Taxonomy/CTaxonomy.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Taxonomy;

		namespace Classifier {


			/*! 
			 *
			 *		\class		CNodeChildrenOrderClassificationItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CNodeChildrenOrderClassificationItem {
				// public methods
				public:
					//! Constructor
					CNodeChildrenOrderClassificationItem(CHierarchyNode *node, QHash<QPair<CHierarchyNode *,CHierarchyNode *>,double> *subsumptionProbabilisticHash);

					//! Destructor
					virtual ~CNodeChildrenOrderClassificationItem();

					virtual CHierarchyNode *getTestingNode();
					virtual QHash<QPair<CHierarchyNode *,CHierarchyNode *>,double> *getSubsumptionProbabilisticHash();

					virtual QList<QPair<CHierarchyNode *,CHierarchyNode *> > *getSubsumptionCalculationOrderList();



				// protected methods
				protected:

				// protected variables
				protected:
					CHierarchyNode *parentNode;

 					QHash<QPair<CHierarchyNode *,CHierarchyNode *>,double> *subsumProbabilisticHash;
					QList<QPair<CHierarchyNode *,CHierarchyNode *> > subsumCalcOrderList;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CNODECHILDRENORDERCLASSIFICATIONITEM_H
