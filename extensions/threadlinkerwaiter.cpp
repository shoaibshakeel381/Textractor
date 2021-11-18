#include "qtcommon.h"
#include "extension.h"
#include "ui_threadlinkerwaiter.h"

extern const char* THRAD_MESSAGE_WAIT;
extern const char* THRAD_MESSAGE_WAIT_TITLE;

int nSentencesJoined = 0;
int delay = 0;
auto c_start = std::chrono::high_resolution_clock::now();
std::wstring sum_sentence(L"");
Settings settings;

class Window : public QDialog, Localizer
{
public:
	Window() : QDialog(nullptr, Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint)
	{
		ui.setupUi(this);

		settings.beginGroup("Thread Linker Message Waiter");
		ui.delayDurationLbl->setText(THRAD_MESSAGE_WAIT);
		char* settingLabel = "Wait Duration";
		auto value = settings.value(settingLabel, 0).toInt();
		ui.delayDuration->setValue(value);
		delay = value/1000;
		connect(ui.delayDuration, qOverload<int>(&QSpinBox::valueChanged), [settingLabel, &value](int newValue) {
			settings.setValue(settingLabel, value = newValue);
			delay = newValue/1000;
		});

		setWindowTitle(THRAD_MESSAGE_WAIT_TITLE);
		// setWindowFlags(Qt::Window);
		QMetaObject::invokeMethod(this, &QWidget::show, Qt::QueuedConnection);
	}

private:
	Ui::LinkWaiterWindow ui;
} window;

bool ProcessSentence(std::wstring& sentence, SentenceInfo sentenceInfo)
{
	if (sentenceInfo["current select"] && sentenceInfo["text number"] != 0)
	{
		std::chrono::duration<double> c_diff = std::chrono::high_resolution_clock::now() - c_start;
		if (c_diff.count() > delay)
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
