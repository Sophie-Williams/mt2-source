# mt2-source
* [Русский](https://github.com/drunkwolfs/mt2-source#Русский)<br />
* [English](https://github.com/drunkwolfs/mt2-source#English)<br />

## Русский
Клиент подходит для 40k клиентов, ключи xTEA стандартные.
Добавлен Ликана.
Добавлена защита запуска. Запустить клиент можно только через патчер и лаунчер. Алгоритм будет описан позднее.
В директории `\client\source\` находятся исходный коды клиента и файл решения Microsoft Visual Studio 2008 SP1<br />
Результат компиляции можно найти в директории `\client\source\bin\`.<br />
В папке `\client\dll\` можно найти все необходимые dll для запуска клиента.
Из решения убрано почти все лишние, оставлено только 4 возможных конфигурации компиляции:
* Release - Рабочая версия, без отладочной информации.
* Distribute - ??
* Debug - Версия для отладки.
* VTune - Версия для анализа производительности.

Была протестирована только Release версия, если у вас проблемы с остальными конфигурациями - опишите ее как можно более подробно [тут](https://github.com/drunkwolfs/mt2-source/issues).<br />
Для компиляции вам потребуется Visual Studio 2008 и обновление Visual Studio 2008 SP1, скачать их можно например тут: http://nnm-club.me/forum/viewtopic.php?t=170398 
### Установка
Скачиваете и устанавливаете git https://git-scm.com/downloads<br />
В консоле выполняете:<br />
`cd ~
git clone git://github.com/drunkwolfs/mt2-source.git`
Файлы скачаются в вашу домашнюю папку/папку пользователя.
Откройте файл `/mt2-source/client/source/Metin2Client_VC90.sln` в Visual Studio 2008 SP1, нажмите Собрать.

## English
