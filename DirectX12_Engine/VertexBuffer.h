#pragma once
#include <d3d12.h>
#include "d3dx12.h"
#include <Windows.h>
#include <wrl.h>
#include "EntityComponentSystem.h"

// Can't include d3dApp or else we get a circular reference.
#define CheckSucceeded(hr) if (!SUCCEEDED(hr)) { DebugBreak();}
template <class T>
class VertexBufferComponent : public Component
{
public:
	VertexBufferComponent(VertexBufferComponent<T>* other);
	VertexBufferComponent();

	void Initialize(ID3D12Device* device, T* data, UINT numVerticies);
	void ZeroMem();

	UINT GetOffSet() const;
	UINT GetStride() const;
	Microsoft::WRL::ComPtr<ID3D12Resource> GetResource() const;
	D3D12_GPU_VIRTUAL_ADDRESS GetMemoryAddress() const;
	D3D12_VERTEX_BUFFER_VIEW Get() const;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_buffer;
	UINT m_uStride;
	UINT m_uOffSet;
	UINT m_uSize;

	D3D12_VERTEX_BUFFER_VIEW m_indexBufferView;

	void* m_rawData;
};

template<class T>
inline VertexBufferComponent<T>::VertexBufferComponent(VertexBufferComponent<T>* other)
{
	this->m_buffer = other->GetResource();
	this->m_uStride = other->GetStride();
}

template<class T>
inline VertexBufferComponent<T>::VertexBufferComponent()
{
	m_uStride = sizeof(T);
	m_uOffSet = 0;
}

template<class T>
inline void VertexBufferComponent<T>::Initialize(ID3D12Device * device, T * data, UINT numVertices)
{
	m_uSize = sizeof(T) * numVertices;

	CheckSucceeded(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(m_uSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_buffer.GetAddressOf())
	));

	CD3DX12_RANGE readRange(0, 0);
	m_buffer->Map(0, &readRange, reinterpret_cast<void**>(&m_rawData));
	CopyMemory(m_rawData, data, m_uSize);
	m_buffer->Unmap(0, nullptr);

	m_indexBufferView.BufferLocation = m_buffer->GetGPUVirtualAddress();
	m_indexBufferView.SizeInBytes = m_uSize;
	m_indexBufferView.StrideInBytes = sizeof(T);

}

template<class T>
inline void VertexBufferComponent<T>::ZeroMem()
{
	ZeroMemory(m_buffer.GetAddressOf(), sizeof(ID3D12Resource));
}

template<class T>
inline UINT VertexBufferComponent<T>::GetOffSet() const
{
	return m_uOffSet;
}

template<class T>
inline UINT VertexBufferComponent<T>::GetStride() const
{
	return m_uStride;
}

template<class T>
inline Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferComponent<T>::GetResource() const
{
	return m_buffer;
}

template<class T>
inline D3D12_GPU_VIRTUAL_ADDRESS VertexBufferComponent<T>::GetMemoryAddress() const
{
	return m_buffer->GetGPUVirtualAddress();
}

template<class T>
inline D3D12_VERTEX_BUFFER_VIEW VertexBufferComponent<T>::Get() const
{
	return m_indexBufferView;
}
