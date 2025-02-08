#include "UIShader.h"
#include "renderer.h"

UIShader::UIShader(Renderer* renderer)
{

    pRenderer = renderer;
    CreateVS("shaders/UIShader.hlsl", "VSmain");
    CreatePS("shaders/UIShader.hlsl", "PSmain");

    shaderIndex = ShaderIndex::UI;


}

UIShader::~UIShader()
{
}
