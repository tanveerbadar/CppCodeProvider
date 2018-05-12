#include "collections.h"
#include "declarations.h"
#include "exceptions.h"
#include "expressions.h"
#include "functions.h"
#include "typedefinition.h"
#include "types.h"
#include <sstream>

namespace CPlusPlusCodeProvider
{
	using std::wstring;
	using std::invalid_argument;
	using std::wostream;
	using std::auto_ptr;
	using std::vector;
	using std::wostringstream;

	BasicType& BasicType::Assign( const CodeObject& object )
	{
		Type::Assign( cast< BasicType >( object ) , true );
		return *this;
	}

	BasicType& BasicType::Duplicate( ) const
	{
		return *new BasicType( *this );
	}

	BasicType::BasicType( const wstring& name )
		 : Type( validate( name ) )
	{
	}

	const wstring& BasicType::validate( const wstring& name )
	{
		const wstring integral [ ] = { L"int" , L"unsigned int" , L"signed int" , L"long" , L"unsigned long" , L"signed long" , L"short" , L"unsigned short" , L"signed short" , L"char" , L"signed char" , L"unsigned char" , L"wchar_t" , L"bool" , L"" } , basictypes [ ] = { L"float" , L"double" , L"long double" , L"" };
		for( unsigned int i = 0 ; integral [ i ].length( ) ; ++i )
			if( name.find( integral [ i ] ) != wstring::npos )
				return name;
		for( int i = 0 ; basictypes [ i ].length( ) ; i++ )
			if( name.find( basictypes [ i ] ) != wstring::npos )
				return name;
		if( name == L"void" )
			return name;
		throw invalid_argument( "" );
	}

	void BasicType::writetext( wostream& declos , wostream& , unsigned long decltabs , unsigned long ) const
	{
		BasicType::writetext( declos , decltabs );
	}

	void BasicType::writetext( wostream& os , unsigned long tabs ) const
	{
		os << wstring( tabs , FormattingData::IndentationCharacter ) << Name( );
	}

	void nestabletype::writetemplateattributes( wostream& os ) const
	{
		writetemplateattributesrecursive( os );
	}

	void nestabletype::writeelaboratedname( wostream& os ) const
	{
		writeelaboratednamerecursive( os );
	}

	Union& Union::Assign( const nestabletype& object )
	{
		Assign( cast< Union >( object ) , true );
		return *this;
	}

	void Union::Assign( const Union& ref , bool )
	{
		Type::Assign( ref , true );
		membervariables.reset( new MemberVariableCollection( *ref.membervariables ) );
		memberfunctions.reset( new MemberFunctionCollection( *ref.memberfunctions ) );
		memberoperators.reset( new MemberOperatorCollection( *ref.memberoperators ) );
	}

	Union& Union::Assign( const CodeObject& object )
	{
		Assign( cast< Union >( object ) , true );
		return *this;
	}

	Union& Union::Duplicate( ) const
	{
		return *new Union( *this );
	}

	Union::Union( const wstring& name )
		 : Type( name ) , membervariables( new MemberVariableCollection ) , memberfunctions( new MemberFunctionCollection ) , memberoperators( new MemberOperatorCollection ) , enclosingtype( 0 )
	{
	}

	Union::Union( const Union& other )
		: memberfunctions( new MemberFunctionCollection( *other.memberfunctions ) ) , memberoperators( new MemberOperatorCollection( *other.memberoperators ) ) , membervariables( new MemberVariableCollection( *other.membervariables ) ) , Type( other ) , enclosingtype( other.enclosingtype )
	{
	}

	bool Union::Inline( ) const
	{
		return is_inline;
	}

	Union& Union::Inline( bool flag )
	{
		is_inline = flag;
		return *this;
	}

	void Union::writetemplateattributesrecursive( wostream& os ) const
	{
		if( enclosingtype )
			enclosingtype -> writetemplateattributes( os );
		if( template_parameters -> size( ) )
		{
			os << L"template< ";
			TemplateParameterCollection::const_iterator i = template_parameters -> begin( );
			( *i ) -> write( os , 0 );
			for( ++i ; i != template_parameters -> end( ) ; ++i )
			{
				os << L" , ";
				( *i ) -> write( os , 0 );
			}
			os << L" > ";
		}
	}

	void Union::writeelaboratednamerecursive( wostream& os ) const
	{
		if( enclosingtype )
			enclosingtype -> writeelaboratedname( os );
		os << Name( );
		if( template_parameters -> size( ) )
		{
			os << L"< " ;
			TemplateParameterCollection::const_iterator i = template_parameters -> begin( );
			os << ( *i ) -> Name( );
			for( ++i ; i != template_parameters -> end( ) ; ++i )
				os << L" , " << ( *i ) -> Name( );
			os << L" >";
		}
		os << L"::";
	}

	void Union::writetext( wostream& os , unsigned long tabs ) const
	{
		Comment( ).write( os , tabs );
		if( !Comment( ).IsEmpty( ) )
			os << L'\n';
		wstring tab( tabs , FormattingData::IndentationCharacter );

		os << tab << L"union";

		os << Name( ) << L'\n' << tab << L"{\n";
		++tabs;
		tab.assign( wstring( tabs , FormattingData::IndentationCharacter ) );
		if( membervariables -> size( ) )
		{
			os << tab << "public:\n";
			for( MemberVariableCollection::const_iterator i = membervariables -> begin( ) ; i != membervariables -> end( ) ; ++i )
			{
				if( std::get< 1 >( *i ) == Public )
				{
					if (std::get< 2 >(*i))
						os << L"mutable ";
					std::get< 0 >(*i)->write(os, tabs);
				}
				os << L";\n";
			}
			os << tab << "private:\n";
			for( MemberVariableCollection::const_iterator i = membervariables -> begin( ) ; i != membervariables -> end( ) ; ++i )
			{
				if (std::get< 1 >(*i) == Private)
				{
					if (std::get< 1 >(*i))
						os << L"mutable ";
					std::get< 0 >(*i)->write(os, tabs);
				}
				os << L";\n";
			}
		}
		if( memberoperators -> size( ) )
		{
			os << tab << "public:\n";
			for( MemberOperatorCollection::const_iterator i = memberoperators -> begin( ) ; i != memberoperators -> end( ) ; ++i )
				if( ( *i ) -> Access( ) == Public )
					( *i ) -> write( os , tabs );
			os << tab << "private:\n";
			for( MemberOperatorCollection::const_iterator i = memberoperators -> begin( ) ; i != memberoperators -> end( ) ; ++i )
				if( ( *i ) -> Access( ) == Private )
					( *i ) -> write( os , tabs );
		}
		if( memberfunctions -> size( ) )
		{
			os << tab << "public:\n";
			for( MemberFunctionCollection::const_iterator i = memberfunctions -> begin( ) ; i != memberfunctions -> end( ) ; ++i )
				if( ( *i ) -> Access( ) == Public )
					( *i ) -> write( os , tabs );
			os << tab << "private:\n";
			for( MemberFunctionCollection::const_iterator i = memberfunctions -> begin( ) ; i != memberfunctions -> end( ) ; ++i )
				if( ( *i ) -> Access( ) == Private )
					( *i ) -> write( os , tabs );
		}
		--tabs;
		tab.assign( wstring( tabs , FormattingData::IndentationCharacter ) );
		os << tab << L"};\n";
	}

