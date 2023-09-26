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

#include <string>
#include <fstream>
#include <streambuf>

#include "Serialization/Serialization.hpp"


namespace claid
{
    class File
    {
        //DECLARE_SERIALIZATION(File)

        protected:
            std::string path;

        private:
            std::string content;

        public:

            template<typename Reflector>
            void reflect(Reflector& r)
            {
                r.member("Path",  path, "Path");
            }

            File();
            File(std::string path);
            virtual ~File() {}

            virtual bool load();

            void setContent(const std::string& content);

            bool saveTo(const std::string& path);
            bool loadFrom(const std::string& path);

            virtual const std::string& getPath();

            virtual const std::string& getContent() const;
    };
}