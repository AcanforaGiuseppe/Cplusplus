#include <dxgi1_4.h>
#include <iostream>
#include <d3d12.h>

ID3D12Heap* Create_Heap(ID3D12Device* device, D3D12_HEAP_TYPE type, UINT64 size)
{
	D3D12_HEAP_DESC heap_desc = {};
	heap_desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
	heap_desc.Properties.Type = type;
	heap_desc.Flags = D3D12_HEAP_FLAG_NONE;
	heap_desc.SizeInBytes = size;

	ID3D12Heap* heap = nullptr;
	HRESULT result = device->CreateHeap(&heap_desc, __uuidof(ID3D12Heap), reinterpret_cast<void**>(&heap));

	if (!SUCCEEDED(result))
		std::cerr << "Unable to create Heap " << result << std::endl;

	return heap;
}

ID3D12Resource* Create_Buffer_in_Heap(ID3D12Device* device, ID3D12Heap* heap, UINT64 heap_offset, UINT64 size)
{
	D3D12_RESOURCE_DESC heap_desc = {};
	heap_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	heap_desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
	heap_desc.Width = size;
	heap_desc.Height = 1;
	heap_desc.DepthOrArraySize = 1;
	heap_desc.MipLevels = 1;
	heap_desc.Format = DXGI_FORMAT_UNKNOWN;
	heap_desc.SampleDesc.Count = 1;
	heap_desc.SampleDesc.Quality = 0;
	heap_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	heap_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	ID3D12Resource* resource = nullptr;
	D3D12_RESOURCE_STATES state;

	switch (heap->GetDesc().Properties.Type)
	{
	case D3D12_HEAP_TYPE_DEFAULT:
		state = D3D12_RESOURCE_STATE_COMMON;
		break;
	case D3D12_HEAP_TYPE_UPLOAD:
		state = D3D12_RESOURCE_STATE_GENERIC_READ;
		break;
	case D3D12_HEAP_TYPE_READBACK:
		state = D3D12_RESOURCE_STATE_COPY_DEST;
		break;
	case D3D12_HEAP_TYPE_CUSTOM:
		state = D3D12_RESOURCE_STATE_COMMON;
		break;
	default:
		break;
	}

	HRESULT result = device->CreatePlacedResource(heap, heap_offset, &heap_desc, state, nullptr, __uuidof(ID3D12Resource), reinterpret_cast<void**>(&resource));

	if (!SUCCEEDED(result))
		std::cerr << "Unable to initialize " << result << std::endl;

	return resource;
}

ID3D12Resource* Create_Buffer(ID3D12Device* device, D3D12_HEAP_TYPE type, UINT64 size)
{
	D3D12_HEAP_PROPERTIES props = {};
	props.Type = type;

	D3D12_RESOURCE_DESC heap_desc = {};
	heap_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	heap_desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
	heap_desc.Width = size;
	heap_desc.Height = 1;
	heap_desc.DepthOrArraySize = 1;
	heap_desc.MipLevels = 1;
	heap_desc.Format = DXGI_FORMAT_UNKNOWN;
	heap_desc.SampleDesc.Count = 1;
	heap_desc.SampleDesc.Quality = 0;
	heap_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	heap_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	ID3D12Resource* resource = nullptr;
	D3D12_RESOURCE_STATES state;

	switch (type)
	{
	case D3D12_HEAP_TYPE_DEFAULT:
		state = D3D12_RESOURCE_STATE_COMMON;
		break;
	case D3D12_HEAP_TYPE_UPLOAD:
		state = D3D12_RESOURCE_STATE_GENERIC_READ;
		break;
	case D3D12_HEAP_TYPE_READBACK:
		state = D3D12_RESOURCE_STATE_COPY_DEST;
		break;
	case D3D12_HEAP_TYPE_CUSTOM:
		state = D3D12_RESOURCE_STATE_COMMON;
		break;
	default:
		break;
	}

	HRESULT result = device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &heap_desc, state, nullptr, __uuidof(ID3D12Resource), reinterpret_cast<void**>(&resource));

	if (!SUCCEEDED(result))
		std::cerr << "Unable to initialize " << result << std::endl;

	return resource;
}

