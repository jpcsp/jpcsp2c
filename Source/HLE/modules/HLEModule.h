/*
*
*
*   
*    5/5/2011 -  synced with 913
*/
class HLEModule
{
public:
    virtual string getName() =0;
    virtual void installModule(int version)=0;
    virtual void uninstallModule(int version)=0;
};