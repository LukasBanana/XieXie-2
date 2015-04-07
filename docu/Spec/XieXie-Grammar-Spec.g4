
// XieXie 2.0 Grammar
// 08/06/2014

grammar XieXie;

//options { language = Cpp; }

program: global_stmnt_list EOF;

code_block:	'{' stmnt_list '}';

// STATEMENTS
stmnt_list:						stmnt*;
decl_stmnt_list: 				decl_stmnt*;
extern_decl_stmnt_list:			extern_decl_stmnt*;
global_stmnt_list:				global_stmnt*;
extern_proc_decl_stmnt_list:	extern_proc_decl_stmnt*;

stmnt	: var_name_stmnt
		| branch_stmnt
		| loop_stmnt
		| ctrl_transfer_stmnt;

global_stmnt	: class_decl_stmnt
				| module_decl_stmnt
				| import_stmnt;

var_name_stmnt	: var_decl_stmnt
				| assign_stmnt
				| expr_stmnt;

decl_stmnt	: var_decl_stmnt
			/*| class_decl_stmnt
			| enum_decl_stmnt
			| flags_decl_stmnt*/
			| proc_decl_stmnt
			| init_decl_stmnt
			| release_decl_stmnt;

extern_decl_stmnt	: /*extern_class_decl_stmnt
					|*/ extern_proc_decl_stmnt
					| extern_init_decl_stmnt
					/*| enum_decl_stmnt
					| flags_decl_stmnt*/;

branch_stmnt	: if_stmnt
				| switch_stmnt;

loop_stmnt	: for_stmnt
			| for_each_stmnt
			| for_range_stmnt
			| repeat_stmnt
			| while_stmnt
			| do_while_stmnt;

ctrl_transfer_stmnt	: break_stmnt
					| continue_stmnt
					| return_stmnt;

assign_stmnt	: copy_assign_stmnt
				| modify_assign_stmnt
				| post_operator_stmnt;

expr_stmnt: value_expr;

// ASSIGN STATEMENTS
copy_assign_stmnt:		var_name (',' var_name)* ':=' expr;
modify_assign_stmnt:	var_name ('+=' | '-=' | '*=' | '/=' | '%=' | '<<=' | '>>=' | '|=' | '&=' | '^=') expr;
post_operator_stmnt:	var_name ('++' | '--');

// BRANCH STATEMENTS
if_stmnt:		'if' if_condition code_block else_stmnt?;
if_condition:	expr;
else_stmnt:		'else' (if_stmnt | code_block);

switch_stmnt:		'switch' expr '{' switch_case_list? '}';
switch_case_list:	switch_case+;
switch_case:		(case_label | default_label) stmnt_list;
case_label:			'case' case_item_list ':';
default_label:		'default' ':';
case_item_list:		case_item (',' case_item)*;
case_item:			expr;

// CTRL TRANSFER STATEMENTS
break_stmnt:	'break';
continue_stmnt:	'continue';
return_stmnt:	'return' expr?;

// LOOP STATEMENTS
for_stmnt:			'for' var_name_stmnt? ';' expr? ';' assign_stmnt? code_block;
for_range_stmnt:	'for' IDENT ':' for_range '..' for_range ('->' INT_LITERAL)? code_block;
for_each_stmnt:		'foreach' IDENT ':' expr code_block;
for_range:			NEGATION? INT_LITERAL;
repeat_stmnt:		'repeat' INT_LITERAL? code_block;
while_stmnt:		'while' expr code_block;
do_while_stmnt:		'do' code_block 'while' expr;

// ATTRIBUTES
attrib_prefix:		'[[' attrib_list ']]';
attrib_list:		attrib (',' attrib)*;
attrib:				IDENT ('(' attrib_arg_list ')')?;
attrib_arg_list:	attrib_arg (',' attrib_arg)*;
attrib_arg:			expr;

// VARIABLES
var_name:			(OBJECT_IDENT | var_name_sub) ('.' var_name_sub)?;
var_name_sub:		IDENT array_access? ('.' var_name_sub)?;

