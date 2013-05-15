/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r615)
 */

#include "Common.h"
#include "PSP.h"

PSP::PSP() {
}

PSP::PSP(ifstream &f) {
	f.read((char *)&data, sizeof(data));
}

PSP::~PSP() {
}

bool PSP::isValid() const {
	return data.e_magic == 0x5053507E;
}
