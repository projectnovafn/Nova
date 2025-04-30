#pragma once

#include "CoreTypes.h"

#ifdef VISUAL_ASSIST_HACK
#define VARARG_DECL( FuncRet, StaticFuncRet, Return, FuncName, Pure, FmtType, ExtraDecl, ExtraCall ) FuncRet FuncName( ExtraDecl FmtType Fmt, ... )
#define VARARG_BODY( FuncRet, FuncName, FmtType, ExtraDecl ) FuncRet FuncName( ExtraDecl FmtType Fmt, ... )
#endif


#define GET_VARARGS(msg, msgsize, len, lastarg, fmt) \
	{ \
		va_list ap; \
		va_start(ap, lastarg); \
		FCString::GetVarArgs(msg, msgsize, fmt, ap); \
		va_end(ap); \
	}
#define GET_VARARGS_WIDE(msg, msgsize, len, lastarg, fmt) \
	{ \
		va_list ap; \
		va_start(ap, lastarg); \
		FCStringWide::GetVarArgs(msg, msgsize, fmt, ap); \
		va_end(ap); \
	}
#define GET_VARARGS_ANSI(msg, msgsize, len, lastarg, fmt) \
	{ \
		va_list ap; \
		va_start(ap, lastarg); \
		FCStringAnsi::GetVarArgs(msg, msgsize, fmt, ap); \
		va_end(ap); \
	}
#define GET_VARARGS_RESULT(msg, msgsize, len, lastarg, fmt, result) \
	{ \
		va_list ap; \
		va_start(ap, lastarg); \
		result = FCString::GetVarArgs(msg, msgsize, fmt, ap); \
		if (result >= msgsize) \
		{ \
			result = -1; \
		} \
		va_end(ap); \
	}
#define GET_VARARGS_RESULT_WIDE(msg, msgsize, len, lastarg, fmt, result) \
	{ \
		va_list ap; \
		va_start(ap, lastarg); \
		result = FCStringWide::GetVarArgs(msg, msgsize, fmt, ap); \
		if (result >= msgsize) \
		{ \
			result = -1; \
		} \
		va_end(ap); \
	}
#define GET_VARARGS_RESULT_ANSI(msg, msgsize, len, lastarg, fmt, result) \
	{ \
		va_list ap; \
		va_start(ap, lastarg); \
		result = FCStringAnsi::GetVarArgs(msg, msgsize, fmt, ap); \
		if (result >= msgsize) \
		{ \
			result = -1; \
		} \
		va_end(ap); \
	}
#define GET_VARARGS_RESULT_UTF8(msg, msgsize, len, lastarg, fmt, result) \
	{ \
		va_list ap; \
		va_start(ap, lastarg); \
		result = FCStringUtf8::GetVarArgs(msg, msgsize, fmt, ap); \
		if (result >= msgsize) \
		{ \
			result = -1; \
		} \
		va_end(ap); \
	}

#define VARARG_EXTRA(...) __VA_ARGS__,
#define VARARG_NONE
#define VARARG_PURE =0