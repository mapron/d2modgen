<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ru_RU">
<context>
    <name>D2ModGen::ConfigPageMergeMods</name>
    <message>
        <source>Overall order of data load is: D2 source from Main settings -&gt;
-&gt; Pre-gen data -&gt; modgen features -&gt; Post-gen data -&gt; output folder.</source>
        <translation type="vanished">Общий порядок загрузки источников: D2директория из Основных настроек -&gt;
-&gt; Пре-генерация -&gt; работа вкладок modgen -&gt; Пост-генерация -&gt; выходная директория.</translation>
    </message>
    <message>
        <source>You can add aditional steps to mod generation, prior or after the d2modgen work
Two additional type of sources are supported:
1. folder with .txt files
2. another D2R mod
After selecting source type, you need to specify conflict resolution, 
for the case additional source contains a file which already been generated.</source>
        <translation type="vanished">Вы можете добавить дополнительные этапы к генерации мода d2mogen, либо до либо после
Два типа источников доступны:
1. директория с txt файлами
2. другой D2R мод
После выбора типа, задайте способ разрешения конфликтов,
который будет использован в случае когда источник содержит файл который уже был сгенерирован.</translation>
    </message>
</context>
<context>
    <name>D2ModGen::ConfigPageMergeModsItem</name>
    <message>
        <source>CSV files folder</source>
        <translation type="vanished">Директория с файлами CSV</translation>
    </message>
    <message>
        <source>D2R mod</source>
        <translation type="vanished">Мод D2R</translation>
    </message>
    <message>
        <source>Replace - clear previous data</source>
        <translation type="vanished">Замена - очистить предыдущие данные, заполнить новые</translation>
    </message>
    <message>
        <source>Append - place all data at the end</source>
        <translation type="vanished">Добавить - поместить все данные из источника в конец</translation>
    </message>
    <message>
        <source>Update - overwrite records with same key</source>
        <translation type="vanished">Обновить - перезаписать только существующие строки</translation>
    </message>
    <message>
        <source>AppendNew - add non-existent data to the end</source>
        <translation type="vanished">Добавить новые - добавить те записи, которых еще нет в таблице</translation>
    </message>
    <message>
        <source>Merge - try Update, then AppendNew</source>
        <translation type="vanished">Слияние - попробовать Обновить, затем Добавить новый</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation type="vanished">Пропустить</translation>
    </message>
    <message>
        <source>Raise error</source>
        <translation type="vanished">Сообщение об ошибке</translation>
    </message>
    <message>
        <source>Mod name:</source>
        <translation type="vanished">Имя мода:</translation>
    </message>
    <message>
        <source>Folder with .txt files:</source>
        <translation type="vanished">Директория с .txt файлами:</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation type="vanished">Тип:</translation>
    </message>
    <message>
        <source>Action on conflict:</source>
        <translation type="vanished">Действие при конфликте:</translation>
    </message>
</context>
<context>
    <name>D2ModGen::MainConfigPage</name>
    <message>
        <source>Use Diablo II legacy installation</source>
        <translation type="vanished">Использовать установку Diablo II legacy</translation>
    </message>
    <message>
        <source>Add key to new char inventory (Basic mod test)</source>
        <translation type="vanished">Добавить ключи в инвентарь нового персонажа</translation>
    </message>
    <message>
        <source>Export all *.txt (for further manual edit)</source>
        <translation type="vanished">Экспортировать все .txt файлы (для дальнейшего редактирования)</translation>
    </message>
    <message>
        <source>Copy Settings.json</source>
        <translation type="vanished">Скопировать Settings.json</translation>
    </message>
    <message>
        <source>Set launch to mod</source>
        <translation type="vanished">Установить запуск BNet на мод</translation>
    </message>
    <message>
        <source>Reset launch to unmodded</source>
        <translation type="vanished">Сбросить запуск на немод.</translation>
    </message>
    <message>
        <source>Make shortcut on Desktop</source>
        <translation type="vanished">Создать ярлык на рабочем столе</translation>
    </message>
    <message>
        <source>Mod id:</source>
        <translation type="vanished">Идентификатор мода:</translation>
    </message>
    <message>
        <source>D2R path:</source>
        <translation type="vanished">Путь к D2R:</translation>
    </message>
    <message>
        <source>D2 legacy path:</source>
        <translation type="vanished">Путь к D2 Legacy:</translation>
    </message>
    <message>
        <source>D2R save and user settings root:</source>
        <translation type="vanished">D2R директория с сохранениями и настройками:</translation>
    </message>
    <message>
        <source>(this will copy default settings to mod folder)</source>
        <translation type="vanished">(это скопирует ваши настройки в новую папку мода)</translation>
    </message>
    <message>
        <source>D2R command arguments (read-only):</source>
        <translation type="vanished">D2R командная строка (только чтение):</translation>
    </message>
    <message>
        <source>(&lt;b&gt;Close Battlenet launcher before actions!&lt;/b&gt;)</source>
        <translation type="vanished">(&lt;b&gt;Закройте Battlenet перед этими действиями!&lt;/b&gt;)</translation>
    </message>
    <message>
        <source>Output directory (leave empty to output to D2R path):</source>
        <translation type="vanished">Выходная директория (оставьте пустой чтобы сохранить мод по пути D2R):</translation>
    </message>
    <message>
        <source>Main</source>
        <translation type="vanished">Главные настройки</translation>
    </message>
    <message>
        <source>This page allow you to select most important options:
-Where input and output data is (D2 installation - Resurrected or Legacy);
-Create new D2R save folder with global settings;
-Create desktop shortcut to launch mod.
However, there are some less important abilities: 
-Seed number (max 2^32) determines how random generation behaves. 
If seed is unchaged, then all generation will be the same every time 
You can edit seed manually or press F5 to generate new one.
-Add key to character inventory for fresh character - now you are sure you started modded version.</source>
        <translation type="vanished">Эта страница позволяет выставить самые важные опции:
-Где находятся входные и выходные данные (D2 установочная директория - Resurrected или  Legacy);
-Создать новую директорию для сохранений D2R с вашими настройками;
-Создать ярлык для запуска мода.
Но помимо главных есть и менее важные настройки: 
-Random seed (max 2^32) определяет повторяемость случайных операций. 
Если сид не меняется, то вся генерация даст идентичный результат при каждом вызове. 
Вы можете изменить сид сами либо нажать F5 чтобы сгенерировать новый.
-Добавить ключи в инвернарь нового персонажа - чтобы убедиться что вы и правда запустили мод. версию</translation>
    </message>
</context>
<context>
    <name>D2ModGen::MainWindow</name>
    <message>
        <source>Generate</source>
        <translation type="vanished">Генерировать</translation>
    </message>
    <message>
        <source>Status label.</source>
        <translation type="vanished">Статусная строка.</translation>
    </message>
    <message>
        <source>Reset to default</source>
        <translation type="vanished">Сброс на умолчания</translation>
    </message>
    <message>
        <source>Enable this tab</source>
        <translation type="vanished">Включить эту вкладку</translation>
    </message>
    <message>
        <source>Select preset...</source>
        <translation type="vanished">Выбрать пресет...</translation>
    </message>
    <message>
        <source>Do not know where to start? Select a preset:</source>
        <translation type="vanished">Не знаете откуда начать? Выберите пресет:</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="vanished">Файл</translation>
    </message>
    <message>
        <source>Actions</source>
        <translation type="vanished">Действия</translation>
    </message>
    <message>
        <source>About</source>
        <translation type="vanished">О программе</translation>
    </message>
    <message>
        <source>Save config...</source>
        <translation type="vanished">Сохранить конфиг...</translation>
    </message>
    <message>
        <source>Load config...</source>
        <translation type="vanished">Загрузить конфиг...</translation>
    </message>
    <message>
        <source>Clear config</source>
        <translation type="vanished">Очистить конфиг</translation>
    </message>
    <message>
        <source>Browse to settings folder</source>
        <translation type="vanished">Открыть папку с настройками</translation>
    </message>
    <message>
        <source>Quit without saving</source>
        <translation type="vanished">Выйти без сохранения</translation>
    </message>
    <message>
        <source>Save and quit</source>
        <translation type="vanished">Сохранить и выйти</translation>
    </message>
    <message>
        <source>Generate mod</source>
        <translation type="vanished">Генерация мода</translation>
    </message>
    <message>
        <source>Create seed</source>
        <translation type="vanished">Создать новый seed</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation type="vanished">Отменить</translation>
    </message>
    <message>
        <source>Theme</source>
        <translation type="vanished">Тема оформления</translation>
    </message>
    <message>
        <source>Language</source>
        <translation type="vanished">Язык</translation>
    </message>
    <message>
        <source>You need to restart for changes apply.</source>
        <translation type="vanished">Нужно перезагрузить для применения.</translation>
    </message>
    <message>
        <source>Start...</source>
        <translation type="vanished">Старт...</translation>
    </message>
    <message>
        <source>Mod &apos;%1&apos; successfully updated (%2).</source>
        <translation type="vanished">Мод &apos;%1&apos; успешно обновлен (%2).</translation>
    </message>
</context>
<context>
    <name>D2ModGen::UIController</name>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="123"/>
        <source>Start...</source>
        <translation>Старт...</translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="131"/>
        <source>Saved.</source>
        <translation>Сохранено.</translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="133"/>
        <source>Error when saving!</source>
        <translation>Ошибка при сохранении!</translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="144"/>
        <source>Loaded.</source>
        <translation>Загружено.</translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="146"/>
        <source>Error when loading!</source>
        <translation>Ошибка при загрузке!</translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="155"/>
        <source>Config is cleared.</source>
        <translation>Конфигурация очищена.</translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="251"/>
        <source>Error: %1</source>
        <translation>Ошика: %1</translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="256"/>
        <source>Mod &apos;%1&apos; is updated.</source>
        <translation>Мод &apos;%1&apos; обновлён.</translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="266"/>
        <source>Failed to locate Battle.net.config</source>
        <translation>Не найден Battle.net.config</translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="273"/>
        <source>Failed to read data from Battle.net.config</source>
        <translation>Не удалось прочитать Battle.net.config</translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="284"/>
        <source>Failed to write data to Battle.net.config</source>
        <translation>Не удалось записать Battle.net.config</translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="288"/>
        <source>Battle.net launch options updated</source>
        <translation>Battle.net параметры запуска обновлены</translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="304"/>
        <source>Failed to write shortcut</source>
        <translation>Не удалось записать ярлык</translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="308"/>
        <source>Shortcut created!</source>
        <translation>Ярлык создан!</translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="324"/>
        <source>Settings updated!</source>
        <translation>Настройки обновлены!</translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="343"/>
        <source>Save files copied (%1)</source>
        <translation>Файлы сохранений скопированы (%1 шт)</translation>
    </message>
    <message>
        <source>Mod &apos;%1&apos; successfully updated (%2).</source>
        <translation type="obsolete">Мод &apos;%1&apos; успешно обновлен (%2).</translation>
    </message>
