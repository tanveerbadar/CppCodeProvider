#ifndef DECLARATIONS_HEADER
#define DECLARATIONS_HEADER

#include <memory>
#include "cppcodeprovider.h"

namespace CPlusPlusCodeProvider
{
	/// <summary>
	/// Represents a declaration's type.
	/// </summary>
	class DeclaratorSpecifier
	{
		Type* type;
		bool is_constant , is_volatile , is_static , is_extern;
	public:
		/// <summary>
		/// Constructs a new declarator given it's type.
		/// </summary>
		/// <parameter IsConst="true" name="name">Type of the declarator.</parameter>
		DeclaratorSpecifier( Type& name );

		/// <summary>
		/// Returns whether the declarator is static or not.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>True if the declarator is static, otherwise, false.</returns>
		bool Static( ) const;
		/// <summary>
		/// Set a value for this declarator to be static or not.
		/// </summary>
		/// <parameter name="flag">True to make this declarator static, otherwise, false.</parameter>
		/// <returns>A reference to this object.</returns>
		DeclaratorSpecifier& Static( bool flag );

		/// <summary>
		/// Returns whether the declarator is extern or not.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>True if the declarator is extern, otherwise, false.</returns>
		bool Extern( ) const;
		/// <summary>
		/// Set a value for this declarator to be extern or not.
		/// </summary>
		/// <parameter name="flag">True to make this declarator extern, otherwise, false.</parameter>
		/// <returns>A reference to this object.</returns>
		DeclaratorSpecifier& Extern( bool flag );

		/// <summary>
		/// Returns whether the declarator is volatile or not.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>True if the declarator is volatile, otherwise, false.</returns>
		bool Volatile( ) const;
		/// <summary>
		/// Set a value for this declarator to be volatile or not.
		/// </summary>
		/// <parameter name="flag">True to make this declarator volatile, otherwise, false.</parameter>
		/// <returns>A reference to this object.</returns>
		DeclaratorSpecifier& Volatile( bool flag );

		/// <summary>
		/// Returns whether the declarator is constant or not.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>True if the declarator is constant, otherwise, false.</returns>
		bool Constant( ) const;
		/// <summary>
		/// Set a value for this declarator to be constant or not.
		/// </summary>
		/// <parameter name="flag">True to make this declarator constant, otherwise, false.</parameter>
		/// <returns>A reference to this object.</returns>
		DeclaratorSpecifier& Constant( bool flag );

		/// <summary>
		/// Returns the type of declarator.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>The type of declarator.</returns>
		const Type& Type( ) const;

		/// <summary>
		/// Outputs this declarator specifier to the given stream.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <parameter name="os">Output stream which receives this declarator specifier.</parameter>
		void write( std::wostream& os ) const;
	};

	typedef TypedCollection< int > IntegerCollection;

	class VariableDeclaration;

	/// <summary>
	/// Represents a declarator in code graph.
	/// </summary>
	class Declarator
	{
		std::wstring name;
		bool is_reference , is_constant , is_volatile;
		mutable bool has_initializer;
		unsigned int level;
		std::auto_ptr< IntegerCollection > indices;
		std::auto_ptr< copying_pointer< Expression > > initializer;

		friend class VariableDeclaration;
	public:
		/// <summary>
		/// Tells whether this declarator has an initializer expression associated with it or not.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>True if an initializer expression is provided.</returns>
		bool HasInitializer( ) const;
		/// <summary>
		/// Gets a reference to the initializer expression if any.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A reference to expression that was given in the argument to Instantiate.</returns>
		Expression& Initializer( ) const;
		/// <summary>
		/// Sets the initializer expression to a new one.
		/// </summary>
		/// <parameter name="expr">New expressi0on to initialize with.</parameter>
		/// <returns> This object with the new initializer expression</returns>
		Declarator& Initializer( const copying_pointer< Expression >& expr );

		/// <summary>
		/// Sets name of the variable to a new value.
		/// </summary>
		/// <parameter IsConst="true" name="name">New name for the variable.</parameter>
		Declarator& Name( const std::wstring& name );
		/// <summary>
		/// Gets name of the variable.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>Returns name of this variable.</returns>
		std::wstring Name( ) const;

