#ifndef FUNCTIONS_HEADER
#define FUNCTIONS_HEADER

#include "cppcodeprovider.h"

namespace CPlusPlusCodeProvider
{
	class CatchClause;
	class VariableDeclaration;
	class TemplateParameter;
	class UserDefinedType;

	typedef TypedCollection< copying_pointer< CatchClause > > CatchClauseCollection;
	typedef TypedCollection< copying_pointer< Type > > TypeCollection;
	typedef TypedCollection< copying_pointer< TemplateParameter > > TemplateParameterCollection;
	typedef TypedCollection< copying_pointer< VariableDeclaration > > ParameterCollection;
	typedef TypedCollection< copying_pointer< CodeObject > > CodeCollection;

	/// <summary>
	/// Represents a free standing function in code graph.
	/// </summary>
	class Function : public CodeObject , public Callable
	{
		friend class UserDefinedType;
		friend class Union;

		std::auto_ptr< CodeCollection > statements;

		std::auto_ptr< CatchClauseCollection > catchclauses;

		std::auto_ptr< TemplateParameterCollection > template_parameters;

		bool is_inline;

		CompilerCapabilities::Capability capability;
	public:

		class Invalid_identifier : public std::exception
		{
		};

		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed Function.</returns>
		Function& Duplicate( ) const;
		/// <summary>
		/// Assigns another object to this Function.
		/// </summary>
		/// <parameter IsConst="true" name="object">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		Function& Assign( const CodeObject& object );

		static copying_pointer< Function > Instantiate( const std::wstring& name_ , CompilerCapabilities::Capability capabilities = CompilerCapabilities::FunctionTryBlocks | CompilerCapabilities::ExceptionSpecifications )
		{
			return copying_pointer< Function >( new Function( name_ , capabilities ) );
		}

		static copying_pointer< Function > Instantiate( const std::wstring& name_ , copying_pointer< VariableDeclaration >& returns , CompilerCapabilities::Capability capabilities = CompilerCapabilities::FunctionTryBlocks | CompilerCapabilities::ExceptionSpecifications)
		{
			return copying_pointer< Function >( new Function( name_ , returns , capabilities ) );
		}

		/// <IsConstMember value="true"/>
		CatchClauseCollection& CatchClauses( ) const;

		/// <IsConstMember value="true"/>
		bool FunctionTryBlock( ) const;

		/// <IsConstMember value="true"/>
		bool Inline( ) const;
		Function& Inline( bool );

		/// <IsConstMember value="true"/>
		bool Template( ) const;
		
		/// <IsConstMember value="true"/>
		TemplateParameterCollection& TemplateParameters( ) const;

		/// <IsConstMember value="true"/>
		CodeCollection& Statements( ) const;

		void declaration( std::wostream& os , unsigned long tabs ) const;
	protected:
		mutable bool separate;

		Function( const std::wstring& , CompilerCapabilities::Capability capabilities = CompilerCapabilities::FunctionTryBlocks | CompilerCapabilities::ExceptionSpecifications );
		Function( const std::wstring& , copying_pointer< VariableDeclaration >& , CompilerCapabilities::Capability capabilities = CompilerCapabilities::FunctionTryBlocks | CompilerCapabilities::ExceptionSpecifications );
		Function( const Function& );

		void Assign( const Function& , bool );

		/// <IsConstMember value="true"/>
		void writetemplateattributes( std::wostream& , unsigned long ) const;
		/// <IsConstMember value="true"/>
		void writebody( std::wostream& , unsigned long ) const;
		/// <IsConstMember value="true"/>
		void writecatchclauses( std::wostream& , unsigned long ) const;

		/// <IsConstMember value="true"/>
		virtual void writedeclaration( std::wostream& , unsigned long ) const;

		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this Function</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
		/// <summary>
		/// Writes out this object split across the two streams.
		/// </summary>
		/// <parameter name="declas">Declaration output stream.</parameter>
		/// <parameter name="defos">Definition output stream.</parameter>
		/// <parameter name="tabs">Indentation for this Function.</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;
	};

