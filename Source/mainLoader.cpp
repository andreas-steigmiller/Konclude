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

//#include <vld.h>

#include <QCoreApplication>

#include "CKoncludeInfo.h"

#include "Utilities/UtilitiesSettings.h"

#include "Logger/CLogger.h"
#include "Logger/CConsolePrintLogObserver.h"
#include "Logger/CQtDebugPrintLogObserver.h"


#include "Control/Loader/CCommandLineLoader.h"
#include "Control/Loader/CDefaultLoaderFactory.h"

#include "Control/Interface/CommandLine/CCommandLinePreparationTranslatorSelector.h"


using namespace Konclude;
using namespace Konclude::Logger;
using namespace Konclude::Utilities;
using namespace Konclude::Control::Loader;
using namespace Konclude::Control::Interface::CommandLine;


void replaceFileHeaderForDirectory(const QString& dirString, const QString& replacementString, const QString& removementString) {
	QDir dir(dirString);
	QStringList fileStringList = dir.entryList(QDir::Files);
	foreach (const QString& fileString, fileStringList) {
		bool bug = false;
		if (fileString.contains("CReasonerEvaluationGroupRequestReasonerWithMeanMinMaxCSVComparisonAnalyser")) {
			bug = true;
		}
		QString replacementDataString;
		QFile file(dirString+fileString);
		if (file.open(QIODevice::ReadOnly)) {
			QString dataString = file.readAll();
			dataString.replace("\r\n","\n");
			if (dataString.startsWith(removementString)) {
				dataString.replace(removementString,"");
				if (!dataString.startsWith(replacementString)) {
					replacementDataString = replacementString+dataString;
				}
			} else {
				if (bug) {
					for (cint64 i = 0; i < qMin(dataString.length(),removementString.length()); ++i) {
						QChar dChar = dataString.at(i);
						QChar rChar = removementString.at(i);
						if (dChar != rChar) {
							bool debug = true;
						}
					}
				}
			}
			file.close();
		}
		if (!replacementDataString.isEmpty()) {
			if (file.open(QIODevice::WriteOnly)) {
				file.write(replacementDataString.toLocal8Bit());
				file.close();
			}
		}
	}

	QStringList subDirStringList = dir.entryList(QDir::Dirs);
	foreach (const QString& subDirString, subDirStringList) {
		if (subDirString != "." && subDirString != "..") {
			replaceFileHeaderForDirectory(dirString+subDirString+"/",replacementString,removementString);
		}
	}

}







void correctSparqlQueryTypesForDirectory(const QString& dirString) {
	QDir dir(dirString);
	QStringList fileStringList = dir.entryList(QDir::Files);
	foreach(const QString& fileString, fileStringList) {
		QString replacementDataString;
		QFile file(dirString + "/" + fileString);
		if (file.open(QIODevice::ReadOnly)) {

			QStringList newFileLines;
			bool replaced = false;
			while (!file.atEnd()) {
				QString line = file.readLine();
				QStringList parts = line.split(" ");
				QStringList newParts;
				while (!parts.isEmpty()) {					
					QString part = parts.takeFirst();
					if (part == "rdf:type") {
						newParts.append(part);
						QString nextPart = parts.takeFirst();
						if (nextPart.startsWith("http://")) {
							newParts.append("<" + nextPart + ">");
							replaced = true;
						} else {
							newParts.append(nextPart);
						}
					} else {
						newParts.append(part);
					}
				}
				QString newLine = newParts.join(" ");
				newFileLines.append(newLine);
			}

			file.close();
			if (replaced) {
				if (file.open(QIODevice::WriteOnly)) {
					file.write(newFileLines.join("").toUtf8());
				}
				file.close();
			}
		}
	}

	QStringList subDirStringList = dir.entryList(QDir::Dirs);
	foreach(const QString& subDirString, subDirStringList) {
		if (subDirString != "." && subDirString != "..") {
			correctSparqlQueryTypesForDirectory(dirString + "/" + subDirString);
		}
	}

}



