<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ko_KR">
<context>
    <name>D2ModGen::ConfigPageMergeMods</name>
    <message>
        <source>Overall order of data load is: D2 source from Main settings -&gt;
-&gt; Pre-gen data -&gt; modgen features -&gt; Post-gen data -&gt; output folder.</source>
        <translation type="vanished">전체 데이터 로드 순서: 메인 설정에서 D2 소스 -&gt;
-&gt; 사전 생성 데이터 -&gt; modgen 기능 -&gt; 후처리 데이터 -&gt; 출력 폴더.</translation>
    </message>
    <message>
        <source>You can add aditional steps to mod generation, prior or after the d2modgen work
Two additional type of sources are supported:
1. folder with .txt files
2. another D2R mod
After selecting source type, you need to specify conflict resolution, 
for the case additional source contains a file which already been generated.</source>
        <translation type="vanished">d2modgen 작업 전후에 모드 생성에 추가 단계를 추가할 수 있습니다.
두 가지 추가 소스 유형이 지원됩니다:
1. .txt 파일이 있는 폴더
2. 다른 D2R 모드
소스 유형을 선택한 후, 추가 소스에 이미 생성된 파일이 포함된 경우를 대비하여 충돌 해결 방법을 지정해야 합니다.</translation>
    </message>
</context>
<context>
    <name>D2ModGen::ConfigPageMergeModsItem</name>
    <message>
        <source>CSV files folder</source>
        <translation type="vanished">CSV 파일 폴더</translation>
    </message>
    <message>
        <source>D2R mod</source>
        <translation type="vanished">D2R 모드</translation>
    </message>
    <message>
        <source>Replace - clear previous data</source>
        <translation type="vanished">대체 - 기존 데이터 삭제</translation>
    </message>
    <message>
        <source>Append - place all data at the end</source>
        <translation type="vanished">추가 - 모든 데이터를 끝에 삽입</translation>
    </message>
    <message>
        <source>Update - overwrite records with same key</source>
        <translation type="vanished">업데이트 - 동일한 키의 데이터 덮어쓰기</translation>
    </message>
    <message>
        <source>AppendNew - add non-existent data to the end</source>
        <translation type="vanished">새 항목 추가 - 존재하지 않는 빈 데이터를 끝에 추가</translation>
    </message>
    <message>
        <source>Merge - try Update, then AppendNew</source>
        <translation type="vanished">병합 - 기존 항목 업데이트 후 새 항목 추가</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation type="vanished">건너뛰기</translation>
    </message>
    <message>
        <source>Raise error</source>
        <translation type="vanished">오류 발생</translation>
    </message>
    <message>
        <source>Mod name:</source>
        <translation type="vanished">모드 이름:</translation>
    </message>
    <message>
        <source>Folder with .txt files:</source>
        <translation type="vanished">.txt 파일이 있는 폴더:</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation type="vanished">유형:</translation>
    </message>
    <message>
        <source>Action on conflict:</source>
        <translation type="vanished">충돌 시 행동:</translation>
    </message>
</context>
<context>
    <name>D2ModGen::MainConfigPage</name>
    <message>
        <source>Use Diablo II legacy installation</source>
        <translation type="vanished">Diablo II 레거시 설치 사용</translation>
    </message>
    <message>
        <source>Add key to new char inventory (Basic mod test)</source>
        <translation type="vanished">새 캐릭터 인벤토리에 키 추가 (기본 모드 테스트)</translation>
    </message>
    <message>
        <source>Export all *.txt (for further manual edit)</source>
        <translation type="vanished">모든 *.txt 내보내기 (추가 수동 편집용)</translation>
    </message>
    <message>
        <source>Copy Settings.json</source>
        <translation type="vanished">Settings.json 복사</translation>
    </message>
    <message>
        <source>Set launch to mod</source>
        <translation type="vanished">모드로 실행 설정</translation>
    </message>
    <message>
        <source>Reset launch to unmodded</source>
        <translation type="vanished">모드 해제된 상태로 실행 초기화</translation>
    </message>
    <message>
        <source>Make shortcut on Desktop</source>
        <translation type="vanished">바탕화면에 바로가기 만들기</translation>
    </message>
    <message>
        <source>Mod id:</source>
        <translation type="vanished">모드 ID:</translation>
    </message>
    <message>
        <source>D2R path:</source>
        <translation type="vanished">D2R 경로:</translation>
    </message>
    <message>
        <source>D2 legacy path:</source>
        <translation type="vanished">D2 레거시 경로:</translation>
    </message>
    <message>
        <source>D2R save and user settings root:</source>
        <translation type="vanished">D2R 저장 및 사용자 설정 루트:</translation>
    </message>
    <message>
        <source>(this will copy default settings to mod folder)</source>
        <translation type="vanished">(기본 설정이 모드 폴더로 복사됩니다)</translation>
    </message>
    <message>
        <source>D2R command arguments (read-only):</source>
        <translation type="vanished">D2R 명령 인수 (읽기 전용):</translation>
    </message>
    <message>
        <source>(&lt;b&gt;Close Battlenet launcher before actions!&lt;/b&gt;)</source>
        <translation type="vanished">(&lt;b&gt;행동 전에 Battlenet 런처를 닫으세요!&lt;/b&gt;)</translation>
    </message>
    <message>
        <source>Output directory (leave empty to output to D2R path):</source>
        <translation type="vanished">출력 디렉토리 (

비워두면 D2R 경로로 출력):</translation>
    </message>
    <message>
        <source>Main</source>
        <translation type="vanished">메인</translation>
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
        <translation type="vanished">이 페이지에서는 가장 중요한 옵션을 선택할 수 있습니다:
- 입력 및 출력 데이터 위치 (D2 설치 - 레저렉티드 또는 레거시);
- 글로벌 설정으로 새 D2R 저장 폴더 생성;
- 모드를 실행할 바탕화면 바로가기 생성.
하지만 덜 중요한 기능들도 있습니다:
- 시드 번호 (최대 2^32)는 랜덤 생성 방식에 영향을 줍니다. 
시드가 변경되지 않으면 모든 생성이 매번 동일합니다.
시드를 수동으로 편집하거나 F5를 눌러 새 시드를 생성할 수 있습니다.
- 새로운 캐릭터 인벤토리에 키 추가 - 이제 모드 버전이 시작되었는지 확인할 수 있습니다.</translation>
    </message>
</context>
<context>
    <name>D2ModGen::MainWindow</name>
    <message>
        <source>Generate</source>
        <translation type="vanished">생성</translation>
    </message>
    <message>
        <source>Status label.</source>
        <translation type="vanished">상태 레이블.</translation>
    </message>
    <message>
        <source>Reset to default</source>
        <translation type="vanished">기본값으로 재설정</translation>
    </message>
    <message>
        <source>Enable this tab</source>
        <translation type="vanished">탭 활성화</translation>
    </message>
    <message>
        <source>Select preset...</source>
        <translation type="vanished">프리셋 선택</translation>
    </message>
    <message>
        <source>Do not know where to start? Select a preset:</source>
        <translation type="vanished">어디서 시작해야 할지 모르겠나요? 프리셋을 선택하세요:</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="vanished">파일</translation>
    </message>
    <message>
        <source>Actions</source>
        <translation type="vanished">행동</translation>
    </message>
    <message>
        <source>About</source>
        <translation type="vanished">정보</translation>
    </message>
    <message>
        <source>Save config...</source>
        <translation type="vanished">설정 저장</translation>
    </message>
    <message>
        <source>Load config...</source>
        <translation type="vanished">설정 불러오기</translation>
    </message>
    <message>
        <source>Clear config</source>
        <translation type="vanished">설정 지우기</translation>
    </message>
    <message>
        <source>Browse to settings folder</source>
        <translation type="vanished">설정 폴더로 이동</translation>
    </message>
    <message>
        <source>Quit without saving</source>
        <translation type="vanished">저장하지 않고 종료</translation>
    </message>
    <message>
        <source>Save and quit</source>
        <translation type="vanished">저장하고 종료</translation>
    </message>
    <message>
        <source>Generate mod</source>
        <translation type="vanished">모드 생성</translation>
    </message>
    <message>
        <source>Create seed</source>
        <translation type="vanished">시드 생성</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation type="vanished">실행 취소</translation>
    </message>
    <message>
        <source>Theme</source>
        <translation type="vanished">테마</translation>
    </message>
    <message>
        <source>Language</source>
        <translation type="vanished">언어</translation>
    </message>
    <message>
        <source>You need to restart for changes apply.</source>
        <translation type="vanished">변경 사항을 적용하려면 재시작해야 합니다.</translation>
    </message>
    <message>
        <source>Start...</source>
        <translation type="vanished">시작</translation>
    </message>
    <message>
        <source>Mod &apos;%1&apos; successfully updated (%2).</source>
        <translation type="vanished">모드 &apos;%1&apos;이(가) 성공적으로 업데이트되었습니다 (%2).</translation>
    </message>
</context>
<context>
    <name>D2ModGen::UIController</name>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="114"/>
        <source>Start...</source>
        <translation type="unfinished">시작</translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="122"/>
        <source>Saved.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="124"/>
        <source>Error when saving!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="135"/>
        <source>Loaded.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="137"/>
        <source>Error when loading!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="146"/>
        <source>Config is cleared.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="242"/>
        <source>Error: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="247"/>
        <source>Mod &apos;%1&apos; is updated.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="257"/>
        <source>Failed to locate Battle.net.config</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="264"/>
        <source>Failed to read data from Battle.net.config</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="275"/>
        <source>Failed to write data to Battle.net.config</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="279"/>
        <source>Battle.net launch options updated</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="295"/>
        <source>Failed to write shortcut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="299"/>
        <source>Shortcut created!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="315"/>
        <source>Settings updated!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/QtApp/UIController.cpp" line="334"/>
        <source>Save files copied (%1)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mod &apos;%1&apos; successfully updated (%2).</source>
        <translation type="obsolete">모드 &apos;%1&apos;이(가) 성공적으로 업데이트되었습니다 (%2).</translation>
    </message>