	void Union::writetext( wostream& declos , wostream& defos , unsigned long decltabs , unsigned long deftabs ) const
	{
		Comment( ).write( declos , decltabs );
		if( !Comment( ).IsEmpty( ) )
			declos << L'\n';
		wstring tab( decltabs , FormattingData::IndentationCharacter );
		declos << tab;

		declos << L"union";

		if( Name( ).size( ) )
			declos << L' ' << Name( );
		declos << L'\n';
		declos << tab << L"{\n";
		++decltabs;
		tab.assign( wstring( decltabs , FormattingData::IndentationCharacter ) );
		declos << tab << "public:\n";
		wstring temp;

		for( MemberVariableCollection::const_iterator i = membervariables -> begin( ) ; i != membervariables -> end( ) ; ++i )
		{
			if (std::get< 1 >(*i) == Public)
				std::get< 0 >(*i)->write(declos, decltabs);
			declos << L";\n";
		}
		declos << tab << "private:\n";
		for( MemberVariableCollection::const_iterator i = membervariables -> begin( ) ; i != membervariables -> end( ) ; ++i )
		{
			if (std::get< 1 >(*i) == Private)
				std::get< 0 >(*i)->write(declos, decltabs);
			declos << L";\n";
		}
		declos << tab << "public:\n";
		for( MemberOperatorCollection::const_iterator i = memberoperators -> begin( ) ; i != memberoperators -> end( ) ; ++i )
			if( ( *i ) -> Access( ) == Public )
				if( !( *i ) -> Inline( ) )
					( *i ) -> write( declos , defos , decltabs , deftabs );
				else
				{
					bool flag = ( *i ) -> separate;
					( *i ) -> separate = false;
					( *i ) -> write( declos , decltabs );
					( *i ) -> separate = flag;
				}
		declos << tab << "private:\n";
		for( MemberOperatorCollection::const_iterator i = memberoperators -> begin( ) ; i != memberoperators -> end( ) ; ++i )
			if( ( *i ) -> Access( ) == Private )
				if( !( *i ) -> Inline( ) )
					( *i ) -> write( declos , defos , decltabs , deftabs );
				else
				{
					bool flag = ( *i ) -> separate;
					( *i ) -> separate = false;
					( *i ) -> write( declos , decltabs );
					( *i ) -> separate = flag;
				}
		declos << tab << "public:\n";
		for( MemberFunctionCollection::const_iterator i = memberfunctions -> begin( ) ; i != memberfunctions -> end( ) ; ++i )
			if( ( *i ) -> Access( ) == Public )
				if( !( *i ) -> Inline( ) )
					( *i ) -> write( declos , defos , decltabs , deftabs );
				else
				{
					bool flag = ( *i ) -> separate;
					( *i ) -> separate = false;
					( *i ) -> write( declos , decltabs );
					( *i ) -> separate = flag;
				}
		declos << tab << "private:\n";
		for( MemberFunctionCollection::const_iterator i = memberfunctions -> begin( ) ; i != memberfunctions -> end( ) ; ++i )
			if( ( *i ) -> Access( ) == Private )
				if( !( *i ) -> Inline( ) )
					( *i ) -> write( declos , defos , decltabs , deftabs );
				else
				{
					bool flag = ( *i ) -> separate;
					( *i ) -> separate = false;
					( *i ) -> write( declos , decltabs );
					( *i ) -> separate = flag;
				}
		--decltabs;
		tab.assign( wstring( decltabs , FormattingData::IndentationCharacter ) );
		declos << tab << L"};\n";
	}

	MemberVariableCollection& Union::MemberVariables( ) const
	{
		return *membervariables;
	}

	MemberFunctionCollection& Union::MemberFunctions( ) const
	{
		if( !Name( ).size( ) )
			throw NotSupportedException( "Only named unions can have member functions." );
		return *memberfunctions;
	}

	MemberOperatorCollection& Union::MemberOperators( ) const
	{
		if( !Name( ).size( ) )
			throw NotSupportedException( "Only named unions can have member operators." );
		return *memberoperators;
	}

	TemplateParameter::TemplateParameter( const wstring& name )
		: Type( name )
	{
	}

	void TemplateParameter::writetext( wostream& declos , wostream& , unsigned long decltabs , unsigned long ) const
	{
		writetext( declos , decltabs );
	}

	void TemplateParameter::writetext( wostream& os , unsigned long ) const
	{
		Comment( ).write( os , 0 );
		if( !Comment( ).IsEmpty( ) )
			os << L'\n';
		os << L"typename " << Name( );
	}

	NontypeParameter& NontypeParameter::Duplicate( ) const
	{
		return *new NontypeParameter( *this );
	}

	NontypeParameter& NontypeParameter::Assign( const CodeObject& object )
	{
		Type::Assign( cast< NontypeParameter >( object ) , true );
		return *this;
	}

	NontypeParameter::NontypeParameter( const wstring& name )
		 : TemplateParameter( validate( name ) )
	{
	}

	bool NontypeParameter::IsIntegral( ) const
	{
		return is_integral;
	}

	const wstring& NontypeParameter::validate( const wstring& name ) const
	{
		const wstring integral [ ] = { L"int" , L"unsigned int" , L"signed int" , L"long" , L"unsigned long" , L"signed long" , L"short" , L"unsigned short" , L"signed short" , L"char" , L"signed char" , L"unsigned char" , L"wchar_t" , L"bool" , L"" } , basictypes [ ] = { L"float" , L"double" , L"long double" , L"" };
		for( unsigned int i = 0 ; i < integral [ i ].length( ) ; ++i )
			if( name.find( integral [ i ] ) != wstring::npos )
			{
				is_integral = true;
				return name;
			}
		for( unsigned int i = 0 ; i < basictypes [ i ].length( ) ; i++ )
			if( name.find( basictypes [ i ] ) != wstring::npos )
				return name;
		if( name.find( L'*' ) == wstring::npos && name.find( L'&' ) == wstring::npos )
			throw invalid_argument( "" );
		return name;
	}

	TypedParameter& TypedParameter::Assign( const CodeObject& object )
	{
		Type::Assign( cast< TypedParameter >( object ) , true );
		return *this;
	}

	TypedParameter& TypedParameter::Duplicate( ) const
	{
		return *new TypedParameter( *this );
	}

	TypedParameter::TypedParameter( const wstring& name )
		 : TemplateParameter( validate( name ) )
	{
	}

	const wstring& TypedParameter::validate( const wstring& name ) const
	{
		try
		{
			BasicType::validate( name );
			throw invalid_argument( "Please provide a parameter different from primitive types." );
		}
		catch( ... )
		{
		}
		return name;
	}

	TemplateTemplateParameter& TemplateTemplateParameter::Assign( const CodeObject& object )
	{
		const TemplateTemplateParameter& ref = cast< TemplateTemplateParameter >( object );
		arguments = ref.arguments;
		Type::Assign( ref , true );
		return *this;
	}

	TemplateTemplateParameter& TemplateTemplateParameter::Duplicate( ) const
	{
		return *new TemplateTemplateParameter( *this );
	}

	TemplateTemplateParameter::TemplateTemplateParameter( const wstring& name , unsigned int args )
		 : TemplateParameter( validate( name ) ) , arguments( args )
	{
	}

	const wstring& TemplateTemplateParameter::validate( const wstring& name ) const
	{
		try
		{
			BasicType::validate( name );
			throw invalid_argument( "Please provide a parameter different from primitive types." );
		}
		catch( ... )
		{
		}
		return name;
	}

	void TemplateTemplateParameter::writetext( wostream& os , unsigned long ) const
	{
		os << L"template< ";
		for( unsigned int i = 0 ; i < arguments-1 ; i++ )
			os << L"typename , ";
		os << L"typename > class " << Name( );
	}

	TypedefinedType& TypedefinedType::Assign( const CodeObject& object )
	{
		Type::Assign( cast< TypedefinedType >( object ) , true );
		return *this;
	}

	TypedefinedType& TypedefinedType::Duplicate( ) const
	{
		return *new TypedefinedType( *this );
	}

	TypedefinedType::TypedefinedType( const copying_pointer< Typedefinition >& typedefinition , const wstring& name )
		 : Type( validate( typedefinition , name ) )
	{
	}

	void TypedefinedType::writetext( wostream& declos , wostream& , unsigned long decltabs , unsigned long ) const
	{
		TypedefinedType::writetext( declos , decltabs );
	}

	void TypedefinedType::writetext( wostream& os , unsigned long ) const
	{
		os << Name( );
	}

