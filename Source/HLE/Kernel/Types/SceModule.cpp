/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r695)
 */

#include "Common.h"
#include "Emulator.h"
#include "Memory.h"
#include "Loader.h"
#include "SceModule.h"
#include "HLE/Kernel/Managers/SceUidManager.h"
#include "HLE/pspSysMem.h"


SceModule *SceModule::previousModule = NULL;
u32 SceModule::sceModuleAddressOffset = 0x08410000;

SceModule::SceModule()
	: psf(NULL), address(0), isFlashModule(false),
	fileFormat(FormatUnknown), loadAddressLow(0), loadAddressHigh(0),
	baseAddress(0), importFixupAttempts(0)
{
	memset((SceModule_t *)this, 0, sizeof(SceModule_t));
	for (u32 i = 0; i < 2; ++i) {
		initSection[i] = 0;
		finiSection[i] = 0;
		stubTextSection[i] = 0;
	}
}

SceModule::SceModule(const SceModule &other) {
	*this = other;
}

SceModule::SceModule(bool isFlashModule)
	: psf(NULL), isFlashModule(isFlashModule),
	loadAddressLow(0), loadAddressHigh(0),
	baseAddress(0), importFixupAttempts(0)
{
	memset((SceModule_t *)this, 0, sizeof(SceModule_t));
	modid = SceUidManager::getNewUid("SceModule");

	// Address this struct will be stored in PSP mem
	// TODO: This messes with loader "base address"
	// since the loader has new SceModule() right at the start,
	// and we'd rather not use smem_high since it will make
	// stack allocations "non-pretty"
	// address = pspSysMem::malloc(2, pspSysMem::PSP_SMEM_Low, size, 0);
	// pspSysMem::AddSysMemInfo(
	//   2, "ModuleMgr", pspSysMem.PSP_SMEM_Low, size, address);

	address = sceModuleAddressOffset;
	sceModuleAddressOffset += (sizeof(SceModule_t) + 63) & ~63;

	// link SceModule structs together
	if (previousModule != NULL)
		previousModule->next = address;
	previousModule = this;

	fileFormat = FormatUnknown;
	unresolvedImports.clear();
}

SceModule::~SceModule() {
	if (psf != NULL) {
		delete psf;
		psf = NULL;
	}
}

void SceModule::operator =(const SceModule &other) {
	memcpy((SceModule_t *)this, (SceModule_t *)&other, sizeof(SceModule_t));

	address = other.address;
	isFlashModule = other.isFlashModule;

	fileFormat = other.fileFormat;
	pspFileName = other.pspFileName;

	loadAddressLow = other.loadAddressLow;
	loadAddressHigh = other.loadAddressHigh;
	baseAddress = other.baseAddress;

	for (u32 i = 0; i < 2; ++i) {
		initSection[i] = other.initSection[i];
		finiSection[i] = other.finiSection[i];
		stubTextSection[i] = other.stubTextSection[i];
	}

	unresolvedImports = other.unresolvedImports;
	importFixupAttempts = other.importFixupAttempts;

	psf = NULL;
	if (other.psf != NULL) {
		psf = new PSF();
		*psf = *other.psf;
	}
}

void SceModule::free() {
	pspSysMem::free(address);
}

void SceModule::write(u32 address) {
	memcpy(Memory::getPointer(address), (SceModule_t *)this, sizeof(SceModule_t));
}

void SceModule::read(u32 address) {
	// TODO: implement
	Emulator::log.error("Unimplemented SceModule read.");
}

void SceModule::copy(const PSPModuleInfo &moduleInfo) {
	attribute = moduleInfo.getAttr();
	version = moduleInfo.getVersion();
	
	strcpy(modname, moduleInfo.getName());

	gp_value = moduleInfo.getGp();
	ent_top = moduleInfo.getExports();
	ent_size = moduleInfo.getExpEnd() - ent_top;
	stub_top = moduleInfo.getImports();
	stub_size = moduleInfo.getImpEnd() - stub_top;
}

PSF *SceModule::getPsf() const {
	return psf;
}

void SceModule::setPsf(const PSF &psf) {
	if (this->psf != NULL) {
		delete this->psf;
		this->psf = NULL;
	}
	this->psf = new PSF();
	*this->psf = psf;
}
