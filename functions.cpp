#include "functions.h"
#include "types.h"
#include "statements.h"
#include "declarations.h"
#include "collections.h"

namespace CPlusPlusCodeProvider
{
	using std::wstring;
	using std::wostream;
	using std::auto_ptr;

	void Function::Assign( const Function& ref , bool )
	{
		Callable::Assign( ref );
		statements.reset( new CodeCollection( *ref.statements ) );
		catchclauses.reset( new CatchClauseCollection( *ref.catchclauses ) );
		template_parameters.reset( new TemplateParameterCollection( *ref.template_parameters ) );
		is_inline = ref.is_inline;
	}

	Function& Function::Assign( const CodeObject& object )
	{
		Assign( cast< Function >( object ) , true );
		return *this;
	}

	Function& Function::Duplicate( ) const
	{
		return *new Function( *this );
	}

	//incomplete
	Function::Function( const Function& other )
		: Callable( other ) , is_inline( other.is_inline ) , catchclauses( new CatchClauseCollection( *other.catchclauses ) ) , template_parameters( new TemplateParameterCollection( *other.template_parameters ) ) , statements( new StatementCollection( *other.statements ) ) , separate( other.separate )
	{
	}

	Function::Function( const wstring& name_ , CompilerCapabilities::Capability capabilities )
		 : Callable( name_ ) , catchclauses( new CatchClauseCollection( ) ) , template_parameters( new TemplateParameterCollection( ) ) , is_inline( false ) , statements( new StatementCollection( ) ) , capability( capabilities ) , separate( true )
	{
	}

	Function::Function( const wstring& name_ , copying_pointer< VariableDeclaration >& type , CompilerCapabilities::Capability capabilities )
		 : Callable( name_ , type ) , catchclauses( new CatchClauseCollection( ) ) , template_parameters( new TemplateParameterCollection( ) ) , is_inline( false ) , statements( new StatementCollection( ) ) , capability( capabilities ) , separate( true )
	{
	}

	CodeCollection& Function::Statements( ) const
	{
		return *statements;
	}

	CatchClauseCollection& Function::CatchClauses( ) const
	{
		return *catchclauses;
	}

	bool Function::FunctionTryBlock( ) const
	{
		return ( capability & CompilerCapabilities::FunctionTryBlocks ) && catchclauses -> size( );
	}

	bool Function::Inline( ) const
	{
		return is_inline;
	}

	Function& Function::Inline( bool val )
	{
		is_inline = val;
		return *this;
	}

	void Function::declaration( wostream& os , unsigned long tabs ) const
	{
		writedeclaration( os , tabs );
	}

	bool Function::Template( ) const
	{
		return template_parameters -> size( ) != 0;
	}

	TemplateParameterCollection& Function::TemplateParameters( ) const
	{
		return *template_parameters;
	}

	void Function::writetext( wostream& declos , wostream& defos , unsigned long decltabs , unsigned long deftabs ) const
	{
		writedeclaration( declos , decltabs );
		write( defos , deftabs );
	}

	void Function::writedeclaration( wostream& os , unsigned long tabs ) const
	{
		writetemplateattributes( os , tabs );
		if( Inline( ) )
			os << L"inline ";
		if( ReturnType( ) )
		{
			( *ReturnType( ) ) -> Declarator( ).Name( L"" );
			( *ReturnType( ) ) -> writeasparameter( os );
		}
		else
			os << L"void ";
		os << Name( );
		writeparameters( os );
		writeexceptionspecs( os );
		os << L';';
	}

	void Function::writetemplateattributes( wostream& os , unsigned long tabs ) const
	{
		if( Template( ) )
		{
			os << wstring( tabs , FormattingData::IndentationCharacter ) << L"template< ";
			if( template_parameters -> size( ) > 0 )
			{
				CPlusPlusCodeProvider::TemplateParameterCollection::const_iterator i = template_parameters -> begin( );
				( *i ) -> write( os , 0 );
				for( ++i ; i != template_parameters -> end( ) ; ++i )
				{
					os << L" , ";
					( *i ) -> write( os , 0 );
				}
			}
			os << L" > ";
		}
	}

