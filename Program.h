//----------------------------------------------------------------------------------------
/**
 * \file       Program.h
 * \author     Patrik Schiller
 * \date       2019/21/05
 *
*/
//----------------------------------------------------------------------------------------
#pragma once
#include "pgr.h"
#include <iostream>
class Program
{
public:
	unsigned int programId;
	std::string programName;
	
	std::vector<std::pair<GLuint, std::string>> shaderList;
	Program();
	// File source
	Program(const char* vertexShaderPath, const char* fragmentShaderPath, std::string progName, const char* geometry = nullptr) {
		std::cout << "[SHADER] Started creation of Shader from File - " << progName << " \n";
		programName = progName;
		GLuint vertexShader = pgr::createShaderFromFile(GL_VERTEX_SHADER, vertexShaderPath);
		if (!vertexShader) { pgr::dieWithError("[SHADER]<File> Error in creating Vertex Shader "+progName+"\n"); }
		shaderList.push_back(std::pair<GLuint, std::string>(vertexShader, "VS"));

		GLuint fragmentShader = pgr::createShaderFromFile(GL_FRAGMENT_SHADER, fragmentShaderPath);
		if (!fragmentShader) { pgr::dieWithError("[SHADER]<File> Error in creating Fragment Shader " + progName + "\n"); }
		shaderList.push_back(std::pair<GLuint, std::string>(vertexShader, "FS"));

		if (!vertexShader || !fragmentShader) {
			pgr::dieWithError("[SHADER]<File> Error in creating shaders " + progName + "\n");
		}
		std::cout << "[SHADER] Shaders created\n";
		programInit(vertexShader, fragmentShader);
	}
	// String source
	Program(const std::string vertexShaderSrc, const std::string fragmentShaderSrc, std::string progName, const char* geometry = nullptr) {
		std::cout << "[SHADER] Started creation of Shader from Source - " << progName << " \n";
		programName = progName;
		GLuint vertexShader = pgr::createShaderFromSource(GL_VERTEX_SHADER, vertexShaderSrc);
		if (!vertexShader) { pgr::dieWithError("[SHADER]<File> Error in creating Vertex Shader " + progName + "\n"); }
		shaderList.push_back(std::pair<GLuint, std::string>(vertexShader, "VS"));

		GLuint fragmentShader = pgr::createShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSrc);
		if (!fragmentShader) { pgr::dieWithError("[SHADER]<File> Error in creating Fragment Shader " + progName + "\n"); }
		shaderList.push_back(std::pair<GLuint, std::string>(vertexShader, "FS"));

		if (!vertexShader || !fragmentShader) {
			pgr::dieWithError("[SHADER]<String> Error in creating shaders\n");
		}
		std::cout << "[SHADER] Shaders created\n";
		programInit(vertexShader, fragmentShader);
	}
	void use() {
		glUseProgram(programId);
	}
	~Program();
	void deleteShaders() {
		for (std::pair<GLuint, std::string> pair : shaderList) {
			glDeleteShader(pair.first);
			std::cout << "[SHADER] Shader " << pair.second << " deleted\n";
		}
	}
private:
	void programInit(GLuint VS, GLuint FS) {
		programId = glCreateProgram();
		glAttachShader(programId, VS);
		glAttachShader(programId, FS);
		glLinkProgram(programId);
		checkCompileErrors(programId, "PROGRAM");
	}
	void checkCompileErrors(GLuint shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};

