#ifndef _SWAP_TABLE_ENTRY_
#define _SWAP_TABLE_ENTRY_

struct SwapTableEntry
{
	SwapTableEntry(char s1, char s2)
	{
		this->s1 = s1;
		this->s2 = s2;
	}

	char s1, s2;
};

#endif