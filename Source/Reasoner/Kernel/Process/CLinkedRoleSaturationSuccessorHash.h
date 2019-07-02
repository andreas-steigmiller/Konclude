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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CLINKEDROLESATURATIONSUCCESSORHASH_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CLINKEDROLESATURATIONSUCCESSORHASH_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CLinkedRoleSaturationSuccessorData.h"


// Other includes
#include "Reasoner/Ontology/CRole.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CLinkedRoleSaturationSuccessorHash
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CLinkedRoleSaturationSuccessorHash {
					// public methods
					public:
						//! Constructor
						CLinkedRoleSaturationSuccessorHash(CProcessContext* context = nullptr);


						CLinkedRoleSaturationSuccessorHash* initRoleSuccessorHash();
						CLinkedRoleSaturationSuccessorHash* copyRoleSuccessorHash(CLinkedRoleSaturationSuccessorHash* copyRoleSuccHash);


						bool hasActiveLinkedSuccessor(CRole* role, CIndividualSaturationProcessNode* linkedIndi, CRole* creationRole = nullptr, cint64 linkCount = 1);
						bool hasLinkedSuccessor(CRole* role, CIndividualSaturationProcessNode* linkedIndi, CRole* creationRole = nullptr, cint64 linkCount = 1);


						CLinkedRoleSaturationSuccessorHash* addLinkedSuccessor(CRole* role, CIndividualSaturationProcessNode* linkedIndi, CRole* creationRole, cint64 linkCount = 1);
						CLinkedRoleSaturationSuccessorHash* addLinkedVALUESuccessor(CRole* role, cint64 nominalID, CRole* creationRole);
						CLinkedRoleSaturationSuccessorHash* deactivateLinkedSuccessor(CRole* role, CIndividualSaturationProcessNode* linkedIndi, CRole* creationRole);
						CLinkedRoleSaturationSuccessorHash* addExtensionSuccessor(CRole* role, CIndividualSaturationProcessNode* linkedIndi, CRole* creationRole, cint64 linkCount = 1);

						CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* getLinkedRoleSuccessorHash();
						
						CLinkedRoleSaturationSuccessorData*& getLinkedRoleSuccessorData(CRole* role, bool create = true);

						CConceptSaturationDescriptor* getLastExaminedConceptDescriptor();
						CLinkedRoleSaturationSuccessorHash* setLastExaminedConceptDescriptor(CConceptSaturationDescriptor* conDes);

					// protected methods
					protected:
						CSaturationSuccessorData*& getLinkedRoleSuccessorData(CLinkedRoleSaturationSuccessorData*& roleSuccData, cint64 linkedIndiID, bool forceNewCreation = false);

					// protected variables
					protected:
						CProcessContext* mContext;
						CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*> mRoleSuccDataHash;
						CConceptSaturationDescriptor* mLastExaminedConDes;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CLINKEDROLESATURATIONSUCCESSORHASH_H