//---------------------------------------------------------------------------

#include <stdio.h>
#pragma hdrstop

#pragma package(smart_init)
#include "main.h"

#define DIRECTINPUT_VERSION 0x0800

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#define SAMPLE_BUFFER_SIZE 8  // arbitrary number of buffer elements

#include <dinput.h>

LPDIRECTINPUT8       g_pDI       = NULL; // The DirectInput object
LPDIRECTINPUTDEVICE8 g_pKeyboard = NULL; // The keyboard device

HRESULT OnCreateDevice( HWND hDlg );
VOID FreeDirectInput();
HRESULT ReadBufferedData( HWND hDlg );

HWND hWnd;

TTriggerManager     TriggerManager;
TFontManager        FontManager;
TCornerTextManager  CornerTextManager;
TNumberShower       NumberShower;
TButtonsManager     ButtonsManager;
TPlayerManager      PlayerManager;
TEnemiesManager     EnemiesManager;
TTextureManager     TextureManager;
TMusicManager       MusicManager;
TTimerManager      *TimerManager;
TFPSShower          FPSShower;
TStatistic          PlayerStatistic;

TEnemyShotsManager *EnemyShotsManager;

nTextureManager*   	texManager;

TPlayerShotsManager* PlayerShotsManager;

TMyD3DButton MessageButton1;
TMyD3DButton MessageButton2;

LPDIRECT3D9             MainD3D           = NULL;
LPDIRECT3DDEVICE9       MainDevice     = NULL;

LPD3DXSPRITE    MainSprite;
LPD3DXFONT      MainFont;
LPD3DXFONT      SlaveFont;

LPD3DXLINE      MainLine;

TMyD3DCheckListBox GameCheckListBox;

bool FindFile;
TLaserParam LaserParam;

D3DXMATRIXA16 IdentityMatrix;
D3DXMATRIXA16 MainMatrix;

int MFONT = 0;
int TFONT = 0;

//------------------------Other----------------------------------------------
TMyD3DButton        PanelButtons[6];
TPoint PPos;

TMyD3DButton MyButton;
//------------------------RECT----------------------------------------------

RECT DrawingRects[15];
RECT MyRect = CRect(0, 0, 108, 36);

//------------------------My Types-------------------------------------------
TLightning Lightnings[100];
TTeslaWeapon RangerTesla;

int UndamagerCoff;
//------------------------Boolean--------------------------------------------
bool AlreadyLoad = false;
bool Fire, FireAlt, Undamager, PauseNow, CanShot, AlreadyPressed;
bool RangerDeads, ScoreQuestion;
bool NeedShowFPS;
bool DeviceWasLost;

//------------------------Double---------------------------------------------
double Boss[15], RangerItems[200][10];

double Fon_top;

double FireInterval;

//------------------------Integer--------------------------------------------
int FireLevel;

int Laser[10];

int RangerWeaponEffects[100][10];

int Lines[100][10];
int RocketShleif[150][5];

int Artefacts[100][3];
int SomeShit;
int CurrentFonNumber;
int RangerArtefacts[15];

int Shleif[100][6];

int Explodings_count;
int Explodings[200][6];

int MType = 0;

void PlayerFire(int xpos, int ypos);


vector<int> stringlist;

string TempFolder = "Data/";

//--------------------------Textures-----------------------------------------
LPDIRECT3DTEXTURE9 Fon;
LPDIRECT3DTEXTURE9 CoolExploding1;
LPDIRECT3DTEXTURE9 CoolExploding2;
LPDIRECT3DTEXTURE9 DominatorUno[3][5];
LPDIRECT3DTEXTURE9 WeaponImages[5][3];
LPDIRECT3DTEXTURE9 Weapon1Effect;
LPDIRECT3DTEXTURE9 RocketShleifImages;
LPDIRECT3DTEXTURE9 GeneralFon;
LPDIRECT3DTEXTURE9 LaserImage;

//Graphics::TBitmap* ImagePictures[6][3];

LPDIRECT3DTEXTURE9 fuck;
LPDIRECT3DTEXTURE9 RangersLogo;
LPDIRECT3DTEXTURE9 Angle[4];
LPDIRECT3DTEXTURE9 Line[4];
LPDIRECT3DTEXTURE9 Icons[4];
LPDIRECT3DTEXTURE9 Light[2];
LPDIRECT3DTEXTURE9 Centre;
LPDIRECT3DTEXTURE9 Panel;
LPDIRECT3DTEXTURE9 WarningSpace;
LPDIRECT3DTEXTURE9 SmallShip;

LPDIRECT3DTEXTURE9 Explo[19];
LPDIRECT3DTEXTURE9 Artefact;
LPDIRECT3DTEXTURE9 BossI;
LPDIRECT3DTEXTURE9 DominatorShot;
LPDIRECT3DTEXTURE9 UndamagerTexture;
LPDIRECT3DTEXTURE9 PlayerTexture;

LPDIRECT3DTEXTURE9 ScreenSaver;
int                ScreenTime;

int CurrentVolume;

HMODULE GRAPHRES_LOCATION = LoadLibrary("GraphResource.dll");
HMODULE TEXTRES_LOCATION = LoadLibrary("TextResource.dll");

//---------------------------------------------------------------------------



class Particle {
public:
	double x, y;
};

class Movable: public Particle {
public:
	double speed, dx, dy;
	virtual void move(double time) {
		x += speed * dx * time;
		y += speed * dy * time;
	}
};

class Drawable {
public:
	virtual void draw() = 0;
};

class Star: public Movable, Drawable {
private:
	D3DCOLOR color;
public:
	Star(D3DCOLOR color): color(color) {
		x = random(800);
		y = 0;
	}
	void draw() {
	}
};


//---------------------------------------------------------------------------

struct tri_vertex {
    float x, y, z, rhw;  // The transformed(screen space) position for the vertex.
    DWORD colour;        // The vertex colour.
};

class Animated {
public:
	virtual void animateStep(double time) = 0;
	
};

class GameObject: public Movable {
public:
	int width, height;
	string texture;
	RECT getBounds() {
		int x = (int)this->x;
		int y = (int)this->y;
		return CRect(x - width >> 1, y - height >> 1, x + width >> 1, y + height >> 1);
	}
};

const DWORD tri_fvf = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
LPDIRECT3DVERTEXBUFFER9 g_list_vb = NULL;
int g_list_count;

//******************************************************************************************
// Function:init_lists
// Whazzit:Create and fill our Vertex Buffer
//******************************************************************************************
HRESULT init_lists(void) {
    HRESULT hr;
	int byte_count = sizeof(tri_vertex) * 30;
    hr = MainDevice->CreateVertexBuffer(byte_count,      //Length
                                        D3DUSAGE_WRITEONLY,//Usage
                                        tri_fvf,           //FVF
                                        D3DPOOL_MANAGED,   //Pool
                                        &g_list_vb,        //ppVertexBuffer
                                        NULL);             //Handle

    if(FAILED(hr)) {
        return hr;
    }

    return D3D_OK;
}

class TStar : public GObject
{
    public:
        int Speed;
        D3DCOLOR Color;
};

class   TStarsManager
{
    private:
        TStar Stars[30];
        LPD3DXLINE* pLine;
    public:
        void InitDrawingInterface(LPD3DXLINE* Line);
        void Move();
        void Set();
        void Draw();
};

void TStarsManager::Move()
{
    int i;
    for(i=0;i<30;i++)
    {
        Stars[i].ypos+=5* Stars[i].Speed /2;
        if(Stars[i].ypos>600)
        {
            Stars[i].ypos=0;
            Stars[i].xpos=random(800);
        }
    }
}

void TStarsManager::Draw()
{
	tri_vertex data[30];
	for (int i = 0; i < 30; i++) {
		data[i].x = Stars[i].xpos;
		data[i].y = Stars[i].ypos;
		data[i].z = 1;
		data[i].rhw = 1;
		data[i].colour = Stars[i].Color;
	}

/*	tri_vertex data[] = {
        //Fan1
        {100, 100, 1, 1, 0xFF000000}, {  0,  0, 1, 1, 0xFFFF0000}, {200,  0, 1, 1, 0xFF00FF00},
        {100, 100, 1, 1, 0xFF000000}, {200,  0, 1, 1, 0xFF00FF00}, {200, 200, 1, 1, 0xFF0000FF},
        {100, 100, 1, 1, 0xFF000000}, {200, 200, 1, 1, 0xFF0000FF}, {  0, 200, 1, 1, 0xFFFFFFFF},
        {100, 100, 1, 1, 0xFF000000}, {  0, 200, 1, 1, 0xFFFFFFFF}, {  0,  0, 1, 1, 0xFFFF0000},
        //Fan2
        { 75, 350, 1, 1, 0xFFFFFFFF}, {  0, 225, 1, 1, 0xFFFF0000}, { 50, 215, 1, 1, 0xFF7F7F00},
        { 75, 350, 1, 1, 0xFFFFFFFF}, { 50, 215, 1, 1, 0xFF7F7F00}, { 75, 205, 1, 1, 0xFF00FF00},
        { 75, 350, 1, 1, 0xFFFFFFFF}, { 75, 205, 1, 1, 0xFF00FF00}, {125, 215, 1, 1, 0xFF007F7F},
        { 75, 350, 1, 1, 0xFFFFFFFF}, {125, 215, 1, 1, 0xFF007F7F}, {150, 235, 1, 1, 0xFF0000FF},
        //Strip1
        {250, 150, 1, 1, 0xFFFF0000}, {300, 50, 1, 1, 0xFF00FF00}, {350, 150, 1, 1, 0xFF0000FF},
        {300, 50, 1, 1, 0xFF00FF00}, {400, 50, 1, 1, 0xFFFF0000}, {350, 150, 1, 1, 0xFF0000FF},
        {400, 50, 1, 1, 0xFFFF0000}, {450, 150, 1, 1, 0xFF7F7F00}, {350, 150, 1, 1, 0xFF0000FF},
        //Strip2
        {250, 350, 1, 1, 0xFFFF0000}, {300, 250, 1, 1, 0xFF00FF00}, {350, 350, 1, 1, 0xFF0000FF},
        {300, 250, 1, 1, 0xFF00FF00}, {400, 250, 1, 1, 0xFFFF0000}, {350, 350, 1, 1, 0xFF0000FF},
        {400, 250, 1, 1, 0xFFFF0000}, {450, 350, 1, 1, 0xFF7F7F00}, {350, 350, 1, 1, 0xFF0000FF},

    }; */


	void *vb_vertices;
	HRESULT hr;
	hr = g_list_vb->Lock(0, //Offset
                         0, //SizeToLock
                         &vb_vertices, //Vertices
                         0);  //Flags	  

	g_list_vb->Unlock();

	memcpy( vb_vertices, data, sizeof(data));

	g_list_vb->Unlock();



	MainDevice->SetFVF(tri_fvf);
    MainDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
           //Bind our Vertex Buffer
    MainDevice->SetStreamSource(0,                   //StreamNumber
                                     g_list_vb,           //StreamData
                                     0,                   //OffsetInBytes
                                     sizeof(tri_vertex)); //Stride

    //Render from our Vertex Buffer
	MainDevice->DrawPrimitive(D3DPT_POINTLIST, //PrimitiveType
                                   0,                  //StartVertex
                                   30);      //PrimitiveCount */

	/*

    D3DXVECTOR2 Vector[2];
    (*pLine)->Begin();
    (*pLine)->Draw(Vector,2,D3DCOLOR_ARGB(255,0,255,0));
    for(int i=0;i<30;i++)
    {
        Vector[0].x=Stars[i].xpos;
        Vector[0].y=Stars[i].ypos;//=CVector(Stars[i].xpos,Stars[i].ypos,0);
        Vector[1].x=Stars[i].xpos+1;
        Vector[1].y=Stars[i].ypos+1;
        (*pLine)->Draw(Vector,2,Stars[i].Color);
    }
    (*pLine)->End(); */
}

void TStarsManager::Set()
{
    for(int i=0;i<30;i++)
    {
        Stars[i].xpos=random(800);
        Stars[i].ypos=random(600);
        Stars[i].Speed=random(3)+1;

		//int bright = - 40 + (70 * Stars[i].Speed);
		int bright = 255 * Stars[i].Speed / 3;
		Stars[i].Color = D3DCOLOR_ARGB(255, bright, bright, bright);


        //Stars[i].Color=D3DCOLOR_ARGB(255, 255,255, (int) (255 * Stars[i].Speed / 3));
    }
}

void TStarsManager::InitDrawingInterface(LPD3DXLINE* Line)
{
    pLine=Line;
}


TStarsManager*      StarsManager;


string loadTextResource(HMODULE location, string name);
vector<string> split(string s, string patt);

class Screen {
    public:
        virtual ~Screen() {
        }
        virtual void render() = 0;
        virtual bool canGoBack() {
            return true;
        }
        virtual bool drawPrevious() {
            return false;
        }
        virtual void doPhysics() {
        }
        virtual void keyDown(int key, LPARAM lParam) {
        }
        virtual void keyUp(int key, LPARAM lParam) {
        }
};

stack<Screen*> screenStack;

