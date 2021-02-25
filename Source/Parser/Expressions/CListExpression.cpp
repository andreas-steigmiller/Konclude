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

#include "CListExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CListExpression::CListExpression(CBuildExpression *expression1, CBuildExpression *expression2, CBuildExpression *expression3) {
				if (expression1) {
					append(expression1);
				}
				if (expression2) {
					append(expression2);
				}
				if (expression3) {
					append(expression3);
				}
			}


			CListExpression::~CListExpression() {
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
