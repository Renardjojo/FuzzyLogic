
namespace FuzzyLogicPCL.FuzzySets
{
    public class LeftFuzzySet : FuzzySet
    {
        public LeftFuzzySet(double min, double max, double heightMax, double baseMin)
            : base(min, max)
        {
            Add(new Point2D(min, 1));
            Add(new Point2D(heightMax, 1));
            Add(new Point2D(baseMin, 0));
            Add(new Point2D(max, 0));
        }
    }
}
