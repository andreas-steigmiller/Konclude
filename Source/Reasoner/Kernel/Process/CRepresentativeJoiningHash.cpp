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

#include "CRepresentativeJoiningHash.h"
#include "CRepresentativeVariableBindingPathSetData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CRepresentativeJoiningHash::CRepresentativeJoiningHash(CProcessContext* context) : CPROCESSHASH<TRepIDPair,CRepresentativeJoiningHashData>(context) {
					mContext = context;
				}


				CRepresentativeJoiningHash* CRepresentativeJoiningHash::initRepresentativeJoiningHash(CRepresentativeJoiningHash* prevHash) {
					if (prevHash) {
						CPROCESSHASH<TRepIDPair, CRepresentativeJoiningHashData>::operator=(*prevHash);
					} else {
						CPROCESSHASH<TRepIDPair,CRepresentativeJoiningHashData>::clear();
					}
					return this;
				}

				CRepresentativeJoiningData* CRepresentativeJoiningHash::getRepresentativeJoiningData(CRepresentativeVariableBindingPathSetData* leftRepData, CRepresentativeVariableBindingPathSetData* rightRepData, bool create) {
					CRepresentativeJoiningData* repJoiningData = nullptr;
					cint64 leftID = leftRepData->getRepresentativeID();
					cint64 rightID = rightRepData->getRepresentativeID();
					if (create) {
						CRepresentativeJoiningHashData& data = CPROCESSHASH<TRepIDPair,CRepresentativeJoiningHashData>::operator[](TRepIDPair(leftID,rightID));
						repJoiningData = data.mVarBindPathJoiningData;
						if (!data.mVarBindPathJoiningData) {
							repJoiningData = CObjectParameterizingAllocator< CRepresentativeJoiningData,CProcessContext* >::allocateAndConstructAndParameterize(mContext->getUsedMemoryAllocationManager(),mContext);
							data.mVarBindPathJoiningData = repJoiningData;
						}
					} else {
						const CRepresentativeJoiningHashData& data = CPROCESSHASH<TRepIDPair,CRepresentativeJoiningHashData>::value(TRepIDPair(leftID,rightID));
						repJoiningData = data.mVarBindPathJoiningData;
					}
					return repJoiningData;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
