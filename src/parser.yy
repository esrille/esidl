/*
 * Copyright 2008, 2009 Google Inc.
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
 * W3C Editor’s Draft 10 July 2009.
 * http://dev.w3.org/2006/webapi/WebIDL/
 *
 * W3C,
 * Web IDL,
 * W3C Working Draft 19 December 2008.
 * http://www.w3.org/TR/WebIDL/
 *
 * Common Object Request Broker Architecture: Core Specification,
 * Version 3.0.3, Object Management Group, Inc., March 2004.
 * http://www.omg.org/technology/documents/formal/corba_iiop.htm.
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
%token CONST
%token DOUBLE
%token EOL
%token EXCEPTION
%token FALSE
%token FLOAT
%token GETRAISES
%token IN
%token INTERFACE
%token LONG
%token MODULE
%token MULTIPLE
%token NATIVE
%token OBJECT
%token OCTET
%token ONEWAY
%token RAISES
%token READONLY
%token SETRAISES
%token SEQUENCE
%token SHORT
%token STRING
%token TRUE
%token TYPEDEF
%token UNSIGNED
%token VALUETYPE
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
%type <node>        Interface
%type <node>        InterfaceInheritance
%type <node>        InterfaceMember
%type <node>        Exception
%type <node>        Typedef
/* %type <node>        ImplementsStatement */
%type <node>        Const
%type <node>        ConstExpr
%type <node>        Literal
%type <node>        BooleanLiteral
%type <node>        Attribute
%type <attr>        ReadOnly
%type <node>        GetRaises
%type <node>        SetRaises
%type <node>        Operation
%type <node>        Raises
%type <node>        ExceptionList
%type <node>        ExceptionMember
%type <node>        ExceptionField
%type <nodeList>    ExtendedAttributeList
%type <nodeList>    ExtendedAttributes
%type <node>        Type
%type <node>        base_type_spec
%type <node>        FloatType
%type <node>        integer_type
%type <node>        signed_int
%type <node>        signed_short_int
%type <node>        signed_long_int
%type <node>        signed_longlong_int
%type <node>        unsigned_int
%type <node>        unsigned_short_int
%type <node>        unsigned_long_int
%type <node>        unsigned_longlong_int
%type <node>        BooleanType
%type <node>        OctetType
%type <node>        AnyType
%type <node>        SequenceType
%type <node>        StringType
%type <node>        ReturnType
%type <node>        ConstType
%type <node>        ScopedNameList
%type <node>        ScopedName
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
%type <node>        Declarator
%type <node>        array_declarator
%type <node>        fixed_array_size_list
%type <node>        fixed_array_size
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
    Module
    | InterfaceDcl
    | Exception
    | Typedef
    | Preprocessor
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

InterfaceDcl :
    Interface
    | ForwardDcl
    ;

/* Note the forward declaration is not supported in Web IDL */
ForwardDcl :
    INTERFACE IDENTIFIER ';'
        {
            Interface* node = new Interface($2, 0, true);
            getCurrent()->add(node);
            free($2);
            $$ = node;
        }
    ;

Interface :
    INTERFACE IDENTIFIER InterfaceInheritance
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
            $$ = getCurrent();
            setCurrent(getCurrent()->getParent());
            if (Node::getFlatNamespace() && getCurrent() == getSpecification())
            {
                setCurrent(dynamic_cast<Module*>(getSpecification()->search(Node::getFlatNamespace())));
            }
        }
    ;

