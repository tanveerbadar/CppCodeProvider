#ifndef EXCEPTION_HEADER
#define EXCEPTION_HEADER

#include <stdexcept>

namespace CPlusPlusCodeProvider
{
	/// <summary>
	/// An exception class thrown when an unimplemented function is called from the client code.
	/// </summary>
	class FunctionNotImplementedException : public std::exception
	{
	public:
		FunctionNotImplementedException( const char* str )
			: std::exception( str )
		{
		}
	};

	class NullPointerException : public std::runtime_error
	{
	public:
		NullPointerException( const char* str )
			: std::runtime_error( str )
		{
		}
	};

	class NotSupportedException : public std::runtime_error
	{
	public:
		NotSupportedException( const char* str )
			: std::runtime_error( str )
		{
		}
	};
}

#endif