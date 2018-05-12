#include "expressions.h"
#include "functions.h"
#include "declarations.h"
#include "collections.h"

namespace CPlusPlusCodeProvider
{
	using std::wstring;
	using std::wostream;
	using std::invalid_argument;
	using std::auto_ptr;

	void UnaryExpression::Assign( const UnaryExpression& ref )
	{
		source.release( ) , source = ref.source;
	}

	UnaryExpression::UnaryExpression( const copying_pointer< Expression >& expr )
		 : source( expr )
	{
	}

	void UnaryExpression::writetext( wostream& os ) const
	{
		source -> write( os , 0 );
	}

	ScopeResolutionExpression::ScopeResolutionExpression( const wstring& first )
		 : left( first )
	{
	}

	ScopeResolutionExpression::ScopeResolutionExpression( const wstring& first , const copying_pointer< ScopeResolutionExpression >& second )
		 : right( second ) , left( first )
	{
	}

	ScopeResolutionExpression& ScopeResolutionExpression::Assign( const CodeObject& object )
	{
		const ScopeResolutionExpression& ref = cast< ScopeResolutionExpression >( object );
		left = ref.left , right = ref.right;
		return *this;
	}

	ScopeResolutionExpression& ScopeResolutionExpression::Duplicate( ) const
	{
		return *new ScopeResolutionExpression( *this );
	}

	void ScopeResolutionExpression::writetext( wostream& os , unsigned long tabs ) const
	{
		os << wstring( tabs , FormattingData::IndentationCharacter ) << left;
		if( right.operator -> ( ) )
			right -> write( os , 0 );
	}

	BinaryExpression& BinaryExpression::Assign( const CodeObject& object )
	{
		const BinaryExpression& ref = cast< BinaryExpression >( object );
		left.release( ) , left = ref.left , right.release( ) , right = ref.right;
		return *this;
	}

	BinaryExpression& BinaryExpression::Duplicate( ) const
	{
		return *new BinaryExpression( *this );
	}

	BinaryExpression::BinaryExpression( const copying_pointer< Expression >& a , const copying_pointer< Expression >& b , int type )
		 : left( a ) , right( b ) , t( type )
	{
	}

	BinaryExpression::BinaryExpression( const copying_pointer< Expression >& a , const copying_pointer< Expression >& b , const wstring& str , CompilerCapabilities::Capability capability )
		 : left( a ) , right( b ) , name( validate( str , capability ) )
	{
	}

	wstring BinaryExpression::validate( const wstring& str , CompilerCapabilities::Capability capability )
	{
		if( str == L"and" )
			return capability & CompilerCapabilities::AlternateKeywords ? str : ( t = Type::And , L"" );
		if( str == L"and_eq" )
			return capability & CompilerCapabilities::AlternateKeywords ? str : ( t = Type::Addassign , L"" );
		if( str == L"bitand" )
			return capability & CompilerCapabilities::AlternateKeywords ? str : ( t = Type::BitwiseAnd , L"" );
		if( str == L"bitor" )
			return capability & CompilerCapabilities::AlternateKeywords ? str : ( t = Type::BitwiseOr , L"" );
		if( str == L"not_eq" )
			return capability & CompilerCapabilities::AlternateKeywords ? str : ( t = Type::NotEqual , L"" );
		if( str == L"or" )
			return capability & CompilerCapabilities::AlternateKeywords ? str : ( t = Type::Or , L"" );
		if( str == L"or_eq" )
			return capability & CompilerCapabilities::AlternateKeywords ? str : ( t = Type::BitwiseOrassign , L"" );
		if( str == L"xor" )
			return capability & CompilerCapabilities::AlternateKeywords ? str : ( t = Type::Xor , L"" );
		if( str == L"xor_eq" )
			return capability & CompilerCapabilities::AlternateKeywords ? str : ( t = Type::Xorassign , L"" );
		throw invalid_argument( "" );
	}