</context>
<context>
    <name>MainConfigPage</name>
    <message>
        <source>Use Diablo II legacy installation</source>
        <translation type="obsolete">Diablo II 레거시 설치 사용</translation>
    </message>
    <message>
        <source>Add key to new char inventory (Basic mod test)</source>
        <translation type="obsolete">새 캐릭터 인벤토리에 키 추가 (기본 모드 테스트)</translation>
    </message>
    <message>
        <source>Export all *.txt (for further manual edit)</source>
        <translation type="obsolete">모든 *.txt 내보내기 (추가 수동 편집용)</translation>
    </message>
    <message>
        <source>Copy Settings.json</source>
        <translation type="obsolete">Settings.json 복사</translation>
    </message>
    <message>
        <source>Set launch to mod</source>
        <translation type="obsolete">모드로 실행 설정</translation>
    </message>
    <message>
        <source>Reset launch to unmodded</source>
        <translation type="obsolete">모드 해제된 상태로 실행 초기화</translation>
    </message>
    <message>
        <source>Make shortcut on Desktop</source>
        <translation type="obsolete">바탕화면에 바로가기 만들기</translation>
    </message>
    <message>
        <source>Mod id:</source>
        <translation type="obsolete">모드 ID:</translation>
    </message>
    <message>
        <source>D2R path:</source>
        <translation type="obsolete">D2R 경로:</translation>
    </message>
    <message>
        <source>D2 legacy path:</source>
        <translation type="obsolete">D2 레거시 경로:</translation>
    </message>
    <message>
        <source>D2R save and user settings root:</source>
        <translation type="obsolete">D2R 저장 및 사용자 설정 루트:</translation>
    </message>
    <message>
        <source>(this will copy default settings to mod folder)</source>
        <translation type="obsolete">(기본 설정이 모드 폴더로 복사됩니다)</translation>
    </message>
    <message>
        <source>D2R command arguments (read-only):</source>
        <translation type="obsolete">D2R 명령 인수 (읽기 전용):</translation>
    </message>
    <message>
        <source>(&lt;b&gt;Close Battlenet launcher before actions!&lt;/b&gt;)</source>
        <translation type="obsolete">(&lt;b&gt;행동 전에 Battlenet 런처를 닫으세요!&lt;/b&gt;)</translation>
    </message>
    <message>
        <source>Output directory (leave empty to output to D2R path):</source>
        <translation type="obsolete">출력 디렉토리 (

비워두면 D2R 경로로 출력):</translation>
    </message>
    <message>
        <source>Main</source>
        <translation type="obsolete">메인</translation>
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
        <translation type="obsolete">이 페이지에서는 가장 중요한 옵션을 선택할 수 있습니다:
- 입력 및 출력 데이터 위치 (D2 설치 - 레저렉티드 또는 레거시);
- 글로벌 설정으로 새 D2R 저장 폴더 생성;
- 모드를 실행할 바탕화면 바로가기 생성.
하지만 덜 중요한 기능들도 있습니다:
- 시드 번호 (최대 2^32)는 랜덤 생성 방식에 영향을 줍니다. 
시드가 변경되지 않으면 모든 생성이 매번 동일합니다.
시드를 수동으로 편집하거나 F5를 눌러 새 시드를 생성할 수 있습니다.
- 새로운 캐릭터 인벤토리에 키 추가 - 이제 모드 버전이 시작되었는지 확인할 수 있습니다.</translation>
    </message>
</context>
<context>
    <name>MainMenuBar</name>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="54"/>
        <source>File</source>
        <translation type="unfinished">파일</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="57"/>
        <source>Save config...</source>
        <translation type="unfinished">설정 저장</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="62"/>
        <source>Load config...</source>
        <translation type="unfinished">설정 불러오기</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="67"/>
        <source>Clear config</source>
        <translation type="unfinished">설정 지우기</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="73"/>
        <source>Config from preset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="76"/>
        <source>Basic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="80"/>
        <source>Friendly chaos</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="84"/>
        <source>Magic rando</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="88"/>
        <source>Oh, no!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="93"/>
        <source>Browse to settings folder</source>
        <translation type="unfinished">설정 폴더로 이동</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="101"/>
        <source>Quit without saving</source>
        <translation type="unfinished">저장하지 않고 종료</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="109"/>
        <source>Save and quit</source>
        <translation type="unfinished">저장하고 종료</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="139"/>
        <source>Actions</source>
        <translation type="unfinished">행동</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="142"/>
        <source>Generate mod</source>
        <translation type="unfinished">모드 생성</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="147"/>
        <source>New seed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="152"/>
        <source>Undo</source>
        <translation type="unfinished">실행 취소</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="157"/>
        <source>Theme</source>
        <translation type="unfinished">테마</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="162"/>
        <source>Dark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="171"/>
        <source>Light</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="180"/>
        <source>Language</source>
        <translation type="unfinished">언어</translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="184"/>
        <source>English</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="189"/>
        <source>Russian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="194"/>
        <source>Korean</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="201"/>
        <source>Help</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/MainMenuBar.qml" line="203"/>
        <source>About</source>
        <translation type="unfinished">정보</translation>
    </message>
</context>
<context>
    <name>MainNavigation</name>
    <message>
        <location filename="../qml/MainNavigation.qml" line="10"/>
        <source>Main</source>
        <translation type="unfinished">메인</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="16"/>
        <source>Extra</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="22"/>
        <source>Randomizers</source>
        <translation type="unfinished">랜덤화 설정</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="28"/>
        <source>Item Randomizer</source>
        <translation type="unfinished">아이템 랜덤화</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="35"/>
        <source>Monster Randomizer</source>
        <translation type="unfinished">몬스터 랜덤화</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="42"/>
        <source>Skill Randomizer</source>
        <translation type="unfinished">스킬 랜덤화</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="49"/>
        <source>Make harder</source>
        <translation type="unfinished">난이도 설정</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="55"/>
        <source>Monster stats</source>
        <translation type="unfinished">몬스터 능력치</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="62"/>
        <source>Monster density</source>
        <translation type="unfinished">몬스터 밀집도</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="69"/>
        <source>Challenge</source>
        <translation type="unfinished">도전적 설정</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="76"/>
        <source>Make easier</source>
        <translation type="unfinished">편의성 설정</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="82"/>
        <source>Horadric Cube</source>
        <translation type="unfinished">호라드릭의 함</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="89"/>
        <source>Gambling</source>
        <translation type="unfinished">도박</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="96"/>
        <source>Character</source>
        <translation type="unfinished">캐릭터</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="103"/>
        <source>Increase Item Drops</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="110"/>
        <source>Increase Rune Drops</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="117"/>
        <source>Perfect Rolls</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="124"/>
        <source>Requirements</source>
        <translation type="unfinished">착용 요구사항</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="131"/>
        <source>Misc</source>
        <translation type="unfinished">기타</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="137"/>
        <source>Quality of life</source>
        <translation type="unfinished">삶의 질 개선</translation>
    </message>
    <message>
        <location filename="../qml/MainNavigation.qml" line="144"/>
        <source>Drops filtering</source>
        <translation type="unfinished">아이템 필터</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../qml/MainWindow.qml" line="8"/>
        <source>D2 mod generator by mapron - 0.7.0</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/MainWindow.qml" line="124"/>
        <source>Status label: Ready</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/MainWindow.qml" line="138"/>
        <source>Generate</source>
        <translation type="unfinished">생성</translation>
    </message>
</context>
<context>
    <name>ModuleChallenge</name>
    <message>
        <location filename="../qml/ModuleChallenge.qml" line="8"/>
        <source>Challenge</source>
        <translation>도전적 설정</translation>
    </message>
    <message>
        <location filename="../qml/ModuleChallenge.qml" line="9"/>
        <source>First, you can disable potion drops entirely (that&apos;s not the same as drop filter).&lt;br&gt;Second, you can change resistance penalty for each difficulty from default 0/40/100.&lt;br&gt;And the last, you can adjust level area on all maps; &lt;br&gt;note that max value of 85 is still used (so set to 20 to basically make all Hell areas lvl 85).</source>
        <translation>첫째, 물약 드롭을 완전히 비활성화할 수 있습니다 (드롭 필터와는 다릅니다).&lt;br&gt;둘째, 각 난이도에 대한 저항력 페널티를 기본값 0/40/100에서 변경할 수 있습니다.&lt;br&gt;마지막으로, 모든 맵의 지역 레벨을 조정할 수 있습니다;&lt;br&gt;최대값 85는 여전히 적용됩니다 (예: 20으로 설정하면 모든 헬 난이도 지역이 레벨 85가 됩니다).</translation>
    </message>
    <message>
        <location filename="../qml/ModuleChallenge.qml" line="12"/>
        <source>Disable drop: All Health pots</source>
        <translation>체력 포션 드롭 비활성화</translation>
    </message>
    <message>
        <location filename="../qml/ModuleChallenge.qml" line="19"/>
        <source>Disable drop: All Mana pots</source>
        <translation>마나 포션 드롭 비활성화</translation>
    </message>
    <message>
        <location filename="../qml/ModuleChallenge.qml" line="26"/>
        <source>Disable drop: Rejuv and Full Rejuv pots</source>
        <translation>활력 및 대량의 활력 물약 드롭 비활성화</translation>
    </message>
    <message>
        <location filename="../qml/ModuleChallenge.qml" line="33"/>
        <source>Normal difficulty resistance penalty, -all%</source>
        <translation>일반 난이도 저항력 감소 (%)</translation>
    </message>
    <message>
        <location filename="../qml/ModuleChallenge.qml" line="43"/>
        <source>Nightmare difficulty resistance penalty, -all%</source>
        <translation>악몽 난이도 저항력 감소 (%)</translation>
    </message>
    <message>
        <location filename="../qml/ModuleChallenge.qml" line="53"/>
        <source>Hell difficulty resistance penalty, -all%</source>
        <translation>지옥 난이도 저항력 감소 (%)</translation>
    </message>
    <message>
        <location filename="../qml/ModuleChallenge.qml" line="63"/>
        <source>Increase Nightmare area levels, +levels</source>
        <translation>악몽 난이도 지역 레벨 증가</translation>
    </message>
    <message>
        <location filename="../qml/ModuleChallenge.qml" line="73"/>
        <source>Increase Hell area levels, +levels</source>
        <translation>지옥 난이도 지역 레벨 증가</translation>
    </message>
    <message>
        <location filename="../qml/ModuleChallenge.qml" line="83"/>
        <source>Go beyond 85 level for areas</source>
        <translation>지역 레벨 85 초과 허용</translation>
    </message>
</context>
<context>
    <name>ModuleCharacter</name>
    <message>
        <location filename="../qml/ModuleCharacter.qml" line="8"/>
        <source>Character</source>
        <translation>캐릭터</translation>
    </message>
    <message>
        <location filename="../qml/ModuleCharacter.qml" line="9"/>
        <source>First three options allow you to change starting items for characters: &lt;br&gt;add Cube, replace id scroll with Tome (useful for No-town runs), and replace health with mana potions. &lt;br&gt;Next two sliders allow you to change how many skill and stat poits you get on levelup. &lt;br&gt;Last slider allow you to lower Strength and Dexterity requirements on all items. </source>
        <translation>처음 세 가지 옵션으로 캐릭터의 시작 아이템을 변경할 수 있습니다: &lt;br&gt;호라드릭의 함 추가, 아이템 식별 스크롤을 아이템 식별 책으로 교체(마을 방문 없이 플레이할 때 유용), 그리고 생명력 물약을 마나 물약으로 교체합니다. &lt;br&gt;다음 두 개의 슬라이더로 레벨업 시 얻는 스킬 포인트와 능력치 포인트의 양을 조절할 수 있습니다. &lt;br&gt;마지막 슬라이더로 모든 아이템의 힘과 민첩성 요구치를 낮출 수 있습니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleCharacter.qml" line="12"/>
        <source>Add Horadric Cube as starting item</source>
        <translation>시작 아이템으로 호라드릭의 함 추가</translation>
    </message>
    <message>
        <location filename="../qml/ModuleCharacter.qml" line="19"/>
        <source>Replace ID scroll with ID tome</source>
        <translation>아이템 식별 스크롤을 아이템 식별 책으로 교체</translation>
    </message>
    <message>
        <location filename="../qml/ModuleCharacter.qml" line="26"/>
        <source>Replace starting Health pots with Mana</source>
        <translation>시작 시 생명력 물약을 마나 물약으로 교체</translation>
    </message>
    <message>
        <location filename="../qml/ModuleCharacter.qml" line="33"/>
        <source>Add Teleport as starting skill for all classes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/ModuleCharacter.qml" line="40"/>
        <source>Stat points gain per level</source>
        <translation>레벨 당 획득하는 능력치 포인트</translation>
    </message>
    <message>
        <location filename="../qml/ModuleCharacter.qml" line="50"/>
        <source>Skill points gain per level (D2R only!)</source>
        <translation>레벨 당 획득하는 스킬 포인트 (디아블로 2 레저렉션 전용)</translation>
    </message>
    <message>
        <location filename="../qml/ModuleCharacter.qml" line="60"/>
        <source>Maximum hard points for skill (D2R only!)</source>
        <translation>스킬당 최대 포인트 수 (디아블로 2 레저렉션 전용)</translation>
    </message>
    <message>
        <location filename="../qml/ModuleCharacter.qml" line="70"/>
        <source>Change mercenaries Health, multiply by</source>
        <translation>용병의 생명력 변경 배율</translation>
    </message>
    <message>
        <location filename="../qml/ModuleCharacter.qml" line="81"/>
        <source>Change mercenaries Damage, multiply by</source>
        <translation>용병의 공격력 변경 배율</translation>
    </message>
