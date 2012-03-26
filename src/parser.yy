/*
 * Copyright 2011, 2012 Esrille Inc.
 * Copyright 2008-2010 Google Inc.
 * Copyright 2007 Nintendo Co., Ltd.
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

/*
 * These coded instructions, statements, and computer programs contain
 * software derived from the following specifications:
 *
 * W3C,
 * Web IDL,
 * W3C Editor’s Draft 22 March 2012.
 * http://dev.w3.org/2006/webapi/WebIDL/
 */

%{

#include "esidl.h"

extern FILE* yyin;

extern "C" int yyparse(void);

void yyerror(const char* message, ...);

extern "C" int yywrap()
{
    return 1;
}

static long stdinOffset;
static long stdinLine;

%}

%locations

%union
{
    int                     attr;
    char*                   name;
    Node*                   node;
    NodeList*               nodeList;
    std::list<std::string>* nameList;
}

%{

int yylex();

%}

%token ANY
%token ATTRIBUTE
%token BOOLEAN
%token BYTE
%token CALLBACK
%token CALLER
%token CONST
%token CREATOR
%token DATE
%token DELETER
%token DICTIONARY
%token DOUBLE
%token ELLIPSIS
%token ENUM
%token EOL
%token EXCEPTION
%token FALSE
%token FLOAT
%token GETRAISES
%token GETTER
%token IMPLEMENTS
%token IN
%token INFINITY
%token INHERIT
%token INTERFACE
%token LONG
%token MODULE
%token NAN
%token NATIVE
%token NULL_LITERAL
%token OBJECT
%token OCTET
%token OMITTABLE
%token OPTIONAL
%token OR
%token PARTIAL
%token RAISES
%token READONLY
%token SETRAISES
%token SETTER
%token SEQUENCE
%token SHORT
%token STATIC
%token STRING
%token STRINGIFIER
%token TRUE
%token TYPEDEF
%token UNRESTRICTED
%token UNSIGNED
%token VOID

%token OP_SCOPE
%token OP_SHL
%token OP_SHR

%token POUND_SIGN

%token <name>       IDENTIFIER
%token <name>       INTEGER_LITERAL
%token <name>       FLOATING_PT_LITERAL
%token <name>       STRING_LITERAL
%token <name>       JAVADOC

%type <node>        Definition
%type <node>        Module
%type <node>        CallbackOrInterface
%type <node>        CallbackRestOrInterface
%type <node>        Interface
%type <node>        PartialInterface
%type <node>        Inheritance
%type <node>        InterfaceMember
%type <node>        Dictionary
%type <node>        DictionaryMember
%type <node>        Default
%type <node>        DefaultValue
%type <node>        Exception
%type <node>        CallbackRest
%type <node>        Typedef
%type <node>        ImplementsStatement
%type <node>        Const
%type <node>        ConstExpr
%type <node>        Literal
%type <node>        BooleanLiteral
%type <node>        FloatLiteral
%type <node>        AttributeOrOperation
%type <node>        StringifierAttributeOrOperation
%type <node>        Attribute
%type <attr>        Inherit
%type <attr>        ReadOnly
%type <node>        GetRaises
%type <node>        SetRaises
%type <node>        Operation
%type <attr>        Qualifiers
%type <attr>        Specials
%type <attr>        Special
%type <node>        OperationRest
%type <name>        OptionalIdentifier
%type <node>        Raises
%type <node>        ExceptionList
%type <node>        OptionalOrRequiredArgument
%type <name>        ArgumentName
%type <attr>        Ellipsis
%type <node>        ExceptionMember
%type <node>        ExceptionField
%type <nodeList>    ExtendedAttributeList
%type <nodeList>    ExtendedAttributes
%type <name>        ArgumentNameKeyword
%type <node>        Type
%type <node>        SingleType
%type <node>        UnionType
%type <node>        UnionMemberType
%type <node>        UnionMemberTypes
%type <node>        NonAnyType
%type <node>        ConstType
%type <node>        PrimitiveType
%type <node>        UnrestrictedFloatType
%type <node>        FloatType
%type <node>        UnsignedIntegerType
%type <node>        IntegerType
%type <attr>        OptionalLong
%type <node>        TypeSuffix
%type <attr>        Null
%type <node>        ReturnType
%type <node>        ScopedNameList
%type <node>        ScopedName
%type <node>        AbsoluteScopedName
%type <node>        RelativeScopedName
%type <node>        ScopedNameParts
%type <node>        ScopedNamePartList
%type <node>        ExtendedAttribute
%type <node>        ExtendedAttributeNoArg
%type <node>        ExtendedAttributeArgList
%type <node>        ExtendedAttributeNamedArgList
%type <node>        ExtendedAttributeScopedName

