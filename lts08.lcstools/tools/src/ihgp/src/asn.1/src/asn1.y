%{
/* headers */
#include "asn1.h"

/* externs */
void yyerror(const char *);

%}

/* starting terminal in prouctions */
%start ModuleDefinition

/* data type for parser */
%union {
	char string[128];
};

/* tokens returned from lexixal analyzer */
%token <string> typereference
%token <string> identifier
%token <string> valuereference
%token <string> modulereference
%token <string> comment
%token <string> number
%token <string> realnumber
%token <string> bstring
%token <string> hstring
%token <string> cstring
%token <string> xmlbstring
%token <string> xmlhstring
%token <string> xmlcstring
%token <string> xmlasn1typename
%token <string> true
%token <string> false
%token <string> ASTERICK
%token <string> AMPERSAND
%token <string> COLONCOLONEQUAL 
%token <string> DOUBLELEFTBRACKET
%token <string> DOUBLERIGHTBRACKET
%token <string> TWOPERIODS
%token <string> ELLIPSIS
%token <string>	LESSTHANSLASH
%token <string> SLASHGREATERTHAN
%token <string> LEFTBRACE
%token <string> RIGHTBRACE
%token <string> LESSTHAN
%token <string> GREATERTHAN
%token <string> COMMA
%token <string> PERIOD
%token <string> LEFTPAREN
%token <string> RIGHTPAREN
%token <string> LEFTBRACKET
%token <string> RIGHTBRACKET
%token <string> DASH
%token <string> COLON
%token <string> EQUAL
%token <string> QUOTATION
%token <string> APOSTROPHE
%token <string> SPACE
%token <string> SEMICOLON
%token <string> AT
%token <string> PIPE
%token <string> EXCLAMATION
%token <string> CARET
%token <string> ABSENT
%token <string> ABSTRACTSYNTAX
%token <string> ALL
%token <string> APPLICATION
%token <string> AUTOMATIC
%token <string> BEGIN
%token <string> BIT
%token <string> BMPString
%token <string> BOOLEAN
%token <string> BY
%token <string> CHARACTER
%token <string> CHOICE
%token <string> CLASS
%token <string> COMPONENT
%token <string> COMPONENTS
%token <string> CONSTRAINED
%token <string> CONTAINING
%token <string> DEFAULT
%token <string> DEFINITIONS
%token <string> EMBEDDED
%token <string> ENCODED
%token <string> END
%token <string> ENUMERATED
%token <string> EXCEPT
%token <string> EXPLICIT
%token <string> EXPORTS
%token <string> EXTENSIBILITY
%token <string> EXTERNAL
%token <string> FALSE
%token <string> FROM
%token <string> GeneralizedTime
%token <string> GeneralString
%token <string> GraphicString
%token <string> IA5String
%token <string> IDENTIFIER
%token <string> IMPLICIT
%token <string> IMPLIED
%token <string> IMPORTS
%token <string> INCLUDES
%token <string> INSTANCE
%token <string> INTEGER
%token <string> INTERSECTION
%token <string> ISO646String
%token <string> MAX
%token <string> MIN
%token <string> MINUSINFINITY
%token <string> NULL
%token <string> NumericString
%token <string> OBJECT
%token <string> ObjectDescriptor
%token <string> OCTET
%token <string> OF
%token <string> OPTIONAL
%token <string> PATTERN
%token <string> PDV
%token <string> PLUSINFINITY
%token <string> PRESENT
%token <string> PrintableString
%token <string> PRIVATE
%token <string> REAL
%token <string> RELATIVEOID
%token <string> SEQUENCE
%token <string> SET
%token <string> SIZE
%token <string> STRING
%token <string> SYNTAX
%token <string> T61String
%token <string> TAGS
%token <string> TeletexString
%token <string> TRUE
%token <string> TYPEIDENTIFIER 
%token <string> UNION
%token <string> UNIQUE
%token <string> UNIVERSAL
%token <string> UniversalString
%token <string> UTCTime
%token <string> UTF8String
%token <string> VideotexString
%token <string> VisibleString
%token <string> WITH

%%

ModuleDefinition:
	ModuleIdentifier DEFINITIONS TagDefault ExtensionDefault 
	COLONCOLONEQUAL BEGIN ModuleBody END
	;

ModuleIdentifier:
	modulereference DefinitiveIdentifier
	;

DefinitiveIdentifier:
	LEFTBRACE DefinitiveObjIdComponentList RIGHTBRACE
	| /* empty */
	;


DefinitiveObjIdComponentList:
	DefinitiveObjIdComponent
	| DefinitiveObjIdComponent DefinitiveObjIdComponentList
	;

DefinitiveObjIdComponent:
	NameForm
	| DefinitiveNumberForm
	| DefinitiveNameAndNumberForm
	;

DefinitiveNumberForm: 
 	number
	;

DefinitiveNameAndNumberForm: 
 	identifier LEFTPAREN DefinitiveNumberForm RIGHTPAREN
	;

TagDefault: 
	EXPLICIT TAGS
	| IMPLICIT TAGS
	| AUTOMATIC TAGS
	| /* empty */
	;

ExtensionDefault: 
	EXTENSIBILITY IMPLIED
	| /* empty */
	;

ModuleBody: 
	Exports Imports AssignmentList
	| /* empty */
	;

Exports: 
	EXPORTS SymbolsExported SEMICOLON
	| EXPORTS ALL SEMICOLON
	| /* empty */
	;

SymbolsExported: 
	SymbolList
	| /* empty */
	;

Imports: 
	IMPORTS SymbolsImported SEMICOLON
	| /* empty */
	;

SymbolsImported: 
	SymbolsFromModuleList
	| /* empty */
	;

SymbolsFromModuleList: 
	SymbolsFromModule
	| SymbolsFromModuleList SymbolsFromModule
	;

SymbolsFromModule: 
	SymbolList FROM GlobalModuleReference
	;

GlobalModuleReference: 
	modulereference AssignedIdentifier
	;

AssignedIdentifier: 
	ObjectIdentifierValue
	| DefinedValue
	| /* empty */
	;

SymbolList: 
	Symbol 
	| SymbolList COMMA Symbol
	;

Symbol: 
	Reference 
	| ParameterizedReference
	;

Reference: 
	typereference
	| valuereference
	| objectclassreference
	| objectreference
	| objectsetreference
	;

AssignmentList: 
	Assignment
	| AssignmentList Assignment
	;

Assignment: 
	TypeAssignment
	| ValueAssignment
	| XMLValueAssignment
	| ValueSetTypeAssignment
	| ObjectClassAssignment
	| ObjectAssignment
	| ObjectSetAssignment
	| ParameterizedAssignment
	;

DefinedType: 
	ExternalTypeReference
	| typereference
	| ParameterizedType
	| ParameterizedValueSetType
	;

ExternalTypeReference: 
	modulereference PERIOD typereference
	;

NonParameterizedTypeName: 
	ExternalTypeReference
	| typereference
	| xmlasn1typename
	;

DefinedValue: 
	ExternalValueReference
	| valuereference
	| ParameterizedValue
	;

ExternalValueReference: 
	modulereference PERIOD valuereference
	;

AbsoluteReference: 
	AT ModuleIdentifier PERIOD ItemSpec
	;

ItemSpec: 
	typereference
	| ItemId PERIOD ComponentId
	;

ItemId: 
	ItemSpec
	;

ComponentId: 
	identifier 
	| number
	| ASTERICK
	;

TypeAssignment: 
	typereference COLONCOLONEQUAL Type
	;

ValueAssignment: 
	valuereference Type COLONCOLONEQUAL Value
	;

XMLValueAssignment: 
	valuereference COLONCOLONEQUAL XMLTypedValue
	;

XMLTypedValue: 
	LESSTHAN AMPERSAND NonParameterizedTypeName  GREATERTHAN
	XMLValue LESSTHANSLASH AMPERSAND NonParameterizedTypeName GREATERTHAN
	| LESSTHAN AMPERSAND NonParameterizedTypeName SLASHGREATERTHAN
	;

ValueSetTypeAssignment: 
	typereference Type COLONCOLONEQUAL ValueSet
	;

ValueSet: 
	LEFTBRACE ElementSetSpecs RIGHTBRACE
	;

Type: 
	BuiltinType | 
	ReferencedType | 
	ConstrainedType
	;

BuiltinType: 
	BitStringType
	| BooleanType
	| CharacterStringType
	| ChoiceType
	| EmbeddedPDVType
	| EnumeratedType
	| ExternalType
	| InstanceOfType
	| IntegerType
	| NullType
	| ObjectClassFieldType
	| ObjectIdentifierType
	| OctetStringType
	| RealType
	| RelativeOIDType
	| SequenceType
	| SequenceOfType
	| SetType
	| SetOfType
	| TaggedType
	;

NamedType: 
	identifier Type
	;

ReferencedType: 
	DefinedType
	| UsefulType
	| SelectionType
	| TypeFromObject
	| ValueSetFromObjects
	;

Value: 
	BuiltinValue 
	| ReferencedValue 
	| ObjectClassFieldValue
	;

XMLValue: 
	XMLBuiltinValue
	| XMLObjectClassFieldValue
	;

BuiltinValue: 
	BitStringValue
	| BooleanValue
	| CharacterStringValue
	| ChoiceValue
	| EmbeddedPDVValue
	| EnumeratedValue
	| ExternalValue
	| InstanceOfValue
	| IntegerValue
	| NullValue
	| ObjectIdentifierValue
	| OctetStringValue
	| RealValue
	| RelativeOIDValue
	| SequenceValue
	| SequenceOfValue
	| SetValue
	| SetOfValue
	| TaggedValue
	;

XMLBuiltinValue: 
	XMLBitStringValue
	| XMLBooleanValue
	| XMLCharacterStringValue
	| XMLChoiceValue
	| XMLEmbeddedPDVValue
	| XMLEnumeratedValue
	| XMLExternalValue
	| XMLInstanceOfValue
	| XMLIntegerValue
	| XMLNullValue
	| XMLObjectIdentifierValue
	| XMLOctetStringValue
	| XMLRealValue
	| XMLRelativeOIDValue
	| XMLSequenceValue
	| XMLSequenceOfValue
	| XMLSetValue
	| XMLSetOfValue
	| XMLTaggedValue
	;

ReferencedValue: 
	DefinedValue
	| ValueFromObject
	;

NamedValue: 
	identifier Value
	;

XMLNamedValue: 
	LESSTHAN AMPERSAND identifier  GREATERTHAN 
	XMLValue LESSTHANSLASH AMPERSAND identifier GREATERTHAN
	;

BooleanType: 
	BOOLEAN
	;

BooleanValue: 
	TRUE 
	| FALSE
	;


XMLBooleanValue: 
	LESSTHAN AMPERSAND true SLASHGREATERTHAN
	| LESSTHAN AMPERSAND false SLASHGREATERTHAN
	;

IntegerType: 
	INTEGER
	| INTEGER LEFTBRACE NamedNumberList RIGHTBRACE
	;

NamedNumberList: 
	NamedNumber
	| NamedNumberList COMMA NamedNumber
	;

NamedNumber: 
	identifier LEFTPAREN SignedNumber RIGHTPAREN
	| identifier LEFTPAREN DefinedValue RIGHTPAREN
	;

SignedNumber: 
	number 
	| DASH number
	;

IntegerValue: 
	SignedNumber 
	| identifier
	;

XMLIntegerValue: 
	SignedNumber
	| LESSTHAN AMPERSAND identifier SLASHGREATERTHAN
	;

EnumeratedType: 
	ENUMERATED LEFTBRACE Enumerations RIGHTBRACE
	;

Enumerations: 
	RootEnumeration
	| RootEnumeration  COMMA ELLIPSIS ExceptionSpec
	| RootEnumeration  COMMA ELLIPSIS 
		ExceptionSpec COMMA AdditionalEnumeration
	;

RootEnumeration: 
	Enumeration
	;

AdditionalEnumeration: 
	Enumeration
	;

Enumeration: 
	EnumerationItem 
	| EnumerationItem COMMA Enumeration
	;

EnumerationItem: 
	identifier 
	| NamedNumber
	;

EnumeratedValue: 
	identifier
	;

XMLEnumeratedValue: 
	LESSTHAN AMPERSAND identifier SLASHGREATERTHAN
	;

RealType: 
	REAL
	;

RealValue: 
	NumericRealValue 
	| SpecialRealValue
	;

NumericRealValue: 
	realnumber
	| DASH realnumber
	| SequenceValue
	;

SpecialRealValue: 
	PLUSINFINITY 
	| MINUSINFINITY
	;

XMLRealValue: 
	XMLNumericRealValue
	| XMLSpecialRealValue
	;

XMLNumericRealValue: 
	realnumber
	| DASH realnumber
	;

XMLSpecialRealValue: 
	LESSTHAN AMPERSAND PLUSINFINITY SLASHGREATERTHAN 
	| LESSTHAN AMPERSAND MINUSINFINITY SLASHGREATERTHAN
	;

BitStringType: 
	BIT STRING 
	| BIT STRING LEFTBRACE NamedBitList RIGHTBRACE
	;

NamedBitList: 
	NamedBit | 
	NamedBitList COMMA NamedBit
	;

NamedBit: 
	identifier LEFTPAREN number RIGHTPAREN
	| identifier LEFTPAREN DefinedValue RIGHTPAREN
	;

BitStringValue: 
	bstring 
	| hstring 
	| LEFTBRACE IdentifierList RIGHTBRACE 
	| LEFTBRACE RIGHTBRACE 
	| CONTAINING Value
	;

IdentifierList: 
	identifier 
	| IdentifierList COMMA identifier
	;

XMLBitStringValue: 
	XMLTypedValue
	| xmlbstring
	| XMLIdentifierList
	| /* empty */
	;

XMLIdentifierList: 
	LESSTHAN AMPERSAND identifier SLASHGREATERTHAN
	| XMLIdentifierList LESSTHAN AMPERSAND identifier SLASHGREATERTHAN
	;

OctetStringType: 
	OCTET STRING
	;

OctetStringValue: 
	bstring 
	| hstring 
	| CONTAINING Value
	;

XMLOctetStringValue: 
	XMLTypedValue
	| xmlhstring
	;

NullType: 
	NULL
	;

NullValue: 
	NULL
	;

XMLNullValue: 
	/* empty */
	;

SequenceType: 
	SEQUENCE "{"  "}"
	| SEQUENCE LEFTBRACE ExtensionAndException 
		OptionalExtensionMarker RIGHTBRACE
	| SEQUENCE LEFTBRACE ComponentTypeLists RIGHTBRACE
	;

ExtensionAndException: 
	ELLIPSIS 
	| ELLIPSIS ExceptionSpec
	;

OptionalExtensionMarker: 
	COMMA ELLIPSIS
	| /* empty */
	;

ComponentTypeLists: 
	RootComponentTypeList
	| RootComponentTypeList COMMA ExtensionAndException 
		ExtensionAdditions OptionalExtensionMarker
	| RootComponentTypeList COMMA ExtensionAndException 
		ExtensionAdditions ExtensionEndMarker COMMA 
		RootComponentTypeList
	| ExtensionAndException ExtensionAdditions ExensionEndMarker
		COMMA RootComponentTypeList
	| ExtensionAndException ExtensionAdditions OptionalExtensionMarker
	;

RootComponentTypeList: 
	ComponentTypeList
	;

ExtensionEndMarker: 
	COMMA ELLIPSIS
	;

ExtensionAdditions: 
	COMMA ExtensionAdditionList
	| /* empty */
	;

ExtensionAdditionList: 
	ExtensionAddition
	| ExtensionAdditionList COMMA ExtensionAddition
	;

ExtensionAddition: 
	ComponentType
	| ExtensionAdditionGroup
	;

ExtensionAdditionGroup: 
	DOUBLELEFTBRACKET VersionNumber ComponentTypeList DOUBLERIGHTBRACKET
	;

VersionNumber: 
	/* empty */
	| number COLON
	;

ComponentTypeList: 
	ComponentType
	| ComponentTypeList COMMA ComponentType
	;

ComponentType: 
	NamedType
	| NamedType OPTIONAL
	| NamedType DEFAULT Value
	| COMPONENTS OF Type
	;

SequenceValue: 
	LEFTBRACE ComponentValueList RIGHTBRACE
	| LEFTBRACE RIGHTBRACE
	;

ComponentValueList: 
	NamedValue
	| ComponentValueList COMMA NamedValue
	;

XMLSequenceValue: 
	XMLComponentValueList
	| /* empty */
	;

XMLComponentValueList: 
	XMLNamedValue
	| XMLComponentValueList XMLNamedValue
	;

SequenceOfType: 
	SEQUENCE OF Type 
	| SEQUENCE OF NamedType
	;

SequenceOfValue: 
	LEFTBRACE ValueList RIGHTBRACE 
	| LEFTBRACE NamedValueList RIGHTBRACE 
	| LEFTBRACE RIGHTBRACE 
	;

ValueList: 
	Value 
	| ValueList COMMA Value

XMLSequenceOfValue: 
	XMLValueList
	| XMLDelimitedItemList
	| XMLSpaceSeparatedList
	| /* empty */
	;

XMLValueList: 
	XMLValueOrEmpty
	| XMLValueOrEmpty XMLValueList
	;

XMLValueOrEmpty: 
	XMLValue
	| LESSTHAN AMPERSAND NonParameterizedTypeName SLASHGREATERTHAN
	;

XMLSpaceSeparatedList: 
	XMLValueOrEmpty
	| XMLValueOrEmpty SPACE XMLSpaceSeparatedList
	;

XMLDelimitedItemList: 
	XMLDelimitedItem
	| XMLDelimitedItem XMLDelimitedItemList
	;

XMLDelimitedItem: 
	LESSTHAN AMPERSAND NonParameterizedTypeName GREATERTHAN XMLValue
		LESSTHANSLASH AMPERSAND NonParameterizedTypeName GREATERTHAN
	| LESSTHAN AMPERSAND identifier GREATERTHAN XMLValue
		LESSTHANSLASH AMPERSAND identifier GREATERTHAN
	;

SetType: 
	SET  "{"  "}"
	| SET  "{"  ExtensionAndException  OptionalExtensionMarker  "}"
	| SET "{"  ComponentTypeLists  "}"
	;

SetValue: 
	LEFTBRACE ComponentValueList RIGHTBRACE
	| LEFTBRACE RIGHTBRACE
	;

XMLSetValue: 
	XMLComponentValueList
	| /* empty */
	;

SetOfType: 
	SET OF Type 
	| SET OF NamedType
	;

SetOfValue: 
	LEFTBRACE ValueList RIGHTBRACE
	| LEFTBRACE NamedValueList RIGHTBRACE
	| LEFTBRACE RIGHTBRACE
	;

XMLSetOfValue: 
	XMLValueList
	| XMLDelimitedItemList
	| XMLSpaceSeparatedList
	| /* empty */
	;

ChoiceType: 
	CHOICE LEFTBRACE AlternativeTypeLists RIGHTBRACE
	;

AlternativeTypeLists: 
	RootAlternativeTypeList
	| RootAlternativeTypeList COMMA ExtensionAndException
		ExtensionAdditionAlternatives OptionalExtensionMarker

RootAlternativeTypeList: 
	AlternativeTypeList
	;

ExtensionAdditionAlternatives: 
	COMMA ExtensionAdditionAlternativesList
	| /* empty */
	;

ExtensionAdditionAlternativesList: 
	ExtensionAdditionAlternative
	| ExtensionAdditionAlternativesList COMMA ExtensionAdditionAlternative
	;

ExtensionAdditionAlternative: 
	ExtensionAdditionAlternativesGroup
	| NamedType
	;

ExtensionAdditionAlternativesGroup: 
	DOUBLELEFTBRACKET VersionNumber AlternativeTypeList DOUBLERIGHTBRACKET
	;

AlternativeTypeList: 
	NamedType
	| AlternativeTypeList COMMA NamedType
	;

ChoiceValue: 
	identifier COLON Value
	;

XMLChoiceValue: 
	| LESSTHAN AMPERSAND identifier GREATERTHAN XMLValue
		LESSTHANSLASH AMPERSAND identifier GREATERTHAN
	;

SelectionType: 
	identifier LESSTHAN Type
	;

TaggedType: 
	Tag Type
	| Tag IMPLICIT Type
	| Tag EXPLICIT Type
	;

Tag: 
	LEFTBRACKET Class ClassNumber RIGHTBRACKET
	;

ClassNumber: 
	number 
	| DefinedValue
	;

Class: 
	UNIVERSAL
	| APPLICATION
	| PRIVATE
	| /* empty */
	;

TaggedValue: 
	Value
	;

XMLTaggedValue: 
	XMLValue
	;

