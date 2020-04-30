/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Wrapper around platform 3D graphics library.
 *
 * @copyright Thyme is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#include "always.h"
#include "dx8caps.h"
#include "matrix4.h"
#include "refcount.h"
#include "renderdevicedesc.h"
#include "shader.h"
#include "texturebase.h"
#include "vector.h"
#include "vector4.h"
#include "w3dtypes.h"
#include "wwstring.h"
#include <captainslog.h>
#include "vertmaterial.h"
#include "texture.h"
class VertexBufferClass;
class IndexBufferClass;
class SurfaceClass;
class DynamicVBAccessClass;
class DynamicIBAccessClass;
class LightEnvironmentClass;
class ZTextureClass;
class DX8_CleanupHook
{
public:
    virtual void ReleaseResources() = 0;
    virtual void ReAcquireResources() = 0;
};

// Some constants to control numbers of things.
enum
{
    MAX_TEXTURE_STAGES = 8,
    VERTEX_BUFFERS = 2,
    GFX_LIGHT_COUNT = 4,
};

#ifdef GAME_DLL
extern uint32_t &g_numberOfDx8Calls;
#else
extern uint32_t g_numberOfDx8Calls;
#endif

#ifdef BUILD_WITH_D3D8
#define DX8CALL(x) \
    DX8Wrapper::Get_D3D_Device8()->x; \
    ++g_numberOfDx8Calls;
#define DX8CALL_HRES(x, res) \
    res = DX8Wrapper::Get_D3D_Device8()->x; \
    ++g_numberOfDx8Calls;
#endif

struct RenderStateStruct
{
    ShaderClass shader;
    VertexMaterialClass *material;
    TextureClass *Textures[MAX_TEXTURE_STAGES];
#ifdef BUILD_WITH_D3D8
    D3DLIGHT8 Lights[GFX_LIGHT_COUNT];
#endif
    bool LightEnable[GFX_LIGHT_COUNT];
    Matrix4 world;
    Matrix4 view;
    unsigned vertex_buffer_types[VERTEX_BUFFERS];
    unsigned index_buffer_type;
    unsigned short vba_offset;
    unsigned short vba_count;
    unsigned short iba_offset;
    VertexBufferClass *vertex_buffers[VERTEX_BUFFERS];
    IndexBufferClass *index_buffer;
    unsigned short index_base_offset;

    RenderStateStruct();
    ~RenderStateStruct();

    RenderStateStruct &operator=(const RenderStateStruct &src);
};

class DX8Wrapper
{
    ALLOW_HOOKING
    enum ChangedStates
    {
        WORLD_CHANGED = 1 << 0,
        VIEW_CHANGED = 1 << 1,
        LIGHT0_CHANGED = 1 << 2,
        LIGHT1_CHANGED = 1 << 3,
        LIGHT2_CHANGED = 1 << 4,
        LIGHT3_CHANGED = 1 << 5,
        TEXTURE0_CHANGED = 1 << 6,
        TEXTURE1_CHANGED = 1 << 7,
        TEXTURE2_CHANGED = 1 << 8,
        TEXTURE3_CHANGED = 1 << 9,
        MATERIAL_CHANGED = 1 << 14,
        SHADER_CHANGED = 1 << 15,
        VERTEX_BUFFER_CHANGED = 1 << 16,
        INDEX_BUFFER_CHANGED = 1 << 17,
        WORLD_IDENTITY = 1 << 18,
        VIEW_IDENTITY = 1 << 19,

