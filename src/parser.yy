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

%type <node>        Definition
%type <node>        Module
%type <node>        Interface
%type <node>        Exception
%type <node>        Typedef
/* %type <node>        ImplementsStatement */
%type <node>        Preprocessor
%type <node>        Const

%type <node>        InterfaceDcl
%type <node>        ForwardDcl

%type <node>        InterfaceMember
%type <node>        Attribute
%type <node>        readonly_attr_spec
%type <node>        attr_spec
%type <node>        Operation

%type <node>        ExceptionMember
%type <node>        ExceptionField

%type <node>        InterfaceInheritance
%type <node>        ScopedNameList
%type <node>        ScopedName
%type <node>        const_type
%type <node>        declarator

%type <node>        type_spec
%type <node>        base_type_spec
%type <node>        template_type_spec

%type <node>        floating_pt_type
%type <node>        integer_type
%type <node>        signed_int
%type <node>        signed_short_int
%type <node>        signed_long_int
%type <node>        signed_longlong_int
%type <node>        unsigned_int
%type <node>        unsigned_short_int
%type <node>        unsigned_long_int
%type <node>        unsigned_longlong_int
%type <node>        boolean_type
%type <node>        octet_type
%type <node>        any_type
%type <node>        sequence_type
%type <node>        string_type
%type <node>        array_declarator
%type <node>        fixed_array_size_list
%type <node>        fixed_array_size
%type <node>        const_exp
%type <node>        or_expr
%type <node>        xor_expr
%type <node>        and_expr
%type <node>        shift_expr
%type <node>        add_expr
%type <node>        mult_expr
%type <node>        unary_expr
%type <node>        primary_expr
%type <node>        literal
%type <node>        boolean_literal
%type <node>        positive_int_const

%type <node>        op_type_spec
%type <node>        raises_expr_opt
%type <node>        raises_expr

%type <node>        get_excep_expr
%type <node>        set_excep_expr
%type <node>        exception_list

%type <nodeList>    ExtendedAttributeList
%type <nodeList>    extended_attribute_list
%type <nodeList>    extended_attributes
%type <node>        extended_attribute
%type <node>        extended_attribute_details

%type <name>        unary_operator

%type <attr>        param_attribute

%token <name>       JAVADOC
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

Const :
    CONST const_type IDENTIFIER '=' const_exp ';'
        {
            ConstDcl* constDcl = new ConstDcl($2, $3, $5);
            getCurrent()->add(constDcl);
            $$ = constDcl;
        }
    ;

const_type :
    integer_type
    | boolean_type
    | floating_pt_type
    | ScopedName
    | octet_type
    ;

const_exp :
    or_expr
    ;

or_expr :
    xor_expr
    | or_expr '|' xor_expr
        {
            $$ = new BinaryExpr("|", $1, $3);
        }
    ;

xor_expr :
    and_expr
    | xor_expr '^' and_expr
        {
            $$ = new BinaryExpr("^", $1, $3);
        }
    ;

and_expr :
    shift_expr
    | and_expr '&' shift_expr
        {
            $$ = new BinaryExpr("&", $1, $3);
        }
    ;

shift_expr :
    add_expr
    | shift_expr OP_SHR add_expr
        {
            $$ = new BinaryExpr(">>", $1, $3);
        }
    | shift_expr OP_SHL add_expr
        {
            $$ = new BinaryExpr("<<", $1, $3);
        }
    ;

add_expr :
    mult_expr
    | add_expr '+' mult_expr
        {
            $$ = new BinaryExpr("+", $1, $3);
        }
    | add_expr '-' mult_expr
        {
            $$ = new BinaryExpr("-", $1, $3);
        }
    ;

mult_expr :
    unary_expr
    | mult_expr '*' unary_expr
        {
            $$ = new BinaryExpr("*", $1, $3);
        }
    | mult_expr '/' unary_expr
        {
            $$ = new BinaryExpr("/", $1, $3);
        }
    | mult_expr '%' unary_expr
        {
            $$ = new BinaryExpr("%", $1, $3);
        }
    ;

unary_expr :
    unary_operator primary_expr
        {
            $$ = new UnaryExpr($1, $2);
        }
    | primary_expr
    ;

unary_operator :
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

primary_expr :
    ScopedName
    | literal
    | '(' const_exp ')'
        {
            $$ = new GroupingExpression($2);
        }
    ;

literal :
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
    | boolean_literal
    ;

boolean_literal :
    TRUE
        {
            $$ = new Literal("TRUE");
        }
    | FALSE
        {
            $$ = new Literal("FALSE");
        }
    ;

positive_int_const :
    const_exp
    ;

Typedef :
    TYPEDEF type_spec declarator ';'
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
    | NATIVE IDENTIFIER ';'
        {
            NativeType* nativeType = new NativeType($2);
            getCurrent()->add(nativeType);
            $$ = nativeType;
        }
    ;

type_spec :
    base_type_spec
    | template_type_spec
    | ScopedName
    ;

base_type_spec :
    floating_pt_type
    | integer_type
    | boolean_type
    | octet_type
    | any_type
    ;

template_type_spec :
    sequence_type
    | string_type
    ;

declarator :
    IDENTIFIER
        {
            $$ = new Member($1);
            free($1);
        }
    | array_declarator
    ;

