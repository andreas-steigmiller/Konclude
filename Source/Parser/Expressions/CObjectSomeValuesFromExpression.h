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

#ifndef KONCLUDE_PARSER_EXPRESSION_COBJECTSOMEVALUESFROMEXPRESSION_H
#define KONCLUDE_PARSER_EXPRESSION_COBJECTSOMEVALUESFROMEXPRESSION_H


// Libraries includes
#include <QString>


// Namespace includes
#include "CClassTermExpression.h"
#include "CObjectPropertyTermExpression.h"
#include "CObjectPropertyTermExpressionAssociator.h"
#include "CClassTermExpressionAssociator.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Parser {

		namespace Expression {


			/*! 
			 *	\class		CObjectSomeValuesFromExpression
			 *	\author		Andreas Steigmiller
			 *	\version	0.1
			 *	\brief		TODO
			 */
			class CObjectSomeValuesFromExpression : public CClassTermExpression, public CObjectPropertyTermExpressionAssociator, public CClassTermExpressionAssociator {
				// public methods
				public:
					//! Constructor
					CObjectSomeValuesFromExpression(CObjectPropertyTermExpression *expression1, CClassTermExpression *expression2);

					//! Destructor
					virtual ~CObjectSomeValuesFromExpression();

					virtual ExpressionType getType();

					virtual cint64 getStructuralHashValue();
					virtual bool compareStructuralEquivalence(CBuildExpression* compareExpression);


					virtual bool visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor);

				// protected methods
				protected:

				// private methods
				private:


				// private variables
				private:

			};

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_EXPRESSION_COBJECTSOMEVALUESFROMEXPRESSION_H