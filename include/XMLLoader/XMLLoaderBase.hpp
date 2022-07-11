#pragma once

#include "XML/XMLNode.hpp"
#include <string>

namespace portaible
{
	namespace XMLLoader
	{
		class XMLLoaderBase
		{
		private:
			std::string desiredTag;


		public:
			XMLLoaderBase(std::string desiredTag) : desiredTag(desiredTag)
			{

			}

			virtual bool execute(std::vector<std::shared_ptr<XMLNode>> xmlNodes) = 0;

			const std::string& getDesiredTag()
			{
				return this->desiredTag;
			}
			
		};

	}
}