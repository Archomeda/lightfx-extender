//! \file RzChromaSDKDefines.h
//! \brief Definitions of global and static variables.

#ifndef _RZSCHROMADKDEFINES_H_
#define _RZSCHROMADKDEFINES_H_

#pragma once

#ifndef GUID_DEFINED
#include <Guiddef.h>
#endif

namespace ChromaSDK
{
    // Keyboards
    //! Razer Blackwidow Chroma device.
    // {2EA1BB63-CA28-428D-9F06-196B88330BBB}
    static const GUID BLACKWIDOW_CHROMA = 
    { 0x2ea1bb63, 0xca28, 0x428d, { 0x9f, 0x06, 0x19, 0x6b, 0x88, 0x33, 0x0b, 0xbb } }; 

    //! Razer Blackwidow Chroma Tournament Edition device.
    // {ED1C1B82-BFBE-418F-B49D-D03F05B149DF}
    static const GUID BLACKWIDOW_CHROMA_TE = 
    { 0xed1c1b82, 0xbfbe, 0x418f, { 0xb4, 0x9d, 0xd0, 0x3f, 0x5, 0xb1, 0x49, 0xdf } };

    //! Razer Deathstalker device.
    // {18C5AD9B-4326-4828-92C4-2669A66D2283}
    static const GUID DEATHSTALKER_CHROMA = 
    { 0x18c5ad9b, 0x4326, 0x4828, { 0x92, 0xc4, 0x26, 0x69, 0xa6, 0x6d, 0x22, 0x83 } };

    //! Overwatch Keyboard.
    // {872AB2A9-7959-4478-9FED-15F6186E72E4}
    static const GUID OVERWATCH_KEYBOARD = 
    { 0x872ab2a9, 0x7959, 0x4478, { 0x9f, 0xed, 0x15, 0xf6, 0x18, 0x6e, 0x72, 0xe4 } };

    // Mice
    //! Razer Deathadder Chroma device.
    // {AEC50D91-B1F1-452F-8E16-7B73F376FDF3}
    static const GUID DEATHADDER_CHROMA = 
    { 0xaec50d91, 0xb1f1, 0x452f, { 0x8e, 0x16, 0x7b, 0x73, 0xf3, 0x76, 0xfd, 0xf3 } };

    //! Razer Mamba Chroma Tournament Edition device.
    // {7EC00450-E0EE-4289-89D5-0D879C19061A}
    static const GUID MAMBA_CHROMA_TE = 
    { 0x7ec00450, 0xe0ee, 0x4289, { 0x89, 0xd5, 0xd, 0x87, 0x9c, 0x19, 0x6, 0x1a } };

    //! Razer Diamondback device.
    // {FF8A5929-4512-4257-8D59-C647BF9935D0}
    static const GUID DIAMONDBACK_CHROMA = 
    { 0xff8a5929, 0x4512, 0x4257, { 0x8d, 0x59, 0xc6, 0x47, 0xbf, 0x99, 0x35, 0xd0 } };

    //! Razer Mamba device.
    // {D527CBDC-EB0A-483A-9E89-66D50463EC6C}
    static const GUID MAMBA_CHROMA = 
    { 0xd527cbdc, 0xeb0a, 0x483a, { 0x9e, 0x89, 0x66, 0xd5, 0x4, 0x63, 0xec, 0x6c } };

    //! Razer Naga Epic device.
    // {D714C50B-7158-4368-B99C-601ACB985E98}
    static const GUID NAGA_EPIC_CHROMA = 
    { 0xd714c50b, 0x7158, 0x4368, { 0xb9, 0x9c, 0x60, 0x1a, 0xcb, 0x98, 0x5e, 0x98 } };

    //! Razer Orochi Chroma device.
    // {52C15681-4ECE-4DD9-8A52-A1418459EB34}
    static const GUID OROCHI_CHROMA = 
    { 0x52c15681, 0x4ece, 0x4dd9, { 0x8a, 0x52, 0xa1, 0x41, 0x84, 0x59, 0xeb, 0x34 } };

    // Headsets
    //! Razer Kraken 7.1 Chroma device.
    // {CD1E09A5-D5E6-4A6C-A93B-E6D9BF1D2092}
    static const GUID KRAKEN71_CHROMA = 
    { 0xcd1e09a5, 0xd5e6, 0x4a6c, { 0xa9, 0x3b, 0xe6, 0xd9, 0xbf, 0x1d, 0x20, 0x92 } };

    // Mouse mat
    //! Razer Firefly device.
    // {80F95A94-73D2-48CA-AE9A-0986789A9AF2}
    static const GUID FIREFLY_CHROMA = 
    { 0x80f95a94, 0x73d2, 0x48ca, { 0xae, 0x9a, 0x9, 0x86, 0x78, 0x9a, 0x9a, 0xf2 } };

    // Keypads
    //! Razer Tartarus device.
    // {00F0545C-E180-4AD1-8E8A-419061CE505E}
    static const GUID TARTARUS_CHROMA = 
    { 0xf0545c, 0xe180, 0x4ad1, { 0x8e, 0x8a, 0x41, 0x90, 0x61, 0xce, 0x50, 0x5e } };

    //! Razer Orbweaver device.
    // {9D24B0AB-0162-466C-9640-7A924AA4D9FD}
    static const GUID ORBWEAVER_CHROMA = 
    { 0x9d24b0ab, 0x162, 0x466c, { 0x96, 0x40, 0x7a, 0x92, 0x4a, 0xa4, 0xd9, 0xfd } };

}

#endif
