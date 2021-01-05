using System;
using System.Collections.Generic;
using System.Linq;

namespace FuzzyLogicPCL.FuzzySets
{
    public class FuzzySet
    {
        protected List<Point2D> Points;
        protected double Min {get;set;}
        protected double Max { get; set; }

        public FuzzySet(double min, double max)
        {
            this.Points = new List<Point2D>();
            this.Min = min;
            this.Max = max;
        }

        public void Add(Point2D pt)
        {
            Points.Add(pt);
            Points.Sort();
        }

        public void Add(double x, double y)
        {
            Point2D pt = new Point2D(x, y);
            Add(pt);
        }

        public override String ToString()
        {
            String result = "[" + Min + "-" + Max + "]:";
            foreach (Point2D pt in Points)
            {
                result += pt.ToString(); 
            }
            return result;
        }

        public static Boolean operator ==(FuzzySet fs1, FuzzySet fs2)
        {
            return fs1.ToString().Equals(fs2.ToString());
        }

        public static Boolean operator !=(FuzzySet fs1, FuzzySet fs2)
        {
            return !(fs1 == fs2);
        }

        public static FuzzySet operator *(FuzzySet fs, double value)
        {
            FuzzySet result = new FuzzySet(fs.Min, fs.Max);
            foreach (Point2D pt in fs.Points)
            {
                result.Add(new Point2D(pt.X, pt.Y * value));
            }
            return result;
        }

        public static FuzzySet operator !(FuzzySet fs)
        {
            FuzzySet result = new FuzzySet(fs.Min, fs.Max);
            foreach (Point2D pt in fs.Points)
            {
                result.Add(new Point2D(pt.X, 1 - pt.Y));
            }
            return result;
        }

        public double DegreeAtValue(double XValue)
        {
            if (ValueOutOfBound(XValue))
            {
                return 0;
            }
            else
            {
                return GetValueFromInterpolation(XValue);
            }
        }

        private bool ValueOutOfBound(double xValue)
        {
            return (xValue < Min || xValue > Max);
        }

        private double GetValueFromInterpolation(double XValue)
        {
            Point2D before = Points.LastOrDefault(pt => pt.X <= XValue);
            Point2D after = Points.FirstOrDefault(pt => pt.X >= XValue);
            if (before.Equals(after))
            {
                return before.Y;
            }
            else
            {
                return InterpolatedValueBetweenTwoPoints(XValue, before, after);
            }
        }

        private double InterpolatedValueBetweenTwoPoints(double xValue, Point2D before, Point2D after)
        {
            return (((before.Y - after.Y) * (after.X - xValue) / (after.X - before.X)) + after.Y);
        }

        public static FuzzySet operator &(FuzzySet fs1, FuzzySet fs2)
        {
            return Merge(fs1, fs2, Math.Min);
        }

        public static FuzzySet operator |(FuzzySet fs1, FuzzySet fs2)
        {
            return Merge(fs1, fs2, Math.Max);
        }

        private static FuzzySet result;
        private static List<Point2D>.Enumerator enum1;
        private static List<Point2D>.Enumerator enum2;
        private static Point2D oldPt1;
        private static int relativePosition;
        private static int newRelativePosition;
        private static double x1;
        private static double x2;
        private static Boolean endOfList1;
        private static Boolean endOfList2;

        private static FuzzySet Merge(FuzzySet fs1, FuzzySet fs2, Func<double, double, double> MergeFt)
        {
            result = new FuzzySet(Math.Min(fs1.Min, fs2.Min), Math.Max(fs1.Max, fs2.Max));

            CreateAndInitIterators(fs1, fs2);
            oldPt1 = enum1.Current;
            InitRelativePositions();

            endOfList1 = false;
            endOfList2 = false;
            while (!endOfList1 && !endOfList2)
            {
                ComputeNewValuesAndPositions();

                if (PositionsHaveChanged())
                {
                    AddIntersectionToResult(fs1, fs2);
                    GoToNextPoints();
                }
                else if (TwoPointsOnX())
                {
                    AddPointToResult(x1, MergeFt(enum1.Current.Y, enum2.Current.Y));
                    oldPt1 = enum1.Current;
                    GoToNextPointOnFS1();
                    GoToNextPointOnFS2();
                }
                else if (FS1BeforeFS2())
                {
                    AddPointToResult(x1, MergeFt(enum1.Current.Y, fs2.DegreeAtValue(x1)));
                    oldPt1 = enum1.Current;
                    GoToNextPointOnFS1();
                }
                else
                {
                    AddPointToResult(x2, MergeFt(fs1.DegreeAtValue(x2), enum2.Current.Y));
                    GoToNextPointOnFS2();
                }
            }

            AddEndPoints(MergeFt);

            return result;
        }
        
        private static bool PositionsHaveChanged()
        {
            return relativePosition != newRelativePosition && relativePosition != 0 && newRelativePosition != 0;
        }

