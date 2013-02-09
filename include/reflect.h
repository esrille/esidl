/*
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
 * WITHOUT WARRANTIES OR CONDITIONS OF Any KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ESIDL_REFLECT_H_INCLUDED
#define ESIDL_REFLECT_H_INCLUDED

#include <stdint.h>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <string>

/**
 * This class provides access to the string-encoded interface meta-data.
 *
 *  ws -> ' '
 *  name -> digits identifier  // digits represents the # of characters in the identifier
 *
 *  interface ->
 *    I name
 *    extends*
 *    implements*
 *    [ operation, setter, getter, constructor, constant ]*
 *
 *  extends -> X name
 *  implements -> M name
 *  operation -> F special* digits type name (type name)* raises*   // digits represent the # of parameters
 *  setter -> S special* 1 v name type raises*
 *  getter -> G special* 0 type name raises*
 *  constructor -> N digits type name (type name)* raises* // digits represent the # of parameters
 *  constant  -> C name value ws  // value represents a double value parseable by strtold()
 *
 *  special ->
 *    g: getter
 *    s: setter
 *    c: creator
 *    d: deleter
 *    f: caller
 *    t: stringifier
 *    o: omittable
 *    V: variadic
 *
 *  type ->
 *    A: any
 *    D: DOMString
 *    Q type: sequence<type>
 *    Q n type: sequence<type, n>           // ES extension
 *    Y type: type[]
 *    Y n type: type[n]                     // ES extension
 *    O name: Object
 *    B type: nullable
 *    v: void
 *    b: boolean
 *    g: byte
 *    h: octet, unsigned byte
 *    s: short
 *    t: unsigned short
 *    l: long
 *    m: unsigned long
 *    x: long long
 *    y: unsigned long long
 *    f: float
 *    d: double
 *    p: void*                              // XXX To be unsupported
 *
 *  raises ->
 *    R name
 *
 *  TODO: embed other extended attributes in the meta-data.
 */
class Reflect
{
public:
    // Type
    static const char kVoid = 'v';
    static const char kBoolean = 'b';
    static const char kByte = 'g';
    static const char kUnsignedByte = 'h';
    static const char kOctet = 'h';
    static const char kShort = 's';
    static const char kUnsignedShort = 't';
    static const char kLong = 'l';
    static const char kUnsignedLong = 'm';
    static const char kLongLong = 'x';
    static const char kUnsignedLongLong = 'y';
    static const char kFloat = 'f';
    static const char kDouble = 'd';
    static const char kString = 'D';
    static const char kAny = 'A';
    static const char kObject = 'O';
    static const char kSequence = 'Q';
    static const char kDate = 'T';
    // Misc.
    static const char kInterface = 'I';
    static const char kExtends = 'X';
    static const char kImplements = 'M';
    static const char kConstant = 'C';
    static const char kOperation = 'F';
    static const char kGetter = 'G';
    static const char kSetter = 'S';
    static const char kConstructor = 'N';
    static const char kException = 'E';
    static const char kRaises = 'R';
    // Special
    static const char kSpecialGetter = 'g';
    static const char kSpecialSetter = 's';
    static const char kSpecialCreator = 'c';
    static const char kSpecialDeleter = 'd';
    static const char kSpecialCaller = 'f';
    static const char kSpecialStringifier = 't';
    static const char kSpecialOmittable = 'o';
    static const char kVariadic = 'V';

    static const char kNullable = '?';

    static const char kUndefinedIsNull = 'n';
    static const char kUndefinedIsEmpty = 'e';
    static const char kNullIsEmpty = 'E';

    // Obsolete
    static const char kArray = 'Y';
    static const char kPointer = 'p';

    class Interface;
    class Array;
    class Sequence;

    static bool isParam(const char* info)
    {
        switch (*info)
        {
        case kBoolean:
        case kByte:
        case kOctet:
        case kShort:
        case kUnsignedShort:
        case kLong:
        case kUnsignedLong:
        case kLongLong:
        case kUnsignedLongLong:
        case kFloat:
        case kDouble:
        case kString:
        case kDate:
        case kAny:
        case kObject:
        case kNullable:
        case kSequence:
        case kArray:
        case kPointer:
            return true;
        default:
            return false;
        }
    }

    static const char* skipDigits(const char* info, unsigned* value = 0)
    {
        unsigned n = 0;
        while (std::isdigit(*info))
        {
            n = 10 * n + (*info - '0');
            ++info;
        }
        if (value)
        {
            *value = n;
        }
        return info;
    }

