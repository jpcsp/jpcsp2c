

class Sample : public HLEModule
{
public:
	static void pspSampleFoo();

	class pspSampleFooFunc : public HLEModuleFunction
	{
	public:
		pspSampleFooFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			pspSampleFoo();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.SampleModule.pspSampleFoo(processor);";
		}
	};

    virtual string getName();
    virtual void installModule(int version);
    virtual void uninstallModule(int version);

	
    //HLEModuleFunction *pspSampleFooFunction;

};