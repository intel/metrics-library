/*========================== begin_copyright_notice ============================

Copyright (C) 2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_gpu_commands_xe2_hpg.h

@brief auto-generated file

DO NOT EDIT
*/

#ifndef __CODEGEN_FILE_DIRECTIVES_OPEN
#define __CODEGEN_FILE_DIRECTIVES_OPEN __pragma(pack(push, 1))
#endif 

#ifndef __CODEGEN_FILE_DIRECTIVES_CLOSE
#define __CODEGEN_FILE_DIRECTIVES_CLOSE __pragma(pack(pop))
#endif 

#ifndef __CODEGEN_NAMESPACE_OPEN
#define __CODEGEN_NAMESPACE_OPEN
#endif 

#ifndef __CODEGEN_NAMESPACE_CLOSE
#define __CODEGEN_NAMESPACE_CLOSE
#endif 

#ifndef __CODEGEN_ACCESS_SPECIFIER_DEFINITION
#define __CODEGEN_ACCESS_SPECIFIER_DEFINITION
#endif 

#ifndef __CODEGEN_ACCESS_SPECIFIER_METHODS
#define __CODEGEN_ACCESS_SPECIFIER_METHODS
#endif 

#ifndef __CODEGEN_ATTRIBUTES_STRUCTURE
#define __CODEGEN_ATTRIBUTES_STRUCTURE
#endif 

#ifndef __CODEGEN_INLINE
#define __CODEGEN_INLINE __inline
#endif 

#ifndef __CODEGEN_UINT16
#define __CODEGEN_UINT16 uint16_t
#endif 

#ifndef __CODEGEN_UINT
#define __CODEGEN_UINT uint32_t
#endif 

#ifndef __CODEGEN_UINT32
#define __CODEGEN_UINT32 uint32_t
#endif 

#ifndef __CODEGEN_UINT64
#define __CODEGEN_UINT64 uint64_t
#endif 

#ifndef __CODEGEN_FLOAT32
#define __CODEGEN_FLOAT32 float
#endif 

#ifndef __CODEGEN_BOOL
#define __CODEGEN_BOOL uint32_t
#endif 

#ifndef __CODEGEN_GPUVA
#define __CODEGEN_GPUVA uint64_t
#endif 

#ifndef __CODEGEN_GPUSIZE
#define __CODEGEN_GPUSIZE uint64_t
#endif 

#ifndef __CODEGEN_DebugType
#define __CODEGEN_DebugType( type )  static __CODEGEN_INLINE void DebugStruct( type s )
#endif 

#ifndef __CODEGEN_DebugAttributeEnum
#define __CODEGEN_DebugAttributeEnum( name, lo, hi, type )
#endif 

#ifndef __CODEGEN_DebugAttributeUInt
#define __CODEGEN_DebugAttributeUInt( name, lo, hi )
#endif 

#ifndef __CODEGEN_DebugAttributeUInt64
#define __CODEGEN_DebugAttributeUInt64( name, lo, hi )
#endif 

#ifndef __CODEGEN_DebugAttributeBool
#define __CODEGEN_DebugAttributeBool( name, lo, hi )
#endif 

#ifndef __CODEGEN_DebugAttributeFloat
#define __CODEGEN_DebugAttributeFloat( name )
#endif 

#ifndef __CODEGEN_DebugAttributeStruct
#define __CODEGEN_DebugAttributeStruct( name )
#endif 

#ifndef __CODEGEN_DebugAttributeArray
#define __CODEGEN_DebugAttributeArray( name )
#endif 

#ifndef __CODEGEN_DebugEnum
#define __CODEGEN_DebugEnum( type ) static __CODEGEN_INLINE void DebugEnum( type e )
#endif 

#ifndef __CODEGEN_DebugEnumValue
#define __CODEGEN_DebugEnumValue( name )
#endif 

#ifndef __CODEGEN_GET_MACRO
#define __CODEGEN_GET_MACRO()
#endif 

#ifndef __CODEGEN_SET_MACRO
#define __CODEGEN_SET_MACRO( name )
#endif 

#ifndef __CODEGEN_C_ASSERT
#define __CODEGEN_C_ASSERT( x )
#endif 

#ifndef __CODEGEN_ASSERT
#define __CODEGEN_ASSERT( x )
#endif 

#define __CODEGEN_BITFIELD(l,h) h-l+1
#define __CODEGEN_OP_LENGTH_BIAS 2
#define __CODEGEN_OP_LENGTH( x ) (((x) / sizeof(__CODEGEN_UINT32)) - __CODEGEN_OP_LENGTH_BIAS)
#define __CODEGEN_OP_SIZE( x ) (((x) + __CODEGEN_OP_LENGTH_BIAS) * sizeof(__CODEGEN_UINT32))

__CODEGEN_FILE_DIRECTIVES_OPEN
__CODEGEN_NAMESPACE_OPEN

struct MI_REPORT_PERF_COUNT
{
    __CODEGEN_ACCESS_SPECIFIER_DEFINITION
    union tagTheStructure
    {
        struct tagCommon
        {
            __CODEGEN_UINT32         DWordLength                                      : __CODEGEN_BITFIELD( 0,  5)    ; 
            __CODEGEN_UINT32         Reserved_6                                       : __CODEGEN_BITFIELD( 6, 22)    ; 
            __CODEGEN_UINT32         MICommandOpcode                                  : __CODEGEN_BITFIELD(23, 28)    ; 
            __CODEGEN_UINT32         CommandType                                      : __CODEGEN_BITFIELD(29, 31)    ; 

            __CODEGEN_UINT64         UseGlobalGTT                                     : __CODEGEN_BITFIELD( 0,  0)    ; 
            __CODEGEN_UINT64         Reserved_33                                      : __CODEGEN_BITFIELD( 1,  3)    ; 
            __CODEGEN_UINT64         CoreModeEnable                                   : __CODEGEN_BITFIELD( 4,  4)    ; 
            __CODEGEN_UINT64         Reserved_37                                      : __CODEGEN_BITFIELD( 5,  5)    ; 
            __CODEGEN_UINT64         MemoryAddress                                    : __CODEGEN_BITFIELD( 6, 63)    ; 

            __CODEGEN_UINT32         ReportID                                                                         ; 

        } Common;
        __CODEGEN_UINT32            RawData[ 4 ];
    } TheStructure;

    __CODEGEN_ACCESS_SPECIFIER_METHODS

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


    typedef enum tagDWORD_LENGTH
    {
        DWORD_LENGTH_EXCLUDES_DWORD_0_1                                  = 2, 
    } DWORD_LENGTH;

    typedef enum tagMI_COMMAND_OPCODE
    {
        MI_COMMAND_OPCODE_MI_REPORT_PERF_COUNT                           = 40, 
    } MI_COMMAND_OPCODE;

    typedef enum tagCOMMAND_TYPE
    {
        COMMAND_TYPE_MI_COMMAND                                          = 0, 
    } COMMAND_TYPE;



    __CODEGEN_DebugEnum( DWORD_LENGTH )
    {
        __CODEGEN_DebugEnumValue( DWORD_LENGTH_EXCLUDES_DWORD_0_1 );
    }

    __CODEGEN_DebugEnum( MI_COMMAND_OPCODE )
    {
        __CODEGEN_DebugEnumValue( MI_COMMAND_OPCODE_MI_REPORT_PERF_COUNT );
    }

    __CODEGEN_DebugEnum( COMMAND_TYPE )
    {
        __CODEGEN_DebugEnumValue( COMMAND_TYPE_MI_COMMAND );
    }



    __CODEGEN_INLINE void Init( void )
    {
        TheStructure.RawData[ 0 ]                                    = 0x14000002;
        TheStructure.RawData[ 1 ]                                    = 0x0;
        TheStructure.RawData[ 2 ]                                    = 0x0;
        TheStructure.RawData[ 3 ]                                    = 0x0;
    }

    static MI_REPORT_PERF_COUNT sInit( void ) 
    {
        MI_REPORT_PERF_COUNT state;
        state.Init();
        return state;
    }


    __CODEGEN_INLINE __CODEGEN_UINT32& GetRawData( const __CODEGEN_UINT index )
    {
        __CODEGEN_ASSERT( index < 4 );
        return TheStructure.RawData[ index ];
    }


    __CODEGEN_INLINE void SetUseGlobalGTT( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.UseGlobalGTT = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetUseGlobalGTT( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.UseGlobalGTT );
    }


