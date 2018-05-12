#include "preprocessor.h"
#include "compilationunit.h"
#include "collections.h"

namespace CPlusPlusCodeProvider
{
	using std::wstring;
	using std::wostream;

	PreprocessorDirective& PreprocessorDirective::Assign( const CodeObject& object )
	{
		Assign( cast< PreprocessorDirective >( object ) );
		return *this;
	}

	void PreprocessorDirective::Assign( const PreprocessorDirective& ref )
	{
		directive = ref.directive;
		type = ref.type;
	}

	PreprocessorDirective& PreprocessorDirective::Duplicate( ) const
	{
		return *new PreprocessorDirective( *this );
	}

	Comment& PreprocessorDirective::Comment( )
	{
		return *comment;
	}

	PreprocessorDirective::PreprocessorDirective( const wstring& str , DirectiveType directivetype )
		: directive( str ) , type( directivetype ) , comment( Comment::Instantiate( ) )
	{
	}

	const wstring& PreprocessorDirective::Directive( ) const
	{
		return directive;
	}

	PreprocessorDirective::DirectiveType PreprocessorDirective::Type( ) const
	{
		return type;
	}

	bool PreprocessorDirective::MultiLine( ) const
	{
		return multiline;
	}

	PreprocessorDirective& PreprocessorDirective::MultiLine( bool val )
	{
		multiline = val;
		return *this;
	}

	void PreprocessorDirective::writetext( wostream& declos , wostream& , unsigned long decltabs , unsigned long ) const
	{
		write( declos , decltabs );
	}

	void PreprocessorDirective::writetext( wostream& os , unsigned long tabs ) const
	{
		comment -> write( os , tabs );
		if( !comment -> IsEmpty( ) )
			os << L'\n';
		switch( type )
		{
		case Include:
			os << L"#include " << directive;
			break;
		case Pragma:
			os << L"#pragma " << directive;
			break;
		case Define:
		case UnDefine:
			if( !multiline )
				os << ( type == Define ? L"#define " : L"#undefine " ) << directive;
			else
			{
				wstring result = directive;
				wstring::size_type n = result.find( L'\n' );
				while( n != wstring::npos )
					result.replace( n , 1 , L"\\\n" ) , n = result.find( L'\n' , ++n );
				os << ( type == Define ? L"#define " : L"#undefine " ) << result;
			}
			break;
		}
		os << L'\n';
	}

	MacroTestDirective& MacroTestDirective::Assign( const CodeObject& object )
	{
		const MacroTestDirective& ref = cast< MacroTestDirective >( object );
		PreprocessorDirective::Assign( ref );
		*codeblock = *ref.codeblock;
		return *this;
	}

	MacroTestDirective& MacroTestDirective::Duplicate( ) const
	{
		return *new MacroTestDirective( *this );
	}

	MacroTestDirective::MacroTestDirective( const MacroTestDirective& other )
		: codeblock( new CompilationUnit( *other.codeblock ) ) , PreprocessorDirective( other.Directive( ) )
	{
	}

	MacroTestDirective::MacroTestDirective( const wstring& directive )
		: PreprocessorDirective( directive )
	{
	}

	CodeCollection& MacroTestDirective::Declarations( ) const
	{
		return codeblock -> Declarations( );
	}

	void MacroTestDirective::writetext( wostream& os , unsigned long ) const
	{
		if( !PreprocessorDirective::MultiLine( ) )
			os << PreprocessorDirective::Directive( );
		else
		{
			wstring result = PreprocessorDirective::Directive( );
			wstring::size_type n = result.find( L'\n' );
			while( n != wstring::npos )
				result.replace( n , 1 , L"\\\n" ) , n = result.find( L'\n' , ++n );
			os << L"#" << result;
		}
		if( codeblock.get( ) )
			codeblock -> write( os );
		os << L"\n#endif";
	}
}