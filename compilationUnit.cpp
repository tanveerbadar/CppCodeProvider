#include <algorithm>
#include <iterator>
#include <iostream>
#include "cppcodeprovider.h"
#include "compilationunit.h"

namespace CPlusPlusCodeProvider
{
	CompilationUnit::CompilationUnit( const std::wstring& str )
		: name( str ) , comment( Comment::Instantiate( ) )
	{
	}

	Comment& CompilationUnit::Comment( )
	{
		return *comment;
	}

	CodeCollection& CompilationUnit::Declarations( )
	{
		return codeblock;
	}

	void CompilationUnit::write( ) const
	{
		std::wofstream os( name.c_str( ) );
		write( os );
	}

	void CompilationUnit::write( std::wostream& os ) const
	{
		comment -> write( os , 0 );
		if( !comment -> IsEmpty( ) )
			os << L'\n';
		for( CodeCollection::const_iterator i = codeblock.begin( ) ; i != codeblock.end( ) ; ++i )
			( *i ) -> write( os , 0 );
	}
}