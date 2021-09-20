/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2021 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_gpu_commands_gen11.h

@brief auto-generated file

DO NOT EDIT
*/

//////////////////////////////////////////////////////////////////////////
// Customizable Macros
#ifndef __CODEGEN_FILE_DIRECTIVES_OPEN
#define __CODEGEN_FILE_DIRECTIVES_OPEN __pragma(pack(push, 1))
#endif // !__CODEGEN_FILE_DIRECTIVES_OPEN

#ifndef __CODEGEN_FILE_DIRECTIVES_CLOSE
#define __CODEGEN_FILE_DIRECTIVES_CLOSE __pragma(pack(pop))
#endif // !__CODEGEN_FILE_DIRECTIVES_CLOSE

#ifndef __CODEGEN_NAMESPACE_OPEN
#define __CODEGEN_NAMESPACE_OPEN
#endif // !__CODEGEN_NAMESPACE_OPEN

#ifndef __CODEGEN_NAMESPACE_CLOSE
#define __CODEGEN_NAMESPACE_CLOSE
#endif // !__CODEGEN_NAMESPACE_CLOSE

#ifndef __CODEGEN_ACCESS_SPECIFIER_DEFINITION
#define __CODEGEN_ACCESS_SPECIFIER_DEFINITION
#endif // !__CODEGEN_ACCESS_SPECIFIER_DEFINITION

#ifndef __CODEGEN_ACCESS_SPECIFIER_METHODS
#define __CODEGEN_ACCESS_SPECIFIER_METHODS
#endif // !__CODEGEN_ACCESS_SPECIFIER_METHODS

#ifndef __CODEGEN_ATTRIBUTES_STRUCTURE
#define __CODEGEN_ATTRIBUTES_STRUCTURE
#endif // !__CODEGEN_ATTRIBUTES_STRUCTURE

#ifndef __CODEGEN_INLINE
#define __CODEGEN_INLINE __inline
#endif // !__CODEGEN_INLINE

#ifndef __CODEGEN_UINT16
#define __CODEGEN_UINT16 uint16_t
#endif // !__CODEGEN_UINT16

#ifndef __CODEGEN_UINT
#define __CODEGEN_UINT uint32_t
#endif // !__CODEGEN_UINT

#ifndef __CODEGEN_UINT32
#define __CODEGEN_UINT32 uint32_t
#endif // !__CODEGEN_UINT32

#ifndef __CODEGEN_UINT64
#define __CODEGEN_UINT64 uint64_t
#endif // !__CODEGEN_UINT64

#ifndef __CODEGEN_FLOAT32
#define __CODEGEN_FLOAT32 float
#endif // !__CODEGEN_FLOAT32

#ifndef __CODEGEN_BOOL
#define __CODEGEN_BOOL uint32_t
#endif // !__CODEGEN_BOOL

#ifndef __CODEGEN_GPUVA
#define __CODEGEN_GPUVA uint64_t
#endif // !__CODEGEN_GPUVA

#ifndef __CODEGEN_GPUSIZE
#define __CODEGEN_GPUSIZE uint64_t
#endif // !__CODEGEN_GPUSIZE

#ifndef __CODEGEN_DebugType
#define __CODEGEN_DebugType( type )  static __CODEGEN_INLINE void DebugStruct( type s )
#endif // !__CODEGEN_DebugType

#ifndef __CODEGEN_DebugAttributeEnum
#define __CODEGEN_DebugAttributeEnum( name, lo, hi, type )
#endif // !__CODEGEN_DebugAttributeEnum

#ifndef __CODEGEN_DebugAttributeUInt
#define __CODEGEN_DebugAttributeUInt( name, lo, hi )
#endif // !__CODEGEN_DebugAttributeUInt

#ifndef __CODEGEN_DebugAttributeUInt64
#define __CODEGEN_DebugAttributeUInt64( name, lo, hi )
#endif // !__CODEGEN_DebugAttributeUInt64

#ifndef __CODEGEN_DebugAttributeBool
#define __CODEGEN_DebugAttributeBool( name, lo, hi )
#endif // !__CODEGEN_DebugAttributeBool

#ifndef __CODEGEN_DebugAttributeFloat
#define __CODEGEN_DebugAttributeFloat( name )
#endif // !__CODEGEN_DebugAttributeFloat

#ifndef __CODEGEN_DebugAttributeStruct
#define __CODEGEN_DebugAttributeStruct( name )
#endif // !__CODEGEN_DebugAttributeStruct

#ifndef __CODEGEN_DebugAttributeArray
#define __CODEGEN_DebugAttributeArray( name )
#endif // !__CODEGEN_DebugAttributeArray

#ifndef __CODEGEN_DebugEnum
#define __CODEGEN_DebugEnum( type ) static __CODEGEN_INLINE void DebugEnum( type e )
#endif // !__CODEGEN_DebugEnum

#ifndef __CODEGEN_DebugEnumValue
#define __CODEGEN_DebugEnumValue( name )
#endif // !__CODEGEN_DebugEnumValue

#ifndef __CODEGEN_GET_MACRO
#define __CODEGEN_GET_MACRO()
#endif // !__CODEGEN_GET_MACRO

#ifndef __CODEGEN_SET_MACRO
#define __CODEGEN_SET_MACRO( name )
#endif // !__CODEGEN_SET_MACRO

#ifndef __CODEGEN_C_ASSERT
#define __CODEGEN_C_ASSERT( x )
#endif // !__CODEGEN_C_ASSERT

#ifndef __CODEGEN_ASSERT
#define __CODEGEN_ASSERT( x )
#endif // !__CODEGEN_ASSERT

//////////////////////////////////////////////////////////////////////////
// Internal Macros
#define __CODEGEN_BITFIELD(l,h) h-l+1
#define __CODEGEN_OP_LENGTH_BIAS 2
#define __CODEGEN_OP_LENGTH( x ) (((x) / sizeof(__CODEGEN_UINT32)) - __CODEGEN_OP_LENGTH_BIAS)
#define __CODEGEN_OP_SIZE( x ) (((x) + __CODEGEN_OP_LENGTH_BIAS) * sizeof(__CODEGEN_UINT32))

//////////////////////////////////////////////////////////////////////////
__CODEGEN_FILE_DIRECTIVES_OPEN
__CODEGEN_NAMESPACE_OPEN

//////////////////////////////////////////////////////////////////////////
/// @brief MI_REPORT_PERF_COUNT
/// @details
///     The MI_REPORT_PERF_COUNT command causes the GFX hardware to write out a
///     snap-shot of performance counters to the address specified in this
///     command along with constant ID field supplied and the time-stamp
///     counter. This write is required to be treated as a cacheable write
///     irrespective of GTT entry memory type.  This command is specific to the
///     render engine.
///     
///     This command is to be used for performance debug mode and can be
///     inserted after events of interest (frequently before and after a
///     3DPRIMITIVE command). SW is entirely responsible for managing the ID
///     field and addresses used by such a series of commands.
///     
///     GTT_SELECT must not be set to 1 (i.e. GGTT) when MI_REPORT_PERF_COUNT
///     command is programmed in a non-privileged batch buffer. Refer to the
///     "User Mode Privileged commands" Table in MI_BATCH_BUFFER_START command
///     section for more details. All batch buffers in PPGTT are considered as
///     Non-privileged.
///     
struct MI_REPORT_PERF_COUNT
{
    __CODEGEN_ACCESS_SPECIFIER_DEFINITION
    union tagTheStructure
    {
        struct tagCommon
        {
            /// DWORD 0
            __CODEGEN_UINT32         DWordLength                                      : __CODEGEN_BITFIELD( 0,  5)    ; ///< U6
            __CODEGEN_UINT32         Reserved_6                                       : __CODEGEN_BITFIELD( 6, 22)    ; ///< U17
            __CODEGEN_UINT32         MICommandOpcode                                  : __CODEGEN_BITFIELD(23, 28)    ; ///< U6
            __CODEGEN_UINT32         CommandType                                      : __CODEGEN_BITFIELD(29, 31)    ; ///< U3

            /// DWORD 1..2
            __CODEGEN_UINT64         UseGlobalGTT                                     : __CODEGEN_BITFIELD( 0,  0)    ; ///< U1
            __CODEGEN_UINT64         Reserved_33                                      : __CODEGEN_BITFIELD( 1,  3)    ; ///< U3
            __CODEGEN_UINT64         CoreModeEnable                                   : __CODEGEN_BITFIELD( 4,  4)    ; ///< U1
            __CODEGEN_UINT64         Reserved_37                                      : __CODEGEN_BITFIELD( 5,  5)    ; ///< U1
            __CODEGEN_UINT64         MemoryAddress                                    : __CODEGEN_BITFIELD( 6, 63)    ; ///< U58

            /// DWORD 3
            __CODEGEN_UINT32         ReportID                                                                         ; ///< U32

        } Common;
        __CODEGEN_UINT32            RawData[ 4 ];
    } TheStructure;

    __CODEGEN_ACCESS_SPECIFIER_METHODS
    //////////////////////////////////////////////////////////////////////////
    /// @name STRUCTURE DEBUG
    /// @{

    /// @brief User-defined debug function for structure
    __CODEGEN_DebugType( MI_REPORT_PERF_COUNT )
    {
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.DWordLength                                 ,  0,  5, DWORD_LENGTH );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.MICommandOpcode                             , 23, 28, MI_COMMAND_OPCODE );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.CommandType                                 , 29, 31, COMMAND_TYPE );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.UseGlobalGTT                                ,  0,  0 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.CoreModeEnable                              ,  4,  4 );
        __CODEGEN_DebugAttributeUInt64( TheStructure.Common.MemoryAddress                               ,  6, 63 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.ReportID                                    ,  0, 31 );
    }
    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name LOCAL ENUMERATIONS
    /// @{

    /// @brief U6
    typedef enum tagDWORD_LENGTH
    {
        DWORD_LENGTH_EXCLUDES_DWORD_0_1                                  = 2, ///< 
    } DWORD_LENGTH;

    /// @brief U6
    typedef enum tagMI_COMMAND_OPCODE
    {
        MI_COMMAND_OPCODE_MI_REPORT_PERF_COUNT                           = 40, ///< 
    } MI_COMMAND_OPCODE;

    /// @brief U3
    typedef enum tagCOMMAND_TYPE
    {
        COMMAND_TYPE_MI_COMMAND                                          = 0, ///< 
    } COMMAND_TYPE;

    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name ENUMERATION DEBUG
    /// @{

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( DWORD_LENGTH )
    {
        __CODEGEN_DebugEnumValue( DWORD_LENGTH_EXCLUDES_DWORD_0_1 );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( MI_COMMAND_OPCODE )
    {
        __CODEGEN_DebugEnumValue( MI_COMMAND_OPCODE_MI_REPORT_PERF_COUNT );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( COMMAND_TYPE )
    {
        __CODEGEN_DebugEnumValue( COMMAND_TYPE_MI_COMMAND );
    }

    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name INITIALIZATION
    /// @{

    /// @brief Explicit member initialization function
    __CODEGEN_INLINE void Init( void )
    {
        TheStructure.RawData[ 0 ]                                    = 0x14000002;
        //TheStructure.Common.DWordLength                            = DWORD_LENGTH_EXCLUDES_DWORD_0_1;
        //TheStructure.Common.MICommandOpcode                        = MI_COMMAND_OPCODE_MI_REPORT_PERF_COUNT;
        //TheStructure.Common.CommandType                            = COMMAND_TYPE_MI_COMMAND;
        TheStructure.RawData[ 1 ]                                    = 0x0;
        TheStructure.RawData[ 2 ]                                    = 0x0;
        //TheStructure.Common.UseGlobalGTT                           = 0;
        //TheStructure.Common.CoreModeEnable                         = 0x0;
        //TheStructure.Common.MemoryAddress                          = 0x0;
        TheStructure.RawData[ 3 ]                                    = 0x0;
        //TheStructure.Common.ReportID                               = 0x0;
    }

    /// @brief Explicit static initialization function
    static MI_REPORT_PERF_COUNT sInit( void ) 
    {
        MI_REPORT_PERF_COUNT state;
        state.Init();
        return state;
    }
    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name ACCESSORS
    /// @{

    /// @brief Raw Data Access
    __CODEGEN_INLINE __CODEGEN_UINT32& GetRawData( const __CODEGEN_UINT index )
    {
        __CODEGEN_ASSERT( index < 4 );
        return TheStructure.RawData[ index ];
    }

    /// @brief Write UseGlobalGTT
    /// @details 
    ///     This field when set ( i.e. bit = 1) selects the GGTT for address
    ///     translation. When this bit is 0 ( default value), HW should use PGTT for
    ///     address translation.
    ///     

    __CODEGEN_INLINE void SetUseGlobalGTT( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.UseGlobalGTT = value;
    }

    /// @brief Read UseGlobalGTT
    __CODEGEN_INLINE __CODEGEN_BOOL GetUseGlobalGTT( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.UseGlobalGTT );
    }

    /// @brief Write CoreModeEnable
    /// @details 
    ///     This bit is set then the address will be offset by the Core ID:If Core
    ///     ID 0, then there is no offsetIf Core ID 1, then the Memory is offset by
    ///     the size of the data(64b).
    ///     

    __CODEGEN_INLINE void SetCoreModeEnable( const __CODEGEN_UINT64 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.CoreModeEnable = value;
    }

    /// @brief Read CoreModeEnable
    __CODEGEN_INLINE __CODEGEN_UINT64 GetCoreModeEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.CoreModeEnable );
    }

    /// @brief Write MemoryAddress
    /// @details 
    ///     This field specifies 64B aligned GFX MEM address where the chap counter
    ///     values are reported. GraphicsAddress [63:48] are ignored by the HW and
    ///     assumed to be in correct canonical form [63:48] == [47]
    ///     
    ///     This field is ignored if "Report to OABUFFER" bit is set.
    ///     

    typedef enum tagMEMORYADDRESS
    {
        MEMORYADDRESS_BIT_SHIFT  = 6,
        MEMORYADDRESS_ALIGN_SIZE = 64,
    } MEMORYADDRESS;

    __CODEGEN_INLINE void SetMemoryAddress( const __CODEGEN_UINT64 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.MemoryAddress = value >> MEMORYADDRESS_BIT_SHIFT;
    }

    /// @brief Read MemoryAddress
    __CODEGEN_INLINE __CODEGEN_UINT64 GetMemoryAddress( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.MemoryAddress << MEMORYADDRESS_BIT_SHIFT );
    }

    /// @brief Write ReportID
    /// @details 
    ///     This field specifies the ID provided by SW for a given report command.
    ///     It can be tracked to use different flavors of these reports based on
    ///     where in command-stream they are inserted.This field is reported only
    ///     when Counter Select Field is 0.
    ///     
    ///     If a privilege access violation occurs, the REPORT ID field in the
    ///     report generated by the next legitimate MI_REPORT_PERF_COUNT will be
    ///     corrupted.
    ///     

    __CODEGEN_INLINE void SetReportID( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.ReportID = value;
    }

    /// @brief Read ReportID
    __CODEGEN_INLINE __CODEGEN_UINT32 GetReportID( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.ReportID );
    }

    /// @}
} __CODEGEN_ATTRIBUTES_STRUCTURE;