</context>
<context>
    <name>ModuleCube</name>
    <message>
        <location filename="../qml/ModuleCube.qml" line="8"/>
        <source>Horadric Cube</source>
        <translation>호라드릭의 함</translation>
    </message>
    <message>
        <location filename="../qml/ModuleCube.qml" line="9"/>
        <source>Create some recipes to cheese/cut corners around the game. &lt;br&gt;Input and results are self-explanatory.</source>
        <translation>게임 진행을 용이하게 하는 레시피들을 추가합니다.&lt;br&gt;재료와 결과물은 직관적으로 이해할 수 있습니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleCube.qml" line="12"/>
        <source>Remove gem component from Rune upgrade recipes (Rune x3,x2 =&amp;gt; next Rune)</source>
        <translation>룬 업그레이드 시 보석 요구사항 제거 (룬 3개 또는 2개 =&gt; 상위 룬)</translation>
    </message>
    <message>
        <source>Add quick portal access recipes:</source>
        <translation type="vanished">빠른 포털 생성 레시피 추가:</translation>
    </message>
    <message>
        <source>Add quick quest recipes:</source>
        <translation type="vanished">퀘스트 아이템 제작 레시피 추가:</translation>
    </message>
    <message>
        <source>Add socketing recipes:</source>
        <translation type="vanished">소켓 추가 레시피 추가:</translation>
    </message>
    <message>
        <source>Add upgrade recipes:</source>
        <translation type="vanished">아이템 등급 상승 레시피 추가:</translation>
    </message>
    <message>
        <location filename="../qml/ModuleCube.qml" line="19"/>
        <source>Add quick portal access recipes:&lt;br&gt;1. TP book + Id scroll = Cow Portal&lt;br&gt;2. TP book + Id scroll x2 = Minor Uber Portal&lt;br&gt;3. TP book + Id scroll x3 = Uber Tristram Portal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/ModuleCube.qml" line="26"/>
        <source>Add quick quest recipes:&lt;br&gt;1. key + Id scroll = Horadric Staff&lt;br&gt;2. key + TP scroll = Khalim Will</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/ModuleCube.qml" line="33"/>
        <source>Add socketing recipes:&lt;br&gt;1. Normal item + TP scroll x1 + Id scroll x1 = Add 3 sockets&lt;br&gt;2. Normal item + TP scroll x1 + Id scroll x2 = Add 4 sockets&lt;br&gt;3. Normal item + TP scroll x2 + Id scroll x1 = Add 5 sockets&lt;br&gt;4. Normal item + TP scroll x2 + Id scroll x2 = Add 6 sockets&lt;br&gt;5. Unique/Set/Rare item + TP scroll x1 + Id scroll x1 = Add 1 socket&lt;br&gt;6. Socketed item + TP scroll x1 = Clear sockets</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/ModuleCube.qml" line="40"/>
        <source>Add upgrade recipes:&lt;br&gt;1. Normal item (normal,magic,rare,unique,set) + Antidote = Exceptional item&lt;br&gt;2. Exceptional item (normal,magic,rare,unique,set) + Antidote = Elite item&lt;br&gt;3. Any item + Stamina x2 = Add Ethereal&lt;br&gt;4. Magic (magic,rare,unique,set) item  + Stamina = Normal item of same type</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/ModuleCube.qml" line="47"/>
        <source>Add cheat Small Charm recipies:&lt;br&gt;1. Any small charm + TP scroll x1 + one of (h pot, mana pot, stamina, antidote, thawing)&lt;br&gt;2. Any small charm + Id scroll x1 + one of (h pot, mana pot, stamina, antidote, thawing)&lt;br&gt;3. Any small charm + key       x1 + one of (h pot, mana pot, stamina, antidote, thawing)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/ModuleCube.qml" line="48"/>
        <source>Different combinations provide one of 10 stat combinations.&lt;br&gt;Just two charms of each kind suppose to provide reasonable endgame stat.&lt;br&gt;Item level is 1, so try several times to craft charm with 0 requirements.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/ModuleCube.qml" line="54"/>
        <source>Simplyfy crafting:</source>
        <translation>크래프팅 간소화:</translation>
    </message>
    <message>
        <location filename="../qml/ModuleCube.qml" line="58"/>
        <source>Remove strict item types for Crafted (any helm, any gloves etc)</source>
        <translation>크래프트 아이템 제작 시 아이템 종류 제한 해제 (모든 투구, 모든 장갑 등)</translation>
    </message>
    <message>
        <location filename="../qml/ModuleCube.qml" line="65"/>
        <source>Remove Rune/Jewel requirement for Crafted&lt;br&gt;(So any recipe is &apos;item + gem&apos;)</source>
        <translation>크래프트 시 룬/주얼 요구사항 제거&lt;br&gt;(모든 레시피가 &apos;아이템 + 보석&apos;으로 변경)</translation>
    </message>
    <message>
        <location filename="../qml/ModuleCube.qml" line="72"/>
        <source>Make Item Level always 99</source>
        <translation>크래프트 아이템 레벨을 항상 99로 설정</translation>
    </message>
</context>
<context>
    <name>ModuleDropFiltering</name>
    <message>
        <location filename="../qml/ModuleDropFiltering.qml" line="8"/>
        <source>Drops filtering</source>
        <translation>아이템 필터</translation>
    </message>
    <message>
        <location filename="../qml/ModuleDropFiltering.qml" line="9"/>
        <source>This tab consists of two sections:&lt;br&gt;1. Make some items have compact names, like &amp;quot;!MP5&amp;quot; for Super Mana Potion. &lt;br&gt;2. Hide item labels on the ground (you still be able to pick them). </source>
        <translation>이 탭은 두 가지 섹션으로 구성되어 있습니다:&lt;br&gt;1. 일부 아이템의 이름을 간단하게 만듭니다. 예: 상급 마나 물약을 &apos;!MP5&apos;로 표시.&lt;br&gt;2. 바닥에 떨어진 아이템의 이름을 숨깁니다 (여전히 줍기 가능).</translation>
    </message>
    <message>
        <location filename="../qml/ModuleDropFiltering.qml" line="12"/>
        <source>&lt;b&gt;Make item names compact&lt;/b&gt;: this will make item names take less space, e.g. &apos;!HP2&apos; for health potion.</source>
        <translation>&lt;b&gt;아이템 이름 간소화&lt;/b&gt;: 아이템 이름이 차지하는 공간을 줄입니다. 예: 체력 물약을 &apos;!HP2&apos;로 표시</translation>
    </message>
    <message>
        <location filename="../qml/ModuleDropFiltering.qml" line="16"/>
        <source>Compact potion names</source>
        <translation>물약 이름 간소화</translation>
    </message>
    <message>
        <location filename="../qml/ModuleDropFiltering.qml" line="23"/>
        <source>Compact TP/ID scrolls</source>
        <translation>마을 포털/아이템 식별 스크롤 간소화</translation>
    </message>
    <message>
        <location filename="../qml/ModuleDropFiltering.qml" line="30"/>
        <source>&lt;b&gt;Hide items on the ground&lt;/b&gt;: this will make item names transparent; &lt;br&gt;you still can pickup them, but their labels will be invisible on Alt press.</source>
        <translation>&lt;b&gt;바닥의 아이템 숨기기&lt;/b&gt;: 아이템 이름을 투명하게 만듭니다. &lt;br&gt;여전히 주울 수 있지만, Alt 키를 눌렀을 때 이름이 보이지 않습니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleDropFiltering.qml" line="34"/>
        <source>ID scroll</source>
        <translation>아이템 식별 스크롤</translation>
    </message>
    <message>
        <location filename="../qml/ModuleDropFiltering.qml" line="41"/>
        <source>TP scroll</source>
        <translation>마을 포털 스크롤</translation>
    </message>
    <message>
        <location filename="../qml/ModuleDropFiltering.qml" line="48"/>
        <source>Health pots 1-3</source>
        <translation>하급/중급/상급 체력 물약</translation>
    </message>
    <message>
        <location filename="../qml/ModuleDropFiltering.qml" line="55"/>
        <source>All Health pots</source>
        <translation>모든 체력 물약</translation>
    </message>
    <message>
        <location filename="../qml/ModuleDropFiltering.qml" line="62"/>
        <source>Mana pots 1-3</source>
        <translation>하급/중급/상급 마나 물약</translation>
    </message>
    <message>
        <location filename="../qml/ModuleDropFiltering.qml" line="69"/>
        <source>All Mana pots</source>
        <translation>모든 마나 물약</translation>
    </message>
    <message>
        <location filename="../qml/ModuleDropFiltering.qml" line="76"/>
        <source>Rejuv pots</source>
        <translation>회복 물약</translation>
    </message>
    <message>
        <location filename="../qml/ModuleDropFiltering.qml" line="83"/>
        <source>Full Rejuv pots</source>
        <translation>대량의 활력 물약</translation>
    </message>
    <message>
        <location filename="../qml/ModuleDropFiltering.qml" line="90"/>
        <source>Bolts/Arrows</source>
        <translation>화살/볼트</translation>
    </message>
    <message>
        <location filename="../qml/ModuleDropFiltering.qml" line="97"/>
        <source>Stamina/Antidote/Thawing</source>
        <translation>체력 회복/해독/해동 물약</translation>
    </message>
    <message>
        <location filename="../qml/ModuleDropFiltering.qml" line="104"/>
        <source>Keys,Fire/Poison pots</source>
        <translation>열쇠, 폭발/독 물약</translation>
    </message>
    <message>
        <location filename="../qml/ModuleDropFiltering.qml" line="111"/>
        <source>Hide low quality/damaged/cracked items</source>
        <translation>질 낮은/손상된/파손된 아이템 숨기기</translation>
    </message>