int main(int argc, char *argv[])
{

	//correctSparqlQueryTypesForDirectory("F:\\Data\\UUPR-Eval-GIT\\Evaluation\\Requests\\Q-000\\Queries\\SPARQL\\KoncludeTestQueries\\SpecialSelections\\AbsorptionBasedQueryAnswering\\KoncludeTestQueries\\EntailmentTestingOntologies");

	//replaceFileHeaderForDirectory(
	//	"./Source/",

	//	"/*\n"
	//	" *\t\tCopyright (C) 2013, 2014, 2015 by the Konclude Developer Team.\n"
	//	" *\n"
	//	" *\t\tThis file is part of the reasoning system Konclude.\n"
	//	" *\t\tFor details and support, see <http://konclude.com/>.\n"
	//	" *\n"
	//	" *\t\tKonclude is free software: you can redistribute it and/or modify it under\n"
	//	" *\t\tthe terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)\n"
	//	" *\t\tas published by the Free Software Foundation.\n"
	//	" *\n"
	//	" *\t\tYou should have received a copy of the GNU Lesser General Public License\n"
	//	" *\t\talong with Konclude. If not, see <http://www.gnu.org/licenses/>.\n"
	//	" *\n"
	//	" *\t\tKonclude is distributed in the hope that it will be useful,\n"
	//	" *\t\tbut WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	//	" *\t\tMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more\n"
	//	" *\t\tdetails, see GNU Lesser General Public License.\n"
	//	" *\n",


	//	"/*\n"
	//	" *\t\tCopyright (C) 2013, 2014, 2015 by the Konclude Developer Team.\n"
	//	" *\n"
	//	" *\t\tThis file is part of the reasoning system Konclude.\n"
	//	" *\t\tFor details and support, see <http://konclude.com/>.\n"
	//	" *\n"
	//	" *\t\tKonclude is free software: you can redistribute it and/or modify it under"
	//	" *\t\tthe terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)"
	//	" *\t\tas published by the Free Software Foundation."
	//	" *\n"
	//	" *\t\tYou should have received a copy of the GNU Lesser General Public License\n"
	//	" *\t\talong with Konclude. If not, see <http://www.gnu.org/licenses/>.\n"
	//	" *\n"
	//	" *\t\tKonclude is distributed in the hope that it will be useful,\n"
	//	" *\t\tbut WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	//	" *\t\tMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more\n"
	//	" *\t\tdetails, see GNU Lesser General Public License.\n"
	//	" *\n"
	//	);




	//replaceFileHeaderForDirectory(
	//	"./Source - Kopie (5)/",


	//	"/*\n"
	//	" *\t\tCopyright (C) 2013-2015, 2019 by the Konclude Developer Team.\n"
	//	" *\n"
	//	" *\t\tThis file is part of the reasoning system Konclude.\n"
	//	" *\t\tFor details and support, see <http://konclude.com/>.\n"
	//	" *\n"
	//	" *\t\tKonclude is free software: you can redistribute it and/or modify\n"
	//	" *\t\tit under the terms of version 3 of the GNU General Public License\n"
	//	" *\t\t(LGPLv3) as published by the Free Software Foundation.\n"
	//	" *\n"
	//	" *\t\tKonclude is distributed in the hope that it will be useful,\n"
	//	" *\t\tbut WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	//	" *\t\tMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n"
	//	" *\t\tGNU General Public License for more details.\n"
	//	" *\n"
	//	" *\t\tYou should have received a copy of the GNU General Public License\n"
	//	" *\t\talong with Konclude. If not, see <http://www.gnu.org/licenses/>.\n"
	//	" *\n",

	//	"/*\n"
	//	" *\t\tCopyright (C) 2013, 2014, 2015 by the Konclude Developer Team.\n"
	//	" *\n"
	//	" *\t\tThis file is part of the reasoning system Konclude.\n"
	//	" *\t\tFor details and support, see <http://konclude.com/>.\n"
	//	" *\n"
	//	" *\t\tKonclude is free software: you can redistribute it and/or modify it under\n"
	//	" *\t\tthe terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)\n"
	//	" *\t\tas published by the Free Software Foundation.\n"
	//	" *\n"
	//	" *\t\tYou should have received a copy of the GNU Lesser General Public License\n"
	//	" *\t\talong with Konclude. If not, see <http://www.gnu.org/licenses/>.\n"
	//	" *\n"
	//	" *\t\tKonclude is distributed in the hope that it will be useful,\n"
	//	" *\t\tbut WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	//	" *\t\tMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more\n"
	//	" *\t\tdetails, see GNU Lesser General Public License.\n"
	//	" *\n"


	//	);




	//replaceFileHeaderForDirectory(
	//	"./Source/",


	//	"/*\n"
	//	" *\t\tCopyright (C) 2013-2015, 2019 by the Konclude Developer Team.\n"
	//	" *\n"
	//	" *\t\tThis file is part of the reasoning system Konclude.\n"
	//	" *\t\tFor details and support, see <http://konclude.com/>.\n"
	//	" *\n"
	//	" *\t\tKonclude is free software: you can redistribute it and/or modify\n"
	//	" *\t\tit under the terms of version 3 of the GNU Lesser General Public\n"
	//	" *\t\tLicense (LGPLv3) as published by the Free Software Foundation.\n"
	//	" *\n"
	//	" *\t\tKonclude is distributed in the hope that it will be useful,\n"
	//	" *\t\tbut WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	//	" *\t\tMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n"
	//	" *\t\tGNU (Lesser) General Public License for more details.\n"
	//	" *\n"
	//	" *\t\tYou should have received a copy of the GNU (Lesser) General Public\n"
	//	" *\t\tLicense along with Konclude. If not, see <http://www.gnu.org/licenses/>.\n"
	//	" *\n",

	//	"/*\n"
	//	" *\t\tCopyright (C) 2013-2015, 2019 by the Konclude Developer Team.\n"
	//	" *\n"
	//	" *\t\tThis file is part of the reasoning system Konclude.\n"
	//	" *\t\tFor details and support, see <http://konclude.com/>.\n"
	//	" *\n"
	//	" *\t\tKonclude is free software: you can redistribute it and/or modify\n"
	//	" *\t\tit under the terms of version 3 of the GNU General Public License\n"
	//	" *\t\t(LGPLv3) as published by the Free Software Foundation.\n"
	//	" *\n"
	//	" *\t\tKonclude is distributed in the hope that it will be useful,\n"
	//	" *\t\tbut WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	//	" *\t\tMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n"
	//	" *\t\tGNU General Public License for more details.\n"
	//	" *\n"
	//	" *\t\tYou should have received a copy of the GNU General Public License\n"
	//	" *\t\talong with Konclude. If not, see <http://www.gnu.org/licenses/>.\n"
	//	" *\n"




	//	);







    char *argTest[] =  {

		"-ORECoutLogObserverLoader ",
		//"-CoutLogObserverLoader -DefaultReasonerLoader +Konclude.Calculation.ProcessorCount=1 +Konclude.ORE.ClassHierarchyResult.WriteDirectEquivalences=false",
		//"-OWLlinkBatchFileLoader +=Konclude.OWLlink.CloseAfterProcessedRequest=false +=Konclude.OWLlink.BlockUntilProcessedRequest=true",
		//"+Konclude.OWLlink.RequestFile=./Configs/testing-config.xml",
		//"-OREBatchProcessingLoader +Konclude.ORE.OperationTask=classification +Konclude.ORE.OntologyFile=Tests/00002.owl +Konclude.ORE.ResponseFile=Tests/Test-response.xml +Konclude.ORE.CloseAfterProcessed=false ",
		//"-OREBatchProcessingLoader +Konclude.ORE.OperationTask=classification +Konclude.ORE.OntologyFile=Tests/00757.owl.xml +Konclude.ORE.ResponseFile=Tests/Test-response.xml +Konclude.ORE.CloseAfterProcessed=false ",
		//"-OREBatchProcessingLoader +Konclude.ORE.OperationTask=sat +Konclude.ORE.OntologyFile=Tests/epilepsy.1639.owl.xml +Konclude.ORE.ResponseFile=Tests/Test-response.xml +Konclude.ORE.CloseAfterProcessed=false +Konclude.ORE.IRIName=http://www.semanticweb.org/ontologies/2009/3/EpilepsyOntology.owl#Epilepsy_with_continuous_spike-and-waves_during_slow-wave_sleep_(other_than_Landau-Kleffner_syndrome) ",
		//"-OREBatchProcessingLoader +Konclude.ORE.OperationTask=sat +Konclude.ORE.OntologyFile=Tests/sanou.3090.owl.xml +Konclude.ORE.ResponseFile=Tests/Test-response.xml +Konclude.ORE.CloseAfterProcessed=false +Konclude.ORE.IRIName=http://www.semanticweb.org/associatedmedicine/lavima/2011/10/Ontology1.owl#Ev?nement ",
		//"-OREBatchProcessingLoader +Konclude.ORE.OperationTask=sat +Konclude.ORE.OntologyFile=Tests/EL-GALEN.owl.xml +Konclude.ORE.ResponseFile=Tests/Test-response.xml +Konclude.ORE.CloseAfterProcessed=false +Konclude.ORE.IRIName=http://www.co-ode.org/ontologies/galen#ArteryOfHeadOrNeck ",
		//"-OREBatchProcessingLoader +Konclude.ORE.OperationTask=sat +Konclude.ORE.OntologyFile=Tests/EL-GALEN.owl.xml +Konclude.ORE.ResponseFile=Tests/Test-response.xml +Konclude.ORE.CloseAfterProcessed=false +Konclude.ORE.IRIName=http://www.co-ode.org/ontologies/galen#ApplicationCategory ",
		//"-OREBatchProcessingLoader +Konclude.ORE.OperationTask=sat +Konclude.ORE.OntologyFile=Tests/ORE-pizza.owl.xml +Konclude.ORE.ResponseFile=Tests/Test-response.xml +Konclude.ORE.CloseAfterProcessed=false +Konclude.ORE.IRIName=http://www.co-ode.org/ontologies/pizza/pizza.owl#SloppyGiuseppe ",


		"-CoutLogObserverLoader ",
		//"-OWLFilesIncompatibleAxiomChangerLoader +=Konclude.Test.Converter.InputDirectory=./Tests/Converter/Input-Berkeleybop/ +=Konclude.Test.Converter.OutputDirectory=E:/Data/MyFiles/SemanticWeb/UUPR-Eval-GIT/Evaluation/Ontologies/O-000/WithoutModifications/Berkeleybop/",
		//"-OWLFilesOWLlinkTestcaseGeneratorLoader +=Konclude.Test.Generator.InputDirectory=E:/Data/MyFiles/SemanticWeb/UUPR-Eval-GIT/Evaluation/Ontologies/O-000/ +=Konclude.Test.Generator.OutputDirectory=E:/Data/MyFiles/SemanticWeb/UUPR-Eval-GIT/Evaluation/Requests/Q-000/ ",
		//"-ReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader  +=Konclude.Test.Generator.InputDirectory=E:/Data/MyFiles/SemanticWeb/UUPR-Eval-GIT/Evaluation/Requests/ +=Konclude.Test.Generator.OutputDirectory=E:/Data/MyFiles/SemanticWeb/UUPR-Eval-GIT/Evaluation/Infos/Statistics/Requests/  +=Konclude.Test.Generator.OntologyStatisticsDirectory=E:/Data/MyFiles/SemanticWeb/UUPR-Eval-GIT/Evaluation/Infos/Statistics/Ontologies/ ",
		//"-ReasonerEvaluationOntologyExtractionLoader  +=Konclude.Test.Generator.InputDirectory=E:/Data/MyFiles/SemanticWeb/UUPR-Eval-GIT/Evaluation/Ontologies/ +=Konclude.Test.Generator.OutputDirectory=E:/Data/MyFiles/SemanticWeb/UUPR-Eval-GIT/Evaluation/Extractions/  +=Konclude.Test.Generator.OntologyStatisticsDirectory=E:/Data/MyFiles/SemanticWeb/UUPR-Eval-GIT/Evaluation/Infos/Statistics/Ontologies/  +=Konclude.Test.Generator.FilterFile=E:/Data/MyFiles/SemanticWeb/UUPR-Eval-GIT/Evaluation/Filters/RLNotQLNotELABoxFilter.txt ",
		//"-TestMemoryAccessLoader ",
		//"-TestHashModificationLoader ",
		//"-TestQHashVsQMapLoader ", 
		//"-TestTaskThroughputLoader ",
		//"-GUITaskThroughputTestViewerLoader ",
		//"-GUILogObserverLoader ",
		//"-ConfigurableCoutLogObserverLoader ",
		//"-TestOWLlinkAPIBatchFileLoader +=Konclude.OWLlink.WriteResponseToStandardOutput=TRUE +=Konclude.OWLlink.OWLlinkAPI.Address=http://127.0.0.1:8081",
		//"-PigenholePrincipleGeneratorLoader +=Konclude.Test.PigeonholePrincipleGenerator.OWLlinkOutputFile=./Tests/PHP-ALC-9.xml +=Konclude.Test.PigeonholePrincipleGenerator.ProblemSize=9",

		//"-ReasonerEvaluationProgramLoader +=Konclude.Evaluation.ProgramFile=./Evaluation/Programs/EvaluationProgram022.xml +=Konclude.Evaluation.Platform=Win64",
		//"-ReasonerEvaluationProgramLoader +=Konclude.Evaluation.ProgramFile=./Evaluation/Programs/Querying-SPARQL-PAGOdATests-Single-Fast001.xml +=Konclude.Evaluation.Platform=Win64",
		//"-ReasonerEvaluationProgramLoader +=Konclude.Evaluation.ProgramFile=./Evaluation/Programs/Konclude-Querying-SPARQL-PAGOdATests-Single-Fast001.xml +=Konclude.Evaluation.Platform=Win64",
		"-ReasonerEvaluationProgramLoader +=Konclude.Evaluation.ProgramFile=./Evaluation/Programs/Querying-SPARQL-PAGOdATests-Batch-Fast001.xml +=Konclude.Evaluation.Platform=Win64",
		//"-ReasonerEvaluationProgramLoader +=Konclude.Evaluation.ProgramFile=./Evaluation/Programs/Querying-SPARQL-KoncludeRandomWalkGeneratedTestQueries-Fast001.xml +=Konclude.Evaluation.Platform=Win64",

		//"-LargeJoinTestDataGeneratorLoader +Konclude.Test.LargeJoinTestDataGenerator.OWLlinkOutputFile=./Tests/Rules/LargeJoinTestData-i50000-p10000.owl.xml +Konclude.Test.LargeJoinTestDataGenerator.IndividualCount=50000 +Konclude.Test.LargeJoinTestDataGenerator.PropertyAssertionCount=10000",
		//"-RecursionTestDataGeneratorLoader +Konclude.Test.RecursionTestDataGenerator.OWLlinkOutputFile=./Tests/Rules/RecursionTestData-i500-p200.owl.xml +Konclude.Test.RecursionTestDataGenerator.IndividualCount=500 +Konclude.Test.RecursionTestDataGenerator.PropertyAssertionCount=200",
		//"-TransitiveConnectedELClassesTestDataGeneratorLoader +Konclude.Test.TransitiveConnectedELClassesTestDataGenerator.OWLlinkOutputFile=./Tests/TransitiveConnectedELClassesTestData-c100.owl.xml +Konclude.Test.TransitiveConnectedELClassesTestDataGenerator.ClassesCount=100",
		//"-TransitiveConnectedELClassesTestDataGeneratorLoader +Konclude.Test.TransitiveConnectedELClassesTestDataGenerator.OWLlinkOutputFile=./Tests/TransitiveConnectedELClassesTestData-c200.owl.xml +Konclude.Test.TransitiveConnectedELClassesTestDataGenerator.ClassesCount=200",
		//"-TransitiveConnectedELClassesTestDataGeneratorLoader +Konclude.Test.TransitiveConnectedELClassesTestDataGenerator.OWLlinkOutputFile=./Tests/TransitiveConnectedELClassesTestData-c300.owl.xml +Konclude.Test.TransitiveConnectedELClassesTestDataGenerator.ClassesCount=300",
		//"-TransitiveConnectedELClassesTestDataGeneratorLoader +Konclude.Test.TransitiveConnectedELClassesTestDataGenerator.OWLlinkOutputFile=./Tests/TransitiveConnectedELClassesTestData-c400.owl.xml +Konclude.Test.TransitiveConnectedELClassesTestDataGenerator.ClassesCount=400",
		//"-TransitiveConnectedELClassesTestDataGeneratorLoader +Konclude.Test.TransitiveConnectedELClassesTestDataGenerator.OWLlinkOutputFile=./Tests/TransitiveConnectedELClassesTestData-c500.owl.xml +Konclude.Test.TransitiveConnectedELClassesTestDataGenerator.ClassesCount=500",
		//"-TransitiveConnectedELClassesTestDataGeneratorLoader +Konclude.Test.TransitiveConnectedELClassesTestDataGenerator.OWLlinkOutputFile=./Tests/TransitiveConnectedELClassesTestData-c1000.owl.xml +Konclude.Test.TransitiveConnectedELClassesTestDataGenerator.ClassesCount=1000",
		//"-TransitiveConnectedELClassesTestDataGeneratorLoader +Konclude.Test.TransitiveConnectedELClassesTestDataGenerator.OWLlinkOutputFile=./Tests/TransitiveConnectedELClassesTestData-c10000.owl.xml +Konclude.Test.TransitiveConnectedELClassesTestDataGenerator.ClassesCount=10000",
		//"-TransitiveConnectedELClassesTestDataGeneratorLoader +Konclude.Test.TransitiveConnectedELClassesTestDataGenerator.OWLlinkOutputFile=./Tests/TransitiveConnectedELClassesTestData-c100000.owl.xml +Konclude.Test.TransitiveConnectedELClassesTestDataGenerator.ClassesCount=100000",
		//"-TransitiveConnectedELClassesTestDataGeneratorLoader +Konclude.Test.TransitiveConnectedELClassesTestDataGenerator.OWLlinkOutputFile=./Tests/TransitiveConnectedELClassesTestData-c1000000.owl.xml +Konclude.Test.TransitiveConnectedELClassesTestDataGenerator.ClassesCount=1000000",

		//"-TestOccurenceUnsatisfiableCacheLoader +Konclude.Analyse.CommonScale=TRUE +Konclude.Analyse.PlotDifference=TRUE +Konclude.Analyse.UpdateInterval=100",								
		//"-DefaultReasonerLoader += Konclude.Calculation.WorkerCount=1 +=Konclude.Calculation.Optimization.UnsatisfiableCacheRetrieval=false +=Konclude.Calculation.Optimization.SatisfiableCacheRetrieval=false +=Konclude.Calculation.Optimization.Backjumping=false +=Konclude.Calculation.Optimization.ProxyIndividuals=false +=Konclude.Calculation.Optimization.MinimizedMergingBranches=false +=Konclude.Calculation.Classification.PseudoModelSubsumptionMerging=false",
		"-ExperimentalReasonerLoader +=Konclude.Calculation.ProcessorCount=1 +=Konclude.Execution.CalculationManager=Konclude.Calculation.Calculator.ConcurrentTaskCalculationManager",
		//"-ExperimentalReasonerLoader += Konclude.Calculation.WorkerCount=1 +=Konclude.Execution.CalculationManager=Konclude.Calculation.Calculator.ConcurrentWorkCalculationManager",
		//"-ExperimentalReasonerLoader += Konclude.Calculation.WorkerCount=1 ",
		//"-DebugReasonerLoader += Konclude.Calculation.WorkerCount=1  +=Konclude.Calculation.Optimization.UnsatisfiableCacheRetrieval=true  +Konclude.Debug.PrecheckReasoning=true",
		//"-ExperimentalReasonerLoader += Konclude.Calculation.WorkerCount=1 +=Konclude.Calculation.Optimization.UnsatisfiableCacheRetrieval=false +=Konclude.Calculation.Optimization.Backjumping=false +=Konclude.Calculation.Optimization.ProxyIndividuals=false +=Konclude.Calculation.Optimization.MinimizedMergingBranches=false +=Konclude.Calculation.Optimization.Backtracking=true +=Konclude.Calculation.Optimization.UnsatisfiableCacheSingleLevelWriting=true +=Konclude.Execution.CalculationManager=Konclude.Calculation.Calculator.ConcurrentWorkCalculationManager",
		//"-AnalyseReasonerLoader += Konclude.Calculation.WorkerCount=1 ",
		//"-GUIDebugReasonerLoader += Konclude.Calculation.WorkerCount=1 ",
		//"-GUIDebugReasonerLoader +=Konclude.Calculation.WorkerCount=1 +=Konclude.Calculation.Optimization.UnsatisfiableCacheRetrieval=false +=Konclude.Calculation.Optimization.Backjumping=false +=Konclude.Calculation.Optimization.ProxyIndividuals=false +=Konclude.Calculation.Optimization.MinimizedMergingBranches=true +=Konclude.Calculation.Optimization.Backtracking=true +=Konclude.Calculation.Optimization.UnsatisfiableCacheSingleLevelWriting=true +=Konclude.Execution.CalculationManager=Konclude.Calculation.Calculator.ConcurrentWorkCalculationManager +=Konclude.Debug.PrecheckReasoning=true",
		//"-DebugReasonerLoader +=Konclude.Calculation.WorkerCount=1 +Konclude.Debug.CacheLineWriteViewer=TRUE +Konclude.Debug.VisualizeCalcBoxIndividuals=TRUE",
		//"-DebugReasonerLoader +=Konclude.Calculation.WorkerCount=1 +Konclude.Debug.CacheLineWriteViewer=TRUE +Konclude.Debug.VisualizeCalcBoxIndividuals=TRUE +Konclude.Debug.PrecheckReasoning=TRUE",
		//"-GUIAnalyseViewerLoader +Konclude.Analyse.WorkRepository.RunTime=TRUE +Konclude.Analyse.WorkRepository.WaitTime=TRUE  +Konclude.Analyse.WorkController.RunTime=TRUE +Konclude.Analyse.WorkController.WaitTime=TRUE +Konclude.Analyse.CommonScale=TRUE +Konclude.Analyse.PlotDifference=TRUE ", 
		//"-GUIAnalyseViewerLoader +Konclude.Analyse.WorkExecuter.RunTime=TRUE +Konclude.Analyse.WorkExecuter.WaitTime=TRUE +Konclude.Analyse.CommonScale=TRUE +Konclude.Analyse.PlotDifference=TRUE ",
		//"-GUIAnalyseViewerLoader +Konclude.Analyse.WorkRepository.WorkCount=TRUE +Konclude.Analyse.WorkController.WorkCount=TRUE", 
		//"-GUIAnalyseViewerLoader +Konclude.Analyse.WorkRegistry.RunTime=TRUE +Konclude.Analyse.WorkRegistry.WaitTime=TRUE +Konclude.Analyse.CommonScale=TRUE +Konclude.Analyse.PlotDifference=TRUE ", 
		//"-GUIAnalyseViewerLoader +Konclude.Analyse.WorkController.RunTime=TRUE +Konclude.Analyse.CommonScale=TRUE +Konclude.Analyse.PlotDifference=TRUE ", 
		//"-GUIAlgorithmMergingAnalyseViewerLoader +Konclude.Analyse.CommonScale=TRUE +Konclude.Analyse.PlotDifference=TRUE +Konclude.Analyse.UpdateInterval=100", 
		//"-GUIAlgorithmRuleAppliedAnalyseViewerLoader +Konclude.Analyse.CommonScale=TRUE +Konclude.Analyse.PlotDifference=FALSE +Konclude.Analyse.UpdateInterval=100", 
		//"-GUIAnalyseViewerLoader +Konclude.Analyse.WorkRegistry.ProcessedWorkCount=TRUE +Konclude.Analyse.PlotDifference=TRUE +Konclude.Analyse.UpdateInterval=1000", 
		//"-GUIAnalyseViewerLoader +Konclude.Analyse.ReasonerManager.QueryTotalWorkCount=TRUE +Konclude.Analyse.ReasonerManager.QueryCalcedWorkCount=TRUE +Konclude.Analyse.UpdateInterval=1000 +Konclude.Analyse.CommonScale=TRUE", 
		//"-GUIReasonerProgressAnalyseViewerLoader +Konclude.Analyse.UpdateInterval=1000", 
		//"-GUIClassificationProgressAnalyseViewerLoader +Konclude.Analyse.CommonScale=TRUE +Konclude.Analyse.UpdateInterval=100", 
		//"-GUIClassificationStatisticsAnalyseViewerLoader +Konclude.Analyse.CommonScale=TRUE +Konclude.Analyse.UpdateInterval=100", 

		//"-GUIAnalyseTextLoader +Konclude.Analyse.Classification.StatusText=TRUE +Konclude.Analyse.CommonScale=TRUE +Konclude.Analyse.UpdateInterval=100", 
		//"-GUIReserveScheduleTaskAnalyseViewerLoader +Konclude.Analyse.CommonScale=TRUE +Konclude.Analyse.PlotDifference=FALSE +Konclude.Analyse.UpdateInterval=20", 
		//"-GUIBlockedProcessorCountAnalyseViewerLoader +Konclude.Analyse.CommonScale=TRUE +Konclude.Analyse.PlotDifference=FALSE +Konclude.Analyse.UpdateInterval=20", 


		"-OWLlinkBatchFileLoader +=Konclude.OWLlink.CloseAfterProcessedRequest=false +=Konclude.OWLlink.BlockUntilProcessedRequest=true",
		//"+Konclude.OWLlink.RequestFile=./Configs/incremental-config.xml",
		"+Konclude.OWLlink.RequestFile=./Configs/testing-config.xml",
		//"+Konclude.OWLlink.RequestFile=./Configs/NoSaturation-config.xml",

		//"-OWLlinkHttpServerLoader",

		"-OWLlinkBatchFileLoader +=Konclude.OWLlink.CloseAfterProcessedRequest=false +=Konclude.OWLlink.BlockUntilProcessedRequest=true",

		//"+Konclude.OWLlink.RequestFile=./Tests/Test.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/action-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/relative-places-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Transportation-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/galen-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/OBO_REL-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/photography-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/tambis-full-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/periodic-table-complex-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/aeo-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/aero+Imports-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/influenza_ontology+Imports-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/pharmacogenomics+Imports-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/snpontology_full-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/CPR_vLast_modification-_July_31th,_2011_Chimezie_Ogbuji+Imports-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/DataImageQualityOntology_V032_vv32-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/FamilyHealthHistory_v1.0+Imports-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/fma_3.1-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/shr_vunknown-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/snpontology_full_v1.6_full-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/biological_process_xp_cell.imports+Imports-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/http%3A%2F%2Fpurllibrary.org%2Fobo%2Ftao%2Fdevelops_from_rules-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Homo_sapiens-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/aktive-portal-ontology-latest#-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/wine-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/cpo-inferred-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/stuff-bfo+Imports-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/BuildingsAndPlaces+Imports-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/human_phenotype_xp_all-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/tutorial-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/d0_v0.9+Imports-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/dmop-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/sadiobjects-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/cmmi_inst15-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/project-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/testdl0-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/animal-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/university-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/realraum+Imports-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/cfg-1.0-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/cows-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/list-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/nif-investigation-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/tmo-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/rifraf20070330-D-classify-request.xml",	
		//"+Konclude.OWLlink.RequestFile=./Tests/grandparent-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/grandparent-D-test-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/computerscience-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/University-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/event_sh_ontology+Imports-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/family-tree-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/DO_XP_ALL_SR-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/sadiobjects-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/people-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/mpeg7ontology-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/phensystem+Imports-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/SocialUnits-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/GRO-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/rob.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/roberts-RL-family-inlaw-problem-konclude.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/propmass+Imports-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/chemical-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/ir-axioms-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/dolce_all_RL-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/hierarchy_closure-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/goslim-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/MGEDOntology-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Obo_Tao-not_has_part-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Britto_2003-D.xml_converted_fullsig_xlink_noclass_nomat_konclude_1.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/FMA-constitutionalPartForNS.20Feb06-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/saturation-test-ontology-2.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Obo_Go-not_has_part-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/kidney-hierarchy-with-part-types-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/kidney-hierarchy-with-part-types-classify-request.xml",



		//"+Konclude.OWLlink.RequestFile=./Tests/datatype-test-ontology-1.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/datatype-test-ontology-2.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/datatype-test-ontology-5.xml",

		//"+Konclude.OWLlink.RequestFile=./Tests/Datatypes/BPO_11_12_10_v0.4-A-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Datatypes/bf88976b-8c5e-4505-ab4b-3a20774de20c_geofile-classify-request.xml",

		//"+Konclude.OWLlink.RequestFile=./Tests/fhkb-merged-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/thesaurus-alternativa.3037-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/82ef0565-1538-47e4-ab9c-a4e07b1d258a_tology-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/82ef0565-1538-47e4-ab9c-a4e07b1d258a_tology-sat1-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/36e2767b-1a78-461b-b58f-98b675450bf7_df-xml-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/diagnostic-ontology.3013-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/74a3fa65-ac34-40a2-9fe5-49629a5c865a_nt-0.4-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/74a3fa65-ac34-40a2-9fe5-49629a5c865a_nt-0.4-sat1-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/00002-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/00002-sat1-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/66d0a1c6-dc39-437e-90d6-63c91fe8beb6_pizza-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/71134246-639b-4ed8-aff8-2261cb41cd4e_tp3-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/e5c03a5b-05db-440a-ae23-4eadcaa1114f_oOntol-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/e5c03a5b-05db-440a-ae23-4eadcaa1114f_oOntol-sat1-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/51e460e9-619a-4683-b9ad-ec261e4bf06d_7-0342-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/c7ca2f60-e50d-46eb-98be-79eb4bcf785e_ontext-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/c7ca2f60-e50d-46eb-98be-79eb4bcf785e_ontext-sat1-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/51e0af4e-f343-45c6-bcc5-b08c88df1b01_en_ins-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/1408c0d7-ab38-415a-a9f3-931edd5b7f8d_genia-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/1408c0d7-ab38-415a-a9f3-931edd5b7f8d_genia-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/73db6ee2-4f1b-4b2a-907b-689bc1d607b8_nt-0.5-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/00c21813-4923-4e1a-9c0e-b6163accae5f_t8-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/00c21813-4923-4e1a-9c0e-b6163accae5f_t8-sat1-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/d26c8557-9b7d-4927-9caf-d6676414d513_WLqjun-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/60fb4605-bf17-45a4-9d3f-8e70a871c013_dation-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/645a3ff8-698d-4904-9cff-9bf134ff6a0c_tology-realize-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/e3b8ac30-036e-4068-ac21-eb39df58d2b5_onship-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/2a7d9049-d7d2-4f3f-90e4-bfb47e553b82_vsto-realize-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/00280-realize-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/41b4490a-1b8d-403b-84e3-0330f78b6e30_tology-realize-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/smartphone-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/smartphone-D-sat1-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/ecgontologyv0.1.3-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/hassupplylineont-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/79e668e8-baec-4337-b415-dacf846a5436_-small-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/30e621e4-e1c8-41b8-8a78-27d2aec5aae4_cedar-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/SNOMED-Jan11-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/universal.rdf-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/ONSTR_v0.3-A-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/ONSTR_v0.3-A-sat1-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/sanou.3090-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/ir-axioms-A-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/nemo-A-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/nemo-A-sat1-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/nikon+Imports-A-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/hgtappontoowl2dltest3-A-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/bco_vunknown+Imports-A-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/240a6af0-3a59-41b6-afe6-b00648b43315__inst1-inst1-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/bao_v1.6.2-A-sat1-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/bao_v1.6.2-A-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/toys-and-dolls-A-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/273536b1-3179-489a-a2e3-36a80ac9b1b7_y_v1.2.rdf-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/00567-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/00c21813-4923-4e1a-9c0e-b6163accae5f_t8.rdf-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/a26b6246-205a-427b-b90b-2d19ae6c0070_rument-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/3a391724-b8c0-43db-a504-5876d9a878fd_gy%2Fpo%2F-sat1-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/gina-A-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/645a3ff8-698d-4904-9cff-9bf134ff6a0c_tology-sat1-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/bco_vunknown+Imports-A-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/obi+Imports-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/event-inoh-pathway-ontology-.1011-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/e751bc98-f19d-4781-bb1f-72b9351a597b_algorithms.owl_functional-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/mw-test-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/owllink-riddle.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Conjunctions-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/obi+Imports-D-classify-request.xml",


		//"+Konclude.OWLlink.RequestFile=\"./Tests/1b SOME SHN request.xml\"",

		//"+Konclude.OWLlink.RequestFile=./Tests/1a-SHN-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/1a-SOME-SHN-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/1b-SHIN-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/1a-SOME-SHIN-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/1c-SHN-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/1a-SHQ-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/1c-SHQ-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/1b-SOME-SHN-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/1b-SHQ-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/1b-SHN-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/1b-SHIN-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/1b-SOME-SHIN-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/2a-SHN-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/2b-SHN-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/3b-ALCN-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/4-ALIF-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/5-SIF-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/6-SIF-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/8-ALCF-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/9-ALCHF-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/10a-ALCN-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/10a-ALCN-n10-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/10a-ALCN-n5-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/10c-ALCN-n18-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/10d-ALCIN-n200-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/10a-ALCON-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/10a+PHP5&6+44b-ALCN-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/10b-ALCN-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/10b-ALCON-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/15-SIF-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/23-ALC-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/28a-ALC-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/36-ALCF-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/36-hilf-ALCF-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/37-ALCHQ-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/38-ALCHQ-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/39-ALCF-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/39b-ALCF-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/39b-ALCN-request.xml",				
		//"+Konclude.OWLlink.RequestFile=./Tests/39d-ALCN-request.xml",				
		//"+Konclude.OWLlink.RequestFile=./Tests/40-ALCF-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/41_1-ALCQ-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/41_10-ALCQ-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/42-ALCIF-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/43-ALHN-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/44a-ALC-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/44b-ALC-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/45-SHI-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/46-SHI-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/48-ALCI-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/49-SHQ-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/50-SHIN-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/51-SIQ-request.xml",				?????
		//"+Konclude.OWLlink.RequestFile=./Tests/52a-SHR.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/53a-SRI.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/53b-SRI.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/53c-8-SRI.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/55-SIOQ-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/56-SIOQ-10-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/60-ALCOIF-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/73-n10-ALCOV-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/70-n10-ALCOV-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/70-n20-ALCOV-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/71-n5-ALCOV-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/71-n10-ALCOV-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/72-n10-ALCOV-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/QNR-1-4b-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/QNR-1-8b-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/QNR-2-4-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/QNR-2-8-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/QNR-2-8b-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/QNR-3-1-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/QNR-3-40-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/QNR-3-120-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/QNR-4-40-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/QNR-4-120-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/QNR-5-17-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/PHP-ALC-2-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/PHP-ALC-10-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/9Live.xml",

		//"+Konclude.OWLlink.RequestFile=./Tests/T98-sat/k_branch_n-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/T98-sat/k_branch_p-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/T98-sat/k_branch_n-n14-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/T98-sat/k_ph_p-n12-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/T98-sat/k_ph_n-n6-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/T98-sat/k_poly_p-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/T98-sat/k_t4p_n-n4-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/T98-sat/TestT98Testsuite-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/T98-sat/TestT98DifficultTestsuite-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/T98-sat/TestT98EasyTestsuite-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/T98-sat/k_path_p-n21-request.xml",

		//"+Konclude.OWLlink.RequestFile=./Tests/T98-sat/k_t4p_p-n21-extract-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/T98-sat/extracted-k_t4p_p-n21-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/T98-sat/extracted-k_ph_p-n6-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/T98-sat/extracted-k_ph_p-n2-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/T98-sat/extracted-k_path_p-n21-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/T98-sat/k_ph_p-n6-extract-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/T98-sat/k_path_p-n2-extract-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/T98-sat/k_ph_p-n2-extract-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/T98-sat/ExtractT98Testsuite-request.xml",

		//"+Konclude.OWLlink.RequestFile=./Tests/TestMergingTestsuite-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/10e-ALCIN-n18-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/TestXXXTestsuite-request.xml",

		//"+Konclude.OWLlink.RequestFile=./Tests/test-ontology-1.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/sat-succ-ext-test-ontology-1.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/sat-succ-ext-test-ontology-2.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/sat-succ-ext-test-ontology-3.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/galen-module1-no-functionality-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/galen-module1-no-inverse-D-classify-request.xml",

		//"+Konclude.OWLlink.RequestFile=./Tests/Rules/TransivityTestData-i1000-p200-consistency-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Rules/ODGI_v1.0_v1.0-D-consistency-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Rules/family-D-Data1000+Rules-consistency-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Rules/ODGI_v1.0_v1.0-D-D100-consistency-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/bcgo_mergedAll-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/TransitiveConnectedELClassesTestData-c10000-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/robert-requests.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/30-requests.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/LUBM-University1-ExamplePart-incremental-assertion-consistency-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/LUBM-University1-ExamplePart-incremental-assertion-classification-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/LUBM-University1-ExamplePart-incremental-assertion-realization-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/UOBM-1-comb-incremental-assertion-consistency-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/LUBM-1-comb-D-inc-ass-consistency-10~#109610-10~#178497-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/UOBM-1-comb-inc-ass-consistency-1302~#168020862-1302~#173377126-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/UOBM-1-comb-D-inc-ass-consistency-949~#87714000-949~#87403539-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/UOBM-1-comb-D-inc-ass-classify-949~#87714000-949~#87403539-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/UOBM-1-comb-D-inc-ass-realize-949~#87714000-949~#87403539-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/USDA_sr25_NF0_5-inc-ass-realization-6~#2734-6~#4734-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/COSMOtop1007-inc-ass-realization-251~#1614038-251~#1571890-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Oly-inc-ass-realization-179~#3659587-179~#3103997-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/fma_3.1-A-inc-ass-consistency-5669~#3171956650-5669~#3209657093-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/USDA_sr25_NF0_5-inc-ass-realization-98~#53487-98~#57403-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Coburn_Cavender_1992-A-inc-ass-realization-13162~#4336737116-13162~#4311226330-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/COSMOtop1007-inc-ass-realization-62~#395680-62~#403848-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/UOBM-1-comb-inc-ass-realization-2605~#334383349-2605~#339870486-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/UOBM-1-comb-inc-ass-realization-2605~#328427052-2605~#338660780-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/36c9acaf-e608-4a77-970d-9066424b1dbe_ne.owl.nooneof-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/a2c54ec3-99ff-41ae-b2b7-33f3a3572767_ESG1.1-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/cd09959f-0e9d-4932-bb83-43642c790c76_MK-instance1-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/710ea879-68b4-471b-8e6c-7181dd128ae1_ry04OWLDTP-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/6889341f-923d-4952-8f99-169cd957a495_RAMrdf-instance1-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/GetSameIndividualsTestRequest-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/OWLlink/all_request.xml",

		//"+Konclude.OWLlink.RequestFile=./Tests/test-request2.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/vincent.xml",

		//"+Konclude.OWLlink.RequestFile=./Tests/tambis-full-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/464e4880-a2c9-4b5c-a8e7-7acfe6a78f85_tambisfull-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/kb-owl-syntax-unraveled-depth-1-disjointness-constant-classes-relation-axioms-value-classes-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/IsEntailedTest-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/GetEntitiesTest-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/genomics-cds-light-demo-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Bio_KB101-D_SHOIQ-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/GO-2-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Three-SAT-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/biomodels-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/DO-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/HP-2-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/sio-dev-D_SRIQ-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/SCT-SEP-SH-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/UOBM-1-comb-D-Types01-request.xml", 
		//"+Konclude.OWLlink.RequestFile=./Tests/genomics-cds-demo-Types01-request.xml", 
		//"+Konclude.OWLlink.RequestFile=./Tests/inconsistent-test-ontology-2.xml", 
		//"+Konclude.OWLlink.RequestFile=./Tests/genomics-cds-light-demo-classify-realize-types-request.xml", 
		//"+Konclude.OWLlink.RequestFile=./Tests/UOBM/UOBM-1-comb-D-consistency-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/UOBM/UOBM-1-comb-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/UOBM/UOBM-1-comb-D+Rules-consistency-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/UOBM/UOBM-1-comb-D+Rules-a-n1-consistency-request.xml", 
		//"+Konclude.OWLlink.RequestFile=./Tests/UOBM/UOBM-1-comb-D+Rules-b-n1-consistency-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/UOBM/UOBM-1-comb-D+Rules-c-n1-consistency-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/UOBM/UOBM-1-comb-D+Rules-d-n1-consistency-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/UOBM/UOBM-1-comb-D+Rules-e-n1-consistency-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/UOBM/UOBM-1-comb-D+Rules-e-n1-consistency-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/UOBM/UOBM-1-comb-D-types01-request.xml", 
		//"+Konclude.OWLlink.RequestFile=./Tests/UOBM/UOBM-1-comb-D-instances01-request.xml", 
		//"+Konclude.OWLlink.RequestFile=./Tests/UOBM/UOBM-1-comb-D-classify-realize-request.xml", 
		//"+Konclude.OWLlink.RequestFile=./Tests/NSA/UOBM/UOBM-1-comb-D+Rule-R1-classify-request.xml", 
		//"+Konclude.OWLlink.RequestFile=./Tests/Rules/UOBM-1-comb-D+NS-N1-consistency-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/roberts-family-full-D-realize-request.xml", 
		//"+Konclude.OWLlink.RequestFile=./Tests/brokenPizza-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/atom-complex-proton-2-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Thesaurus-D2-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/TestTestsuite-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/TestTotalTestsuite-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/TestTestsuiteExampleFunctions-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/TestAtomicClassOverheadTestsuite-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/41_1000-ALCQ-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/test-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/28b-ALC-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/30-SHOIN-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/as-01-ALC.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/npt8-D-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/particle-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/chemistry-complex-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/npt8-D-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/flowers-b-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/flowers-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/R3-ALCR.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/R3b-ALCR.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/R4-ALCR.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/obi-SHOIN-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/obi-large-SHOIN-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/bio_process_plant-SHIF-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/bio_process_cell-SHIF-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/R3b-ALCR.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/ontology-request.xml", 
		//"+Konclude.OWLlink.RequestFile=./Tests/galenb-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/TestOnt-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/galen-ALEHIF+-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/roughtestind-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/mgi-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/biological_process_xp_uber_anatomy.imports-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/ontology1303134889031-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/hgtappontoowl2dltest3-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/christian-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/r3.rdf-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/attachment_3-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/UKTCEdition-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/family-tree-SROIQ-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/comp-graph-caching-test-2.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/galen-undoctored-ALEHIF+-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/fmaOwlDlComponent_1_4_0-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/galenc-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/full-galen-ALEHIF+-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/policyContainmentTest-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/biotop-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/regex-a-D-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/el-galen-ALEH+-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/EL-GALEN-n0-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/el-full-galen-ALEH+-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/LUBM-University1-fully-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/LUBM-University1-fully-PropertyTargets-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/UOBM-1-comb-D-KnownPossibleClassAssertions-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/biotop-SRI-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/quality_bfo_bridge-SHI-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/molecular_funct-ALEHIF+-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/biotop-SRI-assist-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/dco+biotop-D2-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/FMA-constitutionalPartFortNS-D-ALCOIF-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/dolce_all-D-SHOIN-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/biotoplitePizza-SROIQ-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/sat-exp-test-ontology-4.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/any-block-test-1.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/sig-blocking-test-ontology-2.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/cycle-cache-test-ontology-1.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/saturation-test-ontology-1.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/nominal-schema-test-ontology-2.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/nominal-schema-test-ontology-3.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/saturation-disjunction-test-ontology-1.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/atom-complex-proton-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/simpleDadOnt-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/52b-SHR-u.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/52c-SHR-u.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/52d-SHR-u.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/56-SIOQ-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Test-Union-01.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Test-Blocking-01.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Test-Nominal-01.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/TestLoadOnt-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Test-Tracking-01.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Test-Tracking-02.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Test-Tracking-03.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Test-Impl-01.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Test-Impl-02.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Test-El-01.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/Test-El-03.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/51u-1.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/particle-D-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/particle-D-b-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/equivalences-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/felix-ontology-paper-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/TestSupportedTestsuite-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/TestParallelTestsuite-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/test2-knownPossibleDeterministic-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/LUBM-University1-fully-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/LUBM-1-classify-request.xml",
		//"+Konclude.OWLlink.RequestFile=./Tests/query-entailment--analogous-propagation-blocking-test-ontology-1.xml",

		"+Konclude.OWLlink.RequestFile=./Tests/sparql#-D-classify-request.xml",


		"+Konclude.OWLlink.ResponseFile=./Tests/Test-response.xml",

		//"-ReasonerEvaluationLoader +=Konclude.Evaluation.ConfigFile=./Evaluation/Configs/config001.xml +=Konclude.Evaluation.Reasoner.ConfigFile=./Evaluation/Reasoners/HermiT-IR/v1.3.6/reasonerConfig.xml +=Konclude.Evaluation.OutputDirectory=Evaluation/Responses/R-001/",
		//"-ReasonerEvaluationLoader +=Konclude.Evaluation.ConfigFile=./Evaluation/Configs/config001.xml +=Konclude.Evaluation.Reasoner.ConfigFile=./Evaluation/Reasoners/Pellet/v2.3.0/reasonerConfig.xml",
		//"-ReasonerEvaluationLoader +=Konclude.Evaluation.ConfigFile=./Evaluation/Configs/config001.xml +=Konclude.Evaluation.Reasoner.ConfigFile=./Evaluation/Reasoners/FaCT++/v1.5.3/reasonerConfig.xml",
		//"-ReasonerEvaluationLoader +=Konclude.Evaluation.ConfigFile=./Evaluation/Configs/config001.xml +=Konclude.Evaluation.Reasoner.ConfigFile=./Evaluation/Reasoners/Konclude/v0.3.7-620/reasonerConfig.xml",
		//"-ReasonerEvaluationClassificationTimesComparerLoader",
		//"-ReasonerEvaluationLoadingTimesComparerLoader",


		//"-SPARQLBatchFileLoader +=Konclude.SPARQL.CloseAfterProcessedRequest=false +=Konclude.SPARQL.BlockUntilProcessedRequest=true",
		//"+Konclude.SPARQL.RequestFile=./Tests/LUBM-University1-fully-sparql-query1.sparql",
		//"+Konclude.SPARQL.RequestFile=./Tests/LUBM-University1-fully-sparql-query1b.sparql",
		//"+Konclude.SPARQL.RequestFile=./Tests/LUBM-University1-fully-sparql-query2.sparql",
		//"+Konclude.SPARQL.RequestFile=./Tests/LUBM-University1-fully-sparql-query7.sparql",
		//"+Konclude.SPARQL.RequestFile=./Tests/LUBM-University1-fully-sparql-query9.sparql",
		//"+Konclude.SPARQL.RequestFile=./Tests/LUBM-1-comp-sparql-query4.sparql",
		//"+Konclude.SPARQL.RequestFile=./Tests/LUBM-1-comp-sparql-query9.sparql",
		//"+Konclude.SPARQL.ResponseFile=./Tests/Test-response.xml"

	};

	int argcTest = 3;

	//char *argTest[] =  { "asdf", "-w", "AUTO", "-q", "./Tests/1b-SHN-request.xml" };
	//char *argTest2[] =  { "asdf", "-e", "-w", "4", "-c", "./Configs/testing-config.xml", "-q", "./Tests/PHP-ALC-9.xml" };
	//char *argTest2[] =  { "asdf", "classify -r Tests/Test-response.xml -q Tests/EL-GALEN.owl.xml" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/00a1118a-5420-46f0-b4b2-a2585165b28a_ePizza.owl.owl.fss", "-u" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/9Live-2nd.owl", "-u" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/approximated_014b7ef5-e7a8-44b1-b487-098954ed6c2e_go_xp_all.owl_functional.owl", "-u" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/approximated_a3aef80f-4383-4dfb-82a8-6cb2f611c29b_ersections.owl_functional.owl", "-u" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/17ef81df-3ac6-4e0e-a134-7428376eda5a_COSMO.owl_functional.owl", "-u" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/00768.owl_functional.owl", "-u" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/a085debd-ca56-457f-b403-b9938f6f43b1_core.owl_functional.owl", "-u" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/3755a349-2eff-40a3-8cec-59bd5cebcc00_facecustom.owl_functional.owl", "-u" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/9fa5de7c-50d3-4221-977b-77cf8f88e24c_DQontology3.owl_functional.owl", "-u" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/b9ca1e45-0568-48d3-b545-3aba9d7a258b_uzzleMiddle.n3_functional.owl", "-u" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/8947edf9-e8fe-4164-a643-a7beb1a2030b_Design.v35.owl_functional.owl", "-u" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/f547c4c5-1c09-45e2-adb1-e545222262a3_ndardmodel.owl_functional.owl", "-u" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/36c9acaf-e608-4a77-970d-9066424b1dbe_ne.owl.nooneof_functional.owl", "-u" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/0b37300d-a741-4bed-996b-0ee6df83ad07_lifecycle.owl_functional.owl", "-u" };
	//char *argTest2[] = { "asdf", "owllinkserver",  "-c", "./Configs/testing-config.xml", "-p", "8081", "-u", "+=Konclude.Logging.MinLoggingLevel=100 " };
	//char *argTest2[] = { "asdf", "owllinkserver",  "-c", "./Configs/testing-config.xml", "-p", "8081", "-u" };
	//char *argTest2[] =  { "asdf", "owllinkserver",  "-c", "./book-load-config.xml", "-p", "8080", "-u" };
	//char *argTest2[] =  { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/HWV039-1.p.Saffron---3.1.owl.xml", "-u", "-e" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-University1-fully.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-10-comb.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-University1-fully.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/UOBM-1-comb-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/HWV039-1.ow.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/classified_snomed_sdd_ontology.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/food.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/galen-ALEHIF+.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/wine+synABox1000_RL.owx", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/wine-mini-rl.owx", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/approximated_833e3c92-1514-4c79-ad64-e2308999b2e8_wine.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "cons",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/approximated_33bc7729-a6f1-4f7b-b867-00de0d2c899f_a_20110204.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/approximated_928cea39-e250-4901-ad4f-8933a6458141_reidslives.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "cons",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/approximated_c2a001e8-067a-4957-b247-7687f1861cbf_sudoku9i.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "cons",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/759602e5-68de-47e6-bdab-821b9d5f2b10_ryParser_hooks_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/approximated_shr.student-health-record-ontology.2.owl.xml_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "cons",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/approximated_01059cc1-33c1-45e9-a400-2bd097ea01fd_PPIO.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/approximated_3a13f394-00cb-489d-a0c8-882611cd4afb_BATutorial.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/approximated_00285.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/approximated_sse.surgical-secondary-events.2.owl.xml_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/fadeb4a7-67d7-4c18-9652-a9f362cf0f69_EbGdrcN5Y29ycA_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/approximated_29487853-ff19-47d1-87f4-e27cfe4fac15_codo2owl.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/approximated_fc843f79-a493-478a-b9e4-5a078e4737cd_EGFR1.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/roberts-RL-family-full-V.owx", "-u", "-e", "-w", "1", "+=Konclude.Calculation.Realization.RealizePrecomputation.RoleInstances=TRUE" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/approximated_6c53262b-c33f-424f-99bc-348bb995a77b_hReference.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "cons",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/91d671b7-7119-4100-ab45-12625a52e3ff_Rstarvs10.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/88a2b6d9-0932-4842-af12-c5010f5a1167_intervals_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "cons",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/00775.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "cons",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/feaf1577-ec44-4912-9caa-a57a720af930_cfobj.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/710ea879-68b4-471b-8e6c-7181dd128ae1_ry04OWLDTP.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/00938b31-d17b-4e14-9547-8f01e7e88780_extended.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/f2ceaeed-1718-4e88-a964-45c01ac582e4_gist.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/0f3e18c0-8c94-4b6b-8acd-dbfe1e294a2e_university1.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/a88846e2-4510-4d94-9e73-2f65cb562f20_StarTrek.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/kb-owl-syntax-unraveled-depth-3-km-relevant-cardinalities.ofn_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/travel-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/sitmon-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/approximated_36ba3ae1-5cfd-4236-8065-31a467167b24_Assessment.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/bams-from-swanson-98-4-5-07.from-web-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/d1798195-1f1c-4510-ac4c-53a6847ec79d_lom.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/kb-owl-syntax-unraveled-depth-4-cardinalities-disjointness-constant-classes-value-classes.ofn_functional.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/nif-investigation-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/gfo-basic-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/dmop-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/policyContainmentTest-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/biological_process_xp_uber_anatomy.imports-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/UOBM-1-comb-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/biomodels-21_SRIF.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/dolce_all.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/biocaster2010-14-mar-2010-700+Imports-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ontoeduoc_instanciada+Imports-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/DPC2.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/fhkb-merged.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/USDA_sr25_NF0_5.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/nif-investigation-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "cons",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/13b9fc1c-46bb-48c5-b2a0-328425d80d64_sfactoring.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/e33043eb-5bf2-4bb4-b811-b8527ad084d6_intact.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/kb-owl-syntax-unraveled-depth-4-cardinalities-disjointness-constant-classes-value-classes.ofn_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/c343f7b1-c70e-4dbd-bef7-0d3b9aaf6fd7_prsimple.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/a2c54ec3-99ff-41ae-b2b7-33f3a3572767_ESG1.1.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/00566.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/710ea879-68b4-471b-8e6c-7181dd128ae1_ry04OWLDTP.owl_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/8924d677-7a71-4600-82b3-22a3422d5a8d_foodCR.rdf_functional.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ALCHOID/Galen-Full-n1YE-D.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/7ddebe88-f2a2-4ec2-a0b3-a80167ae689d_NCITNCBO.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/17ef81df-3ac6-4e0e-a134-7428376eda5a_COSMO.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ALCHOID/Galen-Full-Heart-n2-D.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/43aa0e99-1169-4efd-84c7-dc3b1c1f9192_mergedtbox.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/FAT-3-UNSAT.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ftc-kb-full.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ore_ont_13954.owl", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "cons",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/PUZ011-1.p.Saffron---4.4.xml", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "owllinkserver",  "-c", "./Configs/testing-config.xml", "-p", "9090", "-q", "Tests/PUZ011-1.p.Saffron---4.4.xml", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-p", "9090", "-q", "Tests/model-2_2-A.owl.xml", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/model-2_2-A.owl.xml", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/sameIndividualOntology1.owl", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/roughtestind-A.owl.xml", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/testFunctionalAndNominal.owl", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/SynTest.owl", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/univ0-small4.owl", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "cons",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/0805c492-a3fa-41e6-b667-9968da4d9e3f_iridlcfds.owl.xml", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/OccupationOntology.v6-min3.owl.xml", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "file",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/OccupationRoleRealisationTestOntologyRequest.xml", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ore_ont_11196.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/enzyo-small-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/pharmacogenomics-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/FMA-constitutionalPartForNS_ALCOID.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/adolena-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/amino-acid-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/atom-complex-proton-2-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/aero-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/BuildingsAndPlaces-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/stuff-dolcelite+Imports-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/Rules/ExplicitVer-edrive-v0.9.6.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classifyroles",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/role-classification-test-01.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "owllinkfile",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/role-classification-test-01-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "owllinkfile",  "-c", "./Configs/testing-config2.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-University1-fully-PropertyTargets-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/edrive-data-v1.0.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classifyroles",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/roberts-RL-family-full-V3.owx", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classifyroles",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/roberts-family-full-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "file",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/smartphone-D-sat2-request.xml", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "file",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/smartphone-D-sat3-request.xml", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "file",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/smartphone-D-sat4-request.xml", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "file",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/smartphone-D-sat5-request.xml", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "file",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/particle-D-classify-request.xml", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/UOBM-1-comb-D.owl.xml", "-x", "http://www.Department13.University0.edu/AssociateProfessor11/Publication10", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "file",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/UOBM-1-comb-D-Types02-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "sparql",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-University1-fully.owl.xml", "-s", "Tests/LUBM-University1-fully-sparql-query9.sparql", "-v", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "sparqlserver",  "-c", "./Configs/querying-config.xml", "-i", "Tests/LUBM-University1-fully.owl.xml", "-p", "8081", "-u" };
	//char *argTest2[] = { "asdf", "sparqlserver",  "-c", "./Configs/querying-config.xml", "-i", "Tests/LUBM-University1-fully-schema.owl.xml", "-p", "8081", "-u" };
	//char *argTest2[] = { "asdf", "sparqlserver",  "-c", "./Configs/querying-persistence-config.xml", "-i", "Tests/LUBM-University1-fully-schema.owl.xml", "-p", "8081", "-u", "+=Konclude.Persistence.OntologyRevisionPersistencer=Konclude.Persistence.DirectoryOWL2XMLFilePersistencer" };
	//char *argTest2[] = { "asdf", "realisation",  "-c", "./Configs/testing-config.xml", "-i", "Tests/edrive-data-v1.0.owl", "-p", "8081", "-u" };
	//char *argTest2[] = { "asdf", "sparqlserver",  "-c", "./Configs/testing-config.xml", "-i", "Tests/roberts-RL-family-full-V.owx", "-p", "8081", "-u" };
	//char *argTest2[] = { "asdf", "file",  "-c", "./Configs/testing-config.xml", "-i", "Tests/roberts-RL-family-full-V-PropertyTargets-request.xml", "-o", "Tests/Test-response.xml", "-u" };
	//char *argTest2[] = { "asdf", "sparql",  "-c", "./Configs/testing-config.xml", "-i", "Tests/roberts-RL-family-full-V.owx", "-p", "8081", "-u", "-s", "Tests/isThirdCousinOf.sparql", "-o", "Tests/Test-response.xml", };
	//char *argTest2[] = { "asdf", "sparql",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-1-comb.owl.xml", "-s", "Tests/LUBM-1-comp-sparql-query9.sparql", "-v", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "sparql",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-1-comb.owl.xml", "-s", "Tests/LUBM-1-comp-sparql-query4.sparql", "-v", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "sparql",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-1-comb.owl.xml", "-s", "Tests/LUBM-1-comp-sparql-query7.sparql", "-v", "-e", "-w", "1", "-u" };
	//char *argTest2[] = { "asdf", "sparql",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-1-comb.owl.xml", "-s", "Tests/LUBM-1-comp-sparql-query4-sorted.sparql", "-v", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "sparql",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-1-comb.owl.xml", "-s", "Tests/LUBM-1-comp-sparql-query4-filtered.sparql", "-v", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "sparql",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-1-comb.owl.xml", "-s", "Tests/LUBM-1-comp-sparql-query8.sparql", "-v", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "sparql",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-1-comb.owl.xml", "-s", "Tests/LUBM-1-comp-sparql-query-all.sparql", "-v", "-e", "-w", "1", "-u" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/otsef-112-xml.owl", "-u", "-e", "-w", "1", "+=Konclude.Calculation.Realization.RealizePrecomputation.RoleInstances=TRUE" };
	//char *argTest2[] = { "asdf", "sparql",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-1-comb.owl.xml", "-s", "Tests/LUBM-1-comp-sparql-query-test-data-literal-1.sparql", "-v", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "sparql",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-1-comb.owl.xml", "-s", "Tests/LUBM-1-comp-sparql-query2-test-anonymous-3.sparql", "-v", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "sparql",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-1-comb.owl.xml", "-s", "Tests/LUBM-1-comp-sparql-query9-test-anonymous-1.sparql", "-v", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "sparql",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-1-comb.owl.xml", "-s", "Tests/LUBM-1-comp-sparql-query9-test-anonymous-4.sparql", "-v", "-e", "-w", "1" }; 
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/abstractOnto.tar/abstractOntology.funcsyntax.2018.01.24.23_42_14.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkserver", "-p", "8080", "-u" };
	//char *argTest2[] = { "asdf", "sparqlserver",  "-c", "./Configs/querying-config.xml", "-i", "Tests/LUBM-1-comb.owl.xml", "-p", "8080", "-u" };
	//char *argTest2[] = { "asdf", "sparql",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-1-comb.owl.xml", "-s", "Tests/LUBM-1-comp-sparql-query9-test-anonymous-4.sparql", "-v", "-e", "-w", "1" }; 
	//char *argTest2[] = { "asdf", "sparqlserver",  "-c", "./Configs/querying-config.xml", "-i", "Tests/query-entailment--analogous-propagation-blocking-test-ontology-1-simple-nominal-cycle.xml", "-p", "8080", "-u" };
	//char *argTest2[] = { "asdf", "sparqlserver",  "-c", "./Configs/querying-config.xml", "-i", "Tests/query-entailment--analogous-propagation-blocking-test-ontology-2-simple-nominal-cycle-and-branches.xml", "-p", "8080", "-u" };
	//char *argTest2[] = { "asdf", "sparqlserver",  "-c", "./Configs/querying-config.xml", "-i", "Tests/query-entailment--analogous-propagation-blocking-test-ontology-3-transivity.xml", "-p", "8080", "-u" };
	//char *argTest2[] = { "asdf", "sparqlserver",  "-c", "./Configs/querying-config.xml", "-i", "Tests/query-entailment--analogous-propagation-blocking-test-ontology-4-new-nominals-cylce-functionals.xml", "-p", "8080", "-u" };
	//char *argTest2[] = { "asdf", "sparqlserver",  "-c", "./Configs/querying-config.xml", "-i", "Tests/query-entailment--analogous-propagation-blocking-test-ontology-5-new-nominals-cylce-atmost.xml", "-p", "8080", "-u" };
	//char *argTest2[] = { "asdf", "sparqlserver",  "-c", "./Configs/querying-config.xml", "-i", "Tests/query-entailment--analogous-propagation-blocking-test-ontology-6-nominal-disjunction-5.xml", "-p", "8080", "-u" };
	//char *argTest2[] = { "asdf", "sparqlserver",  "-c", "./Configs/querying-config.xml", "-p", "8080", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "sparql",  "-c", "./Configs/querying-config.xml", "-r", "Tests/Test-response.xml", "-q", "a", "-s", "Tests/lubm-univ-bench-load-and-default-queries.sparql", "-v", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/family.swrl-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/UOBM-1-comb-D.owl.xml", "-u", "-e", "-w", "1" };

	//char *argTest2[] =  { "asdf", "owllinkfile",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/wine-D-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/univ-bench-dl.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/univ-bench-dl-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-1-comb-D-test-sat-crit-forall.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-1-comb-test-sat-crit-forall.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-1-comb-D.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-1-comb-D-test-disj-rep-cache-expansion-2.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/uniprot-sample-1.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/UOBM-1-comb-rep-cache-test-1.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/UOBM-1-comb-rep-cache-test-2.owl", "-u", "-e", "-w", "1" };
	////char *argTest2[] = { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/UOBM-1-comb-rep-cache-test-3.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/UOBM-1-comb-rep-cache-test-4.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/rep-cache-exp-test-03.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-1-comb.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-1-comb-same-possible-instances-test-03-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-1-comb-role-filler-test-01-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-1-comb-role-complex-filler-test-03-request.xml", "-u", "-e", "-w", "1" };

	//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/CA_provinces_N3.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "sparql",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-1-comb.owl", "-s", "Tests/LUBM-1-comp-sparql-query1b.sparql", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "sparql",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/uniprot-sample-1.owl", "-s", "Tests/uniprot-default-query-8.sparql", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/rep-cache-delay-exp-test-01.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "sparqlserver",  "-c", "./Configs/testing-config.xml", "-i", "Tests/uniprot-sample-1.owl", "-p", "8080", "-u" };
	//char *argTest2[] =  { "asdf", "cons",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/UOBM-1-comb-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "cons",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/UOBM-1-comb-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] =  { "asdf", "file",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/UOBM-1-comb-D-Instances-PeopleWithManyHobies-request.xml", "-u", "-e", "-w", "2" };
	//char *argTest2[] = { "asdf", "file",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/UOBM-1-comb-D-Instances-NonScienceStudent-request.xml", "-u", "-e", "-w", "2" };
	//char *argTest2[] = { "asdf", "sparql",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/UOBM-1-comb-D.owl.xml", "-s", "Tests/UOBM-1-comp-default-query-15.sparql", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "sparql",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/uniprot-sample-1.owl", "-s", "Tests/uniprot-default-query-4.sparql", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "sparql",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/UOBM-1-comb.owl", "-s", "Tests/UOBM-1-comp-test-query-1.sparql", "-u", "-e", "-w", "1" };

	//char *argTest2[] = { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/00751.owl.SROIQ.owl.noABox.owl.fss", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/UOBM-1-comb-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/hp2.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/edrive-hgo-101.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/edrive-data-v1.0.owl", "-u", "-e", "-w", "4" };
	//char *argTest2[] = { "asdf", "realize",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/edrive-hgo-101-wo-neg.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "realize",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ore_ont_6682.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "realize",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ore_ont_5834.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "realize",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ore_ont_6258.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "classify",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/dpo-full.owl", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "classify",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/biotop_vLast_modification_Apr_24,_2012_by_Stefan_Schulz-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "classify",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/Homo_sapiens-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "classify",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/kb-owl-syntax-coreference-IDs-km-relevant-cardinalities-relation-axioms.ofn.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "classify",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/686d7366-a085-4694-9bc8-f4426c3a5d90_Shares.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "classify",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/taxon_union_terms-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/rnao_vr113-D-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ore_ont_11647-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/LUBM-1-comb-anonymous-instances-test-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/grandparent-D-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/rnao_vr113-D-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/rnao-D-OntoJCrawl-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/taxon_union_terms-D-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/university-A-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ore_ont_20-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ore_ont_13052-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "classify",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/approximated_a3aef80f-4383-4dfb-82a8-6cb2f611c29b_ersections.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "classify",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/8597cc9d-4937-4653-8ec5-619edc771a04_nnotations.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "classify",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ore_ont_20.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ore_ont_3685-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ore_ont_7668-classify-request.xml", "-u", "-e", "-w", "1" };
//char *argTest2[] = { "asdf", "owllinkfile",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/6543b930-1de0-4e26-b221-a1e544223b06_zebra-classify-request.xml", "-u", "-e", "-w", "1" };
//char *argTest2[] = { "asdf", "owllinkfile",   "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ore_ont_9540-classify-request.xml", "-u", "-e", "-w", "1" };
//int argcTest2 = 13;


	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/ore_ont_10242--query-nr-6-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_10242.owl.xml", "-u" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_10242--query-nr-8-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_10242.owl.xml", "-u" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_10391--query-nr-3-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_10391.owl.xml", "-u" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_10230--query-nr-3-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_10230.owl.xml", "-u" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_10199--query-nr-2-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_10199.owl.xml", "-u" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_10058--query-nr-6-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_10058.owl.xml", "-u" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_10270--query-nr-8-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_10270.owl.xml", "-u" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_10270--query-nr-8-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_11378.owl.xml", "-u" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_10860--query-nr-8-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_10860.owl.xml", "-u" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_10951--query-nr-6-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_10951.owl.xml", "-u" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_10702--query-nr-1-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_10702.owl.xml", "-u" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_10702--query-nr-2c-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_10702.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_12270--query-nr-7-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_12270.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_12836--query-nr-10-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_12836.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_1270--query-nr-1-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_1270.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_12884--query-nr-10-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_12884.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_13404--query-nr-2-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_13404.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_13584--query-nr-3-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_13584.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_3726--query-nr-2-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_3726.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_5295--query-nr-5-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_5295.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_3255--query-nr-10-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_3255.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_3087--query-nr-2-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_3087.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_15596--query-nr-1-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_15596.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_2504--query-nr-4-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_2504.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_3156--query-nr-2-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_3156.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_16630--query-nr-4-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_16630.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_6727--query-nr-4b-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_6727.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_15413--query-nr-1-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_15413.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_8666--query-nr-7-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_8666.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/LUBM-1-comp-sparql-query9-limit.sparql", "-i", "Tests/LUBM-1-comb.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/edrive-alloc_elk-2.sparq", "-i", "Tests/edrive-hgo-101-naf-assertions-not-subclasses.owl.xml", "-u",  "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/alloc_mx-YXCL-wo-opt.sparql", "-i", "Tests/edrive-hgo-101-naf-assertions-not-subclasses.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/as-sample.sparql", "-i", "Tests/edrive-hgo-101-naf-assertions-not-subclasses.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",   "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_11523--query-nr-9-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_11523.owl.xml", "-u", "-r", "Tests/Test-response.xml", "+=Konclude.Debugging.WriteAnsweringVariableCompositionItems=TRUE" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_16315--query-nr-5-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_16315.owl.xml", "-u", "-r", "Tests/Test-response.xml"  };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ore_ont_7025.owl.xml", "-s", "Tests/ore_ont_7025--query-nr-3-real-cycle-with-concept-terms-SELECT.sparql", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_11296--query-nr-2-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_11296.owl.xml", "-u", "-r", "Tests/Test-response.xml"  };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_6855--query-nr-2-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_6855.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_15596--query-nr-3-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_15596.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_8666--query-nr-10-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_8666.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/edrive-alloc_elk-2-c.sparql", "-i", "Tests/edrive-hgo-101-naf-assertions-not-subclasses.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_1478--query-nr-1-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_1478.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_11378--query-nr-8-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_11378.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_10173--query-nr-8-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_10173.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_8975--query-nr-4-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_8975.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_16056--query-nr-5-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_16056.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_850--query-nr-6-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_850.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_13776--query-nr-7-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_13776.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_2453--query-nr-9-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_2453.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_7668--query-nr-8-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_7668.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_10568--query-nr-5-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_10568.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_15682--query-nr-9-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_15682.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_15753--query-nr-7-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_15753.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_5543--query-nr-10-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_5543.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_8951--query-nr-5-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_8951.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_9540--query-nr-2-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_9540.owl.xml", "-u", "-e", "-r", "Tests/Test-response.xml", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_2700--query-nr-9-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_2700.owl.xml", "-u", "-e", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_2700--query-nr-9-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_13704.owl.xml", "-u", "-e", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_13077--query-nr-4-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_13077.owl.xml", "-u", "-e", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_4572--query-nr-8-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_4572.owl.xml", "-u", "-e", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_2837--query-nr-7-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_2837.owl.xml", "-u", "-e", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_15596--query-nr-2-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_15596.owl.xml", "-u", "-e", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_3500--query-nr-10-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_3500.owl.xml", "-u", "-e", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_8480--query-nr-4-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_8480.owl.xml", "-u", "-e", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_9694--query-nr-10-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_9694.owl.xml", "-u", "-e", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_1579--query-nr-8-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_1579.owl.xml", "-u", "-e", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_3726--query-nr-4-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_3726.owl.xml", "-u", "-e", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_430--query-nr-3-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_430.owl.xml", "-u", "-e", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_16315--query-nr-8-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_16315.owl.xml", "-u", "-e", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_5834--query-nr-8-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_5834.owl.xml", "-u", "-e", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_7569--query-nr-7-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_7569.owl.xml", "-u", "-e", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/LUBM-1-comp-sparql-query9-union.sparql", "-i", "Tests/LUBM-1-comb.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/LUBM-1-comp-sparql-query9-optional-3.sparql", "-i", "Tests/LUBM-1-comb-c.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/LUBM-1-comp-sparql-query9.sparql", "-i", "Tests/LUBM-1-comb.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/edrive-alloc_elk.sparql", "-i", "Tests/edrive-hgo-101-naf-assertions-not-subclasses.owl.xml", "-u", "-e", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/alloc_mx-YXCF-wo-label.sparql", "-i", "Tests/edrive-hgo-101-naf-assertions-not-subclasses.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/edrive-alloc_elk-2.sparql", "-i", "Tests/edrive-hgo-101-naf-assertions-not-subclasses.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/alloc_mx-YXCF-count-test.sparql", "-i", "Tests/edrive-hgo-101-naf-assertions-not-subclasses.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_11647--query-nr-1-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_11647.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_3087--query-nr-5b-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_3087.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_1509--query-nr-9-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_1509.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_1509--query-nr-9-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/lubm400.ttl", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_3087--query-nr-5-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_3087.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_6934--query-nr-2-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_6934.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_8042--query-nr-7-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_8042.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_2318--query-nr-6-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_2318.owl.xml", "-u", "-r", "Tests/Test-response.xml", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/ore_ont_10702--query-nr-3-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_10702.owl.xml", "-u", "-r", "Tests/Test-response.xml", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/sequential-distinct-querying-config.xml", "-s", "Tests/ore_ont_14816--query-nr-3-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_14816.owl.xml", "-u", "-r", "Tests/Test-response.xml", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/ore_ont_14816--query-nr-3-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_14816.owl.xml", "-u", "-r", "Tests/Test-response.xml", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/sequential-distinct-querying-config.xml", "-s", "Tests/ore_ont_12925--query-nr-3-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_12925.owl.xml", "-u", "-r", "Tests/Test-response.xml", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/sequential-distinct-querying-config.xml", "-s", "Tests/ore_ont_399--query-nr-2-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_399.owl.xml", "-u", "-r", "Tests/Test-response.xml", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/sequential-distinct-querying-config.xml", "-s", "Tests/ore_ont_11696--query-nr-5-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_11696.owl.xml", "-u", "-r", "Tests/Test-response.xml", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/sequential-distinct-querying-config.xml", "-s", "Tests/ore_ont_13404--query-nr-1-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_13404.owl.xml", "-u", "-r", "Tests/Test-response.xml", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/sequential-distinct-querying-config.xml", "-s", "Tests/ore_ont_13805--query-nr-5-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_13805.owl.xml", "-u", "-r", "Tests/Test-response.xml", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/sequential-distinct-querying-config.xml", "-s", "Tests/ore_ont_14517--query-nr-8-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_14517.owl.xml", "-u", "-r", "Tests/Test-response.xml", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/sequential-distinct-querying-config.xml", "-s", "Tests/ore_ont_8050--query-nr-1-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_8050.owl.xml", "-u", "-r", "Tests/Test-response.xml", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/sequential-distinct-querying-config.xml", "-s", "Tests/ore_ont_10355--query-nr-7-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_10355.owl.xml", "-u", "-r", "Tests/Test-response.xml", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/sequential-distinct-querying-config.xml", "-s", "Tests/ore_ont_1509--query-nr-2-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_1509.owl.xml", "-u", "-r", "Tests/Test-response.xml", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//int argcTest2 = 13;
//

//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/ore_ont_5834--query-nr-8-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_5834.owl.xml", "-u", "-e", "-r", "Tests/Test-response.xml", "-SPARQLResultsComparisonLoader",   "+=Konclude.SPARQL.RequestFile=Tests/Test-response.xml" , "+=Konclude.SPARQL.ComparisonFile=F:/Data/Temp/Frodo/000-response.xml"  , "+=Konclude.SPARQL.ResponseFile=F:/Data/Temp/Frodo/diff-response.xml" };
//int argcTest2 = 16;



	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/uobm-univ-bench-load-and-anonym-query-9.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-s", "Tests/wine-tree-folded-with-concepts-entailment-query-23.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/lubm-univ-bench-load-and-query-anonymous-8.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/obi-entail-query-1.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/uobm-univ-bench-load-and-default-anonym-query-14.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/uobm-nom-select-query-6.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/uobm-univ-bench-load-and-default-query-4.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/uobm-univ-bench-load-and-default-query-15.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/uobm-nom-entail-query-1.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/uobm-nom-not-entailed-query-1.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/dmkb-tree-folded-with-concepts-entailment-query-10.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/uobm-univ-bench-load-and-default-query-14.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/geoskills-tree-folded-with-concepts-entailment-query-5.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/wine-tree-folded-with-concepts-entailment-query-5.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/family-tree-folded-with-concepts-entailment-query-19.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/dmkb-tree-folded-entailment-query-17.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/uniprot-load-and-default-query-4.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-s", "Tests/uobm-univ-bench-load-and-default-anonym-query-11.sparql", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml",  "-s", "Tests/family-real-cycle-entailment-query-11.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile", "-c", "./Configs/testing-config.xml", "-s", "Tests/family-real-cycle-with-concepts-entailment-query-1.sparql", "-u", "-r", "Tests/Test-response.xml", "-e" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/wine-tree-folded-with-concepts-entailment-query-14.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/lubm-univ-bench-load-and-default-query-4.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/uobm-univ-bench-load-and-default-query-6.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/reactome-load-and-query-7.sparql", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/sparql-load-and-query-test.sparql", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/lubm-univ-bench-sparql-load-and-complex-query-rasqal-test.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/lubm-univ-bench-load-and-connect-virtuoso-and-default-query-4.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/uobm-nom-entail-querkcy-3.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/uobm-nom-entail-query-4f.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/chembl-load-and-default-query-6.sparql", "-u", "-r", "Tests/Test-response.xml" };
//	char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/uobm-univ-bench-people-many-hobbies-incons-modified-load-and-default-query-15.sparql", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/chembl-property-query-60-xref.sparql", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml",  "-s", "Tests/uobm-class-query-055-Man.sparql", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml",  "-s", "Tests/uobm-class-query-106-Thing.sparql", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/uobm-nom-entail-query-3c.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 4"};
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/uniprot-load-and-default-query-4.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/reactome-query-2.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/anonymous-querying-config.xml", "-s", "Tests/reactome-query-1.sparql", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/reactome-query-1.sparql", "-u", "-r", "Tests/Test-response.xml", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/reactome-load-and-query-1.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 8" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/lubm-vlog-sample-0100-query-3.sparql", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/chembl-vlog-query-2b.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=false", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/uobm-univ-bench-load-and-default-query-15.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 8", "+=Konclude.Debugging.LogStorageStatistics=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/reactome-query-2.sparql", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/reactome-query-2.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/lubm-univ-bench-load-and-default-query-7.sparql", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/lubm-univ-bench-load-and-query-anonymous-15.sparql", "-u", "-r", "Tests/Test-response.xml", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/uobm-univ-bench-load-and-default-query-4.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 8", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/lubm-univ-bench-load-and-query-anonymous-20.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 8", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/fly-load-and-query-1.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/uobm-univ-bench-load-and-default-query-12.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/uobm-univ-bench-load-and-anonym-query-9.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/uobm-univ-bench-load-and-default-query-9.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/reactome-part-query-2.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/reactome-query-2.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 8", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/reactome-part-local-78-component-vlog-query-2.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/reactome-part-local-38-component-vlog-query-2.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/reactome-part-local-3-component-vlog-query-2.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/reactome-part-local-17-component-vlog-query-2.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/reactome-part-local-5-component-vlog-query-2.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/reactome-part-local-61-component-vlog-query-2.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 4", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/reactome-part-local-28-component-vlog-query-2.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/reactome-part-local-48-component-vlog-query-2.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/lubm-univ-bench-load-and-query-anonymous-22.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 8", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/reactome/1000000/local-reactome-part-1000000-3-component-query-2.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "-e", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=FALSE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/uobm-univ-bench-10-load-and-default-query-9.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/reactome/1000000/local-reactome-part-1000000-3-component-query-2.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/reactome/100000/reactome-part-57-component-query-2.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 6", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/uobm-univ-bench-load-and-default-query-9.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/reactome-part-1000000-3-load-and-query-2.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=FALSE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/chembl-load-and-default-query-3.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=FALSE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/uobm-univ-bench-50-load-and-default-query-11.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 8", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=FALSE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/reactome/100000/reactome-part-22-component-query-2.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/ore_ont_4429--query-nr-6-real-cycle-with-concept-terms-SELECT.sparql", "-i", "Tests/ore_ont_4429.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/chembl-load-and-default-query-3.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 8", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=FALSE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/chembl-vlog-query-2.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=FALSE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/uniprot-50-load-and-query-7.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 6", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=FALSE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/uniprot-part-5-4-red-query-1.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=FALSE" };
//char *argTest2[] = { "asdf", "cons",  "-c", "./Configs/testing-config.xml",  "-i", "Tests/uobm500.ttl", "-u", "-r", "Tests/Test-response.xml", "-e", "-w 1" };
//char *argTest2[] = { "asdf", "cons",  "-c", "./Configs/testing-config.xml",  "-i", "Tests/uobm100.ttl", "-u", "-r", "Tests/Test-response.xml", "-e", "-w 1" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/people-with-many-hobbies-uobm-sample-010-query-1b.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=TRUE", "+=Konclude.Cache.RepresentativeBackendCache.ThreadedUpdateSynchronization=TRUE", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Cache.RepresentativeBackendCache.RejectingUpdateIncompatibleIndividualAssociationsRatio=0.1", "+=Konclude.Cache.RepresentativeBackendCache.RejectingUpdateIncompatiblePropagationCuttedIndividualLinkedNeighbourRatio=0.00001", "+=Konclude.Cache.RepresentativeBackendCache.InterpretUnchangedLabelsAsCompatibleIndividualAssociationUpdates=TRUE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/people-with-many-hobbies-uobm-sample-050-query-1.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=FALSE", "+=Konclude.Cache.RepresentativeBackendCache.ThreadedUpdateSynchronization=FALSE", "+=Konclude.Cache.RepresentativeBackendCache.SlotUpdateWaitingIncreaseMaximumCount=1" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/uobm-univ-bench-load-and-default-query-15.sparql", "-u", "-e", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Debugging.LogStorageStatistics=TRUE" };
//char *argTest2[] =  { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-s", "Tests/lubm-univ-bench-load-and-default-queries.sparql", "-u", "-v", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/lubm-univ-bench-load-and-default-query-vlog-2.sparql", "-u", "-r", "Tests/Test-response.xml" };
char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/uobm-sample-1-default-anonym-batched-queries.sparql", "-u", "-w 4", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/lubm-sample-0800-default-additional-anonym-vlog-batched-queries.sparql", "-u", "-r", "Tests/Test-response.xml" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/chembl-sample-1-pagoda-vlog-batched-queries.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 6", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=FALSE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/LUBM-1-comp-sparql-load-and-sub-class-query1.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 6", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=FALSE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/lubm-univ-bench-sparql-load-and-class-query-01-test.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=FALSE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/lubm-univ-bench-sparql-load-and-class-query-04-test.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=FALSE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/lubm-univ-bench-sparql-load-and-property-query-04-test.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=FALSE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/lubm-univ-bench-sparql-load-and-individual-mixed-class-query-03-test.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=FALSE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-config.xml", "-s", "Tests/lubm-univ-bench-sparql-load-and-individual-mixed-class-query-06-test.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=FALSE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/lubm-univ-bench-sparql-load-and-individual-mixed-property-query-02-test.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=FALSE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/lubm-univ-bench-sparql-load-and-complex-concept-query-01-test.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=FALSE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/lubm-univ-bench-sparql-load-and-property-path-query-01-test.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics=TRUE", "+=Konclude.Debugging.WriteDebuggingRepresentativeCacheContent=FALSE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-persistence-config.xml", "-s", "Tests/lubm-univ-bench-sparql-incemental-load-and-query-03-test.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Persistence.OntologyRevisionPersistencer=Konclude.Persistence.RedlandExternalTriplesDSNOnlyWritingPersistencer", "+=Konclude.Persistence.RedlandExternalTriplesDSNOnlyWritingPersistencer.DirectFullTriplesLoading=FALSE" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-persistence-config.xml", "-s", "Tests/lubm-univ-bench-sparql-incemental-load-and-query-02-test.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Persistence.OntologyRevisionPersistencer=Konclude.Persistence.RedlandExternalTriplesSPARQLServicePersistencer", "+=Konclude.Persistence.RedlandExternalTriplesSPARQLServicePersistencer.ServiceAddress=http://127.0.0.1:8890/sparql" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-persistence-config.xml", "-s", "Tests/lubm-univ-bench-sparql-incemental-load-and-query-02-test.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Persistence.OntologyRevisionPersistencer=Konclude.Persistence.RedlandExternalTriplesSPARQLServicePersistencer", "+=Konclude.Persistence.RedlandExternalTriplesSPARQLServicePersistencer.ServiceAddress=http://127.0.0.1:8890/sparql" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-persistence-config.xml", "-s", "Tests/lubm-univ-bench-sparql-incemental-load-and-query-02-test.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Persistence.OntologyRevisionPersistencer=Konclude.Persistence.RedlandExternalTriplesSPARQLServiceLoadingHTTPTransactionWritingPersistencer" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/querying-persistence-config.xml", "-s", "Tests/lubm-univ-bench-sparql-incemental-load-and-query-02-test.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Persistence.OntologyRevisionPersistencer=Konclude.Persistence.MultiPersistencer" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/sequential-distinct-all-batch-querying-config.xml", "-s", "Tests/people-with-many-hobbies-uobm-sample-001-query-4.sparql", "-u", "-r", "Tests/Test-response.xml", "-w 1", "+=Konclude.Cache.RepresentativeBackendCache.ThreadedUpdateSynchronization=FALSE +=Konclude.SPARQL.Server.ThreadedSocketWriting=FALSE  +=Konclude.Calculation.Precomputation.TotalPrecomputor.IndividualsPrecompuationLimitIncreasingFactorPercent=100 +=Konclude.Calculation.Precomputation.TotalPrecomputor.IndividualsHandlingCountOntologySizeMaxIncreaseFactor=1 +=Konclude.Calculation.Precomputation.TotalPrecomputor.IndividualsPrecompuationSize=10 +=Konclude.Calculation.Optimization.BackendCriticalNeighbourExpansionLimit=100 +=Konclude.Calculation.Optimization.BackendCriticalNeighbourExpansionPriorityReductionCount=85 +=Konclude.Cache.RepresentativeBackendCache.RejectingUpdateIncompatiblePropagationCuttedIndividualLinkedNeighbourCount=1000 +=Konclude.Cache.RepresentativeBackendCache.InterpretUnchangedLabelsAsCompatibleIndividualAssociationUpdates=TRUE" };
int argcTest2 = 12;
	//char *argTest2[] = { "asdf", "sparqlserver",  "-c", "./Configs/testing-config.xml", "-p", "8080", "-u" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/chembl-vlog-query-2.sparql", "-u", "-w 1" };
