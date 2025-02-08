#pragma once
#include "ShaderSet.h"

class Renderer;

struct TessCBuffer
{
	float cbEdgeFactor; //4�p�`�̕ӂ̕����ʂ̎w��
	float cbInsideFactor; //4�p�`�̓����̕����ʂ̎w��
	float heightFactor; //heightMap�̍����ɑ΂��Ă�����l
	float dummy;
	float uvScale[2];//uv�X�P�[��
	float heghtMapUVScale[2];//heightmap��uv�X�P�[��

};
class TerrainShader : public ShaderSet
{
public:

	TerrainShader(Renderer* renderer);
	~TerrainShader();

	void SetMaterialCbuffer(MaterialCBuffer data);

	void SetTessBuffer(TessCBuffer data);

	virtual void Uninit(void)override;



private:

	ID3D11Buffer* tessBuffer;


};