	const wstring& TypedefinedType::validate( const copying_pointer< Typedefinition >& typedefinition , const wstring& name )
	{
		for( StringCollection::const_iterator iter = typedefinition -> Synonyms( ).begin( ) ; iter != typedefinition -> Synonyms( ).end( ) ; ++iter )
			if( name == *iter )
				return name;
		throw invalid_argument( "Given name does not exist as a typedefinition." );
	}

	void Enumeration::Assign( const Enumeration& ref , bool )
	{
		Type::Assign( ref , true );
		enumerators = ref.enumerators;
	}

	Enumeration& Enumeration::Assign( const CodeObject&object )
	{
		Assign( cast< Enumeration >( object ) , true );
		return *this;
	}

	Enumeration& Enumeration::Assign( const nestabletype& object )
	{
		Assign( cast< Enumeration >( object ) , true );
		return *this;
	}

	Enumeration& Enumeration::Duplicate( ) const
	{
		return *new Enumeration( *this );
	}

	Enumeration::Enumeration( const wstring& str )
		: Type( str ) , enclosingtype( 0 )
	{
	}

	Enumeration::Enumeration( const wstring& str , const enumeratorlist& enumerator )
		 : Type( str ) , enumerators( enumerator ) , enclosingtype( 0 )
	{
	}

	Enumeration::Enumeration( const Enumeration& other )
		 : Type( other ) , enumerators( other.enumerators )
	{
	}

	enumeratorlist& Enumeration::Enumerators( )
	{
		return enumerators;
	}

	void Enumeration::writetext( wostream& os , unsigned long tabs ) const
	{
		Comment( ).write( os , tabs );
		if( !Comment( ).IsEmpty( ) )
			os << L'\n';
		wstring tab( tabs , FormattingData::IndentationCharacter );
		os << tab;

		os << L"enum";

		if( Name( ).length( ) )
			os << Name( );
		os << L'\n' << tab << L"{\n";
		tab.assign( tabs + 1 , FormattingData::IndentationCharacter );
		if( enumerators.size( ) > 0 )
		{
			enumeratorlist::const_iterator iter = enumerators.begin( );
			iter -> first.first -> write( os , tabs );
			if( !iter -> first.first -> IsEmpty( ) )
				os << tab;
			os << iter -> first.second;
			if( *PrimitiveExpression::emptyexpression != *( iter -> second ) )
			{
				os << L" = ";
				iter -> second -> write( os , 0 );
			}
			for( ++iter ; iter != enumerators.end( ) ; ++iter )
			{
				os << L" , \n";
				iter -> first.first -> write( os , tabs );
				if( !iter -> first.first -> IsEmpty( ) )
					os << tab;
				os << iter -> first.second;
				if( *PrimitiveExpression::emptyexpression != *( iter -> second ) )
				{
					os << L" = ";
					iter -> second -> write( os , 0 );
				}
			}
			os << L'\n';
		}
		os << tab << L"};\n";
	}

	void Enumeration::writetext( wostream& declos , wostream& , unsigned long decltabs , unsigned long ) const
	{
		writetext( declos , decltabs );
	}

	void Enumeration::writetemplateattributesrecursive( wostream& ) const
	{
		throw NotSupportedException( "An enumeration cannot have template parameters." );
	}

	void Enumeration::writeelaboratednamerecursive( wostream& ) const
	{
		throw NotSupportedException( "An enumeration cannot be written out of containing type." );
	}

	BaseType::BaseType( UserDefinedType& u , const StringCollection& args , AccessRestrictions a )
		: access( a ) , actual( &u ) , arguments( new StringCollection( args ) )
	{
	}

	BaseType::BaseType( const BaseType& other )
		: access( other.access ) , actual( other.actual ) , arguments( new StringCollection( *other.arguments ) )
	{
	}

	BaseType& BaseType::operator = ( const BaseType& other )
	{
		if( this != &other )
		{
			arguments.reset( new StringCollection( *other.arguments ) );
			access = other.access , actual = other.actual;
		}
		return *this;
	}

	UserDefinedType& BaseType::Type( ) const
	{
		return *actual;
	}

	bool BaseType::Virtual( ) const
	{
		return is_virtual;
	}

	BaseType& BaseType::Virtual( bool flag )
	{
		is_virtual = flag;
		return *this;
	}

	void BaseType::write( wostream& os ) const
	{
		switch( access )
		{
		case Public:
			os << L"public ";
			break;
		case Private:
			os << L"private ";
			break;
		case Protected:
			os << L"protected ";
			break;
		}
		if( is_virtual )
			os << L"virtual ";
		os << actual -> instantiate( *arguments );
	}

	UserDefinedType& UserDefinedType::Duplicate( ) const
	{
		return *new UserDefinedType( *this );
	}

	void UserDefinedType::Assign( const UserDefinedType& ref , bool )
	{
		Type::Assign( ref , true );
		typedefinitions.reset( new MemberTypedefinitionCollection( *ref.typedefinitions ) );
		memberuserdefinedtypes.reset( new NestedTypeCollection( *ref.memberuserdefinedtypes ) );
		memberfunctions.reset( new MemberFunctionCollection( *ref.memberfunctions ) );
		memberenumerations.reset( new MemberEnumerationCollection( *ref.memberenumerations ) );
		memberunions.reset( new MemberUnionCollection( *ref.memberunions ) );
		memberoperators.reset( new MemberOperatorCollection( *ref.memberoperators ) );
		membervariables.reset( new MemberVariableCollection( *ref.membervariables ) );
		constructors.reset( new ConstructorCollection( *ref.constructors ) );
		destructor = ref.destructor;
		template_parameters.reset( new TemplateParameterCollection( *ref.template_parameters ) );
		bases.reset( new BaseTypeCollection( *ref.bases ) );
		specialized_parameters.reset( new TypeCollection( *ref.specialized_parameters ) );
		is_sealed = ref.is_sealed;
		usertype = ref.usertype;
		enclosingtype = ref.enclosingtype;
	}

	UserDefinedType& UserDefinedType::Assign( const nestabletype& object )
	{
		Assign( cast< UserDefinedType >( object ) , true );
		return *this;
	}

	UserDefinedType& UserDefinedType::Assign( const CodeObject& object )
	{
		Assign( cast< UserDefinedType >( object ) , true );
		return *this;
	}

	UserDefinedType::UserDefinedType( const wstring& name , UserType type )
		 : Type( name ) , typedefinitions( new MemberTypedefinitionCollection ) , memberuserdefinedtypes( new NestedTypeCollection ) , memberfunctions( new MemberFunctionCollection ) , memberenumerations( new MemberEnumerationCollection ) , memberunions( new MemberUnionCollection ) , membervariables( new MemberVariableCollection ) , memberoperators( new MemberOperatorCollection ) , constructors( new ConstructorCollection ) , template_parameters( new TemplateParameterCollection ) , usertype( type ) , destructor( 0 ) , specialized_parameters( new TypeCollection ) , bases( new BaseTypeCollection ) , enclosingtype( 0 ) , nestinglevel( 0 )
	{
	}

	UserDefinedType::UserDefinedType( const UserDefinedType& other )
		 : Type( other ) , usertype( other.usertype ) , typedefinitions( new MemberTypedefinitionCollection( *other.typedefinitions ) ) , memberuserdefinedtypes( new NestedTypeCollection( *other.memberuserdefinedtypes ) ) , memberfunctions( new MemberFunctionCollection( *other.memberfunctions ) ) , memberenumerations( new MemberEnumerationCollection( *other.memberenumerations ) ) , memberunions( new MemberUnionCollection( *other.memberunions ) ) , membervariables( new MemberVariableCollection( *other.membervariables ) ) , memberoperators( new MemberOperatorCollection( *other.memberoperators ) ) , constructors( new ConstructorCollection( *other.constructors ) ) , destructor( other.destructor ) , template_parameters( new TemplateParameterCollection( *other.template_parameters ) ) , specialized_parameters( other.specialized_parameters.get( ) ? new TypeCollection( *other.specialized_parameters ) : 0 ) , bases( other.bases.get( ) ? new BaseTypeCollection( *other.bases ) : 0 ) , enclosingtype( other.enclosingtype ) , nestinglevel( 0 )
	{
	}

