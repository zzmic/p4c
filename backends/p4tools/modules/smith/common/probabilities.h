#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_PROBABILITIES_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_PROBABILITIES_H_

#include <cstdint>

static struct Probabilities {
    // assignment or method call
    uint16_t ASSIGNMENTORMETHODCALLSTATEMENT_ASSIGN = 75;
    uint16_t ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_CALL = 25;
    // probabilities of assignment types
    uint16_t ASSIGNMENTORMETHODCALLSTATEMENT_ASSIGN_BIT = 100;
    uint16_t ASSIGNMENTORMETHODCALLSTATEMENT_ASSIGN_STRUCTLIKE = 0;
    // probabilities of method calls
    uint16_t ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_ACTION = 44;
    uint16_t ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_FUNCTION = 45;
    uint16_t ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_TABLE = 10;
    uint16_t ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_CTRL = 5;
    uint16_t ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_BUILT_IN = 1;

    // probabilities for statements
    uint16_t STATEMENT_SWITCH = 5;
    uint16_t STATEMENT_ASSIGNMENTORMETHODCALL = 30;
    uint16_t STATEMENT_IF = 3;
    uint16_t STATEMENT_RETURN = 2;
    uint16_t STATEMENT_EXIT = 0;
    uint16_t STATEMENT_BLOCK = 2;
    // Add probabilities for for-loop and for-in-loop statement(s).
    // TODO(zzmic): Determine the probability for for-loop and for-in-loop statement(s).
    // TODO(zzmic): Dramatically the probability for assignment or method call statement intentionally 
    // to test out whether for-loop and for-in-loop statement(s) could be generated.
    uint16_t STATEMENT_FOR = 29;
    uint16_t STATEMENT_FOR_IN = 29;

    // probabilities to pick a slice when requesting an lval
    uint16_t SCOPE_LVAL_PATH = 90;
    uint16_t SCOPE_LVAL_SLICE = 10;

    // probabilities for statements or declarations
    uint16_t STATEMENTORDECLARATION_VAR = 10;
    uint16_t STATEMENTORDECLARATION_CONSTANT = 10;
    uint16_t STATEMENTORDECLARATION_STATEMENT = 80;

    // probabilities for unary bit expressions
    uint16_t EXPRESSION_BIT_UNARY_NEG = 20;
    uint16_t EXPRESSION_BIT_UNARY_CMPL = 20;
    uint16_t EXPRESSION_BIT_UNARY_CAST = 10;
    uint16_t EXPRESSION_BIT_UNARY_FUNCTION = 50;
    // probabilities for binary bit expressions
    uint16_t EXPRESSION_BIT_BINARY_MUL = 5;
    uint16_t EXPRESSION_BIT_BINARY_DIV = 5;
    uint16_t EXPRESSION_BIT_BINARY_MOD = 5;
    uint16_t EXPRESSION_BIT_BINARY_ADD = 10;
    uint16_t EXPRESSION_BIT_BINARY_SUB = 10;
    uint16_t EXPRESSION_BIT_BINARY_ADDSAT = 10;
    uint16_t EXPRESSION_BIT_BINARY_SUBSAT = 10;
    uint16_t EXPRESSION_BIT_BINARY_LSHIFT = 5;
    uint16_t EXPRESSION_BIT_BINARY_RSHIFT = 5;
    uint16_t EXPRESSION_BIT_BINARY_BAND = 10;
    uint16_t EXPRESSION_BIT_BINARY_BOR = 10;
    uint16_t EXPRESSION_BIT_BINARY_BXOR = 10;
    uint16_t EXPRESSION_BIT_BINARY_CONCAT = 5;
    // probabilities for ternary bit expressions
    uint16_t EXPRESSION_BIT_BINARY_SLICE = 50;
    uint16_t EXPRESSION_BIT_BINARY_MUX = 50;
    // probabilities for bit expressions
    uint16_t EXPRESSION_BIT_VAR = 20;
    uint16_t EXPRESSION_BIT_INT_LITERAL = 5;
    uint16_t EXPRESSION_BIT_BIT_LITERAL = 25;
    uint16_t EXPRESSION_BIT_UNARY = 10;
    uint16_t EXPRESSION_BIT_BINARY = 20;
    uint16_t EXPRESSION_BIT_TERNARY = 10;

