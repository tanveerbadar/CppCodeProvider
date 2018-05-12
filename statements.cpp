#include "statements.h"
#include "declarations.h"
#include "expressions.h"
#include "types.h"
#include "functions.h"
#include "collections.h"

namespace CPlusPlusCodeProvider
{
	using std::wstring;
	using std::wostream;
	using std::invalid_argument;
	using std::auto_ptr;

	Comment& UsingStatement::Comment( )
	{
		return *comment;
	}

	UsingStatement& UsingStatement::Duplicate( ) const
	{
		return *new UsingStatement( *this );
	}

	UsingStatement& UsingStatement::Assign( const CodeObject& object )
	{
		const UsingStatement& ref = cast< UsingStatement >( object );

		comment -> Assign( *ref.comment );
		name -> Assign( *ref.name) ;
		return *this;
	}

	void UsingStatement::writetext( wostream& os , unsigned long tabs ) const
	{
		os << wstring( tabs , L'\n' ) << L"using ";
		name -> write( os , 0 );
		os << L";\n";
	}

	ExpressionStatement& ExpressionStatement::Assign( const CodeObject&object )
	{		
		expression = cast< ExpressionStatement >( object ).expression;
		return *this;
	}

	ExpressionStatement& ExpressionStatement::Duplicate( ) const
	{
		return *new ExpressionStatement( *this );
	}

	Comment& ExpressionStatement::Comment( )
	{
		return *comment;
	}

	ExpressionStatement::ExpressionStatement( )
		 : expression( 0 ) , is_empty( true ) , comment( Comment::Instantiate( ) )
	{
	}

	ExpressionStatement::ExpressionStatement( const copying_pointer< Expression >& expr )
		 : expression( expr ) , is_empty( false ) , comment( Comment::Instantiate( ) )
	{
	}

	bool ExpressionStatement::IsEmpty( ) const
	{
		return is_empty;
	}

	void ExpressionStatement::writetext( wostream& os , unsigned long tabs ) const
	{
		comment -> write( os , tabs );
		if( !comment -> IsEmpty( ) )
			os << L'\n';
		if( !is_empty )
			expression -> write( os , tabs );
		os << L';';
	}

	JumpStatement::JumpStatement( const std::wstring& str )
		: label( str ) , type( JumpType::Goto )
	{
	}

	JumpStatement::JumpStatement( int t )
		: type( t )
	{
		if( type == JumpType::Goto )
			throw invalid_argument( "" );
	}

	JumpStatement::JumpStatement( const copying_pointer< Expression >& e )
		: expr( new copying_pointer< Expression >( static_cast< Expression* >( &( e -> Duplicate( ) ) ) ) ) , type( JumpType::Return )
	{
	}

	JumpStatement::JumpStatement( const JumpStatement& other )
		: label( other.label ) , type( other.type )
	{
		if( type == JumpType::Return )
			expr = new copying_pointer< Expression >( static_cast< Expression* >( &( other.expr -> operator ->( ) -> Duplicate( ) ) ) );
	}

	JumpStatement& JumpStatement::Assign( const CodeObject& object )
	{
		const JumpStatement& ref = cast< JumpStatement >( object );
		type = ref.type , label = ref.label;
		if( expr -> operator -> ( ) )
			delete expr -> release( );
		if( ref.expr )
			expr = new copying_pointer< Expression >( dynamic_cast< Expression* >( &( ref.expr -> operator ->( ) -> Duplicate( ) ) ) );
		return *this;
	}

	JumpStatement& JumpStatement::Duplicate( ) const
	{
		return *new JumpStatement( *this );
	}

	JumpStatement::~JumpStatement( )
	{
		if( type == JumpType::Return )
			delete expr;
	}

	void JumpStatement::writetext( wostream& os , unsigned long tabs ) const
	{
		wstring tab( tabs , FormattingData::IndentationCharacter );
		if( type == JumpType::Continue )
			os << tab << L"continue;";
		else if( type == JumpType::Break )
			os << tab << L"break;";
		else if( type == JumpType::Goto )
			os << tab << L"goto " << label << L';';
		else if( type == JumpType::Return )
		{
			os << tab << L"return";
			if( expr )
			{
				os << L' ';
				( *expr ) -> write( os , 0 );
			}
			os << L';';
		}
	}

	LabelStatement& LabelStatement::Assign( const CodeObject& object )
	{
		const LabelStatement& ref = cast< LabelStatement >( object );
		statement = ref.statement , label = ref.label;
		return *this;
	}

