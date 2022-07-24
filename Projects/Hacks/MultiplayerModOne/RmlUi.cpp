
#include "stdafx.h"
#include "RmlUi.h"
#if MAFIAC_RMLUI
#include <RmlUi/Debugger.h>
#include "RwRenderer.h"

extern CRw2D Rw2D;

CRmlRenderInterface::CRmlRenderInterface(Context* pContext) :
	CRmlBaseRenderInterface(pContext),
	m_bScissor(false)
{
}

void CRmlRenderInterface::RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture, const Rml::Vector2f& translation)
{
	static RwIm2DVertex rgVertices[10000];
	static RwImVertexIndex rgIndices[10000];

	if (num_vertices > ARRAY_COUNT(rgVertices) || num_indices > ARRAY_COUNT(rgIndices))
		return;

	float fZ = RwIm2DGetNearScreenZ();
	float fRHW = 1.0f / CGTA::GetCurrentRWCamera()->nearPlane;

	for (size_t i = 0; i < num_vertices; i++)
	{
		CVector3D::TransformCoordinate(CVector3D(vertices[i].position.x + translation.x, vertices[i].position.y + translation.y, fZ), m_matTransform, rgVertices[i].vec);
		rgVertices[i].vec.x -= 0.5f;
		rgVertices[i].vec.y -= 0.5f;
		rgVertices[i].rhw = fRHW;
		rgVertices[i].emissiveColor = CARGB(vertices[i].colour.alpha, vertices[i].colour.red, vertices[i].colour.green, vertices[i].colour.blue);
		rgVertices[i].u = vertices[i].tex_coord.x;
		rgVertices[i].v = vertices[i].tex_coord.y;
	}

	for (size_t i = 0; i < num_indices; i++)
	{
		rgIndices[i] = indices[i];
	}

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, FALSE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, FALSE);
	Texture* pTexture = (Texture*)texture;
	RwRaster* pRwRaster = nullptr;
	if (pTexture != nullptr)
		pRwRaster = Rw2D.GetRwRaster(pTexture);
	RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, rwFILTERLINEAR);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, rwBLENDINVSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, TRUE);
	RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, rwTEXTUREADDRESSCLAMP);
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (unsigned int)pRwRaster);
	RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, rgVertices, num_vertices, rgIndices, num_indices);
}

#include <d3d8.h>

void CRmlRenderInterface::EnableScissorRegion(bool enable)
{
	if (!enable)
	{
		m_vecScissorPos = Rml::Vector2i(0, 0);
		m_vecScissorSize = Rml::Vector2i(CGTA::GetWidth(), CGTA::GetHeight());
		m_bScissor = true;
	}

	UpdateScissor();
}

void CRmlRenderInterface::SetScissorRegion(int x, int y, int width, int height)
{
	m_vecScissorPos = Rml::Vector2i(x, y);
	m_vecScissorSize = Rml::Vector2i(width, height);
	m_bScissor = true;

	UpdateScissor();
}

void CRmlRenderInterface::UpdateScissor()
{
	if (m_bScissor)
	{
		auto pD3DDevice = CGTA::GetD3DDevice();
		D3DVIEWPORT8 Viewport;
		Viewport.X = m_vecScissorPos.x;
		Viewport.Y = m_vecScissorPos.y;
		Viewport.Width = m_vecScissorSize.x;
		Viewport.Height = m_vecScissorSize.y;
		pD3DDevice->SetViewport(&Viewport);
	}
}

CRmlUi2::CRmlUi2(Context* pContext) :
	CRmlUi(pContext),
	m_pDocument(nullptr)
{
	m_pRenderInterface = new CRmlRenderInterface(pContext);
}

CRmlUi2::~CRmlUi2()
{
	if (m_bInitialised)
	{
		if (m_pDocument != nullptr)
			m_pDocument->Close();
	}
}

struct ApplicationData {
	bool show_text = true;
	Rml::String animal = "dog";
} my_data;

bool CRmlUi2::Initialise()
{
	if (!CRmlUi::Initialise())
		return false;

#if 0
	struct FontFace {
		Rml::String filename;
		bool fallback_face;
	};
	FontFace font_faces[] = {
		{ "/Fonts/OpenSans-Regular.ttf",    false },
		{ "/Fonts/OpenSans-Italic.ttf",     false },
		{ "/Fonts/OpenSans-Bold.ttf",       false },
		{ "/Fonts/OpenSans-BoldItalic.ttf", false },
		{ "/Fonts/Pricedown.ttf", false },
		{ "/Fonts/NotoEmoji-Regular.ttf",    true  },
	};

	for (const FontFace& face : font_faces)
	{
		Rml::LoadFontFace(face.filename, face.fallback_face);
	}
#endif

	m_pContext->GetFileSystem()->Enumerate(_gstr("/Fonts"), [](const Galactic3D::CFileMgr::tDirectoryEntry& Entry, void* pUser) {
		auto pRmlUi = ((CRmlUi2*)pUser);
		GString Path = Entry.pszPath;
		PathUtil::AppendPath(Path, Entry.pszFileName);
		//auto pStream = Strong<Stream>::New(pRmlUi->m_pContext->GetFileSystem()->Open(Path.c_str(), false));
		//if (pStream != nullptr)
		{
			//_glogprintf(_gstr("Loading Font - %s\n"), Path.c_str());
			UTF8String Path2(false, Path.c_str(), Path.length());
			Rml::LoadFontFace(Rml::String(Path2), false);
		}
		return true;
	}, this);

	if (Rml::DataModelConstructor constructor = m_pRmlContext->CreateDataModel("animals"))
	{
		constructor.Bind("show_text", &my_data.show_text);
		constructor.Bind("animal", &my_data.animal);
	}

	Reload();

	Rml::Debugger::SetVisible(true);

	return true;
}

void CRmlUi2::Reload()
{
	if (m_bInitialised)
	{
		if (m_pDocument != nullptr)
			m_pDocument->Close();
		m_pDocument = nullptr;

		Rml::Factory::ClearStyleSheetCache();
		Rml::Factory::ClearTemplateCache();
	}

	m_pDocument = m_pRmlContext->LoadDocument("/Rml/Main.rml");
	if (m_pDocument)
	{
		//Rml::Element* element = m_pDocument->GetElementById("world");
		//element->SetInnerRML(reinterpret_cast<const char*>(u8"🌍😃"));
		//element->SetProperty("font-size", "1.5em");

		m_pDocument->Show();
	}
}
#endif
