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

#include "CRoleReapplyHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CRoleReapplyHash::CRoleReapplyHash(CProcessContext* processContext) : CPROCESSHASH<CRole*,CReapplyQueue>(processContext) {
				}


				CRoleReapplyHash::~CRoleReapplyHash() {
				}


				CRoleReapplyHash* CRoleReapplyHash::initRoleReapplyHash(CRoleReapplyHash* prevRoleReapplyHash) {
					if (prevRoleReapplyHash) {
						CPROCESSHASH<CRole*, CReapplyQueue>::operator =(*prevRoleReapplyHash);
					} else {
						CPROCESSHASH<CRole*,CReapplyQueue>::clear();
					}
					return this;
				}



				CReapplyQueue* CRoleReapplyHash::getRoleReapplyQueue(CRole* role, bool create) {
					if (create) {
						return &this->operator[](role);
					} else {
						if (contains(role)) {
							return &this->operator[](role);
						} else {
							return nullptr;
						}
					}
				}


				bool CRoleReapplyHash::containsRoleReapplyQueue(CRole* role) {
					return contains(role);
				}


				CReapplyQueueIterator CRoleReapplyHash::getRoleReapplyIterator(CRole* role, bool clearDynamicReapplyQueue) {
					CReapplyQueue* reapplyQueue = nullptr;
					if (tryGetValuePointer(role,reapplyQueue)) {
						return reapplyQueue->getIterator(clearDynamicReapplyQueue);
					}
					return CReapplyQueueIterator(nullptr,nullptr);
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
