/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r615)
 */

#pragma once

class PSP {
public:
	PSP();
	PSP(ifstream &f);
	~PSP();

	bool isValid() const;

private:
	struct {
		u32 e_magic;
	} data;
};