</context>
<context>
    <name>ModuleGambling</name>
    <message>
        <location filename="../qml/ModuleGambling.qml" line="8"/>
        <source>Gambling</source>
        <translation>도박</translation>
    </message>
    <message>
        <location filename="../qml/ModuleGambling.qml" line="9"/>
        <source>First option enables Charms in Gambling window, but only for D2 Legacy.&lt;br&gt;Next sliders allow you increase basic chance of getting Unique/Rare/Set/High-quality item.</source>
        <translation>첫 번째 옵션은 디아블로 2 클래식 버전에서만 도박 창에서 부적(Charm)을 얻을 수 있게 합니다.&lt;br&gt;슬라이더를 통해 유니크/레어/세트/고품질 아이템을 얻을 기본 확률을 높일 수 있습니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleGambling.qml" line="12"/>
        <source>Allow gambling for charms and jewels (Legacy only)&lt;br&gt;Note: crashes D2R - so enabled only for D2.</source>
        <translation>도박으로 부적과 보석을 얻을 수 있게 함 (클래식 버전 전용)&lt;br&gt;주의: 디아블로 2 레저렉션에서는 게임 충돌이 발생하므로 클래식 버전에서만 활성화됩니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleGambling.qml" line="19"/>
        <source>Increase Unique chance, times&lt;br&gt;Note: you still can get a lot of failed uniques for no known reason.</source>
        <translation>유니크 아이템 출현 확률 증가 배율&lt;br&gt;참고: 알 수 없는 이유로 여전히 많은 유니크 아이템 생성에 실패할 수 있습니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleGambling.qml" line="29"/>
        <source>Increase Set chance, times</source>
        <translation>세트 아이템 출현 확률 증가 배율</translation>
    </message>
    <message>
        <location filename="../qml/ModuleGambling.qml" line="39"/>
        <source>Increase Rare chance, times</source>
        <translation>레어 아이템 출현 확률 증가 배율</translation>
    </message>
    <message>
        <location filename="../qml/ModuleGambling.qml" line="49"/>
        <source>Increase Exceptional upgrade chance, times</source>
        <translation>익셉셔널 등급 업그레이드 확률 증가 배율</translation>
    </message>
    <message>
        <location filename="../qml/ModuleGambling.qml" line="59"/>
        <source>Increase Elite upgrade chance, times&lt;br&gt;Note: seems like Exc. check is done beforehead, so if it&amp;apos;s high enough, elite won&amp;apos;t generate.</source>
        <translation>엘리트 등급 업그레이드 확률 증가 배율&lt;br&gt;참고: 익셉셔널 등급 체크가 먼저 이루어지는 것 같아, 그 확률이 높으면 엘리트 등급 아이템이 생성되지 않을 수 있습니다.</translation>
    </message>
</context>
<context>
    <name>ModuleItemDrops</name>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="8"/>
        <source>Increase Item drops</source>
        <translation>아이템 드롭 증가</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="9"/>
        <source>All help and explanation on this page assume you are familiar with Treasure Class (TC) and NoDrop terminology.&lt;br&gt;Basically all options here modifying TC pick weights - so one preferred option become less rare.&lt;br&gt;First 3 sliders are for increasing chance for Uniques/Sets/Rares. &lt;br&gt;Note that increase is accurate when your chances are low, but with high MF it can have diminishing return.</source>
        <translation>이 페이지의 모든 도움말과 설명은 보물 등급(TC)과 NoDrop 개념에 익숙하다는 가정 하에 작성되었습니다.&lt;br&gt;기본적으로 여기 있는 모든 옵션은 TC 선택 가중치를 조정합니다 - 즉, 선호되는 옵션의 희귀도가 낮아집니다.&lt;br&gt;첫 3개의 슬라이더는 유니크/세트/레어 아이템의 드롭 확률을 증가시킵니다.&lt;br&gt;확률이 낮을 때는 증가폭이 정확하지만, MF가 높을 경우에는 효과가 점차 감소할 수 있습니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="12"/>
        <source>Select preset...</source>
        <translation type="unfinished">프리셋 선택</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="12"/>
        <source>I want to find Tyrael&apos;s today!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="12"/>
        <source>Just tired of finding my first Shako</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="12"/>
        <source>A little bit more of good stuff, not crazy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="17"/>
        <source>Increase Unique Chance</source>
        <translation>유니크 아이템 확률 증가</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="27"/>
        <source>Increase Set Chance</source>
        <translation>세트 아이템 확률 증가</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="37"/>
        <source>Increase Rare Chance</source>
        <translation>레어 아이템 확률 증가</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="47"/>
        <source>Increase Chance of High-level equipment</source>
        <translation>고레벨 장비 확률 증가</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="48"/>
        <source>This will reduce the reduce the chance that equipment of lower level will drop as a fallback.&lt;br&gt;For example, for level 85 TC there is a 98% chance that it will drop from 84 level TC, and so on.</source>
        <translation>이 설정은 고레벨 장비 드롭 시&lt;br&gt;낮은 레벨 장비 드롭 확률을 줄입니다.&lt;br&gt;예: 85레벨 TC의 경우 84레벨 TC에서 드롭될 확률이 98%입니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="57"/>
        <source>Increase Champion/Unique item count</source>
        <translation>챔피언/유니크 몬스터 아이템 드롭 개수 증가</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="58"/>
        <source>This will make Unique bosses to drop 4 items instead of just 1, and Champions drop 2 items instead of one.&lt;br&gt;Note that potion drops are slightly reduced.</source>
        <translation>이 설정으로 유니크 보스는 1개 대신 4개의 아이템을, 챔피언은 1개 대신 2개의 아이템을 드롭합니다.&lt;br&gt;물약 드롭 수가 약간 줄어듭니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="64"/>
        <source>Always quest drops from bosses</source>
        <translation>보스가 항상 퀘스트 아이템 드롭</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="71"/>
        <source>Adjust proportion of main drop classes</source>
        <translation>주요 드롭 클래스의 비율 조정</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="75"/>
        <source>NoDrop (higher=LESS drops)</source>
        <translation>노드롭 (높을수록 드롭률 낮음)</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="76"/>
        <source>NoDrop slider provides ability to gradually reduce NoDrop picks&lt;br&gt;(it basically similar to increasing players count in the game)</source>
        <translation>슬라이더로 노드롭 선택을 점진적으로 줄일 수 있습니다.&lt;br&gt;(게임 내 플레이어 수를 증가시키는 것과 유사한 효과)</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="86"/>
        <source>Good TC (Runes/Gems/Jewellery)</source>
        <translation>룬/보석/장신구 드롭률 설정</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="87"/>
        <source>That will make Gems/Runes/Rings/Amulets/Charms drop far more often&lt;br&gt;compared to equipment (armor/weapons).</source>
        <translation>이 설정은 보석/룬/반지/목걸이/부적이&lt;br&gt;장비(방어구/무기)보다 훨씬 더 자주 드롭되도록 합니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="97"/>
        <source>Equipment (armor/weapons)</source>
        <translation>무기/방어구 드롭률 설정</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="98"/>
        <source>Relative chance of equipment dropping compared to other items (or NoDrop).</source>
        <translation>다른 아이템(또는 NoDrop)에 비해 장비가 드롭될 확률입니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="108"/>
        <source>Gold (higher=MORE gold)</source>
        <translation>골드 드롭량 설정</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="109"/>
        <source>Relative chance of gold compared to other items.</source>
        <translation>다른 아이템에 비해 골드가 드롭될 확률입니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="119"/>
        <source>Junk (keys/bolts/etc) (higher=MORE junk)</source>
        <translation>열쇠/화살 등의 잡화</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="120"/>
        <source>Relative chance of junk items compared to other items.</source>
        <translation>다른 아이템에 비해 잡화가 드롭될 확률입니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="130"/>
        <source>Make all Uniques have equal rarity on same base</source>
        <translation>동일한 기본 아이템에서 모든 유니크가 같은 희귀도를 가짐</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemDrops.qml" line="131"/>
        <source>Now Uniques with equal item base will have equal chance to drop.&lt;br&gt;For example Tyrael&apos;s and Templar&apos;s will have equal chance. (and all rings too)</source>
        <translation>이제 같은 기본 아이템을 가진 유니크 아이템은 동일한 확률로 드롭됩니다.&lt;br&gt;예: 티리엘의 권능과 템플러의 갑옷이 동일한 확률로 드롭됩니다. (모든 유니크 반지도 마찬가지)</translation>
    </message>