	void Function::writebody( wostream& os , unsigned long tabs ) const
	{
		wstring tab( tabs , FormattingData::IndentationCharacter );
		os << L'\n' << tab << L'{' << L'\n';
		if( !FunctionTryBlock( ) && catchclauses -> size( ) )
			os << tab << wstring( 1 , FormattingData::IndentationCharacter ) << L"try" << L'\n' << tab
			<< wstring( 1 , FormattingData::IndentationCharacter ) << L'{' << L'\n';
		if( statements.get( ) )
		{
			++tabs;
			for( CodeCollection::const_iterator i = statements -> begin( ) ; i != statements -> end( ) ; ++i )
			{
				( **i ).write( os , tabs );
				os << L'\n';
			}
			--tabs;
		}
		if( !FunctionTryBlock( ) && catchclauses -> size( ) )
		{
			os << tab << wstring( 1 , FormattingData::IndentationCharacter ) << L"}\n";
			writecatchclauses( os , tabs + 1 );
		}
		os << tab << L"}\n";
	}

	void Function::writecatchclauses( wostream& os , unsigned long tabs ) const
	{
		for( CatchClauseCollection::const_iterator i = CatchClauses( ).begin( ) ; i != CatchClauses( ).end( ) ; ++i )
			( *i ) -> write( os , tabs );
	}

	void Function::writetext( wostream& os , unsigned long tabs ) const
	{
		Comment( ).write( os , tabs );
		if( !Comment( ).IsEmpty( ) )
			os << L'\n';
		os << wstring( tabs , FormattingData::IndentationCharacter );
		writetemplateattributes( os , 0 );
		if( is_inline )
			os << L"inline ";
		if( ReturnType( ) )
		{
			( *ReturnType( ) ) -> Declarator( ).Name( L"" );
			( *ReturnType( ) ) -> writeasparameter( os );
		}
		else
			os << L"void ";
		os << Name( );
		writeparameters( os );
		writeexceptionspecs( os );
		if( FunctionTryBlock( ) )
			os << wstring( tabs , FormattingData::IndentationCharacter ) << L"try";
		writebody( os , tabs );
		if( FunctionTryBlock( ) )
			writecatchclauses( os , tabs );
	}

	Operator& Operator::Assign( const CodeObject& object )
	{
		const Operator& ref =  cast< Operator >( object );
		t = ref.t;
		return *this;
	}

	Operator& Operator::Duplicate( ) const
	{
		return *new Operator( *this );
	}

	Operator::Operator( const wstring& name , OperatorType ty , CompilerCapabilities::Capability capability )
		 : Function( name , capability ) , t( ty )
	{
	}

	Operator::Operator( const wstring& name , OperatorType ty , copying_pointer< VariableDeclaration >& returns , CompilerCapabilities::Capability capability )
		 : Function( name , returns , capability ) , t( ty )
	{
	}

	wstring Operator::Name( ) const
	{
		std::wstring str( L"operator " );
		str += Function::Name( );
		str += L' ';
		return str;
	}

	Operator::OperatorType Operator::validate( OperatorType t , size_t size ) const
	{
		switch( t )
		{
		case unary:
			if( size > 1 )
				throw Invalid_ArgumentList( );
			break;
		case binary:
			if( size > 2 )
				throw Invalid_ArgumentList( );
			break;
		case call: 
		case conv:
			throw Invalid_identifier( );
		}
		return t;
	}

	AccessRestrictions MemberFunction::Access( ) const
	{
		return access;
	}

	MemberFunction& MemberFunction::Assign( const CodeObject& object )
	{
		const MemberFunction& ref = cast< MemberFunction >( object );
		Function::Assign( ref , true );
		enclosingtype = ref.enclosingtype;
		is_virtual = ref.is_virtual;
		is_static = ref.is_static;
		is_constant = ref.is_constant;
		is_volatile = ref.is_volatile;
		access = ref.access;
		return *this;
	}

	MemberFunction& MemberFunction::Duplicate( ) const
	{
		return *new MemberFunction( *this );
	}

	MemberFunction::MemberFunction( const nestabletype& object , const wstring& name , AccessRestrictions restrictions , CompilerCapabilities::Capability capability )
		 : enclosingtype( &object ) , Function( name , capability ) , access( restrictions ) , is_constant( false ) , is_pure( false ) , is_static( false ) , is_virtual( false ) , is_volatile( false )
	{
	}

	MemberFunction::MemberFunction( const nestabletype& object , const wstring& name , copying_pointer< VariableDeclaration >& returns , AccessRestrictions restrictions , CompilerCapabilities::Capability capability )
		 : enclosingtype( &object ) , Function( name , returns , capability ) , access( restrictions ) , is_constant( false ) , is_pure( false ) , is_static( false ) , is_virtual( false ) , is_volatile( false )
	{
	}

	const nestabletype& MemberFunction::EnclosingType( ) const
	{
		return *enclosingtype;
	}

	bool MemberFunction::Constant( ) const
	{
		return is_constant;
	}