	void BinaryExpression::writetext( wostream& os , unsigned long tabs ) const
	{
		os << wstring( tabs , FormattingData::IndentationCharacter );
		left -> write( os , 0 );
		if( name.length( ) )
		{
			os << L' ' << name << L' ';
			right -> write( os , 0 );
			return;
		}
		if( t == Type::Subscript )
			os << L" [ ";
		else
			switch( t )
			{
			case Type::Addition:
				os << L" + ";
				break;
			case Type::Subtraction:
				os << L" - ";
				break;
			case Type::Multiplication:
				os << L" * ";
				break;
			case Type::Division:
				os << L" / ";
				break;
			case Type::Modulus:
				os << L" % ";
				break;
			case Type::Assignment:
				os << L" = ";
				break;
			case Type::And:
				os << L" && ";
				break;
			case Type::Or:
				os << L" || ";
				break;
			case Type::Xor:
				os << L" ^ ";
				break;
			case Type::MemberAccess:
				os << L'.';
				break;
			case Type::Comma:
				os << L" , ";
				break;
			case Type::LeftShift:
				os << L" << ";
				break;
			case Type::RightShift:
				os << L" >> ";
				break;
			case Type::Less:
				os << L" < ";
				break;
			case Type::Greater:
				os << L" > ";
				break;
			case Type::LessEqual:
				os << L" <= ";
				break;
			case Type::GreaterEqual:
				os << L" >= ";
				break;
			case Type::Equal:
				os << L" == ";
				break;
			case Type::NotEqual:
				os << L" != ";
				break;
			case Type::BitwiseAnd:
				os << L" & ";
				break;
			case Type::BitwiseOr:
				os << L" | ";
				break;
			case Type::Addassign:
				os << L" += ";
				break;
			case Type::Subtractassign:
				os << L" -= ";
				break;
			case Type::Multiplyassign:
				os << L" *= ";
				break;
			case Type::Divideassign:
				os << L" /= ";
				break;
			case Type::Modulusassign:
				os << L" %= ";
				break;
			case Type::LeftShiftassign:
				os << L" <<= ";
				break;
			case Type::RightShiftassign:
				os << L" >>= ";
				break;
			case Type::BitwiseAndassign:
				os << L" &= ";
				break;
			case Type::BitwiseOrassign:
				os << L" |= ";
				break;
			case Type::Xorassign:
				os << L" ^= ";
				break;
			}
		right -> write( os , 0 );
		if( t == Type::Subscript )
			os << L" ] ";
	}

	PrefixExpression& PrefixExpression::Assign( const CodeObject& object )
	{
		const PrefixExpression& ref = cast< PrefixExpression >( object );
		UnaryExpression::Assign( ref );
		t = ref.t;
		return *this;
	}

	PrefixExpression& PrefixExpression::Duplicate( ) const
	{
		return *new PrefixExpression( *this );
	}

	PrefixExpression::PrefixExpression( const copying_pointer< Expression >& expression , int type )
		 : UnaryExpression( expression ) , t( type )
	{
	}

	PrefixExpression::PrefixExpression( const copying_pointer< Expression >& expression , const wstring& type , CompilerCapabilities::Capability capability )
		 : UnaryExpression( expression ) , name( validate( type , capability ) )
	{
	}

	wstring PrefixExpression::validate( const wstring& str , CompilerCapabilities::Capability capability )
	{
		if( str == L"compl" )
			return capability & CompilerCapabilities::AlternateKeywords ? str : ( t = Type::Complement , L"" );
		if( str == L"not" )
			return capability & CompilerCapabilities::AlternateKeywords ? str : ( t = Type::Not , L"" );
		throw invalid_argument( "" );
	}

	void PrefixExpression::writetext( wostream& os , unsigned long ) const
	{
		if( name.length( ) )
		{
			os << name;
			UnaryExpression::writetext( os );
			return;
		}
		switch( t )
		{
		case Type::Sizeof:
			os << L"sizeof( ";
			break;
		case Type::Addressof:
			os << L'&';
			break;
		case Type::Increment:
			os << L"++";
			break;
		case Type::Decrement:
			os << L"--";
			break;
		case Type::Plus:
			os << L'+';
			break;
		case Type::Negate:
			os << L'-';
			break;
		case Type::Not:
			os << L'!';
			break;
		case Type::Complement:
			os << L'~';
			break;
		case Type::Dereference:
			os << L'*';
			break;
		}
		UnaryExpression::writetext( os );
		if( t == Type::Sizeof )
			os << L" )";
	}

	PostfixExpression& PostfixExpression::Assign( const CodeObject& object )
	{
		const PostfixExpression& ref = cast< PostfixExpression >( object );
		UnaryExpression::Assign( ref );
		t = ref.t;
		return *this;
	}

