Anleitung um Projekt zu kompilieren:

Voraussetzungen:
*   VS2008 SP1
*   DirectX 9


I   OGRE Erstellen

1.  OgreSDK_vc9_v1-7-1p1.exe ausführen und entpacken
2.  Umgebunsvariable auf den ogreSDK_vc9_v1-7-1p1 ordner
3.  OGRE.sln öffnen
4.  Eigenschaften von SampleBrowser
        Buildereignisse
            Prälinkereignis
                Befehlszeile -> hier Wert rauslöschen!
5.  ALL_BUILD erstellen
6.  Folgende Datein ins Arbeitsverzeichnis vom Client kopieren
    Anmerkung:
        für Release beschrieben. bei Debug die _d Dateien verwenden
    
    OgreMain.dll
    OIS.dll
    plugins.cfg
    RenderSystem_Direct3D9.dll
    RenderSystem_GL.dll
    resources.cfg
    
7.  plugins.cfg editieren

# Defines plugins to load

# Define plugin folder
PluginFolder=.

# Define plugins
 Plugin=RenderSystem_Direct3D9
 Plugin=RenderSystem_GL


8.  resources.cfg editieren

# Resources required by the sample browser and most samples.
[Essential]

# Common sample resources needed by many of the samples.
# Rarely used resources should be separately loaded by the
# samples which require them.
[Popular]
FileSystem=./media/materials/scripts
FileSystem=./media/materials/textures
FileSystem=./media/models
FileSystem=./media/Gorilla

[General]
FileSystem=../../media


II  FMOD
1.  API 43207 win32 installieren
2.  Umgebunsvariable FMOD_HOME auf den ordner api
3.  fmodex.dll und fmodexL.dll ins Arbeisverzeichnis vom Client kopieren

III pthread
1.  version w32 2.8.0 gibts auf ftp://sourceware.org/pub/pthreads-win32/
2.  PTHREAD_HOME umgebunsvariable auf den hauptordner
3.  pthreadVC2.dll ins Arbeisverzeichnis vom Client kopieren

IV  LUA
1.  die beiden ordner lua und luaplus werden benötigt
2.  LUA_HOME auf den lua ordner
3.  LUAPLUS_HOME auf den luaplus ordner
4.  lua5.1.dll, lua51.dll, luaplus_1100.dll
    ins Arbeisverzeichnis vom Server kopieren


V   graviator_client.vcproj
1.  Arbeitsverzeichnis auf bin\$(ConfigurationName)

VI  graviator_server.vcproj
1.  Arbeitsverzeichnis auf bin\$(ConfigurationName)
2.  vergewissern, dass settings.lua im Arbeitsverzeichnis ist