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

#ifndef KONCLUDE_TEST_ANALYSE_READER_CCALCULATIONTABLEAUCOMPLETIONALGORITHMUPDATEREADER_H
#define KONCLUDE_TEST_ANALYSE_READER_CCALCULATIONTABLEAUCOMPLETIONALGORITHMUPDATEREADER_H

// Libraries includes
#include <QString>


// Namespace includes


// Other includes
#include "Test/Analyse/AnalyseSettings.h"
#include "Test/Analyse/CAnalyseValue.h"
#include "Test/Analyse/CAnalyseValueUpdateReader.h"
#include "Test/Analyse/CAnalyseValueDescription.h"
#include "Test/Analyse/CIntegerAnalyseValue.h"
#include "Test/Analyse/CConvertStringAnalyseValue.h"

#include "Reasoner/Kernel/Algorithm/CCalculationTableauCompletionTaskHandleAlgorithm.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;
	using namespace Scheduler;

	using namespace Reasoner::Kernel::Algorithm;

	namespace Test {

		namespace Analyse {

			namespace Reader {

				/*! 
				 *
				 *		\class		CCalculationTableauCompletionAlgorithmUpdateReader
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCalculationTableauCompletionAlgorithmUpdateReader : public CAnalyseValueUpdateReader {
					// public methods
					public:
						//! Constructor
						CCalculationTableauCompletionAlgorithmUpdateReader(CCalculationTableauCompletionTaskHandleAlgorithm *algorithm);

						//! Destructor
						virtual ~CCalculationTableauCompletionAlgorithmUpdateReader();

						virtual CAnalyseValue *getAnalyseValueUpdate(CAnalyseValueDescription *valueDes);

						virtual CAnalyseValueDescription *getAtmostRuleCountValueDescription();
						virtual CAnalyseValueDescription *getAtleastRuleCountValueDescription();
						virtual CAnalyseValueDescription *getOrRuleCountValueDescription();
						virtual CAnalyseValueDescription *getAndRuleCountValueDescription();
						virtual CAnalyseValueDescription *getAllRuleCountValueDescription();
						virtual CAnalyseValueDescription *getSomeRuleCountValueDescription();

						virtual CAnalyseValueDescription *getRulesCountValueDescription();

					// protected methods
					protected:

					// protected variables
					protected:
						CCalculationTableauCompletionTaskHandleAlgorithm *alg;

						CAnalyseValueDescription *atmostRuleCountValueDescription;
						CAnalyseValueDescription *atleastRuleCountValueDescription;
						CAnalyseValueDescription *orRuleCountValueDescription;
						CAnalyseValueDescription *andRuleCountValueDescription;
						CAnalyseValueDescription *someRuleCountValueDescription;
						CAnalyseValueDescription *allRuleCountValueDescription;

						CAnalyseValueDescription *rulesCountValueDescription;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Reader

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_ANALYSE_READER_CCALCULATIONTABLEAUCOMPLETIONALGORITHMUPDATEREADER_H