</context>
<context>
    <name>D2ModGen::VersionChecker</name>
    <message>
        <location filename="../src/QtApp/VersionChecker.cpp" line="91"/>
        <source>checking...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/QtApp/VersionChecker.cpp" line="93"/>
        <source>error=</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MainConfigPage</name>
    <message>
        <source>Use Diablo II legacy installation</source>
        <translation type="obsolete">Использовать установку Diablo II legacy</translation>
    </message>
    <message>
        <source>Add key to new char inventory (Basic mod test)</source>
        <translation type="obsolete">Добавить ключи в инвентарь нового персонажа</translation>
    </message>
    <message>
        <source>Export all *.txt (for further manual edit)</source>
        <translation type="obsolete">Экспортировать все .txt файлы (для дальнейшего редактирования)</translation>
    </message>
    <message>
        <source>Copy Settings.json</source>
        <translation type="obsolete">Скопировать Settings.json</translation>
    </message>
    <message>
        <source>Set launch to mod</source>
        <translation type="obsolete">Установить запуск BNet на мод</translation>
    </message>
    <message>
        <source>Reset launch to unmodded</source>
        <translation type="obsolete">Сбросить запуск на немод.</translation>
    </message>
    <message>
        <source>Make shortcut on Desktop</source>
        <translation type="obsolete">Создать ярлык на рабочем столе</translation>
    </message>
    <message>
        <source>Mod id:</source>
        <translation type="obsolete">Идентификатор мода:</translation>
    </message>
    <message>
        <source>D2R path:</source>
        <translation type="obsolete">Путь к D2R:</translation>
    </message>
    <message>
        <source>D2 legacy path:</source>
        <translation type="obsolete">Путь к D2 Legacy:</translation>
    </message>
    <message>
        <source>D2R save and user settings root:</source>
        <translation type="obsolete">D2R директория с сохранениями и настройками:</translation>
    </message>
    <message>
        <source>(this will copy default settings to mod folder)</source>
        <translation type="obsolete">(это скопирует ваши настройки в новую папку мода)</translation>
    </message>
    <message>
        <source>D2R command arguments (read-only):</source>
        <translation type="obsolete">D2R командная строка (только чтение):</translation>
    </message>
    <message>
        <source>(&lt;b&gt;Close Battlenet launcher before actions!&lt;/b&gt;)</source>
        <translation type="obsolete">(&lt;b&gt;Закройте Battlenet перед этими действиями!&lt;/b&gt;)</translation>
    </message>
    <message>
        <source>Output directory (leave empty to output to D2R path):</source>
        <translation type="obsolete">Выходная директория (оставьте пустой чтобы сохранить мод по пути D2R):</translation>
    </message>
    <message>
        <source>Main</source>
        <translation type="obsolete">Главные настройки</translation>
    </message>
    <message>
        <source>This page allow you to select most important options:
-Where input and output data is (D2 installation - Resurrected or Legacy);
-Create new D2R save folder with global settings;
-Create desktop shortcut to launch mod.
However, there are some less important abilities: 
-Seed number (max 2^32) determines how random generation behaves. 
If seed is unchaged, then all generation will be the same every time 
You can edit seed manually or press F5 to generate new one.
-Add key to character inventory for fresh character - now you are sure you started modded version.</source>
        <translation type="obsolete">Эта страница позволяет выставить самые важные опции:
-Где находятся входные и выходные данные (D2 установочная директория - Resurrected или  Legacy);
-Создать новую директорию для сохранений D2R с вашими настройками;
-Создать ярлык для запуска мода.
Но помимо главных есть и менее важные настройки: 
-Random seed (max 2^32) определяет повторяемость случайных операций. 
Если сид не меняется, то вся генерация даст идентичный результат при каждом вызове. 
Вы можете изменить сид сами либо нажать F5 чтобы сгенерировать новый.
-Добавить ключи в инвернарь нового персонажа - чтобы убедиться что вы и правда запустили мод. версию</translation>
    </message>
</context>
<context>
    <name>MainMenuBar</name>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="54"/>
        <source>File</source>
        <translation>Файл</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="57"/>
        <source>Save config...</source>
        <translation>Сохранить конфиг...</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="62"/>
        <source>Load config...</source>
        <translation>Загрузить конфиг...</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="67"/>
        <source>Clear config</source>
        <translation>Очистить конфиг</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="73"/>
        <source>Config from preset</source>
        <translation>Конфиг из пресета</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="76"/>
        <source>Basic</source>
        <translation>Базовый</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="80"/>
        <source>Friendly chaos</source>
        <translation>Дружный хаос</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="84"/>
        <source>Magic rando</source>
        <translation>Магический рандом</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="88"/>
        <source>Oh, no!</source>
        <translation>О, нет!</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="93"/>
        <source>Browse to settings folder</source>
        <translation>Открыть папку с настройками</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="101"/>
        <source>Quit without saving</source>
        <translation>Выйти без сохранения</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="109"/>
        <source>Save and quit</source>
        <translation>Сохранить и выйти</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="139"/>
        <source>Actions</source>
        <translation>Действия</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="142"/>
        <source>Generate mod</source>
        <translation>Генерация мода</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="147"/>
        <source>New seed</source>
        <translation>Новый посев</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="152"/>
        <source>Undo</source>
        <translation>Отменить</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="157"/>
        <source>Theme</source>
        <translation>Тема оформления</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="162"/>
        <source>Dark</source>
        <translation>Тёмная</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="171"/>
        <source>Light</source>
        <translation>Светлая</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="180"/>
        <source>Language</source>
        <translation>Язык</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="184"/>
        <source>English</source>
        <translation>English</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="189"/>
        <source>Russian</source>
        <translation>Russian</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="194"/>
        <source>Korean</source>
        <translation>Korean</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="201"/>
        <source>Help</source>
        <translation>Помощь</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="203"/>
        <source>About</source>
        <translation>О программе</translation>
    </message>
</context>
<context>
    <name>MainNavigation</name>
    <message>
        <location filename="../qml/MainNavigation.qml" line="10"/>
        <source>Main</source>
        <translation>Главные настройки</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="16"/>
        <source>Extra</source>
        <translation>Дополнительно</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="22"/>
        <source>Randomizers</source>
        <translation>Рандомизация</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="28"/>
        <source>Item Randomizer</source>
        <translation>Рандомайзер предметов</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="35"/>
        <source>Monster Randomizer</source>
        <translation>Рандомайзер Монстров</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="42"/>
        <source>Skill Randomizer</source>
        <translation>Рандомайзер Навыков</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="49"/>
        <source>Make harder</source>
        <translation>Сделать сложней</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="55"/>
        <source>Monster stats</source>
        <translation>Параметры монстров</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="62"/>
        <source>Monster density</source>
        <translation>Плотность монстров</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="69"/>
        <source>Challenge</source>
        <translation>Челендж</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="76"/>
        <source>Make easier</source>
        <translation>Сделать легче</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="82"/>
        <source>Horadric Cube</source>
        <translation>Куб Хорадрика</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="89"/>
        <source>Gambling</source>
        <translation>Азартная игра</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="96"/>
        <source>Character</source>
        <translation>Персонаж</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="103"/>
        <source>Increase Item Drops</source>
        <translation>Больше предметов</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="110"/>
        <source>Increase Rune Drops</source>
        <translation>Больше рун</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="117"/>
        <source>Perfect Rolls</source>
        <translation>Идеальные параметры</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="124"/>
        <source>Requirements</source>
        <translation>Требования</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="131"/>
        <source>Misc</source>
        <translation>Прочее</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="137"/>
        <source>Quality of life</source>
        <translation>Упрощение жизни</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="144"/>
        <source>Drops filtering</source>
        <translation>Фильтр лута</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../qml/MainWindow.qml" line="9"/>
        <source>D2 mod generator by mapron</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/MainWindow.qml" line="124"/>
        <source>Check app updates</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/MainWindow.qml" line="143"/>
        <source>Status label: Ready</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../qml/MainWindow.qml" line="156"/>
        <source>Generate</source>
        <translation>Генерировать</translation>
    </message>
</context>
<context>
    <name>ModuleChallenge</name>
    <message>
        <location filename="../qml/Modules/ModuleChallenge.qml" line="9"/>
        <source>Challenge</source>
        <translation>Челендж</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleChallenge.qml" line="10"/>
        <source>First, you can disable potion drops entirely (that&apos;s not the same as drop filter).&lt;br&gt;Second, you can change resistance penalty for each difficulty from default 0/40/100.&lt;br&gt;And the last, you can adjust level area on all maps; &lt;br&gt;note that max value of 85 is still used (so set to 20 to basically make all Hell areas lvl 85).</source>
        <translation>Для начала, вы можете отключить дроп всех зелий (и это не тоже самое что фильтр - подобрать не сможете).&lt;br&gt;Далее вы можете изменить штраф сопротивлений персонажа с дефолтных 0/40/100.&lt;br&gt;Ну и в конце вы можете поднять уровень зон на всех картах;&lt;br&gt;примечание, 85 уровень является потолком, значения выше будут снижены до него.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleChallenge.qml" line="13"/>
        <source>Disable drop: All Health pots</source>
        <translation>Отключить дроп: Все лечебные зелья</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleChallenge.qml" line="20"/>
        <source>Disable drop: All Mana pots</source>
        <translation>Отключить дроп: Все зелья Маны</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleChallenge.qml" line="27"/>
        <source>Disable drop: Rejuv and Full Rejuv pots</source>
        <translation>Отключить дроп: Все зелья восстановления</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleChallenge.qml" line="34"/>
        <source>Normal difficulty resistance penalty, -all%</source>
        <translation>Штраф ко всем сопротивлениям, Normal, -all%</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleChallenge.qml" line="44"/>
        <source>Nightmare difficulty resistance penalty, -all%</source>
        <translation>Штраф ко всем сопротивлениям, Nightmare, -all%</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleChallenge.qml" line="54"/>
        <source>Hell difficulty resistance penalty, -all%</source>
        <translation>Штраф ко всем сопротивлениям, Hell, -all%</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleChallenge.qml" line="64"/>
        <source>Increase Nightmare area levels, +levels</source>
        <translation>Увеличить уровень зон на сложности Nightmare, +уровней</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleChallenge.qml" line="74"/>
        <source>Increase Hell area levels, +levels</source>
        <translation>Увеличить уровень зон на сложности Hell, +уровней</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleChallenge.qml" line="84"/>
        <source>Go beyond 85 level for areas</source>
        <translation>Позволить генерацию зон выше 85 уровня</translation>
    </message>
