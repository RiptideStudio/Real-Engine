#include <d3d11.h>
#include <wrl/client.h>
#include <wincodec.h>
#include <vector>

using Microsoft::WRL::ComPtr;

class Texture : public Component
{
public:
    void Update(float dt) {}
    Texture(Microsoft::WRL::ComPtr<ID3D11Device> device)
        : m_device(device)
    {}

    HRESULT LoadFromFile(const wchar_t* filename)
    {
        HRESULT hr = S_OK;

        ComPtr<IWICImagingFactory> factory;
        hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
        if (FAILED(hr))
        {
            return hr;
        }

        ComPtr<IWICBitmapDecoder> decoder;
        hr = factory->CreateDecoderFromFilename(filename, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
        if (FAILED(hr))
        {
            return hr;
        }

        ComPtr<IWICBitmapFrameDecode> frame;
        hr = decoder->GetFrame(0, &frame);
        if (FAILED(hr))
        {
            return hr;
        }

        UINT width, height;
        hr = frame->GetSize(&width, &height);
        if (FAILED(hr))
        {
            return hr;
        }

        ComPtr<IWICFormatConverter> converter;
        hr = factory->CreateFormatConverter(&converter);
        if (FAILED(hr))
        {
            return hr;
        }

        hr = converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom);
        if (FAILED(hr))
        {
            return hr;
        }

        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;

        std::vector<BYTE> pixelData(width * height * 4);
        hr = converter->CopyPixels(nullptr, width * 4, static_cast<UINT>(pixelData.size()), pixelData.data());
        if (FAILED(hr))
        {
            return hr;
        }

        D3D11_SUBRESOURCE_DATA data = {};
        data.pSysMem = pixelData.data();
        data.SysMemPitch = width * 4;

        hr = m_device->CreateTexture2D(&desc, &data, m_texture.ReleaseAndGetAddressOf());
        if (FAILED(hr))
        {
            return hr;
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = desc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = -1;
        srvDesc.Texture2D.MostDetailedMip = 0;

        hr = m_device->CreateShaderResourceView(m_texture.Get(), &srvDesc, m_srv.ReleaseAndGetAddressOf());
        if (FAILED(hr))
        {
            return hr;
        }

        return S_OK;
    }

    ID3D11ShaderResourceView* GetResourceView() const { return m_srv.Get(); }

private:
    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11Texture2D> m_texture;
    ComPtr<ID3D11ShaderResourceView> m_srv;
};
