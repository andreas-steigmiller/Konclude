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

#ifndef KONCLUDE_REASONER_KERNEL_TASK_CSATISFIABLETASKANSWERERQUERYINGMATERIALIZATIONADAPTER_H
#define KONCLUDE_REASONER_KERNEL_TASK_CSATISFIABLETASKANSWERERQUERYINGMATERIALIZATIONADAPTER_H

// Libraries includes


// Namespace includes


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/CConcept.h"

#include "Reasoner/Answerer/CAnsweringMessageDataObserver.h"
#include "Reasoner/Answerer/CAnsweringHandler.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Answerer;

		namespace Kernel {

			namespace Task {

				/*! 
				 *
				 *		\class		CSatisfiableTaskAnswererQueryingMaterializationAdapter
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSatisfiableTaskAnswererQueryingMaterializationAdapter {
					// public methods
					public:
						//! Constructor
						CSatisfiableTaskAnswererQueryingMaterializationAdapter(QList<CIndividual*> indiList, cint64 matDataIdentifer, CAnsweringHandler* answeringHandler, CConcreteOntology* testingOntology = nullptr, CAnsweringMessageDataObserver* observer = nullptr);

						QList<CIndividual*> getIndividualList();
						CConcreteOntology* getTestingOntology();
						CAnsweringHandler* getAnsweringHandler();

						CAnsweringMessageDataObserver* getAnswererMessageDataObserver();

						cint64 getMaterializationDataIdentifier();

					// protected methods
					protected:

					// protected variables
					protected:
						QList<CIndividual*> mIndiList;
						CConcreteOntology* mOntology;
						CAnsweringHandler* mAnsweringHandler;
						CAnsweringMessageDataObserver* mMessageObserver;
						cint64 mMatDataIdentifier;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Task

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_TASK_CSATISFIABLETASKANSWERERQUERYINGMATERIALIZATIONADAPTER_H
