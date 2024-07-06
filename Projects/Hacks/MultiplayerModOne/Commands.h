#pragma once

class CMafiaCSayCommandHandler : public CCommandHandler
{
public:
	virtual void Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) override;
};

class CMafiaCUnhandledCommandHandler : public CUnhandledCommandHandler
{
public:
	virtual void Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) override;
};

class CSetNameCommandHandler : public CCommandHandler
{
public:
	virtual void Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) override;
};

class CConnectCommandHandler : public CCommandHandler
{
public:
	virtual void Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) override;
};

class CReconnectCommandHandler : public CCommandHandler
{
public:
	virtual void Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) override;
};

class CDisconnectCommandHandler : public CCommandHandler
{
public:
	virtual void Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) override;
};

class CMafiaCExitCommandHandler : public CCommandHandler
{
public:
	virtual void Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) override;
};

class CConsoleCommandHandler : public CCommandHandler
{
public:
	virtual void Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) override;
};

class CGameStatsCommandHandler : public CCommandHandler
{
public:
	virtual void Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) override;
};

class CChatScaleCommandHandler : public CCommandHandler
{
public:
	virtual void Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) override;
};