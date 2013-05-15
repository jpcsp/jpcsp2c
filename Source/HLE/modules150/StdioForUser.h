/*
*
*
*   
*    5/5/2011 -  synced with 913
*/

class StdioForUser : public HLEModule
{
public:
	static void sceKernelStdioRead();
	static void sceKernelStdioLseek();
	static void sceKernelStdioSendChar();
	static void sceKernelStdioWrite();
	static void sceKernelStdioClose();
	static void sceKernelStdioOpen();
	static void sceKernelStdin();
	static void sceKernelStdout();
	static void sceKernelStderr();
	class sceKernelStdioReadFunc : public HLEModuleFunction
	{
	public:
		sceKernelStdioReadFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceKernelStdioRead();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceKernelStdioReadFunc;";
		}
	};
	class sceKernelStdioLseekFunc  : public HLEModuleFunction
	{
	public:
		sceKernelStdioLseekFunc (string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceKernelStdioLseek();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceKernelStdioLseekFunc;";
		}
	};
	class sceKernelStdioSendCharFunc : public HLEModuleFunction
	{
	public:
		sceKernelStdioSendCharFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			 sceKernelStdioSendChar();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceKernelStdioSendCharFunc;";
		}
	};
	class sceKernelStdioWriteFunc : public HLEModuleFunction
	{
	public:
		sceKernelStdioWriteFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceKernelStdioWrite();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceKernelStdioReadFunc;";
		}
	};
    class sceKernelStdioCloseFunc : public HLEModuleFunction
	{
	public:
		sceKernelStdioCloseFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			 sceKernelStdioClose();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceKernelStdioCloseFunc;";
		}
	};
	class sceKernelStdioOpenFunc  : public HLEModuleFunction
	{
	public:
		sceKernelStdioOpenFunc (string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceKernelStdioOpen();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceKernelStdioOpenFunc;";
		}
	};
	class sceKernelStdinFunc  : public HLEModuleFunction
	{
	public:
		sceKernelStdinFunc (string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceKernelStdin();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceKernelStdinFunc;";
		}
	};

	class sceKernelStdoutFunc  : public HLEModuleFunction
	{
	public:
		sceKernelStdoutFunc (string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceKernelStdout();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceKernelStdoutFunc;";
		}
	};
	class sceKernelStderrFunc : public HLEModuleFunction
	{
	public:
		sceKernelStderrFunc  (string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceKernelStderr();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceKernelStderrFunc;";
		}
	};

    virtual string getName();
    virtual void installModule(int version);
    virtual void uninstallModule(int version);


};