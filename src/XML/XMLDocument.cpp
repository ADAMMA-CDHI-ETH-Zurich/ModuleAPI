/***************************************************************************
* Copyright (C) 2023 ETH Zurich
* Core AI & Digital Biomarker, Acoustic and Inflammatory Biomarkers (ADAMMA)
* Centre for Digital Health Interventions
* 
* Authors: Patrick Langer
* 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*         http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
***************************************************************************/
#include "XML/XMLDocument.hpp"
#include <fstream>

namespace claid
{
	XMLDocument::XMLDocument()
	{

	}

	XMLDocument::XMLDocument(std::shared_ptr<XMLNode> node) : xmlNode(node)
	{

	}

	bool XMLDocument::loadFromFile(const File& file)
	{
		XMLParser xmlParser;

		return (xmlParser.parseFromString(file.getContent(), this->xmlNode));
	}

	bool XMLDocument::loadFromFile(const std::string& filePath)
	{
		std::ifstream file(filePath);
		std::string fileContent;

		if (!file.is_open())
		{
			Logger::printfln("Error! Could not open file \"%s\".", filePath.c_str());
			return false;
		}

		file.seekg(0, std::ios::end);
		fileContent.reserve(file.tellg());
		file.seekg(0, std::ios::beg);

		fileContent.assign((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());

		XMLParser xmlParser(filePath);

		return (xmlParser.parseFromString(fileContent, this->xmlNode, filePath));
		
	}

	bool XMLDocument::saveToFile(const std::string& filePath)
	{
		std::string data;
		if (this->xmlNode == nullptr)
		{
			return false;
		}

		this->xmlNode->toString(data);

		std::ofstream file(filePath);

		if (!file.is_open())
		{
			Logger::printfln("Error! Could not open file \"%s\".", filePath.c_str());
			return false;
		}

		file.write(data.c_str(), data.length());

		return true;

	}

	bool XMLDocument::loadFromString(const std::string& string)
	{
		XMLParser xmlParser;

		return (xmlParser.parseFromString(string, this->xmlNode));
	}

	void XMLDocument::setXMLNode(std::shared_ptr<XMLNode> xmlNode)
	{
		this->xmlNode = xmlNode;
	}

	std::shared_ptr<XMLNode> XMLDocument::getXMLNode()
	{
		return this->xmlNode;
	}
}