    static const char* skipName(const char* info)
    {
        unsigned length;
        info = skipDigits(info, &length);
        return info + length;
    }

    static const char* skipNullable(const char* info)
    {
        if (*info == kNullable)
        {
            ++info;
        }
        return info;
    }

    static const char* skipType(const char* info)
    {
        switch (*info)
        {
        case kVoid:
        case kAny:
        case kPointer:
        case kDate:
            return ++info;
        case kBoolean:
        case kByte:
        case kOctet:
        case kShort:
        case kUnsignedShort:
        case kLong:
        case kUnsignedLong:
        case kLongLong:
        case kUnsignedLongLong:
        case kFloat:
        case kDouble:
        case kString:
            return skipNullable(++info);
        case kSequence:
            return skipType(skipDigits(++info));
        case kObject:
            return skipName(++info);
        case kArray:
            return skipType(skipDigits(++info));
        default:
            return 0;
        }
    }

    /**
     * This represents a type record from the specified reflection data.
     */
    class Type
    {
        const char* info;

        friend class Array;
        friend class Sequence;

    public:
        /**
         * Constructs an object which represents the specified type.
         * @param info the string encoded reflection data generated by esidl.
         */
        Type(const char* info) :
            info(info)
        {
        }

        /**
         * Gets the type of this type descriptor.
         */
        char getType() const
        {
            return *info;
        }

        /**
         * Checks if this type is an interface.
         */
        bool isInterface() const
        {
            return (getType() == kInterface) ? true : false;
        }

        /**
         * Checks if this type is an exception.
         */
        bool isException() const
        {
            return (getType() == kException) ? true : false;
        }

        /**
         * Checks if this type is an array.
         */
        bool isArray() const
        {
            return (getType() == kArray) ? true : false;
        }

        /**
         * Checks if this type is a sequence.
         */
        bool isSequence() const
        {
            return (getType() == kSequence) ? true : false;
        }

        /**
         * Checks if this type is an integer.
         */
        bool isInteger() const
        {
            switch (getType())
            {
            case kByte:
            case kOctet:
            case kShort:
            case kUnsignedShort:
            case kLong:
            case kUnsignedLong:
            case kLongLong:
            case kUnsignedLongLong:
                return true;
            default:
                return false;
            }
        }

        /**
         * Checks if this type is a boolean.
         */
        bool isBoolean() const
        {
            return (getType() == kBoolean) ? true : false;
        }

        /**
         * Checks if this type is a floating point number.
         */
        bool isFloat() const
        {
            switch (getType())
            {
            case kFloat:
            case kDouble:
                return true;
            default:
                return false;
            }
        }

        /**
         * Checks if this type is an object.
         */
        bool isObject() const
        {
            return (getType() == kObject) ? true : false;
        }

        /**
         * Checks if this type is any.
         */
        bool isAny() const
        {
            return (getType() == kAny) ? true : false;
        }

        /**
         * Checks if this type is a string.
         */
        bool isString() const
        {
            return (getType() == kString) ? true : false;
        }

        /**
         * Checks if this type is a nullable type.
         */
        bool isNullable() const
        {
            switch (getType())
            {
            case kBoolean:
            case kByte:
            case kOctet:
            case kShort:
            case kUnsignedShort:
            case kLong:
            case kUnsignedLong:
            case kLongLong:
            case kUnsignedLongLong:
            case kFloat:
            case kDouble:
            case kString:
                return info[1] == kNullable;
            default:
                return false;
            }
        }

        /**
         * Gets the size of this type.
         */
        int getSize() const
        {
            switch (getType())
            {
            case kVoid:
                return 0;
            case kBoolean:
                return sizeof(bool);
            case kByte:
            case kOctet:
                return sizeof(uint8_t);
            case kShort:
            case kUnsignedShort:
                return sizeof(uint16_t);
            case kLong:
            case kUnsignedLong:
                return sizeof(uint32_t);
            case kLongLong:
            case kUnsignedLongLong:
                return sizeof(uint64_t);
            case kFloat:
                return sizeof(float);
            case kDouble:
                return sizeof(double);
            case kPointer:
            case kObject:
                return sizeof(void*);
            case kArray:
                {
                    Array a(info);
                    return a.getType().getSize() * a.getRank();
                }
                break;
            case kSequence:
                {
                    Sequence seq(info);
                    return seq.getType().getSize() * seq.getMax();
                }
                break;
            default:
                return 0;
            }
        }

