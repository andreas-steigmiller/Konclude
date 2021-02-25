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

#include "COWLLinkFileComandLinePreparationTranslator.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace CommandLine {

				COWLLinkFileComandLinePreparationTranslator::COWLLinkFileComandLinePreparationTranslator(CLoaderFactory *loaderFactory) : CCommandLinePreparationTranslator(loaderFactory) {
					mInterfaceTypeString = QString("OWLlink");
					mMinNecessaryArguments = true;
					mFirstArgumentTranslation = false;
				}


				COWLLinkFileComandLinePreparationTranslator::~COWLLinkFileComandLinePreparationTranslator() {
				}

				QStringList COWLLinkFileComandLinePreparationTranslator::combineTranslatedArguments() {
					QStringList translatedArgList;

					if (!mFirstArgumentTranslation) {
						mMinNecessaryArguments = false;
					}

					if (!mRequestFileTranslated) {
						LOG(ERROR,getLogDomain(),logTr("OWLlink request file not specified."),this);
						mMinNecessaryArguments = false;
					}

					if (!mMinNecessaryArguments) {
						LOG(ERROR,getLogDomain(),logTr("Execution with inadequate command line arguments."),this);
						mHelpRequested = true;
					}

					if (!mHelpRequested) {
						if (mLogConfList.isEmpty()) {
							mLogConfList.append(QString("-ConfigurableCoutLogObserverLoader -LoggerConfigurationLoader"));
						}
						translatedArgList += mLogConfList;
						translatedArgList += mGeneralConfList;

						translatedArgList += QString("-DefaultReasonerLoader ");
						translatedArgList += mReasonerConfList;
						translatedArgList += mGeneralConfList;

						if (!mConfigConfList.isEmpty()) {
							translatedArgList += QString("-%1BatchFileLoader +=Konclude.%1.CloseAfterProcessedRequest=false +=Konclude.%1.BlockUntilProcessedRequest=true").arg(mInterfaceTypeString);
							translatedArgList += mConfigConfList;
						}

						translatedArgList += QString("-%1BatchFileLoader +=Konclude.%1.CloseAfterProcessedRequest=true").arg(mInterfaceTypeString);
						if (mExtendendLogging) {
							translatedArgList += QString("+=Konclude.%1.LoggingExtendedProcessingTimes=true").arg(mInterfaceTypeString);
						}
						translatedArgList += mRequestConfList;

						translatedArgList += mMiscellaneousList;
					} else {
						translatedArgList += QString("-ConfigurableCoutLogObserverLoader -CommandLineTranlatorHelpLoader");
					}
					return translatedArgList;
				}

				bool COWLLinkFileComandLinePreparationTranslator::translate(const QString& argumentText, QStringList& remainingArgList) {

					bool argumentTranslated = false;


					QString modArgText = argumentText.trimmed();
					if (!mSkipTranslation) {
						if (CCommandLinePreparationTranslator::translate(argumentText,remainingArgList)) {
							argumentTranslated = true;
						} else {
							if (modArgText.indexOf("-") == 0) {
							} else if (modArgText == "owllinkfile" || modArgText == "file") {
								argumentTranslated = true;
								mFirstArgumentTranslation = true;
							}
						}
					}
					return argumentTranslated;
				}

				bool COWLLinkFileComandLinePreparationTranslator::canTranslate(const QString& string) {
					if (QString::compare(string,"owllinkfile",Qt::CaseInsensitive) == 0 || QString::compare(string,"file",Qt::CaseInsensitive) == 0) {
						return true;
					}
					return false;
				}


			}; // end namespace CommandLine

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
