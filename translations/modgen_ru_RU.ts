<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ru_RU">
<context>
    <name>D2ModGen::ConfigPageMergeMods</name>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="146"/>
        <source>Overall order of data load is: D2 source from Main settings -&gt;
-&gt; Pre-gen data -&gt; modgen features -&gt; Post-gen data -&gt; output folder.</source>
        <translation>Общий порядок загрузки источников: D2директория из Основных настроек -&gt;
-&gt; Пре-генерация -&gt; работа вкладок modgen -&gt; Пост-генерация -&gt; выходная директория.</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="203"/>
        <source>You can add aditional steps to mod generation, prior or after the d2modgen work
Two additional type of sources are supported:
1. folder with .txt files
2. another D2R mod
After selecting source type, you need to specify conflict resolution, 
for the case additional source contains a file which already been generated.</source>
        <translation>Вы можете добавить дополнительные этапы к генерации мода d2mogen, либо до либо после
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
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="43"/>
        <source>CSV files folder</source>
        <translation>Директория с файлами CSV</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="43"/>
        <source>D2R mod</source>
        <translation>Мод D2R</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="48"/>
        <source>Replace - clear previous data</source>
        <translation>Замена - очистить предыдущие данные, заполнить новые</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="49"/>
        <source>Append - place all data at the end</source>
        <translation>Добавить - поместить все данные из источника в конец</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="50"/>
        <source>Update - overwrite records with same key</source>
        <translation>Обновить - перезаписать только существующие строки</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="51"/>
        <source>AppendNew - add non-existent data to the end</source>
        <translation>Добавить новые - добавить те записи, которых еще нет в таблице</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="52"/>
        <source>Merge - try Update, then AppendNew</source>
        <translation>Слияние - попробовать Обновить, затем Добавить новый</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="53"/>
        <source>Skip</source>
        <translation>Пропустить</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="54"/>
        <source>Raise error</source>
        <translation>Сообщение об ошибке</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="62"/>
        <source>Mod name:</source>
        <translation>Имя мода:</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="71"/>
        <source>Folder with .txt files:</source>
        <translation>Директория с .txt файлами:</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="74"/>
        <source>Type:</source>
        <translation>Тип:</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="77"/>
        <source>Action on conflict:</source>
        <translation>Действие при конфликте:</translation>
    </message>
</context>
<context>
    <name>D2ModGen::MainConfigPage</name>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="92"/>
        <source>Use Diablo II legacy installation</source>
        <translation>Использовать установку Diablo II legacy</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="110"/>
        <source>Add key to new char inventory (Basic mod test)</source>
        <translation>Добавить ключи в инвентарь нового персонажа</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="112"/>
        <source>Export all *.txt (for further manual edit)</source>
        <translation>Экспортировать все .txt файлы (для дальнейшего редактирования)</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="115"/>
        <source>Copy Settings.json</source>
        <translation>Скопировать Settings.json</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="117"/>
        <source>Set launch to mod</source>
        <translation>Установить запуск BNet на мод</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="118"/>
        <source>Reset launch to unmodded</source>
        <translation>Сбросить запуск на немод.</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="119"/>
        <source>Make shortcut on Desktop</source>
        <translation>Создать ярлык на рабочем столе</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="134"/>
        <source>Mod id:</source>
        <translation>Идентификатор мода:</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="149"/>
        <source>D2R path:</source>
        <translation>Путь к D2R:</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="160"/>
        <source>D2 legacy path:</source>
        <translation>Путь к D2 Legacy:</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="171"/>
        <source>D2R save and user settings root:</source>
        <translation>D2R директория с сохранениями и настройками:</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="176"/>
        <source>(this will copy default settings to mod folder)</source>
        <translation>(это скопирует ваши настройки в новую папку мода)</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="185"/>
        <source>D2R command arguments (read-only):</source>
        <translation>D2R командная строка (только чтение):</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="197"/>
        <source>(&lt;b&gt;Close Battlenet launcher before actions!&lt;/b&gt;)</source>
        <translation>(&lt;b&gt;Закройте Battlenet перед этими действиями!&lt;/b&gt;)</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="215"/>
        <source>Output directory (leave empty to output to D2R path):</source>
        <translation>Выходная директория (оставьте пустой чтобы сохранить мод по пути D2R):</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="313"/>
        <source>Main</source>
        <translation>Главные настройки</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="318"/>
        <source>This page allow you to select most important options:
-Where input and output data is (D2 installation - Resurrected or Legacy);
-Create new D2R save folder with global settings;
-Create desktop shortcut to launch mod.
However, there are some less important abilities: 
-Seed number (max 2^32) determines how random generation behaves. 
If seed is unchaged, then all generation will be the same every time 
You can edit seed manually or press F5 to generate new one.
-Add key to character inventory for fresh character - now you are sure you started modded version.</source>
        <translation>Эта страница позволяет выставить самые важные опции:
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
        <location filename="../src/QtApp/MainWindow.cpp" line="102"/>
        <source>Generate</source>
        <translation>Генерировать</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="111"/>
        <source>Status label.</source>
        <translation>Статусная строка.</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="166"/>
        <source>Reset to default</source>
        <translation>Сброс на умолчания</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="167"/>
        <source>Enable this tab</source>
        <translation>Включить эту вкладку</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="203"/>
        <source>Select preset...</source>
        <translation>Выбрать пресет...</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="214"/>
        <source>Do not know where to start? Select a preset:</source>
        <translation>Не знаете откуда начать? Выберите пресет:</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="265"/>
        <source>File</source>
        <translation>Файл</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="266"/>
        <source>Actions</source>
        <translation>Действия</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="267"/>
        <source>About</source>
        <translation>О программе</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="268"/>
        <source>Save config...</source>
        <translation>Сохранить конфиг...</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="269"/>
        <source>Load config...</source>
        <translation>Загрузить конфиг...</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="270"/>
        <source>Clear config</source>
        <translation>Очистить конфиг</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="271"/>
        <source>Config from preset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="272"/>
        <source>Browse to settings folder</source>
        <translation>Открыть папку с настройками</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="274"/>
        <source>Quit without saving</source>
        <translation>Выйти без сохранения</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="275"/>
        <source>Save and quit</source>
        <translation>Сохранить и выйти</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="276"/>
        <source>Generate mod</source>
        <translation>Генерация мода</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="277"/>
        <source>Create seed</source>
        <translation>Создать новый seed</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="278"/>
        <source>Undo</source>
        <translation>Отменить</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="279"/>
        <source>Theme</source>
        <translation>Тема оформления</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="280"/>
        <source>Language</source>
        <translation>Язык</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="318"/>
        <source>You need to restart for changes apply.</source>
        <translation>Нужно перезагрузить для применения.</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="417"/>
        <source>Start...</source>
        <translation>Старт...</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="427"/>
        <source>Mod &apos;%1&apos; successfully updated (%2).</source>
        <translation>Мод &apos;%1&apos; успешно обновлен (%2).</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="124"/>
        <source>Randomizers</source>
        <translation>Рандомизация</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="125"/>
        <source>Make harder</source>
        <translation>Сделать сложней</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="126"/>
        <source>Make easier</source>
        <translation>Сделать легче</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="127"/>
        <source>Misc</source>
        <translation>Прочее</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="128"/>
        <source>Plugins</source>
        <translation>Плагины</translation>
    </message>
</context>
</TS>