//char *argTest2[] = { "asdf", "sparqlfile",  "-c", "./Configs/testing-config.xml", "-s", "Tests/uobm-univ-bench-10-load-and-default-query-4.sparql", "-u", "-w 8" };
//int argcTest2 = 8;


	//char *argTest2[] = { "asdf", "realize",  "-c", "./Configs/testing-config.xml",  "-i", "Tests/VO_merged.owl", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "realize",  "-c", "./Configs/testing-config.xml",  "-i", "Tests/ore_ont_2574.owl.xml", "-u", "-r", "Tests/Test-response.xml" };
	//char *argTest2[] = { "asdf", "realize",  "-c", "./Configs/testing-config.xml",  "-i", "Tests/ore_ont_16420.owl.xml", "-u", "-r", "Tests/Test-response.xml", "-e" };
	//char *argTest2[] = { "asdf", "realize",  "-c", "./Configs/testing-config.xml",  "-i", "Tests/ore_ont_9668.owl.xml", "-u", "-r", "Tests/Test-response.xml", "-e" };
	//char *argTest2[] = { "asdf", "realize",  "-c", "./Configs/testing-config.xml",  "-i", "Tests/galen-ALEHIF+.owl.xml", "-u", "-r", "Tests/Test-response.xml", "-e" };
	//char *argTest2[] = { "asdf", "realize",  "-c", "./Configs/testing-config.xml",  "-i", "Tests/ore_ont_3164.owl.xml", "-u", "-r", "Tests/Test-response.xml", "-e" };
	//int argcTest2 = 11;

	//char *argTest2[] = { "asdf", "owllinkfile",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/action-D-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/tutorial-D-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/aero-D-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/stuff-D-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/phen+Imports-D-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/wine-D-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/example6.1-1-D-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/attachment_3-D-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ir-axioms-D-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/fma_3.1-D-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",  "-c", "./Configs/evaluation-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/pgkb-hcls08+Imports-D-classify-request.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/query-extraction-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/obi-2015-02-28.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/UOBM-1-comb-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/query-extraction-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/fma_3.1-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/query-extraction-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/dmkb+Imports-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/query-extraction-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/family-full-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/query-extraction-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/nif_v2.9.1_-_January_23,_2012+Imports-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/query-extraction-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/GeoSkills-D.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/query-extraction-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/winecr.rdf+Imports-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/OS4-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/inconsistent+Imports-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/pasta_new_1-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/model-2_2-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/08083461-91eb-4eb6-bf8c-388cde2954ef_Table_saw.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/d78f9069-414d-4f6c-a048-c62f16c1983d_RANKL.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ore_ont_5206.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ore_ont_714.owl.xml", "-u", "-e", "-w", "1" };
