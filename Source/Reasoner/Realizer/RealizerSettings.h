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

#ifndef KONCLUDE_REASONER_REALIZER_REALIZERSETTINGS
#define KONCLUDE_REASONER_REALIZER_REALIZERSETTINGS

// Libraries includes
#include <QtGlobal>
#include <QEvent>
#include <QPair>

// Namespace includes


// Other includes


// Logger includes


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {

			/*! 
			 *
			 *		\file		RealizerSettings
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */



			// forward declarations
			class CRealizingTestingItem;
			class COntologyRealizingItem;
			class COptimizedKPSetConceptInstancesItem;
			class COptimizedKPSetIndividualItem;
			class COptimizedKPSetConceptInstancesData;
			class COptimizedKPSetRoleInstancesData;
			class COptimizedKPSetRoleInstancesItem;
			class CRealizingTestingStep;
			class CPossibleInstancesIndividualsMergingData;

			class COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData;
			class COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData;
			class COptimizedRepresentativeKPSetOntologyRealizingItem;
			class COntologyRealizingDynamicRequirmentProcessingData;

			typedef QPair<COptimizedKPSetRoleInstancesItem*,bool> TRoleItemInversionPair;

			// Custom Events >= 2000
			const QEvent::Type EVENTPREALIZEONTOLOGY					= (QEvent::Type)2000;
			const QEvent::Type EVENTCALLBACKREALIZEDONTOLOGY			= (QEvent::Type)2001;
			const QEvent::Type EVENTREALIZINGCALCULATEDCALLBACK			= (QEvent::Type)2002;
			const QEvent::Type EVENTREALIZATIONMESSAGE					= (QEvent::Type)2003;



		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude


#endif // end KONCLUDE_REASONER_REALIZER_REALIZERSETTINGS