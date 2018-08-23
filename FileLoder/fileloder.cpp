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
	�P�D�t�H���_�[���Ɂi.txt�j�����݂��邩�������A���݂����ꍇ��texture_infomation��,texture_test01.txt���i�[
	�Q�DTextureFileName �ց@texture_infomation�̏����i�[���@.txt�i4��for���[�v�ŕ�����������j
	outputfile��Resource.h�֏������݂������Ă���B
	*/
	outputfile << "#pragma once" << std::endl;
	outputfile << "#include <string>" << std::endl;
	outputfile << "namespace Texture {" << std::endl;

	/*�e�N�X�`���[�t�H���_����������,Resource.h�ւ̏�������*/
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

	/*�e�N�X�`���[�t�H���_����������,Resource.h�ւ̏�������*/
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


	/*�e�N�X�`���[�t�H���_����������,Resource.h�ւ̏�������*/
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
	/*�V�F�[�_�t�H���_����������AResource.h�ւ̏�������*/
	search_name = files.shader_name + "\\*." + files.shader_extension;
	std::vector<std::string> shader_infomation;
	std::string ShaderFileName = "";
	hFind = FindFirstFile(search_name.c_str(), &win32fd[0]);
	/*
	�P�D�t�H���_�[���Ɂi.txt�j�����݂��邩�������A���݂����ꍇ��shader_infomation��,shader_test01.txt���i�[
	�Q�DShaderFileName �ց@shader_infomation�̏����i�[���@.txt�i4��for���[�v�ŕ�����������j
	outputfile��Resource.h�֏������݂������Ă���B
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
	/*���f����񌟍����Resource.h�ւ̏�������*/
	std::vector<std::string> model_infomation;
	std::string ModelFileName = "";
	search_name = files.model_name + "\\*." + files.model_extension;
	hFind = FindFirstFile(search_name.c_str(), &win32fd[0]);
	/*
	�P�D�t�H���_�[���Ɂi.txt�j�����݂��邩�������A���݂����ꍇ��model_infomation��,model_test01.txt���i�[
	�Q�DModelFileName �ց@model_infomation�̏����i�[���@.txt�i4��for���[�v�ŕ�����������j
	outputfile��Resource.h�֏������݂������Ă���B
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