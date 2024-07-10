<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ko_KR">
<context>
    <name>D2ModGen::ConfigPageMergeMods</name>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="141"/>
        <source>Overall order of data load is: D2 source from Main settings -&gt;
-&gt; Pre-gen data -&gt; modgen features -&gt; Post-gen data -&gt; output folder.</source>
        <translation>전체 데이터 로드 순서: 메인 설정에서 D2 소스 -&gt;
-&gt; 사전 생성 데이터 -&gt; modgen 기능 -&gt; 후처리 데이터 -&gt; 출력 폴더.</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="198"/>
        <source>You can add aditional steps to mod generation, prior or after the d2modgen work
Two additional type of sources are supported:
1. folder with .txt files
2. another D2R mod
After selecting source type, you need to specify conflict resolution, 
for the case additional source contains a file which already been generated.</source>
        <translation>d2modgen 작업 전후에 모드 생성에 추가 단계를 추가할 수 있습니다.
두 가지 추가 소스 유형이 지원됩니다:
1. .txt 파일이 있는 폴더
2. 다른 D2R 모드
소스 유형을 선택한 후, 추가 소스에 이미 생성된 파일이 포함된 경우를 대비하여 충돌 해결 방법을 지정해야 합니다.</translation>
    </message>
</context>
<context>
    <name>D2ModGen::ConfigPageMergeModsItem</name>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="43"/>
        <source>CSV files folder</source>
        <translation>CSV 파일 폴더</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="43"/>
        <source>D2R mod</source>
        <translation>D2R 모드</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="48"/>
        <source>Replace - clear previous data</source>
        <translation>대체 - 기존 데이터 삭제</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="49"/>
        <source>Append - place all data at the end</source>
        <translation>추가 - 모든 데이터를 끝에 삽입</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="50"/>
        <source>Update - overwrite records with same key</source>
        <translation>업데이트 - 동일한 키의 데이터 덮어쓰기</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="51"/>
        <source>AppendNew - add non-existent data to the end</source>
        <translation>새 항목 추가 - 존재하지 않는 빈 데이터를 끝에 추가</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="52"/>
        <source>Merge - try Update, then AppendNew</source>
        <translation>병합 - 기존 항목 업데이트 후 새 항목 추가</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="53"/>
        <source>Skip</source>
        <translation>건너뛰기</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="54"/>
        <source>Raise error</source>
        <translation>오류 발생</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="62"/>
        <source>Mod name:</source>
        <translation>모드 이름:</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="71"/>
        <source>Folder with .txt files:</source>
        <translation>.txt 파일이 있는 폴더:</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="74"/>
        <source>Type:</source>
        <translation>유형:</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/ConfigPageMergeMods.cpp" line="77"/>
        <source>Action on conflict:</source>
        <translation>충돌 시 행동:</translation>
    </message>
</context>
<context>
    <name>D2ModGen::MainConfigPage</name>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="91"/>
        <source>Use Diablo II legacy installation</source>
        <translation>Diablo II 레거시 설치 사용</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="109"/>
        <source>Add key to new char inventory (Basic mod test)</source>
        <translation>새 캐릭터 인벤토리에 키 추가 (기본 모드 테스트)</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="111"/>
        <source>Export all *.txt (for further manual edit)</source>
        <translation>모든 *.txt 내보내기 (추가 수동 편집용)</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="114"/>
        <source>Copy Settings.json</source>
        <translation>Settings.json 복사</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="116"/>
        <source>Set launch to mod</source>
        <translation>모드로 실행 설정</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="117"/>
        <source>Reset launch to unmodded</source>
        <translation>모드 해제된 상태로 실행 초기화</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="118"/>
        <source>Make shortcut on Desktop</source>
        <translation>바탕화면에 바로가기 만들기</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="133"/>
        <source>Mod id:</source>
        <translation>모드 ID:</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="148"/>
        <source>D2R path:</source>
        <translation>D2R 경로:</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="159"/>
        <source>D2 legacy path:</source>
        <translation>D2 레거시 경로:</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="170"/>
        <source>D2R save and user settings root:</source>
        <translation>D2R 저장 및 사용자 설정 루트:</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="175"/>
        <source>(this will copy default settings to mod folder)</source>
        <translation>(기본 설정이 모드 폴더로 복사됩니다)</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="184"/>
        <source>D2R command arguments (read-only):</source>
        <translation>D2R 명령 인수 (읽기 전용):</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="196"/>
        <source>(&lt;b&gt;Close Battlenet launcher before actions!&lt;/b&gt;)</source>
        <translation>(&lt;b&gt;행동 전에 Battlenet 런처를 닫으세요!&lt;/b&gt;)</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="214"/>
        <source>Output directory (leave empty to output to D2R path):</source>
        <translation>출력 디렉토리 (

비워두면 D2R 경로로 출력):</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="309"/>
        <source>Main</source>
        <translation>메인</translation>
    </message>
    <message>
        <location filename="../src/QtApp/ConfigPages/MainConfigPage.cpp" line="314"/>
        <source>This page allow you to select most important options:
