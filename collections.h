#ifndef COLLECTIONS_HEADER
#define COLLECTIONS_HEADER

#include <list>
#include <string>
#include "utility.h"

namespace CPlusPlusCodeProvider
{
	/// <summary>
	/// Provides a modified interface to std::list&lt; T &gt;.
	/// </summary>
	template< typename T > class TypedCollection
	{
		std::list< T > data;
	public:
		typedef typename std::list< T >::iterator iterator;
		typedef typename std::list< T >::const_iterator const_iterator;
		typedef typename std::list< T >::size_type size_type;

		/// <summary>
		/// Appends a sequence to this one.
		/// </summary>
		/// <parameter name="start">First item to append.</parameter>
		/// <parameter name="finish">Element past the last item.</parameter>
		template< typename iter > void append( iter start , iter finish )
		{
			data.insert( data.end( ) , start , finish );
		}

		/// <summary>
		/// Allows you to iterate the sequence.
		/// </summary>
		/// <returns>An iterator that points to the beginning of the sequence.</returns>
		iterator begin( )
		{
			return data.begin( );
		}

		/// <summary>
		/// Allows you to iterate the sequence.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>An iterator that points to the beginning of the sequence.</returns>
		const_iterator begin( ) const
		{
			return data.begin( );
		}

		/// <summary>
		/// Allows you to iterator the sequence.
		/// </summary>
		/// <returns>An iterator that points to the element past the end of sequence.</returns>
		iterator end( )
		{
			return data.end( );
		}

		/// <summary>
		/// Allows you to iterator the sequence.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>An iterator that points to the element past the end of sequence.</returns>
		const_iterator end( ) const
		{
			return data.end( );
		}

		/// <summary>
		/// Returns you the size of sequence.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A number equal to the elements in the sequence.</returns>
		size_type size( ) const
		{
			return data.size( );
		}

		/// <summary>
		/// Purges all the elements contained in the sequenece.
		/// </summary>
		void clear( )
		{
			data.clear( );
		}

		/// <summary>
		/// Adds an element to the end of sequenece.
		/// </summary>
		/// <parameter name="value" IsConst="true">Element to be added.</parameter>
		/// <returns>A reference to this object.</returns>
		TypedCollection& push_back( const T& value )
		{
			data.push_back( value );
			return *this;
		}

		/// <summary>
		/// Provides a single empty TypedCollection in order to save space, both at compile time and at runtime.
		/// </summary>
		static const TypedCollection< T > empty;
	};

	/// <summary>
	/// A partial specialization of TypedCollection for pointers, this class owns the pointers put into it and deletes them when purge is called.
	/// </summary>
	template< typename T > class TypedCollection< T* >
	{
		std::list< T* > data;
	public:
		typedef typename std::list< T* >::const_iterator const_iterator;
		typedef typename std::list< T* >::size_type size_type;

		/// <summary>
		/// Deletes all the contained pointer.
		/// </summary>
		~TypedCollection( )
		{
			purge( );
		}

		/// <summary>
		/// Appends a sequence to this one.
		/// </summary>
		/// <parameter name="start">First item to append.</parameter>
		/// <parameter name="finish">Element past the last item.</parameter>
		template< typename iter > void append( iter start , iter finish )
		{
			data.insert( data.end( ) , start , finish );
		}

		/// <summary>
		/// Allows you to iterate the sequence.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>An iterator that points to the beginning of the sequence.</returns>
		const_iterator begin( ) const
		{
			return data.begin( );
		}

		/// <summary>
		/// Allows you to iterator the sequence.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>An iterator that points to the element past the end of sequence.</returns>
		const_iterator end( ) const
		{
			return data.end( );
		}

		/// <summary>
		/// Returns you the size of sequence.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A number equal to the elements in the sequence.</returns>
		size_type size( ) const
		{
			return data.size( );
		}

		/// <summary>
		/// Purges all the elements contained in the sequenece.
		/// </summary>
		void clear( )
		{
			data.clear( );
		}

		/// <summary>
		/// Adds an element to the end of sequenece.
		/// </summary>
		/// <parameter name="value">Element to be added.</parameter>
		/// <returns>A reference to this object.</returns>
		TypedCollection& push_back( T& value )
		{
			data.push_back( &value );
			return *this;
		}

		/// <summary>
		/// Performs delete on all contained pointers and after that empties the collection.
		/// </summary>
		void purge( )
		{
			for( const_iterator iter = begin( ) ; iter != end( ) ; ++iter )
				delete *iter;
			clear( );
		}

		/// <summary>
		/// Provides a single empty TypedCollection in order to save space, both at compile time and at runtime.
		/// </summary>
		static const TypedCollection< T* > empty;
	};

