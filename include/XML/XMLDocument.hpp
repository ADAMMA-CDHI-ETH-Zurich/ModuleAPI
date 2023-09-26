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
#pragma once

#include "XML/XMLParser.hpp"
#include "XMLNode.hpp"
#include "File/File.hpp"
#include "XML/XMLSerializer.hpp"
namespace claid
{
	class XMLDocument
	{
		private:
			std::shared_ptr<XMLNode> xmlNode;

		public:

			XMLDocument();
			XMLDocument(std::shared_ptr<XMLNode> node);

			bool loadFromFile(const File& file);
			bool loadFromFile(const std::string& file);
			bool saveToFile(const std::string& file);

			// Can be used to load an XML from a raw string.
			bool loadFromString(const std::string& string);

			void setXMLNode(std::shared_ptr<XMLNode> xmlNode);

			std::shared_ptr<XMLNode> getXMLNode();

			template<typename T>
			bool serializeObjectToFile(const std::string& path, T& object)
			{
				XMLSerializer serializer;
				serializer.serialize(object);
				this->xmlNode = serializer.getXMLNode();
				return this->saveToFile(path);
			}

	};
}