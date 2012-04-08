//---------------------------------------------------------------------------


#pragma hdrstop
//----------------------------------------


#include "TextureManager.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


void nTextureManager::Init(LPDIRECT3DDEVICE9* Device, HMODULE TexSource)
{
    OurDevice=Device;
	Source=TexSource;
}
//---------------------------------------------------------------------------
int nTextureManager::Create(string tName, string tPath, bool inFile)
{
	TextureParam texParam;
	texParam.Name = tName;
	texParam.Path = tPath;
	texParam.InFile = inFile;
	texParam.Ex = false;

	int Res = Create(texParam);
	if(Res==D3D_OK)
	{
		Map[tName] = texParam;
		return 0;
	}
	return -1;
}

int nTextureManager::Create(TextureParam & texParam)
{
	HRESULT Res;
	if(texParam.Ex)
	{
		if(texParam.InFile)
			Res=D3DXCreateTextureFromFileExA(*OurDevice,texParam.Path.c_str(),texParam.Width,texParam.Height,0,0,D3DFMT_FROM_FILE,D3DPOOL_MANAGED,
									D3DX_DEFAULT,D3DX_DEFAULT,texParam.Color,NULL,NULL,&(texParam.Texture));
		else
			Res=D3DXCreateTextureFromResourceEx(*OurDevice,Source,texParam.Path.c_str(),texParam.Width,texParam.Height,0,0,D3DFMT_FROM_FILE,D3DPOOL_MANAGED,
									D3DX_DEFAULT,D3DX_DEFAULT,texParam.Color,NULL,NULL,&(texParam.Texture));
	}
	else
	{
		if(texParam.InFile)
			Res=D3DXCreateTextureFromFile(*OurDevice,texParam.Path.c_str(),&(texParam.Texture));
		else
			Res=D3DXCreateTextureFromResourceA(*OurDevice,Source,texParam.Path.c_str(),&(texParam.Texture));

    }

	return Res;
}

int nTextureManager::Create(string tName, string tPath, bool inFile, int width, int height, D3DCOLOR color)
{
	TextureParam texParam;
	texParam.Name = tName;
	texParam.Path = tPath;
	texParam.InFile = inFile;
	texParam.Width = width;
	texParam.Height = height;
	texParam.Color = color;
	texParam.Ex = true;

	int Res = Create(texParam);
	if(Res==D3D_OK)
	{
		Map[tName] = texParam;
		return 0;
	}

	return -1;
}

//---------------------------------------------------------------------------
void nTextureManager::ReloadAll()
{
	map<string,TextureParam>::iterator itr;
	for(itr = Map.begin(); itr != Map.end(); ++itr)
	{
		itr->second.Texture->Release();
		Create(itr->second);
	}
}
//---------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 * nTextureManager::Get(string tName)
{
	if(Map.count(tName)==0)
		return NULL;
	return &(Map[tName].Texture);

}

void nTextureManager::Free(string tName)
{
	if(Map.count(tName)==0)
		return;
	Map[tName].Texture->Release();
}

void nTextureManager::FreeAll()
{
	map<string,TextureParam>::iterator itr;
	for(itr = Map.begin(); itr != Map.end(); ++itr)
		itr->second.Texture->Release();
}


TextureManager::TextureManager(LPDIRECT3DDEVICE9 device, HMODULE source): device(device), source(source) {
}

LPDIRECT3DTEXTURE9 TextureManager::load(string name, string path, bool inFile = false) {
	LPDIRECT3DTEXTURE9 texture = get(name);
	if (texture != NULL)
		return texture;
	HRESULT result;
	if(inFile)
		result = D3DXCreateTextureFromFile(device, path.c_str(), &texture);
	else
		result = D3DXCreateTextureFromResourceA(device, source, path.c_str(), &texture);
	if (result != D3D_OK)
		return NULL;
	set(name, texture);
	return texture;
}

LPDIRECT3DTEXTURE9 TextureManager::load(string name, string path, int width, int height, D3DCOLOR color, bool inFile = false) {
	LPDIRECT3DTEXTURE9 texture = get(name);
	if (texture != NULL)
		return texture;
	HRESULT result;
	if (inFile)
		result = D3DXCreateTextureFromFileExA(device, path.c_str(), width, height, 0, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED,
									D3DX_DEFAULT, D3DX_DEFAULT, color, NULL, NULL, &texture);
	else
		result = D3DXCreateTextureFromResourceEx(device, source, path.c_str(), width, height, 0, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED,
									D3DX_DEFAULT, D3DX_DEFAULT, color, NULL, NULL, &texture);
	if (result != D3D_OK)
		return NULL;
	set(name, texture);
	return texture;
}

LPDIRECT3DTEXTURE9 TextureManager::get(string name) {
	map<string, LPDIRECT3DTEXTURE9>::iterator i = loaded.find(name);
	if (i == loaded.end())
		return NULL;
	return i->second;
}

void TextureManager::set(string name, LPDIRECT3DTEXTURE9 texture) {
	loaded[name] = texture;		
}