class ArtPanel: public Screen {
    private:
        int ArtPanelState;
        int ArtefactPanelPos;
        LPDIRECT3DTEXTURE9 ArtPanelTex;
        LPDIRECT3DTEXTURE9 ArtefactImg[12];
        vector<string> ArtList;
    public:
        ArtPanel() {
            ArtPanelState = 1;
            ArtefactPanelPos = -200;
            TextureManager.CreateEx("Artpanel", false, 0, 0, D3DCOLOR_ARGB(255, 0, 0, 0), &ArtPanelTex);

            for(int i = 0; i < 12; i++)
                TextureManager.CreateEx("l" + str(i), false, 0, 0, TCOLOR, &ArtefactImg[i]);

            for (int i = 0; i < 11; i++)
                ArtList.push_back( loadTextResource(TEXTRES_LOCATION, "Text_artefact_" + str(i + 1) ) );
        }
        bool drawPrevious() {
            return true;
        }
        void doPhysics() {

        }
        void render() {
            if (ArtPanelState == ARTPANEL_VISIBLE) {
                MainSprite->Draw(ArtPanelTex, NULL, NULL, NULL, FCOLOR);

                int qa = 0;

                for(int i = 0; i < 11; i++)
                    if(RangerArtefacts[i] != 0) {
                        MainSprite->Draw(ArtefactImg[i], NULL, NULL, &CVector(24, 43 + qa, 0), FCOLOR);
                        qa += 20;
                    }

                for(int i = 0; i < GameCheckListBox.ItemCount; i++) {
                    if(GameCheckListBox.Items[i].Checked)
                        MainSprite->Draw(GameCheckListBox.Texture, &GameCheckListBox.CheckedItem, NULL, &CVector(GameCheckListBox.Position.left,
                                         GameCheckListBox.Position.top + 20 * i, 0), FCOLOR);
                    else
                        MainSprite->Draw(GameCheckListBox.Texture, &GameCheckListBox.UncheckedItem, NULL, &CVector(GameCheckListBox.Position.left,
                                         GameCheckListBox.Position.top + 20 * i, 0), FCOLOR);

                    GameCheckListBox.Font->DrawTextA(MainSprite, GameCheckListBox.Items[i].Name.c_str(), -1,
                                                    &CRect(GameCheckListBox.Position.left + 20, GameCheckListBox.Position.top + 20 * i, 2000, 2000), 0, D3DCOLOR_ARGB(255, 0, 0, 0));
                }

                MainSprite->Draw(ArtefactImg[11], NULL, NULL, &CVector(25, 43 + qa, 0), FCOLOR);

                string Line = ""; //"На борту "+String((int)PlayerManager.PlayerParameters[7])+" КБ.";

                GameCheckListBox.Font->DrawTextA(MainSprite, Line.c_str(), -1, &CRect(55, 43 + qa, 1000, 1000), 0, D3DCOLOR_XRGB(0, 0, 0));

                if(GameCheckListBox.CheckedItemNumber > -1) {

                    int Number = stringlist[GameCheckListBox.CheckedItemNumber];
                    string text = ArtList[Number];
                    MainFont->DrawTextA(MainSprite, text.c_str(), -1, &CRect(35, 325, 155 + 35, 100 + 325), 0, D3DCOLOR_XRGB(0, 0, 0));
                }
            }

            if (ArtPanelState == ARTPANEL_SHOWING) {



                /* to be done */
                MainSprite->Draw(ArtPanelTex, NULL, NULL, &CVector(ArtefactPanelPos, 0, 0), FCOLOR);
                ArtefactPanelPos += 4;

                stringlist.clear();

                if(ArtefactPanelPos >= 4) {
                    GameCheckListBox.Visible = true;
                    GameCheckListBox.ItemCount = 0;
                    GameCheckListBox.Position = CRect(50, 43, 1000, 1000);

                    for(int i = 0; i < 11; i++)
                        if(RangerArtefacts[i] != 0) {
                            GameCheckListBox.ItemCount++;
                            GameCheckListBox.Items[GameCheckListBox.ItemCount - 1].Name = strlist[i].c_str();

                            if(RangerArtefacts[i] == 1)
                                GameCheckListBox.Items[GameCheckListBox.ItemCount - 1].Checked = true;
                            else
                                GameCheckListBox.Items[GameCheckListBox.ItemCount - 1].Checked = false;

                            stringlist.push_back(i);
                        }

                    ArtPanelState = ARTPANEL_VISIBLE;

                    if(GameCheckListBox.ItemCount > 0)
                        GameCheckListBox.CheckedItemNumber = 0;
                    else
                        GameCheckListBox.CheckedItemNumber = -1;
                }
            }

            if (ArtPanelState == ARTPANEL_HIDING) {


                /* hiding */
                MainSprite->Draw(ArtPanelTex, NULL, NULL, &CVector(ArtefactPanelPos, 0, 0), FCOLOR);
                ArtefactPanelPos -= 4;

                if(ArtefactPanelPos == -200) {
                    ArtPanelState = ARTPANEL_HIDDEN;
                    Pause(0);
                    PlayerManager.RebuildParameters();
                }
            }
        }
        void keyDown(int key, LPARAM lParam) {
            if (key == BT_P) {
                ArtPanelState = 3;
                GameCheckListBox.Visible = false;
            }
        }
};

class CoolMessageBox: public Screen {
    private:
        string text;
        int type;
        int buttons;
        vector<string> lines;
    public:
        CoolMessageBox(string text, int type, int buttons, int messageID) {
            this->text = text;
            this->type = type;
            this->buttons = buttons;
            this->lines.clear();

            int l = text.length();

            int lineSize;

            if (l <= 75)             lineSize = 25;

            if (l <= 100 && l > 75)  lineSize = 28;

            if (l > 100 && l <= 150) lineSize = 30;

            if (l > 150 && l < 200)  lineSize = 35;

            if (l >= 200)            lineSize = 45;

            vector<string> parts;

            int last = 0;

            for (int i = 0; i < l; i++)
                if (text[i] == ' ') {
                    if (i - last) parts.push_back( text.substr(last, i - last) );

                    last = i + 1;
                }

            if (l - last) parts.push_back( text.substr(last, l - last) );

            string s;

            for (int i = 0; i < parts.size(); i++) {
                if (s.length() + 1 + parts[i].length() > lineSize) {
                    lines.push_back(s);
                    s = "";
                }

                s = s + " " + parts[i];
            }

            if (s.length())
                lines.push_back(s);


            int TextHeight = 12 * lines.size();
            int MessageHeight = TextHeight + 150;

            MessageButton1.CurrentState = 1;

            if(buttons == 1) {
                MessageButton1.Position = CRect(400 - 24, 265 + MessageHeight / 2 - 20, 400 + 24, 265 + MessageHeight / 2 + 21 - 20);
                ButtonsManager.Add(MessageButton1, GB_MESSAGE1, &MessageButton1Event);
                ButtonsManager.ChangeVisible(GB_MESSAGE1, true);
            }

            if(buttons == 2) {
                MessageButton1.Position = CRect(400 - 24 - 30, 265 + MessageHeight / 2 - 20, 400 - 24 + 48 - 30, 265 + MessageHeight / 2 + 21 - 20);
                ButtonsManager.Add(MessageButton1, GB_MESSAGE1, &MessageButton1Event);
                ButtonsManager.ChangeVisible(GB_MESSAGE1, true);

                MessageButton2.Position = CRect(400 - 24 + 30, 265 + MessageHeight / 2 - 20, 400 - 24 + 48 + 30, 265 + MessageHeight / 2 + 21 - 20);
                ButtonsManager.Add(MessageButton2, GB_MESSAGE2, &MessageButton2Event);
                ButtonsManager.ChangeVisible(GB_MESSAGE2, true);
            }
        }

        void render() {
            LPD3DXSPRITE Sprite = MainSprite;
            D3DXMATRIXA16 Temp;

            int maxLength = 0;

            for (int i = 0; i < lines.size(); i++)
                maxLength = max(maxLength, lines[i].length());

            int TextWidth = 5.5 * maxLength;
            int MessageWidth = TextWidth + 80;

            if(MessageWidth < 303) {
                MessageWidth = 303;
                TextWidth = MessageWidth - 80;
            }

            int TextHeight = 12 * lines.size();
            int MessageHeight = TextHeight + 150;

            D3DXVECTOR3 Vec = CVector(-(800 - MessageWidth) / 2, -(600 - MessageHeight) / 2, 0);

            Sprite->Draw(Angle[0], NULL, &Vec, NULL, FCOLOR);

            int VertLines = (MessageWidth - 277) / 6;
            int HorizLines = (MessageHeight - 186) / 6;
            int q;

            for (int i = 0; i < VertLines; i++) {
                Sprite->Draw(Line[0], NULL, &Vec, &CVector(140 + 6 * i, 0, 0), FCOLOR);

                for(q = 0; q < HorizLines; q++)
                    Sprite->Draw(Centre, NULL, &Vec, &CVector(140 + 6 * i, 93 + 6 * q, 0), FCOLOR);

                Sprite->Draw(Line[2], NULL, &Vec, &CVector(140 + 6 * i, 93 + 6 * HorizLines, 0), FCOLOR);
            }

            Sprite->Draw(Angle[1], NULL, &Vec, &CVector(140 + VertLines * 6, 0, 0), FCOLOR);

            for (int i = 0; i < HorizLines; i++) {
                Sprite->Draw(Line[3], NULL, &Vec, &CVector(0, 93 + 6 * i, 0), FCOLOR);

                for(q = 0; q < VertLines; q++)
                    Sprite->Draw(Centre, NULL, &Vec, &CVector(140 + 6 * q, 93 + 6 * i, 0), FCOLOR);

                Sprite->Draw(Line[1], NULL, &Vec, &CVector(140 + 6 * VertLines, 93 + 6 * i, 0), FCOLOR);
            }

            Sprite->Draw(Angle[3], NULL, &Vec, &CVector(0, 93 + 6 * HorizLines, 0), FCOLOR);
            Sprite->Draw(Angle[2], NULL, &Vec, &CVector(140 + VertLines * 6, 93 + 6 * HorizLines, 0), FCOLOR);

            Sprite->Draw(Light[type / 10 - 1], NULL, &Vec, &CVector((MessageWidth - 161) / 2, 0, 0), FCOLOR);
            Sprite->Draw(Icons[type % 10 - 1], NULL, &Vec, &CVector(12, 55, 0), FCOLOR);

            for (int i = 0; i < lines.size(); i++) {
                string s = lines[i];
                MainFont->DrawTextA(Sprite,
                                   s.c_str(),
                                   -1,
                                   &CRect((TextWidth - s.length() * 5.5) / 2 + 50 - Vec.x, 60 + 12 * i - Vec.y, 1000, 1000), 0, D3DCOLOR_XRGB(0, 0, 0));
            }

            ButtonsManager.Draw();
            Sprite->SetTransform(&IdentityMatrix);
        }
        bool drawPrevious() {
            return true;
        }
        void keyDown(int key, LPARAM lParam) {
            switch(key) {
                case BT_ESC:
                    MessageButton2Event();
                    break;

                case BT_ENTER:
                    MessageButton1Event();
                    break;
            }
        }
};

class RecordsScreen: public Screen {
    private:
        LPDIRECT3DTEXTURE9 background;
    public:
        RecordsScreen() {
            TextureManager.Create("Controls", false, &background);
        }
        void render() {
            MainSprite->Draw(background, NULL, NULL, NULL, FCOLOR);
            MainFont->DrawTextA(MainSprite, "You did not end game yet.", -1, &CRect(260, 250, 1000, 1000), 0, D3DCOLOR_XRGB(125, 125, 255));
            MainFont->DrawTextA(MainSprite, "Для возврата нажмите Esc.", -1, &CRect(265, 500, 1000, 1000), 0, D3DCOLOR_XRGB(125, 125, 255));
        }
};

class AuthorsScreen: public Screen {
    private:
        LPDIRECT3DTEXTURE9 background;
        int AuthorsTemp;
        int DevVar;
        vector<string> slist;
    public:
        AuthorsScreen() {
            TextureManager.Create("Authors", false, &background);
            AuthorsTemp = 0;
            slist = split( loadTextResource(TEXTRES_LOCATION, "Text_Developers"), "\n");
        }
        void render() {
            MainSprite->Draw(background, NULL, NULL, NULL, FCOLOR);

            for (int i = 0; i < slist.size(); i++)
                if (550 - DevVar + 22 * i >= 0)
                    MainFont->DrawTextA(MainSprite, slist[i].c_str(), -1, &CRect(340, (550 - DevVar + 22 * i + 20), 2000, 530), 0, D3DCOLOR_XRGB(255, 0, 0));

            int diff = 550 - DevVar + 22 * slist.size();

            if(diff <= 0) {
                MainFont->DrawTextA(MainSprite, "Для возврата нажмите Esc.", -1, &CRect(340, (20 + 5), 2000, 2000), 0, D3DCOLOR_XRGB(255, 0, 0));
            }
        }
        void doPhysics() {
            int a = slist.size() - 2;

            if((550 - DevVar + 22 * a >= 5) || (AuthorsTemp == 10))
                DevVar += 20;
            else
                AuthorsTemp += 1;
        }
};

class ControlsScreen: public Screen {
    private:
        LPDIRECT3DTEXTURE9 background;
    public:
        ControlsScreen() {
            TextureManager.Create("Controls", false, &background);
        }
        void render() {
            MainSprite->Draw(background, NULL, NULL, NULL, FCOLOR);

            RECT rc;

            SetRect( &rc, 215, 50, 2000, 2000 );
            MainFont->DrawTextA(MainSprite, "[Up,Down,Left,Right] - перемещение корабля.", -1, &rc, 0, D3DCOLOR_XRGB(125, 125, 255));
            SetRect( &rc, 285, 90, 2000, 2000 );
            MainFont->DrawTextA(MainSprite, "[1,2,3,4,5] - смена оружия.", -1, &rc, 0, D3DCOLOR_XRGB(125, 125, 255));
            SetRect( &rc, 292, 130, 2000, 2000 );
            MainFont->DrawTextA(MainSprite, "[Shift] - кварковая бомба.", -1, &rc, 0, D3DCOLOR_XRGB(125, 125, 255));
            SetRect( &rc, 250, 170, 2000, 2000 );
            MainFont->DrawTextA(MainSprite, "[Cntl] - стрельба первичного оружия.", -1, &rc, 0, D3DCOLOR_XRGB(125, 125, 255));
            SetRect( &rc, 232, 210, 2000, 2000 );
            MainFont->DrawTextA(MainSprite, "[Alt] - стрельба альтернативного оружия.", -1, &rc, 0, D3DCOLOR_XRGB(125, 125, 255));
            SetRect( &rc, 242, 250, 2000, 2000 );
            MainFont->DrawTextA(MainSprite, "[A] - автострельба первичного оружия.", -1, &rc, 0, D3DCOLOR_XRGB(125, 125, 255));
            SetRect( &rc, 226, 290, 2000, 2000 );
            MainFont->DrawTextA(MainSprite, "[S] - автострельба альтенативного оружия.", -1, &rc, 0, D3DCOLOR_XRGB(125, 125, 255));
            SetRect( &rc, 295, 330, 2000, 2000 );
            MainFont->DrawTextA(MainSprite, "[P] - показать артефакты.", -1, &rc, 0, D3DCOLOR_XRGB(125, 125, 255));
            SetRect( &rc, 325, 370, 2000, 2000 );
            MainFont->DrawTextA(MainSprite, "[F] - показать FPS.", -1, &rc, 0, D3DCOLOR_XRGB(125, 125, 255));
            SetRect( &rc, 318, 410, 2000, 2000 );
            MainFont->DrawTextA(MainSprite, "[Pause] - пауза игры.", -1, &rc, 0, D3DCOLOR_XRGB(125, 125, 255));
            SetRect( &rc, 347, 450, 2000, 2000 );
            MainFont->DrawTextA(MainSprite, "[Esc] - выход.", -1, &rc, 0, D3DCOLOR_XRGB(125, 125, 255));
            SetRect( &rc, 292, 510, 2000, 2000 );
            MainFont->DrawTextA(MainSprite, "Для возврата нажмите Esc.", -1, &rc, 0, D3DCOLOR_XRGB(125, 125, 255));
        }
};

