/*
 * Copyright 2008 Google Inc.
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
 * W3C Working Draft 29 August 2008.
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

void yyerror(const char* message)
{
    printf("error: %s\n", message);
}

extern "C" int yywrap()
{
    return 1;
}

%}

%pure-parser

%union
{
    int                     attr;
    char*                   name;
    Node*                   node;
    NodeList*               nodeList;
    std::list<std::string>* nameList;
}

%{

int yylex(YYSTYPE* yylval);

%}

%token ANY
%token ATTRIBUTE
%token BOOLEAN
%token CASE
%token CHAR
%token COMPONENT
%token CONST
%token CONSUMES
%token DEFAULT
%token DOUBLE
%token EXCEPTION
%token EMITS
%token ENUM
%token EVENTTYPE
%token FALSE
%token FINDER
%token FIXED
%token FLOAT
%token GETRAISES
%token HOME
%token IN
%token INOUT
%token INTERFACE
%token LONG
%token MANAGES
%token MODULE
%token MULTIPLE
%token NATIVE
%token OBJECT
%token OCTET
%token ONEWAY
%token OUT
%token PRIMARYKEY
%token PROVIDES
%token PUBLISHES
%token RAISES
%token READONLY
%token SETRAISES
%token SEQUENCE
%token SHORT
%token STRING
%token STRUCT
%token SUPPORTS
%token SWITCH
%token TRUE
%token TYPEDEF
%token UNSIGNED
%token UNION
%token USES
%token UUID
%token VALUETYPE
%token VOID
%token WCHAR
%token WSTRING
%token VARIANT

%token OP_SCOPE
%token OP_SHL
%token OP_SHR

%token POUND_SIGN
%token PRAGMA_ID

%token <name>       IDENTIFIER
%token <name>       INTEGER_LITERAL
%token <name>       CHARACTER_LITERAL
%token <name>       WIDE_CHARACTER_LITERAL
%token <name>       FLOATING_PT_LITERAL
%token <name>       STRING_LITERAL
%token <name>       WIDE_STRING_LITERAL
%token <name>       FIXED_PT_LITERAL

%type <node>        interface_inheritance_spec
%type <node>        scoped_name_list
%type <node>        scoped_name
%type <node>        const_type
%type <node>        declarator
%type <node>        complex_declarator

%type <node>        type_spec
%type <node>        simple_type_spec
%type <node>        base_type_spec
%type <node>        template_type_spec
%type <node>        constr_type_spec

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
%type <node>        char_type
%type <node>        wide_char_type
%type <node>        boolean_type
%type <node>        octet_type
%type <node>        any_type
%type <node>        sequence_type
%type <node>        string_type
%type <node>        wide_string_type
%type <node>        variant_type
%type <node>        array_declarator
%type <node>        fixed_array_size_list
%type <node>        fixed_array_size
%type <node>        fixed_pt_const_type
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

%type <node>        object_type
%type <node>        op_type_spec
%type <node>        raises_expr_opt
%type <node>        raises_expr
%type <node>        param_type_spec

%type <node>        uuid_type

%type <nodeList>    extended_attribute_opt
%type <nodeList>    extended_attribute_list
%type <nodeList>    extended_attributes
%type <node>        extended_attribute

%type <name>        unary_operator
%type <name>        simple_declarator

%type <nameList>    simple_declarator_list

%type <attr>        param_attribute

%type <nodeList>    declarators

%token <name>       JAVADOC
%type <name>        javadoc

%%

specification :
    | definition_list
    ;

definition_list :
    javadoc
        {
            setJavadoc($1);
            free($1);
        }
    definition
        {
            setJavadoc(0);
        }
    | definition_list javadoc
        {
            setJavadoc($2);
            free($2);
        }
    definition
        {
            setJavadoc(0);
        }
    ;

definition :
    module ';'
    | interface ';'
    | except_dcl ';'
    | type_dcl ';'
    | value ';'
    | const_dcl ';'
    | preprocessor
    ;

module :
    extended_attribute_opt MODULE IDENTIFIER
        {
            Node* node = getCurrent()->search($3);
            if (node)
            {
                if (Module* module = dynamic_cast<Module*>(node))
                {
                    setCurrent(module);
                }
                else
                {
                    fprintf(stderr, "Inv. module name '%s'\n", $3);
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                Module* module = new Module($3);
                module->setExtendedAttributes($1);
                getCurrent()->add(module);
                setCurrent(module);
            }
            free($3);
        }
    '{' definition_list '}'
        {
            setCurrent(getCurrent()->getParent());
        }
    ;

interface :
    interface_dcl
    | forward_dcl
    ;

interface_dcl :
    interface_header '{' interface_body '}'
        {
            setCurrent(getCurrent()->getParent());
        }
    ;

forward_dcl :
    INTERFACE IDENTIFIER
        {
            Interface* node = new Interface($2, 0, true);
            getCurrent()->add(node);
            free($2);
        }
    ;

interface_header :
    INTERFACE IDENTIFIER interface_inheritance_spec
        {
            Interface* node = new Interface($2, $3);
            getCurrent()->add(node);
            setCurrent(node);
            free($2);
        }
    | INTERFACE IDENTIFIER
        {
            Interface* node = new Interface($2);
            getCurrent()->add(node);
            setCurrent(node);
            free($2);
        }
    | extended_attribute_list INTERFACE IDENTIFIER interface_inheritance_spec
        {
            Interface* node = new Interface($3, $4);
            node->setExtendedAttributes($1);
            getCurrent()->add(node);
            setCurrent(node);
            free($3);
        }
    | extended_attribute_list INTERFACE IDENTIFIER
        {
            Interface* node = new Interface($3);
            node->setExtendedAttributes($1);
            getCurrent()->add(node);
            setCurrent(node);
            free($3);
        }
    ;

interface_body :
    export_list_opt
    ;

export_list_opt :
    /* empty */
    | export_list
    ;