	MemberFunction& MemberFunction::Constant( bool val )
	{
		is_constant = val;
		if( val )
			is_static = false;
		return *this;
	}

	bool MemberFunction::Virtual( ) const
	{
		return is_virtual;
	}

	MemberFunction& MemberFunction::Virtual( bool val )
	{
		is_virtual = val;
		if( val )
		{
			TemplateParameters( ).clear( );
			is_static = false;
		}
		return *this;
	}

	bool MemberFunction::Pure( ) const
	{
		return is_pure;
	}

	MemberFunction& MemberFunction::Pure( bool val )
	{
		is_pure = val;
		if( val )
		{
			TemplateParameters( ).clear( );
			is_static = false;
			is_virtual = true;
		}
		return *this;
	}

	bool MemberFunction::Static( ) const
	{
		return is_static;
	}

	MemberFunction& MemberFunction::Static( bool val )
	{
		is_static = val;
		if( val )
			is_virtual = is_volatile = is_constant = false;
		return *this;
	}

	bool MemberFunction::Volatile( ) const
	{
		return is_volatile;
	}

	MemberFunction& MemberFunction::Volatile( bool val )
	{
		is_volatile = val;
		if( val )
			is_static = false;
		return *this;
	}
	
	bool MemberFunction::ForceBody( ) const
	{
		return force_body;
	}

	MemberFunction& MemberFunction::ForceBody( bool flag )
	{
		force_body = flag;
		return *this;
	}

	void MemberFunction::writedeclaration( wostream& os , unsigned long tabs ) const
	{
		writetemplateattributes( os , tabs );
		if( Inline( ) )
			os << L"inline ";
		if( !Template( ) )
			if( Static( ) )
				os << L"static ";
			else if( Virtual( ) )
				os << L"virtual ";
		if( ReturnType( ) )
		{
			( *ReturnType( ) ) -> Declarator( ).Name( L"" );
			( *ReturnType( ) ) -> writeasparameter( os );
		}
		else
			os << L"void ";
		os << Name( );
		writeparameters( os );
		writeexceptionspecs( os );
		if( Volatile( ) )
			os << L" volatile";
		if( Constant( ) )
			os << L" const";
		if( is_pure )
			os << L" = 0";
		os << L';';
	}

	void MemberFunction::writetext( wostream& os , unsigned long tabs ) const
	{
		wstring tab( tabs , FormattingData::IndentationCharacter );
		os << tab;
		if( separate )
			enclosingtype -> writetemplateattributes( os );
		writetemplateattributes( os , 0 );
		if( Inline( ) )
			os << L"inline ";
		if( !Template( ) )
			if( Static( ) )
				os << L"static ";
		if( ReturnType( ) )
		{
			( *ReturnType( ) ) -> Declarator( ).Name( L"" );
			( *ReturnType( ) ) -> writeasparameter( os );
		}
		else
			os << L"void ";
		if( separate )
			enclosingtype -> writeelaboratedname( os );
		os << Name( );
		writeparameters( os );
		if( Volatile( ) )
			os << L" volatile";
		if( Constant( ) )
			os << L" const";
		writeexceptionspecs( os );
		if( FunctionTryBlock( ) )
			os << L'\n' << tab << L"try";
		writebody( os , tabs );
		if( FunctionTryBlock( ) )
			writecatchclauses( os , tabs );
	}

	AccessRestrictions MemberOperator::Access( ) const
	{
		return access;
	}

	MemberOperator& MemberOperator::Assign( const CodeObject& object )
	{
		const MemberOperator& ref = cast< MemberOperator >( object );
		Operator::Assign( ref );
		enclosingtype = ref.enclosingtype , is_virtual = ref.is_virtual , is_const = ref.is_const , is_volatile = ref.is_volatile , access = ref.access;
		return *this;
	}

	MemberOperator& MemberOperator::Duplicate( ) const
	{
		return *new MemberOperator( *this );
	}

	MemberOperator::MemberOperator( const nestabletype& object , const wstring& name , OperatorType t , AccessRestrictions restrictions , CompilerCapabilities::Capability capability )
		 : Operator( name , t , capability ) , enclosingtype( &object ) , access( restrictions ) , is_const( false ) , is_virtual( false ) , is_volatile( false ) , is_pure( false )
	{
	}

	MemberOperator::MemberOperator( const nestabletype& object , const wstring& name , OperatorType t , copying_pointer< VariableDeclaration >& returns , AccessRestrictions restrictions , CompilerCapabilities::Capability capability )
		 : Operator( name , t , returns , capability ) , enclosingtype( &object ) , access( restrictions ) , is_const( false ) , is_virtual( false ) , is_volatile( false ) , is_pure( false )
	{
	}

