/*
*
*
*   
*    5/5/2011 -  synced with 913
*/
class HLEModuleFunction
{
public:
    int syscallCode;
    string moduleName;
    string functionName;
    int nid;

    HLEModuleFunction(string moduleName, string functionName); 
    void setSyscallCode(int syscallCode); 
    int getSyscallCode();   
    string getModuleName();
    string getFunctionName(); 
    void setNid(int nid); 
    int getNid(); 
    virtual void execute()=0; 
    virtual string compiledString()=0;

};