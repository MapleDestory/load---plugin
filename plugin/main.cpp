#include "main.h"

void GetDirectoryFiles(const fs::path& path, std::vector<HMODULE>* Modules)
{
	Modules->clear();
	WIN32_FIND_DATA Data;

	auto FindIndex = path.native().find(_T("*."));
	auto Directory = path.native().substr(0, FindIndex);

	auto hFindFile = ::FindFirstFile(path.native().c_str(), &Data);
	if (!hFindFile || hFindFile == INVALID_HANDLE_VALUE)
		throw std::runtime_error("Failed to directory scan.\n");
	else
	{
		do
		{
			const auto& Filename = Directory + Data.cFileName;
			if (fs::exists(Filename))
			{
				HMODULE hModule = ::LoadLibrary(Filename.c_str());
				if (!hModule)
				{
					auto filename = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(Data.cFileName);
					throw std::runtime_error("Load plugins error file: " + filename);
				}
				else
				{
					Modules->push_back(hModule);
				}
			}

		}while(::FindNextFile(hFindFile, &Data));
	}
	::FindClose(hFindFile);
}

auto main(int, TCHAR**) -> int
{
	std::vector<HMODULE> Modules;
	try
	{
		GetDirectoryFiles(R"(.\plugins\*.dll)", &Modules);

		for (auto hModule : Modules)
		{
			if (hModule)
			{
				FARPROC fpAddress = ::GetProcAddress(hModule, "SDK_Impl_Instance");
				if (fpAddress)
					reinterpret_cast<int(*)(const std::string&)>(fpAddress)("Hi...");
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return std::cin.get();
}