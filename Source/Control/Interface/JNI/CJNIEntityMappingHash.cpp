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

#include "CJNIEntityMappingHash.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace JNI {


#ifdef KONCLUDE_COMPILE_JNI_INTERFACE


				CJNIEntityMappingHash::CJNIEntityMappingHash() {
				}

				CJNIEntityMappingHash::~CJNIEntityMappingHash() {
				}

				jobject CJNIEntityMappingHash::getMappedObject(CBuildExpression* expression) {
					return (jobject)mEntityMapHash.value(expression,nullptr);
				}

				CJNIEntityMappingHash* CJNIEntityMappingHash::inserObjectMapping(CBuildExpression* expression, jobject obj) {
					mEntityMapHash.insert(expression,obj);
					return this;
				}

#endif


			}; // end namespace JNI 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