	bool MemberOperator::Virtual( ) const
	{
		return is_virtual;
	}

	MemberOperator& MemberOperator::Virtual( bool val )
	{
		is_virtual = val;
		return *this;
	}

	MemberOperator& MemberOperator::Pure( bool flag )
	{
		is_pure = flag;
		return *this;
	}

	bool MemberOperator::Pure( ) const
	{
		return is_pure;
	}

	bool MemberOperator::Constant( ) const
	{
		return is_const;
	}

	MemberOperator& MemberOperator::Constant( bool val )
	{
		is_const = val;
		return *this;
	}

	bool MemberOperator::Volatile( ) const
	{
		return is_volatile;
	}

	MemberOperator& MemberOperator::Volatile( bool val )
	{
		is_volatile = val;
		return *this;
	}
	
	bool MemberOperator::ForceBody( ) const
	{
		return force_body;
	}

	MemberOperator& MemberOperator::ForceBody( bool flag )
	{
		force_body = flag;
		return *this;
	}

	void MemberOperator::writedeclaration( wostream& os , unsigned long tabs ) const
	{
		os << wstring( tabs , FormattingData::IndentationCharacter );
		if( !Virtual( ) )
			writetemplateattributes( os , 0 );
		if( Virtual( ) )
			os << L"virtual ";
		if( Inline( ) )
			os << L"inline ";
		if( ReturnType( ) )
		{
			( *ReturnType( ) ) -> Declarator( ).Name( L"" );
			( *ReturnType( ) ) -> writeasparameter( os );
		}
		else
			os << L"void ";
		os << Operator::Name( );
		writeparameters( os );
		writeexceptionspecs( os );
		if( Volatile( ) )
			os << L" volatile";
		if( Constant( ) )
			os << L" const";
		if( is_pure )
			os << L" = 0";
		os << L';';
	}

	void MemberOperator::writetext( wostream& os , unsigned long tabs ) const
	{
		Comment( ).write( os , tabs );
		if( !Comment( ).IsEmpty( ) )
			os << L'\n';
		wstring tab( tabs , FormattingData::IndentationCharacter );
		os << tab;
		if( separate )
			enclosingtype -> writetemplateattributes( os );
		if( !Virtual( ) )
			writetemplateattributes( os , tabs );
		else
			os << L"virtual ";
		if( Inline( ) )
			os << L"inline ";
		if( ReturnType( ) )
		{
			( *ReturnType( ) ) -> Declarator( ).Name( L"" );
			( *ReturnType( ) ) -> writeasparameter( os );
		}
		else
			os << L"void ";
		if( separate )
			enclosingtype -> writeelaboratedname( os );
		os << Operator::Name( );
		writeparameters( os );
		if( Volatile( ) )
			os << L" volatile";
		if( Constant( ) )
			os << L" const";
		writeexceptionspecs( os );
		if( FunctionTryBlock( ) )
			os << tab << L"try\n";
		writebody( os , tabs );
		if( FunctionTryBlock( ) )
			writecatchclauses( os , tabs );
	}

	Constructor& Constructor::Assign( const CodeObject& object )
	{
		const Constructor& ref = cast< Constructor >( object );
		Function::Assign( ref , true );
		is_explicit = ref.is_explicit , enclosingtype = ref.enclosingtype , access = ref.access;
		initializer_list.reset( new ParameterCollection( *ref.initializer_list ) );
		return *this;
	}

	Constructor& Constructor::Duplicate( ) const
	{
		return *new Constructor( *this );
	}

	Constructor::Constructor( const Constructor& other )
		 : enclosingtype( other.enclosingtype ) , Function( other ) , is_explicit( other.is_explicit ) , access( other.access ) , initializer_list( new ParameterCollection( *other.initializer_list ) )
	{
	}

	Constructor::Constructor( const UserDefinedType& object , AccessRestrictions restrictions , bool exp , CompilerCapabilities::Capability capability )
		 : enclosingtype( &object ) , Function( object.Name( ) , capability ) , is_explicit( exp ) , access( restrictions ) , initializer_list( new ParameterCollection( ) )
	{
	}

	copying_pointer< VariableDeclaration >* Constructor::ReturnType( ) const
	{
		return 0;
	}

	AccessRestrictions Constructor::Access( ) const
	{
		return access;
	}

	Constructor& Constructor::Access( AccessRestrictions a )
	{
		access = a; 
		return *this;
	}

