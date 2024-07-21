/**************************************************************************************************
 * Copyright (c) 2023-2024 NWSOFT                                                                 *
 *                                                                                                *
 * Permission is hereby granted, free of charge, to any person obtaining a copy                   *
 * of this software and associated documentation files (the "Software"), to deal                  *
 * in the Software without restriction, including without limitation the rights                   *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell                      *
 * copies of the Software, and to permit persons to whom the Software is                          *
 * furnished to do so, subject to the following conditions:                                       *
 *                                                                                                *
 * The above copyright notice and this permission notice shall be included in all                 *
 * copies or substantial portions of the Software.                                                *
 *                                                                                                *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR                     *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,                       *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE                    *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER                         *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,                  *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE                  *
 * SOFTWARE.                                                                                      *
 **************************************************************************************************/

/**
 * @file symbols.hpp
 * @brief This file contains the definitions of various symbols used in the project.
 *
 * The symbols include mathematical operators, subscripts, and superscripts.
 *
 * @author Andy Zhang
 * @date 9th October 2023
 */

#pragma once

#include <string_view>

/**
 * @namespace adaptiday::__internals::symbols
 * @brief The namespace containing various unicode symbols.
 *
 * @deprecated This namespace is deprecated and will be removed in the future, as the unicode output is not flexible
 * enough.
 * @warning Usage of this namespace is strongly discouraged. Use the adaptiday::prettyPrint namespace for basic tools,
 * and implement yours in adaptiday::prettyPrint::printers.
 */
namespace adaptiday::__internals::symbols
{
    /// @brief The large dot symbol (Unicode U+25C9)
    constexpr std::string_view LARGE_DOT = "\u25C9";
} // namespace adaptiday::__internals::symbols
