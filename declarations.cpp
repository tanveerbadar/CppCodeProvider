#include "declarations.h"
#include "expressions.h"
#include "functions.h"
#include "collections.h"
#include "types.h"

namespace CPlusPlusCodeProvider
{
	using std::wostream;
	using std::wstring;
	using std::auto_ptr;

	DeclaratorSpecifier::DeclaratorSpecifier( CPlusPlusCodeProvider::Type& t )
		: type( &t ) , is_constant( false ) , is_extern( false ) , is_static( false ) , is_volatile( false )
	{
	}

	bool DeclaratorSpecifier::Static( ) const
	{
		return is_static;
	}

	DeclaratorSpecifier& DeclaratorSpecifier::Static( bool val )
	{
		is_static = val;
		if( val )
			is_extern = false;
		return *this;
	}

	bool DeclaratorSpecifier::Volatile( ) const
	{
		return is_volatile;
	}

	DeclaratorSpecifier& DeclaratorSpecifier::Volatile( bool val )
	{
		is_volatile = val;
		return *this;
	}

	DeclaratorSpecifier& DeclaratorSpecifier::Constant( bool flag )
	{
		is_constant = flag;
		return *this;
	}

	bool DeclaratorSpecifier::Constant( ) const
	{
		return is_constant;
	}

	bool DeclaratorSpecifier::Extern( ) const
	{
		return is_extern;
	}

	DeclaratorSpecifier& DeclaratorSpecifier::Extern( bool val )
	{
		is_extern = val;
		if( val )
			is_static = false;
		return *this;
	}

	const Type& DeclaratorSpecifier::Type( ) const
	{
		return *type;
	}

	void DeclaratorSpecifier::write( wostream& os ) const
	{
		if( is_constant )
			os << "const ";
		if( is_volatile )
			os << "volatile ";
		
		const nestabletype* udt = dynamic_cast< nestabletype* >( type );

		if( udt && dynamic_cast< const Enumeration* >( udt ) == 0 )
		{
			udt -> writetemplateattributes( os );
			os << type -> Name( ) << L' ';
			udt -> writeelaboratedname( os );
		}
		else
			os << type -> Name( ) << L' ';
	}

	Declarator& Declarator::Name( const wstring& str )
	{
		name = str;
		return *this;
	}

	wstring Declarator::Name( ) const
	{
		return name;
	}

	bool Declarator::Volatile( ) const
	{
		return is_volatile && level;
	}

	Declarator& Declarator::Volatile( bool val )
	{
		is_volatile	= val && level;
		return *this;
	}

	Declarator& Declarator::Constant( bool flag )
	{
		is_constant = flag && level;
		return *this;
	}

	bool Declarator::Constant( ) const
	{
		return is_constant && level;
	}

	bool Declarator::Reference( ) const
	{
		return is_reference && !indices -> size( );
	}

	Declarator& Declarator::Reference( bool val )
	{
		is_reference = val && !indices -> size( );
		return *this;
	}

	unsigned int Declarator::IndirectionLevel( ) const
	{
		return level;
	}

	Declarator& Declarator::IndirectionLevel( unsigned int val )
	{
		level = val;
		return *this;
	}

	IntegerCollection& Declarator::Indices( ) const
	{
		return *indices;
	}

	Declarator::Declarator( const wstring& Name )
		: name( Name ) , indices( new IntegerCollection ) , level( 0 ) , initializer( 0 ) , has_initializer( false ) , is_constant( false ) , is_reference( false ) , is_volatile( false )
	{
	}

	Declarator::Declarator( const Declarator& other )
		: name( other.name ) , is_reference( other.is_reference ) , is_constant( other.is_constant ) , is_volatile( other.is_volatile ) , level( other.level ) , indices( new IntegerCollection( *other.indices ) ) , has_initializer( other.has_initializer )
	{
		if( other.has_initializer )
			initializer.reset( new copying_pointer< Expression >( static_cast< Expression* >( &( *other.initializer ) -> Duplicate( ) ) ) );
	}

	Declarator& Declarator::operator = ( const Declarator& other )
	{
		if( this != &other )
		{
			name = other.name , is_reference = other.is_reference , is_constant = other.is_constant , is_volatile = other.is_volatile , level = other.level , indices.reset( new IntegerCollection( *other.indices ) ) , has_initializer = other.has_initializer;
		if( other.has_initializer )
			initializer.reset( new copying_pointer< Expression >( static_cast< Expression* >( &( *other.initializer ) -> Duplicate( ) ) ) );
		}
		return *this;
	}

	bool Declarator::HasInitializer( ) const
	{
		return has_initializer;
	}

	Expression& Declarator::Initializer( ) const
	{
		return **initializer;
	}

	Declarator& Declarator::Initializer( const copying_pointer< Expression >& expr )
	{
		initializer.reset( new copying_pointer< Expression >( static_cast< Expression* >( &expr -> Duplicate( ) ) ) );
		has_initializer = true;
		return *this;
	}