        TEXTURES_CHANGED = TEXTURE0_CHANGED | TEXTURE1_CHANGED | TEXTURE2_CHANGED | TEXTURE3_CHANGED,
        LIGHTS_CHANGED = LIGHT0_CHANGED | LIGHT1_CHANGED | LIGHT2_CHANGED | LIGHT3_CHANGED,
    };

public:
#ifdef PLATFORM_WINDOWS
    static bool Init(HWND hwnd, bool lite = false);
#endif
    static void Shutdown();
    static void Do_Onetime_Device_Dependent_Inits();
    static void Do_Onetime_Device_Dependent_Shutdowns();
    static void Begin_Scene(void);
    static void End_Scene(bool flip_frames = true);
    static void Clear(
        bool clear_color, bool clear_z_stencil, const Vector3 &color, float alpha, float z = 1.0f, unsigned int stencil = 0);
    static void Set_Vertex_Buffer(const VertexBufferClass *vb, int number);
    static void Set_Vertex_Buffer(const DynamicVBAccessClass &vba);
    static void Set_Index_Buffer(const IndexBufferClass *ib, unsigned short index_base_offset);
    static void Set_Index_Buffer(const DynamicIBAccessClass &iba, unsigned short index_base_offset);
    static void Set_Gamma(float gamma, float bright, float contrast, bool calibrate = true, bool uselimit = true);
    static void Set_Light_Environment(LightEnvironmentClass *light_env);
    static void Apply_Render_State_Changes();
    static void Draw_Triangles(unsigned int buffer_type, unsigned short start_index, unsigned short polygon_count,
        unsigned short min_vertex_index, unsigned short vertex_count);
    static void Draw_Triangles(unsigned short start_index, unsigned short polygon_count, unsigned short min_vertex_index,
        unsigned short vertex_count);
    static void Draw_Strip(unsigned short start_index, unsigned short polygon_count, unsigned short min_vertex_index,
        unsigned short vertex_count);
    static w3dtexture_t Create_Texture(
        unsigned width, unsigned height, WW3DFormat format, MipCountType mip_level_count, w3dpool_t pool, bool rendertarget);
    static w3dtexture_t Create_Texture(w3dsurface_t surface, MipCountType mip_level_count);
    static w3dsurface_t Create_Surface(unsigned width, unsigned height, WW3DFormat format);
    static w3dsurface_t Create_Surface(const char *name);
    static SurfaceClass *_Get_DX8_Back_Buffer(unsigned int num = 0);
    static void Begin_Statistics();
    static void End_Statistics();
    static TextureClass *Create_Render_Target(int width, int height, WW3DFormat format);
    static void Set_Render_Target(w3dsurface_t render_target, bool use_default_depth_buffer = false);
    static void Set_Render_Target_With_Z(TextureClass *texture, ZTextureClass *z_texture);
    static void Set_Texture(unsigned stage, TextureClass *texture);
    static void Set_Material(const VertexMaterialClass *material);

#ifdef BUILD_WITH_D3D8
    static const char *Get_DX8_Render_State_Name(D3DRENDERSTATETYPE state);
    static const char *Get_DX8_Texture_Stage_State_Name(D3DTEXTURESTAGESTATETYPE state);
    static void Get_DX8_Texture_Stage_State_Value_Name(StringClass &name, D3DTEXTURESTAGESTATETYPE state, unsigned value);
    static void Get_DX8_Render_State_Value_Name(StringClass &name, D3DRENDERSTATETYPE state, unsigned value);
    static void Set_Viewport(CONST D3DVIEWPORT8 *pViewport);
    static void Set_Light(unsigned index, const D3DLIGHT8 *light);
    static void Set_Transform(D3DTRANSFORMSTATETYPE transform, const Matrix4 &m);
    static void Get_Transform(D3DTRANSFORMSTATETYPE transform, Matrix4 &m);
    static void Set_DX8_Render_State(D3DRENDERSTATETYPE state, unsigned value);
    static void Set_DX8_Texture_Stage_State(unsigned stage, D3DTEXTURESTAGESTATETYPE state, unsigned value);
    static void Set_DX8_Texture(unsigned stage, w3dbasetexture_t texture);
    static IDirect3DDevice8 *Get_D3D_Device8() { return s_d3dDevice; }
    static IDirect3DSurface8 *_Get_DX8_Front_Buffer();
    static Vector4 Convert_Color(unsigned color);
    static unsigned int Convert_Color(const Vector4 &color);
    static unsigned int Convert_Color(const Vector3 &color, const float alpha);
    static void Set_DX8_Light(int index, D3DLIGHT8 *light);
    static void _Set_DX8_Transform(D3DTRANSFORMSTATETYPE transform, const Matrix4 &m);
#endif
    static const char *Get_DX8_Texture_Address_Name(unsigned value);
    static const char *Get_DX8_Texture_Filter_Name(unsigned value);
    static const char *Get_DX8_Texture_Arg_Name(unsigned value);
    static const char *Get_DX8_Texture_Op_Name(unsigned value);
    static const char *Get_DX8_Texture_Transform_Flag_Name(unsigned value);
    static const char *Get_DX8_ZBuffer_Type_Name(unsigned value);
    static const char *Get_DX8_Fill_Mode_Name(unsigned value);
    static const char *Get_DX8_Shade_Mode_Name(unsigned value);
    static const char *Get_DX8_Blend_Name(unsigned value);
    static const char *Get_DX8_Cull_Mode_Name(unsigned value);
    static const char *Get_DX8_Cmp_Func_Name(unsigned value);
    static const char *Get_DX8_Fog_Mode_Name(unsigned value);
    static const char *Get_DX8_Stencil_Op_Name(unsigned value);
    static const char *Get_DX8_Material_Source_Name(unsigned value);
    static const char *Get_DX8_Vertex_Blend_Flag_Name(unsigned value);
    static const char *Get_DX8_Patch_Edge_Style_Name(unsigned value);
    static const char *Get_DX8_Debug_Monitor_Token_Name(unsigned value);
    static const char *Get_DX8_Blend_Op_Name(unsigned value);
    static void Log_DX8_ErrorCode(unsigned error);
    static void Handle_DX8_ErrorCode(unsigned error);
    static int Get_Texture_Bit_Depth() { return s_textureBitDepth; }

