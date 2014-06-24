
// XieXie 2.0 Grammar
// 08/06/2014

grammar XieXie;

options {
	language = Cpp;
	backtrack = true;
	memoize = true;
	k = 2;
}

program: class_decl_stmnt_list? EOF;

code_block:	'{' stmnt_list? '}';

// STATEMENTS
stmnt_list:				stmnt+;
decl_stmnt_list: 		decl_stmnt+;
class_decl_stmnt_list:	class_decl_stmnt+;

stmnt	: decl_stmnt
		| branch_stmnt
		| loop_stmnt
		| assign_stmnt
		| ctrl_transfer_stmnt;

decl_stmnt	: variable_decl_stmnt
			| class_decl_stmnt
			| enum_decl_stmnt
			| flags_decl_stmnt
			| function_decl_stmnt;

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
copy_assign_stmnt:		variable_ident ':=' expr;
modify_assign_stmnt:	variable_ident ('+=' | '-=' | '*=' | '/=' | '%=' | '<<=' | '>>=' | '|=' | '&=' | '^=') expr;
post_operator_stmnt:	variable_ident ('++' | '--');

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
pattern	: expr_pattern
		| enum_case_pattern;

expr_pattern: 		expr;

enum_case_pattern:	(variable_ident '.')? enum_case_name;
enum_case_name:		IDENT;

// LOOP STATEMENTS
for_stmnt:			'for' for_init? ';' expr? ';' assign_stmnt? code_block;
for_each_stmnt:		'foreach' for_each_init ':' expr code_block;
for_ever_stmnt:		'forever' code_block;
for_range_stmnt:	'for' variable_name ':' for_range '..' for_range ('->' for_range)? code_block;
while_stmnt:		'while' expr code_block;
do_while_stmnt:		'do' code_block 'while' expr;

for_init:		variable_decl_stmnt | assign_stmnt;
for_each_init:	variable_decl_stmnt;
for_range:		INT_LITERAL;

// ATTRIBUTES
attribute_prefix:			'[[' attribute_list ']]';
attribute_list:				attribute (',' attribute)+;
attribute:					IDENT ('(' attribute_argument_list ')')?;
attribute_argument_list:	attribute_argument (',' attribute_argument)+;
attribute_argument:			expr;

// VARIABLES
variable_ident:		variable_name ('.' variable_name)*;
variable_name:		IDENT;

variable_decl_stmnt:	type_denoter IDENT variable_init?;
variable_init:			':=' expr;

// CLASSES
class_decl_stmnt:	class_modifier? 'class' class_name type_inheritance? class_body;
class_modifier:		'extern';
class_body:			'{' decl_stmnt_list? '}';
class_name:			IDENT;

type_inheritance:		':' variable_ident;
type_multi_inheritance:	':' variable_ident (',' variable_ident)*;

// ENUMERATIONS
enum_decl_stmnt:	'enum' variable_name enum_body;
enum_body:			'{' enum_entry_list? '}';
enum_entry_list:	enum_entry (',' enum_entry)*;
enum_entry:			enum_entry_name (':=' expr)?;
enum_entry_name:	IDENT;

// FLAGS
flags_decl_stmnt:	'flags' variable_name type_multi_inheritance? flags_body;
flags_body:			'{' flags_entry_list? '}';
flags_entry_list:	flags_entry (',' flags_entry)*;
flags_entry:		IDENT;

// FUNCTIONS
function_decl_stmnt:	attribute_prefix? function_head code_block;
function_head:			function_modifier? type_denoter variable_name '(' parameter_list? ')';
function_modifier:		'static';
parameter_list:			parameter (',' parameter)*;
parameter:				type_denoter variable_name (':=' expr)?;

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
value_expr:			literal_expr | bracket_expr | cast_expr | call_expr | unary_expr;
literal_expr:		LITERAL;
bracket_expr:		'(' arithmetic_expr ')';
cast_expr: 			'(' type_denoter ')' value_expr;
call_expr:			function_call;
unary_expr:			('~' | '-' | 'not') value_expr;

function_call:	variable_ident '(' argument_list? ')';
argument_list:	argument (',' argument)*;
argument:		(expr | variable_name ':' expr);

// TYPE DENOTERS
type_denoter	: buildin_type_denoter
				| custom_type_denoter;

buildin_type_denoter	: BOOL_TYPE_DENOTER
						| INT_TYPE_DENOTER
						| FLOAT_TYPE_DENOTER;

custom_type_denoter:	variable_ident;

BOOL_TYPE_DENOTER:		'bool';
INT_TYPE_DENOTER:		'int';
FLOAT_TYPE_DENOTER:		'float';

// LITERALS
fragment
Letter:		( 'a'..'z' | 'A'..'Z' );

fragment
BinDigit:	('0'..'1');

fragment
OctDigit:	('0'..'7');

fragment
DecDigit:	('0'..'9');

fragment
HexDigit:	('0'..'9' | 'a'..'f' | 'A'..'F');

LITERAL		: INT_LITERAL
			| FLOAT_LITERAL
			| BOOL_LITERAL
			| NULL_LITERAL
			| THIS_LITERAL
			| CHAR_LITERAL
			| STRING_LITERAL;

BIN_LITERAL:	'0b' BinDigit+;
OCT_LITERAL:	'0o' OctDigit+;
DEC_LITERAL:	( '0' | '1'..'9' DecDigit* );
HEX_LITERAL:	'0x' HexDigit+;

NULL_LITERAL:	'null';
THIS_LITERAL:	'this';
SUPER_LITERAL:	'super';

BOOL_LITERAL:	( 'true' | 'false' );

INT_LITERAL	: BIN_LITERAL
			| OCT_LITERAL
			| DEC_LITERAL
			| HEX_LITERAL;

FLOAT_LITERAL:	DEC_LITERAL '.' DEC_LITERAL;
CHAR_LITERAL:	'\'' ( EscapeCharacter | ~( '\'' | '\\' | '\r' | '\n' ) ) '\'';


STRING_LITERAL	: STANDARD_STRING_LITERAL
				| VERBATIM_STRING_LITERAL;
				;

STANDARD_STRING_LITERAL:	'"' ( EscapeCharacter | ~('\\' | '"' | '\r' | '\n') )* '"';
VERBATIM_STRING_LITERAL:	'@' '"' ( ~('\\' | '"') )* '"';

fragment
EscapeCharacter	: '\\' ('b' | 't' | 'n' | 'f' | 'r' | '\"' | '\'' | '\\')
				| '\\x' HexDigit HexDigit
				| '\\u' HexDigit HexDigit HexDigit HexDigit
				| '\\U' HexDigit HexDigit HexDigit HexDigit HexDigit HexDigit HexDigit HexDigit;

IDENT:	Letter (Letter | '0'..'9' | '_')*;

// WHITE SPACES & COMMENTS
WS:		(' ' | '\r' | '\t' | '\u000C' | '\n') { skip(); };

BLOCK_COMMENT:	'/*' ( options { greedy = false; } : . )* '*/' { $channel = HIDDEN; };
LINE_COMMENT:	'//' ~('\n'|'\r')* '\r'? '\n' { $channel = HIDDEN; };



