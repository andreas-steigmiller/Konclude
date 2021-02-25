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

#ifndef KONCLUDE_RENDERER_CRENDEREDRDFTRIPLEITEMBASELINKER_H
#define KONCLUDE_RENDERER_CRENDEREDRDFTRIPLEITEMBASELINKER_H

// Libraries includes
#include <QDomElement>

// Namespace includes
#include "RendererSettings.h"
#include "CRenderedItemLinker.h"
#include "CRenderedRDFNodeItemBaseLinker.h"


// Other includes



// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

namespace Konclude {

	namespace Renderer {



		/*! 
		*
		*		\class		CRenderedRDFTripleItemBaseLinker
		*		\author		Andreas Steigmiller
		*		\version	0.1
		*		\brief		TODO
		*
		*/
		class CRenderedRDFTripleItemBaseLinker : public CRenderedItemLinker {
			// public methods
			public:
				//! Constructor
				CRenderedRDFTripleItemBaseLinker();
				virtual ~CRenderedRDFTripleItemBaseLinker();

				CRenderedRDFNodeItemBaseLinker* getRDFNodeItem(cint64 pos);
				CRenderedRDFTripleItemBaseLinker* setRDFNodeItem(cint64 pos, CRenderedRDFNodeItemBaseLinker* node);
				CRenderedRDFTripleItemBaseLinker* clearRDFNodeItem();

				CRenderedRDFTripleItemBaseLinker* getNextItemLinker();

			// protected methods
			protected:

			// protected variables
			protected:
				CRenderedRDFNodeItemBaseLinker* mNodes[3];

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Renderer

}; // end namespace Konclude

#endif // KONCLUDE_RENDERER_CRENDEREDRDFTRIPLEITEMBASELINKER_H