__CODEGEN_C_ASSERT( 16 == sizeof( MI_REPORT_PERF_COUNT ) );
        
//////////////////////////////////////////////////////////////////////////
/// @brief MI_STORE_REGISTER_MEM
/// @details
///     The MI_STORE_REGISTER_MEM command requests a register read from a
///     specified memory mapped register location in the device and store of
///     that DWord to memory. The register address is specified along with the
///     command to perform the read.
///     
///     
///     The command temporarily halts command execution.
///     
///     The memory address for the write is snooped on the host bus.
///     
///     This command should not be used from within a "non-privilege" batch
///     buffer to access global virtual space. doing so will be treated as
///     privilege access violation. Refer "User Mode Privilege Command" in
///     MI_BATCH_BUFFER_START command section to know HW behavior on
///     encountering privilege access violation. This command can be used within
///     ring buffers and/or "privilege" batch buffers to access global virtual
///     space.
///     
///     This command will cause undefined data to be written to memory if given
///     register addresses for the PGTBL_CTL_0 or FENCE registers.
///     
///     
///     
struct MI_STORE_REGISTER_MEM
{
    __CODEGEN_ACCESS_SPECIFIER_DEFINITION
    union tagTheStructure
    {
        struct tagCommon
        {
            /// DWORD 0
            __CODEGEN_UINT32         DWordLength                                      : __CODEGEN_BITFIELD( 0,  7)    ; ///< U8
            __CODEGEN_UINT32         Reserved_8                                       : __CODEGEN_BITFIELD( 8, 18)    ; ///< U11
            __CODEGEN_UINT32         AddCSMMIOStartOffset                             : __CODEGEN_BITFIELD(19, 19)    ; ///< U1
            __CODEGEN_UINT32         Reserved_20                                      : __CODEGEN_BITFIELD(20, 20)    ; ///< U1
            __CODEGEN_UINT32         PredicateEnable                                  : __CODEGEN_BITFIELD(21, 21)    ; ///< U1
            __CODEGEN_UINT32         UseGlobalGTT                                     : __CODEGEN_BITFIELD(22, 22)    ; ///< U1
            __CODEGEN_UINT32         MICommandOpcode                                  : __CODEGEN_BITFIELD(23, 28)    ; ///< U6
            __CODEGEN_UINT32         CommandType                                      : __CODEGEN_BITFIELD(29, 31)    ; ///< U3

            /// DWORD 1
            __CODEGEN_UINT32         Reserved_32                                      : __CODEGEN_BITFIELD( 0,  1)    ; ///< U2
            __CODEGEN_UINT32         RegisterAddress                                  : __CODEGEN_BITFIELD( 2, 22)    ; ///< U21
            __CODEGEN_UINT32         Reserved_55                                      : __CODEGEN_BITFIELD(23, 31)    ; ///< U9

            /// DWORD 2..3
            __CODEGEN_UINT64         Reserved_64                                      : __CODEGEN_BITFIELD( 0,  1)    ; ///< U2
            __CODEGEN_UINT64         MemoryAddress                                    : __CODEGEN_BITFIELD( 2, 63)    ; ///< U62

        } Common;
        __CODEGEN_UINT32            RawData[ 4 ];
    } TheStructure;

    __CODEGEN_ACCESS_SPECIFIER_METHODS
    //////////////////////////////////////////////////////////////////////////
    /// @name STRUCTURE DEBUG
    /// @{

    /// @brief User-defined debug function for structure
    __CODEGEN_DebugType( MI_STORE_REGISTER_MEM )
    {
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.DWordLength                                 ,  0,  7, DWORD_LENGTH );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.AddCSMMIOStartOffset                        , 19, 19 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.PredicateEnable                             , 21, 21 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.UseGlobalGTT                                , 22, 22 );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.MICommandOpcode                             , 23, 28, MI_COMMAND_OPCODE );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.CommandType                                 , 29, 31, COMMAND_TYPE );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.RegisterAddress                             ,  2, 22 );
        __CODEGEN_DebugAttributeUInt64( TheStructure.Common.MemoryAddress                               ,  2, 63 );
    }
    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name LOCAL ENUMERATIONS
    /// @{

    /// @brief U8
    typedef enum tagDWORD_LENGTH
    {
        DWORD_LENGTH_EXCLUDES_DWORD_0_1                                  = 2, ///< 
    } DWORD_LENGTH;

    /// @brief U6
    typedef enum tagMI_COMMAND_OPCODE
    {
        MI_COMMAND_OPCODE_MI_STORE_REGISTER_MEM                          = 36, ///< 
    } MI_COMMAND_OPCODE;

    /// @brief U3
    typedef enum tagCOMMAND_TYPE
    {
        COMMAND_TYPE_MI_COMMAND                                          = 0, ///< 
    } COMMAND_TYPE;

    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name ENUMERATION DEBUG
    /// @{

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( DWORD_LENGTH )
    {
        __CODEGEN_DebugEnumValue( DWORD_LENGTH_EXCLUDES_DWORD_0_1 );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( MI_COMMAND_OPCODE )
    {
        __CODEGEN_DebugEnumValue( MI_COMMAND_OPCODE_MI_STORE_REGISTER_MEM );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( COMMAND_TYPE )
    {
        __CODEGEN_DebugEnumValue( COMMAND_TYPE_MI_COMMAND );
    }

    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name INITIALIZATION
    /// @{

    /// @brief Explicit member initialization function
    __CODEGEN_INLINE void Init( void )
    {
        TheStructure.RawData[ 0 ]                                    = 0x12000002;
        //TheStructure.Common.DWordLength                            = DWORD_LENGTH_EXCLUDES_DWORD_0_1;
        //TheStructure.Common.AddCSMMIOStartOffset                   = 0x0;
        //TheStructure.Common.PredicateEnable                        = 0;
        //TheStructure.Common.UseGlobalGTT                           = 0;
        //TheStructure.Common.MICommandOpcode                        = MI_COMMAND_OPCODE_MI_STORE_REGISTER_MEM;
        //TheStructure.Common.CommandType                            = COMMAND_TYPE_MI_COMMAND;
        TheStructure.RawData[ 1 ]                                    = 0x0;
        //TheStructure.Common.RegisterAddress                        = 0x0;
        TheStructure.RawData[ 2 ]                                    = 0x0;
        TheStructure.RawData[ 3 ]                                    = 0x0;
        //TheStructure.Common.MemoryAddress                          = 0x0;
    }

    /// @brief Explicit static initialization function
    static MI_STORE_REGISTER_MEM sInit( void ) 
    {
        MI_STORE_REGISTER_MEM state;
        state.Init();
        return state;
    }
    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name ACCESSORS
    /// @{

    /// @brief Raw Data Access
    __CODEGEN_INLINE __CODEGEN_UINT32& GetRawData( const __CODEGEN_UINT index )
    {
        __CODEGEN_ASSERT( index < 4 );
        return TheStructure.RawData[ index ];
    }

    /// @brief Write AddCSMMIOStartOffset
    /// @details 
    ///     This bit controls the functionality of the “Register Address” field in
    ///     the command.
    ///     

    __CODEGEN_INLINE void SetAddCSMMIOStartOffset( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.AddCSMMIOStartOffset = value;
    }

    /// @brief Read AddCSMMIOStartOffset
    __CODEGEN_INLINE __CODEGEN_UINT32 GetAddCSMMIOStartOffset( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.AddCSMMIOStartOffset );
    }

    /// @brief Write PredicateEnable
    /// @details 
    ///     
    ///     If set, this command is executed (or not) depending on the current value
    ///     of the MI_PREDICATE internal state bit in MMIO register
    ///     MI_PREDICATE_RESULT[0]. This command is ignored only if PredicateEnable
    ///     is set and value in the MMIO register MI_PREDICATE_RESULT[0] is 0.
    ///     

    __CODEGEN_INLINE void SetPredicateEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.PredicateEnable = value;
    }

    /// @brief Read PredicateEnable
    __CODEGEN_INLINE __CODEGEN_BOOL GetPredicateEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.PredicateEnable );
    }

    /// @brief Write UseGlobalGTT
    /// @details 
    ///     It is allowed for this bit to be set when executing this command from a
    ///     privileged (secure) batch or ring buffer. This bit must be clear when
    ///     programmed from within a non-privileged batch buffer. This bit must be 1
    ///     if the Per Process GTT Enable bit is clear. This command will use the
    ///     global GTT to translate the Address and this command must be executing
    ///     from a privileged (secure) batch buffer.
    ///     

    __CODEGEN_INLINE void SetUseGlobalGTT( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.UseGlobalGTT = value;
    }

    /// @brief Read UseGlobalGTT
    __CODEGEN_INLINE __CODEGEN_BOOL GetUseGlobalGTT( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.UseGlobalGTT );
    }

    /// @brief Write RegisterAddress
    /// @details 
    ///     This field specifies Bits 22:2 of the Register offset the DWord will be
    ///     read from. As the register address must be DWord-aligned, Bits 1:0 of
    ///     that address MBZ.
    ///     
    ///     
    ///     Storing a VGA register is not permitted and will store an UNDEFINED
    ///     value.
    ///     
    ///     The values of PGTBL_CTL0 or any of the FENCE registers cannot be stored
    ///     to memory; UNDEFINED values will be written to memory if the addresses
    ///     of these registers are specified.
    ///     
    ///     
    ///     

    typedef enum tagREGISTERADDRESS
    {
        REGISTERADDRESS_BIT_SHIFT  = 2,
        REGISTERADDRESS_ALIGN_SIZE = 4,
    } REGISTERADDRESS;

    __CODEGEN_INLINE void SetRegisterAddress( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.RegisterAddress = value >> REGISTERADDRESS_BIT_SHIFT;
    }

    /// @brief Read RegisterAddress
    __CODEGEN_INLINE __CODEGEN_UINT32 GetRegisterAddress( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.RegisterAddress << REGISTERADDRESS_BIT_SHIFT );
    }

    /// @brief Write MemoryAddress
    /// @details 
    ///     This field specifies the address of the memory location where the
    ///     register value specified in the DWord above will be written. The address
    ///     specifies the DWord location of the data.Range =
    ///     GraphicsVirtualAddress[63:2] for a DWord register
    ///     GraphicsAddress [63:48] are ignored by the HW and assumed to be in
    ///     correct canonical form [63:48] == [47].
    ///     

    typedef enum tagMEMORYADDRESS
    {
        MEMORYADDRESS_BIT_SHIFT  = 2,
        MEMORYADDRESS_ALIGN_SIZE = 4,
    } MEMORYADDRESS;

    __CODEGEN_INLINE void SetMemoryAddress( const __CODEGEN_UINT64 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.MemoryAddress = value >> MEMORYADDRESS_BIT_SHIFT;
    }

    /// @brief Read MemoryAddress
    __CODEGEN_INLINE __CODEGEN_UINT64 GetMemoryAddress( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.MemoryAddress << MEMORYADDRESS_BIT_SHIFT );
    }

    /// @}
} __CODEGEN_ATTRIBUTES_STRUCTURE;

__CODEGEN_C_ASSERT( 16 == sizeof( MI_STORE_REGISTER_MEM ) );
        
//////////////////////////////////////////////////////////////////////////
/// @brief PIPE_CONTROL
/// @details
///     The PIPE_CONTROL command is used to effect the synchronization described
///     above.
///     
///     SW must follow below programming restrictions when
///     programmingPIPECONTROL command for POCS:
///     
///     
///     Write cache flush bits must not be set (Render Target Cache Flush
///     Enable, DC Flush Enable, Depth Cache Flush Enable )
///     
///     Post Sync Operations must not be set to “Write PS Depth Count”
///     
///     “Stall at Pixel Scoreboard” must not be set
///     
///     “Protected Memory Application ID” must not be programmed.
///     
///     “Notify Enable” must not be set.
///     
///     “Depth Stall Enable” must not be set.
///     
///     “Generic Media State Clear” must not be set.
///     
///     “PSD Sync Enable” must not be set.
///     
///     “Protected Memory Enable” must not be set.
///     
///     “Protected Memory Disable” must not be set.
///     
///     
///     
///     Only in debug mode and for Debug ability "Notify Enable" is allowed to
///     be set in PIPECONTROL command programmed for POSH.
///     
struct PIPE_CONTROL
{
    __CODEGEN_ACCESS_SPECIFIER_DEFINITION
    union tagTheStructure
    {
        struct tagCommon
        {
            /// DWORD 0
            __CODEGEN_UINT32         DWordLength                                      : __CODEGEN_BITFIELD( 0,  7)    ; ///< U8
            __CODEGEN_UINT32         Reserved_8                                       : __CODEGEN_BITFIELD( 8, 15)    ; ///< U8
            __CODEGEN_UINT32         _3DCommandSubOpcode                              : __CODEGEN_BITFIELD(16, 23)    ; ///< U8
            __CODEGEN_UINT32         _3DCommandOpcode                                 : __CODEGEN_BITFIELD(24, 26)    ; ///< U3
            __CODEGEN_UINT32         CommandSubType                                   : __CODEGEN_BITFIELD(27, 28)    ; ///< U2
            __CODEGEN_UINT32         CommandType                                      : __CODEGEN_BITFIELD(29, 31)    ; ///< U3

            /// DWORD 1
            __CODEGEN_UINT32         DepthCacheFlushEnable                            : __CODEGEN_BITFIELD( 0,  0)    ; ///< U1
            __CODEGEN_UINT32         StallAtPixelScoreboard                           : __CODEGEN_BITFIELD( 1,  1)    ; ///< U1
            __CODEGEN_UINT32         StateCacheInvalidationEnable                     : __CODEGEN_BITFIELD( 2,  2)    ; ///< U1
            __CODEGEN_UINT32         ConstantCacheInvalidationEnable                  : __CODEGEN_BITFIELD( 3,  3)    ; ///< U1
            __CODEGEN_UINT32         VFCacheInvalidationEnable                        : __CODEGEN_BITFIELD( 4,  4)    ; ///< U1
            __CODEGEN_UINT32         DCFlushEnable                                    : __CODEGEN_BITFIELD( 5,  5)    ; ///< U1
            __CODEGEN_UINT32         ProtectedMemoryApplicationID                     : __CODEGEN_BITFIELD( 6,  6)    ; ///< U1
            __CODEGEN_UINT32         PipeControlFlushEnable                           : __CODEGEN_BITFIELD( 7,  7)    ; ///< U1
            __CODEGEN_UINT32         NotifyEnable                                     : __CODEGEN_BITFIELD( 8,  8)    ; ///< U1
            __CODEGEN_UINT32         IndirectStatePointersDisable                     : __CODEGEN_BITFIELD( 9,  9)    ; ///< U1
            __CODEGEN_UINT32         TextureCacheInvalidationEnable                   : __CODEGEN_BITFIELD(10, 10)    ; ///< U1
            __CODEGEN_UINT32         InstructionCacheInvalidateEnable                 : __CODEGEN_BITFIELD(11, 11)    ; ///< U1
            __CODEGEN_UINT32         RenderTargetCacheFlushEnable                     : __CODEGEN_BITFIELD(12, 12)    ; ///< U1
            __CODEGEN_UINT32         DepthStallEnable                                 : __CODEGEN_BITFIELD(13, 13)    ; ///< U1
            __CODEGEN_UINT32         PostSyncOperation                                : __CODEGEN_BITFIELD(14, 15)    ; ///< U2
            __CODEGEN_UINT32         GenericMediaStateClear                           : __CODEGEN_BITFIELD(16, 16)    ; ///< U1
            __CODEGEN_UINT32         PSDSyncEnable                                    : __CODEGEN_BITFIELD(17, 17)    ; ///< U1
            __CODEGEN_UINT32         TLBInvalidate                                    : __CODEGEN_BITFIELD(18, 18)    ; ///< U1
            __CODEGEN_UINT32         GlobalSnapshotCountReset                         : __CODEGEN_BITFIELD(19, 19)    ; ///< U1
            __CODEGEN_UINT32         CommandStreamerStallEnable                       : __CODEGEN_BITFIELD(20, 20)    ; ///< U1
            __CODEGEN_UINT32         StoreDataIndex                                   : __CODEGEN_BITFIELD(21, 21)    ; ///< U1
            __CODEGEN_UINT32         ProtectedMemoryEnable                            : __CODEGEN_BITFIELD(22, 22)    ; ///< U1
            __CODEGEN_UINT32         LRIPostSyncOperation                             : __CODEGEN_BITFIELD(23, 23)    ; ///< U1
            __CODEGEN_UINT32         DestinationAddressType                           : __CODEGEN_BITFIELD(24, 24)    ; ///< U1
            __CODEGEN_UINT32         Reserved_57                                      : __CODEGEN_BITFIELD(25, 25)    ; ///< U1
            __CODEGEN_UINT32         FlushLLC                                         : __CODEGEN_BITFIELD(26, 26)    ; ///< U1
            __CODEGEN_UINT32         ProtectedMemoryDisable                           : __CODEGEN_BITFIELD(27, 27)    ; ///< U1
            __CODEGEN_UINT32         TileCacheFlushEnable                             : __CODEGEN_BITFIELD(28, 28)    ; ///< U1
            __CODEGEN_UINT32         CommandCacheInvalidateEnable                     : __CODEGEN_BITFIELD(29, 29)    ; ///< U1
            __CODEGEN_UINT32         Reserved_62                                      : __CODEGEN_BITFIELD(30, 31)    ; ///< U2

            /// DWORD 2
            __CODEGEN_UINT32         Reserved_64                                      : __CODEGEN_BITFIELD( 0,  1)    ; ///< U2
            __CODEGEN_UINT32         Address                                          : __CODEGEN_BITFIELD( 2, 31)    ; ///< U30

            /// DWORD 3
            __CODEGEN_UINT32         AddressHigh                                                                      ; ///< U32

            /// DWORD 4..5
            __CODEGEN_UINT64         ImmediateData                                                                    ; ///< U64

        } Common;
        __CODEGEN_UINT32            RawData[ 6 ];
    } TheStructure;

    __CODEGEN_ACCESS_SPECIFIER_METHODS
    //////////////////////////////////////////////////////////////////////////
    /// @name STRUCTURE DEBUG
    /// @{

    /// @brief User-defined debug function for structure
    __CODEGEN_DebugType( PIPE_CONTROL )
    {
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.DWordLength                                 ,  0,  7, DWORD_LENGTH );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common._3DCommandSubOpcode                         , 16, 23, _3D_COMMAND_SUB_OPCODE );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common._3DCommandOpcode                            , 24, 26, _3D_COMMAND_OPCODE );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.CommandSubType                              , 27, 28, COMMAND_SUBTYPE );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.CommandType                                 , 29, 31, COMMAND_TYPE );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.DepthCacheFlushEnable                       ,  0,  0 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.StallAtPixelScoreboard                      ,  1,  1 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.StateCacheInvalidationEnable                ,  2,  2 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.ConstantCacheInvalidationEnable             ,  3,  3 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.VFCacheInvalidationEnable                   ,  4,  4 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.DCFlushEnable                               ,  5,  5 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.ProtectedMemoryApplicationID                ,  6,  6 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.PipeControlFlushEnable                      ,  7,  7 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.NotifyEnable                                ,  8,  8 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.IndirectStatePointersDisable                ,  9,  9 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.TextureCacheInvalidationEnable              , 10, 10 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.InstructionCacheInvalidateEnable            , 11, 11 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.RenderTargetCacheFlushEnable                , 12, 12 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.DepthStallEnable                            , 13, 13 );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.PostSyncOperation                           , 14, 15, POST_SYNC_OPERATION );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.GenericMediaStateClear                      , 16, 16 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.PSDSyncEnable                               , 17, 17 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.TLBInvalidate                               , 18, 18 );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.GlobalSnapshotCountReset                    , 19, 19, GLOBAL_SNAPSHOT_COUNT_RESET );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.CommandStreamerStallEnable                  , 20, 20 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.StoreDataIndex                              , 21, 21 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.ProtectedMemoryEnable                       , 22, 22 );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.LRIPostSyncOperation                        , 23, 23, LRI_POST_SYNC_OPERATION );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.DestinationAddressType                      , 24, 24, DESTINATION_ADDRESS_TYPE );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.FlushLLC                                    , 26, 26 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.ProtectedMemoryDisable                      , 27, 27 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.TileCacheFlushEnable                        , 28, 28 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.CommandCacheInvalidateEnable                , 29, 29 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.Address                                     ,  2, 31 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.AddressHigh                                 ,  0, 31 );
        __CODEGEN_DebugAttributeUInt64( TheStructure.Common.ImmediateData                               ,  0, 63 );
    }
    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name LOCAL ENUMERATIONS
    /// @{

    /// @brief U8
    typedef enum tagDWORD_LENGTH
    {
        DWORD_LENGTH_DWORD_COUNT_N                                       = 4, ///< 
    } DWORD_LENGTH;

    /// @brief U8
    typedef enum tag_3D_COMMAND_SUB_OPCODE
    {
        _3D_COMMAND_SUB_OPCODE_PIPE_CONTROL                              = 0, ///< 
    } _3D_COMMAND_SUB_OPCODE;

    /// @brief U3
    typedef enum tag_3D_COMMAND_OPCODE
    {
        _3D_COMMAND_OPCODE_PIPE_CONTROL                                  = 2, ///< 
    } _3D_COMMAND_OPCODE;

    /// @brief U2
    typedef enum tagCOMMAND_SUBTYPE
    {
        COMMAND_SUBTYPE_GFXPIPE_3D                                       = 3, ///< 
    } COMMAND_SUBTYPE;

    /// @brief U3
    typedef enum tagCOMMAND_TYPE
    {
        COMMAND_TYPE_GFXPIPE                                             = 3, ///< 
    } COMMAND_TYPE;

    /// @brief U2
    typedef enum tagPOST_SYNC_OPERATION
    {
        POST_SYNC_OPERATION_NO_WRITE                                     = 0, ///< No write occurs as a result of this instruction. This can be used to implement a "trap" operation, etc.
        POST_SYNC_OPERATION_WRITE_IMMEDIATE_DATA                         = 1, ///< Write the QWord containing Immediate Data Low, High DWs to the Destination Address
        POST_SYNC_OPERATION_WRITE_PS_DEPTH_COUNT                         = 2, ///< Write the 64-bit PS_DEPTH_COUNT register to the Destination Address
        POST_SYNC_OPERATION_WRITE_TIMESTAMP                              = 3, ///< Write the 64-bit TIMESTAMP register(i.e. "Reported Timestamp Count" 0x2358 for render pipe) to the Destination Address.
    } POST_SYNC_OPERATION;

    /// @brief U1
    typedef enum tagGLOBAL_SNAPSHOT_COUNT_RESET
    {
        GLOBAL_SNAPSHOT_COUNT_RESET_DONT_RESET                           = 0, ///< Do not reset the snapshot counts or Statistics Counters.
        GLOBAL_SNAPSHOT_COUNT_RESET_RESET                                = 1, ///< Reset the snapshot count in Gen4 for all the units and reset the Statistics Counters except as noted above.
    } GLOBAL_SNAPSHOT_COUNT_RESET;

    /// @brief U1
    typedef enum tagLRI_POST_SYNC_OPERATION
    {
        LRI_POST_SYNC_OPERATION_NO_LRI_OPERATION                         = 0, ///< No LRI operation occurs as a result of this instruction. The Post-Sync Operation field is valid and may be used to specify an operation.
        LRI_POST_SYNC_OPERATION_MMIO_WRITE_IMMEDIATE_DATA                = 1, ///< Write the DWord contained in Immediate Data Low (DW3) to the MMIO offset specified in the Address field.
    } LRI_POST_SYNC_OPERATION;

    /// @brief U1
    typedef enum tagDESTINATION_ADDRESS_TYPE
    {
        DESTINATION_ADDRESS_TYPE_PPGTT                                   = 0, ///< Use PPGTT address space for DW write
        DESTINATION_ADDRESS_TYPE_GGTT                                    = 1, ///< Use GGTT address space for DW write
    } DESTINATION_ADDRESS_TYPE;

    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name ENUMERATION DEBUG
    /// @{

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( DWORD_LENGTH )
    {
        __CODEGEN_DebugEnumValue( DWORD_LENGTH_DWORD_COUNT_N );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( _3D_COMMAND_SUB_OPCODE )
    {
        __CODEGEN_DebugEnumValue( _3D_COMMAND_SUB_OPCODE_PIPE_CONTROL );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( _3D_COMMAND_OPCODE )
    {
        __CODEGEN_DebugEnumValue( _3D_COMMAND_OPCODE_PIPE_CONTROL );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( COMMAND_SUBTYPE )
    {
        __CODEGEN_DebugEnumValue( COMMAND_SUBTYPE_GFXPIPE_3D );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( COMMAND_TYPE )
    {
        __CODEGEN_DebugEnumValue( COMMAND_TYPE_GFXPIPE );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( POST_SYNC_OPERATION )
    {
        __CODEGEN_DebugEnumValue( POST_SYNC_OPERATION_NO_WRITE );
        __CODEGEN_DebugEnumValue( POST_SYNC_OPERATION_WRITE_IMMEDIATE_DATA );
        __CODEGEN_DebugEnumValue( POST_SYNC_OPERATION_WRITE_PS_DEPTH_COUNT );
        __CODEGEN_DebugEnumValue( POST_SYNC_OPERATION_WRITE_TIMESTAMP );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( GLOBAL_SNAPSHOT_COUNT_RESET )
    {
        __CODEGEN_DebugEnumValue( GLOBAL_SNAPSHOT_COUNT_RESET_DONT_RESET );
        __CODEGEN_DebugEnumValue( GLOBAL_SNAPSHOT_COUNT_RESET_RESET );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( LRI_POST_SYNC_OPERATION )
    {
        __CODEGEN_DebugEnumValue( LRI_POST_SYNC_OPERATION_NO_LRI_OPERATION );
        __CODEGEN_DebugEnumValue( LRI_POST_SYNC_OPERATION_MMIO_WRITE_IMMEDIATE_DATA );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( DESTINATION_ADDRESS_TYPE )
    {
        __CODEGEN_DebugEnumValue( DESTINATION_ADDRESS_TYPE_PPGTT );
        __CODEGEN_DebugEnumValue( DESTINATION_ADDRESS_TYPE_GGTT );
    }

    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name INITIALIZATION
    /// @{

    /// @brief Explicit member initialization function
    __CODEGEN_INLINE void Init( void )
    {
        TheStructure.RawData[ 0 ]                                    = 0x7a000004;
        //TheStructure.Common.DWordLength                            = DWORD_LENGTH_DWORD_COUNT_N;
        //TheStructure.Common._3DCommandSubOpcode                    = _3D_COMMAND_SUB_OPCODE_PIPE_CONTROL;
        //TheStructure.Common._3DCommandOpcode                       = _3D_COMMAND_OPCODE_PIPE_CONTROL;
        //TheStructure.Common.CommandSubType                         = COMMAND_SUBTYPE_GFXPIPE_3D;
        //TheStructure.Common.CommandType                            = COMMAND_TYPE_GFXPIPE;
        TheStructure.RawData[ 1 ]                                    = 0x0;
        //TheStructure.Common.DepthCacheFlushEnable                  = 0;
        //TheStructure.Common.StallAtPixelScoreboard                 = 0;
        //TheStructure.Common.StateCacheInvalidationEnable           = 0;
        //TheStructure.Common.ConstantCacheInvalidationEnable        = 0;
        //TheStructure.Common.VFCacheInvalidationEnable              = 0;
        //TheStructure.Common.DCFlushEnable                          = 0;
        //TheStructure.Common.ProtectedMemoryApplicationID           = 0x0;
        //TheStructure.Common.PipeControlFlushEnable                 = 0;
        //TheStructure.Common.NotifyEnable                           = 0;
        //TheStructure.Common.IndirectStatePointersDisable           = 0;
        //TheStructure.Common.TextureCacheInvalidationEnable         = 0;
        //TheStructure.Common.InstructionCacheInvalidateEnable       = 0;
        //TheStructure.Common.RenderTargetCacheFlushEnable           = 0;
        //TheStructure.Common.DepthStallEnable                       = 0;
        //TheStructure.Common.PostSyncOperation                      = POST_SYNC_OPERATION_NO_WRITE;
        //TheStructure.Common.GenericMediaStateClear                 = 0;
        //TheStructure.Common.PSDSyncEnable                          = 0;
        //TheStructure.Common.TLBInvalidate                          = 0x0;
        //TheStructure.Common.GlobalSnapshotCountReset               = GLOBAL_SNAPSHOT_COUNT_RESET_DONT_RESET;
        //TheStructure.Common.CommandStreamerStallEnable             = 0x0;
        //TheStructure.Common.StoreDataIndex                         = 0x0;
        //TheStructure.Common.ProtectedMemoryEnable                  = 0x0;
        //TheStructure.Common.LRIPostSyncOperation                   = LRI_POST_SYNC_OPERATION_NO_LRI_OPERATION;
        //TheStructure.Common.DestinationAddressType                 = DESTINATION_ADDRESS_TYPE_PPGTT;
        //TheStructure.Common.FlushLLC                               = 0;
        //TheStructure.Common.ProtectedMemoryDisable                 = 0x0;
        //TheStructure.Common.TileCacheFlushEnable                   = 0x0;
        //TheStructure.Common.CommandCacheInvalidateEnable           = 0;
        TheStructure.RawData[ 2 ]                                    = 0x0;
        //TheStructure.Common.Address                                = 0x0;
        TheStructure.RawData[ 3 ]                                    = 0x0;
        //TheStructure.Common.AddressHigh                            = 0x0;
        TheStructure.RawData[ 4 ]                                    = 0x0;
        TheStructure.RawData[ 5 ]                                    = 0x0;
        //TheStructure.Common.ImmediateData                          = 0x0;
    }

    /// @brief Explicit static initialization function
    static PIPE_CONTROL sInit( void ) 
    {
        PIPE_CONTROL state;
        state.Init();
        return state;
    }
    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name ACCESSORS
    /// @{

    /// @brief Raw Data Access
    __CODEGEN_INLINE __CODEGEN_UINT32& GetRawData( const __CODEGEN_UINT index )
    {
        __CODEGEN_ASSERT( index < 6 );
        return TheStructure.RawData[ index ];
    }

    /// @brief Write DepthCacheFlushEnable
    /// @details 
    ///     Setting this bit enables flushing (i.e. writing back the dirty lines to
    ///     memory and invalidating the tags) of depth related caches. This bit
    ///     applies to HiZ cache, Stencil cache and depth cache.
    ///     
    ///     Ideally depth caches need to be flushed only when depth is required to
    ///     be coherent in memory for later use as a texture, source or honoring CPU
    ///     lock. This bit must be DISABLED for End-of-pipe (Read) fences,
    ///     PS_DEPTH_COUNT or TIMESTAMP queries.
    ///     
    ///     Format: Enable
    ///     
    ///     DEPTH_CACHE_FLUSH_ENABLE_FLUSH_DISABLED (0): Depth relates caches (HiZ,
    ///     Stencil and Depth) are NOT flushed.
    ///     
    ///     DEPTH_CACHE_FLUSH_ENABLE_FLUSH_ENABLED (1): Depth relates caches (HiZ,
    ///     Stencil and Depth) are flushed.

    __CODEGEN_INLINE void SetDepthCacheFlushEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.DepthCacheFlushEnable = value;
    }

    /// @brief Read DepthCacheFlushEnable
    __CODEGEN_INLINE __CODEGEN_BOOL GetDepthCacheFlushEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.DepthCacheFlushEnable );
    }

    /// @brief Write StallAtPixelScoreboard
    /// @details 
    ///     Defines the behavior of PIPE_CONTROL command at the pixel scoreboard.
    ///     
    ///     This bit must be DISABLED for End-of-pipe (Read) fences, PS_DEPTH_COUNT
    ///     or TIMESTAMP queries. This bit is ignored if Depth Stall Enable is set.
    ///     Further the render cache is not flushed even if Write Cache Flush Enable
    ///     bit is set.
    ///     
    ///     Format: Enable
    ///     
    ///     STALL_AT_PIXEL_SCOREBOARD_DISABLE (0): Stall at the pixel scoreboard is
    ///     disabled.
    ///     
    ///     STALL_AT_PIXEL_SCOREBOARD_ENABLE (1): Stall at the pixel scoreboard is
    ///     enabled.

    __CODEGEN_INLINE void SetStallAtPixelScoreboard( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.StallAtPixelScoreboard = value;
    }

    /// @brief Read StallAtPixelScoreboard
    __CODEGEN_INLINE __CODEGEN_BOOL GetStallAtPixelScoreboard( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.StallAtPixelScoreboard );
    }

    /// @brief Write StateCacheInvalidationEnable
    /// @details 
    ///     Setting this bit is independent of any other bit in this packet. This
    ///     bit controls the invalidation of the L1 and L2 state caches at the top
    ///     of the pipe i.e. at the parsing time.
    ///     
    ///     When “State Cache redirect to CS Section enable” bit is set in MMIO
    ///     register SLICE_COMMON_ECO_CHCICKEN1 (0731Ch), “Command Cache Invalidate
    ///     Enable” must be also set upon setting “State Cache Invalidate” bit in
    ///     PIPE_CONTROL command.
    ///     

    __CODEGEN_INLINE void SetStateCacheInvalidationEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.StateCacheInvalidationEnable = value;
    }

    /// @brief Read StateCacheInvalidationEnable
    __CODEGEN_INLINE __CODEGEN_BOOL GetStateCacheInvalidationEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.StateCacheInvalidationEnable );
    }

    /// @brief Write ConstantCacheInvalidationEnable
    /// @details 
    ///     Setting this bit is independent of any other bit in this packet. This
    ///     bit controls the invalidation of the constant cache at the top of the
    ///     pipe i.e. at the parsing time.
    ///     

    __CODEGEN_INLINE void SetConstantCacheInvalidationEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.ConstantCacheInvalidationEnable = value;
    }

    /// @brief Read ConstantCacheInvalidationEnable
    __CODEGEN_INLINE __CODEGEN_BOOL GetConstantCacheInvalidationEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.ConstantCacheInvalidationEnable );
    }

    /// @brief Write VFCacheInvalidationEnable
    /// @details 
    ///     Setting this bit is independent of any other bit in this packet. This
    ///     bit controls the invalidation of VF address based cache at the top of
    ///     the pipe i.e. at the parsing time.
    ///     
    ///     When executed from POCS it results in invalidating the VFR cache.
    ///     

    __CODEGEN_INLINE void SetVFCacheInvalidationEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.VFCacheInvalidationEnable = value;
    }

    /// @brief Read VFCacheInvalidationEnable
    __CODEGEN_INLINE __CODEGEN_BOOL GetVFCacheInvalidationEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.VFCacheInvalidationEnable );
    }

    /// @brief Write DCFlushEnable
    /// @details 
    ///     Setting this bit enables flushing of the L3$ portions that caches DC
    ///     writes.
    ///     
    ///     DC Flush (L3 Flush) by default doesn’t result in flushing/invalidating
    ///     the IA Coherent lines from L3$, however this can be achieved by setting
    ///     control bit “Pipe line flush Coherent lines”  in “L3SQCREG4” register.
    ///     

    __CODEGEN_INLINE void SetDCFlushEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.DCFlushEnable = value;
    }

    /// @brief Read DCFlushEnable
    __CODEGEN_INLINE __CODEGEN_BOOL GetDCFlushEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.DCFlushEnable );
    }

    /// @brief Write ProtectedMemoryApplicationID
    /// @details 
    ///     Context ID for Media Applications
    ///     

    __CODEGEN_INLINE void SetProtectedMemoryApplicationID( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.ProtectedMemoryApplicationID = value;
    }

    /// @brief Read ProtectedMemoryApplicationID
    __CODEGEN_INLINE __CODEGEN_UINT32 GetProtectedMemoryApplicationID( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.ProtectedMemoryApplicationID );
    }

    /// @brief Write PipeControlFlushEnable
    /// @details 
    ///     Hardware on parsing PIPECONTROL command with Pipe Control Flush Enable
    ///     set will wait for all the outstanding post sync operations corresponding
    ///     to previously executed PIPECONTROL commands are complete before making
    ///     forward progress.
    ///     

    __CODEGEN_INLINE void SetPipeControlFlushEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.PipeControlFlushEnable = value;
    }

    /// @brief Read PipeControlFlushEnable
    __CODEGEN_INLINE __CODEGEN_BOOL GetPipeControlFlushEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.PipeControlFlushEnable );
    }

    /// @brief Write NotifyEnable
    /// @details 
    ///     If ENABLED, a Sync Completion Interrupt will be generated (if enabled by
    ///     the MI Interrupt Control registers) once the sync operation is complete.
    ///     See Interrupt Control Registers in Memory Interface Registers for
    ///     details.
    ///     

    __CODEGEN_INLINE void SetNotifyEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.NotifyEnable = value;
    }

    /// @brief Read NotifyEnable
    __CODEGEN_INLINE __CODEGEN_BOOL GetNotifyEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.NotifyEnable );
    }

    /// @brief Write IndirectStatePointersDisable
    /// @details 
    ///     At the completion of the post-sync operation associated with this pipe
    ///     control packet, the indirect state pointers in the hardware are
    ///     considered invalid; the indirect pointers are not saved in the context.
    ///     If any new indirect state commands are executed in the command stream
    ///     while the pipe control is pending, the new indirect state commands are
    ///     preserved.
    ///     
    ///     [DevIVB+]: Using Invalidate State Pointer (ISP) only inhibits context
    ///     restoring of Push Constant (3DSTATE_CONSTANT_*) commands. Push Constant
    ///     commands are only considered as Indirect State Pointers. Once ISP is
    ///     issued in a context, SW must initialize by programming push constant
    ///     commands for all the shaders (at least to zero length) before attempting
    ///     any rendering operation for the same context.
    ///     
    ///     When executed in POCS results in inhibiting the context restore of
    ///     3DSTATE_CONSTANT_VS from POSH engine context.
    ///     
    ///     

    __CODEGEN_INLINE void SetIndirectStatePointersDisable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.IndirectStatePointersDisable = value;
    }

    /// @brief Read IndirectStatePointersDisable
    __CODEGEN_INLINE __CODEGEN_BOOL GetIndirectStatePointersDisable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.IndirectStatePointersDisable );
    }

    /// @brief Write TextureCacheInvalidationEnable
    /// @details 
    ///     Setting this bit is independent of any other bit in this packet. This
    ///     bit controls the invalidation of the texture caches at the top of the
    ///     pipe i.e. at the parsing time.
    ///     

    __CODEGEN_INLINE void SetTextureCacheInvalidationEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.TextureCacheInvalidationEnable = value;
    }

    /// @brief Read TextureCacheInvalidationEnable
    __CODEGEN_INLINE __CODEGEN_BOOL GetTextureCacheInvalidationEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.TextureCacheInvalidationEnable );
    }

    /// @brief Write InstructionCacheInvalidateEnable
    /// @details 
    ///     Setting this bit is independent of any other bit in this packet. This
    ///     bit controls the invalidation of the L1 and L2 at the top of the pipe
    ///     i.e. at the parsing time.
    ///     

    __CODEGEN_INLINE void SetInstructionCacheInvalidateEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.InstructionCacheInvalidateEnable = value;
    }

    /// @brief Read InstructionCacheInvalidateEnable
    __CODEGEN_INLINE __CODEGEN_BOOL GetInstructionCacheInvalidateEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.InstructionCacheInvalidateEnable );
    }

    /// @brief Write RenderTargetCacheFlushEnable
    /// @details 
    ///     Setting this bit will force Render Cache to be flushed to memory prior
    ///     to this synchronization point completing. This bit must be set for all
    ///     write fence sync operations to assure that results from operations
    ///     initiated prior to this command are visible in memory once software
    ///     observes this synchronization.
    ///     
    ///     Whenever a Binding Table Index (BTI) used by a Render Target Message
    ///     points to a different RENDER_SURFACE_STATE, SWmust issue a Render Target
    ///     Cache Flush by enabling this bit.
    ///     
    ///     When render target flush is set due to new association of BTI, PS
    ///     Scoreboard Stall bit must be set in this packet.
    ///     
    ///     Format: Enable
    ///     
    ///     RENDER_TARGET_CACHE_FLUSH_ENABLE_DISABLE_FLUSH (0): Render Target Cache
    ///     is NOT flushed.
    ///     
    ///     RENDER_TARGET_CACHE_FLUSH_ENABLE_ENABLE_FLUSH (1): Render Target Cache
    ///     is flushed.

    __CODEGEN_INLINE void SetRenderTargetCacheFlushEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.RenderTargetCacheFlushEnable = value;
    }

    /// @brief Read RenderTargetCacheFlushEnable
    __CODEGEN_INLINE __CODEGEN_BOOL GetRenderTargetCacheFlushEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.RenderTargetCacheFlushEnable );
    }

    /// @brief Write DepthStallEnable
    /// @details 
    ///     This bit must be set when obtaining a "visible pixel" count to preclude
    ///     the possible inclusion in the PS_DEPTH_COUNT value written to memory of
    ///     some fraction of pixels from objects initiated after the PIPE_CONTROL
    ///     command.
    ///     
    ///     This bit must be DISABLED for operations other than writing
    ///     PS_DEPTH_COUNT.
    ///     
    ///     This bit will have no effect (besides preventing write cache flush) if
    ///     set in a PIPE_CONTROL command issued to the Media pipe.
    ///     
    ///     Format: Enable
    ///     
    ///     DEPTH_STALL_ENABLE_DISABLE (0): 3D pipeline will not stall subsequent
    ///     primitives at the Depth Test stage.
    ///     
    ///     DEPTH_STALL_ENABLE_ENABLE (1): 3D pipeline will stall any subsequent
    ///     primitives at the Depth Test stage until the Sync and Post-Sync
    ///     operations complete.

    __CODEGEN_INLINE void SetDepthStallEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.DepthStallEnable = value;
    }

    /// @brief Read DepthStallEnable
    __CODEGEN_INLINE __CODEGEN_BOOL GetDepthStallEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.DepthStallEnable );
    }

    /// @brief Write PostSyncOperation
    /// @details 
    ///     This field specifies an optional action to be taken upon completion of
    ///     the synchronization operation.
    ///     
    ///     This field must be cleared if the LRI Post-Sync Operation bit is set.
    ///     
    ///     If executed in non-secure batch buffer, the address given will be in a
    ///     PPGTT address space. If in a secure ring or batch, address given will be
    ///     in GGTT space
    ///     

    __CODEGEN_INLINE void SetPostSyncOperation( const POST_SYNC_OPERATION value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.PostSyncOperation = value;
    }

    /// @brief Read PostSyncOperation
    __CODEGEN_INLINE POST_SYNC_OPERATION GetPostSyncOperation( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return static_cast<POST_SYNC_OPERATION>( TheStructure.Common.PostSyncOperation );
    }

    /// @brief Write GenericMediaStateClear
    /// @details 
    ///     If set, all generic media state context information will be invalidated.
    ///     Any state invalidated will not be saved as part of the render engine
    ///     context image. The state only becomes valid once it is parsed by the
    ///     command streamer.
    ///     

    __CODEGEN_INLINE void SetGenericMediaStateClear( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.GenericMediaStateClear = value;
    }

    /// @brief Read GenericMediaStateClear
    __CODEGEN_INLINE __CODEGEN_BOOL GetGenericMediaStateClear( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.GenericMediaStateClear );
    }

    /// @brief Write PSDSyncEnable
    /// @details 
    ///     If set, Pixel Shader Dispatch Units will stall successive PS threads
    ///     from being dispatched until all the prior PS threads complete. Once all
    ///     PSDs are synced up, post-sync LRI can be optionally used to change EU
    ///     enables.
    ///     
    ///     If this bit is set, these bits must not be set:  Depth Stall Enable
    ///      Command Streamer Stall Enable
    ///      Render Target Cache Flush Enable
    ///      Stall At Pixel Scoreboard
    ///      
    ///     

    __CODEGEN_INLINE void SetPSDSyncEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.PSDSyncEnable = value;
    }

    /// @brief Read PSDSyncEnable
    __CODEGEN_INLINE __CODEGEN_BOOL GetPSDSyncEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.PSDSyncEnable );
    }

    /// @brief Write TLBInvalidate
    /// @details 
    ///     If ENABLED, all TLBs belonging to Render Engine will be invalidated once
    ///     the flush operation is complete. Note that if the flush TLB invalidation
    ///     mode is clear, a TLB invalidate will occur irrespective of this bit
    ///     setting
    ///     
    ///     If ENABLED, PIPE_CONTROL command will flush the in flight data  written
    ///     out by render engine to Global Observation point on flush done. Also
    ///     Requires stall bit ([20] of DW1) set.
    ///     
    ///     If ENABLED, all TLBs belonging to Render Engine will be invalidated once
    ///     the flush operation is complete. Note that if the flush TLB invalidation
    ///     mode is clear, a TLB invalidate will occur irrespective of this bit
    ///     setting.
    ///     
    ///     Post Sync Operation or CS stall must be set to ensure a TLB invalidate
    ///     occurs.  Otherwise no cycle will occur to the TLB cache to invalidate.
    ///     

    __CODEGEN_INLINE void SetTLBInvalidate( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.TLBInvalidate = value;
    }

    /// @brief Read TLBInvalidate
    __CODEGEN_INLINE __CODEGEN_UINT32 GetTLBInvalidate( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.TLBInvalidate );
    }

    /// @brief Write GlobalSnapshotCountReset
    /// @details 
    ///     If set, the snapshot registers defined for the Gen4 debug capability are
    ///     reset after the flush completes. The Statistics Counters are also reset;
    ///     SW must never set this bit during normal operation since the Statistics
    ///     Counters are intended to be free running.
    ///     
    ///     This debug mode bit must not be exercised on any product.
    ///     
    ///     TIMESTAMP is not reset by PIPE_CONTROL with this bit set.
    ///      When Post Sync Operation is set to "Write PS Depth Count" along with
    ///     Global Snapshot Count Reset, PS Depth Count is Reported first before
    ///     resetting the value.
    ///     

    __CODEGEN_INLINE void SetGlobalSnapshotCountReset( const GLOBAL_SNAPSHOT_COUNT_RESET value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.GlobalSnapshotCountReset = value;
    }

    /// @brief Read GlobalSnapshotCountReset
    __CODEGEN_INLINE GLOBAL_SNAPSHOT_COUNT_RESET GetGlobalSnapshotCountReset( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return static_cast<GLOBAL_SNAPSHOT_COUNT_RESET>( TheStructure.Common.GlobalSnapshotCountReset );
    }

    /// @brief Write CommandStreamerStallEnable
    /// @details 
    ///     If ENABLED, the sync operation will not occur until all previous flush
    ///     operations pending a completion of those previous flushes will complete,
    ///     including the flush produced from this command. This enables the command
    ///     to act similar to the legacy MI_FLUSH command.
    ///     

    __CODEGEN_INLINE void SetCommandStreamerStallEnable( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.CommandStreamerStallEnable = value;
    }

    /// @brief Read CommandStreamerStallEnable
    __CODEGEN_INLINE __CODEGEN_UINT32 GetCommandStreamerStallEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.CommandStreamerStallEnable );
    }

    /// @brief Write StoreDataIndex
    /// @details 
    ///     This field is valid only if the post-sync operation is not 0. If this
    ///     bit is set, the store data address is index into the global hardware
    ///     status page when destination address type in the command is set to 1
    ///     (GGTT). The store data address is index into the per-process hardware
    ///     status page when destination address type in the command is set to 0
    ///     (PPGTT).
    ///     

    __CODEGEN_INLINE void SetStoreDataIndex( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.StoreDataIndex = value;
    }

    /// @brief Read StoreDataIndex
    __CODEGEN_INLINE __CODEGEN_UINT32 GetStoreDataIndex( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.StoreDataIndex );
    }

    /// @brief Write ProtectedMemoryEnable
    /// @details 
    ///     After completion of the flush, the hardware will limit all access to the
    ///     Protected Content Memory. Only command streamer initiated cacheable
    ///     writes are allowed to non-PCM memory.
    ///     
    ///     Enabling Protected Memory Access and disabling protected memory access
    ///     must be programmed in pairs. Enabling Protected Memory Access and
    ///     disabling protected memory access must be programmed in the same
    ///     dispatch of commands to HW.
    ///     
    ///     [DevBDW+]: Protected memory access disabled by programming Protected
    ///     Memory Disable in Pipe Control command.
    ///     
    ///     This bit is ignored if only read-only invalidate bits are set (no write
    ///     flush, depth stall, or post-sync op)
    ///     

    __CODEGEN_INLINE void SetProtectedMemoryEnable( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.ProtectedMemoryEnable = value;
    }

    /// @brief Read ProtectedMemoryEnable
    __CODEGEN_INLINE __CODEGEN_UINT32 GetProtectedMemoryEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.ProtectedMemoryEnable );
    }

    /// @brief Write LRIPostSyncOperation
    /// @details 
    ///     This bit caues a post sync operation with an LRI (Load Register
    ///     Immediate) operation. If this bit is set then the Post-Sync Operation
    ///     field must be cleared.
    ///     

    __CODEGEN_INLINE void SetLRIPostSyncOperation( const LRI_POST_SYNC_OPERATION value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.LRIPostSyncOperation = value;
    }

    /// @brief Read LRIPostSyncOperation
    __CODEGEN_INLINE LRI_POST_SYNC_OPERATION GetLRIPostSyncOperation( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return static_cast<LRI_POST_SYNC_OPERATION>( TheStructure.Common.LRIPostSyncOperation );
    }

    /// @brief Write DestinationAddressType
    /// @details 
    ///     Defines address space of Destination Address
    ///     
    ///     Ignored if ""No Write" is selected in Operation.
    ///     

    __CODEGEN_INLINE void SetDestinationAddressType( const DESTINATION_ADDRESS_TYPE value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.DestinationAddressType = value;
    }

    /// @brief Read DestinationAddressType
    __CODEGEN_INLINE DESTINATION_ADDRESS_TYPE GetDestinationAddressType( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return static_cast<DESTINATION_ADDRESS_TYPE>( TheStructure.Common.DestinationAddressType );
    }

    /// @brief Write FlushLLC
    /// @details 
    ///     If enabled, at the end of the current pipe-control the last level cache
    ///     is cleared of all the cachelines which have been determined as being
    ///     part of the Frame Buffer.
    ///     
    ///     SW must always program Post-Sync Operation to "Write Immediate Data"
    ///     when Flush LLC is set.
    ///     

    __CODEGEN_INLINE void SetFlushLLC( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.FlushLLC = value;
    }

    /// @brief Read FlushLLC
    __CODEGEN_INLINE __CODEGEN_BOOL GetFlushLLC( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.FlushLLC );
    }

    /// @brief Write ProtectedMemoryDisable
    /// @details 
    ///     On setting this bit * Command Streamer does an implied Stalling Flush
    ///     with Render,Depth and DC Caches Flushes * On completion of flush all RO
    ///     caches are invalidated. Followed by * Command streamer sends messages to
    ///     L3 and TDL to wipe out the storage contents in L3 and EUs. Followed by *
    ///     The hardware access to the Protected Content Memory is disabled. Setting
    ///     Protected Memory Enable bit along with this bit is illegal.
    ///     

    __CODEGEN_INLINE void SetProtectedMemoryDisable( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.ProtectedMemoryDisable = value;
    }

    /// @brief Read ProtectedMemoryDisable
    __CODEGEN_INLINE __CODEGEN_UINT32 GetProtectedMemoryDisable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.ProtectedMemoryDisable );
    }

    /// @brief Write TileCacheFlushEnable
    /// @details 
    ///     Setting this bit will force Tile Cache (contains both color and depth
    ///     data) to be flushed to memory prior to this synchronization point
    ///     completing. This bit must be set for all write fence sync operations to
    ///     assure that results from operations initiated prior to this command are
    ///     visible in memory once software observes this synchronization.
    ///     
    ///     
    ///     
    ///     SW must always set CS Stall bit when Tile Cache Flush Enable bit is set
    ///     in the PIPECONTROL command.
    ///     
    ///     
    ///     SW must ensure level1 depth and color caches are flushed prior to
    ///     flushing the tile cache. This can be achieved by following means:
    ///     
    ///     Single PIPECONTROL command to flush level1 caches and the tile cache.
    ///     Attributes listed below must be set. OR
    ///     
    ///     Tile Cache Flush Enable
    ///     
    ///     Render Target Cache Flush Enable
    ///     
    ///     Depth Cache Flush Enable
    ///     
    ///     
    ///     
    ///     
    ///     Flushing of L1 caches followed by flushing of tile cache through two
    ///     different PIEPCONTROL commands. SW must ensure not to issue any
    ///     rendering commands between the two PIPECONTROL commands.
    ///     
    ///     
    ///     
    ///     
    ///     
    ///     

    __CODEGEN_INLINE void SetTileCacheFlushEnable( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.TileCacheFlushEnable = value;
    }

    /// @brief Read TileCacheFlushEnable
    __CODEGEN_INLINE __CODEGEN_UINT32 GetTileCacheFlushEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.TileCacheFlushEnable );
    }

    /// @brief Write CommandCacheInvalidateEnable
    /// @details 
    ///     When set the command cache for commands parsed at the top of the pipe
    ///     will be invalidated.  This bit is independent from the other bits in
    ///     this command and will be executed prior to the pipeline being flushed.
    ///     

    __CODEGEN_INLINE void SetCommandCacheInvalidateEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.CommandCacheInvalidateEnable = value;
    }

    /// @brief Read CommandCacheInvalidateEnable
    __CODEGEN_INLINE __CODEGEN_BOOL GetCommandCacheInvalidateEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.CommandCacheInvalidateEnable );
    }

    /// @brief Write Address
    /// @details 
    ///     If Post Sync Operation is set to 1h ([DevIVB+]: LRI Post-Sync Operation
    ///     must be clear): Bits 31:3 specify the QW address of where the Immediate
    ///     Data following this DW in the packet to be stored. Bit 2 MBZ Ignored if
    ///     "No Write" is the selected in Post-Sync Operation [DevIVB+]: If LRI
    ///     Post-Sync Operation is set: Bits 22:2 (Bits 31:23 are reserved MBZ)
    ///     specify the MMIO offset destination for the data in the Immediate Data
    ///     Low (DW3) field. Only DW writes are valid.
    ///     

    typedef enum tagADDRESS
    {
        ADDRESS_BIT_SHIFT  = 2,
        ADDRESS_ALIGN_SIZE = 4,
    } ADDRESS;

    __CODEGEN_INLINE void SetAddress( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.Address = value >> ADDRESS_BIT_SHIFT;
    }

    /// @brief Read Address
    __CODEGEN_INLINE __CODEGEN_UINT32 GetAddress( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.Address << ADDRESS_BIT_SHIFT );
    }

    /// @brief Write AddressHigh
    /// @details 
    ///     This field specifies the 4GB aligned base address of gfx 4GB virtual
    ///     address space within the host's 64-bit virtual address space. This field
    ///     is valid only if the post-sync operation is not 0 and the LRI Post-Sync
    ///     Operation is clear.
    ///     

    __CODEGEN_INLINE void SetAddressHigh( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.AddressHigh = value;
    }

    /// @brief Read AddressHigh
    __CODEGEN_INLINE __CODEGEN_UINT32 GetAddressHigh( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.AddressHigh );
    }

    /// @brief Write ImmediateData
    /// @details 
    ///     This field specifies the QWord value to be written to the targeted
    ///     location.
    ///     Only valid when Post-Sync Operation is 1h (Write Immediate Data) or LRI
    ///     Post-Sync Operation is set.
    ///     Ignored if Post-Sync Operation is "No write", "Write PS_DEPTH_COUNT" or
    ///     "Write TIMESTAMP".
    ///     

    __CODEGEN_INLINE void SetImmediateData( const __CODEGEN_UINT64 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.ImmediateData = value;
    }

    /// @brief Read ImmediateData
    __CODEGEN_INLINE __CODEGEN_UINT64 GetImmediateData( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.ImmediateData );
    }

    /// @}
} __CODEGEN_ATTRIBUTES_STRUCTURE;

__CODEGEN_C_ASSERT( 24 == sizeof( PIPE_CONTROL ) );
        
//////////////////////////////////////////////////////////////////////////
/// @brief MI_STORE_DATA_IMM
/// @details
///     The MI_STORE_DATA_IMM command requests a write of the QWord constant
///     supplied in the packet to the specified Memory Address. As the write
///     targets a System Memory Address, the write operation is coherent with
///     the CPU cache (i.e., the processor cache is snooped).
///     
///     This command supports writing to multiple consecutive dwords or qwords
///     memory locations from the starting address.
///     
///      This command should not be used within a "non-privilege" batch buffer
///     to access global virtual space, doing so will be treated as privilege
///     access violation. Refer "User Mode Privilege Command" in
///     MI_BATCH_BUFFER_START command section to know HW behavior on
///     encountering privilege access violation. This command can be used within
///     ring buffers and/or privilege batch buffers to access global virtual
///     space.
///      This command can be used for general software synchronization through
///     variables in cacheable memory (i.e., where software does not need to
///     poll un-cached memory or device registers).
///      This command simply initiates the write operation with command
///     execution proceeding normally. Although the write operation is
///     guaranteed to complete eventually, there is no mechanism to synchronize
///     command execution with the completion (or even initiation) of these
///     operations.
///     
///     
///     
struct MI_STORE_DATA_IMM
{
    __CODEGEN_ACCESS_SPECIFIER_DEFINITION
    union tagTheStructure
    {
        struct tagCommon
        {
            /// DWORD 0
            __CODEGEN_UINT32         DWordLength                                      : __CODEGEN_BITFIELD( 0,  9)    ; ///< U10
            __CODEGEN_UINT32         Reserved_10                                      : __CODEGEN_BITFIELD(10, 20)    ; ///< U11
            __CODEGEN_UINT32         StoreQword                                       : __CODEGEN_BITFIELD(21, 21)    ; ///< U1
            __CODEGEN_UINT32         UseGlobalGTT                                     : __CODEGEN_BITFIELD(22, 22)    ; ///< U1
            __CODEGEN_UINT32         MICommandOpcode                                  : __CODEGEN_BITFIELD(23, 28)    ; ///< U6
            __CODEGEN_UINT32         CommandType                                      : __CODEGEN_BITFIELD(29, 31)    ; ///< U3

            /// DWORD 1..2
            __CODEGEN_UINT64         CoreModeEnable                                   : __CODEGEN_BITFIELD( 0,  0)    ; ///< U1
            __CODEGEN_UINT64         Reserved_33                                      : __CODEGEN_BITFIELD( 1,  1)    ; ///< U1
            __CODEGEN_UINT64         Address                                          : __CODEGEN_BITFIELD( 2, 47)    ; ///< U46, Sub-structure
            __CODEGEN_UINT64         Address_Reserved_80                              : __CODEGEN_BITFIELD(48, 63)    ; ///< U16, Sub-structure

            /// DWORD 3
            __CODEGEN_UINT32         DataDWord0                                                                       ; ///< U32

            /// DWORD 4
            __CODEGEN_UINT32         DataDWord1                                                                       ; ///< U32

        } Common;
        __CODEGEN_UINT32            RawData[ 5 ];
    } TheStructure;

    __CODEGEN_ACCESS_SPECIFIER_METHODS
    //////////////////////////////////////////////////////////////////////////
    /// @name STRUCTURE DEBUG
    /// @{

    /// @brief User-defined debug function for structure
    __CODEGEN_DebugType( MI_STORE_DATA_IMM )
    {
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.DWordLength                                 ,  0,  9, DWORD_LENGTH );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.StoreQword                                  , 21, 21 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.UseGlobalGTT                                , 22, 22 );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.MICommandOpcode                             , 23, 28, MI_COMMAND_OPCODE );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.CommandType                                 , 29, 31, COMMAND_TYPE );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.CoreModeEnable                              ,  0,  0 );
        __CODEGEN_DebugAttributeUInt64( TheStructure.Common.Address                                     ,  2, 47 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.DataDWord0                                  ,  0, 31 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.DataDWord1                                  ,  0, 31 );
    }
    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name LOCAL ENUMERATIONS
    /// @{

    /// @brief U10
    typedef enum tagDWORD_LENGTH
    {
        DWORD_LENGTH_STORE_DWORD                                         = 2, ///< 
        DWORD_LENGTH_STORE_QWORD                                         = 3, ///< 
    } DWORD_LENGTH;

    /// @brief U6
    typedef enum tagMI_COMMAND_OPCODE
    {
        MI_COMMAND_OPCODE_MI_STORE_DATA_IMM                              = 32, ///< 
    } MI_COMMAND_OPCODE;

    /// @brief U3
    typedef enum tagCOMMAND_TYPE
    {
        COMMAND_TYPE_MI_COMMAND                                          = 0, ///< 
    } COMMAND_TYPE;

    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name ENUMERATION DEBUG
    /// @{

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( DWORD_LENGTH )
    {
        __CODEGEN_DebugEnumValue( DWORD_LENGTH_STORE_DWORD );
        __CODEGEN_DebugEnumValue( DWORD_LENGTH_STORE_QWORD );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( MI_COMMAND_OPCODE )
    {
        __CODEGEN_DebugEnumValue( MI_COMMAND_OPCODE_MI_STORE_DATA_IMM );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( COMMAND_TYPE )
    {
        __CODEGEN_DebugEnumValue( COMMAND_TYPE_MI_COMMAND );
    }

    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name INITIALIZATION
    /// @{

    /// @brief Explicit member initialization function
    __CODEGEN_INLINE void Init( void )
    {
        TheStructure.RawData[ 0 ]                                    = 0x10000002;
        //TheStructure.Common.DWordLength                            = DWORD_LENGTH_STORE_DWORD;
        //TheStructure.Common.StoreQword                             = 0;
        //TheStructure.Common.UseGlobalGTT                           = 0;
        //TheStructure.Common.MICommandOpcode                        = MI_COMMAND_OPCODE_MI_STORE_DATA_IMM;
        //TheStructure.Common.CommandType                            = COMMAND_TYPE_MI_COMMAND;
        TheStructure.RawData[ 1 ]                                    = 0x0;
        TheStructure.RawData[ 2 ]                                    = 0x0;
        //TheStructure.Common.CoreModeEnable                         = 0x0;
        //TheStructure.Common.Address                                = 0x0;
        TheStructure.RawData[ 3 ]                                    = 0x0;
        //TheStructure.Common.DataDWord0                             = 0x0;
        TheStructure.RawData[ 4 ]                                    = 0x0;
        //TheStructure.Common.DataDWord1                             = 0x0;
    }

    /// @brief Explicit static initialization function
    static MI_STORE_DATA_IMM sInit( void ) 
    {
        MI_STORE_DATA_IMM state;
        state.Init();
        return state;
    }
    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name ACCESSORS
    /// @{

    /// @brief Raw Data Access
    __CODEGEN_INLINE __CODEGEN_UINT32& GetRawData( const __CODEGEN_UINT index )
    {
        __CODEGEN_ASSERT( index < 5 );
        return TheStructure.RawData[ index ];
    }

    /// @brief Write DWordLength
    /// @details 
    ///     DWord Length programmed must not exceed 0x3FE.
    ///     
    ///     If RS is enabled in the batch buffer, then the value of this field must
    ///     not exceed 0x3F.
    ///     

    __CODEGEN_INLINE void SetDWordLength( const DWORD_LENGTH value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.DWordLength = value;
    }

    /// @brief Read DWordLength
    __CODEGEN_INLINE DWORD_LENGTH GetDWordLength( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return static_cast<DWORD_LENGTH>( TheStructure.Common.DWordLength );
    }

    /// @brief Write StoreQword
    /// @details 
    ///     If set, this command generates Qword writes to memory, two "Data Dword"
    ///     are paired to form a Qword. Number of qwords generated depends upon the
    ///     number of "Data Dword" programmed in the command. If 'x' number of "Data
    ///     Dwords" are programmed in this command it results in "x/2" qword writes
    ///     to memory.  If reset this command generates Dwords writes to memory.
    ///     Number of dwords generated depends upon the number of "Data Dword"
    ///     programmed in the command. If 'x' number of "Data Dwords" are programmed
    ///     in this command it results in "x" dword writes to memory.
    ///     

    __CODEGEN_INLINE void SetStoreQword( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.StoreQword = value;
    }

    /// @brief Read StoreQword
    __CODEGEN_INLINE __CODEGEN_BOOL GetStoreQword( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.StoreQword );
    }

    /// @brief Write UseGlobalGTT
    /// @details 
    ///     If set, this command will use the global GTT to translate the Address
    ///     and this command must be executing from a privileged (secure) batch
    ///     buffer. If clear, the PPGTT will be used. It is allowed for this bit to
    ///     be clear when executing this command from a privileged (secure) batch
    ///     buffer. This bit must be '1' if the Per Process GTT Enable bit is clear.
    ///     

    __CODEGEN_INLINE void SetUseGlobalGTT( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.UseGlobalGTT = value;
    }

    /// @brief Read UseGlobalGTT
    __CODEGEN_INLINE __CODEGEN_BOOL GetUseGlobalGTT( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.UseGlobalGTT );
    }

    /// @brief Write CoreModeEnable
    /// @details 
    ///     This bit is set then the address will be offset by the Core ID: If Core
    ///     ID 0, then there is no offset If Core ID 1, then the Memory is offset by
    ///     the size of the data(32b or 64b based off number of DW length).
    ///     

    __CODEGEN_INLINE void SetCoreModeEnable( const __CODEGEN_UINT64 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.CoreModeEnable = value;
    }

    /// @brief Read CoreModeEnable
    __CODEGEN_INLINE __CODEGEN_UINT64 GetCoreModeEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.CoreModeEnable );
    }

    /// @brief Write Address
    /// @details 
    ///     Bits 47:2 of a 48-bit GraphicsAddress. Look for the definition of bits
    ///     1:0 in the referring register.
    ///     

    typedef enum tagADDRESS
    {
        ADDRESS_BIT_SHIFT  = 2,
        ADDRESS_ALIGN_SIZE = 4,
    } ADDRESS;

    __CODEGEN_INLINE void SetAddress( const __CODEGEN_UINT64 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.Address = value >> ADDRESS_BIT_SHIFT;
    }

    /// @brief Read Address
    __CODEGEN_INLINE __CODEGEN_UINT64 GetAddress( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.Address << ADDRESS_BIT_SHIFT );
    }

    /// @brief Write DataDWord0
    /// @details 
    ///     This field specifies the DWord value to be written to the targeted
    ///     location.For a QWord write this DWord is the lower DWord of the QWord to
    ///     be reported (DW 0).
    ///     

    __CODEGEN_INLINE void SetDataDWord0( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.DataDWord0 = value;
    }

    /// @brief Read DataDWord0
    __CODEGEN_INLINE __CODEGEN_UINT32 GetDataDWord0( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.DataDWord0 );
    }

    /// @brief Write DataDWord1
    /// @details 
    ///     This field specifies the upper DWord value to be written to the targeted
    ///     QWord location (DW 1).
    ///     

    __CODEGEN_INLINE void SetDataDWord1( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.DataDWord1 = value;
    }

    /// @brief Read DataDWord1
    __CODEGEN_INLINE __CODEGEN_UINT32 GetDataDWord1( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.DataDWord1 );
    }

    /// @}
} __CODEGEN_ATTRIBUTES_STRUCTURE;