InterfaceInheritance :
    /* empty */
        {
            $$ = 0;
        }
    | ':' ScopedNameList
        {
            $$ = $2;
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
    | Attribute
    | Operation
    | Preprocessor
    ;

Exception :
    EXCEPTION IDENTIFIER
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

Typedef :
    TYPEDEF Type Declarator ';'
        {
            Member* m = static_cast<Member*>($3);
            // In flat namespace mode, even a valid typedef can define a new type for the spec using the exactly same name.
            if (dynamic_cast<ArrayDcl*>(m) || !dynamic_cast<ScopedName*>($2) || m->getQualifiedName() != $2->getQualifiedName())
            {
                m->setSpec($2);
                m->setTypedef(true);
                getCurrent()->add(m);
            }
            $$ = m;
        }
    | NATIVE IDENTIFIER ';'  /* Note 'native' is not supported in Web IDL */
        {
            NativeType* nativeType = new NativeType($2);
            getCurrent()->add(nativeType);
            $$ = nativeType;
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

/* Note in Web IDL, ConstExpr is just a literal.
 * The support for the expressions are the ES extensions.
 */
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
    | FLOATING_PT_LITERAL
        {
            $$ = new Literal($1);
            free($1);
        }
    | BooleanLiteral
    ;

BooleanLiteral :
    TRUE
        {
            $$ = new Literal("TRUE");
        }
    | FALSE
        {
            $$ = new Literal("FALSE");
        }
    ;

Attribute :
    ReadOnly ATTRIBUTE Type IDENTIFIER GetRaises SetRaises ';'
        {
            Attribute* attr = new Attribute($4, $3, $1);
            attr->setGetRaises($5);
            attr->setSetRaises($6);
            getCurrent()->add(attr);
            $$ = attr;
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

GetRaises :
    /* empty */
        {
            $$ = 0;
        }
    | GETRAISES ExceptionList
        {
            $$ = $2;
        }
    ;

SetRaises :
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
    ReturnType IDENTIFIER
        {
            OpDcl* op = new OpDcl($2, $1);
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

Raises :
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
    | Argument Arguments
    ;

Arguments :
    /* empty */
    | ',' Argument Arguments
    ;

Argument :
    ExtendedAttributeList In Type IDENTIFIER
        {
            ParamDcl* param = new ParamDcl($4, $3, ParamDcl::AttrIn);
            param->setExtendedAttributes($1);
            getCurrent()->add(param);
            free($4);
        }
    ;

In :
    /* empty */
    | IN
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
    '[' JavaDoc
        {
            setJavadoc($3);
            free($3);
        }
    ExtendedAttribute ExtendedAttributes ']'
        {
            popJavadoc();
            if ($6)
            {
                $$ = $6;
            }
            else
            {
                $$ = new NodeList;
            }
            $$->push_front($5);
        }
    ;

ExtendedAttributes :
    /* empty */
        {
            $$ = 0;
        }
    | ',' JavaDoc
        {
            setJavadoc($2);
            free($2);
        }
    ExtendedAttribute ExtendedAttributes
        {
            if ($5)
            {
                $$ = $5;
            }
            else
            {
                $$ = new NodeList;
            }
            $$->push_front($4);
        }
    ;

ExtendedAttribute :
    ExtendedAttributeNoArg
    | ExtendedAttributeArgList
    | ExtendedAttributeNamedArgList
    | ExtendedAttributeScopedName
    ;

Type :
    base_type_spec
    | ScopedName
    | AnyType
    ;

base_type_spec :
    integer_type
    | BooleanType
    | OctetType
    | FloatType
    | StringType
    | SequenceType
    ;

FloatType :
    FLOAT
        {
            $$ = new Type("float");
        }
    | DOUBLE
        {
            $$ = new Type("double");
        }
    ;

integer_type :
    signed_int
    | unsigned_int
    ;

signed_int :
    signed_short_int
    | signed_long_int
    | signed_longlong_int
    ;

signed_short_int :
    SHORT
        {
            $$ = new Type("short");
        }
    ;

signed_long_int :
    LONG
        {
            $$ = new Type("long");
        }
    ;

signed_longlong_int :
    LONG LONG
        {
            $$ = new Type("long long");
        }
    ;

unsigned_int :
    unsigned_short_int
    | unsigned_long_int
    | unsigned_longlong_int
    ;

unsigned_short_int :
    UNSIGNED SHORT
        {
            $$ = new Type("unsigned short");
        }
    ;

unsigned_long_int :
    UNSIGNED LONG
        {
            $$ = new Type("unsigned long");
        }
    ;

unsigned_longlong_int :
    UNSIGNED LONG LONG
        {
            $$ = new Type("unsigned long long");
        }
    ;

BooleanType :
    BOOLEAN
        {
            $$ = new Type("boolean");
        }
    ;

OctetType :
    OCTET
        {
            $$ = new Type("octet");
        }
    ;

AnyType :
    ANY
        {
            $$ = new Type("any");
        }
    ;

SequenceType :
    SEQUENCE '<' Type ',' positive_int_const '>'
        {
            $$ = new SequenceType($3, $5);
            $$->setParent(getCurrent());
        }
    | SEQUENCE '<' Type '>'
        {
            $$ = new SequenceType($3);
            $$->setParent(getCurrent());
        }
    ;

StringType :
    STRING
        {
            $$ = new Type("string");
        }
    ;

ReturnType :
    Type
    | VOID
        {
            $$ = new Type("void");
        }
    ;

/* ConstType will be added in the future spec. */
ConstType :
    integer_type
    | BooleanType
    | FloatType
    | ScopedName
    | OctetType
    ;

ScopedNameList :
    ScopedName
        {
            $$ = new Node();
            $$->add($1);
        }
    | ScopedName ',' ScopedNameList
        {
            $3->add($1);
            $$ = $3;
        }
    ;

ScopedName :
    IDENTIFIER
        {
            ScopedName* name = new ScopedName($1);
            free($1);
            $$ = name;
        }
    | OP_SCOPE IDENTIFIER
        {
            ScopedName* name;
            if (Node::getFlatNamespace())
            {
                name = new ScopedName($2);
            }
            else
            {
                name = new ScopedName("::");
                name->getName() += $2;
            }
            $$ = name;
            free($2);
        }
    | ScopedName OP_SCOPE IDENTIFIER
        {
            ScopedName* name = static_cast<ScopedName*>($1);
            if (Node::getFlatNamespace())
            {
                name->getName() = $3;
            }
            else
            {
                name->getName() += "::";
                name->getName() += $3;
            }
            $$ = name;
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

positive_int_const :
    ConstExpr
    ;

Declarator :
    IDENTIFIER
        {
            $$ = new Member($1);
            free($1);
        }
    | array_declarator
    ;

array_declarator :
    IDENTIFIER fixed_array_size_list
        {
            $2->getName() = $1;
            $$ = $2;
            free($1);
        }
    ;

fixed_array_size_list :
    fixed_array_size
        {
            $$ = new ArrayDcl;
            $$->add($1);
        }
    | fixed_array_size_list fixed_array_size
        {
            $1->add($2);
            $$ = $1;
        }
    ;

fixed_array_size :
    '[' positive_int_const ']'
        {
            $$ = $2;
        }
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
            yylloc.last_line = atol($2);
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
            yylloc.last_line = atol($2);
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
            yylloc.last_line = atol($2);
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
        }
    ;

JavaDoc :
    /* empty */
        {
            $$ = 0;
        }
    | JAVADOC
    ;
