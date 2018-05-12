#ifndef TYPES_HEADER
#define TYPES_HEADER

#include "cppcodeprovider.h"
#include <vector>
#include <map>

namespace CPlusPlusCodeProvider
{
	class Constructor;
	class Destructor;
	class VariableDeclaration;
	class MemberFunction;
	class MemberOperator;
	typedef TypedCollection< copying_pointer< MemberFunction > > MemberFunctionCollection;
	typedef TypedCollection< copying_pointer< MemberOperator > > MemberOperatorCollection;
	typedef TypedCollection< std::tuple< copying_pointer< VariableDeclaration > , AccessRestrictions , bool > > MemberVariableCollection;

	class Typedefinition;

	/// <summary>
	/// Represents a typedefinition of another type in code graph.
	/// </summary>
	class TypedefinedType : public Type
	{
		static const std::wstring& validate( const copying_pointer< Typedefinition >& , const std::wstring& );
	public:
		/// <summary>
		/// Creates a new TypedefinedType object from a typedefinition and a synonym from it.
		/// </summary>
		/// <parameter IsConst="true" name="typedefinition">Typedefinition which will be used for this type.</parameter>
		/// <parameter IsConst="true" name="name">A synonym in the given typedefinition.</parameter>
		/// <exception ref="std::invalid_argument">Thrown if name does not exist in the synonyms list of given typedefinition.</exception>
		/// <returns>A proxy object that acts like a TypedefinedType.</returns>
		static copying_pointer< TypedefinedType > Instantiate( const copying_pointer< Typedefinition >& typedefinition , const std::wstring& name )
		{
			return copying_pointer< TypedefinedType >( new TypedefinedType( typedefinition , name ) );
		}

		/// <summary>
		/// Assigns another object to this TypedefinedType.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		TypedefinedType& Assign( const CodeObject& object );
		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed TypedefinedType.</returns>
		TypedefinedType& Duplicate( ) const;
	protected:
		/// <summary>
		/// Constructs a new TypedefinedType object from a typedefinition and a synonym from it.
		/// </summary>
		/// <parameter name="typedefinition" IsConst="true">Typedefinition which will be used for this type.</parameter>
		/// <parameter name="name" IsConst="true">A synonym in the given typedefinition.</parameter>
		/// <exception ref="std::invalid_argument">Thrown if name does not exist in the synonyms list of given typedefinition.</exception>
		TypedefinedType( const copying_pointer< Typedefinition >& , const std::wstring& );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this TypedefinedType</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;

		/// <summary>
		/// Writes out this object split across the two streams.
		/// </summary>
		/// <parameter name="declas">Declaration output stream.</parameter>
		/// <parameter name="defos">Definition output stream.</parameter>
		/// <parameter name="tabs">Indentation for this TypedefinedType.</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;
	};

	/// <summary>
	/// Base class for all template parameters.
	/// </summary>
	class TemplateParameter : public Type
	{
	protected:
		/// <summary>
		/// Constructs a new TemplateParameter given its name.
		/// </summary>
		/// <parameter IsConst="true" name="name">Name of template parameter.</parameter>
		TemplateParameter( const std::wstring& name );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this TemplateParameter</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;

		/// <summary>
		/// Writes out this object split across the two streams.
		/// </summary>
		/// <parameter name="declas">Declaration output stream.</parameter>
		/// <parameter name="defos">Definition output stream.</parameter>
		/// <parameter name="tabs">Indentation for this TemplateParameter.</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;
	};

	/// <summary>
	/// Represents a non-type template parameter in code-graph.
	/// </summary>
	class NontypeParameter : public TemplateParameter
	{
		/// <summary>
		/// Validates a given name so that it represents a built-in type or a pointer or a reference to some other type.
		/// </summary>
		/// <parameter IsConst="true" name="name">Name of the template parameter.</parameter>
		/// <IsConstMember value="true"/>
		/// <exception ref="std::invalid_argument">Thrown if name is not an integral type or a pointer to a floating point type.</exception>
		/// <returns>Given name if it is a primitive type string.</returns>
		const std::wstring& validate( const std::wstring& name ) const;