__CODEGEN_C_ASSERT( 20 == sizeof( MI_STORE_DATA_IMM ) );
        
//////////////////////////////////////////////////////////////////////////
/// @brief MI_LOAD_REGISTER_IMM
/// @details
///     The MI_LOAD_REGISTER_IMM command requests a write of up to a DWord
///     constant supplied in the command to the specified Register Offset (i.e.,
///     offset into Memory-Mapped Register Range).
///     Any offset that is to a destination outside of the GT core will allow
///     the parser to continue once the cycle is at the GT boundary and not
///     destination. Any other address will ensure the destination is updated
///     prior to parsing the next command
///     
///     Many MMIO bits require the engine to be IDLE prior to updating the
///     value.  Command streamer does not implicitly put in a pipeline flush in
///     the cases a MMIO bit requires the engine to be IDLE.  In the case there
///     was a 3DPRIMITIVE command or GPGPU_WALKER command without any stalling
///     PIPE_CONTROL, one must be inserted prior to a MI_LOAD_REGISTER_IMMEDIATE
///     that is updating a bit that requires the engine to be IDLE.
///     
///     When executed from a non-privileged batch buffer, MMIO writes are only
///     allowed to the registers listed in User Mode Non-Privileged Registers
///     for the corresponding engine, any writes targeting the register not
///     listed in the User Mode Non-Privileged Register will convert this
///     command to a NOOP.
///     
///     The following addresses should NOT be used for LRIs:
///     
///     0x8800 - 0x88FF
///     
///     >= 0xC0000
///     
///     
///     
///     Limited LRI cycles to the Display Engine (0x40000-0xBFFFF) are allowed,
///     but must be spaced to allow only one pending at a time. This can be done
///     by issuing an SRM to the same address immediately after each LRI.
///     
///     Programming an MMIO register is equivalent to programming a non-pipeline
///     state to the hardware and hence an explicit stalling flush needs to be
///     programmed prior to programming this command. However for certain MMIO
///     registers based on their functionality doing an explicit stalling flush
///     is exempted. Listed below are the exempted registers.
///     
///     3DPRIM_END_OFFSET - Auto Draw End Offset
///     
///     3DPRIM_START_VERTEX - Load Indirect Start Vertex
///     
///     3DPRIM_VERTEX_COUNT - Load Indirect Vertex Count
///     
///     3DPRIM_INSTANCE_COUNT - Load Indirect Instance Count
///     
///     3DPRIM_START_INSTANCE - Load Indirect Start Instance
///     
///     3DPRIM_BASE_VERTEX - Load Indirect Base Vertex
///     
///     3DPRIM_XP0 - Load Indirect Extended Parameter 0
///     
///     3DPRIM_XP1 - Load Indirect Extended Parameter 1
///     
///     3DPRIM_XP2 - Load Indirect Extended Parameter 2
///     
///     
///     
struct MI_LOAD_REGISTER_IMM
{
    __CODEGEN_ACCESS_SPECIFIER_DEFINITION
    union tagTheStructure
    {
        struct tagCommon
        {
            /// DWORD 0
            __CODEGEN_UINT32         DWordLength                                      : __CODEGEN_BITFIELD( 0,  7)    ; ///< U8
            __CODEGEN_UINT32         ByteWriteDisables                                : __CODEGEN_BITFIELD( 8, 11)    ; ///< U4
            __CODEGEN_UINT32         ForcePosted                                      : __CODEGEN_BITFIELD(12, 12)    ; ///< U1
            __CODEGEN_UINT32         Reserved_13                                      : __CODEGEN_BITFIELD(13, 18)    ; ///< U6
            __CODEGEN_UINT32         AddCSMMIOStartOffset                             : __CODEGEN_BITFIELD(19, 19)    ; ///< U1
            __CODEGEN_UINT32         Reserved_20                                      : __CODEGEN_BITFIELD(20, 22)    ; ///< U3
            __CODEGEN_UINT32         MICommandOpcode                                  : __CODEGEN_BITFIELD(23, 28)    ; ///< U6
            __CODEGEN_UINT32         CommandType                                      : __CODEGEN_BITFIELD(29, 31)    ; ///< U3

            /// DWORD 1..2
            __CODEGEN_UINT64         Reserved_32                                      : __CODEGEN_BITFIELD( 0,  1)    ; ///< U2
            __CODEGEN_UINT64         RegisterOffset                                   : __CODEGEN_BITFIELD( 2, 22)    ; ///< U21
            __CODEGEN_UINT64         Reserved_55                                      : __CODEGEN_BITFIELD(23, 31)    ; ///< U9
            __CODEGEN_UINT64         DataDWord                                        : __CODEGEN_BITFIELD(32, 63)    ; ///< U32

        } Common;
        __CODEGEN_UINT32            RawData[ 3 ];
    } TheStructure;

    __CODEGEN_ACCESS_SPECIFIER_METHODS
    //////////////////////////////////////////////////////////////////////////
    /// @name STRUCTURE DEBUG
    /// @{

    /// @brief User-defined debug function for structure
    __CODEGEN_DebugType( MI_LOAD_REGISTER_IMM )
    {
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.DWordLength                                 ,  0,  7, DWORD_LENGTH );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.ByteWriteDisables                           ,  8, 11 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.ForcePosted                                 , 12, 12 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.AddCSMMIOStartOffset                        , 19, 19 );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.MICommandOpcode                             , 23, 28, MI_COMMAND_OPCODE );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.CommandType                                 , 29, 31, COMMAND_TYPE );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.RegisterOffset                              ,  2, 22 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.DataDWord                                   , 32, 63 );
    }
    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name LOCAL ENUMERATIONS
    /// @{

    /// @brief U8
    typedef enum tagDWORD_LENGTH
    {
        DWORD_LENGTH_EXCLUDES_DWORD_0_1                                  = 1, ///< 
    } DWORD_LENGTH;

    /// @brief U6
    typedef enum tagMI_COMMAND_OPCODE
    {
        MI_COMMAND_OPCODE_MI_LOAD_REGISTER_IMM                           = 34, ///< 
    } MI_COMMAND_OPCODE;

    /// @brief U3
    typedef enum tagCOMMAND_TYPE
    {
        COMMAND_TYPE_MI_COMMAND                                          = 0, ///< 
    } COMMAND_TYPE;

    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name ENUMERATION DEBUG
    /// @{

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( DWORD_LENGTH )
    {
        __CODEGEN_DebugEnumValue( DWORD_LENGTH_EXCLUDES_DWORD_0_1 );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( MI_COMMAND_OPCODE )
    {
        __CODEGEN_DebugEnumValue( MI_COMMAND_OPCODE_MI_LOAD_REGISTER_IMM );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( COMMAND_TYPE )
    {
        __CODEGEN_DebugEnumValue( COMMAND_TYPE_MI_COMMAND );
    }

    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name INITIALIZATION
    /// @{

    /// @brief Explicit member initialization function
    __CODEGEN_INLINE void Init( void )
    {
        TheStructure.RawData[ 0 ]                                    = 0x11000001;
        //TheStructure.Common.DWordLength                            = DWORD_LENGTH_EXCLUDES_DWORD_0_1;
        //TheStructure.Common.ByteWriteDisables                      = 0x0;
        //TheStructure.Common.ForcePosted                            = 0;
        //TheStructure.Common.AddCSMMIOStartOffset                   = 0x0;
        //TheStructure.Common.MICommandOpcode                        = MI_COMMAND_OPCODE_MI_LOAD_REGISTER_IMM;
        //TheStructure.Common.CommandType                            = COMMAND_TYPE_MI_COMMAND;
        TheStructure.RawData[ 1 ]                                    = 0x0;
        TheStructure.RawData[ 2 ]                                    = 0x0;
        //TheStructure.Common.RegisterOffset                         = 0x0;
        //TheStructure.Common.DataDWord                              = 0x0;
    }

    /// @brief Explicit static initialization function
    static MI_LOAD_REGISTER_IMM sInit( void ) 
    {
        MI_LOAD_REGISTER_IMM state;
        state.Init();
        return state;
    }
    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name ACCESSORS
    /// @{

    /// @brief Raw Data Access
    __CODEGEN_INLINE __CODEGEN_UINT32& GetRawData( const __CODEGEN_UINT index )
    {
        __CODEGEN_ASSERT( index < 3 );
        return TheStructure.RawData[ index ];
    }

    /// @brief Write ByteWriteDisables
    /// @details 
    ///     Range: Must specify a valid register write operation
    ///     
    ///     If [11:8] is '1111b', then this command will behave as a NOOP.
    ///     Otherwise, the value is forwarded to the destination register.
    ///     

    __CODEGEN_INLINE void SetByteWriteDisables( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.ByteWriteDisables = value;
    }

    /// @brief Read ByteWriteDisables
    __CODEGEN_INLINE __CODEGEN_UINT32 GetByteWriteDisables( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.ByteWriteDisables );
    }

    /// @brief Write ForcePosted
    /// @details 
    ///     When set CS generates posted write to the MMIO register offset.  The
    ///     last cycle will be a non-posted cycle.
    ///     
    ///     This bit is for hardware internal use and must not be set by SW while
    ///     programming MI_LOAD_REGISTER_IMM command in command buffers.
    ///     

    __CODEGEN_INLINE void SetForcePosted( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.ForcePosted = value;
    }

    /// @brief Read ForcePosted
    __CODEGEN_INLINE __CODEGEN_BOOL GetForcePosted( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.ForcePosted );
    }

    /// @brief Write AddCSMMIOStartOffset
    /// @details 
    ///     This bit controls the functionality of the "Register Address" field in
    ///     the command.
    ///     

    __CODEGEN_INLINE void SetAddCSMMIOStartOffset( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.AddCSMMIOStartOffset = value;
    }

    /// @brief Read AddCSMMIOStartOffset
    __CODEGEN_INLINE __CODEGEN_UINT32 GetAddCSMMIOStartOffset( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.AddCSMMIOStartOffset );
    }

    /// @brief Write RegisterOffset
    /// @details 
    ///     This field specifies bits [22:2] of the offset into the Memory Mapped
    ///     Register Range (i.e., this field specifies a DWord offset).
    ///     

    typedef enum tagREGISTEROFFSET
    {
        REGISTEROFFSET_BIT_SHIFT  = 2,
        REGISTEROFFSET_ALIGN_SIZE = 4,
    } REGISTEROFFSET;

    __CODEGEN_INLINE void SetRegisterOffset( const __CODEGEN_UINT64 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.RegisterOffset = value >> REGISTEROFFSET_BIT_SHIFT;
    }

    /// @brief Read RegisterOffset
    __CODEGEN_INLINE __CODEGEN_UINT64 GetRegisterOffset( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.RegisterOffset << REGISTEROFFSET_BIT_SHIFT );
    }

    /// @brief Write DataDWord
    /// @details 
    ///     This field specifies the DWord value to be written to the targeted
    ///     location.
    ///     

    __CODEGEN_INLINE void SetDataDWord( const __CODEGEN_UINT64 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.DataDWord = value;
    }

    /// @brief Read DataDWord
    __CODEGEN_INLINE __CODEGEN_UINT64 GetDataDWord( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.DataDWord );
    }

    /// @}
} __CODEGEN_ATTRIBUTES_STRUCTURE;

