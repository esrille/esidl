/*
 * Copyright 2011 Esrille Inc.
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
#include <org/w3c/dom/Document.h>
#include <org/w3c/dom/events/Event.h>
#include <org/w3c/dom/events/EventListener.h>
#include <org/w3c/dom/Test.h>

#include <string>

class TestInstance: public PluginInstance
{
public:
    TestInstance(NPP npp, NPObject* window) :
        PluginInstance(npp, window),
        downHandler(0),
        document(0) {
        initialize();
    }
    ~TestInstance();

private:
    org::w3c::dom::events::EventListener downHandler;
    org::w3c::dom::Document document;
    Object testClass;

    void initialize();
    void drawCharts(org::w3c::dom::Document document);
    void down(org::w3c::dom::events::Event evt);
};

class EventHandler : public ObjectImp
{
    TestInstance* instance;
    void (TestInstance::*handler)(org::w3c::dom::events::Event evt);
public:
    EventHandler(TestInstance* instance,
                 void (TestInstance::*handler)(org::w3c::dom::events::Event)) :
        instance(instance),
        handler(handler) {
    }
    virtual void handleEvent(org::w3c::dom::events::Event evt) {
        (instance->*handler)(evt);
    }
    // Object
    virtual Any message_(uint32_t selector, const char* id, int argc, Any* argv) {
        return org::w3c::dom::events::EventListener::dispatch(this, selector, id, argc, argv);
    }
};

class TestObject : public ObjectImp
{
    TestInstance* instance;
public:
    explicit TestObject(TestInstance* instance) :
        instance(instance) {
    }
    signed char testByte(signed char value) {
        return value;
    }
    unsigned int testVariadic(Variadic<std::string> args) {
        printf("arg.length = %u\n", args.size());
        for (unsigned int i = 0; i < args.size(); ++i)
            printf("  %u: %s\n", i, args[i].c_str());
        return args.size();
    }
    Object testObject(Object value) {
        printf("testObject: %p\n", value.self());
        return value;
    }
    // Object
    virtual Any message_(uint32_t selector, const char* id, int argc, Any* argv) {
        return org::w3c::dom::Test::dispatch(this, selector, id, argc, argv);
    }
};

class TestClass : public ObjectImp
{
    TestInstance* instance;
public:
    explicit TestClass(TestInstance* instance) :
        instance(instance) {
    }
    // Object
    virtual Any message_(uint32_t selector, const char* id, int argc, Any* argv) {
        TestObject* test = 0;
        switch (argc) {
        case 0:
            return new(std::nothrow) TestObject(instance);
            break;
        default:
            break;
        }
        return test;
    }
};

#endif // TEST_H_INCLUDED
