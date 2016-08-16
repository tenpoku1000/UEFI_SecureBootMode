
// Copyright 2015 Shin'ichi Ichikawa. Released under the MIT license.

#include <efi.h>
#include <efilib.h>
#include "efi_status.h"

#define true 1
#define false 0

typedef UINT8 bool;

static void init(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE* system_table);
static void read_key(void);
static void reset_system(EFI_STATUS status);
static void error_print(CHAR16* msg, EFI_STATUS* status);
static bool is_setup_mode(UINT8 pk, UINT8 audit_mode, UINT8 deployed_mode, UINT8 setup_mode, UINT8 secure_boot);
static bool is_user_mode(UINT8 pk, UINT8 audit_mode, UINT8 deployed_mode, UINT8 setup_mode);
static bool is_audit_mode(UINT8 pk, UINT8 audit_mode, UINT8 deployed_mode, UINT8 setup_mode, UINT8 secure_boot);
static bool is_deployed_mode(UINT8 pk, UINT8 audit_mode, UINT8 deployed_mode, UINT8 setup_mode);
static EFI_STATUS get_PK(UINT8* pk);
static EFI_STATUS get_variable(CHAR16* variable_name, UINT8* data);
static void get_pk_and_mode_variables(void);

EFI_STATUS efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE* system_table)
{
    init(image_handle, system_table);

    Print(L"When you press any key, the system will reboot.\n");
    Print(L"\n");

    get_pk_and_mode_variables();

    reset_system(EFI_SUCCESS);

    return EFI_SUCCESS;
}

static void init(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE* system_table)
{
    InitializeLib(image_handle, system_table);

    EFI_STATUS status = EFI_SUCCESS;

    if ((NULL == ST->ConIn) || (EFI_SUCCESS != (status = ST->ConIn->Reset(ST->ConIn, 0)))){

        error_print(L"Input device unavailable.\n", ST->ConIn ? &status : NULL);
    }
}

static void read_key(void)
{
    if (ST->ConIn){

        EFI_STATUS local_status = EFI_SUCCESS;

        do{
            EFI_INPUT_KEY key;

            local_status = ST->ConIn->ReadKeyStroke(ST->ConIn, &key);

        } while (EFI_SUCCESS != local_status);
    }
}

static void reset_system(EFI_STATUS status)
{
    read_key();

    RT->ResetSystem(EfiResetCold, status, 0, NULL);
}

static void error_print(CHAR16* msg, EFI_STATUS* status)
{
    Print(msg);

    if (status){

        Print(L"EFI_STATUS = %d, %s\n", *status, print_status_msg(*status));
    }

    reset_system(EFI_SUCCESS);
}

static bool is_setup_mode(
    UINT8 pk, UINT8 audit_mode, UINT8 deployed_mode, UINT8 setup_mode, UINT8 secure_boot)
{
    if ((0 == pk) &&
        (0 == audit_mode) && (0 == deployed_mode) && (1 == setup_mode) && (0 == secure_boot)){

        return true;
    }

    return false;
}

static bool is_user_mode(UINT8 pk, UINT8 audit_mode, UINT8 deployed_mode, UINT8 setup_mode)
{
    if ((0 != pk) &&
        (0 == audit_mode) && (0 == deployed_mode) && (0 == setup_mode)){

        return true;
    }

    return false;
}

static bool is_audit_mode(
    UINT8 pk, UINT8 audit_mode, UINT8 deployed_mode, UINT8 setup_mode, UINT8 secure_boot)
{
    if ((0 == pk) &&
        (1 == audit_mode) && (0 == deployed_mode) && (0 == setup_mode) && (0 == secure_boot)){

        return true;
    }

    return false;
}

static bool is_deployed_mode(UINT8 pk, UINT8 audit_mode, UINT8 deployed_mode, UINT8 setup_mode)
{
    if ((0 != pk) &&
        (0 == audit_mode) && (1 == deployed_mode) && (0 == setup_mode)){

        return true;
    }

    return false;
}

static EFI_STATUS get_PK(UINT8* pk)
{
    UINTN data_size = sizeof(UINT8);

    EFI_STATUS status = RT->GetVariable(L"PK", &EfiGlobalVariable, NULL, &data_size, pk);

    switch (status){
    case EFI_NOT_FOUND:
        return EFI_SUCCESS;
    case EFI_BUFFER_TOO_SMALL:
//        break;
    case EFI_SUCCESS:
        *pk = 1;
        return EFI_SUCCESS;
    default:
        break;
    }

    return status;
}

static EFI_STATUS get_variable(CHAR16* variable_name, UINT8* data)
{
    UINTN data_size = sizeof(UINT8);

    EFI_STATUS status = RT->GetVariable(variable_name, &EfiGlobalVariable, NULL, &data_size, data);

    switch (status){
    case EFI_NOT_FOUND:
//        break;
    case EFI_SUCCESS:
        return EFI_SUCCESS;
    default:
        break;
    }

    return status;
}

static void get_pk_and_mode_variables(void)
{
    UINT8 pk = 0;

    EFI_STATUS status = get_PK(&pk);

    if (EFI_ERROR(status)){

        error_print(L"get_PK() failed.\n", &status);
    }

    UINT8 audit_mode = 0;

    status = get_variable(L"AuditMode", &audit_mode);

    if (EFI_ERROR(status)){

        error_print(L"get_variable(AuditMode) failed.\n", &status);
    }

    UINT8 deployed_mode = 0;

    status = get_variable(L"DeployedMode", &deployed_mode);

    if (EFI_ERROR(status)){

        error_print(L"get_variable(DeployedMode) failed.\n", &status);
    }

    UINT8 setup_mode = 0;

    status = get_variable(L"SetupMode", &setup_mode);

    if (EFI_ERROR(status)){

        error_print(L"get_variable(SetupMode) failed.\n", &status);
    }

    UINT8 secure_boot = 0;

    status = get_variable(L"SecureBoot", &secure_boot);

    if (EFI_ERROR(status)){

        error_print(L"get_variable(SecureBoot) failed.\n", &status);
    }

    Print(
        L"%s, audit_mode(%d), deployed_mode(%d), setup_mode(%d), secure_boot(%d)\n",
        (0 == pk) ? L"PK == NULL" : L"PK != NULL", audit_mode, deployed_mode, setup_mode, secure_boot
    );
    Print(L"\n");

    if (is_audit_mode(pk, audit_mode, deployed_mode, setup_mode, secure_boot)){

        Print(L"The system is AuditMode.\n");

    }else if (is_deployed_mode(pk, audit_mode, deployed_mode, setup_mode)){

        Print(L"The system is DeployedMode.\n");

    }else if (is_setup_mode(pk, audit_mode, deployed_mode, setup_mode, secure_boot)){

        Print(L"The system is SetupMode.\n");

    }else if (is_user_mode(pk, audit_mode, deployed_mode, setup_mode)){

        Print(L"The system is UserMode.\n");

    }else{

        Print(L"The system is unknown mode.\n");
    }
}

