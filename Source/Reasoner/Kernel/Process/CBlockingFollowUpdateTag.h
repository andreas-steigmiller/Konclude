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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CBLOCKINGFOLLOWUPDATETAG_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CBLOCKINGFOLLOWUPDATETAG_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessTag.h"
#include "CProcessTagger.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CBlockingFollowUpdateTag
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBlockingFollowUpdateTag : public CProcessTag {
					// public methods
					public:
						//! Constructor
						CBlockingFollowUpdateTag(cint64 processTag = 0);

						//! Destructor
						virtual ~CBlockingFollowUpdateTag();

						cint64 getBlockingFollowTag();
						CBlockingFollowUpdateTag* setBlockingFollowTag(cint64 BlockingFollowTag);
						CBlockingFollowUpdateTag* setBlockingFollowTag(CProcessTagger* processTagger);
						CBlockingFollowUpdateTag* initBlockingFollowTag(cint64 BlockingFollowTag);
						CBlockingFollowUpdateTag* initBlockingFollowTag(CProcessTagger* processTagger);
						bool updateBlockingFollowTag(cint64 BlockingFollowTag);
						bool updateBlockingFollowTag(CProcessTagger* processTagger);

						
						bool isBlockingFollowTagUpdated(cint64 BlockingFollowTag);
						bool isBlockingFollowTagUpToDate(cint64 BlockingFollowTag);
						bool isBlockingFollowTagUpdated(CProcessTagger* processTagger);
						bool isBlockingFollowTagUpToDate(CProcessTagger* processTagger);


					// protected methods
					protected:

					// protected variables
					protected:

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CBLOCKINGFOLLOWUPDATETAG_H