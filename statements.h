#ifndef STATEMENTS_HEADER
#define STATEMENTS_HEADER

#include <list>
#include "cppcodeprovider.h"

namespace CPlusPlusCodeProvider
{
	class ScopeResolutionExpression;

	/// <summary>
	/// Represents a using directive in code graph.
	/// </summary>
	class UsingStatement : public Statement
	{
		copying_pointer< Comment > comment;
		copying_pointer< ScopeResolutionExpression > name;
	public:
		/// <summary>
		/// Returns a reference to the comments applied to this using statement.
		/// </summary>
		/// <returns>A reference to comments.</returns>
		Comment& Comment( );

		/// <summary>
		/// Creates a new UsingStatement from a ScopeResolutionExpression object.
		/// </summary>
		/// <parameter name="name" IsConst="true">Name of the entity which will be declared in the current scope.</parameter>
		/// <returns>A proxy object that acts like a UsingStatement.</returns>
		static copying_pointer< UsingStatement > Instantiate( const copying_pointer< ScopeResolutionExpression >& name )
		{
			return copying_pointer< UsingStatement >( new UsingStatement( name ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed UsingStatement.</returns>
		UsingStatement& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this UsingStatement.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		UsingStatement& Assign( const CodeObject& other );
	private:
		/// <summary>
		/// Constructs a new UsingStatement from a ScopeResolutionExpression object.
		/// </summary>
		/// <parameter name="obj" IsConst="true">Name of the entity which will be declared in the current scope.</parameter>
		UsingStatement( const copying_pointer < ScopeResolutionExpression >& obj );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this UsingStatement</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	/// <summary>
	/// Represents an expressions statement in code-graph.
	/// </summary>
	class ExpressionStatement : public Statement
	{
		copying_pointer< Expression > expression;
		bool is_empty;
		copying_pointer< Comment > comment;
	public:
		/// <summary>
		/// Returns a reference to the comments applied to this expression statement.
		/// </summary>
		/// <returns>A reference to comments.</returns>
		Comment& Comment( );

		/// <summary>
		/// Creates an empty ExpressionStatement.
		/// </summary>
		/// <returns>A proxy object that acts like an ExpressionStatement.</returns>
		static copying_pointer< ExpressionStatement > Instantiate( )
		{
			return copying_pointer< ExpressionStatement >( new ExpressionStatement( ) );
		}
		
		/// <summary>
		/// Creates an ExpressionStatement from an Expression.
		/// </summary>
		/// <parameter IsConst="true" name="expr">Expression which will be used as a statement.</parameter>
		/// <returns>A proxy object that acts like an ExpressionStatement.</returns>
		static copying_pointer< ExpressionStatement > Instantiate( const copying_pointer< Expression >& expr )
		{
			return copying_pointer< ExpressionStatement >( new ExpressionStatement( expr ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed ExpressionStatement.</returns>
		ExpressionStatement& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this ExpressionStatement.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		ExpressionStatement& Assign( const CodeObject& object );

		/// <summary>
		/// Returns true if this object is empty.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>True if the statement is just a semi-colon, otherwise, false.</returns>
		bool IsEmpty( ) const;
	protected:
		/// <summary>
		/// Constructs an empty ExpressionStatement.
		/// </summary>
		ExpressionStatement( );
		/// <summary>
		/// Constructs an ExpressionStatement from an Expression.
		/// </summary>
		/// <parameter IsConst="true" name="expr">Expression which will be used as a statement.</parameter>
		ExpressionStatement( const copying_pointer< Expression >& expr );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this ExpressionStatement</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	/// <summary>
	/// Represents a jump statement in code-graph.
	/// </summary>
	class JumpStatement : public Statement
	{
		std::wstring label;
		copying_pointer< Expression >* expr;
		int type;
	public:
		/// <summary>
		/// Denotes different types of jumps.
		/// </summary>
		struct JumpType
		{
			enum
			{
				/// <summary>
				/// Represents a return statement in code.
				/// </summary>
				Return ,
				/// <summary>
				/// Represents a goto statement in code.
				/// </summary>
				Goto ,
				/// <summary>
				/// Represents a break statement inside a loop or switch statement.
				/// </summary>
				Break ,
				/// <summary>
				/// Represents a continue statement inside a loop.
				/// </summary>
				Continue
			};
		};

		/// <summary>
		/// Creates a new JumpStatement with JumpType set to Goto.
		/// </summary>
		/// <parameter IsConst="true" name="target">Target label of the goto statement.</parameter>
		/// <returns>A proxy object that acts like a JumpStatement.</returns>
		static copying_pointer< JumpStatement > Instantiate( const std::wstring& target )
		{
			return copying_pointer< JumpStatement >( new JumpStatement( target ) );
		}

		/// <summary>
		/// Creates a new JumpStatement other than a goto statement.
		/// </summary>
		/// <parameter name="type">Type of this statement.</parameter>
		/// <exception ref="std::invalid_argument">Thrown if type of jump matches goto which is invalid for this function.</exception>
		/// <returns>A proxy object that acts like a JumpStatement.</returns>
		static copying_pointer< JumpStatement > Instantiate( int type )
		{
			return copying_pointer< JumpStatement >( new JumpStatement( type ) );
		}

		/// <summary>
		/// Creates a new JumpStatement with JumpType set to Return.
		/// </summary>
		/// <parameter IsConst="true" name="expr">Expression which will be returned to the calling function.</parameter>
		/// <returns>A proxy object that acts like a JumpStatement.</returns>
		static copying_pointer< JumpStatement > Instantiate( const copying_pointer< Expression >& expr )
		{
			return copying_pointer< JumpStatement >( new JumpStatement( expr ) );
		}

		~JumpStatement( );

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed JumpStatement.</returns>
		JumpStatement& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this JumpStatement.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		JumpStatement& Assign( const CodeObject& object );
	protected:
		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this JumpStatement</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	private:
		/// <summary>
		/// Constructs a new JumpStatement with JumpType set to Goto.
		/// </summary>
		/// <parameter name="target" IsConst="true">Target label of the goto statement.</parameter>
		JumpStatement( const std::wstring& target );
		/// <summary>
		/// Constructs a new JumpStatement other than a goto statement.
		/// </summary>
		/// <parameter name="type">Type of this statement.</parameter>
		/// <exception ref="std::invalid_argument">Thrown if type of jump matches goto which is invalid for this function.</exception>
		JumpStatement( int type );
		/// <summary>
		/// Constructs a new JumpStatement with JumpType set to Return.
		/// </summary>
		/// <parameter name="expr" IsConst="true">Expression which will be returned to the calling function.</parameter>
		JumpStatement( const copying_pointer< Expression >& expr );
		/// <summary>
		/// Constructs a new JumpStatement from another one.
		/// </summary>
		/// <parameter name="other">Another JumpStatement to use as source.</parameter>
		JumpStatement( const JumpStatement& other );
	};

	/// <summary>
	/// Represents a labelled statement in code-graph.
	/// </summary>
	class LabelStatement : public Statement
	{
		std::wstring label;
		const Statement* statement;
	public:
		/// <summary>
		/// Creates a new LabelStatement from the given label and Statement object.
		/// </summary>
		/// <parameter name="str">Label to apply to the given Statement.</parameter>
		/// <parameter name="st">Statement to which given label will be applied.</parameter>
		/// <returns>A proxy object that acts like a LabelStatement.</returns>
		static copying_pointer< LabelStatement > Instantiate( const std::wstring& str , const Statement& st )
		{
			return copying_pointer< LabelStatement >( new LabelStatement( str , st ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed LabelStatement.</returns>
		LabelStatement& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this LabelStatement.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		LabelStatement& Assign( const CodeObject& object );

		/// <IsConstMember value="true"/>
		std::wstring Label( ) const;
	protected:
		/// <summary>
		/// Constructs a new LabelStatement from the given label and Statement object.
		/// </summary>
		/// <parameter name="str">Label to apply to the given Statement.</parameter>
		/// <parameter name="st">Statement to which given label will be applied.</parameter>
		LabelStatement( const std::wstring& str , const Statement& st );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this LabelStatement</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	typedef TypedCollection< copying_pointer< CodeObject > > StatementCollection;

	/// <summary>
	/// Represents a compound statement in code-graph.
	/// </summary>
	class StatementBlock : public Statement
	{
		std::auto_ptr< StatementCollection > statements;
	public:
		/// <summary>
		/// Creates a new StatementBlock from given statements.
		/// </summary>
		/// <parameter name="statements">Statements which will be part of this statement block.</parameter>
		/// <returns>A proxy object which acts like a StatementBlock.</returns>
		static copying_pointer< StatementBlock > Instantiate( StatementCollection& statements )
		{
			return copying_pointer< StatementBlock >( new StatementBlock( statements ) );
		}

		static copying_pointer< StatementBlock > Instantiate( )
		{
			return copying_pointer< StatementBlock >( new StatementBlock );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed StatementBlock.</returns>
		StatementBlock& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this StatementBlock.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		StatementBlock& Assign( const CodeObject& object );

		/// <summary>
		/// Returns all the statements in this statement block.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A collection of all the statements in this code block.</returns>
		StatementCollection& Statements( ) const;
	protected:
		/// <summary>
		/// Constructs an empty StatementBlock.
		/// </summary>
		StatementBlock( );
		/// <summary>
		/// Constructs a new StatementBlock from given statements.
		/// </summary>
		/// <parameter name="statements">Statements which will be part of this statement block.</parameter>
		StatementBlock( StatementCollection& );
		/// <summary>
		/// Constructs a new StatementBlock from another one.
		/// </summary>
		/// <parameter name="other">Another StatementBlock that acts like a source.</parameter>
		StatementBlock( const StatementBlock& other );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this StatementBlock</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	class VariableDeclaration;

	/// <summary>
	/// Represents a catch clause in code-graph.
	/// </summary>
	class CatchClause : public StatementBlock
	{
		bool empty;
		copying_pointer< Comment > comment;
		copying_pointer< VariableDeclaration > var;
	public:
		/// <summary>
		/// Returns a reference to the comments applied to this catch block.
		/// </summary>
		/// <returns>A reference to comments.</returns>
		Comment& Comment( );

		/// <summary>
		/// Creates a catch block that catches every exception.
		/// </summary>
		/// <returns>A proxy object which acts like CatchBlock.</returns>
		static copying_pointer< CatchClause > Instantiate( )
		{
			return copying_pointer< CatchClause >( new CatchClause );
		}

		/// <summary>
		/// Creates a CatchClause that catches exception of the specified type into a variable named by str.
		/// </summary>
		/// <parameter IsConst="true" name="decl">Declaration of exception variable.</parameter>
		/// <returns>A proxy object which acts like CatchBlock.</returns>
		static copying_pointer< CatchClause > Instantiate( const copying_pointer< VariableDeclaration >& decl )
		{
			return copying_pointer< CatchClause >( new CatchClause( decl ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed CatchClause.</returns>
		CatchClause& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this CatchClause.
		/// </summary>
		/// <parameter IsConst="true" name="object">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		CatchClause& Assign( const CodeObject& object );

		/// <summary>
		/// Returns the declaration of exception variable.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>Declaration of the exception which is caught.</returns>
		const VariableDeclaration& CatchType( ) const;
	protected:
		/// <summary>
		/// Constructs a catch block that catches every exception.
		/// </summary>
		CatchClause( );
		/// <summary>
		/// Constructs a CatchClause that catches exception of the specified type into a variable named by str.
		/// </summary>
		/// <parameter IsConst="true" name="decl">Declaration of exception variable.</parameter>
		CatchClause( const copying_pointer< VariableDeclaration >& decl );
		
		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this CatchClause</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	class PrimitiveExpression;

	/// <summary>
	/// Represents a case statement in code-graph.
	/// </summary>
	class Case : public StatementBlock
	{
		copying_pointer< PrimitiveExpression > label;
		bool has_variable_declarations , is_default;
		copying_pointer< CPlusPlusCodeProvider::Comment > comment;
	public:
		/// <summary>
		/// Returns a reference to the comments applied to this case.
		/// </summary>
		/// <returns>A reference to comments.</returns>
		CPlusPlusCodeProvider::Comment& Comment( );

		/// <summary>
		/// Creates a new default Case.
		/// </summary>
		/// <parameter name="variables">True if this statement block has variable declarations.</parameter>
		/// <returns>A proxy object which acts like a Case.</returns>
		static copying_pointer< Case > Instantiate( bool variables = false )
		{
			return copying_pointer< Case >( new Case( variables ) );
		}

		/// <summary>
		/// Creates a new Case with a label.
		/// </summary>
		/// <parameter IsConst="true" name="case_label">Label for this case.</parameter>
		/// <parameter name="variables">True if this statement block has variable declarations.</parameter>
		/// <returns>A proxy object which acts like a Case.</returns>
		static copying_pointer< Case > Instantiate( const copying_pointer< PrimitiveExpression >& case_label , bool variables = false )
		{
			return copying_pointer< Case >( new Case( case_label , variables ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed Case.</returns>
		Case& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this Assign.
		/// </summary>
		/// <parameter IsConst="true" name="object">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		Case& Assign( const CodeObject& object );

		/// <summary>
		/// Returns the label of this case object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A constant reference to the PrimitiveExpression provided in the constructor.</returns>
		const PrimitiveExpression& Label( ) const;
	protected:
		/// <summary>
		/// Constructs a new default Case.
		/// </summary>
		/// <parameter name="variables">True if this statement block has variable declarations.</parameter>
		Case( bool variables = false );
		/// <summary>
		/// Constructs a new Case with a label.
		/// </summary>
		/// <parameter IsConst="true" name="case_label">Label for this case.</parameter>
		/// <parameter name="variables">True if this statement block has variable declarations.</parameter>
		Case( const copying_pointer< PrimitiveExpression >& case_label , bool variables = false );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this Case</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	typedef TypedCollection< copying_pointer< Case > > CaseCollection;

	/// <summary>
	/// Represents a switch statement in code-graph.
	/// </summary>
	class SwitchStatement : public Statement
	{
		copying_pointer< Expression > condition;
		copying_pointer< Comment > comment;
	public:
		/// <summary>
		/// Returns a reference to the comments applied to this switch statement.
		/// </summary>
		/// <returns>A reference to comments.</returns>
		Comment& Comment( );

		/// <summary>
		/// Creates a new switch statement from a given expression.
		/// </summary>
		/// <parameter IsConst="true" name="multivalue_predicate">An expression whose evaluation will cause some case to be executed.</parameter>
		/// <returns>A proxy object which acts like a SwitchStatement.</returns>
		static copying_pointer< SwitchStatement > Instantiate( const copying_pointer< Expression >& multivalue_predicate )
		{
			return copying_pointer< SwitchStatement >( new SwitchStatement( multivalue_predicate ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed SwitchStatement.</returns>
		SwitchStatement& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this SwitchStatement.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		SwitchStatement& Assign( const CodeObject& object );

		/// <summary>
		/// Returns the cases contained in this switch statement.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A reference to the collection containing all the cases in this statement.</returns>
		CaseCollection& Cases( ) const;
	protected:
		/// <summary>
		/// Constructs another SwitchStatement from another.
		/// </summary>
		/// <parameter name="other">Another switch statement to use as the source.</parameter>
		SwitchStatement( const SwitchStatement& other );
		/// <summary>
		/// Constructs a new switch statement from a given expression.
		/// </summary>
		/// <parameter IsConst="true" name="multivalue_predicate">An expression whose evaluation will cause some case to be executed.</parameter>
		SwitchStatement( const copying_pointer< Expression >& );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this SwitchStatement</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	private:
		std::auto_ptr< CaseCollection > cases;
	};

	typedef TypedCollection< copying_pointer< CatchClause > > CatchClauseCollection;

	/// <summary>
	/// Represents an exception handling block in code-graph.
	/// </summary>
	class TryCatchClause : public StatementBlock
	{
		std::auto_ptr< CatchClauseCollection > catchclauses;
		copying_pointer< Comment > comment;
	public:
		/// <summary>
		/// Returns a reference to the comments applied to this try block.
		/// </summary>
		/// <returns>A reference to comments.</returns>
		Comment& Comment( );

		/// <summary>
		/// Creates an empty try-catch statement.
		/// </summary>
		/// <returns>A proxy object which acts like a TryCatchClause.</returns>
		static copying_pointer< TryCatchClause > Instantiate( )
		{
			return copying_pointer< TryCatchClause >( new TryCatchClause );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed TryCatchClause.</returns>
		TryCatchClause& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this TryCatchClause.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		TryCatchClause& Assign( const CodeObject& object );

		/// <summary>
		/// Returns the catch block in this try statement.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A reference to all CatchClause contained in this try statement.</returns>
		CatchClauseCollection& CatchClauses( ) const;
	protected:
		/// <summary>
		/// Constructs an empty try-catch statement.
		/// </summary>
		TryCatchClause( );
		/// <summary>
		/// Constructs a TryCatchClause from another one.
		/// </summary>
		/// <parameter name="other">Another TryCatchClause object to use as source.</parameter>
		TryCatchClause( const TryCatchClause& other );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this TryCatchClause</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	/// <summary>
	/// Represents selection statement in code-graph.
	/// </summary>
	class ConditionClause : public StatementBlock
	{
		StatementBlock else_block;
		copying_pointer< Expression > condition;
		copying_pointer< Comment > comment;
	public:
		/// <summary>
		/// Returns a reference to the comments applied to this if-else statement.
		/// </summary>
		/// <returns>A reference to comments.</returns>
		Comment& Comment( );

		/// <summary>
		/// Creates a new if-else statement from the given conditional.
		/// </summary>
		/// <parameter IsConst="true" name="cond">The condition which will be checked.</parameter>
		/// <returns>A proxy object which acts like ConditionClause.</returns>
		static copying_pointer< ConditionClause > Instantiate( const copying_pointer< Expression >& cond )
		{
			return copying_pointer< ConditionClause >( new ConditionClause( cond ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed ConditionClause.</returns>
		ConditionClause& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this ConditionClause.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		ConditionClause& Assign( const CodeObject& object );

		/// <summary>
		/// Returns the statement block which is the else part.
		/// </summary>
		/// <returns>A reference to all statements contained in else block.</returns>
		StatementBlock& Else( );

		/// <summary>
		/// Returns the condition that is checked.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A reference to the expression which is checked during execution.</returns>
		const Expression& Condition( ) const;
	protected:
		/// <summary>
		/// Constructs a new if-else statement from the given conditional.
		/// </summary>
		/// <parameter IsConst="true" name="cond">The condition which will be checked.</parameter>
		ConditionClause( const copying_pointer< Expression >& cond );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this ConditionClause</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	/// <summary>
	/// Base class for all iteration statements.
	/// </summary>
	class IterationClause : public StatementBlock
	{
		copying_pointer< Expression > condition;
		copying_pointer< Comment > comment;
	public:
		/// <summary>
		/// Returns a reference to the comments applied to this loop.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A reference to comments.</returns>
		const Comment& Comment( ) const;

		/// <summary>
		/// Returns the expression which is checked once each iteration.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A reference to the condition expression.</returns>
		const Expression& Condition( ) const;
		/// <summary>
		/// Sets the expression which is checked once each iteration.
		/// </summary>
		/// <parameter name="expr" IsConst="true">The expression which is checked once each iteration.</returns>
		void Condition( const copying_pointer< Expression >& expr );
	protected:
		/// <summary>
		/// Constructs an empty loop, or an infinite for loop.
		/// </summary>
		IterationClause( );
		/// <summary>
		/// Constructs a new IterationClause with the given condition.
		/// </summary>
		/// <parameter IsConst="true" name="expr">Condition which will be checked once each iteration.</parameter>
		IterationClause( const copying_pointer< Expression >& expr );

		/// <summary>
		/// Assigns one IterationClause to another.
		/// </summary>
		/// <parameter IsConst="true" name="other">Another IterationClause which will be assigned to this one.</parameter>
		void Assign( const IterationClause& other );
	};

	/// <summary>
	/// Represents a for loop in code-graph.
	/// </summary>
	class ForLoop : public IterationClause
	{
		copying_pointer< Expression > init , change;
	public:
		/// <summary>
		/// Creates an infinite for loop.
		/// </summary>
		/// <returns>A proxy object which acts like ForLoop.</returns>
		static copying_pointer< ForLoop > Instantiate( )
		{
			return copying_pointer< ForLoop >( new ForLoop( ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed ForLoop.</returns>
		ForLoop& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this ForLoop.
		/// </summary>
		/// <parameter IsConst="true" name="object">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		ForLoop& Assign( const CodeObject& object );

		/// <summary>
		/// Returns the initializers for this for loop.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A reference to the expression which has all the initializers for this loop.</returns>
		const Expression& Initailization( ) const;
		/// <summary>
		/// Sets the initializers for this for loop.
		/// </summary>
		/// <parameter IsConst="true" name="initializer">Initializers for this for loop.</parameter>
		/// <returns>A reference to this object.</returns>
		ForLoop& Initialization( const copying_pointer< Expression >& initializer );

		/// <summary>
		/// Returns the change expression associated with this for loop.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A reference to the change expression for this for loop.</returns>
		const Expression& Change( ) const;
		/// <summary>
		/// Sets the change expression associated with this for loop.
		/// </summary>
		/// <parameter IsConst="true" name="expr">The expression which is executed after each iteration.</parameter>
		/// <returns>A reference to this object.</returns>
		ForLoop& Change( const copying_pointer< Expression >& expr );
	protected:
		/// <summary>
		/// Creates an infinite for loop.
		/// </summary>
		ForLoop( );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this ForLoop</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	/// <summary>
	/// Represents a while loop in code-graph.
	/// </summary>
	class WhileLoop : public IterationClause
	{
	public:
		/// <summary>
		/// Creates a new while loop from the given loop condition.
		/// </summary>
		/// <parameter IsConst="true" name="condition">Loop condition which will be checked before every iteration.</parameter>
		/// <returns>A proxy object which acts like a WhileLoop.</returns>
		static copying_pointer< WhileLoop > Instantiate( const copying_pointer< Expression >& condition )
		{
			return copying_pointer< WhileLoop >( new WhileLoop( condition ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed WhileLoop.</returns>
		WhileLoop& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this WhileLoop.
		/// </summary>
		/// <parameter IsConst="true" name="object">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		WhileLoop& Assign( const CodeObject& object );
	protected:
		/// <summary>
		/// Constructs a new while loop from the given loop condition.
		/// </summary>
		/// <parameter IsConst="true" name="condition">Loop condition which will be checked before every iteration.</parameter>
		WhileLoop( const copying_pointer< Expression >& );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this WhileLoop</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	/// <summary>
	/// Represents a do-while loop in code-graph.
	/// </summary>
	class DoWhileLoop : public IterationClause
	{
	public:
		/// <summary>
		/// Creates a new do-while loop from the given loop condition.
		/// </summary>
		/// <parameter IsConst="true" name="condition">Loop condition which will be checked after every iteration.</parameter>
		/// <returns>A proxy object which acts like a DoWhileLoop.</returns>
		static copying_pointer< DoWhileLoop > Instantiate( const copying_pointer< Expression >& condition )
		{
			return copying_pointer< DoWhileLoop >( new DoWhileLoop( condition ) );
		}

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed DoWhileLoop.</returns>
		DoWhileLoop& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this DoWhileLoop.
		/// </summary>
		/// <parameter IsConst="true" name="object">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		DoWhileLoop& Assign( const CodeObject& object );
	protected:
		/// <summary>
		/// Constructs a new do-while loop from the given loop condition.
		/// </summary>
		/// <parameter IsConst="true" name="condition">Loop condition which will be checked after every iteration.</parameter>
		DoWhileLoop( const copying_pointer< Expression >& condition );

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this DoWhileLoop</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};
}
#endif