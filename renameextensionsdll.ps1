foreach ($extension in @(
    "Bing Translate",
    "Copy to Clipboard",
    "DeepL Translate",
    "DevTools DeepL Translate",
    "DevTools Papago Translate",
    "DevTools Systran Translate",
    "Extra Newlines",
    "Extra Window",
    "Google Translate",
    "Lua",
    "Regex Filter",
    "Regex Replacer",
    "Regex Replacer Process Hooked",
    "Remove Repeated Characters",
    "Remove Repeated Leading Sentence",
    "Remove Repeated Phrases",
    "Remove Repeated Phrases 2",
    "Remove 30 Repeated Sentences",
    "Replacer",
    "Replacer Process Hooked",
    "Styler",
    "Thread Linker",
    "Yandex Translate"
))
{
    Copy-Item -Force -Recurse -Verbose -Destination "$extension.xdll" -Path "$extension.dll";
    Remove-Item  -Force -Verbose -Path "$extension.dll";
}