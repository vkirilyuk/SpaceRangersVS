//---------------------------------------------------------------------------

#include <stdio.h>
#pragma hdrstop

#include "main.h"

#include "TextureManager.h"

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

EnemyBuilder * enemyBuilder;

TFontManager        FontManager;
TCornerTextManager  CornerTextManager;
TNumberShower       NumberShower;
TButtonsManager     ButtonsManager;
TPlayerManager      PlayerManager;
TMusicManager       MusicManager;
TTimerManager      *TimerManager;
TFPSShower          FPSShower;
TStatistic          PlayerStatistic;

TextureManager * textureManager;

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
bool Fire, FireAlt, Undamager, CanShot, AlreadyPressed;
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
LPDIRECT3DTEXTURE9 WeaponImages[5][3];
LPDIRECT3DTEXTURE9 Weapon1Effect;
LPDIRECT3DTEXTURE9 RocketShleifImages;
LPDIRECT3DTEXTURE9 GeneralFon;
LPDIRECT3DTEXTURE9 LaserImage;

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
/*
class TexturedObject {
private:
	RECT rect;
	LPDIRECT3DTEXTURE9 texture;
	D3DXVECTOR3 center;
	D3DXVECTOR3 position;
	D3DCOLOR color;
	bool hasRect, hasTexture, hasCenter, hasPosition, hasColor;
public:
	TexturedObject() {
		color = FCOLOR;
		hasRect = hasTexture = hasCenter = hasPosition = hasColor = NULL;
	}
	virtual LPDIRECT3DTEXTURE9 getTexture() { if (hasTexture) return texture; return NULL; }
	virtual RECT * getRect() { if (hasRect) return &rect; return NULL; }
	virtual D3DXVECTOR3 * getCenter() { if (hasCenter) return &center; return NULL; }
	virtual D3DXVECTOR3 * getPosition() { if (hasPosition) return &position; return NULL; }
	virtual D3DCOLOR getColor() { return color; }
	virtual void setTexture(LPDIRECT3DTEXTURE9 texture) {this->texture = texture; hasTexture = true; }
	virtual void setRect(RECT rect) { this->rect = rect; hasRect = true; }
	virtual void setCenter(D3DXVECTOR3 center) { this->center = center; hasCenter = true; }
	virtual void setPosition(D3DXVECTOR3 position) { this->position = position; hasPosition = true; }
	virtual void setColor(D3DCOLOR color) { this->color = color; hasColor = true; }
	void draw() {
		MainSprite->Draw(getTexture(), getRect(), getCenter(), getPosition(), getColor());
	}
}; */

class StaticPicture: public TexturedObject {
public:
	StaticPicture(LPDIRECT3DTEXTURE9 texture, RECT rect, D3DXVECTOR3 position, D3DCOLOR color = FCOLOR) {
		setTexture(texture);
		setRect(rect);
		setPosition(position);
		setColor(color);
	}

	StaticPicture(LPDIRECT3DTEXTURE9 texture, D3DXVECTOR3 position, D3DCOLOR color = FCOLOR) {
		setTexture(texture);
		setPosition(position);
		setColor(color);
	}

