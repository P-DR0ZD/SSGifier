#include<iostream>
#include <string>
#include<direct.h>
#include <filesystem>
#include "ArgumentQueue.h"
#include"Utils.h"
bool SDDS::ArgumentQueue::addArg(const char* switchStr,const char* arg) {
	bool rc;
	Argument* tmpArg = new Argument();
	rc = tmpArg->parseArg(switchStr,arg);
	mArgs[mArgCnt++] = tmpArg;
	return rc;
}
bool SDDS::ArgumentQueue::execute() {
	bool rc = false;
	bool showVersion = false, showHelp = false, emptyArg = false, unkownArg = false;
	std::string unkownSwitchStr;
	std::string emptySwtichStr;
	std::string inputFile;
	std::string outputFileDir;
	std::string styleSheet;
	if (mArgCnt == 0) {
		mMessage = new char[] { "Must at least specify an input file and output file."};
	}
	else {
		for (size_t i = 0; i < mArgCnt; i++) {
			switch (mArgs[i]->getArgTag())
			{
			case Argument::ArgTag::INPUT:
				inputFile = mArgs[i]->mArg;
				break;
			case Argument::ArgTag::OUTPUT:
				outputFileDir = mArgs[i]->mArg;
				break;
			case Argument::ArgTag::VERSION:
				showVersion = !showVersion;
				break;
			case Argument::ArgTag::EMPTY:
				emptyArg = !emptyArg;
				emptySwtichStr = mArgs[i]->mSwitchStr;
				break;
			case Argument::ArgTag::UNKOWN:
				unkownArg = !unkownArg;
				unkownSwitchStr = mArgs[i]->mSwitchStr;
				break;
			case Argument::ArgTag::HELP:
				showHelp = true;
				break;
			case Argument::ArgTag::CSS:
				styleSheet = mArgs[i]->mArg;
				break;
			}
		}
		if (unkownArg)
			std::cout << "Unkown switch " << "\"" << unkownSwitchStr << "\"." << std::endl << "Use --help or -h for help." << std::endl;
		else if (showHelp)
			std::cout << "Please " << std::endl;
		else if (showVersion)
			std::cout << "SSGifier V0.1 by Tong Liu" << std::endl;
		else if (emptyArg)
			std::cout << "Empty argument for " << '"' << emptySwtichStr << '"' << " is provided." << std::endl;
		else if (inputFile.empty())
			std::cout << "Input file is necessary to be provided." << std::endl;
		else {
			if (outputFileDir.empty()) {
				std::cout << "Default directory \"dist\" will be used as the output directory." << std::endl;
				outputFileDir = "dist/";
			}
			if (SDDS::checkIfItsDirectory(inputFile)) { //if the input arg is a directory
				for (const auto& entry : std::filesystem::directory_iterator(inputFile)) {
					convertFile(entry.path().string(), outputFileDir,styleSheet);
				}
			}
			else
				convertFile(inputFile, outputFileDir,styleSheet);
		}
	}
	return rc;
}
SDDS::ArgumentQueue::~ArgumentQueue() {
	for (size_t i = 0; i < mArgCnt; i++) {
		delete mArgs[i];
	}
}
bool SDDS::Argument::parseArg(const char* switchStr,const char* arg) {
	if (mArg != nullptr) 
		delete mArg;
	if (arg != nullptr) {
		mArg = new char[std::strlen(arg)];
		std::strcpy(mArg, arg);
	}
	mSwitchStr = new char[std::strlen(switchStr)];
	strcpy(mSwitchStr, switchStr);

	if (std::strcmp(switchStr, "--help") == 0 || std::strcmp(switchStr, "-h") == 0) {
		mArgTag = ArgTag::HELP;
	}
	else if (std::strcmp(switchStr, "--input") == 0 || std::strcmp(switchStr,"-i") == 0){
			mArgTag = ArgTag::INPUT;
	}
	else if (std::strcmp(switchStr, "--output" ) == 0 || std::strcmp(switchStr,"-o") == 0) {
			mArgTag = ArgTag::OUTPUT;

	}
	else if (std::strcmp(switchStr, "--version") == 0 || std::strcmp(switchStr, "-v") == 0) {
		mArgTag = ArgTag::VERSION;
	}
	else if (std::strcmp(switchStr, "--stylesheet") == 0 || std::strcmp(switchStr, "-s") == 0)
		mArgTag = ArgTag::CSS;
		else
	{
		mArgTag = ArgTag::UNKOWN;
		return false;
	}
	// check if argument is necessary for the current switch
	if (mArgTag != ArgTag::VERSION && mArgTag != ArgTag::HELP && mArgTag != ArgTag::UNKOWN ) {
		if (arg == nullptr) {
			mArgTag = ArgTag::EMPTY;
			return false;
		}
	}
		return true;
	
}

SDDS::Argument::ArgTag SDDS::Argument::getArgTag() {
	return mArgTag;
}
SDDS::Argument::~Argument() {
	//if (mArg != nullptr)
		//delete[] mArg;
}

void SDDS::ArgumentQueue::convertFile(std::string inputFile, std::string outputFileDir, std::string stylesheet)
{

	std::string fileContent = SDDS::readFileAsHtmlStr(inputFile).c_str();
	std::filesystem::create_directory(outputFileDir);
	if (fileContent.empty()) {
		std::cout << "Cannot find file: " << inputFile << std::endl;
	}
	else {
		std::string fileName = SDDS::extractFileName(inputFile);
		fileName = fileName.substr(0, fileName.find('.'));
		const std::string HTMLContentStr = SDDS::generateHTMLAsStr(fileName, fileContent, stylesheet);
		if (outputFileDir.substr(0, 2) == "./" || outputFileDir.substr(0, 2) == ".\\") //remove the ./ or .\ at the begining of output file directory.
			outputFileDir = outputFileDir.substr(outputFileDir.find(".") + 1, outputFileDir.length());
		if (outputFileDir.find("\\") == std::string::npos && outputFileDir.find("/") == std::string::npos) //Append output path with "/" if the output arg is not postfixed with /
			outputFileDir.append("/");
		std::string filePath = "./" + outputFileDir + fileName + ".html";
		if (SDDS::writeHTMLToFile(filePath, HTMLContentStr))
			std::cout << filePath << " is successfully converted and written." << std::endl;
		else
			std::cout << "Could not write " << filePath << std::endl;
	}
}
