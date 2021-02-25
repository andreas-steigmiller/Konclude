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

#include "CCommandRecordData.h"


namespace Konclude {

	namespace Control {

		namespace Command {


			CCommandRecordData::CCommandRecordData(CCommand *command, const QString &recordDomain, double recordErrorLevel, CCommandRecordData *nextRecordData) 
					: CContextRecordData(recordErrorLevel,nextRecordData) {

				comm = command;
				recDomain = recordDomain;
				prevCommandRecordData = nextRecordData;
			}


			CCommandRecordData::~CCommandRecordData() {
			}


			CCommand *CCommandRecordData::getCommand() {
				return comm;
			}

			CCommandRecordData *CCommandRecordData::setCommand(CCommand *command) {
				comm = command;
				return this;
			}


			QString CCommandRecordData::getRecordDomain() {
				return recDomain;
			}

			CCommandRecordData *CCommandRecordData::setRecordDomain(const QString &recordDomain) {
				recDomain = recordDomain;
				return this;
			}

			CCommandRecordData *CCommandRecordData::getNextCommandRecordData() {
				return prevCommandRecordData;
			}


			CContextRecordData *CCommandRecordData::setNextRecordData(CContextRecordData *recordData) {
				CContextRecordData::setNextRecordData(recordData);
				CCommandRecordData *commRecData = dynamic_cast<CCommandRecordData *>(recordData);
				if (commRecData) {
					prevCommandRecordData = commRecData;
				}
				return this;
			}



		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude
