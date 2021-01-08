/*
 * Project : FuzzyLogic
 * Editing by Six Jonathan
 * Date : 2021-01-07 - 10 h 05
 * 
 * Copyright (C) 2021 Six Jonathan
 * This file is subject to the license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <vector>
#include <math.h>
#include <string>

#include "AI/FuzzyLogic/FuzzyExpression.hpp"
#include "AI/FuzzyLogic/LinguisticValue.hpp"
#include "AI/FuzzyLogic/FuzzyValue.hpp"
#include "Macro/ClassUtility.hpp"

namespace AI::FuzzyLogic
{
    template <typename TPrecisionType>
    class FuzzySystem;

    template <typename TPrecisionType = float>
    class FuzzyRule
    {    
        private:
    
        protected:
    
        #pragma region attribut

        std::vector<FuzzyExpression<TPrecisionType>>   m_premises;
        FuzzyExpression<TPrecisionType>                m_conclusion;
        
        #pragma endregion //!attribut
    
        #pragma region methods

        inline constexpr
        void changeOverallRuleDegree(TPrecisionType in_localDegree, TPrecisionType& in_ruleDegree) const noexcept
        {
            in_ruleDegree = std::min(in_ruleDegree, in_localDegree);
        }

        [[nodiscard]] inline constexpr
        TPrecisionType searchAndComputePremiseDegree(std::vector<FuzzyValue<TPrecisionType>>& in_problem, const FuzzyExpression<TPrecisionType>& in_rulePremise, LinguisticValue<TPrecisionType>* in_val) const noexcept
        {
            in_val = nullptr;
            TPrecisionType premiseDegree = static_cast<TPrecisionType>(0);
            typename std::vector<FuzzyValue<TPrecisionType>>::iterator it = in_problem.begin();
            bool valueFound = false;
            FuzzyValue problemValue;

            while(++it != in_problem.end() && !valueFound)
            {
                problemValue = *it;
                if (&in_rulePremise.getLinguisticValue() == &problemValue.getLinguistiqueVariable())
                {
                    premiseDegree = computeDegree(in_rulePremise, in_val, problemValue);
                    valueFound = true;
                }
            }
            return premiseDegree;
        }

        [[nodiscard]] inline constexpr
        TPrecisionType computeDegree(const FuzzyExpression<TPrecisionType>& in_rulePremise, const LinguisticValue<TPrecisionType>* in_val, const FuzzyValue<TPrecisionType>& in_problemeValue) const noexcept
        {
            in_val = in_rulePremise.getLinguisticValue().linguisticValueByName(in_rulePremise.getName());
            if (in_val != nullptr)
            {
                return in_val->degreeAtValue(in_problemeValue.getValue()); // this is fuzzyfication here
            }
            else
            {
                return static_cast<TPrecisionType>(0);
            }
        }

        [[nodiscard]] inline constexpr
        bool linguisticValueNotFound(const LinguisticValue<TPrecisionType>* const in_val) const noexcept
        {
            return in_val == nullptr;
        }

        [[nodiscard]] inline constexpr
        FuzzySet::FuzzySet<TPrecisionType> computeResultingFuzzySet(TPrecisionType in_ruleDegree) const noexcept
        {
            return m_conclusion.getLinguisticValue().linguisticValueByName(m_conclusion.getName())->getFuzzySet() * in_ruleDegree;
        }

        #pragma endregion //!methods
    
        public:
    
        #pragma region constructor/destructor
    
        constexpr inline
        FuzzyRule () noexcept 					            = default;
    
        constexpr inline
        FuzzyRule (const FuzzyRule& other) noexcept			= default;
    
        constexpr inline
        FuzzyRule (FuzzyRule&& other) noexcept				= default;
    
        inline
        ~FuzzyRule () noexcept				                = default;
    
        constexpr inline
        FuzzyRule& operator=(FuzzyRule const& other) noexcept		= default;
    
        constexpr inline
        FuzzyRule& operator=(FuzzyRule && other) noexcept			= default;

        explicit constexpr inline
        FuzzyRule(const std::vector<FuzzyExpression<TPrecisionType>>& in_premis, const FuzzyExpression<TPrecisionType>& in_conclusion) noexcept
            :   m_premises      {in_premis},
                m_conclusion    {in_conclusion}
        {}

        explicit constexpr
        FuzzyRule(const std::string& in_ruleStr, const FuzzySystem<TPrecisionType>& in_fuzzySystem) noexcept
        :   m_premises      {},
            m_conclusion    {}
        {
            // Split premises and conclusion
            const std::string thenStrKey = " THEN ";
            std::string::size_type indexStr = in_ruleStr.find(thenStrKey);
            std::string rule[2] {in_ruleStr.substr(0, indexStr), in_ruleStr.substr(indexStr + thenStrKey.length())};

            if (indexStr != std::string::npos && rule[1].find(thenStrKey) == std::string::npos)
            {
                // Compute and add premises
                std::vector<std::string> prem;
                const std::string andStrKey = " AND ";

                rule[0] = rule[0].substr(3); // remove "IF"
                indexStr = rule[0].find(andStrKey);

                while (indexStr != std::string::npos)
                {
                    prem.emplace_back(rule[0].substr(0, indexStr));
                    prem.emplace_back(rule[0].substr(indexStr + andStrKey.length()));
                    indexStr = prem.back().find(andStrKey);
                }

                const std::string isStrKey = " IS ";

                for (const std::string& exp : prem)
                {
                    indexStr = exp.find(isStrKey);
                    std::string res[2] {exp.substr(0, indexStr), exp.substr(indexStr + isStrKey.length())};

                    if (indexStr != std::string::npos && res[1].find(isStrKey) == std::string::npos)
                    {
                        auto pLV  = in_fuzzySystem.linguisticVariableByName(res[0]);
                        if (pLV != nullptr)
                            m_premises.emplace_back(FuzzyExpression<TPrecisionType>(*pLV, res[1]));
                    }
                }

                // Add conclusion
                indexStr = rule[1].find(isStrKey);
                std::string conclu[2] {rule[1].substr(0, indexStr), rule[1].substr(indexStr + isStrKey.length())};

                if (indexStr != std::string::npos && conclu[1].find(isStrKey) == std::string::npos)
                {
                    auto pLV  = in_fuzzySystem.linguisticVariableByName(conclu[0]);
                    if (pLV != nullptr)
                        m_conclusion = FuzzyExpression<TPrecisionType>(*pLV, conclu[1]);
                }
            }
        }

        #pragma endregion //!constructor/destructor
    
        #pragma region methods

        [[nodiscard]] inline constexpr
        bool const tryApply(FuzzySet::FuzzySet<TPrecisionType>& in_rst, std::vector<FuzzyValue<TPrecisionType>>& in_problem) const noexcept
        {
            TPrecisionType m_ruleDegree = static_cast<TPrecisionType>(1);
            LinguisticValue<TPrecisionType>* val = nullptr;

            for (const FuzzyExpression<TPrecisionType>& localPremise : m_premises)
            {
                FuzzyExpression<TPrecisionType> rulePremise = localPremise;
                TPrecisionType localDegree = searchAndComputePremiseDegree(in_problem, rulePremise, val);

                if (linguisticValueNotFound(val))
                {
                    return false;
                }

                changeOverallRuleDegree(localDegree, m_ruleDegree);
            }
            in_rst = computeResultingFuzzySet(m_ruleDegree);
            return true;
        }

        #pragma endregion //!methods
    
        #pragma region accessor/mutator

        DEFAULT_GETTER_SETTER(Premises, m_premises)
        DEFAULT_GETTER_SETTER(Conclusion, m_conclusion)

        #pragma endregion //!accessor/mutator
    
        #pragma region operator
        #pragma endregion //!operator
    
        #pragma region convertor
        #pragma endregion //!convertor
    
    };
} /*namespace AI::FuzzyLogic*/