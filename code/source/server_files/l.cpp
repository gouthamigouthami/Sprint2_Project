#include<iostream>
using namespace std;
enum LogPriority {
        TraceP, DebugP, InfoP, WarnP, ErrorP, FatalP
};

class BasicLogger {
private:
        static LogPriority verbosity;

public:
        static void Log(LogPriority priority, const char* message) {
            if (priority >= verbosity) {
                    switch (priority) {
                    case TraceP: std::cout << "Trace: \t"; break;
                    case DebugP: std::cout << "Debug: \t"; break;
                    case InfoP: std::cout << "Info: \t"; break;
                    case WarnP: std::cout << "Warn: \t"; break;
                    case ErrorP: std::cout << "Error: \t"; break;
                    case FatalP: std::cout << "Fatal: \t"; break;
                    }

                    std::cout << message << "\n";
            }
        }
	static void SetVerbosity(LogPriority new_priority) {
        verbosity = new_priority;
}
};
LogPriority BasicLogger::verbosity = TraceP;
int main()
{
	 BasicLogger obje();
	//obje.Log();
}
