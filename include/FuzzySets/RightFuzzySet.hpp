
namespace FuzzyLogicPCL.FuzzySets
{
    public class RightFuzzySet : FuzzySet
    {
        public RightFuzzySet(double min, double max, double heightMin, double baseMax)
            : base(min, max)
        {
            Add(new Point2D(min, 0));
            Add(new Point2D(heightMin, 0));
            Add(new Point2D(baseMax, 1));
            Add(new Point2D(max, 1));
        }
    }
}