    __CODEGEN_INLINE void SetCoreModeEnable( const __CODEGEN_UINT64 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.CoreModeEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT64 GetCoreModeEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.CoreModeEnable );
    }


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

    __CODEGEN_INLINE __CODEGEN_UINT64 GetMemoryAddress( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.MemoryAddress << MEMORYADDRESS_BIT_SHIFT );
    }


    __CODEGEN_INLINE void SetReportID( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.ReportID = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetReportID( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.ReportID );
    }

} __CODEGEN_ATTRIBUTES_STRUCTURE;

__CODEGEN_C_ASSERT( 16 == sizeof( MI_REPORT_PERF_COUNT ) );
        
struct MI_STORE_REGISTER_MEM
{
    __CODEGEN_ACCESS_SPECIFIER_DEFINITION
    union tagTheStructure
    {
        struct tagCommon
        {
            __CODEGEN_UINT32         DWordLength                                      : __CODEGEN_BITFIELD( 0,  7)    ; 
            __CODEGEN_UINT32         Reserved_8                                       : __CODEGEN_BITFIELD( 8, 15)    ; 
            __CODEGEN_UINT32         WorkloadPartitionIDOffsetEnable                  : __CODEGEN_BITFIELD(16, 16)    ; 
            __CODEGEN_UINT32         MMIORemapEnable                                  : __CODEGEN_BITFIELD(17, 17)    ; 
            __CODEGEN_UINT32         Reserved_18                                      : __CODEGEN_BITFIELD(18, 18)    ; 
            __CODEGEN_UINT32         AddCSMMIOStartOffset                             : __CODEGEN_BITFIELD(19, 19)    ; 
            __CODEGEN_UINT32         Reserved_20                                      : __CODEGEN_BITFIELD(20, 20)    ; 
            __CODEGEN_UINT32         PredicateEnable                                  : __CODEGEN_BITFIELD(21, 21)    ; 
            __CODEGEN_UINT32         UseGlobalGTT                                     : __CODEGEN_BITFIELD(22, 22)    ; 
            __CODEGEN_UINT32         MICommandOpcode                                  : __CODEGEN_BITFIELD(23, 28)    ; 
            __CODEGEN_UINT32         CommandType                                      : __CODEGEN_BITFIELD(29, 31)    ; 

            __CODEGEN_UINT32         Reserved_32                                      : __CODEGEN_BITFIELD( 0,  1)    ; 
            __CODEGEN_UINT32         RegisterAddress                                  : __CODEGEN_BITFIELD( 2, 22)    ; 
            __CODEGEN_UINT32         Reserved_55                                      : __CODEGEN_BITFIELD(23, 31)    ; 

            __CODEGEN_UINT64         Reserved_64                                      : __CODEGEN_BITFIELD( 0,  1)    ; 
            __CODEGEN_UINT64         MemoryAddress                                    : __CODEGEN_BITFIELD( 2, 63)    ; 

        } Common;
        __CODEGEN_UINT32            RawData[ 4 ];
    } TheStructure;

    __CODEGEN_ACCESS_SPECIFIER_METHODS

    __CODEGEN_DebugType( MI_STORE_REGISTER_MEM )
    {
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.DWordLength                                 ,  0,  7, DWORD_LENGTH );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.WorkloadPartitionIDOffsetEnable             , 16, 16 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.MMIORemapEnable                             , 17, 17 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.AddCSMMIOStartOffset                        , 19, 19 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.PredicateEnable                             , 21, 21 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.UseGlobalGTT                                , 22, 22 );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.MICommandOpcode                             , 23, 28, MI_COMMAND_OPCODE );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.CommandType                                 , 29, 31, COMMAND_TYPE );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.RegisterAddress                             ,  2, 22 );
        __CODEGEN_DebugAttributeUInt64( TheStructure.Common.MemoryAddress                               ,  2, 63 );
    }


    typedef enum tagDWORD_LENGTH
    {
        DWORD_LENGTH_EXCLUDES_DWORD_0_1                                  = 2, 
    } DWORD_LENGTH;

    typedef enum tagMI_COMMAND_OPCODE
    {
        MI_COMMAND_OPCODE_MI_STORE_REGISTER_MEM                          = 36, 
    } MI_COMMAND_OPCODE;

    typedef enum tagCOMMAND_TYPE
    {
        COMMAND_TYPE_MI_COMMAND                                          = 0, 
    } COMMAND_TYPE;



    __CODEGEN_DebugEnum( DWORD_LENGTH )
    {
        __CODEGEN_DebugEnumValue( DWORD_LENGTH_EXCLUDES_DWORD_0_1 );
    }

    __CODEGEN_DebugEnum( MI_COMMAND_OPCODE )
    {
        __CODEGEN_DebugEnumValue( MI_COMMAND_OPCODE_MI_STORE_REGISTER_MEM );
    }

    __CODEGEN_DebugEnum( COMMAND_TYPE )
    {
        __CODEGEN_DebugEnumValue( COMMAND_TYPE_MI_COMMAND );
    }



    __CODEGEN_INLINE void Init( void )
    {
        TheStructure.RawData[ 0 ]                                    = 0x12000002;
        TheStructure.RawData[ 1 ]                                    = 0x0;
        TheStructure.RawData[ 2 ]                                    = 0x0;
        TheStructure.RawData[ 3 ]                                    = 0x0;
    }

    static MI_STORE_REGISTER_MEM sInit( void ) 
    {
        MI_STORE_REGISTER_MEM state;
        state.Init();
        return state;
    }


    __CODEGEN_INLINE __CODEGEN_UINT32& GetRawData( const __CODEGEN_UINT index )
    {
        __CODEGEN_ASSERT( index < 4 );
        return TheStructure.RawData[ index ];
    }


    __CODEGEN_INLINE void SetWorkloadPartitionIDOffsetEnable( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.WorkloadPartitionIDOffsetEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetWorkloadPartitionIDOffsetEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.WorkloadPartitionIDOffsetEnable );
    }


    __CODEGEN_INLINE void SetMMIORemapEnable( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.MMIORemapEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetMMIORemapEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.MMIORemapEnable );
    }


    __CODEGEN_INLINE void SetAddCSMMIOStartOffset( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.AddCSMMIOStartOffset = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetAddCSMMIOStartOffset( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.AddCSMMIOStartOffset );
    }


    __CODEGEN_INLINE void SetPredicateEnable( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.PredicateEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetPredicateEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.PredicateEnable );
    }


    __CODEGEN_INLINE void SetUseGlobalGTT( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.UseGlobalGTT = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetUseGlobalGTT( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.UseGlobalGTT );
    }


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

    __CODEGEN_INLINE __CODEGEN_UINT32 GetRegisterAddress( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.RegisterAddress << REGISTERADDRESS_BIT_SHIFT );
    }


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

    __CODEGEN_INLINE __CODEGEN_UINT64 GetMemoryAddress( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.MemoryAddress << MEMORYADDRESS_BIT_SHIFT );
    }

} __CODEGEN_ATTRIBUTES_STRUCTURE;

__CODEGEN_C_ASSERT( 16 == sizeof( MI_STORE_REGISTER_MEM ) );
        
struct PIPE_CONTROL
{
    __CODEGEN_ACCESS_SPECIFIER_DEFINITION
    union tagTheStructure
    {
        struct tagCommon
        {
            __CODEGEN_UINT32         DWordLength                                      : __CODEGEN_BITFIELD( 0,  7)    ; 
            __CODEGEN_UINT32         PredicateEnable                                  : __CODEGEN_BITFIELD( 8,  8)    ; 
            __CODEGEN_UINT32         DataportFlush                                    : __CODEGEN_BITFIELD( 9,  9)    ; 
            __CODEGEN_UINT32         L3ReadOnlyCacheInvalidationEnable                : __CODEGEN_BITFIELD(10, 10)    ; 
            __CODEGEN_UINT32         UnTypedDataPortCacheFlush                        : __CODEGEN_BITFIELD(11, 11)    ; 
            __CODEGEN_UINT32         Reserved_12                                      : __CODEGEN_BITFIELD(12, 12)    ; 
            __CODEGEN_UINT32         CompressionControlSurface_CCSFlush               : __CODEGEN_BITFIELD(13, 13)    ; 
            __CODEGEN_UINT32         WorkloadPartitionIDOffsetEnable                  : __CODEGEN_BITFIELD(14, 14)    ; 
            __CODEGEN_UINT32         SFSyncStall                                      : __CODEGEN_BITFIELD(15, 15)    ; 
            __CODEGEN_UINT32         _3DCommandSubOpcode                              : __CODEGEN_BITFIELD(16, 23)    ; 
            __CODEGEN_UINT32         _3DCommandOpcode                                 : __CODEGEN_BITFIELD(24, 26)    ; 
            __CODEGEN_UINT32         CommandSubType                                   : __CODEGEN_BITFIELD(27, 28)    ; 
            __CODEGEN_UINT32         CommandType                                      : __CODEGEN_BITFIELD(29, 31)    ; 

            __CODEGEN_UINT32         DepthCacheFlushEnable                            : __CODEGEN_BITFIELD( 0,  0)    ; 
            __CODEGEN_UINT32         StallAtPixelScoreboard                           : __CODEGEN_BITFIELD( 1,  1)    ; 
            __CODEGEN_UINT32         StateCacheInvalidationEnable                     : __CODEGEN_BITFIELD( 2,  2)    ; 
            __CODEGEN_UINT32         ConstantCacheInvalidationEnable                  : __CODEGEN_BITFIELD( 3,  3)    ; 
            __CODEGEN_UINT32         VFCacheInvalidationEnable                        : __CODEGEN_BITFIELD( 4,  4)    ; 
            __CODEGEN_UINT32         Reserved_37                                      : __CODEGEN_BITFIELD( 5,  5)    ; 
            __CODEGEN_UINT32         ProtectedMemoryApplicationID                     : __CODEGEN_BITFIELD( 6,  6)    ; 
            __CODEGEN_UINT32         PipeControlFlushEnable                           : __CODEGEN_BITFIELD( 7,  7)    ; 
            __CODEGEN_UINT32         NotifyEnable                                     : __CODEGEN_BITFIELD( 8,  8)    ; 
            __CODEGEN_UINT32         IndirectStatePointersDisable                     : __CODEGEN_BITFIELD( 9,  9)    ; 
            __CODEGEN_UINT32         TextureCacheInvalidationEnable                   : __CODEGEN_BITFIELD(10, 10)    ; 
            __CODEGEN_UINT32         InstructionCacheInvalidateEnable                 : __CODEGEN_BITFIELD(11, 11)    ; 
            __CODEGEN_UINT32         RenderTargetCacheFlushEnable                     : __CODEGEN_BITFIELD(12, 12)    ; 
            __CODEGEN_UINT32         DepthStallEnable                                 : __CODEGEN_BITFIELD(13, 13)    ; 
            __CODEGEN_UINT32         PostSyncOperation                                : __CODEGEN_BITFIELD(14, 15)    ; 
            __CODEGEN_UINT32         GenericMediaStateClear                           : __CODEGEN_BITFIELD(16, 16)    ; 
            __CODEGEN_UINT32         PSSStallSyncEnable                               : __CODEGEN_BITFIELD(17, 17)    ; 
            __CODEGEN_UINT32         TLBInvalidate                                    : __CODEGEN_BITFIELD(18, 18)    ; 
            __CODEGEN_UINT32         DepthStallSyncEnable                             : __CODEGEN_BITFIELD(19, 19)    ; 
            __CODEGEN_UINT32         CommandStreamerStallEnable                       : __CODEGEN_BITFIELD(20, 20)    ; 
            __CODEGEN_UINT32         StoreDataIndex                                   : __CODEGEN_BITFIELD(21, 21)    ; 
            __CODEGEN_UINT32         ProtectedMemoryEnable                            : __CODEGEN_BITFIELD(22, 22)    ; 
            __CODEGEN_UINT32         LRIPostSyncOperation                             : __CODEGEN_BITFIELD(23, 23)    ; 
            __CODEGEN_UINT32         DestinationAddressType                           : __CODEGEN_BITFIELD(24, 24)    ; 
            __CODEGEN_UINT32         AMFSFlushEnable                                  : __CODEGEN_BITFIELD(25, 25)    ; 
            __CODEGEN_UINT32         Reserved_58                                      : __CODEGEN_BITFIELD(26, 26)    ; 
            __CODEGEN_UINT32         ProtectedMemoryDisable                           : __CODEGEN_BITFIELD(27, 27)    ; 
            __CODEGEN_UINT32         Reserved_60                                      : __CODEGEN_BITFIELD(28, 28)    ; 
            __CODEGEN_UINT32         CommandCacheInvalidateEnable                     : __CODEGEN_BITFIELD(29, 29)    ; 
            __CODEGEN_UINT32         Reserved_62                                      : __CODEGEN_BITFIELD(30, 30)    ; 
            __CODEGEN_UINT32         TBIMRForceBatchClosure                           : __CODEGEN_BITFIELD(31, 31)    ; 

            __CODEGEN_UINT32         Reserved_64                                      : __CODEGEN_BITFIELD( 0,  1)    ; 
            __CODEGEN_UINT32         Address                                          : __CODEGEN_BITFIELD( 2, 31)    ; 

            __CODEGEN_UINT32         AddressHigh                                                                      ; 

            __CODEGEN_UINT64         ImmediateData                                                                    ; 

        } Common;
        __CODEGEN_UINT32            RawData[ 6 ];
    } TheStructure;

    __CODEGEN_ACCESS_SPECIFIER_METHODS

    __CODEGEN_DebugType( PIPE_CONTROL )
    {
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.DWordLength                                 ,  0,  7, DWORD_LENGTH );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.PredicateEnable                             ,  8,  8 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.DataportFlush                               ,  9,  9 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.L3ReadOnlyCacheInvalidationEnable           , 10, 10 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.UnTypedDataPortCacheFlush                   , 11, 11 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.CompressionControlSurface_CCSFlush          , 13, 13 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.WorkloadPartitionIDOffsetEnable             , 14, 14 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.SFSyncStall                                 , 15, 15 );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common._3DCommandSubOpcode                         , 16, 23, _3D_COMMAND_SUB_OPCODE );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common._3DCommandOpcode                            , 24, 26, _3D_COMMAND_OPCODE );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.CommandSubType                              , 27, 28, COMMAND_SUBTYPE );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.CommandType                                 , 29, 31, COMMAND_TYPE );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.DepthCacheFlushEnable                       ,  0,  0 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.StallAtPixelScoreboard                      ,  1,  1 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.StateCacheInvalidationEnable                ,  2,  2 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.ConstantCacheInvalidationEnable             ,  3,  3 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.VFCacheInvalidationEnable                   ,  4,  4 );
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
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.PSSStallSyncEnable                          , 17, 17 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.TLBInvalidate                               , 18, 18 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.DepthStallSyncEnable                        , 19, 19 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.CommandStreamerStallEnable                  , 20, 20 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.StoreDataIndex                              , 21, 21 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.ProtectedMemoryEnable                       , 22, 22 );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.LRIPostSyncOperation                        , 23, 23, LRI_POST_SYNC_OPERATION );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.DestinationAddressType                      , 24, 24, DESTINATION_ADDRESS_TYPE );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.AMFSFlushEnable                             , 25, 25 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.ProtectedMemoryDisable                      , 27, 27 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.CommandCacheInvalidateEnable                , 29, 29 );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.TBIMRForceBatchClosure                      , 31, 31, TBIMR_FORCE_BATCH_CLOSURE );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.Address                                     ,  2, 31 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.AddressHigh                                 ,  0, 31 );
        __CODEGEN_DebugAttributeUInt64( TheStructure.Common.ImmediateData                               ,  0, 63 );
    }


    typedef enum tagDWORD_LENGTH
    {
        DWORD_LENGTH_DWORD_COUNT_N                                       = 4, 
    } DWORD_LENGTH;

    typedef enum tag_3D_COMMAND_SUB_OPCODE
    {
        _3D_COMMAND_SUB_OPCODE_PIPE_CONTROL                              = 0, 
    } _3D_COMMAND_SUB_OPCODE;

    typedef enum tag_3D_COMMAND_OPCODE
    {
        _3D_COMMAND_OPCODE_PIPE_CONTROL                                  = 2, 
    } _3D_COMMAND_OPCODE;

    typedef enum tagCOMMAND_SUBTYPE
    {
        COMMAND_SUBTYPE_GFXPIPE_3D                                       = 3, 
    } COMMAND_SUBTYPE;

    typedef enum tagCOMMAND_TYPE
    {
        COMMAND_TYPE_GFXPIPE                                             = 3, 
    } COMMAND_TYPE;

    typedef enum tagPOST_SYNC_OPERATION
    {
        POST_SYNC_OPERATION_NO_WRITE                                     = 0, 
        POST_SYNC_OPERATION_WRITE_IMMEDIATE_DATA                         = 1, 
        POST_SYNC_OPERATION_WRITE_PS_DEPTH_COUNT                         = 2, 
        POST_SYNC_OPERATION_WRITE_TIMESTAMP                              = 3, 
    } POST_SYNC_OPERATION;

    typedef enum tagLRI_POST_SYNC_OPERATION
    {
        LRI_POST_SYNC_OPERATION_NO_LRI_OPERATION                         = 0, 
        LRI_POST_SYNC_OPERATION_MMIO_WRITE_IMMEDIATE_DATA                = 1, 
    } LRI_POST_SYNC_OPERATION;

    typedef enum tagDESTINATION_ADDRESS_TYPE
    {
        DESTINATION_ADDRESS_TYPE_PPGTT                                   = 0, 
        DESTINATION_ADDRESS_TYPE_GGTT                                    = 1, 
    } DESTINATION_ADDRESS_TYPE;

    typedef enum tagTBIMR_FORCE_BATCH_CLOSURE
    {
        TBIMR_FORCE_BATCH_CLOSURE_NO_BATCH_CLOSURE                       = 0, 
        TBIMR_FORCE_BATCH_CLOSURE_CLOSE_BATCH                            = 1, 
    } TBIMR_FORCE_BATCH_CLOSURE;



    __CODEGEN_DebugEnum( DWORD_LENGTH )
    {
        __CODEGEN_DebugEnumValue( DWORD_LENGTH_DWORD_COUNT_N );
    }

    __CODEGEN_DebugEnum( _3D_COMMAND_SUB_OPCODE )
    {
        __CODEGEN_DebugEnumValue( _3D_COMMAND_SUB_OPCODE_PIPE_CONTROL );
    }

    __CODEGEN_DebugEnum( _3D_COMMAND_OPCODE )
    {
        __CODEGEN_DebugEnumValue( _3D_COMMAND_OPCODE_PIPE_CONTROL );
    }

    __CODEGEN_DebugEnum( COMMAND_SUBTYPE )
    {
        __CODEGEN_DebugEnumValue( COMMAND_SUBTYPE_GFXPIPE_3D );
    }

    __CODEGEN_DebugEnum( COMMAND_TYPE )
    {
        __CODEGEN_DebugEnumValue( COMMAND_TYPE_GFXPIPE );
    }

    __CODEGEN_DebugEnum( POST_SYNC_OPERATION )
    {
        __CODEGEN_DebugEnumValue( POST_SYNC_OPERATION_NO_WRITE );
        __CODEGEN_DebugEnumValue( POST_SYNC_OPERATION_WRITE_IMMEDIATE_DATA );
        __CODEGEN_DebugEnumValue( POST_SYNC_OPERATION_WRITE_PS_DEPTH_COUNT );
        __CODEGEN_DebugEnumValue( POST_SYNC_OPERATION_WRITE_TIMESTAMP );
    }

    __CODEGEN_DebugEnum( LRI_POST_SYNC_OPERATION )
    {
        __CODEGEN_DebugEnumValue( LRI_POST_SYNC_OPERATION_NO_LRI_OPERATION );
        __CODEGEN_DebugEnumValue( LRI_POST_SYNC_OPERATION_MMIO_WRITE_IMMEDIATE_DATA );
    }

    __CODEGEN_DebugEnum( DESTINATION_ADDRESS_TYPE )
    {
        __CODEGEN_DebugEnumValue( DESTINATION_ADDRESS_TYPE_PPGTT );
        __CODEGEN_DebugEnumValue( DESTINATION_ADDRESS_TYPE_GGTT );
    }

    __CODEGEN_DebugEnum( TBIMR_FORCE_BATCH_CLOSURE )
    {
        __CODEGEN_DebugEnumValue( TBIMR_FORCE_BATCH_CLOSURE_NO_BATCH_CLOSURE );
        __CODEGEN_DebugEnumValue( TBIMR_FORCE_BATCH_CLOSURE_CLOSE_BATCH );
    }



    __CODEGEN_INLINE void Init( void )
    {
        TheStructure.RawData[ 0 ]                                    = 0x7a000004;
        TheStructure.RawData[ 1 ]                                    = 0x0;
        TheStructure.RawData[ 2 ]                                    = 0x0;
        TheStructure.RawData[ 3 ]                                    = 0x0;
        TheStructure.RawData[ 4 ]                                    = 0x0;
        TheStructure.RawData[ 5 ]                                    = 0x0;
    }

    static PIPE_CONTROL sInit( void ) 
    {
        PIPE_CONTROL state;
        state.Init();
        return state;
    }


    __CODEGEN_INLINE __CODEGEN_UINT32& GetRawData( const __CODEGEN_UINT index )
    {
        __CODEGEN_ASSERT( index < 6 );
        return TheStructure.RawData[ index ];
    }


    __CODEGEN_INLINE void SetPredicateEnable( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.PredicateEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetPredicateEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.PredicateEnable );
    }


    __CODEGEN_INLINE void SetDataportFlush( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.DataportFlush = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetDataportFlush( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.DataportFlush );
    }


    __CODEGEN_INLINE void SetL3ReadOnlyCacheInvalidationEnable( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.L3ReadOnlyCacheInvalidationEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetL3ReadOnlyCacheInvalidationEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.L3ReadOnlyCacheInvalidationEnable );
    }


    __CODEGEN_INLINE void SetUnTypedDataPortCacheFlush( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.UnTypedDataPortCacheFlush = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetUnTypedDataPortCacheFlush( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.UnTypedDataPortCacheFlush );
    }


    __CODEGEN_INLINE void SetCompressionControlSurface_CCSFlush( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.CompressionControlSurface_CCSFlush = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetCompressionControlSurface_CCSFlush( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.CompressionControlSurface_CCSFlush );
    }


    __CODEGEN_INLINE void SetWorkloadPartitionIDOffsetEnable( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.WorkloadPartitionIDOffsetEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetWorkloadPartitionIDOffsetEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.WorkloadPartitionIDOffsetEnable );
    }


    __CODEGEN_INLINE void SetSFSyncStall( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.SFSyncStall = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetSFSyncStall( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.SFSyncStall );
    }


    __CODEGEN_INLINE void SetDepthCacheFlushEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.DepthCacheFlushEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetDepthCacheFlushEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.DepthCacheFlushEnable );
    }


    __CODEGEN_INLINE void SetStallAtPixelScoreboard( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.StallAtPixelScoreboard = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetStallAtPixelScoreboard( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.StallAtPixelScoreboard );
    }


    __CODEGEN_INLINE void SetStateCacheInvalidationEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.StateCacheInvalidationEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetStateCacheInvalidationEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.StateCacheInvalidationEnable );
    }


    __CODEGEN_INLINE void SetConstantCacheInvalidationEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.ConstantCacheInvalidationEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetConstantCacheInvalidationEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.ConstantCacheInvalidationEnable );
    }


    __CODEGEN_INLINE void SetVFCacheInvalidationEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.VFCacheInvalidationEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetVFCacheInvalidationEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.VFCacheInvalidationEnable );
    }


    __CODEGEN_INLINE void SetProtectedMemoryApplicationID( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.ProtectedMemoryApplicationID = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetProtectedMemoryApplicationID( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.ProtectedMemoryApplicationID );
    }


    __CODEGEN_INLINE void SetPipeControlFlushEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.PipeControlFlushEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetPipeControlFlushEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.PipeControlFlushEnable );
    }


    __CODEGEN_INLINE void SetNotifyEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.NotifyEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetNotifyEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.NotifyEnable );
    }


    __CODEGEN_INLINE void SetIndirectStatePointersDisable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.IndirectStatePointersDisable = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetIndirectStatePointersDisable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.IndirectStatePointersDisable );
    }


    __CODEGEN_INLINE void SetTextureCacheInvalidationEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.TextureCacheInvalidationEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetTextureCacheInvalidationEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.TextureCacheInvalidationEnable );
    }


    __CODEGEN_INLINE void SetInstructionCacheInvalidateEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.InstructionCacheInvalidateEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetInstructionCacheInvalidateEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.InstructionCacheInvalidateEnable );
    }


    __CODEGEN_INLINE void SetRenderTargetCacheFlushEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.RenderTargetCacheFlushEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetRenderTargetCacheFlushEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.RenderTargetCacheFlushEnable );
    }


    __CODEGEN_INLINE void SetDepthStallEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.DepthStallEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetDepthStallEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.DepthStallEnable );
    }


    __CODEGEN_INLINE void SetPostSyncOperation( const POST_SYNC_OPERATION value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.PostSyncOperation = value;
    }

    __CODEGEN_INLINE POST_SYNC_OPERATION GetPostSyncOperation( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return static_cast<POST_SYNC_OPERATION>( TheStructure.Common.PostSyncOperation );
    }


    __CODEGEN_INLINE void SetGenericMediaStateClear( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.GenericMediaStateClear = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetGenericMediaStateClear( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.GenericMediaStateClear );
    }


    __CODEGEN_INLINE void SetPSSStallSyncEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.PSSStallSyncEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetPSSStallSyncEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.PSSStallSyncEnable );
    }


    __CODEGEN_INLINE void SetTLBInvalidate( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.TLBInvalidate = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetTLBInvalidate( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.TLBInvalidate );
    }


    __CODEGEN_INLINE void SetDepthStallSyncEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.DepthStallSyncEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetDepthStallSyncEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.DepthStallSyncEnable );
    }


    __CODEGEN_INLINE void SetCommandStreamerStallEnable( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.CommandStreamerStallEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetCommandStreamerStallEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.CommandStreamerStallEnable );
    }


    __CODEGEN_INLINE void SetStoreDataIndex( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.StoreDataIndex = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetStoreDataIndex( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.StoreDataIndex );
    }


    __CODEGEN_INLINE void SetProtectedMemoryEnable( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.ProtectedMemoryEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetProtectedMemoryEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.ProtectedMemoryEnable );
    }


    __CODEGEN_INLINE void SetLRIPostSyncOperation( const LRI_POST_SYNC_OPERATION value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.LRIPostSyncOperation = value;
    }

    __CODEGEN_INLINE LRI_POST_SYNC_OPERATION GetLRIPostSyncOperation( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return static_cast<LRI_POST_SYNC_OPERATION>( TheStructure.Common.LRIPostSyncOperation );
    }


    __CODEGEN_INLINE void SetDestinationAddressType( const DESTINATION_ADDRESS_TYPE value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.DestinationAddressType = value;
    }

    __CODEGEN_INLINE DESTINATION_ADDRESS_TYPE GetDestinationAddressType( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return static_cast<DESTINATION_ADDRESS_TYPE>( TheStructure.Common.DestinationAddressType );
    }


    __CODEGEN_INLINE void SetAMFSFlushEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.AMFSFlushEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetAMFSFlushEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.AMFSFlushEnable );
    }


    __CODEGEN_INLINE void SetProtectedMemoryDisable( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.ProtectedMemoryDisable = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetProtectedMemoryDisable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.ProtectedMemoryDisable );
    }


    __CODEGEN_INLINE void SetCommandCacheInvalidateEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.CommandCacheInvalidateEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetCommandCacheInvalidateEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.CommandCacheInvalidateEnable );
    }


    __CODEGEN_INLINE void SetTBIMRForceBatchClosure( const TBIMR_FORCE_BATCH_CLOSURE value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.TBIMRForceBatchClosure = value;
    }

    __CODEGEN_INLINE TBIMR_FORCE_BATCH_CLOSURE GetTBIMRForceBatchClosure( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return static_cast<TBIMR_FORCE_BATCH_CLOSURE>( TheStructure.Common.TBIMRForceBatchClosure );
    }


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

    __CODEGEN_INLINE __CODEGEN_UINT32 GetAddress( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.Address << ADDRESS_BIT_SHIFT );
    }


    __CODEGEN_INLINE void SetAddressHigh( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.AddressHigh = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetAddressHigh( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.AddressHigh );
    }


    __CODEGEN_INLINE void SetImmediateData( const __CODEGEN_UINT64 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.ImmediateData = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT64 GetImmediateData( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.ImmediateData );
    }

} __CODEGEN_ATTRIBUTES_STRUCTURE;

__CODEGEN_C_ASSERT( 24 == sizeof( PIPE_CONTROL ) );
        
struct MI_STORE_DATA_IMM
{
    __CODEGEN_ACCESS_SPECIFIER_DEFINITION
    union tagTheStructure
    {
        struct tagCommon
        {
            __CODEGEN_UINT32         DWordLength                                      : __CODEGEN_BITFIELD( 0,  9)    ; 
            __CODEGEN_UINT32         ForceWriteCompletionCheck                        : __CODEGEN_BITFIELD(10, 10)    ; 
            __CODEGEN_UINT32         WorkloadPartitionIDOffsetEnable                  : __CODEGEN_BITFIELD(11, 11)    ; 
            __CODEGEN_UINT32         Reserved_12                                      : __CODEGEN_BITFIELD(12, 20)    ; 
            __CODEGEN_UINT32         StoreQword                                       : __CODEGEN_BITFIELD(21, 21)    ; 
            __CODEGEN_UINT32         UseGlobalGTT                                     : __CODEGEN_BITFIELD(22, 22)    ; 
            __CODEGEN_UINT32         MICommandOpcode                                  : __CODEGEN_BITFIELD(23, 28)    ; 
            __CODEGEN_UINT32         CommandType                                      : __CODEGEN_BITFIELD(29, 31)    ; 

            __CODEGEN_UINT64         CoreModeEnable                                   : __CODEGEN_BITFIELD( 0,  0)    ; 
            __CODEGEN_UINT64         Reserved_33                                      : __CODEGEN_BITFIELD( 1,  1)    ; 
            __CODEGEN_UINT64         Address                                          : __CODEGEN_BITFIELD( 2, 63)    ; 

            __CODEGEN_UINT32         DataDWord0                                                                       ; 

            __CODEGEN_UINT32         DataDWord1                                                                       ; 

        } Common;
        __CODEGEN_UINT32            RawData[ 5 ];
    } TheStructure;

    __CODEGEN_ACCESS_SPECIFIER_METHODS

    __CODEGEN_DebugType( MI_STORE_DATA_IMM )
    {
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.DWordLength                                 ,  0,  9, DWORD_LENGTH );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.ForceWriteCompletionCheck                   , 10, 10 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.WorkloadPartitionIDOffsetEnable             , 11, 11 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.StoreQword                                  , 21, 21 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.UseGlobalGTT                                , 22, 22 );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.MICommandOpcode                             , 23, 28, MI_COMMAND_OPCODE );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.CommandType                                 , 29, 31, COMMAND_TYPE );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.CoreModeEnable                              ,  0,  0 );
        __CODEGEN_DebugAttributeUInt64( TheStructure.Common.Address                                     ,  2, 63 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.DataDWord0                                  ,  0, 31 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.DataDWord1                                  ,  0, 31 );
    }


    typedef enum tagDWORD_LENGTH
    {
        DWORD_LENGTH_STORE_DWORD                                         = 2, 
        DWORD_LENGTH_STORE_QWORD                                         = 3, 
    } DWORD_LENGTH;

    typedef enum tagMI_COMMAND_OPCODE
    {
        MI_COMMAND_OPCODE_MI_STORE_DATA_IMM                              = 32, 
    } MI_COMMAND_OPCODE;

    typedef enum tagCOMMAND_TYPE
    {
        COMMAND_TYPE_MI_COMMAND                                          = 0, 
    } COMMAND_TYPE;



    __CODEGEN_DebugEnum( DWORD_LENGTH )
    {
        __CODEGEN_DebugEnumValue( DWORD_LENGTH_STORE_DWORD );
        __CODEGEN_DebugEnumValue( DWORD_LENGTH_STORE_QWORD );
    }

    __CODEGEN_DebugEnum( MI_COMMAND_OPCODE )
    {
        __CODEGEN_DebugEnumValue( MI_COMMAND_OPCODE_MI_STORE_DATA_IMM );
    }

    __CODEGEN_DebugEnum( COMMAND_TYPE )
    {
        __CODEGEN_DebugEnumValue( COMMAND_TYPE_MI_COMMAND );
    }



    __CODEGEN_INLINE void Init( void )
    {
        TheStructure.RawData[ 0 ]                                    = 0x10000002;
        TheStructure.RawData[ 1 ]                                    = 0x0;
        TheStructure.RawData[ 2 ]                                    = 0x0;
        TheStructure.RawData[ 3 ]                                    = 0x0;
        TheStructure.RawData[ 4 ]                                    = 0x0;
    }

    static MI_STORE_DATA_IMM sInit( void ) 
    {
        MI_STORE_DATA_IMM state;
        state.Init();
        return state;
    }


    __CODEGEN_INLINE __CODEGEN_UINT32& GetRawData( const __CODEGEN_UINT index )
    {
        __CODEGEN_ASSERT( index < 5 );
        return TheStructure.RawData[ index ];
    }


    __CODEGEN_INLINE void SetDWordLength( const DWORD_LENGTH value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.DWordLength = value;
    }

    __CODEGEN_INLINE DWORD_LENGTH GetDWordLength( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return static_cast<DWORD_LENGTH>( TheStructure.Common.DWordLength );
    }


    __CODEGEN_INLINE void SetForceWriteCompletionCheck( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.ForceWriteCompletionCheck = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetForceWriteCompletionCheck( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.ForceWriteCompletionCheck );
    }


    __CODEGEN_INLINE void SetWorkloadPartitionIDOffsetEnable( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.WorkloadPartitionIDOffsetEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetWorkloadPartitionIDOffsetEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.WorkloadPartitionIDOffsetEnable );
    }


    __CODEGEN_INLINE void SetStoreQword( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.StoreQword = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetStoreQword( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.StoreQword );
    }


    __CODEGEN_INLINE void SetUseGlobalGTT( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.UseGlobalGTT = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetUseGlobalGTT( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.UseGlobalGTT );
    }


    __CODEGEN_INLINE void SetCoreModeEnable( const __CODEGEN_UINT64 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.CoreModeEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT64 GetCoreModeEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.CoreModeEnable );
    }


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

    __CODEGEN_INLINE __CODEGEN_UINT64 GetAddress( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.Address << ADDRESS_BIT_SHIFT );
    }


    __CODEGEN_INLINE void SetDataDWord0( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.DataDWord0 = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetDataDWord0( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.DataDWord0 );
    }


    __CODEGEN_INLINE void SetDataDWord1( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.DataDWord1 = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetDataDWord1( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.DataDWord1 );
    }

} __CODEGEN_ATTRIBUTES_STRUCTURE;

__CODEGEN_C_ASSERT( 20 == sizeof( MI_STORE_DATA_IMM ) );
        
struct MI_LOAD_REGISTER_IMM
{
    __CODEGEN_ACCESS_SPECIFIER_DEFINITION
    union tagTheStructure
    {
        struct tagCommon
        {
            __CODEGEN_UINT32         DWordLength                                      : __CODEGEN_BITFIELD( 0,  7)    ; 
            __CODEGEN_UINT32         ByteWriteDisables                                : __CODEGEN_BITFIELD( 8, 11)    ; 
            __CODEGEN_UINT32         ForcePosted                                      : __CODEGEN_BITFIELD(12, 12)    ; 
            __CODEGEN_UINT32         Reserved_13                                      : __CODEGEN_BITFIELD(13, 16)    ; 
            __CODEGEN_UINT32         MMIORemapEnable                                  : __CODEGEN_BITFIELD(17, 17)    ; 
            __CODEGEN_UINT32         Reserved_18                                      : __CODEGEN_BITFIELD(18, 18)    ; 
            __CODEGEN_UINT32         AddCSMMIOStartOffset                             : __CODEGEN_BITFIELD(19, 19)    ; 
            __CODEGEN_UINT32         Reserved_20                                      : __CODEGEN_BITFIELD(20, 22)    ; 
            __CODEGEN_UINT32         MICommandOpcode                                  : __CODEGEN_BITFIELD(23, 28)    ; 
            __CODEGEN_UINT32         CommandType                                      : __CODEGEN_BITFIELD(29, 31)    ; 

            __CODEGEN_UINT64         Reserved_32                                      : __CODEGEN_BITFIELD( 0,  1)    ; 
            __CODEGEN_UINT64         RegisterOffset                                   : __CODEGEN_BITFIELD( 2, 22)    ; 
            __CODEGEN_UINT64         Reserved_55                                      : __CODEGEN_BITFIELD(23, 31)    ; 
            __CODEGEN_UINT64         DataDWord                                        : __CODEGEN_BITFIELD(32, 63)    ; 

        } Common;
        __CODEGEN_UINT32            RawData[ 3 ];
    } TheStructure;

    __CODEGEN_ACCESS_SPECIFIER_METHODS

    __CODEGEN_DebugType( MI_LOAD_REGISTER_IMM )
    {
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.DWordLength                                 ,  0,  7, DWORD_LENGTH );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.ByteWriteDisables                           ,  8, 11 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.ForcePosted                                 , 12, 12 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.MMIORemapEnable                             , 17, 17 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.AddCSMMIOStartOffset                        , 19, 19 );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.MICommandOpcode                             , 23, 28, MI_COMMAND_OPCODE );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.CommandType                                 , 29, 31, COMMAND_TYPE );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.RegisterOffset                              ,  2, 22 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.DataDWord                                   , 32, 63 );
    }


    typedef enum tagDWORD_LENGTH
    {
        DWORD_LENGTH_EXCLUDES_DWORD_0_1                                  = 1, 
    } DWORD_LENGTH;

    typedef enum tagMI_COMMAND_OPCODE
    {
        MI_COMMAND_OPCODE_MI_LOAD_REGISTER_IMM                           = 34, 
    } MI_COMMAND_OPCODE;

    typedef enum tagCOMMAND_TYPE
    {
        COMMAND_TYPE_MI_COMMAND                                          = 0, 
    } COMMAND_TYPE;



    __CODEGEN_DebugEnum( DWORD_LENGTH )
    {
        __CODEGEN_DebugEnumValue( DWORD_LENGTH_EXCLUDES_DWORD_0_1 );
    }

    __CODEGEN_DebugEnum( MI_COMMAND_OPCODE )
    {
        __CODEGEN_DebugEnumValue( MI_COMMAND_OPCODE_MI_LOAD_REGISTER_IMM );
    }

    __CODEGEN_DebugEnum( COMMAND_TYPE )
    {
        __CODEGEN_DebugEnumValue( COMMAND_TYPE_MI_COMMAND );
    }



    __CODEGEN_INLINE void Init( void )
    {
        TheStructure.RawData[ 0 ]                                    = 0x11000001;
        TheStructure.RawData[ 1 ]                                    = 0x0;
        TheStructure.RawData[ 2 ]                                    = 0x0;
    }

    static MI_LOAD_REGISTER_IMM sInit( void ) 
    {
        MI_LOAD_REGISTER_IMM state;
        state.Init();
        return state;
    }


    __CODEGEN_INLINE __CODEGEN_UINT32& GetRawData( const __CODEGEN_UINT index )
    {
        __CODEGEN_ASSERT( index < 3 );
        return TheStructure.RawData[ index ];
    }


    __CODEGEN_INLINE void SetByteWriteDisables( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.ByteWriteDisables = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetByteWriteDisables( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.ByteWriteDisables );
    }


    __CODEGEN_INLINE void SetForcePosted( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.ForcePosted = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetForcePosted( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.ForcePosted );
    }


    __CODEGEN_INLINE void SetMMIORemapEnable( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.MMIORemapEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetMMIORemapEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.MMIORemapEnable );
    }


    __CODEGEN_INLINE void SetAddCSMMIOStartOffset( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.AddCSMMIOStartOffset = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetAddCSMMIOStartOffset( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.AddCSMMIOStartOffset );
    }


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

    __CODEGEN_INLINE __CODEGEN_UINT64 GetRegisterOffset( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.RegisterOffset << REGISTEROFFSET_BIT_SHIFT );
    }


    __CODEGEN_INLINE void SetDataDWord( const __CODEGEN_UINT64 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.DataDWord = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT64 GetDataDWord( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.DataDWord );
    }

} __CODEGEN_ATTRIBUTES_STRUCTURE;

__CODEGEN_C_ASSERT( 12 == sizeof( MI_LOAD_REGISTER_IMM ) );
        