	bool UserDefinedType::IsSpecialized( ) const
	{
		return specialized_parameters.get( ) && specialized_parameters -> size( ) != 0;
	}

	BaseTypeCollection& UserDefinedType::BaseTypes( ) const
	{
		return *bases;
	}

	bool UserDefinedType::Template( ) const
	{
		return template_parameters -> size( ) != 0;
	}

	bool UserDefinedType::Inline( ) const
	{
		return is_inline;
	}

	UserDefinedType& UserDefinedType::Inline( bool flag )
	{
		is_inline = flag;
		return *this;
	}

	TemplateParameterCollection& UserDefinedType::TemplateParameters( ) const
	{
		return *template_parameters;
	}

	bool UserDefinedType::IsAbstract( ) const
	{
		return destructor ? destructor -> Pure( ) : false;
	}

	UserDefinedType& UserDefinedType::Abstract( bool flag )
	{
		if( !destructor )
			destructor = &Destructor::Instantiate( *this , Public ) -> Duplicate( );
		destructor -> Pure( flag );
		return *this;
	}

	nestabletype* UserDefinedType::EnclosingType( ) const
	{
		return enclosingtype;
	}

	UserDefinedType& UserDefinedType::EnclosingType( nestabletype* type )
	{
		enclosingtype = type;
		return *this;
	}

	UserDefinedType::UserType UserDefinedType::Usertype( ) const
	{
		return usertype;
	}

	MemberTypedefinitionCollection& UserDefinedType::Typedefinitions( ) const
	{
		return *typedefinitions;
	}

	NestedTypeCollection& UserDefinedType::MemberTypes( ) const
	{
		return *memberuserdefinedtypes;
	}

	MemberFunctionCollection& UserDefinedType::MemberFunctions( ) const
	{
		return *memberfunctions;
	}

	MemberEnumerationCollection& UserDefinedType::MemberEnumerations( ) const
	{
		return *memberenumerations;
	}

	MemberUnionCollection& UserDefinedType::MemberUnions( ) const
	{
		return *memberunions;
	}

	MemberVariableCollection& UserDefinedType::MemberVariables( ) const
	{
		return *membervariables;
	}

	MemberOperatorCollection& UserDefinedType::MemberOperators( ) const
	{
		return *memberoperators;
	}

	bool UserDefinedType::IsSealed( ) const
	{
		return is_sealed;
	}

	UserDefinedType& UserDefinedType::Sealed( bool flag )
	{
		is_sealed = flag;
		AccessRestrictions access = flag ? Private : Public;
		for( ConstructorCollection::const_iterator i = Constructors( ).begin( ) , j = Constructors( ).end( ) ; i != j ; i++ )
		{
			copying_pointer< Constructor > p = *i;
			p -> Access( access );
		}
		return *this;
	}

	ConstructorCollection& UserDefinedType::Constructors( ) const
	{
		return *constructors;
	}

	Destructor* UserDefinedType::Destructor( ) const
	{
		return destructor;
	}

	UserDefinedType& UserDefinedType::Destructor( copying_pointer< class Destructor >& dstr )
	{
		destructor = dstr.release( );
		return *this;
	}

	bool UserDefinedType::hasbacklog( ) const
	{
		return !render_backlog.empty( );
	}

	void UserDefinedType::writeelaboratednamerecursive( wostream& os ) const
	{
		if( enclosingtype )
			enclosingtype -> writeelaboratedname( os );
		os << UserDefinedType::Name( );
		if( template_parameters -> size( ) )
		{
			os << L"< " ;
			TemplateParameterCollection::const_iterator i = template_parameters -> begin( );
			os << ( *i ) -> Name( );
			for( ++i ; i != template_parameters -> end( ) ; ++i )
				os << L" , " << ( *i ) -> Name( );
			os << L" >";
		}
		os << L"::";
	}

	wstring UserDefinedType::instantiate( ) const
	{
		return instantiate( StringCollection( ) );
	}

	wstring UserDefinedType::instantiate( const StringCollection& templatearguments ) const
	{
		if( templatearguments.size( ) != template_parameters -> size( ) )
			throw invalid_argument( "Template arguments must match the number of template parameters." );
		std::wostringstream os;
		os << UserDefinedType::Name( );
		if( templatearguments.size( ) )
		{
			StringCollection::const_iterator iter = templatearguments.begin( );
			os << L"< " << *iter;
			for( ++iter ; iter != templatearguments.end( ) ; ++iter )
				os << L" , " << *iter;
			os << L" > ";
		}
		return os.str( );
	}

	void UserDefinedType::declaration( wostream& os , unsigned long tabs ) const
	{
		os << wstring( tabs , FormattingData::IndentationCharacter );
		writetemplateattributes( os );
		os << ( usertype == Class ? L" class " : L" struct " ) << Name( ) << L";\n";
	}

	void UserDefinedType::writetemplateattributesrecursive( wostream& os ) const
	{
		if( enclosingtype )
			enclosingtype -> writetemplateattributes( os );
		if( template_parameters -> size( ) )
		{
			os << L"template< ";
			TemplateParameterCollection::const_iterator i = template_parameters -> begin( );
			( *i ) -> write( os , 0 );
			for( ++i ; i != template_parameters -> end( ) ; ++i )
			{
				os << L" , ";
				( *i ) -> write( os , 0 );
			}
			os << L" > ";
		}
	}

	void UserDefinedType::writespecializedparameters( wostream& os ) const
	{
		if( specialized_parameters -> size( ) )
		{
			TypeCollection::const_iterator iter = specialized_parameters -> begin( );
			os << L"< " << ( *iter ) -> Name( );
			for( ++iter ; iter != specialized_parameters -> end( ) ; ++iter )
				os << L" , " << ( *iter ) -> Name( );
			os << L" > ";
		}
	}

	void UserDefinedType::writebasetypes( wostream& os ) const
	{
		if( bases -> size( ) )
		{
			os << L" : ";
			BaseTypeCollection::const_iterator iter = bases -> begin( );
			iter -> write( os );
			for( ++iter ; iter != bases -> begin( ) ; ++iter )
			{
				os << L" , ";
				iter -> write( os );
			}
		}
	}