    static int Get_Main_Thread_ID() { return s_mainThreadID; }
    static const DX8Caps *Get_Caps()
    {
        captainslog_assert(s_currentCaps != nullptr);
        return s_currentCaps;
    }
    static bool Supports_DXTC() { return s_currentCaps->Supports_DXTC(); }
    static bool Has_Stencil();
    static WW3DFormat Get_Back_Buffer_Format();
    static void Get_Device_Resolution(int &set_w, int &set_h, int &set_bits, bool &set_windowed);
    static const w3dadapterid_t &Get_Current_Adapter_Identifier() { return s_currentAdapterIdentifier; }

private:
    static bool Create_Device();
    static bool Reset_Device(bool reacquire);
    static void Release_Device();
    static void Reset_Statistics();
    static void Enumerate_Devices();
    static void Set_Default_Global_Render_States();
    static void Invalidate_Cached_Render_States();
    static int Get_Render_Device(void);
    static const RenderDeviceDescClass &Get_Render_Device_Desc(int deviceidx);
    static bool Set_Device_Resolution(
        int width = -1, int height = -1, int bits = -1, int windowed = -1, bool resize_window = false);
    static bool Set_Render_Device(int dev = -1, int resx = -1, int resy = -1, int bits = -1, int windowed = -1,
        bool resize_window = false, bool reset_device = false, bool restore_assets = true);
    static void Get_Render_Target_Resolution(int &set_w, int &set_h, int &set_bits, bool &set_windowed);
    static void Draw_Sorting_IB_VB(unsigned int primitive_type, unsigned short start_index, unsigned short polygon_count,
        unsigned short min_vertex_index, unsigned short vertex_count);
    static void Draw(unsigned int primitive_type, unsigned short start_index, unsigned short polygon_count,
        unsigned short min_vertex_index = 0, unsigned short vertex_count = 0);
#ifdef BUILD_WITH_D3D8
    static bool Find_Color_And_Z_Mode(int resx, int resy, int bitdepth, D3DFORMAT *set_colorbuffer, D3DFORMAT *set_backbuffer, D3DFORMAT *set_zmode);
    static bool Find_Color_Mode(D3DFORMAT colorbuffer, int resx, int resy, UINT *mode);
    static bool Find_Z_Mode(D3DFORMAT colorbuffer, D3DFORMAT backbuffer, D3DFORMAT *zmode);
    static bool Test_Z_Mode(D3DFORMAT colorbuffer, D3DFORMAT backbuffer, D3DFORMAT zmode);
    static void Compute_Caps(WW3DFormat display_format);
    static void Get_Format_Name(unsigned int format, StringClass *format_name);
#endif

private:
#ifdef GAME_DLL
#ifdef BUILD_WITH_D3D8
    static IDirect3D8 *(__stdcall *&s_d3dCreateFunction)(unsigned);
    static HMODULE &s_d3dLib;
    static IDirect3D8 *&s_d3dInterface;
    static IDirect3DDevice8 *&s_d3dDevice;
    static D3DMATRIX &s_oldPrj;
    static D3DMATRIX &s_oldView;
    static D3DMATRIX &s_oldWorld;
    static D3DPRESENT_PARAMETERS &s_presentParameters;
    static D3DCOLOR &s_fogColor;
    static D3DFORMAT &s_displayFormat;
#endif
    static ARRAY_DEC(w3dbasetexture_t, s_textures, MAX_TEXTURE_STAGES);
#ifdef PLATFORM_WINDOWS
    static HWND &s_hwnd;
#endif
    static void *&s_shadowMap;
    static ARRAY_DEC(unsigned, s_renderStates, 256);
    static ARRAY2D_DEC(unsigned, s_textureStageStates, MAX_TEXTURE_STAGES, 32);
    static ARRAY_DEC(Vector4, s_vertexShaderConstants, 96);
    static ARRAY_DEC(unsigned, s_pixelShaderConstants, 32);
    static bool &s_isInitialised;
    static bool &s_isWindowed;
    static bool &s_debugIsWindowed;
    static RenderStateStruct &s_renderState;
    static unsigned &s_renderStateChanged;
    static float &s_zNear;
    static float &s_zFar;
    static Matrix4 &s_projectionMatrix;
    static int &s_mainThreadID;
    static int &s_currentRenderDevice;
    static DX8Caps *&s_currentCaps;
    static int &s_resolutionWidth;
    static int &s_resolutionHeight;
    static int &s_bitDepth;
    static int &s_textureBitDepth;
    static ARRAY_DEC(bool, s_currentLightEnables, GFX_LIGHT_COUNT);
    static unsigned &s_matrixChanges;
    static unsigned &s_materialChanges;
    static unsigned &s_vertexBufferChanges;
    static unsigned &s_indexBufferChanges;
    static unsigned &s_lightChanges;
    static unsigned &s_textureChanges;
    static unsigned &s_renderStateChanges;
    static unsigned &s_textureStageStateChanges;
    static unsigned &s_drawCalls;
    static unsigned &s_lastFrameMatrixChanges;
    static unsigned &s_lastFrameMaterialChanges;
    static unsigned &s_lastFrameVertexBufferChanges;
    static unsigned &s_lastFrameIndexBufferChanges;
    static unsigned &s_lastFrameLightChanges;
    static unsigned &s_lastFrameTextureChanges;
    static unsigned &s_lastFrameRenderStateChanges;
    static unsigned &s_lastFrameTextureStageStateChanges;
    static unsigned &s_lastFrameNumberDX8Calls;
    static unsigned &s_lastFrameDrawCalls;
    static DynamicVectorClass<StringClass> &s_renderDeviceNameTable;
    static DynamicVectorClass<StringClass> &s_renderDeviceShortNameTable;
    static DynamicVectorClass<RenderDeviceDescClass> &s_renderDeviceDescriptionTable;
    static w3dadapterid_t &s_currentAdapterIdentifier;
    static ARRAY_DEC(Matrix4, s_DX8Transforms, 257);
    static bool &s_EnableTriangleDraw;
    static int &s_ZBias;
    static Vector3 &s_ambientColor;
    static bool &s_isDeviceLost;
    static int &s_FPUPreserve;
    static unsigned long &s_vertexShader;
    static unsigned long &s_pixelShader;
    static LightEnvironmentClass *&s_lightEnvironment;
    static unsigned long &s_vertexProcessingBehavior;
    static bool &s_fogEnable;
    static w3dsurface_t &s_currentRenderTarget;
    static w3dsurface_t &s_currentDepthBuffer;
    static w3dsurface_t &s_defaultRenderTarget;
    static w3dsurface_t &s_defaultDepthBuffer;
    static bool &s_isRenderToTexture;
    static unsigned int &s_drawPolygonLowBoundLimit;
    static unsigned long &s_frameCount;
    static bool &s_DX8SingleThreaded;
    static DX8_CleanupHook *&s_cleanupHook;
#else
#ifdef BUILD_WITH_D3D8
    static IDirect3D8 *(__stdcall *s_d3dCreateFunction)(unsigned);
    static HMODULE s_d3dLib;
    static IDirect3D8 *s_d3dInterface;
    static IDirect3DDevice8 *s_d3dDevice;
    static D3DMATRIX s_oldPrj;
    static D3DMATRIX s_oldView;
    static D3DMATRIX s_oldWorld;
    static D3DPRESENT_PARAMETERS s_presentParameters;
    static D3DCOLOR s_fogColor;
    static D3DFORMAT s_displayFormat;
#endif
#ifdef PLATFORM_WINDOWS
    static HWND s_hwnd;
#endif
    static void *s_shadowMap; // Not sure what type this actually is for now.
    static unsigned s_renderStates[256];
    static w3dbasetexture_t s_textures[MAX_TEXTURE_STAGES];
    static unsigned s_textureStageStates[MAX_TEXTURE_STAGES][32];
    static Vector4 s_vertexShaderConstants[96]; // Not 100% sure this is a Vector4 array
    static unsigned s_pixelShaderConstants[32]; // Not 100% on type, seems unused.
    static bool s_isInitialised;
    static bool s_isWindowed;
    static bool s_debugIsWindowed;
    static RenderStateStruct s_renderState;
    static unsigned s_renderStateChanged;
    static float s_zNear;
    static float s_zFar;
    static Matrix4 s_projectionMatrix;
    static int s_mainThreadID;
    static int s_currentRenderDevice;
    static DX8Caps *s_currentCaps;
    static int s_resolutionWidth;
    static int s_resolutionHeight;
    static int s_bitDepth;
    static int s_textureBitDepth;
    static bool s_currentLightEnables[GFX_LIGHT_COUNT];
    static unsigned s_matrixChanges;
    static unsigned s_materialChanges;
    static unsigned s_vertexBufferChanges;
    static unsigned s_indexBufferChanges;
    static unsigned s_lightChanges;
    static unsigned s_textureChanges;
    static unsigned s_renderStateChanges;
    static unsigned s_textureStageStateChanges;
    static unsigned s_drawCalls;
    static unsigned s_lastFrameMatrixChanges;
    static unsigned s_lastFrameMaterialChanges;
    static unsigned s_lastFrameVertexBufferChanges;
    static unsigned s_lastFrameIndexBufferChanges;
    static unsigned s_lastFrameLightChanges;
    static unsigned s_lastFrameTextureChanges;
    static unsigned s_lastFrameRenderStateChanges;
    static unsigned s_lastFrameTextureStageStateChanges;
    static unsigned s_lastFrameNumberDX8Calls;
    static unsigned s_lastFrameDrawCalls;
    static DynamicVectorClass<StringClass> s_renderDeviceNameTable;
    static DynamicVectorClass<StringClass> s_renderDeviceShortNameTable;
    static DynamicVectorClass<RenderDeviceDescClass> s_renderDeviceDescriptionTable;
    static w3dadapterid_t s_currentAdapterIdentifier;
    static Matrix4 s_DX8Transforms[257];
    static bool s_EnableTriangleDraw;
    static int s_ZBias;
    static Vector3 s_ambientColor;
    static bool s_isDeviceLost;
    static int s_FPUPreserve;
    static unsigned long s_vertexShader;
    static unsigned long s_pixelShader;
    static LightEnvironmentClass *s_lightEnvironment;
    static unsigned long s_vertexProcessingBehavior;
    static bool s_fogEnable;
    static w3dsurface_t s_currentRenderTarget;
    static w3dsurface_t s_currentDepthBuffer;
    static w3dsurface_t s_defaultRenderTarget;
    static w3dsurface_t s_defaultDepthBuffer;
    static bool s_isRenderToTexture;
    static unsigned int s_drawPolygonLowBoundLimit;
    static unsigned long s_frameCount;
    static bool s_DX8SingleThreaded;
    static DX8_CleanupHook *s_cleanupHook;
#endif
};

