#ifndef _PH_LSASUP_H
#define _PH_LSASUP_H

#ifdef __cplusplus
extern "C" {
#endif

PHLIBAPI
NTSTATUS
NTAPI
PhOpenLsaPolicy(
    _Out_ PLSA_HANDLE PolicyHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_opt_ PUNICODE_STRING SystemName
    );

PHLIBAPI
LSA_HANDLE
NTAPI
PhGetLookupPolicyHandle(
    VOID
    );

PHLIBAPI
BOOLEAN
NTAPI
PhLookupPrivilegeName(
    _In_ PLUID PrivilegeValue,
    _Out_ PPH_STRING *PrivilegeName
    );

PHLIBAPI
BOOLEAN
NTAPI
PhLookupPrivilegeDisplayName(
    _In_ PPH_STRINGREF PrivilegeName,
    _Out_ PPH_STRING *PrivilegeDisplayName
    );

PHLIBAPI
BOOLEAN
NTAPI
PhLookupPrivilegeValue(
    _In_ PPH_STRINGREF PrivilegeName,
    _Out_ PLUID PrivilegeValue
    );

PHLIBAPI
NTSTATUS
NTAPI
PhLookupSid(
    _In_ PSID Sid,
    _Out_opt_ PPH_STRING *Name,
    _Out_opt_ PPH_STRING *DomainName,
    _Out_opt_ PSID_NAME_USE NameUse
    );

PHLIBAPI
VOID
NTAPI
PhLookupSids(
    _In_ ULONG Count,
    _In_ PSID *Sids,
    _Out_ PPH_STRING **FullNames
    );

PHLIBAPI
NTSTATUS
NTAPI
PhLookupName(
    _In_ PPH_STRINGREF Name,
    _Out_opt_ PSID *Sid,
    _Out_opt_ PPH_STRING *DomainName,
    _Out_opt_ PSID_NAME_USE NameUse
    );

PHLIBAPI
PPH_STRING
NTAPI
PhGetSidFullName(
    _In_ PSID Sid,
    _In_ BOOLEAN IncludeDomain,
    _Out_opt_ PSID_NAME_USE NameUse
    );

PHLIBAPI
PPH_STRING
NTAPI
PhSidToStringSid(
    _In_ PSID Sid
    );

PHLIBAPI
PPH_STRING
NTAPI
PhGetTokenUserString(
    _In_ HANDLE TokenHandle,
    _In_ BOOLEAN IncludeDomain
    );

PHLIBAPI
PPH_STRING
NTAPI
PhGetCapabilitySidName(
    _In_ PSID CapabilitySid
    );

PHLIBAPI
PPH_STRING
NTAPI
PhGetCapabilityGuidName(
    _In_ PPH_STRING GuidString
    );

#ifdef __cplusplus
}
#endif

#endif
