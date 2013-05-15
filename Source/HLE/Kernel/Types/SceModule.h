/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r695)
 */

#pragma once

#include "Format/DeferredStub.h"
#include "Format/PSF.h"
#include "Format/PSPModuleInfo.h"

// PSP info
// http://psp.jim.sh/pspsdk-doc/structSceModule.html
struct SceModule_t {
	u32  next;        // should be handled by a manager
	u16  attribute;
	u16  version;
	char modname[28]; // 27 printable chars
	u32  unk0;
	u32  unk1;
	u32  modid;
	u32  unk2[4];
	u32  ent_top;     // we'll use bytes (instead of number of entries)
	u32  ent_size;
	u32  stub_top;
	u32  stub_size;   // we'll use bytes (instead of number of entries)
	u32  unk3[4];
	u32  entry_addr;
	u32  gp_value;
	u32  text_addr;
	u32  text_size;
	u32  data_size;
	u32  bss_size;
	u32  nsegment;   // ? maybe init/text/sceStub.text/fini
	u32  segmentaddr[4]; // ?
	u32  segmentsize[4]; // ?
};

// After initialising an instance please call write() at least once.
class SceModule : public SceModule_t {
public:
	SceModule();
	SceModule(const SceModule &other);
	SceModule(bool isFlashModule);
	~SceModule();

	void operator =(const SceModule &other);

	// for use when unloading modules
	void free();

	void write(u32 address);
	void read(u32 address);

	// Initialise ourself from a PSPModuleInfo object.
    // PSPModuleInfo object comes from the loader/ELF.
	void copy(const PSPModuleInfo &moduleInfo);

	PSF *getPsf() const;
	void setPsf(const PSF &psf);

	// internal info
	u32 address;
	bool isFlashModule;

	// loader stuff
	u32 fileFormat;     // see Loader class for valid formats
	string pspFileName; // boot path, for thread argument

	// the space consumed by the program image
	u32 loadAddressLow;
	u32 loadAddressHigh;
	u32 baseAddress; // should in theory be the same as loadAddressLow

	// address/size pairs, used by the debugger/instruction counter
	u32 initSection[2];
	u32 finiSection[2];
	u32 stubTextSection[2];

	// deferred import resolving
	list<DeferredStub> unresolvedImports;
	u32 importFixupAttempts;

private:
	static u32 sceModuleAddressOffset;
	static SceModule *previousModule;

	PSF *psf; // for xmb title, etc
};
