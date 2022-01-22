#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include<string>
#include<cstdlib>

#pragma warning(disable : 4996)

const char* logl_root = "E:/git/RTR_2020/OpenGL/Windows/Native/ProgrammablePipeline/00_Practice/01_Project/06_Integrated_Basic";

class FileSystem
{
private:
	typedef std::string(*Builder) (const std::string& path);
public:
	static std::string getPath(const std::string& path)
	{
		static std::string(*pathBuilder)(std::string const&) = getPathBuilder();
		return (*pathBuilder)(path);
	}

private:
	static std::string const& getRoot()
	{
		static char const* envRoot = getenv("LOGL_ROOT_PATH");
		static char const* givenRoot = (envRoot != nullptr ? envRoot : logl_root);
		static std::string root = (givenRoot != nullptr ? givenRoot : "");
		return root;
	}

	static Builder getPathBuilder()
	{
		if (getRoot() != "")
			return &FileSystem::getPathRelativeRoot;
		else
			return &FileSystem::getPathRelativeBinary;
	}

	static std::string getPathRelativeRoot(const std::string& path)
	{
		return getRoot() + std::string("/") + path;
	}
	
	static std::string getPathRelativeBinary(const std::string& path)
	{
		return "../../../" + path;
	}
};

#endif