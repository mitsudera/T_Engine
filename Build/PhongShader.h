#pragma once
#include "ShaderSet.h"

class Renderer;


class PhongShader : public ShaderSet
{
public:

	PhongShader(Renderer* renderer);
	~PhongShader();

	void SetMaterialCbuffer(MaterialCBuffer data);


private:



};