	PostfixExpression& PostfixExpression::Duplicate( ) const
	{
		return *new PostfixExpression( *this );
	}

	PostfixExpression::PostfixExpression( const copying_pointer< Expression >& expression , int type )
		 : UnaryExpression( expression ) , t( type )
	{
	}

	void PostfixExpression::writetext( wostream& os , unsigned long ) const
	{
		UnaryExpression::writetext( os );
		switch( t )
		{
		case Type::Increment:
			os << L"++";
			break;
		case Type::Decrement:
			os << L"--";
			break;
		case Type::Dereference:
			os << L" -> ";
			break;
		}
	}

	Cast& Cast::Assign( const CodeObject& object )
	{
		const Cast& ref = cast< Cast >( object );
		target = ref.target , expression = ref.expression;
		return *this;
	}

	Cast& Cast::Duplicate( ) const
	{
		return *new Cast( *this );
	}

	Cast::Cast( const wstring& targettype , const Expression& expr , CastType cast )
		 : target( targettype ) , expression( &expr ) , type( cast )
	{
	}

	Cast::CastType Cast::Casttype( ) const
	{
		return type;
	}

	wstring Cast::Target( ) const
	{
		return target;
	}

	const Expression& Cast::Source( ) const
	{
		return *expression;
	}

	void Cast::writetext( wostream& os , unsigned long tabs ) const
	{
		os << wstring( tabs , FormattingData::IndentationCharacter );
		switch( type )
		{
		case Dynamic:
			os << L"dynamic_cast< ";
			break;
		case Static:
			os << L"static_cast< ";
			break;
		case Reinterpret:
			os << L"reinterpret_cast< ";
			break;
		case Const:
			os << L"const_cast< ";
			break;
		case CStyle:
			os << L"( ";
			break;
		}
		os << target;
		if( type == CStyle )
		{
			os << L" )";
			expression -> write( os , 0 );
		}
		else
		{
			os << L" >( ";
			expression -> write( os , 0 );
			os << L" )";
		}
	}

	ArgumentReference::ArgumentReference( const VariableDeclaration& arg )
		 : argument( &arg )
	{
	}

	ArgumentReference& ArgumentReference::Assign( const CodeObject& object )
	{
		const ArgumentReference& ref = cast< ArgumentReference >( object );
		argument = ref.argument;
		return *this;
	}

	ArgumentReference& ArgumentReference::Duplicate( ) const
	{
		return *new ArgumentReference( *this );
	}

	void ArgumentReference::writetext( wostream& os , unsigned long ) const
	{
		os << argument -> Declarator( ).Name( );
	}

	ConditionalExpression& ConditionalExpression::Assign( const CodeObject& object )
	{
		const ConditionalExpression& ref = cast< ConditionalExpression >( object );
		test = ref.test , correct = ref.correct , incorrect = ref.incorrect;
		return *this;
	}

	ConditionalExpression& ConditionalExpression::Duplicate( ) const
	{
		return *new ConditionalExpression( *this );
	}

	ConditionalExpression::ConditionalExpression( const Expression& expression , const Expression& True , const Expression& False )
		 : test( &expression ) , correct( &True ) , incorrect( &False )
	{
	}

	void ConditionalExpression::writetext( wostream& os , unsigned long tabs ) const
	{
		os << wstring( tabs , FormattingData::IndentationCharacter );
		test -> write( os , 0 );
		os << L" ? ";
		correct -> write( os , 0 );
		os << L" : ";
		incorrect -> write( os , 0 );
	}

	CallExpression& CallExpression::Assign( const CodeObject& object )
	{
		const CallExpression& ref = cast< CallExpression >( object );
		if( ref.target.get( ) )
			target.reset( new copying_pointer< Expression >( static_cast< Expression* >( &( *ref.target ) -> Duplicate( ) ) ) );
		method = ref.method , arguments.reset( new ExpressionCollection( *ref.arguments ) );
		return *this;
	}

	CallExpression& CallExpression::Duplicate( ) const
	{
		return *new CallExpression( *this );
	}

	CallExpression::CallExpression( const CallExpression& other )
		 : target( other.target.get( ) ? new copying_pointer< Expression >( *other.target ) : 0 ) , method( other.method ) , arguments( new ExpressionCollection( *other.arguments ) )
	{
	}

