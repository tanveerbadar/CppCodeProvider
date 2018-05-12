#ifndef COMMENTS_HEADER
#define COMMENTS_HEADER

#include <string>
#include "cppcodeprovider.h"

namespace CPlusPlusCodeProvider
{
	///// <summary>
	///// This class encapsulates a comment.
	///// </summary>
	//class Comment : public CodeObject
	//{
	//	mutable std::wstring text;
	//	const CodeObject* object;
	//	mutable bool multiline;
	//	mutable bool object_source;
	//public:
	//	/// <summary>
	//	/// Creates a new Comment given its contents.
	//	/// </summary>
	//	/// <parameter name="contents" IsConst="true">Contents that will be commented out.</parameter>
	//	/// <returns>A proxy object that behaves like a Comment.</returns>
	//	static copying_pointer< Comment > Instantiate( const std::wstring& contents = L"" )
	//	{
	//		return copying_pointer< Comment >( new Comment( contents ) );
	//	}

	//	/// <summary>
	//	/// Creates a new Comment from another code object, in essence writing it out in comments.
	//	/// </summary>
	//	/// <parameter name="object" IsConst="true">The CodeObject which will be commented.</parameter>
	//	/// <returns>A proxy object that behaves like a Comment.</returns>
	//	static copying_pointer< Comment > Instantiate( const CodeObject& object )
	//	{
	//		return copying_pointer< Comment >( new Comment( object ) );
	//	}

	//	/// <summary>
	//	/// Assigns another object to this Comment.
	//	/// </summary>
	//	/// <parameter name="object" IsConst="true">Given object to assign to.</parameter>
	//	/// <returns>This object with contents replaced by the given one.</returns>
	//	Comment& Assign( const CodeObject& object );
	//	/// <summary>
	//	/// Returns a new copy of this object.
	//	/// </summary>
	//	/// <IsConstMember value="true"/>
	//	/// <returns>A newly constructed Comment.</returns>
	//	Comment& Duplicate( ) const;

	//	/// <summary>
	//	/// This function allows you ot access the contents of this comment.
	//	/// </summary>
	//	/// <IsConstMember value="true"/>
	//	/// <returns>Returns the text contents of this object.</returns>
	//	std::wstring Text( ) const;

	//	/// <summary>
	//	/// Returns a value whether this Comment is single line comment or block comment.
	//	/// </summary>
	//	/// <IsConstMember value="true"/>
	//	/// <returns>True if this comment is a block comment, otherwise, false.</returns>
	//	bool MultiLine( ) const;
	//	/// <summary>
	//	/// Makes this comment a single line comment or block comment.
	//	/// </summary>
	//	/// <parameter name="flag">True to make this comment a single line comment, otherwise false for block comment.</parameter>
	//	Comment& MultiLine( bool flag );

	//	/// <summary>
	//	/// Tells whether this comment is empty or contains content.
	//	/// </summary>
	//	/// <IsConstMember value="true"/>
	//	/// <returns>True if this comment is empty.</returns>
	//	bool IsEmpty( ) const;
	//protected:
	//	/// <summary>
	//	/// This constructor constructs a Comment object given its contents.
	//	/// </summary>
	//	/// <parameter IsConst="true" name="content">Contents of the comment</parameter>
	//	Comment( const std::wstring& content );

	//	/// <summary>
	//	/// This constructor constructs a Comment object from another CodeObject.
	//	/// </summary>
	//	/// <parameter IsConst="true" name="object">Another CodeObject that will be written as comments.</parameter>
	//	Comment( const CodeObject& object );

	//	/// <summary>
	//	/// Writes out the contents to the provided stream.
	//	/// </summary>
	//	/// <parameter name="os">Stream which receives the output.</parameter>
	//	/// <parameter name="tabs">Indentation for this Comment</parameter>
	//	/// <IsConstMember value="true"/>
	//	void writetext( std::wostream& os , unsigned long tabs ) const;

	//	/// <summary>
	//	/// Writes out this object split across the two streams.
	//	/// </summary>
	//	/// <parameter name="declas">Declaration output stream.</parameter>
	//	/// <parameter name="defos">This parameter is not used.</parameter>
	//	/// <parameter name="tabs">Indentation for this Comment</parameter>
	//	/// <IsConstMember value="true"/>
	//	void writetext( std::wostream& declos , std::wostream& defos , unsigned long decltabs , unsigned long deftabs ) const;
	//};
}

#endif