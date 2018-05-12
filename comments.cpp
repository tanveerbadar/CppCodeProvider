#include "cppcodeprovider.h"
#include <sstream>

namespace CPlusPlusCodeProvider
{
	using std::wstring;
	using std::wostream;

	Comment& Comment::Assign( const CodeObject& object )
	{
		const Comment& ref = cast< Comment >( object );
		text = ref.text;
		return *this;
	}

	Comment& Comment::Duplicate( ) const
	{
		return *new Comment( text );
	}

	Comment::Comment( const wstring& comment )
		 : text( comment ) , object_source( false ) , multiline( false )
	{
	}

	Comment::Comment( const CodeObject& other )
		: object( &other ) , object_source( true ) , multiline( false )
	{
	}

	bool Comment::IsEmpty( ) const
	{
		return !text.length( ) && !object_source;
	}

	bool Comment::MultiLine( ) const
	{
		return multiline;
	}

	Comment& Comment::MultiLine( bool flag )
	{
		multiline = flag;
		return *this;
	}

	wstring Comment::Text( ) const
	{
		return text;
	}

	void Comment::writetext( wostream& os , unsigned long tabs ) const
	{
		if( object_source )
		{
			std::wstringstream ws;
			object -> write( ws , tabs );
			text = ws.str( );
			multiline = text.find_first_of( L'\n' ) != wstring::npos;
		}
		if( !text.size( ) )
			return;
		if( !MultiLine( ) )
			os << L"/*" << text << L"*/";
		else
		{
			os << L"//";
			wstring result = text;
			wstring::size_type n = result.find( L'\n' );
			while( n != wstring::npos )
				result.replace( n , 1 , L'\n' + wstring( tabs , FormattingData::IndentationCharacter ) + L"//" ) , n = result.find( L'\n' , ++n );
			os << result;
		}
	}

	void Comment::writetext( wostream& declos , wostream& , unsigned long decltabs , unsigned long ) const
	{
		Comment::writetext( declos , decltabs );
	}
}