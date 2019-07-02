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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CCACHEVALUE_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CCACHEVALUE_H

// Libraries includes
#include <QHash>

// Namespace includes


// Other includes
#include "Utilities/CTrible.hpp"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		CCacheValue
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCacheValue : public CTrible<qint64> {
					// public methods
					public:
						enum CACHEVALUEIDENTIFIER { CACHEVALCONCEPTONTOLOGYTAG, CACHEVALNEGATEDCONCEPTONTOLOGYTAG, 
									CACHEVALROLEONTOLOGYTAG, CACHEVALUETERMINATOR, CACHEVALUENOTHING, CACHEVALTAGANDCONCEPT, CACHEVALTAGANDNEGATEDCONCEPT};

						//! Constructor
						CCacheValue();
						CCacheValue(const qint64 &first, const qint64 &second, const qint64 &third);
						CCacheValue(const qint64 &tag, const qint64 &identification, const CACHEVALUEIDENTIFIER &cacheValueIdentifier = CACHEVALCONCEPTONTOLOGYTAG);

						CCacheValue* initCacheValue(const qint64 &tag, const qint64 &identification, const CACHEVALUEIDENTIFIER &cacheValueIdentifier = CACHEVALCONCEPTONTOLOGYTAG);

						bool isCachedConcept();
						bool isCachedRole();
						bool isCachedTerminator();
						bool isCachedNothing();

						
						qint64 getTag() const;
						qint64 getIdentification() const;
						CACHEVALUEIDENTIFIER getCacheValueIdentifier() const;


					// protected methods
					protected:

					// protected variables
					protected:

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CCACHEVALUE_H