	LabelStatement& LabelStatement::Duplicate( ) const
	{
		return *new LabelStatement( *this );
	}

	LabelStatement::LabelStatement( const wstring& str , const Statement& st )
		 : label( str ) , statement( &st )
	{
	}

	void LabelStatement::writetext( wostream& os , unsigned long tabs ) const
	{
		os << wstring( tabs , FormattingData::IndentationCharacter ) << label << L": ";
		statement -> write( os , 0 );
	}

	StatementBlock& StatementBlock::Assign( const CodeObject& object )
	{
		statements.reset( new StatementCollection( *cast< StatementBlock >( object ).statements ) );
		return *this;
	}

	StatementBlock& StatementBlock::Duplicate( ) const
	{
		return *new StatementBlock( *this );
	}

	StatementBlock::StatementBlock( )
		 : statements( new StatementCollection( ) )
	{
	}

	StatementBlock::StatementBlock( StatementCollection& statement )
		 : statements( new StatementCollection( statement ) )
	{
	}

	StatementBlock::StatementBlock( const StatementBlock& other )
		 : statements( new StatementCollection( *other.statements ) )
	{
	}

	StatementCollection& StatementBlock::Statements( ) const
	{
		return *statements;
	}

	void StatementBlock::writetext( wostream& os , unsigned long tabs ) const
	{
		if( !statements -> size( ) )
			return;
		wstring tab( tabs , FormattingData::IndentationCharacter );
		if( statements -> size( ) > 1 )
			os << tab << L"{\n";
		++tabs;
		for( StatementCollection::const_iterator i = statements -> begin( ) ; i != statements -> end( ) ; ++i )
		{
			( **i ).write( os , tabs );
			os << L'\n';
		}
		--tabs;
		if( statements -> size( ) > 1 )
			os << tab << L"}\n";
	}

	CatchClause::CatchClause( const copying_pointer< VariableDeclaration >& decl )
		 : var( decl ) , empty( false ) , comment( Comment::Instantiate( ) )
	{
	}

	CatchClause& CatchClause::Assign( const CodeObject& object )
	{
		const CatchClause& ref = cast< CatchClause >( object );
		StatementBlock::Assign( ref );
		var = ref.var , empty = ref.empty;
		return *this;
	}

	CatchClause& CatchClause	::Duplicate( ) const
	{
		return *new CatchClause( *this );
	}

	Comment& CatchClause::Comment( )
	{
		return *comment;
	}

	const VariableDeclaration& CatchClause::CatchType( ) const
	{
		return *var;
	}

	void CatchClause::writetext( wostream& os , unsigned long tabs ) const
	{
		wstring tab( tabs , FormattingData::IndentationCharacter );
		os << tab << L"catch( ";
		if( empty )
			os << L"...";
		else
		{
			var -> writeasparameter( os );
		}
		os << L" )\n";
		comment -> write( os , tabs );
		if( !comment -> IsEmpty( ) )
			os << L'\n';
		if( !Statements( ).size( ) )
			os << tab << L"{\n";
		StatementBlock::writetext( os , tabs );
		if( !Statements( ).size( ) )
			os << tab << L"}\n";
	}

	CatchClause::CatchClause( )
		: var( 0 ) , empty( true ) , comment( Comment::Instantiate( ) )
	{
	}

	void IterationClause::Assign( const IterationClause& ref )
	{
		StatementBlock::Assign( ref );
		condition.release( ) , condition = ref.condition;
	}

	const Comment& IterationClause::Comment( ) const
	{
		return *comment;
	}

	IterationClause::IterationClause( )
		: condition( PrimitiveExpression::Instantiate( ) ) , comment( Comment::Instantiate( ) )
	{
	}

	IterationClause::IterationClause( const copying_pointer< Expression >& c )
		 : condition( c ) , comment( Comment::Instantiate( ) )
	{
	}

	const Expression& IterationClause::Condition( ) const
	{
		return *condition;
	}

	void IterationClause::Condition( const copying_pointer< Expression >& expression )
	{
		condition = expression;
	}

	SwitchStatement& SwitchStatement::Assign( const CodeObject& object )
	{
		const SwitchStatement& ref = cast< SwitchStatement >( object );
		condition.release( ) , condition = ref.condition;
		cases.reset( new CaseCollection( *ref.cases ) );
		return *this;
	}