class BriefingScreen: public Screen {
    private:
        LPDIRECT3DTEXTURE9 background;
        bool BriefingPause;
        bool BriefingScrollUp, BriefingScrollDown;
        double BriefingSpeed;
        double Briefinga;
        double Briefingc;
        vector<string> list;
        LPDIRECT3DTEXTURE9 Briefingbmp1;
    public:
        BriefingScreen(int Number) {
            string FileName = "Data\\mission_1.mis";
            freopen(FileName.c_str(), "r", stdin);

            int w;
            scanf("%d\n", &w);
            char s[250];
            gets(s);

            while(s[0] != '#') {
                list.push_back(s);
                gets(s);
            }

            BriefingSpeed = 1;
            Briefinga = 1;

            TextureManager.Create("BriefingFon", false, &background);
            TextureManager.CreateEx("BriefingPanel", false, 0, 0, D3DCOLOR_XRGB(0, 0, 0), &Briefingbmp1);
            TextureManager.CreateEx("BriefingButtonTexture", false, 0, 0, TCOLOR, &MyButton.Texture);
        }
        void render() {
            MainSprite->Draw(background, NULL, NULL, &CVector(760 - Briefinga, 760 - Briefinga, 0), FCOLOR);
            MainSprite->Draw(background, NULL, NULL, &CVector(-Briefinga, -Briefinga, 0), FCOLOR);

            for(int i = 0; i < list.size(); i++)
                if(400 - Briefingc + 20 * i >= 0)
                    if(400 - Briefingc + 20 * i <= 580)
                        MainFont->DrawTextA(MainSprite, list[i].c_str(), -1, &CRect(335, 400 - Briefingc + 20 * i, SCREEN_WIDTH, SCREEN_HEIGHT), 0, D3DCOLOR_XRGB(0, 255, 255));

            MainSprite->Draw(Briefingbmp1, NULL, NULL, NULL, FCOLOR);
            ButtonsManager.Draw();
        }
        void doPhysics() {
            Briefinga += 0.333;

            if(Briefinga >= 760)
                Briefinga = 0;

            if((!BriefingScrollUp) && (!BriefingScrollDown))
                Briefingc += BriefingSpeed / 3;

            if(BriefingScrollUp)
                Briefingc += 2;

            if(BriefingScrollDown)
                Briefingc -= 2;

            if(Briefingc > 20 * list.size() - 110) {
                BriefingSpeed = 0;
                Briefingc = 20 * list.size() - 110;
            }

            if(Briefingc < 0) {
                BriefingSpeed = 0;
                Briefingc = 0;
            }
        }
        void keyUp(int key, LPARAM lParam) {
            switch(key) {
                case BT_UP:
                    BriefingScrollUp = false;
                    break;

                case BT_DOWN:
                    BriefingScrollDown = false;
                    break;
            }
        }
        void keyDown(int key, LPARAM lParam) {
            switch(key) {
                case BT_NUMPLUS:
                    BriefingSpeed += 0.5;

                    if(BriefingSpeed > 2) BriefingSpeed = 2;

                    break;

                case BT_NUMMINUS:
                    BriefingSpeed -= 0.5;

                    if(BriefingSpeed < -2)BriefingSpeed = -2;

                    break;

                case BT_UP:
                    BriefingScrollUp = true;
                    break;

                case BT_DOWN:
                    BriefingScrollDown = true;
                    break;

                case BT_ENTER:
                    BriefingButtonEvent();
                    break;
            }
        }
};

class MainMenuScreen: public Screen {
    private:
        void SetUpMenu() {
            EnemiesManager.EnemySeries = random(3);

            int i;

            for(i = 0; i < 6; i++)
                PanelButtons[i].Visible = true;

            for(i = 0; i < 100; i++) {
                Shleif[i][0] = 0;
                Shleif[i][1] = 0;
                Shleif[i][2] = 0;
            }

            for(i = 0; i < 100; i++) {
                Artefacts[i][1] = 0;
                Artefacts[i][2] = 0;
                Artefacts[i][3] = 0;
            }

            for(i = 0; i < 100; i++) {
                Explodings[i][1] = 0;
                Explodings[i][2] = 0;
                Explodings[i][3] = 0;
            }

            PlayerManager.Player.xpos = 350;
            PlayerManager.Player.ypos = 500;
            PlayerManager.Player.HP = 1000000;
            PlayerManager.Player.MaxHP = 1000000;
            PlayerManager.Player.WeaponPower = 1;
            PlayerManager.Player.LeftSpeed = 0;
            PlayerManager.Player.RightSpeed = 0;
            PlayerManager.Player.TopSpeed = 0;
            PlayerManager.Player.BottomSpeed = 0;
            PlayerManager.Player.CurrentWeapon = 1;
            PlayerManager.Player.Visible = true;

            Fire = false;
            FireAlt = false;

            for(int i = 1; i <= 5; i++)
                EnemiesManager.EnemySetMax(i, 0);

            Boss[1] = 0;

            EnemyShotsManager->Clear();

            PlayerShotsManager->Clear();

            EnemiesManager.EnemySetMax(1, 25);

            for(int i = 0; i < 25; i++) {
                EnemiesManager.Enemies[0][i].ypos -= random(1000) - 400;
            }


            StarsManager->Set();

            Undamager = false;
            AlreadyLoad = true;
            StartTimers(true);
        }
    public:
        MainMenuScreen() {
            SetUpMenu();
        }
        void render() {
            D3DXMatrixScaling(&MainMatrix, 1, 1, 1);
            MainDevice->SetTransform(D3DTS_WORLD, &MainMatrix);
            //        MainDevice->SetTransform(D3DTS_WORLD,&IdentityMatrix);

            int i;
            D3DXMATRIXA16 Matr;
            D3DXMatrixScaling(&Matr, FON_SCALING, FON_SCALING, 1);
            MainSprite->SetTransform(&Matr);

            float Z = FON_SCALING;

            if(Fon_top <= 3400)
                MainSprite->Draw(Fon, &CRect(0, (3400 - Fon_top) / Z, 800 / Z, (4000 - Fon_top) / Z), NULL, NULL, FCOLOR);

            if(Fon_top > 3400) {
                MainSprite->Draw(Fon, &CRect(0, 0, 800 / Z, (4000 - Fon_top) / Z), NULL, &CVector(0, (Fon_top - 3400) / Z, 0), FCOLOR);
                MainSprite->Draw(Fon, &CRect(0, (7400 - Fon_top) / Z, 800 / Z, 4000 / Z), NULL, &CVector(0, 0, 0), FCOLOR);
            }

            MainSprite->SetTransform(&IdentityMatrix);
            PlayerShotsManager->Draw();

            MainSprite->End();
            StarsManager->Draw();
            MainSprite->Begin(D3DXSPRITE_ALPHABLEND);
            MainDevice->SetTransform(D3DTS_WORLD, &MainMatrix);

            EnemyShotsManager->Draw();
            EnemiesManager.Draw();

            for(i = 0; i < 100; i++)
                if(Shleif[i][1] == 1) {
                    RECT Rect1 = UpdateRect(DrawingRects[12], Shleif[i][2], Shleif[i][3]);
                    RECT Rect2 = UpdateRect(DrawingRects[12], (5 - Shleif[i][4] / 5), 0);

                    MainSprite->Draw(RocketShleifImages, &CRect(Rect2.left, Rect2.top, Rect2.right, Rect2.bottom), NULL,
                                     &CVector(Rect1.left, Rect1.top, 0), FCOLOR);
                }

            PlayerManager.Draw();

            for(i = 0; i < 200; i++)
                if((Explodings[i][0] == 1)) {
                    MainSprite->Draw(Explo[(Explodings[i][2])], NULL, NULL, &CVector(Explodings[i][4], Explodings[i][5], 0), FCOLOR);
                    Explodings[i][2] += 1;

                    if(Explodings[i][2] >= Explodings[i][3])
                        Explodings[i][0] = 0;
                }

            if(PanelButtons[0].Visible) {
                MainSprite->Draw(fuck, NULL, NULL, &CVector(800 - 224 - 40, 100, 0), FCOLOR);

                for(i = 0; i < 6; i++)
                    MainSprite->Draw(PanelButtons[i].Texture, &PanelButtons[i].States[PanelButtons[i].CurrentState - 1],
                                     NULL, &CVector(PanelButtons[i].Position.left, PanelButtons[i].Position.top, 0), FCOLOR);
            }

            MainSprite->Draw(GeneralFon, &CRect(0, 0, 43, 600), NULL, &CVector(0, 0, 0), D3DCOLOR_ARGB(150, 255, 255, 255));
            MainSprite->Draw(GeneralFon, &CRect(43, 0, 715 + 43, 43), NULL, &CVector(43, 0, 0), D3DCOLOR_ARGB(150, 255, 255, 255));
            MainSprite->Draw(GeneralFon, &CRect(758, 0, 42 + 758, 600), NULL, &CVector(758, 0, 0), D3DCOLOR_ARGB(150, 255, 255, 255));
            MainSprite->Draw(GeneralFon, &CRect(43, 558, 715 + 43, 42 + 558), NULL, &CVector(43, 558, 0), D3DCOLOR_ARGB(150, 255, 255, 255));

            MainSprite->Draw(RangersLogo, NULL, NULL, &CVector(505, 10, 0), FCOLOR);

            int FPS = FPSShower.GetFPS();

            if((FPS != 0) && (NeedShowFPS))
                MainFont->DrawTextA(MainSprite, (str(FPS) + " fps").c_str(), -1, &CRect(370, 20, 1000, 1000), 0, D3DCOLOR_XRGB(255, 0, 0));
        }
        void keyDown(int key, LPARAM lParam) {
            if (key == BT_ESC) {
                exit(0);
            }
        }
        bool canGoBack() {
            return false;
        }
        void doPhysics() {
            PlayerManager.TryPlayerFire();
            PlayerShotsManager->Move();
            EnemyShotsManager->Move();
            MoveArtefacts();
            StarsManager->Move();
            EnemiesManager.MoveEnemies();
            MoveShleif();
            MoveRocketShleif();
            PlayerManager.RepairPlayerShip();
            TestForCrash();

            if(Undamager) {
                UndamagerCoff -= 1;

                if(UndamagerCoff < 0)
                    Undamager = false;
            }

            PlayerStatistic.GameTime += 1 / 75.0;
            ShipShleif(PlayerManager.Player.xpos, PlayerManager.Player.ypos);

			if((PlayerManager.Player.Lifes >= 0) && (!PauseNow) && (!RangerDeads))
				Fon_top += 1;
			if(Fon_top >= 4000) Fon_top = 0;
        }
};