    // probabilities for unary int expressions
    uint16_t EXPRESSION_INT_UNARY_NEG = 20;
    uint16_t EXPRESSION_INT_UNARY_CMPL = 20;
    uint16_t EXPRESSION_INT_UNARY_FUNCTION = 50;
    // probabilities for binary int expressions
    uint16_t EXPRESSION_INT_BINARY_MUL = 5;
    uint16_t EXPRESSION_INT_BINARY_DIV = 5;
    uint16_t EXPRESSION_INT_BINARY_MOD = 5;
    uint16_t EXPRESSION_INT_BINARY_ADD = 10;
    uint16_t EXPRESSION_INT_BINARY_SUB = 10;
    uint16_t EXPRESSION_INT_BINARY_LSHIFT = 0;
    uint16_t EXPRESSION_INT_BINARY_RSHIFT = 0;
    uint16_t EXPRESSION_INT_BINARY_BAND = 10;
    uint16_t EXPRESSION_INT_BINARY_BOR = 10;
    uint16_t EXPRESSION_INT_BINARY_BXOR = 10;
    // probabilities for ternary int expressions
    uint16_t EXPRESSION_INT_BINARY_MUX = 50;
    // probabilities for int expressions
    uint16_t EXPRESSION_INT_VAR = 20;
    uint16_t EXPRESSION_INT_INT_LITERAL = 5;
    uint16_t EXPRESSION_INT_UNARY = 10;
    uint16_t EXPRESSION_INT_BINARY = 20;

    // probabilities for boolean expressions
    uint16_t EXPRESSION_BOOLEAN_VAR = 15;
    uint16_t EXPRESSION_BOOLEAN_LITERAL = 20;
    uint16_t EXPRESSION_BOOLEAN_NOT = 35;
    uint16_t EXPRESSION_BOOLEAN_LAND = 5;
    uint16_t EXPRESSION_BOOLEAN_LOR = 5;
    uint16_t EXPRESSION_BOOLEAN_CMP = 10;
    uint16_t EXPRESSION_BOOLEAN_FUNCTION = 5;
    uint16_t EXPRESSION_BOOLEAN_BUILT_IN = 5;
    // probabilities for comparisons
    uint16_t EXPRESSION_BOOLEAN_CMP_EQU = 50;
    uint16_t EXPRESSION_BOOLEAN_CMP_NEQ = 50;

    // probabilities for structlike expressions
    uint16_t EXPRESSION_STRUCT_VAR = 50;
    uint16_t EXPRESSION_STRUCT_LITERAL = 30;
    uint16_t EXPRESSION_STRUCT_FUNCTION = 20;

    // probabilities for state transitions in the parser
    uint16_t P4STATE_TRANSITION_ACCEPT = 50;
    uint16_t P4STATE_TRANSITION_REJECT = 10;
    uint16_t P4STATE_TRANSITION_STATE = 30;
    uint16_t P4STATE_TRANSITION_SELECT = 10;

    uint16_t BASETYPE_BIT = 50;
    uint16_t BASETYPE_SIGNED_BIT = 0;
    uint16_t BASETYPE_VARBIT = 0;
    uint16_t BASETYPE_INT = 10;
    uint16_t BASETYPE_ERROR = 0;
    uint16_t BASETYPE_BOOL = 10;
    uint16_t BASETYPE_STRING = 0;
    uint16_t DERIVED_ENUM = 0;
    uint16_t DERIVED_HEADER = 5;
    uint16_t DERIVED_HEADER_STACK = 2;
    uint16_t DERIVED_STRUCT = 5;
    uint16_t DERIVED_HEADER_UNION = 0;
    uint16_t DERIVED_TUPLE = 0;
    uint16_t TYPE_VOID = 0;
    uint16_t TYPE_MATCH_KIND = 0;

