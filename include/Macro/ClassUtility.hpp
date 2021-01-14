/*
 * Project : Multithread
 * Editing by Six Jonathan
 * Date : 2020-12-02 - 20 h 23
 * 
 * 
 * MIT License
 * 
 * Copyright (c) 2020 Six Jonathan
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#define DEFAULT_GETTER(variableName, variable)                                                                    \
        [[nodiscard]] inline constexpr                                                                            \
        decltype(variable)& get ## variableName () noexcept { return variable;}

#define BY_VALUE_GETTER(variableName, variable)                                                                   \
        [[nodiscard]] inline constexpr                                                                            \
        decltype(variable) get ## variableName () const noexcept { return variable;}


#define DEFAULT_CONST_GETTER(variableName, variable)                                                              \
        [[nodiscard]] inline constexpr                                                                            \
        const decltype(variable)& get ## variableName () const noexcept { return variable;}


#define DEFAULT_SETTER(variableName, variable)                                                                    \
        inline constexpr                                                                                          \
        void set ## variableName (const decltype(variable)& in_ ## variableName ) noexcept { variable = in_ ## variableName ;}


#define DEFAULT_GETTER_SETTER(variableName, variable)                                                             \
        DEFAULT_CONST_GETTER(variableName, variable)                                                                    \
        DEFAULT_SETTER(variableName, variable)
