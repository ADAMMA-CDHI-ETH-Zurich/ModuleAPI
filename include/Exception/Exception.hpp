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
#include <assert.h>

#include <string.h>

#include <exception>
#include <sstream>
#include <iostream>

#include <list>
#include <chrono>
#include <thread>
#include "Logger/Logger.hpp"
#include "ExceptionHandler/ExceptionHandler.hpp"
#define CLAID_THROW(ex, msg)\
{\
	std::ostringstream ex_str;\
	ex_str << msg;\
    claid::Logger::printfln("%s", ex_str.str().c_str());\
	claid::Logger::printfln("\n");\
	std::this_thread::sleep_for(std::chrono::milliseconds(500));\
	if(claid::ExceptionHandler::isExeceptionHandlerRegistered())\
	{\
		claid::ExceptionHandler::invokeExceptionHandler(ex_str.str(), __FILE__, __LINE__);\
		std::this_thread::sleep_for(std::chrono::milliseconds(15000));\
	}\
	throw ex(ex_str.str(), __FILE__, __LINE__);\
}
// std::this_thread::sleep_for(std::chrono::milliseconds(1000));

namespace claid
{
	class Exception : public std::exception
	{

	protected:
		Exception()  noexcept {}

	public:


		Exception(const std::string& message, const char* file=NULL, int line=0) 
		{
			addInfo(message, file, line);
		}

		virtual ~Exception() noexcept {}

		void addInfo(const std::string& message, const char* file=NULL, int line=0)
		{
			mInfos.push_back(Info(message, file ? std::string(file) : "", line));
		}

		virtual const char* what() const noexcept;


		std::string message() const noexcept;



	public:




		

	public:


		struct Info {
			std::string message;	
			std::string file;		
			int         line;		

		public:
			Info(const std::string& iMessage, const std::string& iFile, int iLine) :
				message(iMessage), file(iFile), line(iLine) {}

			std::string what(std::size_t messageWidth) const;
		};


		const Info& getInfo() const 
		{ 
			assert(!mInfos.empty()); 
			return mInfos.front(); 
		}

	protected:
		std::list<Info> mInfos;

		mutable std::string mMessage; 
	};

}