</context>
<context>
    <name>ModuleCharacter</name>
    <message>
        <location filename="../qml/Modules/ModuleCharacter.qml" line="9"/>
        <source>Character</source>
        <translation>Персонаж</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCharacter.qml" line="10"/>
        <source>First three options allow you to change starting items for characters: &lt;br&gt;add Cube, replace id scroll with Tome (useful for No-town runs), and replace health with mana potions. &lt;br&gt;Next two sliders allow you to change how many skill and stat poits you get on levelup. &lt;br&gt;Last slider allow you to lower Strength and Dexterity requirements on all items. </source>
        <translation>Первые опции позволяют менять предметы с которыми создается новый персонаж: &lt;br&gt;куб хорадрика, том идентификации и зелья в поясе.  &lt;br&gt;Дальше слайдеры позволяют изменить кол-во очков на левелапе. &lt;br&gt;Последняя опция позволяет снизить требования характеристик у всех базовых предметов. </translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCharacter.qml" line="13"/>
        <source>Add Horadric Cube as starting item</source>
        <translation>Добавить Horadric Cube стартовым предметом</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCharacter.qml" line="20"/>
        <source>Replace ID scroll with ID tome</source>
        <translation>Заменить стартовый свиток ID на том</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCharacter.qml" line="27"/>
        <source>Replace starting Health pots with Mana</source>
        <translation>Заменить стартовые зелья лечения на ману</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCharacter.qml" line="34"/>
        <source>Add Teleport as starting skill for all classes</source>
        <translation>Добавить Телепорт как стартовый навык</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCharacter.qml" line="41"/>
        <source>Stat points gain per level</source>
        <translation>Очков характеристик при повышении уровня</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCharacter.qml" line="51"/>
        <source>Skill points gain per level (D2R only!)</source>
        <translation>Очков умений при повышении уровня (только D2R!)</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCharacter.qml" line="61"/>
        <source>Maximum hard points for skill (D2R only!)</source>
        <translation>Максимум жестких очков умений для 1 навыка (только D2R!)</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCharacter.qml" line="71"/>
        <source>Change mercenaries Health, multiply by</source>
        <translation>Изменить Здоровье наёмников, множитель</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCharacter.qml" line="81"/>
        <source>Change mercenaries Damage, multiply by</source>
        <translation>Изменить Урон наёмников, множитель</translation>
    </message>
</context>
<context>
    <name>ModuleCube</name>
    <message>
        <location filename="../qml/Modules/ModuleCube.qml" line="9"/>
        <source>Horadric Cube</source>
        <translation>Куб Хорадрика</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCube.qml" line="10"/>
        <source>Create some recipes to cheese/cut corners around the game. &lt;br&gt;Input and results are self-explanatory.</source>
        <translation>Позволяет включить рецепты которые ускоряют прохождение игры.&lt;br&gt;Вход и результаты должны быть понятны.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCube.qml" line="13"/>
        <source>Remove gem component from Rune upgrade recipes (Rune x3,x2 =&gt; next Rune)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCube.qml" line="20"/>
        <source>Add quick portal access recipes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCube.qml" line="21"/>
        <source>1. TP book + Id scroll = Cow Portal&lt;br&gt;2. TP book + Id scroll x2 = Minor Uber Portal&lt;br&gt;3. TP book + Id scroll x3 = Uber Tristram Portal&lt;br&gt;4. TP book + Id scroll x4 = Colossal Ancients Portal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCube.qml" line="27"/>
        <source>Add quick quest recipes (Horadric Staff + Khalim Will)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCube.qml" line="28"/>
        <source>1. key + Id scroll = Horadric Staff&lt;br&gt;2. key + TP scroll = Khalim Will</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCube.qml" line="34"/>
        <source>Add socketing recipes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCube.qml" line="35"/>
        <source>1. Normal item + TP scroll x1 + Id scroll x1 = Add 3 sockets&lt;br&gt;2. Normal item + TP scroll x1 + Id scroll x2 = Add 4 sockets&lt;br&gt;3. Normal item + TP scroll x2 + Id scroll x1 = Add 5 sockets&lt;br&gt;4. Normal item + TP scroll x2 + Id scroll x2 = Add 6 sockets&lt;br&gt;5. Unique/Set/Rare item + TP scroll x1 + Id scroll x1 = Add 1 socket&lt;br&gt;6. Socketed item + TP scroll x1 = Clear sockets</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCube.qml" line="41"/>
        <source>Add quality upgrade recipes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCube.qml" line="42"/>
        <source>1. Normal item (normal,magic,rare,unique,set) + Antidote = Exceptional item&lt;br&gt;2. Exceptional item (normal,magic,rare,unique,set) + Antidote = Elite item&lt;br&gt;3. Magic (magic,rare,unique,set) item  + Stamina = Normal item of same type</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCube.qml" line="48"/>
        <source>Add cheat Small Charm recipies</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCube.qml" line="49"/>
        <source>Any small charm + one of (TP scroll x1, Id scroll x1 , key x1) + one of (h pot, mana pot, stamina, antidote, thawing) = crafted char (see below all recipes)&lt;br&gt;Just two charms of each kind suppose to provide reasonable endgame stat.&lt;br&gt;Item level is 1, so try several times to craft charm with 0 requirements.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCube.qml" line="75"/>
        <source>Remove strict item types for Crafted</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCube.qml" line="76"/>
        <source>Now any kind of gloves will be suitable, not specific type of gloves.&lt;br&gt;Same for helm, shield, weapon.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCube.qml" line="82"/>
        <source>Remove Rune/Jewel for Crafted</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCube.qml" line="83"/>
        <source>So any craft recipe is &apos;item + gem&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remove gem component from Rune upgrade recipes (Rune x3,x2 =&amp;gt; next Rune)</source>
        <translation type="obsolete">Убрать требования дракоценных камней для рецептов апгрейда рун (для апгрейда достаточно 2 или 3 руны)</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCube.qml" line="71"/>
        <source>Simplyfy crafting:</source>
        <translation type="unfinished">Упростить рецепты для крафта:</translation>
    </message>
    <message>
        <source>Remove strict item types for Crafted (any helm, any gloves etc)</source>
        <translation type="obsolete">Убрать жесткие требования по типу предмета (=любой шлем, любые перчатки и тд)</translation>
    </message>
    <message>
        <source>Remove Rune/Jewel requirement for Crafted&lt;br&gt;(So any recipe is &apos;item + gem&apos;)</source>
        <translation type="obsolete">Убрать необходимость рун и драгоценностей для крафта&lt;br&gt;(т.о. достаточно предмета и камня)</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleCube.qml" line="89"/>
        <source>Make Item Level always 99</source>
        <translation type="unfinished">Генерировать item level равный 99</translation>
    </message>
</context>
<context>
    <name>ModuleDropFiltering</name>
    <message>
        <location filename="../qml/Modules/ModuleDropFiltering.qml" line="9"/>
        <source>Drops filtering</source>
        <translation>Фильтр лута</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleDropFiltering.qml" line="10"/>
        <source>This tab consists of two sections:&lt;br&gt;1. Make some items have compact names, like &amp;quot;!MP5&amp;quot; for Super Mana Potion. &lt;br&gt;2. Hide item labels on the ground (you still be able to pick them). </source>
        <translation>Здесь имеются 2 основные возможности:&lt;br&gt;1. Сократить имена для предметов,&amp;quot;!MP5&amp;quot; для наилучшего зелья Маны.&lt;br&gt;2. Скрыть предметы на земле полностью. </translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleDropFiltering.qml" line="13"/>
        <source>&lt;b&gt;Make item names compact&lt;/b&gt;: this will make item names take less space, e.g. &apos;!HP2&apos; for health potion.</source>
        <translation>&lt;b&gt;Сократить названия предметов&lt;/b&gt;: сократит надписи, например &amp;apos;!HP2&amp;apos; для зелья лечения.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleDropFiltering.qml" line="17"/>
        <source>Compact potion names</source>
        <translation>Сократить надписи зелий</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleDropFiltering.qml" line="24"/>
        <source>Compact TP/ID scrolls</source>
        <translation>Сократить TP/ID свитки</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleDropFiltering.qml" line="31"/>
        <source>&lt;b&gt;Hide items on the ground&lt;/b&gt;: this will make item names transparent; &lt;br&gt;you still can pickup them, but their labels will be invisible on Alt press.</source>
        <translation>&lt;b&gt;Скрыть предметы&lt;/b&gt;: надписи становятся прозрачными; &lt;br&gt; вы сможете их подобрать, но надписи невидимы при нажатии Alt.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleDropFiltering.qml" line="31"/>
        <source>&lt;br&gt;&lt;b&gt;With D2R: RotW - just use game loot filter! You don&apos;t need those anymore!&lt;b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleDropFiltering.qml" line="38"/>
        <source>ID scroll</source>
        <translation>ID свиток</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleDropFiltering.qml" line="45"/>
        <source>TP scroll</source>
        <translation>TP свиток</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleDropFiltering.qml" line="52"/>
        <source>Health pots 1-3</source>
        <translation>Зелья лечения 1-3</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleDropFiltering.qml" line="59"/>
        <source>All Health pots</source>
        <translation>Все лечебные зелья</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleDropFiltering.qml" line="66"/>
        <source>Mana pots 1-3</source>
        <translation>Зелья маны 1-3</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleDropFiltering.qml" line="73"/>
        <source>All Mana pots</source>
        <translation>Все зелья маны</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleDropFiltering.qml" line="81"/>
        <source>Rejuv pots</source>
        <translation>Зелье восстановления</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleDropFiltering.qml" line="88"/>
        <source>Full Rejuv pots</source>
        <translation>Зелье полного восстановления</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleDropFiltering.qml" line="94"/>
        <source>Bolts/Arrows</source>
        <translation>Колчаны</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleDropFiltering.qml" line="101"/>
        <source>Stamina/Antidote/Thawing</source>
        <translation>Зелья выносливости/противояд/разморозка</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleDropFiltering.qml" line="108"/>
        <source>Keys,Fire/Poison pots</source>
        <translation>Ключи,метательные зелья</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleDropFiltering.qml" line="115"/>
        <source>Hide low quality/damaged/cracked items</source>
        <translation>Скрыть низкокачественные/поврежденные предметы</translation>
    </message>
