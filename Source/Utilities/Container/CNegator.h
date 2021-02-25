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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CNEGATOR_H
#define KONCLUDE_UTILITIES_CONTAINER_CNEGATOR_H

// Namespace includes
#include "ContainerSettings.h"

// Library includes


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {


			/*! 
			*
			*		\class		CNegator
			*		\author		Andreas Steigmiller
			*		\version	0.1
			*		\brief		TODO
			*
			*/
			class CNegator {
				// public methods
				public:
					//! Constructor
					inline CNegator() : mNegation(false) {}

					inline CNegator(const CNegator& negator) {
						mNegation = negator.mNegation;
					}

					inline CNegator(const bool& negated) {
						mNegation = negated;
					}

					//! Destructor
					inline ~CNegator() {}

					inline bool isNegated() const {
						return mNegation;
					}

					inline CNegator* initNeg(bool negation) {
						mNegation = negation;
						return this;
					}

					inline bool getNegation() const {
						return isNegated();
					}

					inline CNegator* setNegation(bool negated) {
						mNegation = negated;
						return this;
					}

					inline bool toggleNegation(bool toggle = true) {
						return mNegation = mNegation^toggle;
					}

				// protected methods
				protected:

				// protected variables
				protected:
					bool mNegation;

				// private methods
				private:

				// private variables
				private:
			};


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude


#endif // KONCLUDE_UTILITIES_CONTAINER_CNEGATOR_H
