/*
 * Project : FuzzyLogic
 * Editing by Six Jonathan
 * Date : 2021-01-07 - 11 h 23
 * 
 * Copyright (C) 2021 Six Jonathan
 * This file is subject to the license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Macro/ClassUtility.hpp"
#include "AI/FuzzyLogic/LinguisticVariable.hpp"

namespace AI::FuzzyLogic
{
    template <typename TPrecisionType = float>
    class FuzzyValue
    {
        private:
    
        protected:
    
        #pragma region attribut

        LinguisticVariable<TPrecisionType>  m_linguistiqueVariable;
        TPrecisionType                      m_value;

        #pragma endregion //!attribut
    
        public:
    
        #pragma region constructor/destructor
    
        constexpr inline
        FuzzyValue () noexcept 					        = default;
    
        constexpr inline
        FuzzyValue (const FuzzyValue& other) noexcept	= default;
    
        constexpr inline
        FuzzyValue (FuzzyValue&& other) noexcept		= default;
    
        inline
        ~FuzzyValue () noexcept				            = default;
    
        constexpr inline
        FuzzyValue& operator=(FuzzyValue const& other) noexcept		= default;
    
        constexpr inline
        FuzzyValue& operator=(FuzzyValue && other) noexcept			= default;

        explicit constexpr inline
        FuzzyValue(const LinguisticVariable<TPrecisionType>& in_lv, TPrecisionType in_value) noexcept
            :   m_linguistiqueVariable  {in_lv},
                m_value                 {in_value}
        {}
    
        #pragma endregion //!constructor/destructor
    
        #pragma region methods
        #pragma endregion //!methods
    
        #pragma region accessor/mutator

        DEFAULT_GETTER_SETTER(LinguistiqueVariable, m_linguistiqueVariable)
        DEFAULT_GETTER_SETTER(Value, m_value)

        #pragma endregion //!accessor/mutator
    };
} /*namespace AI::FuzzyLogic*/
