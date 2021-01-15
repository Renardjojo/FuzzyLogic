/*
 * Project : FuzzyLogic
 * Editing by Six Jonathan
 * Date : 2021-01-07 - 13 h 33
 * 
 * Copyright (C) 2021 Six Jonathan
 * This file is subject to the license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Macro/ClassUtility.hpp"

namespace AI::FuzzyLogic::FuzzySet
{
    /**
     * @brief Point2D Is basic point with overload of operator< that compare only the X position. Usfull for the FuzzyLogic
     * 
     * @tparam TPrecisionType 
     */
    template <typename TPrecisionType = float>
    class Point2D
    {
        private:
    
        protected:
    
        #pragma region attribut

        TPrecisionType m_x;
        TPrecisionType m_y;

        #pragma endregion //!attribut
    
        public:
    
        #pragma region constructor/destructor
    
        constexpr inline
        Point2D () noexcept 					    = default;
    
        constexpr inline
        Point2D (const Point2D<TPrecisionType>& other) noexcept	    = default;
    
        constexpr inline
        Point2D (Point2D<TPrecisionType>&& other) noexcept			= default;
    
        inline
        ~Point2D () noexcept				        = default;
    
        constexpr inline
        Point2D& operator=(Point2D<TPrecisionType> const& other) noexcept		= default;
    
        constexpr inline
        Point2D& operator=(Point2D<TPrecisionType>&& other) noexcept			= default;
    
        #pragma endregion //!constructor/destructor

        #pragma region methods

        explicit constexpr inline
        Point2D(TPrecisionType in_x, TPrecisionType in_y) noexcept
            :   m_x {in_x},
                m_y {in_y}
        {}

        std::string toString() const noexcept
        {
            return std::string("(" + std::to_string(m_x) + ";" + std::to_string(m_y) + ")");
        }  

        #pragma endregion //!methods 

        #pragma region mutator/accessor

        DEFAULT_SETTER(X, m_x)
        DEFAULT_SETTER(Y, m_y)

        DEFAULT_GETTER(X, m_x)
        DEFAULT_GETTER(Y, m_y)

        DEFAULT_CONST_GETTER(X, m_x)
        DEFAULT_CONST_GETTER(Y, m_y)

        #pragma endregion //!mutator/accessor 

        #pragma region operator

        [[nodiscard]] constexpr inline
        bool operator<(const Point2D<TPrecisionType>& other) const noexcept
        {
            return m_x < other.getX();
        }

        [[nodiscard]] constexpr inline
        bool operator==(const Point2D<TPrecisionType>& other) const noexcept
        {
            return m_x == other.getX();
        }

        #pragma endregion //!operator

    };
} /*namespace AI::FuzzyLogic::FuzzySet*/