inline RenderStateStruct::RenderStateStruct() : material(0), index_buffer(0)
{
    for (unsigned i = 0; i < MAX_TEXTURE_STAGES; ++i)
        Textures[i] = 0;
    for (int i = 0; i < VERTEX_BUFFERS; i++) {
        vertex_buffers[i] = 0;
    }
}

inline RenderStateStruct::~RenderStateStruct()
{
    // Ref_Ptr_Release(material);
    for (int i = 0; i < VERTEX_BUFFERS; i++) {
        // Ref_Ptr_Release(vertex_buffers[i]);
    }
    // Ref_Ptr_Release(index_buffer);
    for (unsigned i = 0; i < MAX_TEXTURE_STAGES; ++i) {
        // Ref_Ptr_Release(Textures[i]);
    }
}

inline RenderStateStruct &RenderStateStruct::operator=(const RenderStateStruct &src)
{
    // Ref_Ptr_Set(material, src.material);
    for (int i = 0; i < VERTEX_BUFFERS; i++) {
        // Ref_Ptr_Set(vertex_buffers[i], src.vertex_buffers[i]);
    }

    // Ref_Ptr_Set(index_buffer, src.index_buffer);

    for (unsigned i = 0; i < MAX_TEXTURE_STAGES; ++i) {
        // Ref_Ptr_Set(Textures[i], src.Textures[i]);
    }

    LightEnable[0] = src.LightEnable[0];
    LightEnable[1] = src.LightEnable[1];
    LightEnable[2] = src.LightEnable[2];
    LightEnable[3] = src.LightEnable[3];
#ifdef BUILD_WITH_D3D8
    if (LightEnable[0]) {
        Lights[0] = src.Lights[0];

        if (LightEnable[1]) {
            Lights[1] = src.Lights[1];

            if (LightEnable[2]) {
                Lights[2] = src.Lights[2];

                if (LightEnable[3]) {
                    Lights[3] = src.Lights[3];
                }
            }
        }
    }
#endif
    shader = src.shader;
    world = src.world;
    view = src.view;

    for (int i = 0; i < VERTEX_BUFFERS; i++) {
        vertex_buffer_types[i] = src.vertex_buffer_types[i];
    }

    index_buffer_type = src.index_buffer_type;
    vba_offset = src.vba_offset;
    vba_count = src.vba_count;
    iba_offset = src.iba_offset;
    index_base_offset = src.index_base_offset;

    return *this;
}

