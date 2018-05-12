#ifndef UTILITY_HEADER
#define UTILITY_HEADER

#include "exceptions.h"

namespace CPlusPlusCodeProvider
{
	/// <summary>
	/// This class is a proxy pointer that copies pointee when it is copied. In other words it provides value semantics to a pointer.
	/// </summary>
	/// <requirements>Requires T to support a Duplicate function which returns a new copy of the object pointed to by.</requirements>
	template< typename T > class copying_pointer
	{
		T* ptr;
	public:
		/// <summary>
		/// Constructs a default copying_pointer.
		/// </summary>
		copying_pointer( )
		{
		}

		/// <summary>
		/// Constructs a copying_pointer that wraps the given pointer and owns it.
		/// </summary>
		/// <parameter name="p">Given pointer to wrap.</parameter>
		explicit copying_pointer( T* p )
			 : ptr( p )
		{
		}

		/// <summary>
		/// Constructs a copying_pointer that wraps the given pointer and owns it.
		/// </summary>
		/// <parameter name="p">Given pointer to wrap.</parameter>
		explicit copying_pointer( T& ref )
			: ptr( &ref )
		{
		}

		/// <summary>
		/// Initializes a copying_pointer with another copying_pointer.
		/// </summary>
		/// <parameter name="other>Other copying_pointer to initialize with.</parameter>
		/// <requirements>Requires U to support a Duplicate function which returns an object convertable to T.</requirements>
		template< typename U > copying_pointer( const copying_pointer< U >& other )
			: ptr( other.pointer( ) ? dynamic_cast< U*>( &( other -> Duplicate( ) ) ) : 0 )
		{
		}

		/// <summary>
		/// Initializes a copying_pointer with another copying_pointer.
		/// </summary>
		/// <parameter name="other" IsConst="true">Other copying_pointer to initialize with.</parameter>
		copying_pointer( const copying_pointer& other )
			: ptr( other.ptr ? static_cast< T* >( &( other -> Duplicate( ) ) ) : 0 )
		{
		}

		/// <summary>
		/// Assigns another copying_pointer to this one.
		/// </summary>
		/// <parameter name="other" IsConst="true">Other copying_pointer to assign to this one.
		/// <returns>This object.</returns>
		copying_pointer& operator = ( const copying_pointer& other )
		{
			if( this != &other )
				*this = other.ptr;
			return *this;
		}

		/// <summary>
		/// Assigns a naked pointer to this one.
		/// </summary>
		/// <parameter name="p">A pointer to assign to this one.</paramter>
		/// <returns>This object with internal pointer replaced with the given one.</returns>
		copying_pointer& operator = ( T* p )
		{
			if( !p )
			{
				delete ptr;
				ptr = 0;
				return *this;
			}
			if( p != ptr )
			{
				if( ptr )
					delete ptr;
				ptr = static_cast< T* >( &p -> Duplicate( ) );
			}
			return *this;
		}

		/// <summary>
		/// Allows this object to act as the pointed one.
		/// </summary>
		/// <returns>A reference to the object pointed to.</returns>
		T& operator*( )
		{
			return *ptr;
		}

		/// <summary>
		/// Allows this object to act as the pointed one.
		/// </summary>
		/// <IsConstant value="true"/>
		/// <returns>A constant reference to the object pointed to.</returns>
		const T& operator*( ) const
		{
			return *ptr;
		}

		/// <summary>
		/// Allows this object to act as the pointed one.
		/// </summary>
		/// <returns>A pointer to the object pointed to.</returns>
		T* operator -> ( )
		{
			return ptr;
		}

		/// <summary>
		/// Allows this object to act as the pointed one.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>A constant pointer to the object pointed to.</returns>
		const T* operator -> ( ) const
		{
			return ptr;
		}

		/// <summary>
		/// Releases the owned pointer.
		/// </summary>
		/// <returns>Previously owned pointer.</returns>
		T* release( )
		{
			T* temp = ptr;
			ptr = 0;
			return temp;
		}

		/// <summary>
		/// Returns the stored pointer.
		/// </summary>
		/// <IsConstMember value="true"/>
		/// <returns>Returns a constant pointer to owned object.</returns>
		const T* pointer( ) const
		{
			return ptr;
		}

		/// <summary>
		/// Returns the stored pointer.
		/// </summary>
		/// <returns>Returns the pointer to owned object.</returns>
		T* pointer( )
		{
			return ptr;
		}

		/// <summary>
		/// Deletes the owned pointer if it is non-null.
		/// </summary>
		~copying_pointer( )
		{
			if( ptr )
				delete ptr;
		}
	};

	/// <summary>
	/// Provides accessibility levels for member declarations
	/// </summary>
	enum AccessRestrictions
	{
		/// <summary>
		/// Marks that the member has public access.
		/// </summary>
		Public ,
		/// <summary>
		/// Marks that the memeber has protected access.
		/// </summary>
		Protected ,
		/// <summary>
		/// Marks that the member has private access.
		/// </summary>
		Private ,
		/// <summary>
		/// Marks that the memeber has default access, public in struct and private in class.
		/// </summary>
		Default
	};

	/// <summary>
	/// Function to downcast an object.
	/// </summary>
	/// <parameter name="object" IsConst="true">The object which will be cast to another type.</parameter>
	/// <exception ref="CodeObject::TypesNotSame">Thrown if dynamic_cast fails.</exception>
	/// <requirements>T must be a direct or indirect base class of U.</requirements>
	template< typename T , typename U > const T& cast( const U& object )
	{
		return dynamic_cast< const T& >( object );
	}

	/// <summary>
	/// Assigns a pointer to another one, possibly releasing the object pointed to by it.
	/// </summary>
	/// <parameter name="a">Pointer which will have the new value.</parameter>
	/// <parameter name="b">Pointer whose object will be copied.</parameter>
	/// <requirements>T must have an assignment operator that takes an object reference of type T.</requirements>
	template< typename T > void Assign( T* a , const T* b )
	{
		if( a && b )
			*a = *b;
		else if( a || b )
		{
			if( a )
			{
				delete a;
				a = 0;
			}
			else
				a = new T( *b );
		}
	}
}

#endif