</context>
<context>
    <name>ModuleGambling</name>
    <message>
        <location filename="../qml/Modules/ModuleGambling.qml" line="9"/>
        <source>Gambling</source>
        <translation>Азартная игра</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleGambling.qml" line="10"/>
        <source>First option enables Charms in Gambling window, but only for D2 Legacy.&lt;br&gt;Next sliders allow you increase basic chance of getting Unique/Rare/Set/High-quality item.</source>
        <translation>Можно включить выигрыш талисманов в окне азартной игры, но только для D2 Legacy</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleGambling.qml" line="13"/>
        <source>Allow gambling for charms and jewels (Legacy only)&lt;br&gt;Note: crashes D2R - so enabled only for D2.</source>
        <translation>Позволяет получить талисманы в азартной игре (только D2)&lt;br&gt;Т.к. D2R пока крашится, включено только для Legacy.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleGambling.qml" line="20"/>
        <source>Increase Unique chance, times&lt;br&gt;Note: you still can get a lot of failed uniques for no known reason.</source>
        <translation>Повысить шанс Уникальных, разы&lt;br&gt;Примечание: неточно, вы иногда будете получать &amp;apos;failed unique&amp;apos;</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleGambling.qml" line="30"/>
        <source>Increase Set chance, times</source>
        <translation>Увеличить шанс наборных предметов, разы</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleGambling.qml" line="40"/>
        <source>Increase Rare chance, times</source>
        <translation>Увеличить шансы на Редкие предметы, разы</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleGambling.qml" line="50"/>
        <source>Increase Exceptional upgrade chance, times</source>
        <translation>Увеличить шанс апгреда до Exceptional качества, разы</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleGambling.qml" line="60"/>
        <source>Increase Elite upgrade chance, times&lt;br&gt;Note: seems like Exc. check is done beforehead, so if it&amp;apos;s high enough, elite won&amp;apos;t generate.</source>
        <translation>Увеличить шанс апгреда до Elite качества, разы&lt;br&gt;Прим: похоже что проверка на Exceptional делается до этой.</translation>
    </message>
    <message>
        <source>Horadric Cube</source>
        <translation type="vanished">Куб Хорадрика</translation>
    </message>
    <message>
        <source>Create some recipes to cheese/cut corners around the game. &lt;br&gt;Input and results are self-explanatory.</source>
        <translation type="vanished">Позволяет включить рецепты которые ускоряют прохождение игры.&lt;br&gt;Вход и результаты должны быть понятны.</translation>
    </message>
    <message>
        <source>Remove gem component from Rune upgrade recipes (Rune x3,x2 =&amp;gt; next Rune)</source>
        <translation type="vanished">Убрать требования дракоценных камней для рецептов апгрейда рун (для апгрейда достаточно 2 или 3 руны)</translation>
    </message>
    <message>
        <source>Add quick portal access recipes:</source>
        <translation type="vanished">Добавить рецепты создания порталов для быстрого доступа:</translation>
    </message>
    <message>
        <source>Add quick quest recipes:</source>
        <translation type="vanished">Рецепты для быстрого получения квестовых предметов:</translation>
    </message>
    <message>
        <source>Add socketing recipes:</source>
        <translation type="vanished">Добавить рецепты для получения точного числа сокетов:</translation>
    </message>
    <message>
        <source>Add upgrade recipes:</source>
        <translation type="vanished">Добавить рецепты на апгрейд предметов:</translation>
    </message>
    <message>
        <source>Add cheat Small Charm recipies:</source>
        <translation type="vanished">Добавить читерские рецепты на Small Charm:</translation>
    </message>
    <message>
        <source>Simplyfy crafting:</source>
        <translation type="vanished">Упростить рецепты для крафта:</translation>
    </message>
    <message>
        <source>Remove strict item types for Crafted (any helm, any gloves etc)</source>
        <translation type="vanished">Убрать жесткие требования по типу предмета (=любой шлем, любые перчатки и тд)</translation>
    </message>
    <message>
        <source>Remove Rune/Jewel requirement for Crafted&lt;br&gt;(So any recipe is &apos;item + gem&apos;)</source>
        <translation type="vanished">Убрать необходимость рун и драгоценностей для крафта&lt;br&gt;(т.о. достаточно предмета и камня)</translation>
    </message>
    <message>
        <source>Make Item Level always 99</source>
        <translation type="vanished">Генерировать item level равный 99</translation>
    </message>
</context>
<context>
    <name>ModuleItemDrops</name>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="9"/>
        <source>Increase Item drops</source>
        <translation>Увеличить дроп предметов</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="10"/>
        <source>All help and explanation on this page assume you are familiar with Treasure Class (TC) and NoDrop terminology.&lt;br&gt;Basically all options here modifying TC pick weights - so one preferred option become less rare.&lt;br&gt;First 3 sliders are for increasing chance for Uniques/Sets/Rares. &lt;br&gt;Note that increase is accurate when your chances are low, but with high MF it can have diminishing return.</source>
        <translation>Объяснения здесь подразумевают что вы знакомы с понятием Treasure Class (TC) и NoDrop.&lt;br&gt;В основном все опции здесь меняют выборку в рамках того или иного TC, смещая шансы.&lt;br&gt;Первые 3 слайдера позволяют увеличить шансы Уник/Сетовы/Редких предметов.&lt;br&gt;Примечение, если ваши шасны получить предмет нужного качества высоки, то настройка достаточно точна.&lt;br&gt;В противном случае (высокий MF), увеличение частоты будет немного ниже.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="13"/>
        <source>Select preset...</source>
        <translation type="unfinished">Выбрать пресет...</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="13"/>
        <source>I want to find Tyrael&apos;s today!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="13"/>
        <source>Just tired of finding my first Shako</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="13"/>
        <source>A little bit more of good stuff, not crazy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="18"/>
        <source>Increase Unique Chance</source>
        <translation>Увеличить шанс Уникальных предметов</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="29"/>
        <source>Increase Set Chance</source>
        <translation>Увеличить шанс Наборных предметов</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="40"/>
        <source>Increase Rare Chance</source>
        <translation>Увеличить шанс Редких предметов</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="51"/>
        <source>Increase Chance of High-level equipment</source>
        <translation>Увеличить шанс предметов высокого уровня</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="52"/>
        <source>This will reduce the reduce the chance that equipment of lower level will drop as a fallback.&lt;br&gt;For example, for level 85 TC there is a 98% chance that it will drop from 84 level TC, and so on.</source>
        <translation>Эта настройка уменьшает шанс того что при попытке дропа &lt;br&gt;высокого уровня будет низкоуровневый дроп вместо него.&lt;br&gt;например, для TC 85 уровня существует 98% шанс что будет выброшен TC 84 и так далее.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="62"/>
        <source>Increase Champion/Unique/Superunique dropped item count</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="63"/>
        <source>Change item+potion ratio:&lt;br&gt;Champion: 2 items + 2 potions&lt;br&gt;Unique: 4 items + 2 potions&lt;br&gt;Superuniques: 6 items + 0 potions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Increase Champion/Unique item count</source>
        <translation type="vanished">Увеличить число дропов с элитных монстров</translation>
    </message>
    <message>
        <source>This will make Unique bosses to drop 4 items instead of just 1, and Champions drop 2 items instead of one.&lt;br&gt;Note that potion drops are slightly reduced.</source>
        <translation type="vanished">Уникальные боссы теперь будут кидать 4 предмета вместо одного, и чемпионы 2 вместо одного.&lt;br&gt;Кол-во бросаемых зелий снижено.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="69"/>
        <source>Always quest drops from bosses</source>
        <translation>Всегда квестовые дропы с боссов</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="76"/>
        <source>Adjust proportion of main drop classes</source>
        <translation>Изменить соотношение между основными типами дропа</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="80"/>
        <source>Increse drop count (via NoDrop)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="81"/>
        <source>This provides ability to gradually reduce NoDrop picks&lt;br&gt;(it basically similar to increasing players count in the game)&lt;br&gt;This only affect base monster drops</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>NoDrop (higher=LESS drops)</source>
        <translation type="vanished">NoDrop (больше=МЕНЬШЕ дропов)</translation>
    </message>
    <message>
        <source>NoDrop slider provides ability to gradually reduce NoDrop picks&lt;br&gt;(it basically similar to increasing players count in the game)</source>
        <translation type="vanished">NoDrop позволяет снизить долю NoDrop в TC&lt;br&gt;(механизм похож на повышение кол-ва игроков в игре)</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="91"/>
        <source>Good TC (Runes/Gems/Jewellery)</source>
        <translation>Good TC (Руны/Камни/Ювелирка)</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="92"/>
        <source>That will make Gems/Runes/Rings/Amulets/Charms drop far more often&lt;br&gt;compared to equipment (armor/weapons).</source>
        <translation>Это позволит падать камням/рунам/кольцам/амулетам/талисманам намного чаще&lt;br&gt;в сравнении с другим снаряжением (броне и оружию).</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="102"/>
        <source>Equipment (armor/weapons)</source>
        <translation>Экипировка (доспехи/оружие)</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="103"/>
        <source>Relative chance of equipment dropping compared to other items (or NoDrop).</source>
        <translation>Относительный шанс выпадания экипировки в соотношении с остальным (и NoDrop)</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="113"/>
        <source>Gold (higher=MORE gold)</source>
        <translation>Золото (выше=БОЛЬШЕ золота)</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="114"/>
        <source>Relative chance of gold compared to other items.</source>
        <translation>Относительный шанс на золото в сравнении с другими предметами.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="124"/>
        <source>Junk (keys/bolts/etc) (higher=MORE junk)</source>
        <translation>Барахло (ключи/болты/и тд) (выше=БОЛЬШЕ барахла)</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="125"/>
        <source>Relative chance of junk items compared to other items.</source>
        <translation>Относительный шанс неценных предметов в сранвении с остальным.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="135"/>
        <source>Make all Uniques have equal rarity on same base</source>
        <translation>Все Уникальные предметы имеют одинаковую редкость для общей базы</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemDrops.qml" line="136"/>
        <source>Now Uniques with equal item base will have equal chance to drop.&lt;br&gt;For example Tyrael&apos;s and Templar&apos;s will have equal chance. (and all rings too)</source>
        <translation>Если у предметов одинаковая база, то шанс выпадения у всех становится равный.&lt;br&gt;Например, Tyrael&apos;s и  Templar&apos;s имеют равный шанс. (и все кольца/амулеты)</translation>
    </message>
</context>
<context>
    <name>ModuleItemRandomizer</name>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="9"/>
        <source>Item Randomizer</source>
        <translation>Рандомайзер предметов</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="10"/>
        <source>What item randomizer does in short - it reads all possible item properties from Uniques, Sets, etc,&lt;br&gt;And then reassign properties back, but in random order (also it does not mean every original will be used)&lt;br&gt;For details, check descriptions of every option.</source>
        <translation>Что в целом делает Рандомайзер - читает все возможные свойства из предметов/камней в одну кучу,&lt;br&gt;затем проходит по всем предметам и назначает им новые свойства из этой кучи (пула).&lt;br&gt;Для подробностей, смотрите подсказки к разным опциям.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="13"/>
        <source>Select preset...</source>
        <translation type="unfinished">Выбрать пресет...</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="13"/>
        <source>I want to be overpowered machine!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="13"/>
        <source>Want to have fresh experience but balanced if possible</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Crazy-ness (or &apos;NON-balance level&apos;, lower = more balance, 100=chaos)</source>
        <translation type="vanished">Уровень безумия (или имбалансности, ниже=больше баланса, 100=хаос)</translation>
    </message>
    <message>
        <source>Crazyness level - determine level difference to be used when selecting new properties for item/rune/etc.&lt;br&gt;With &apos;10&apos; it will select between level-10 and level+10 at first, if there are &lt;50 candidates,&lt;br&gt;then it will select level-30..level+30, and finally it will try fully random. &lt;br&gt;In short, lower value = more balance in terms of original affix level and item level.</source>
        <translation type="vanished">Безумие определяет насколько сильная будет велика разница в уровнях для подбора новых свойств.&lt;br&gt;С &apos;10&apos; начнется выбор от исходный_уровень-10, исходный_уровень+10, и если выборка меньше 50,&lt;br&gt;то попробовать +-30, и если и этого не хватит, то уже полный диапазон. &lt;br&gt;Вкратце, чем ниже значение, тем более балансный результат.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="13"/>
        <source>I want crazy 9000+</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="18"/>
        <source>Buff level range</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="19"/>
        <source>Crazyness level - determine level difference to be used when selecting new properties for item/rune/etc.&lt;br&gt;For example, with value &apos;10&apos;, new item can be 10 levels higher in terms of used properties.&lt;br&gt;That does not affect item level or its requirements.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="27"/>
        <source>Nerf level range</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="28"/>
        <source>This is just as previous option, but forces item to be sometimes worse than original.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="37"/>
        <source>Item type fit percent (0% = fully random, 100% = all according to item type)</source>
        <translation>Доля параметров соответствия типу(0%=рандом, 100% = все параметры специфичны)</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="38"/>
        <source>Item fit slider allow you to select how much item affixes will be related to original item type.&lt;br&gt;For example, if you choose 80%, then 4 of 5 affixes will be selected to pool for specific item type&lt;br&gt;Item can have have several pools related to its type - say, scepter is a rod and a melee weapon.&lt;br&gt;Item type-specific properties will be picked in proportion to all types.</source>
        <translation>Эта настройка позволяет выбрать насколько аффиксы предмета будут соотноситься с типом оригинального предмета.&lt;br&gt;Например, если выбрать 80%, то 4 из 5 аффиксов будет выбрано не из общего пула, а из пула для типа предмета.&lt;br&gt;Если предмет имеет несколько типов (например, скипетр это дробящее оружие, оружие ближнего боя и просто оружие)&lt;br&gt;то аффиксы будут выбраны равномерно из всех их них.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="47"/>
        <source>Caster items percent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="48"/>
        <source>How many generated items will be caster-specific.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="57"/>
        <source>How many original properties to keep, percent</source>
        <translation>Как много исходных свойств предмета сохранять, процент</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="58"/>
        <source>You can select how many properties of original item you want to keep.&lt;br&gt;If 0, then every item will be fully randomized.&lt;br&gt;If 50, then half of genereted properties will be original, and half randomized.&lt;br&gt;If 100, then every property will be property of original item. (you can reduce an amount of props)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="67"/>
        <source>Relative property counts (min/max), compared to non-randomized original:</source>
        <translation>Число свойств (мин/макс), по сравнению с оригинальным до рандомизации:</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="71"/>
        <source>Uniques/Gems/Runes/Affixes, Min</source>
        <translation>Уник./Камни/Руны/Аффиксы, Минимум</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="72"/>
        <source>This and the next options determine new property count will be after generation.&lt;br&gt;If Min=Max=100%%, then property count will be exactly as original (except rare corner cases).&lt;br&gt;If Min=50%% and Max=200%%, then new property count will be at least half as original, and twice at best.&lt;br&gt;For example, if item has 5 properties, then worst case is 2, and best case is 9 (because 9 is maximum for Uniques)</source>
        <translation>Эта и следующая опция определяют новое число свойств после генерации&lt;br&gt;Если Мин=Макс=100%%, то новое число свойств будет всегда в точности как у оригинала.&lt;br&gt;Если Мин=50%% и Макс=200%, то новое число может быть как в два раза меньше, так и больше.&lt;br&gt;например, если оригинал имел 5 свойств, то с рандомайзером будет от 2 до 9 (потому что 9 это уже максимум для Уников)</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="82"/>
        <source>Uniques/Gems/Runes/Affixes, Max</source>
        <translation>Уник./Камни/Руны/Аффиксы, Максимум</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="93"/>
        <source>Set items, Min</source>
        <translation>Наборные предметы, Минимум</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="104"/>
        <source>Set items, Max</source>
        <translation>Наборные предметы, Максимум</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="115"/>
        <source>Number of versions of each unique</source>
        <translation>Количество версий каждого уникального предмета</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="116"/>
        <source>allow you to have different uniques with same name and level, but different properties,&lt;br&gt;you will have N different uniques with differnet stats;&lt;br&gt;so you have an opportunity to pick same item again to check it out.&lt;br&gt;This works only with Uniques, not Sets.</source>
        <translation>Позволяет иметь разные уникальные предметы с одним именем и уровнем, но разным набором свойств,&lt;br&gt;у вас будет N разных уникальных предметов, каждый со случайными параметрами;&lt;br&gt;так у вас есть возможность подбирать один и тот же предмет несколько раз чтобы оценить его снова.&lt;br&gt;К сожалению работает только с Уникальнными, не наборными предметами.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="125"/>
        <source>Number of different incompatible Sets</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="126"/>
        <source>Whis will create additional sets (Angelic 2, Angelic 3) with different properties.</source>
        <translation>Позволит иметь несколько несовместимых между собой копий Наборных предметов&lt;br&gt;(Ангельский 1, Ангельский 2 и тп)</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="170"/>
        <source>Update existing properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="171"/>
        <source>Properties that are selected to remain will be updated with new value range.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="178"/>
        <source>Set items partial bonus</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="179"/>
        <source>How generated items will use partial set bonuses:&lt;br&gt;Keep original - Existing item logic is preserved;&lt;br&gt;No partial - space for extra attributes will used for &apos;Blue&apos; properties&lt;br&gt;Use partial - extra set attributes will require other pieces, &apos;Green&apos; props.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="180"/>
        <source>Keep original</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="180"/>
        <source>No partial</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="180"/>
        <source>Use partial</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="191"/>
        <source>Adjust amount of different attribute rolls:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="195"/>
        <source>Resistances:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="205"/>
        <source>Aura when equip:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="215"/>
        <source>On hit/attack/struck procs:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="225"/>
        <source>+skills:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="235"/>
        <source>Stats - life/mana/attributes:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Prevent duplicate properties on items</source>
        <translation type="vanished">Предотвращать дубликаты свойств на предметах</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="135"/>
        <source>Randomize magix/rare affixes</source>
        <translation>Рандомизация аффиксов редких и магических предметов</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="136"/>
        <source>This will modify rare and magic suffixes - &lt;br&gt;so they can include properties of any other item in the game. &lt;br&gt;Note that their properties are read even without this option.</source>
        <translation>С этой опцией свойства магических и редких предметов тоже перемешаны;&lt;br&gt;т.о. они могут иметь свойство любого предмета в игре. &lt;br&gt;Примечание, свойства читаются в общий пул независимо от этой опции.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="142"/>
        <source>Randomize gem and runes properties</source>
        <translation>Рандомизировать свойства камней и рун</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="143"/>
        <source>This will modify gem and rune properties - &lt;br&gt;so they can include properties of any other item in the game. &lt;br&gt;Note that their properties are read even without this option.</source>
        <translation>Камни и руны смогут иметь свойства от любого другого предмета в игре.&lt;br&gt;Примечание, их свойства читаются в общий пул независимо от этой опции.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="149"/>
        <source>Replace skills with oskills</source>
        <translation>Заменить бонус навыков для класса на отвязанные от класса</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="156"/>
        <source>Replace charges with oskills</source>
        <translation>Заменить заряды на бонус к навыкам</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleItemRandomizer.qml" line="163"/>
        <source>Remove Knockback/Monster flee</source>
        <translation>Убрать свойства Отбрасывания и Обращения в бегство</translation>
    </message>
    <message>
        <source>Add extra attributes to randomize (comma-separated)</source>
        <translation type="vanished">Дополнительные атрибуты (разделенные запятой)</translation>
    </message>