class GameScreen: public Screen {
    private:
        void StartLevel(int level) {
            ButtonsManager.ChangeVisible(GB_BRIEFING, false);

            EnemiesManager.EnemySeries = random(3);

            FireInterval = 1;
            CurrentFonNumber = random(6) + 1;

            int Index = TextureManager.FindByTexture(&Fon);

            if(Index >= 0)
                TextureManager.Free(Index);

            string Nm = "Fon" + str(CurrentFonNumber);
            TextureManager.Create(Nm, false, &Fon);

            PlayerManager.Clear();


            RangerDeads = false;
            PauseNow = false;
            AlreadyPressed = false;
            Fire = false;
            FireAlt = false;
            PauseNow = false;
            Undamager = false;

            PlayerManager.Player.xpos = 370;
            PlayerManager.Player.ypos = 430;
            PlayerManager.Player.Lifes = 3;
            PlayerManager.Player.Experience = 0;
            PlayerManager.Player.HP = 100;
            PlayerManager.Player.MaxHP = 100;
            PlayerManager.Player.WeaponPower = 1;
            PlayerManager.Player.LeftSpeed = 0;
            PlayerManager.Player.RightSpeed = 0;
            PlayerManager.Player.TopSpeed = 0;
            PlayerManager.Player.BottomSpeed = 0;
            PlayerManager.Player.CurrentWeapon = 1;
            PlayerManager.Player.Visible = true;

            PlayerManager.Player.EnabledNewLifes = false;
            double x[15] = {0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1};
            memcpy(PlayerManager.PlayerParameters, x, sizeof(double) * 15);

            FireInterval = 1;

            PlayerManager.RangerWeapon[1][0][1] = 1;
            PlayerManager.RangerWeapon[1][1][1] = 1;

            PlayerManager.RangerWeapon[1][0][3] = 25;
            PlayerManager.RangerWeapon[1][1][3] = 50;
            PlayerManager.RangerWeapon[2][0][3] = 30;
            PlayerManager.RangerWeapon[2][1][3] = 150;
            PlayerManager.RangerWeapon[3][0][3] = 3;
            PlayerManager.RangerWeapon[3][1][3] = 50;
            PlayerManager.RangerWeapon[4][0][3] = 40;
            PlayerManager.RangerWeapon[4][1][3] = 75;
            PlayerManager.RangerWeapon[5][0][3] = 100;
            PlayerManager.RangerWeapon[5][1][3] = 300;

            for(int k = 1; k < 6; k++)
                for(int h = 0; h < 2; h++)
                    PlayerManager.RangerWeapon[k][h][5] = PlayerManager.RangerWeapon[k][h][3];

            TimerManager->SetStatus(T9, true);
            TimerManager->SetStatus(T18, true);

            TimerManager->SetStatus(DEAD_TIMER, false);

            int i, c;

            for(i = 0; i < 12; i++)
                RangerArtefacts[i] = 0;

            for(c = 0; c < 5; c++)
                for (i = 0; i < 25; i++)
                    EnemiesManager.Enemies[c][i].Exists = false;

            Boss[1] = 0;

            for(i = 0; i < 100; i++)
                Artefacts[i][1] = 0;

            PlayerShotsManager->Clear();

            EnemyShotsManager->Clear();

            for(i = 0; i < 100; i++)
                Shleif[i][1] = 0;

            StarsManager->Set();

            for(i = 0; i < 5; i++) {
                EnemiesManager.EnemySetMax(i + 1, 0);

                if(i == 0)
                    EnemiesManager.EnemySetMax(i + 1, 5);
            }

            for(i = 0; i < 200; i++)
                Explodings[i][0] = 0;

            PlayerStatistic.Clear();
            PlayerStatistic.ShowStatistic = false;

            char s[250] = "2";

            while(s[0] != '#')
                gets(s);

            InitTriggers();
        }
    public:
        GameScreen(int level) {
            StartLevel(level);
        }
        void render() {
            int i;
            D3DXMATRIXA16 Matrix;
            D3DXMatrixScaling(&Matrix, FON_SCALING, FON_SCALING, 1);
            MainSprite->SetTransform(&Matrix);

            float Z = FON_SCALING;

            if(Fon_top <= 3400)
                MainSprite->Draw(Fon, &CRect(0, (3400 - Fon_top) / Z, 800 / Z, (4000 - Fon_top) / Z), NULL, NULL, FCOLOR);

            if(Fon_top > 3400) {
                MainSprite->Draw(Fon, &CRect(0, 0, 800 / Z, (4000 - Fon_top) / Z), NULL, &CVector(0, (Fon_top - 3400) / Z, 0), FCOLOR);
                MainSprite->Draw(Fon, &CRect(0, (7400 - Fon_top) / Z, 800 / Z, 4000 / Z), NULL, &CVector(0, 0, 0), FCOLOR);
            }

            MainSprite->SetTransform(&IdentityMatrix);

            for(i = 0; i < 150; i++)
                if(RocketShleif[i][1] == 1) {
                    int h = (RocketShleif[i][4] - 1) / 2 + 1;
                    RECT Rect1 = UpdateRect(DrawingRects[12], RocketShleif[i][2], RocketShleif[i][3]);
                    RECT Rect2 = UpdateRect(DrawingRects[12], h, 0);
                    MainSprite->Draw(RocketShleifImages, &CRect(Rect2.left, Rect2.top, Rect2.right, Rect2.bottom), NULL,
                                     &CVector(Rect1.left, Rect1.top, 0), FCOLOR);
                }

            MainSprite->End();
            StarsManager->Draw();

            for(i = 0; i < 100; i++)
                if(Lines[i][1] == 1) {
                    D3DCOLOR Col = D3DCOLOR_ARGB(255, 200, 200, 255);
                    DrawLighting(Lines[i][2], Lines[i][3], Lines[i][4], Lines[i][5], Col);
                    DrawLighting(Lines[i][2], Lines[i][3], Lines[i][4], Lines[i][5], Col);
                    DrawLighting(Lines[i][2], Lines[i][3], Lines[i][4], Lines[i][5], Col);
                    Lines[i][1] = 0;
                }

            MainSprite->Begin(D3DXSPRITE_ALPHABLEND);
            MainDevice->SetTransform(D3DTS_WORLD, &MainMatrix);

            PlayerShotsManager->Draw();

            EnemyShotsManager->Draw();

            EnemiesManager.Draw();

            for(i = 0; i < 100; i++)
                if(RangerWeaponEffects[i][1] == 1) {
                    int Z = WEFFECT_SCALING;
                    D3DXMatrixScaling(&Matrix, Z, Z, 1);
                    MainSprite->SetTransform(&Matrix);

                    int Count = RangerWeaponEffects[i][4] / 2;
                    RECT Rect1, Rect2;
                    Rect1 = UpdateRect(DrawingRects[13], RangerWeaponEffects[i][2] / Z, RangerWeaponEffects[i][3] / Z);
                    Rect2 = UpdateRect(DrawingRects[13], 199 * Count / 2, 0); //199, а не DrawingRects[13].Width() потому,
                    // что первичная ширина была 199 и она не делится на 2

                    MainSprite->Draw(Weapon1Effect, &CRect(Rect2.left, Rect2.top, Rect2.right, Rect2.bottom), NULL,
                                     &CVector(Rect1.left, Rect1.top, 0), FCOLOR);

                    RangerWeaponEffects[i][4]++;

                    if(RangerWeaponEffects[i][4] == RangerWeaponEffects[i][5])
                        RangerWeaponEffects[i][1] = 0;

                    MainSprite->SetTransform(&IdentityMatrix);
                }

            for(i = 0; i < 100; i++)
                if(Artefacts[i][1] == 1)
                    MainSprite->Draw(Artefact, NULL, NULL, &CVector(Artefacts[i][2], Artefacts[i][3], 0), FCOLOR);

            if(Boss[1] == 1) {
                MainSprite->Draw(BossI, NULL, NULL, &CVector(Boss[2], Boss[3], 0), FCOLOR);

                MainSprite->End();

                D3DXVECTOR2 Vector[2];

                MainLine->Begin();
                MainDevice->SetTransform(D3DTS_WORLD, &MainMatrix);

                if(Boss[5] > Boss[6] / 2) {
                    Vector[0].x = Boss[2];
                    Vector[0].y = (Boss[3] - 10);
                    Vector[1].x = (Boss[2] + 200);
                    Vector[1].y = (Boss[3] - 10);
                    MainLine->Draw(Vector, 2, D3DCOLOR_ARGB(255, 0, 255, 0));

                    Vector[0] = Vector[1];
                    Vector[1].x = (Boss[2] + 200.0 * (Boss[5] - Boss[6] / 2) / Boss[6] * 2);
                    MainLine->Draw(Vector, 2, D3DCOLOR_ARGB(255, 255, 0, 0));
                } else {
                    Vector[0].x = Boss[2];
                    Vector[0].y = (Boss[3] - 10);
                    Vector[1].x = (Boss[2] + 200.0 * Boss[5] / 15000);
                    Vector[1].y = (Boss[3] - 10);
                    MainLine->Draw(Vector, 2, D3DCOLOR_ARGB(255, 255, 0, 0));
                }

                MainLine->End();

                MainSprite->Begin(D3DXSPRITE_ALPHABLEND);
                MainDevice->SetTransform(D3DTS_WORLD, &MainMatrix);
            }

            for(i = 0; i < 200; i++)
                if((Explodings[i][0] == 1)) {
                    switch(Explodings[i][1]) {
                        case 1:
                            MainSprite->Draw(Explo[Explodings[i][2]], NULL, NULL,
                                             &CVector(Explodings[i][4], Explodings[i][5], 0), FCOLOR);
                            break;

                        case 2: {
                            int Z = EXPL_SCALING;

                            D3DXMatrixScaling(&Matrix, Z, Z, 1);
                            MainSprite->SetTransform(&Matrix);

                            int Count = Explodings[i][2] / 2;
                            RECT Rect1 = UpdateRect(DrawingRects[6], (Explodings[i][4] - 30) / Z, Explodings[i][5] / Z);
                            RECT Rect2 = UpdateRect(DrawingRects[6], 65 * Count, 0);
                            MainSprite->Draw(CoolExploding1, &CRect(Rect2.left, Rect2.top, Rect2.right, Rect2.bottom),
                                             NULL, &CVector(Rect1.left, Rect1.top, 0), FCOLOR);

                            MainSprite->SetTransform(&IdentityMatrix);
                        }
                        break;

                        case 3: {
                            int Z = EXPL_SCALING;

                            D3DXMatrixScaling(&Matrix, Z, Z, 1);
                            MainSprite->SetTransform(&Matrix);

                            int Count = Explodings[i][2] / 4;
                            RECT Rect1, Rect2;
                            Rect1 = UpdateRect(DrawingRects[5], (Explodings[i][4] - 30) / Z, Explodings[i][5] / Z);
                            Rect2 = UpdateRect(DrawingRects[5], 121 * Count, 0);
                            MainSprite->Draw(CoolExploding2, &CRect(Rect2.left, Rect2.top, Rect2.right, Rect2.bottom),
                                             NULL, &CVector(Rect1.left, Rect1.top, 0), FCOLOR);

                            MainSprite->SetTransform(&IdentityMatrix);
                        }
                        break;
                    }

                    Explodings[i][2] += 1;

                    if(Explodings[i][2] >= Explodings[i][3])
                        Explodings[i][0] = 0;
                }

            if(Laser[1] == 1) {
                if(!FindFile)
                    MainSprite->Draw(LaserImage, NULL, NULL, &CVector(PlayerManager.Player.xpos - 100, PlayerManager.Player.ypos - 550, 0), FCOLOR);

                if(FindFile) {
                    D3DXMatrixScaling(&Matrix, LaserParam.ScalingX, LaserParam.ScalingY, 1);
                    MainSprite->SetTransform(&Matrix);
                    MainSprite->Draw(LaserImage, NULL, NULL, &CVector((PlayerManager.Player.xpos - LaserParam.DeltaX) / LaserParam.ScalingX,
                                     (PlayerManager.Player.ypos - LaserParam.DeltaY) / LaserParam.ScalingY, 0), FCOLOR);
                    MainSprite->SetTransform(&IdentityMatrix);
                }
            }


            for(i = 0; i < 100; i++)
                if(Shleif[i][1] == 1) {
                    RECT Rect1 = UpdateRect(DrawingRects[12], Shleif[i][2], Shleif[i][3]);
                    RECT Rect2 = UpdateRect(DrawingRects[12], (5 - Shleif[i][4] / 5), 0);
                    MainSprite->Draw(RocketShleifImages, &CRect(Rect2.left, Rect2.top, Rect2.right, Rect2.bottom), NULL,
                                     &CVector(Rect1.left, Rect1.top, 0), FCOLOR);
                }

            if(PlayerManager.Player.Visible) {
                PlayerManager.Draw();

                if(Undamager) {
                    D3DXMatrixRotationZ(&Matrix, random(360) * 3.14 / 180);
                    Matrix = Translate(PlayerManager.Player.xpos + 24, PlayerManager.Player.ypos + 24, 0, Matrix);
                    MainSprite->SetTransform(&Matrix);

                    if(UndamagerCoff > 100)
                        MainSprite->Draw(UndamagerTexture, NULL, NULL, &CVector(-64, -64, 0), FCOLOR);
                    else {
                        int Alpha = UndamagerCoff * 2.55;
                        MainSprite->Draw(UndamagerTexture, NULL, NULL, &CVector(-64, -64, 0), D3DCOLOR_ARGB(Alpha, 255, 255, 255));
                    }

                    for(i = 0; i < 10; i++) {
                        int Angle = random(360);
                        int C = (UndamagerCoff > 100) ? 255 : UndamagerCoff * 2.55;
                        D3DCOLOR Col = D3DCOLOR_ARGB(C, 200, 200, 255);
                        int x1 = PlayerManager.Player.xpos + 24 + 40 * cos(Angle * 3.14 / 180);
                        int y1 = PlayerManager.Player.ypos + 24 + 40 * sin(Angle * 3.14 / 180);
                        int x2 = PlayerManager.Player.xpos + 24 + 15 * cos(Angle * 3.14 / 180);
                        int y2 = PlayerManager.Player.ypos + 24 + 15 * sin(Angle * 3.14 / 180);
                        DrawLighting(x1, y1, x2, y2, Col);
                    }

                    MainSprite->SetTransform(&IdentityMatrix);
                }
            }


            D3DCOLOR CurrColor = D3DCOLOR_XRGB(255, 0, 0);

            MainSprite->Draw(Panel, NULL, NULL, &CVector(0, 575, 0), FCOLOR);

            if(PlayerManager.Player.HP <= PlayerManager.Player.MaxHP / 4) {
                MainSprite->Draw(WarningSpace, NULL, NULL, &CVector(416, 578, 0), FCOLOR);
                CurrColor = D3DCOLOR_XRGB(255, 255, 0);
            }

            string value;
            int pos;
            value = string( (str((int)PlayerManager.Player.HP) + "/" + str((int)PlayerManager.Player.MaxHP)).c_str() );
            pos = 465 - (value.length() * 4);
            MainFont->DrawTextA(MainSprite, value.c_str(), -1, &CRect(pos, 580, 1000, 1000), 0, CurrColor);
            CurrColor = D3DCOLOR_XRGB(255, 0, 0);
            value = string( str((int)PlayerManager.Player.Experience).c_str());
            pos = 585 - (value.length() * 4);
            MainFont->DrawTextA(MainSprite, value.c_str(), -1, &CRect(pos, 580, 1000, 1000), 0, CurrColor);


            if(PlayerManager.Player.EnabledNewLifes) {
                if(PlayerManager.Player.Lifes <= 9)
                    for(i = 0; i < PlayerManager.Player.Lifes; i++)
                        MainSprite->Draw(SmallShip, NULL, NULL, &CVector(720 - PlayerManager.Player.Lifes * 7.5 + 15 * i, 580, 0), FCOLOR);
                else {
                    MainSprite->Draw(SmallShip, NULL, NULL, &CVector(720, 581, 0), FCOLOR);
                    MainFont->DrawTextA(MainSprite, str(PlayerManager.Player.Lifes).c_str(), -1, &CRect(720 - str(PlayerManager.Player.Lifes).length() * 7, 580, 1000, 1000), 0, CurrColor);
                }
            } else {
                MainFont->DrawTextA(MainSprite, "Ещё не открыто.", -1, &CRect(680, 580, 1000, 1000), 0, CurrColor);
            }

            if((PauseNow) && (!RangerDeads))
                MainFont->DrawTextA(MainSprite, "Пауза. Нажмите Pause для продолжения.", -1, &CRect(35, 580, 1000, 1000), 0, CurrColor);

            CornerTextManager.DrawTextEx(5);

            int FPS = FPSShower.GetFPS();

            if((FPS != 0) && (NeedShowFPS))
                MainFont->DrawTextA(MainSprite, (str(FPS) + " fps").c_str(), -1, &CRect(370, 20, 1000, 1000), 0, D3DCOLOR_XRGB(255, 0, 0));

            /*if(PlayerStatistic.ShowStatistic)
              {
              CurrColor=D3DCOLOR_XRGB(100,100,255);

              string Sz;
              Sz="Штипов убито "+ string(String(PlayerStatistic.KilledShtips).c_str());
              SlaveFont->DrawTextA(MainSprite,Sz.c_str(),-1,&CRect(700,0,800,600),0,CurrColor);
              Sz="Меноков убито "+string(String(PlayerStatistic.KilledMenoks).c_str());
              SlaveFont->DrawTextA(MainSprite,Sz.c_str(),-1,&CRect(700,10,800,600),0,CurrColor);
              Sz="Смершей убито "+String(PlayerStatistic.KilledSmershes);
              SlaveFont->DrawTextA(MainSprite,Sz.c_str(),-1,&CRect(700,20,800,600),0,CurrColor);
              Sz="Ургантов убито "+String(PlayerStatistic.KilledUrgants);
              SlaveFont->DrawTextA(MainSprite,Sz.c_str(),-1,&CRect(700,30,800,600),0,CurrColor);
              Sz="Эквенторов убито "+String(PlayerStatistic.KilledEcventors);
              SlaveFont->DrawTextA(MainSprite,Sz.c_str(),-1,&CRect(700,40,800,600),0,CurrColor);
              Sz="Выстрелов сделано "+String(PlayerStatistic.ShotsFired);
              SlaveFont->DrawTextA(MainSprite,Sz.c_str(),-1,&CRect(700,60,800,600),0,CurrColor);
              Sz="Выстрелов попало "+String(PlayerStatistic.ShotsHit);
              SlaveFont->DrawTextA(MainSprite,Sz.c_str(),-1,&CRect(700,70,800,600),0,CurrColor);

              if(PlayerStatistic.ShotsFired!=0)
                Sz="Меткость "+String(PlayerStatistic.ShotsHit*100/PlayerStatistic.ShotsFired)+"%";
              else
                Sz="Меткость 0%";
              SlaveFont->DrawTextA(MainSprite,Sz.c_str(),-1,&CRect(700,80,800,600),0,CurrColor);

              int GameTime=PlayerStatistic.GameTime;
              Sz="Время игры "+String(GameTime/3600)+":"+String(GameTime%3600/60)+":"+String(GameTime%60);
              SlaveFont->DrawTextA(MainSprite,Sz.c_str(),-1,&CRect(700,100,800,600),0,CurrColor);
                  } */
            //--------------------Current end of last code------------------------
            NumberShower.Draw();
        }

