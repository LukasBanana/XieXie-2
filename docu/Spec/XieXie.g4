
// XieXie 2.0 Grammar
// 08/06/2014

grammar XieXie;

//options { language = Cpp; }

program: class_decl_stmnt_list? EOF;

code_block:	'{' stmnt_list? '}';

// STATEMENTS
stmnt_list:				stmnt+;
decl_stmnt_list: 		decl_stmnt+;
extern_decl_stmnt_list:	extern_decl_stmnt+;
class_decl_stmnt_list:	class_decl_stmnt+;

stmnt	: variable_decl_stmnt
		| branch_stmnt
		| loop_stmnt
		| assign_stmnt
		| ctrl_transfer_stmnt;

decl_stmnt	: variable_decl_stmnt
			| class_decl_stmnt
			| enum_decl_stmnt
			| flags_decl_stmnt
			| function_decl_stmnt
			| init_decl_stmnt;

extern_decl_stmnt	: extern_class_decl_stmnt
					| extern_function_decl_stmnt
					| extern_init_decl_stmnt
					| enum_decl_stmnt
					| flags_decl_stmnt;

branch_stmnt	: if_stmnt
				| switch_stmnt;

loop_stmnt	: for_stmnt
			| for_each_stmnt
			| for_ever_stmnt
			| for_range_stmnt
			| while_stmnt
			| do_while_stmnt;

ctrl_transfer_stmnt	: break_stmnt
					| continue_stmnt
					| return_stmnt;

assign_stmnt	: copy_assign_stmnt
				| modify_assign_stmnt
				| post_operator_stmnt;

// ASSIGN STATEMENTS
copy_assign_stmnt:		(variable_name ':=')+ expr;
modify_assign_stmnt:	variable_name ('+=' | '-=' | '*=' | '/=' | '%=' | '<<=' | '>>=' | '|=' | '&=' | '^=') expr;
post_operator_stmnt:	variable_name ('++' | '--');

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
case_item:			pattern;

// CTRL TRANSFER STATEMENTS
break_stmnt:	'break';
continue_stmnt:	'continue';
return_stmnt:	'return' expr?;

// PATTERNS
pattern				: expr_pattern
					| enum_case_pattern;

expr_pattern: 		arithmetic_expr;

enum_case_pattern:	(variable_name '.')? enum_case_name;
enum_case_name:		IDENT;

// LOOP STATEMENTS
for_stmnt:			'for' for_init? ';' arithmetic_expr? ';' assign_stmnt? code_block;
for_each_stmnt:		'foreach' for_each_init ':' expr code_block;
for_ever_stmnt:		'forever' code_block;
for_range_stmnt:	'for' variable_ident ':' for_range '..' for_range ('->' for_range)? code_block;
while_stmnt:		'while' expr code_block;
do_while_stmnt:		'do' code_block 'while' expr;

for_init:		variable_decl_stmnt | assign_stmnt;
for_each_init:	variable_decl_stmnt;
for_range:		INT_LITERAL;

// ATTRIBUTES
attribute_prefix:			'[[' attribute_list ']]';
attribute_list:				attribute (',' attribute)+;
attribute:					IDENT ('(' attribute_argument_list ')')?;
attribute_argument_list:	attribute_argument (',' attribute_argument)*;
attribute_argument:			expr;

// VARIABLES
variable_name:		variable_ident array_access_list? ('.' variable_name)?;
variable_ident:		IDENT;

variable_decl_stmnt:		data_type_denoter IDENT variable_init? (',' IDENT variable_init?)*;
variable_init:				':=' expr;

// ARRAYS
array_access_list:	array_access_list+;
array_access:		'[' array_index ']';
array_index:		arithmetic_expr;

