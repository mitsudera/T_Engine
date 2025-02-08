#pragma once
#include "Coreminimal.h"
#include "CBufferManager.h"


#define MAX_DIREC_LIGHT (4)
#define MAX_POINT_LIGHT (8)


class DirectionalLightComponent;
class PointLightComponent;
class GameEngine;
class World;


struct DIREC_LIGHT_PARAM
{
	XMFLOAT4	m_Direction;	    // ライトの方向
	XMFLOAT4	m_Diffuse;	        // 拡散光の色
	XMFLOAT4	m_Ambient;		    // 環境光の色
	int			m_Enable;
};
// ライト用定数バッファ構造体
struct DIREC_LIGHT_CBUFFER
{
	XMFLOAT4	m_Direction[MAX_DIREC_LIGHT];	    // ライトの方向
	XMFLOAT4	m_Diffuse[MAX_DIREC_LIGHT];	        // 拡散光の色
	XMFLOAT4	m_Ambient[MAX_DIREC_LIGHT];		    // 環境光の色
	XMINT4		m_Enable[MAX_DIREC_LIGHT];
	XMINT4		allEnable;

};


struct POINT_LIGHT_PARAM
{
	XMFLOAT4	m_Position;	    // ライトの方向
	XMFLOAT4	m_Diffuse;	        // 拡散光の色
	XMFLOAT4	m_Ambient;		    // 環境光の色
	XMFLOAT4	m_Attenuation;	    // 減衰率    
	XMFLOAT4    m_intensity;       // ライトの強度
	int			m_Enable;
};

// ライト用定数バッファ構造体
struct POINT_LIGHT_CBUFFER
{
	XMFLOAT4	m_Position[MAX_POINT_LIGHT];	    // ライトの位置
	XMFLOAT4	m_Diffuse[MAX_POINT_LIGHT];	        // 拡散光の色
	XMFLOAT4	m_Ambient[MAX_POINT_LIGHT];		    // 環境光の色
	XMFLOAT4	m_Attenuation[MAX_POINT_LIGHT];	    // 減衰率    
	XMFLOAT4    m_intensity[MAX_POINT_LIGHT];       // ライトの強度
	XMINT4		m_Enable[MAX_POINT_LIGHT];
	XMINT4		allEnable;

};




class LightManager
{

public:


    LightManager(World* world);
    ~LightManager();

    void Awake();
	void Update();

	void ClearLight(void);

    void SetLightNear(XMFLOAT3 pos);//指定のポジションに近いライトをオンにする

	int AddLight(DirectionalLightComponent* com);
	int AddLight(PointLightComponent* com);

	void RemoveLight(DirectionalLightComponent* com);
	void RemoveLight(PointLightComponent* com);



	void SetPointLight(PointLightComponent* pointlightComponent, int index);
	void SetDirecLight(DirectionalLightComponent* direclightComponent, int index);

	DirectionalLightComponent* GetMainLight(void);

	void SetAllEnable(BOOL flag);
private:
    GameEngine* pGameEngine;
	World* pWorld;
	CBufferManager* cBufferManager;

    DIREC_LIGHT_CBUFFER direcLightCBufferStruct;
    POINT_LIGHT_CBUFFER pointLightCBufferStruct;


    list<DirectionalLightComponent*> direcLightList;
    list<PointLightComponent*> pointLightList;

    ID3D11Buffer* direcLightBuffer;
    ID3D11Buffer* pointLightBuffer;



};