#ifdef BUILD_WITH_D3D8
inline void DX8Wrapper::Set_DX8_Render_State(D3DRENDERSTATETYPE state, unsigned value)
{
    if (s_renderStates[state] == value)
        return;

    s_renderStates[state] = value;
    DX8CALL(SetRenderState(state, value));
}

inline void DX8Wrapper::Set_DX8_Texture_Stage_State(unsigned stage, D3DTEXTURESTAGESTATETYPE state, unsigned value)
{
    if (stage < 8) {
        if (s_textureStageStates[stage][state] == value) {
            return;
        }

        s_textureStageStates[stage][state] = value;
        DX8CALL(SetTextureStageState(stage, state, value));
        ++s_textureStageStateChanges;
    } else {
        DX8CALL(SetTextureStageState(stage, state, value));
    }
}

inline void DX8Wrapper::Set_DX8_Texture(unsigned stage, w3dbasetexture_t texture)
{
    if (stage >= MAX_TEXTURE_STAGES) {
        DX8CALL(SetTexture(stage, nullptr));
        return;
    }

    if (s_textures[stage] == texture) {
        return;
    }

    if (s_textures[stage] != W3D_TYPE_INVALID_TEXTURE) {
        s_textures[stage]->Release();
    }

    s_textures[stage] = texture;
    s_textures[stage]->AddRef();
    DX8CALL(SetTexture(stage, texture));
    ++s_textureChanges;
}

