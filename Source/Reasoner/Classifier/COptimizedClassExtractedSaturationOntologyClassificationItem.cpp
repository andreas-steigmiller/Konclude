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

#include "COptimizedClassExtractedSaturationOntologyClassificationItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COptimizedClassExtractedSaturationOntologyClassificationItem::COptimizedClassExtractedSaturationOntologyClassificationItem(CConfigurationBase *configuration, CClassifierStatistics *nextClassificationStatistics) 
					: COntologyClassificationItem(configuration,nextClassificationStatistics) {
				
				mTopSatTestItem = nullptr;
				mBottomSatTestItem = nullptr;
			}

			QHash<CConcept*,COptimizedClassExtractedSaturationSatisfiableTestingItem*>* COptimizedClassExtractedSaturationOntologyClassificationItem::getClassSatisfiableTestItemHash() {
				return &mClassSatItemHash;
			}
			QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*>* COptimizedClassExtractedSaturationOntologyClassificationItem::getClassSatisfiableTestItemList() {
				return &mClassSatTestItemContainer;
			}


			COptimizedClassExtractedSaturationSatisfiableTestingItem* COptimizedClassExtractedSaturationOntologyClassificationItem::getTopConceptSatisfiableTestItem() {
				return mTopSatTestItem;
			}

			COptimizedClassExtractedSaturationSatisfiableTestingItem* COptimizedClassExtractedSaturationOntologyClassificationItem::getBottomConceptSatisfiableTestItem() {
				return mBottomSatTestItem;
			}


			COptimizedClassExtractedSaturationOntologyClassificationItem* COptimizedClassExtractedSaturationOntologyClassificationItem::initTopBottomSatisfiableTestingItems(COptimizedClassExtractedSaturationSatisfiableTestingItem* topItem, COptimizedClassExtractedSaturationSatisfiableTestingItem* bottomItem) {
				mTopSatTestItem = topItem;
				mBottomSatTestItem = bottomItem;
				return this;
			}

			COptimizedClassExtractedSaturationSatisfiableTestingItem* COptimizedClassExtractedSaturationOntologyClassificationItem::getClassSatisfiableTestItem(CConcept* satTestConcept, bool create) {
				COptimizedClassExtractedSaturationSatisfiableTestingItem* item = nullptr;
				item = mClassSatItemHash.value(satTestConcept);
				if (!item && create) {
					item = new COptimizedClassExtractedSaturationSatisfiableTestingItem();
					item->initSatisfiableTestingItem(satTestConcept);
					mClassSatItemHash.insert(satTestConcept,item);
					mClassSatTestItemContainer.append(item);
				}
				return item;
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude