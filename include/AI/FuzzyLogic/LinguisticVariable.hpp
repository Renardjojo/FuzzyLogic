
/*
 * Project : FuzzyLogic
 * Editing by Six Jonathan
 * Date : 2021-01-07 - 10 h 57
 * 
 * Copyright (C) 2021 Six Jonathan
 * This file is subject to the license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <string>
#include <vector>
#include "Macro/ClassUtility.hpp"
#include "AI/FuzzyLogic/LinguisticValue.hpp"
#include "AI/FuzzyLogic/FuzzySets/FuzzySet.hpp"

namespace AI::FuzzyLogic
{
    template <typename TPrecisionType = float>
    class LinguisticVariable
    {
        private:
    
        protected:
    
        #pragma region attribut

        std::string m_name;
        std::vector<LinguisticValue<TPrecisionType>> m_values;
        TPrecisionType m_minValue;
        TPrecisionType m_maxValue;
        std::string m_unit;

        #pragma endregion //!attribut
    
        public:
    
        #pragma region constructor/destructor
    
        constexpr inline
        LinguisticVariable () noexcept 					                        = default;
    
        constexpr inline
        LinguisticVariable (const LinguisticVariable& other) noexcept			= default;
    
        constexpr inline
        LinguisticVariable (LinguisticVariable&& other) noexcept				= default;
    
        inline
        ~LinguisticVariable () noexcept				                            = default;
    
        constexpr inline
        LinguisticVariable& operator=(LinguisticVariable const& other) noexcept		= default;
    
        constexpr inline
        LinguisticVariable& operator=(LinguisticVariable && other) noexcept			= default;

        explicit constexpr inline
        LinguisticVariable(const std::string& in_name, TPrecisionType in_min, TPrecisionType in_max, const std::string& in_unit = "") noexcept
            :   m_values    {},
                m_name      {in_name},
                m_minValue  {in_min},
                m_maxValue  {in_max},
                m_unit      {in_unit}
        {}
    
        #pragma endregion //!constructor/destructor
    
        #pragma region methods

        constexpr inline
        void addValue(const LinguisticValue<TPrecisionType>& in_lv) noexcept
        {
            m_values.emplace_back(in_lv);
        }

        constexpr inline
        void addValue(const std::string& in_name, FuzzySet::FuzzySet<TPrecisionType> in_fussySet, const std::string& in_unit = "") noexcept
        {
            m_values.emplace_back(LinguisticValue<TPrecisionType>(in_name, in_fussySet, in_unit));
        }

        constexpr inline
        void clearValues() noexcept
        {
            m_values.clear();
        }

        constexpr inline
        const LinguisticValue<TPrecisionType>* const linguisticValueByName(const std::string& in_name) const noexcept
        {
            for (const LinguisticValue<TPrecisionType>& val : m_values)
            {
                if (val.getName() == in_name)
                {
                    return &val;
                }
            }
            return nullptr;
        }

        #pragma endregion //!methods
    
        #pragma region accessor/mutator

        DEFAULT_CONST_GETTER(Name, m_name)
        DEFAULT_GETTER(Name, m_name)
        DEFAULT_SETTER(Name, m_name)

        DEFAULT_GETTER_SETTER(Values, m_values)

        DEFAULT_SETTER(Min, m_minValue)
        DEFAULT_GETTER(Min, m_minValue)
        DEFAULT_CONST_GETTER(Min, m_minValue)

        DEFAULT_SETTER(Max, m_maxValue)
        DEFAULT_GETTER(Max, m_maxValue)
        DEFAULT_CONST_GETTER(Max, m_maxValue)

        DEFAULT_GETTER_SETTER(Unit, m_unit)

        #pragma endregion //!accessor/mutator    
    };
} /*namespace AI::FuzzyLogic*/