	StaticPicture(LPDIRECT3DTEXTURE9 texture, RECT rect, D3DCOLOR color = FCOLOR) {
		setTexture(texture);
		setRect(rect);
		setColor(color);
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
		return CRect(x - width / 2, y - height / 2, x + width / 2, y + height / 2);
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

class TStarsManager
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

void defineTextures() {
	for(int i = 0; i < 6; i++) {
		string TexName = TempFolder + "TempBmp" + str(i) + ".bmp";
		textureManager->addDeclaration(TextureManager::Declaration(TexName, true));
	}

	textureManager->loadConfiguration("Data/textures.json");
}

class D3DRenderer {
public:
	void drawTexture(string name, const RECT *rect, const D3DXVECTOR3 * center = NULL, const D3DXVECTOR3 * position = NULL, D3DCOLOR color = FCOLOR) {
		LPDIRECT3DTEXTURE9 texture = textureManager->get(name);
		MainSprite->Draw(texture, rect, center, position, color);
	}
/*	void drawTexture(string name, int x, int y, D3DCOLOR color = FCOLOR) {
		LPDIRECT3DTEXTURE9 texture = textureManager->get(name);

	}
	void drawTexture(string name, int x, int y, RECT & part, D3DCOLOR color = FCOLOR) {

	} */
} * renderer = new D3DRenderer();

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
			ArtPanelTex = textureManager->get("ArtPanel");

			for (int i = 0; i < 12; i++)
				ArtefactImg[i] = textureManager->get("l" + str(i));

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
			background = textureManager->get("Controls");
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
			
			background = textureManager->get("Authors");
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

			background = textureManager->get("Controls");
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

			background = textureManager->get("BriefingFon");
			Briefingbmp1 = textureManager->get("BriefingPanel");
			MyButton.Texture = textureManager->get("BriefingButtonTexture");
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

#include <iostream>

class MainMenuScreen: public Screen {
    private:
        void SetUpMenu() {

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

            Boss[1] = 0;

            PlayerShotsManager->Clear();

            StarsManager->Set();

            Undamager = false;
            AlreadyLoad = true;
            StartTimers(true);
        }
    public:
		vector<EnemyShip> ships;
		EnemyShip ship;
        MainMenuScreen() {
            SetUpMenu();
			for (int i = 0; i < 50; i++) {
				string klasses[] = {"shtip", "menok", "smersh", "urgant", "ekventor"};
				EnemyShip ship = enemyBuilder->get( klasses[ rand() % 5 ] );
				ship.y = - (rand() % 1000);
				ship.x = rand() % 800;
				ships.push_back(ship);
			}
        }
        void render() {
            D3DXMatrixScaling(&MainMatrix, 1, 1, 1);
            MainDevice->SetTransform(D3DTS_WORLD, &MainMatrix);

            int i;
            D3DXMATRIXA16 Matr;
            D3DXMatrixScaling(&Matr, FON_SCALING, FON_SCALING, 1);
            MainSprite->SetTransform(&Matr);


            float Z = FON_SCALING;

			vector<TexturedObject> data;

            if(Fon_top <= 3400)
				data.push_back( StaticPicture(textureManager->get("Fon1"), CRect(0, (3400 - Fon_top) / Z, 800 / Z, (4000 - Fon_top) / Z)));
			else {
				data.push_back( StaticPicture(textureManager->get("Fon1"), CRect(0, 0, 800 / Z, (4000 - Fon_top) / Z), CVector(0, (Fon_top - 3400) / Z, 0)));
				data.push_back( StaticPicture(textureManager->get("Fon1"), CRect(0, (7400 - Fon_top) / Z, 800 / Z, 4000 / Z)));
			}

			for (vector<TexturedObject>::iterator i = data.begin(); i != data.end(); i++) {
				MainSprite->Draw(i->getTexture(), i->getRect(), i->getCenter(), i->getPosition(), i->getColor());
			}
				//i->draw();   
			data.clear();

            MainSprite->SetTransform(&IdentityMatrix);

            PlayerShotsManager->Draw();

            MainSprite->End();
            StarsManager->Draw();
            MainSprite->Begin(D3DXSPRITE_ALPHABLEND);
            MainDevice->SetTransform(D3DTS_WORLD, &MainMatrix);


            for(i = 0; i < 100; i++)
                if(Shleif[i][1] == 1) {
                    RECT Rect1 = UpdateRect(DrawingRects[12], Shleif[i][2], Shleif[i][3]);
                    RECT Rect2 = UpdateRect(DrawingRects[12], (5 - Shleif[i][4] / 5), 0);

                    MainSprite->Draw(RocketShleifImages, &Rect2, NULL,
                                     &CVector(Rect1.left, Rect1.top, 0), FCOLOR);
                }

            PlayerManager.Draw();

			for (vector<EnemyShip>::iterator ship = ships.begin(); ship != ships.end(); ship++) {
				ship->nextAnimation();
				ship->move(1);
				if (ship->y > 900) {
					ship->x = rand() % 800;
					ship->y = -50.;
				}

				double scaling = ship->scaling;

				D3DXMATRIXA16 m;

				D3DXMatrixScaling(&m, scaling, scaling, 1);

				MainSprite->SetTransform(&m);
				MainSprite->Draw(ship->getTexture(), ship->getRect(), ship->getCenter(), ship->getPosition(), ship->getColor());
				MainSprite->SetTransform(&IdentityMatrix);
			}

            for(i = 0; i < 200; i++)
                if((Explodings[i][0] == 1)) {
                    MainSprite->Draw(Explo[(Explodings[i][2])], NULL, NULL, &CVector(Explodings[i][4], Explodings[i][5], 0), FCOLOR);
                    Explodings[i][2] += 1;

                    if(Explodings[i][2] >= Explodings[i][3])
                        Explodings[i][0] = 0;
                }						

            if(PanelButtons[0].Visible) {
				data.push_back( StaticPicture(fuck, CVector(800 - 224 - 40, 100, 0)));

                for(i = 0; i < 6; i++)
					data.push_back( StaticPicture(PanelButtons[i].Texture, PanelButtons[i].States[PanelButtons[i].CurrentState - 1],
						CVector(PanelButtons[i].Position.left, PanelButtons[i].Position.top, 0)));
            }


			data.push_back( StaticPicture(GeneralFon, CRect(0, 0, 43, 600), CVector(0, 0, 0), D3DCOLOR_ARGB(150, 255, 255, 255)));
			data.push_back( StaticPicture(GeneralFon, CRect(43, 0, 715 + 43, 43), CVector(43, 0, 0), D3DCOLOR_ARGB(150, 255, 255, 255)));
			data.push_back( StaticPicture(GeneralFon, CRect(758, 0, 42 + 758, 600), CVector(758, 0, 0), D3DCOLOR_ARGB(150, 255, 255, 255)));
			data.push_back( StaticPicture(GeneralFon, CRect(43, 558, 715 + 43, 42 + 558), CVector(43, 558, 0), D3DCOLOR_ARGB(150, 255, 255, 255)));

			data.push_back( StaticPicture(RangersLogo, CVector(505, 10, 0)));

			for (vector<TexturedObject>::iterator i = data.begin(); i != data.end(); i++) {
				MainSprite->Draw(i->getTexture(), i->getRect(), i->getCenter(), i->getPosition(), i->getColor());
				//i->draw();
			}

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
            MoveArtefacts();
            StarsManager->Move();
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

			if((PlayerManager.Player.Lifes >= 0) && (!RangerDeads))
				Fon_top += 1;
			if(Fon_top >= 4000) Fon_top = 0;
        }
};

class GameScreen: public Screen {
    private:
        void StartLevel(int level) {
            ButtonsManager.ChangeVisible(GB_BRIEFING, false);

            FireInterval = 1;
            CurrentFonNumber = random(6) + 1;
			
						
			Fon = textureManager->get("Fon" + str(CurrentFonNumber));

            PlayerManager.Clear();


            RangerDeads = false;
            AlreadyPressed = false;
            Fire = false;
            FireAlt = false;
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

            TimerManager->SetStatus(DEAD_TIMER, false);

            int i;

            for(i = 0; i < 12; i++)
                RangerArtefacts[i] = 0;            

            Boss[1] = 0;

            for(i = 0; i < 100; i++)
                Artefacts[i][1] = 0;

            PlayerShotsManager->Clear();

            for(i = 0; i < 100; i++)
                Shleif[i][1] = 0;

            StarsManager->Set();

            for(i = 0; i < 200; i++)
                Explodings[i][0] = 0;

            PlayerStatistic.Clear();
            PlayerStatistic.ShowStatistic = false;

            char s[250] = "2";

            while(s[0] != '#')
                gets(s);
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
                MainSprite->Draw(LaserImage, NULL, NULL, &CVector(PlayerManager.Player.xpos - 100, PlayerManager.Player.ypos - 550, 0), FCOLOR);
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

            if(!RangerDeads)
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
            MoveArtefacts();
            StarsManager->Move();
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

			if((PlayerManager.Player.Lifes >= 0) && (!RangerDeads))
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
}
//---------------------------------------------------------------------------
void __fastcall ShowScreen() {
    __int64 BeginTime;
    BeginTime = GetTickCount();

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
    while (screenStack.size() > 1) {
        delete screenStack.top();
        screenStack.pop();
    }

    StartTimers(true);

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

	textureManager->reloadAll();

    //--------Just part of code, that must assembly when game restore

    PlayerManager.StopMoving();
    DeviceWasLost = false;
}
//---------------------------------------------------------------------------

void __fastcall LoadD3DXResources() {
	int i;

    for(i = 0; i < 6; i++) {

        string TexName = TempFolder + "TempBmp" + str(i) + ".bmp";
		PanelButtons[i].Texture = textureManager->get(TexName);
    }

	DominatorShot = textureManager->get("DominatorSh");
	RangersLogo = textureManager->get("RangersLogo");
	RocketShleifImages = textureManager->get("WeaponUno23");

	string Nm = "Fon" + str(random(6) + 1);
	Fon = textureManager->get(Nm);

	GeneralFon = textureManager->get("GeneralFon");
	
    for(i = 0; i < 5; i++)
        for(int m = 0; m < 2; m++) {
            string ResName = "WeaponUno" + str(i + 1) + str(m + 1);
			WeaponImages[i][m] = textureManager->get(ResName);
        }	
	
	for(i = 0; i < 18; i++) {
        string  TexName = "eg";

        if(i < 10)    TexName += "0" + str(i);

        if(i >= 10)   TexName += str(i);

		Explo[i] = textureManager->get(TexName);
    }

	fuck = textureManager->get("Image3");
	Weapon1Effect = textureManager->get("weapon1effect");
	Artefact = textureManager->get("Artefact");
	BossI = textureManager->get("Boss");
	CoolExploding1 = textureManager->get("CoolExploding1");
	CoolExploding2 = textureManager->get("CoolExploding2");
	LaserImage = textureManager->get("laserTexture");
	Panel = textureManager->get("d123");
	WarningSpace = textureManager->get("d12345");
	SmallShip = textureManager->get("SmallShip");
	GameCheckListBox.Texture = textureManager->get("CheckListBoxTexture");
	UndamagerTexture = textureManager->get("Undamager");

    //-------------------MessageBox---------------------------

	for (i = 0; i < 4; i++) {
        string FileName = "Angle" + str(i + 1);
		Angle[i] = textureManager->get(FileName);
    }

    for (i = 0; i < 4; i++) {
        string FileName = "Line" + str(i + 1);
		Line[i] = textureManager->get(FileName);
    }

    for (i = 0; i < 4; i++) {
        string FileName = "Icon" + str(i + 1);
		Icons[i] = textureManager->get(FileName);
    }

    for (i = 0; i < 2; i++) {
        string FileName = "Light" + str(i + 1);
		Light[i] = textureManager->get(FileName);
    }


	Centre = textureManager->get("Centre");
	MessageButton1.Texture = textureManager->get("Ok");
	MessageButton2.Texture = textureManager->get("Cancel");
    //-------------------End MessageBox
	ScreenSaver = textureManager->get("Screen");
	PlayerTexture = textureManager->get("Ranger");
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

        if(Boss[1] == 1) {
            RECT FirstShip = UpdateRect(CRect(20, 20, 190, 160), Boss[2], Boss[3]);

            if(TestRects(FirstShip, SecondShip)) {
                PlayerManager.PlayerDead();
                PlayerManager.Player.xpos = 375;
                PlayerManager.Player.ypos = 650;
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
    int i = 0;

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
}
//---------------------------------------------------------------------------
void __fastcall DeadTimer() {
    if((PlayerManager.Player.Lifes >= 0) && (PlayerManager.Player.EnabledNewLifes)) {
        PlayerManager.Player.Visible = true;
        RangerDeads = false;
        Undamager = true;
        UndamagerCoff = 300;
        TimerManager->SetStatus(DEAD_TIMER, false);
    }

    if((PlayerManager.Player.Lifes < 0) || (!PlayerManager.Player.EnabledNewLifes)) {
        PlayerManager.Player.Lifes = -1;
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
	enemyBuilder = new EnemyBuilder(textureManager);
	enemyBuilder->loadConfiguration("Data/enemies.json");

    MusicManager.Init((HDC)hWnd);
    MusicManager.SetGlobalVolume(CurrentVolume);

    FontManager.Init(&MainDevice, &MainSprite);
    FontManager.AddFontEx(MFONT, "Trebuchet MS", 16);
    //    FontManager.CreateFontA("Trebuchet MS",16);
    MainFont = FontManager.GetFont(MFONT);


    //TextureManager.Init(&MainDevice, GRAPHRES_LOCATION);

    CornerTextManager.Init(&MainFont);
    NumberShower.Init(&MainFont);

    FPSShower.Init();

    ButtonsManager.Init();
    ButtonsManager.InitDrawingInterface(&MainSprite);

    PlayerManager.Init("Data/Player.ini", textureManager);
    PlayerManager.InitDrawingInterface(&MainSprite);

    PlayerShotsManager = new TPlayerShotsManager();
    PlayerShotsManager->Clear();
    PlayerShotsManager->InitDrawingInterface(&MainSprite);

    TimerManager = new TTimerManager();
    TimerManager->Init();
    TimerManager->AddTimerEx(AIPath, 500, &AIRepathTimer, false);
    TimerManager->AddTimerEx(AIT, 26, &AITimer, false);
    TimerManager->AddTimerEx(DEAD_TIMER, 1200, &DeadTimer, false);
    TimerManager->AddTimerEx(END_GAME_TIMER, 2000, &End_game_timer, false);
    TimerManager->AddTimerEx(ENGINE_TIMER, 1000 / 75, EngineTimer, true);

    StarsManager = new TStarsManager();
    StarsManager->InitDrawingInterface(&MainLine);
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


LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

#define _DEBUG
#include "guicon.h"

/*  Make the class name into a global variable  */
char szClassName[ ] = "WindowsApp";
int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
	RedirectIOToConsole();

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

	textureManager = new TextureManager(MainDevice, GRAPHRES_LOCATION);
	defineTextures();

    init_lists();

    if( OnCreateDevice(hWnd) == E_FAIL)
        return 0;

    InitManagers();		

    LoadResources();
    LoadD3DXResources();

    InitControls();

    ShowScreen();

    screenStack.push(new MainMenuScreen());

    do {
        if( PeekMessage( &messages, NULL, 0, 0, PM_REMOVE ) ) {
            TranslateMessage( &messages );
            DispatchMessage( &messages );
        } else {
            TimerManager->Update(1000 / 75.0);
            ReadBufferedData(hWnd);
            RenderingTimer();
        }
    } while( messages.message != WM_QUIT );

    MusicManager.SetGlobalVolume(CurrentVolume);
    textureManager->freeAll();
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