		/// <summary>
		/// Returns whether the variable is a reference or not.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>True if the variable is a reference, otherwise, false.</returns>
		bool Reference( ) const;
		/// <summary>
		/// Set a value for this variable to be a reference or not.
		/// </summary>
		/// <parameter name="flag">True to make this variable a reference, otherwise, false.</parameter>
		/// <returns>A reference to this object.</returns>
		Declarator& Reference( bool flag );

		/// <summary>
		/// Returns the indirection level of this variable.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>The number of indirection operators applied to this variable.</returns>
		unsigned int IndirectionLevel( ) const;
		/// <summary>
		/// Sets the indirection level for this variable.
		/// </summary>
		/// <parameter name="value">Indirection level for this variable.</parameter>
		/// <returns>A reference to this object.</returns>
		Declarator& IndirectionLevel( unsigned int value );

		/// <summary>
		/// Returns whether the declarator is volatile or not.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>True if the declarator is volatile, otherwise, false.</returns>
		bool Volatile( ) const;
		/// <summary>
		/// Set a value for this declarator to be volatile or not.
		/// </summary>
		/// <parameter name="flag">True to make this declarator volatile, otherwise, false.</parameter>
		/// <returns>A reference to this object.</returns>
		Declarator& Volatile( bool flag );

		/// <summary>
		/// Returns whether the declarator is constant or not.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>True if the declarator is constant, otherwise, false.</returns>
		bool Constant( ) const;
		/// <summary>
		/// Set a value for this declarator to be constant or not.
		/// </summary>
		/// <parameter name="flag">True to make this declarator constant, otherwise, false.</parameter>
		/// <returns>A reference to this object.</returns>
		Declarator& Constant( bool flag );

		/// <summary>
		/// Returns the indices in the declaration for an array.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A collection of integers which are the indices of array in this declaration.</returns>
		IntegerCollection& Indices( ) const;

		/// <summary>
		/// Copy constructs a VariableDeclaration from another one.
		/// </summary>
		/// <parameter IsConst="true" name="other">Other VariableDeclaration to use as a source.</parameter>
		Declarator( const Declarator& other );
		/// <summary>
		/// Creates a new VariableDeclaration object from the given arguments.
		/// </summary>
		/// <parameter IsConst="true" name="str">Name of the variable.</summary>
		Declarator( const std::wstring& str );

		/// <summary>
		/// Assigns a new Declarator to this object.
		/// </summary>
		/// <parameter name="other" IsConst="true">Another Declarator to assign to this object.</parameter>
		/// <returns>Reference to this object with its contents being replaced by the given Declarator.</returns>
		Declarator& operator = ( const Declarator& other );

		/// <summary>
		/// Outputs this declarator to the given stream.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <parameter name="os">Output stream which receives this declarator.</parameter>
		void write( std::wostream& os ) const;
	};

	class Declaration : public CodeObject
	{
		DeclaratorSpecifier type;
		copying_pointer< Comment > comment;
	public:
		/// <summary>
		/// Returns a reference to the comments applied to this variable declaration.
		/// </summary>
		/// <returns>A reference to comments.</returns>
		Comment& Comment( );

		/// <summary>
		/// Returns a reference to the comments applied to this variable declaration.
		/// </summary>
		/// <returns>A reference to comments.</returns>
		/// <IsConstMember value="true"/>
		const CPlusPlusCodeProvider::Comment& Comment( ) const;

		/// <summary>
		/// Returns the declarator specifier for this variable declaration.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A DeclaratorSpecifier for this declaration.</returns>
		/// <IsConstMember value="true"/>
		const DeclaratorSpecifier& Specifier( ) const;

		/// <summary>
		/// Returns the declarator specifier for this variable declaration.
		/// </summary>
		/// <returns>A DeclaratorSpecifier for this declaration.</returns>
		DeclaratorSpecifier& Specifier( );
	protected:
		Declaration( Type& specifier );
		~Declaration( ) = 0;

