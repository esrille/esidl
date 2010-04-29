/*
 * Copyright 2009-2010 Google Inc.
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

#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include <esnpapi.h>

#include <org/w3c/dom.h>
#include <org/w3c/dom/Test.h>

#include <string>

class EventHandler;
class TestClass;

class TestInstance: public PluginInstance
{
    EventHandler* downHandler;
    org::w3c::dom::Document* document;
    TestClass* testClass;

    void initialize();
    void drawCharts(org::w3c::dom::Document* document);

public:
    TestInstance(NPP npp, NPObject* window) :
        PluginInstance(npp, window),
        downHandler(0),
        document(0)
    {
        initialize();
    }
    ~TestInstance();

    void down(org::w3c::dom::events::Event* evt);
};

class EventHandler : public org::w3c::dom::events::EventListener
{
    TestInstance* instance;
    void (TestInstance::*handler)(org::w3c::dom::events::Event* evt);

public:
    EventHandler(TestInstance* instance, void (TestInstance::*handler)(org::w3c::dom::events::Event* evt)) :
        instance(instance),
        handler(handler)
    {
    }
    virtual void handleEvent(org::w3c::dom::events::Event* evt)
    {
        (instance->*handler)(evt);
    }
    unsigned int retain()
    {
        return instance->retain(this);
    };
    unsigned int release()
    {
        return instance->release(this);
    };
};

class TestObject : public org::w3c::dom::Test
{
    TestInstance* instance;
public:
    explicit TestObject(TestInstance* instance)
        : instance(instance)
    {
    }
    virtual signed char testByte(signed char value)
    {
        return value;
    }
    virtual unsigned int testVariadic(Sequence<std::string> args)
    {
        printf("arg.length = %u\n", args.getLength());
        for (unsigned int i = 0; i < args.getLength(); ++i)
        {
            printf("  %u: %s\n", i, args[i].c_str());
        }
        return args.getLength();
    }
    unsigned int retain()
    {
        return instance->retain(this);
    };
    unsigned int release()
    {
        return instance->release(this);
    };
};

class TestClass : public org::w3c::dom::Test_Constructor
{
    TestInstance* instance;
public:
    explicit TestClass(TestInstance* instance)
        : instance(instance)
    {
    }
    virtual org::w3c::dom::Test* createInstance()
    {
        return new (std::nothrow) TestObject(instance);
    }
    unsigned int retain()
    {
        return instance->retain(this);
    };
    unsigned int release()
    {
        return instance->release(this);
    };
};

#endif // TEST_H_INCLUDED
