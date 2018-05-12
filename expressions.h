#ifndef EXPRESSIONS_HEADER
#define EXPRESSIONS_HEADER

#include "cppcodeprovider.h"
#include <memory>

namespace CPlusPlusCodeProvider
{
	/// <summary>
	/// Base class for all unary operators.
	/// </summary>
	class UnaryExpression : public Expression
	{
		copying_pointer< Expression > source;
	public:
		/// <summary>
		/// Expression to which to apply the operator.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>Expression on which unary operator will be applied.</returns>
		const Expression& Source( ) const;
	protected:
		/// <summary>
		/// Constructs a new UnaryExpression expression.
		/// </summary>
		/// <parameter name="expression">Expression on which unary operator will be applied.</parameter>
		UnaryExpression( const copying_pointer< Expression >& expression );

		/// <summary>
		/// Assigns given UnaryExpression to this unary expression.
		/// </summary>
		/// <parameter IsConst="true" name="other">Another unary expression whose contents are copied to this one.</parameter>
		void Assign( const UnaryExpression& other );
		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os ) const;
	};

	/// <summary>
	/// Allows you to use scope-resolution operator.
	/// </summary>
	class ScopeResolutionExpression : public Expression
	{
		std::wstring left;
		copying_pointer< ScopeResolutionExpression > right;
	public:
		/// <summary>
		/// Returns a newly constructed ScopeResoutionExpression.
		/// </summary>
		/// <parameter IsConst="true" name="first">Name that appears on the right of scope-resolution operator.</parameter>
		/// <returns>A proxy object that acts like a ScopeResolutionExpression.</returns>
		static copying_pointer< ScopeResolutionExpression > Instantiate( const std::wstring& first )
		{
			return copying_pointer< ScopeResolutionExpression >( new ScopeResolutionExpression( first ) );
		}

		/// <summary>
		/// Returns a newly constructed ScopeResoutionExpression.
		/// </summary>
		/// <parameter IsConst="true" name="first">Name that appears on the right of scope-resolution operator.</parameter>
		/// <parameter IsConst="true" name="second">Expression that appears on the left of scope-resolution operator.</parameter>
		/// <returns>A proxy object that acts like a ScopeResolutionExpression.</returns>
		static copying_pointer< ScopeResolutionExpression > Instantiate( const std::wstring& first , const copying_pointer< ScopeResolutionExpression >& second )
		{
			return copying_pointer< ScopeResolutionExpression >( new ScopeResolutionExpression( first , second ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed ScopeResolutionExpression.</returns>
		ScopeResolutionExpression& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this ScopeResolutionExpression.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		ScopeResolutionExpression& Assign( const CodeObject& object );
	protected:
		/// <summary>
		/// Constructs a new ScopeResoutionExpression object.
		/// </summary>
		/// <parameter name="first" IsConst="true">Name that appears on the right of scope-resolution operator.</parameter>
		ScopeResolutionExpression( const std::wstring& first );
		/// <summary>
		/// Constructs a new ScopeResoutionExpression object.
		/// </summary>
		/// <parameter name="first" IsConst="true">Name that appears on the right of scope-resolution operator.</parameter>
		/// <parameter name="second" IsConst="true">Expression that appears on the left of scope-resolution operator.</parameter>
		ScopeResolutionExpression( const std::wstring& first , const copying_pointer< ScopeResolutionExpression >& );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this ScopeResolutionExpression</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	/// <summary>
	/// Represents a binary expression in code-graph.
	/// </summary>
	class BinaryExpression : public Expression
	{
		copying_pointer< Expression > left , right;
		std::wstring name;
		std::wstring validate( const std::wstring& , CompilerCapabilities::Capability capability );
		int t;
	public:
		/// <summary>
		/// Represents the type of binary operator.
		/// </summary>
		struct Type
		{
			static const int
									/// <summary>
									/// Represents '+' operator in code.
									/// </summary>
									Addition = 1 , 
									/// <summary>
									/// Represents '-' operator in code.
									/// </summary>
									Subtraction = 2 , 
									/// <summary>
									/// Represents '*' operator in code.
									/// </summary>
									Multiplication = 3 , 
									/// <summary>
									/// Represents '/' operator in code.
									/// </summary>
									Division = 4 , 
									/// <summary>
									/// Represents '%' operator in code.
									/// </summary>
									Modulus = 5 , 
									/// <summary>
									/// Represents '=' operator in code.
									/// </summary>
									Assignment = 6 , 
									/// <summary>
									/// Represents '[ ]' operator in code.
									/// </summary>
									Subscript = 7 , 
									/// <summary>
									/// Represents '&amp;&amp;' operator in code.
									/// </summary>
									And = 8 , 
									/// <summary>
									/// Represents '||' operator in code.
									/// </summary>
									Or = 9 , 
									/// <summary>
									/// Represents '.' operator in code.
									/// </summary>
									MemberAccess = 10 , 
									/// <summary>
									/// Represents ',' operator in code.
									/// </summary>
									Comma = 11 , 
									/// <summary>
									/// Represents '&lt;&lt;' operator in code.
									/// </summary>
									LeftShift = 12 , 
									/// <summary>
									/// Represents '&gt;&gt;' operator in code.
									/// </summary>
									RightShift = 13 , 
									/// <summary>
									/// Represents '&lt;' operator in code.
									/// </summary>
									Less = 14 , 
									/// <summary>
									/// Represents '&gt;' operator in code.
									/// </summary>
									Greater = 15 , 
									/// <summary>
									/// Represents '&lt;=' operator in code.
									/// </summary>
									LessEqual = 16 , 
									/// <summary>
									/// Represents '&gt;=' operator in code.
									/// </summary>
									GreaterEqual = 17 , 
									/// <summary>
									/// Represents '==' operator in code.
									/// </summary>
									Equal = 18 , 
									/// <summary>
									/// Represents '!=' operator in code.
									/// </summary>
									NotEqual = 19 , 
									/// <summary>
									/// Represents '&amp;' operator in code.
									/// </summary>
									BitwiseAnd = 20 , 
									/// <summary>
									/// Represents '|' operator in code.
									/// </summary>
									BitwiseOr = 21 , 
									/// <summary>
									/// Represents '^' operator in code.
									/// </summary>
									Xor = 22 , 
									/// <summary>
									/// Represents '+=' operator in code.
									/// </summary>
									Addassign = 23 , 
									/// <summary>
									/// Represents '-=' operator in code.
									/// </summary>
									Subtractassign = 24 , 
									/// <summary>
									/// Represents '*=' operator in code.
									/// </summary>
									Multiplyassign = 25 , 
									/// <summary>
									/// Represents '/=' operator in code.
									/// </summary>
									Divideassign = 26 , 
									/// <summary>
									/// Represents '%=' operator in code.
									/// </summary>
									Modulusassign = 27 , 
									/// <summary>
									/// Represents '&lt;&lt;=' operator in code.
									/// </summary>
									LeftShiftassign = 28 , 
									/// <summary>
									/// Represents '&gt;&gt;=' operator in code.
									/// </summary>
									RightShiftassign = 29 , 
									/// <summary>
									/// Represents '&amp;=' operator in code.
									/// </summary>
									BitwiseAndassign = 30 , 
									/// <summary>
									/// Represents '|=' operator in code.
									/// </summary>
									BitwiseOrassign = 31 , 
									/// <summary>
									/// Represents '^=' operator in code.
									/// </summary>
									Xorassign = 32;
		};

		/// <summary>
		/// Returns a newly constructed BinaryExpression given the two expressions and a binary operator.
		/// </summary>
		/// <parameter name="a">Expression on the left side of binary operator.</parameter>
		/// <parameter name="b">Expression on the right side of the binary operator.</parameter>
		/// <parameter name="type">Binary operator that joins the two expression.</parameter>
		/// <returns>A proxy object that acts like a BinaryExpression.</returns>
		static copying_pointer< BinaryExpression > Instantiate( const copying_pointer< Expression >& a , const copying_pointer< Expression >& b , int type )
		{
			return copying_pointer< BinaryExpression >( new BinaryExpression( a , b , type ) );
		}

		/// <summary>
		/// Returns a newly constructed BinaryExpression given the two expressions and the binary operator in string form.
		/// </summary>
		/// <parameter IsConst="true" name="a">Expression on the left side of binary operator.</parameter>
		/// <parameter name="b" IsConst="true">Expression on the right side of the binary operator.</parameter>
		/// <parameter name="op" IsConst="true">Binary operator's name that joins the two expression.</parameter>
		/// <parameter name="capability">A CompilerCapabilities object with AlternateKeywords bit set if the compiler supports alternate keywords, otherwise, op is converted to its equivalent enumerated representation.</parameter>
		/// <exception ref="std::invalid_argument">Thrown if the op does not match any of the predefined operators.</exception>
		/// <returns>A proxy object that acts like a BinaryExpression.</returns>
		static copying_pointer< BinaryExpression > Instantiate( const copying_pointer< Expression >& a , const copying_pointer< Expression >& b , const std::wstring& op , CompilerCapabilities::Capability capability = CompilerCapabilities::AlternateKeywords )
		{
			return copying_pointer< BinaryExpression >( new BinaryExpression( a , b , op , capability ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed BinaryExpression.</returns>
		BinaryExpression& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this BinaryExpression.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		BinaryExpression& Assign( const CodeObject& object );
	protected:
		/// <summary>
		/// Constructs a new BinaryExpression given the two expressions and a binary operator.
		/// </summary>
		/// <parameter name="a">Expression on the left side of binary operator.</parameter>
		/// <parameter name="b">Expression on the right side of the binary operator.</parameter>
		/// <parameter name="type">Binary operator that joins the two expression.</parameter>
		BinaryExpression( const copying_pointer< Expression >& a , const copying_pointer< Expression >& b , int type );
		/// <summary>
		/// Constructs a new BinaryExpression given the two expressions and the binary operator in string form.
		/// </summary>
		/// <parameter IsConst="true" name="a">Expression on the left side of binary operator.</parameter>
		/// <parameter name="b" IsConst="true">Expression on the right side of the binary operator.</parameter>
		/// <parameter name="op" IsConst="true">Binary operator's name that joins the two expression.</parameter>
		/// <parameter name="capability">A CompilerCapabilities object with AlternateKeywords bit set if the compiler supports alternate keywords, otherwise, op is converted to its equivalent enumerated representation.</parameter>
		BinaryExpression( const copying_pointer< Expression >& a , const copying_pointer< Expression >& b , const std::wstring& op , CompilerCapabilities::Capability capability = CompilerCapabilities::AlternateKeywords );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this BinaryExpression</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	/// <summary>
	/// Represents a prefix expression in code-graph.
	/// </summary>
	class PrefixExpression : public UnaryExpression
	{
		std::wstring name;
		std::wstring validate( const std::wstring& , CompilerCapabilities::Capability capability );
		int t;
	public:
		/// <summary>
		/// Represents the type of prefix operator.
		/// </summary>
		struct Type
		{
			static const int 
									/// <summary>
									/// Represents '++' operator in code.
									/// </summary>
									Increment = 1 , 
									/// <summary>
									/// Represents '--' operator in code.
									/// </summary>
									Decrement = 2 , 
									/// <summary>
									/// Represents '+' operator in code.
									/// </summary>
									Plus = 3 , 
									/// <summary>
									/// Represents '-' operator in code.
									/// </summary>
									Negate = 4 , 
									/// <summary>
									/// Represents '!' operator in code.
									/// </summary>
									Not = 5 , 
									/// <summary>
									/// Represents '~' operator in code.
									/// </summary>
									Complement = 6 , 
									/// <summary>
									/// Represents '*' operator in code.
									/// </summary>
									Dereference = 7 , 
									/// <summary>
									/// Represents '&amp;' operator in code.
									/// </summary>
									Addressof = 8 , 
									/// <summary>
									/// Represents 'sizeof( )' operator in code.
									/// </summary>
									Sizeof = 9;
		};

		/// <summary>
		/// Returns a newly constructed PrefixExpression from an expression and a unary operator.
		/// </summary>
		/// <parameter name="expression">Expression on which the prefix operator is applied.</parameter>
		/// <parameter name="type">Prefix operator to apply on the expression.</parameter>
		/// <returns>A proxy object that acts like a PrefixExpression.</returns>
		static copying_pointer< PrefixExpression > Instantiate( const copying_pointer< Expression >& expression , int type )
		{
			return copying_pointer< PrefixExpression >( new PrefixExpression( expression , type ) );
		}

		/// <summary>
		/// Returns a newly constructed PrefixExpression from an expression and a unary operator in string form.
		/// </summary>
		/// <parameter IsConst="true" name="expression">Expression on which the prefix operator is applied.</parameter>
		/// <parameter name="name" IsConst="true">Prefix operator's name to apply on the expression.</parameter>
		/// <parameter name="capability">A CompilerCapabilities object with AlternateKeywords bit set if the compiler supports alternate keywords, otherwise, name is converted to its equivalent enumerated representation.</parameter>
		/// <exception ref="std::invalid_argument">Thrown if the op does not match any of the predefined operators.</exception>
		/// <returns>A proxy object that acts like a PrefixExpression.</returns>
		static copying_pointer< PrefixExpression > Instantiate( const copying_pointer< Expression >& expression , const std::wstring& name , CompilerCapabilities::Capability capability = CompilerCapabilities::AlternateKeywords )
		{
			return copying_pointer< PrefixExpression >( new PrefixExpression( expression , name , capability ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed PrefixExpression.</returns>
		PrefixExpression& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this PrefixExpression.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		PrefixExpression& Assign( const CodeObject& object );
	protected:
		/// <summary>
		/// Constructs a new PrefixExpression from an expression and a unary operator.
		/// </summary>
		/// <parameter name="expression">Expression on which the prefix operator is applied.</parameter>
		/// <parameter name="type">Prefix operator to apply on the expression.</parameter>
		PrefixExpression( const copying_pointer< Expression >& expression , int type );
		/// <summary>
		/// Constructs a new PrefixExpression from an expression and a unary operator in string form.
		/// </summary>
		/// <parameter IsConst="true" name="expression">Expression on which the prefix operator is applied.</parameter>
		/// <parameter name="name" IsConst="true">Prefix operator's name to apply on the expression.</parameter>
		/// <parameter name="capability">A CompilerCapabilities object with AlternateKeywords bit set if the compiler supports alternate keywords, otherwise, name is converted to its equivalent enumerated representation.</parameter>
		PrefixExpression( const copying_pointer< Expression >& expression , const std::wstring& name , CompilerCapabilities::Capability capability = CompilerCapabilities::AlternateKeywords );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this PrefixExpression</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	/// <summary>
	/// Represents a postfix expression in code-graph.
	/// </summary>
	class PostfixExpression : public UnaryExpression
	{
		int t;
	public:
		/// <summary>
		/// Represents the postfix operators.
		/// </summary>
		struct Type
		{
			static int const 
									/// <summary>
									/// Represents '++' operator in code.
									/// </summary>
									Increment = 1 , 
									/// <summary>
									/// Represents '*' operator in code.
									/// </summary>
									Decrement = 2 , 
									/// <summary>
									/// Represents '-&gt;' operator in code.
									/// </summary>
									Dereference = 3;
		};

		/// <summary>
		/// Returns a newly constructed PostfixExpression from an expression and a unary operator.
		/// </summary>
		/// <parameter name="expression">Expression on which the postfix operator is applied.</parameter>
		/// <parameter name="type">Postfix operator to apply on the expression.</parameter>
		/// <returns>A proxy object that acts like a PostfixExpression.</returns>
		static copying_pointer< PostfixExpression > Instantiate( const copying_pointer< Expression >& expression , int type )
		{
			return copying_pointer< PostfixExpression >( new PostfixExpression( expression , type ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed PostfixExpression.</returns>
		PostfixExpression& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this PostfixExpression.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		PostfixExpression& Assign( const CodeObject& object );
	protected:
		/// <summary>
		/// Constructs a new PostfixExpression from an expression and a unary operator.
		/// </summary>
		/// <parameter name="expression">Expression on which the postfix operator is applied.</parameter>
		/// <parameter name="type">Postfix operator to apply on the expression.</parameter>
		PostfixExpression( const copying_pointer< Expression >& expression , int type );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this PostfixExpression</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	class VariableDeclaration;

	/// <summary>
	/// Represents a function's argument's reference in some expression.
	/// </summary>
	class ArgumentReference : public Expression
	{
		const VariableDeclaration* argument;
	public:
		/// <summary>
		/// Returns a newly constructed ArgumentReference that references the given variable.
		/// </summary>
		/// <parameter IsConst="true" name="arg">Argument that is referenced.</parameter>
		/// <returns>A proxy object that acts like an ArgumentReference.</returns>
		static copying_pointer< ArgumentReference > Instantiate( const VariableDeclaration& arg )
		{
			return copying_pointer< ArgumentReference >( new ArgumentReference( arg ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed ArgumentReference.</returns>
		ArgumentReference& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this ArgumentReference.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		ArgumentReference& Assign( const CodeObject& object );
	protected:
		/// <summary>
		/// Constructs a new ArgumentReference that references the given variable.
		/// </summary>
		/// <parameter IsConst="true" name="arg">Argument that is referenced.</parameter>
		ArgumentReference( const VariableDeclaration& arg );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this ArgumentReference</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	/// <summary>
	/// Represents a cast in code.
	/// </summary>
	class Cast : public Expression
	{
		std::wstring target;
		const Expression* expression;
	public:
		/// <summary>
		/// Represents the type of case expression.
		/// </summary>
		enum CastType
		{
			/// <summary>
			/// Represents a static_cast&lt; &gt; in code.
			/// </summary>
			Static , 
			/// <summary>
			/// Represents a const_cast&lt; &gt; in code.
			/// </summary>
			Const , 
			/// <summary>
			/// Represents a reinterpret_cast&lt; &gt; in code.
			/// </summary>
			Reinterpret , 
			/// <summary>
			/// Represents a dynamic_cast&lt; &gt; in code.
			/// </summary>
			Dynamic , 
			/// <summary>
			/// Represents a ( [Type] )[expression] style cast in code.
			/// </summary>
			CStyle
		};

		/// <summary>
		/// Creates a new Cast object.
		/// </summary>
		/// <parameter name="targettype" IsConst="true">Target type to cast to.</parameter>
		/// <parameter name="source" IsConst="true">Source expression to cast.</parameter>
		/// <parameter name="type">Cast type to use.</parameter>
		/// <returns>A proxy object that acts like a Cast.</returns>
		static copying_pointer< Cast > Instantiate( const std::wstring& targettype , const Expression& source , CastType type )
		{
			return copying_pointer< Cast >( new Cast( targettype , source , type ) );
		}

		/// <summary>
		/// Assigns another object to this Cast.
		/// </summary>
		/// <parameter IsConst="true" name="object">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		Cast& Assign( const CodeObject& object );
		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed Cast.</returns>
		Cast& Duplicate( ) const;

		/// <summary>
		/// Returns the type to which the cast is performed.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>The type to cast to.</returns>
		std::wstring Target( ) const;
		/// <summary>
		/// Returns the source expression to cast.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>Source expression to cast.</returns>
		const Expression& Source( ) const;
		/// <summary>
		/// Returns the type of cast to perform.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>Cast's type.</returns>
		CastType Casttype( ) const;
	protected:
		/// <summary>
		/// Creates a new Cast object.
		/// </summary>
		/// <parameter name="targettype" IsConst="true">Target type to cast to.</parameter>
		/// <parameter name="source" IsConst="true">Source expression to cast.</parameter>
		/// <parameter name="type">Cast type to use.</parameter>
		Cast( const std::wstring& targettype , const Expression& source , CastType type );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this Cast</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	private : 
		CastType type;
	};

	/// <summary>
	/// Represents a conditional expression in code.
	/// </summary>
	class ConditionalExpression : public Expression
	{
		const Expression *test , *correct , *incorrect;
	public:
		/// <summary>
		/// Creates a new ConditionalExpression object.
		/// </summary>
		/// <parameter IsConst="true" name="a">Condition expression</parameter>
		/// <parameter name="b" IsConst="true">True expression</parameter>
		/// <parameter name="c" IsConst="true">False expression</parameter>
		/// <returns>A proxy object that acts like a ConditionalExpression.</returns>
		static copying_pointer< ConditionalExpression > Instantiate( const Expression& a , const Expression& b , const Expression& c )
		{
			return copying_pointer< ConditionalExpression >( new ConditionalExpression( a , b , c ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed ConditionalExpression.</returns>
		ConditionalExpression& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this ConditionalExpression.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		ConditionalExpression& Assign( const CodeObject& object );

		/// <summary>
		/// Returns the condition expression.
		/// <summary>
		/// <IsConstMember value="true"/>
		/// <returns>A reference to the condition.</returns>
		const Expression& Condition( ) const;
		/// <summary>
		/// Returns the true part to conditional expression.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A reference to the true expression.</returns>
		const Expression& True( ) const;
		/// <summary>
		/// Returns the false part to conditional expression.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A reference to the false expression.</returns>
		const Expression& False( ) const;
	protected:
		/// <summary>
		/// Creates a new ConditionalExpression object.
		/// </summary>
		/// <parameter name="a" IsConst="true">Control expression</parameter>
		/// <parameter name="b" IsConst="true">True expression</parameter>
		/// <parameter name="c" IsConst="true">False expression</parameter>
		ConditionalExpression( const Expression& a , const Expression& b , const Expression& c );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this ConditionalExpression</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	/// <summary>
	/// Represents a variable reference in an expression.
	/// </summary>
	class VariableReference : public Expression
	{
		const VariableDeclaration* reference;
	public:
		/// <summary>
		/// Creates a new VariableReference object.
		/// </summary>
		/// <parameter name="variable" IsConst="true">VariableDeclaration which is referenced.</parameter>
		/// <returns>A proxy object that acts like a VariableReference.</returns>
		static copying_pointer< VariableReference > Instantiate( const VariableDeclaration& variable )
		{
			return copying_pointer< VariableReference >( new VariableReference( variable ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed VariableReference.</returns>
		VariableReference& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this VariableReference.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		VariableReference& Assign( const CodeObject& object );
	protected:
		/// <summary>
		/// Creates a new VariableReference object.
		/// </summary>
		VariableReference( );

		/// <summary>
		/// Creates a new VariableReference object.
		/// </summary>
		/// <parameter name="variable" IsConst="true">VariableDeclaration which is referenced.</parameter>
		VariableReference( const VariableDeclaration& variable );

		bool Empty( ) const;

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this VariableReference</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	class Function;
	class MemberFunction;
	typedef TypedCollection< copying_pointer< Expression > > ExpressionCollection;

	/// <summary>
	/// Represents a function call in code.
	/// </summary>
	class CallExpression : public Expression
	{
		std::auto_ptr< copying_pointer< Expression > > target;
		const Function* method;
		std::auto_ptr< ExpressionCollection > arguments;
		bool is_pointer;
	public:
		/// <summary>
		/// Creates a new CallExpression object.
		/// </summary>
		/// <parameter name="func" IsConst="true">Function to call.</parameter>
		/// <parameter name="expr" IsConst="true">An optional expression on which to invoke the function.</parameter>
		/// <returns>A proxy object that acts like a CallExpression.</returns>
		static copying_pointer< CallExpression > Instantiate( const Function& func , const copying_pointer< Expression >& expr = copying_pointer< Expression >( 0 ) )
		{
			return copying_pointer< CallExpression >( new CallExpression( expr , func ) );
		}

		/// <summary>
		/// Creates a new CallExpression object.
		/// </summary>
		/// <parameter name="func" IsConst="true">MemberFunction to call.</parameter>
		/// <parameter name="expr" IsConst="true">An expression on which to invoke the function.</parameter>
		/// <parameter name="pointer">Tells whether to use '-&gt;' or '.' for function call.</parameter>
		/// <returns>A proxy object that acts like a CallExpression.</returns>
		static copying_pointer< CallExpression > Instantiate( const MemberFunction& func , const copying_pointer< Expression > & expr , bool pointer = false )
		{
			return copying_pointer< CallExpression >( new CallExpression( expr , func , pointer ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed CallExpression.</returns>
		CallExpression& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this CallExpression.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		CallExpression& Assign( const CodeObject& object );

		/// <summary>
		/// Gives access to the invoking expression.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>Returns a pointer to any expression on the left hand side of function call if any.</returns>
		const copying_pointer< Expression >* InvokingExpression( ) const;
		/// <summary>
		/// Provides access to all the arguments for this function call.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>An ExpressionCollection containing all the arguments.</returns>
		ExpressionCollection& Arguments( ) const;
		/// <summary>
		/// Gives the function that will be called.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A reference to the function that is called.</returns>
		const Function& Method( ) const;
	protected:
		/// <summary>
		/// Constructs a CallExpression object from another one.
		/// </summary>
		/// <parameter IsConst="true" name="other">Another CallExpression object that serves as the source for this one.</parameter>
		CallExpression( const CallExpression& other );
		/// <summary>
		/// Constructs a new CallExpression object.
		/// </summary>
		/// <parameter name="func" IsConst="true">Function to call.</parameter>
		/// <parameter name="expr" IsConst="true">An optional expression on which to invoke the function.</parameter>
		CallExpression( const copying_pointer< Expression >& expr , const Function& func );
		/// <summary>
		/// Constructs a new CallExpression object.
		/// </summary>
		/// <parameter name="func" IsConst="true">MemberFunction to call.</parameter>
		/// <parameter name="expr" IsConst="true">An expression on which to invoke the function.</parameter>
		/// <parameter name="pointer">Tells whether to use '-&gt;' or '.' for function call.</parameter>
		CallExpression( const copying_pointer< Expression >& expr , const MemberFunction& func , bool pointer );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this CallExpression</parameter>
		/// <exception ref="std::invalid_argument">Thrown if no object is provided on which to invoke this method.</exception>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	/// <summary>
	/// Represents a reference to some function in code graph.
	/// </summary>
	class MethodReference : public Expression
	{
		const Function* method;
	public:
		/// <summary>
		/// Creates a new MethodReference object.
		/// </summary>
		/// <parameter name="func" IsConst="true">Function which will be referenced.</parameter>
		/// <returns>A proxy object that acts like MethodReference,</returns>
		static copying_pointer< MethodReference > Instantiate( const Function& func )
		{
			return copying_pointer< MethodReference >( new MethodReference( func ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed MethodReference.</returns>
		MethodReference& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this MethodReference.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		MethodReference& Assign( const CodeObject& object );
	protected:
		/// <summary>
		/// Constructs a new MethodReference object.
		/// </summary>
		/// <parameter IsConst="true" name="func">Function which will be referenced.</parameter>
		MethodReference( const Function& func );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this MethodReference</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	/// <summary>
	/// Represents a dynamic memory allocation expression in code graph.
	/// </summary>
	class NewExpression : public Expression
	{
		const Type* type;
		std::auto_ptr< ExpressionCollection > arguments;
		unsigned int level;
		bool is_pointer , is_array;
		std::auto_ptr< copying_pointer< Expression > > size;
	public:
		/// <summary>
		/// Creates a new NewExpression given a type and its constructor's arguments.
		/// </summary>
		/// <parameter IsConst="true" name="t">The type whose object will be constructed.</parameter>
		/// <parameter name="arguments">The arguments which will be passed to the constructor,</parameter>
		/// <returns>A proxy object that acts like NewExpression.</returns>
		static copying_pointer< NewExpression > Instantiate( const Type& t , ExpressionCollection& arguments )
		{
			return copying_pointer< NewExpression >( new NewExpression( t , arguments ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed NewExpression.</returns>
		NewExpression& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this NewExpression.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		NewExpression& Assign( const CodeObject& object );

		/// <summary>
		/// Gets a value that indicates the number of pointer indirections applied to this new declarator.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>	A number to indicate the number of pointer indirections.</returns>
		unsigned int IndirectionLevel( ) const;
		/// <summary>
		/// Sets a value that indicates the number of pointer indirections applied to this new declarator.
		/// </summary>
		/// <parameter name="count">A number to indicate the number of pointer indirections.</parameter>
		/// <returns>A reference to this object.</returns>
		NewExpression& IndirectionLevel( unsigned int count );

		/// <summary>
		/// Gets a value that indicates whether this new declarator is an array or not.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A flag to indicate whether this new declarator is an array or not.</returns>
		bool Array( ) const;
		/// <summary>
		/// Sets a value that indicates whether this new declarator is an array or not.
		/// </summary>
		/// <parameter name="flag">A flag to indicate whether this new declarator is an array or not.</parameter>
		/// <returns>A reference to this object.</returns>
		NewExpression& Array( bool flag );

		/// <summary>
		/// Gets a value that indicates whether this new declarator is a pointer or not.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A flag to indicate whether this new declarator is a pointer or not.</returns>
		bool Pointer( ) const;
		/// <summary>
		/// Sets a value that indicates whether this new declarator is a pointer or not.
		/// </summary>
		/// <parameter name="flag">A flag to indicate whether this new declarator is a pointer or not.</parameter>
		/// <returns>A reference to this object.</returns>
		NewExpression& Pointer( bool flag );

		/// <summary>
		/// Gets a pointer to an Expression that indicates the size of array to allocate
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A pointer to an Expression that indicates the size of array to allocate.</returns>
		const copying_pointer< Expression >* Size( ) const;
		/// <summary>
		/// Sets a pointer to an Expression that indicates the size of array to allocate
		/// </summary>
		/// <parameter name="size">A pointer to an Expression that indicates the size of array to allocate.</parameter>
		/// <returns>A reference to this object.</returns>
		NewExpression& Size( copying_pointer< Expression >* size );
	protected:
		NewExpression( const NewExpression& );
		/// <summary>
		/// Constructs a new NewExpression given a type and its constructor's arguments.
		/// </summary>
		/// <parameter IsConst="true" name="t">The type whose object will be constructed.</parameter>
		/// <parameter name="arguments">The arguments which will be passed to the constructor,</parameter>
		NewExpression( const Type& t , ExpressionCollection& arguments );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this NewExpression</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	/// <summary>
	/// Represents an expression in code graph that is surrounded by parenthesis.
	/// </summary>
	class ParenthesizedExpression : public Expression
	{
		const Expression* source;
	public:
		/// <summary>
		/// Creates a new ParenthesizedExpression given another expression.
		/// </summary>
		/// <parameter IsConst="true" name="expression">The expression to surround in parenthesis.</parameter>
		/// <returns>A proxy object that acts like ParenthesizedExpression.</returns>
		static copying_pointer< ParenthesizedExpression > Instantiate( const Expression& expression )
		{
			return copying_pointer< ParenthesizedExpression >( new ParenthesizedExpression( expression ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed ParenthesizedExpression.</returns>
		ParenthesizedExpression& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this ParenthesizedExpression.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		ParenthesizedExpression& Assign( const CodeObject& object );
	protected:
		/// <summary>
		/// Constructs a new ParenthesizedExpression given another expression.
		/// </summary>
		/// <parameter name="expression" IsConst="true">The expression to surround in parenthesis.</parameter>
		ParenthesizedExpression( const Expression& expression );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this ParenthesizedExpression</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	/// <summary>
	/// Represents a deallcation expression in code graph.
	/// </summary>
	class DeleteExpression : public Expression
	{
		copying_pointer< Expression > type;
		bool is_array;
	public:
		/// <summary>
		/// Creates a new DeleteExpression given another source expression.
		/// </summary>
		/// <parameter name="t">The expression whose result will be deleted.</parameter>
		/// <returns>A proxy object that acts like DeleteExpression.</returns>
		static copying_pointer< DeleteExpression > Instantiate( const copying_pointer< Expression >& t )
		{
			return copying_pointer< DeleteExpression >( new DeleteExpression( t ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed DeleteExpression.</returns>
		DeleteExpression& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this DeleteExpression.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		DeleteExpression& Assign( const CodeObject& object );

		/// <summary>
		/// Returns if this expression deletes an array.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>True if delete is performed on an array.</returns>
		bool Array( ) const;
		/// <summary>
		/// Sets or clears a flag to delete an array.
		/// </summary>
		/// <returns>This object with the flag set or cleared.</returns>
		DeleteExpression& Array( bool );


		/// <summary>
		/// Returns the Expression to delete.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A reference to the expression which will be deleted.</returns>
		const Expression& Target( ) const;
	protected:
		/// <summary>
		/// Constructs a new DeleteExpression given another source expression.
		/// </summary>
		/// <parameter name="t">The expression whose result will be deleted.</parameter>
		DeleteExpression( const copying_pointer< Expression >& t );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this DeleteExpression</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	/// <summary>
	/// Represents a throw expression in code.
	/// </summary>
	class ThrowExpression : public Expression
	{
		copying_pointer< Expression > tothrow;
	public:
		/// <summary>
		/// Creates a new ThrowExpression from another expression.
		/// </summary>
		/// <parameter name="expr>Expression to throw.</parameter>
		/// <returns>A proxy object that acts like ThrowExpression.</returns>
		static copying_pointer< ThrowExpression > Instantiate( const copying_pointer< Expression >& expr )
		{
			return copying_pointer< ThrowExpression >( new ThrowExpression( expr ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed ThrowExpression.</returns>
		ThrowExpression& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this ThrowExpression.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		ThrowExpression& Assign( const CodeObject& object );

		/// <summary>
		/// Gives access to the expression to throw.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>Expression which is thrown.</returns>
		const Expression& Throws( ) const;
	protected:
		/// <summary>
		/// Constructs a new ThrowExpression from another expression.
		/// </summary>
		/// <parameter name="expr>Expression to throw.</parameter>
		ThrowExpression( const copying_pointer< Expression >& expr );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this ThrowExpression</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	/// <summary>
	/// Returns a primitive expression like a number, in code graph.
	/// </summary>
	class PrimitiveExpression : public Expression
	{
		std::wstring expression;
	public:
		static const copying_pointer< PrimitiveExpression > emptyexpression;
		/// <summary>
		/// Creates a new PrimitiveExpression from a string.
		/// </summary>
		/// <parameter IsConst="true" name="expression">String to use in this expression.</parameter>
		/// <returns>A proxy object that acts like PrimitiveExpression.</returns>
		static copying_pointer< PrimitiveExpression > Instantiate( const std::wstring& expression = L"" )
		{
			return copying_pointer< PrimitiveExpression >( new PrimitiveExpression( expression ) );
		}

		bool operator != ( const Expression& other ) const
		{
			return false;
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed PrimitiveExpression.</returns>
		PrimitiveExpression& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this PrimitiveExpression.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		PrimitiveExpression& Assign( const CodeObject& );
	protected:
		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this PrimitiveExpression</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	private:
		/// <summary>
		/// Constructs a new PrimitiveExpression from a string.
		/// </summary>
		/// <parameter IsConst="true" name="expression">String to use in this expression.</parameter>
		PrimitiveExpression( const std::wstring& expression = L"" );
	};
	
	struct CaptureType
	{
		static const int None = 0 ,
									Reference = 1 ,
									Value = 2;
	};

	class LambdaCapture : public VariableReference
	{
		int captureType;
	public:
		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed LambdaExpression.</returns>
		LambdaCapture& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this LambdaExpression.
		/// </summary>
		/// <parameter IsConst="true" name="object">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		LambdaCapture& Assign( const CodeObject& object );

		static copying_pointer< LambdaCapture > Instantiate( int captureMode )
		{
			return copying_pointer< LambdaCapture >( new LambdaCapture( captureMode ) );
		}

		static copying_pointer< LambdaCapture > Instantiate( copying_pointer< VariableDeclaration >& capturedVariable , int captureMode )
		{
			return copying_pointer< LambdaCapture >( new LambdaCapture( capturedVariable , captureMode ) );
		}
	protected:
		LambdaCapture( int captureMode );
		LambdaCapture( copying_pointer< VariableDeclaration >& capturedVariable , int captureMode );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this LambdaExpression</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	typedef TypedCollection< copying_pointer< VariableDeclaration > > ParameterCollection;
	typedef TypedCollection< copying_pointer< CodeObject > > CodeCollection;
	typedef TypedCollection< copying_pointer< LambdaCapture > > CaptureCollection;
	typedef TypedCollection< std::wstring > StringCollection;

	class LambdaExpression : public Expression , public Callable
	{
		std::auto_ptr< CodeCollection > statements;

		std::auto_ptr< CaptureCollection > captures;

		bool is_mutable;
	public:
		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed LambdaExpression.</returns>
		LambdaExpression& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this LambdaExpression.
		/// </summary>
		/// <parameter IsConst="true" name="object">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		LambdaExpression& Assign( const CodeObject& object );

		static copying_pointer< LambdaExpression > Instantiate( )
		{
			return copying_pointer< LambdaExpression >( new LambdaExpression( ) );
		}

		static copying_pointer< LambdaExpression > Instantiate( copying_pointer< VariableDeclaration >& returns )
		{
			return copying_pointer< LambdaExpression >( new LambdaExpression( returns ) );
		}

		bool Mutable( ) const;
		LambdaExpression& Mutable( bool flag );

		CaptureCollection& Captures( ) const;

		/// <IsConstMember value="true"/>
		CodeCollection& Statements( ) const;
	protected:
		LambdaExpression( );
		LambdaExpression( copying_pointer< VariableDeclaration >& );
		LambdaExpression( const LambdaExpression& );

		void Assign( const LambdaExpression& , bool );

		/// <IsConstMember value="true"/>
		void writecapturelist( std::wostream& , unsigned long ) const;
		/// <IsConstMember value="true"/>
		void writereturntype( std::wostream& ) const;
		/// <IsConstMember value="true"/>
		void writebody( std::wostream& , unsigned long ) const;

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this LambdaExpression</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};
}

#endif