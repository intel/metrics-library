/******************************************************************************\

Copyright © 2020, Intel Corporation

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

@file ml_tools.h

@brief Contains a set of helper functions.

\******************************************************************************/
#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief LogType enumeration.
    //////////////////////////////////////////////////////////////////////////
    enum class LogType : uint32_t
    {
        Critical = ML_BIT( 0 ), // Unexpected unrecoverable errors, use for asserts only.
        Error    = ML_BIT( 1 ), // Unexpected recoverable errors.
        Warning  = ML_BIT( 2 ), // Expected errors and warnings.
        Info     = ML_BIT( 3 ), // Information messages.
        Debug    = ML_BIT( 4 ), // Debug messages.
        Traits   = ML_BIT( 5 ), // Traits messages.
        Entered  = ML_BIT( 6 ), // Entered a function.
        Exiting  = ML_BIT( 7 ), // Exiting a function.
        Input    = ML_BIT( 8 ), // Input parameter.
        Output   = ML_BIT( 9 ), // Output parameter.
    };

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
            ML_ASSERT( target != nullptr );
            ML_ASSERT( source != nullptr );
            ML_ASSERT( sourceSize <= targetSize );

            iu_memcpy_s( target, targetSize, static_cast<const void*>( source ), sourceSize );
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
            ML_FUNCTION_LOG( int32_t{ 0 } );

            if( value1 < value2 )
            {
                log.m_Result = ( ( value2 - value1 ) < 0x80000000 ) ? -1 : 1;
            }
            else if( value1 > value2 )
            {
                log.m_Result = ( ( value1 - value2 ) < 0x80000000 ) ? 1 : -1;
            }
            else
            {
                log.m_Result = 0;
            }

            return log.m_Result;
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

            ML_ASSERT( bitsize <= maxBitsize );

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
            std::vector<std::string>& lines )
        {
            std::string line;

            while( std::getline( stream, line ) )
            {
                if( Constants::Log::m_FilterEnabled )
                {
                    FilterLine( line, lines );
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
        /// @param message      user's message.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE static void Log(
            const LogType      type,
            const std::string& functionName,
            const Values&... values )
        {
            if( CheckLogLevel( type ) )
            {
                if( IuLogCheckShowMode( IU_DBG_SHOW_FUNCTION ) )
                {
                    T::Debug::GetFunctionNameLength() = static_cast<uint32_t>( functionName.length() );
                }

                std::vector<std::string> lines;
                std::istringstream       stream( T::Debug::Format( values... ) );

                // Split message into a separate lines.
                GetLines( stream, lines );

                // Print out each line.
                for( size_t i = 0; i < lines.size(); ++i )
                {
                    Print( type, functionName, lines[i] );
                }
            }
        }

    private:
        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs user's message to console.
        /// @param type         debug log type.
        /// @param functionName function name.
        /// @param message      user's message.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static void Print(
            const LogType      type,
            const std::string& functionName,
            const std::string& message )
        {
            switch( type )
            {
                case LogType::Critical:
                    ML_LOG( LOG_CRITICAL, functionName.c_str(), message.c_str() );
                    break;

                case LogType::Error:
                    ML_LOG( LOG_ERROR, functionName.c_str(), message.c_str() );
                    break;

                case LogType::Warning:
                    ML_LOG( LOG_WARNING, functionName.c_str(), message.c_str() );
                    break;

                case LogType::Debug:
                    ML_LOG( LOG_DEBUG, functionName.c_str(), message.c_str() );
                    break;

                case LogType::Info:
                    ML_LOG( LOG_INFO, functionName.c_str(), message.c_str() );
                    break;

                case LogType::Traits:
                    ML_LOG( LOG_TRAITS, functionName.c_str(), message.c_str() );
                    break;

                case LogType::Entered:
                    ML_LOG( LOG_ENTERED, functionName.c_str(), message.c_str() );
                    break;

                case LogType::Exiting:
                    ML_LOG( LOG_EXITING, functionName.c_str(), message.c_str() );
                    break;

                case LogType::Input:
                    ML_LOG( LOG_INPUT, functionName.c_str(), message.c_str() );
                    break;

                case LogType::Output:
                    ML_LOG( LOG_OUTPUT, functionName.c_str(), message.c_str() );
                    break;

                default:
                    ML_ASSERT_ALWAYS();
                    break;
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
            std::vector<std::string>& lines )
        {
            static uint32_t filteredLines = 0;

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
