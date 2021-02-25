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

#ifndef KONCLUDE_PARSER_CSPARQLKNOWLEDGEBASESPLITTINGOPERATIONDATA_H
#define KONCLUDE_PARSER_CSPARQLKNOWLEDGEBASESPLITTINGOPERATIONDATA_H

// Libraries includes
#include <QString>
#include <QStringList>

// Namespace includes


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Utilities;

	namespace Parser {



		/*! 
		 *
		 *		\class		CSPARQLKnowledgeBaseSplittingOperationData
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CSPARQLKnowledgeBaseSplittingOperationData {
			// public methods
			public:
				enum SPARQL_COMMAND_TYPE {
					SPARQL_QUERY, SPARQL_UPDATE_MODIFY, SPARQL_UPDATE_MANAGE
				};


				//! Constructor
				CSPARQLKnowledgeBaseSplittingOperationData(SPARQL_COMMAND_TYPE operationType, const QString& knowledgeBaseString, const QStringList& operationPartStrings);


				SPARQL_COMMAND_TYPE getOperationType();
				QString getKnowledgeBaseString();
				QStringList getOperationPartStringList();


			// protected methods
			protected:


			// protected variables
			protected:
				SPARQL_COMMAND_TYPE mOperationType;
				QString mKnowledgeBaseString;
				QStringList mOperationPartStrings;


			// private methods
			private:


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CSPARQLKNOWLEDGEBASESPLITTINGOPERATIONDATA_H
