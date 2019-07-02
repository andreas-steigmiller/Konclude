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

#ifndef KONCLUDE_CONTROL_LOADER_CCLICONSISTENCYBATCHPROCESSINGLOADER_H
#define KONCLUDE_CONTROL_LOADER_CCLICONSISTENCYBATCHPROCESSINGLOADER_H

// Libraries includes
#include <QString>
#include <QRegExp>
#include <QTime>
#include <QDir>

// Namespace includes
#include "LoaderSettings.h"
#include "CCLIBatchProcessingLoader.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


#include <stdio.h>
#include <iostream>


namespace Konclude {

	using namespace Logger;

	namespace Control {

		namespace Loader {

			/*! 
			 *
			 *		\class		CCLIConsistencyBatchProcessingLoader
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CCLIConsistencyBatchProcessingLoader : public CCLIBatchProcessingLoader {
				// public methods
				public:
					//! Constructor
					CCLIConsistencyBatchProcessingLoader();

					//! Destructor
					virtual ~CCLIConsistencyBatchProcessingLoader();


				// protected methods
				protected:

					void createConsistencyTestingCommands();
					virtual void createTestingCommands();

					virtual void writeCommandOutput(const QString& outputFileName, CCommand* processedCommand);


				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_LOADER_CCLICONSISTENCYBATCHPROCESSINGLOADER_H