        bool canGoBack() {
            return false;
        }

        void keyUp(int key, LPARAM lParam) {
            switch (key) {
                case BT_LEFT:
                    PlayerManager.Player.MovingLeft = false;
                    break;

                case BT_UP:
                    PlayerManager.Player.MovingUp = false;
                    break;

                case BT_RIGHT:
                    PlayerManager.Player.MovingRight = false;
                    break;

                case BT_DOWN:
                    PlayerManager.Player.MovingDown = false;
                    break;

                case BT_CTRL:
                    Fire = false;
                    break;

                case BT_ALT:
                    FireAlt = false;
                    break;
            }
        }

        void keyDown(int key, LPARAM lParam) {
            switch (key) {
                case BT_ESC:
                    screenStack.push(new CoolMessageBox("Выйти в главное меню?!", IC_QUESTION + LIGHT_BLUE, 2, MESSAGETYPE_QUIT));
                    break;

                    //            MessageBoxManager.AddMessageBox("Выйти в главное меню?!",IC_QUESTION+LIGHT_BLUE,2,MESSAGETYPE_QUIT); break;
                case BT_PAUSE:
                    Pause(2);
                    break;

                case BT_LEFT:
                    PlayerManager.Player.MovingLeft = true;
                    break;

                case BT_UP:
                    PlayerManager.Player.MovingUp = true;
                    break;

                case BT_RIGHT:
                    PlayerManager.Player.MovingRight = true;
                    break;

                case BT_DOWN:
                    PlayerManager.Player.MovingDown = true;
                    break;

                case BT_SHIFT:
                    SuperWeapon();
                    break;

                case BT_CTRL:
                    Fire = true;
                    break;

                case BT_A:
                    Fire = !Fire;
                    break;

                case BT_ALT:
                    FireAlt = true;
                    break;

                case BT_S:
                    FireAlt = !FireAlt;
                    break;

                case BT_O:
                    PlayerStatistic.ShowStatistic = !PlayerStatistic.ShowStatistic;
                    break;

                case BT_P:
                    screenStack.push( new ArtPanel());
                    break;

                case BT_1:
                case BT_2:
                case BT_3:
                case BT_4:
                case BT_5: {
                    if((Laser[1] == 0) && (PlayerManager.RangerWeapon[key - BT_1 + 1][0][1] == 1))
                        PlayerManager.Player.CurrentWeapon = key - BT_1 + 1;

                    if(PlayerManager.RangerWeapon[key - BT_1 + 1][0][1] == 0)
                        CornerTextManager.Add("This weapon isn't avalaible yet.", 200);

                    if(Laser[1] == 1)
                        CornerTextManager.Add("Can't change weapon. Laser is enabled.", 200);

                    break;
                }
            }
        }

        void doPhysics() {
            PlayerManager.TryPlayerFire();
            PlayerShotsManager->Move();
            EnemyShotsManager->Move();
            MoveArtefacts();
            StarsManager->Move();
            EnemiesManager.MoveEnemies();
            MoveShleif();
            MoveRocketShleif();
            PlayerManager.RepairPlayerShip();
            TestForCrash();

            if(Undamager) {
                UndamagerCoff -= 1;

                if(UndamagerCoff < 0)
                    Undamager = false;
            }

            PlayerStatistic.GameTime += 1 / 75.0;

            PlayerManager.MovePlayerShip();
            PlayerManager.ProceedPhysics();
            ProceedPhysics();
            UpdateGameOverNewScore();
            TriggerManager.UpdateTriggers();

			if((PlayerManager.Player.Lifes >= 0) && (!PauseNow) && (!RangerDeads))
				Fon_top += 1;
			if(Fon_top >= 4000) Fon_top = 0;
        }
};





