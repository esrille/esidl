/*
 * Copyright 2011 Esrille Inc.
 * Copyright 2008-2010 Google Inc.
 * Copyright 2006, 2007 Nintendo Co., Ltd.
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

#ifndef ESNPAPI_OBJECT_H_INCLUDED
#define ESNPAPI_OBJECT_H_INCLUDED

#include <Any.h>
#include <nullable.h>
#include <sequence.h>
#include <Variadic.h>

#include <assert.h>
#include <cstring>

class Object
{
protected:
    Object* object;

public:
    virtual unsigned int retain_() {
        if (object && object != this)
            return object->retain_();
        return 1;
    }
    virtual unsigned int release_() {
        if (object && object != this) {
            if (object->release_() == 0)
                object = 0;
        }
        return 0;
    }

public:
    Object() :
        object(0) {
    }
    Object(Object* other) :
        object(other) {
        if (other)
            other->retain_();
    }
    Object(const Object& other) :
        object(other.self()) {
        const_cast<Object*>(&other)->retain_();
    }
    virtual ~Object() {
        release_();
    }
    virtual Any message_(uint32_t selector, const char* id, int argc, Any* argv) {
        if (object && object != this)
            return object->message_(selector, id, argc, argv);
        printf("Warning: %s: forwarding a message '%s' to self(%p->%p)", __func__, id, this, object);
        assert(0);
        return Any();
    }
    bool operator!() const {
        return !object;
    }
    operator void*( ) const {
        return object;
    }
    Object* self() const {
        return object;
    }
    Object& operator=(const Object& other)
    {
        if (this != &other) {
            const_cast<Object*>(&other)->retain_();
            release_();
            object = other.self();
        }
        return *this;
    }

    // Predefined argument count
    static const int GETTER_ = -1;
    static const int SETTER_ = -2;
    static const int SPECIAL_GETTER_ = -3;
    static const int SPECIAL_DELETER_ = -4;
    static const int SPECIAL_SETTER_ = -5;
    static const int SPECIAL_CREATOR_ = -6;
    static const int SPECIAL_SETTER_CREATOR_ = -7;
    static const int HAS_PROPERTY_ = -8;
    static const int HAS_OPERATION_ = -9;
    static const int IS_KIND_OF_ = -10;
    static const int STRINGIFY_ = -11;
    static const int CALLBACK_ = 0x4000000;

    // Max # of variadic arguments
    static const int MAX_VARIADIC_ = 16;
};

template <typename X>
X interface_cast(const Object& object)
{
    return X(object.self());
}

#endif  // ESNPAPI_OBJECT_H_INCLUDED