    // probabilities for the types of constant declarations
    uint16_t CONSTANTDECLARATION_TYPE_BASE = 80;
    uint16_t CONSTANTDECLARATION_TYPE_STRUCT = 0;
    // probabilities for the base types in constant declarations
    uint16_t CONSTANTDECLARATION_BASETYPE_BIT = BASETYPE_BIT;
    uint16_t CONSTANTDECLARATION_BASETYPE_SIGNED_BIT = BASETYPE_SIGNED_BIT;
    uint16_t CONSTANTDECLARATION_BASETYPE_VARBIT = BASETYPE_VARBIT;
    uint16_t CONSTANTDECLARATION_BASETYPE_INT = BASETYPE_INT;
    uint16_t CONSTANTDECLARATION_BASETYPE_ERROR = BASETYPE_ERROR;
    uint16_t CONSTANTDECLARATION_BASETYPE_BOOL = BASETYPE_BOOL;
    uint16_t CONSTANTDECLARATION_BASETYPE_STRING = BASETYPE_STRING;
    uint16_t CONSTANTDECLARATION_DERIVED_ENUM = DERIVED_ENUM;
    uint16_t CONSTANTDECLARATION_DERIVED_HEADER = DERIVED_HEADER;
    uint16_t CONSTANTDECLARATION_DERIVED_HEADER_STACK = 0;
    uint16_t CONSTANTDECLARATION_DERIVED_STRUCT = DERIVED_STRUCT;
    uint16_t CONSTANTDECLARATION_DERIVED_HEADER_UNION = DERIVED_HEADER_UNION;
    uint16_t CONSTANTDECLARATION_DERIVED_TUPLE = DERIVED_TUPLE;
    uint16_t CONSTANTDECLARATION_TYPE_VOID = TYPE_VOID;
    uint16_t CONSTANTDECLARATION_TYPE_MATCH_KIND = TYPE_MATCH_KIND;

    // probabilities for the function return types
    uint16_t FUNCTIONDECLARATION_TYPE_BASE = 90;
    uint16_t FUNCTIONDECLARATION_TYPE_STRUCT = 9;
    // probabilities for the base types in function declarations
    uint16_t FUNCTIONDECLARATION_BASETYPE_BIT = BASETYPE_BIT;
    uint16_t FUNCTIONDECLARATION_BASETYPE_SIGNED_BIT = BASETYPE_SIGNED_BIT;
    uint16_t FUNCTIONDECLARATION_BASETYPE_VARBIT = BASETYPE_VARBIT;
    uint16_t FUNCTIONDECLARATION_BASETYPE_INT = 0;
    uint16_t FUNCTIONDECLARATION_BASETYPE_ERROR = BASETYPE_ERROR;
    uint16_t FUNCTIONDECLARATION_BASETYPE_BOOL = BASETYPE_BOOL;
    uint16_t FUNCTIONDECLARATION_BASETYPE_STRING = BASETYPE_STRING;
    uint16_t FUNCTIONDECLARATION_DERIVED_ENUM = DERIVED_ENUM;
    uint16_t FUNCTIONDECLARATION_DERIVED_HEADER = DERIVED_HEADER;
    // there is no support yet for header stack initialization
    uint16_t FUNCTIONDECLARATION_DERIVED_HEADER_STACK = 0;
    uint16_t FUNCTIONDECLARATION_DERIVED_STRUCT = DERIVED_STRUCT;
    uint16_t FUNCTIONDECLARATION_DERIVED_HEADER_UNION = DERIVED_HEADER_UNION;
    uint16_t FUNCTIONDECLARATION_DERIVED_TUPLE = DERIVED_TUPLE;
    uint16_t FUNCTIONDECLARATION_TYPE_VOID = 1;
    uint16_t FUNCTIONDECLARATION_TYPE_MATCH_KIND = TYPE_MATCH_KIND;

