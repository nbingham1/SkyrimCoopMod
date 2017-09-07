/*
	THIS FILE IS A PART OF THE SKYRIM DRAGON SCRIPT PROJECT	
				(C) Alexander Blade 2011
			http://Alexander.SannyBuilder.com
*/

#pragma once

#include "plugin.h"
#include <windows.h>

enum
{
	StackElSize = 4,
	StackMaxLen = 32
};

/*extern DWORD ___stack[MAX_STACK_LEN];
extern DWORD ___stackindex;
extern DWORD ___result;*/

class stack
{
private:
	DWORD ___stack[MAX_STACK_LEN];
	DWORD ___stackindex;
	DWORD ___result;

public:
	stack()
	{
		for (int i = 0; i < MAX_STACK_LEN; i++)
		{
			___stack[i] = 0;
			___stackindex = 0;
			___result = 0;
		}
	}
	~stack()
	{
	}

	inline void StackReset()
	{
		___stackindex = 0;
	}

	template <typename T>
	void __stdcall Push(T value) 
	{
		if (sizeof(T) > StackElSize)
		{
			// Each arg shud be 32 bits length
			MessageBoxA(0, "Native argument has a size greater than 32 bits", "Native call error", MB_ICONERROR);
			ExitProcess(0);
		}
		else if (sizeof(T) < StackElSize)
		{
			// Null current stack entry
			___stack[___stackindex] = 0;
		}
		//PrintDebug("Push called , ___stackindex = %d", ___stackindex);
		___stack[___stackindex] = *reinterpret_cast<DWORD *>(&value);
		___stackindex++;
	}

	inline void Call(char *clname, char *fname)
	{
		NativeCall(clname, fname, ___stackindex, ___stack, &___result);
	}

	template <typename T>
	inline T GetResult()
	{
		return *reinterpret_cast<T *>(&___result);
	}
};

class NativeInvoke
{
/*private:
	static inline void StackReset()
	{
		___stackindex = 0;
	}

	template <typename T>
	static void __stdcall Push(T value) 
	{
		if (sizeof(T) > StackElSize)
		{
			// Each arg shud be 32 bits length
			MessageBoxA(0, "Native argument has a size greater than 32 bits", "Native call error", MB_ICONERROR);
			ExitProcess(0);
		}
		else if (sizeof(T) < StackElSize)
		{
			// Null current stack entry
			___stack[___stackindex] = 0;
		}
		//PrintDebug("Push called , ___stackindex = %d", ___stackindex);
		___stack[___stackindex] = *reinterpret_cast<DWORD *>(&value);
		___stackindex++;
	}

	static inline void Call(char *clname, char *fname)
	{
		NativeCall(clname, fname, ___stackindex, ___stack, &___result);
	}

	template <typename T>
	static inline T GetResult()
	{
		return *reinterpret_cast<T *>(&___result);
	}*/

public:
	template <typename R>
	static inline R Invoke(char *clname, char *fname)
	{
		stack s;
		s.StackReset();
		s.Call(clname, fname);
		return s.GetResult<R>();
	}

	template <typename R, typename T1>
	static inline R Invoke(char *clname, char *fname, T1 p1)
	{
		stack s;
		s.StackReset();
		s.Push(p1);
		s.Call(clname, fname);
		return s.GetResult<R>();
	}

	template <typename R, typename T1, typename T2>
	static inline R Invoke(char *clname, char *fname, T1 p1, T2 p2)
	{
		stack s;
		s.StackReset();
		s.Push(p1);
		s.Push(p2);
		s.Call(clname, fname);
		return s.GetResult<R>();
	}

