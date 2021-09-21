#pragma once

struct tHackEventDataOpenFile : public tHackEventData
{
	const char* m_pszPath;
	const char* m_pszMode;

	Galactic3D::Strong<Galactic3D::Stream> m_pStream;
};
