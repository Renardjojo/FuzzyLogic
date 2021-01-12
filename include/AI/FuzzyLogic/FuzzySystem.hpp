/*
 * Project : FuzzyLogic
 * Editing by Six Jonathan
 * Date : 2021-01-07 - 11 h 33
 * 
 * Copyright (C) 2021 Six Jonathan
 * This file is subject to the license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <string>
#include <vector>
#include "Macro/ClassUtility.hpp"
#include "AI/FuzzyLogic/FuzzySets/FuzzySet.hpp"
#include "AI/FuzzyLogic/LinguisticVariable.hpp"
#include "AI/FuzzyLogic/FuzzyRule.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"

namespace AI::FuzzyLogic
{
    template <typename TPrecisionType = float>
    class FuzzySystem;

    template <typename TPrecisionType>
    class FuzzySystem
    {
        private:
    
        protected:
    
        #pragma region attribut

        std::string m_name;
        std::vector<LinguisticVariable<TPrecisionType>> m_inputs;
        LinguisticVariable<TPrecisionType> m_output;
        std::vector<FuzzyRule<TPrecisionType>> m_rules;
        std::vector<FuzzyValue<TPrecisionType>> m_problem;

        #pragma endregion //!attribut
    
        public:
    
        #pragma region constructor/destructor
    
        constexpr inline
        FuzzySystem () noexcept 					    = default;
    
        constexpr inline
        FuzzySystem (const FuzzySystem& other) noexcept	= default;
    
        constexpr inline
        FuzzySystem (FuzzySystem&& other) noexcept		= default;
    
        inline
        ~FuzzySystem () noexcept				        = default;
    
        constexpr inline
        FuzzySystem& operator=(FuzzySystem const& other) noexcept		= default;
    
        constexpr inline
        FuzzySystem& operator=(FuzzySystem && other) noexcept			= default;
    
        explicit constexpr inline
        FuzzySystem(const std::string& in_name) noexcept
            :   m_name      {in_name},
                m_inputs    {}, 
                m_rules     {},
                m_problem   {}
        {}

        #pragma endregion //!constructor/destructor
    
        #pragma region methods

        constexpr inline
        void addInputVariable(const LinguisticVariable<TPrecisionType>& in_lv) noexcept
        {
            m_inputs.emplace_back(in_lv);
        }

        constexpr inline
        void addFuzzyRule(const FuzzyRule<TPrecisionType>& in_fuzzyRules) noexcept
        {
            m_rules.emplace_back(in_fuzzyRules);
        }

        constexpr inline
        void addFuzzyRule(const std::string& in_ruleStr) noexcept
        {
            m_rules.emplace_back(FuzzyRule<TPrecisionType>(in_ruleStr, *this));
        }

        constexpr inline
        void setInputVariable(const LinguisticVariable<TPrecisionType>& m_inputVar, TPrecisionType in_value) noexcept
        {
            m_problem.emplace_back(FuzzyValue<TPrecisionType>(m_inputVar, in_value));
        }
        
        constexpr inline
        void resetCase() noexcept
        {
            m_problem.clear();
        }

        [[nodiscard]] constexpr inline 
        TPrecisionType solve() noexcept
        {
            FuzzySet::FuzzySet<TPrecisionType> res (m_output.getMin(), m_output.getMax());
            res.add(m_output.getMin(), static_cast<TPrecisionType>(0));
            res.add(m_output.getMax(), static_cast<TPrecisionType>(0));
            for (const FuzzyRule<TPrecisionType>& rule : m_rules)
            {
                FuzzySet::FuzzySet<TPrecisionType> resultingSet;
                if (rule.tryApply(resultingSet, m_problem))
                {
                    res = res | resultingSet;
                }
            }            

            return res.centroid();
        }

        [[nodiscard]] constexpr inline 
        const LinguisticVariable<TPrecisionType>* const linguisticVariableByName(const std::string& in_name) const noexcept
        {
            for (const LinguisticVariable<TPrecisionType>& input : m_inputs)
            {
                if (input.getName() == in_name)
                {
                    return &input;
                }
            }

            if (m_output.getName() == in_name)
            {
                return &m_output;
            }
            return nullptr;
        }

        #pragma endregion //!methods
    
        #pragma region accessor/mutator

        DEFAULT_GETTER_SETTER(Name, m_name)

        DEFAULT_GETTER(Inputs, m_inputs)
        DEFAULT_SETTER(Inputs, m_inputs)

        DEFAULT_GETTER(Output, m_output)
        DEFAULT_SETTER(Output, m_output)

        DEFAULT_GETTER(Rules, m_rules)
        DEFAULT_SETTER(Rules, m_rules)

        DEFAULT_GETTER_SETTER(Problem, m_problem)

        #pragma endregion //!accessor/mutator
    };
} /*namespace AI::FuzzyLogic*/
