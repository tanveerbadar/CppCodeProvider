#ifndef CPPCODEPROVIDER_HEADER
#define CPPCODEPROVIDER_HEADER

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iosfwd>
#include <string>
#include <memory>
#include "utility.h"

namespace CPlusPlusCodeProvider
{
	/// <summary>
	/// Base class of all code-graph constructs' hierarchy.
	/// </summary>
	class CodeObject
	{
	public:
		/// <summary>
		/// Destructs a CodeObject.
		/// </summary>
		virtual ~CodeObject( ) = 0;

		/// <summary>
		/// This function returns an exact copy of calling object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A copy of this object.</returns>
		virtual CodeObject& Duplicate( ) const = 0;
		/// <summary>
		/// This function assigns another object to this one.
		/// </summary>
		/// <parameter name="object" IsConst="true">This function assigns the given object to this one.</parameter>
		/// <returns>Invoking object with contents replaced by those of parameter.</returns>
		virtual CodeObject& Assign( const CodeObject& object ) = 0;

		/// <summary>
		/// Writes out this object to the given stream.
		/// </summary>
		/// <parameter name="os">Given Stream which receives the output.</parameter>
		/// <parameter name="tabs">Number of tabs to output at the start of each line</parameter>
		/// <IsConstMember value="true"/>
		void write( std::wostream& os , unsigned long tabs ) const;

