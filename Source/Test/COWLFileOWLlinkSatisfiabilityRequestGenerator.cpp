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

#include "COWLFileOWLlinkSatisfiabilityRequestGenerator.h"


namespace Konclude {

	namespace Test {



		COWLFileOWLlinkSatisfiabilityRequestGenerator::COWLFileOWLlinkSatisfiabilityRequestGenerator(CConfiguration *config) 
					: CLogIdentifier("::Konclude::Test::OWLFileOWLlinkClassifyRequestGenerator",this),mConfig(config) {


			mReasonerTestKBName = "http://www.konclude.com/evaluation-testsuite/test-ontology";
		}


		COWLFileOWLlinkSatisfiabilityRequestGenerator::~COWLFileOWLlinkSatisfiabilityRequestGenerator() {
		}



		QDomDocument COWLFileOWLlinkSatisfiabilityRequestGenerator::createRequestDocument() {
			QDomDocument document;

			QString owllinkNS = "http://www.owllink.org/owllink#";
			QString owlNS = "http://www.w3.org/2002/07/owl#";

			QDomElement rootNode;

			rootNode = document.createElementNS(owllinkNS,"RequestMessage");
			//rootNode.setAttributeNS("xsi:schemaLocation","http://www.owllink.org/owllink# http://www.owllink.org/owllink-20091116.xsd");
			rootNode.setAttributeNS("http://www.w3.org/2001/XMLSchema-instance","xsi:schemaLocation","http://www.owllink.org/owllink# http://www.owllink.org/owllink-20091116.xsd");
			rootNode.setAttribute("xmlns:owl",owlNS);
			document.appendChild(rootNode);

			return document;
		}


		QSet<QString> COWLFileOWLlinkSatisfiabilityRequestGenerator::loadClassEntities(const QString& requestFileName) {

			CConcreteOntologyEntityCollectorBuilder entityGenBuilder;
			COWL2QtXMLOntologyStreamParser* owl2Parser = new COWL2QtXMLOntologyStreamParser(&entityGenBuilder);
			owl2Parser->parseOntology(requestFileName);
			delete owl2Parser;

			QSet<QString> classNameSet = *entityGenBuilder.getClassNameSet();
			return classNameSet;
		}



		bool COWLFileOWLlinkSatisfiabilityRequestGenerator::generateOWLlinkSatisfiabilityRequests(const QString& requestFileName, const QString& responseFileName, cint64 testCount) {
			bool generatedTestfiles = false;
			QSet<QString> classNameTestingSet;
			QSet<QString> classNameSet = loadClassEntities(requestFileName);
			cint64 maxTestClassCount = qMin(testCount,(cint64)classNameSet.count());

			cint64 slashIndex = qMax(requestFileName.lastIndexOf("/"),requestFileName.lastIndexOf("\\"));
			QString fileNameString = requestFileName;
			if (slashIndex >= 0) {
				fileNameString = requestFileName.mid(slashIndex+1);
			}
			uint randInitNumber = 0;
			for (QString::const_iterator it = requestFileName.constBegin(), itEnd = requestFileName.constEnd(); it != itEnd; ++it) {
				QChar nameChar(*it);
				randInitNumber += nameChar.unicode();
			}
			qsrand(randInitNumber);

			for (cint64 testNr = 0; testNr < maxTestClassCount; ++testNr) {				
				cint64 nextTextClassNr = qrand() % classNameSet.count();
				QSet<QString>::iterator classNameIt = classNameSet.begin();
				for (cint64 i = 0; i < nextTextClassNr; ++i) {
					++classNameIt;
				}
				QString nextTestClassName = *classNameIt;
				cint64 nextClassSwitchCount = classNameSet.count();
				while (classNameTestingSet.contains(nextTestClassName) && nextClassSwitchCount > 0) {
					++classNameIt;
					++nextTextClassNr;
					if (classNameIt == classNameSet.end()) {
						classNameIt == classNameSet.begin();
						nextTextClassNr = 0;
					}
					nextTestClassName = *classNameIt;
					--nextClassSwitchCount;
				}
				if (classNameTestingSet.contains(nextTestClassName)) {
					break;
				}

				QString classNameTestFileAppendix = getClassNameRequestFileAppendix(nextTestClassName,nextTextClassNr);
				QString testResponseFileName = QString(responseFileName).arg(classNameTestFileAppendix);

				generatedTestfiles |= generateOWLlinkSatisfiabilityRequest(requestFileName,testResponseFileName,nextTestClassName);
			}
			return generatedTestfiles;
		}

