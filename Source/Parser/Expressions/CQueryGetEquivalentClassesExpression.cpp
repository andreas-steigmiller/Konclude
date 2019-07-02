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

#include "CQueryGetEquivalentClassesExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CQueryGetEquivalentClassesExpression::CQueryGetEquivalentClassesExpression(const QString& name, CClassTermExpression* classTermExp) 
					: CNameAssociator(name),CClassTermExpressionAssociator(classTermExp) {
			}


			CQueryGetEquivalentClassesExpression::~CQueryGetEquivalentClassesExpression() {
			}


			CBuildExpression::ExpressionType CQueryGetEquivalentClassesExpression::getType() {
				return BETGETEQUIVALENTCLASSES;
			}


			cint64 CQueryGetEquivalentClassesExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CClassTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CQueryGetEquivalentClassesExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CQueryGetEquivalentClassesExpression* compExpCast = dynamic_cast<CQueryGetEquivalentClassesExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CClassTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CQueryGetEquivalentClassesExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CClassTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude