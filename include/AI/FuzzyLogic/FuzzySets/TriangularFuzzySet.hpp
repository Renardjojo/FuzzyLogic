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
    class TriangularFuzzySet : public FuzzySet<TPrecisionType>
    {
        public:
    
        #pragma region constructor/destructor
    
        constexpr inline
        TriangularFuzzySet () noexcept 					        = default;
    
        constexpr inline
        TriangularFuzzySet (const TriangularFuzzySet& other) noexcept	= default;
    
        constexpr inline
        TriangularFuzzySet (TriangularFuzzySet&& other) noexcept		= default;
    
        inline
        ~TriangularFuzzySet () noexcept				            = default;
    
        constexpr inline
        TriangularFuzzySet& operator=(TriangularFuzzySet const& other) noexcept		= default;
    
        constexpr inline
        TriangularFuzzySet& operator=(TriangularFuzzySet && other) noexcept			= default;
    
        explicit constexpr inline
        TriangularFuzzySet(TPrecisionType in_min, TPrecisionType in_max, TPrecisionType in_triangleBegin, TPrecisionType in_triangleCenter, TPrecisionType in_triangleEnd) noexcept
            : FuzzySet<TPrecisionType>(in_min, in_max)
        {
            FuzzySet<TPrecisionType>::add(Point2D<TPrecisionType>(in_min, static_cast<TPrecisionType>(0)));
            FuzzySet<TPrecisionType>::add(Point2D<TPrecisionType>(in_triangleBegin, static_cast<TPrecisionType>(0)));
            FuzzySet<TPrecisionType>::add(Point2D<TPrecisionType>(in_triangleCenter, static_cast<TPrecisionType>(1)));
            FuzzySet<TPrecisionType>::add(Point2D<TPrecisionType>(in_triangleEnd, static_cast<TPrecisionType>(0)));
            FuzzySet<TPrecisionType>::add(Point2D<TPrecisionType>(in_max, static_cast<TPrecisionType>(0)));
        }

        #pragma endregion //!constructor/destructor    
    };
} /*namespace AI::FuzzyLogic::FuzzySet*/