	class nestabletype;

	/// <summary>
	/// Represents a member function in code graph.
	/// </summary>
	class MemberFunction : public Function
	{
		const nestabletype* enclosingtype;
		bool is_virtual , is_static , is_constant , is_volatile , is_pure , force_body;
		AccessRestrictions access;
	public:
		static copying_pointer< MemberFunction > Instantiate( const nestabletype& type , const std::wstring& name , copying_pointer< VariableDeclaration >& returns , AccessRestrictions restrictions , CompilerCapabilities::Capability capabilities = CompilerCapabilities::FunctionTryBlocks | CompilerCapabilities::ExceptionSpecifications )
		{
			return copying_pointer< MemberFunction >( new MemberFunction( type , name , returns , restrictions , capabilities ) );
		}

		static copying_pointer< MemberFunction > Instantiate( const nestabletype& type , const std::wstring& name , AccessRestrictions restrictions , CompilerCapabilities::Capability capabilities = CompilerCapabilities::FunctionTryBlocks | CompilerCapabilities::ExceptionSpecifications )
		{
			return copying_pointer< MemberFunction >( new MemberFunction( type , name , restrictions , capabilities ) );
		}

		/// <summary>
		/// Assigns another object to this MemberFunction.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		MemberFunction& Assign( const CodeObject& object );
		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed MemberFunction.</returns>
		MemberFunction& Duplicate( ) const;

		bool ForceBody( ) const;
		MemberFunction& ForceBody( bool flag );

		bool Virtual( ) const;
		MemberFunction& Virtual( bool flag );

		bool Static( ) const;
		MemberFunction& Static( bool flag );

		bool Constant( ) const;
		MemberFunction& Constant( bool flag );

		bool Volatile( ) const;
		MemberFunction& Volatile( bool flag );

		bool Pure( ) const;
		MemberFunction& Pure( bool flag );

		/// <IsConstMember value="true"/>
		AccessRestrictions Access( ) const;

		const nestabletype& EnclosingType( ) const;
	protected:
		MemberFunction( const nestabletype& , const std::wstring& , AccessRestrictions , CompilerCapabilities::Capability capabilities = CompilerCapabilities::FunctionTryBlocks | CompilerCapabilities::ExceptionSpecifications );
		MemberFunction( const nestabletype& , const std::wstring& , copying_pointer< VariableDeclaration >& , AccessRestrictions , CompilerCapabilities::Capability capabilities = CompilerCapabilities::FunctionTryBlocks | CompilerCapabilities::ExceptionSpecifications );

		/// <IsConstMember value="true"/>
		void writedeclaration( std::wostream& , unsigned long ) const;
		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this MemberFunction</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	class UserDefinedType;
	/// <summary>
	/// Represents a constructor in code graph.
	/// </summary>
	class Constructor : public Function
	{
		bool is_explicit;
		std::auto_ptr< ParameterCollection > initializer_list;
		const UserDefinedType* enclosingtype;
		AccessRestrictions access;
	public:
		static copying_pointer< Constructor > Instantiate( const UserDefinedType& type , AccessRestrictions restrictions = Public , bool flag = false , CompilerCapabilities::Capability capabilities = CompilerCapabilities::FunctionTryBlocks | CompilerCapabilities::ExceptionSpecifications )
		{
			return copying_pointer< Constructor >( new Constructor( type , restrictions , flag , capabilities ) );
		}

		/// <IsConstMember value="true"/>
		copying_pointer< VariableDeclaration >* ReturnType( ) const;

		/// <summary>
		/// Assigns another object to this Constructor.
		/// </summary>
		/// <parameter IsConst="true" name="object">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		Constructor& Assign( const CodeObject& object );
		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed Constructor.</returns>
		Constructor& Duplicate( ) const;

