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

#include "CConfigDependedCalculationFactory.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Calculation {


				CConfigDependedCalculationFactory::CConfigDependedCalculationFactory(CTaskHandleAlgorithmBuilder* taskHandleAlgBuilder) {
					mTaskHandleAlgBuilder = taskHandleAlgBuilder;
				}


				CConfigDependedCalculationFactory::~CConfigDependedCalculationFactory() {
				}



				CCalculationManager *CConfigDependedCalculationFactory::createCalculationManager(CConfigurationProvider* configurationProvider) {
					CCalculationManager* calculationManager = nullptr;

					CConfigurationBase* config = nullptr;
					if (configurationProvider) {
						config = configurationProvider->getCurrentConfiguration();
					}

					CWatchDog *watchDog = 0;

					bool configErrorFlag = false;

					bool taskCalcManager = false;
					bool workCalcManager = false;

					QString calcManagerString = CConfigDataReader::readConfigString(config,"Konclude.Execution.CalculationManager",QString(),&configErrorFlag);
					if (!configErrorFlag) {
						if (calcManagerString.isEmpty() || calcManagerString == "Konclude.Calculation.Calculator.ConcurrentTaskCalculationManager") {
							taskCalcManager = true;
						} else {
							taskCalcManager = true;
						}
					} else {
						taskCalcManager = true;
					}

					if (taskCalcManager) {
						calculationManager = new CConcurrentTaskCalculationManager(watchDog);
					}

					return calculationManager;
				}


				CCalculationManager* CConfigDependedCalculationFactory::initializeManager(CCalculationManager* calculationManager, CConfigurationProvider* configurationProvider) {
					CCalculationEnvironmentFactory *calcContextFactory = new CConfigDependedCalculationEnvironmentFactory(mTaskHandleAlgBuilder);
					calculationManager->initializeManager(calcContextFactory,configurationProvider);
					return calculationManager;
				}



			}; // end namespace Calculation

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
