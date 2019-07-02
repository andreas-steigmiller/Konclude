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

#ifndef KONCLUDE_CONTROL_LOADER_CREASONEREVALUATIONCLASSIFICATIONTIMESCOMPARERLOADER_H
#define KONCLUDE_CONTROL_LOADER_CREASONEREVALUATIONCLASSIFICATIONTIMESCOMPARERLOADER_H

// Libraries includes
#include <QString>
#include <QRegExp>

// Namespace includes
#include "LoaderSettings.h"
#include "CLoader.h"
#include "CLoaderFactory.h"

// Other includes
#include "Test/Evaluation/CReasonerEvaluationCollector.h"
#include "Test/Evaluation/CReasonerEvaluationClassificationTimeExtractor.h"
#include "Test/Evaluation/CReasonerEvaluationGroupRequestReasonerComparisonAnalyser.h"
#include "Test/Evaluation/CReasonerEvaluationAvaragerSummarizer.h"
#include "Test/Evaluation/CReasonerEvaluationGroupRequestReasonerCSVComparisonAnalyser.h"
#include "Test/Evaluation/CReasonerEvaluationGroupRequestReasonerWithMeanMinMaxCSVComparisonAnalyser.h"
#include "Test/Evaluation/CReasonerEvaluationGroupReasonerWinnerCountCSVComparisonAnalyser.h"
#include "Test/Evaluation/CReasonerEvaluationHighchartPlotter.h"
#include "Test/Evaluation/CReasonerEvaluationPathConverter.h"
#include "Test/Evaluation/CReasonerEvaluationExpressivitySelectorFactory.h"
#include "Test/Evaluation/CReasonerEvaluationDirectoryStructureSelectorFactory.h"


// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


#include <stdio.h>
#include <iostream>


namespace Konclude {

	using namespace Logger;
	using namespace Test::Evaluation;

	namespace Control {

		namespace Loader {

			/*! 
			 *
			 *		\class		CReasonerEvaluationClassificationTimesComparerLoader
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationClassificationTimesComparerLoader : public CLoader {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationClassificationTimesComparerLoader();

					//! Destructor
					virtual ~CReasonerEvaluationClassificationTimesComparerLoader();


					virtual CLoader *init(CLoaderFactory *loaderFactory = 0, CConfiguration *config = 0);
					virtual CLoader *load();
					virtual CLoader *exit();



				// protected methods
				protected:

					
				// protected variables
				protected:
					QString mInputDir;
					QString mOutputDir;
					QString mOutputDirClassify;


					CConfiguration *loaderConfig;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_LOADER_CREASONEREVALUATIONCLASSIFICATIONTIMESCOMPARERLOADER_H