	/// <summary>
	/// Specialization for std::wstring.
	/// </summary>
	template< > class TypedCollection< std::wstring >
	{
		std::list< std::wstring > data;
	public:
		typedef std::list< std::wstring >::const_iterator const_iterator;
		typedef std::list< std::wstring >::size_type size_type;

		/// <summary>
		/// Allows you to iterate the sequence.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>An iterator that points to the beginning of the sequence.</returns>
		const_iterator begin( ) const
		{
			return data.begin( );
		}

		/// <summary>
		/// Allows you to iterator the sequence.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>An iterator that points to the element past the end of sequence.</returns>
		const_iterator end( ) const
		{
			return data.end( );
		}

		/// <summary>
		/// Returns you the size of sequence.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A number equal to the elements in the sequence.</returns>
		size_type size( ) const
		{
			return data.size( );
		}

		/// <summary>
		/// Purges all the elements contained in the sequenece.
		/// </summary>
		void clear( )
		{
			data.clear( );
		}

		/// <summary>
		/// Adds an element to the end of sequenece.
		/// </summary>
		/// <parameter name="value" IsConst="true">Element to be added.</parameter>
		/// <returns>A reference to this object.</returns>
		TypedCollection& push_back( const std::wstring& value )
		{
			data.push_back( value );
			return *this;
		}

		/// <summary>
		/// Provides a single empty TypedCollection&lt; std::wstring &gt; in order to save space, both at compile time and at runtime.
		/// </summary>
		static const TypedCollection< std::wstring > empty;
	};

	/// <summary>
	/// Provides a strongly typed collection of strings.
	/// </summary>
	typedef TypedCollection< std::wstring > StringCollection;

	class Typedefinition;
	/// <summary>
	/// Provides a strongly typed collection of typedefinitions.
	/// </summary>
	typedef TypedCollection< copying_pointer< Typedefinition > > TypedefinitionCollection;

	class CodeObject;
	/// <summary>
	/// Provides a strongly typed collection of all statements.
	/// </summary>
	typedef TypedCollection< copying_pointer< CodeObject > > CodeCollection;

	class Constructor;
	/// <summary>
	/// Provides a strongly typed collection of constructors.
	/// </summary>
	typedef TypedCollection< copying_pointer< Constructor > > ConstructorCollection;

	class Comment;
	/// <summary>
	/// Provides a strongly typed collection of comments.
	/// </summary>
	typedef TypedCollection< copying_pointer< Comment > > CommentCollection;

	class Namespace;
	/// <summary>
	/// Provides a strongly typed collection of namespaces.
	/// </summary>
	typedef TypedCollection< copying_pointer< Namespace > > NamespaceCollection;

	class UserDefinedType;
	/// <summary>
	/// Provides a strongly typed collection of user defined types.
	/// </summary>
	typedef TypedCollection< copying_pointer< UserDefinedType > > UserDefinedTypeCollection;

	class Function;
	/// <summary>
	/// Provides a strongly typed collection of functions.
	/// </summary>
	typedef TypedCollection< copying_pointer< Function > > FunctionCollection;

	class Enumeration;
	/// <summary>
	/// Provides a strongly typed collection of enumerations.
	/// </summary>
	typedef TypedCollection< copying_pointer< Enumeration > > EnumerationCollection;

	class Union;
	/// <summary>
	/// Provides a strongly typed collection of unions.
	/// </summary>
	typedef TypedCollection< copying_pointer< Union > > UnionCollection;

	class Operator;
	/// <summary>
	/// Provides a strongly typed collection of user defined operators.
	/// </summary>
	typedef TypedCollection< copying_pointer< Operator > > OperatorCollection;

	class MemberFunction;
	/// <summary>
	/// Provides a strongly typed collection of member functions.
	/// </summary>
	typedef TypedCollection< copying_pointer< MemberFunction > > MemberFunctionCollection;

	class MemberOperator;
	/// <summary>
	/// Provides a strongly typed collection of member user defined operators.
	/// </summary>
	typedef TypedCollection< copying_pointer< MemberOperator > > MemberOperationCollection;

	class PreprocessorDirective;
	/// <summary>
	/// Provides a strongly typed collection of preprocessor directives.
	/// </summary>
	typedef TypedCollection< copying_pointer< PreprocessorDirective > > PreprocessorDirectiveCollection;

	class MemberType;
	/// <summary>
	/// Provides a strongly typed collection of member user defined types.
	/// </summary>
	typedef TypedCollection< copying_pointer< MemberType > > MemberTypeCollection;

	class Expression;
	/// <summary>
	/// Provides a strongly typed collection of expressions.
	/// </summary>
	typedef TypedCollection< copying_pointer< Expression > > ExpressionCollection;

	class CatchClause;
	/// <summary>
	/// Provides a strongly typed collection of catch blocks.
	/// </summary>
	typedef TypedCollection< copying_pointer< CatchClause > > CatchClauseCollection;

	class Type;
	/// <summary>
	/// Provides a strongly typed collection of any type.
	/// </summary>
	typedef TypedCollection< copying_pointer< Type > > TypeCollection;

	class TemplateParameter;
	/// <summary>
	/// Provides a strongly typed collection of template parameters.
	/// </summary>
	typedef TypedCollection< copying_pointer< TemplateParameter > > TemplateParameterCollection;

	class VariableDeclaration;
	/// <summary>
	/// Provides a strongly typed collection of variables.
	/// </summary>
	typedef TypedCollection< copying_pointer< VariableDeclaration > > VariableCollection , ParameterCollection;

	class ScopeResolutionExpression;
	/// <summary>
	/// Provides a strongly typed collection of scope-resolutions expression.
	/// </summary>
	typedef TypedCollection< copying_pointer< ScopeResolutionExpression > > ScopeResolutionExpressionCollection;
}

#endif