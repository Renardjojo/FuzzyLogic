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
    class LeftFuzzySet : public FuzzySet<TPrecisionType>
    {
        public:
    
        #pragma region constructor/destructor
    
        constexpr inline
        LeftFuzzySet () noexcept 					        = default;
    
        constexpr inline
        LeftFuzzySet (const LeftFuzzySet& other) noexcept	= default;
    
        constexpr inline
        LeftFuzzySet (LeftFuzzySet&& other) noexcept		= default;
    
        inline
        ~LeftFuzzySet () noexcept				            = default;
    
        constexpr inline
        LeftFuzzySet& operator=(LeftFuzzySet const& other) noexcept		= default;
    
        constexpr inline
        LeftFuzzySet& operator=(LeftFuzzySet && other) noexcept			= default;
    
        explicit constexpr inline
        LeftFuzzySet(TPrecisionType in_min, TPrecisionType in_max, TPrecisionType in_heightMax, TPrecisionType in_baseMin) noexcept
            : FuzzySet<TPrecisionType>(in_min, in_max)
        {
            FuzzySet<TPrecisionType>::add(Point2D<TPrecisionType>(in_min, static_cast<TPrecisionType>(1)));
            FuzzySet<TPrecisionType>::add(Point2D<TPrecisionType>(in_heightMax , static_cast<TPrecisionType>(1)));
            FuzzySet<TPrecisionType>::add(Point2D<TPrecisionType>(in_baseMin, static_cast<TPrecisionType>(0)));
            FuzzySet<TPrecisionType>::add(Point2D<TPrecisionType>(in_max, static_cast<TPrecisionType>(0)));
        }

        #pragma endregion //!constructor/destructor    
    };
} /*namespace AI::FuzzyLogic::FuzzySet*/