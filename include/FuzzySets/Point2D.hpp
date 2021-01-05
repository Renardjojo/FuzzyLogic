using System;

namespace FuzzyLogicPCL
{
    public class Point2D : IComparable
    {
        public double X { get; set; }
        public double Y { get; set; }

        public Point2D(double x, double y)
        {
            this.X = x;
            this.Y = y;
        }

        public int CompareTo(object obj)
        {
            return (int)(this.X - ((Point2D) obj).X);
        }

        public override String ToString()
        {
            return "(" + this.X + ";" + this.Y + ")";
        }
    }
}
