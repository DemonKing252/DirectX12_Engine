#pragma once
#include <DirectXMath.h>
class ConstantBuffer
{
public:
	DirectX::XMMATRIX World;
	DirectX::XMMATRIX Model;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Projection;
};

