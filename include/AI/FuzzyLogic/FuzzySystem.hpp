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
        
        //Represente the input linguistic variable with the variable associate to the current problem to solve
        std::vector<FuzzyValue<TPrecisionType>> m_inputs;

        //Represente the output linguistic variable use in with the problem is solved
        LinguisticVariable<TPrecisionType> m_output;

        //Represent all the combinaision the solve the probelem between inputs and the outputs linguistic varaible
        std::vector<FuzzyRule<TPrecisionType>> m_rules;


        #pragma endregion //!attribut
    
        #pragma region method
        [[nodiscard]] constexpr inline
        FuzzyValue<TPrecisionType>* const inputFuzzyValueByName(const std::string& in_name) noexcept
        {
            for (FuzzyValue<TPrecisionType>& input : m_inputs)
            {
                if (input.getName() == in_name)
                {
                    return &input;
                }
            }
            return nullptr;
        }

        #pragma endregion //!method

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
                m_rules     {}
        {}

        #pragma endregion //!constructor/destructor
    
        #pragma region methods

        constexpr inline
        void addInputVariable(const LinguisticVariable<TPrecisionType>& in_lv, TPrecisionType in_problemValue = static_cast<TPrecisionType>(0)) noexcept
        {
            for (auto&& input : m_inputs)
            {
                for (auto&& inputlinguisticValue : input.getLinguisticVariable().getValues())
                {
                    for (auto&& newInputlinguisticValue : in_lv.getValues())
                    {
                        std::vector<FuzzyExpression<TPrecisionType>> premis { FuzzyExpression<TPrecisionType>(input.getLinguisticVariable(), inputlinguisticValue.getName()),
                                                                              FuzzyExpression<TPrecisionType>(in_lv,                         newInputlinguisticValue.getName())};
                        FuzzyExpression<TPrecisionType> conclusion (m_output, m_output.getValues().front().getName());

                        m_rules.emplace_back(FuzzyRule<TPrecisionType>(premis, conclusion));
                    }
                }
            }

            m_inputs.emplace_back(in_lv, in_problemValue);
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
        void setFuzzyValue(const std::string& in_nameLinguistiqueVariable, TPrecisionType in_value) noexcept
        {
            FuzzyValue<TPrecisionType>* const pFuzzyValue = inputFuzzyValueByName(in_nameLinguistiqueVariable);

            if (pFuzzyValue)
            {
                pFuzzyValue->setValue(in_value);
            }
        }

        [[nodiscard]] constexpr inline 
        TPrecisionType solve() noexcept
        {
            //Fuzzication
            FuzzySet::FuzzySet<TPrecisionType> fuzzication(m_output.getMin(), m_output.getMax());
            fuzzication.add(m_output.getMin(), static_cast<TPrecisionType>(0));
            fuzzication.add(m_output.getMax(), static_cast<TPrecisionType>(0));

            for (const FuzzyRule<TPrecisionType>& rule : m_rules)
            {
                FuzzySet::FuzzySet<TPrecisionType> resultingSet;
                if (rule.tryApply(resultingSet, m_inputs))
                {
                    fuzzication = fuzzication | resultingSet;
                }
            }            

            //Defuzzification
            return fuzzication.centroid();
        }

        [[nodiscard]] constexpr inline
        const LinguisticVariable<TPrecisionType>* const inputLinguisticVariableByName(const std::string& in_name) const noexcept
        {
            for (const FuzzyValue<TPrecisionType>& input : m_inputs)
            {
                if (input.getName() == in_name)
                {
                    return &input.getLinguisticVariable();
                }
            }
            return nullptr;
        }

        [[nodiscard]] constexpr inline 
        const LinguisticVariable<TPrecisionType>* const linguisticVariableByName(const std::string& in_name) const noexcept
        {
            for (const FuzzyValue<TPrecisionType>& input : m_inputs)
            {
                if (input.getName() == in_name)
                {
                    return &input.getLinguisticVariable();
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

        DEFAULT_CONST_GETTER(Inputs, m_inputs)
        DEFAULT_GETTER(Inputs, m_inputs)
        DEFAULT_SETTER(Inputs, m_inputs)

        DEFAULT_CONST_GETTER(Output, m_output)
        DEFAULT_GETTER(Output, m_output)
        DEFAULT_SETTER(Output, m_output)

        DEFAULT_GETTER(Rules, m_rules)
        DEFAULT_SETTER(Rules, m_rules)

        #pragma endregion //!accessor/mutator
    };
} /*namespace AI::FuzzyLogic*/