	template <typename R, typename T1, typename T2, typename T3>
	static inline R Invoke(char *clname, char *fname, T1 p1, T2 p2, T3 p3)
	{
		stack s;
		s.StackReset();
		s.Push(p1);
		s.Push(p2);
		s.Push(p3);
		s.Call(clname, fname);
		return s.GetResult<R>();
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4>
	static inline R Invoke(char *clname, char *fname, T1 p1, T2 p2, T3 p3, T4 p4)
	{
		stack s;
		s.StackReset();
		s.Push(p1);
		s.Push(p2);
		s.Push(p3);
		s.Push(p4);
		s.Call(clname, fname);
		return s.GetResult<R>();
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
	static inline R Invoke(char *clname, char *fname, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		stack s;
		s.StackReset();
		s.Push(p1);
		s.Push(p2);
		s.Push(p3);
		s.Push(p4);
		s.Push(p5);
		s.Call(clname, fname);
		return s.GetResult<R>();
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	static inline R Invoke(char *clname, char *fname, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
		stack s;
		s.StackReset();
		s.Push(p1);
		s.Push(p2);
		s.Push(p3);
		s.Push(p4);
		s.Push(p5);
		s.Push(p6);
		s.Call(clname, fname);
		return s.GetResult<R>();
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	static inline R Invoke(char *clname, char *fname, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
	{
		stack s;
		s.StackReset();
		s.Push(p1);
		s.Push(p2);
		s.Push(p3);
		s.Push(p4);
		s.Push(p5);
		s.Push(p6);
		s.Push(p7);
		s.Call(clname, fname);
		return s.GetResult<R>();
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	static inline R Invoke(char *clname, char *fname, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
	{
		stack s;
		s.StackReset();
		s.Push(p1);
		s.Push(p2);
		s.Push(p3);
		s.Push(p4);
		s.Push(p5);
		s.Push(p6);
		s.Push(p7);
		s.Push(p8);
		s.Call(clname, fname);
		return s.GetResult<R>();
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
	static inline R Invoke(char *clname, char *fname, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
	{
		stack s;
		s.StackReset();
		s.Push(p1);
		s.Push(p2);
		s.Push(p3);
		s.Push(p4);
		s.Push(p5);
		s.Push(p6);
		s.Push(p7);
		s.Push(p8);
		s.Push(p9);
		s.Call(clname, fname);
		return s.GetResult<R>();
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
	static inline R Invoke(char *clname, char *fname, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
	{
		stack s;
		s.StackReset();
		s.Push(p1);
		s.Push(p2);
		s.Push(p3);
		s.Push(p4);
		s.Push(p5);
		s.Push(p6);
		s.Push(p7);
		s.Push(p8);
		s.Push(p9);
		s.Push(p10);
		s.Call(clname, fname);
		return s.GetResult<R>();
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
	static inline R Invoke(char *clname, char *fname, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11)
	{
		stack s;
		s.StackReset();
		s.Push(p1);
		s.Push(p2);
		s.Push(p3);
		s.Push(p4);
		s.Push(p5);
		s.Push(p6);
		s.Push(p7);
		s.Push(p8);
		s.Push(p9);
		s.Push(p10);
		s.Push(p11);
		s.Call(clname, fname);
		return s.GetResult<R>();
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
	static inline R Invoke(char *clname, char *fname, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12)
	{
		stack s;
		s.StackReset();
		s.Push(p1);
		s.Push(p2);
		s.Push(p3);
		s.Push(p4);
		s.Push(p5);
		s.Push(p6);
		s.Push(p7);
		s.Push(p8);
		s.Push(p9);
		s.Push(p10);
		s.Push(p11);
		s.Push(p12);
		s.Call(clname, fname);
		return s.GetResult<R>();
	}

};

class ObscriptInvoke
{
public:
	static inline double Invoke(char *fname)
	{
		return ObscriptCall(fname, 0, 0, 0);
	}

	template <typename T1>
	static inline double Invoke(char *fname, T1 p1)
	{
		return ObscriptCall(fname, *reinterpret_cast<DWORD *>(&p1), 0, 0);
	}

	template <typename T1, typename T2>
	static inline double Invoke(char *fname, T1 p1, T2 p2)
	{
		return ObscriptCall(fname, *reinterpret_cast<DWORD *>(&p1), *reinterpret_cast<DWORD *>(&p2), 0);
	}

	template <typename T1, typename T2, typename T3>
	static inline double Invoke(char *fname, T1 p1, T2 p2, T3 p3)
	{
		return ObscriptCall(fname, *reinterpret_cast<DWORD *>(&p1), *reinterpret_cast<DWORD *>(&p2), *reinterpret_cast<DWORD *>(&p3));
	}
};