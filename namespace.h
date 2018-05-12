#ifndef NAMESPACE_HEADER
#define NAMESPACE_HEADER

#include <memory>
#include "cppcodeprovider.h"

namespace CPlusPlusCodeProvider
{
	typedef TypedCollection< copying_pointer< CodeObject > > CodeCollection;

	/// <summary>
	/// Provides services that allow you to use a namespace in code-graph.
	/// </summary>
	class Namespace : public CodeObject
	{
		std::auto_ptr< CodeCollection > codeblock;

		std::wstring name;
		copying_pointer< Comment > comment;
	public:
		/// <summary>
		/// Returns a reference to the comments applied to this namespace.
		/// </summary>
		/// <returns>A reference to comments.</returns>
		Comment& Comment( );

		/// <summary>
		/// Creates a new Namespace with optionally given name.
		/// <summary>
		/// <parameter IsConst="true" name="str">Name of this Namespace if any.</parameter>
		/// <returns>A proxy object that behaves like Namespace.</returns>
		static copying_pointer< Namespace > Instantiate( const std::wstring& str = L"" )
		{
			return copying_pointer< Namespace >( new Namespace( str ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed Namespace.</returns>
		Namespace& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this Namespace.
		/// </summary>
		/// <parameter IsConst="true" name="object">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		Namespace& Assign( const CodeObject& object );

		/// <summary>
		/// Provides access to name of this namespace.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>This namespace's name.</returns>
		const std::wstring& Name( ) const;

		/// <summary>
		/// Provides access to all entities declared in this namespaces.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A CodeCollection containing all the declarations.</returns>
		CodeCollection& Declarations( ) const;
	protected:
		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this Namespace</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
		/// <summary>
		/// Writes out this object split across the two streams.
		/// </summary>
		/// <parameter name="declas">Declaration output stream.</parameter>
		/// <parameter name="defos">Definition output stream.</parameter>
		/// <parameter name="tabs">Indentation for this Namespace.</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;
	private:
		/// <summary>
		/// Constructs a new Namespace object given it's name.
		/// </summary>
		/// <parameter IsConst="true" name="str">Name of this Namespace if any.</parameter>
		Namespace( const std::wstring& str );
		/// <summary>
		/// Copy constructs a new namespace from a given namespace.
		/// </summary>
		/// <parameter IsConst="true" name="other">Other namespace to copy into this one.</parameter>
		Namespace( const Namespace& other );
	};
}

#endif