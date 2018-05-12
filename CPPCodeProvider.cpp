#include "CPPCodeProvider.h"
#include "collections.h"
#include "declarations.h"

namespace CPlusPlusCodeProvider
{
	wchar_t FormattingData::IndentationCharacter = L'\t';

	using std::wstring;
	using std::wostream;

	CodeObject::CodeObject( )
	{
	}

	CodeObject::~CodeObject( )
	{
	}

	void CodeObject::write( wostream& os , unsigned long i ) const
	{
		writetext( os , i );
	}

	void CodeObject::write( wostream& declos , wostream& defos , unsigned long decltabs , unsigned long deftabs ) const
	{
		writetext( declos , defos , decltabs , deftabs );
	}

	Expression::Expression( )
	{
	}

	void Expression::writetext( wostream& declos , wostream& , unsigned long decltabs , unsigned long ) const
	{
		write( declos , decltabs );
	}

	Statement::Statement( )
	{
	}

	void Statement::writetext( wostream& declos , wostream& , unsigned long decltabs , unsigned long ) const
	{
		write( declos , decltabs );
	}

	Type& Type::Assign( const CodeObject& object )
	{
		Assign( cast< Type >( object ) , true );
		return *this;
	}

	void Type::Assign( const Type& ref , bool )
	{
		name = ref.name;
	}

	Type::Type( const wstring& _Name )
		: name( _Name ) , comment( Comment::Instantiate( ) )
	{
	}

	Comment& Type::Comment( )
	{
		return *comment;
	}

	const Comment& Type::Comment( ) const
	{
		return *comment;
	}

	wstring Type::Name( ) const
	{
		return name;
	}

	CodeSnippet::CodeSnippet( const wstring& code )
		: snippet( code )
	{
	}

	CodeSnippet& CodeSnippet::Assign( const CodeObject& object )
	{
		snippet = cast< CodeSnippet >( object).snippet;
		return *this;
	}

	CodeSnippet& CodeSnippet::Duplicate( ) const
	{
		return *new CodeSnippet( *this );
	}

	void CodeSnippet::writetext( wostream& os , unsigned long ) const
	{
		os << snippet;
	}

	void CodeSnippet::writetext( wostream& declos , wostream& , unsigned long , unsigned long deftabs ) const
	{
		CodeSnippet::writetext( declos , deftabs );
	}

	Callable::Callable( const Callable& other )
		: specs( other.specs ) , name( other.name ) , argumentlist( new ParameterCollection( *other.argumentlist ) ) , exception_list( new StringCollection( *other.exception_list ) ) , return_type( other.return_type.get( ) ? new copying_pointer< VariableDeclaration >( *other.return_type ) : 0 ) ,  comment( other.comment ) 
	{
	}

	Callable::Callable( const wstring& name_ , copying_pointer< VariableDeclaration >& type )
		: name( name_ ) , argumentlist( new ParameterCollection( ) ) , exception_list( new StringCollection( ) ) , specs( ExceptionSpecification::all ) , return_type( new copying_pointer< VariableDeclaration >( type ) ) , comment( Comment::Instantiate( ) )
	{
	}

	Callable::Callable( const wstring& name_ )
		: name( name_ ) , argumentlist( new ParameterCollection( ) ) , exception_list( new StringCollection( ) ) , specs( ExceptionSpecification::all ) , return_type( 0 ) , comment( Comment::Instantiate( ) )
	{
	}

	Callable& Callable::Assign( const Callable& ref )
	{
		argumentlist.reset( new ParameterCollection( *ref.argumentlist ) );
		if( ref.return_type.get( ) )
			return_type.reset( new copying_pointer< VariableDeclaration >( &( *ref.return_type ) -> Duplicate( ) ) );
		else
			return_type.reset( );
		name = ref.name;
		exception_list.reset( new StringCollection( *ref.exception_list ) );
		specs = ref.specs;
		return *this;
	}

	Callable& Callable::Duplicate( ) const
	{
		return *new Callable( *this );
	}

	Comment& Callable::Comment( )
	{
		return *comment;
	}

	const Comment& Callable::Comment( ) const
	{
		return *comment;
	}

	wstring Callable::Name( ) const
	{
		return name;
	}

	ParameterCollection& Callable::Parameters( ) const
	{
		return *argumentlist;
	}

	int Callable::ExceptionSpecifications( ) const
	{
		return specs;
	}

	Callable& Callable::ExceptionSpecifications( int spec )
	{
		specs = spec;
		if( specs != ExceptionSpecification::some )
			exception_list -> clear( );
		return *this;
	}

	StringCollection& Callable::ExceptionList( ) const
	{
		return *exception_list;
	}

	copying_pointer< VariableDeclaration >* Callable::ReturnType( ) const
	{
		return return_type.get( );
	}

	void Callable::writeparameters( wostream& os ) const
	{
		const ParameterCollection& declarations = Parameters( );
		os << L"( ";
		if( declarations.size( ) )
		{
			ParameterCollection::const_iterator i = declarations.begin( );
			( *i ) -> writeasparameter( os );
			for( ++i ; i != declarations.end( ) ; ++i )
			{
				os << L" , ";
				( *i ) -> writeasparameter( os );
			}
			os << L' ';
		}
		os << L')';
	}

	void Callable::writeexceptionspecs( wostream& os ) const
	{
		if( ExceptionSpecifications( ) == ExceptionSpecification::none )
			os << L" throw( )";
		else if( ExceptionSpecifications( ) == ExceptionSpecification::some )
		{
			if( ExceptionList( ).size( ) > 0 )
			{
				StringCollection::const_iterator i = ExceptionList( ).begin( );
				os << L" throw( " << *i;
				for( ++i ; i != ExceptionList( ).end( ) ; ++i )
					os << L" , " << *i;
				os << L" )";
			}
		}
	}
}