export_list :
    javadoc
        {
            setJavadoc($1);
            free($1);
        }
    export
        {
            setJavadoc(0);
        }
    | export_list javadoc
        {
            setJavadoc($2);
            free($2);
        }
    export
        {
            setJavadoc(0);
        }
    ;

export :
    type_dcl ';'
    | const_dcl ';'
    | except_dcl ';'
    | attr_dcl ';'
    | op_dcl ';'
    | preprocessor
    ;

interface_inheritance_spec :
    ':' scoped_name_list
        {
            $$ = $2;
        }
    ;

scoped_name_list :
    scoped_name
        {
            $$ = new Node();
            $$->add($1);
        }
    | scoped_name_list ',' scoped_name
        {
            $1->add($3);
            $$ = $1;
        }
    ;

scoped_name :
    IDENTIFIER
        {
            ScopedName* name = new ScopedName($1);
            free($1);
            assert(name->search(getCurrent()));
            $$ = name;
        }
    | OP_SCOPE IDENTIFIER
        {
            ScopedName* name = new ScopedName("::");
            name->getName() += $2;
            free($2);
            assert(name->search(getCurrent()));
            $$ = name;
        }
    | scoped_name OP_SCOPE IDENTIFIER
        {
            ScopedName* name = static_cast<ScopedName*>($1);
            name->getName() += "::";
            name->getName() += $3;
            free($3);
            assert(name->search(getCurrent()));
            $$ = name;
        }
    ;

value :
    value_box_dcl
    | value_forward_dcl
    ;

value_forward_dcl :
    VALUETYPE IDENTIFIER
    ;

value_box_dcl :
    VALUETYPE IDENTIFIER type_spec
    ;

const_dcl :
    CONST const_type IDENTIFIER '=' const_exp
        {
            getCurrent()->add(new ConstDcl($2, $3, $5));
        }
    ;

const_type :
    integer_type
    | char_type
    | wide_char_type
    | boolean_type
    | floating_pt_type
    | string_type
    | wide_string_type
    | fixed_pt_const_type
    | scoped_name
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
    scoped_name
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
    | STRING_LITERAL
        {
            $$ = new Literal($1);
            free($1);
        }
    | WIDE_STRING_LITERAL
        {
            $$ = new Literal($1);
            free($1);
        }
    | CHARACTER_LITERAL
        {
            $$ = new Literal($1);
            free($1);
        }
    | WIDE_CHARACTER_LITERAL
        {
            $$ = new Literal($1);
            free($1);
        }
    | FIXED_PT_LITERAL
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

type_dcl :
    TYPEDEF type_declarator
    | struct_type
    | union_type
    | enum_type
    | NATIVE simple_declarator
    | constr_forward_decl
    ;

type_declarator :
    type_spec declarators
        {
            for (NodeList::iterator i = $2->begin();
                 i != $2->end();
                 ++i)
            {
                assert(dynamic_cast<Member*>(*i));
                Member* m = static_cast<Member*>(*i);
                m->setSpec($1);
                m->setTypedef(true);
                getCurrent()->add(m);
            }
            delete $2;
        }
    ;

type_spec :
    simple_type_spec
    | constr_type_spec
    ;

simple_type_spec :
    base_type_spec
    | template_type_spec
    | scoped_name
    ;

base_type_spec :
    floating_pt_type
    | integer_type
    | char_type
    | wide_char_type
    | boolean_type
    | octet_type
    | any_type
    | object_type
    | variant_type
    ;

template_type_spec :
    sequence_type
    | string_type
    | wide_string_type
    | fixed_pt_type
    ;