</context>
<context>
    <name>ModuleMonDensity</name>
    <message>
        <location filename="../qml/Modules/ModuleMonDensity.qml" line="9"/>
        <source>Monster density</source>
        <translation>Плотность монстров</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonDensity.qml" line="10"/>
        <source>Density slider - that will affect increase of density of regular monsters &lt;br&gt;(note that it is higly dependant on map and tiles; max density can be reached on open area tiles.)&lt;br&gt;Boss packs slider icrease number of Champion/Uniques in zones; again, that also not a precise setting.&lt;br&gt;Last Checkbox can be used to quickly setup boss count on Normal and Nightmare exactly equal to Hell count.</source>
        <translation>Слайдер плотности - влияет на кол-во обычных монстров &lt;br&gt;(и это сильно зависит от карты и ее набора тайлов, на открытых тайлах потенциала к увеличению куда больше.)&lt;br&gt;Слайдер для элитных паков увеличит их число в зоне, но эта настройка не сильно точна (плюс у маленьких зон есть свой лимит).</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonDensity.qml" line="13"/>
        <source>Increase density, times</source>
        <translation>Увеличить плотность, раз</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonDensity.qml" line="23"/>
        <source>Increase Boss packs count, times</source>
        <translation>Увеличить кол-во элитных паков, раз</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonDensity.qml" line="33"/>
        <source>Increase monster groups population, +count&lt;br&gt;&lt;b&gt;Beware! This setting have the most impact on getting screen laggy!&lt;b&gt;&lt;br&gt;&lt;b&gt;Having value above +3 will work well only if previous options are not maxed!&lt;/b&gt;</source>
        <translation>Увеличить численность групп монстров, +штук&lt;br&gt;&lt;b&gt;Осторожно! Для D2R это сильно влияет на прозводительность и fps!&lt;b&gt;&lt;br&gt;&lt;b&gt;Иметь здесь +3 или выше плохая идея если предыдущие опции уже на максимуме!&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonDensity.qml" line="43"/>
        <source>Use Hell elite pack count on Nightmare and Normal</source>
        <translation>Использовать кол-во элитных паков на Normal и Nightmare такое же, как на Hell</translation>
    </message>
</context>
<context>
    <name>ModuleMonRandomizer</name>
    <message>
        <location filename="../qml/Modules/ModuleMonRandomizer.qml" line="9"/>
        <source>Monster Randomizer</source>
        <translation>Рандомайзер Монстров</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonRandomizer.qml" line="10"/>
        <source>When this feature enabled, now any area in the game can spawn any basic monsters (Super Uniques, Act bosses etc not affected).&lt;br&gt;For normal difficulty, each monster will have a &apos;level-adjusted&apos; copy which corresponds&lt;br&gt;to area&apos;s level (so you will have adequate monster stats in Act 1 for guest from Act 5).&lt;br&gt;For Nightmare/Hell, adjustment is done by area level.&lt;br&gt;Monster will have adjusted their minion spawns and skill levels, too.</source>
        <translation>При включении этой возможности, теперь каждая зона карты в игре может сгенерировать любых врагов из любых актов.&lt;br&gt;Для Normal, враги получают подогнанные по уровню невидимые копии (т.к. иначе невозможно победить врагов из 5 акта в первом)&lt;br&gt;Для Nigtmare/Hell, параметры автоматически подгонятся под уровень зоны.&lt;br&gt;Монстры так же получают адаптированные парамтеры для миньонов и уровни навыков.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonRandomizer.qml" line="13"/>
        <source>Random monster spawn in areas</source>
        <translation>Случайная генерация монстров в каждой зоне</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonRandomizer.qml" line="20"/>
        <source>Spawned different types in one area</source>
        <translation>Сколько в одной зоне может быть разных типов монстров</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonRandomizer.qml" line="21"/>
        <source>This option choose how many different types will spawn at once (default is 3, which is D2 original behavior).&lt;br&gt;Note that some map tiles have predefined spawns (shaman camps etc) that can not be randomized.</source>
        <translation>Сколько в одной генерации будет различных типов врагов (по умолчанию 3, как и в оригинале D2). &lt;br&gt;Обратите внимение, что некоторые зоны имеют тайлы с фиксированными спавнами (группы шаманов например).</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonRandomizer.qml" line="30"/>
        <source>Maximum possible monster types in one area (population variety)</source>
        <translation>Максимально возможно потенциальное число типов монстров</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonRandomizer.qml" line="31"/>
        <source>This option defines how many possibilites for monster types are exist in one zone;&lt;br&gt;Make this value lower if you want less variety between game restarts.&lt;br&gt;Note: D2 legacy support only 12 max, so value will be lowered implicitly.</source>
        <translation>Эта опция регулирует, как много разных типов в принципе &lt;br&gt;могут быть в одной зоне от генерации к генерации.&lt;br&gt;Примечание: для D2 Legacy значение будет автоматом снижено до 12.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonRandomizer.qml" line="40"/>
        <source>Random resistances</source>
        <translation>Случайные сопротивления</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonRandomizer.qml" line="47"/>
        <source>Use Hell resistances on NM/Normal</source>
        <translation>Использовать сопротивления от Hell на NM/Normal</translation>
    </message>
</context>
<context>
    <name>ModuleMonStats</name>
    <message>
        <location filename="../qml/Modules/ModuleMonStats.qml" line="9"/>
        <source>Monster stats</source>
        <translation>Параметры монстров</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonStats.qml" line="10"/>
        <source>Sliders allow you to change monster stats on all difficulties.&lt;br&gt;You can either lower or raise stats up 10x or 10x times.&lt;br&gt;That is orthogonal to /playersX setting - it will multiply stats independent.</source>
        <translation>Слайдеры позволяют настроить базовые параметры врагов на всех сложностях.&lt;br&gt;Вы можете понизить или повысить их в 10 раз.&lt;br&gt;Настройка /playerX с этими опциями не связана, и умножает параметры независимо.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonStats.qml" line="13"/>
        <source>Monster Attack Rating, multiply by</source>
        <translation>Атака монстров, умножить на</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonStats.qml" line="24"/>
        <source>Monster Defense, multiply by</source>
        <translation>Защита  монстров, умножить на</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonStats.qml" line="35"/>
        <source>Monster HP, multiply by</source>
        <translation>Здоровье монстров, умножить на</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonStats.qml" line="46"/>
        <source>Monster Damage, multiply by</source>
        <translation>Урон монстров, умножить на</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonStats.qml" line="57"/>
        <source>Monster EXP gain, multiply by</source>
        <translation>Получение опыта с монстров, умножить на</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonStats.qml" line="68"/>
        <source>Maximum resistance allowed for base monsters, percent:</source>
        <translation>Максимальное сопротивление для базовых монстров, процент:</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonStats.qml" line="69"/>
        <source>If you make this below 100, then regular monsters will have NO IMMUNITIES at all!&lt;br&gt;Use with caution. You can set this to 110-115, to make cold immunes breakable.&lt;br&gt;Also this do not prevent Enchanted monsters to have immunities.</source>
        <translation>Если вы сделаете это ниже 100, то обычные монстры никогда НЕ БУДУТ ИММУННЫ никогда!&lt;br&gt;Используйте осторожно. Можете поставить 110-115, т.о. cold иммуны станут ломаться с Conviction.&lt;br&gt;Также учтите, что это не влияет возможность Enchanted врагов иметь иммунитет.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonStats.qml" line="78"/>
        <source>Change monster resistances, by multiplying damage from magic they take</source>
        <translation>Изменить общее сопротивление монстров, умножив урон который они могут получать от магии</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonStats.qml" line="79"/>
        <source>DO NOT affect IMMUNE monsters and monsters with 0 resistance!&lt;br&gt;It affects resistance perent indirectly, through this calculation:&lt;br&gt;Lets say you have 200% selected for multiply, and monster has 80% basic Fire resist.&lt;br&gt;That means, he has 20% damage taken from fire, and new value will be 40% damage, or 60% resist.&lt;br&gt;Damage is capped at 100%, so you won&apos;t get negative reistance.&lt;br&gt;Also monster can&apos;t become immune to element, maximum resistance is 99%</source>
        <translation>НЕ ВЛИЯЕТ на ИММУННЫХ монстров и монстров с 0 сопротивлением!&lt;br&gt;Это изменяет сопротивление неявно, с помощью следующего вычисления:&lt;br&gt;Если  вы выбрали 200%, и у монстра уже есть 80% сопротивления Огню.&lt;br&gt;Т.о. монстр получает 20% урона от огня, и новое значение должно быть 40% урона, или 60% сопротивления.&lt;br&gt;Урон ограничен сверху 100%, что дает 0 сопротивление (отрицательного не будет).&lt;br&gt;Так же это не дает новых ИММУНОВ, т.е. новое сопротивление будет не больше 99%</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonStats.qml" line="88"/>
        <source>Apply resistance options to following:</source>
        <translation>Применить опции сопротивления к следующим стихиям:</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonStats.qml" line="94"/>
        <source>Elemental (Fire/Cold/Lightning)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonStats.qml" line="101"/>
        <source>Poison</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonStats.qml" line="109"/>
        <source>Magic (Non-elemental)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleMonStats.qml" line="116"/>
        <source>Physical</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ModulePerfectRoll</name>
    <message>
        <location filename="../qml/Modules/ModulePerfectRoll.qml" line="9"/>
        <source>Perfect rolls</source>
        <translation>Идеальные параметры</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModulePerfectRoll.qml" line="10"/>
        <source>Remove random effect when rolling between min and max value for something.</source>
        <translation>Убирает эффект разброса параметров для различных предметов. Всегда макс.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModulePerfectRoll.qml" line="15"/>
        <source>Perfect rolls for crafting in the Cube</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModulePerfectRoll.qml" line="22"/>
        <source>Perfect Uniques</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModulePerfectRoll.qml" line="29"/>
        <source>Perfect Rune Word rolls</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModulePerfectRoll.qml" line="36"/>
        <source>Perfect Set items</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModulePerfectRoll.qml" line="44"/>
        <source>Perfect Magic/Rare affixes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModulePerfectRoll.qml" line="51"/>
        <source>Perfect predetermined crafted affixes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModulePerfectRoll.qml" line="58"/>
        <source>Always max defense Armor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModulePerfectRoll.qml" line="67"/>
        <source>Unique items level 1 power multiply</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModulePerfectRoll.qml" line="78"/>
        <source>Unique items level 50 power multiply</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModulePerfectRoll.qml" line="89"/>
        <source>Set items level 1 power multiply</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModulePerfectRoll.qml" line="100"/>
        <source>Set items level 50 power multiply</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModulePerfectRoll.qml" line="111"/>
        <source>Magic affix level 1 power multiply</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModulePerfectRoll.qml" line="122"/>
        <source>Magic affix level 50 power multiply</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModulePerfectRoll.qml" line="133"/>
        <source>Rune Words power multiply</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModulePerfectRoll.qml" line="144"/>
        <source>Crafted items power multiply</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModulePerfectRoll.qml" line="155"/>
        <source>Rune/Gems power multiply</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ModuleQol</name>
    <message>
        <location filename="../qml/Modules/ModuleQol.qml" line="9"/>
        <source>Quality of life</source>
        <translation>Упрощение жизни</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleQol.qml" line="10"/>
        <source>Tiny quality things to make you life easier: &lt;br&gt;1. Insreasing stacks for tomes/quivers/keys;&lt;br&gt;2. Remove Unique charm limit (useful with randomizer);&lt;br&gt;3. Allow usage of some skills in the town;&lt;br&gt;4. Reduce item costs. </source>
        <translation>Набор небольших твиков для упрощения жизни.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleQol.qml" line="15"/>
        <source>Increase tome sizes 20 -&gt; 60</source>
        <translation>Увеличить вместимость книг 20 -&gt; 60</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleQol.qml" line="22"/>
        <source>Increase key chain 12 -&gt; 50</source>
        <translation>Увеличить связки ключей 12 -&gt; 60</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleQol.qml" line="29"/>
        <source>Increase quivers size -&gt; 511</source>
        <translation>Увеличить колчаны до 511</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleQol.qml" line="30"/>
        <source>D2R already have 500 quiver size, so use this for LoD</source>
        <translation>В D2R колчаны уже 500 стрел, нужно только для D2 LoD</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleQol.qml" line="36"/>
        <source>Remove limit on Unique charms</source>
        <translation>Убрать ограничение на 1 уникальный талисман в инвентаре</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleQol.qml" line="44"/>
        <source>Allow using Teleport, BC, BO in town</source>
        <translation>Разрешить использовать Телепорт и боевые приказы в городе</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleQol.qml" line="51"/>
        <source>Allow all skills in town</source>
        <translation>Разрешить все заклинанияи в городе</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleQol.qml" line="58"/>
        <source>Show ilevel in items&apos; titles</source>
        <translation>Показывать ilevel в скобках</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleQol.qml" line="65"/>
        <source>Disable experience penalty after level 70</source>
        <translation>Отключить штраф опыт после уровня 70</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleQol.qml" line="66"/>
        <source>That means you won&apos;t have around 1% EXP at higher levels, so high level grind will be much faster.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleQol.qml" line="74"/>
        <source>Reduce costs of skills and stats on items (affects repair cost mostly)</source>
        <translation>Снизить стоимость (в золоте) для параметров на предметах (снизить цену починки)</translation>
    </message>