    // probabilities for types in header structures
    uint16_t HEADERTYPEDECLARATION_FIELD_BASE = 100;
    uint16_t HEADERTYPEDECLARATION_FIELD_STRUCT = 0;
    // probabilities for the base types in header type declarations
    uint16_t HEADERTYPEDECLARATION_BASETYPE_BIT = BASETYPE_BIT;
    uint16_t HEADERTYPEDECLARATION_BASETYPE_SIGNED_BIT = BASETYPE_SIGNED_BIT;
    uint16_t HEADERTYPEDECLARATION_BASETYPE_VARBIT = BASETYPE_VARBIT;
    uint16_t HEADERTYPEDECLARATION_BASETYPE_INT = 0;
    uint16_t HEADERTYPEDECLARATION_BASETYPE_ERROR = BASETYPE_ERROR;
    uint16_t HEADERTYPEDECLARATION_BASETYPE_BOOL = 0;
    uint16_t HEADERTYPEDECLARATION_BASETYPE_STRING = BASETYPE_STRING;
    uint16_t HEADERTYPEDECLARATION_DERIVED_ENUM = DERIVED_ENUM;
    uint16_t HEADERTYPEDECLARATION_DERIVED_HEADER = 0;
    uint16_t HEADERTYPEDECLARATION_DERIVED_HEADER_STACK = 0;
    // FIXME: Headers should be able to have structs?
    uint16_t HEADERTYPEDECLARATION_DERIVED_STRUCT = 0;
    uint16_t HEADERTYPEDECLARATION_DERIVED_HEADER_UNION = 0;
    uint16_t HEADERTYPEDECLARATION_DERIVED_TUPLE = 0;
    uint16_t HEADERTYPEDECLARATION_TYPE_VOID = 0;
    uint16_t HEADERTYPEDECLARATION_TYPE_MATCH_KIND = 0;

    // probabilities for the parameter direction
    uint16_t PARAMETER_DIR_IN = 33;
    uint16_t PARAMETER_DIR_OUT = 33;
    uint16_t PARAMETER_DIR_INOUT = 33;
    // probabilities for the base types in parameter declarations
    uint16_t PARAMETER_BASETYPE_BIT = BASETYPE_BIT;
    uint16_t PARAMETER_BASETYPE_SIGNED_BIT = BASETYPE_SIGNED_BIT;
    uint16_t PARAMETER_BASETYPE_VARBIT = BASETYPE_VARBIT;
    uint16_t PARAMETER_BASETYPE_INT = 0;
    uint16_t PARAMETER_BASETYPE_ERROR = BASETYPE_ERROR;
    uint16_t PARAMETER_BASETYPE_BOOL = 0;
    uint16_t PARAMETER_BASETYPE_STRING = BASETYPE_STRING;
    uint16_t PARAMETER_DERIVED_ENUM = DERIVED_ENUM;
    uint16_t PARAMETER_DERIVED_HEADER = DERIVED_HEADER;
    uint16_t PARAMETER_DERIVED_HEADER_STACK = 0;
    uint16_t PARAMETER_DERIVED_STRUCT = DERIVED_STRUCT;
    uint16_t PARAMETER_DERIVED_HEADER_UNION = DERIVED_HEADER_UNION;
    uint16_t PARAMETER_DERIVED_TUPLE = DERIVED_TUPLE;
    uint16_t PARAMETER_TYPE_VOID = TYPE_VOID;
    uint16_t PARAMETER_TYPE_MATCH_KIND = TYPE_MATCH_KIND;
    // probabilities for the directionless base types in parameter declarations
    // the value is coming from the control, back ends can adjust these probs
    uint16_t PARAMETER_NONEDIR_BASETYPE_BIT = PARAMETER_BASETYPE_BIT;
    uint16_t PARAMETER_NONEDIR_BASETYPE_SIGNED_BIT = PARAMETER_BASETYPE_SIGNED_BIT;
    uint16_t PARAMETER_NONEDIR_BASETYPE_VARBIT = PARAMETER_BASETYPE_VARBIT;
    uint16_t PARAMETER_NONEDIR_BASETYPE_INT = PARAMETER_BASETYPE_INT;
    uint16_t PARAMETER_NONEDIR_BASETYPE_ERROR = PARAMETER_BASETYPE_ERROR;
    uint16_t PARAMETER_NONEDIR_BASETYPE_BOOL = PARAMETER_BASETYPE_BOOL;
    uint16_t PARAMETER_NONEDIR_BASETYPE_STRING = PARAMETER_BASETYPE_STRING;
    uint16_t PARAMETER_NONEDIR_DERIVED_ENUM = PARAMETER_DERIVED_ENUM;
    uint16_t PARAMETER_NONEDIR_DERIVED_HEADER = PARAMETER_DERIVED_HEADER;
    uint16_t PARAMETER_NONEDIR_DERIVED_HEADER_STACK = PARAMETER_DERIVED_HEADER_STACK;
    uint16_t PARAMETER_NONEDIR_DERIVED_HEADER_UNION = DERIVED_HEADER_UNION;
    uint16_t PARAMETER_NONEDIR_DERIVED_STRUCT = DERIVED_STRUCT;
    uint16_t PARAMETER_NONEDIR_DERIVED_TUPLE = DERIVED_TUPLE;
    uint16_t PARAMETER_NONEDIR_TYPE_VOID = TYPE_VOID;
    uint16_t PARAMETER_NONEDIR_TYPE_MATCH_KIND = TYPE_MATCH_KIND;