constr_type_spec :
    struct_type
    | union_type
    | enum_type
    ;

declarators :
    declarator
        {
            $$ = new NodeList;
            $$->push_back($1);
        }
    | declarators ',' declarator
        {
            $1->push_back($3);
            $$ = $1;
        }
    ;

declarator :
    simple_declarator
        {
            $$ = new Member($1);
            free($1);
        }
    | complex_declarator
    ;

simple_declarator_list :
    simple_declarator
        {
            $$ = new std::list<std::string>;
            $$->push_back($1);
            free($1);
        }
    | simple_declarator_list ',' simple_declarator
        {
            $1->push_back($3);
            $$ = $1;
            free($3);
        }
    ;

simple_declarator :
    IDENTIFIER
    ;

complex_declarator :
    array_declarator
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
    | LONG DOUBLE
        {
            $$ = new Type("long double");
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

char_type :
    CHAR
        {
            $$ = new Type("char");
        }
    ;

wide_char_type :
    WCHAR
        {
            $$ = new Type("wchar");
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

object_type :
    OBJECT
        {
            $$ = new Type("Object");
        }
    ;

variant_type :
    VARIANT
        {
            $$ = new Type("Variant");
        }
    ;

struct_type :
    STRUCT IDENTIFIER
        {
            StructType* st = new StructType($2);
            getCurrent()->add(st);
            setCurrent(st);
            free($2);
        }
    '{' member_list '}'
        {
            setCurrent(getCurrent()->getParent());
        }
    ;

member_list_opt :
    /* empty */
    | member_list
    ;

member_list :
    member
    | member_list member
    ;

member :
    type_spec declarators ';'
        {
            for (NodeList::iterator i = $2->begin();
                 i != $2->end();
                 ++i)
            {
                assert(dynamic_cast<Member*>(*i));
                static_cast<Member*>(*i)->setSpec($1);
                getCurrent()->add(*i);
            }
            delete $2;
        }
    ;

union_type :
    UNION IDENTIFIER SWITCH '(' switch_type_spec ')' '{' switch_body '}'
    ;

switch_type_spec :
    integer_type
    | char_type
    | boolean_type
    | enum_type
    | scoped_name
    ;

switch_body :
    case
    | switch_body case
    ;

case :
    case_label_list element_spec ';'
    ;

case_label_list :
    case_label
    | case_label_list case_label
    ;

case_label :
    CASE const_exp ':'
    | DEFAULT ':'
    ;

element_spec :
    type_spec declarator
    ;

enum_type :
    ENUM IDENTIFIER
        {
            EnumType* en = new EnumType($2);
            getCurrent()->add(en);
            setCurrent(en);
            free($2);
        }
    '{' enumerator_list '}'
        {
            setCurrent(getCurrent()->getParent());
        }
    ;

enumerator_list :
    enumerator
    | enumerator_list ',' enumerator
    ;

enumerator :
    IDENTIFIER
        {
            getCurrent()->add(new Node($1));
            free($1);
        }
    ;

sequence_type :
    SEQUENCE '<' simple_type_spec ',' positive_int_const '>'
        {
            $$ = new SequenceType($3, $5);
        }
    | SEQUENCE '<' simple_type_spec '>'
        {
            $$ = new SequenceType($3);
        }
    ;

string_type :
    STRING '<' positive_int_const '>'
        {
            $$ = new Type("string");    // XXX
        }
    | STRING
        {
            $$ = new Type("string");
        }
    ;

wide_string_type :
    WSTRING '<' positive_int_const '>'
        {
            $$ = new Type("wstring");   // XXX
        }
    | WSTRING
        {
            $$ = new Type("wstring");
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

attr_dcl :
    readonly_attr_spec
    | attr_spec
    ;

except_dcl :
    EXCEPTION IDENTIFIER
        {
            ExceptDcl* exc = new ExceptDcl($2);
            getCurrent()->add(exc);
            setCurrent(exc);
            free($2);
        }
    '{' member_list_opt '}'
        {
            setCurrent(getCurrent()->getParent());
        }
    ;

op_dcl :
    op_attribute_opt op_type_spec IDENTIFIER
        {
            OpDcl* op = new OpDcl($3, $2);
            getCurrent()->add(op);
            setCurrent(op);
            free($3);
        }
    parameter_dcls raises_expr_opt
        {
            OpDcl* op = static_cast<OpDcl*>(getCurrent());
            op->setRaises($6);
            setCurrent(op->getParent());
        }
    | extended_attribute_list op_attribute_opt op_type_spec IDENTIFIER
        {
            OpDcl* op = new OpDcl($4, $3);
            op->setExtendedAttributes($1);
            getCurrent()->add(op);
            setCurrent(op);
            free($4);
        }
    parameter_dcls raises_expr_opt
        {
            OpDcl* op = static_cast<OpDcl*>(getCurrent());
            op->setRaises($7);
            setCurrent(op->getParent());
        }
    ;

op_attribute_opt :
    /* empty */
    | ONEWAY
    ;

op_type_spec :
    param_type_spec
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
    extended_attribute_opt param_attribute param_type_spec simple_declarator
        {
            ParamDcl* param = new ParamDcl($4, $3, $2);
            param->setExtendedAttributes($1);
            getCurrent()->add(param);
            free($4);
        }
    ;

param_attribute :
    IN
        {
            $$ = ParamDcl::In;
        }
    | OUT
        {
            $$ = ParamDcl::Out;
        }
    | INOUT
        {
            $$ = ParamDcl::InOut;
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
    RAISES '(' scoped_name_list ')'
        {
            $$ = $3;
        }
    ;

string_literal_list :
    STRING_LITERAL
    | string_literal_list ',' STRING_LITERAL
    ;

param_type_spec :
    base_type_spec
    | string_type
    | wide_string_type
    | scoped_name
    | uuid_type
    ;

fixed_pt_type :
    FIXED '<' positive_int_const ',' positive_int_const '>'
    ;

fixed_pt_const_type :
    FIXED
        {
            $$ = new Type("fixed");
        }
    ;

constr_forward_decl :
    STRUCT IDENTIFIER
        {
            getCurrent()->add(new StructType($2));
            free($2);
        }
    | UNION IDENTIFIER
    ;

readonly_attr_spec :
    extended_attribute_opt READONLY ATTRIBUTE param_type_spec simple_declarator raises_expr
        {
            Attribute* attr = new Attribute($5, $4, true);
            attr->setExtendedAttributes($1);
            getCurrent()->add(attr);
        }
    | extended_attribute_opt READONLY ATTRIBUTE param_type_spec simple_declarator_list
        {
            for (std::list<std::string>::iterator i = $5->begin();
                 i != $5->end();
                 ++i)
            {
                Attribute* attr = new Attribute(*i, $4, true);
                attr->setExtendedAttributes($1);
                getCurrent()->add(attr);
            }
            delete $5;
        }
    ;

attr_spec :
    extended_attribute_opt ATTRIBUTE param_type_spec simple_declarator attr_raises_expr
        {
            Attribute* attr = new Attribute($4, $3);
            attr->setExtendedAttributes($1);
            getCurrent()->add(attr);
        }
    | extended_attribute_opt ATTRIBUTE param_type_spec simple_declarator_list
        {
            for (std::list<std::string>::iterator i = $4->begin();
                 i != $4->end();
                 ++i)
            {
                Attribute* attr = new Attribute(*i, $3);
                attr->setExtendedAttributes($1);
                getCurrent()->add(attr);
            }
            delete $4;
        }
    ;

attr_raises_expr :
    get_excep_expr
    | get_excep_expr set_excep_expr
    | set_excep_expr
    ;

get_excep_expr :
    GETRAISES exception_list
    ;

set_excep_expr :
    SETRAISES exception_list
    ;

exception_list :
    '(' scoped_name_list ')'
    ;

preprocessor:
    POUND_SIGN INTEGER_LITERAL STRING_LITERAL INTEGER_LITERAL
        {
            // # LINENUM FILENAME FLAGS
            // FLAGS: 1) new file 2) return
            switch (atoi($4))
            {
            case 1: // New file
                getCurrent()->add(new Include($3));
                Node::incLevel();
                break;
            case 2: // Return
                Node::decLevel();
                break;
            }
            free($2);
            free($3);
            free($4);
        }
    | POUND_SIGN INTEGER_LITERAL STRING_LITERAL
        {
            // # LINENUM FILENAME
            if (strcmp("1", $2) == 0)
            {
                setFilename($3);
            }
            free($2);
            free($3);
        }
    | PRAGMA_ID scoped_name STRING_LITERAL
        {
            getCurrent()->add(new PragmaID($2, $3));
            free($3);
        }
    | PRAGMA_ID scoped_name '=' STRING_LITERAL ';'
        {
            getCurrent()->add(new PragmaID($2, $4));
            free($4);
        }
    ;

uuid_type:
    UUID
        {
            $$ = new Type("uuid");
        }
    ;

javadoc:
    /* empty */
        {
            $$ = 0;
        }
    | JAVADOC
    ;

extended_attribute_opt :
    /* empty */
        {
            $$ = 0;
        }
    | extended_attribute_list
    ;

extended_attribute_list :
    '[' extended_attributes ']'
        {
            $$ = $2;
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
    IDENTIFIER
        {
            $$ = new ExtendedAttribute($1);
        }
    | IDENTIFIER '=' IDENTIFIER
        {
            $$ = new ExtendedAttribute($1, $3);
        }
    ;
