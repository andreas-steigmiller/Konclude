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

#ifndef KONCLUDE_UTILITIES_CSINGLETONPROVIDER_H
#define KONCLUDE_UTILITIES_CSINGLETONPROVIDER_H

// Libraries includes
#include <QMutex>

// Namespace includes


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Utilities {

		/*!
		 *
		 *		\class		CSingletonProvider
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		template<class T>
		class CSingletonProvider {
			// public Functions
			public:
				//! Constructor
				CSingletonProvider();
				//! Destructor
				virtual ~CSingletonProvider();

				static T *getInstance();
				static bool hasInstance();

			// private Functions
			private:

			// private Variables
			private:
				static QMutex *syncMutex;
				static T *instance;
		
		};


		template<class T>
		T *CSingletonProvider<T>::instance = 0;

		template<class T>
		QMutex *CSingletonProvider<T>::syncMutex = new QMutex();

		template<class T>
		CSingletonProvider<T>::CSingletonProvider() {
		}


		template<class T>
		CSingletonProvider<T>::~CSingletonProvider() {
		}


		template<class T>
		T *CSingletonProvider<T>::getInstance() {
			if (instance == 0) {
				syncMutex->lock();
				if (instance == 0) {
					instance = new T();
				}
				syncMutex->unlock();
			}	
			return instance;
		}


		template<class T>
		bool CSingletonProvider<T>::hasInstance() {
			return instance != 0;
		}


	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CSINGLETONPROVIDER_H
