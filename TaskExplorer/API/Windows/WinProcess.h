#pragma once
#include "../ProcessInfo.h"
#include "WinJob.h"
#include "WinToken.h"
#include "WinGDI.h"


class CWinProcess : public CProcessInfo
{
	Q_OBJECT

public:
	CWinProcess(QObject *parent = nullptr);
	virtual ~CWinProcess();

	virtual bool ValidateParent(CProcessInfo* pParent) const;

	virtual bool IsFullyInitialized() const			{ QReadLocker Locker(&m_Mutex); return m_IsFullyInitialized; }

	// Basic
	virtual void* GetQueryHandle() const;
	virtual bool IsWoW64() const;
	virtual QString GetArchString() const;
	virtual quint64 GetSessionID() const;
	virtual CWinTokenPtr GetToken() const			{ QReadLocker Locker(&m_Mutex); return m_pToken; }
	virtual quint16 GetSubsystem() const;
	virtual QString GetSubsystemString() const;
	virtual quint64 GetRawCreateTime() const;
	virtual QString GetWorkingDirectory() const;
	//virtual QString GetAppDataDirectory() const;

	// Flags
	virtual bool IsSubsystemProcess() const;

	// Dynamic
	virtual quint64 GetPagedPool() const			{ QReadLocker Locker(&m_Mutex); return m_QuotaPagedPoolUsage; }
	virtual quint64 GetPeakPagedPool() const		{ QReadLocker Locker(&m_Mutex); return m_QuotaPeakPagedPoolUsage; }
	virtual quint64 GetNonPagedPool() const			{ QReadLocker Locker(&m_Mutex); return m_QuotaNonPagedPoolUsage; }
	virtual quint64 GetPeakNonPagedPool() const		{ QReadLocker Locker(&m_Mutex); return m_QuotaPeakNonPagedPoolUsage; }

	virtual quint64 GetSharedWorkingSetSize() const;
	virtual quint64 GetShareableWorkingSetSize() const;
	virtual quint64 GetMinimumWS() const;
	virtual quint64 GetMaximumWS() const;

	virtual quint32	GetPeakNumberOfHandles() const;
	virtual quint64 GetUpTime() const;
	virtual quint64 GetSuspendTime() const;
	virtual int		GetHangCount() const;
	virtual int		GetGhostCount() const;

	virtual QString GetPriorityString() const		{ return GetPriorityString(GetPriority()); }
	virtual QString GetBasePriorityString() const	{ return GetBasePriorityString(GetBasePriority()); }
	virtual QString GetPagePriorityString() const	{ return GetPagePriorityString(GetPagePriority()); }
	virtual QString GetIOPriorityString() const		{ return GetIOPriorityString(GetIOPriority()); }
	static QString GetPriorityString(quint32 value);
	static QString GetBasePriorityString(quint32 value);
	static QString GetPagePriorityString(quint32 value);
	static QString GetIOPriorityString(quint32 value);
	virtual STATUS SetPriority(long Value);
	virtual STATUS SetBasePriority(long Value)		{ return ERR(); }
	virtual STATUS SetPagePriority(long Value);
	virtual STATUS SetIOPriority(long Value);

	virtual STATUS SetAffinityMask(quint64 Value);

	virtual STATUS Terminate(bool bForce);

	virtual bool IsSuspended() const;
	virtual STATUS Suspend();
	virtual STATUS Resume();

	virtual void AddService(const QString& Name)	{ QWriteLocker Locker(&m_Mutex); if(!m_ServiceList.contains(Name)) m_ServiceList.append(Name); }
	virtual QStringList GetServiceList() const		{ QReadLocker Locker(&m_Mutex); return m_ServiceList; }

	// GDI, USER handles
	virtual quint32 GetGdiHandles() const			{ QReadLocker Locker(&m_Mutex); return m_GdiHandles; }
	virtual quint32 GetUserHandles() const			{ QReadLocker Locker(&m_Mutex); return m_UserHandles; }
	virtual void SetWndHandles(quint32 WndHandles)  { QWriteLocker Locker(&m_Mutex); m_WndHandles = WndHandles; m_pMainWnd.clear(); } // invalidate cached main window pointer
	virtual quint32 GetWndHandles() const			{ QReadLocker Locker(&m_Mutex); return m_WndHandles; }

	virtual QString GetWindowTitle() const;
	virtual QString GetWindowStatusString() const;

	// OS context
	virtual quint32 GetOsContextVersion() const;
	virtual QString GetOsContextString() const;

	virtual QString GetDEPStatusString() const;

	virtual QString GetCFGuardString() const;

	// Other Fields
	virtual QString GetUserName() const;
	virtual quint64 GetProcessSequenceNumber() const;

	virtual QMap<QString, SEnvVar>	GetEnvVariables() const;
	virtual STATUS					DeleteEnvVariable(const QString& Name) { return EditEnvVariable(Name, QString()); }
	virtual STATUS					EditEnvVariable(const QString& Name, const QString& Value);