		/// <IsConstMember value="true"/>
		std::wstring Name( ) const;

		Constructor& Explicit( bool );
		/// <IsConstMember value="true"/>
		bool Explicit( ) const;

		/// <IsConstMember value="true"/>
		AccessRestrictions Access( ) const;
		Constructor& Access( AccessRestrictions );

		/// <IsConstMember value="true"/>
		ParameterCollection& InitializerList( ) const;
	protected:
		Constructor( const Constructor& );
		Constructor( const UserDefinedType& , AccessRestrictions , bool = false , CompilerCapabilities::Capability capabilities = CompilerCapabilities::FunctionTryBlocks | CompilerCapabilities::ExceptionSpecifications );

		/// <IsConstMember value="true"/>
		void writedeclaration( std::wostream& , unsigned long ) const;
		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this Constructor</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	private:
		/// <IsConstMember value="true"/>
		void writeinitializerlist( std::wostream& , unsigned long ) const;
	};

	/// <summary>
	/// Represents a destructor in code graph.
	/// </summary>
	class Destructor : public Function
	{
		const UserDefinedType* enclosingtype;
		bool is_virtual , is_pure;
		AccessRestrictions access;
	public:
		static copying_pointer< Destructor > Instantiate( const UserDefinedType& type , AccessRestrictions restrictions = Public , CompilerCapabilities::Capability capabilities = CompilerCapabilities::FunctionTryBlocks | CompilerCapabilities::ExceptionSpecifications )
		{
			return copying_pointer< Destructor >( new Destructor( type , restrictions , capabilities ) );
		}

		/// <IsConstMember value="true"/>
		copying_pointer< VariableDeclaration >* ReturnType( ) const;

		/// <summary>
		/// Assigns another object to this Destructor.
		/// </summary>
		/// <parameter IsConst="true" name="object">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		Destructor& Assign( const CodeObject& object );
		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed Destructor.</returns>
		Destructor& Duplicate( ) const;

		/// <IsConstMember value="true"/>
		std::wstring Name( ) const;

		/// <IsConstMember value="true"/>
		bool Pure( ) const;
		Destructor& Pure( bool );

		bool Virtual( ) const;
		Destructor& Virtual( bool );

		AccessRestrictions Access( ) const;
	protected:
		Destructor( const UserDefinedType& , AccessRestrictions , CompilerCapabilities::Capability capabilities = CompilerCapabilities::FunctionTryBlocks | CompilerCapabilities::ExceptionSpecifications );

		/// <IsConstMember value="true"/>
		void writedeclaration( std::wostream& , unsigned long ) const;
		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this Destructor</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	};

	/// <summary>
	/// Represents a user defined operator in code graph.
	/// </summary>
	class Operator : public Function
	{
	public:
		enum OperatorType
		{
			unary , binary , call , conv
		};

		class Invalid_ArgumentList : public std::exception
		{
		};

		static copying_pointer< Operator >Instantiate( const std::wstring& name , OperatorType t_ , CompilerCapabilities::Capability capabilities = CompilerCapabilities::FunctionTryBlocks | CompilerCapabilities::ExceptionSpecifications )
		{
			return copying_pointer< Operator >( new Operator( name , t_ , capabilities ) );
		}

		static copying_pointer< Operator >Instantiate( const std::wstring& name , OperatorType t_ , copying_pointer< VariableDeclaration >& returns , CompilerCapabilities::Capability capabilities = CompilerCapabilities::FunctionTryBlocks | CompilerCapabilities::ExceptionSpecifications )
		{
			return copying_pointer< Operator >( new Operator( name , t_ , returns , capabilities ) );
		}

		/// <summary>
		/// Assigns another object to this Operator.
		/// </summary>
		/// <parameter IsConst="true" name="object">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		Operator& Assign( const CodeObject& object );
		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed Operator.</returns>
		Operator& Duplicate( ) const;