/* ES extensions */
%type <node>        InterfaceDcl
%type <node>        ForwardDcl
%type <node>        OrExpr
%type <node>        XorExpr
%type <node>        AndExpr
%type <node>        ShiftExpr
%type <node>        AddExpr
%type <node>        MultExpr
%type <node>        UnaryExpr
%type <node>        PrimaryExpr
%type <name>        UnaryOperator
%type <node>        Preprocessor
%type <node>        positive_int_const
%type <name>        JavaDoc

%%

Definitions :
    /* empty */
    | JavaDoc
        {
            setJavadoc($1);
            free($1);
        }
    ExtendedAttributeList Definition
        {
            if ($4)
            {
                $4->setExtendedAttributes($3);
            }
            setJavadoc(0);
        }
    Definitions
    ;

Definition :
    Module          /* backward compatibility */
    | CallbackOrInterface
    | PartialInterface
    | Dictionary
    | Exception
    | Enum
    | Typedef
    | ImplementsStatement
    | Preprocessor  /* esidl extension */
    ;

Module :
    MODULE IDENTIFIER
        {
            if (!Node::getFlatNamespace())
            {
                Node* node = getCurrent()->search($2);
                if (node)
                {
                    if (Module* module = dynamic_cast<Module*>(node))
                    {
                        setCurrent(module);
                    }
                    else
                    {
                        fprintf(stderr, "%d.%d-%d.%d: '%s' is not a valid module name.\n",
                                @1.first_line, @1.first_column, @2.last_line, @2.last_column,
                                $2);
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    Module* module = new Module($2);
                    getCurrent()->add(module);
                    setCurrent(module);
                }
            }
            free($2);
        }
    '{' Definitions '}' ';'
        {
            $$ = getCurrent();
            if (!Node::getFlatNamespace())
            {
                setCurrent(getCurrent()->getParent());
            }
        }
    ;

CallbackOrInterface :
    CALLBACK CallbackRestOrInterface
        {
            if ($2)
            {
                if (Interface* node = dynamic_cast<Interface*>($2)) {
                    node->setAttr(node->getAttr() | Interface::Callback);
                    $$ = $2;
                } else if (OpDcl* op = dynamic_cast<OpDcl*>($2)) {
                    Node* node = op->getParent();
                    node->setAttr(node->getAttr() | Interface::CallbackIsFunctionOnly);
                    $$ = node;
                }
            }

        }
    | InterfaceDcl
    ;

CallbackRestOrInterface :
    CallbackRest
    | InterfaceDcl
    ;

InterfaceDcl :
    Interface
    | ForwardDcl    /* esidl extension */
    ;

Interface :
    INTERFACE IDENTIFIER Inheritance
        {
            Node* extends = $3;
            if (!extends)
            {
                if (strcmp($2, Node::getBaseObjectName()) == 0)
                {
                    assert($2[0] == ':' && $2[1] == ':');
                    strcpy($2, strrchr($2, ':') + 1);
                }
                else
                {
                    std::string qualifiedName = getCurrent()->getQualifiedName();
                    qualifiedName += "::";
                    qualifiedName += $2;
                    if (qualifiedName != Node::getBaseObjectName())
                    {
                        ScopedName* name = new ScopedName(Node::getBaseObjectName());
                        extends = new Node();
                        extends->add(name);
                    }
                }
            }
            Interface* node = new Interface($2, extends);
            if (Node::getFlatNamespace() && !extends)
            {
                setCurrent(getSpecification());
            }
            getCurrent()->add(node);
            setCurrent(node);
            free($2);
        }
    '{' InterfaceMembers '}' ';'
        {
            getCurrent()->setLocation(&@1, &@8);
            $$ = getCurrent();
            setCurrent(getCurrent()->getParent());
            if (Node::getFlatNamespace() && getCurrent() == getSpecification())
            {
                setCurrent(dynamic_cast<Module*>(getSpecification()->search(Node::getFlatNamespace())));
            }
        }
    ;

PartialInterface :
    PARTIAL INTERFACE IDENTIFIER
        {
            Interface* node = new Interface($3);
            if (Node::getFlatNamespace())
            {
                setCurrent(getSpecification());
            }
            getCurrent()->add(node);
            setCurrent(node);
            node->setAttr(node->getAttr() | Interface::Supplemental);
            free($3);
        }
    '{' InterfaceMembers '}' ';'
        {
            getCurrent()->setLocation(&@1, &@8);
            $$ = getCurrent();
            setCurrent(getCurrent()->getParent());
            if (Node::getFlatNamespace() && getCurrent() == getSpecification())
            {
                setCurrent(dynamic_cast<Module*>(getSpecification()->search(Node::getFlatNamespace())));
            }
        }
    ;

InterfaceMembers :
    /* empty */
    | JavaDoc
        {
            setJavadoc($1);
            free($1);
        }
    ExtendedAttributeList InterfaceMember
        {
            if ($4)
            {
                $4->setExtendedAttributes($3);
            }
            setJavadoc(0);
        }
    InterfaceMembers
    ;

InterfaceMember :
    Const
    | AttributeOrOperation
    | Preprocessor
    ;

Dictionary :
    DICTIONARY IDENTIFIER Inheritance
        {
            Node* extends = $3;
            if (!extends)
            {
                std::string qualifiedName = getCurrent()->getQualifiedName();
                qualifiedName += "::";
                qualifiedName += $2;
                assert((qualifiedName != Node::getBaseObjectName()));  // TODO: make this runtime check
                ScopedName* name = new ScopedName(Node::getBaseObjectName());
                extends = new Node();
                extends->add(name);
            }
            Interface* node = new Dictionary($2, extends);
            if (Node::getFlatNamespace() && !extends)
            {
                setCurrent(getSpecification());
            }
            getCurrent()->add(node);
            setCurrent(node);
            free($2);
        }
    '{' DictionaryMembers '}' ';'
        {
            getCurrent()->setLocation(&@1, &@8);
            $$ = getCurrent();
            setCurrent(getCurrent()->getParent());
            if (Node::getFlatNamespace() && getCurrent() == getSpecification())
            {
                setCurrent(dynamic_cast<Module*>(getSpecification()->search(Node::getFlatNamespace())));
            }
        }
    ;

DictionaryMembers :
    /* empty */
    | JavaDoc
        {
            setJavadoc($1);
            free($1);
        }
    ExtendedAttributeList DictionaryMember
        {
            if ($4)
            {
                $4->setExtendedAttributes($3);
            }
            setJavadoc(0);
        }
    DictionaryMembers
    ;

DictionaryMember :
    Type IDENTIFIER Default ';'
        {
            Attribute* attr = new Attribute($2, $1);
            if ($3)
            {
                attr->setDefaultValue($3);
            }
            getCurrent()->add(attr);
            $$ = attr;
        }

    ;

Default:
    /* empty */
        {
            $$ = 0;
        }
    | '=' DefaultValue
        {
            $$ = $2;
        }
    ;

DefaultValue :
    /* empty */         /* TODO: Do we need an empty here? */
        {
            $$ = 0;
        }
    | ConstExpr
        {
            $$ = $1;
        }
    | STRING_LITERAL
        {
            $$ = new Literal($1);   /* TODO: Verify this still works */
            free($1);
        }
    ;

/* TODO: Support 'Inheritance'. */
Exception :
    EXCEPTION IDENTIFIER Inheritance
        {
            ExceptDcl* exc = new ExceptDcl($2);
            getCurrent()->add(exc);
            setCurrent(exc);
            free($2);
        }
    '{' ExceptionMembers '}' ';'
        {
            $$ = getCurrent();
            setCurrent(getCurrent()->getParent());
        }
    ;

ExceptionMembers :
    /* empty */
    | JavaDoc
        {
            setJavadoc($1);
            free($1);
        }
    ExtendedAttributeList ExceptionMember
        {
            if ($4)
            {
                $4->setExtendedAttributes($3);
            }
            setJavadoc(0);
        }
    ExceptionMembers
    ;

Inheritance :
    /* empty */
        {
            $$ = 0;
        }
    | ':' ScopedNameList
        {
            $$ = $2;
        }
    ;

/* TODO: Support 'Enum'. */
Enum :
    ENUM IDENTIFIER '{' EnumValueList '}' ';'
    ;

EnumValueList :
    STRING_LITERAL EnumValues
    ;

EnumValues:
    /* empty */
    | ',' STRING_LITERAL EnumValues
    ;

CallbackRest :
    IDENTIFIER '=' ReturnType
        {
            ScopedName* name = new ScopedName(Node::getBaseObjectName());
            Node* extends = new Node();
            extends->add(name);

            Interface* node = new Interface($1, extends);
            if (Node::getFlatNamespace())
            {
                setCurrent(getSpecification());
            }
            getCurrent()->add(node);
            setCurrent(node);

            OpDcl* op = new OpDcl("", $3, OpDcl::Caller);
            node->add(op);
            setCurrent(op);
        }
    '(' ArgumentList ')' ';'
        {
            $$ = getCurrent();
            setCurrent(getCurrent()->getParent()->getParent());
            if (Node::getFlatNamespace() && getCurrent() == getSpecification())
            {
                setCurrent(dynamic_cast<Module*>(getSpecification()->search(Node::getFlatNamespace())));
            }

            free($1);
        }
    ;

Typedef :
    TYPEDEF Type IDENTIFIER ';'
        {
            Member* m = new Member($3);
            // In flat namespace mode, even a valid typedef can define a new type for the spec using the exactly same name.
            if (!dynamic_cast<ScopedName*>($2) || m->getQualifiedName() != $2->getQualifiedName())
            {
                m->setSpec($2);
                m->setTypedef(true);
                getCurrent()->add(m);
            }
            $$ = m;
        }
    | NATIVE IDENTIFIER ';'  /* esidl extension */
        {
            NativeType* nativeType = new NativeType($2);
            getCurrent()->add(nativeType);
            $$ = nativeType;
        }
    ;

ImplementsStatement :
    ScopedName IMPLEMENTS ScopedName ';'
        {
            Implements* implements = new Implements(static_cast<ScopedName*>($1), static_cast<ScopedName*>($3));
            getCurrent()->add(implements);
            implements->setLocation(&@1, &@4);
            $$ = implements;
        }
    ;

Const :
    CONST ConstType IDENTIFIER '=' ConstExpr ';'
        {
            ConstDcl* constDcl = new ConstDcl($2, $3, $5);
            getCurrent()->add(constDcl);
            $$ = constDcl;
        }
    ;

ConstExpr :
    OrExpr
    ;

OrExpr :
    XorExpr
    | OrExpr '|' XorExpr
        {
            $$ = new BinaryExpr("|", $1, $3);
        }
    ;

XorExpr :
    AndExpr
    | XorExpr '^' AndExpr
        {
            $$ = new BinaryExpr("^", $1, $3);
        }
    ;

AndExpr :
    ShiftExpr
    | AndExpr '&' ShiftExpr
        {
            $$ = new BinaryExpr("&", $1, $3);
        }
    ;

ShiftExpr :
    AddExpr
    | ShiftExpr OP_SHR AddExpr
        {
            $$ = new BinaryExpr(">>", $1, $3);
        }
    | ShiftExpr OP_SHL AddExpr
        {
            $$ = new BinaryExpr("<<", $1, $3);
        }
    ;

AddExpr :
    MultExpr
    | AddExpr '+' MultExpr
        {
            $$ = new BinaryExpr("+", $1, $3);
        }
    | AddExpr '-' MultExpr
        {
            $$ = new BinaryExpr("-", $1, $3);
        }
    ;

MultExpr :
    UnaryExpr
    | MultExpr '*' UnaryExpr
        {
            $$ = new BinaryExpr("*", $1, $3);
        }
    | MultExpr '/' UnaryExpr
        {
            $$ = new BinaryExpr("/", $1, $3);
        }
    | MultExpr '%' UnaryExpr
        {
            $$ = new BinaryExpr("%", $1, $3);
        }
    ;

UnaryExpr :
    UnaryOperator PrimaryExpr
        {
            $$ = new UnaryExpr($1, $2);
        }
    | PrimaryExpr
    ;

UnaryOperator :
    '-'
        {
            $$ = "-";
        }
    | '+'
        {
            $$ = "+";
        }
    | '~'
        {
            $$ = "~";
        }
    ;

PrimaryExpr :
    ScopedName
    | Literal
    | '(' ConstExpr ')'
        {
            $$ = new GroupingExpression($2);
        }
    ;

Literal :
    INTEGER_LITERAL
        {
            $$ = new Literal($1);
            free($1);
        }
    | BooleanLiteral
    | FloatLiteral
    | NULL_LITERAL
        {
            $$ = new Literal("null");
        }
    ;

BooleanLiteral :
    TRUE
        {
            $$ = new Literal("true");
        }
    | FALSE
        {
            $$ = new Literal("false");
        }
    ;

FloatLiteral :
    FLOATING_PT_LITERAL
        {
            $$ = new Literal($1);
            free($1);
        }
    | INFINITY
        {
            $$ = new Literal("Infinity");
        }
    | NAN
        {
            $$ = new Literal("NaN");
        }
    ;

AttributeOrOperation :
    STRINGIFIER StringifierAttributeOrOperation
        {
            if ($2)
            {
                Member* m = static_cast<Member*>($2);
                uint32_t attr = m->getAttr();
                attr |= Member::Stringifier;
                m->setAttr(attr);
            }
            /* TODO: else mark Interface::Stringifier */
            $$ = $2;
        }
    | Attribute
    | Operation
    ;

StringifierAttributeOrOperation :
    Attribute
    | OperationRest
    | ';'
        {
            $$ = 0;
        }
    ;

/* TODO: Support 'Inherit' */
Attribute :
    Inherit ReadOnly ATTRIBUTE Type IDENTIFIER GetRaises SetRaises /* backward compatibility */ ';'
        {
            Attribute* attr = new Attribute($5, $4, $2);
            attr->setGetRaises($6);
            attr->setSetRaises($7);
            getCurrent()->add(attr);
            $$ = attr;
        }
    ;

Inherit :
    /* empty */
        {
            $$ = false;
        }
    | INHERIT
        {
            $$ = true;
        }
    ;

ReadOnly :
    /* empty */
        {
            $$ = false;
        }
    | READONLY
        {
            $$ = true;
        }
    ;

GetRaises :     /* backward compatibility */
    /* empty */
        {
            $$ = 0;
        }
    | GETRAISES ExceptionList
        {
            $$ = $2;
        }
    ;

SetRaises :     /* backward compatibility */
    /* empty */
        {
            $$ = 0;
        }
    | SETRAISES ExceptionList
        {
            $$ = $2;
        }
    ;

Operation :
    Qualifiers OperationRest
        {
            OpDcl* op = static_cast<OpDcl*>($2);
            if (op)
            {
                op->setLocation(&@1, &@2);
                uint32_t attr = op->getAttr();
                /* TODO: Check attr is valid */
                if (($1 & OpDcl::IndexGetter) || ($1 & OpDcl::IndexDeleter))
                {
                    op->check(op->getParamCount() == 1,
                              "Getters and deleters MUST be declared to take a single argument.");
                }
                if (($1 & OpDcl::IndexCreator) || ($1 & OpDcl::IndexSetter))
                {
                    op->check(op->getParamCount() == 2,
                              "Setters and creators MUST be declared to take two arguments.");
                }
                if ($1 & OpDcl::IndexMask)
                {
                    Node* spec = dynamic_cast<ParamDcl*>(*(op->begin()))->getSpec();
                    op->check(spec->getName() == "unsigned long" || spec->getName() == "string",
                              "The first argument MUST be an unsigned long or a DOMString.");
                }
                attr |= $1;
                op->setAttr(attr);
            }
            $$ = $2;
        }
    ;

Qualifiers :
    STATIC
        {
            $$ = OpDcl::Static;
        }
    | OMITTABLE Specials
        {
            $$ = OpDcl::Omittable | $2;
        }
    | Specials
    ;

Specials :
    /* empty */
        {
            $$ = 0;
        }
    | Specials Special  /* Use left recursion */
        {
            $$ = $1 | $2;
        }
    ;

Special :
    GETTER
        {
            $$ = OpDcl::IndexGetter;
        }
    | SETTER
        {
            $$ = OpDcl::IndexSetter;
        }
    | CREATOR
        {
            $$ = OpDcl::IndexCreator;
        }
    | DELETER
        {
            $$ = OpDcl::IndexDeleter;
        }
    | CALLER
        {
            $$ = OpDcl::Caller;
        }
    ;

OperationRest :
    ReturnType OptionalIdentifier
        {
            OpDcl* op = new OpDcl($2 ? $2 : "", $1);
            getCurrent()->add(op);
            setCurrent(op);
            free($2);
        }
    '(' ArgumentList ')' Raises ';'
        {
            OpDcl* op = static_cast<OpDcl*>(getCurrent());
            op->setRaises($7);
            setCurrent(op->getParent());
            $$ = op;
        }
    ;

OptionalIdentifier :
    /* empty */
        {
            $$ = 0;
        }
    | IDENTIFIER
    ;

Raises :    /* backward compatibility */
    /* empty */
        {
            $$ = 0;
        }
    | RAISES ExceptionList
        {
            $$ = $2;
        }
    ;

ExceptionList :
    '(' ScopedNameList ')'
        {
            $$ = $2;
        }
    ;

ArgumentList :
    /* empty */
    | Arguments
    ;

Arguments :
    Argument
    | Arguments ',' Argument   /* Use left recursion */
    ;

Argument :
    ExtendedAttributeList In OptionalOrRequiredArgument
        {
            if ($3)
                $3->setExtendedAttributes($1);
        }
    ;

In :    /* backward compatibility */
    /* empty */
    | IN
    ;

OptionalOrRequiredArgument:
    OPTIONAL Type ArgumentName Default
        {
            ParamDcl* param = new ParamDcl($3, $2, ParamDcl::Optional | ParamDcl::AttrIn);
            getCurrent()->add(param);
            /* TODO: process Default */
            free($3);
            $$ = param;
        }
    | Type Ellipsis ArgumentName
        {
            ParamDcl* param = new ParamDcl($3, $1, ParamDcl::AttrIn | $2);
            getCurrent()->add(param);
            free($3);
            $$ = param;
        }
    ;

ArgumentName:
    ArgumentNameKeyword
    | IDENTIFIER
    ;

Ellipsis :
    /* empty */
        {
            $$ = 0;
        }
    | ELLIPSIS
        {
            $$ = ParamDcl::Variadic;
        }
    ;

ExceptionMember :
    Const
    | ExceptionField
    ;

ExceptionField :
    Type IDENTIFIER ';'
        {
            Member* m = new Member($2);
            free($2);
            m->setSpec($1);
            getCurrent()->add(m);
            $$ = m;
        }
    ;

ExtendedAttributeList :
    /* empty */
        {
            $$ = 0;
        }
    |
        {
            pushJavadoc();
        }
    '[' ExtendedAttributes ']'
        {
            $$ = $3;
            popJavadoc();
        }
    ;

ExtendedAttributes :
    JavaDoc
        {
            setJavadoc($1);
            free($1);
        }
    ExtendedAttribute
        {
            $$ = new NodeList;
            $$->push_back($3);
        }
    | ExtendedAttributes ',' JavaDoc  /* Use left recursion */
        {
            setJavadoc($3);
            free($3);
        }
    ExtendedAttribute
        {
            assert($1);
            $$ = $1;
            $$->push_back($5);
        }
    ;

ExtendedAttribute :
    ExtendedAttributeNoArg
        {
            $$->setLocation(&@1);
        }
    | ExtendedAttributeArgList
        {
            $$->setLocation(&@1);
        }
    | ExtendedAttributeNamedArgList
        {
            $$->setLocation(&@1);
        }
    | ExtendedAttributeScopedName
        {
            $$->setLocation(&@1);
        }
    ;

ArgumentNameKeyword:
    ATTRIBUTE
        {
            $$ = strdup("attribute");
        }
    | CALLBACK
        {
            $$ = strdup("callback");
        }
    | CONST
        {
            $$ = strdup("const");
        }
    | CREATOR
        {
            $$ = strdup("creator");
        }
    | DELETER
        {
            $$ = strdup("deleter");
        }
    | DICTIONARY
        {
            $$ = strdup("dictionary");
        }
    | ENUM
        {
            $$ = strdup("enum");
        }
    | EXCEPTION
        {
            $$ = strdup("exception");
        }
    | GETTER
        {
            $$ = strdup("getter");
        }
    | IMPLEMENTS
        {
            $$ = strdup("implements");
        }
    | INHERIT
        {
            $$ = strdup("inherit");
        }
    | INTERFACE
        {
            $$ = strdup("interface");
        }
    | CALLER
        {
            $$ = strdup("legacycaller");
        }
    | PARTIAL
        {
            $$ = strdup("partial");
        }
    | SETTER
        {
            $$ = strdup("setter");
        }
    | STATIC
        {
            $$ = strdup("static");
        }
    | STRINGIFIER
        {
            $$ = strdup("stringifier");
        }
    | TYPEDEF
        {
            $$ = strdup("typedef");
        }
    | UNRESTRICTED
        {
            $$ = strdup("unrestricted");
        }
    ;

Type :
    SingleType
    | UnionType Null TypeSuffix
        {
            $1->setAttr($1->getAttr() | $2);
            if ($3)
            {
                static_cast<ArrayType*>($3)->setSpec($1);
                $$ = $3;
                $$->setParent(getCurrent());
            }
            else
            {
                $$ = $1;
            }
        }
    ;

SingleType :
    NonAnyType
    | ANY TypeSuffix
        {
            $$ = new Type("any");
            if ($2)
            {
                static_cast<ArrayType*>($2)->setSpec($$);
                $$ = $2;
                $$->setParent(getCurrent());
            }
            $$->setLocation(&@1);
        }
    ;

/* TODO: Support 'UnionType' */
UnionType :
    '(' UnionMemberType OR UnionMemberType UnionMemberTypes ')'
    ;

UnionMemberType :
    NonAnyType
    | UnionType Null TypeSuffix
    | ANY '[' ']' Null TypeSuffix
    ;

UnionMemberTypes :
    /* empty */
    | OR UnionMemberType UnionMemberTypes
    ;

NonAnyType  :
    PrimitiveType Null TypeSuffix
        {
            $1->setAttr($1->getAttr() | $2);
            if ($3)
            {
                static_cast<ArrayType*>($3)->setSpec($1);
                $$ = $3;
                $$->setParent(getCurrent());
            }
            else
            {
                $$ = $1;
            }
        }
    | STRING Null TypeSuffix
        {
            $$ = new Type("string");
            $$->setAttr($$->getAttr() | $2);
            if ($3)
            {
                static_cast<ArrayType*>($3)->setSpec($$);
                $$ = $3;
                $$->setParent(getCurrent());
            }
            $$->setLocation(&@1);
        }
    | ScopedName Null TypeSuffix     /* in esidl, "object" is treatetd as one of ScopedNames. */
        {
            $1->setAttr($1->getAttr() | $2);
            if ($3)
            {
                static_cast<ArrayType*>($3)->setSpec($1);
                $$ = $3;
                $$->setParent(getCurrent());
            }
            else
            {
                $$ = $1;
            }
        }
    | SEQUENCE '<' Type '>' Null
        {
            $$ = new SequenceType($3);
            $$->setAttr($$->getAttr() | $5);
            $$->setParent(getCurrent());
            $$->setLocation(&@1, &@4);
        }
    | SEQUENCE '<' Type ',' positive_int_const '>' Null   /* esidl extension */
        {
            $$ = new SequenceType($3, $5);
            $$->setAttr($$->getAttr() | $7);
            $$->setParent(getCurrent());
            $$->setLocation(&@1, &@6);
        }
    | DATE TypeSuffix
        {
            $$ = new Type("Date");
            if ($2)
            {
                static_cast<ArrayType*>($2)->setSpec($$);
                $$ = $2;
                $$->setParent(getCurrent());
            }
            $$->setLocation(&@1);
        }
    ;

ConstType :
    PrimitiveType Null
        {
            $1->setAttr($1->getAttr() | $2);
            $$ = $1;
        }
    | ScopedName Null   /* esidl extension */
        {
            $1->setAttr($1->getAttr() | $2);
            $$ = $1;
        }
    ;

PrimitiveType :
    UnsignedIntegerType
    | UnrestrictedFloatType
    | BOOLEAN
        {
            $$ = new Type("boolean");
            $$->setLocation(&@1);
        }
    | BYTE
        {
            $$ = new Type("byte");
            $$->setLocation(&@1);
        }
    | OCTET
        {
            $$ = new Type("octet");
            $$->setLocation(&@1);
        }
    ;

UnrestrictedFloatType:
    UNRESTRICTED FloatType
        {
            /* Support 'unrestricted'. */
            $2->getName() = "unrestricted " + $2->getName();
            $$ = $2;
            $$->setLocation(&@1, &@2);
        }
    | FloatType
    ;

FloatType :
    FLOAT
        {
            $$ = new Type("float");
            $$->setLocation(&@1);
        }
    | DOUBLE
        {
            $$ = new Type("double");
            $$->setLocation(&@1);
        }
    ;

UnsignedIntegerType :
    IntegerType
    | UNSIGNED IntegerType
        {
            $2->getName() = "unsigned " + $2->getName();
            $$ = $2;
            $$->setLocation(&@1, &@2);
        }
    ;

IntegerType :
    SHORT
        {
            $$ = new Type("short");
            $$->setLocation(&@1);
        }
    | LONG OptionalLong
        {
            $$ = new Type($2 ? "long long" : "long");
            $$->setLocation(&@1);
        }
    ;

OptionalLong :
    /* empty */
        {
            $$ = false;
        }
    | LONG
        {
            $$ = true;
        }
    ;

TypeSuffix :
    /* empty */
        {
            $$ = 0;
        }
    | '[' ']' Null TypeSuffix
        {
            ArrayType* array = new ArrayType;
            array->setSpec($4);
            $$ = array;
            $$->setAttr($$->getAttr() | $3);
        }
    | '[' positive_int_const ']' Null TypeSuffix  /* Note positive_int_const is an esidl extension. */
        {
            ArrayType* array = new ArrayType($2);
            array->setSpec($5);
            $$ = array;
            $$->setAttr($$->getAttr() | $4);
        }
    ;

Null :
    /* empty */
        {
            $$ = 0;
        }
    | '?'
        {
            $$ = Node::Nullable;
        }
    ;

ReturnType :
    Type
    | VOID
        {
            $$ = new Type("void");
            $$->setLocation(&@1);
        }
    ;

ScopedNameList :
    ScopedName
        {
            $$ = new Node();
            $$->add($1);
        }
    | ScopedNameList ',' ScopedName  /* Use left recursion */
        {
            assert($1);
            $$ = $1;
            $$->add($3);
        }
    ;

ScopedName :
    AbsoluteScopedName
        {
            $$->setLocation(&@1);
        }
    | RelativeScopedName
        {
            $$->setLocation(&@1);
        }
    ;

AbsoluteScopedName :
    OP_SCOPE IDENTIFIER ScopedNameParts
        {
            if (!Node::getFlatNamespace())
            {
                if (!$3)
                {
                    ScopedName* name = new ScopedName("::");
                    name->getName() += $2;
                    $$ = name;
                }
                else
                {
                    ScopedName* name = static_cast<ScopedName*>($3);
                    name->getName() = $2 + name->getName();
                    name->getName() = "::" + name->getName();
                    $$ = name;
                }
            }
            else
            {
                if ($3)
                {
                    $$ = $3;
                }
                else
                {
                    $$ = new ScopedName($2);
                }
            }
            free($2);
        }
    ;

RelativeScopedName :
    IDENTIFIER ScopedNameParts
        {
            if (!Node::getFlatNamespace())
            {
                if (!$2)
                {
                    $$ = new ScopedName($1);
                }
                else
                {
                    ScopedName* name = static_cast<ScopedName*>($2);
                    name->getName() = $1 + name->getName();
                    $$ = name;
                }
            }
            else
            {
                if ($2)
                {
                    $$ = $2;
                }
                else
                {
                    $$ = new ScopedName($1);
                }
            }
            free($1);
        }
    ;

ScopedNameParts :
    /* empty */
        {
            $$ = 0;
        }
    | ScopedNamePartList
    ;

ScopedNamePartList :
    OP_SCOPE IDENTIFIER
        {
            if (!Node::getFlatNamespace())
            {
                ScopedName* name = new ScopedName("::");
                name->getName() += $2;
                $$ = name;
            }
            else
            {
                $$ = new ScopedName($2);
            }
            free($2);
        }
    | ScopedNamePartList OP_SCOPE IDENTIFIER  /* Use left recursion */
        {
            if (!Node::getFlatNamespace())
            {
                ScopedName* name = static_cast<ScopedName*>($1);
                name->getName() += "::";
                name->getName() += $3;
                $$ = name;
            }
            else
            {
                $$ = new ScopedName($3);
            }
            free($3);
        }
    ;

ExtendedAttributeNoArg :
    IDENTIFIER
        {
            $$ = new ExtendedAttribute($1, 0);
            free($1);
            setJavadoc(0);
        }
    ;

ExtendedAttributeArgList :
    IDENTIFIER
        {
            $<node>$ = getCurrent();
            OpDcl* op = new OpDcl("", 0);
            // We need to set parent for op here. Otherwise, some names cannot be resolved.
            op->setParent(getCurrent());
            setCurrent(op);
        }
    '(' ArgumentList ')'
        {
            $$ = new ExtendedAttribute($1, getCurrent());
            setCurrent($<node>2);
            free($1);
            setJavadoc(0);
        }
    ;

ExtendedAttributeNamedArgList :
    IDENTIFIER '=' IDENTIFIER
        {
            $<node>$ = getCurrent();
            OpDcl* op = new OpDcl($3, 0);
            // We need to set parent for op here. Otherwise, some names cannot be resolved.
            op->setParent(getCurrent());
            setCurrent(op);
            free($3);
        }
    '(' ArgumentList ')'
        {
            $$ = new ExtendedAttribute($1, getCurrent());
            setCurrent($<node>4);
            free($1);
            setJavadoc(0);
        }
    ;

ExtendedAttributeScopedName :
    IDENTIFIER '=' ScopedName
        {
            $$ = new ExtendedAttribute($1, $3);
            free($1);
            setJavadoc(0);
        }
    ;

/* ES extensions */

/* Note the forward declaration is dropped in Web IDL */
ForwardDcl :
    INTERFACE IDENTIFIER ';'
        {
            Interface* node = new Interface($2, 0, true);
            getCurrent()->add(node);
            free($2);
            $$ = node;
        }
    ;

positive_int_const :
    ConstExpr
    ;

Preprocessor :
    POUND_SIGN INTEGER_LITERAL STRING_LITERAL INTEGER_LITERAL EOL
        {
            // # LINENUM FILENAME FLAGS
            // FLAGS: 1) new file 2) return
            switch (atoi($4))
            {
            case 1: // New file
                getCurrent()->add(new Include($3));
                setFilename($3);
                break;
            case 2: // Return
                setFilename($3);
                break;
            }
            if (strcmp($3, "\"<stdin>\"") == 0)
            {
                stdinLine = atol($2);
                yylloc.last_line = stdinLine - stdinOffset;
            }
            else
            {
                yylloc.last_line = atol($2);
            }
            free($2);
            free($3);
            free($4);
            $$ = 0;
        }
    | POUND_SIGN INTEGER_LITERAL STRING_LITERAL INTEGER_LITERAL INTEGER_LITERAL INTEGER_LITERAL EOL
        {
            // # LINENUM FILENAME FLAGS
            // FLAGS: 1) new file 2) return
            switch (atoi($4))
            {
            case 1: // New file
                getCurrent()->add(new Include($3, strcmp("3", $5) == 0));
                setFilename($3);
                break;
            case 2: // Return
                setFilename($3);
                break;
            }
            if (strcmp($3, "\"<stdin>\"") == 0)
            {
                stdinLine = atol($2);
                yylloc.last_line = stdinLine - stdinOffset;
            }
            else
            {
                yylloc.last_line = atol($2);
            }
            free($2);
            free($3);
            free($4);
            $$ = 0;
        }
    | POUND_SIGN INTEGER_LITERAL STRING_LITERAL EOL
        {
            // # LINENUM FILENAME
            if (strcmp("1", $2) == 0)
            {
                setFilename($3);
            }
            if (strcmp($3, "\"<stdin>\"") == 0)
            {
                stdinLine = atol($2);
                yylloc.last_line = stdinLine - stdinOffset;
            }
            else
            {
                yylloc.last_line = atol($2);
            }
            free($2);
            free($3);
            $$ = 0;
        }
    | POUND_SIGN IDENTIFIER IDENTIFIER STRING_LITERAL EOL
        {
            if (strcmp($2, "pragma") == 0 && strcmp($3, "source") == 0)
            {
                setBaseFilename($4);
            }
            free($2);
            free($3);
            free($4);
            $$ = 0;
            stdinOffset = stdinLine;
            yylloc.last_line = 1;
        }
    ;

JavaDoc :
    /* empty */
        {
            $$ = 0;
        }
    | JAVADOC
    ;