	CallExpression::CallExpression( const copying_pointer< Expression >& expr , const MemberFunction& func , bool pointer )
		: target( expr.pointer( ) ? new copying_pointer< Expression >( static_cast< Expression* >( &expr -> Duplicate( ) ) ) : throw NullPointerException( "A member function must be invoked through a target object." ) ) , method( &func ) , is_pointer( pointer ) , arguments( new ExpressionCollection )
	{
	}

	CallExpression::CallExpression( const copying_pointer< Expression >& expr , const Function& func )
		: target( expr.pointer( ) ? new copying_pointer< Expression >( static_cast< Expression* >( &expr -> Duplicate( ) ) ) : 0 ) , method( &func ) , arguments( new ExpressionCollection )
	{
	}

	const Function& CallExpression::Method( ) const
	{
		return *method;
	}

	ExpressionCollection& CallExpression::Arguments( ) const
	{
		return *arguments;
	}

	void CallExpression::writetext( wostream& os , unsigned long tabs ) const
	{
		if( dynamic_cast< const MemberFunction* >( method ) == 0 )
		{
			if( target.get( ) )
				( *target ) -> write( os , tabs );
			else
				os << wstring( tabs , FormattingData::IndentationCharacter );
		}
		else
		{
			//( *target ) -> write( os , tabs );//buggy
			os << is_pointer ? L" -> " : L".";
		}
		os << method -> Name( ) << L"( ";
		if( arguments.get( ) && arguments -> size( ) != 0 )
		{
			ExpressionCollection::const_iterator i = arguments -> begin( );
			( *i ) -> write( os , 0 );
			for( ++i ; i != arguments -> end( ) ; ++i )
			{
				os << L" , ";
				( *i ) -> write( os , 0 );
			}
			os << L' ';
		}
		os << L')';
	}

	VariableReference& VariableReference::Assign( const CodeObject& object )
	{
		const VariableReference& ref = cast< VariableReference >( object );
		reference = ref.reference;
		return *this;
	}

	VariableReference& VariableReference::Duplicate( ) const
	{
		return *new VariableReference( *this );
	}

	VariableReference::VariableReference( )
		 : reference( 0 )
	{
	}

	VariableReference::VariableReference( const VariableDeclaration& variable )
		 : reference( &variable )
	{
	}

	bool VariableReference::Empty( ) const
	{
		return !reference;
	}

	void VariableReference::writetext( wostream& os , unsigned long ) const
	{
		os << reference -> Declarator( ).Name( );
	}

	MethodReference& MethodReference::Assign( const CodeObject& object )
	{
		const MethodReference& ref = cast< MethodReference >( object );
		method = ref.method;
		return *this;
	}

	MethodReference& MethodReference::Duplicate( ) const
	{
		return *new MethodReference( *this );
	}

	MethodReference::MethodReference( const Function& func )
		 : method( &func )
	{
	}

	void MethodReference::writetext( wostream& os , unsigned long ) const
	{
		os << method -> Name( );
	}

	NewExpression& NewExpression::Assign( const CodeObject& object )
	{
		const NewExpression& ref = cast< NewExpression >( object );
		type = ref.type;
		arguments.reset( new ExpressionCollection( *ref.arguments ) );
		level = ref.level;
		is_pointer = ref.is_pointer , is_array = ref.is_array;
		size.release( ) , size.reset( new copying_pointer< Expression >( *ref.size ) );
		return *this;
	}

	NewExpression& NewExpression::Duplicate( ) const
	{
		return *new NewExpression( *this );
	}

	NewExpression::NewExpression( const NewExpression& other )
		: type( other.type ) , arguments( new ExpressionCollection( *other.arguments ) ) , level( other.level ) , is_pointer( other.is_pointer ) , size( other.size.get( ) ? new copying_pointer< Expression >( *other.size ) : 0 ) , is_array( other.is_array )
	{
	}

	NewExpression::NewExpression( const Type& t , ExpressionCollection& args )
		 : type( &t ) , arguments( new ExpressionCollection( args ) ) , level( 0 ) , is_array( false ) , is_pointer( false )
	{
	}

	bool NewExpression::Array( ) const
	{
		return is_array;
	}

	NewExpression& NewExpression::Array( bool flag )
	{
		is_array = flag;
		return *this;
	}