		/// <summary>
		/// Writes out this object split across the two streams.
		/// </summary>
		/// <parameter name="declas">Declaration output stream.</parameter>
		/// <parameter name="defos">Definition output stream</parameter>
		/// <parameter name="tabs">Number of tabs to output at the start of each line</parameter>
		/// <IsConstMember value="true"/>
		void write( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;
	protected:
		/// <summary>
		/// Default constructor for CodeObject, does nothing.
		/// </summary>
		CodeObject( );
		/// <summary>
		/// Writes out this object to the given stream.
		/// </summary>
		/// <parameter name="os">Given Stream which receives the output.</parameter>
		/// <parameter name="tabs">Number of tabs to output at the start of each line.</parameter>
		/// <IsConstMember value="true"/>
		virtual void writetext( std::wostream& os , unsigned long tabs ) const = 0;

		/// <summary>
		/// Writes out this object split across the two streams.
		/// </summary>
		/// <parameter name="declas">Declaration output stream.</parameter>
		/// <parameter name="defos">Definition output stream</parameter>
		/// <parameter name="tabs">Number of tabs to output at the start of each line</parameter>
		/// <IsConstMember value="true"/>
		virtual void writetext( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const = 0;
	};

	/// <summary>
	/// This class encapsulates a comment.
	/// </summary>
	class Comment : public CodeObject
	{
		mutable std::wstring text;
		const CodeObject* object;
		mutable bool multiline;
		mutable bool object_source;
	public:
		/// <summary>
		/// Creates a new Comment given its contents.
		/// </summary>
		/// <parameter name="contents" IsConst="true">Contents that will be commented out.</parameter>
		/// <returns>A proxy object that behaves like a Comment.</returns>
		static copying_pointer< Comment > Instantiate( const std::wstring& contents = L"" )
		{
			return copying_pointer< Comment >( new Comment( contents ) );
		}

		/// <summary>
		/// Creates a new Comment from another code object, in essence writing it out in comments.
		/// </summary>
		/// <parameter name="object" IsConst="true">The CodeObject which will be commented.</parameter>
		/// <returns>A proxy object that behaves like a Comment.</returns>
		static copying_pointer< Comment > Instantiate( const CodeObject& object )
		{
			return copying_pointer< Comment >( new Comment( object ) );
		}

		/// <summary>
		/// Assigns another object to this Comment.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		Comment& Assign( const CodeObject& object );
		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed Comment.</returns>
		Comment& Duplicate( ) const;

		/// <summary>
		/// This function allows you to access the contents of this comment.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>Returns the text contents of this object.</returns>
		std::wstring Text( ) const;

		/// <summary>
		/// Returns a value whether this Comment is single line comment or block comment.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>True if this comment is a block comment, otherwise, false.</returns>
		bool MultiLine( ) const;
		/// <summary>
		/// Makes this comment a single line comment or block comment.
		/// </summary>
		/// <parameter name="flag">True to make this comment a single line comment, otherwise false for block comment.</parameter>
		Comment& MultiLine( bool flag );

		/// <summary>
		/// Tells whether this comment is empty or contains content.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>True if this comment is empty.</returns>
		bool IsEmpty( ) const;
	protected:
		/// <summary>
		/// This constructor constructs a Comment object given its contents.
		/// </summary>
		/// <parameter IsConst="true" name="content">Contents of the comment</parameter>
		Comment( const std::wstring& content );

		/// <summary>
		/// This constructor constructs a Comment object from another CodeObject.
		/// </summary>
		/// <parameter IsConst="true" name="object">Another CodeObject that will be written as comments.</parameter>
		Comment( const CodeObject& object );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this Comment</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;

		/// <summary>
		/// Writes out this object split across the two streams.
		/// </summary>
		/// <parameter name="declas">Declaration output stream.</parameter>
		/// <parameter name="defos">This parameter is not used.</parameter>
		/// <parameter name="tabs">Indentation for this Comment</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;
	};

	/// <summary>
	/// Base class for all types, provides common services like naming to all types.
	/// </summary>
	class Type : public CodeObject
	{
		std::wstring name;
		copying_pointer< Comment > comment;
	public:
		/// <summary>
		/// Returns a reference to the comments applied to this Type.
		/// </summary>
		/// <returns>A reference to comments.</returns>
		Comment& Comment( );

		/// <summary>
		/// Returns a reference to the comments applied to this Type.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A reference to comments.</returns>
		const CPlusPlusCodeProvider::Comment& Comment( ) const;

		/// <summary>
		/// Assigns a type to this object.
		/// </summary>
		/// <parameter IsConst="true" name="object">Given object which must be a type, its name is copied to this object.</parameter>
		/// <returns>This object with its name replaced by the other one's.</returns>
		Type& Assign( const CodeObject& object );

		/// <summary>
		/// This function allows you to access the name of this type.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>Returns the name of this type.</returns>
		virtual std::wstring Name( ) const;
	protected:
		/// <summary>
		/// Constructs a new object given its typename.
		/// </summary>
		/// <parameter IsConst="true" name="type_name">Name of this type</parameter>
		Type( const std::wstring& type_name );

		/// <summary>
		/// Assigns given object to this type.
		/// </summary>
		/// <parameter IsConst="true" name="other">Another type whose name is copied to this one.</parameter>
		/// <parameter>This parameter is not used.It is just there to help resolve this function.</parameter>
		void Assign( const Type& other , bool );
	};

	/// <summary>
	/// Base class for all expressions.
	/// </summary>
	class Expression : public CodeObject
	{
	protected:
		/// <summary>
		/// Constructs a default expression.
		/// </summary>
		Expression( );

		/// <summary>
		/// Writes out this object split across the two streams.
		/// </summary>
		/// <parameter name="declas">Declaration output stream.</parameter>
		/// <parameter name="defos">This parameter is not used.</parameter>
		/// <parameter name="decltabs">Indentation for this Expression</parameter>
		/// <parameter name="deftabs">This parameter is not used.</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;
	};

	/// <summary>
	/// Base class for all statements.
	/// </summary>
	class Statement : public CodeObject
	{
	protected:
		/// <summary>
		/// Constructs a default statement.
		/// </summary>
		Statement( );

		/// <summary>
		/// Writes out this object split across the two streams.
		/// </summary>
		/// <parameter name="declas">Declaration output stream.</parameter>
		/// <parameter name="defos">This parameter is not used.</parameter>
		/// <parameter name="decltabs">Indentation for this Statement</parameter>
		/// <parameter name="deftabs">This parameter is not used.</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;
	};

	/// <summary>
	/// This class allows you to put literal code in the expression tree.
	/// </summary>
	class CodeSnippet : public CodeObject
	{
		std::wstring snippet;
	public:
		/// <summary>
		/// This function allows you to get a CodeSnippet given a string.
		/// </summary>
		/// <parameter name="code" IsConst="true">Code snippet you want to output.</parameter>
		/// <returns>A proxy object that behaves like a CodeSnippet.</returns>
		static copying_pointer< CodeSnippet > Instantiate( const std::wstring& code )
		{
			return copying_pointer< CodeSnippet > ( new CodeSnippet( code ) );
		}

		/// <summary>
		/// Assigns another object to this CodeSnippet.
		/// </summary>
		/// <parameter IsConst="true" name="object">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		CodeSnippet& Assign( const CodeObject& object );
		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed CodeSnippet.</returns>
		CodeSnippet& Duplicate( ) const;
	protected:
		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">This parameter is not used.</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;

		/// <summary>
		/// Writes out this object split across the two streams.
		/// </summary>
		/// <parameter name="declas">Declaration output stream.</parameter>
		/// <parameter name="defos">This parameter is not used.</parameter>
		/// <parameter name="tabs">This parameter is not used.</parameter>
		void writetext( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;
	private:
		/// <summary>
		/// Constructs a new CodeSnippet given its contents.
		/// </summary>
		/// <parameter IsConst="true" name="contents">Contents that will be part of this CodeSnippet.</parameter>
		CodeSnippet( const std::wstring& contents );
	};

	class VariableDeclaration;
	template< typename T > class TypedCollection;
	typedef TypedCollection< copying_pointer< VariableDeclaration > > ParameterCollection;
	typedef TypedCollection< std::wstring > StringCollection;

	class Callable
	{
		std::wstring name;
		std::auto_ptr< StringCollection > exception_list;
		std::auto_ptr< ParameterCollection > argumentlist;
		std::auto_ptr< copying_pointer< VariableDeclaration > > return_type;
		copying_pointer< Comment > comment;
		int specs;
	public:
		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed Function.</returns>
		virtual Callable& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this Function.
		/// </summary>
		/// <parameter IsConst="true" name="object">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		virtual Callable& Assign( const Callable& object );

		/// <summary>
		/// Returns a constant reference to the comments applied to this function.
		/// </summary>
		/// <returns>A constant reference to comments.</returns>
		const Comment& Comment( ) const;

		/// <summary>
		/// Returns a reference to the comments applied to this function.
		/// </summary>
		/// <returns>A reference to comments.</returns>
		CPlusPlusCodeProvider::Comment& Comment( );

		virtual std::wstring Name( ) const;
		/// <summary>
		/// Provides access to all the arguments for this function call.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>An ExpressionCollection containing all the arguments.</returns>
		virtual ParameterCollection& Parameters( ) const;

		int ExceptionSpecifications( ) const;
		Callable& ExceptionSpecifications( int );

		/// <IsConstMember value="true"/>
		StringCollection& ExceptionList( ) const;

		/// <IsConstMember value="true"/>
		virtual copying_pointer< VariableDeclaration >* ReturnType( ) const;
	protected:
		Callable( const std::wstring& name );
		Callable( const std::wstring& name_ , copying_pointer< VariableDeclaration >& type );
		Callable( const Callable& other );

		/// <IsConstMember value="true"/>
		void writeparameters( std::wostream& ) const;
		/// <IsConstMember value="true"/>
		void writeexceptionspecs( std::wostream& ) const;
	};

	/// <summary>
	/// Tells which exceptions, if any or all, this function is allowed to throw under normal circumstances.
	struct ExceptionSpecification
	{
		/// <summary>
		/// This function does not throw any exceptions.
		/// </summary>
		static const int none = 0 , 
		/// <summary>
		/// This functions throws some exceptions provided in the exception specification list.
		/// </summary>
		some = 1, 
		/// <summary>
		/// This function is a classic C function and may throw any exception.
		/// </summary>
		all = 2;
	};

	/// <summary>
	/// Defines different compiler capabilities to allow adaptive output.
	/// </summary>
	struct CompilerCapabilities
	{
		typedef unsigned int Capability;

		/// <summary>
		/// Specifies that the compiler is a pain in the ass to work with.
		/// </summary>
		static const Capability		None													= 0x0000;
		/// <summary>
		/// Specifies that the compiler supports exceptoin specifications.
		/// </summary>
		static const Capability		ExceptionSpecifications					 = 0x0001;
		/// <summary>
		/// Specifies that the compiler supports function try-blocks.
		/// </summary>
		static const Capability		FunctionTryBlocks							 = 0x0002;
		/// <summary>
		/// Specifies that the compiler supports alternate keywords to logical and bitwise operators.
		/// </summary>
		static const Capability		AlternateKeywords							 = 0x0004;
		/// <summary>
		/// Specifies that the compiler supports export keyword.
		/// </summary>
		static const Capability		ExportKeyword									 = 0x0008;
		/// <summary>
		/// Specifies that the compiler supports out-of-class template member definitions.
		/// </summary>
		static const Capability		OutofClassTemplates						 = 0x0010;
	};

	struct FormattingData
	{
		/// <summary>
		/// Specifies the indentation character which is used while inserting whitespace.
		/// </summary>
		static wchar_t IndentationCharacter;
	};
}

#endif