int main(int argc, char** argv)
{
	HRESULT HR = CoInitializeEx(nullptr, 0);

	if (!SUCCEEDED(HR))
	{
		std::cerr << "Unable to initialize " << HR << std::endl;
		return -1;
	}

	IDXGIFactory* factory = nullptr;	HRESULT result = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, __uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));

	if (!SUCCEEDED(result))
	{
		std::cerr << "Unable to create IDXGIFactory " << result << std::endl;
		return -1;
	}

	std::cout << "OK" << std::endl;
	ID3D12Debug* debug = nullptr;
	result = D3D12GetDebugInterface(__uuidof(ID3D12Debug), reinterpret_cast<void**>(&debug));

	if (!SUCCEEDED(result))
	{
		std::cerr << "Unable to create debug " << result << std::endl;
		return -1;
	}
	else
	{
		debug->EnableDebugLayer();
	}

	UINT adapter_index = 0;
	IDXGIAdapter* adapter = nullptr;
	IDXGIAdapter* best_adapter = nullptr;
	DXGI_ADAPTER_DESC adapter_desc;
	SIZE_T best_adapter_VM = 0;

	for (;;) // while(true)
	{
		result = factory->EnumAdapters(adapter_index, &adapter);

		if (SUCCEEDED(result))
		{
			adapter_index++;
			result = adapter->GetDesc(&adapter_desc);

			if (!SUCCEEDED(result))
			{
				std::cerr << "Unable to get description of the adapter " << result << std::endl;
				return -1;
			}

			std::wcout << adapter_desc.Description << " " <<
				adapter_desc.DedicatedVideoMemory / (1024 * 1024) << " " <<
				adapter_desc.DedicatedSystemMemory / (1024 * 1024) << " " <<
				adapter_desc.SharedSystemMemory / (1024 * 1024) <<
				std::endl;

			if (best_adapter == nullptr || adapter_desc.DedicatedVideoMemory > best_adapter_VM)
			{
				best_adapter = adapter;
				best_adapter_VM = adapter_desc.DedicatedVideoMemory;
			}
		}
		else if (result == DXGI_ERROR_NOT_FOUND)
		{
			break;
		}
		else
		{
			std::cerr << "Unable to Enumerate IDXGIAdapter " << result << std::endl;
			return -1;
		}
	}
	std::cout << best_adapter << std::endl;

	UINT output_index = 0;
	IDXGIOutput* output = nullptr;
	DXGI_OUTPUT_DESC output_desc;

	for (;;) // while(true)
	{
		result = best_adapter->EnumOutputs(output_index, &output);

		if (SUCCEEDED(result))
		{
			output_index++;
			result = output->GetDesc(&output_desc);

			if (!SUCCEEDED(result))
			{
				std::cerr << "Unable to get description of the output " << result << std::endl;
				return -1;
			}

			std::wcout
				<< output_desc.DeviceName
				<< " " << output_desc.DesktopCoordinates.left
				<< " " << output_desc.DesktopCoordinates.top
				<< " " << output_desc.DesktopCoordinates.right
				<< " " << output_desc.DesktopCoordinates.bottom
				<< std::endl;
		}
		else if (result == DXGI_ERROR_NOT_FOUND)
		{
			break;
		}
		else
		{
			std::cerr << "Unable to Enumerate IDXGIOutput " << result << std::endl;
			return -1;
		}
	}

	// Create Direct3D12 device
	ID3D12Device* device = nullptr;	result = D3D12CreateDevice(best_adapter, D3D_FEATURE_LEVEL_12_1, __uuidof(ID3D12Device), reinterpret_cast<void**>(&device));

	if (!SUCCEEDED(result))
	{
		std::cerr << "Unable to create Device " << result << std::endl;
		return -1;
	}
	else
	{
		std::cout << "Device Created" << std::endl;
	}

	ID3D12Resource* buffer_upload = Create_Buffer(device, D3D12_HEAP_TYPE_UPLOAD, 1024);
	ID3D12Resource* buffer_default = Create_Buffer(device, D3D12_HEAP_TYPE_DEFAULT, 1024);
	ID3D12Resource* buffer_readback = Create_Buffer(device, D3D12_HEAP_TYPE_READBACK, 1024);

	D3D12_RESOURCE_DESC upload_desc = buffer_upload->GetDesc();
	std::cout << upload_desc.Width << std::endl;

	D3D12_RESOURCE_DESC default_desc = buffer_default->GetDesc();
	std::cout << default_desc.Width << std::endl;

	D3D12_RESOURCE_DESC readback_desc = buffer_readback->GetDesc();
	std::cout << readback_desc.Width << std::endl;

	UINT subresource = 0;
	D3D12_RANGE* range = nullptr;
	unsigned char* data = nullptr;
	result = buffer_upload->Map(subresource, range, reinterpret_cast<void**>(&data));

	if (!SUCCEEDED(result))
	{
		std::cerr << "Failed to map buffer" << result << std::endl;
		return -1;
	}

	for (size_t i = 0; i < 1024; i++)
		data[i] = i;

	buffer_upload->Unmap(subresource, range);

	D3D12_COMMAND_QUEUE_DESC queue_desc = {};
	queue_desc.Type = D3D12_COMMAND_LIST_TYPE_COPY;

	ID3D12CommandQueue* queue = nullptr;
	result = device->CreateCommandQueue(&queue_desc, __uuidof(ID3D12CommandQueue), reinterpret_cast<void**>(&queue));

	if (!SUCCEEDED(result))
	{
		std::cerr << "Failed to CommandQueue " << result << std::endl;
		return -1;
	}

	ID3D12Fence* fence = nullptr;
	result = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), reinterpret_cast<void**>(&fence));

	if (!SUCCEEDED(result))
	{
		std::cerr << "Failed to create Fence " << result << std::endl;
		return -1;
	}

	HANDLE event = CreateEvent(nullptr, 0, 0, nullptr);

	ID3D12CommandAllocator* command_allocator = nullptr;
	result = device->CreateCommandAllocator(queue_desc.Type, __uuidof(ID3D12CommandAllocator), reinterpret_cast<void**>(&command_allocator));

	if (!SUCCEEDED(result))
	{
		std::cerr << "Failed to CommandAllocator " << result << std::endl;
		return -1;
	}

	ID3D12GraphicsCommandList* graphics_command_list = nullptr;
	result = device->CreateCommandList(0, queue_desc.Type, command_allocator, nullptr, __uuidof(ID3D12GraphicsCommandList), reinterpret_cast<void**>(&graphics_command_list));

	if (!SUCCEEDED(result))
	{
		std::cerr << "Failed to create GraphicsCommandList " << result << std::endl;
		return -1;
	}

	graphics_command_list->CopyResource(buffer_default, buffer_upload);
	result = graphics_command_list->Close();
	queue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList**>(&graphics_command_list));
	queue->Signal(fence, 1);
	fence->SetEventOnCompletion(1, event);

	WaitForSingleObject(event, INFINITE);

	graphics_command_list->Reset(command_allocator, nullptr);
	graphics_command_list->CopyResource(buffer_readback, buffer_default);
	graphics_command_list->Close();

	queue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList**>(&graphics_command_list));
	queue->Signal(fence, 2);
	fence->SetEventOnCompletion(2, event);

	WaitForSingleObject(event, INFINITE);

	result = buffer_readback->Map(subresource, range, reinterpret_cast<void**>(&data));

	if (!SUCCEEDED(result))
	{
		std::cerr << "Failed to read Buffer " << result << std::endl;
		return -1;
	}

	for (size_t i = 0; i < 1024; i++)
		std::cout << static_cast<int>(data[i]) << std::endl;

	buffer_readback->Unmap(subresource, range);

	ID3D12Heap* heap = Create_Heap(device, D3D12_HEAP_TYPE_DEFAULT, 256 * 1024);

	if (heap == nullptr)
		std::cerr << "Failed to create an Heap" << std::endl;

	D3D12_ROOT_PARAMETER1 Parameters[2] = {};
	Parameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	Parameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	Parameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	Parameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_VERSIONED_ROOT_SIGNATURE_DESC Signature = {};
	Signature.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
	Signature.Desc_1_1.NumParameters = 2;
	Signature.Desc_1_1.pParameters = Parameters;

	ID3DBlob* blob = nullptr;
	result = D3D12SerializeVersionedRootSignature(&Signature, &blob, nullptr);

	if (!SUCCEEDED(result))
	{
		std::cerr << "Failed to serialize root signature " << result << std::endl;
		return -1;
	}

	ID3D12RootSignature* Root_signature = nullptr;
	result = device->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), __uuidof(ID3D12RootSignature), reinterpret_cast<void**>(&Root_signature));

	if (!SUCCEEDED(result))
	{
		std::cerr << "Failed to create root signature " << result << std::endl;
		return -1;
	}

	D3D12_DESCRIPTOR_HEAP_DESC Descriptor_heap_desc = {};
	Descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	Descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	Descriptor_heap_desc.NumDescriptors = 2;

	ID3D12DescriptorHeap* Descriptor_heap = nullptr;
	result = device->CreateDescriptorHeap(&Descriptor_heap_desc, __uuidof(ID3D12DescriptorHeap), reinterpret_cast<void**>(&Descriptor_heap));

	if (!SUCCEEDED(result))
	{
		std::cerr << "Failed to create Descriptor Heap" << result << std::endl;
		return -1;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC Resourse_view_desc = {};
	Resourse_view_desc.Format = DXGI_FORMAT_R32_FLOAT;
	Resourse_view_desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	Resourse_view_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	Resourse_view_desc.Buffer.NumElements = 1024 / 4;

	D3D12_CPU_DESCRIPTOR_HANDLE Descriptor_handle = {};
	device->CreateShaderResourceView(buffer_default, &Resourse_view_desc, Descriptor_handle);

	//TODO device->CreateUnorderedAccessView(,)

	ID3D12Resource* resource_1 = Create_Buffer_in_Heap(device, heap, 0, 2 * 1024);
	ID3D12Resource* resource_2 = Create_Buffer_in_Heap(device, heap, 0, 2 * 1024);

	buffer_upload->Release();
	buffer_default->Release();
	buffer_readback->Release();
	device->Release();
	factory->Release();

	return 0;
}