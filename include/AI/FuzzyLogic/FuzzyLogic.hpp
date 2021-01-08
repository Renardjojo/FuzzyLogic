/*
 * Project : FuzzyLogic
 * Editing by Six Jonathan
 * Date : 2021-01-07 - 10 h 01
 * 
 * Copyright (C) 2021 Six Jonathan
 * This file is subject to the license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "AI/FuzzyLogic/FuzzySets/Point2D.hpp"
#include "AI/FuzzyLogic/FuzzySets/FuzzySet.hpp"
#include "AI/FuzzyLogic/FuzzySets/LeftFuzzySet.hpp"
#include "AI/FuzzyLogic/FuzzySets/RightFuzzySet.hpp"
#include "AI/FuzzyLogic/FuzzySets/TrapezoidalFuzzySet.hpp"
#include "AI/FuzzyLogic/FuzzySets/TriangularFuzzySet.hpp"
#include "AI/FuzzyLogic/LinguisticValue.hpp"
#include "AI/FuzzyLogic/LinguisticVariable.hpp"
#include "AI/FuzzyLogic/FuzzySystem.hpp"
#include "AI/FuzzyLogic/FuzzyValue.hpp"
#include "AI/FuzzyLogic/FuzzyRule.hpp"
#include "AI/FuzzyLogic/FuzzyExpression.hpp"


namespace AI::FuzzyLogic
{
    class FuzzyLogic
    {
        private:
    
        protected:
    
        #pragma region attribut
        #pragma endregion //!attribut
    
        #pragma region static attribut
        #pragma endregion //! static attribut
    
        #pragma region methods
        #pragma endregion //!methods
    
        public:
    
        #pragma region constructor/destructor
    
        constexpr inline
        FuzzyLogic () noexcept 					                = default;
    
        constexpr inline
        FuzzyLogic (const FuzzyLogic& other) noexcept			= default;
    
        constexpr inline
        FuzzyLogic (FuzzyLogic&& other) noexcept				= default;
    
        inline
        ~FuzzyLogic () noexcept				                    = default;
    
        constexpr inline
        FuzzyLogic& operator=(FuzzyLogic const& other) noexcept		= default;
    
        constexpr inline
        FuzzyLogic& operator=(FuzzyLogic && other) noexcept			= default;
    
        #pragma endregion //!constructor/destructor
    
        #pragma region methods
        #pragma endregion //!methods
    
        #pragma region accessor
        #pragma endregion //!accessor
    
        #pragma region mutator
        #pragma endregion //!mutator
    
        #pragma region operator
        #pragma endregion //!operator
    
        #pragma region convertor
        #pragma endregion //!convertor
    
    };
} /*namespace AI::FuzzyLogic*/