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
#include "File/File.hpp"


namespace claid
{
    File::File()
    {

    }

    File::File(std::string path) : path(path)
    {
        
    }

    bool File::load()
    {
        return this->loadFrom(this->path);
    }

    void File::setContent(const std::string& content)
    {
        this->content = content;
    }

    bool File::saveTo(const std::string& path)
    {
        std::ofstream file(path);

        if(!file.is_open())
        {
            return false;
        }


        file << this->content;
        return true;
    }

    bool File::loadFrom(const std::string& path)
    {
        this->path = path;
        std::ifstream file(path);

        if(!file.is_open())
        {
            return false;
        }

        this->content = std::string ((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());


        return true;
    }

    const std::string& File::getPath()
    {
        return this->path;
    }


    const std::string& File::getContent() const
    {
        return this->content;
    }
}

REGISTER_SERIALIZATION(claid::File)
