#pragma once
#include <string.h>
#include "teardown/functions/memory.h"

class small_string
{
public:
	union
	{
		char* m_HeapBuffer;
		char m_StackBuffer[16] = { 0 };
	};

	small_string() {
		Teardown::malloc(sizeof(this));
		memset(this, 0, sizeof(this));
	};

	small_string(const char* cStr)
	{
		size_t lLenght = cStr ? strlen(cStr) : 0;

		if (lLenght == 0)
			return;

		char* cDest = m_StackBuffer;
		if (lLenght > 15)
		{
			cDest = static_cast<char*>(Teardown::malloc(lLenght + 1));
			m_HeapBuffer = cDest;
			m_StackBuffer[15] = 1;
		}

		memcpy(cDest, cStr, lLenght);
		cDest[lLenght] = 0;
	}

	~small_string() {
		if (m_StackBuffer[15])
			Teardown::free(m_HeapBuffer);
	}

	inline const char* c_str() { return m_StackBuffer[15] ? m_HeapBuffer : &m_StackBuffer[0]; }
	inline size_t len() { return m_StackBuffer[15] ? strlen(m_HeapBuffer) : strlen(m_StackBuffer); }

	static void* operator new(size_t size) { return Teardown::malloc(size); }
	static void operator delete(void* p) { Teardown::free(p); }
	static void operator delete(void* p, size_t) { Teardown::free(p); }

	void operator=(small_string ssSource) {
		size_t lSourceLen = ssSource.len();

		if (ssSource.m_StackBuffer[15])
		{
			this->m_HeapBuffer = static_cast<char*>(Teardown::malloc(ssSource.len() + 1));
			memcpy(this->m_HeapBuffer, ssSource.m_HeapBuffer, lSourceLen);
			this->m_HeapBuffer[lSourceLen] = 0;
			this->m_StackBuffer[15] = 1;
		}
		else
		{
			if (this->m_StackBuffer[15])
			{
				Teardown::free(this->m_HeapBuffer);
				this->m_StackBuffer[15] = 0;
			}
			memcpy(this->m_StackBuffer, ssSource.m_StackBuffer, lSourceLen);
			this->m_StackBuffer[lSourceLen] = 0;
		}
	}

	inline void resize(size_t lNewSize)
	{
		if (lNewSize < 15 && this->m_StackBuffer[15])
		{
			void* pOldHeapPtr = this->m_HeapBuffer;
			memcpy(this->m_StackBuffer, this->m_HeapBuffer, lNewSize);
			this->m_StackBuffer[15] = 0;
			Teardown::free(pOldHeapPtr);
			return;
		}

		if (!this->m_StackBuffer[15])
		{
			char* pNewHeapBuff = static_cast<char*>(Teardown::malloc(lNewSize));
			Teardown::memmove(pNewHeapBuff, this->m_StackBuffer, 15);
			this->m_HeapBuffer = pNewHeapBuff;
			this->m_StackBuffer[15] = 1;
		}
		else
		{
			this->m_HeapBuffer = (char*)Teardown::realloc(this->m_HeapBuffer, lNewSize);
			this->m_HeapBuffer[lNewSize] = 0;
		}
	}

	inline void concat(const char* cNewStr)
	{
		size_t lSelfLen = this->len();
		size_t lNewLen = strlen(cNewStr);

		size_t lTotalStrLen = lSelfLen + lNewLen;

		this->resize(lTotalStrLen);

		char* cDest = this->m_StackBuffer;
		if (lTotalStrLen > 15)
			cDest = this->m_HeapBuffer;

		Teardown::memmove(cDest + lSelfLen, (void*)cNewStr, lNewLen);
		cDest[lTotalStrLen] = 0;
	}
};