//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ore_ont_15993.owl.xml", "-u", "-e", "-w", "1" };
//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/678d4a1c-2536-4af5-a511-279a0fae7662_e164.owl.xml", "-u", "-e", "-w", "1" };
//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/e0c9b069-4d23-440a-aa77-c258cae11ab5_photograph1.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/model-2_2+Imports-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/attachment-2_2+Imports-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/collections-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/example6.1-1_2+Imports-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/4b914d8c-db0d-458e-820b-40948ca12e12_connexor.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ore_ont_15635.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/figure-10+Imports-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "owllinkfile",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/figure-10+Imports-A-types.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "realize",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/simplemesh-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ta-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/jamesdean-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/4b914d8c-db0d-458e-820b-40948ca12e12_connexor.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/OWL2DL-5.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "classify",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/gazetteer_2-A.owl.xml", "-u", "-e", "-w", "1" };
	//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/testing-config.xml", "-r", "Tests/Test-response.xml", "-q", "Tests/ore_ont_4516.owl.xml", "-u", "-e", "-w", "1" };
	//int argcTest2 = 13;
	

//char *argTest2[] = { "asdf", "consistency",  "-c", "./Configs/query-extraction-config.xml", "-r", "Tests/Test-response.xml", "-q", "F:/Data/UUPR-Eval-GIT/Evaluation/Ontologies/O-000/WithoutModifications/ORE2015/ore_ont_10425.owl.xml", "-u", "-e", "-w", "1" };
//int argcTest2 = 13;


	//char *argTest2[] = { "asdf", "-CoutLogObserverLoader", "-SPARQLResultsComparisonLoader",   "+=Konclude.SPARQL.RequestFile=F:/Data/Temp/Frodo/000-response.xml" , "+=Konclude.SPARQL.ComparisonFile=F:/Data/Temp/Frodo/000-AQ-response.xml"  , "+=Konclude.SPARQL.ResponseFile=F:/Data/Temp/Frodo/diff-response.xml" };
	//int argcTest2 = 6;

	//

	//QSet<QString> indi1Set;
	//QFile file1("F:\\Data\\Temp\\Frodo\\000-response.xml");
	////QFile file1("F:\\Data\\Sync\\UUPR-derivo-GIT\\Konclude\\Tests\\Test-response-1.xml");
	//if (file1.open(QIODevice::ReadOnly)) {
	//	while (!file1.atEnd()) {
	//		QString line = file1.readLine().trimmed();
	//		if (line.startsWith("<uri>")) {
	//			QString indiName = line.mid(5, line.length() - 11);
	//			indi1Set.insert(indiName);
	//		}
	//		if (line.startsWith("<binding name=\"x\"><uri>")) {
	//			QString indiName = line.mid(23, line.length() - 16 - 23);
	//			indi1Set.insert(indiName);
	//		}

	//	}
	//}

	//QSet<QString> indi2Set;
	////QFile file2("F:\\Data\\Temp\\Frodo\\uobm-man-k-000-response.xml");
	//QFile file2("F:\\Data\\Sync\\UUPR-derivo-GIT\\Konclude\\Tests\\Test-response.xml");
	//cint64 count2 = 0;
	//if (file2.open(QIODevice::ReadOnly)) {
	//	while (!file2.atEnd()) {
	//		QString line = file2.readLine().trimmed();
	//		if (line.startsWith("<uri>")) {
	//			QString indiName = line.mid(5, line.length() - 11);
	//			indi2Set.insert(indiName);
	//			count2++;
	//		}
	//		if (line.startsWith("<binding name=\"x\"><uri>")) {
	//			QString indiName = line.mid(23, line.length() - 16 - 23);
	//			if (indi2Set.contains(indiName)) {
	//				bool debug = true;
	//			}
	//			indi2Set.insert(indiName);
	//			count2++;
	//		}
	//	}
	//}

	//for (QString indi2 : indi2Set) {
	//	if (!indi1Set.contains(indi2)) {
	//		bool debug = true;
	//	}
	//}

	//for (QString indi1 : indi1Set) {
	//	if (!indi2Set.contains(indi1)) {
	//		bool debug = true;
	//	}
	//}

	//file1.close();
	//file2.close();



	//int argcTest2 = 14;
	//int argcTest2 = 12;
	//int argcTest2 = 6;
	//int argcTest2 = 12;

	//int argcTest2 = 10;

    char **argTrans = argv;
	int argcTrans = argc;

