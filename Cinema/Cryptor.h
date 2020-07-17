#ifndef _CRYPTOR_
#define _CRYPTOR_

#include <vector>
#include "CSet.h"
#include "SwapTableEntry.h"

struct Cryptor
{
	Cryptor()
	{
		additionalCryptions = 0;
		reverse = false;
	}
	void addCharset(Charset set)
	{
		this->csets.push_back(set);
	}
	void clearCharsets()
	{
		csets.clear();
	}
	void setUseReverse(bool use)
	{
		reverse = use;
	}
	bool getUseReverse()
	{
		return reverse;
	}
	void setAdditionalCryptions(int i)
	{
		additionalCryptions = i;
	}
	int getAdditionalCryptions()
	{
		return additionalCryptions;
	}
	std::string encrypt(std::string text)		// Encrypts parameter "text"
	{
		std::vector<int> vShift;
		std::string result = "";			// REPLACE LATER (MAYBE)
		for (int i = 0; i < csets.size(); i++)
		{
			vShift.push_back(1);		// REPLACE LATER
		}

		int cryptions = 1 + additionalCryptions;

		while (cryptions > 0)
		{
			int textlength = text.length();
			result = "";
			for (int i = 0; i < textlength; i++)
			{
				char c = text.at(i);
				bool found = false;
				for (int isets = 0; isets < csets.size(); isets++)
				{
					int pos = csets.at(isets).set.find(c);
					if (pos != std::string::npos)
					{
						found = true;
						pos += vShift.at(isets);
						while (pos >= csets.at(isets).set.length())
							pos -= csets.at(isets).set.length();
						result = result + csets.at(isets).set.at(pos);
						vShift.at(isets) += 1;		// REPLACE LATER

						while (vShift.at(isets) >= csets.at(isets).set.length())
							vShift.at(isets) -= csets.at(isets).set.length();
						break;
					}
				}
				if (!found)
					result += c;
			}
			// Reverse
			if (reverse)
				result = std::string(result.rbegin(), result.rend());

			// Swap table
			for (int iswap = 0; iswap < swapTable.size(); iswap++)
			{
				for (int i = 0; i < result.length(); i++)
				{
					if (result.at(i) == swapTable.at(iswap).s1)
						result.at(i) = swapTable.at(iswap).s2;
					else if (result.at(i) == swapTable.at(iswap).s2)
						result.at(i) = swapTable.at(iswap).s1;
				}
			}

			// Clear shifts
			for (int i = 0; i < vShift.size(); i++)
			{
				vShift.at(i) = 1;		// REPLACE LATER
			}

			text = result;
			cryptions--;
		}

		return result;
	}


	std::string decrypt(std::string text)		// Decrypts parameter "text"
	{
		std::vector<int> vShift;
		std::string result = "";			// REPLACE LATER (MAYBE)
		for (int i = 0; i < csets.size(); i++)
		{
			vShift.push_back(0);		// REPLACE LATER
		}

		int cryptions = 1 + additionalCryptions;

		while (cryptions > 0)
		{
			// Reverse
			if (reverse)
				text = std::string(text.rbegin(), text.rend());

			result = text;

			// Swap table
			for (int iswap = swapTable.size() - 1; iswap >= 0; iswap--)
			{
				for (int i = 0; i < text.length(); i++)
				{
					if (text.at(i) == swapTable.at(iswap).s1)
						text.at(i) = swapTable.at(iswap).s2;
					else if (text.at(i) == swapTable.at(iswap).s2)
						text.at(i) = swapTable.at(iswap).s1;
				}
			}

			// Calculate shifts
			for (int i = 0; i < text.length(); i++)
			{
				for (int is = 0; is < vShift.size(); is++)
				{
					if (csets.at(is).set.find(text.at(i)) != std::string::npos)
						vShift.at(is)++;
				}
			}

			//result = text;
			for (int i = text.length() - 1; i >= 0; i--)
			{
				for (int isets = csets.size() - 1; isets >= 0; isets--)
				{
					int positionInCharset = csets.at(isets).set.find(text.at(i));
					if (positionInCharset != std::string::npos)
					{
						while (vShift.at(isets) < 0)
							vShift.at(isets) += csets.at(isets).set.length();

						int pos = positionInCharset - vShift.at(isets);
						while (pos < 0)
							pos += csets.at(isets).set.length();

						text.at(i) = csets.at(isets).set.at(pos);
						vShift.at(isets)--;
					}
				}
			}

			// Clear shifts
			for (int i = 0; i < vShift.size(); i++)
			{
				vShift.at(i) = 0;		// REPLACE LATER
			}

			result = text;
			cryptions--;
		}

		return result;
	}

	void addSwapTableEntry(SwapTableEntry entry)
	{
		swapTable.push_back(entry);
	}
	void clearSwapTable()
	{
		swapTable.clear();
	}

	std::vector<Charset> csets;
	std::vector<SwapTableEntry> swapTable;
	int additionalCryptions;
	bool reverse;

private:
	bool replace(std::string& str, const std::string& from, const std::string& to) {
		size_t start_pos = str.find(from);
		if (start_pos == std::string::npos)
			return false;
		str.replace(start_pos, from.length(), to);
		return true;
	}

};

#endif