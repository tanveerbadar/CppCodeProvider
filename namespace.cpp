#include "namespace.h"
#include "collections.h"

namespace CPlusPlusCodeProvider
{
	using std::wstring;
	using std::wostream;
	using std::auto_ptr;

	Namespace& Namespace::Assign( const CodeObject& object )
	{
		const Namespace& ref = cast< Namespace >( object );
		codeblock.reset( new CodeCollection( *ref.codeblock ) );
		name = ref.name;
		return *this;
	}

	Comment& Namespace::Comment( )
	{
		return *comment;
	}

	Namespace::Namespace( const wstring& str )
		 : name( str ) , codeblock( new CodeCollection( ) ) , comment( Comment::Instantiate( ) )
	{
	}

	Namespace::Namespace( const Namespace& other )
		 : name( other.name ) , codeblock( new CodeCollection( *other.codeblock ) ) , comment( other.comment )
	{
	}

	Namespace& Namespace::Duplicate( ) const
	{
		return *new Namespace( *this );
	}

	CodeCollection& Namespace::Declarations( ) const
	{
		return *codeblock;
	}

	void Namespace::writetext( wostream& os , unsigned long tabs ) const
	{
		comment -> write( os , tabs );
		if( !comment -> IsEmpty( ) )
			os << L'\n';
		wstring tab( tabs , FormattingData::IndentationCharacter );
		os << tab << L"namespace";
		if( name.length( ) )
			os << L' ' << name;
		os << L'\n' << tab << L'{' << L'\n';
		++tabs;
		for( CodeCollection::const_iterator i = codeblock -> begin( ) ; i != codeblock -> end( ) ; ++i )
			( **i ).write( os , tabs );
		--tabs;
		os << tab << L"}\n";
	}

	void Namespace::writetext( wostream& declos , wostream& defos , unsigned long decltabs , unsigned long deftabs ) const
	{
		comment -> write( declos , decltabs );
		if( !comment -> IsEmpty( ) )
			declos << L'\n';
		wstring tab( decltabs , FormattingData::IndentationCharacter ) , deftab( decltabs , FormattingData::IndentationCharacter );
		declos << tab << L"namespace";
		defos << deftab << L"namespace";
		if( name.length( ) )
		{
			declos << L' ' << name;
			defos << L' ' << name;
		}
		declos << L'\n' << tab << L'{' << L'\n';
		defos << FormattingData::IndentationCharacter  << deftab << L"{\n";
		++decltabs;
		++deftabs;
		for( CodeCollection::const_iterator i = codeblock -> begin( ) ; i != codeblock -> end( ) ; ++i )
			( **i ).write( declos , defos , decltabs , deftabs );
		--decltabs;
		--deftabs;
		declos << tab << L"}\n";
		defos << deftab << L"}\n";
	}
}