__CODEGEN_C_ASSERT( 12 == sizeof( MI_LOAD_REGISTER_IMM ) );
        
//////////////////////////////////////////////////////////////////////////
/// @brief MI_COPY_MEM_MEM
/// @details
///     The MI_COPY_MEM_MEM command reads a DWord from memory and stores the
///     value of that DWord to back to memory.   The source and destination
///     addresses are specified in the command. The command temporarily halts
///     command execution.
///     
///     This command should not be used within a "non-privileged" batch buffer
///     to access global virtual space; doing so will be treated as privilege
///     access violation. Refer to the "User Mode Privilege Command" in
///     MI_BATCH_BUFFER_START command section to learn more about HW behavior on
///     encountering a privilege access violation.
///     This command can be used within ring buffers and/or privilege batch
///     buffers to access global virtual space.
///     
struct MI_COPY_MEM_MEM
{
    __CODEGEN_ACCESS_SPECIFIER_DEFINITION
    union tagTheStructure
    {
        struct tagCommon
        {
            /// DWORD 0
            __CODEGEN_UINT32         DWordLength                                      : __CODEGEN_BITFIELD( 0,  7)    ; ///< U8
            __CODEGEN_UINT32         Reserved_8                                       : __CODEGEN_BITFIELD( 8, 20)    ; ///< U13
            __CODEGEN_UINT32         UseGlobalGTTDestination                          : __CODEGEN_BITFIELD(21, 21)    ; ///< U1
            __CODEGEN_UINT32         UseGlobalGTTSource                               : __CODEGEN_BITFIELD(22, 22)    ; ///< U1
            __CODEGEN_UINT32         MICommandOpcode                                  : __CODEGEN_BITFIELD(23, 28)    ; ///< U6
            __CODEGEN_UINT32         CommandType                                      : __CODEGEN_BITFIELD(29, 31)    ; ///< U3

            /// DWORD 1..2
            __CODEGEN_UINT64         Reserved_32                                      : __CODEGEN_BITFIELD( 0,  1)    ; ///< U2
            __CODEGEN_UINT64         DestinationMemoryAddress                         : __CODEGEN_BITFIELD( 2, 47)    ; ///< U46, Sub-structure
            __CODEGEN_UINT64         DestinationMemoryAddress_Reserved_80             : __CODEGEN_BITFIELD(48, 63)    ; ///< U16

            /// DWORD 3..4
            __CODEGEN_UINT64         Reserved_96                                      : __CODEGEN_BITFIELD( 0,  1)    ; ///< U2
            __CODEGEN_UINT64         SourceMemoryAddress                              : __CODEGEN_BITFIELD( 2, 47)    ; ///< U46, Sub-structure
            __CODEGEN_UINT64         SourceMemoryAddress_Reserved_144                 : __CODEGEN_BITFIELD(48, 63)    ; ///< U16, Sub-structure

        } Common;
        __CODEGEN_UINT32            RawData[ 5 ];
    } TheStructure;

    __CODEGEN_ACCESS_SPECIFIER_METHODS
    //////////////////////////////////////////////////////////////////////////
    /// @name STRUCTURE DEBUG
    /// @{

    /// @brief User-defined debug function for structure
    __CODEGEN_DebugType( MI_COPY_MEM_MEM )
    {
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.DWordLength                                 ,  0,  7, DWORD_LENGTH );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.UseGlobalGTTDestination                     , 21, 21, USE_GLOBAL_GTT_DESTINATION );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.UseGlobalGTTSource                          , 22, 22, USE_GLOBAL_GTT_SOURCE );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.MICommandOpcode                             , 23, 28, MI_COMMAND_OPCODE );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.CommandType                                 , 29, 31, COMMAND_TYPE );
        __CODEGEN_DebugAttributeUInt64( TheStructure.Common.DestinationMemoryAddress                    ,  2, 47 );
        __CODEGEN_DebugAttributeUInt64( TheStructure.Common.SourceMemoryAddress                         ,  2, 47 );
    }
    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name LOCAL ENUMERATIONS
    /// @{

    /// @brief U8
    typedef enum tagDWORD_LENGTH
    {
        DWORD_LENGTH_EXCLUDES_DWORD_0_1                                  = 3, ///< 
    } DWORD_LENGTH;

    /// @brief U1
    typedef enum tagUSE_GLOBAL_GTT_DESTINATION
    {
        USE_GLOBAL_GTT_DESTINATION_PER_PROCESS_GRAPHICS_ADDRESS          = 0, ///< 
        USE_GLOBAL_GTT_DESTINATION_GLOBAL_GRAPHICS_ADDRESS               = 1, ///< This command will use the global GTT to translate the Address and this command must be executing from a privileged (secure) batch buffer.
    } USE_GLOBAL_GTT_DESTINATION;

    /// @brief U1
    typedef enum tagUSE_GLOBAL_GTT_SOURCE
    {
        USE_GLOBAL_GTT_SOURCE_PER_PROCESS_GRAPHICS_ADDRESS               = 0, ///< 
        USE_GLOBAL_GTT_SOURCE_GLOBAL_GRAPHICS_ADDRESS                    = 1, ///< This command will use the global GTT to translate the Address and this command must be executing from a privileged (secure) batch buffer.
    } USE_GLOBAL_GTT_SOURCE;

    /// @brief U6
    typedef enum tagMI_COMMAND_OPCODE
    {
        MI_COMMAND_OPCODE_MI_MEM_TO_MEM                                  = 46, ///< 
    } MI_COMMAND_OPCODE;

    /// @brief U3
    typedef enum tagCOMMAND_TYPE
    {
        COMMAND_TYPE_MI_COMMAND                                          = 0, ///< 
    } COMMAND_TYPE;

    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name ENUMERATION DEBUG
    /// @{

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( DWORD_LENGTH )
    {
        __CODEGEN_DebugEnumValue( DWORD_LENGTH_EXCLUDES_DWORD_0_1 );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( USE_GLOBAL_GTT_DESTINATION )
    {
        __CODEGEN_DebugEnumValue( USE_GLOBAL_GTT_DESTINATION_PER_PROCESS_GRAPHICS_ADDRESS );
        __CODEGEN_DebugEnumValue( USE_GLOBAL_GTT_DESTINATION_GLOBAL_GRAPHICS_ADDRESS );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( USE_GLOBAL_GTT_SOURCE )
    {
        __CODEGEN_DebugEnumValue( USE_GLOBAL_GTT_SOURCE_PER_PROCESS_GRAPHICS_ADDRESS );
        __CODEGEN_DebugEnumValue( USE_GLOBAL_GTT_SOURCE_GLOBAL_GRAPHICS_ADDRESS );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( MI_COMMAND_OPCODE )
    {
        __CODEGEN_DebugEnumValue( MI_COMMAND_OPCODE_MI_MEM_TO_MEM );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( COMMAND_TYPE )
    {
        __CODEGEN_DebugEnumValue( COMMAND_TYPE_MI_COMMAND );
    }

    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name INITIALIZATION
    /// @{

    /// @brief Explicit member initialization function
    __CODEGEN_INLINE void Init( void )
    {
        TheStructure.RawData[ 0 ]                                    = 0x17000003;
        //TheStructure.Common.DWordLength                            = DWORD_LENGTH_EXCLUDES_DWORD_0_1;
        //TheStructure.Common.UseGlobalGTTDestination                = USE_GLOBAL_GTT_DESTINATION_PER_PROCESS_GRAPHICS_ADDRESS;
        //TheStructure.Common.UseGlobalGTTSource                     = USE_GLOBAL_GTT_SOURCE_PER_PROCESS_GRAPHICS_ADDRESS;
        //TheStructure.Common.MICommandOpcode                        = MI_COMMAND_OPCODE_MI_MEM_TO_MEM;
        //TheStructure.Common.CommandType                            = COMMAND_TYPE_MI_COMMAND;
        TheStructure.RawData[ 1 ]                                    = 0x0;
        TheStructure.RawData[ 2 ]                                    = 0x0;
        //TheStructure.Common.DestinationMemoryAddress               = 0x0;
        TheStructure.RawData[ 3 ]                                    = 0x0;
        TheStructure.RawData[ 4 ]                                    = 0x0;
        //TheStructure.Common.SourceMemoryAddress                    = 0x0;
    }

    /// @brief Explicit static initialization function
    static MI_COPY_MEM_MEM sInit( void ) 
    {
        MI_COPY_MEM_MEM state;
        state.Init();
        return state;
    }
    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name ACCESSORS
    /// @{

    /// @brief Raw Data Access
    __CODEGEN_INLINE __CODEGEN_UINT32& GetRawData( const __CODEGEN_UINT index )
    {
        __CODEGEN_ASSERT( index < 5 );
        return TheStructure.RawData[ index ];
    }

    /// @brief Write UseGlobalGTTDestination
    /// @details 
    ///     It is allowed for this bit to be set when executing this command from a
    ///     privileged (secure) batch buffer or ring buffer. This bit must be clear
    ///     when programmed from within a non-privileged batch buffer. This bit must
    ///     be 1 if the Per Process GTT Enable bit is clear.
    ///     

    __CODEGEN_INLINE void SetUseGlobalGTTDestination( const USE_GLOBAL_GTT_DESTINATION value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.UseGlobalGTTDestination = value;
    }

    /// @brief Read UseGlobalGTTDestination
    __CODEGEN_INLINE USE_GLOBAL_GTT_DESTINATION GetUseGlobalGTTDestination( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return static_cast<USE_GLOBAL_GTT_DESTINATION>( TheStructure.Common.UseGlobalGTTDestination );
    }

    /// @brief Write UseGlobalGTTSource
    /// @details 
    ///     It is allowed for this bit to be set when executing this command from a
    ///     privileged (secure) batch buffer or ring buffer. This bit must be clear
    ///     when programmed from within a non-privileged batch buffer. This bit must
    ///     be 1 if the Per Process GTT Enable bit is clear.
    ///     

    __CODEGEN_INLINE void SetUseGlobalGTTSource( const USE_GLOBAL_GTT_SOURCE value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.UseGlobalGTTSource = value;
    }

    /// @brief Read UseGlobalGTTSource
    __CODEGEN_INLINE USE_GLOBAL_GTT_SOURCE GetUseGlobalGTTSource( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return static_cast<USE_GLOBAL_GTT_SOURCE>( TheStructure.Common.UseGlobalGTTSource );
    }

    /// @brief Write DestinationMemoryAddress
    /// @details 
    ///     Bits 47:2 of a 48-bit GraphicsAddress. Look for the definition of bits
    ///     1:0 in the referring register.
    ///     

    typedef enum tagDESTINATIONMEMORYADDRESS
    {
        DESTINATIONMEMORYADDRESS_BIT_SHIFT  = 2,
        DESTINATIONMEMORYADDRESS_ALIGN_SIZE = 4,
    } DESTINATIONMEMORYADDRESS;

    __CODEGEN_INLINE void SetDestinationMemoryAddress( const __CODEGEN_UINT64 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.DestinationMemoryAddress = value >> DESTINATIONMEMORYADDRESS_BIT_SHIFT;
    }

    /// @brief Read DestinationMemoryAddress
    __CODEGEN_INLINE __CODEGEN_UINT64 GetDestinationMemoryAddress( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.DestinationMemoryAddress << DESTINATIONMEMORYADDRESS_BIT_SHIFT );
    }

    /// @brief Write SourceMemoryAddress
    /// @details 
    ///     Bits 47:2 of a 48-bit GraphicsAddress. Look for the definition of bits
    ///     1:0 in the referring register.
    ///     

    typedef enum tagSOURCEMEMORYADDRESS
    {
        SOURCEMEMORYADDRESS_BIT_SHIFT  = 2,
        SOURCEMEMORYADDRESS_ALIGN_SIZE = 4,
    } SOURCEMEMORYADDRESS;

    __CODEGEN_INLINE void SetSourceMemoryAddress( const __CODEGEN_UINT64 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.SourceMemoryAddress = value >> SOURCEMEMORYADDRESS_BIT_SHIFT;
    }

    /// @brief Read SourceMemoryAddress
    __CODEGEN_INLINE __CODEGEN_UINT64 GetSourceMemoryAddress( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.SourceMemoryAddress << SOURCEMEMORYADDRESS_BIT_SHIFT );
    }

    /// @}
} __CODEGEN_ATTRIBUTES_STRUCTURE;

__CODEGEN_C_ASSERT( 20 == sizeof( MI_COPY_MEM_MEM ) );

//////////////////////////////////////////////////////////////////////////
/// @brief MI_LOAD_REGISTER_REG
/// @details
///     The MI_LOAD_REGISTER_REG command reads from a source register location
///     and writes that value to a destination register location.
///       
///     Any offset that is to a destination outside of the GT core will allow
///     the parser to continue once the cycle is at the GT boundry and not
///     destination. Any other address will ensure the destination is updated
///     prior to parsing the next command
///       
///     
///     The command temporarily halts commands that will cause cycles down the
///     3D pipeline.
///     
///     Destination register with mask implemented (Ex: Some registers have bits
///     [31:16] as mask bits and bits[15:0] as data) will not get updated unless
///     the  value read from source register has the bits corresponding to the
///     mask bits set. Note that any mask implemented register when read returns
///     "0" for the bits corresponding to mask location. When the source and
///     destination are mask implemented registers, destination register will
///     not get updated with the source register contents.
///     
///     This command is not allowed to update the privilege register range when
///     executed from a non-privilege batch buffer.
///     
struct MI_LOAD_REGISTER_REG
{
    __CODEGEN_ACCESS_SPECIFIER_DEFINITION
    union tagTheStructure
    {
        struct tagCommon
        {
            /// DWORD 0
            __CODEGEN_UINT32         DWordLength                                      : __CODEGEN_BITFIELD( 0,  7)    ; ///< U8
            __CODEGEN_UINT32         Reserved_8                                       : __CODEGEN_BITFIELD( 8, 22)    ; ///< U15
            __CODEGEN_UINT32         MICommandOpcode                                  : __CODEGEN_BITFIELD(23, 28)    ; ///< U6
            __CODEGEN_UINT32         CommandType                                      : __CODEGEN_BITFIELD(29, 31)    ; ///< U3

            /// DWORD 1
            __CODEGEN_UINT32         Reserved_32                                      : __CODEGEN_BITFIELD( 0,  1)    ; ///< U2
            __CODEGEN_UINT32         SourceRegisterAddress                            : __CODEGEN_BITFIELD( 2, 22)    ; ///< U21
            __CODEGEN_UINT32         Reserved_55                                      : __CODEGEN_BITFIELD(23, 31)    ; ///< U9

            /// DWORD 2
            __CODEGEN_UINT32         Reserved_64                                      : __CODEGEN_BITFIELD( 0,  1)    ; ///< U2
            __CODEGEN_UINT32         DestinationRegisterAddress                       : __CODEGEN_BITFIELD( 2, 22)    ; ///< U21
            __CODEGEN_UINT32         Reserved_87                                      : __CODEGEN_BITFIELD(23, 31)    ; ///< U9

        } Common;
        __CODEGEN_UINT32            RawData[ 3 ];
    } TheStructure;

    __CODEGEN_ACCESS_SPECIFIER_METHODS
    //////////////////////////////////////////////////////////////////////////
    /// @name STRUCTURE DEBUG
    /// @{

    /// @brief User-defined debug function for structure
    __CODEGEN_DebugType( MI_LOAD_REGISTER_REG )
    {
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.DWordLength                                 ,  0,  7, DWORD_LENGTH );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.MICommandOpcode                             , 23, 28, MI_COMMAND_OPCODE );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.CommandType                                 , 29, 31, COMMAND_TYPE );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.SourceRegisterAddress                       ,  2, 22 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.DestinationRegisterAddress                  ,  2, 22 );
    }
    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name LOCAL ENUMERATIONS
    /// @{

    /// @brief U8
    typedef enum tagDWORD_LENGTH
    {
        DWORD_LENGTH_EXCLUDES_DWORD_0_1                                  = 1, ///< 
    } DWORD_LENGTH;

    /// @brief U6
    typedef enum tagMI_COMMAND_OPCODE
    {
        MI_COMMAND_OPCODE_MI_LOAD_REGISTER_REG                           = 42, ///< 
    } MI_COMMAND_OPCODE;

    /// @brief U3
    typedef enum tagCOMMAND_TYPE
    {
        COMMAND_TYPE_MI_COMMAND                                          = 0, ///< 
    } COMMAND_TYPE;

    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name ENUMERATION DEBUG
    /// @{

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( DWORD_LENGTH )
    {
        __CODEGEN_DebugEnumValue( DWORD_LENGTH_EXCLUDES_DWORD_0_1 );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( MI_COMMAND_OPCODE )
    {
        __CODEGEN_DebugEnumValue( MI_COMMAND_OPCODE_MI_LOAD_REGISTER_REG );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( COMMAND_TYPE )
    {
        __CODEGEN_DebugEnumValue( COMMAND_TYPE_MI_COMMAND );
    }

    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name INITIALIZATION
    /// @{

    /// @brief Explicit member initialization function
    __CODEGEN_INLINE void Init( void )
    {
        TheStructure.RawData[ 0 ]                                    = 0x15000001;
        //TheStructure.Common.DWordLength                            = DWORD_LENGTH_EXCLUDES_DWORD_0_1;
        //TheStructure.Common.MICommandOpcode                        = MI_COMMAND_OPCODE_MI_LOAD_REGISTER_REG;
        //TheStructure.Common.CommandType                            = COMMAND_TYPE_MI_COMMAND;
        TheStructure.RawData[ 1 ]                                    = 0x0;
        //TheStructure.Common.SourceRegisterAddress                  = 0x0;
        TheStructure.RawData[ 2 ]                                    = 0x0;
        //TheStructure.Common.DestinationRegisterAddress             = 0x0;
    }

    /// @brief Explicit static initialization function
    static MI_LOAD_REGISTER_REG sInit( void ) 
    {
        MI_LOAD_REGISTER_REG state;
        state.Init();
        return state;
    }
    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name ACCESSORS
    /// @{

    /// @brief Raw Data Access
    __CODEGEN_INLINE __CODEGEN_UINT32& GetRawData( const __CODEGEN_UINT index )
    {
        __CODEGEN_ASSERT( index < 3 );
        return TheStructure.RawData[ index ];
    }

    /// @brief Write SourceRegisterAddress
    /// @details 
    ///     This field specifies Bits 22:2 of the Register offset the DWord will be
    ///     written to. As the register address must be DWord-aligned, Bits 1:0 of
    ///     that address MBZ.
    ///     

    typedef enum tagSOURCEREGISTERADDRESS
    {
        SOURCEREGISTERADDRESS_BIT_SHIFT  = 2,
        SOURCEREGISTERADDRESS_ALIGN_SIZE = 4,
    } SOURCEREGISTERADDRESS;

    __CODEGEN_INLINE void SetSourceRegisterAddress( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.SourceRegisterAddress = value >> SOURCEREGISTERADDRESS_BIT_SHIFT;
    }

    /// @brief Read SourceRegisterAddress
    __CODEGEN_INLINE __CODEGEN_UINT32 GetSourceRegisterAddress( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.SourceRegisterAddress << SOURCEREGISTERADDRESS_BIT_SHIFT );
    }

    /// @brief Write DestinationRegisterAddress
    /// @details 
    ///     This field specifies Bits 22:2 of the Register offset the DWord will be
    ///     written to. As the register address must be DWord-aligned, Bits 1:0 of
    ///     that address MBZ.
    ///     

    typedef enum tagDESTINATIONREGISTERADDRESS
    {
        DESTINATIONREGISTERADDRESS_BIT_SHIFT  = 2,
        DESTINATIONREGISTERADDRESS_ALIGN_SIZE = 4,
    } DESTINATIONREGISTERADDRESS;

    __CODEGEN_INLINE void SetDestinationRegisterAddress( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.DestinationRegisterAddress = value >> DESTINATIONREGISTERADDRESS_BIT_SHIFT;
    }

    /// @brief Read DestinationRegisterAddress
    __CODEGEN_INLINE __CODEGEN_UINT32 GetDestinationRegisterAddress( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.DestinationRegisterAddress << DESTINATIONREGISTERADDRESS_BIT_SHIFT );
    }

    /// @}
} __CODEGEN_ATTRIBUTES_STRUCTURE;

