#ifndef PREPROCESSOR_HEADER
#define PREPROCESSOR_HEADER

#include "cppcodeprovider.h"
#include <memory>

namespace CPlusPlusCodeProvider
{
	/// <summary>
	/// Represents a preprocessing directive in code graph.
	/// </summary>
	class PreprocessorDirective : public CodeObject
	{
		std::wstring directive;//The preprocessor directive
		bool multiline;//Used to tell if the directive spans multiple lines.
		copying_pointer< Comment > comment;
	public:
		/// <summary>
		/// Returns a reference to the comments applied to this preprocessor directive.
		/// </summary>
		/// <returns>A reference to comments.</returns>
		Comment& Comment( );

		/// <summary>
		/// Dictates the specific type of preprocessor directive
		/// </summary>
		enum DirectiveType
		{
			/// <summary>
			/// A pragma directive.
			/// </summary>
			Pragma , 
			/// <summary>
			/// A file include.
			/// </summary>
			Include , 
			/// <summary>
			/// A macro definition.
			/// </summary>
			Define , 
			/// <summary>
			/// A macro undefine
			/// </summary>
			UnDefine , 
			/// <summary>
			/// The default preprocessor directive
			/// </summary>
			Default
		};

		/// <summary>
		/// Assigns another object to this PreprocessorDirective.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		PreprocessorDirective& Assign( const CodeObject& object );
		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed PreprocessorDirective.</returns>
		PreprocessorDirective& Duplicate( ) const;

		/// <summary>
		/// Creates a new PreprocessorDirective from a given string and directive type.
		/// </summary>
		/// <parameter IsConst="true" name="directive">The text which follows this directive,</parameter>
		/// <parameter name="type">Type of this PreprocessorDirective.</parameter>
		/// <returns>A proxy object that acts like a PreprocessorDirective.</returns>
		static copying_pointer< PreprocessorDirective > Instantiate( const std::wstring& directive , DirectiveType type = Default )
		{
			return copying_pointer< PreprocessorDirective >( new PreprocessorDirective( directive , type ) );
		}

		/// <summary>
		/// Returns the directive given to Instantiate.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A reference to the directive for this object.</returns>
		const std::wstring& Directive( ) const;

		/// <summary>
		/// Returns the directive type for this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A DirectiveType enumeration member for this object.</returns>
		DirectiveType Type( ) const;


		/// <summary>
		/// Returns a value that tell whether this directive is multiline or not.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>True if this directive is multiline, otherwise, false.</returns>
		bool MultiLine( ) const;

		/// <summary>
		/// Sets a value to indicate whether this directive is multiline or not.
		/// </summary>
		/// <parameter name="flag">A value which decides this directive to be multiline or not.</parameter>
		/// <returns>A reference to this object</returns>
		PreprocessorDirective& MultiLine( bool flag );
	protected:
		/// <summary>
		/// Constructs a new PreprocessorDirective from a given string and directive type.
		/// </summary>
		/// <parameter IsConst="true" name="directive">The text which follows this directive,</parameter>
		/// <parameter name="type">Type of this PreprocessorDirective.</parameter>
		PreprocessorDirective( const std::wstring& directive , DirectiveType type = Default );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this PreprocessorDirective</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
		/// <summary>
		/// Writes out this object split across the two streams.
		/// </summary>
		/// <parameter name="declas">Declaration output stream.</parameter>
		/// <parameter name="defos">Definition output stream.</parameter>
		/// <parameter name="tabs">Indentation for this PreprocessorDirective.</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;

		/// <summary>
		/// Assigns a PreprocessorDirective to another.
		/// </summary>
		/// <parameter name="other">The other PreprocessorDirective to assign to this one.</parameter>
		void Assign( const PreprocessorDirective& other);
	private:
		DirectiveType type;
	};

	class CompilationUnit;
	typedef TypedCollection< copying_pointer< CodeObject > > CodeCollection;

	/// <summary>
	/// Represents one of #if, #ifdef or #ifndef directives in code graph.
	/// </summary>
	class MacroTestDirective : public PreprocessorDirective
	{
		std::auto_ptr< CompilationUnit > codeblock;
	public:
		/// <summary>
		/// Creates a new MacroTestDirective given the text for it.
		/// </summary>
		/// <parameter IsConst="true" name="directive">The text which will follow #if.</parameter>
		/// <returns>A proxy object that acts like MacroTestDirective.</returns>
		static copying_pointer< MacroTestDirective > Instantiate( const std::wstring& directive )
		{
			return copying_pointer< MacroTestDirective >( new MacroTestDirective( directive ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed MacroTestDirective.</returns>
		MacroTestDirective& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this MacroTestDirective.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		MacroTestDirective& Assign( const CodeObject& object );

		/// <summary>
		/// Provides access to all the declarations in this preprocessor directive.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A collection of declarations defined in this preprocessor directive.</returns>
		CodeCollection& Declarations( ) const;
	protected:
		MacroTestDirective( const MacroTestDirective& );
		/// <summary>
		/// Constructs a new MacroTestDirective given the text for it.
		/// </summary>
		/// <parameter IsConst="true" name="directive">The text which will follow #if.</parameter>
		MacroTestDirective( const std::wstring& directive );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this MacroTestDirective</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};
}

#endif