</context>
<context>
    <name>ModuleItemRandomizer</name>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="8"/>
        <source>Item Randomizer</source>
        <translation>아이템 랜덤화</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="9"/>
        <source>What item randomizer does in short - it reads all possible item properties from Uniques, Sets, etc,&lt;br&gt;And then reassign properties back, but in random order (also it does not mean every original will be used)&lt;br&gt;For details, check descriptions of every option.</source>
        <translation>유니크, 세트 아이템 등의 아이템 옵션을 읽어들인 다음,&lt;br&gt;이를 무작위로 선택해 다시 할당합니다.&lt;br&gt;이 과정에서 원래 아이템이 가진 모든 옵션이 반드시 사용되는 것은 아닙니다.&lt;br&gt;자세한 내용은 각 항목의 설명을 확인하세요.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="12"/>
        <source>Select preset...</source>
        <translation type="unfinished">프리셋 선택</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="12"/>
        <source>I want to be overpowered machine!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="12"/>
        <source>Want to have some fun without flying to space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="12"/>
        <source>Want to have fresh experience but balanced if possible</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="17"/>
        <source>Crazy-ness (or &apos;NON-balance level&apos;, lower = more balance, 100=chaos)</source>
        <translation>랜덤화 강도 (밸런스 조절: 0 = 기본값. 100 = 완전 랜덤)</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="18"/>
        <source>Crazyness level - determine level difference to be used when selecting new properties for item/rune/etc.&lt;br&gt;With &apos;10&apos; it will select between level-10 and level+10 at first, if there are &lt;50 candidates,&lt;br&gt;then it will select level-30..level+30, and finally it will try fully random. &lt;br&gt;In short, lower value = more balance in terms of original affix level and item level.</source>
        <translation>아이템과 룬의 옵션 종류와 수치의 변화 폭을 결정합니다.&lt;br&gt;낮은 값은 원래 아이템 레벨에 가까운 옵션을, 높은 값은 더 넓은 범위의 옵션을 생성합니다.&lt;br&gt;값이 낮을수록 게임 밸런스가 유지되며, 높을수록 예측 불가능한 조합이 나올 수 있습니다.&lt;br&gt;예: 10이면 아이템 레벨 ±10 범위, 30이면 ±30 범위에서 옵션을 선택합니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="27"/>
        <source>Item type fit percent (0% = fully random, 100% = all according to item type)</source>
        <translation>아이템 유형 일치율 (0% = 완전 랜덤, 100% = 모두 아이템 타입에 맞음)</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="28"/>
        <source>Item fit slider allow you to select how much item affixes will be related to original item type.&lt;br&gt;For example, if you choose 80%, then 4 of 5 affixes will be selected to pool for specific item type&lt;br&gt;Item can have have several pools related to its type - say, scepter is a rod and a melee weapon.&lt;br&gt;Item type-specific properties will be picked in proportion to all types.</source>
        <translation>아이템 옵션이 원래 아이템 유형과 얼마나 일치할지 결정합니다.&lt;br&gt;높은 값은 아이템 유형에 맞는 옵션을, 낮은 값은 더 다양한 옵션을 생성합니다.&lt;br&gt;예: 80%면 5개 중 4개의 옵션이 해당 아이템 유형에 적합합니다.&lt;br&gt;아이템은 여러 유형에 속할 수 있습니다. 예를 들어, 홀 지팡이는 지팡이이면서 근접 무기입니다.&lt;br&gt;옵션은 아이템의 모든 해당 유형을 고려하여 선택됩니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="37"/>
        <source>How many original properties to keep, percent</source>
        <translation>원래 옵션 유지 비율(%)</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="38"/>
        <source>You can select how many properties of original item you want to keep.&lt;br&gt;If 0, then every item will be fully randomized.&lt;br&gt;If 50, then half of genereted properties will be original, and half randomized.&lt;br&gt;If 100, then every property will be property of original item. (you can reduce an amount of props)</source>
        <translation>원래 아이템의 옵션 종류와 수치를 얼마나 유지할지 결정합니다.&lt;br&gt;0%는 완전 랜덤화, 50%는 절반 유지, 100%는 모든 원래 옵션을 유지합니다.&lt;br&gt;옵션의 개수는 이 설정과 별개로 변경될 수 있습니다.&lt;br&gt;유지된 옵션의 수치는 랜덤화 강도 설정에 따라 변경될 수 있습니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="47"/>
        <source>Relative property counts (min/max), compared to non-randomized original:</source>
        <translation>기본값 대비 옵션 개수 비율:</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="51"/>
        <source>Uniques/Gems/Runes/Affixes, Min</source>
        <translation>유니크/보석/룬/접두사, 최소</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="52"/>
        <source>This and the next options determine new property count will be after generation.&lt;br&gt;If Min=Max=100%%, then property count will be exactly as original (except rare corner cases).&lt;br&gt;If Min=50%% and Max=200%%, then new property count will be at least half as original, and twice at best.&lt;br&gt;For example, if item has 5 properties, then worst case is 2, and best case is 9 (because 9 is maximum for Uniques)</source>
        <translation>새로 생성되는 아이템의 옵션 개수를 원본 대비 비율로 설정합니다.&lt;br&gt;최소값과 최대값을 조절하여 옵션 수의 범위를 지정할 수 있습니다.&lt;br&gt;예: 최소 50%, 최대 200%면 원본의 절반에서 두 배 사이의 옵션 개수가 생성됩니다.&lt;br&gt;이는 옵션의 종류나 수치가 아닌 개수에만 영향을 줍니다.&lt;br&gt;유니크 아이템의 최대 옵션 개수는 9개로 제한됩니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="62"/>
        <source>Uniques/Gems/Runes/Affixes, Max</source>
        <translation>유니크/보석/룬/접두사, 최대</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="73"/>
        <source>Set items, Min</source>
        <translation>세트 아이템, 최소</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="84"/>
        <source>Set items, Max</source>
        <translation>세트 아이템, 최대</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="95"/>
        <source>Number of versions of each unique</source>
        <translation>변형된 유니크 아이템 개수</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="96"/>
        <source>allow you to have different uniques with same name and level, but different properties,&lt;br&gt;you will have N different uniques with differnet stats;&lt;br&gt;so you have an opportunity to pick same item again to check it out.&lt;br&gt;This works only with Uniques, not Sets.</source>
        <translation>같은 유니크 아이템을 여러 버전으로 생성합니다.&lt;br&gt;이름과 레벨은 같지만 다른 옵션 종류와 수치를 가진 아이템들이 생성되어 다양성이 증가합니다.&lt;br&gt;이를 통해 같은 유니크 아이템을 여러 번 찾아 비교해볼 수 있습니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="105"/>
        <source>Number of different incompatible Sets</source>
        <translation>변형된 세트 아이템 개수</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="106"/>
        <source>Whis will create additional sets (Angelic 2, Angelic 3) with different properties.</source>
        <translation>같은 세트 아이템을 여러 버전으로 생성합니다.&lt;br&gt;예: 값이 3이면 &apos;천사의 세트 1&apos;, &apos;천사의 세트 2&apos;, &apos;천사의 세트 3&apos;과 같이 동일한 세트의 서로 다른 버전이 생성됩니다.&lt;br&gt;각 변형은 고유한 옵션과 세트 효과를 가집니다.&lt;br&gt;이를 통해 같은 세트 아이템을 여러 가지 다른 버전으로 경험할 수 있으며, 게임의 다양성이 증가합니다.&lt;br&gt;주의: 서로 다른 변형의 세트 아이템은 함께 세트 효과를 발동시키지 않습니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="115"/>
        <source>Prevent duplicate properties on items</source>
        <translation>아이템에 중복 속성 방지</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="122"/>
        <source>Randomize magix/rare affixes</source>
        <translation>매직/레어 접두사 랜덤화</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="123"/>
        <source>This will modify rare and magic suffixes - &lt;br&gt;so they can include properties of any other item in the game. &lt;br&gt;Note that their properties are read even without this option.</source>
        <translation>매직과 레어 아이템의 접두사/접미사 옵션을 수정합니다.&lt;br&gt;게임 내 다른 아이템의 옵션들도 포함될 수 있어 더 다양한 조합이 가능해집니다.&lt;br&gt;이 옵션을 끄더라도 접두사/접미사 옵션은 랜덤화 과정에서 참고됩니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="129"/>
        <source>Randomize gem and runes properties</source>
        <translation>보석/룬 속성 랜덤화</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="130"/>
        <source>This will modify gem and rune properties - &lt;br&gt;so they can include properties of any other item in the game. &lt;br&gt;Note that their properties are read even without this option.</source>
        <translation>보석과 룬의 옵션 종류와 수치를 수정합니다.&lt;br&gt;게임 내 다른 아이템의 옵션들도 포함될 수 있어 더 다양한 조합이 가능해집니다.&lt;br&gt;이 옵션을 끄더라도 보석과 룬의 옵션은 랜덤화 과정에서 참고됩니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="136"/>
        <source>Replace skills with oskills</source>
        <translation>기존 스킬 옵션을 타 클래스 스킬(oskills)로 변경</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="143"/>
        <source>Replace charges with oskills</source>
        <translation>충전 스킬 옵션을 타 클래스 스킬(oskills)로 변경</translation>
    </message>
    <message>
        <location filename="../qml/ModuleItemRandomizer.qml" line="150"/>
        <source>Remove Knockback/Monster flee</source>
        <translation>밀어내기/몬스터 도주 제거</translation>
    </message>
    <message>
        <source>Add extra attributes to randomize (comma-separated)</source>
        <translation type="vanished">랜덤화할 추가 속성 추가 (쉼표로 구분)</translation>
    </message>
</context>
<context>
    <name>ModuleMonDensity</name>
    <message>
        <location filename="../qml/ModuleMonDensity.qml" line="8"/>
        <source>Monster density</source>
        <translation>몬스터 밀집도</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonDensity.qml" line="9"/>
        <source>Density slider - that will affect increase of density of regular monsters &lt;br&gt;(note that it is higly dependant on map and tiles; max density can be reached on open area tiles.)&lt;br&gt;Boss packs slider icrease number of Champion/Uniques in zones; again, that also not a precise setting.&lt;br&gt;Last Checkbox can be used to quickly setup boss count on Normal and Nightmare exactly equal to Hell count.</source>
        <translation>밀집도 슬라이더: 일반 몬스터의 출현 빈도를 조절합니다.&lt;br&gt;(주의: 맵과 지형에 따라 효과가 다릅니다. 넓은 개방 지역에서 최대 효과를 볼 수 있습니다.)&lt;br&gt;보스군 슬라이더: 구역 내 챔피언/유니크 몬스터의 수를 증가시킵니다. 이 역시 정확한 수치 조절은 아닙니다.&lt;br&gt;마지막 체크박스: 일반과 악몽 난이도의 보스 수를 지옥 난이도와 동일하게 설정합니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonDensity.qml" line="12"/>
        <source>Increase density, times</source>
        <translation>일반 몬스터 밀집도 증가 배율</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonDensity.qml" line="22"/>
        <source>Increase Boss packs count, times</source>
        <translation>보스군 수 증가 배율</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonDensity.qml" line="32"/>
        <source>Increase monster groups population, +count&lt;br&gt;&lt;b&gt;Beware! This setting have the most impact on getting screen laggy!&lt;b&gt;&lt;br&gt;&lt;b&gt;Having value above +3 will work well only if previous options are not maxed!&lt;/b&gt;</source>
        <translation>몬스터 그룹 크기 증가 (+수)&lt;br&gt;&lt;b&gt;주의! 이 설정은 게임 성능에 가장 큰 영향을 줍니다!&lt;b&gt;&lt;br&gt;&lt;b&gt;+3 이상의 값은 위 옵션들이 최대가 아닐 때만 원활히 작동합니다!&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonDensity.qml" line="42"/>
        <source>Use Hell elite pack count on Nightmare and Normal</source>
        <translation>일반/악몽 난이도에 지옥 난이도의 엘리트 무리 수 적용</translation>
    </message>