EmbeddedPDVType: 
	EMBEDDED PDV
	;

EmbeddedPDVValue: 
	SequenceValue
	;

XMLEmbeddedPDVValue: 
	XMLSequenceValue
	;

ExternalType: 
	EXTERNAL
	;

ExternalValue: 
	SequenceValue
	;

XMLExternalValue: 
	XMLSequenceValue
	;

ObjectIdentifierType: 
	OBJECT IDENTIFIER
	;

ObjectIdentifierValue: 
	LEFTBRACE ObjIdComponentsList RIGHTBRACE
	| LEFTBRACE DefinedValue ObjIdComponentsList RIGHTBRACE
	;

ObjIdComponentsList: 
	ObjIdComponents
	| ObjIdComponents ObjIdComponentsList
	;

ObjIdComponents: 
	NameForm
	| NumberForm
	| NameAndNumberForm
	| DefinedValue
	;

NameForm: 
	identifier
	;

NumberForm: 
	number 
	| DefinedValue
	;

NameAndNumberForm: 
	identifier LEFTBRACE NumberForm RIGHTBRACE
	;

XMLObjectIdentifierValue: 
	XMLObjIdComponentList
	;

XMLObjIdComponentList: 
	XMLObjIdComponent
	| XMLObjIdComponent AMPERSAND PERIOD AMPERSAND XMLObjIdComponentList
	;

XMLObjIdComponent: 
	NameForm
	| XMLNumberForm
	| XMLNameAndNumberForm
	;

XMLNumberForm: 
	number
	;

XMLNameAndNumberForm: 
	identifier AMPERSAND LEFTPAREN AMPERSAND 
		XMLNumberForm AMPERSAND RIGHTPAREN
	;

RelativeOIDType: 
	RELATIVEOID
	;

RelativeOIDValue: 
	LEFTPAREN RelativeOIDComponentsList RIGHTPAREN
	;

RelativeOIDComponentsList: 
	RelativeOIDComponents
	| RelativeOIDComponents RelativeOIDComponentsList
	;

RelativeOIDComponents: 
	NumberForm
	| NameAndNumberForm
	| DefinedValue
	;

XMLRelativeOIDValue: 
	XMLRelativeOIDComponentList
	;

XMLRelativeOIDComponentList: 
	XMLRelativeOIDComponent
	| XMLRelativeOIDComponent AMPERSAND PERIOD AMPERSAND 
		XMLRelativeOIDComponentList

XMLRelativeOIDComponent: 
	XMLNumberForm
	| XMLNameAndNumberForm
	;

CharacterStringType: 
	RestrictedCharacterStringType 
	| UnrestrictedCharacterStringType
	;

RestrictedCharacterStringType: 
	BMPString
	| GeneralString
	| GraphicString
	| IA5String
	| ISO646String
	| NumericString
	| PrintableString
	| TeletexString
	| T61String
	| UniversalString
	| UTF8String
	| VideotexString
	| VisibleString
	;

RestrictedCharacterStringValue: 
	cstring 
	| CharacterStringList 
	| Quadruple 
	| Tuple
	;

CharacterStringList: 
	LEFTBRACE CharSyms RIGHTBRACE
	;

CharSyms: 
	CharsDefn 	
	| CharSyms COMMA CharsDefn
	;

CharsDefn: 
	cstring 
	| Quadruple 
	| Tuple 
	| DefinedValue
	;

Quadruple: 
	LEFTBRACE Group COMMA Plane COMMA Row COMMA Cell RIGHTBRACE
	;

Group: 
	number
	;

Plane: 
	number
	;

Row: 
	number
	;

Cell: 
	number
	;

Tuple: 
	LEFTBRACE TableColumn COMMA TableRow RIGHTBRACE
	;

TableColumn: 
	number
	;

TableRow: 
	number
	;

XMLRestrictedCharacterStringValue: 
	xmlcstring
	;

UnrestrictedCharacterStringType: 
	CHARACTER STRING
	;

CharacterStringValue: 
	RestrictedCharacterStringValue 
	| UnrestrictedCharacterStringValue
	;

XMLCharacterStringValue: 
	XMLRestrictedCharacterStringValue
	| XMLUnrestrictedCharacterStringValue
	;

UnrestrictedCharacterStringValue: 
	SequenceValue
	;

