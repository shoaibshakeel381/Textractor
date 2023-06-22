#include "Extension.h"
#include <mutex>

bool ProcessSentence(std::wstring& sentence, SentenceInfo sentenceInfo)
{
	if (sentenceInfo["current select"] == 0 || sentenceInfo["text number"] == 0) return false;

	static std::mutex m;
	static std::wstring previous;

	std::wregex reg(L"\\s");
	sentence = std::regex_replace(sentence, reg, L""); //Removing all whitespace from the sentence, text does not always match otherwise

	m.lock(); //lock mutex before using previous to prevent race conditions
	size_t found = sentence.find(previous); //Searches for an exact match in order to avoid unwanted behaviour

	if (found != -1) //Case "sentence" matches "previous"
	{
		sentence.erase(0, previous.length());
		previous += sentence;
		m.unlock();
		return true;
	}
	else
	{
		previous = sentence;
		m.unlock();
		return true; //Returning true in order to keep the removal of the whitespace.
	} //Keeps all the sentences consistent, always not needed
}