</context>
<context>
    <name>ModuleRequirements</name>
    <message>
        <location filename="../qml/Modules/ModuleRequirements.qml" line="9"/>
        <source>Requirements</source>
        <translation>Требования</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRequirements.qml" line="10"/>
        <source>Options for lowering/raising requirements for equimpemnt. </source>
        <translation>Опции для снижения/повышения требований к экипировки </translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRequirements.qml" line="13"/>
        <source>Change Strength requirements on items, %</source>
        <translation>Изменить требования к Силе на предметах, %</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRequirements.qml" line="14"/>
        <source>If you set value above 100%, Strength requirements &lt;br&gt; become higher compared to the original.</source>
        <translation>Если выставить значение выше 100%, требования к Силе &lt;br&gt; станут выше, чем это было в оригинале.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRequirements.qml" line="24"/>
        <source>Change Dexterity requirements on items, %</source>
        <translation>Изменить требования к Ловкости на предметах, %</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRequirements.qml" line="35"/>
        <source>Change Character level requirement on regular items, %</source>
        <translation>Изменить требование к уровню персонажа на обычных предметах, %</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRequirements.qml" line="36"/>
        <source>That controls &apos;Level required:&apos; you see on the items.</source>
        <translation>Влияет на поле &apos;Минимальный уровень:&apos; который вы видите в игре на предметах.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRequirements.qml" line="46"/>
        <source>Change Character level requirement on Unique/Set items, %</source>
        <translation>Изменить требование к уровню персонажа на Уникальных/Наборных предметах, %</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRequirements.qml" line="57"/>
        <source>Change Character level requirement on Magic/Rare items, %</source>
        <translation>Изменить требование к уровню персонажа на Магических/Редких предметах, %</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRequirements.qml" line="68"/>
        <source>Change Minimal level required to spent points into skill, %</source>
        <translation>Изменить минимальный уровень необходимый для навыков персонажа, %</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRequirements.qml" line="69"/>
        <source>For example, if you select 50%, you can get level 30 skill on level 15 instead.</source>
        <translation>Если вы выберете 50%, сможете распределить очки в навык 30 уровня уже на 15 уровне.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRequirements.qml" line="79"/>
        <source>Change quality level on regular items, %</source>
        <translation>Изменить qlvl (качественный уровень) на обычных предметах, %</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRequirements.qml" line="80"/>
        <source>That controls how valueable item is from drop calculation process.&lt;br&gt; Lower value means you get item dropped much earlier in the game.</source>
        <translation>Управляет тем, насколько ценный предмет с точки зрения расчета дропа.&lt;br&gt; Значение ниже дает возможность получать ценные и редкие предметы раньше.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRequirements.qml" line="90"/>
        <source>Change quality level on Unique/Set items, %</source>
        <translation>Изменить qlvl (качественный уровень) на Уникальных/Наборных предметах, %</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRequirements.qml" line="101"/>
        <source>Change quality level on Magic/Rare items, %</source>
        <translation>Изменить qlvl (качественный уровень) на Магических/Редких предметах, %</translation>
    </message>
</context>
<context>
    <name>ModuleRuneDrops</name>
    <message>
        <location filename="../qml/Modules/ModuleRuneDrops.qml" line="9"/>
        <source>Increase Rune drops</source>
        <translation>Увеличить дроп рун</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRuneDrops.qml" line="10"/>
        <source>All help and explanation on this page assume you are familiar with Treasure Class (TC) and NoDrop terminology.&lt;br&gt;Basically all options here modifying TC pick weights - so one preferred option become less rare.</source>
        <translation>Объяснения здесь подразумевают что вы знакомы с понятием Treasure Class (TC) и NoDrop.&lt;br&gt;В основном все опции здесь меняют выборку в рамках того или иного TC, смещая шансы.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRuneDrops.qml" line="13"/>
        <source>Select preset...</source>
        <translation type="unfinished">Выбрать пресет...</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRuneDrops.qml" line="13"/>
        <source>Gimme pile of Ber runes!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRuneDrops.qml" line="13"/>
        <source>I want to make Infinity at least once in my life</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRuneDrops.qml" line="13"/>
        <source>I want high runes to be rare, but want more rune drops overall</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRuneDrops.qml" line="18"/>
        <source>Increase Rare Rune drops</source>
        <translation>Увеличить шанс дропа редких рун</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRuneDrops.qml" line="19"/>
        <source>That will drastically improve High Rune drop rates! You probably don&amp;apos;t need more than 50x.&lt;br&gt;Value here increases chance of dropping Zod in &apos;Runes 17&apos; TC&lt;br&gt;Rarity of other runes will change proportionally &lt;br&gt;(so High Runes still be more rare in the same manner).</source>
        <translation>Эта настройка крайне сильно увеличивает шансы выпадения редких рун!&lt;br&gt;Вам скорее всего не нужно больше 50х.&lt;br&gt;Значение здесь повышает шанс выпадения Zod руны в &apos;Runes 17&apos; TC&lt;br&gt;Редкость остальных рун меняется пропорционально&lt;br&gt;(но все равно нелинейно как и прежде).</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRuneDrops.qml" line="28"/>
        <source>Increase Runes chance in Good TC</source>
        <translation>Увеличить шанс дропа рун в Good TC</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRuneDrops.qml" line="29"/>
        <source>When dropping from Good TC, that will make Runes drop more often &lt;br&gt;compared to other jewellery (Gems/Rings/Amulets/Charms)</source>
        <translation>Когда кидается Good TC, руны будут выпадать намного чаще &lt;br&gt;в сравнении с остальной бижутерией.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRuneDrops.qml" line="38"/>
        <source>Switch (Ber,Jah) with (Cham,Zod) in rarity</source>
        <translation>Поменять (Ber,Jah) с (Cham,Zod) в редкости</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRuneDrops.qml" line="39"/>
        <source>When game desides to drop Jah rune, it drops Zod instead. And vice versa.</source>
        <translation>Когда игра решит выбросить Jah, упадет Zod вместо этого. И наоборот.</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRuneDrops.qml" line="45"/>
        <source>Make Countess Drop higher runes (up to Runes 17=Zod on Hell)</source>
        <translation>Countess будет выбрасывать руны более высокого уровня (вплоть до Zod на Hell)</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRuneDrops.qml" line="52"/>
        <source>Increase Countess Runes drop count to 5</source>
        <translation>Увеличить число рун с Countess до 5</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleRuneDrops.qml" line="59"/>
        <source>Increase Wraiths Runes drop</source>
        <translation>Увеличить частоту дропа рун с призраков</translation>
    </message>
