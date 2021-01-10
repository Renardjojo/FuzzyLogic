/*
 * Project : FuzzyLogic
 * Editing by Six Jonathan
 * Date : 2021-01-07 - 15 h 56
 * 
 * Copyright (C) 2021 Six Jonathan
 * This file is subject to the license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <algorithm>    // std::sort, std::find_if
#include <vector>       // std::vector
#include <string>       // std::string
#include <functional>   // std::function
#include "Macro/ClassUtility.hpp" //DEFAULT_GETTER_SETTER
#include "AI/FuzzyLogic/FuzzySets/Point2D.hpp" //Point2D

namespace AI::FuzzyLogic::FuzzySet
{
    template <typename TPrecisionType = float>
    class FuzzySet
    {
        private:
    
        protected:
    
        #pragma region attribut

        std::vector<Point2D<TPrecisionType>>   m_points;
        TPrecisionType  m_min;
        TPrecisionType  m_max;

        #pragma endregion //!attribut
    
        #pragma region methods

        [[nodiscard]] constexpr inline 
        bool valueOutOfBound(TPrecisionType in_xValue) const noexcept
        {
            return in_xValue < m_min || in_xValue > m_max;
        }

        [[nodiscard]] constexpr inline 
        TPrecisionType getValueFromInterpolation(TPrecisionType in_xValue) const noexcept
        {
            auto before = std::find_if(m_points.rbegin(), m_points.rend(), [&](const Point2D<TPrecisionType>& pt) { return pt.getX() <= in_xValue; });
            auto after = std::find_if(m_points.begin(), m_points.end(), [&](const Point2D<TPrecisionType>& pt) { return pt.getX() >= in_xValue; });

            if (*before == *after)
            {
                return before->getY();
            }
            else
            {
                return interpolatedValueBetweenTwoPoints(in_xValue, *before, *after);
            }
        }

        [[nodiscard]] constexpr inline 
        TPrecisionType interpolatedValueBetweenTwoPoints(TPrecisionType in_xValue, const Point2D<TPrecisionType>& in_before, const Point2D<TPrecisionType>& in_after) const noexcept
        {
            return (((in_before.getY() - in_after.getY()) * (in_after.getX() - in_xValue) / (in_after.getX() - in_before.getX())) + in_after.getY());
        }

        [[nodiscard]] constexpr inline
        static int getSign(TPrecisionType value) noexcept
        {
            return value > static_cast<TPrecisionType>(0) ? 1 :
                value < static_cast<TPrecisionType>(0) ? -1 : 0;
        }
        
        [[nodiscard]] constexpr inline
        static bool positionsHaveChanged(int in_relativePosition, int in_newRelativePosition) noexcept
        {
            return in_relativePosition != in_newRelativePosition && in_relativePosition != 0 && in_newRelativePosition != 0;
        }
        
        constexpr inline
        static void addIntersectionToResult(const FuzzySet<TPrecisionType>& in_fs1, const FuzzySet<TPrecisionType>& in_fs2, TPrecisionType in_x1, TPrecisionType in_x2, FuzzySet<TPrecisionType>& in_result, const Point2D<TPrecisionType>& in_oldPt1) noexcept
        {
            // Compute the points coordinates
            TPrecisionType x = (in_x1 == in_x2 ? in_oldPt1.getX() : std::min(in_x1, in_x2));
            TPrecisionType xPrime = std::max(in_x1, in_x2);
            // Intersection
            TPrecisionType slope1 = 0;
            TPrecisionType slope2 = 0;
            TPrecisionType delta = 0;
            if (xPrime - x != static_cast<TPrecisionType>(0))
            {
                slope1 = (in_fs1.degreeAtValue(xPrime) - in_fs1.degreeAtValue(x)) / (xPrime - x);
                slope2 = (in_fs2.degreeAtValue(xPrime) - in_fs2.degreeAtValue(x)) / (xPrime - x);
            }
            if (slope1 != slope2)
            {
                delta = (in_fs2.degreeAtValue(x) - in_fs1.degreeAtValue(x)) / (slope1 - slope2);
            }
            // Add point
            in_result.add(x + delta, in_fs1.degreeAtValue(x + delta));
        }
        
        [[nodiscard]] constexpr inline
        static bool goToNextPointOnFuzzySet(typename std::vector<Point2D<TPrecisionType>>::const_iterator& in_it, typename std::vector<Point2D<TPrecisionType>>::const_iterator in_itEnd) noexcept
        {
            return ++in_it == in_itEnd;
        }

        [[nodiscard]] constexpr inline
        bool notEnoughPoints() const noexcept
        {
            return m_points.size() < 2;
        }

        [[nodiscard]] constexpr inline
        bool isRectangle(const Point2D<TPrecisionType>& in_oldPt, const Point2D<TPrecisionType>& in_newPt) const noexcept
        {
            return in_oldPt.getY() == in_newPt.getY();
        }

        [[nodiscard]] constexpr inline
        TPrecisionType computeRectangleArea(const Point2D<TPrecisionType>& in_oldPt, const Point2D<TPrecisionType>& in_newPt) const noexcept
        {
            return std::min(in_oldPt.getY(), in_newPt.getY()) * (in_newPt.getX() - in_oldPt.getX());
        }

        [[nodiscard]] constexpr inline
        TPrecisionType computeTriangleArea(const Point2D<TPrecisionType>& in_oldPt, const Point2D<TPrecisionType>& in_newPt) const noexcept
        {
            return (in_newPt.getX() - in_oldPt.getX()) * (std::abs(in_newPt.getY() - in_oldPt.getY())) / static_cast<TPrecisionType>(2);
        }

        constexpr inline
        void incrementAreas(const Point2D<TPrecisionType>& in_oldPt, const Point2D<TPrecisionType>& in_newPt, TPrecisionType localArea, TPrecisionType factor, TPrecisionType& in_totalArea, TPrecisionType& in_ponderatedArea) const noexcept
        {
            in_totalArea += localArea;
            in_ponderatedArea += ((in_newPt.getX() - in_oldPt.getX()) * factor + in_oldPt.getX()) * localArea;
        }

        #pragma endregion //!methods
    
        public:
    
        #pragma region constructor/destructor
    
        constexpr inline
        FuzzySet () noexcept 					    = default;
    
        constexpr inline
        FuzzySet (const FuzzySet& other) noexcept	= default;
    
        constexpr inline
        FuzzySet (FuzzySet&& other) noexcept	    = default;
    
        inline
        ~FuzzySet () noexcept				        = default;
    
        constexpr inline
        FuzzySet& operator=(FuzzySet const& other) noexcept		= default;
    
        constexpr inline
        FuzzySet& operator=(FuzzySet && other) noexcept			= default;
    
        explicit constexpr inline
        FuzzySet (TPrecisionType in_min, TPrecisionType in_max) noexcept
            :   m_points{},
                m_min   {in_min},
                m_max   {in_max}
        {}

        #pragma endregion //!constructor/destructor
    
        #pragma region methods

        constexpr inline
        void add(const Point2D<TPrecisionType>& pt) noexcept
        {
            m_points.emplace_back(pt);
            std::sort(m_points.begin(), m_points.end());
        }

        constexpr inline
        void add(TPrecisionType x, TPrecisionType y) noexcept
        {
            add(Point2D<TPrecisionType>(x, y));
        }

        [[nodiscard]] constexpr inline
        std::string toString() const noexcept
        {
            std::string result ("[" + m_min + "-" + m_max + "]:");
            for (const Point2D<TPrecisionType>& pt : m_points)
            {
                result += pt.toString(); 
            }
            return result;
        }

        [[nodiscard]] constexpr inline
        TPrecisionType degreeAtValue(TPrecisionType in_xValue) const noexcept
        {
            if (valueOutOfBound(in_xValue))
            {
                return static_cast<TPrecisionType>(0);
            }
            else
            {
                return getValueFromInterpolation(in_xValue);
            }
        }

        [[nodiscard]] constexpr inline
        static FuzzySet<TPrecisionType> merge(const FuzzySet<TPrecisionType>& in_fs1, const FuzzySet<TPrecisionType>& in_fs2, std::function<TPrecisionType(TPrecisionType, TPrecisionType)> in_mergeFt) noexcept
        {
            FuzzySet<TPrecisionType> result (std::min(in_fs1.getMin(), in_fs2.getMin()), std::max(in_fs1.getMax(), in_fs2.getMax()));

            typename std::vector<Point2D<TPrecisionType>>::const_iterator it1 = in_fs1.getPoints().begin();
            typename std::vector<Point2D<TPrecisionType>>::const_iterator it2 = in_fs2.getPoints().begin();

            Point2D<TPrecisionType> oldPt1 = *it1;

            //relativePosition is the difference between Y1 and Y2. -1 if Y1 > Y2. 1 if Y1 < Y2. 0 if Y1 == Y2
            int relativePosition    = 0;
            int newRelativePosition = getSign(it1->getY() - it2->getY());

            bool endOfList1 = false;
            bool endOfList2 = false;
            TPrecisionType x1;
            TPrecisionType x2;

            while (!endOfList1 && !endOfList2)
            {
                //compute new values and positions
                x1 = it1->getX();
                x2 = it2->getX();
                relativePosition = newRelativePosition;
                newRelativePosition = getSign(it1->getY() - it2->getY());

                if (positionsHaveChanged(relativePosition, newRelativePosition))
                {
                    addIntersectionToResult(in_fs1, in_fs2, x1, x2, result, oldPt1);

                    //go to the next points
                    if (x1 < x2)
                    {
                        oldPt1 = *it1;
                        endOfList1 = goToNextPointOnFuzzySet(it1, in_fs1.getPoints().end());
                    }
                    else if (x1 > x2)
                    {
                        endOfList2 = goToNextPointOnFuzzySet(it2, in_fs2.getPoints().end());
                    }
                }
                else if (x1 == x2)
                {
                    result.add(x1, in_mergeFt(it1->getY(), it2->getY()));
                    oldPt1 = *it1;
                    endOfList1 = goToNextPointOnFuzzySet(it1, in_fs1.getPoints().end());
                    endOfList2 = goToNextPointOnFuzzySet(it2, in_fs2.getPoints().end());
                }
                else if (x1 < x2)
                {
                    result.add(x1, in_mergeFt(it1->getY(), in_fs2.degreeAtValue(x1)));
                    oldPt1 = *it1;
                    endOfList1 = goToNextPointOnFuzzySet(it1, in_fs1.getPoints().end());
                }
                else
                {
                    result.add(x2, in_mergeFt(in_fs1.degreeAtValue(x2), it2->getY()));
                    endOfList2 = goToNextPointOnFuzzySet(it2, in_fs2.getPoints().end());
                }
            }

            //Add the end points
            if (!endOfList1)
            {
                while (!endOfList1)
                {
                    result.add(it1->getX(), in_mergeFt(0, it1->getY()));
                    endOfList1 = goToNextPointOnFuzzySet(it1, in_fs1.getPoints().end());
                }
            }
            else if (!endOfList2)
            {
                while (!endOfList2)
                {
                    result.add(it2->getX(), in_mergeFt(0, it2->getY()));
                    endOfList2 = goToNextPointOnFuzzySet(it2, in_fs2.getPoints().end());
                }
            }

            return result;
        }

        [[nodiscard]] constexpr inline
        TPrecisionType centroid() const noexcept
        {
            if (notEnoughPoints())
                return 0;

            //init aera
            TPrecisionType ponderatedArea = 0;
            TPrecisionType totalArea = 0;
            TPrecisionType localArea;
            Point2D<TPrecisionType> oldPt, newPt;
            bool isNotTheFirstPoint = false;

            for (const Point2D<TPrecisionType>& localNewPt : m_points)
            {
                newPt = localNewPt;
                if (isNotTheFirstPoint)
                {
                    if (isRectangle(oldPt, newPt))
                    {
                        localArea = computeRectangleArea(oldPt, newPt);
                        incrementAreas(oldPt, newPt, localArea, static_cast<TPrecisionType>(1) / static_cast<TPrecisionType>(2), totalArea, ponderatedArea);
                    }
                    else
                    {
                        // We have two geometric shapes : a rectangle and a triangle
                        // For the rectangle
                        localArea = computeRectangleArea(oldPt, newPt);
                        incrementAreas(oldPt, newPt, localArea, static_cast<TPrecisionType>(1) / static_cast<TPrecisionType>(2), totalArea, ponderatedArea);
                        // For the triangle
                        localArea = computeTriangleArea(oldPt, newPt);
                        TPrecisionType factor;

                        if (newPt.getY() > oldPt.getY())
                        {
                            factor = static_cast<TPrecisionType>(2) / static_cast<TPrecisionType>(3);
                        }
                        else
                        {
                            factor = static_cast<TPrecisionType>(1) / static_cast<TPrecisionType>(3);
                        }
                        incrementAreas(oldPt, newPt, localArea, factor, totalArea, ponderatedArea);

                    }
                }
                else
                {
                    isNotTheFirstPoint = true;
                }
                oldPt = newPt;
            }
            return ponderatedArea / totalArea;
        }

        #pragma endregion //!methods
    
        #pragma region accessor/mutator

        DEFAULT_GETTER_SETTER(Points, m_points)
        DEFAULT_GETTER_SETTER(Min, m_min)
        DEFAULT_GETTER_SETTER(Max, m_max)

        #pragma endregion //!accessor/mutator
        
        #pragma region convertor
        #pragma endregion //!convertor
    
    };

        #pragma region operator

        template <typename TPrecisionType = float>
        bool operator ==(const FuzzySet<TPrecisionType>& in_lhs, FuzzySet<TPrecisionType> in_rhs)
        {
            return in_lhs.toString() == in_rhs.toString();
        }

        template <typename TPrecisionType = float>
        bool operator !=(const FuzzySet<TPrecisionType>& in_lhs, const FuzzySet<TPrecisionType>& in_rhs)
        {
            return !(in_lhs == in_rhs);
        }

        template <typename TPrecisionType = float>
        FuzzySet<TPrecisionType> operator *(const FuzzySet<TPrecisionType>& in_fs, TPrecisionType in_value)
        {
            FuzzySet<TPrecisionType> result(in_fs.getMin(), in_fs.getMax());
            for (const Point2D<TPrecisionType>& pt : in_fs.getPoints())
            {
                result.add(pt.getX(), pt.getY() * in_value);
            }
            return result;
        }

        template <typename TPrecisionType = float>
        FuzzySet<TPrecisionType> operator !(const FuzzySet<TPrecisionType>& fs)
        {
            FuzzySet<TPrecisionType> result(fs.Min, fs.Max);
            for (const Point2D<TPrecisionType>& pt : fs.m_points)
            {
                result.add(pt.getX(), static_cast<TPrecisionType>(1) - pt.getY());
            }
            return result;
        }

        template <typename TPrecisionType = float>
        FuzzySet<TPrecisionType> operator &(const FuzzySet<TPrecisionType>& in_lhs, const FuzzySet<TPrecisionType>& in_rhs)
        {
            return FuzzySet<TPrecisionType>::merge(in_lhs, in_rhs, [](TPrecisionType a, TPrecisionType b) {return std::min(a, b); });
        }

        template <typename TPrecisionType = float>
        FuzzySet<TPrecisionType> operator |(const FuzzySet<TPrecisionType>& in_lhs, const FuzzySet<TPrecisionType>& in_rhs)
        {
            return FuzzySet<TPrecisionType>::merge(in_lhs, in_rhs, [](TPrecisionType a, TPrecisionType b) {return std::max(a, b); });
        }

        #pragma endregion //!operator

} /*namespace AI::FuzzyLogic::FuzzySet*/