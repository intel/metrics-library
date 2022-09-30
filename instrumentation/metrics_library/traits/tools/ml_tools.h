/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2022 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_tools.h

@brief Contains a set of helper functions.
*/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief LogType enumeration.
    //////////////////////////////////////////////////////////////////////////
    enum class LogType : uint32_t
    {
        Critical = ML_BIT( 0 ),  // Unexpected unrecoverable errors, use for asserts only.
        Error    = ML_BIT( 1 ),  // Unexpected recoverable errors.
        Warning  = ML_BIT( 2 ),  // Expected errors and warnings.
        Info     = ML_BIT( 3 ),  // Information messages.
        Debug    = ML_BIT( 4 ),  // Debug messages.
        Traits   = ML_BIT( 5 ),  // Traits messages.
        Entered  = ML_BIT( 6 ),  // Entered a function.
        Exiting  = ML_BIT( 7 ),  // Exiting a function.
        Input    = ML_BIT( 8 ),  // Input parameter.
        Output   = ML_BIT( 9 ),  // Output parameter.
        Csv      = ML_BIT( 10 ), // Csv file dumping.
    };

    //////////////////////////////////////////////////////////////////////////
    /// @brief  Overloaded bitwise "and" operator for enumerations.
    /// @param  value1  first enumerator.
    /// @param  value2  second enumerator.
    /// @return         result of bitwise "and" operator.
    //////////////////////////////////////////////////////////////////////////
    template <typename EnumerationType>
    ML_INLINE constexpr EnumerationType operator&(
        const EnumerationType value1,
        const EnumerationType value2 )
    {
        ML_STATIC_ASSERT( std::is_enum<EnumerationType>::value, "The template parameter is not an enumeration type." );

        return static_cast<EnumerationType>(
            static_cast<typename std::underlying_type<EnumerationType>::type>( value1 ) &
            static_cast<typename std::underlying_type<EnumerationType>::type>( value2 ) );
    }

    //////////////////////////////////////////////////////////////////////////
    /// @brief  Overloaded bitwise "or" operator for enumerations.
    /// @param  value1  first enumerator.
    /// @param  value2  second enumerator.
    /// @return         result of bitwise "or" operator.
    //////////////////////////////////////////////////////////////////////////
    template <typename EnumerationType>
    ML_INLINE constexpr EnumerationType operator|(
        const EnumerationType value1,
        const EnumerationType value2 )
    {
        ML_STATIC_ASSERT( std::is_enum<EnumerationType>::value, "The template parameter is not an enumeration type." );

        return static_cast<EnumerationType>(
            static_cast<typename std::underlying_type<EnumerationType>::type>( value1 ) |
            static_cast<typename std::underlying_type<EnumerationType>::type>( value2 ) );
    }

    //////////////////////////////////////////////////////////////////////////
    /// @brief  Overloaded bitwise "xor" operator for enumerations.
    /// @param  value1  first enumerator.
    /// @param  value2  second enumerator.
    /// @return         result of bitwise "xor" operator.
    //////////////////////////////////////////////////////////////////////////
    template <typename EnumerationType>
    ML_INLINE constexpr EnumerationType operator^(
        const EnumerationType value1,
        const EnumerationType value2 )
    {
        ML_STATIC_ASSERT( std::is_enum<EnumerationType>::value, "The template parameter is not an enumeration type." );

        return static_cast<EnumerationType>(
            static_cast<typename std::underlying_type<EnumerationType>::type>( value1 ) ^
            static_cast<typename std::underlying_type<EnumerationType>::type>( value2 ) );
    }

    //////////////////////////////////////////////////////////////////////////
    /// @brief ToolsTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct ToolsTrait
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( ToolsTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( ToolsTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "ToolsTrait<Traits>";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Allocates and constructs object instance wrapped in a unique pointer.
        /// @param  arguments   constructor arguments.
        /// @return             new object instance of type 'Type' wrapped in unique_ptr.
        //////////////////////////////////////////////////////////////////////////
        template <typename Type, typename... Arguments>
        ML_INLINE static std::unique_ptr<Type> AllocateUnique( Arguments&&... arguments )
        {
            return std::unique_ptr<Type>( new( std::nothrow ) Type( std::forward<Arguments>( arguments )... ) );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Copies memory.
        /// @param target       target memory.
        /// @param targetSize   target memory size.
        /// @param source       source memory.
        /// @param sourceSize   source memory size.
        //////////////////////////////////////////////////////////////////////////
        template <typename Source, typename Destination>
        ML_INLINE static void MemoryCopy(
            Destination*   target,
            const uint32_t targetSize,
            const Source*  source,
            const uint32_t sourceSize )
        {
            ML_ASSERT_NO_ADAPTER( target != nullptr );
            ML_ASSERT_NO_ADAPTER( source != nullptr );
            ML_ASSERT_NO_ADAPTER( sourceSize <= targetSize );

            iu_memcpy_s( target, targetSize, static_cast<const void*>( source ), sourceSize );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Copies array.
        /// @return target          target array.
        /// @param  targetIndex     target start index.
        /// @param  source          source array.
        /// @param  sourceIndex     source start index.
        /// @param  count           slots to copy.
        /// @param  allowOverrun    allow to overrun if target_index + count >= target_size.
        //////////////////////////////////////////////////////////////////////////
        template <typename TargetArray, typename SourceArray>
        ML_INLINE static void ArrayCopy(
            TargetArray&       target,
            const uint32_t     targetIndex,
            const SourceArray& source,
            const uint32_t     sourceIndex,
            const uint32_t     count,
            const bool         allowOverrun )
        {
            const bool     overrun    = ( targetIndex + count ) >= target.size();
            const uint32_t reportSize = sizeof( target[0] );

            if( overrun == false )
            {
                auto targetMemory = &( target[targetIndex] );
                auto sourceMemory = &( source[sourceIndex] );

                const uint32_t targetSize = reportSize * target.size();
                const uint32_t sourceSize = reportSize * count;

                MemoryCopy( targetMemory, targetSize, sourceMemory, sourceSize );
            }
            else if( allowOverrun )
            {
                const uint32_t count0 = target.size() - targetIndex;
                const uint32_t count1 = count - count0;

                const uint32_t size0 = reportSize * count0;
                const uint32_t size1 = reportSize * count1;

                auto targetMemory0 = &( target[targetIndex] );
                auto sourceMemory0 = &( source[0] );

                auto targetMemory1 = &( target[0] );
                auto sourceMemory1 = &( source[count0] );

                MemoryCopy( targetMemory0, size0, sourceMemory0, size0 );
                MemoryCopy( targetMemory1, size1, sourceMemory1, size1 );

                ML_ASSERT_NO_ADAPTER( count1 < target.size() );
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns masked value.
        /// @param  value   value to mask.
        /// @param  mask    mask.
        /// @return         masked value.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static uint32_t GetMaskedValue(
            uint32_t value,
            uint32_t mask )
        {
            if( mask )
            {
                value &= mask;
                while( !( mask & 1 ) )
                {
                    mask >>= 1;
                    value >>= 1;
                }
            }
            else
            {
                value = 0;
            }
            return value;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Compares two measure points (counters overflow is also handled).
        /// @param value1   first value to compare.
        /// @param value2   second value to compare.
        /// @return        -1 if value1 is lesser than value2.
        ///                 1 if value1 is greater than value2.
        ///                 0 if value1 is equal to value2.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static int32_t Compare32(
            const uint32_t value1,
            const uint32_t value2 )
        {
            int32_t result = 0;
            if( value1 < value2 )
            {
                result = ( ( value2 - value1 ) < 0x80000000 ) ? -1 : 1;
            }
            else if( value1 > value2 )
            {
                result = ( ( value1 - value2 ) < 0x80000000 ) ? 1 : -1;
            }

            return result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Compares two measure points (counters overflow is also handled).
        /// @param value1   first value to compare.
        /// @param value2   second value to compare.
        /// @return        -1 if value1 is lesser than value2.
        ///                 1 if value1 is greater than value2.
        ///                 0 if value1 is equal to value2.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static int32_t Compare64(
            const uint64_t value1,
            const uint64_t value2 )
        {
            int32_t result = 0;

            if( value1 < value2 )
            {
                result = ( ( value2 - value1 ) < 0x8000000000000000 ) ? -1 : 1;
            }
            else if( value1 > value2 )
            {
                result = ( ( value1 - value2 ) < 0x8000000000000000 ) ? 1 : -1;
            }

            return result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns a calculated hash for a given value.
        /// @param  value   a value to hash.
        /// @return         a calculated hash.
        //////////////////////////////////////////////////////////////////////////
        template <typename HashType>
        ML_INLINE static size_t GetHash( const HashType value )
        {
            return std::hash<HashType>()( value );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns a difference between two measure points,
        ///         counters overflow is also handled.
        /// @param  end     end counter.
        /// @param  begin   begin counter.
        /// @param  bitsize bitsize of the counters.
        /// @return         difference between two measure points.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static uint64_t CountersDelta(
            uint64_t end,
            uint64_t begin,
            uint32_t bitsize )
        {
            const uint32_t maxBitsize = 64;
            const uint64_t mask       = ( bitsize != maxBitsize ) ? ( ( ML_BIT( bitsize ) ) - 1 ) : -1;

            ML_ASSERT_NO_ADAPTER( bitsize <= maxBitsize );

            end &= mask;
            begin &= mask;

            if( end >= begin )
            {
                return ( end - begin );
            }
            else
            {
                return end + ( ( bitsize != maxBitsize ) ? ( ML_BIT( bitsize ) ) : 0 ) - begin;
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Checks whether Metrics Library debug layer is enabled.
        /// @return true if the debug log layer is enabled, false otherwise.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static bool CheckLogLayer()
        {
            return IuLogCheckLayer( IU_DBG_LAYER_ML );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Checks debug log level.
        /// @param  level   debug log level.
        /// @return         true if the debug log level is enabled, false otherwise.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static bool CheckLogLevel( const LogType level )
        {
            const uint32_t debugLevel = static_cast<uint32_t>( level );

            return IuLogCheckLevel( debugLevel, IU_DBG_LAYER_ML );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns a separated lines for a given string.
        /// @param  stream    stream to parse.
        /// @return lines     separated lines.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static void GetLines(
            std::istringstream&       stream,
            std::vector<std::string>& lines,
            TT::Debug&                debug )
        {
            std::string line;

            while( std::getline( stream, line ) )
            {
                if constexpr( Constants::Log::m_FilterEnabled )
                {
                    FilterLine( line, lines, debug );
                }
                else
                {
                    lines.push_back( line );
                }
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs user's message to console.
        /// @param type         log type.
        /// @param functionName function name.
        /// @param context      context.
        /// @param values       user's message.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE static void Log(
            const LogType      type,
            const std::string& functionName,
            TT::Context*       context,
            const Values&... values )
        {
            if( CheckLogLevel( type ) )
            {
                if( context == nullptr )
                {
                    LogWithoutContext( type, functionName, values... );
                    return;
                }

                TT::Debug& debug = context->m_Debug;

                debug.m_Aligned = IuLogCheckShowMode( IU_DBG_ALIGNED );
                debug.m_Length  = IuLogCheckShowMode( IU_DBG_SHOW_FUNCTION )
                     ? static_cast<uint32_t>( functionName.length() )
                     : 0;

                std::vector<std::string> lines;
                std::istringstream       stream( debug.Format( values... ) );

                // Split message into a separate lines.
                GetLines( stream, lines, debug );

                // Print out each line.
                for( size_t i = 0; i < lines.size(); ++i )
                {
                    Print( type, functionName, lines[i], context->m_AdapterId );
                }
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs user's message to console.
        ///        Conceptual function name: Log.
        ///        Allows to use Log() function without context param.
        /// @param type         log type.
        /// @param functionName function name.
        /// @param values       user's message.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE static void LogWithoutContext(
            const LogType      type,
            const std::string& functionName,
            const Values&... values )
        {
            TT::Debug debug;

            debug.m_Aligned = IuLogCheckShowMode( IU_DBG_ALIGNED );
            debug.m_Length  = IuLogCheckShowMode( IU_DBG_SHOW_FUNCTION )
                 ? static_cast<uint32_t>( functionName.length() )
                 : 0;

            std::vector<std::string> lines;
            std::istringstream       stream( debug.Format( values... ) );

            // Split message into a separate lines.
            GetLines( stream, lines, debug );

            // Print out each line.
            for( size_t i = 0; i < lines.size(); ++i )
            {
                Print( type, functionName, lines[i], IU_ADAPTER_ID_UNKNOWN );
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Opens csv file.
        /// @param  name    identification of the csv file.
        /// @param  file    output file stream.
        /// @param  context context.
        /// @return success if the file has been opened properly.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode OpenCsv(
            const std::string& name,
            std::ofstream&     file,
            TT::Context*       context = nullptr )
        {
            ML_FUNCTION_LOG( StatusCode::Success, context );

            std::string fileName = Constants::Library::m_Name;

            fileName += "_";
            fileName += name;
            fileName += "_";
            fileName += T::ToolsOs::GetCurrentTime();
            fileName += ".csv";

            file.open( fileName );

            return file.is_open()
                ? log.m_Result
                : log.m_Result = StatusCode::CannotOpenFile;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Writes user's message to a csv file.
        /// @param context context.
        /// @param values  user's message.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE static void WriteToCsv(
            TT::Context* context,
            const Values&... values )
        {
            if( context == nullptr )
            {
                TT::Debug debug;
                debug.PrintCsv( nullptr, values... );
            }
            else
            {
                context->m_Debug.PrintCsv( context, values... );
            }
        }

    private:
        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs user's message to console.
        /// @param type         debug log type.
        /// @param functionName function name.
        /// @param message      user's message.
        /// @param adapterId    adapter id.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static void Print(
            const LogType      type,
            const std::string& functionName,
            const std::string& message,
            const uint32_t     adapterId )
        {
            switch( type )
            {
                case LogType::Critical:
                    ML_LOG( adapterId, LOG_CRITICAL, functionName.c_str(), message.c_str() );
                    break;

                case LogType::Error:
                    ML_LOG( adapterId, LOG_ERROR, functionName.c_str(), message.c_str() );
                    break;

                case LogType::Warning:
                    ML_LOG( adapterId, LOG_WARNING, functionName.c_str(), message.c_str() );
                    break;

                case LogType::Debug:
                    ML_LOG( adapterId, LOG_DEBUG, functionName.c_str(), message.c_str() );
                    break;

                case LogType::Info:
                    ML_LOG( adapterId, LOG_INFO, functionName.c_str(), message.c_str() );
                    break;

                case LogType::Traits:
                    ML_LOG( adapterId, LOG_TRAITS, functionName.c_str(), message.c_str() );
                    break;

                case LogType::Entered:
                    ML_LOG( adapterId, LOG_ENTERED, functionName.c_str(), message.c_str() );
                    break;

                case LogType::Exiting:
                    ML_LOG( adapterId, LOG_EXITING, functionName.c_str(), message.c_str() );
                    break;

                case LogType::Input:
                    ML_LOG( adapterId, LOG_INPUT, functionName.c_str(), message.c_str() );
                    break;

                case LogType::Output:
                    ML_LOG( adapterId, LOG_OUTPUT, functionName.c_str(), message.c_str() );
                    break;

                default:
                    ML_ASSERT_ALWAYS_ADAPTER( adapterId );
                    break;
            }

            if constexpr( T::ConstantsOs::Log::m_FlushEachLog )
            {
                fflush( stdout );
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Filter each line to log them, which contain a given string,
        ///         with chosen next lines.
        /// @param  line    line to filter.
        /// @return lines   separated lines.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static void FilterLine(
            const std::string&        line,
            std::vector<std::string>& lines,
            TT::Debug&                debug )
        {
            uint32_t& filteredLines = debug.m_FilteredLines;

            if( line.find( Constants::Log::m_FilteredString ) != std::string::npos )
            {
                lines.push_back( line );
                filteredLines = Constants::Log::m_FilteredLines;
            }
            else if( filteredLines > 0 )
            {
                lines.push_back( line );
                --filteredLines;
            }
        }
    };
} // namespace ML
