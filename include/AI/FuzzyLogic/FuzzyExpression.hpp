/*
 * Project : FuzzyLogic
 * Editing by Six Jonathan
 * Date : 2021-01-07 - 09 h 42
 * 
 * Copyright (C) 2021 Six Jonathan
 * This file is subject to the license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <string>
#include "Macro/ClassUtility.hpp"
#include "AI/FuzzyLogic/LinguisticVariable.hpp"

namespace AI::FuzzyLogic
{
    template <typename TPrecisionType = float>
    class FuzzyExpression
    {
        private:

        protected:

        #pragma region attribut

        LinguisticVariable<TPrecisionType>      m_linguisticVariable;
        std::string                             m_linguisticValueName;

        #pragma endregion //!attribut

        public:

        #pragma region constructor/destructor

        constexpr inline
        FuzzyExpression () noexcept 					                = default;

        constexpr inline
        FuzzyExpression (const FuzzyExpression& other) noexcept			= default;

        constexpr inline
        FuzzyExpression (FuzzyExpression&& other) noexcept				= default;

        inline
        ~FuzzyExpression () noexcept				                    = default;

        constexpr inline
        FuzzyExpression& operator=(FuzzyExpression const& other) noexcept		= default;

        constexpr inline
        FuzzyExpression& operator=(FuzzyExpression && other) noexcept			= default;

        explicit constexpr inline
        FuzzyExpression (const LinguisticVariable<TPrecisionType>& in_linguistiqueVariable, const std::string& in_linguisticValueName) noexcept
            :   m_linguisticVariable { in_linguistiqueVariable },
                m_linguisticValueName {in_linguisticValueName}
        {}

        #pragma endregion //!constructor/destructor

        #pragma region accessor/mutator

        DEFAULT_GETTER_SETTER(LinguisticVariable, m_linguisticVariable)
        DEFAULT_GETTER_SETTER(Name, m_linguisticValueName)

        #pragma endregion //!accessor
    };
} /*namespace AI::FuzzyLogic*/