	unsigned int NewExpression::IndirectionLevel( ) const
	{
		return level;
	}

	NewExpression& NewExpression::IndirectionLevel( unsigned int n )
	{
		level = n;
		return *this;
	}

	bool NewExpression::Pointer( ) const
	{
		return is_pointer;
	}

	NewExpression& NewExpression::Pointer( bool val )
	{
		is_pointer = val;
		return *this;
	}

	void NewExpression::writetext( wostream& os , unsigned long tabs ) const
	{
		os << wstring( tabs , FormattingData::IndentationCharacter ) << L"new " << type -> Name( );
		if( Pointer( ) )
			os << wstring( IndirectionLevel( ) , L'*' );
		if( is_array )
		{
			os << L" [ ";
			( *size ) -> write( os , 0 );
			os << L" ] ";
		}
		else if( arguments -> size( ) )
		{
			os << L"( ";
			ExpressionCollection::const_iterator i = arguments -> begin( );
			( *i ) -> write( os , 0 );
			for( ++i ; i != arguments -> end( ) ; ++i )
			{
				os << L" , ";
				( *i ) -> write( os , 0 );
			}
			os << L" )";
		}
	}

	ParenthesizedExpression::ParenthesizedExpression( const Expression& expression )
		 : source( &expression )
	{
	}

	ParenthesizedExpression& ParenthesizedExpression::Assign( const CodeObject& object )
	{
		const ParenthesizedExpression& ref = cast< ParenthesizedExpression >( object );
		source = ref.source;
		return *this;
	}

	ParenthesizedExpression& ParenthesizedExpression::Duplicate( ) const
	{
		return *new ParenthesizedExpression( *this );
	}

	void ParenthesizedExpression::writetext( wostream& os , unsigned long ) const
	{
		os << L"( ";
		source -> write( os , 0 );
		os << L" )";
	}

	DeleteExpression& DeleteExpression::Assign( const CodeObject&object )
	{
		const DeleteExpression& ref = cast< DeleteExpression >( object );
		type.release( ) , type = ref.type;
		is_array = ref.is_array;
		return *this;
	}

	DeleteExpression& DeleteExpression::Duplicate( ) const
	{
		return *new DeleteExpression( *this );
	}

	DeleteExpression::DeleteExpression( const copying_pointer< Expression >& expr )
		 : type( expr ) , is_array( false )
	{
	}

	bool DeleteExpression::Array( ) const
	{
		return is_array;
	}

	DeleteExpression& DeleteExpression::Array( bool flag )
	{
		is_array = flag;
		return *this;
	}

	const Expression& DeleteExpression::Target( ) const
	{
		return *type;
	}

	void DeleteExpression::writetext( wostream& os , unsigned long tabs ) const
	{
		os << wstring( tabs , FormattingData::IndentationCharacter ) << L"delete ";
		if( is_array )
			os << L"[ ] ";
		type -> write( os , 0 );
	}

	ThrowExpression& ThrowExpression::Assign( const CodeObject& object )
	{
		const ThrowExpression& ref = cast< ThrowExpression >( object );
		tothrow.release( ) , tothrow = ref.tothrow;
		return *this;
	}

	ThrowExpression& ThrowExpression::Duplicate( ) const
	{
		return *new ThrowExpression( *this );
	}

	ThrowExpression::ThrowExpression( const copying_pointer< Expression >& expr )
		 : tothrow( expr )
	{
	}

	void ThrowExpression::writetext( wostream& os , unsigned long tabs ) const
	{
		os << wstring( tabs , FormattingData::IndentationCharacter ) << L"throw ";
		tothrow -> write( os , 0 );
	}

	const copying_pointer< PrimitiveExpression > PrimitiveExpression::emptyexpression;

	PrimitiveExpression::PrimitiveExpression( const wstring& str )
		: expression( str )
	{
	}

	PrimitiveExpression& PrimitiveExpression::Duplicate( ) const
	{
		return *new PrimitiveExpression( *this );
	}

	PrimitiveExpression& PrimitiveExpression::Assign( const CodeObject& object )
	{
		expression = cast< PrimitiveExpression >( object ).expression;
		return *this;
	}

	void PrimitiveExpression::writetext( wostream& os , unsigned long ) const
	{
		os << expression;
	}

