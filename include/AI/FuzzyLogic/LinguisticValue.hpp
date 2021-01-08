/*
 * Project : FuzzyLogic
 * Editing by Six Jonathan
 * Date : 2021-01-07 - 10 h 52
 * 
 * Copyright (C) 2021 Six Jonathan
 * This file is subject to the license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <string>
#include "Macro/ClassUtility.hpp"
#include "AI/FuzzyLogic/FuzzySets/FuzzySet.hpp"

namespace AI::FuzzyLogic
{
    template <typename TPrecisionType = float>
    class LinguisticValue
    {
        private:

        protected:

        #pragma region attribut

        FuzzySet::FuzzySet<TPrecisionType>    m_fuzzySet;
        std::string m_name;

        #pragma endregion //!attribut

        public:

        #pragma region constructor/destructor

        constexpr inline
        LinguisticValue () noexcept 					                = default;

        constexpr inline
        LinguisticValue (const LinguisticValue& other) noexcept			= default;

        constexpr inline
        LinguisticValue (LinguisticValue&& other) noexcept				= default;

        inline
        ~LinguisticValue () noexcept				                    = default;

        constexpr inline
        LinguisticValue& operator=(LinguisticValue const& other) noexcept		= default;

        constexpr inline
        LinguisticValue& operator=(LinguisticValue && other) noexcept			= default;

        explicit constexpr inline
        LinguisticValue(const std::string& in_name, const FuzzySet::FuzzySet<TPrecisionType>& in_fuzzySet) noexcept
            :   m_name      {in_name},
                m_fuzzySet  {in_fuzzySet}
        {}

        #pragma endregion //!constructor/destructor

        #pragma region methods

        [[nodiscard]] inline constexpr
        TPrecisionType degreeAtValue(TPrecisionType in_val) const noexcept
        {
            return m_fuzzySet.degreeAtValue(in_val);
        }

        #pragma endregion //!methods

        #pragma region accessor/mutator

        DEFAULT_GETTER_SETTER(FuzzySet, m_fuzzySet)
        DEFAULT_GETTER_SETTER(Name, m_name)

        #pragma endregion //!accessor/mutator

    };
} /*namespace AI::FuzzyLogic*/