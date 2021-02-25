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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONERKONCLUDEEVALUATIONPROVIDER_H
#define KONCLUDE_TEST_EVALUATION_CREASONERKONCLUDEEVALUATIONPROVIDER_H

// Libraries includes
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QObject>

// Namespace includes
#include "CReasonerEvaluationTerminationResult.h"
#include "CReasonerEvaluationProvider.h"


// Other includes
#include "Config/CConfigDataReader.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerKoncludeEvaluationProvider
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerKoncludeEvaluationProvider : public QObject, public CReasonerEvaluationProvider {
				Q_OBJECT

				// public methods
				public:
					//! Constructor
					CReasonerKoncludeEvaluationProvider();

					//! Destructor
					virtual ~CReasonerKoncludeEvaluationProvider();


					virtual bool createReasoner(CConfiguration *config);
					virtual CReasonerEvaluationTerminationResult* destroyReasoner();

					virtual QString getReasonerAddressString();

				// protected methods
				protected:
					bool assistTermination(Q_PID processID);

				public slots:
					void processError(QProcess::ProcessError error);
					void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

				// protected variables
				protected:
					QString mReasonerName;
					QString mReasonerBinaryFile;
					QString mReasonerBinaryArguments;
					cint64 mReasonerPort;

					QProcess* mProcess;
					bool mProcessError;
					bool mProcessFinished;

					cint64 mKillTimeout;
					QString mKillScriptString;
					QString mKillScriptArgumentsString;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONERKONCLUDEEVALUATIONPROVIDER_H