		mutable bool is_integral;
	public:
		/// <summary>
		/// Creates a new non-type parameter given an fundamental integral type or pointer to a floating pointer type.
		/// </summary>
		/// <parameter IsConst="true" name="name">Name of the fundamental integral type or pointer to a floating pointer type.</parameter>
		/// <exception ref="std::invalid_argument">Thrown if name is not an integral type or a pointer to a floating point type.</exception>
		/// <returns>A proxy object that acts like NontypeParameter.</returns>
		static copying_pointer< NontypeParameter > Instantiate( const std::wstring& name )
		{
			return copying_pointer< NontypeParameter >( new NontypeParameter( name ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed NontypeParameter.</returns>
		NontypeParameter& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this NontypeParameter.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		NontypeParameter& Assign( const CodeObject& object );

		/// <summary>
		/// Returns true only if the non-type parameter is an integral type.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>True only if the non-type parameter is an integral one.</returns>
		bool IsIntegral( ) const;
	protected:
		/// <summary>
		/// Constructs a new NontypeParameter given its name.
		/// </summary>
		/// <parameter IsConst="true" name="name">Name of template parameter.</parameter>
		NontypeParameter( const std::wstring& name );
	};

	/// <summary>
	/// Represents an unbound typed template parameter in code-graph.
	/// </summary>
	class TypedParameter : public TemplateParameter
	{
		/// <summary>
		/// Validates a given name so that it does not represent a built-in type or a pointer or a reference to some other type.
		/// </summary>
		/// <parameter IsConst="true" name="name">Name of the template parameter.</parameter>
		/// <IsConstMember value="true"/>
		/// <exception ref="std::invalid_argument">Thrown if name is an integral type or a pointer to a floating point type.</exception>
		/// <returns>Given name if it is not a built-in type.</returns>
		const std::wstring& validate( const std::wstring& name ) const;
	public:
		/// <summary>
		/// Creates a new TypedParameter given its name.
		/// </summary>
		/// <parameter IsConst="true" name="name">Name of the template parameter.</parameter>
		/// <exception ref="std::invalid_argument">Thrown if name is a fundamental type.</exception>
		/// <returns>A proxy object that acts like TypedParameter.</returns>
		static copying_pointer< TypedParameter >Instantiate( const std::wstring& name )
		{
			return copying_pointer< TypedParameter >( new TypedParameter( name ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed TypedParameter.</returns>
		TypedParameter& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this TypedParameter.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		TypedParameter& Assign( const CodeObject& object );
	protected:
		/// <summary>
		/// Constructs a new TypedParameter given its name.
		/// </summary>
		/// <parameter IsConst="true" name="name">Name of template parameter.</parameter>
		TypedParameter( const std::wstring& );
	};

	/// <summary>
	/// Represents a template template parameter in code-graph.
	/// </summary>
	class TemplateTemplateParameter : public TemplateParameter
	{
		/// <summary>
		/// Validates a given name so that it does not represent a built-in type or a pointer or a reference to some other type.
		/// </summary>
		/// <parameter IsConst="true" name="name">Name of the template parameter.</parameter>
		/// <IsConstMember value="true"/>
		/// <exception ref="std::invalid_argument">Thrown if name is an integral type or a pointer to a floating point type.</exception>
		/// <returns>Given name if it is not a built-in type.</returns>
		const std::wstring& validate( const std::wstring& ) const;

		unsigned int arguments;
	public:
		/// <summary>
		/// Constructs a new TemplateParameter given its name and number of template arguments.
		/// </summary>
		/// <parameter IsConst="true" name="name">Name of template parameter.</parameter>
		/// <parameter name="count">Number of template arguments that this parameter will take.</parameter>
		/// <exception ref="std::invalid_argument">Thrown if name is a fundamental type.</exception>
		/// <returns>A proxy object that acts like TemplateTemplateParameter.</returns>
		static copying_pointer< TemplateTemplateParameter > Instantiate( const std::wstring& name , unsigned int args )
		{
			if( args < 1 || args > 64 )
				throw std::invalid_argument( "" );
			return copying_pointer< TemplateTemplateParameter >( new TemplateTemplateParameter( name , args ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed TemplateTemplateParameter.</returns>
		TemplateTemplateParameter& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this TemplateTemplateParameter.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		TemplateTemplateParameter& Assign( const CodeObject& object );
	protected:
		/// <summary>
		/// Constructs a new TemplateParameter given its name and number of template arguments.
		/// </summary>
		/// <parameter name="name" IsConst="true">Name of template parameter.</parameter>
		/// <parameter name="count">Number of template arguments that this parameter will take.</parameter>
		TemplateTemplateParameter( const std::wstring& name , unsigned int count );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this TemplateTemplateParameter</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	/// <summary>
	/// Represents a built-in type in code-graph.
	/// </summary>
	class BasicType : public Type
	{
	public:
		/// <summary>
		/// Creates a new BasicType given the name of a fundamental type.
		/// </summary>
		/// <parameter IsConst="true" name="str">Name of the fundamental type.</parameter>
		/// <exception ref="std::invalid_argument">Thrown if str is not a fundamental type or void.</exception>
		/// <returns>A proxy object that acts like BasicType.</returns>
		static copying_pointer< BasicType > Instantiate( const std::wstring& str )
		{
			return copying_pointer< BasicType >( new BasicType( str ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed BasicType.</returns>
		BasicType& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this BasicType.
		/// </summary>
		/// <parameter IsConst="true" name="object">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		BasicType& Assign( const CodeObject& object );

		/// <summary>
		/// Validates a given name so that it represents a built-in type or a pointer or a reference to some other type.
		/// </summary>
		/// <parameter IsConst="true" name="name">Name of the type.</parameter>
		/// <IsConstMember value="true"/>
		/// <exception ref="std::invalid_argument">Thrown if name is not an integral type or a pointer to a floating point type.</exception>
		/// <returns>Given name if it is a primitive type string.</returns>
		static const std::wstring& validate( const std::wstring& name );
	protected:
		/// <summary>
		/// Constructs a new BasicType given the name of a fundamental type.
		/// </summary>
		/// <parameter IsConst="true" name="str">Name of the fundamental type.</parameter>
		BasicType( const std::wstring& str );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this BasicType</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const; //in-appropriate

		/// <summary>
		/// Writes out this object split across the two streams.
		/// </summary>
		/// <parameter name="declas">Declaration output stream.</parameter>
		/// <parameter name="defos">Definition output stream.</parameter>
		/// <parameter name="tabs">Indentation for this BasicType.</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;
	};

	class nestabletype
	{
	public:
		void writetemplateattributes( std::wostream& os ) const;
		void writeelaboratedname( std::wostream& os ) const;
	protected:
		virtual void writetemplateattributesrecursive( std::wostream& os ) const = 0;
		virtual void writeelaboratednamerecursive( std::wostream& os ) const = 0;
	};

	typedef TypedCollection< copying_pointer< TemplateParameter > > TemplateParameterCollection;

	/// <summary>
	/// Represents an unnamed union in code-graph.
	/// </summary>
	class Union : public Type , public nestabletype
	{
		std::auto_ptr< MemberVariableCollection > membervariables;
		std::auto_ptr< MemberFunctionCollection > memberfunctions;
		std::auto_ptr< MemberOperatorCollection > memberoperators;
		std::auto_ptr< TemplateParameterCollection > template_parameters;

		mutable std::vector< const CodeObject* > render_backlog;
		bool is_templated , is_inline;
		nestabletype* enclosingtype;
	public:
		/// <summary>
		/// Creates a new unnamed union
		/// </summary>
		/// <parameter IsConst="true" name="name">Name of the union.</parameter>
		/// <returns>A proxy object that acts like a union.</returns>
		static copying_pointer< Union > Instantiate( const std::wstring& name = L"" )
		{
			return copying_pointer< Union >( new Union( name ) );
		}

		Union& Assign( const nestabletype& object );

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed Union.</returns>
		Union& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this Union.
		/// </summary>
		/// <parameter IsConst="true" name="object">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		Union& Assign( const CodeObject& object );

		nestabletype* EnclosingType( ) const;
		Union& EnclosingType( nestabletype* type );

		/// <summary>
		/// Returns a collection of member variables.
		/// </summary>
		/// <exception ref="std::exception">Thrown if the union is an unnamed union.</exception>
		/// <IsConstMember value="true"/>
		/// <returns>A MemberVariableCollection containing all member variables defined in this union.</returns>
		MemberVariableCollection& MemberVariables( ) const;

		/// <summary>
		/// Returns a collection of member functions.
		/// </summary>
		/// <exception ref="std::exception">Thrown if the union is an unnamed union.</exception>
		/// <IsConstMember value="true"/>
		/// <returns>A MemberFunctionCollection containing all member functions defined in this union.</returns>
		MemberFunctionCollection& MemberFunctions( ) const;

		/// <summary>
		/// Returns a collection of member operators.
		/// </summary>
		/// <exception ref="std::exception">Thrown if the union is an unnamed union.</exception>
		/// <IsConstMember value="true"/>
		/// <returns>A MemberOperatorCollection containing all member operators defined in this union.</returns>
		MemberOperatorCollection& MemberOperators( ) const;

		bool Inline( ) const;
		Union& Inline( bool flag );
	protected:
		/// <summary>
		/// Construction a default Union object.
		/// </summary>
		/// <parameter IsConst="true" name="name">Name of the union.</parameter>
		Union( const std::wstring& name = L"" );
		/// <summary>
		/// Copy constructs a Union object given another one.
		/// </summary>
		/// <parameter IsConst="true" name="other">Other Union to use a source object.</parameter>
		Union( const Union& other );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this Union</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;

		/// <summary>
		/// Writes out this object split across the two streams.
		/// </summary>
		/// <parameter name="declas">Declaration output stream.</parameter>
		/// <parameter name="defos">Definition output stream.</parameter>
		/// <parameter name="tabs">Indentation for this Union.</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;

		void writetemplateattributesrecursive( std::wostream& os ) const;
		void writeelaboratednamerecursive( std::wostream& ) const;
		private:
			void Assign( const Union& object , bool );
	};

	typedef std::pair< copying_pointer< Comment > , std::wstring > key_type;
}

namespace std
{
	template< > struct less< CPlusPlusCodeProvider::key_type > : public binary_function< CPlusPlusCodeProvider::key_type , CPlusPlusCodeProvider::key_type , bool >
	{
		bool operator( ) ( const first_argument_type& arga , const second_argument_type& argb ) const
		{
			return arga.second < argb.second;
		}
	};
}

namespace CPlusPlusCodeProvider
{
	typedef TypedCollection< std::wstring > StringCollection;
	typedef std::map< key_type , copying_pointer< Expression > > enumeratorlist;

	/// <summary>
	/// Represents an enumeration in code-graph.
	/// </summary>
	class Enumeration : public Type , public nestabletype
	{
		enumeratorlist enumerators;
		nestabletype* enclosingtype;
	public:
		/// <summary>
		/// Creates a new enumeration given its optional name.
		/// </summary>
		/// <parameter IsConst="true" name="str">Name of this enumeration.</parameter>
		/// <returns>A proxy object that acts like Enumeration.</returns>
		static copying_pointer< Enumeration > Instantiate( const std::wstring& str = L"" )
		{
			return copying_pointer< Enumeration >( new Enumeration( str ) );
		}

		/// <summary>
		/// Creates a new enumeration given its optional name and enumerators.
		/// </summary>
		/// <parameter IsConst="true" name="str">Name of this enumeration.</parameter>
		/// <parameter name="enumerators">Some of the enumerators with optional values in this enumeration.</parameter>
		/// <returns>A proxy object that acts like Enumeration.</returns>
		static copying_pointer< Enumeration > Instantiate( const std::wstring& str , const enumeratorlist& enumerators )
		{
			return copying_pointer< Enumeration >( new Enumeration( str , enumerators ) );
		}

		Enumeration& Assign( const nestabletype& object );

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed Enumeration.</returns>
		Enumeration& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this Enumeration.
		/// </summary>
		/// <parameter IsConst="true" name="object">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		Enumeration& Assign( const CodeObject& object );

		/// <IsConstMember value="true"/>
		enumeratorlist& Enumerators( );

		Enumeration& EnclosingType( nestabletype* enclosing );
		nestabletype* EnclosingType( ) const;
	protected:
		/// <summary>
		/// Constructs a new enumeration given its optional name.
		/// </summary>
		/// <parameter IsConst="true" name="str">Name of this enumeration.</parameter>
		Enumeration( const std::wstring& str = L"" );
		/// <summary>
		/// Constructs a new enumeration given its optional name and enumerators.
		/// </summary>
		/// <parameter IsConst="true" name="str">Name of this enumeration.</parameter>
		/// <parameter name="enumerators">Some of the enumerators with optional values in this enumeration.</parameter>
		Enumeration( const std::wstring& str , const enumeratorlist& enumerators );
		/// <summary>
		/// Constructs a new enumeration from another one.
		/// </summary>
		/// <parameter IsConst="true" name="other">Another enumeration to use for construction.</parameter>
		Enumeration( const Enumeration& other );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this Enumeration</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;

		/// <summary>
		/// Writes out this object split across the two streams.
		/// </summary>
		/// <parameter name="declas">Declaration output stream.</parameter>
		/// <parameter name="defos">Definition output stream.</parameter>
		/// <parameter name="tabs">Indentation for this Enumeration.</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;

		void writetemplateattributesrecursive( std::wostream& os ) const;
		void writeelaboratednamerecursive( std::wostream& os ) const;
	private:
		void Enumeration::Assign( const Enumeration& ref , bool flag );
	};

	class UserDefinedType;

	/// <summary>
	/// Represents a base class in a class definition.
	/// </summary>
	class BaseType
	{
		AccessRestrictions access;
		UserDefinedType *actual;
		std::auto_ptr< StringCollection > arguments;
		bool is_virtual;
	public:
		/// <summary>
		/// Constructs a new BaseType object from a UserDefinedType, its template arguments if any and access type for this base class.
		/// </summary>
		/// <parameter name="type">UserDefinedType which is the base class.</parameter>
		/// <parameter name="args">Template arguments for the base class if any.</parameter>
		/// <parameter name="access">Access type for base class.</parameter>
		BaseType( UserDefinedType& type , const StringCollection& args , AccessRestrictions access );
		/// <summary>
		/// Constructs a BaseType object from another one.
		/// </summary>
		/// <parameter name="other">Another BaseType object which is copied into this one.</parameter>
		BaseType( const BaseType& other );
		/// <summary>
		/// Assigns another BaseType object to this one.
		/// </summary>
		/// <parameter name="other">Another BaseType object which is copied into this one.</parameter>
		/// <returns>A reference to this object.</returns>
		BaseType& operator = ( const BaseType& other );

		bool Virtual( ) const;
		BaseType& Virtual( bool flag );

		/// <summary>
		/// Writes this object to the given stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <IsConstMember value="true"/>
		void write( std::wostream& ) const;

		UserDefinedType& Type( ) const;
	};

	class MemberType;
	class Typedefinition;

	typedef TypedCollection< std::pair< copying_pointer< UserDefinedType > , AccessRestrictions > > NestedTypeCollection;
	typedef TypedCollection< copying_pointer< Constructor > > ConstructorCollection;
	typedef TypedCollection< copying_pointer< Type > > TypeCollection;
	typedef TypedCollection< BaseType > BaseTypeCollection;
	typedef TypedCollection< std::pair< copying_pointer< Typedefinition > , AccessRestrictions > > MemberTypedefinitionCollection;
	typedef TypedCollection< std::pair< copying_pointer< Enumeration > , AccessRestrictions > > MemberEnumerationCollection;
	typedef TypedCollection< std::pair< copying_pointer< Union > , AccessRestrictions > > MemberUnionCollection;

	/// <summary>
	/// Represents a user-defined type in code-graph.
	/// </summary>
	class UserDefinedType : public Type , public nestabletype
	{
		std::auto_ptr< BaseTypeCollection > bases;
		std::auto_ptr< TypeCollection > specialized_parameters;
		std::auto_ptr< MemberTypedefinitionCollection > typedefinitions;
		std::auto_ptr< NestedTypeCollection > memberuserdefinedtypes;
		std::auto_ptr< MemberFunctionCollection > memberfunctions;
		std::auto_ptr< MemberEnumerationCollection > memberenumerations;
		std::auto_ptr< MemberUnionCollection > memberunions;
		std::auto_ptr< MemberVariableCollection > membervariables;
		std::auto_ptr< MemberOperatorCollection > memberoperators;
		std::auto_ptr< ConstructorCollection > constructors;
		Destructor* destructor;

		std::auto_ptr< TemplateParameterCollection > template_parameters;
		mutable std::vector< const CodeObject* > render_backlog;

		bool is_sealed , is_inline;
		nestabletype* enclosingtype;
		mutable int nestinglevel;
	public:
		/// <summary>
		/// Represents either a class or structure in code graph.
		/// </summary>
		enum UserType
		{
			/// <summary>
			/// Specified to generate a class in code.
			/// </summary>
			Class , 
			/// <summary>
			/// Specified to generate a struct in code.
			/// </summary>
			Struct
		};

		/// <summary>
		/// Creates a new user-defined type given its name and type.
		/// </summary>
		/// <parameter IsConst="true" name="name">Name of this user-defined type.</parameter>
		/// <parameter name="usertype">Either a class or Struct.</parameter>
		/// <returns>A proxy object that acts like UserDefinedType.</returns>
		static copying_pointer< UserDefinedType > Instantiate( const std::wstring& name , UserType usertype )
		{
			return copying_pointer< UserDefinedType >( new UserDefinedType( name , usertype ) );
		}

		UserDefinedType& Assign( const nestabletype& object );

		/// <summary>
		/// Assigns another object to this UserDefinedType.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		UserDefinedType& Assign( const CodeObject& object );
		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed UserDefinedType.</returns>
		UserDefinedType& Duplicate( ) const;

		/// <summary>
		/// Returns a collection of base classes for this type.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A collection containing all base classes for this type.</returns>
		BaseTypeCollection& BaseTypes( ) const;

		/// <summary>
		/// Gets a value which tells if this user defined type is a template class.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>True if this user defined type is a template class.</returns>
		bool Template( ) const;

		/// <summary>
		/// Gets a value which tells if this user defined type is a complete or partial template specialization.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>True if this user defined type is a complete or partial template specialization.</returns>
		bool IsSpecialized( ) const;

		bool Inline( ) const;
		UserDefinedType& Inline( bool flag );

		/// <summary>
		/// Gets a value which tells whether this user defined type is a class or structure.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A value from the enumeration UserType.</returns>
		UserType Usertype( ) const;

		/// <summary>
		/// Gets all the template parameters for this user defined type.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A collection containing all template parameters for this user defined type.</returns>
		TemplateParameterCollection& TemplateParameters( ) const;

		/// <summary>
		/// Get all nested typedefinitions for this user defined type.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A collection containing all nested typedefinitions for this user defined type.</returns>
		MemberTypedefinitionCollection& Typedefinitions( ) const;

		/// <summary>
		/// Get all nested types for this user defined type.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A collection containing all nested types for this user defined type.</returns>
		NestedTypeCollection& MemberTypes( ) const;

		/// <summary>
		/// Get all member functions in this user defined type.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A collection containing all member functions in this user defined type.</returns>
		MemberFunctionCollection& MemberFunctions( ) const;

		/// <summary>
		/// Get all nested enumerations for this user defined type.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A collection containing all enumerations types for this user defined type.</returns>
		MemberEnumerationCollection& MemberEnumerations( ) const;

		/// <summary>
		/// Get all nested unions for this user defined type.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A collection containing all nested unions for this user defined type.</returns>
		MemberUnionCollection& MemberUnions( ) const;

		/// <summary>
		/// Get all member variables in this user defined type.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A collection containing all member variables in this user defined type.</returns>
		MemberVariableCollection& MemberVariables( ) const;

		/// <summary>
		/// Get all member overloaded operators in this user defined type.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A collection containing all member overloaded operators in this user defined type.</returns>
		MemberOperatorCollection& MemberOperators( ) const;

		/// <summary>
		/// Get all constructors in this user defined type.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A collection containing all constructors in this user defined type.</returns>
		ConstructorCollection& Constructors( ) const;

		/// <summary>
		/// Gets a value which tells if this user defined type is an abstract type.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>True if this user defined type is an abstract type.</returns>
		bool IsAbstract( ) const;
		/// <summary>
		/// Sets a value to tell if this user defined type is an abstract type.
		/// </summary>
		/// <parameter name="flag">True to make this type an abstract type.</parameter>
		/// <returns>A reference to this object so that other operations can be cascaded.</returns>
		UserDefinedType& Abstract( bool flag );

		/// <summary>
		/// Gets a value which tells if this user defined type is a sealed type.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>True if this user defined type is a sealed type.</returns>
		bool IsSealed( ) const;
		/// <summary>
		/// Sets a value to tell if this user defined type is a sealed type.
		/// </summary>
		/// <parameter name="flag">True to make this type a sealed type.</parameter>
		/// <returns>A reference to this object so that other operations can be cascaded.</returns>
		UserDefinedType& Sealed( bool flag );
		/// <summary>
		/// Gets a reference to destructor for this user defined type, if any.
		/// </summary>	
		/// <returns>A pointer to the destructor for this user defined type.</returns>
		Destructor* Destructor( ) const;

		UserDefinedType& Destructor( copying_pointer< class Destructor >& );
		
		/// <summary>
		/// Gets all the types used for complete or partial specialization of this user defined type.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A collection containing all types used for complete or partial specialization of this user defined type.</returns>
		TypeCollection& SpecializedParameters( ) const;

		nestabletype* EnclosingType( ) const;
		UserDefinedType& EnclosingType( nestabletype* );

		/// <summary>
		/// Gets a string containing the declarator specifier corresponding to this user defined type.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A string containing the declarator specifier corresponding to this user defined type.</returns>
		std::wstring instantiate( ) const;
		/// <summary>
		/// Gets a string containing the declarator specifier corresponding to this user defined type with the given template arguments.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <exception ref="std::invalid_argument">Thrown if number of strings in arguments does not equal number of template arguments.</exception>
		/// <returns>A string containing the declarator specifier corresponding to this user defined type with the given template arguments.</returns>
		std::wstring instantiate( const StringCollection& ) const;

		bool hasbacklog( ) const;

		void writebacklog( std::wostream& os , unsigned long tabs ) const;
		void writebacklog( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;
	protected:
		/// <summary>
		/// Constructs a new user-defined type given its name and type.
		/// </summary>
		/// <parameter IsConst="true" name="name">Name of this user-defined type.</parameter>
		/// <parameter name="usertype">Either a class or Struct.</parameter>
		UserDefinedType( const std::wstring& name , UserType usertype );
		/// <summary>
		/// Constructs a new UserDefinedType object from a given one.
		/// </summary>
		/// <parameter name="other">Other UserDefinedType object to use as source.</parameter>
		UserDefinedType( const UserDefinedType& other );
		/// <summary>
		/// Assigns a given UserDefinedType object to this one. This function is more efficient as there is no need for dynamic_cast.
		/// </summary>
		/// <parameter name="other">Other UserDefinedType object that will be assigned.</parameter>
		/// <parameter>This parameter is not used.</parameter>
		void Assign( const UserDefinedType& other , bool );
		/// <summary>
		/// Writes the template parameters for this user defined type.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation.</parameter>
		/// <IsConstMember value="true"/>
		void writetemplateattributesrecursive( std::wostream& os ) const;
		/// <summary>
		/// Writes out the elaborated name of this user defined type to the given stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <IsConstMember value="true"/>
		void writeelaboratednamerecursive( std::wostream& ) const;

		/// <summary>
		/// Writes out the definition of this user defined type's members to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation</parameter>
		/// <IsConstMember value="true"/>
		void writemembers( std::wostream& os , unsigned long tabs ) const;
		void writemembers( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;
		/// <summary>
		/// Writes out the definition of this user defined type to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation</parameter>
		/// <IsConstMember value="true"/>
		void writebody( std::wostream& os , unsigned long tabs ) const;
		void writebody( std::wostream& declos , std::wostream& defos , unsigned long decltabs, unsigned long deftabs ) const;
		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
		/// <summary>
		/// Writes out this object split across the two streams.
		/// </summary>
		/// <parameter name="declas">Declaration output stream.</parameter>
		/// <parameter name="defos">Definition output stream.</parameter>
		/// <parameter name="tabs">Indentation for this UserDefinedType.</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;
		/// <summary>
		/// Writes out the declaration for this user defined type to the given stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation</parameter>
		/// <IsConstMember value="true"/>
		virtual void declaration( std::wostream& os , unsigned long tabs ) const;

		void writeforwarddeclarations( std::wostream& os , unsigned long tabs ) const;
		void writespecializedparameters( std::wostream& os ) const;
		void writebasetypes( std::wostream& os ) const;
	private:
		UserType usertype;
	};

	class Function;
	class FunctionPtr : public Type , public Callable
	{
		const Function* function;
	public:
		static copying_pointer< FunctionPtr > Instantiate( const std::wstring& name , const Function& function )
		{
			return copying_pointer< FunctionPtr >( new FunctionPtr( name , function ) );
		}

		/// <summary>
		/// Assigns another object to this FunctionPtr.
		/// </summary>
		/// <parameter IsConst="true" name="object">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		FunctionPtr& Assign( const CodeObject& object );
		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed FunctionPtr.</returns>
		FunctionPtr& Duplicate( ) const;

		/// <IsConstMember value="true"/>
		const class Function& Function( ) const;
	protected:
		FunctionPtr( const std::wstring& , const class Function& );

		void Assign( const FunctionPtr& , bool );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this FunctionPtr</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;

		/// <summary>
		/// Writes out this object split across the two streams.
		/// </summary>
		/// <parameter name="declas">Declaration output stream.</parameter>
		/// <parameter name="defos">Definition output stream.</parameter>
		/// <parameter name="tabs">Indentation for this FunctionPtr.</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;
	};

	class MemberFunction;
	
	/// <summary>
	/// Represents a pointer to member function.
	/// </summary>
	class MemFun : public FunctionPtr
	{
		const UserDefinedType* containingtype;
	public:
		static copying_pointer< MemFun > Instantiate( const std::wstring& name , const MemberFunction& function )
		{
			return copying_pointer< MemFun >( new MemFun( name , function ) );
		}

		static copying_pointer< MemFun > Instantiate( const std::wstring& name , const UserDefinedType& containingtype , const MemberFunction& function )
		{
			return copying_pointer< MemFun >( new MemFun( name , containingtype , function ) );
		}

		/// <summary>
		/// Assigns another object to this MemFun.
		/// </summary>
		/// <parameter IsConst="true" name="object">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		MemFun& Assign( const CodeObject& object );
		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed MemFun.</returns>
		MemFun& Duplicate( ) const;

		/// <IsConstMember value="true"/>
		const UserDefinedType& ContainingType( ) const;
	protected:
		MemFun( const std::wstring& , const MemberFunction& );
		MemFun( const std::wstring& , const UserDefinedType& , const MemberFunction& );

		void Assign( const MemFun& , bool );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this MemFun</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;

		/// <summary>
		/// Writes out this object split across the two streams.
		/// </summary>
		/// <parameter name="declas">Declaration output stream.</parameter>
		/// <parameter name="defos">Definition output stream.</parameter>
		/// <parameter name="tabs">Indentation for this MemFun.</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;
	};

	/// <summary>
	/// Represents a pointer-to-member in code-graph.
	/// </summary>
	class PointerToMember : public Type
	{
		const Type* type;
		const UserDefinedType* containingtype;
		bool initialized;
	public:
		static copying_pointer< PointerToMember > Instantiate( const std::wstring& name , const UserDefinedType& containingtype , const Type& type )
		{
			return copying_pointer< PointerToMember >( new PointerToMember( name , containingtype , type ) );
		}

		/// <summary>
		/// Assigns another object to this PointerToMember.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		PointerToMember& Assign( const CodeObject& object );
		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed PointerToMember.</returns>
		PointerToMember& Duplicate( ) const;
	protected:
		PointerToMember( const std::wstring& , const UserDefinedType& , const Type& );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this PointerToMember</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;

		/// <summary>
		/// Writes out this object split across the two streams.
		/// </summary>
		/// <parameter name="declas">Declaration output stream.</parameter>
		/// <parameter name="defos">Definition output stream.</parameter>
		/// <parameter name="tabs">Indentation for this PointerToMember.</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;
	};
}

#endif