inline void DX8Wrapper::Set_Transform(D3DTRANSFORMSTATETYPE transform, const Matrix4 &m)
{
    switch (transform) {
        case D3DTS_WORLD:
            s_renderState.world = m.Transpose();
            s_renderStateChanged |= WORLD_CHANGED;
            s_renderStateChanged &= ~WORLD_IDENTITY;
            break;
        case D3DTS_VIEW:
            s_renderState.view = m.Transpose();
            s_renderStateChanged |= VIEW_CHANGED;
            s_renderStateChanged &= ~VIEW_IDENTITY;
            break;
        case D3DTS_PROJECTION: {
            Matrix4 ProjectionMatrix = m.Transpose();
            s_zFar = 0.0f;
            s_zNear = 0.0f;
            DX8CALL(SetTransform(D3DTS_PROJECTION, (D3DMATRIX *)&ProjectionMatrix));
        } break;
        default:
            s_matrixChanges++;
            Matrix4 m2 = m.Transpose();
            DX8CALL(SetTransform(transform, (D3DMATRIX *)&m2));
            break;
    }
}

inline void DX8Wrapper::Get_Transform(D3DTRANSFORMSTATETYPE transform, Matrix4 &m)
{
    D3DMATRIX mat;

    switch (transform) {
        case D3DTS_WORLD:
            if (s_renderStateChanged & WORLD_IDENTITY)
                m.Make_Identity();
            else
                m = s_renderState.world.Transpose();
            break;
        case D3DTS_VIEW:
            if (s_renderStateChanged & VIEW_IDENTITY)
                m.Make_Identity();
            else
                m = s_renderState.view.Transpose();
            break;
        default:
            DX8CALL(GetTransform(transform, &mat));
            m = *(Matrix4 *)&mat;
            m = m.Transpose();
            break;
    }
}
#endif