__CODEGEN_C_ASSERT( 12 == sizeof( MI_LOAD_REGISTER_REG ) );

//////////////////////////////////////////////////////////////////////////
/// @brief MI_NOOP
/// @details
///     The MI_NOOP command basically performs a "no operation" in the command
///     stream and is typically used to pad the command stream (e.g., in order
///     to pad out a batch buffer to a QWord boundary). However, there is one
///     minor (optional) function this command can perform - a 22-bit value can
///     be loaded into the MI NOPID register. This provides a general-purpose
///     command stream tagging ("breadcrumb") mechanism (e.g., to provide
///     sequencing information for a subsequent breakpoint interrupt).
///     
struct MI_NOOP
{
    __CODEGEN_ACCESS_SPECIFIER_DEFINITION
    union tagTheStructure
    {
        struct tagCommon
        {
            /// DWORD 0
            __CODEGEN_UINT32         IdentificationNumber                             : __CODEGEN_BITFIELD( 0, 21)    ; ///< U22
            __CODEGEN_UINT32         IdentificationNumberRegisterWriteEnable          : __CODEGEN_BITFIELD(22, 22)    ; ///< U1
            __CODEGEN_UINT32         MICommandOpcode                                  : __CODEGEN_BITFIELD(23, 28)    ; ///< U6
            __CODEGEN_UINT32         CommandType                                      : __CODEGEN_BITFIELD(29, 31)    ; ///< U3

        } Common;
        __CODEGEN_UINT32            RawData[ 1 ];
    } TheStructure;

    __CODEGEN_ACCESS_SPECIFIER_METHODS
    //////////////////////////////////////////////////////////////////////////
    /// @name STRUCTURE DEBUG
    /// @{

    /// @brief User-defined debug function for structure
    __CODEGEN_DebugType( MI_NOOP )
    {
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.IdentificationNumber                        ,  0, 21 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.IdentificationNumberRegisterWriteEnable     , 22, 22 );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.MICommandOpcode                             , 23, 28, MI_COMMAND_OPCODE );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.CommandType                                 , 29, 31, COMMAND_TYPE );
    }
    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name LOCAL ENUMERATIONS
    /// @{

    /// @brief U6
    typedef enum tagMI_COMMAND_OPCODE
    {
        MI_COMMAND_OPCODE_MI_NOOP                                        = 0, ///<
    } MI_COMMAND_OPCODE;

    /// @brief U3
    typedef enum tagCOMMAND_TYPE
    {
        COMMAND_TYPE_MI_COMMAND                                          = 0, ///<
    } COMMAND_TYPE;

    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name ENUMERATION DEBUG
    /// @{

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( MI_COMMAND_OPCODE )
    {
        __CODEGEN_DebugEnumValue( MI_COMMAND_OPCODE_MI_NOOP );
    }

    /// @brief User-defined debug function for enumeration
    __CODEGEN_DebugEnum( COMMAND_TYPE )
    {
        __CODEGEN_DebugEnumValue( COMMAND_TYPE_MI_COMMAND );
    }

    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name INITIALIZATION
    /// @{

    /// @brief Explicit member initialization function
    __CODEGEN_INLINE void Init( void )
    {
        TheStructure.RawData[ 0 ]                                    = 0x0;
        //TheStructure.Common.IdentificationNumber                   = 0x0;
        //TheStructure.Common.IdentificationNumberRegisterWriteEnable = 0;
        //TheStructure.Common.MICommandOpcode                        = MI_COMMAND_OPCODE_MI_NOOP;
        //TheStructure.Common.CommandType                            = COMMAND_TYPE_MI_COMMAND;
    }

    /// @brief Explicit static initialization function
    static MI_NOOP sInit( void )
    {
        MI_NOOP state;
        state.Init();
        return state;
    }
    /// @}

    //////////////////////////////////////////////////////////////////////////
    /// @name ACCESSORS
    /// @{

    /// @brief Raw Data Access
    __CODEGEN_INLINE __CODEGEN_UINT32& GetRawData( const __CODEGEN_UINT index )
    {
        __CODEGEN_ASSERT( index < 1 );
        return TheStructure.RawData[ index ];
    }

    /// @brief Write IdentificationNumber
    /// @details
    ///     This field contains a 22-bit number which can be written to the MI NOPID
    ///     register.
    ///

    __CODEGEN_INLINE void SetIdentificationNumber( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.IdentificationNumber = value;
    }

    /// @brief Read IdentificationNumber
    __CODEGEN_INLINE __CODEGEN_UINT32 GetIdentificationNumber( void ) const
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.IdentificationNumber );
    }

    /// @brief Write IdentificationNumberRegisterWriteEnable
    /// @details
    ///     This field enables the value in the Identification Number field to be
    ///     written into the MI NOPID register. If disabled, that register is
    ///     unmodified, making this command an effective "no operation" function.
    ///
    ///     Format: Enable

    __CODEGEN_INLINE void SetIdentificationNumberRegisterWriteEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.IdentificationNumberRegisterWriteEnable = value;
    }

    /// @brief Read IdentificationNumberRegisterWriteEnable
    __CODEGEN_INLINE __CODEGEN_BOOL GetIdentificationNumberRegisterWriteEnable( void ) const
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.IdentificationNumberRegisterWriteEnable );
    }

    /// @}
} __CODEGEN_ATTRIBUTES_STRUCTURE;

__CODEGEN_C_ASSERT( 4 == sizeof( MI_NOOP ) );
        
__CODEGEN_NAMESPACE_CLOSE
__CODEGEN_FILE_DIRECTIVES_CLOSE

#undef __CODEGEN_FILE_DIRECTIVES_OPEN
#undef __CODEGEN_FILE_DIRECTIVES_CLOSE
#undef __CODEGEN_NAMESPACE_OPEN
#undef __CODEGEN_NAMESPACE_CLOSE
#undef __CODEGEN_ACCESS_SPECIFIER_DEFINITION
#undef __CODEGEN_ACCESS_SPECIFIER_METHODS
#undef __CODEGEN_ATTRIBUTES_STRUCTURE
#undef __CODEGEN_INLINE
#undef __CODEGEN_UINT16
#undef __CODEGEN_UINT
#undef __CODEGEN_UINT32
#undef __CODEGEN_UINT64
#undef __CODEGEN_FLOAT32
#undef __CODEGEN_BOOL
#undef __CODEGEN_GPUVA
#undef __CODEGEN_GPUSIZE
#undef __CODEGEN_DebugType
#undef __CODEGEN_DebugAttributeEnum
#undef __CODEGEN_DebugAttributeUInt
#undef __CODEGEN_DebugAttributeUInt64
#undef __CODEGEN_DebugAttributeBool
#undef __CODEGEN_DebugAttributeFloat
#undef __CODEGEN_DebugAttributeStruct
#undef __CODEGEN_DebugAttributeArray
#undef __CODEGEN_DebugEnum
#undef __CODEGEN_DebugEnumValue
#undef __CODEGEN_GET_MACRO
#undef __CODEGEN_SET_MACRO
#undef __CODEGEN_C_ASSERT
#undef __CODEGEN_ASSERT

#undef __CODEGEN_BITFIELD
#undef __CODEGEN_OP_LENGTH_BIAS
#undef __CODEGEN_OP_LENGTH
#undef __CODEGEN_OP_SIZE