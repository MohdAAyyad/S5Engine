#include "ShaderHandler.h"

std::unique_ptr<ShaderHandler>ShaderHandler::shaderInstance = nullptr;
std::map<std::string, GLuint> ShaderHandler::programs = std::map<std::string, GLuint>();

ShaderHandler::ShaderHandler()
{

}

ShaderHandler::~ShaderHandler()
{
	OnDestroy();
}

void ShaderHandler::OnDestroy()
{

	/*
		std::map<std::string, GLuint>::iterator p;

	for (p = programs.begin(); p != programs.end(); p++)
	{
		glDeleteProgram(p->second);
	}
	
	*/
	if (programs.size() > 0)
	{
		for (auto p : programs)
		{
			glDeleteProgram(p.second); //Delete the glUnit of all the programs
		}
		programs.clear();
	}

}

GLuint ShaderHandler::GetShader(const std::string& shaderName_)
{
	if (programs.find(shaderName_) != programs.end()) //find uses an iterator. You check if you're at the end of the map
	{
		return programs[shaderName_];
	}
		return 0; //GLuint is unsigned duhh. Can't return -1
}

std::string ShaderHandler::ReadShader(const std::string& filePath_)
{
	std::string shaderCode = "";
	std::ifstream file;
	file.exceptions(std::ifstream::badbit);
	try {
		file.open(filePath_);
		std::stringstream tmpStream;
		tmpStream << file.rdbuf(); //rdbuf returns a pointer to the streambuffer of the file. << is overriden here and means: save the contents into tmpstream
		file.close();

		shaderCode = tmpStream.str(); //convert the string stream into an actual string

	}
	catch (std::ifstream::failure error_)
	{
		Debugger::Error("Could not read shader: " + filePath_, "ShaderHandler.cpp", __LINE__);
		return "";
	}

	return shaderCode;
}

GLuint  ShaderHandler::CreateShader(GLenum shaderType_, const std::string& source_, const std::string& shaderName_)
{
	GLint compileResult = 0;
	GLuint shader = glCreateShader(shaderType_);
	const char* shaderCodePtr = source_.c_str();
	const int shaderCodeSize = source_.size();

	//Load the shader

	glShaderSource(shader, 1, &shaderCodePtr, &shaderCodeSize); //1 is how many shaders we wanna create, and we only want 1
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

	if (!compileResult) //Catch anything that's not 1
	{
		GLint infoLogLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength); //Will fill in whatever the lenght of the info line
		std::vector<char> shaderLog(infoLogLength); //This will create objects based on the infoLogLenght. We didn't use Reserve cause Reserve wouldn't create objects
		glGetShaderInfoLog(shader, infoLogLength, NULL, &shaderLog[0]); //If sth wrong goes wrong, OpenGL will fill in the actual log information // If the length of the returned string is not required, a value of NULL can be passed in the length argument.
		std::string shaderString(shaderLog.begin(), shaderLog.end());
		Debugger::Error("Error compiling shader " + shaderName_ + ". Error: \n" + shaderString, "ShaderHandler.cpp", __LINE__);
		return 0;
	}

	return shader;
}


void ShaderHandler::CreateProgramGL(const std::string& shaderName_,
	const std::string& vertexShaderFileName_,
	const std::string& fragmentShaderFileName_)
{
	std::string vertexShaderCode = ReadShader(vertexShaderFileName_);
	std::string fragmentShaderCode = ReadShader(fragmentShaderFileName_);

	if (vertexShaderCode == "" || fragmentShaderCode == "")
	{
		return;
	}

	GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderCode, shaderName_);
	GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderCode, shaderName_);

	if (vertexShader == 0 || fragmentShader == 0)
	{
		return;
	}

	GLint linkResult = 0;
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &linkResult);

	if (!linkResult)
	{
		GLint infoLoglength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLoglength);
		std::vector<char>programLog(infoLoglength);
		glGetProgramInfoLog(program, infoLoglength, NULL, &programLog[0]);
		std::string programString(programLog.begin(), programLog.end());
		Debugger::Error("Failed to link shader " + shaderName_ + ". Error: " + programString, "ShaderHandler.cpp", __LINE__);

		//Even if the linking didn't work, opengl still created the shaders and the program, so we need to delete them
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(program);

		return;
	}

	programs[shaderName_] = program;
	//The program has been created successfully and it is now counted for. You can delete the shaders now.
	//We do not delete the program until the end
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

ShaderHandler* ShaderHandler::GetInstance()
{
	if (shaderInstance.get() == nullptr)
	{
		shaderInstance.reset(new ShaderHandler);
	}

	return shaderInstance.get();
}