	void Declarator::write( wostream& os ) const
	{
		os << wstring( level , L'*' );
		if( is_reference )
			os << L'&';
		if( is_constant || is_volatile )
			os << L' ';
		if( is_constant )
			os << L"const ";
		if( is_volatile )
			os << L"volatile ";
		os << name;
		if( indices -> size( ) )
		{
			IntegerCollection::const_iterator iter = indices -> begin( );
			os << L" [ " << *iter << L" ]";
			for( ++iter ; iter != indices -> end( ) ; ++iter )
				os << L"[ " << *iter << L" ]";
		}
		if( has_initializer )
		{ 
			os << L" = ";
			( *initializer ) -> write( os , 0 );
		}
	}

	Declaration::Declaration( Type& specifier )
		: type( specifier ) , comment( Comment::Instantiate( ) )
	{
	}

	Declaration::~Declaration( )
	{
	}

	void Declaration::Assign( const Declaration& other , bool )
	{
		type = other.type;
	}

	Comment& Declaration::Comment( )
	{
		return *comment;
	}

	const Comment& Declaration::Comment( ) const
	{
		return *comment;
	}

	DeclaratorSpecifier& Declaration::Specifier( )
	{
		return type;
	}

	const DeclaratorSpecifier& Declaration::Specifier( ) const
	{
		return type;
	}

	void Declaration::writetext( wostream& os , unsigned long tabs ) const
	{
		comment -> write( os , tabs );
		if( !comment -> IsEmpty( ) )
			os << L'\n' << wstring( tabs , FormattingData::IndentationCharacter );
		type.write( os );
	}

	VariableDeclaration& VariableDeclaration::Duplicate( ) const
	{
		return *new VariableDeclaration( *this );
	}

	VariableDeclaration& VariableDeclaration::Assign( const CodeObject& other )
	{
		const VariableDeclaration& ref = cast< VariableDeclaration >( other );
		Declaration::Assign( ref , true );
		declarator = ref.declarator;
		return *this;
	}

	VariableDeclaration::VariableDeclaration( Type& specifier , const wstring& variable )
		: Declaration( specifier ) , declarator( variable )
	{
	}

	const Declarator& VariableDeclaration::Declarator( ) const
	{
		return declarator;
	}

	Declarator& VariableDeclaration::Declarator( )
	{
		return declarator;
	}

	void VariableDeclaration::writeasparameter( wostream& os ) const
	{
		Declaration::writetext( os , 0 );
		declarator.write( os );
	}

	void VariableDeclaration::writetext( wostream& declos , wostream& , unsigned long , unsigned long deftabs ) const
	{
		throw FunctionNotImplementedException( "This function has not been implemented yet." );
		VariableDeclaration::writetext( declos , deftabs );
		if( declarator.HasInitializer( ) )
		{
		}
	}

	void VariableDeclaration::declaration( wostream& os , unsigned long tabs ) const
	{
		Comment( ).write( os , 0 );
		if( !Comment( ).IsEmpty( ) )
			os << L'\n';
		os << wstring( tabs , FormattingData::IndentationCharacter );
		if( Specifier( ).Static( ) )
			os << L"static ";
		Specifier( ).write( os );
		bool flag = declarator.has_initializer;
		declarator.has_initializer = false;
		declarator.write( os );
		declarator.has_initializer = flag;
		os << L";";
	}

	void VariableDeclaration::writetext( wostream& os , unsigned long tabs ) const
	{
		Comment( ).write( os , tabs );
		if( !Comment( ).IsEmpty( ) )
			os << L'\n';
		os << wstring( tabs , FormattingData::IndentationCharacter );
		if( Specifier( ).Static( ) )
			os << L"static ";
		if( Specifier( ).Extern( ) )
			os << L"extern ";
		Specifier( ).write( os );
		declarator.write( os );
		os << L';';
	}

	VariableDeclarationList& VariableDeclarationList::Duplicate( ) const
	{
		return *new VariableDeclarationList( *this );
	}

	VariableDeclarationList& VariableDeclarationList::Assign( const CodeObject& object )
	{
		const VariableDeclarationList& ref = cast< VariableDeclarationList >( object );
		Declaration::Assign( ref , true );
		declarators.reset( new DeclaratorList( *ref.declarators ) );
		return *this;
	}

	DeclaratorList& VariableDeclarationList::Declarators( )
	{
		return *declarators;
	}

	VariableDeclarationList::VariableDeclarationList( Type& name )
		: Declaration( name ) , declarators( new DeclaratorList )
	{
	}

	VariableDeclarationList::VariableDeclarationList( const VariableDeclarationList& other )
		: Declaration( other ) , declarators( new DeclaratorList( *other.declarators ) )
	{
	}

	void VariableDeclarationList::writetext( wostream& declos , wostream& , unsigned long , unsigned long deftabs ) const
	{
		VariableDeclarationList::writetext( declos , deftabs );
	}

	void VariableDeclarationList::writetext( wostream& os , unsigned long tabs ) const
	{
		if( declarators -> size( ) )
		{
			Comment( ).write( os , tabs );
			if( !Comment( ).IsEmpty( ) )
				os << L'\n';
			os << wstring( tabs , FormattingData::IndentationCharacter );
			Specifier( ).write( os );
			DeclaratorList::const_iterator iter = declarators -> begin( );
			iter -> write( os );
			for( ++iter ; iter != declarators -> end( ) ; ++iter )
			{
				os << L" , ";
				iter -> write( os );
			}
			os << L';';
		}
	}
}