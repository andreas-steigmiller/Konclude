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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CREUSINGREVIEWDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CREUSINGREVIEWDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CIndividualProcessNode.h"
#include "CProcessContext.h"


// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CReusingReviewData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CReusingReviewData {
					// public methods
					public:
						//! Constructor
						CReusingReviewData(CProcessContext* context = nullptr);

						CReusingReviewData* initReviewData(CReusingReviewData* sigBlockRevData);

						CReusingReviewData* insert(cint64 depth, cint64 indiID);
						bool contains(cint64 indiID);
						CReusingReviewData* remove(cint64 indiID);

						bool isEmpty();
						bool hasNextIndividualID();
						cint64 takeNextIndividualID();

					// protected methods
					protected:

					// protected variables
					protected:
						CPROCESSSET<cint64> mIndividualSet;
						CPROCESSMAP<cint64,cint64> mDepthIndividualMap;
						CProcessContext* mContext;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CREUSINGREVIEWDATA_H
