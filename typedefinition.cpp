#include "typedefinition.h"
#include "types.h"
#include "collections.h"

namespace CPlusPlusCodeProvider
{
	using std::wstring;
	using std::wostream;
	using std::auto_ptr;

	Typedefinition& Typedefinition::Assign( const CodeObject& object )
	{
		const Typedefinition& ref = cast< Typedefinition >( object );
		defining_type = ref.defining_type;
 		synonyms.reset( new StringCollection( *ref.synonyms ) );
		return *this;
	}

	Typedefinition& Typedefinition::Duplicate( ) const
	{
		return *new Typedefinition( *this );
	}

	Comment& Typedefinition::Comment( )
	{
		return *comment;
	}

	Typedefinition::Typedefinition( const wstring& name , const wstring& synonym )
		 : defining_type( name ) , synonyms( new StringCollection( ) ) , comment( Comment::Instantiate( ) )
	{
		synonyms -> push_back( synonym );
	}

	Typedefinition::Typedefinition( const Typedefinition& other )
		: defining_type( other.defining_type ) , synonyms( new StringCollection( *other.synonyms ) ) , comment( other.comment )
	{
	}

	StringCollection& Typedefinition::Synonyms( ) const
	{
		return *synonyms;
	}

	void Typedefinition::writetext( wostream& declos , wostream& , unsigned long decltabs , unsigned long ) const
	{
		Typedefinition::writetext( declos , decltabs );
	}

	void Typedefinition::writetext( wostream& os , unsigned long tabs ) const //incorrect
	{
		comment -> write( os , tabs );
		if( !comment -> IsEmpty( ) )
			os << L'\n';
		os << wstring( tabs , FormattingData::IndentationCharacter ) << L"typedef " << defining_type << L' ';
		StringCollection::const_iterator i = synonyms -> begin( );
		os << *i;
		for( ++i ; i != synonyms -> end( ) ; ++i )
			os << L" , " << *i;
		os << L";";
	}
}