	LambdaCapture::LambdaCapture( int captureMode )
		: captureType( captureMode ) , VariableReference( )
	{
	}

	LambdaCapture::LambdaCapture( copying_pointer< VariableDeclaration >& ref , int captureMode )
		: captureType( captureMode ) , VariableReference( *ref )
	{
	}

	LambdaCapture& LambdaCapture::Duplicate( ) const
	{
		return *new LambdaCapture( *this );
	}

	LambdaCapture& LambdaCapture::Assign( const CodeObject& other )
	{
		const LambdaCapture& ref = cast< LambdaCapture >( other );
		VariableReference::Assign( ref );
		captureType = ref.captureType;

		return *this;
	}

	void LambdaCapture::writetext( wostream& os , unsigned long tabs ) const
	{
		if( captureType == CaptureType::Reference )
			os << L"&";
		else if( captureType == CaptureType::Value )
			os << L"=";

		if( !Empty( ) )
			VariableReference::writetext( os , tabs );
	}

	LambdaExpression::LambdaExpression( )
		: Callable( L"" ) , statements( new CodeCollection( ) ) , is_mutable( false ) , captures( new CaptureCollection( ) )
	{
	}

	LambdaExpression::LambdaExpression( copying_pointer< VariableDeclaration >& returns )
		: Callable( L"" , returns ) , statements( new CodeCollection( ) ) , is_mutable( false ) , captures( new CaptureCollection( ) )
	{
	}
	
	//incomplete
	LambdaExpression::LambdaExpression( const LambdaExpression& other )
		: Callable( other ) , statements( new CodeCollection( *other.statements ) ) , is_mutable( other.is_mutable ) , captures( new CaptureCollection( *other.captures ) )
	{
	}

	LambdaExpression& LambdaExpression::Duplicate( ) const
	{
		return *new LambdaExpression( *this );
	}

	LambdaExpression& LambdaExpression::Assign( const CodeObject& object )
	{
		const LambdaExpression& ref = cast< LambdaExpression >( object );
		Callable::Assign( ref );
		statements.reset( new CodeCollection( *ref.statements ) );
		captures.reset( new CaptureCollection( *ref.captures ) );

		is_mutable = ref.is_mutable;

		return *this;
	}

	bool LambdaExpression::Mutable( ) const
	{
		return is_mutable;
	}

	LambdaExpression& LambdaExpression::Mutable( bool flag )
	{
		is_mutable = flag;
		return *this;
	}

	CaptureCollection& LambdaExpression::Captures( ) const
	{
		return *captures;
	}

	CodeCollection& LambdaExpression::Statements( ) const
	{
		return *statements;
	}

	void LambdaExpression::writebody( wostream& os , unsigned long tabs ) const
	{
		wstring tab( tabs , FormattingData::IndentationCharacter );
		os << L'\n' << tab << L'{' << L'\n';
		if( statements.get( ) )
		{
			++tabs;
			for( CodeCollection::const_iterator i = statements -> begin( ) ; i != statements -> end( ) ; ++i )
			{
				( **i ).write( os , tabs );
				os << L'\n';
			}
			--tabs;
		}
		os << tab << L"}\n";
	}

	void LambdaExpression::writereturntype( wostream& os ) const
	{
		if( ReturnType( ) )
		{
			os << L" -> ";
			( *ReturnType( ) ) -> Declarator( ).Name( L"" );
			( *ReturnType( ) ) -> writeasparameter( os );
		}
	}

	void LambdaExpression::writecapturelist( wostream& os , unsigned long tabs ) const
	{
		wstring tab( tabs , FormattingData::IndentationCharacter );
		CaptureCollection::const_iterator i = captures -> begin( );
		os << L" [ ";
		( *i ) -> write( os , tabs );
		for( ++i ; i != captures -> end( ) ; ++i )
		{
			os << L" , ";
			( *i ) -> write( os , tabs );
		}
		os << L" ]";
	}

	void LambdaExpression::writetext( wostream& os , unsigned long tabs ) const
	{
		Comment( ).write( os , tabs );
		if( !Comment( ).IsEmpty( ) )
			os << L'\n';

		writecapturelist( os , tabs );
		writeparameters( os );

		if( Mutable( ) )
			os << " mutable";

		writeexceptionspecs( os );
		writereturntype( os );
		writebody( os , tabs );
	}
}