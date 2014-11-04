#include "Exception.h"

using namespace std;

Exception::Exception() noexcept
{
}

Exception::Exception(const char *file, int line) noexcept
{
	{
		const char *delim = strrchr(file, '\\');
		if (delim)
			file = delim +1;
	}
	{
		const char *delim = strrchr(file, '/');
		if (delim)
			file = delim +1;
	}
	for (; *file; file++){
		if ( *file == '.' )
			break;
		putInCodeStor(*file);
	}
	putInCodeStor(':');
	putInCodeStor(line);
}

Exception::Exception(const char16_t *msg, const char *file, int line) noexcept : Exception(file, line)
{
	*this << msg;
}

Exception::Exception(const QString msg, const char *file, int line) noexcept
	: Exception(reinterpret_cast<const char16_t*> (msg.constData()), file, line)
{
	static_assert(sizeof(remove_pointer<decltype(msg.constData())>::type) == sizeof(char16_t), "different character types");
}

Exception::Exception(const std::exception &exp, const char *file, int line) noexcept : Exception(file, line)
{
	Exception *e = (Exception*) dynamic_cast<const Exception*>(&exp);
	if ( e == nullptr ){
		if ( dynamic_cast<const std::bad_alloc*>(&exp) != nullptr ){
			*this << QCoreApplication::translate("Exception", "Not enough memory to complete operation");
			return;
		}
		for (const char *c = exp.what(); *c; c++)
			putInMsgStor(*c);
		return;
	}
	{
		putInCodeStor('\n');
		if (e->posInCodeStor + posInCodeStor < DIM(codeStor)  && !e->codeWasNotFullyFormed  ){
			std::memcpy(codeStor+posInCodeStor, e->codeStor, e->posInCodeStor);
			posInCodeStor += e->posInCodeStor;
		}
		else{
			std::memcpy(codeStor, e->codeStor, e->posInCodeStor);
			posInCodeStor = e->posInCodeStor;
			codeWasNotFullyFormed = true;
		}
	}
	{// put e message to the end of the current stor
		e->glueMessage();
		msgStorEnd = DIM(messageStor) - e->posInMsgStor;
		std::memcpy(messageStor + msgStorEnd, e->messageStor, e->posInMsgStor);
		messageWasNotFullyFormed = e->messageWasNotFullyFormed;
	}
}

Exception::Exception(const char16_t *msg, std::exception &exp, const char *file, int line) noexcept
	:Exception(exp, file, line)
{
	*this << msg;
}

QString Exception::message()
{
	glueMessage();
	QString msg;
	if ( messageWasNotFullyFormed )
		msg += ">>";
	msg += QString::fromUtf8(messageStor, posInMsgStor);
	return msg;
}

QString Exception::code()
{
	QString msg;
	if ( codeWasNotFullyFormed )
		msg += "!\n";
	msg += QString::fromUtf8(codeStor, posInCodeStor);
	return msg;
}

Exception &Exception::operator <<(si64 a) noexcept
{
	if ( !messageWasNotFullyFormed ){
		char buff[30];
		#pragma clang diagnostic push
		#pragma clang diagnostic ignored "-Wformat"
		int num = snprintf( buff, DIM(buff), "%lld", a );
		#pragma clang diagnostic pop
		ASSERT(num > 0);
		if (num > 0 )
			for(char *c = buff; *c; c++)
				putInMsgStor(*c);
	}
	return *this;
}

Exception &Exception::operator <<(const QString &s) noexcept
{
	static_assert( sizeof( *s.constData() ) == sizeof(char16_t), "Expecting utf16" );
	*this << (const char16_t *) s.constData();
	return *this;
}

Exception &Exception::operator <<(const char16_t *s) noexcept
{
	if ( messageWasNotFullyFormed )
		return *this;

	mbstate_t mbs = std::mbstate_t();
	char buff[MB_CUR_MAX];

	for ( ; *s; s++ ){
		size_t length = c16rtomb(buff,*s,&mbs);
		if ((length==0) || (length>MB_CUR_MAX))
			continue;
		for (size_t i=0; i<length; ++i )
			putInMsgStor( buff[i] );
	}

	return *this;
}

void Exception::putInCodeStor(char c)
{
	if ( posInCodeStor < DIM(codeStor) )
		codeStor[posInCodeStor++] = c;
	else
		codeWasNotFullyFormed = true;
}

void Exception::putInCodeStor(si32 a)
{
	char buff[30];
	int ret = snprintf(buff, DIM(buff), "%d", a);
	ASSERT(ret > 0);
	(void) ret;
	for ( char *c = buff; *c; c++ )
		putInCodeStor(*c);
}

void Exception::putInMsgStor(char c)
{
	if ( posInMsgStor < msgStorEnd )
		messageStor[posInMsgStor++] = c;
	else
		messageWasNotFullyFormed = true;
}

void Exception::glueMessage()
{
	int tailSize = DIM(messageStor) - msgStorEnd;
	if (tailSize == 0)
		return;
	putInMsgStor('\n');
	if (messageWasNotFullyFormed && msgStorEnd != DIM(messageStor))
		posInMsgStor = 0;
	std::memcpy(messageStor + posInMsgStor, messageStor + msgStorEnd, tailSize);
	posInMsgStor += tailSize;
	msgStorEnd = DIM(messageStor);
}

Exception &endl( Exception &s ){
	return s << u"\n";
}

QString Message( std::exception &e ){
	Exception *exp = dynamic_cast<Exception*>(&e);
	if ( exp != nullptr )
		return exp->message() + "\n\n" + exp->code();
	return QString( e.what() );
}