        /**
         * Gets the interface of this type.
         */
        const std::string getQualifiedName() const
        {
            if (!isObject())
            {
                return "";
            }
            unsigned length;
            const char* name = skipDigits(info + 1, &length);
            return std::string(name, length);
        }
    };

    /**
     * This represents a function parameter loaded from the specified reflection data.
     */
    class Parameter
    {
        const char* info;

    public:
        /** Default constructor
         */
        Parameter() :
            info("")
        {
        }

        /**
         * Constructs an object which represents the specified parameter.
         * @param info the string encoded reflection data generated by esidl.
         */
        Parameter(const char* info) :
            info(info)
        {
        }

        /**
         * Gets the type of this parameter.
         */
        Type getType() const
        {
            return Type(info);
        }

        /**
         * Gets the name of this parameter.
         */
        const std::string getName() const
        {
            const char* name = skipType(info);
            unsigned length;
            name = skipDigits(name, &length);
            return std::string(name, length);
        }

        bool next()
        {
            info = skipType(info);
            info = skipName(info);
            if (!isParam(info))
            {
                info = "";
                return false;
            }
            return true;
        }
    };

    /**
     * This represents an operation loaded from the specified reflection data.
     */
    class Method
    {
        const char* info;

    public:
        /** Default constructor
         */
        Method() :
            info(0)
        {
        }

        /**
         * Constructs an object which represents the specified operation.
         * @param info the string encoded reflection data generated by esidl.
         */
        Method(const char* info) :
            info(info)
        {
        }

        /**
         * Copy-constructor.
         */
        Method(const Method& operation) :
            info(operation.info)
        {
        }

        /**
         * Gets the type of this type descriptor.
         */
        char getType() const
        {
            return *info;
        }

        bool isOperation() const
        {
            return (getType() == kOperation) ? true : false;
        }

        bool isConstructor() const
        {
            return (getType() == kConstructor) ? true : false;
        }

        bool isGetter() const
        {
            return (getType() == kGetter) ? true : false;
        }

        bool isSetter() const
        {
            return (getType() == kSetter) ? true : false;
        }

        /**
         * Gets the name of this function.
         */
        const std::string getName() const
        {
            const char* name = skipType(skipDigits(skipSpecial(info + 1)));
            unsigned length;
            name = skipDigits(name, &length);
            return std::string(name, length);
        }

        /**
         * Gets the type of the return value of this function.
         */
        Type getReturnType() const
        {
            return Type(skipDigits(skipSpecial(info + 1)));
        }

        /**
         * Gets the number of arguments.
         */
        unsigned getParameterCount() const
        {
            unsigned paramCount;
            skipDigits(skipSpecial(info + 1), &paramCount);
            return paramCount;
        }

        /**
         * Gets an argument iterater.
         */
        Parameter listParameter() const
        {
            return Parameter(skipDigits(skipSpecial(info + 1)));
        }

        static const char* skip(const char* info)
        {
            const char* p = skipSpecial(info + 1);
            unsigned count;
            p = skipDigits(p, &count);
            ++count;  // for name
            do {
                p = skipType(p);
                p = skipName(p);
            } while (--count != 0);
            // skip R
            while (*p == kRaises)
            {
                p = skipName(p + 1);
            }
            return p;
        }

        static const char* skipSpecial(const char* info)
        {
            while (*info && !std::isdigit(*info))
            {
                ++info;
            }
            return info;
        }

        bool hasSpecial(char special) const
        {
            for (const char* p = info + 1; *p && !std::isdigit(*p); ++p)
            {
                if (*p == special)
                {
                    return true;
                }
            }
            return false;
        }

        bool isSpecialGetter() const
        {
            return hasSpecial(kSpecialGetter);
        }

        bool isSpecialSetter() const
        {
            return hasSpecial(kSpecialSetter);
        }

        bool isSpecialCreator() const
        {
            return hasSpecial(kSpecialCreator);
        }

        bool isSpecialDeleter() const
        {
            return hasSpecial(kSpecialDeleter);
        }

        bool isSpecialCaller() const
        {
            return hasSpecial(kSpecialCaller);
        }

        bool isSpecialStringifier() const
        {
            return hasSpecial(kSpecialStringifier);
        }

