#pragma once
#include "teardown/functions/memory.h"

template<typename T>
class small_vector
{
public:
	small_vector() {};
	small_vector(uint32_t capacity) { reserve(capacity); };
	small_vector(const small_vector&) = delete;
	void operator=(const small_vector&) = delete;
	~small_vector() { clear(); }

	T* begin() const { return m_Data; }
	T* end() const { return m_Data + m_Size; }
	T* data() const { return m_Data; }
	uint32_t size() const { return m_Size; }
	uint32_t capacity() const { return m_Capacity; }

	void push_back(const T& value)
	{
		if (m_Capacity <= m_Size)
			reserve(2 * m_Capacity + 1);

		m_Data[m_Size] = value;
		++m_Size;
	}

	void reserve(uint32_t capacity)
	{
		if (auto memory = Teardown::alloc(capacity))
		{
			T* data = m_Data;
			uint32_t size = m_Size;

			if (data != nullptr)
			{
				if (size >= capacity)
					size = capacity;
				else
					memset(memory, sizeof(T) * size, sizeof(T) * capacity - size);

				memcpy(m_Data, data, sizeof(T) * size);
				Teardown::free(data);
				m_Size = size;
			}
			else
			{
				m_Size = 0;
				memset(memory, 0, sizeof(T) * capacity);
			}

			m_Data = memory;
			m_Capacity = capacity;
		}
	}

	void clear()
	{
		if (m_Data != nullptr)
		{
			m_Size = 0;
			m_Capacity = 0;
			Teardown::free(m_Data);
			m_Data = nullptr;
		}
	}

private:
	uint32_t m_Size = 0;
	uint32_t m_Capacity = 0;
	T* m_Data = nullptr;
};