struct MI_COPY_MEM_MEM
{
    __CODEGEN_ACCESS_SPECIFIER_DEFINITION
    union tagTheStructure
    {
        struct tagCommon
        {
            __CODEGEN_UINT32         DWordLength                                      : __CODEGEN_BITFIELD( 0,  7)    ; 
            __CODEGEN_UINT32         Reserved_8                                       : __CODEGEN_BITFIELD( 8, 20)    ; 
            __CODEGEN_UINT32         UseGlobalGTTDestination                          : __CODEGEN_BITFIELD(21, 21)    ; 
            __CODEGEN_UINT32         UseGlobalGTTSource                               : __CODEGEN_BITFIELD(22, 22)    ; 
            __CODEGEN_UINT32         MICommandOpcode                                  : __CODEGEN_BITFIELD(23, 28)    ; 
            __CODEGEN_UINT32         CommandType                                      : __CODEGEN_BITFIELD(29, 31)    ; 

            __CODEGEN_UINT64         Reserved_32                                      : __CODEGEN_BITFIELD( 0,  1)    ; 
            __CODEGEN_UINT64         DestinationMemoryAddress                         : __CODEGEN_BITFIELD( 2, 63)    ; 

            __CODEGEN_UINT64         Reserved_96                                      : __CODEGEN_BITFIELD( 0,  1)    ; 
            __CODEGEN_UINT64         SourceMemoryAddress                              : __CODEGEN_BITFIELD( 2, 63)    ; 

        } Common;
        __CODEGEN_UINT32            RawData[ 5 ];
    } TheStructure;

    __CODEGEN_ACCESS_SPECIFIER_METHODS

    __CODEGEN_DebugType( MI_COPY_MEM_MEM )
    {
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.DWordLength                                 ,  0,  7, DWORD_LENGTH );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.UseGlobalGTTDestination                     , 21, 21, USE_GLOBAL_GTT_DESTINATION );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.UseGlobalGTTSource                          , 22, 22, USE_GLOBAL_GTT_SOURCE );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.MICommandOpcode                             , 23, 28, MI_COMMAND_OPCODE );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.CommandType                                 , 29, 31, COMMAND_TYPE );
        __CODEGEN_DebugAttributeUInt64( TheStructure.Common.DestinationMemoryAddress                    ,  2, 63 );
        __CODEGEN_DebugAttributeUInt64( TheStructure.Common.SourceMemoryAddress                         ,  2, 63 );
    }


    typedef enum tagDWORD_LENGTH
    {
        DWORD_LENGTH_EXCLUDES_DWORD_0_1                                  = 3, 
    } DWORD_LENGTH;

    typedef enum tagUSE_GLOBAL_GTT_DESTINATION
    {
        USE_GLOBAL_GTT_DESTINATION_PER_PROCESS_GRAPHICS_ADDRESS          = 0, 
        USE_GLOBAL_GTT_DESTINATION_GLOBAL_GRAPHICS_ADDRESS               = 1, 
    } USE_GLOBAL_GTT_DESTINATION;

    typedef enum tagUSE_GLOBAL_GTT_SOURCE
    {
        USE_GLOBAL_GTT_SOURCE_PER_PROCESS_GRAPHICS_ADDRESS               = 0, 
        USE_GLOBAL_GTT_SOURCE_GLOBAL_GRAPHICS_ADDRESS                    = 1, 
    } USE_GLOBAL_GTT_SOURCE;

    typedef enum tagMI_COMMAND_OPCODE
    {
        MI_COMMAND_OPCODE_MI_COPY_MEM_MEM                                = 46, 
    } MI_COMMAND_OPCODE;

    typedef enum tagCOMMAND_TYPE
    {
        COMMAND_TYPE_MI_COMMAND                                          = 0, 
    } COMMAND_TYPE;



    __CODEGEN_DebugEnum( DWORD_LENGTH )
    {
        __CODEGEN_DebugEnumValue( DWORD_LENGTH_EXCLUDES_DWORD_0_1 );
    }

    __CODEGEN_DebugEnum( USE_GLOBAL_GTT_DESTINATION )
    {
        __CODEGEN_DebugEnumValue( USE_GLOBAL_GTT_DESTINATION_PER_PROCESS_GRAPHICS_ADDRESS );
        __CODEGEN_DebugEnumValue( USE_GLOBAL_GTT_DESTINATION_GLOBAL_GRAPHICS_ADDRESS );
    }

    __CODEGEN_DebugEnum( USE_GLOBAL_GTT_SOURCE )
    {
        __CODEGEN_DebugEnumValue( USE_GLOBAL_GTT_SOURCE_PER_PROCESS_GRAPHICS_ADDRESS );
        __CODEGEN_DebugEnumValue( USE_GLOBAL_GTT_SOURCE_GLOBAL_GRAPHICS_ADDRESS );
    }

    __CODEGEN_DebugEnum( MI_COMMAND_OPCODE )
    {
        __CODEGEN_DebugEnumValue( MI_COMMAND_OPCODE_MI_COPY_MEM_MEM );
    }

    __CODEGEN_DebugEnum( COMMAND_TYPE )
    {
        __CODEGEN_DebugEnumValue( COMMAND_TYPE_MI_COMMAND );
    }



    __CODEGEN_INLINE void Init( void )
    {
        TheStructure.RawData[ 0 ]                                    = 0x17000003;
        TheStructure.RawData[ 1 ]                                    = 0x0;
        TheStructure.RawData[ 2 ]                                    = 0x0;
        TheStructure.RawData[ 3 ]                                    = 0x0;
        TheStructure.RawData[ 4 ]                                    = 0x0;
    }

    static MI_COPY_MEM_MEM sInit( void ) 
    {
        MI_COPY_MEM_MEM state;
        state.Init();
        return state;
    }


    __CODEGEN_INLINE __CODEGEN_UINT32& GetRawData( const __CODEGEN_UINT index )
    {
        __CODEGEN_ASSERT( index < 5 );
        return TheStructure.RawData[ index ];
    }


    __CODEGEN_INLINE void SetUseGlobalGTTDestination( const USE_GLOBAL_GTT_DESTINATION value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.UseGlobalGTTDestination = value;
    }

    __CODEGEN_INLINE USE_GLOBAL_GTT_DESTINATION GetUseGlobalGTTDestination( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return static_cast<USE_GLOBAL_GTT_DESTINATION>( TheStructure.Common.UseGlobalGTTDestination );
    }


    __CODEGEN_INLINE void SetUseGlobalGTTSource( const USE_GLOBAL_GTT_SOURCE value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.UseGlobalGTTSource = value;
    }

    __CODEGEN_INLINE USE_GLOBAL_GTT_SOURCE GetUseGlobalGTTSource( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return static_cast<USE_GLOBAL_GTT_SOURCE>( TheStructure.Common.UseGlobalGTTSource );
    }


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

    __CODEGEN_INLINE __CODEGEN_UINT64 GetDestinationMemoryAddress( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.DestinationMemoryAddress << DESTINATIONMEMORYADDRESS_BIT_SHIFT );
    }


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

    __CODEGEN_INLINE __CODEGEN_UINT64 GetSourceMemoryAddress( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.SourceMemoryAddress << SOURCEMEMORYADDRESS_BIT_SHIFT );
    }

} __CODEGEN_ATTRIBUTES_STRUCTURE;

__CODEGEN_C_ASSERT( 20 == sizeof( MI_COPY_MEM_MEM ) );
        
struct MI_LOAD_REGISTER_REG
{
    __CODEGEN_ACCESS_SPECIFIER_DEFINITION
    union tagTheStructure
    {
        struct tagCommon
        {
            __CODEGEN_UINT32         DWordLength                                      : __CODEGEN_BITFIELD( 0,  7)    ; 
            __CODEGEN_UINT32         Reserved_8                                       : __CODEGEN_BITFIELD( 8, 15)    ; 
            __CODEGEN_UINT32         MMIORemapEnableSource                            : __CODEGEN_BITFIELD(16, 16)    ; 
            __CODEGEN_UINT32         MMIORemapEnableDestination                       : __CODEGEN_BITFIELD(17, 17)    ; 
            __CODEGEN_UINT32         AddCSMMIOStartOffsetSource                       : __CODEGEN_BITFIELD(18, 18)    ; 
            __CODEGEN_UINT32         AddCSMMIOStartOffsetDestination                  : __CODEGEN_BITFIELD(19, 19)    ; 
            __CODEGEN_UINT32         Reserved_20                                      : __CODEGEN_BITFIELD(20, 22)    ; 
            __CODEGEN_UINT32         MICommandOpcode                                  : __CODEGEN_BITFIELD(23, 28)    ; 
            __CODEGEN_UINT32         CommandType                                      : __CODEGEN_BITFIELD(29, 31)    ; 

