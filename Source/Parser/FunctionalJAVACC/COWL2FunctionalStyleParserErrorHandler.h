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

#ifndef KONCLUDE_PARSER_FUNCTIONALJAVACC_COWL2FUNCTIONALSTYLEPARSERERRORHANDLER_H
#define KONCLUDE_PARSER_FUNCTIONALJAVACC_COWL2FUNCTIONALSTYLEPARSERERRORHANDLER_H


// Libraries includes
#include <QString>


// Namespace includes
#include "FunctionalJAVACCSettings.h"
#include "ErrorHandler.h"


// Other includes



// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;



namespace Konclude {

	namespace Parser {

		using namespace Expression;

		namespace FunctionalJAVACC {


			/*! 
			 *
			 *		\class		COWL2FunctionalStyleParserErrorHandler
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COWL2FunctionalStyleParserErrorHandler : public ErrorHandler {
				// public methods
				public:
					//! Constructor
					COWL2FunctionalStyleParserErrorHandler();


					virtual void handleUnexpectedToken(int expectedKind, JAVACC_STRING_TYPE expectedToken, Token *actual, OWL2FunctionalStyleParser *parser);

					virtual void handleParseError(Token *last, Token *unexpected, JAVACC_SIMPLE_STRING production, OWL2FunctionalStyleParser *parser);

					bool hasError();
					QString getErrorString();

				// protected methods
				protected:

				// protected variables
				protected:
					QString mErrorString;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace FunctionalJAVACC

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_FUNCTIONALJAVACC_COWL2FUNCTIONALSTYLEPARSERERRORHANDLER_H