</context>
<context>
    <name>ModuleMonRandomizer</name>
    <message>
        <location filename="../qml/ModuleMonRandomizer.qml" line="8"/>
        <source>Monster Randomizer</source>
        <translation>몬스터 랜덤화</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonRandomizer.qml" line="9"/>
        <source>When this feature enabled, now any area in the game can spawn any basic monsters (Super Uniques, Act bosses etc not affected).&lt;br&gt;For normal difficulty, each monster will have a &apos;level-adjusted&apos; copy which corresponds&lt;br&gt;to area&apos;s level (so you will have adequate monster stats in Act 1 for guest from Act 5).&lt;br&gt;For Nightmare/Hell, adjustment is done by area level.&lt;br&gt;Monster will have adjusted their minion spawns and skill levels, too.</source>
        <translation>이 기능을 켜면 모든 지역에 모든 종류의 일반 몬스터가 등장할 수 있습니다 (슈퍼 유니크, 액트 보스 제외).&lt;br&gt;일반 난이도: 몬스터가 현재 등장한 지역의 레벨에 맞춰 능력치가 조정됩니다.&lt;br&gt;예: 액트 5의 몬스터가 액트 1에 나타나도 액트 1 레벨에 맞는 능력치를 가집니다.&lt;br&gt;악몽/지옥 난이도: 몬스터의 능력치가 단순히 현재 지역의 레벨에 맞춰 조정됩니다.&lt;br&gt;몬스터의 하수인 생성과 스킬 레벨도 함께 조정됩니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonRandomizer.qml" line="12"/>
        <source>Random monster spawn in areas</source>
        <translation>랜덤 몬스터 생성</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonRandomizer.qml" line="19"/>
        <source>Spawned different types in one area</source>
        <translation>한 지역 내 출현 몬스터 종류 수</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonRandomizer.qml" line="20"/>
        <source>This option choose how many different types will spawn at once (default is 3, which is D2 original behavior).&lt;br&gt;Note that some map tiles have predefined spawns (shaman camps etc) that can not be randomized.</source>
        <translation>한 번에 출현할 몬스터 종류의 수를 설정합니다 (기본값 3, 원작과 동일).&lt;br&gt;주의: 일부 지역(예: 샤먼 캠프)은 미리 정해진 몬스터가 출현하여 랜덤화되지 않습니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonRandomizer.qml" line="29"/>
        <source>Maximum possible monster types in one area (population variety)</source>
        <translation>한 지역 최대 몬스터 종류 수 (다양성)</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonRandomizer.qml" line="30"/>
        <source>This option defines how many possibilites for monster types are exist in one zone;&lt;br&gt;Make this value lower if you want less variety between game restarts.&lt;br&gt;Note: D2 legacy support only 12 max, so value will be lowered implicitly.</source>
        <translation>한 지역에 등장 가능한 몬스터 종류의 최대 수를 설정합니다.&lt;br&gt;게임 재시작 시 다양성을 줄이려면 이 값을 낮게 설정하세요.&lt;br&gt;주의: 원작은 최대 12종까지만 지원하므로, 그 이상으로 설정하면 자동으로 12로 조정됩니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonRandomizer.qml" line="39"/>
        <source>Random resistances</source>
        <translation>랜덤 저항력 부여</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonRandomizer.qml" line="46"/>
        <source>Use Hell resistances on NM/Normal</source>
        <translation>악몽/일반 난이도에서 지옥 난이도 저항력 사용</translation>
    </message>
</context>
<context>
    <name>ModuleMonStats</name>
    <message>
        <location filename="../qml/ModuleMonStats.qml" line="8"/>
        <source>Monster stats</source>
        <translation>몬스터 능력치</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonStats.qml" line="9"/>
        <source>Sliders allow you to change monster stats on all difficulties.&lt;br&gt;You can either lower or raise stats up 10x or 10x times.&lt;br&gt;That is orthogonal to /playersX setting - it will multiply stats independent.</source>
        <translation>슬라이더를 사용하여 모든 난이도의 몬스터 능력치를 조정할 수 있습니다.&lt;br&gt;능력치를 0.1배에서 10배까지 조정할 수 있습니다.&lt;br&gt;이 설정은 /players X 명령어와 독립적으로 작동합니다 - 능력치에 별도로 곱해집니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonStats.qml" line="12"/>
        <source>Monster Attack Rating, multiply by</source>
        <translation>몬스터 공격 등급 배율</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonStats.qml" line="23"/>
        <source>Monster Defense, multiply by</source>
        <translation>몬스터 방어력 배율</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonStats.qml" line="34"/>
        <source>Monster HP, multiply by</source>
        <translation>몬스터 생명력 배율</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonStats.qml" line="45"/>
        <source>Monster Damage, multiply by</source>
        <translation>몬스터 공격력 배율</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonStats.qml" line="56"/>
        <source>Monster EXP gain, multiply by</source>
        <translation>몬스터 경험치 획득량 배율</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonStats.qml" line="67"/>
        <source>Maximum resistance allowed for base monsters, percent:</source>
        <translation>일반 몬스터의 최대 저항력 (%)</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonStats.qml" line="68"/>
        <source>If you make this below 100, then regular monsters will have NO IMMUNITIES at all!&lt;br&gt;Use with caution. You can set this to 110-115, to make cold immunes breakable.&lt;br&gt;Also this do not prevent Enchanted monsters to have immunities.</source>
        <translation>이 설정은 인챈트된 몬스터가 면역을 가지는 것을 막지 않습니다.&lt;br&gt;참고: 몬스터의 저항력이 100% 이상일 경우 몬스터는 면역 상태가 됩니다.&lt;br&gt;최대 저항력을 110-115%로 설정하면, 원래 100%에서 면역이던 몬스터의 면역이 깨질 수 있습니다.&lt;br&gt;저항력 감소 효과로 100% 아래로 낮출 수 있어 면역을 깰 수 있게 됩니다.&lt;br&gt;참고: 면역은 파괴참, 선고, 저항 감소, 노화, 피해 증폭 스킬로만 제거할 수 있습니다.&lt;br&gt;몬스터의 저항 감소 옵션은 면역에 영향을 주지 않습니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonStats.qml" line="77"/>
        <source>Change monster resistances, by multiplying damage from magic they take</source>
        <translation>몬스터의 속성 피해 배율</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonStats.qml" line="78"/>
        <source>DO NOT affect IMMUNE monsters and monsters with 0 resistance!&lt;br&gt;It affects resistance perent indirectly, through this calculation:&lt;br&gt;Lets say you have 200% selected for multiply, and monster has 80% basic Fire resist.&lt;br&gt;That means, he has 20% damage taken from fire, and new value will be 40% damage, or 60% resist.&lt;br&gt;Damage is capped at 100%, so you won&apos;t get negative reistance.&lt;br&gt;Also monster can&apos;t become immune to element, maximum resistance is 99%</source>
        <translation>면역 몬스터와 0 저항력을 가진 몬스터에는 영향을 주지 않습니다.&lt;br&gt;이 설정은 몬스터가 받는 속성 피해량을 직접적으로 조절합니다:&lt;br&gt;- 면역(100% 저항)이나 무저항(0% 저항) 몬스터에는 영향을 주지 않습니다.&lt;br&gt;- 설정 값이 낮을수록 몬스터가 받는 속성 피해가 낮아집니다.&lt;br&gt;- 설정 값이 높을수록 몬스터가 받는 속성 피해가 높아집니다.&lt;br&gt;예(몬스터가 저항력이 80%, 20%의 속성 피해를 받을 때):&lt;br&gt;1. 설정 값 10%: 몬스터가 받는 새 속성 피해 = 2% (매우 낮아짐)&lt;br&gt;2. 설정 값 200%: 몬스터가 받는 새 속성 피해 = 40% (높아짐)&lt;br&gt;- 이 설정으로 몬스터가 100% 피해를 받게 되지는 않습니다.&lt;br&gt;- 몬스터가 받는 속성 피해는 최소 1%, 최대 99%로 제한됩니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonStats.qml" line="88"/>
        <source>Apply resistance options to following:</source>
        <translation>저항력 옵션 적용 대상:</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonStats.qml" line="92"/>
        <source>Elemental (Fire/Cold/Lightning)</source>
        <translation>원소 (화염/냉기/번개)</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonStats.qml" line="99"/>
        <source>Poison</source>
        <translation>독</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonStats.qml" line="106"/>
        <source>Magic (Non-elemental)</source>
        <translation>마법 (비원소)</translation>
    </message>
    <message>
        <location filename="../qml/ModuleMonStats.qml" line="113"/>
        <source>Physical</source>
        <translation>물리</translation>
    </message>
</context>
<context>
    <name>ModulePerfectRoll</name>
    <message>
        <location filename="../qml/ModulePerfectRoll.qml" line="8"/>
        <source>Perfect rolls</source>
        <translation>완벽한 수치</translation>
    </message>
    <message>
        <location filename="../qml/ModulePerfectRoll.qml" line="9"/>
        <source>Remove random effect when rolling between min and max value for something.</source>
        <translation>아이템 속성의 최소값과 최대값 사이에서 무작위로 결정되는 효과를 제거합니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModulePerfectRoll.qml" line="12"/>
        <source>Perfect rolls for crafting in the Cube</source>
        <translation>호라드릭의 함 제작 시 완벽한 수치</translation>
    </message>
    <message>
        <location filename="../qml/ModulePerfectRoll.qml" line="19"/>
        <source>Perfect Uniques</source>
        <translation>유니크 아이템 완벽한 수치</translation>
    </message>
    <message>
        <location filename="../qml/ModulePerfectRoll.qml" line="26"/>
        <source>Perfect Rune Word rolls</source>
        <translation>룬워드 완벽한 수치</translation>
    </message>
    <message>
        <location filename="../qml/ModulePerfectRoll.qml" line="33"/>
        <source>Perfect Set items</source>
        <translation>세트 아이템 완벽한 수치</translation>
    </message>
    <message>
        <location filename="../qml/ModulePerfectRoll.qml" line="40"/>
        <source>Perfect Magic/Rare affixes</source>
        <translation>매직/레어 아이템 접사 완벽한 수치</translation>
    </message>
    <message>
        <location filename="../qml/ModulePerfectRoll.qml" line="47"/>
        <source>Perfect predetermined crafted affixes</source>
        <translation>제작 아이템의 고정 접사 완벽한 수치</translation>
    </message>
    <message>
        <location filename="../qml/ModulePerfectRoll.qml" line="54"/>
        <source>Always max defense Armor</source>
        <translation>방어구 항상 최대 방어력</translation>
    </message>
    <message>
        <location filename="../qml/ModulePerfectRoll.qml" line="61"/>
        <source>Unique items level 1 power multiply</source>
        <translation>레벨 1 유니크 아이템 능력치 배율</translation>
    </message>
    <message>
        <location filename="../qml/ModulePerfectRoll.qml" line="72"/>
        <source>Unique items level 50 power multiply</source>
        <translation>레벨 50 유니크 아이템 능력치 배율</translation>
    </message>
    <message>
        <location filename="../qml/ModulePerfectRoll.qml" line="83"/>
        <source>Set items level 1 power multiply</source>
        <translation>레벨 1 세트 아이템 능력치 배율</translation>
    </message>
    <message>
        <location filename="../qml/ModulePerfectRoll.qml" line="94"/>
        <source>Set items level 50 power multiply</source>
        <translation>레벨 50 세트 아이템 능력치 배율</translation>
    </message>
    <message>
        <location filename="../qml/ModulePerfectRoll.qml" line="105"/>
        <source>Magic affix level 1 power multiply</source>
        <translation>레벨 1 매직 접사 능력치 배율</translation>
    </message>
    <message>
        <location filename="../qml/ModulePerfectRoll.qml" line="116"/>
        <source>Magic affix level 50 power multiply</source>
        <translation>레벨 50 매직 접사 능력치 배율</translation>
    </message>
    <message>
        <location filename="../qml/ModulePerfectRoll.qml" line="127"/>
        <source>Rune Words power multiply</source>
        <translation>룬워드 능력치 배율</translation>
    </message>
    <message>
        <location filename="../qml/ModulePerfectRoll.qml" line="138"/>
        <source>Crafted items power multiply</source>
        <translation>제작 아이템 능력치 배율</translation>
    </message>
    <message>
        <location filename="../qml/ModulePerfectRoll.qml" line="149"/>
        <source>Rune/Gems power multiply</source>
        <translation>룬/보석 능력치 배율</translation>
    </message>
