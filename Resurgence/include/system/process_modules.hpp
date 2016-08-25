#pragma once

#include <headers.hpp>
#include <vector>
#include "portable_executable.hpp"


//
// Injection flags
// 
#define INJECTION_TYPE_LOADLIBRARY  0x001
#define INJECTION_TYPE_LDRLOADLL    0x002

#define INJECTION_ERASE_HEADERS 0x001
#define INJECTION_HIDE_MODULE   0x002

namespace resurgence
{
	namespace system
	{
		class process;

		class process_module
		{
		public:
			process_module();
			process_module(process* proc, PLDR_DATA_TABLE_ENTRY entry);
			process_module(process* proc, PLDR_DATA_TABLE_ENTRY32 entry);
			process_module(process* proc, PRTL_PROCESS_MODULE_INFORMATION entry);

			const uint8_t*              get_base() const { return _base; }
			size_t                      get_size() const { return _size; }
			const std::wstring&         get_name() const { return _name; }
			const std::wstring&         get_path() const { return _path; }
			bool                        is_valid() const { return _base != nullptr; }

			const portable_executable&  get_pe();

			uintptr_t                   get_proc_address(const std::string& name);
		private:
			process*            _process;
			uint8_t*            _base;
			size_t              _size;
			std::wstring        _name;
			std::wstring        _path;
			portable_executable _pe;
		};
		
		class process_modules
		{
		public:
			process_modules(process* proc);

			std::vector<process_module> get_all_modules();
			process_module              get_main_module();
			process_module              get_module_by_name(const std::wstring& name);
			process_module              get_module_by_address(const std::uint8_t* address);
			process_module              get_module_by_load_order(uint32_t i);

			NTSTATUS                    inject_module(const std::wstring& path, uint32_t injectionType, uint32_t flags, process_module* module = nullptr);
		private:
			friend class process;
			process_modules();

			process* _process;
		};
	}
}
