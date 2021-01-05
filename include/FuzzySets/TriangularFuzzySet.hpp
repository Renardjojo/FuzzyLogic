
namespace FuzzyLogicPCL.FuzzySets
{
    public class TriangularFuzzySet : FuzzySet
    {
        public TriangularFuzzySet(double min, double max, double triangleBegin, double triangleCenter, double triangleEnd) : base(min, max)
        {
            Add(new Point2D(min, 0));
            Add(new Point2D(triangleBegin, 0));
            Add(new Point2D(triangleCenter, 1));
            Add(new Point2D(triangleEnd, 0));
            Add(new Point2D(max, 0));
        }
    }
}