        private static void CreateAndInitIterators(FuzzySet fs1, FuzzySet fs2)
        {
            enum1 = fs1.Points.GetEnumerator();
            enum2 = fs2.Points.GetEnumerator();
            enum1.MoveNext();
            enum2.MoveNext();
        }
        
        private static void InitRelativePositions()
        {
            relativePosition = 0;
            newRelativePosition = Math.Sign(enum1.Current.Y - enum2.Current.Y);
        }

        private static void ComputeNewValuesAndPositions()
        {
            x1 = enum1.Current.X;
            x2 = enum2.Current.X;
            relativePosition = newRelativePosition;
            newRelativePosition = Math.Sign(enum1.Current.Y - enum2.Current.Y);
        }
        
        private static void AddIntersectionToResult(FuzzySet fs1, FuzzySet fs2)
        {
            // Compute the points coordinates
            double x = (x1 == x2 ? oldPt1.X : Math.Min(x1, x2));
            double xPrime = Math.Max(x1, x2);
            // Intersection
            double slope1 = 0;
            double slope2 = 0;
            double delta = 0;
            if (xPrime-x != 0)
            {
                slope1 = (fs1.DegreeAtValue(xPrime) - fs1.DegreeAtValue(x)) / (xPrime - x);
                slope2 = (fs2.DegreeAtValue(xPrime) - fs2.DegreeAtValue(x)) / (xPrime - x);
            }
            if (slope1 != slope2)
            {
                delta = (fs2.DegreeAtValue(x) - fs1.DegreeAtValue(x)) / (slope1 - slope2);
            }
            // Add point
            result.Add(x + delta, fs1.DegreeAtValue(x + delta));
        }

        private static void GoToNextPoints()
        {
            if (x1 < x2)
            {
                oldPt1 = enum1.Current;
                GoToNextPointOnFS1();
            }
            else if (x1 > x2)
            {
                GoToNextPointOnFS2();
            }
        }

        private static void GoToNextPointOnFS2()
        {
            endOfList2 = !(enum2.MoveNext());
        }

        private static void GoToNextPointOnFS1()
        {
            endOfList1 = !(enum1.MoveNext());
        }

        private static bool TwoPointsOnX()
        {
            return x1 == x2;
        }

        private static bool FS1BeforeFS2()
        {
            return x1 < x2;
        }

        private static void AddPointToResult(double x, double y)
        {
            result.Add(x, y);
        }

        private static void AddEndPoints(Func<double, double, double> MergeFt)
        {
            if (!endOfList1)
            {
                while (!endOfList1)
                {
                    AddPointToResult(enum1.Current.X, MergeFt(0, enum1.Current.Y));
                    GoToNextPointOnFS1();
                }
            }
            else if (!endOfList2)
            {
                while (!endOfList2)
                {
                    AddPointToResult(enum2.Current.X, MergeFt(0, enum2.Current.Y));
                    GoToNextPointOnFS2();
                }
            }
        }

        private double ponderatedArea;
        private double totalArea;
        private Point2D oldPt;
        private Point2D newPt;

        public double Centroid()
        {
            if (NotEnoughPoints())
            {
                return 0;
            }
            else
            {
                InitAreas();
                double localArea;
                oldPt = null;
                foreach (Point2D localNewPt in Points)
                {
                    newPt = localNewPt;
                    if (!IsFirstPoint(oldPt))
                    {
                        if (IsRectangle())
                        {
                            localArea = ComputeRectangleArea();
                            IncrementAreas(localArea, 1.0 / 2.0);
                        }
                        else
                        {
                            // We have two geometric shapes : a rectangle and a triangle
                            // For the rectangle
                            localArea = ComputeRectangleArea();
                            IncrementAreas(localArea, 1.0 / 2.0);
                            // For the triangle
                            localArea = ComputeTriangleArea();
                            double factor;
                            if (newPt.Y > oldPt.Y)
                            {
                                factor = 2.0 / 3.0;
                            }
                            else
                            {
                                factor = 1.0 / 3.0;
                            }
                            IncrementAreas(localArea, factor);

                        }
                    }
                    oldPt = newPt;
                }
                double centroid = ponderatedArea / totalArea;
                return centroid;
            }
        }

        private bool NotEnoughPoints()
        {
            return Points.Count < 2;
        }

        private void InitAreas()
        {
            ponderatedArea = 0;
            totalArea = 0;
        }

        private bool IsFirstPoint(Point2D oldPt)
        {
            return oldPt == null;
        }

        private bool IsRectangle()
        {
            return oldPt.Y == newPt.Y;
        }

        private double ComputeRectangleArea()
        {
            return Math.Min(oldPt.Y, newPt.Y) * (newPt.X - oldPt.X);
        }

        private double ComputeTriangleArea()
        {
            return (newPt.X - oldPt.X) * (Math.Abs(newPt.Y - oldPt.Y)) / 2;
        }

        private void IncrementAreas(double localArea, double factor)
        {
            totalArea += localArea;
            ponderatedArea += ((newPt.X - oldPt.X) * factor + oldPt.X) * localArea;
        }
    }
}