    // probabilities for types in struct structures
    uint16_t STRUCTTYPEDECLARATION_FIELD_BASE = 75;
    uint16_t STRUCTTYPEDECLARATION_FIELD_STRUCT = 25;
    uint16_t STRUCTTYPEDECLARATION_FIELD_STACK = 0;
    // probabilities for the headers struct type
    uint16_t STRUCTTYPEDECLARATION_HEADERS_HEADER = 90;
    uint16_t STRUCTTYPEDECLARATION_HEADERS_STACK = 10;
    // probabilities for the base types in struct type declarations
    uint16_t STRUCTTYPEDECLARATION_BASETYPE_BIT = BASETYPE_BIT;
    uint16_t STRUCTTYPEDECLARATION_BASETYPE_SIGNED_BIT = BASETYPE_SIGNED_BIT;
    uint16_t STRUCTTYPEDECLARATION_BASETYPE_VARBIT = BASETYPE_VARBIT;
    uint16_t STRUCTTYPEDECLARATION_BASETYPE_INT = 0;
    uint16_t STRUCTTYPEDECLARATION_BASETYPE_ERROR = BASETYPE_ERROR;
    uint16_t STRUCTTYPEDECLARATION_BASETYPE_BOOL = BASETYPE_BOOL;
    uint16_t STRUCTTYPEDECLARATION_BASETYPE_STRING = BASETYPE_STRING;
    uint16_t STRUCTTYPEDECLARATION_DERIVED_ENUM = DERIVED_ENUM;
    uint16_t STRUCTTYPEDECLARATION_DERIVED_HEADER = DERIVED_HEADER;
    uint16_t STRUCTTYPEDECLARATION_DERIVED_HEADER_STACK = DERIVED_HEADER_STACK;
    uint16_t STRUCTTYPEDECLARATION_DERIVED_STRUCT = DERIVED_STRUCT;
    uint16_t STRUCTTYPEDECLARATION_DERIVED_HEADER_UNION = DERIVED_HEADER_UNION;
    uint16_t STRUCTTYPEDECLARATION_DERIVED_TUPLE = DERIVED_TUPLE;
    uint16_t STRUCTTYPEDECLARATION_TYPE_VOID = 0;
    uint16_t STRUCTTYPEDECLARATION_TYPE_MATCH_KIND = 0;
    // probabilities for types in struct structures
    uint16_t TYPEDECLARATION_HEADER = 75;
    uint16_t TYPEDECLARATION_STRUCT = 25;
    uint16_t TYPEDECLARATION_UNION = 0;

