#include "extension.h"

const int MAX_SENTENCES_JOINED = 6;
int nSentencesJoined = 0;
auto c_start = std::chrono::high_resolution_clock::now();
std::wstring sum_sentence(L"");

bool ProcessSentence(std::wstring& sentence, SentenceInfo sentenceInfo)
{
	if (sentenceInfo["current select"] && sentenceInfo["text number"] != 0)
	{
		std::chrono::duration<double> c_diff = std::chrono::high_resolution_clock::now() - c_start;
		if ( c_diff.count() > 0.5 || nSentencesJoined >= MAX_SENTENCES_JOINED )
		{
			//sum_sentence.clear();
			sum_sentence = sentence;
			nSentencesJoined = 1;
		} else {
			sum_sentence += L"\n" + sentence;
			nSentencesJoined++;
		}
		c_start = std::chrono::high_resolution_clock::now();
		sentence = sum_sentence;

		return true;
	}

	return false;
}
