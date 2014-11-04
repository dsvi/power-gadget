#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "precomp.h"

#define EXCEPTION( exp ) Exception exp(  __FILE__, __LINE__ )
#define NESTED_EXCEPTION( exp, fromExp ) Exception exp( fromExp, __FILE__, __LINE__ )

#define THROW(msg) throw Exception( msg, __FILE__, __LINE__ )
#define THROW_NESTED(msg, exp) throw Exception( msg, __FILE__, __LINE__ )


class Exception : public std::exception
{
public:
	Exception() noexcept;
	Exception(const char *file, int line) noexcept;
	Exception(const char16_t* msg, const char *file, int line) noexcept;
	Exception(const QString msg, const char *file, int line) noexcept;
	/// '/n' will be added to separate different exception messages
	/// even though the exp is const it is not thread safe.
	Exception(const std::exception &exp, const char *file, int line) noexcept;
	/// '/n' will be added to separate different exception messages
	Exception(const char16_t* msg, std::exception &exp, const char *file, int line) noexcept;

	QString message();
	/// Service error code
	QString code();

	Exception &operator<<(si64 a) noexcept;
	Exception &operator<<(const QString &s) noexcept;
	Exception &operator<<(const char16_t *s) noexcept;

private:
	char        messageStor[8192];
	char        codeStor[2048];
	size_t      posInMsgStor = 0;
	size_t      msgStorEnd = DIM(messageStor);
	size_t      posInCodeStor = 0;
	/// signals overflow of the buffer
	bool        messageWasNotFullyFormed = false;
	bool        codeWasNotFullyFormed = false;

	/// should put into code store only from constructor!
	void        putInCodeStor(char c);
	void        putInCodeStor(int a);
	void        putInMsgStor(char c);
	void        glueMessage();
};

typedef Exception& (*EMPTR)(Exception&);

inline
Exception &operator<<(Exception &e, EMPTR m) noexcept {
	return m(e);
}

Exception &endl(Exception &);

QString Message( std::exception &e );


#endif // EXCEPTION_H
