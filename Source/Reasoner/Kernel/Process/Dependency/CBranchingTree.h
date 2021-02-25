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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CBRANCHINGTREE_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CBRANCHINGTREE_H

// Libraries includes


// Namespace includes
#include "DependencySettings.h"
#include "CBranchTreeNode.h"
#include "CIndependentBaseDependencyNode.h"
#include "CDependencyNode.h"


// Other includes
#include "Reasoner/Kernel/Process/CProcessContext.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				namespace Dependency {

					/*! 
					 *
					 *		\class		CBranchingTree
					 *		\author		Andreas Steigmiller
					 *		\version	0.1
					 *		\brief		TODO
					 *
					 */
					class CBranchingTree {
						// public methods
						public:
							//! Constructor
							CBranchingTree(CProcessContext* processContext = nullptr);

							CBranchingTree* initBranchingTree(CBranchingTree* tree);

							CBranchTreeNode* getBranchTreeNode(Task::CSatisfiableCalculationTask* task, bool forceBranchCreation = false);

							CDependencyNode* getBaseDependencyNode(bool create = true);

						// protected methods
						protected:

						// protected variables
						protected:
							CProcessContext* mPocessContext;
							CBranchTreeNode* mRootNode;
							CBranchTreeNode* mCurrNode;
							CBranchTreeNode* mPrevCurrNode;

							CDependencyNode* mBaseDepNode;


						// private methods
						private:

						// private variables
						private:


					};

				}; // end namespace Dependency

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CBRANCHINGTREE_H