var_decl_stmnt:		(type_denoter | auto_type_denoter) var_decl_list;
var_decl_list:		var_decl (',' var_decl)*;
var_decl:			IDENT var_init?
var_init:			':=' expr;

// ARRAYS
array_access:		'[' array_index ']' array_access?;
array_index:		expr;

// IMPORTS
import_stmnt:	'import' (STRING_LITERAL | IDENT);

// CLASSES
class_decl_stmnt:			attrib_prefix? (intern_class_decl_stmnt | extern_class_decl_stmnt);
base_class_ident:			':' ident;
class_visibility:			class_visibility_type (':')?;
class_visibility_type:		'public' | 'private';

intern_class_decl_stmnt:	'class' IDENT base_class_ident? class_body;
class_body:					'{' class_body_segment_list '}';
class_body_segment_list:	class_body_segment*;
class_body_segment:			class_visibility? decl_stmnt_list;

extern_class_decl_stmnt:	'extern' 'class' IDENT base_class_ident? extern_class_body;
extern_class_body:			'{' extern_decl_stmnt_list '}';

anonymous_class:			class_body;

// MODULES
module_decl_stmnt:			attrib_prefix? 'module' IDENT module_body;
module_body:				'{' extern_proc_decl_stmnt_list '}';

/*
// ENUMERATIONS
enum_decl_stmnt:	'enum' IDENT enum_body;
enum_body:			'{' enum_entry_list? '}';
enum_entry_list:	enum_entry (',' enum_entry)*;
enum_entry:			enum_entry_name (':=' expr)?;
enum_entry_name:	IDENT;

// FLAGS
flags_decl_stmnt:			'flags' IDENT base_flags_varname_list? flags_body;
flags_body:					'{' flags_entry_list? '}';
flags_entry_list:			flags_entry (',' flags_entry)*;
flags_entry:				IDENT;
base_flags_varname_list:	':' var_name (',' var_name)*;
*/

// PROCEDURES
proc_decl_stmnt:			attrib_prefix? proc_signature code_block;
extern_proc_decl_stmnt:		attrib_prefix? proc_signature;
proc_signature:				storage_modifier? return_type_denoter IDENT '(' param_list? ')';
storage_modifier:			'static';

init_decl_stmnt:			attrib_prefix? init_head code_block;
extern_init_decl_stmnt:		attrib_prefix? init_head;
init_head:					'init' '(' param_list? ')' base_init?;
base_init:					':' OBJECT_IDENT '(' param_list? ')';

release_decl_stmnt:			'release' code_block;

// PARAMETERS:
param_list:		param (',' param)*;
param:			type_denoter IDENT (':=' expr)?;

// ARGUMENTS
arg_list:	arg (',' arg)*;
arg:		(IDENT ':')? expr;

// EXPRESSIONS
expr_list:	expr (',' expr)*;

expr:				logic_or_expr;
logic_or_expr:		logic_and_expr ('or' logic_and_expr)*;
logic_and_expr:		bitwise_or_expr ('and' bitwise_or_expr)*;
bitwise_or_expr:	bitwise_xor_expr ('|' bitwise_xor_expr)*;
bitwise_xor_expr:	bitwise_and_expr ('^' bitwise_and_expr)*;
bitwise_and_expr:	equality_expr ('&' equality_expr)*;
equality_expr:		relation_expr (('=' | '!=') relation_expr)*;
relation_expr:		shift_expr (('<' | '>' | '<=' | '>=') shift_expr)*;
shift_expr:			add_expr (('<<' | '>>') add_expr)*;
add_expr:			sub_expr ('+' sub_expr)*;
sub_expr:			mul_expr ('-' mul_expr)*;
mul_expr:			div_expr ('*' div_expr)*;
div_expr:			value_expr (('/' | '%') value_expr)*;

value_expr:			primary_value_expr;