	void UserDefinedType::writemembers( wostream& declos , wostream& defos , unsigned long decltabs , unsigned long deftabs ) const
	{
		render_backlog.clear( );
		wostringstream public_decstream , protected_decstream , private_decstream;
		wstring deftab( deftabs , FormattingData::IndentationCharacter ) , dectab( decltabs , FormattingData::IndentationCharacter );
		++deftabs , ++decltabs;
		for( MemberEnumerationCollection::const_iterator iter = memberenumerations -> begin( ) ; iter != memberenumerations -> end( ) ; ++iter )
			if( iter -> first -> Name( ).size( ) )
				switch( iter -> second )
				{
				case Public:
					iter -> first -> write( public_decstream , decltabs );
					public_decstream << L'\n';
					break;
				case Protected:
					iter -> first -> write( protected_decstream , decltabs );
					protected_decstream << L'\n';
					break;
				case Private:
					iter -> first -> write( private_decstream , decltabs );
					private_decstream << L'\n';
					break;
				}

		for( MemberUnionCollection::const_iterator iter = memberunions -> begin( ) ; iter != memberunions -> end( ) ; ++iter )
			if( iter -> first -> Name( ).size( ) )
				switch( iter -> second )
				{
				case Public:
					if( iter -> first -> Inline( ) )
						iter -> first -> write( public_decstream , decltabs );
					else
						iter -> first -> write( public_decstream , defos , decltabs , deftabs );
					public_decstream << L'\n';
					break;
				case Protected:
					if( iter -> first -> Inline( ) )
						iter -> first -> write( protected_decstream , decltabs );
					else
						iter -> first -> write( protected_decstream , defos , decltabs , deftabs );
					protected_decstream << L'\n';
					break;
				case Private:
					if( iter -> first -> Inline( ) )
						iter -> first -> write( private_decstream , decltabs );
					else
						iter -> first -> write( private_decstream, defos , decltabs , deftabs );
					private_decstream << L'\n';
					break;
				}

		for( NestedTypeCollection::const_iterator iter = memberuserdefinedtypes -> begin( ) ; iter != memberuserdefinedtypes -> end( ) ; ++iter )
		{
			bool flag = true;
			for( BaseTypeCollection::const_iterator base_iter = iter -> first -> BaseTypes( ).begin( ) ; flag && base_iter != iter -> first -> BaseTypes( ).end( ) ; ++base_iter )
				if( &base_iter -> Type( ) == this )
					flag = false;
			if( flag )
			{
				++iter -> first -> nestinglevel;
				switch( iter -> second )
				{
				case Public:
					if( iter -> first -> Inline( ) )
					{
						iter -> first -> write( public_decstream , defos , decltabs , deftabs );
						public_decstream << L'\n';
					}
					else
					{
						iter -> first -> declaration( public_decstream , decltabs );
						public_decstream << L'\n';
						render_backlog.push_back( &*( iter -> first ) );
					}
					break;
				case Protected:
					if( iter -> first -> Inline( ) )
					{
						iter -> first -> write( protected_decstream , defos , decltabs , deftabs );
						protected_decstream << L'\n';
					}
					else
					{
						iter -> first -> declaration( protected_decstream , decltabs );
						protected_decstream << L'\n';
						render_backlog.push_back( &*( iter -> first ) );
					}
					break;
				case Private:
					if( iter -> first -> Inline( ) )
					{
						iter -> first -> write( private_decstream , defos , decltabs , deftabs );
						private_decstream << L'\n';
					}
					else
					{
						iter -> first -> declaration( private_decstream , decltabs );
						private_decstream << L'\n';
						render_backlog.push_back( &*( iter -> first ) );
					}
					break;
				}
			}
			else
				render_backlog.push_back( &*( iter -> first ) );
		}

		for( MemberVariableCollection::iterator iter = membervariables -> begin( ) ; iter != membervariables -> end( ) ; ++iter )
			switch (std::get< 1 >(*iter))
			{
			case Public:
				if (std::get< 0 >(*iter)->Specifier().Static())
					if (std::get< 0 >(*iter)->Specifier().Constant() && BasicType::validate(std::get< 0 >(*iter)->Specifier().Type().Name()).size())
					{
					bool flag = std::get< 0 >(*iter)->Specifier().Extern();
					std::get< 0 >(*iter)->Specifier().Extern(false);
					std::get< 0 >(*iter)->write(public_decstream, decltabs);
					std::get< 0 >(*iter)->Specifier().Extern(flag);
					}
					else
						std::get< 0 >(*iter)->write(public_decstream, defos, decltabs, deftabs);
				else
					std::get< 0 >(*iter)->declaration(public_decstream, decltabs);
				break;
			case Protected:
				if (std::get< 0 >(*iter)->Specifier().Static())
					if (std::get< 0 >(*iter)->Specifier().Constant() && BasicType::validate(std::get< 0 >(*iter)->Specifier().Type().Name()).size())
					{
					bool flag = std::get< 0 >(*iter)->Specifier().Extern();
					std::get< 0 >(*iter)->Specifier().Extern(false);
					std::get< 0 >(*iter)->write(protected_decstream, decltabs);
					std::get< 0 >(*iter)->Specifier().Extern(flag);
					}
					else
						std::get< 0 >(*iter)->write(protected_decstream, defos, decltabs, deftabs);
				else
					std::get< 0 >(*iter)->declaration(protected_decstream, decltabs);
				break;
			case Private:
				if (std::get< 0 >(*iter)->Specifier().Static())
					if (std::get< 0 >(*iter)->Specifier().Constant() && BasicType::validate(std::get< 0 >(*iter)->Specifier().Type().Name()).size())
					{
					bool flag = std::get< 0 >(*iter)->Specifier().Extern();
					std::get< 0 >(*iter)->Specifier().Extern(false);
					std::get< 0 >(*iter)->write(private_decstream, decltabs);
					std::get< 0 >(*iter)->Specifier().Extern(flag);
					}
					else
						std::get< 0 >(*iter)->write(private_decstream, defos, decltabs, deftabs);
				else
					std::get< 0 >(*iter)->declaration(private_decstream, decltabs);
				break;
			}

		for( MemberFunctionCollection::const_iterator iter = memberfunctions -> begin( ) ; iter != memberfunctions -> end( ) ; ++iter )
			switch( ( *iter ) -> Access( ) )
			{
			case Public:
				if( ( *iter ) -> Inline( ) && !( *iter ) -> Pure( ) )
					( *iter ) -> write( public_decstream , decltabs );
				else
				{
					if( ( *iter ) -> ForceBody( ) )
						( *iter ) -> write( public_decstream , defos , decltabs , deftabs );
					else
						( *iter ) -> declaration( public_decstream , decltabs );
				}
				public_decstream << L'\n';
				break;
			case Protected:
				if( ( *iter ) -> Inline( ) && !( *iter ) -> Pure( ) )
					( *iter ) -> write( protected_decstream , decltabs );
				else
				{
					if( ( *iter ) -> ForceBody( ) )
						( *iter ) -> write( protected_decstream , defos , decltabs , deftabs );
					else
						( *iter ) -> declaration( protected_decstream , decltabs );
				}
				protected_decstream << L'\n';
				break;
			case Private:
				if( ( *iter ) -> Inline( ) && !( *iter ) -> Pure( ) )
					( *iter ) -> write( private_decstream , decltabs );
				else
				{
					if( ( *iter ) -> ForceBody( ) )
						( *iter ) -> write( private_decstream , defos , decltabs , deftabs );
					else
						( *iter ) -> declaration( private_decstream , decltabs );
				}
				private_decstream << L'\n';
				break;
			}

		for( MemberOperatorCollection::const_iterator iter = memberoperators -> begin( ) ; iter != memberoperators -> end( ) ; ++iter )
			switch( ( *iter ) -> Access( ) )
			{
			case Public:
				if( ( *iter ) -> Inline( ) && !( *iter ) -> Pure( ) )
					( *iter ) -> write( public_decstream , decltabs );
				else
				{
					if( ( *iter ) -> ForceBody( ) )
						( *iter ) -> write( public_decstream , defos , decltabs , deftabs );
					else
						( *iter ) -> declaration( public_decstream , decltabs );
				}
				public_decstream << L'\n';
				break;
			case Protected:
				if( ( *iter ) -> Inline( ) && !( *iter ) -> Pure( ) )
					( *iter ) -> write( protected_decstream , decltabs );
				else
				{
					if( ( *iter ) -> ForceBody( ) )
						( *iter ) -> write( protected_decstream , defos , decltabs , deftabs );
					else
						( *iter ) -> declaration( protected_decstream , decltabs );
				}
				protected_decstream << L'\n';
				break;
			case Private:
				if( ( *iter ) -> Inline( ) && !( *iter ) -> Pure( ) )
					( *iter ) -> write( private_decstream , decltabs );
				else
				{
					if( ( *iter ) -> ForceBody( ) )
						( *iter ) -> write( private_decstream , defos , decltabs , deftabs );
					else
						( *iter ) -> declaration( private_decstream , decltabs );
				}
				private_decstream << L'\n';
				break;
			}

		for( ConstructorCollection::const_iterator iter = constructors -> begin( ) ; iter != constructors -> end( ) ; ++iter )
			switch( ( *iter ) -> Access( ) )
			{
			case Public:
				if( ( *iter ) -> Inline( ) )
					( *iter ) -> write( public_decstream , decltabs );
				else
					( *iter ) -> write( public_decstream , defos , decltabs , deftabs );
				public_decstream << L'\n';
				break;
			case Protected:
				if( ( *iter ) -> Inline( ) )
					( *iter ) -> write( protected_decstream , decltabs );
				else
					( *iter ) -> write( protected_decstream , defos , decltabs , deftabs );
				protected_decstream << L'\n';
				break;
			case Private:
				if( ( *iter ) -> Inline( ) )
					( *iter ) -> write( private_decstream , decltabs );
				else
					( *iter ) -> write( private_decstream , defos , decltabs , deftabs );
				private_decstream << L'\n';
				break;
			}

		if( destructor )
			switch( destructor -> Access( ) )
			{
			case Public:
				if( destructor -> Inline( ) && !destructor -> Pure( ) )
					destructor -> write( public_decstream , decltabs );
				else
					destructor -> write( public_decstream , defos , decltabs , deftabs );
				break;
			case Protected:
				if( destructor -> Inline( ) && !destructor -> Pure( ) )
					destructor -> write( protected_decstream , decltabs );
				else
					destructor -> write( protected_decstream , defos , decltabs , deftabs );
				break;
			case Private:
				if( destructor -> Inline( ) && !destructor -> Pure( ) )
					destructor -> write( private_decstream , decltabs );
				else
					destructor -> write( private_decstream , private_decstream , decltabs , deftabs );
				break;
			}

		wstring public_decstr = public_decstream.str( ) , protected_decstr = protected_decstream.str( ) , private_decstr = private_decstream.str( );

		if( public_decstr.size( ) )
			declos << dectab << L"public:\n" << public_decstr;
		if( protected_decstr.size( ) )
			declos << dectab << L"protected:\n" << protected_decstr;
		if( private_decstr.size( ) )
			declos << dectab << L"private:\n" << private_decstr;
	}