	Constructor& Constructor::Explicit( bool val )
	{
		is_explicit = val;
		return *this;
	}

	bool Constructor::Explicit( ) const
	{
		return is_explicit;
	}

	ParameterCollection& Constructor::InitializerList( ) const
	{
		return *initializer_list;
	}

	wstring Constructor::Name( ) const
	{
		return enclosingtype -> Name( );
	}

	void Constructor::writeinitializerlist( wostream& os , unsigned long tabs ) const
	{
		if( initializer_list -> size( ) == 0 )
			return;
		os << wstring( tabs , FormattingData::IndentationCharacter ) << L": ";
		ParameterCollection::const_iterator i = initializer_list -> begin( );
		( *i ) -> write( os , 0 );
		for( ++i ; i != initializer_list -> end( ) ; ++i )
		{
			os << L" , ";
			( *i ) -> write( os , 0 );
		}
	}

	void Constructor::writedeclaration( wostream& os , unsigned long tabs ) const
	{
		os << wstring( tabs , FormattingData::IndentationCharacter );
		writetemplateattributes( os , 0 );
		if( Explicit( ) )
			os << L"explicit ";
		os << Constructor::Name( );
		writeparameters( os );
		writeexceptionspecs( os );
		os << L';';
	}

	void Constructor::writetext( wostream& os , unsigned long tabs ) const
	{
		wstring tab( tabs , FormattingData::IndentationCharacter );
		os << tab;
		if( separate )
			enclosingtype -> writetemplateattributes( os );
		writetemplateattributes( os , 0 );
		if( Explicit( ) )
			os << L"explicit ";
		if( Inline( ) )
			os << L"inline ";
		if( separate )
			enclosingtype -> writeelaboratedname( os );
		os << Constructor::Name( );
		writeparameters( os );
		writeexceptionspecs( os );
		if( FunctionTryBlock( ) )
			os << tab << L"try\n";
		if( initializer_list -> size( ) )
			writeinitializerlist( os , tabs + 1 );
		writebody( os , tabs );
		if( FunctionTryBlock( ) )
			writecatchclauses( os , tabs );
	}

	Destructor& Destructor::Assign( const CodeObject& object )
	{
		const Destructor& ref = cast< Destructor >( object );
		Function::Assign( ref , true );
		enclosingtype = ref.enclosingtype , is_virtual = ref.is_virtual , access = ref.access;
		return *this;
	}

	Destructor& Destructor::Duplicate( ) const
	{
		return *new Destructor( *this );
	}

	Destructor::Destructor( const UserDefinedType& object , AccessRestrictions restrictions , CompilerCapabilities::Capability capability )
		 : enclosingtype( &object ) , Function( object.Name( ) , capability ) , access( restrictions ) , is_virtual( false ) , is_pure( false )
	{
	}

	copying_pointer< VariableDeclaration >* Destructor::ReturnType( ) const
	{
		return 0;
	}

	wstring Destructor::Name( ) const
	{
		return L'~' + enclosingtype -> Name( );
	}

	bool Destructor::Pure( ) const
	{
		return is_pure;
	}

	Destructor& Destructor::Pure( bool flag )
	{
		is_pure = flag;
		if( is_pure )
			is_virtual = true;
		return *this;
	}

	bool Destructor::Virtual( ) const
	{
		return is_virtual;
	}

	Destructor& Destructor::Virtual( bool flag )
	{
		is_virtual = flag;
		return *this;
	}

	AccessRestrictions Destructor::Access( ) const
	{
		return access;
	}

	void Destructor::writedeclaration( wostream& os , unsigned long tabs ) const
	{
		os << wstring( tabs , FormattingData::IndentationCharacter );
		if( Virtual( ) )
			os << L"virtual ";
		os << Destructor::Name( ) << L"( )";
		writeexceptionspecs( os );
		if( Pure( ) )
			os << L" = 0";
		os << L";\n";
	}

	void Destructor::writetext( wostream& os , unsigned long tabs ) const
	{
		wstring tab( tabs , FormattingData::IndentationCharacter );
		os << tab;
		if( separate )
			enclosingtype -> writetemplateattributes( os );
		if( Inline( ) )
			os << L"inline ";
		if( Virtual( ) )
			os << L"virtual ";
		if( separate )
			enclosingtype -> writeelaboratedname( os );
		os << Destructor::Name( ) << L"( )";
		writeexceptionspecs( os );
		if( FunctionTryBlock( ) )
			os << tab << L"try\n";
		writebody( os , tabs );
		if( FunctionTryBlock( ) )
			writecatchclauses( os , tabs );
	}
}