#ifndef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED

	//if (argc <= 1) {
	//	argTrans = argTest;
	//	argcTrans = argcTest;
	//}


	if (argc <= 1) {
		argTrans = argTest2;
		argcTrans = argcTest2;
	}

#endif // !KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED



	//double x1 = std::numeric_limits<double>::quiet_NaN();
	//double x2 = QString("nan").toDouble();
	//if (x1 == x2) {
	//	bool bug = true;
	//}
	//if (x1 < x2) {
	//	bool bug = true;
	//}
	//if (x1 > x2) {
	//	bool bug = true;
	//}



	QCoreApplication a(argcTrans, argTrans);

	CLogger *logger = CLogger::getInstance();



	try {

		//QDir::setCurrent("F:/Data/UUPR-Eval-GIT");

		QString koncludeString(QString("%1 - %2").arg(CKoncludeInfo::getKoncludeName()).arg(CKoncludeInfo::getKoncludeNameExtension()));

		LOG(INFO,"::Konclude::Main","Starting Konclude ... ",0);
		LOG(INFO,"::Konclude::Main",koncludeString,0);
		LOG(INFO,"::Konclude::Main",QString("%1, %2, Version %3 (%4)\r\n").arg(CKoncludeInfo::getKoncludeDescription()).arg(CKoncludeInfo::getKoncludeBitPlatformString()).arg(CKoncludeInfo::getKoncludeVersionString()).arg(CKoncludeInfo::getKoncludeCompilationDateString()),0);

		CLoaderFactory *loaderFactory = new CDefaultLoaderFactory();

		QStringList arguments = CCommandLineLoader::getEncodedArguments(argcTrans,argTrans,true,false);

		CCommandLinePreparationTranslatorSelector transSelector(loaderFactory);
		arguments = transSelector.translateArguments(arguments);

		CCommandLineLoader *cmdLineLoader = new CCommandLineLoader(arguments,false);
		cmdLineLoader->init(loaderFactory);

		cmdLineLoader->load();
		a.exec();
		cmdLineLoader->exit();
		delete cmdLineLoader;
		delete loaderFactory;

		

		LOG(INFO,"::Konclude::Main",logTr("Stopping Konclude ... ."),0);

	} catch (...) {
		LOG(CATASTROPHIC,"::Konclude::Main",logTr("Stop because of an unknown catastrophic error."),0);
	}

	
	logger->shutdownLogger();






	QSet<QString> indi1Set;
	//QFile file1("response.xml");
	QFile file1("F:\\Data\\Temp\\Frodo\\000-response.xml");
	//QFile file1("F:\\Data\\Temp\\Frodo\\conc-vs-seq\\000-response.xml");
	//QFile file1("F:\\Data\\Sync\\UUPR-derivo-GIT\\Konclude\\Tests\\Test-response-1.xml");
	if (file1.open(QIODevice::ReadOnly)) {
		QString bindings;
		cint64 count = 0;
		while (!file1.atEnd()) {
			QString line = file1.readLine().trimmed();
			bool x = true;
			if (line.startsWith("<uri>")) {
				QString indiName = line.mid(5, line.length() - 11);
				if (count > 0) {
					bindings += "\t";
				}
				bindings += indiName;
				++count;
				if (count >= 2) {
					indi1Set.insert(bindings);
					bindings = QString();
					count = 0;
				}
			}
			//if (line.startsWith("<binding name=\"x\"><uri>")) {
			//	QString indiName = line.mid(23, line.length() - 16 - 23);
			//	indi1Set.insert(indiName);
			//}

		}
	}

	QSet<QString> indi2Set;
	//QFile file2("F:\\Data\\Temp\\Frodo\\uobm-man-k-000-response.xml");
	QFile file2("F:\\Data\\Sync\\UUPR-derivo-GIT\\Konclude\\Tests\\Test-response.xml");
	//QFile file2("expected.xml");
	cint64 resultCount = 0;
	if (file2.open(QIODevice::ReadOnly)) {
		QString bindings;
		cint64 count = 0;
		while (!file2.atEnd()) {
			QString line = file2.readLine().trimmed();

			if (line.startsWith("<result>")) {
				resultCount++;
			}

			if (line.startsWith("<binding name=\"x\"><uri>") || line.startsWith("<binding name=\"Y\"><uri>") || line.startsWith("<binding name=\"X\"><uri>") || line.startsWith("<binding name=\"Z\"><uri>")) {
				QString indiName = line.mid(23, line.length() - 16 - 23);
				if (count > 0) {
					bindings += "\t";
				}
				bindings += indiName;
				++count;
				if (count >= 2) {
					indi2Set.insert(bindings);
					bindings = QString();
					count = 0;
				}

			}
		}
	}

	if (resultCount != 156) {
		bool bug = true;
	}

	bool unexpectedResult = false;
	for (QString indi2 : indi2Set) {
		if (!indi1Set.contains(indi2)) {
			QByteArray data = indi2.toUtf8();
			cout << "additional answer: " << data.constData() << endl;
			unexpectedResult = true;
		}
	}

	for (QString indi1 : indi1Set) {
		if (!indi2Set.contains(indi1)) {
			QByteArray data = indi1.toUtf8();
			cout << "missing answer: " << data.constData() << endl;
			unexpectedResult = true;
		}
	}

	file1.close();
	file2.close();

	if (unexpectedResult) {
		return 1;
	}

	return 0;
}
