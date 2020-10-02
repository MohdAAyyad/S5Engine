#include "Debugger.h"

//Initialize static variables
MessageType Debugger::currentSev = MessageType::TYPE_NONE;
std::string Debugger::outputName = "";

void Debugger::DebugInit()
{
	outputName = "EngineLog.txt";
	std::ofstream out;
	out.open(outputName.c_str(), std::ios::out); //Open the file and clear it
	out.close(); //Close the file
	currentSev = MessageType::TYPE_FATAL_ERROR;
}
void Debugger::SetSeverity(MessageType type_)
{
	currentSev = type_;
}
void Debugger::Info(const std::string& message_, const std::string& fileName_, const int line_)
{
	Log(MessageType::TYPE_INFO, "[Info]: " + message_, fileName_, line_);
}
void Debugger::Trace(const std::string& message_, const std::string& fileName_, const int line_)
{
	Log(MessageType::TYPE_TRACE, "[Trace]: " + message_, fileName_, line_);
}
void Debugger::Warning(const std::string& message_, const std::string& fileName_, const int line_)
{
	Log(MessageType::TYPE_WARNING, "[Warning]: " + message_, fileName_, line_);
}
void Debugger::Error(const std::string& message_, const std::string& fileName_, const int line_)
{
	Log(MessageType::TYPE_ERROR, "[Error]: " + message_, fileName_, line_);
}
void Debugger::Fatal(const std::string& message_, const std::string& fileName_, const int line_)
{
	Log(MessageType::TYPE_FATAL_ERROR, "[Fatal Error]: " + message_, fileName_, line_);
}
void Debugger::Log(const MessageType type_, const std::string& message_, const std::string& fileName_, const int line_)
{
	if (type_ <= currentSev && currentSev > MessageType::TYPE_NONE) //Only severities less or equal to the current sev can be written 
	{
		std::ofstream out;
		out.open(outputName.c_str(), std::ios::app | std::ios::out); //Append to the end of the file + write to the file
		out << message_ << " in: " << fileName_ << " on line: " << line_ << std::endl;
		out.flush(); //Flush the bit stream to the text file that was opened
		out.close();
	}
}