primary_value_expr	: literal_expr
					| var_access_expr
					| alloc_expr
					| bracket_expr
					| cast_expr
					| call_expr
					| unary_expr
					| init_list_expr
					| postfix_value_expr;

postfix_value_expr:	primary_value_expr array_access? ('.' (proc_call | var_name))?;

var_access_expr		: var_name;

init_list_expr: 	'{' expr_list? '}';

alloc_expr:			'new' type_denoter ctor_init? anonymous_class?;
ctor_init:			'(' arg_list? ')';

literal_expr:		LITERAL;
bracket_expr:		'(' expr ')';
cast_expr: 			'(' type_denoter ')' value_expr;
call_expr:			proc_call;
unary_expr:			('~' | '-' | 'not') value_expr;

proc_call:			var_name '(' arg_list? ')';

// TYPE DENOTERS
type_denoter			: builtin_type_denoter
						| array_type_denoter
						| pointer_type_denoter;

builtin_type_denoter	: BOOL_TYPE_DENOTER
						| INT_TYPE_DENOTER
						| FLOAT_TYPE_DENOTER;

return_type_denoter		: VOID_TYPE_DENOTER
						| type_denoter;

array_type_denoter:		type_denoter LR_PAREN;
LR_PAREN:				'[]';

pointer_type_denoter:	IDENT ('@')?;

auto_type_denoter		: 'var'
						| 'const';

VOID_TYPE_DENOTER:		'void';
BOOL_TYPE_DENOTER:		'bool';
INT_TYPE_DENOTER:		'int';
FLOAT_TYPE_DENOTER:		'float';

// LITERALS
LETTER:	[a-zA-Z];

BIN_DIGIT:	[01];
OCT_DIGIT:	[0-7];
DEC_DIGIT:	[0-9];
HEX_DIGIT:	[0-9a-fA-F];

LITERAL	: BIN_LITERAL
		| OCT_LITERAL
		| DEC_LITERAL
		| HEX_LITERAL
		| BOOL_LITERAL
		| NULL_LITERAL;

BIN_LITERAL:	'0b' BIN_DIGIT+;
OCT_LITERAL:	'0o' OCT_DIGIT+;
DEC_LITERAL:	DEC_DIGIT+;
HEX_LITERAL:	'0x' HEX_DIGIT+;

NULL_LITERAL:	'null';

BOOL_LITERAL	: 'true'
				| 'false';

INT_LITERAL	: BIN_LITERAL
			| OCT_LITERAL
			| DEC_LITERAL
			| HEX_LITERAL;

FLOAT_LITERAL:		DEC_LITERAL '.' DEC_LITERAL;

STRING_LITERAL	: STANDARD_STRING_LITERAL
				| VERBATIM_STRING_LITERAL;

STANDARD_STRING_LITERAL:	'"' QUOTED_TEXT '"';
VERBATIM_STRING_LITERAL:	'@' '"' VERBATIM_TEXT '"';

QUOTED_TEXT:		QUOTED_TEXT_ITEM QUOTED_TEXT?;
QUOTED_TEXT_ITEM	: ESCAPE_CHAR
					| ~["\\\u000A\u000D];

VERBATIM_TEXT:		VERBATIM_TEXT_ITEM VERBATIM_TEXT?;
VERBATIM_TEXT_ITEM	: ~["\\\u000A\u000D];

ESCAPE_CHAR	: '\\' [0\\tnr"']
			| '\\x' HEX_DIGIT HEX_DIGIT
			| '\\u' HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT
			| '\\U' HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT;

IDENT:			(LETTER | '_') (LETTER | '_' | DEC_DIGIT)*;			// Identifier
OBJECT_IDENT:	'this' | 'super';

NEGATION:	'-'

// WHITE SPACES & COMMENTS
WS:				[ \t\r\n]+ -> skip;									// Skip white spaces

BLOCK_COMMENT:	'/*' (BLOCK_COMMENT | .)*? '*/' -> channel(HIDDEN);	// Nesting allowed
LINE_COMMENT:	 .*? '\n' -> channel(HIDDEN);


