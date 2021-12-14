#pragma once

class CMafiaCSayCommandHandlerII : public CCommandHandler
{
public:
	virtual void Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) override;
};

class CMafiaCUnhandledCommandHandlerII : public CUnhandledCommandHandler
{
public:
	virtual void Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) override;
};

class CSetNameCommandHandlerII : public CCommandHandler
{
public:
	virtual void Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) override;
};

class CConnectCommandHandlerII : public CCommandHandler
{
public:
	virtual void Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) override;
};

class CReconnectCommandHandlerII : public CCommandHandler
{
public:
	virtual void Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) override;
};

class CDisconnectCommandHandlerII : public CCommandHandler
{
public:
	virtual void Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) override;
};

class CMafiaCExitCommandHandlerII : public CCommandHandler
{
public:
	virtual void Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) override;
};

class CConsoleCommandHandlerII : public CCommandHandler
{
public:
	virtual void Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) override;
};

class CGameStatsCommandHandlerII : public CCommandHandler
{
public:
	virtual void Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) override;
};

class CChatScaleCommandHandlerII : public CCommandHandler
{
public:
	virtual void Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) override;
};