        bool isSpecialOmittable() const
        {
            return hasSpecial(kSpecialOmittable);
        }

        bool isVariadic() const
        {
            return hasSpecial(kVariadic);
        }
    };

    /**
     * This represents a constant loaded from the specified reflection data.
     */
    class Constant
    {
        const char* info;

    public:
        /** Default constructor
         */
        Constant() :
            info(0)
        {
        }

        /**
         * Constructs an object which represents the specified constant.
         * @param info the string encoded reflection data generated by esidl.
         */
        Constant(const char* info) :
            info(info)
        {
        }

        /**
         * Gets the type of this constant.
         */
        Type getType() const
        {
            return Type(info + 1);
        }

        /**
         * Gets the name of this constant.
         */
        const std::string getName() const
        {
            const char* name = skipType(info + 1);
            unsigned length;
            name = skipDigits(name, &length);
            return std::string(name, length);
        }

        /**
         * Gets the value of this constant.
         */
        double getValue() const
        {
            return std::strtod(skipName(skipType(info + 1)), 0);
        }

        static const char* skip(const char* info)
        {
            const char* p = skipType(info + 1);
            p = strchr(p, ' ');
            return p ? ++p : 0;
        }
    };

    /**
     * This represents an interface loaded from the specified reflection data.
     */
    class Interface
    {
        const char* info;
        const char* qualifiedName;
        int methodCount;
        int constantCount;
        int constructorCount;
        int inheritedMethodCount;

        static const char* step(const char* info)
        {
            if (!info)
            {
                return info;
            }
            switch (*info)
            {
            case kInterface:
            case kExtends:
            case kImplements:
                info = skipName(++info);
                break;
            case kConstant:
                info = Constant::skip(info);
                break;
            case kOperation:
            case kSetter:
            case kGetter:
            case kConstructor:
                info = Method::skip(info);
                break;
            }

            return info;
        }

    public:
        /**
         * Default constructor.
         */
        Interface() :
            info(0),
            qualifiedName(0),
            methodCount(0),
            constantCount(0),
            constructorCount(0),
            inheritedMethodCount(0)
        {
        }

        /**
         * Constructs a new object which represents the specified interface.
         * @param info the string encoded reflection data generated by esidl.
         * @param qualifiedName the qualified name of this interface.
         */
        Interface(const char* info, const char* qualifiedName = 0) :
            info(info),
            qualifiedName(qualifiedName),
            methodCount(0),
            constantCount(0),
            constructorCount(0),
            inheritedMethodCount(0)
        {
            // TODO: Validate info and qualifiedName
            const char* p = info;
            // skip I
            p = skipName(++p);
            // skip X
            while (*p == kExtends || *p == kImplements)
            {
                p = skipName(++p);
            }
            while (p && *p)
            {
                switch (*p)
                {
                case kConstant:
                    p = Constant::skip(p);
                    if (p)
                    {
                        ++constantCount;
                    }
                    break;
                case kOperation:
                case kSetter:
                case kGetter:
                    p = Method::skip(p);
                    if (p)
                    {
                        ++methodCount;
                    }
                    break;
                case kConstructor:
                    p = Method::skip(p);
                    if (p)
                    {
                        ++constructorCount;
                    }
                    break;
                default:
                    p = 0;
                    break;
                }
            }
        }

        /**
         * Copy-constructor.
         */
        Interface(const Interface& interface) :
            info(interface.info),
            qualifiedName(interface.qualifiedName),
            methodCount(interface.methodCount),
            constantCount(interface.constantCount),
            constructorCount(interface.constructorCount),
            inheritedMethodCount(interface.inheritedMethodCount)
        {
        }

        /**
         * Gets the name of this interface.
         */
        std::string getName() const
        {
            std::string name = getQualifiedName();
            size_t pos = name.rfind(':');
            if (pos == name.npos)
            {
                return name;
            }
            return name.substr(pos + 1);
        }

        /**
         * Gets the qualified name of this interface.
         */
        std::string getQualifiedName() const
        {
            unsigned length;
            const char* name = skipDigits(info + 1, &length);
            return std::string(name, length);
        }

        /**
         * Gets the module name of this interface.
         */
        std::string getQualifiedModuleName() const
        {
            std::string name = getQualifiedName();
            size_t pos = name.rfind(':');
            if (pos == name.npos)
            {
                return "";
            }
            return name.substr(0, pos + 1);
        }

