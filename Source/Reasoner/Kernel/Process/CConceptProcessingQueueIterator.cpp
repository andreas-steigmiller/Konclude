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

#include "CConceptProcessingQueueIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CConceptProcessingQueueIterator::CConceptProcessingQueueIterator() {
					mDataVec = nullptr;
					mDesCount = 0;
					mDefaultPriorityDescriptorLinker = nullptr;
					mSortedPriorityDescriptorLinker = nullptr;

					mDesIndex = 0;

				}


				CConceptProcessingQueueIterator::CConceptProcessingQueueIterator(CConceptProcessingPriorityQueueData* dataVec, cint64 desCount) {
					mDataVec = dataVec;
					mDesCount = desCount;
					mDefaultPriorityDescriptorLinker = nullptr;
					mSortedPriorityDescriptorLinker = nullptr;

					mDesIndex = 0;

					loadNextDescriptors();
				}


				CConceptProcessingQueueIterator::~CConceptProcessingQueueIterator() {
				}

				void CConceptProcessingQueueIterator::loadNextDescriptors() {
					if (mDesCount > 0) {
						while (!mDefaultPriorityDescriptorLinker && !mSortedPriorityDescriptorLinker && mDesIndex < mMaxIndex) {
							CConceptProcessingPriorityQueueData& mData = mDataVec[mDesIndex];
							mDefaultPriorityDescriptorLinker = mData.mDefaultPriorityDescriptorLinker;
							mSortedPriorityDescriptorLinker = mData.mSortedPriorityDescriptorLinker;
							++mDesIndex;
						}
					}
				}

				CConceptProcessDescriptor* CConceptProcessingQueueIterator::next(bool moveNext) {
					if (!mDefaultPriorityDescriptorLinker && !mSortedPriorityDescriptorLinker) {
						loadNextDescriptors();
					}
					CConceptProcessDescriptor* des = nullptr;
					if (mDefaultPriorityDescriptorLinker) {
						des = mDefaultPriorityDescriptorLinker;
						if (moveNext) {
							mDefaultPriorityDescriptorLinker = mDefaultPriorityDescriptorLinker->getNext();
						}
					} else if (mSortedPriorityDescriptorLinker) {
						des = mSortedPriorityDescriptorLinker;
						if (moveNext) {
							mSortedPriorityDescriptorLinker = mSortedPriorityDescriptorLinker->getNext();
						}
					}
					if (des && moveNext) {
						--mDesCount;
					}
					return des;
				}

				bool CConceptProcessingQueueIterator::hasNext() {
					return mDesCount > 0;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
