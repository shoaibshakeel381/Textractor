#include "qtcommon.h"
#include "translatewrapper.h"
#include "network.h"

extern const wchar_t* TRANSLATION_ERROR;
extern const char* SUGOI_HOST;
extern const char* SUGOI_PORT;

const char* TRANSLATION_PROVIDER = "Sugoi Offline Translate";
const char* GET_API_KEY_FROM = nullptr;
extern const QStringList languagesTo
{
   "English"
},
languagesFrom
{
	"Japanese"
};
extern const std::unordered_map<std::wstring, std::wstring> codes
{
	{ { L"Bulgarian" }, { L"BG" } },
	{ { L"Chinese" }, { L"ZH" } },
	{ { L"Chinese (Simplified)" }, { L"ZH" } },
	{ { L"Czech" }, { L"CS" } },
	{ { L"Danish" }, { L"DA" } },
	{ { L"Dutch" }, { L"NL" } },
	{ { L"English" }, { L"EN" } },
	{ { L"English (American)" }, { L"EN-US" } },
	{ { L"English (British)" }, { L"EN-GB" } },
	{ { L"Estonian" }, { L"ET" } },
	{ { L"Finnish" }, { L"FI" } },
	{ { L"French" }, { L"FR" } },
	{ { L"German" }, { L"DE" } },
	{ { L"Greek" }, { L"EL" } },
	{ { L"Hungarian" }, { L"HU" } },
	{ { L"Indonesian" }, { L"ID" } },
	{ { L"Italian" }, { L"IT" } },
	{ { L"Japanese" }, { L"JA" } },
	{ { L"Latvian" }, { L"LV" } },
	{ { L"Lithuanian" }, { L"LT" } },
	{ { L"Polish" }, { L"PL" } },
	{ { L"Portuguese" }, { L"PT" } },
	{ { L"Portuguese (Brazil)" }, { L"PT-BR" } },
	{ { L"Portuguese (Portugal)" }, { L"PT-PT" } },
	{ { L"Romanian" }, { L"RO" } },
	{ { L"Russian" }, { L"RU" } },
	{ { L"Slovak" }, { L"SK" } },
	{ { L"Slovenian" }, { L"SL" } },
	{ { L"Spanish" }, { L"ES" } },
	{ { L"Swedish" }, { L"SV" } },
	{ { L"Turkish" }, { L"TR" } },
	{ { L"?" }, { L"auto" } }
};

bool translateSelectedOnly = false, useRateLimiter = false, rateLimitSelected = false, useCache = true, useFilter = true;
int tokenCount = 30, rateLimitTimespan = 60000, maxSentenceSize = 10000;
std::wstring dontTranslateIfMatch = L"";
QString sugoiHost, sugoiPort;

BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			sugoiHost = settings.value(SUGOI_HOST).toString();
			if (sugoiHost.isEmpty()) sugoiHost = "localhost";
			auto sugoiHostEdit = new QLineEdit(sugoiHost);
			QObject::connect(sugoiHostEdit, &QLineEdit::textChanged, [sugoiHostEdit](QString newValue) { settings.setValue(SUGOI_HOST, sugoiHost =  newValue); });
			display->addRow(SUGOI_HOST, sugoiHostEdit);

			sugoiPort = settings.value(SUGOI_PORT).toString();
			if (sugoiPort.isEmpty()) sugoiPort = "14366";
			auto sugoiPortEdit = new QLineEdit(sugoiPort);
			sugoiPortEdit->setValidator(new QIntValidator(0, 99999));
			QObject::connect(sugoiPortEdit, &QLineEdit::textChanged, [sugoiPortEdit](QString newValue) { settings.setValue(SUGOI_PORT, sugoiPort =  newValue); });
			display->addRow(SUGOI_PORT, sugoiPortEdit);
		}
		break;
	}
	return TRUE;
}

std::pair<bool, std::wstring> Translate(const std::wstring& text, TranslationParam tlp)
{
	std::string cleanedText = WideStringToString(text);
	for (auto& ch : cleanedText) if (ch == '\n') ch = ' ';
	if (HttpRequest httpRequest{
		L"Mozilla/5.0 Textractor",
		sugoiHost.toStdWString().c_str(),
		L"POST",
		NULL,
		FormatString(R"({"content":"%s","message":"translate sentences"})", JSON::Escape(cleanedText)),
		L"Content-type: application/json",
		sugoiPort.toUInt(),
		NULL,
		0
		})
		if (auto translation = Copy(JSON::Parse(httpRequest.response).String())) return { true, std::regex_replace(translation.value(), std::wregex(L"<unk>"), L" ") };
		else return { false, FormatString(L"%s: %s", TRANSLATION_ERROR, httpRequest.response) };
	else return { false, FormatString(L"%s (code=%u)", TRANSLATION_ERROR, httpRequest.errorCode) };
}
