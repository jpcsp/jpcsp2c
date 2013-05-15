/*
 * 
 *  30/04/2011 - synced with 913
 *               (actual change was in r898)
 */


class PspGeList {
public:
	PspGeList(int list_addr, int stall_addr, int cbid, int arg_addr);
	void pushSignalCallback(int arg);
	void pushFinishCallback(int arg);

	int list_addr;
    int stall_addr;
    int cbid;
    int arg_addr;
    int context_addr; // pointer to 2k buffer for storing GE context, used as a paramater for the callbacks?

    int base;
    int pc;
    int stack[32];
    int stackIndex;
    int currentStatus;
    int syncStatus;
    int id;

    int thid; // the thread we are blocking

};