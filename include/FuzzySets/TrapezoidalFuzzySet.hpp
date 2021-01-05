
namespace FuzzyLogicPCL.FuzzySets
{
    public class TrapezoidalFuzzySet : FuzzySet
    {
        public TrapezoidalFuzzySet(double min, double max, double baseLeft, double heightLeft, double heightRight, double baseRight)
            : base(min, max)
        {
            Add(new Point2D(min, 0));
            Add(new Point2D(baseLeft, 0));
            Add(new Point2D(heightLeft, 1));
            Add(new Point2D(heightRight, 1));
            Add(new Point2D(baseRight, 0));
            Add(new Point2D(max, 0));
        }
    }
}
