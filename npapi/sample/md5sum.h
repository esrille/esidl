/*
 * Copyright 2011 Esrille Inc.
 * Copyright 2010 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SAMPLE_MD5SUM_H_
#define SAMPLE_MD5SUM_H_

#include <esnpapi.h>
#include <com/rsa/MD5.h>

class MD5SumInstance : public PluginInstance
{
    Object md5class;
    void initialize();
public:
    MD5SumInstance(NPP npp, NPObject* window) :
        PluginInstance(npp, window) {
        initialize();
    }
    ~MD5SumInstance();
};

class MD5Object : public ObjectImp
{
    MD5SumInstance* instance;
public:
    explicit MD5Object(MD5SumInstance* instance) :
        instance(instance) {
    }
    void update(std::string input) {
    }
    std::string final() {
        return "900150983cd24fb0d6963f7d28e17f72";  // dummy
    }
    // Object
    virtual Any message_(uint32_t selector, const char* id, int argc, Any* argv) {
        return com::rsa::MD5::dispatch(this, selector, id, argc, argv);
    }
};

class MD5Constructor : public ObjectImp {
    MD5SumInstance* instance;
public:
    explicit MD5Constructor(MD5SumInstance* instance) :
        instance(instance) {
    }
    // Object
    virtual Any message_(uint32_t selector, const char* id, int argc, Any* argv) {
        MD5Object* md5 = 0;
        switch (argc) {
        case 0:
            md5 = new(std::nothrow) MD5Object(instance);
            break;
        default:
            break;
        }
        return md5;
    }
};

#endif  // SAMPLE_MD5SUM_H_