XMLUnrestrictedCharacterStringValue: 
	XMLSequenceValue
	;

UsefulType: 
	typereference
	;

ConstrainedType: 
	Type Constraint
	| TypeWithConstraint
	;

TypeWithConstraint: 
	SET Constraint OF Type
	| SET SizeConstraint OF Type
	| SEQUENCE Constraint OF Type
	| SEQUENCE SizeConstraint OF Type
	| SET Constraint OF NamedType
	| SET SizeConstraint OF NamedType
	| SEQUENCE Constraint OF NamedType
	| SEQUENCE SizeConstraint OF NamedType
	;

Constraint: 
	LEFTBRACE ConstraintSpec ExceptionSpec RIGHTBRACE
	;

ConstraintSpec: 
	SubtypeConstraint
	| GeneralConstraint
	;

ExceptionSpec: 
	EXCLAMATION ExceptionIdentification 
	| /* empty */
	;

ExceptionIdentification: 
	SignedNumber
	| DefinedValue
	| Type COLON Value
	;

SubtypeConstraint: 
	ElementSetSpecs
	;

ElementSetSpecs: 
	RootElementSetSpec
	| RootElementSetSpec COMMA ELLIPSIS 
	| RootElementSetSpec COMMA ELLIPSIS COMMA AdditionalElementSetSpec
	;

RootElementSetSpec: 
	ElementSetSpec
	;

AdditionalElementSetSpec: 
	ElementSetSpec
	;

ElementSetSpec: 
	Unions 
	| ALL Exclusions
	;

Unions: 
	Intersections
	| UElems UnionMark Intersections
	;

UElems: 
	Unions
	;

Intersections: 
	IntersectionElements
	| IElems IntersectionMark IntersectionElements
	;

IElems: 
	Intersections

IntersectionElements: 
	Elements 
	| Elems Exclusions
	;

Elems: 
	Elements
	;

Exclusions: 
	EXCEPT Elements
	;

UnionMark: 
	PIPE 
	| UNION

IntersectionMark: 
	CARET 	
	| INTERSECTION
	;

Elements: 
	SubtypeElements
	| ObjectSetElements
	| LEFTBRACE ElementSetSpec RIGHTBRACE
	;

SubtypeElements: 
	SingleValue
	| ContainedSubtype
	| ValueRange
	| PermittedAlphabet
	| SizeConstraint
	| TypeConstraint
	| InnerTypeConstraints
	| PatternConstraint
	;

SingleValue: 
	Value
	;

ContainedSubtype: 
	Includes Type
	;

Includes: 
	INCLUDES 
	| /* empty */
	;

ValueRange: 
	LowerEndpoint TWOPERIODS UpperEndpoint
	;

LowerEndpoint: 
	LowerEndValue 
	| LowerEndValue LESSTHAN
	;

UpperEndpoint: 
	UpperEndValue 
	| LESSTHAN UpperEndValue
	;

LowerEndValue: 
	Value 
	| MIN
	;

UpperEndValue: 
	Value 
	| MAX
	;

SizeConstraint: 
	SIZE Constraint
	;

PermittedAlphabet: 
	FROM Constraint
	;

TypeConstraint: 
	Type
	;

InnerTypeConstraints: 
	WITH COMPONENT SingleTypeConstraint
	| WITH COMPONENTS MultipleTypeConstraints
	;

SingleTypeConstraint: 
	Constraint
	;

MultipleTypeConstraints: 
	FullSpecification 
	| PartialSpecification
	;

FullSpecification: 
	LEFTBRACE TypeConstraints RIGHTBRACE
	;

PartialSpecification: 
	LEFTBRACE ELLIPSIS COMMA TypeConstraints RIGHTBRACE
	;

TypeConstraints: 
	NamedConstraint
	| NamedConstraint COMMA TypeConstraints
	;

NamedConstraint: 
	identifier ComponentConstraint
	;

ComponentConstraint: 
	ValueConstraint PresenceConstraint
	;

ValueConstraint: 
	Constraint 
	| /* empty */
	;

PresenceConstraint: 
	PRESENT 
	| ABSENT 
	| OPTIONAL 
	| /* empty */
	;

PatternConstraint: 
	PATTERN Value
	;

%%

main(int argc, char **argv)
{
	yyparse();
}