		QString COWLFileOWLlinkSatisfiabilityRequestGenerator::getClassNameRequestFileAppendix(const QString& className, cint64 testNumber) {
			QString classNameFileAppendixString = className;
			cint64 lastSlash = classNameFileAppendixString.lastIndexOf("/");
			if (lastSlash != -1) {
				classNameFileAppendixString = classNameFileAppendixString.mid(lastSlash+1);
			}
			cint64 lastGatt = classNameFileAppendixString.lastIndexOf("#");
			if (lastGatt != -1) {
				classNameFileAppendixString = classNameFileAppendixString.mid(lastGatt+1);
			}
			QString nextClassNameFileAppendixString;
			cint64 maxClassChar = 15;
			for (QString::const_iterator charIt = classNameFileAppendixString.constBegin(), charItEnd = classNameFileAppendixString.constEnd(); charIt != charItEnd; ++charIt) {
				QChar character(*charIt);
				if (character.isLetterOrNumber()) {
                    char asciiCharacter = character.toLatin1();
					if (asciiCharacter != 0 && maxClassChar > 0) {
						--maxClassChar;
						nextClassNameFileAppendixString += asciiCharacter;
					}
				}
			}
			return QString("%1~%2").arg(testNumber).arg(nextClassNameFileAppendixString);
		}


		bool COWLFileOWLlinkSatisfiabilityRequestGenerator::generateOWLlinkSatisfiabilityRequest(const QString& requestFileName, const QString& responseFileName, const QString& className) {

			QDomDocument requestDocument = createRequestDocument();
			QDomElement rootElement = requestDocument.documentElement();

			QDomElement cKBElement = requestDocument.createElement("CreateKB");
			cKBElement.setAttribute("kb",mReasonerTestKBName);
			rootElement.appendChild(cKBElement);

			QDomElement loadElement = requestDocument.createElement("LoadOntologies");
			loadElement.setAttribute("kb",mReasonerTestKBName);
			QDomElement ontoIRIElement = requestDocument.createElement("OntologyIRI");
			ontoIRIElement.setAttribute("IRI",QString("file:%1").arg(requestFileName));
			loadElement.appendChild(ontoIRIElement);
			rootElement.appendChild(loadElement);

			QDomElement isSatClassElement = requestDocument.createElement("IsClassSatisfiable");
			isSatClassElement.setAttribute("kb",mReasonerTestKBName);
			QDomElement classElement = requestDocument.createElement("owl:Class");
			classElement.setAttribute("IRI",className);
			isSatClassElement.appendChild(classElement);
			rootElement.appendChild(isSatClassElement);

			QDomElement releaseKBElement = requestDocument.createElement("ReleaseKB");
			releaseKBElement.setAttribute("kb",mReasonerTestKBName);
			rootElement.appendChild(releaseKBElement);

			QFile outFile(responseFileName);
			if (outFile.open(QIODevice::WriteOnly)) {
				outFile.write(requestDocument.toByteArray());
				outFile.close();
			}

			LOG(INFO,getLogDomain(),logTr("Generated OWLlink satisfiability request '%1' for ontology '%2'.").arg(responseFileName).arg(requestFileName),this);

			return false;
		}


	}; // end namespace Test

}; // end namespace Konclude