</context>
<context>
    <name>ModuleQol</name>
    <message>
        <location filename="../qml/ModuleQol.qml" line="8"/>
        <source>Quality of life</source>
        <translation>삶의 질 개선</translation>
    </message>
    <message>
        <location filename="../qml/ModuleQol.qml" line="9"/>
        <source>Tiny quality things to make you life easier: &lt;br&gt;1. Insreasing stacks for tomes/quivers/keys;&lt;br&gt;2. Remove Unique charm limit (useful with randomizer);&lt;br&gt;3. Allow usage of some skills in the town;&lt;br&gt;4. Reduce item costs. </source>
        <translation>게임 플레이를 더 편리하게 만들어줄 작은 개선사항들:&lt;br&gt;1. 책/화살통/열쇠의 최대 보유량 증가;&lt;br&gt;2. 유니크 부적의 제한 해제 (랜덤화 기능과 함께 사용 시 유용);&lt;br&gt;3. 마을에서 일부 스킬 사용 허용;&lt;br&gt;4. 아이템 비용 감소.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleQol.qml" line="12"/>
        <source>Increase tome sizes 20 -&gt; 60</source>
        <translation>책 최대 보유량 증가 20 -&gt; 60</translation>
    </message>
    <message>
        <location filename="../qml/ModuleQol.qml" line="19"/>
        <source>Increase key chain 12 -&gt; 50</source>
        <translation>열쇠 최대 보유량 증가 12 -&gt; 50</translation>
    </message>
    <message>
        <location filename="../qml/ModuleQol.qml" line="26"/>
        <source>Increase quivers size -&gt; 511</source>
        <translation>화살통 최대 보유량 증가 -&gt; 511</translation>
    </message>
    <message>
        <location filename="../qml/ModuleQol.qml" line="27"/>
        <source>D2R already have 500 quiver size, so use this for LoD</source>
        <translation>디아블로 2 레저렉션은 이미 500개의 화살/볼트를 보유할 수 있으므로, 디아블로 2 클래식에서 사용하세요</translation>
    </message>
    <message>
        <location filename="../qml/ModuleQol.qml" line="33"/>
        <source>Remove limit on Unique charms</source>
        <translation>유니크 부적의 제한 해제</translation>
    </message>
    <message>
        <location filename="../qml/ModuleQol.qml" line="40"/>
        <source>Allow using Teleport, BC, BO in town</source>
        <translation>마을에서 텔레포트, 전투 명령, 전투 외침 사용 허용</translation>
    </message>
    <message>
        <location filename="../qml/ModuleQol.qml" line="47"/>
        <source>Allow all skills in town</source>
        <translation>마을에서 모든 스킬 사용 허용</translation>
    </message>
    <message>
        <location filename="../qml/ModuleQol.qml" line="54"/>
        <source>Show ilevel in items&apos; titles</source>
        <translation>아이템 이름에 아이템 레벨 표시</translation>
    </message>
    <message>
        <location filename="../qml/ModuleQol.qml" line="61"/>
        <source>Disable experience penalty after level 70</source>
        <translation>70레벨 이후 경험치 페널티 비활성화</translation>
    </message>
    <message>
        <location filename="../qml/ModuleQol.qml" line="62"/>
        <source>That means you won&apos;t have around 1% EXP at higher levels, so high level grind will be much faster.</source>
        <translation>이는 높은 레벨에서 약 1%의 경험치 손실을 없앤다는 의미이므로, 높은 레벨에서의 경험치 획득이 훨씬 빨라질 것입니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleQol.qml" line="68"/>
        <source>Reduce costs of skills and stats on items (affects repair cost mostly)</source>
        <translation>아이템의 스킬과 능력치 요구사항 감소 (주로 수리 비용에 영향)</translation>
    </message>
</context>
<context>
    <name>ModuleRequirements</name>
    <message>
        <location filename="../qml/ModuleRequirements.qml" line="8"/>
        <source>Requirements</source>
        <translation>착용 요구사항</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRequirements.qml" line="9"/>
        <source>Options for lowering/raising requirements for equimpemnt. </source>
        <translation>장비의 착용 요구사항을 낮추거나 높이는 옵션입니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRequirements.qml" line="12"/>
        <source>Change Strength requirements on items, %</source>
        <translation>아이템의 힘 요구치 변경, %</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRequirements.qml" line="13"/>
        <source>If you set value above 100%, Strength requirements &lt;br&gt; become higher compared to the original.</source>
        <translation>100% 이상으로 설정하면 힘 요구치가&lt;br&gt; 원래보다 높아집니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRequirements.qml" line="22"/>
        <source>Change Dexterity requirements on items, %</source>
        <translation>아이템의 민첩 요구치 변경, %</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRequirements.qml" line="32"/>
        <source>Change Character level requirement on regular items, %</source>
        <translation>일반 아이템의 요구 레벨 변경, %</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRequirements.qml" line="33"/>
        <source>That controls &apos;Level required:&apos; you see on the items.</source>
        <translation>아이템에 표시되는 &apos;필요 레벨:&apos;을 조정합니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRequirements.qml" line="42"/>
        <source>Change Character level requirement on Unique/Set items, %</source>
        <translation>유니크/세트 아이템의 요구 레벨 변경, %</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRequirements.qml" line="52"/>
        <source>Change Character level requirement on Magic/Rare items, %</source>
        <translation>매직/레어 아이템의 요구 레벨 변경, %</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRequirements.qml" line="62"/>
        <source>Change Minimal level required to spent points into skill, %</source>
        <translation>스킬 포인트 사용에 필요한 최소 레벨 변경, %</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRequirements.qml" line="63"/>
        <source>For example, if you select 50%, you can get level 30 skill on level 15 instead.</source>
        <translation>예: 50%로 설정하면 원래 30레벨에 얻을 수 있는 스킬을 15레벨에 얻을 수 있습니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRequirements.qml" line="72"/>
        <source>Change quality level on regular items, %</source>
        <translation>일반 아이템의 아이템 레벨 변경, %</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRequirements.qml" line="73"/>
        <source>That controls how valueable item is from drop calculation process.&lt;br&gt; Lower value means you get item dropped much earlier in the game.</source>
        <translation>아이템 드롭 계산 시 아이템의 등급을 조정합니다.&lt;br&gt; 낮은 값으로 설정하면 게임 초반에 해당 아이템을 얻을 수 있습니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRequirements.qml" line="82"/>
        <source>Change quality level on Unique/Set items, %</source>
        <translation>유니크/세트 아이템의 아이템 레벨 변경, %</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRequirements.qml" line="92"/>
        <source>Change quality level on Magic/Rare items, %</source>
        <translation>매직/레어 아이템의 아이템 레벨 변경, %</translation>
    </message>
</context>
<context>
    <name>ModuleRuneDrops</name>
    <message>
        <location filename="../qml/ModuleRuneDrops.qml" line="8"/>
        <source>Increase Rune drops</source>
        <translation>룬 드롭률 증가</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRuneDrops.qml" line="9"/>
        <source>All help and explanation on this page assume you are familiar with Treasure Class (TC) and NoDrop terminology.&lt;br&gt;Basically all options here modifying TC pick weights - so one preferred option become less rare.</source>
        <translation>이 페이지의 모든 설명은 보물 등급(TC)과 NoDrop 개념에 대한 이해를 전제로 합니다.&lt;br&gt;여기의 모든 옵션은 TC 선택 가중치를 조정하여 원하는 아이템의 희귀도를 낮춥니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRuneDrops.qml" line="12"/>
        <source>Select preset...</source>
        <translation type="unfinished">프리셋 선택</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRuneDrops.qml" line="12"/>
        <source>Gimme pile of Ber runes!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/ModuleRuneDrops.qml" line="12"/>
        <source>I want to make Infinity at least once in my life</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/ModuleRuneDrops.qml" line="12"/>
        <source>I want high runes to be rare, but want more rune drops overall</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/ModuleRuneDrops.qml" line="17"/>
        <source>Increase Rare Rune drops</source>
        <translation>희귀 룬 드롭률 증가</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRuneDrops.qml" line="18"/>
        <source>That will drastically improve High Rune drop rates! You probably don&amp;apos;t need more than 50x.&lt;br&gt;Value here increases chance of dropping Zod in &apos;Runes 17&apos; TC&lt;br&gt;Rarity of other runes will change proportionally &lt;br&gt;(so High Runes still be more rare in the same manner).</source>
        <translation>상위 룬 드롭률을 크게 향상시킵니다. 50배 이상은 권장하지 않습니다.&lt;br&gt;이 값은 &apos;룬 17&apos; TC에서 조드 룬의 드롭 확률을 증가시킵니다.&lt;br&gt;다른 룬의 희귀도도 비례적으로 변경됩니다.&lt;br&gt;(상위 룬은 여전히 상대적으로 더 희귀합니다)</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRuneDrops.qml" line="27"/>
        <source>Increase Runes chance in Good TC</source>
        <translation>고급 TC에서 룬 확률 증가</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRuneDrops.qml" line="28"/>
        <source>When dropping from Good TC, that will make Runes drop more often &lt;br&gt;compared to other jewellery (Gems/Rings/Amulets/Charms)</source>
        <translation>고급 TC에서 드롭 시, 룬이 다른 장신구(보석/반지/목걸이/부적)보다&lt;br&gt;더 자주 드롭되도록 합니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRuneDrops.qml" line="37"/>
        <source>Switch (Ber,Jah) with (Cham,Zod) in rarity</source>
        <translation>베르/자 룬과 참/조드 룬의 희귀도 교체</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRuneDrops.qml" line="38"/>
        <source>When game desides to drop Jah rune, it drops Zod instead. And vice versa.</source>
        <translation>자 룬이 드롭될 때 대신 조드 룬이 드롭되며, 그 반대도 마찬가지입니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRuneDrops.qml" line="44"/>
        <source>Make Countess Drop higher runes (up to Runes 17=Zod on Hell)</source>
        <translation>카운테스가 조드 룬 까지 드롭</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRuneDrops.qml" line="51"/>
        <source>Increase Countess Runes drop count to 5</source>
        <translation>카운테스의 룬 드롭 개수를 5개로 증가</translation>
    </message>
    <message>
        <location filename="../qml/ModuleRuneDrops.qml" line="58"/>
        <source>Increase Wraiths Runes drop</source>
        <translation>망령(Wraith) 몬스터의 룬 드롭률 증가</translation>
    </message>
