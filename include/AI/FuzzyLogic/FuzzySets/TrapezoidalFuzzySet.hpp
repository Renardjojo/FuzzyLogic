/*
 * Project : FuzzyLogic
 * Editing by Six Jonathan
 * Date : 2021-01-07 - 13 h 41
 * 
 * Copyright (C) 2021 Six Jonathan
 * This file is subject to the license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "AI/FuzzyLogic/FuzzySets/FuzzySet.hpp"

namespace AI::FuzzyLogic::FuzzySet
{
    template <typename TPrecisionType = float>
    class TrapezoidalFuzzySet : public FuzzySet<TPrecisionType>
    {
        public:
    
        #pragma region constructor/destructor
    
        constexpr inline
        TrapezoidalFuzzySet () noexcept 					        = default;
    
        constexpr inline
        TrapezoidalFuzzySet (const TrapezoidalFuzzySet& other) noexcept	= default;
    
        constexpr inline
        TrapezoidalFuzzySet (TrapezoidalFuzzySet&& other) noexcept		= default;
    
        inline
        ~TrapezoidalFuzzySet () noexcept				            = default;
    
        constexpr inline
        TrapezoidalFuzzySet& operator=(TrapezoidalFuzzySet const& other) noexcept		= default;
    
        constexpr inline
        TrapezoidalFuzzySet& operator=(TrapezoidalFuzzySet && other) noexcept			= default;
    
        explicit constexpr inline
        TrapezoidalFuzzySet(TPrecisionType in_min, TPrecisionType in_max, TPrecisionType in_baseLeft, TPrecisionType in_heightLeft, TPrecisionType in_heightRight, TPrecisionType in_baseRight) noexcept
            : FuzzySet<TPrecisionType>(in_min, in_max)
        {
            FuzzySet<TPrecisionType>::add(Point2D<TPrecisionType>(in_min, static_cast<TPrecisionType>(0)));
            FuzzySet<TPrecisionType>::add(Point2D<TPrecisionType>(in_baseLeft, static_cast<TPrecisionType>(0)));
            FuzzySet<TPrecisionType>::add(Point2D<TPrecisionType>(in_heightLeft, static_cast<TPrecisionType>(1)));
            FuzzySet<TPrecisionType>::add(Point2D<TPrecisionType>(in_heightRight, static_cast<TPrecisionType>(1)));
            FuzzySet<TPrecisionType>::add(Point2D<TPrecisionType>(in_baseRight, static_cast<TPrecisionType>(0)));
            FuzzySet<TPrecisionType>::add(Point2D<TPrecisionType>(in_max, static_cast<TPrecisionType>(0)));
        }

        #pragma endregion //!constructor/destructor    
    };
} /*namespace AI::FuzzyLogic::FuzzySet*/