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

#include "Exception/Exception.hpp"
#include "ExceptionHandler/ExceptionHandler.hpp"

namespace claid {


const char* Exception::what() const noexcept
{
	const std::size_t INDENT = 4;
	const char* INDENT_STR = "    ";

	mMessage = "";

	std::size_t maxMessageSize = 0;
	for(std::list<Info>::const_iterator i = mInfos.begin();
		i!=mInfos.end(); ++i)
	{
		std::size_t msgSize = i->message.size();

		if(i!=mInfos.begin())
			msgSize+=INDENT;

		if(msgSize > maxMessageSize)
			maxMessageSize = msgSize;
	}

	for(std::list<Info>::const_iterator i = mInfos.begin();
		i!=mInfos.end(); ++i)
	{
		if(i!=mInfos.begin()) {
			mMessage += std::string("\n") + INDENT_STR;
			mMessage += i->what(maxMessageSize-INDENT);
		} else
			mMessage += i->what(maxMessageSize);

	}
	
	return mMessage.c_str();
}

std::string Exception::message() const noexcept
{
	std::string s;
	for(std::list<Info>::const_iterator i = mInfos.begin(); i!=mInfos.end(); ++i)
	{
		if(i!=mInfos.begin())
			s += ", " + i->message;
		else
			s = i->message;
	}

	return s;
}


std::string Exception::Info::what(std::size_t messageWidth) const
{
	std::string s = message;

	for(std::size_t i=message.size(); i<messageWidth; ++i)
		s += " ";

	if(!file.empty())
    {
        std::stringstream ss;
		ss << " (" << file << ":" << line << ")";
        s += ss.str();
    }
	return s;
}


}