	SwitchStatement& SwitchStatement::Duplicate( ) const
	{
		return *new SwitchStatement( *this );
	}

	Comment& SwitchStatement::Comment( )
	{
		return *comment;
	}

	SwitchStatement::SwitchStatement( const copying_pointer< Expression >& expr )
		 : condition( expr ) , cases( 0 ) , comment( Comment::Instantiate( ) )
	{
	}

	SwitchStatement::SwitchStatement( const SwitchStatement& other )
		 : condition( other.condition ) , cases( new CaseCollection( *other.cases ) ) , comment( other.comment )
	{
	}

	Case& Case::Assign( const CodeObject& object )
	{
		const Case& ref = cast< Case >( object );
		label = ref.label , has_variable_declarations = ref.has_variable_declarations , is_default = ref.is_default;
		return *this;
	}

	Case& Case::Duplicate( ) const
	{
		return *new Case( *this );
	}

	Case::Case( bool variables )
		: label( PrimitiveExpression::Instantiate( ) ) , is_default( true ) , has_variable_declarations( variables ) , comment( Comment::Instantiate( ) )
	{
	}

	Case::Case( const copying_pointer< PrimitiveExpression >& expression , bool variables )
		 : label( expression ) , has_variable_declarations( variables ) , is_default( false ) , comment( Comment::Instantiate( ) )
	{
	}

	void Case::writetext( wostream& os , unsigned long tabs ) const
	{
		wstring tab( tabs , FormattingData::IndentationCharacter );
		if( is_default )
			os << tab << L"default";
		else
		{
			os << tab << L"case ";
			label -> write( os , 0 );
		}
		os << L":\n";
		if( has_variable_declarations && !Statements( ).size( ) )
			os << tab<< L"{\n";
		StatementBlock::writetext( os , tabs );
		if( has_variable_declarations && !Statements( ).size( ) )
			os << tab << L"}\n";
	}

	void SwitchStatement::writetext( wostream& os , unsigned long tabs ) const
	{
		comment -> write( os , tabs );
		if( !comment -> IsEmpty( ) )
			os << L'\n';
		wstring tab( tabs , FormattingData::IndentationCharacter );
		os << tab << L"switch( ";
		condition -> write( os , 0 );
		os << L" )\n";
		os << tab << L"{\n";
		if( cases.get( ) )
			for( CaseCollection::const_iterator i = cases -> begin( ) ; i != cases -> end( ) ; ++i )
				( *i ) -> write( os , tabs );
		os << tab << L"}\n";
	}

	TryCatchClause& TryCatchClause::Assign( const CodeObject& object )
	{
		const TryCatchClause& ref = cast< TryCatchClause >( object );
		StatementBlock::Assign( ref );
		catchclauses.reset( new CatchClauseCollection( *ref.catchclauses ) );
		return *this;
	}

	TryCatchClause& TryCatchClause::Duplicate( ) const
	{
		return *new TryCatchClause( *this );
	}

	Comment& TryCatchClause::Comment( )
	{
		return *comment;
	}

	TryCatchClause::TryCatchClause( )
		 : catchclauses( new CatchClauseCollection ) , comment( Comment::Instantiate( ) )
	{
	}

	TryCatchClause::TryCatchClause( const TryCatchClause& other )
		 : StatementBlock( other ) , catchclauses( new CatchClauseCollection( *other.catchclauses ) ) , comment( other.comment )
	{
	}

	CatchClauseCollection& TryCatchClause::CatchClauses( ) const
	{
		return *catchclauses;
	}

	void TryCatchClause::writetext( wostream& os , unsigned long tabs ) const
	{
		comment -> write( os , tabs );
		if( !comment -> IsEmpty( ) )
			os << L'\n';
		wstring tab( tabs , FormattingData::IndentationCharacter );
		os << tab << L"try\n";
		if( Statements( ).size( ) < 2 )
			os << tab << L"{\n";

		StatementBlock::writetext( os , tabs );

		if( Statements( ).size( ) < 2 )
			os << tab << L"}\n";

		for( CatchClauseCollection::const_iterator i = catchclauses -> begin( );i != catchclauses -> end( ); ++i )
			( **i ).write( os , tabs );
	}

	ConditionClause& ConditionClause::Assign( const CodeObject& object )
	{
		const ConditionClause& ref = cast< ConditionClause >( object );
		condition.release( ) , condition = ref.condition;
		else_block.Assign( ref.else_block );
		return *this;
	}