		void Assign( const Declaration& other , bool );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this Declaration</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	/// <summary>
	/// Represents a variable declaration in code graph.
	/// </summary>
	class VariableDeclaration : public Declaration
	{
		Declarator declarator;
	public:
		/// <summary>
		/// Creates a new VariableDeclaration from its specifier and declarator.
		/// </summary>
		/// <parameter name="specifier">The declarator specifier for this variable declaration.</parameter>
		/// <parameter name="declarator">The declarator for this variable declaration.</parameter>
		/// <returns>A proxy object that acts like a VariableDeclaration.</returns>
		static copying_pointer< VariableDeclaration > Instantiate( Type& specifier , const std::wstring& declarator )
		{
			return copying_pointer< VariableDeclaration >( new VariableDeclaration( specifier , declarator ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed VariableDeclaration.</returns>
		VariableDeclaration& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this VariableDeclaration.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		VariableDeclaration& Assign( const CodeObject& object );

		/// <summary>
		/// Returns the declarator for this variable declaration.
		/// </summary>
		/// <returns>A Declarator for this declaration.</returns>
		Declarator& Declarator( );
		/// <summary>
		/// Returns the declarator for this variable declaration.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A Declarator for this declaration.</returns>
		const CPlusPlusCodeProvider::Declarator& Declarator( ) const;

		/// <IsConstMember value="true"/>
		void writeasparameter( std::wostream& os ) const;

		void declaration( std::wostream& os , unsigned long tabs ) const;
	protected:
		/// <summary>
		/// Constructs a new VariableDeclaration from its specifier and declarator.
		/// </summary>
		/// <parameter name="specifier">The declarator specifier for this variable declaration.</parameter>
		/// <parameter name="declarator">The declarator for this variable declaration.</parameter>
		VariableDeclaration( Type& , const std::wstring& );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this VariableDeclaration</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
		/// <summary>
		/// Writes out this object split across the two streams.
		/// </summary>
		/// <parameter name="declas">Declaration output stream.</parameter>
		/// <parameter name="defos">Definition output stream.</parameter>
		/// <parameter name="tabs">Indentation for this VariableDeclaration.</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;
	};

	typedef TypedCollection< Declarator > DeclaratorList;

	/// <summary>
	/// Represents multiple variable declarations in code graph.
	/// </summary>
	class VariableDeclarationList : public Declaration
	{
		std::auto_ptr< DeclaratorList > declarators;
	public:
		/// <summary>
		/// Creates a new VariableDeclaration from its specifier.
		/// </summary>
		/// <parameter name="specifier">The declarator specifier for this declaration.</parameter>
		/// <returns>A proxy object that acts like VariableDeclaration.</returns>
		static copying_pointer< VariableDeclarationList > Instantiate( Type& specifier )
		{
			return copying_pointer< VariableDeclarationList >( new VariableDeclarationList( specifier ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed VariableDeclarationList.</returns>
		VariableDeclarationList& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this VariableDeclarationList.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		VariableDeclarationList& Assign( const CodeObject& );
		/// <summary>
		/// Returns the list of declarators for this variable declaration.
		/// </summary>
		/// <returns>A DeclaratorList for this declaration.</returns>
		DeclaratorList& Declarators( );
	protected:
		/// <summary>
		/// Constructs a new VariableDeclaration from its specifier.
		/// </summary>
		/// <parameter name="specifier">The declarator specifier for this declaration.</parameter>
		VariableDeclarationList( Type& specifier );
		/// <summary>
		/// Constructs a new VariableDeclaration from another one.
		/// </summary>
		/// <parameter name="other">Another VariableDeclaration to use as source object.</parameter>
		VariableDeclarationList( const VariableDeclarationList& other );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this VariableDeclarationList</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
		/// <summary>
		/// Writes out this object split across the two streams.
		/// </summary>
		/// <parameter name="declas">Declaration output stream.</parameter>
		/// <parameter name="defos">Definition output stream.</parameter>
		/// <parameter name="tabs">Indentation for this VariableDeclarationList.</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;
	};
}

#endif