	void UserDefinedType::writemembers( wostream& os , unsigned long tabs ) const
	{
		render_backlog.clear( );
		wostringstream public_stream , protected_stream , private_stream;
		wstring tab( tabs , FormattingData::IndentationCharacter );
		++tabs;

		for( MemberEnumerationCollection::const_iterator iter = memberenumerations -> begin( ) ; iter != memberenumerations -> end( ) ; ++iter )
			if( iter -> first -> Name( ).size( ) )
				switch( iter -> second )
				{
				case Public:
					iter -> first -> write( public_stream , tabs );
					public_stream << L'\n';
					break;
				case Protected:
					iter -> first -> write( protected_stream , tabs );
					protected_stream << L'\n';
					break;
				case Private:
					iter -> first -> write( private_stream , tabs );
					private_stream << L'\n';
					break;
				}

		for( MemberUnionCollection::const_iterator iter = memberunions -> begin( ) ; iter != memberunions -> end( ) ; ++iter )
			if( iter -> first -> Name( ).size( ) )
				switch( iter -> second )
				{
				case Public:
					iter -> first -> write( public_stream , tabs );
					public_stream << L'\n';
					break;
				case Protected:
					iter -> first -> write( protected_stream , tabs );
					protected_stream << L'\n';
					break;
				case Private:
					iter -> first -> write( private_stream , tabs );
					private_stream << L'\n';
					break;
				}

		for( NestedTypeCollection::const_iterator iter = memberuserdefinedtypes -> begin( ) ; iter != memberuserdefinedtypes -> end( ) ; ++iter )
		{
			bool flag = true;
			for( BaseTypeCollection::const_iterator base_iter = iter -> first -> BaseTypes( ).begin( ) ; flag && base_iter != iter -> first -> BaseTypes( ).end( ) ; ++base_iter )
				if( &base_iter -> Type( ) == this )
					flag = false;
			if( flag )
			{
				++iter -> first -> nestinglevel;
				switch( iter -> second )
				{
				case Public:
					iter -> first -> write( public_stream , tabs );
					public_stream << L'\n';
					break;
				case Protected:
					iter -> first -> write( protected_stream , tabs );
					protected_stream << L'\n';
					break;
				case Private:
					iter -> first -> write( private_stream , tabs );
					private_stream << L'\n';
					break;
				}
			}
			else
				render_backlog.push_back( &*( iter -> first ) );
		}

		for( MemberVariableCollection::iterator iter = membervariables -> begin( ) ; iter != membervariables -> end( ) ; ++iter )
			switch (std::get< 1 >(*iter))
			{
			case Public:
				if (std::get< 0 >(*iter)->Specifier().Static())
					if (std::get< 0 >(*iter)->Specifier().Constant() && BasicType::validate(std::get< 0 >(*iter)->Specifier().Type().Name()).size())
					{
					bool flag = std::get< 0 >(*iter)->Specifier().Extern();
					std::get< 0 >(*iter)->Specifier().Extern(false);
					std::get< 0 >(*iter)->write(public_stream, tabs);
					std::get< 0 >(*iter)->Specifier().Extern(flag);
					}
					else
					{
						std::get< 0 >(*iter)->declaration(public_stream, tabs);
						render_backlog.push_back(&*(std::get< 0 >(*iter)));
					}
				else
				{
					if (std::get< 2 >(*iter))
						public_stream << wstring(tabs, FormattingData::IndentationCharacter)  << L"mutable ";
					std::get< 0 >(*iter)->declaration(public_stream, 0);
					private_stream << std::endl;
				}
				break;
			case Protected:
				if (std::get< 0 >(*iter)->Specifier().Static())
					if (std::get< 0 >(*iter)->Specifier().Constant() && BasicType::validate(std::get< 0 >(*iter)->Specifier().Type().Name()).size())
					{
					bool flag = std::get< 0 >(*iter)->Specifier().Extern();
					std::get< 0 >(*iter)->Specifier().Extern(false);
					std::get< 0 >(*iter)->write(protected_stream, tabs);
					std::get< 0 >(*iter)->Specifier().Extern(flag);
					}
					else
					{
						std::get< 0 >(*iter)->declaration(protected_stream, tabs);
						render_backlog.push_back(&*(std::get< 0 >(*iter)));
					}
				else
				{
					if (std::get< 2 >(*iter))
						protected_stream << wstring(tabs, FormattingData::IndentationCharacter) << L"mutable ";
					std::get< 0 >(*iter)->declaration(protected_stream, 0);
					private_stream << std::endl;
				}
				break;
			case Private:
				if (std::get< 0 >(*iter)->Specifier().Static())
					if (std::get< 0 >(*iter)->Specifier().Constant() && BasicType::validate(std::get< 0 >(*iter)->Specifier().Type().Name()).size())
					{
					bool flag = std::get< 0 >(*iter)->Specifier().Extern();
					std::get< 0 >(*iter)->Specifier().Extern(false);
					std::get< 0 >(*iter)->write(private_stream, tabs);
					std::get< 0 >(*iter)->Specifier().Extern(flag);
					}
					else
					{
						std::get< 0 >(*iter)->declaration(private_stream, tabs);
						render_backlog.push_back(&*(std::get< 0 >(*iter)));
					}
				else
				{
					if (std::get< 2 >(*iter))
						private_stream << wstring(tabs, FormattingData::IndentationCharacter) << L"mutable ";
					std::get< 0 >(*iter)->declaration(private_stream, 0);
					private_stream << std::endl;
				}
				break;
			}

		bool flag;
		for( MemberFunctionCollection::const_iterator iter = memberfunctions -> begin( ) ; iter != memberfunctions -> end( ) ; ++iter )
			switch( ( *iter ) -> Access( ) )
			{
			case Public:
				if( !( *iter ) -> Pure( ) )
				{
					flag = ( *iter ) -> separate;
					( *iter ) -> separate = false;
					( *iter ) -> write( public_stream , tabs );
					( *iter ) -> separate = flag;
				}
				else
				{
					( *iter ) -> declaration( public_stream , tabs );
					if( ( *iter ) -> ForceBody ( ) )
						render_backlog.push_back( &**iter );
				}
				public_stream << L'\n';
				break;
			case Protected:
				if( !( *iter ) -> Pure( ) )
				{
					flag = ( *iter ) -> separate;
					( *iter ) -> separate = false;
					( *iter ) -> write( protected_stream , tabs );
					( *iter ) -> separate = flag;
				}
				else
				{
					( *iter ) -> declaration( protected_stream , tabs );
					if( ( *iter ) -> ForceBody ( ) )
						render_backlog.push_back( &**iter );
				}
				protected_stream << L'\n';
				break;
			case Private:
				if( !( *iter ) -> Pure( ) )
				{
					flag = ( *iter ) -> separate;
					( *iter ) -> separate = false;
					( *iter ) -> write( private_stream , tabs );
					( *iter ) -> separate = flag;
				}
				else
				{
					( *iter ) -> declaration( private_stream , tabs );
					if( ( *iter ) -> ForceBody ( ) )
						render_backlog.push_back( &**iter );
				}
				private_stream << L'\n';
				break;
			}

		for( MemberOperatorCollection::const_iterator iter = memberoperators -> begin( ) ; iter != memberoperators -> end( ) ; ++iter )
			switch( ( *iter ) -> Access( ) )
			{
			case Public:
				if( !( *iter ) -> Pure( ) )
				{
					flag = ( *iter ) -> separate;
					( *iter ) -> separate = false;
					( *iter ) -> write( public_stream , tabs );
					( *iter ) -> separate = flag;
				}
				else
				{
					( *iter ) -> declaration( public_stream , tabs );
					if( ( *iter ) -> ForceBody ( ) )
						render_backlog.push_back( &**iter );
				}
				public_stream << L'\n';
				break;
			case Protected:
				if( !( *iter ) -> Pure( ) )
				{
					flag = ( *iter ) -> separate;
					( *iter ) -> separate = false;
					( *iter ) -> write( protected_stream , tabs );
					( *iter ) -> separate = flag;
				}
				else
				{
					( *iter ) -> declaration( protected_stream , tabs );
					if( ( *iter ) -> ForceBody ( ) )
						render_backlog.push_back( &**iter );
				}
				protected_stream << L'\n';
				break;
			case Private:
				if( !( *iter ) -> Pure( ) )
				{
					flag = ( *iter ) -> separate;
					( *iter ) -> separate = false;
					( *iter ) -> write( private_stream , tabs );
					( *iter ) -> separate = flag;
				}
				else
				{
					( *iter ) -> declaration( private_stream , tabs );
					if( ( *iter ) -> ForceBody ( ) )
						render_backlog.push_back( &**iter );
				}
				private_stream << L'\n';
				break;
			}

		for( ConstructorCollection::const_iterator iter = constructors -> begin( ) ; iter != constructors -> end( ) ; ++iter )
			switch( ( *iter ) -> Access( ) )
			{
			case Public:
				flag = ( *iter ) -> separate;
				( *iter ) -> separate = false;
				( *iter ) -> write( public_stream , tabs );
				( *iter ) -> separate = flag;
				public_stream << L'\n';
				break;
			case Protected:
				flag = ( *iter ) -> separate;
				( *iter ) -> separate = false;
				( *iter ) -> write( protected_stream , tabs );
				( *iter ) -> separate = flag;
				protected_stream << L'\n';
				break;
			case Private:
				flag = ( *iter ) -> separate;
				( *iter ) -> separate = false;
				( *iter ) -> write( private_stream , tabs );
				( *iter ) -> separate = flag;
				private_stream << L'\n';
				break;
			}

		if( destructor )
			switch( destructor -> Access( ) )
			{
			case Public:
				if( !destructor -> Pure( ) )
				{
					flag = destructor -> separate;
					destructor -> separate = false;
					destructor -> write( public_stream , tabs );
					destructor -> separate = flag;
				}
				else
				{
					destructor -> declaration( public_stream , tabs );
					render_backlog.push_back( destructor );
				}
				break;
			case Protected:
				if( !destructor -> Pure( ) )
				{
					flag = destructor -> separate;
					destructor -> separate = false;
					destructor -> write( protected_stream , tabs );
					destructor -> separate = flag;
				}
				else
				{
					destructor -> declaration( protected_stream , tabs );
					render_backlog.push_back( destructor );
				}
				break;
			case Private:
				if( !destructor -> Pure( ) )
				{
					flag = destructor -> separate;
					destructor -> separate = false;
					destructor -> write( private_stream , tabs );
					destructor -> separate = flag;
				}
				else
				{
					destructor -> declaration( private_stream , tabs );
					render_backlog.push_back( destructor );
				}
				break;
			}

		wstring public_str = public_stream.str( ) , protected_str = protected_stream.str( ) , private_str = private_stream.str( );

		if( public_str.size( ) )
			os << tab << L"public:\n" << public_str;
		if( protected_str.size( ) )
			os << tab << L"protected:\n" << protected_str;
		if( private_str.size( ) )
			os << tab << L"private:\n" << private_str;
	}

