/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r663)
 */

#pragma once

class PSPModuleInfo {
public:
	PSPModuleInfo();
	PSPModuleInfo(ifstream &f);
	PSPModuleInfo(u32 address);
	~PSPModuleInfo();

	void read(ifstream &f);
	void read(u32 address);

	u16 getAttr() const;
	u16 getVersion() const;
	const char *getName() const;
	u32 getGp() const;
	u32 getExports() const;
	u32 getExpEnd() const;
	u32 getImports() const;
	u32 getImpEnd() const;
	string getNamez() const;

private:
	string m_namez; // string version of m_name
	struct {
		u16  m_attr;
		u16  m_version;
		char m_name[28];
		u32  m_gp;
		u32  m_exports; // .lib.ent
		u32  m_exp_end;
		u32  m_imports; // .lib.stub
		u32  m_imp_end;
	} data;
};
