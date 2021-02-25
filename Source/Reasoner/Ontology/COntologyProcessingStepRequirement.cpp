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

#include "COntologyProcessingStepRequirement.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyProcessingStepRequirement::COntologyProcessingStepRequirement(COntologyProcessingStep* requiredProcessingStep, cint64 requiredProcessingFlags, cint64 forbiddenProcessingFlags, cint64 requiredErrorFlags, cint64 forbiddenErrorFlags) {
				mRequiredProcessingStep = requiredProcessingStep;
				mRequiredProcessingFlags = requiredProcessingFlags;
				mForbiddenProcessingFlags = forbiddenProcessingFlags;
				mRequiredErrorFlags = requiredErrorFlags;
				mForbiddenErrorFlags = forbiddenErrorFlags;
			}


			COntologyProcessingStep* COntologyProcessingStepRequirement::getRequiredProcessingStep() {
				return mRequiredProcessingStep;
			}

			COntologyProcessingStepRequirement* COntologyProcessingStepRequirement::setRequiredProcessingStep(COntologyProcessingStep* requiredProcessingStep) {
				mRequiredProcessingStep = requiredProcessingStep;
				return this;
			}

			bool COntologyProcessingStepRequirement::isRequirementSatisfied(COntology* ontology) {
				if (ontology) {
					CConcreteOntology* conOntology = dynamic_cast<CConcreteOntology*>(ontology);
					if (conOntology) {
						COntologyProcessingSteps* processingSteps = conOntology->getProcessingSteps();
						if (processingSteps) {
							COntologyProcessingStepDataVector* proStepDataVec = processingSteps->getOntologyProcessingStepDataVector();
							COntologyProcessingStepData* proStepData = proStepDataVec->getProcessingStepData(mRequiredProcessingStep->getProcessingStepID());
							if (proStepData) {
								COntologyProcessingStatus* proStatus = proStepData->getProcessingStatus();
								if (!proStatus->hasPartialProcessingFlags(mRequiredProcessingFlags)) {
									return false;
								}
								if (proStatus->hasPartialProcessingFlags(mForbiddenProcessingFlags)) {
									return false;
								}
								if (!proStatus->hasPartialErrorFlags(mRequiredErrorFlags)) {
									return false;
								}
								if (proStatus->hasPartialErrorFlags(mForbiddenErrorFlags)) {
									return false;
								}
								return true;
							}
						}
					}
				}
				return false;
			}

			COntologyProcessingStepRequirement* COntologyProcessingStepRequirement::submitRequirementUpdate(cint64 processingFlags, cint64 errorFlags) {
				return this;
			}


			const QString COntologyProcessingStepRequirement::getRequirementFailureString(COntology* ontology) {
				QString failureString;
				if (!isRequirementSatisfied(ontology)) {
					failureString = QString("'%1' processing step failed.").arg(mRequiredProcessingStep->getProcessingStepName());
				}
				return failureString;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
