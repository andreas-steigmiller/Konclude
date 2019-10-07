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

#ifndef KONCLUDE_PARSER_EXPRESSION_CLISTVARIABLEASSOCIATOR_H
#define KONCLUDE_PARSER_EXPRESSION_CLISTVARIABLEASSOCIATOR_H


// Libraries includes


// Namespace includes
#include "ExpressionSettings.h"
#include "CExpressionVariable.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Parser {

		namespace Expression {

			/*! 
			 *
			 *		\class		CListExpressionVariableAssociator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CListVariableAssociator {
				// public methods
				public:
					//! Constructor
					CListVariableAssociator(CExpressionVariable *variable1 = 0, CExpressionVariable *variable2 = 0, CExpressionVariable *variable3 = 0);
					CListVariableAssociator(const CEXPRESSIONLIST<CExpressionVariable*>& variableList);
					CListVariableAssociator(CEXPRESSIONLIST<CExpressionVariable*>* variableList);

					CListVariableAssociator* initExpressionAssociator(CExpressionVariable *variable1 = 0, CExpressionVariable *variable2 = 0, CExpressionVariable *variable3 = 0);

					CEXPRESSIONLIST<CExpressionVariable*>* getVariableList();
					const CEXPRESSIONLIST<CExpressionVariable*>* getConstVariableList() const;
					cint64 getVariableCount();

					virtual cint64 getStructuralHashValue();
					virtual bool compareStructuralEquivalence(const CListVariableAssociator& listExpressionAssociator);


				// protected methods
				protected:

				// protected variables
				protected:
					CEXPRESSIONLIST<CExpressionVariable*> mVariableList;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_EXPRESSION_CLISTVARIABLEASSOCIATOR_H