</context>
<context>
    <name>ModuleSkillRandomizer</name>
    <message>
        <location filename="../qml/Modules/ModuleSkillRandomizer.qml" line="9"/>
        <source>Skill Randomizer</source>
        <translation>Рандомайзер Навыков</translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleSkillRandomizer.qml" line="10"/>
        <source>This allow to bring &apos;some&apos; randomization into character skills.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleSkillRandomizer.qml" line="13"/>
        <source>Randomize skill tabs within each character</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleSkillRandomizer.qml" line="14"/>
        <source>That DOES not move skills between characters! What it can do:&lt;br&gt;-Move a skill to a different skill tab;&lt;br&gt;-Move a skill to a higher/lower level;&lt;br&gt;-Change skill requirements.&lt;br&gt;So, after level 50 there is not much of a difference. Synergies are also untouched.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleSkillRandomizer.qml" line="20"/>
        <source>Randomize element types for some damage skills</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleSkillRandomizer.qml" line="21"/>
        <source>For some skills, where it is easy to change damage type, it will randomize between 5 different elements:&lt;br&gt;Cold, Fire, Lightning, Magic and Poison&lt;br&gt;WARNING! Skill descriptions won&apos;t be updated and will be completely wrong!&lt;br&gt;Also, character info screen does not show difference between cold and magic correctly.&lt;br&gt;You need to find by yourself what damage (magic or cold) was rolled.&lt;br&gt;For poison, all damage is dealt over 2 seconds and slightly more than original.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleSkillRandomizer.qml" line="27"/>
        <source>Ensure that element types are different from original</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/Modules/ModuleSkillRandomizer.qml" line="28"/>
        <source>If the source element is Cold, then new randomized element never should be Cold.&lt;br&gt;If this turned off, ~20% of skills will remain original behaviour.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Navigation</name>
    <message>
        <source>Main</source>
        <translation type="obsolete">Главные настройки</translation>
    </message>
    <message>
        <source>Randomizers</source>
        <translation type="obsolete">Рандомизация</translation>
    </message>
    <message>
        <source>Item Randomizer</source>
        <translation type="obsolete">Рандомайзер предметов</translation>
    </message>
    <message>
        <source>Monster Randomizer</source>
        <translation type="obsolete">Рандомайзер Монстров</translation>
    </message>
    <message>
        <source>Skill Randomizer</source>
        <translation type="obsolete">Рандомайзер Навыков</translation>
    </message>
    <message>
        <source>Make harder</source>
        <translation type="obsolete">Сделать сложней</translation>
    </message>
    <message>
        <source>Monster stats</source>
        <translation type="obsolete">Параметры монстров</translation>
    </message>
    <message>
        <source>Monster density</source>
        <translation type="obsolete">Плотность монстров</translation>
    </message>
    <message>
        <source>Challenge</source>
        <translation type="obsolete">Челендж</translation>
    </message>
    <message>
        <source>Make easier</source>
        <translation type="obsolete">Сделать легче</translation>
    </message>
    <message>
        <source>Horadric Cube</source>
        <translation type="obsolete">Куб Хорадрика</translation>
    </message>
    <message>
        <source>Gambling</source>
        <translation type="obsolete">Азартная игра</translation>
    </message>
    <message>
        <source>Character</source>
        <translation type="obsolete">Персонаж</translation>
    </message>
    <message>
        <source>Requirements</source>
        <translation type="obsolete">Требования</translation>
    </message>
    <message>
        <source>Misc</source>
        <translation type="obsolete">Прочее</translation>
    </message>
    <message>
        <source>Quality of life</source>
        <translation type="obsolete">Упрощение жизни</translation>
    </message>
    <message>
        <source>Drops filtering</source>
        <translation type="obsolete">Фильтр лута</translation>
    </message>
</context>
<context>
    <name>PageMain</name>
    <message>
        <location filename="../qml/PageMain.qml" line="16"/>
        <source>Main</source>
        <translation>Главные настройки</translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="21"/>
        <source>Reset to default</source>
        <translation>Сброс на умолчания</translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="30"/>
        <source>Input folder (mandatory):</source>
        <translation>Исходная директория (обязательно):</translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="31"/>
        <source>Generally, just choose folder with Game.exe.&lt;br&gt;Note: for ProjectD2, select subfolder named &apos;ProjectD2&apos; in Diablo II.&lt;br&gt;Press Detect! to find information in Windows Registry.</source>
        <translation>Обычно, просто выберите папку игры где находится Game.exe&lt;br&gt;Note: для ProjectD2, выберите подкаталог &apos;ProjectD2&apos; в Diablo II.&lt;br&gt;Нажмите Детктировать! чтобы найти установку в реестре</translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="44"/>
        <source>Detect!</source>
        <translation>Детектировать!</translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="48"/>
        <location filename="../qml/PageMain.qml" line="75"/>
        <source>Browse...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="63"/>
        <source>Output folder (optional, leave empty to auto-detect):</source>
        <translation>Выходная директория (опционально, оставьте пустым для автоопределения)</translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="93"/>
        <source>Target D2 Game version:</source>
        <translation>Целевая версия D2:</translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="121"/>
        <source>D2 Legacy</source>
        <translation>D2 Legacy</translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="121"/>
        <source>D2 Resurrected</source>
        <translation>D2 Resurrected</translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="138"/>
        <source>D2R - LoD</source>
        <translation>D2R - LoD</translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="138"/>
        <source>D2R - RotW</source>
        <translation>D2R - RotW</translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="155"/>
        <source>This determines source data that is used by randomizer&lt;br&gt;&lt;b&gt;Game data archive&lt;/b&gt; - For D2R Resurrected, this is CASC storage;&lt;br&gt;&lt;b&gt;Game data archive&lt;/b&gt; - For D2 Legacy this is .mpq file;&lt;br&gt;&lt;b&gt;Folder with extracted data&lt;/b&gt; - If you want to use D2R select folder containing modinfo.json, otherwise select game root&lt;br&gt;&lt;b&gt;Only .txt&lt;/b&gt; - Folder with .txt files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="163"/>
        <source>This determines output type:&lt;br&gt;&lt;b&gt;D2 Resurrected mod&lt;/b&gt; - create D2R mod with all needed subfolders, path is determined by config name (see Extra tab)&lt;br&gt;&lt;b&gt;Full folders&lt;/b&gt; - create folder structure (data/global/...) with txt/json&lt;br&gt;&lt;b&gt;Only .txt&lt;/b&gt; - Folder with .txt files, note that for D2R that will mean json files will not be written&lt;br&gt;&lt;b&gt;Dry run&lt;/b&gt; - read input, process randomizer, but don&apos;t write anything</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="164"/>
        <source>Dry run</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="177"/>
        <source>Seed determine how reproduceable generation results are.&lt;br&gt;If seed stays the same, output of generation is also always exactly the same.&lt;br&gt;If you want each generation be stable and reproduceable, remove &lt;b&gt;Create new seed&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="154"/>
        <source>Input mode, where to read game data:</source>
        <translation>Тип входной директории:</translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="156"/>
        <source>Folder with extracted data</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="156"/>
        <location filename="../qml/PageMain.qml" line="164"/>
        <source>Only .txt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="156"/>
        <source>Game data archive (CASC or Storm=mpq)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="162"/>
        <source>Output format, how files and folders will be created:</source>
        <translation>Выходной формат директории:</translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="164"/>
        <source>D2 Resurrected mod</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="164"/>
        <source>Full folders</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mod id:</source>
        <translation type="obsolete">Идентификатор мода:</translation>
    </message>
    <message>
        <source>Use Diablo II legacy installation</source>
        <translation type="obsolete">Использовать установку Diablo II legacy</translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="174"/>
        <source>Random seed:</source>
        <translation>Случайный посев:</translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="186"/>
        <source>Generate new</source>
        <translation>Сгенерировать новый</translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="190"/>
        <source>Create new seed on every Generate call</source>
        <translation>Пересоздавать при каждом нажатии Генерировать</translation>
    </message>
    <message>
        <source>Output directory (leave empty to output to D2R path):</source>
        <translation type="obsolete">Выходная директория (оставьте пустой чтобы сохранить мод по пути D2R):</translation>
    </message>
    <message>
        <source>Export all *.txt (for further manual edit)</source>
        <translation type="obsolete">Экспортировать все .txt файлы (для дальнейшего редактирования)</translation>
    </message>
</context>
<context>
    <name>PageTool</name>
    <message>
        <source>Main</source>
        <translation type="obsolete">Главные настройки</translation>
    </message>
    <message>
        <source>D2R save and user settings root:</source>
        <translation type="obsolete">D2R директория с сохранениями и настройками:</translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="78"/>
        <source>Copy Settings.json</source>
        <translation>Скопировать Settings.json</translation>
    </message>
    <message>
        <source>(this will copy default settings to mod folder)</source>
        <translation type="obsolete">(это скопирует ваши настройки в новую папку мода)</translation>
    </message>
    <message>
        <source>D2R command arguments (read-only):</source>
        <translation type="obsolete">D2R командная строка (только чтение):</translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="27"/>
        <source>Extra helpful utilities</source>
        <translation>Дополнительные возможности</translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="43"/>
        <source>Mod id:</source>
        <translation>Идентификатор мода:</translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="44"/>
        <source>That will affect game&apos;s saves folder subdirectory and mod subfolder for D2R</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="62"/>
        <source>D2R folder for mod save and settings:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="82"/>
        <source>This will copy Game Settings to mod folder&lt;br&gt;That will allow to skip configuring mod on launch.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="88"/>
        <source>Copy all save files (.d2s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="92"/>
        <source>this will copy all your character save files to modded folder.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="111"/>
        <source>D2 commandline arguments (read-only):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="122"/>
        <source>Choose convenient way to launch the mod:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="127"/>
        <source>Option 1:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="131"/>
        <source>Make shortcut on Desktop</source>
        <translation type="unfinished">Создать ярлык на рабочем столе</translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="143"/>
        <source>Option 2:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="146"/>
        <source>Reset launch to unmodded</source>
        <translation>Сбросить запуск на &quot;без мода&quot;</translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="151"/>
        <source>Set launch to mod</source>
        <translation type="unfinished">Установить запуск BNet на мод</translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="156"/>
        <source>(Close Battlenet launcher before actions!)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="163"/>
        <source>Locale id for D2R:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Randomizers</source>
        <translation type="vanished">Рандомизация</translation>
    </message>
    <message>
        <source>Make harder</source>
        <translation type="vanished">Сделать сложней</translation>
    </message>
    <message>
        <source>Make easier</source>
        <translation type="vanished">Сделать легче</translation>
    </message>
    <message>
        <source>Misc</source>
        <translation type="vanished">Прочее</translation>
    </message>
    <message>
        <source>Plugins</source>
        <translation type="vanished">Плагины</translation>
    </message>
</context>
<context>
    <name>WidgetCommonTab</name>
    <message>
        <location filename="../qml/Widgets/WidgetCommonTab.qml" line="36"/>
        <source>Enable this tab</source>
        <translation>Включить эту вкладку</translation>
    </message>
    <message>
        <location filename="../qml/Widgets/WidgetCommonTab.qml" line="41"/>
        <source>Reset to default</source>
        <translation>Сброс на умолчания</translation>
    </message>
</context>
<context>
    <name>WidgetPreset</name>
    <message>
        <location filename="../qml/Widgets/WidgetPreset.qml" line="16"/>
        <source>Don&apos;t know where to start? Select a preset:</source>
        <translation>Не знаете с чего начать? Выберите пресет:</translation>
    </message>
</context>
</TS>