    // probabilities for types in struct structures
    uint16_t TYPEDEFDECLARATION_BASE = 75;
    uint16_t TYPEDEFDECLARATION_STRUCTLIKE = 25;
    uint16_t TYPEDEFDECLARATION_STACK = 0;
    // probabilities for the base types in typedef declarations
    uint16_t TYPEDEFDECLARATION_BASETYPE_BIT = BASETYPE_BIT;
    uint16_t TYPEDEFDECLARATION_BASETYPE_SIGNED_BIT = BASETYPE_SIGNED_BIT;
    uint16_t TYPEDEFDECLARATION_BASETYPE_VARBIT = BASETYPE_VARBIT;
    uint16_t TYPEDEFDECLARATION_BASETYPE_INT = 0;
    uint16_t TYPEDEFDECLARATION_BASETYPE_ERROR = BASETYPE_ERROR;
    uint16_t TYPEDEFDECLARATION_BASETYPE_BOOL = 0;
    uint16_t TYPEDEFDECLARATION_BASETYPE_STRING = BASETYPE_STRING;
    uint16_t TYPEDEFDECLARATION_DERIVED_ENUM = DERIVED_ENUM;
    uint16_t TYPEDEFDECLARATION_DERIVED_HEADER = DERIVED_HEADER;
    uint16_t TYPEDEFDECLARATION_DERIVED_HEADER_STACK = DERIVED_HEADER_STACK;
    uint16_t TYPEDEFDECLARATION_STRUCT = DERIVED_STRUCT;
    uint16_t TYPEDEFDECLARATION_HEADER_UNION = DERIVED_HEADER_UNION;
    uint16_t TYPEDEFDECLARATION_TUPLE = DERIVED_TUPLE;
    uint16_t TYPEDEFDECLARATION_TYPE_VOID = TYPE_VOID;
    uint16_t TYPEDEFDECLARATION_TYPE_MATCH_KIND = TYPE_MATCH_KIND;

    // probabilities for the types of constant declarations
    uint16_t VARIABLEDECLARATION_TYPE_BASE = 80;
    uint16_t VARIABLEDECLARATION_TYPE_STRUCT = 15;
    uint16_t VARIABLEDECLARATION_TYPE_STACK = 5;
    // probabilities for the base types in variable declarations
    uint16_t VARIABLEDECLARATION_BASETYPE_BIT = BASETYPE_BIT;
    uint16_t VARIABLEDECLARATION_BASETYPE_SIGNED_BIT = BASETYPE_SIGNED_BIT;
    uint16_t VARIABLEDECLARATION_BASETYPE_VARBIT = BASETYPE_VARBIT;
    uint16_t VARIABLEDECLARATION_BASETYPE_INT = 0;
    uint16_t VARIABLEDECLARATION_BASETYPE_ERROR = BASETYPE_ERROR;
    uint16_t VARIABLEDECLARATION_BASETYPE_BOOL = BASETYPE_BOOL;
    uint16_t VARIABLEDECLARATION_BASETYPE_STRING = BASETYPE_STRING;
    uint16_t VARIABLEDECLARATION_DERIVED_ENUM = DERIVED_ENUM;
    uint16_t VARIABLEDECLARATION_DERIVED_HEADER = DERIVED_HEADER;
    uint16_t VARIABLEDECLARATION_DERIVED_HEADER_STACK = DERIVED_HEADER_STACK;
    uint16_t VARIABLEDECLARATION_DERIVED_STRUCT = DERIVED_STRUCT;
    uint16_t VARIABLEDECLARATION_DERIVED_HEADER_UNION = DERIVED_HEADER_UNION;
    uint16_t VARIABLEDECLARATION_DERIVED_TUPLE = DERIVED_TUPLE;
    uint16_t VARIABLEDECLARATION_TYPE_VOID = TYPE_VOID;
    uint16_t VARIABLEDECLARATION_TYPE_MATCH_KIND = TYPE_MATCH_KIND;
} PCT;

static struct Declarations {
    // minimum and maximum number of type declarations
    uint16_t MIN_TYPE = 1;
    uint16_t MAX_TYPE = 8;

    // minimum and maximum number of statements in a block statement
    uint16_t BLOCKSTATEMENT_MIN_STAT = 3;
    uint16_t BLOCKSTATEMENT_MAX_STAT = 10;

    // minimum and maximum number of callable declarations
    uint16_t MIN_CALLABLES = 0;
    uint16_t MAX_CALLABLES = 4;

    // minimum and maximum variable declarations
    uint16_t MIN_VAR = 0;
    uint16_t MAX_VAR = 5;

    uint16_t MIN_INSTANCE = 0;
    uint16_t MAX_INSTANCE = 2;

    uint16_t MIN_ACTION = 0;
    uint16_t MAX_ACTION = 2;

    uint16_t MIN_TABLE = 0;
    uint16_t MAX_TABLE = 3;
} DECL;

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_PROBABILITIES_H_ */
