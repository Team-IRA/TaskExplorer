#pragma once
#include <qobject.h>
#include "ProcessInfo.h"
#include "../Common/Common.h"

#ifndef PH_NO_NETWORK_PROTOCOL
#define PH_IPV4_NETWORK_TYPE 0x1
#define PH_IPV6_NETWORK_TYPE 0x2
#define PH_NETWORK_TYPE_MASK 0x3

#define PH_TCP_PROTOCOL_TYPE 0x10
#define PH_UDP_PROTOCOL_TYPE 0x20
#define PH_PROTOCOL_TYPE_MASK 0x30

#define PH_NO_NETWORK_PROTOCOL 0x0
#define PH_TCP4_NETWORK_PROTOCOL (PH_IPV4_NETWORK_TYPE | PH_TCP_PROTOCOL_TYPE)
#define PH_TCP6_NETWORK_PROTOCOL (PH_IPV6_NETWORK_TYPE | PH_TCP_PROTOCOL_TYPE)
#define PH_UDP4_NETWORK_PROTOCOL (PH_IPV4_NETWORK_TYPE | PH_UDP_PROTOCOL_TYPE)
#define PH_UDP6_NETWORK_PROTOCOL (PH_IPV6_NETWORK_TYPE | PH_UDP_PROTOCOL_TYPE)
#endif

class CSocketInfo: public QObject
{
	Q_OBJECT

public:
	CSocketInfo(QObject *parent = nullptr);
	virtual ~CSocketInfo();

	virtual quint64			GetHashID()			{ QReadLocker Locker(&m_Mutex); return m_HashID; }

	virtual ulong				GetProtocolType()	{ QReadLocker Locker(&m_Mutex); return m_ProtocolType; }
	virtual QString				GetProtocolString();
	virtual QHostAddress		GetLocalAddress()	{ QReadLocker Locker(&m_Mutex); return m_LocalAddress; }
	virtual quint16				GetLocalPort()		{ QReadLocker Locker(&m_Mutex); return m_LocalPort; }
	virtual QHostAddress		GetRemoteAddress()	{ QReadLocker Locker(&m_Mutex); return m_RemoteAddress; }
	virtual quint16				GetRemotePort()		{ QReadLocker Locker(&m_Mutex); return m_RemotePort; }
	virtual ulong				GetState()			{ QReadLocker Locker(&m_Mutex); return m_State; }
	virtual QString				GetStateString();
	virtual quint64				GetProcessId()		{ QReadLocker Locker(&m_Mutex); return m_ProcessId; }
    virtual QDateTime			GetCreateTime()		{ QReadLocker Locker(&m_Mutex); return m_CreateTime; }

	virtual QString				GetProcessName()	{ QReadLocker Locker(&m_Mutex); return m_ProcessName; }
	virtual CProcessPtr			GetProcess()		{ QReadLocker Locker(&m_Mutex); return m_pProcess; }

	virtual SSockStats			GetStats()			{ QReadLocker Locker(&m_StatsMutex); return m_Stats; }

	virtual void				MarkForRemoval()			{ QWriteLocker Locker(&m_StatsMutex); m_RemoveTimeStamp = GetCurTick(); }
	virtual bool				IsMarkedForRemoval() const	{ QReadLocker Locker(&m_StatsMutex); return m_RemoveTimeStamp != 0; }
	virtual bool				CanBeRemoved() const		{ QReadLocker Locker(&m_StatsMutex); return m_RemoveTimeStamp != 0 ? GetCurTick() - m_RemoveTimeStamp > 3000 : false; } // todo customize

	virtual bool				Match(quint64 ProcessId, ulong ProtocolType, const QHostAddress& LocalAddress, quint16 LocalPort, const QHostAddress& RemoteAddress, quint16 RemotePort, bool bStrict);

	static quint64				MkHash(quint64 ProcessId, ulong ProtocolType, const QHostAddress& LocalAddress, quint16 LocalPort, const QHostAddress& RemoteAddress, quint16 RemotePort);

protected:
	virtual void				UpdateStats();

	quint64						m_HashID;

	ulong						m_ProtocolType;
	QHostAddress				m_LocalAddress;
	quint16						m_LocalPort;
	QHostAddress				m_RemoteAddress;
	quint16						m_RemotePort;
	ulong						m_State;
	quint64						m_ProcessId;
    QDateTime					m_CreateTime;

	QString						m_ProcessName;
	CProcessRef					m_pProcess;

	quint64						m_RemoveTimeStamp;


	// I/O stats
	mutable QReadWriteLock		m_StatsMutex;
	SSockStats					m_Stats;

	mutable QReadWriteLock		m_Mutex;
};

typedef QSharedPointer<CSocketInfo> CSocketPtr;
typedef QWeakPointer<CSocketInfo> CSocketRef;