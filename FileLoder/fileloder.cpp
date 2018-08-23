#include "fileloder.h"

void FileLoder::FileScanWrite::Search_in_forder(Information_in_Folder files) noexcept(false)
{
	HANDLE hFind;
	WIN32_FIND_DATA win32fd[3];//defined at Windwos.h
	FILE* fp;

	static std::ofstream outputfile("Resource.h");
	std::vector<std::string> texture_infomation;
	int nCount = 0;
	std::string resource_filename = "Resource.h";
	fp = fopen(resource_filename.c_str(), "w");
	std::string TextureFileName = "";
	std::string search_name;
	/*
	１．フォルダー内に（.txt）が存在するか検索し、存在した場合はtexture_infomationへ,texture_test01.txtを格納
	２．TextureFileName へ　texture_infomationの情報を格納し　.txt（4回分forループで文字列を消去）
	outputfileでResource.hへ書き込みをさせている。
	*/
	outputfile << "#pragma once" << std::endl;
	outputfile << "#include <string>" << std::endl;
	outputfile << "namespace Texture {" << std::endl;

	/*テクスチャーフォルダ内を検索後,Resource.hへの書き込み*/
	search_name = files.texture_name + "\\*." + files.texture_extension[0];
	hFind = FindFirstFile(search_name.c_str(), &win32fd[0]);
	do {
		if (win32fd[0].dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			MessageBox(NULL, "Error", "Error", MB_OK);
		}
		else {
			nCount += 1;
			texture_infomation.push_back(win32fd[0].cFileName);
			//printf("%s\n", texture_infomation.back().c_str());
			TextureFileName = texture_infomation.back().c_str();
			for (int i = 0; i < 4; i++)
			{
				TextureFileName.pop_back();
			}
			outputfile << "const std::string " << TextureFileName.c_str() << " = " << "\"" << files.texture_name.c_str() << "/" << texture_infomation.back().c_str() << "\";" << std::endl;
		}
	} while (FindNextFile(hFind, &win32fd[0]));

	/*テクスチャーフォルダ内を検索後,Resource.hへの書き込み*/
	search_name = files.texture_name + "\\*." + files.texture_extension[1];
	hFind = FindFirstFile(search_name.c_str(), &win32fd[1]);

	do {
		if (win32fd[1].dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			MessageBox(NULL, "Error", "Error", MB_OK);
		}
		else {
			nCount += 1;
			texture_infomation.push_back(win32fd[1].cFileName);
			//printf("%s\n", texture_infomation.back().c_str());
			TextureFileName = texture_infomation.back().c_str();
			for (int i = 0; i < 4; i++)
			{
				TextureFileName.pop_back();
			}
			outputfile << "const std::string " << TextureFileName.c_str() << " = " << "\"" << files.texture_name.c_str() << "/" << texture_infomation.back().c_str() << "\";" << std::endl;
		}
	} while (FindNextFile(hFind, &win32fd[1]));


	/*テクスチャーフォルダ内を検索後,Resource.hへの書き込み*/
	search_name = files.texture_name + "\\*." + files.texture_extension[2];
	hFind = FindFirstFile(search_name.c_str(), &win32fd[2]);

	do {
		if (win32fd[2].dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			MessageBox(NULL, "Error", "Error", MB_OK);
		}
		else {
			nCount += 1;
			texture_infomation.push_back(win32fd[2].cFileName);
			//printf("%s\n", texture_infomation.back().c_str());
			TextureFileName = texture_infomation.back().c_str();
			for (int i = 0; i < 4; i++)
			{
				TextureFileName.pop_back();
			}
			outputfile << "const std::string " << TextureFileName.c_str() << " = " << "\"" << files.texture_name.c_str() << "/" << texture_infomation.back().c_str() << "\";" << std::endl;
		}
	} while (FindNextFile(hFind, &win32fd[2]));
	outputfile << "}" << std::endl;
	/*シェーダフォルダ内を検索後、Resource.hへの書き込み*/
	search_name = files.shader_name + "\\*." + files.shader_extension;
	std::vector<std::string> shader_infomation;
	std::string ShaderFileName = "";
	hFind = FindFirstFile(search_name.c_str(), &win32fd[0]);
	/*
	１．フォルダー内に（.txt）が存在するか検索し、存在した場合はshader_infomationへ,shader_test01.txtを格納
	２．ShaderFileName へ　shader_infomationの情報を格納し　.txt（4回分forループで文字列を消去）
	outputfileでResource.hへ書き込みをさせている。
	*/
	outputfile << "\nnamespace Shader {" << std::endl;
	do {
		if (win32fd[0].dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			MessageBox(NULL, "Error", "Error", MB_OK);
		}
		else {
			nCount += 1;
			shader_infomation.push_back(win32fd[0].cFileName);
			//printf("%s\n", shader_infomation.back().c_str());
			ShaderFileName = shader_infomation.back().c_str();
			for (int i = 0; i < 4; i++)
			{
				ShaderFileName.pop_back();
			}
			outputfile << "const std::string " << ShaderFileName.c_str() << " = " << "\"" << files.shader_name.c_str() << "/" << shader_infomation.back().c_str() << "\";" << std::endl;
		}
	} while (FindNextFile(hFind, &win32fd[0]));

	outputfile << "}" << std::endl;
	/*モデル情報検索後にResource.hへの書き込み*/
	std::vector<std::string> model_infomation;
	std::string ModelFileName = "";
	search_name = files.model_name + "\\*." + files.model_extension;
	hFind = FindFirstFile(search_name.c_str(), &win32fd[0]);
	/*
	１．フォルダー内に（.txt）が存在するか検索し、存在した場合はmodel_infomationへ,model_test01.txtを格納
	２．ModelFileName へ　model_infomationの情報を格納し　.txt（4回分forループで文字列を消去）
	outputfileでResource.hへ書き込みをさせている。
	*/
	outputfile << "\nnamespace Model {" << std::endl;
	do {
		if (win32fd[0].dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			MessageBox(NULL, "Error", "Error", MB_OK);
		}
		else {
			nCount += 1;
			model_infomation.push_back(win32fd[0].cFileName);
			//printf("%s\n", model_infomation.back().c_str());
			ModelFileName = model_infomation.back().c_str();
			for (int i = 0; i < 2; i++)
			{
				ModelFileName.pop_back();
			}
			outputfile << "const std::string " << ModelFileName.c_str() << " = " << "\"" << files.model_name.c_str() << "/" << model_infomation.back().c_str() << "\";" << std::endl;
		}
	} while (FindNextFile(hFind, &win32fd[0]));

	outputfile << "}" << std::endl;

	FindClose(hFind);
	fclose(fp);
}
int main() {
	FileLoder::Information_in_Folder files;
	files.texture_name = "texture";
	files.texture_extension[0] = "png";
	files.texture_extension[1] = "jpg";
	files.texture_extension[2] = "bmp";
	files.shader_name = "shader";
	files.shader_extension = "txt";
	files.model_name = "model";
	files.model_extension = "x";
	FileLoder::FileScanWrite::Search_in_forder(files);
	return 0;
}