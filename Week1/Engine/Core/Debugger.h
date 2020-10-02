#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <iostream>
#include <string>
#include <fstream>


enum class MessageType : unsigned short
{
	TYPE_NONE,
	TYPE_FATAL_ERROR,
	TYPE_ERROR,
	TYPE_WARNING,
	TYPE_TRACE,
	TYPE_INFO
};

class Debugger
{
public:
	Debugger() = delete;
	Debugger(Debugger&&) = delete; //Disable the move constructor
	Debugger& operator=(const Debugger&) = delete; //Disable operator = for copy and move
	Debugger& operator=(Debugger&&) = delete;

	static void DebugInit();
	static void SetSeverity(MessageType type_);
	static void Info(const std::string& message_, const std::string& fileName_, const int line_);
	static void Trace(const std::string& message_, const std::string& fileName_, const int line_);
	static void Warning(const std::string& message_, const std::string& fileName_, const int line_);
	static void Error(const std::string& message_, const std::string& fileName_, const int line_);
	static void Fatal(const std::string& message_, const std::string& fileName_, const int line_);

private:

	static void Log(const MessageType type_, const std::string& message_, const std::string& fileName_, const int line_);

	static MessageType currentSev;
	static std::string outputName;
};

#endif // !DEBUGGER_H