	void UserDefinedType::writeforwarddeclarations( wostream& os , unsigned long tabs ) const
	{
		wostringstream public_stream , protected_stream , private_stream;
		++tabs;
		wstring tab( tabs , FormattingData::IndentationCharacter );
		for( MemberEnumerationCollection::const_iterator iter = memberenumerations -> begin( ) ; iter != memberenumerations -> end( ) ; ++iter )
			if( iter -> first -> Name( ).size( ) )
				switch( iter -> second )
				{
				case Public:
					public_stream << tab << L"enum " << iter -> first -> Name( ) << L";\n\n";
					break;
				case Protected:
					protected_stream << tab << L"enum " << iter -> first -> Name( ) << L";\n\n";
					break;
				case Private:
					private_stream << tab << L"enum " << iter -> first -> Name( ) << L";\n\n";
					break;
				}
			else
				switch( iter -> second )
				{
				case Public:
					iter -> first -> write( public_stream , tabs );
					public_stream << L'\n';
					break;
				case Protected:
					iter -> first -> write( protected_stream , tabs );
					protected_stream << L'\n';
					break;
				case Private:
					iter -> first -> write( private_stream , tabs );
					private_stream << L'\n';
					break;
				}

		for( MemberUnionCollection::const_iterator iter = memberunions -> begin( ) ; iter != memberunions -> end( ) ; ++iter )
			if( iter -> first -> Name( ).size( ) )
				switch( iter -> second )
				{
				case Public:
					public_stream << tab << L"union " << iter -> first -> Name( ) << L";\n\n";
					break;
				case Protected:
					protected_stream << tab << L"union " << iter -> first -> Name( ) << L";\n\n";
					break;
				case Private:
					private_stream << tab << L"union " << iter -> first -> Name( ) << L";\n\n";
					break;
				}
			else
				switch( iter -> second )
				{
				case Public:
					iter -> first -> write( public_stream , tabs );
					public_stream << L'\n';
					break;
				case Protected:
					iter -> first -> write( protected_stream , tabs );
					protected_stream << L'\n';
					break;
				case Private:
					iter -> first -> write( private_stream , tabs );
					private_stream << L'\n';
					break;
				}

		for( NestedTypeCollection::const_iterator iter = memberuserdefinedtypes -> begin( ) ; iter != memberuserdefinedtypes -> end( ) ; ++iter )
			if( iter -> first -> Name( ).size( ) )
				switch( iter -> second )
				{
				case Public:
					iter -> first -> declaration( public_stream , tabs );
					public_stream << L'\n';
					break;
				case Protected:
					iter -> first -> declaration( protected_stream , tabs );
					protected_stream << L'\n';
					break;
				case Private:
					iter -> first -> declaration( private_stream , tabs );
					private_stream << L'\n';
					break;
				}

		for( MemberTypedefinitionCollection::const_iterator iter = typedefinitions -> begin( ) ; iter != typedefinitions -> end( ) ; ++iter )
			switch( iter -> second )
			{
			case Public:
				iter -> first -> write( public_stream , tabs );
				public_stream << L'\n';
				break;
			case Protected:
				iter -> first -> write( protected_stream , tabs );
				protected_stream << L'\n';
				break;
			case Private:
				iter -> first -> write( private_stream , tabs );
				private_stream << L'\n';
				break;
			}

		tab = wstring( --tabs , FormattingData::IndentationCharacter );

		wstring public_str = public_stream.str( ) , protected_str = protected_stream.str( ) , private_str = private_stream.str( );

		if( public_str.size( ) )
			os << tab << L"public:\n" << public_str;
		if( protected_str.size( ) )
			os << tab << L"protected:\n" << protected_str;
		if( private_str.size( ) )
			os << tab << L"private:\n" << private_str;
	}