	ConditionClause& ConditionClause::Duplicate( ) const
	{
		return *new ConditionClause( *this );
	}

	Comment& ConditionClause::Comment( )
	{
		return *comment;
	}

	ConditionClause::ConditionClause( const copying_pointer< Expression >& expr )
		: condition( expr ) , comment( Comment::Instantiate( ) )
	{
	}

	const Expression& ConditionClause::Condition( ) const
	{
		return *condition;
	}

	StatementBlock& ConditionClause::Else( )
	{
		return else_block;
	}

	void ConditionClause::writetext( wostream& os , unsigned long tabs ) const
	{
		wstring tab( tabs , FormattingData::IndentationCharacter );
		os << tab << L"if( ";
		condition -> write( os , 0 );
		os << L" )\n";
		comment -> write( os , tabs );
		if( !comment -> IsEmpty( ) )
			os << L'\n';
		StatementBlock::writetext( os , tabs );
		if( else_block.Statements( ).size( ) > 0 )
		{
			os << L"\nelse\n";
			else_block.write( os , tabs );
		}
	}

	ForLoop& ForLoop::Assign( const CodeObject&object )
	{
		const ForLoop& ref = cast< ForLoop >( object );
		IterationClause::Assign( ref );
		init.release( ) , init = ref.init , change.release( ) , change = ref.change;
		return *this;
	}

	ForLoop& ForLoop	::Duplicate( ) const
	{
		return *new ForLoop( *this );
	}

	ForLoop::ForLoop( )
		: init( PrimitiveExpression::Instantiate( ) ) , change( PrimitiveExpression::Instantiate( ) )
	{
	}

	const Expression& ForLoop::Change( ) const
	{
		return *change;
	}

	ForLoop& ForLoop::Change( const copying_pointer< Expression >& expression )
	{
		change = expression;
		return *this;
	}

	const Expression& ForLoop::Initailization( ) const
	{
		return *init;
	}

	ForLoop& ForLoop::Initialization( const copying_pointer< Expression >& expr )
	{
		init = expr;
		return *this;
	}

	void ForLoop::writetext( wostream& os , unsigned long tabs ) const
	{
		Comment( ).write( os , tabs );
		if( !Comment( ).IsEmpty( ) )
			os << L'\n';
		wstring tab( tabs , FormattingData::IndentationCharacter );
		os << tab << L"for( ";
		init -> write( os , 0 );
		os << L" ; ";
		Condition( ).write( os , 0 );
		os << L" ; ";
		Change( ).write( os , 0 );
		os << L" )\n";
		StatementBlock::writetext( os , tabs );
	}

	WhileLoop& WhileLoop::Assign( const CodeObject& object )
	{
		IterationClause::Assign( cast< WhileLoop >( object ) );
		return *this;
	}

	WhileLoop& WhileLoop::Duplicate( ) const
	{
		return *new WhileLoop( *this );
	}

	WhileLoop::WhileLoop( const copying_pointer< Expression >& condition )
		 : IterationClause( condition )
	{
	}

	void WhileLoop::writetext( wostream& os , unsigned long tabs ) const
	{
		Comment( ).write( os , tabs );
		if( !Comment( ).IsEmpty( ) )
			os << L'\n';
		wstring tab( tabs , FormattingData::IndentationCharacter );
		os << tab << L"while( ";
		Condition( ).write( os , 0 );
		os << L" )\n";
		StatementBlock::writetext( os , tabs );
	}

	DoWhileLoop& DoWhileLoop::Assign( const CodeObject&object )
	{		
		IterationClause::Assign( cast< DoWhileLoop >( object ) );
		return *this;
	}

	DoWhileLoop& DoWhileLoop::Duplicate( ) const
	{
		return *new DoWhileLoop( *this );
	}

	DoWhileLoop::DoWhileLoop( const copying_pointer< Expression >& condition )
		 : IterationClause( condition )
	{
	}

	void DoWhileLoop::writetext( wostream& os , unsigned long tabs ) const
	{
		Comment( ).write( os , tabs );
		if( !Comment( ).IsEmpty( ) )
			os << L'\n';
		wstring tab( tabs , FormattingData::IndentationCharacter );
		os << tab << L"do\n";
		StatementBlock::writetext( os , tabs );
 		os << tab << L"while( ";
		Condition( ).write( os , 0 );
		os << L" );\n";
	}
}