</context>
<context>
    <name>ModuleSkillRandomizer</name>
    <message>
        <location filename="../qml/ModuleSkillRandomizer.qml" line="8"/>
        <source>Skill Randomizer</source>
        <translation>스킬 랜덤화</translation>
    </message>
    <message>
        <location filename="../qml/ModuleSkillRandomizer.qml" line="9"/>
        <source>This allow to bring &apos;some&apos; randomization into character skills.</source>
        <translation>이 옵션은 캐릭터 스킬에 일정 수준의 랜덤 요소를 추가합니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleSkillRandomizer.qml" line="12"/>
        <source>Randomize skill tabs within each character</source>
        <translation>각 캐릭터의 스킬 트리 랜덤화</translation>
    </message>
    <message>
        <location filename="../qml/ModuleSkillRandomizer.qml" line="13"/>
        <source>That DOES not move skills between characters! What it can do:&lt;br&gt;-Move a skill to a different skill tab;&lt;br&gt;-Move a skill to a higher/lower level;&lt;br&gt;-Change skill requirements.&lt;br&gt;So, after level 50 there is not much of a difference. Synergies are also untouched.</source>
        <translation>주의: 이 옵션은 캐릭터 간 스킬을 이동시키지 않습니다.&lt;br&gt;가능한 변경 사항:&lt;br&gt;- 스킬을 다른 스킬 트리로 이동&lt;br&gt;- 스킬의 필요 레벨 상향 또는 하향 조정&lt;br&gt;- 스킬 습득 조건 변경&lt;br&gt;참고: 레벨 50 이후에는 큰 영향이 없으며, 스킬 시너지는 변경되지 않습니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleSkillRandomizer.qml" line="19"/>
        <source>Randomize element types for some damage skills</source>
        <translation>일부 공격 스킬의 속성 변경</translation>
    </message>
    <message>
        <location filename="../qml/ModuleSkillRandomizer.qml" line="20"/>
        <source>For some skills, where it is easy to change damage type, it will randomize between 5 different elements:&lt;br&gt;Cold, Fire, Lightning, Magic and Poison&lt;br&gt;WARNING! Skill descriptions won&apos;t be updated and will be completely wrong!&lt;br&gt;Also, character info screen does not show difference between cold and magic correctly.&lt;br&gt;You need to find by yourself what damage (magic or cold) was rolled.&lt;br&gt;For poison, all damage is dealt over 2 seconds and slightly more than original.</source>
        <translation>속성 변경이 가능한 일부 스킬의 경우, 다음 5가지 속성 중 하나로 랜덤 변경됩니다:&lt;br&gt;냉기, 화염, 번개, 마법, 독&lt;br&gt;주의:&lt;br&gt;1. 스킬 설명은 업데이트되지 않아 잘못된 정보를 표시합니다!&lt;br&gt;2. 캐릭터 정보 화면에서 냉기와 마법 속성의 차이가 정확히 표시되지 않습니다.&lt;br&gt;3. 실제 적용된 속성(마법 또는 냉기)은 직접 확인해야 합니다.&lt;br&gt;4. 독 속성의 경우, 모든 데미지가 2초에 걸쳐 발생하며 원래보다 약간 높습니다.</translation>
    </message>
    <message>
        <location filename="../qml/ModuleSkillRandomizer.qml" line="26"/>
        <source>Ensure that element types are different from original</source>
        <translation>원래 속성과 다르게 변경 보장</translation>
    </message>
    <message>
        <location filename="../qml/ModuleSkillRandomizer.qml" line="27"/>
        <source>If the source element is Cold, then new randomized element never should be Cold.&lt;br&gt;If this turned off, ~20% of skills will remain original behaviour.</source>
        <translation>이 옵션을 켜면, 원래 속성과 다른 속성으로만 변경됩니다.&lt;br&gt;예: 원래 냉기 속성이면 냉기 외 다른 속성으로 변경&lt;br&gt;이 옵션을 끄면, 약 20%의 스킬이 원래 속성을 유지합니다.</translation>
    </message>
</context>
<context>
    <name>Navigation</name>
    <message>
        <source>Main</source>
        <translation type="obsolete">메인</translation>
    </message>
    <message>
        <source>Randomizers</source>
        <translation type="obsolete">랜덤화 설정</translation>
    </message>
    <message>
        <source>Item Randomizer</source>
        <translation type="obsolete">아이템 랜덤화</translation>
    </message>
    <message>
        <source>Monster Randomizer</source>
        <translation type="obsolete">몬스터 랜덤화</translation>
    </message>
    <message>
        <source>Skill Randomizer</source>
        <translation type="obsolete">스킬 랜덤화</translation>
    </message>
    <message>
        <source>Make harder</source>
        <translation type="obsolete">난이도 설정</translation>
    </message>
    <message>
        <source>Monster stats</source>
        <translation type="obsolete">몬스터 능력치</translation>
    </message>
    <message>
        <source>Monster density</source>
        <translation type="obsolete">몬스터 밀집도</translation>
    </message>
    <message>
        <source>Challenge</source>
        <translation type="obsolete">도전적 설정</translation>
    </message>
    <message>
        <source>Make easier</source>
        <translation type="obsolete">편의성 설정</translation>
    </message>
    <message>
        <source>Horadric Cube</source>
        <translation type="obsolete">호라드릭의 함</translation>
    </message>
    <message>
        <source>Gambling</source>
        <translation type="obsolete">도박</translation>
    </message>
    <message>
        <source>Character</source>
        <translation type="obsolete">캐릭터</translation>
    </message>
    <message>
        <source>Requirements</source>
        <translation type="obsolete">착용 요구사항</translation>
    </message>
    <message>
        <source>Misc</source>
        <translation type="obsolete">기타</translation>
    </message>
    <message>
        <source>Quality of life</source>
        <translation type="obsolete">삶의 질 개선</translation>
    </message>
    <message>
        <source>Drops filtering</source>
        <translation type="obsolete">아이템 필터</translation>
    </message>
</context>
<context>
    <name>PageMain</name>
    <message>
        <location filename="../qml/PageMain.qml" line="14"/>
        <source>Main</source>
        <translation type="unfinished">메인</translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="19"/>
        <source>This page contains mandatory inputs for generation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="23"/>
        <source>Reset to default</source>
        <translation type="unfinished">기본값으로 재설정</translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="32"/>
        <source>Input folder (mandatory):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="33"/>
        <source>Generally, just choose folder with Game.exe.&lt;br&gt;Note: for ProjectD2, select subfolder named &apos;ProjectD2&apos; in Diablo II.&lt;br&gt;Press Detect! to find information in Windows Registry.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="45"/>
        <source>Detect!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="52"/>
        <source>Output folder (optional, leave empty to auto-detect):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="63"/>
        <source>Target D2 Game version:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="64"/>
        <source>D2 Legacy (Pre-D2R): 1.14, ProjectD2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="64"/>
        <source>D2 Resurrected - LoD</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="64"/>
        <source>D2 Resurrected - RotW</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="70"/>
        <source>Input mode, where to read game data:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="71"/>
        <source>This determines source data that is used by randomizer&lt;br&gt;Game data archive - For D2R Resurrected, this is CASC storage;&lt;br&gt;Game data archive - For D2 Legacy this is .mpq files;&lt;br&gt;Folder with extracted data - If you want to use D2R select folder containing modinfo.json&lt;br&gt;Last option is for folder with .txt files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="72"/>
        <source>Game data archive</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="72"/>
        <source>Folder with extracted data</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="72"/>
        <location filename="../qml/PageMain.qml" line="80"/>
        <source>Only .txt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="78"/>
        <source>Output format, how files and folders will be created:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="79"/>
        <source>This determines output type:&lt;br&gt;First one will create D2R mod with all needed subfolders,&lt;br&gt;Second one will create folder structure with txt/json&lt;br&gt;Last one will only output .txt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="80"/>
        <source>D2 Resurrected mod</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="80"/>
        <source>Full folders</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mod id:</source>
        <translation type="obsolete">모드 ID:</translation>
    </message>
    <message>
        <source>Use Diablo II legacy installation</source>
        <translation type="obsolete">Diablo II 레거시 설치 사용</translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="90"/>
        <source>Random seed:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="93"/>
        <source>Seed determine how reproduceable generation results are.&lt;br&gt;If seed stays the same, output of generation is also always exactly the same.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="100"/>
        <source>Generate new</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageMain.qml" line="104"/>
        <source>Create new seed on every Generate call</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Output directory (leave empty to output to D2R path):</source>
        <translation type="obsolete">출력 디렉토리 (

비워두면 D2R 경로로 출력):</translation>
    </message>
    <message>
        <source>Export all *.txt (for further manual edit)</source>
        <translation type="obsolete">모든 *.txt 내보내기 (추가 수동 편집용)</translation>
    </message>
</context>
<context>
    <name>PageTool</name>
    <message>
        <source>Main</source>
        <translation type="obsolete">메인</translation>
    </message>
    <message>
        <source>D2R save and user settings root:</source>
        <translation type="obsolete">D2R 저장 및 사용자 설정 루트:</translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="76"/>
        <source>Copy Settings.json</source>
        <translation type="unfinished">Settings.json 복사</translation>
    </message>
    <message>
        <source>(this will copy default settings to mod folder)</source>
        <translation type="obsolete">(기본 설정이 모드 폴더로 복사됩니다)</translation>
    </message>
    <message>
        <source>D2R command arguments (read-only):</source>
        <translation type="obsolete">D2R 명령 인수 (읽기 전용):</translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="21"/>
        <source>Extra helpful utilities</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="31"/>
        <source>Mod id:</source>
        <translation type="unfinished">모드 ID:</translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="32"/>
        <source>That will affect game&apos;s saves folder subdirectory and mod subfolder for D2R</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="44"/>
        <source>Force output for all txt/json (for further manual edit)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="45"/>
        <source>This will force to output all game files,&lt;br&gt;not only used by randomizer in current configuration.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="61"/>
        <source>D2R folder for mod save and settings:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="80"/>
        <source>This will copy Game Settings to mod folder&lt;br&gt;That will allow to skip configuring mod on launch.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="86"/>
        <source>Copy all save files (.d2s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="90"/>
        <source>this will copy all your character save files to modded folder.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="108"/>
        <source>D2 commandline arguments (read-only):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="119"/>
        <source>Choose convenient way to launch the mod:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="124"/>
        <source>Option 1:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="128"/>
        <source>Make shortcut on Desktop</source>
        <translation type="unfinished">바탕화면에 바로가기 만들기</translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="140"/>
        <source>Option 2:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="143"/>
        <source>Reset launch to unmodded</source>
        <translation type="unfinished">모드 해제된 상태로 실행 초기화</translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="148"/>
        <source>Set launch to mod</source>
        <translation type="unfinished">모드로 실행 설정</translation>
    </message>
    <message>
        <location filename="../qml/PageTool.qml" line="153"/>
        <source>(Close Battlenet launcher before actions!)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Randomizers</source>
        <translation type="vanished">랜덤화 설정</translation>
    </message>
    <message>
        <source>Make harder</source>
        <translation type="vanished">난이도 설정</translation>
    </message>
    <message>
        <source>Make easier</source>
        <translation type="vanished">편의성 설정</translation>
    </message>
    <message>
        <source>Misc</source>
        <translation type="vanished">기타</translation>
    </message>
    <message>
        <source>Plugins</source>
        <translation type="vanished">플러그인</translation>
    </message>
</context>
<context>
    <name>WidgetCommonTab</name>
    <message>
        <location filename="../qml/WidgetCommonTab.qml" line="36"/>
        <source>Enable this tab</source>
        <translation type="unfinished">탭 활성화</translation>
    </message>
    <message>
        <location filename="../qml/WidgetCommonTab.qml" line="41"/>
        <source>Reset to default</source>
        <translation type="unfinished">기본값으로 재설정</translation>
    </message>
</context>
<context>
    <name>WidgetPreset</name>
    <message>
        <location filename="../qml/WidgetPreset.qml" line="16"/>
        <source>Don&apos;t know where to start? Select a preset:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