inline void DX8Wrapper::Handle_DX8_ErrorCode(unsigned error)
{
#ifdef BUILD_WITH_D3D8
    if (error != D3D_OK) {
        DX8Wrapper::Log_DX8_ErrorCode(error);
    }
#endif
}

inline void DX8Wrapper::Set_Texture(unsigned stage, TextureClass *texture)
{
    if (texture == s_renderState.Textures[stage])
        return;
    Ref_Ptr_Set(texture, s_renderState.Textures[stage]);
    s_renderStateChanged |= (TEXTURE0_CHANGED << stage);
}

inline void DX8Wrapper::Set_Material(const VertexMaterialClass *material)
{
    if (material == s_renderState.material)
        return;
    VertexMaterialClass *v = const_cast<VertexMaterialClass *>(material);
    Ref_Ptr_Set(v, s_renderState.material);
    s_renderStateChanged |= MATERIAL_CHANGED;
}

#ifdef BUILD_WITH_D3D8
inline Vector4 DX8Wrapper::Convert_Color(unsigned color)
{
    Vector4 col;
    col[3] = ((color & 0xff000000) >> 24) / 255.0f;
    col[0] = ((color & 0xff0000) >> 16) / 255.0f;
    col[1] = ((color & 0xff00) >> 8) / 255.0f;
    col[2] = ((color & 0xff) >> 0) / 255.0f;
    return col;
}

inline unsigned int DX8Wrapper::Convert_Color(const Vector3 &color, const float alpha)
{
    return D3DCOLOR_COLORVALUE(color.X, color.Y, color.Z, alpha);
}

inline unsigned int DX8Wrapper::Convert_Color(const Vector4 &color)
{
    return D3DCOLOR_COLORVALUE(color.X, color.Y, color.Z, color.W);
}

inline void DX8Wrapper::Set_Light(unsigned index, const D3DLIGHT8 *light)
{
    if (light) {
        s_renderState.Lights[index] = *light;
        s_renderState.LightEnable[index] = true;
    } else {
        s_renderState.LightEnable[index] = false;
    }
    s_renderStateChanged |= (LIGHT0_CHANGED << index);
}

inline void DX8Wrapper::Set_DX8_Light(int index, D3DLIGHT8 *light)
{
    if (light) {
        s_lightChanges++;
        DX8CALL(SetLight(index, light));
        DX8CALL(LightEnable(index, TRUE));
        s_currentLightEnables[index] = true;
    } else if (s_currentLightEnables[index]) {
        s_lightChanges++;
        s_currentLightEnables[index] = false;
        DX8CALL(LightEnable(index, FALSE));
    }
}

inline void DX8Wrapper::_Set_DX8_Transform(D3DTRANSFORMSTATETYPE transform, const Matrix4 &m)
{
    captainslog_assert(transform <= D3DTS_WORLD);
    s_DX8Transforms[transform] = m;
    s_matrixChanges++;
    DX8CALL(SetTransform(transform, (D3DMATRIX *)&m));
}
#endif