	virtual QString GetStatusString() const;

	virtual bool HasDebugger() const;
	virtual STATUS AttachDebugger();
	virtual STATUS DetachDebugger();

	virtual bool IsSystemProcess() const;
	virtual bool IsServiceProcess() const;
	virtual bool IsUserProcess() const;
	virtual bool IsElevated() const;
	virtual bool TokenHasChanged() const;
	//virtual bool IsJobProcess() const;
	virtual bool IsInJob() const;
	virtual bool IsImmersiveProcess() const;
	virtual bool IsNetProcess() const;
	virtual quint64 GetConsoleHostId() const;

	virtual QString GetPackageName() const; 
	virtual QString GetAppID() const; 
	virtual quint32 GetDPIAwareness() const;
	virtual QString GetDPIAwarenessString() const;


	virtual quint64 GetJobObjectID() const;

	virtual quint8 GetProtection() const;
	virtual QString GetProtectionString() const;
	virtual STATUS SetProtectionFlag(quint8 Flag, bool bForce = false);
	virtual QList<QPair<QString, QString>> GetMitigationDetails() const;

	virtual QString GetUsedDesktop() const {QReadLocker Locker(&m_Mutex); return m_UsedDesktop;}

	virtual bool IsCriticalProcess() const;
	virtual STATUS SetCriticalProcess(bool bSet, bool bForce = false);
	virtual STATUS ReduceWS();

	virtual STATUS LoadModule(const QString& Path);

	virtual void OpenPermissions();

	virtual CWinJobPtr		GetJob() const;

	virtual QMap<quint64, CMemoryPtr> GetMemoryMap() const;

	virtual QList<CWndPtr> GetWindows() const;
	virtual CWndPtr	GetMainWindow() const;

	virtual void UpdateDns(const QString& HostName, const QList<QHostAddress>& Addresses);

	struct STask
	{
		QString Name;
		QString Path;
	};
	virtual QList<STask>	GetTasks() const;

	struct SDriver
	{
		QString Name;
		QString Path;
	};
	virtual QList<SDriver>	GetUmdfDrivers() const;

	struct SWmiProvider
	{
		QString ProviderName;
		QString NamespacePath;
		QString FileName;
		QString UserName;
	};
	virtual QList<SWmiProvider>	QueryWmiProviders() const;

public slots:
	virtual bool	UpdateThreads();
	virtual bool	UpdateHandles();
	virtual bool	UpdateModules();
	virtual bool	UpdateWindows();

	void			OnAsyncDataDone(bool IsPacked, quint32 ImportFunctions, quint32 ImportModules);

private slots:
	//bool UpdateDynamicDataExt();

protected:
	friend class CWindowsAPI;
	friend class CWinGpuMonitor;

	bool InitStaticData(quint64 ProcessId);
	bool InitStaticData(struct _SYSTEM_PROCESS_INFORMATION* process, bool bFullProcessInfo);
	bool InitStaticData(bool bLoadFileName = true); // *NOT Thread Safe* internal function to be called by other InitStaticData's
	bool UpdateDynamicData(struct _SYSTEM_PROCESS_INFORMATION* process, bool bFullProcessInfo, quint64 sysTotalTime);
	bool UpdateThreadData(struct _SYSTEM_PROCESS_INFORMATION* process, bool bFullProcessInfo, quint64 sysTotalTime, quint64 sysTotalCycleTime);
	bool UpdateTokenData(bool MonitorChange);
	void UpdateCPUCycles(quint64 sysTotalTime, quint64 sysTotalCycleTime);
	void UnInit();

	void AddNetworkIO(int Type, quint32 TransferSize);
	void AddDiskIO(int Type, quint32 TransferSize);

	// Other fields
	QList<QString>					m_ServiceList;

	// Dynamic
	quint64							m_QuotaPagedPoolUsage;
	quint64							m_QuotaPeakPagedPoolUsage;
	quint64							m_QuotaNonPagedPoolUsage;
	quint64							m_QuotaPeakNonPagedPoolUsage;

	// GDI, USER handles
    quint32							m_GdiHandles;
    quint32							m_UserHandles;
	quint32							m_WndHandles;

	bool							m_IsCritical;

	// Token
	CWinTokenPtr					m_pToken;

	// MainWindow
	CWndRef							m_pMainWnd;
	QString							m_UsedDesktop;

private:
	//volatile quint64 m_lastExtUpdate;
	//void	UpdateExtDataIfNeeded() const;
	void UpdateWsCounters() const;

	bool							m_IsFullyInitialized;
	quint64							m_LastUpdateThreads;
	quint64							m_LastUpdateHandles;

	struct SWinProcess* m;
};

QVariantList GetProcessUnloadedDlls(quint64 ProcessId);