// CLASSES
class_decl_stmnt:			intern_class_decl_stmnt | extern_class_decl_stmnt;
intern_class_decl_stmnt:	'class' class_name type_inheritance? class_body;
extern_class_decl_stmnt:	'extern' 'class' class_name extern_class_body;
class_body:					'{' (class_visibility? decl_stmnt_list?)* '}';
class_visibility:			class_visibility_type ':';
class_visibility_type:		'public' | 'private';
extern_class_body:			'{' extern_decl_stmnt_list? '}';
class_name:					IDENT;

type_inheritance:			':' variable_name;
type_multi_inheritance:		':' variable_name (',' variable_name)*;

// ENUMERATIONS
enum_decl_stmnt:	'enum' variable_ident enum_body;
enum_body:			'{' enum_entry_list? '}';
enum_entry_list:	enum_entry (',' enum_entry)*;
enum_entry:			enum_entry_name (':=' expr)?;
enum_entry_name:	IDENT;

// FLAGS
flags_decl_stmnt:	'flags' variable_ident type_multi_inheritance? flags_body;
flags_body:			'{' flags_entry_list? '}';
flags_entry_list:	flags_entry (',' flags_entry)*;
flags_entry:		IDENT;

// FUNCTIONS
function_decl_stmnt:		attribute_prefix? function_head code_block;
extern_function_decl_stmnt:	attribute_prefix? function_head;
function_head:				function_modifier? return_type_denoter variable_ident '(' parameter_list? ')';
function_modifier:			'static';
parameter_list:				parameter (',' parameter)*;
parameter:					data_type_denoter variable_ident (':=' expr)?;

init_decl_stmnt:			attribute_prefix? init_head code_block;
extern_init_decl_stmnt:		attribute_prefix? init_head;
init_head:					function_modifier? 'init' '(' parameter_list? ')';

// EXPRESSIONS
expr_list:	expr (',' expr)*;

expr	: arithmetic_expr
		| init_list_expr;

init_list_expr: '{' expr_list? '}';

arithmetic_expr:	logic_or_expr;
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

value_expr			: literal_expr
					| object_expr
					| bracket_expr
					| cast_expr
					| call_expr
					| unary_expr;

object_expr			: variable_name
					| new_expr;

new_expr:			'new' data_type_denoter (array_dimension | constructor_init)?;
array_dimension:	'[' arithmetic_expr ']';
constructor_init:	'(' argument_list? ')';

literal_expr:		LITERAL;
bracket_expr:		'(' arithmetic_expr ')';
cast_expr: 			'(' data_type_denoter ')' value_expr;
call_expr:			function_call;
unary_expr:			('~' | '-' | 'not') value_expr;

function_call:		variable_name '(' argument_list? ')';
argument_list:		argument (',' argument)*;
argument:			(expr | variable_ident ':' expr);

// TYPE DENOTERS
data_type_denoter	: builtin_type_denoter
					| array_type_denoter
					| class_type_denoter;

builtin_type_denoter	: BOOL_TYPE_DENOTER
						| INT_TYPE_DENOTER
						| FLOAT_TYPE_DENOTER;

return_type_denoter	: VOID_TYPE_DENOTER
					| data_type_denoter;

array_type_denoter:		data_type_denoter '[]';

class_type_denoter:		variable_name;

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
		| NULL_LITERAL
		| THIS_LITERAL
		| STRING_LITERAL;

BIN_LITERAL:	'0b' BIN_DIGIT+;
OCT_LITERAL:	'0o' OCT_DIGIT+;
DEC_LITERAL:	DEC_DIGIT+;
HEX_LITERAL:	'0x' HEX_DIGIT+;

NULL_LITERAL:	'null';
THIS_LITERAL:	'this';
SUPER_LITERAL:	'super';

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

IDENT:	(LETTER | '_') (LETTER | '_' | DEC_DIGIT)*;			// Identifier

// WHITE SPACES & COMMENTS
WS:				[ \t\r\n]+ -> skip;									// Skip white spaces

BLOCK_COMMENT:	'/*' (BLOCK_COMMENT | .)*? '*/' -> channel(HIDDEN);	// Nesting allowed
LINE_COMMENT:	 .*? '\n' -> channel(HIDDEN);