		/// <IsConstMember value="true"/>
		OperatorType OpType( ) const;

		/// <IsConstMember value="true"/>
		std::wstring Name( ) const;
	protected:
		Operator( const std::wstring& , OperatorType , CompilerCapabilities::Capability capabilities = CompilerCapabilities::FunctionTryBlocks | CompilerCapabilities::ExceptionSpecifications );
		Operator( const std::wstring& , OperatorType , copying_pointer< VariableDeclaration >& , CompilerCapabilities::Capability capabilities = CompilerCapabilities::FunctionTryBlocks | CompilerCapabilities::ExceptionSpecifications );
	private:
		/// <IsConstMember value="true"/>
		OperatorType validate( OperatorType , size_t ) const;

		OperatorType t;
	};

	/// <summary>
	/// Represents a user defined member operator in code graph.
	/// </summary>
	class MemberOperator : public Operator
	{
		const nestabletype* enclosingtype;
		bool is_virtual , is_const , is_pure , is_volatile , force_body;
		AccessRestrictions access;
	public:
		static copying_pointer< MemberOperator > Instantiate( const nestabletype& t , const std::wstring& name , OperatorType t_ , AccessRestrictions restrictions , CompilerCapabilities::Capability capabilities = CompilerCapabilities::FunctionTryBlocks | CompilerCapabilities::ExceptionSpecifications )
		{
			return copying_pointer< MemberOperator >( new MemberOperator( t , name , t_ , restrictions , capabilities ) );
		}

		static copying_pointer< MemberOperator > Instantiate( const nestabletype& t , const std::wstring& name , OperatorType t_ , copying_pointer< VariableDeclaration >& returns , AccessRestrictions restrictions , CompilerCapabilities::Capability capabilities = CompilerCapabilities::FunctionTryBlocks | CompilerCapabilities::ExceptionSpecifications )
		{
			return copying_pointer< MemberOperator >( new MemberOperator( t , name , t_ , returns , restrictions , capabilities ) );
		}

		/// <summary>
		/// Assigns another object to this MemberOperator.
		/// </summary>
		/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
		/// <returns>This object with contents replaced by the given one.</returns>
		MemberOperator& Assign( const CodeObject& object );
		/// <summary>
		/// Returns a new copy of this object.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A newly constructed MemberOperator.</returns>
		MemberOperator& Duplicate( ) const;

		bool ForceBody( ) const;
		MemberOperator& ForceBody( bool flag );

		/// <IsConstMember value="true"/>
		bool Virtual( ) const;
		MemberOperator& Virtual( bool flag );

		bool Constant( ) const;
		MemberOperator& Constant( bool flag );

		bool Volatile( ) const;
		MemberOperator& Volatile( bool flag );

		bool Pure( ) const;
		MemberOperator& Pure( bool flag );

		/// <IsConstMember value="true"/>
		AccessRestrictions Access( ) const;
	protected:
		MemberOperator( const nestabletype& , const std::wstring& , OperatorType , AccessRestrictions , CompilerCapabilities::Capability capabilities = CompilerCapabilities::FunctionTryBlocks | CompilerCapabilities::ExceptionSpecifications );
		MemberOperator( const nestabletype& , const std::wstring& , OperatorType , copying_pointer< VariableDeclaration >& , AccessRestrictions , CompilerCapabilities::Capability capabilities = CompilerCapabilities::FunctionTryBlocks | CompilerCapabilities::ExceptionSpecifications );

		/// <IsConstMember value="true"/>
		void writedeclaration( std::wostream& , unsigned long ) const;
		/// <summary>
		/// Writes out the contents to the provided stream.
		/// </summary>
		/// <parameter name="os">Stream which receives the output.</parameter>
		/// <parameter name="tabs">Indentation for this MemberOperator</parameter>
		/// <IsConstMember value="true"/>
		void writetext( std::wostream& os , unsigned long tabs ) const;
	private:
		OperatorType T;
	};
}

#endif