//---------------------------------------------------------------------------
void ProceedPhysics() {
    int i;
    TPlayer & Player = PlayerManager.Player;
    double pp11 = PlayerManager.PlayerParameters[11];
    double pp5 = PlayerManager.PlayerParameters[5];
    double pp10 = PlayerManager.PlayerParameters[10];
    double pp3 = PlayerManager.PlayerParameters[3];


    ShipShleif(Player.xpos, Player.ypos);

    for(int i = 0; i < 300; i++) {

        RECT PlayerShip = PlayerManager.GetPlayerRect();

        if(EnemyShotsManager->Test(i, PlayerShip, NULL)) {
            if(!Undamager) {
                TPoint spos = EnemyShotsManager->GetPos(i);
                Exploding(1, spos.x, spos.y);
                int power = EnemyShotsManager->GetDamage(i) * (100 - pp11 - pp5) / 100 - pp10 - pp3;

                if(power <= 7)
                    power = 7;

                Player.HP -= power;

                if(Player.HP <= 0)
                    PlayerManager.PlayerDead();
            }
        }
    }

    for(i = 0; i < 100; i++)
        if((Artefacts[i][1] == 1) && (Artefacts[i][2] >= Player.xpos - 10) && (Artefacts[i][2] <= Player.xpos + 60) && (Artefacts[i][3] >= Player.ypos - 10) && (Artefacts[i][3] <= Player.ypos + 60)) {
            Artefacts[i][1] = 0;

            int Art = PlayerManager.NewPlayerArtefact();

            /* string ArtefactText;
            if(Art<12)
                ArtefactText="Вы подняли "+strlists[Art]+" Нажмите кнопку 'P' для более детальной информации.";
            if((Art>=13)&&(Art<=14))
                ArtefactText="Вы подняли ремонтный комплект. Ваш корпус был полностью отремонтирован.";
            if(Art==15)
                ArtefactText="Вы подняли дополнительную заряд для искажателя пространства.";
            CornerTextManager.Add(ArtefactText,200); */
        }
}
//---------------------------------------------------------------------------
void __fastcall AIRepathTimer() {
    TPoint a = SetNewCource();

    PlayerManager.Player.RightSpeed = (a.x - PlayerManager.Player.xpos) / 38;
    PlayerManager.Player.TopSpeed = (a.y - PlayerManager.Player.ypos) / 38;
}
//---------------------------------------------------------------------------
void __fastcall AITimer() {
    PlayerManager.Player.xpos += PlayerManager.Player.RightSpeed;
    PlayerManager.Player.ypos += PlayerManager.Player.TopSpeed;

    if(PlayerManager.Player.xpos < 0)PlayerManager.Player.xpos = 0;

    if(PlayerManager.Player.xpos > 750)PlayerManager.Player.xpos = 750;

    if(PlayerManager.Player.ypos < 0)PlayerManager.Player.ypos = 0;

    if(PlayerManager.Player.ypos > 509)PlayerManager.Player.ypos = 509;

    if(FireLevel < 20)
        FireLevel++;

    for (int i = 0; i < 20; i++)
        if(FireLevel == 20)
            if(EnemiesManager.Enemies[0][i].ypos > 0)
                if(TestRects(CRect(PlayerManager.Player.xpos + 23, 0, PlayerManager.Player.xpos + 28, PlayerManager.Player.ypos),
                             CRect(EnemiesManager.Enemies[0][i].xpos - 9, 0, EnemiesManager.Enemies[0][i].xpos + 9, PlayerManager.Player.ypos + 50))) {

                    PlayerFire(PlayerManager.Player.xpos, PlayerManager.Player.ypos);
                    FireLevel = 0;
                }
}
//---------------------------------------------------------------------------
void __fastcall InitTriggers() {
    TTrigger Temp;

    int reason;
    scanf("%d\n", &reason);

    while(reason != -1) {
        int value;
        scanf("%d\n", &value);
        Temp.Reason_type = reason;

        if(reason == TR_SCORE)
            Temp.Reason_score = value;

        char s[1000];
        gets(s);
        int len = 0;

        while(s[0] != '$') {
            Temp.Script[len++] = s;
            gets(s);
        }

        Temp.Script_length = len;
        TriggerManager.AddTrigger(Temp);
        scanf("%d\n", &reason);
    }

    Temp.Reason_type = TR_SCORE;

    for(int i = 0; i < 5; i++) {
        Temp.Reason_score = WEAPON_SCORE[i];
        Temp.Script_length = 3;
        Temp.Script[0] = "UnlockWeapon(" + str(i + 1) + ",0);";
        Temp.Script[1] = "UnlockWeapon(" + str(i + 1) + ",1);";
        Temp.Script[2] = "AddCornerText('New weapon avalaible',200);";
        TriggerManager.AddTrigger(Temp);
    }

    Temp.Reason_type = Trigger_reason_time;
    Temp.Reason_time = 10;
    Temp.Script_length = 1;
    Temp.Script[0] = ""; //"CreateMessageBox('Сработал триггер времени!',"+string(LIGHT_BLUE+IC_INFORMATION)+",1,"+string(MESSAGETYPE_INFORMATION)+");";
    TriggerManager.AddTrigger(Temp);
}
//---------------------------------------------------------------------------
void __fastcall ShowScreen() {
    __int64 BeginTime;
    BeginTime = GetTickCount();

    int FPS;

    while(BeginTime + ScreenTime >= GetTickCount()) {
        MainDevice->Clear( 0, NULL, D3DCLEAR_TARGET , D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0 );
        MainDevice->BeginScene();

        MainSprite->Begin(D3DXSPRITE_ALPHABLEND);

        D3DXMATRIXA16 Matr;
        D3DXMatrixScaling(&Matr, 1 / 1.28, 1 / 1.28, 1);
        MainSprite->SetTransform(&Matr);
        MainSprite->Draw(ScreenSaver, NULL, NULL, NULL, FCOLOR);

        MainFont->DrawTextA(MainSprite, str(FPSShower.GetFPS()).c_str(), -1, &CRect(0, 0, 100, 100), 0, TCOLOR);

        MainSprite->End();
        MainDevice->EndScene();

        if(MainDevice->Present( NULL, NULL, NULL, NULL ) == D3DERR_DEVICELOST)
            DeviceWasLost = true;

        FPSShower.UpdateFPS();
    }
}
//---------------------------------------------------------------------------
void __fastcall BackToMainMenu() {

    TimerManager->SetStatus(T9, false);
    TimerManager->SetStatus(T18, false);

    while (screenStack.size() > 1) {
        delete screenStack.top();
        screenStack.pop();
    }

    StartTimers(true);
    PauseNow = false;

    for(int i = 0; i < 6; i++)
        PanelButtons[i].Visible = true;

    GameCheckListBox.Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall MessageButton1Event() {
    ButtonsManager.ChangeVisible(GB_MESSAGE1, false);
    ButtonsManager.ChangeVisible(GB_MESSAGE2, false);

    switch(MType) {
        case MESSAGETYPE_LOSEGAME: {
            KeyDown(BT_ESC, 0);
            //            unsigned short fa=27;
            //            TShiftState shift;
            //            PostMessage(hWnd,WM_KEYDOWN,fa,0);
        }
        break;

        case MESSAGETYPE_WINGAME: {
            int k = PlayerManager.Player.Experience;
            string Score = str(k);

            screenStack.push( new CoolMessageBox("Сохранить рекорд на " + Score + " очков? ", LIGHT_BLUE + IC_QUESTION, 2, MESSAGETYPE_SCOREQUESTION) );

            ScoreQuestion = true;
            Boss[1] = 3;
        }
        break;

        case MESSAGETYPE_SCOREQUESTION: {
            ScoreQuestion = false;
            BackToMainMenu();
        }
        break;

        case MESSAGETYPE_INFORMATION:
            Pause(0);
            break;

        case MESSAGETYPE_QUIT:
            BackToMainMenu();
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall MessageButton2Event() {
    ButtonsManager.ChangeVisible(GB_MESSAGE1, false);
    ButtonsManager.ChangeVisible(GB_MESSAGE2, false);

    switch(MType) {
        case MESSAGETYPE_SCOREQUESTION:
            BackToMainMenu();
            break;

        case MESSAGETYPE_QUIT:
            Pause(0);
            break;
    }
}

void __fastcall RenderingTimer() {
    if(DeviceWasLost)
        return;

    MainDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0 );
    MainDevice->BeginScene();
    
	
	MainSprite->Begin(D3DXSPRITE_ALPHABLEND);


    stack<Screen*> temp, temp2(screenStack);

    do {
        temp.push(temp2.top());
        temp2.pop();
    } while (temp.top()->drawPrevious() && temp2.size());

    while (temp.size()) {
        temp.top()->render();
        temp.pop();
    }

    MainSprite->End();

	//StarsManager->Draw();

	/*
     MainDevice->SetFVF(tri_fvf);

        MainDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
           //Bind our Vertex Buffer
       MainDevice->SetStreamSource(0,                   //StreamNumber
                                     g_list_vb,           //StreamData
                                     0,                   //OffsetInBytes
                                     sizeof(tri_vertex)); //Stride

       //Render from our Vertex Buffer
       MainDevice->DrawPrimitive(D3DPT_TRIANGLELIST, //PrimitiveType
                                   0,                  //StartVertex
                                   g_list_count);      //PrimitiveCount */


    MainDevice->EndScene();

    if(MainDevice->Present( NULL, NULL, NULL, NULL ) == D3DERR_DEVICELOST) {
        DeviceWasLost = true;
    }

    FPSShower.UpdateFPS();
}
//---------------------------------------------------------------------------
void __fastcall EngineTimer() {
    if(DeviceWasLost)
        return;

	screenStack.top()->doPhysics();    

    if(CornerTextManager.TextExists())
        CornerTextManager.Update();
    
}
//---------------------------------------------------------------------------
void __fastcall RestoreMyD3D() {
    MainSprite->OnLostDevice();
    MainFont->OnLostDevice();
    MainDevice->Release();
    InitD3D(hWnd);
    TextureManager.ReloadAll();
    texManager->ReloadAll();

    //--------Just part of code, that must assembly when game restore

    PlayerManager.StopMoving();
    DeviceWasLost = false;
}
//---------------------------------------------------------------------------

void __fastcall LoadD3DXResources() {
    int i;

    for(i = 0; i < 6; i++) {

        string TexName = TempFolder + "TempBmp" + str(i) + ".bmp";
        TextureManager.Create(TexName, true, &PanelButtons[i].Texture);
    }

    TextureManager.CreateEx("DominatorSh", false, 0, 0, TCOLOR, &DominatorShot);
    TextureManager.CreateEx("Image4", false, 0, 0, D3DCOLOR_XRGB(0, 0, 0), &RangersLogo);
    TextureManager.Create("WeaponUno23", false, &RocketShleifImages);
    int l = random(6) + 1;
    string Nm = "Fon" + str(l);
    TextureManager.Create(Nm, false, &Fon);
    TextureManager.Create("GeneralFon", false, &GeneralFon);

    for(int x = 0; x < 3; x++)
        for(i = 0; i < 5; i++) {
            string ResName = "Dominator_" + str(x) + "_" + str(i);
            TextureManager.Create(ResName, false, &DominatorUno[x][i]);
        }

    for(i = 0; i < 5; i++)
        for(int m = 0; m < 2; m++) {
            string ResName = "WeaponUno" + str(i + 1) + str(m + 1);
            TextureManager.CreateEx(ResName, false, 0, 0, TCOLOR, &WeaponImages[i][m]);
        }

    TextureManager.CreateEx("Image3", false, 0, 0, TCOLOR, &fuck);
    TextureManager.CreateEx("weapon1effect", false, 0, 0, TCOLOR, &Weapon1Effect);
    TextureManager.CreateEx("Artefact", false, 0, 0, TCOLOR, &Artefact);
    TextureManager.CreateEx("Boss", false, 0, 0, TCOLOR, &BossI);

    for(i = 0; i < 18; i++) {
        string  TexName = "eg";

        if(i < 10)    TexName += "0" + str(i);

        if(i >= 10)   TexName += str(i);

        TextureManager.CreateEx(TexName, false, 0, 0, TCOLOR, &Explo[i]);
    }

    TextureManager.CreateEx("CoolExploding1", false, 0, 0, TCOLOR, &CoolExploding1);
    TextureManager.CreateEx("CoolExploding2", false, 0, 0, TCOLOR, &CoolExploding2);

    if(!FindFile)
        TextureManager.Create("laserTexture", false, &LaserImage);
    else {
        string FileName;

        if(LaserParam.TexturePath[2] == ':')
            FileName = LaserParam.TexturePath;
        else
            FileName = LaserParam.TexturePath;

        TextureManager.Create(FileName, true, &LaserImage);
    }

    TextureManager.Create("d123", false, &Panel);
    TextureManager.CreateEx("d12345", false, 0, 0, FCOLOR, &WarningSpace);
    TextureManager.CreateEx("SmallShip", false, 0, 0, D3DCOLOR_ARGB(255, 0, 0, 0), &SmallShip);
    TextureManager.Create("CheckListBoxTexture", false, &GameCheckListBox.Texture);
    TextureManager.Create("Undamager", false, &UndamagerTexture);

    //-------------------MessageBox---------------------------
    for (i = 0; i < 4; i++) {
        string FileName = "Angle" + str(i + 1);
        TextureManager.CreateEx(FileName, false, 0, 0, TCOLOR, &Angle[i]);
    }

    for (i = 0; i < 4; i++) {
        string FileName = "Line" + str(i + 1);
        TextureManager.CreateEx(FileName, false, 0, 0, TCOLOR, &Line[i]);
    }

    for (i = 0; i < 4; i++) {
        string FileName = "Icon" + str(i + 1);
        TextureManager.CreateEx(FileName, false, 0, 0, TCOLOR, &Icons[i]);
    }

    for (i = 0; i < 2; i++) {
        string FileName = "Light" + str(i + 1);
        TextureManager.CreateEx(FileName, false, 0, 0, TCOLOR, &Light[i]);
    }

    TextureManager.CreateEx("Centre", false, 0, 0, TCOLOR, &Centre);
    TextureManager.CreateEx("Ok", false, 0, 0, TCOLOR, &MessageButton1.Texture);
    TextureManager.CreateEx("Cancel", false, 0, 0, TCOLOR, &MessageButton2.Texture);
    //-------------------End MessageBox-------------------------

    TextureManager.Create("Screen", false, &ScreenSaver);

    if(TextureManager.CreateEx("Ranger", false, 0, 0, TCOLOR, &PlayerTexture) == -1)
        PostQuitMessage(0);
}
//---------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd ) {
    if( NULL == ( MainD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    D3DPRESENT_PARAMETERS d3dpp;

    D3DDISPLAYMODE Mode;
    MainD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Mode);

    ZeroMemory( &d3dpp, sizeof(d3dpp) );

    d3dpp.MultiSampleType	= D3DMULTISAMPLE_NONE; //No multi-sampling
    d3dpp.MultiSampleQuality	= 0; //No multi-sampling
    d3dpp.Flags				= 0;  //No flags to set
    //	d3dpp.BackBufferFormat  = p_format; //Display format


    d3dpp.BackBufferFormat = Mode.Format;

    d3dpp.Windowed = true;
//    	d3dpp.Windowed = false;
//    	d3dpp.FullScreen_RefreshRateInHz=75;


    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    //	d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_DEFAULT;
    d3dpp.EnableAutoDepthStencil = false;
    //	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    d3dpp.BackBufferWidth = SCREEN_WIDTH;
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.BackBufferCount = 1;

    if( FAILED( MainD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                       D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                       &d3dpp, &MainDevice ) ) ) {
        return E_FAIL;
    }

    //MainDevice->SetRenderState( D3DRS_AMBIENT, 0xffffffff );

    D3DXCreateSprite(MainDevice, &MainSprite);

    D3DXCreateFontA(MainDevice, 12, 0, FW_DONTCARE, 1, FALSE, DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                    NULL, &SlaveFont);

    D3DXCreateLine(MainDevice, &MainLine);
    MainLine->SetAntialias(true);

    DeviceWasLost = false;

    D3DXMatrixIdentity(&IdentityMatrix);
    return S_OK;
}
//---------------------------------------------------------------------------
void __fastcall SetConstants() {
    DrawingRects[0] = CRect(0, 0, 41, 37);
    DrawingRects[1] = CRect(0, 0, 32, 35);
    DrawingRects[2] = CRect(0, 0, 55, 42);
    DrawingRects[3] = CRect(0, 0, 70, 76);
    DrawingRects[4] = CRect(0, 0, 73, 117);

    DrawingRects[5] = CRect(0, 0, 121, 125);
    DrawingRects[6] = CRect(0, 0, 65, 65);

    ScreenTime = 1000;
    CurrentVolume = 50;
}

vector<string> split(string s, string patt) {
    vector<string> result;
    size_t pos = 0, l = patt.length();

    do {
        size_t p = s.find(patt, pos);

        if (p != string::npos) {
            result.push_back( s.substr(pos, p - pos) );
            pos = p + l;
        } else {
            result.push_back( s.substr(pos, string::npos) );
            pos = p;
        }
    } while (pos != string::npos);

    return result;
}

string loadTextResource(HMODULE location, string name) {
    HRSRC hRes = FindResourceA(location, name.c_str(), RT_RCDATA);
    HGLOBAL hMem = LoadResource(TEXTRES_LOCATION, hRes);
    char* pMem = (char*)LockResource(hMem);
    DWORD size = SizeofResource(TEXTRES_LOCATION, hRes);
    char * buff = new char[size + 1];
    memcpy(buff, pMem, size);
    buff[size] = 0;

    string result(buff);
    delete[] buff;
    return result;
}

//---------------------------------------------------------------------------
void __fastcall LoadResources() {
    if(TEXTRES_LOCATION == NULL) {
        exit(0);
        return;
    }

    if(GRAPHRES_LOCATION == NULL) {
        exit(0);
        return;
    }

    for (int i = 0; i < 6; i++)
        for (int y = 0; y < 3; y++) {
            string name = "Image" + str(y);
            HBITMAP bitmap = LoadBitmapA(GRAPHRES_LOCATION, name.c_str());
            //SetBkMode(bitmap, TRANSPARENT);
            const string data[] = {"Новая игра", "Временно не работает ^_^",
                                   "И это тоже барахлит", "Управление", "Разработчики", "Выход"
                                  };
        }

    /*    for(int i = 0; i < 6; i++) {
            for(int y = 0; y < 3; y++) {

                if(y == 0)
                    ImagePictures[i][y]->Canvas->Font->Color = clWhite;
                else
                    ImagePictures[i][y]->Canvas->Font->Color = clYellow;

                int X, Y;
                X = (ImagePictures[i][y]->Width - str.length() * 6) / 2;
                Y = (ImagePictures[i][y]->Height - ImagePictures[i][y]->Canvas->Font->Size) / 2;
                ImagePictures[i][y]->Canvas->TextOutA(X, Y, str.c_str());
            }

            Graphics::TBitmap* Temp = new Graphics::TBitmap;
            Temp->Assign(ImagePictures[i][0]);
            Temp->Width = 256;
            Temp->Height = 128;

            for(int z = 0; z < 3; z++)
                Temp->Canvas->Draw(0, 36 * z, ImagePictures[i][z]);

            string TexName = TempFolder + "TempBmp" + str(i) + ".bmp";
            Temp->PixelFormat = pf24bit;
            Temp->SaveToFile(TexName.c_str());
            Temp->Free();
        }           */
}
//---------------------------------------------------------------------------
void __fastcall ActivateCheat(int Cheat) {
    switch(Cheat) {
        case 1:
            PlayerManager.PlayerParameters[7] += 1000;
            break;

        case 2:
            PlayerManager.Player.Experience += 50000;
            break;

        case 4:
            PlayerManager.Player.Lifes += 5;
            break;

        case 3: {
            for(int i = 0; i < 11; i++)
                RangerArtefacts[i] = 1;

            PlayerManager.RebuildParameters();
        }
        break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TestForCrash() {
    RECT SecondShip = PlayerManager.GetPlayerRect();

    if(!Undamager) {
        for(int v = 0; v < 5; v++)
            for (int i = 0; i < 25; i++)
                if(EnemiesManager.Enemies[v][i].Exists) {
                    RECT FirstShip = EnemiesManager.GetEnemyRect(v + 1, i);

                    if(TestRects(FirstShip, SecondShip)) {
                        int Damage = EnemiesManager.Enemies[v][i].HP;
                        EnemiesManager.Enemies[v][i].HP -= PlayerManager.Player.HP;

                        if(EnemiesManager.Enemies[v][i].HP <= 0) {
                            MusicManager.OpenAndPlayEx(TempFolder + "sound//expl0.wav", 10, 59, false);
                            EnemiesManager.EnemyDead(v + 1, i);
                        }

                        PlayerManager.Player.HP -= Damage * 2;

                        if(PlayerManager.Player.HP <= 0)
                            PlayerManager.PlayerDead();
                    }
                }

        if(Boss[1] == 1) {
            RECT FirstShip = UpdateRect(CRect(20, 20, 190, 160), Boss[2], Boss[3]);

            if(TestRects(FirstShip, SecondShip)) {
                PlayerManager.PlayerDead();
                PlayerManager.Player.xpos = 375;
                PlayerManager.Player.ypos = 650;
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall MoveShleif() {
    for(int i = 0; i < 100; i++)
        if(Shleif[i][1] == 1) {
            Shleif[i][3] += 4;
            Shleif[i][4] += 3;

            if(Shleif[i][4] >= Shleif[i][5])
                Shleif[i][1] = 0;
        }
}
//---------------------------------------------------------------------------
void __fastcall MoveRocketShleif() {
    for(int j = 0; j < 150; j++)
        if(RocketShleif[j][1] == 1) {
            RocketShleif[j][4] -= 1;

            if(RocketShleif[j][4] == 0)
                RocketShleif[j][1] = 0;
        }
}
//---------------------------------------------------------------------------
void __fastcall PlayerShipFire(int WeaponType, int WeaponLevel, bool SecondWeapon) {
    TPoint ppos = PlayerManager.GetPlayerPos();
    double pp6 = PlayerManager.PlayerParameters[6];
    double pp4 = PlayerManager.PlayerParameters[4];
    PlayerShotsManager->Add(ppos, WeaponType * 10 + SecondWeapon + 1, WeaponLevel, pp4, pp6);
}
//---------------------------------------------------------------------------
void __fastcall UpdateGameOverNewScore() {

    PlayerManager.Player.MaxHP = 100 + PlayerManager.Player.Experience / 25;

    PlayerManager.Player.DroidRepair = PlayerManager.Player.Experience / 2000 + 1;

    if(PlayerManager.Player.DroidRepair > 10)
        PlayerManager.Player.DroidRepair = 10;

    EnemiesManager.EnemySetMax(2, min(PlayerManager.Player.Experience / 500, 20));
    EnemiesManager.EnemySetMax(3, min(PlayerManager.Player.Experience / 2000, 10));
    EnemiesManager.EnemySetMax(4, min(PlayerManager.Player.Experience / 3000, 4));
    EnemiesManager.EnemySetMax(5, min(PlayerManager.Player.Experience / 5000, 2));

    if((PlayerManager.Player.Experience >= 50000) && Boss[1] == 0) {
        Boss[1] = 1;
        Boss[2] = 300;
        Boss[3] = -200;
        Boss[4] = 2;
        Boss[5] = 30000;
        Boss[6] = 30000;
        Boss[7] = 25;
        Boss[8] = 50;
    }

    PlayerManager.PlayerParameters[10] = PlayerManager.Player.Experience / 1000;

    if(PlayerManager.PlayerParameters[10] >= 15)
        PlayerManager.PlayerParameters[10] = 15;

    PlayerManager.PlayerParameters[11] = PlayerManager.Player.Experience / 500;

    if(PlayerManager.PlayerParameters[11] >= 40)
        PlayerManager.PlayerParameters[11] = 40;
}
//---------------------------------------------------------------------------
void __fastcall MoveArtefacts() {
    int i;

    for(i = 0; i < 100; i++)
        if(Artefacts[i][1] == 1) {
            Artefacts[i][3] += 2;

            if(Artefacts[i][3] >= 600)
                Artefacts[i][1] = 0;
        }
}
//---------------------------------------------------------------------------

void __fastcall DrawLighting(int X1, int Y1, int X2, int Y2, D3DCOLOR Color) {
    int LHeight = abs(Y2 - Y1);
    int LWidth = abs(X2 - X1);

    D3DXVECTOR2 Vector[7];

    Vector[0].x = X1;
    Vector[0].y = Y1;

    for(int i = 1; i < 6; i++) {
        Vector[i].x = (X1 + (X2 - X1) / 6 * i + random(10 + LWidth / 25) - 5 - LWidth / 50);
        Vector[i].y = (Y1 + (Y2 - Y1) / 6 * i + random(10 + LHeight / 25) - 5 - LHeight / 50);
    }

    Vector[6].x = X2;
    Vector[6].y = Y2;

    MainLine->Begin();
    MainDevice->SetTransform(D3DTS_WORLD, &MainMatrix);
    MainLine->Draw(Vector, 7, Color);
    MainLine->End();
}
//---------------------------------------------------------------------------
void __fastcall FirstWeaponShieldExplode(int X, int Y, int Power) {
    int i, b, x, y;

    for(i = 0; i < 5; i++)
        for(b = 0; b < 25; b++)
            if(EnemiesManager.Enemies[i][b].Exists) {
                x = EnemiesManager.Enemies[i][b].xpos - X;
                y = EnemiesManager.Enemies[i][b].ypos - Y;

                if(y * y + x * x < 100 * 100)
                    EnemiesManager.Enemies[i][b].HP -= Power;

                if(EnemiesManager.Enemies[i][b].HP < 0)
                    EnemiesManager.EnemyDead(i + 1, b);
            }

    i = 0;

    while(RangerWeaponEffects[i][1] != 0)
        i++;

    RangerWeaponEffects[i][1] = 1;
    RangerWeaponEffects[i][2] = X - 100;
    RangerWeaponEffects[i][3] = Y - 100;
    RangerWeaponEffects[i][4] = 0;
    RangerWeaponEffects[i][5] = 40;
}

//---------------------------------------------------------------------------
void __fastcall StartTimers(bool Start) {
    TimerManager->SetStatus(AIT, Start);
    TimerManager->SetStatus(AIPath, Start);
}
//---------------------------------------------------------------------------
void __fastcall DevelopersButton() {
    MainFont->Release();
    D3DXCreateFont(MainDevice, 28, 0, FW_DONTCARE, 1, FALSE, DEFAULT_CHARSET,
                   OUT_DEFAULT_PRECIS, 1, DEFAULT_PITCH | FF_DONTCARE,
                   "Comic Sans MS", &MainFont);

    screenStack.push(new AuthorsScreen());
}
//---------------------------------------------------------------------------
void __fastcall ControlsButton() {
    MainFont->Release();
    D3DXCreateFont(MainDevice, 24, 0, FW_DONTCARE, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 1,
                   DEFAULT_PITCH | FF_DONTCARE, "Comic Sans MS", &MainFont);

    screenStack.push(new ControlsScreen());
}
//---------------------------------------------------------------------------
void __fastcall RecordButton() {
    return;

    screenStack.push(new RecordsScreen());
}
//---------------------------------------------------------------------------
TPoint __fastcall SetNewCource() {
    int ShipPosition[10][100][3];
    int i, x, z, y;
    int NewRangerX[10], NewRangerY[10];

    for(i = 0; i < 10; i++)
        for(x = 0; x < 30; x++) {
            ShipPosition[i][x][2] = EnemiesManager.Enemies[0][x].ypos + 18 * (i + 1);
            ShipPosition[i][x][1] = EnemiesManager.Enemies[0][x].xpos;
        }

    for(x = 0; x < 20; x++)
        for(i = 0; i < 21; i++) {
            for(y = 1; y < 11; y++) {
                int X, Y;

                if((i / 2) * 2 == i) X = i / 2;
                else X = -((i + 1) / 2);

                if(x > 10)Y = 10 - x;
                else Y = x;

                NewRangerX[y - 1] = PlayerManager.Player.xpos + y * X;
                NewRangerY[y - 1] = PlayerManager.Player.ypos + y * Y;
            }

            bool result = true;

            for(y = 0; y < 10; y++) {
                if((NewRangerX[y] < 50) || (NewRangerX[y] > 850))
                    result = false;

                if((NewRangerY[y] < 50) || (NewRangerY[y] > 550))
                    result = false;

                for(z = 0; z < 30; z++)
                    if(TestRects(CRect(NewRangerX[y] + 5, NewRangerY[y] + 5, NewRangerX[y] + 45, NewRangerY[y] + 55), CRect(ShipPosition[y][z][1], ShipPosition[y][z][2], ShipPosition[y][z][1] + 16, ShipPosition[y][z][2] + 32)))
                        result = false;
            }

            if(result == true) {
                TPoint a;
                a.x = NewRangerX[9];
                a.y = NewRangerY[9];

                int PPos = PlayerManager.Player.xpos;

                if(NewRangerX[9] == PPos) {
                    int q;

                    if(PlayerManager.Player.ypos > 400) a.y -= 25;

                    if(PlayerManager.Player.xpos > 400) a.x = PlayerManager.Player.xpos - 50;

                    if(PlayerManager.Player.xpos < 350) a.x = PlayerManager.Player.xpos + 50;

                    if((PlayerManager.Player.xpos >= 350) && (PlayerManager.Player.xpos <= 400))
                        if(random(3) == 0) {
                            a.x = PlayerManager.Player.xpos + (random(3) - 1) * (random(30) + 20);
                            a.y = PlayerManager.Player.ypos + random(30);
                        }
                }

                return a;
            }
        }

    TPoint a;
    a.x = PlayerManager.Player.xpos;
    a.y = PlayerManager.Player.ypos;

    return a;
}
//---------------------------------------------------------------------------
void __fastcall StartBriefing(int Number) {
    screenStack.push(new BriefingScreen(Number));

    MainFont->Release();
    D3DXCreateFont(MainDevice, 16, 0, FW_DONTCARE, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                   DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Trebuchet MS", &MainFont);

    ButtonsManager.ChangeVisible(GB_BRIEFING, true);
}
//---------------------------------------------------------------------------
void __fastcall ShipShleif(int X, int Y) {
    int i;
    int l;

    for(int k = 0; k < 2; k++)
        for(int f = 0; f < 4; f++) {
            i = 0;

            while(Shleif[i][1] != 0)
                i++;

            Shleif[i][1] = 1;
            Shleif[i][2] = X + 19 + f + 8 * k;
            Shleif[i][3] = Y + 52;
            Shleif[i][4] = 0;

            if(k == 0)
                l = 2 - f;
            else l = (1 - f);

            if (l < 0)l = 0 - l;

            Shleif[i][5] = 10 * (3 - l) - random(10);
        }
}
//---------------------------------------------------------------------------
void __fastcall SuperWeapon() {
    if(PlayerManager.PlayerParameters[7] > 0) {
        PlayerManager.PlayerParameters[7] -= 1;

        for(int i = 1; i <= 5; i++)
            for(int w = 0; w < 25; w++)
                if(EnemiesManager.Enemies[i - 1][w].Exists) {
                    EnemiesManager.Enemies[i - 1][w].HP -= 200;

                    if(EnemiesManager.Enemies[i - 1][w].HP <= 0)
                        EnemiesManager.EnemyDead(i, w);
                }
    }
}
//---------------------------------------------------------------------------
void __fastcall NewArtefact(int X, int Y) {
    int i = 0;

    while (Artefacts[i][1] != 0)
        i++;

    Artefacts[i][1] = 1;
    Artefacts[i][2] = X;
    Artefacts[i][3] = Y;
}
//---------------------------------------------------------------------------
void __fastcall Exploding(int type, int X, int Y) {
    int i = 1;

    while (Explodings[i][0] != 0) {
        i++;

        if(i > 200)
            return;
    }

    Explodings[i][0] = 1;
    Explodings[i][1] = 1;
    Explodings[i][2] = 1;
    Explodings[i][3] = 18;
    Explodings[i][4] = X - 10;
    Explodings[i][5] = Y - 10;

    if(type == 2) {
        Explodings[i][4] = X + 5;
        Explodings[i][1] = 2;
        Explodings[i][3] = 44;
    }

    if(type == 3) {
        Explodings[i][1] = 3;
        Explodings[i][3] = 64;
    }
}
//---------------------------------------------------------------------------
void __fastcall Pause(int Type) {
    if(Type == 2) {
        PauseNow = !PauseNow;
        TimerManager->SetStatus(T9, !PauseNow);
        TimerManager->SetStatus(T18, !PauseNow);
    } else {
        PauseNow = Type;
        TimerManager->SetStatus(T9, !Type);
        TimerManager->SetStatus(T18, !Type);
    }
}
void __fastcall KeyDown(int Key, LPARAM lParam) {

    if(Key == BT_F)
        NeedShowFPS = !NeedShowFPS;

    if (Key == BT_ESC) {
        if (screenStack.top()->canGoBack()) {
            if (screenStack.size() > 2) {
                delete screenStack.top();
                screenStack.pop();
                return;
            }

            BackToMainMenu();
            //delete screenStack.top();
            //screenStack.pop();
            return;
        }
    }

    screenStack.top()->keyDown(Key, lParam);
}
//---------------------------------------------------------------------------
void __fastcall KeyUp(int Key, LPARAM lParam) {
    screenStack.top()->keyUp(Key, lParam);
}
//---------------------------------------------------------------------------
void __fastcall End_game_timer() {
    if(Boss[1] == 2) {
        for(int a = 0; a < 5; a++)
            for(int b = 0; b < 25; b++)
                if(EnemiesManager.Enemies[a][b].Exists) {
                    EnemiesManager.Enemies[a][b].Exists = false;
                    EnemiesManager.EnemyDead(a + 1, b);
                }

        SomeShit++;

        if(SomeShit == 1)
            TimerManager->AddTimerEx(END_GAME_TIMER, 1300, &End_game_timer, true);

        if(SomeShit == 2) {
            TimerManager->SetStatus(END_GAME_TIMER, false);
            screenStack.push( new CoolMessageBox("Вы стали величайшим героем и спасли Галактику от доминаторов! ", IC_INFORMATION + LIGHT_BLUE, 1, MESSAGETYPE_WINGAME));
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall DeadTimer() {
    if((PlayerManager.Player.Lifes >= 0) && (PlayerManager.Player.EnabledNewLifes)) {
        PlayerManager.Player.Visible = true;
        RangerDeads = false;
        Pause(0);
        Undamager = true;
        UndamagerCoff = 300;
        TimerManager->SetStatus(DEAD_TIMER, false);
    }

    if((PlayerManager.Player.Lifes < 0) || (!PlayerManager.Player.EnabledNewLifes)) {
        PlayerManager.Player.Lifes = -1;

        if(SomeShit == 1) {
            for(int a = 0; a < 5; a++)
                for(int b = 0; b < 25; b++)
                    if(EnemiesManager.Enemies[a][b].Exists) {
                        EnemiesManager.Enemies[a][b].Exists = false;
                        EnemiesManager.EnemyDead(a + 1, b);
                    }

            TimerManager->AddTimerEx(DEAD_TIMER, 1300, &DeadTimer, true);
        }

        if(SomeShit == 2) {
            TimerManager->SetStatus(DEAD_TIMER, false);
            screenStack.push(new CoolMessageBox("Вы не смогли убить Блазера и спасти галактику от доминаторов. ", LIGHT_RED + IC_ERROR, 1, MESSAGETYPE_LOSEGAME));
        }
    }

    SomeShit++;
}
//---------------------------------------------------------------------------
void __fastcall MouseMove( WPARAM wParam, LPARAM lParam) {
    int X = LOWORD(lParam);
    int Y = HIWORD(lParam);

    for(int i = 0; i < 6; i++)
        if(PanelButtons[i].Visible)
            if(TestForCursorPosition(PanelButtons[i].Position, X, Y)) {
                PanelButtons[i].CurrentState = 2;
            } else
                PanelButtons[i].CurrentState = 1;

    ButtonsManager.MouseMove(X, Y);
}
//---------------------------------------------------------------------------

void __fastcall MouseDown( WPARAM wParam, LPARAM lParam, int Button) {
    int X = LOWORD(lParam);
    int Y = HIWORD(lParam);

    for(int i = 0; i < 6; i++)
        if(PanelButtons[i].Visible)
            if(TestForCursorPosition(PanelButtons[i].Position, X, Y)) {
                int a = PanelButtons[i].ID;

                switch(a) {
                    case 0:
                        StartBriefing(1);
                        break;

                    case 2:
                        RecordButton();
                        break;

                    case 3:
                        ControlsButton();
                        break;

                    case 4:
                        DevelopersButton();
                        break;

                    case 5:
                        exit(0);
                        break;
                }
            }

    if(GameCheckListBox.Visible)
        for(int i = 0; i < GameCheckListBox.ItemCount; i++) {
            RECT MyRect;
            TEXTMETRIC b;
            GameCheckListBox.Font->GetTextMetrics(&b);
            SetRect(&MyRect, GameCheckListBox.Position.left, GameCheckListBox.Position.top + 20 * i,
                    // Тут я посчитал нужным напихать еще комментариев.
                    // При стиле текста BOLD надо делить вот ту формулу на 2.5, тогда все получаестя
                    // Но при нормальном стиле надо ставить 2.6-2.7, иначе будет срабатывать нажатие вне гриниц текста.
                    // Да, и еще. Надо сместить границу на пикселов 10 влево, тогда все получается отлично.

                    GameCheckListBox.Position.left + 20 + b.tmHeight * GameCheckListBox.Items[i].Name.length() / 2.75 + 10,
                    GameCheckListBox.Position.top + 20 + 20 * i);

            if(TestForCursorPosition(MyRect, X, Y)) {
                GameCheckListBox.Items[i].Checked = !GameCheckListBox.Items[i].Checked;
                GameCheckListBox.CheckedItemNumber = i;
            }

            if(GameCheckListBox.Items[i].Checked)
                RangerArtefacts[ stringlist[i] ] = 1;
            else
                RangerArtefacts[ stringlist[i] ] = 2;
        }

    ButtonsManager.MouseDown(X, Y);
}
//---------------------------------------------------------------------------

void __fastcall MouseUp(WPARAM wParam, LPARAM lParam, int Button) {
    int X = LOWORD(lParam);
    int Y = HIWORD(lParam);

    ButtonsManager.MouseUp(X, Y);
}
//---------------------------------------------------------------------------
void __fastcall MouseWheel(WPARAM wParam, LPARAM lParam) {
}

void __fastcall KeyPress(WPARAM wParam, LPARAM lParam) {
    int Key = (TCHAR) wParam;

    switch(Key) {
        case '+': {
            CurrentVolume += 5;

            if(CurrentVolume > 100) {
                CurrentVolume = 100;
                return;
            }

            MusicManager.SetGlobalVolume(CurrentVolume);
            CornerTextManager.Add("Громкость игры увеличена до " + str(CurrentVolume) + "%.", 1);
        }
        break;

        case '-': {
            CurrentVolume -= 5;

            if(CurrentVolume < 0) {
                CurrentVolume = 0;
                return;
            }

            MusicManager.SetGlobalVolume(CurrentVolume);
            CornerTextManager.Add("Громкость игры уменьшена до " + str(CurrentVolume) + "%.", 1);
        }
        break;
    }
}
//---------------------------------------------------------------------------
void __fastcall BriefingButtonEvent() {
    screenStack.push(new GameScreen(1));
}
//---------------------------------------------------------------------------
void __fastcall InitManagers() {
    MusicManager.Init((HDC)hWnd);
    MusicManager.SetGlobalVolume(CurrentVolume);

    FontManager.Init(&MainDevice, &MainSprite);
    FontManager.AddFontEx(MFONT, "Trebuchet MS", 16);
    //    FontManager.CreateFontA("Trebuchet MS",16);
    MainFont = FontManager.GetFont(MFONT);

    TextureManager.Init(&MainDevice, GRAPHRES_LOCATION);

    CornerTextManager.Init(&MainFont);
    NumberShower.Init(&MainFont);

    FPSShower.Init();

    ButtonsManager.Init();
    ButtonsManager.InitDrawingInterface(&MainSprite);

    PlayerManager.Init("Data/Player.ini", &TextureManager);
    PlayerManager.InitDrawingInterface(&MainSprite);

    EnemiesManager.Init();
    EnemiesManager.InitDrawingInterface(MainSprite);

    PlayerShotsManager = new TPlayerShotsManager();
    PlayerShotsManager->Clear();
    PlayerShotsManager->InitDrawingInterface(&MainSprite);

    TimerManager = new TTimerManager();
    TimerManager->Init();
    TimerManager->AddTimerEx(T9, 10000, &Time9, false);
    TimerManager->AddTimerEx(T18, 25, &Time18, false);
    TimerManager->AddTimerEx(AIPath, 500, &AIRepathTimer, false);
    TimerManager->AddTimerEx(AIT, 26, &AITimer, false);
    TimerManager->AddTimerEx(DEAD_TIMER, 1200, &DeadTimer, false);
    TimerManager->AddTimerEx(END_GAME_TIMER, 2000, &End_game_timer, false);
    TimerManager->AddTimerEx(ENGINE_TIMER, 1000 / 75, EngineTimer, true);

    StarsManager = new TStarsManager();
    StarsManager->InitDrawingInterface(&MainLine);

    EnemyShotsManager = new TEnemyShotsManager();
    EnemyShotsManager->Init(&PlayerManager.Player);
    EnemyShotsManager->InitDrawingInterface(&MainSprite);
}
//---------------------------------------------------------------------------

void PlayerFire(int xpos, int ypos) {
    string FolderName = TempFolder + "sound/1.wav";
    MusicManager.OpenAndPlayEx(FolderName, 60, 100, false);


    PlayerShotsManager->Add(TPoint(xpos, ypos), 11, 1, 100, 100);
}
//---------------------------------------------------------------------------
void __fastcall InitControls() {
    for(int i = 0; i < 6; i++) {
        PanelButtons[i].Position = CRect(7 + 537, 7 + 40 * i + 101, 7 + 208 + 537, 7 + 40 * i + 36 + 101);

        for(int z = 0; z < 3; z++)
            PanelButtons[i].States[z] = CRect(0, 36 * z, 208, 36 * (z + 1));

        PanelButtons[i].Visible = true;
        PanelButtons[i].ID = i;
        PanelButtons[i].CurrentState = 1;
    }

    MyButton.CurrentState = 1;
    MyButton.Position = CRect(755, 536, 755 + 31, 536 + 54);
    MyButton.States[0] = CRect(0, 0, 31, 54);
    MyButton.States[1] = CRect(31, 0, 62, 54);
    MyButton.States[2] = CRect(62, 0, 93, 54);

    ButtonsManager.ChangeVisible(GB_BRIEFING, false);

    GameCheckListBox.Visible = false;
    GameCheckListBox.VisibleMainForm = false;
    GameCheckListBox.CheckedItem = CRect(0, 0, 20, 20);
    GameCheckListBox.UncheckedItem = CRect(0, 40, 20, 60);

    D3DXCreateFont(MainDevice, 20, 0, FW_DONTCARE, 1, FALSE, DEFAULT_CHARSET,
                   OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                   "Comic Sans MS", &GameCheckListBox.Font);

    MessageButton1.Visible = false;
    MessageButton1.CurrentState = 1;
    MessageButton1.States[0] = CRect(0, 0, 48, 21);
    MessageButton1.States[1] = CRect(0, 21, 48, 21 * 2);
    MessageButton1.States[2] = CRect(0, 21 * 2, 48, 21 * 3);

    MessageButton2.Visible = false;
    MessageButton2.CurrentState = 1;
    MessageButton2.States[0] = CRect(0, 0, 48, 21);
    MessageButton2.States[1] = CRect(0, 21, 48, 21 * 2);
    MessageButton2.States[2] = CRect(0, 21 * 2, 48, 21 * 3);

    ButtonsManager.Add(MyButton, GB_BRIEFING, &BriefingButtonEvent);
    ButtonsManager.Add(MessageButton1, GB_MESSAGE1, &MessageButton1Event);
    ButtonsManager.Add(MessageButton2, GB_MESSAGE2, &MessageButton2Event);
}
//---------------------------------------------------------------------------
void __fastcall Time9() {
    int NewMax = EnemiesManager.EnemiesMax[0] + 1;
    NewMax += 1;

    if(NewMax > 25)
        TimerManager->SetStatus(T9, false);
    else
        EnemiesManager.EnemySetMax(1, NewMax);
}
//---------------------------------------------------------------------------
void __fastcall Time18() {
    if(Boss[1] == 1) {
        Boss[5]++;

        if(Boss[5] > Boss[6])
            Boss[5] = Boss[6];

        if(Boss[3] < 25)
            Boss[3] += 2;

        if(Boss[3] > -50)
            EnemyShotsManager->Create(6, Boss[2] + 75, Boss[3] + 75);

        for(int i = 0; i < 100; i++)
            if(PlayerShotsManager->Test(i,
                                        CRect(Boss[2] + 30, Boss[3] + 30, Boss[2] + 170, Boss[3] + 150),
                                        &Boss[5]) ) {

                Exploding(1, Boss[2] + 30 + random(140), Boss[3] + 30 + random(120));

                if(Boss[5] <= 0) {
                    Boss[1] = 2;
                    Exploding(3, Boss[2] + 25, Boss[3] + 25);
                    SomeShit = 0;
                    TimerManager->SetStatus(END_GAME_TIMER, true);
                    TimerManager->SetStatus(T18, false);
                }
            }
    }
}
//---------------------------------------------------------------------------


LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[ ] = "WindowsApp";

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{

    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */


    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursorFromFile( (TempFolder + "main.ani").c_str());

    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hWnd = CreateWindowEx (
               0, szClassName, "Mini Space Rangers",
               WS_OVERLAPPED  | \
               WS_MINIMIZEBOX | \
               WS_POPUP,
               0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, HWND_DESKTOP, NULL, hThisInstance, NULL);


    ShowWindow (hWnd, nFunsterStil);
    UpdateWindow(hWnd);


    SetConstants();

    if( InitD3D(hWnd) == E_FAIL)
        return 0;

    init_lists();

    if( OnCreateDevice(hWnd) == E_FAIL)
        return 0;

    InitManagers();

    LoadResources();
    LoadD3DXResources();

    InitControls();

    ShowScreen();

    screenStack.push(new MainMenuScreen());

    while( messages.message != WM_QUIT ) {
        if( PeekMessage( &messages, NULL, 0, 0, PM_REMOVE ) ) {
            TranslateMessage( &messages );
            DispatchMessage( &messages );
        } else {
            TimerManager->Update(1000 / 75.0);
            ReadBufferedData(hWnd);
            RenderingTimer();
        }
    }

    MusicManager.SetGlobalVolume(CurrentVolume);
    TextureManager.FreeAll();
    FreeDirectInput();

    return messages.wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {                /* handle the messages */
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_MOUSEMOVE:
            MouseMove(wParam, lParam);
            break;

        case WM_LBUTTONDOWN:
            MouseDown(wParam, lParam, mbLeft);
            break;

        case WM_RBUTTONDOWN:
            MouseDown(wParam, lParam, mbRight);
            break;

        case WM_MBUTTONDOWN:
            MouseDown(wParam, lParam, mbMiddle);
            break;

        case WM_LBUTTONUP:
            MouseUp(wParam, lParam, mbLeft);
            break;

        case WM_RBUTTONUP:
            MouseUp(wParam, lParam, mbRight);
            break;

        case WM_MBUTTONUP:
            MouseUp(wParam, lParam, mbMiddle);
            break;

        case WM_MOUSEWHEEL:
            MouseWheel(wParam, lParam);
            break;

        case WM_CHAR:
            KeyPress(wParam, lParam);
            break;

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

HRESULT OnCreateDevice( HWND hDlg ) {
    HRESULT hr;
    BOOL    bExclusive;
    BOOL    bForeground;
    BOOL    bImmediate;
    BOOL    bDisableWindowsKey;
    DWORD   dwCoopFlags;

    FreeDirectInput();

    bExclusive         = false;
    bForeground        = true;
    bImmediate         = false;
    bDisableWindowsKey = false;

    if( bExclusive )
        dwCoopFlags = DISCL_EXCLUSIVE;
    else
        dwCoopFlags = DISCL_NONEXCLUSIVE;

    if( bForeground )
        dwCoopFlags |= DISCL_FOREGROUND;
    else
        dwCoopFlags |= DISCL_BACKGROUND;

    if( bDisableWindowsKey && !bExclusive && bForeground )
        dwCoopFlags |= DISCL_NOWINKEY;

    if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION,
                                         IID_IDirectInput8, (VOID**)&g_pDI, NULL ) ) )
        return hr;

    if( FAILED( hr = g_pDI->CreateDevice( GUID_SysKeyboard, &g_pKeyboard, NULL ) ) )
        return hr;

    if( FAILED( hr = g_pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
        return hr;

    hr = g_pKeyboard->SetCooperativeLevel( hDlg, dwCoopFlags );

    if( hr == DIERR_UNSUPPORTED && !bForeground && bExclusive ) {
        FreeDirectInput();
        return S_OK;
    }

    if( FAILED(hr) )
        return hr;

    if( !bImmediate ) {
        DIPROPDWORD dipdw;

        dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj        = 0;
        dipdw.diph.dwHow        = DIPH_DEVICE;
        dipdw.dwData            = SAMPLE_BUFFER_SIZE; // Arbitary buffer size

        if( FAILED( hr = g_pKeyboard->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ) )
            return hr;
    }

    g_pKeyboard->Acquire();

    return S_OK;
}


VOID FreeDirectInput() {
    if( g_pKeyboard )
        g_pKeyboard->Unacquire();

    SAFE_RELEASE( g_pKeyboard );
    SAFE_RELEASE( g_pDI );
}


HRESULT ReadBufferedData( HWND hDlg ) {
    string              strNewText = TEXT("");
    string              strLetter;
    DIDEVICEOBJECTDATA didod[ SAMPLE_BUFFER_SIZE ];  // Receives buffered data
    DWORD              dwElements;
    DWORD              i;
    HRESULT            hr;

    if( NULL == g_pKeyboard )
        return S_OK;

    dwElements = SAMPLE_BUFFER_SIZE;
    hr = g_pKeyboard->GetDeviceData( sizeof(DIDEVICEOBJECTDATA),
                                     didod, &dwElements, 0 );

    if( hr != DI_OK ) {
        hr = g_pKeyboard->Acquire();

        while( hr == DIERR_INPUTLOST )
            hr = g_pKeyboard->Acquire();

        return S_OK;
    }

    for( i = 0; i < dwElements; i++ ) {
        // this will display then scan code of the key
        // plus a 'D' - meaning the key was pressed
        //   or a 'U' - meaning the key was released

        if( didod[ i ].dwData & 0x80)
            KeyDown(    didod[ i ].dwOfs    ,   0   );
        else
            KeyUp(  didod[ i ].dwOfs    ,   0   );
    }


    return S_OK;
}
//---------------------------------------------------------------------------