            __CODEGEN_UINT32         Reserved_32                                      : __CODEGEN_BITFIELD( 0,  1)    ; 
            __CODEGEN_UINT32         SourceRegisterAddress                            : __CODEGEN_BITFIELD( 2, 22)    ; 
            __CODEGEN_UINT32         Reserved_55                                      : __CODEGEN_BITFIELD(23, 31)    ; 

            __CODEGEN_UINT32         Reserved_64                                      : __CODEGEN_BITFIELD( 0,  1)    ; 
            __CODEGEN_UINT32         DestinationRegisterAddress                       : __CODEGEN_BITFIELD( 2, 22)    ; 
            __CODEGEN_UINT32         Reserved_87                                      : __CODEGEN_BITFIELD(23, 31)    ; 

        } Common;
        __CODEGEN_UINT32            RawData[ 3 ];
    } TheStructure;

    __CODEGEN_ACCESS_SPECIFIER_METHODS

    __CODEGEN_DebugType( MI_LOAD_REGISTER_REG )
    {
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.DWordLength                                 ,  0,  7, DWORD_LENGTH );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.MMIORemapEnableSource                       , 16, 16 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.MMIORemapEnableDestination                  , 17, 17 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.AddCSMMIOStartOffsetSource                  , 18, 18 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.AddCSMMIOStartOffsetDestination             , 19, 19 );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.MICommandOpcode                             , 23, 28, MI_COMMAND_OPCODE );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.CommandType                                 , 29, 31, COMMAND_TYPE );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.SourceRegisterAddress                       ,  2, 22 );
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.DestinationRegisterAddress                  ,  2, 22 );
    }


    typedef enum tagDWORD_LENGTH
    {
        DWORD_LENGTH_EXCLUDES_DWORD_0_1                                  = 1, 
    } DWORD_LENGTH;

    typedef enum tagMI_COMMAND_OPCODE
    {
        MI_COMMAND_OPCODE_MI_LOAD_REGISTER_REG                           = 42, 
    } MI_COMMAND_OPCODE;

    typedef enum tagCOMMAND_TYPE
    {
        COMMAND_TYPE_MI_COMMAND                                          = 0, 
    } COMMAND_TYPE;



    __CODEGEN_DebugEnum( DWORD_LENGTH )
    {
        __CODEGEN_DebugEnumValue( DWORD_LENGTH_EXCLUDES_DWORD_0_1 );
    }

    __CODEGEN_DebugEnum( MI_COMMAND_OPCODE )
    {
        __CODEGEN_DebugEnumValue( MI_COMMAND_OPCODE_MI_LOAD_REGISTER_REG );
    }

    __CODEGEN_DebugEnum( COMMAND_TYPE )
    {
        __CODEGEN_DebugEnumValue( COMMAND_TYPE_MI_COMMAND );
    }



    __CODEGEN_INLINE void Init( void )
    {
        TheStructure.RawData[ 0 ]                                    = 0x15000001;
        TheStructure.RawData[ 1 ]                                    = 0x0;
        TheStructure.RawData[ 2 ]                                    = 0x0;
    }

    static MI_LOAD_REGISTER_REG sInit( void ) 
    {
        MI_LOAD_REGISTER_REG state;
        state.Init();
        return state;
    }


    __CODEGEN_INLINE __CODEGEN_UINT32& GetRawData( const __CODEGEN_UINT index )
    {
        __CODEGEN_ASSERT( index < 3 );
        return TheStructure.RawData[ index ];
    }


    __CODEGEN_INLINE void SetMMIORemapEnableSource( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.MMIORemapEnableSource = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetMMIORemapEnableSource( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.MMIORemapEnableSource );
    }


    __CODEGEN_INLINE void SetMMIORemapEnableDestination( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.MMIORemapEnableDestination = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetMMIORemapEnableDestination( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.MMIORemapEnableDestination );
    }


    __CODEGEN_INLINE void SetAddCSMMIOStartOffsetSource( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.AddCSMMIOStartOffsetSource = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetAddCSMMIOStartOffsetSource( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.AddCSMMIOStartOffsetSource );
    }


    __CODEGEN_INLINE void SetAddCSMMIOStartOffsetDestination( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.AddCSMMIOStartOffsetDestination = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetAddCSMMIOStartOffsetDestination( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.AddCSMMIOStartOffsetDestination );
    }


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

    __CODEGEN_INLINE __CODEGEN_UINT32 GetSourceRegisterAddress( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.SourceRegisterAddress << SOURCEREGISTERADDRESS_BIT_SHIFT );
    }


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

    __CODEGEN_INLINE __CODEGEN_UINT32 GetDestinationRegisterAddress( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.DestinationRegisterAddress << DESTINATIONREGISTERADDRESS_BIT_SHIFT );
    }

} __CODEGEN_ATTRIBUTES_STRUCTURE;

__CODEGEN_C_ASSERT( 12 == sizeof( MI_LOAD_REGISTER_REG ) );
        
struct MI_NOOP
{
    __CODEGEN_ACCESS_SPECIFIER_DEFINITION
    union tagTheStructure
    {
        struct tagCommon
        {
            __CODEGEN_UINT32         IdentificationNumber                             : __CODEGEN_BITFIELD( 0, 21)    ; 
            __CODEGEN_UINT32         IdentificationNumberRegisterWriteEnable          : __CODEGEN_BITFIELD(22, 22)    ; 
            __CODEGEN_UINT32         MICommandOpcode                                  : __CODEGEN_BITFIELD(23, 28)    ; 
            __CODEGEN_UINT32         CommandType                                      : __CODEGEN_BITFIELD(29, 31)    ; 

        } Common;
        __CODEGEN_UINT32            RawData[ 1 ];
    } TheStructure;

    __CODEGEN_ACCESS_SPECIFIER_METHODS

    __CODEGEN_DebugType( MI_NOOP )
    {
        __CODEGEN_DebugAttributeUInt(   TheStructure.Common.IdentificationNumber                        ,  0, 21 );
        __CODEGEN_DebugAttributeBool(   TheStructure.Common.IdentificationNumberRegisterWriteEnable     , 22, 22 );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.MICommandOpcode                             , 23, 28, MI_COMMAND_OPCODE );
        __CODEGEN_DebugAttributeEnum(   TheStructure.Common.CommandType                                 , 29, 31, COMMAND_TYPE );
    }


    typedef enum tagMI_COMMAND_OPCODE
    {
        MI_COMMAND_OPCODE_MI_NOOP                                        = 0, 
    } MI_COMMAND_OPCODE;

    typedef enum tagCOMMAND_TYPE
    {
        COMMAND_TYPE_MI_COMMAND                                          = 0, 
    } COMMAND_TYPE;



    __CODEGEN_DebugEnum( MI_COMMAND_OPCODE )
    {
        __CODEGEN_DebugEnumValue( MI_COMMAND_OPCODE_MI_NOOP );
    }

    __CODEGEN_DebugEnum( COMMAND_TYPE )
    {
        __CODEGEN_DebugEnumValue( COMMAND_TYPE_MI_COMMAND );
    }



    __CODEGEN_INLINE void Init( void )
    {
        TheStructure.RawData[ 0 ]                                    = 0x0;
    }

    static MI_NOOP sInit( void ) 
    {
        MI_NOOP state;
        state.Init();
        return state;
    }


    __CODEGEN_INLINE __CODEGEN_UINT32& GetRawData( const __CODEGEN_UINT index )
    {
        __CODEGEN_ASSERT( index < 1 );
        return TheStructure.RawData[ index ];
    }


    __CODEGEN_INLINE void SetIdentificationNumber( const __CODEGEN_UINT32 value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.IdentificationNumber = value;
    }

    __CODEGEN_INLINE __CODEGEN_UINT32 GetIdentificationNumber( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.IdentificationNumber );
    }


    __CODEGEN_INLINE void SetIdentificationNumberRegisterWriteEnable( const __CODEGEN_BOOL value )
    {
        __CODEGEN_SET_MACRO( value );
        TheStructure.Common.IdentificationNumberRegisterWriteEnable = value;
    }

    __CODEGEN_INLINE __CODEGEN_BOOL GetIdentificationNumberRegisterWriteEnable( void ) const 
    {
        __CODEGEN_GET_MACRO();
        return ( TheStructure.Common.IdentificationNumberRegisterWriteEnable );
    }

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