        /**
         * Gets the qualified name of the super interface.
         */
        std::string getQualifiedSuperName() const
        {
            const char* super = skipName(info + 1);
            if (*super != kExtends)
            {
                return "";
            }
            unsigned length;
            super = skipDigits(super + 1, &length);
            return std::string(super, length);
        }

        const char* getQualifiedSuperName(unsigned* length) const
        {
            const char* super = skipName(info + 1);
            if (*super != kExtends)
            {
                return 0;
            }
            return skipDigits(super + 1, length);
        }

        /**
         * Gets the number of methods in this interface.
         * @return the method count excluding super class methods.
         */
        unsigned getMethodCount() const
        {
            return methodCount;
        }

        /**
         * Gets the specified method.
         * @param n the method number excluding super class methods.
         */
        Method getMethod(unsigned n) const
        {
            const char* p = info;
            while (p)
            {
                switch (*p)
                {
                case kOperation:
                case kSetter:
                case kGetter:
                    if (n == 0)
                    {
                        return Method(p);
                    }
                    --n;
                }
                p = step(p);
            }
            return Method();
        }

        /**
         * Gets the number of constants in this interface.
         */
        unsigned getConstantCount() const
        {
            return constantCount;
        }

        /**
         * Gets the specified constant.
         * @param n the constant number.
         */
        Constant getConstant(unsigned n) const
        {
            const char* p = info;
            while (p)
            {
                if (*p == kConstant)
                {
                    if (n == 0)
                    {
                        return Constant(p);
                    }
                    --n;
                }
                p = step(p);
            }
            return Constant();
        }

        /**
         * Gets the number of constructors in this interface.
         * @return true if this interface has a constructor interface.
         */
        unsigned getConstructorCount() const
        {
            return constructorCount;
        }

        /**
         * Gets the constructor interface for this interface.
         * @param n the constructor number.
         * @return the constructor method.
         */
        Method getConstructor(unsigned n) const
        {
            const char* p = info;
            while (p)
            {
                if (*p == kConstructor)
                {
                    if (n == 0)
                    {
                        return Method(p);
                    }
                    --n;
                }
                p = step(p);
            }
            return Method();
        }

        /**
         * Gets the number of methods in this interface.
         * @return the method count excluding super class methods.
         */
        unsigned getInheritedMethodCount() const
        {
            return inheritedMethodCount;
        }

        void setInheritedMethodCount(unsigned count)
        {
            inheritedMethodCount = count;
        }

        const char* getMetaData() const
        {
            return qualifiedName ? qualifiedName : info;  // TODO: should return info, but qualifiedName is still used in the interfaceStore classes
        }
    };

    /**
     * This represents an array type loaded from the specified reflection data.
     */
    class Array
    {
        const char* info;

    public:
        /**
         * Default constructor.
         */
        Array() :
            info(0)
        {
        }

        /**
         * Constructs a new object which represents the specified array.
         * @param info the string encoded reflection data generated by esidl.
         */
        Array(const char* info) :
            info(info)
        {
        }

        /**
         * Gets the type of elements value of this array.
         */
        Type getType() const
        {
            return Type(skipDigits(info + 1));
        }

        /**
         * Gets the rank of this array.
         */
        unsigned getRank() const
        {
            unsigned rank;
            skipDigits(info + 1, &rank);
            return rank;
        }
    };

    /**
     * This represents a sequence type loaded from the specified reflection data.
     */
    class Sequence
    {
        const char* info;

    public:
        /**
         * Default constructor.
         */
        Sequence() :
            info(0)
        {
        }

        /**
         * Constructs a new object which represents the specified sequence.
         * @param info the string encoded reflection data generated by esidl.
         */
        Sequence(const char* info) :
            info(info)
        {
        }

        Sequence(Type type) :
            info(type.info)
        {
        }

        /**
         * Gets the type of elements value of this sequence.
         */
        Type getType() const
        {
            return Type(skipDigits(info + 1));
        }

        /**
         * Gets the maximum size of this sequence. Returns zero if this is a variable length sequence.
         */
        unsigned getMax() const
        {
            unsigned max;
            skipDigits(info + 1, &max);
            return max;
        }
    };

    struct SymbolData
    {
        const char* symbol;
        unsigned offset;  // offset to metadata
        unsigned number;  // symbol number
    };
};

#endif  // ESIDL_REFLECT_H_INCLUDED
