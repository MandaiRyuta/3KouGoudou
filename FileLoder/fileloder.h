#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>
#pragma warning ( disable : 4996 )

namespace FileLoder
{
	struct Information_in_Folder {
		std::string texture_name;
		std::string texture_extension[3];
		std::string shader_name;
		std::string shader_extension;
		std::string model_name;
		std::string model_extension;
	};

	class FileScanWrite
	{
	public:
		static void Search_in_forder(Information_in_Folder files) noexcept(false);
	};
}


