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
    class RightFuzzySet : public FuzzySet<TPrecisionType>
    {
        public:
    
        #pragma region constructor/destructor
    
        constexpr inline
        RightFuzzySet () noexcept 					        = default;
    
        constexpr inline
        RightFuzzySet (const RightFuzzySet& other) noexcept	= default;
    
        constexpr inline
        RightFuzzySet (RightFuzzySet&& other) noexcept		= default;
    
        inline
        ~RightFuzzySet () noexcept				            = default;
    
        constexpr inline
        RightFuzzySet& operator=(RightFuzzySet const& other) noexcept		= default;
    
        constexpr inline
        RightFuzzySet& operator=(RightFuzzySet && other) noexcept			= default;
    
        explicit constexpr inline
        RightFuzzySet(TPrecisionType in_min, TPrecisionType in_max, TPrecisionType in_heightMax, TPrecisionType in_baseMin) noexcept
            : FuzzySet<TPrecisionType>(in_min, in_max)
        {
            FuzzySet<TPrecisionType>::add(Point2D<TPrecisionType>(in_min, static_cast<TPrecisionType>(0)));
            FuzzySet<TPrecisionType>::add(Point2D<TPrecisionType>(in_heightMax, static_cast<TPrecisionType>(0)));
            FuzzySet<TPrecisionType>::add(Point2D<TPrecisionType>(in_baseMin, static_cast<TPrecisionType>(1)));
            FuzzySet<TPrecisionType>::add(Point2D<TPrecisionType>(in_max, static_cast<TPrecisionType>(1)));
        }

        #pragma endregion //!constructor/destructor    
    };
} /*namespace AI::FuzzyLogic::FuzzySet*/