	void UserDefinedType::writebody( wostream& declos , wostream& defos , unsigned long decltabs, unsigned long deftabs ) const
	{
		wstring tab( decltabs , FormattingData::IndentationCharacter );
		declos << L'\n' << tab << L"{\n";
		writeforwarddeclarations( declos , decltabs );
		writemembers( declos , defos , decltabs , deftabs );
		declos << tab << L"};\n";
	}

	void UserDefinedType::writebody( wostream& os , unsigned long tabs ) const
	{
		wstring tab( tabs , FormattingData::IndentationCharacter );
		os << L'\n' << tab << L"{\n";
		writeforwarddeclarations( os , tabs );
		writemembers( os , tabs );
		os << tab << L"};\n";
	}

	void UserDefinedType::writetext( wostream& declos , wostream& defos , unsigned long decltabs , unsigned long deftabs ) const
	{
		declos << wstring( decltabs , FormattingData::IndentationCharacter );
		UserDefinedType::writetemplateattributes( declos );
		declos << ( usertype == Class ? L"class " : L"struct " );
		UserDefinedType::writeelaboratedname( declos );
		writespecializedparameters( declos );
		writebasetypes( declos );
		writebody( declos , defos , decltabs , deftabs );

		// render_backlog will contain only UDTs which derive from this type.
		if( nestinglevel == 0 && render_backlog.size( ) != 0 )
			for( vector< const CodeObject* >::const_iterator iter = render_backlog.begin( ) ; iter != render_backlog.end( ) ; ++iter )
			{
				const UserDefinedType* udt = dynamic_cast< const UserDefinedType* >( *iter );
				if( udt )
				{
					--udt -> nestinglevel;
					udt -> UserDefinedType::write( declos , defos , decltabs , deftabs );
				}
			}
	}

	void UserDefinedType::writetext( wostream& os , unsigned long tabs ) const
	{
		os << wstring( tabs , FormattingData::IndentationCharacter );
		UserDefinedType::writetemplateattributes( os );
		os << ( usertype == Class ? L"class " : L"struct " );
		UserDefinedType::writeelaboratedname( os );
		writespecializedparameters( os );
		writebasetypes( os );
		writebody( os , tabs );

		// render_backlog will contain all UDTs which derive from this type, all member functions which are abstract and have bodies,
		// all member variables which are static but not constant or of integral type and the abstract destructor if any.
		if( nestinglevel == 0 && render_backlog.size( ) != 0 )
			for( vector< const CodeObject* >::const_iterator iter = render_backlog.begin( ) ; iter != render_backlog.end( ) ; ++iter )
			{
				const UserDefinedType* udt = dynamic_cast< const UserDefinedType* >( *iter );
				if( udt )
				{
					--udt -> nestinglevel;
					udt -> UserDefinedType::write( os , tabs );
				}
				else
				{
					( *iter ) -> write( os , tabs );
					os << L'\n';
				}
			}
	}

	FunctionPtr::FunctionPtr( const wstring& name , const class Function& method )
		 : Type( name ) , function( &method ) , Callable( name )
	{
	}

	void FunctionPtr::Assign( const FunctionPtr& ref , bool )
	{
		Type::Assign( ref , true );
		Callable::Assign( ref );
		function = ref.function;
	}

	FunctionPtr& FunctionPtr::Assign( const CodeObject& object )
	{
		Assign( cast< FunctionPtr >( object ) , true );
		return *this;
	}

	FunctionPtr& FunctionPtr::Duplicate( ) const
	{
		return *new FunctionPtr( *this );
	}

	const Function& FunctionPtr::Function( ) const
	{
		return *function;
	}

	void FunctionPtr::writetext( wostream& os , unsigned long tabs ) const
	{
		if( function -> ReturnType( ) )
			( *function -> ReturnType( ) ) -> write( os , tabs );
		else
			os << wstring( tabs , FormattingData::IndentationCharacter ) << L"void";
		os << L" ( " << Type::Name( ) << L" )( ";
		if( function -> Parameters( ).size( ) )
		{
			ParameterCollection::const_iterator iter = function -> Parameters( ).begin( );
			( *iter ) -> Specifier( ).write( os );
			( *iter ) -> Declarator( ).write( os );
			for( ++iter ; iter != function -> Parameters( ).end( ) ; ++iter )
			{
				os << L" , " ;
				( *iter ) -> Specifier( ).write( os );
				( *iter ) -> Declarator( ).write( os );
			}
		}
		os << L" )";
	}

	void FunctionPtr::writetext( wostream& declos , wostream& , unsigned long decltabs , unsigned long ) const
	{
		writetext( declos , decltabs );
	}

	MemFun& MemFun::Duplicate( ) const
	{
		return *new MemFun( *this );
	}

	void MemFun::Assign( const MemFun& ref , bool )
	{
		FunctionPtr::Assign( ref , true );
		containingtype = ref.containingtype;
	}

	MemFun& MemFun::Assign( const CodeObject& object )
	{
		Assign( cast< MemFun >( object ) , true );
		return *this;
	}

	MemFun::MemFun( const wstring& name , const MemberFunction& function )
		 : FunctionPtr( name , function ) , containingtype( 0 )
	{
	}

	MemFun::MemFun( const wstring& name , const UserDefinedType& type , const MemberFunction& function )
		 : FunctionPtr( name , function ) , containingtype( &type )
	{
	}

	void MemFun::writetext( wostream& os , unsigned long tabs ) const
	{
		os << wstring( tabs , FormattingData::IndentationCharacter );
		if( Function( ).ReturnType( ) )
			( *Function( ).ReturnType( ) ) -> write( os , tabs );
		else
			os << wstring( tabs , FormattingData::IndentationCharacter ) << L"void";
		os << L"( ";
		const MemberFunction& mem = cast< MemberFunction >( Function( ) );
		mem.EnclosingType( ).writeelaboratedname( os );
		os << Type::Name( ) << L" )( ";
		if( Function( ).Parameters( ).size( ) )
		{
			ParameterCollection::const_iterator iter = Function( ).Parameters( ).begin( );
			( *iter ) -> Specifier( ).write( os );
			( *iter ) -> Declarator( ).write( os );
			for( ++iter ; iter != Function( ).Parameters( ).end( ) ; ++iter )
			{
				os << L" , " ;
				( *iter ) -> Specifier( ).write( os );
				( *iter ) -> Declarator( ).write( os );
			}
		}
		os << L" )";
	}

	void MemFun::writetext( wostream& declos , wostream& , unsigned long decltabs , unsigned long ) const
	{
		writetext( declos , decltabs );
	}

	PointerToMember::PointerToMember( const wstring& name , const UserDefinedType& enclosingtype , const Type& pointertype )
		 : Type( name ) , containingtype( &enclosingtype ) , type( &pointertype )
	{
	}

	PointerToMember& PointerToMember::Assign( const CodeObject& object )
	{
		const PointerToMember& ref = cast< PointerToMember >( object );
		Type::Assign( ref , true );
		type = ref.type , containingtype = ref.containingtype , initialized = ref.initialized;
		return *this;
	}

	PointerToMember& PointerToMember::Duplicate( ) const
	{
		return *new PointerToMember( *this );
	}

	void PointerToMember::writetext( wostream& , unsigned long ) const
	{
		throw FunctionNotImplementedException( "You cannot output a function pointer at this moment." );
	}

	void PointerToMember::writetext( wostream& , wostream& , unsigned long , unsigned long ) const
	{
		throw FunctionNotImplementedException( "You cannot output a function pointer at this moment." );
	}
}