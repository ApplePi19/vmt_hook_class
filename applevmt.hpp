#pragma once
#include <Windows.h>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <memory>

class apple_vmt {
private:
	uintptr_t** table_ptr = nullptr;
	uintptr_t* original_tbl = nullptr;
	uintptr_t* new_tbl = nullptr;
	size_t estimated_funcs = 0;
	
	void estimate_length(std::uintptr_t* vftbl_start,std::size_t &length)
	{
		MEMORY_BASIC_INFORMATION meme_info = { 0 };
		do
		{
			length++;
			VirtualQuery(reinterpret_cast<const void*>(vftbl_start[length]), &meme_info, sizeof(meme_info));
		} while (meme_info.Protect == PAGE_EXECUTE_READ || meme_info.Protect == PAGE_EXECUTE_READWRITE);
	}
public:
	void init(void* class_ptr)
	{
		this->table_ptr = static_cast<uintptr_t**>(class_ptr);
		this->original_tbl = *this->table_ptr;
		this->estimate_length(this->original_tbl, this->estimated_funcs);
		this->new_tbl = new uintptr_t[this->estimated_funcs];
		memcpy(this->new_tbl, this->original_tbl, sizeof(uintptr_t)*this->estimated_funcs);
		*this->table_ptr = this->new_tbl;
	}

	inline void hook_func(void* function, size_t func_index)
	{
		this->new_tbl[func_index] = reinterpret_cast<uintptr_t>(function);
	}

	inline void unhook_func(size_t func_index)
	{
		this->new_tbl[func_index] = this->original_tbl[func_index];
	}

	template <typename t = void*>
	inline t get_original(std::size_t function_index)
	{
		return reinterpret_cast<t>(this->original_tbl[function_index]);
	}

	void restore()
	{
		*this->table_ptr = this->original_tbl;
		delete [] this->new_tbl;
	}
};