floating_pt_type :
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

boolean_type :
    BOOLEAN
        {
            $$ = new Type("boolean");
        }
    ;

octet_type :
    OCTET
        {
            $$ = new Type("octet");
        }
    ;

any_type :
    ANY
        {
            $$ = new Type("any");
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

ExceptionMember :
    Const
    | ExceptionField
    ;

ExceptionField :
    type_spec IDENTIFIER ';'
        {
            Member* m = new Member($2);
            free($2);
            m->setSpec($1);
            getCurrent()->add(m);
            $$ = m;
        }
    ;

sequence_type :
    SEQUENCE '<' type_spec ',' positive_int_const '>'
        {
            $$ = new SequenceType($3, $5);
            $$->setParent(getCurrent());
        }
    | SEQUENCE '<' type_spec '>'
        {
            $$ = new SequenceType($3);
            $$->setParent(getCurrent());
        }
    ;

string_type :
    STRING
        {
            $$ = new Type("string");
        }
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

Attribute :
    readonly_attr_spec
    | attr_spec
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

Operation :
    op_type_spec IDENTIFIER
        {
            OpDcl* op = new OpDcl($2, $1);
            getCurrent()->add(op);
            setCurrent(op);
            free($2);
        }
    parameter_dcls raises_expr_opt ';'
        {
            OpDcl* op = static_cast<OpDcl*>(getCurrent());
            op->setRaises($5);
            setCurrent(op->getParent());
            $$ = op;
        }
    ;

op_type_spec :
    type_spec
    | VOID
        {
            $$ = new Type("void");
        }
    ;

parameter_dcls :
    '(' param_dcl_list ')'
    | '(' ')'
    ;

param_dcl_list :
    param_dcl
    | param_dcl_list ',' param_dcl
    ;

param_dcl :
    ExtendedAttributeList param_attribute type_spec IDENTIFIER
        {
            ParamDcl* param = new ParamDcl($4, $3, $2);
            param->setExtendedAttributes($1);
            getCurrent()->add(param);
            free($4);
        }
    ;

param_attribute :
    /* empty */
        {
            $$ = ParamDcl::AttrIn;
        }
    | IN
        {
            $$ = ParamDcl::AttrIn;
        }
    ;

raises_expr_opt :
    /* empty */
        {
            $$ = 0;
        }
    | raises_expr
    ;

raises_expr :
    RAISES '(' ScopedNameList ')'
        {
            $$ = $3;
        }
    ;

readonly_attr_spec :
    READONLY ATTRIBUTE type_spec IDENTIFIER get_excep_expr set_excep_expr ';'
        {
            Attribute* attr = new Attribute($4, $3, true);
            attr->setGetRaises($5);
            attr->setSetRaises($6);
            getCurrent()->add(attr);
            $$ = attr;
        }
    ;

attr_spec :
    ATTRIBUTE type_spec IDENTIFIER get_excep_expr set_excep_expr ';'
        {
            Attribute* attr = new Attribute($3, $2);
            attr->setGetRaises($4);
            attr->setSetRaises($5);
            getCurrent()->add(attr);
            $$ = attr;
        }
    ;

get_excep_expr :
    /* empty */
        {
            $$ = 0;
        }
    | GETRAISES exception_list
        {
            $$ = $2;
        }
    ;

set_excep_expr :
    /* empty */
        {
            $$ = 0;
        }
    | SETRAISES exception_list
        {
            $$ = $2;
        }
    ;

exception_list :
    '(' ScopedNameList ')'
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

ExtendedAttributeList :
    /* empty */
        {
            $$ = 0;
        }
    | extended_attribute_list
    ;

extended_attribute_list :
        {
            pushJavadoc();
        }
    '[' extended_attributes ']'
        {
            popJavadoc();
            $$ = $3;
        }
    ;

extended_attributes :
    extended_attribute
        {
            $$ = new NodeList;
            $$->push_back($1);
        }
    | extended_attributes ',' extended_attribute
        {
            $1->push_back($3);
            $$ = $1;
        }
    ;

extended_attribute :
    JavaDoc
        {
            setJavadoc($1);
            free($1);
        }
    IDENTIFIER extended_attribute_details
        {
            $$ = new ExtendedAttribute($3, $4);
            free($3);
            setJavadoc(0);
        }
    ;

extended_attribute_details :
    /* empty */
        {
            $$ = 0;
        }
    | '=' ScopedName
        {
            $$ = $2;
        }
    | '=' IDENTIFIER
        {
            $<node>$ = getCurrent();
            OpDcl* op = new OpDcl($2, 0);
            // We need to set parent for op here. Otherwise, some names cannot be resolved.
            op->setParent(getCurrent());
            setCurrent(op);
            free($2);
        }
    parameter_dcls
        {
            $$ = getCurrent();
            setCurrent($<node>3);
        }
    |
        {
            $<node>$ = getCurrent();
            OpDcl* op = new OpDcl("", 0);
            // We need to set parent for op here. Otherwise, some names cannot be resolved.
            op->setParent(getCurrent());
            setCurrent(op);
        }
    parameter_dcls
        {
            $$ = getCurrent();
            setCurrent($<node>1);
        }
    ;
