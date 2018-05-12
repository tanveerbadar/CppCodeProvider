#ifndef COMPILATIONUNIT_HEADER
#define COMPILATIONUNIT_HEADER

#include <fstream>
#include "collections.h"

namespace CPlusPlusCodeProvider
{
	typedef TypedCollection< copying_pointer< CodeObject > > CodeCollection;

	/// <summary>
	/// Provides an abstraction of a source file or a header.
	/// </summary>
	class CompilationUnit
	{
		CodeCollection codeblock;

		std::wstring name;
		copying_pointer< Comment > comment;
	public:
		/// <summary>
		/// Returns a reference to the comments applied to this compilation unit.
		/// </summary>
		/// <returns>A reference to comments.</returns>
		Comment& Comment( );

		/// <summary>
		/// Construct a compilation unit with the given name.
		/// </summary>
		/// <parameter IsConst="true" name="file">Name of the file for this compilation unit.</parameter>
		CompilationUnit( const std::wstring& file );

		/// <summary>
		/// Provides access to all the declarations in this compilation unit.
		/// </summary>
		/// <returns>A collection of declarations defined in this compilation unit.</returns>
		CodeCollection& Declarations( );

		/// <summary>
		/// Returns name of this compilation unit.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>Name that was provided to the constructor.</returns>
		std::wstring Name( ) const;

		/// <summary>
		/// Writes out this compilation unit to the file whose name was given in the constructor.
		/// </summary>
		/// <IsConstMember value="true"/>
		void write( ) const;

		/// <summary>
		/// Writes out this compilation unit to the given stream.
		/// </summary>
		/// <parameter name="os">Stream to output this compilation unit to.</parameter>
		/// <IsConstMember value="true"/>
		void write( std::wostream& os ) const;
	};
}

#endif