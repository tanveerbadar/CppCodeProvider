#ifndef TYPEDEFINITION_HEADER
#define TYPEDEFINITION_HEADER

#include "cppcodeprovider.h"
#include <memory>

namespace CPlusPlusCodeProvider
{
	typedef TypedCollection< std::wstring > StringCollection;

	/// <summary>
	/// Allows you to use a typedefinition in code-graph.
	/// </summary>
	class Typedefinition : public CodeObject
	{
		std::wstring defining_type;
		std::auto_ptr< StringCollection > synonyms;
		copying_pointer< Comment > comment;
	public:
		/// <summary>
		/// Returns a reference to the comments applied to this typedefinition.
		/// </summary>
		/// <returns>A reference to comments.</returns>
		Comment& Comment( );

		/// <summary>
		/// Creates a new Typedefintion given a type and synonym.
		/// </summary>
		/// <parameter IsConst="true" name="type">Type which is renamed.</parameter>
		/// <parameter IsConst="true" name="synonym">New name for the given type.</parameter>
		/// <returns>A proxy object that acts like a Typedefinition.</returns>
		static copying_pointer< Typedefinition > Instantiate( const std::wstring& type , const std::wstring& synonym )
		{
			return copying_pointer< Typedefinition >( new Typedefinition( type , synonym ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed Typedefinition.</returns>
		Typedefinition& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this Typedefinition.
		/// </summary>
		/// <parameter IsConst="true" name="object">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		Typedefinition& Assign( const CodeObject& object );

		/// <summary>
		/// Provides access to the synonyms defined in this typedefinition.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A string collection.</returns>
		StringCollection& Synonyms( ) const;
	protected:
		/// <summary>
		/// Constructs anew typedefinition given a type and synonym.
		/// </summary>
		/// <parameter IsConst="true" name="type">Type which is renamed.</parameter>
		/// <parameter IsConst="true" name="synonym">New name for the given type.</parameter>
		Typedefinition( const std::wstring& type , const std::wstring& synonym );
		/// <summary>
		/// Copy constructs a new typedefinition from a given typedefinition.
		/// </summary>
		/// <parameter IsConst="true" name="other">Typedefintion to copy.</parameter>
		Typedefinition( const Typedefinition& other );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this Typedefinition</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
		/// <summary>
		/// Writes out this object split across the two streams.
		/// </summary>
		/// <parameter name="declas">Declaration output stream.</parameter>
		/// <parameter name="defos">Definition output stream.</parameter>
		/// <parameter name="tabs">Indentation for this Typedefinition.</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;
	};
}

#endif