-Where input and output data is (D2 installation - Resurrected or Legacy);
-Create new D2R save folder with global settings;
-Create desktop shortcut to launch mod.
However, there are some less important abilities: 
-Seed number (max 2^32) determines how random generation behaves. 
If seed is unchaged, then all generation will be the same every time 
You can edit seed manually or press F5 to generate new one.
-Add key to character inventory for fresh character - now you are sure you started modded version.</source>
        <translation>이 페이지에서는 가장 중요한 옵션을 선택할 수 있습니다:
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
        <location filename="../src/QtApp/MainWindow.cpp" line="100"/>
        <source>Generate</source>
        <translation>생성</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="109"/>
        <source>Status label.</source>
        <translation>상태 레이블.</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="164"/>
        <source>Reset to default</source>
        <translation>기본값으로 재설정</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="165"/>
        <source>Enable this tab</source>
        <translation>탭 활성화</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="201"/>
        <source>Select preset...</source>
        <translation>프리셋 선택</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="212"/>
        <source>Do not know where to start? Select a preset:</source>
        <translation>어디서 시작해야 할지 모르겠나요? 프리셋을 선택하세요:</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="263"/>
        <source>File</source>
        <translation>파일</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="264"/>
        <source>Actions</source>
        <translation>행동</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="265"/>
        <source>About</source>
        <translation>정보</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="266"/>
        <source>Save config...</source>
        <translation>설정 저장</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="267"/>
        <source>Load config...</source>
        <translation>설정 불러오기</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="268"/>
        <source>Clear config</source>
        <translation>설정 지우기</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="269"/>
        <source>Browse to settings folder</source>
        <translation>설정 폴더로 이동</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="271"/>
        <source>Quit without saving</source>
        <translation>저장하지 않고 종료</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="272"/>
        <source>Save and quit</source>
        <translation>저장하고 종료</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="273"/>
        <source>Generate mod</source>
        <translation>모드 생성</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="274"/>
        <source>Create seed</source>
        <translation>시드 생성</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="275"/>
        <source>Undo</source>
        <translation>실행 취소</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="276"/>
        <source>Theme</source>
        <translation>테마</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="277"/>
        <source>Language</source>
        <translation>언어</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="306"/>
        <source>You need to restart for changes apply.</source>
        <translation>변경 사항을 적용하려면 재시작해야 합니다.</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="404"/>
        <source>Start...</source>
        <translation>시작</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="414"/>
        <source>Mod &apos;%1&apos; successfully updated (%2).</source>
        <translation>모드 &apos;%1&apos;이(가) 성공적으로 업데이트되었습니다 (%2).</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="122"/>
        <source>Randomizers</source>
        <translation>랜덤화 설정</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="123"/>
        <source>Make harder</source>
        <translation>난이도 설정</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="124"/>
        <source>Make easier</source>
        <translation>편의성 설정</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="125"/>
        <source>Misc</source>
        <translation>기타</translation>
    </message>
    <message>
        <location filename="../src/QtApp/MainWindow.cpp" line="126"/>
        <source>Plugins</source>
        <translation>플러그인</translation>
    </message>
</context>
</TS>